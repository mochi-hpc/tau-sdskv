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

using namespace tausdskv;

Plugin *Plugin::GetInstance() {
    static Plugin instance;

    return &instance;
}

PluginStatus Plugin::Dump(Tau_plugin_event_dump_data_t const &data) {
    auto &thread_state = GetThreadState(data.tid);

    for (auto const &f : thread_state.functions) {
        std::cout << data.tid << ":" << f.first << " = "
                  << "{ count = " << f.second.count << ", total_time = " << f.second.total_time
                  << ", unpaired_exits = " << f.second.unpaired_exits << "}" << std::endl;
    }

    return PluginStatus::Success;
}

PluginStatus Plugin::Mpit(Tau_plugin_event_mpit_data_t const &data) {
    std::cout << "Hello from " << __PRETTY_FUNCTION__ << std::endl;
    return PluginStatus::Success;
}

PluginStatus
Plugin::MetadataRegistrationComplete(Tau_plugin_event_metadata_registration_data_t const &data) {
    std::cout << "tausdskv: Registering " << data.name << std::endl;

    return PluginStatus::Success;
}

PluginStatus Plugin::PostInit(Tau_plugin_event_post_init_data_t const &data) {
    return PluginStatus::Success;
}

PluginStatus Plugin::PreEndOfExecution(Tau_plugin_event_pre_end_of_execution_data_t const &data) {
    auto &thread_state = GetThreadState(data.tid);
    std::cout << "tausdskv: Thread " << data.tid << ", Hello from " << __PRETTY_FUNCTION__
              << std::endl;
    return PluginStatus::Success;
}

PluginStatus Plugin::EndOfExecution(Tau_plugin_event_end_of_execution_data_t const &data) {
    auto &thread_state = GetThreadState(data.tid);
    std::cout << "tausdskv: Thread " << data.tid << ", Hello from " << __PRETTY_FUNCTION__
              << std::endl;
    return PluginStatus::Success;
}

PluginStatus Plugin::Send(Tau_plugin_event_send_data_t const &data) {
    auto &thread_state = GetThreadState(data.tid);
    std::cout << "Hello from " << __PRETTY_FUNCTION__ << std::endl;
    return PluginStatus::Success;
}

PluginStatus Plugin::Recv(Tau_plugin_event_recv_data_t const &data) {
    auto &thread_state = GetThreadState(data.tid);
    std::cout << "Hello from " << __PRETTY_FUNCTION__ << std::endl;
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
    std::cout << "Hello from " << __PRETTY_FUNCTION__ << std::endl;
    return PluginStatus::Success;
}

Plugin::ThreadState &Plugin::GetThreadState(int tid) {
    {
        // Get mutex as read first
        std::shared_lock<std::shared_mutex> read_thread_state(mutex_);

        if (tid < thread_state_.size()) {
            return *thread_state_[tid];
        }
    }

    {
        // If this is the first time we're seeing this tid, we need to add it
        std::unique_lock<std::shared_mutex> write_thread_state(mutex_);

        while (tid >= thread_state_.size()) {
            thread_state_.push_back(std::make_unique<ThreadState>());
        }

        return *thread_state_[tid];
    }
}