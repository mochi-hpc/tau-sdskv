/**
 * @file plugin.cpp
 * @author Andrew Gaspar (agaspar@lanl.gov)
 * @brief Implementation of tau-sdskv plugin
 * @date 2019-05-01
 *
 * @copyright Copyright (c) 2019 Triad National Security, LLC
 *
 */

// Self Include
#include <plugin.hpp>

// STL Includes
#include <iostream>
#include <optional>
#include <sstream>

using namespace tau_sdskeyval;

namespace {
static Plugin *instance = nullptr;
}

void Plugin::InitializeInstance(int argc, char **argv) {
    if (instance) return;

    try {
        instance = new Plugin("tcp", "tcp://127.0.0.1:1234", "tau");
    } catch (SdskvException const &ex) {
        if (ex.Status() == SDSKV_ERR_DB_NAME) {
            std::cerr << "Database name does not exist!" << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }
}

Plugin *Plugin::GetInstance() { return instance; }

int Plugin::GlobalEndOfExecution(Tau_plugin_event_end_of_execution_data_t *data) {
    if (data->tid != 0) {
        return 1;
    }

    auto result = GetInstance()->EndOfExecution(*data);

    delete instance;
    instance = nullptr;

    return static_cast<int>(result);
}

Plugin::Plugin(std::string_view addr_str,
               std::string_view server_address_str,
               std::string_view db_name)
    : mid_(addr_str, MargoMode::Client, true, -1),
      client_(mid_),
      server_address_(mid_.Lookup(server_address_str)),
      kvph_(client_.ProviderHandleCreate(server_address_, 1)),
      db_id_(kvph_.Open(db_name)) {
    static_assert(offsetof(Plugin, client_) > offsetof(Plugin, mid_),
                  "This code assumes mid_ is initialized before client_");

    static_assert(offsetof(Plugin, server_address_) > offsetof(Plugin, mid_),
                  "This code assumes mid_ is initialized before server_address_");

    static_assert(offsetof(Plugin, kvph_) > offsetof(Plugin, client_),
                  "This code assumes client_ is initialized before kvph_");
}

void Plugin::SetPrefix(std::string_view prefix) { prefix_ = prefix; }

PluginStatus Plugin::Dump(Tau_plugin_event_dump_data_t const &data) {
    auto &thread_state = GetThreadState(data.tid);

    std::vector<std::string> keys;
    std::vector<size_t> values;

    for (auto const &f : thread_state.functions) {
        auto const prefix = [&]() {
            std::stringstream ss;
            ss << prefix_ << ":" << data.tid << ":function[" << f.first << "]";
            return ss.str();
        }();

        keys.push_back(prefix + ":count");
        values.push_back(f.second.count);

        keys.push_back(prefix + ":total_time");
        values.push_back(f.second.total_time);

        keys.push_back(prefix + ":unpaired_exits");
        values.push_back(f.second.unpaired_exits);
    }

    std::vector<void const *> keys_c;
    keys_c.reserve(keys.size());

    std::vector<hg_size_t> key_sizes_c;
    key_sizes_c.reserve(keys.size());

    for (auto &k : keys) {
        keys_c.push_back(k.c_str());
        key_sizes_c.push_back(k.size() + 1);
    }

    std::vector<void const *> values_c;
    values_c.reserve(keys.size());

    std::vector<hg_size_t> value_sizes_c;
    value_sizes_c.reserve(values.size());

    for (auto &v : values) {
        values_c.push_back(&v);
        value_sizes_c.push_back(sizeof(size_t));
    }

    kvph_.PutMulti(db_id_,
                   keys.size(),
                   keys_c.data(),
                   key_sizes_c.data(),
                   values_c.data(),
                   value_sizes_c.data());

    return PluginStatus::Success;
}

PluginStatus Plugin::Mpit(Tau_plugin_event_mpit_data_t const &data) {
    return PluginStatus::Success;
}

PluginStatus
Plugin::MetadataRegistrationComplete(Tau_plugin_event_metadata_registration_data_t const &data) {
    // std::cout << "tau_sdskeyval: Registering " << data.name << std::endl;

    return PluginStatus::Success;
}

PluginStatus Plugin::PostInit(Tau_plugin_event_post_init_data_t const &data) {
    return PluginStatus::Success;
}

PluginStatus Plugin::PreEndOfExecution(Tau_plugin_event_pre_end_of_execution_data_t const &data) {
    auto &thread_state = GetThreadState(data.tid);
    return PluginStatus::Success;
}

PluginStatus Plugin::EndOfExecution(Tau_plugin_event_end_of_execution_data_t const &data) {
    auto &thread_state = GetThreadState(data.tid);
    return PluginStatus::Success;
}

PluginStatus Plugin::Send(Tau_plugin_event_send_data_t const &data) {
    auto &thread_state = GetThreadState(data.tid);
    return PluginStatus::Success;
}

PluginStatus Plugin::Recv(Tau_plugin_event_recv_data_t const &data) {
    auto &thread_state = GetThreadState(data.tid);
    return PluginStatus::Success;
}

PluginStatus Plugin::FunctionEntry(Tau_plugin_event_function_entry_data_t const &data) {
    auto &thread_state = GetThreadState(data.tid);
    auto &function = thread_state.functions[data.timer_name];

    if (function.depth == 0) {
        function.last_start = data.timestamp;
    }

    function.depth++;
    return PluginStatus::Success;
}

PluginStatus Plugin::FunctionExit(Tau_plugin_event_function_exit_data_t const &data) {
    auto &thread_state = GetThreadState(data.tid);
    auto &function = thread_state.functions[data.timer_name];

    if (function.depth == 0) {
        function.unpaired_exits++;
        function.last_start = std::nullopt;
        return PluginStatus::Success;
    }

    function.depth--;

    if (function.depth == 0) {
        function.total_time += data.timestamp - function.last_start.value();
        function.last_start = std::nullopt;
        function.count++;
    }

    return PluginStatus::Success;
}

PluginStatus Plugin::AtomicEventTrigger(Tau_plugin_event_atomic_event_trigger_data_t const &data) {
    auto &thread_state = GetThreadState(data.tid);
    return PluginStatus::Success;
}

Plugin::ThreadState &Plugin::GetThreadState(int tid) {
    {
        // Get mutex as read first
        std::shared_lock<std::shared_mutex> read_thread_state(*mutex_);

        if (tid < thread_state_.size()) {
            return *thread_state_[tid];
        }
    }

    {
        // If this is the first time we're seeing this tid, we need to add it
        std::unique_lock<std::shared_mutex> write_thread_state(*mutex_);

        while (tid >= thread_state_.size()) {
            thread_state_.push_back(std::make_unique<ThreadState>());
        }

        return *thread_state_[tid];
    }
}