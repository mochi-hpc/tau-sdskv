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
    std::cout << "tausdskv: Thread " << data.tid << ", Hello from " << __PRETTY_FUNCTION__
              << std::endl;
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
    std::cout << "tausdskv: Thread " << data.tid << ", Hello from " << __PRETTY_FUNCTION__
              << std::endl;
    return PluginStatus::Success;
}

PluginStatus Plugin::EndOfExecution(Tau_plugin_event_end_of_execution_data_t const &data) {
    std::cout << "tausdskv: Thread " << data.tid << ", Hello from " << __PRETTY_FUNCTION__
              << std::endl;
    return PluginStatus::Success;
}

PluginStatus Plugin::Send(Tau_plugin_event_send_data_t const &data) {
    std::cout << "Hello from " << __PRETTY_FUNCTION__ << std::endl;
    return PluginStatus::Success;
}

PluginStatus Plugin::Recv(Tau_plugin_event_recv_data_t const &data) {
    std::cout << "Hello from " << __PRETTY_FUNCTION__ << std::endl;
    return PluginStatus::Success;
}

PluginStatus Plugin::FunctionEntry(Tau_plugin_event_function_entry_data_t const &data) {
    std::cout << "tausdskv: Thread " << data.tid << ", Entering " << data.timer_name
              << ", Timestamp " << data.timestamp << std::endl;
    return PluginStatus::Success;
}

PluginStatus Plugin::FunctionExit(Tau_plugin_event_function_exit_data_t const &data) {
    std::cout << "tausdskv: Thread " << data.tid << ", Exiting " << data.timer_name
              << ", Timestamp " << data.timestamp << std::endl;
    return PluginStatus::Success;
}

PluginStatus Plugin::AtomicEventTrigger(Tau_plugin_event_atomic_event_trigger_data_t const &data) {
    std::cout << "Hello from " << __PRETTY_FUNCTION__ << std::endl;
    return PluginStatus::Success;
}