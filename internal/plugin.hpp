/**
 * @file plugin.hpp
 * @author Andrew Gaspar (agaspar@lanl.gov)
 * @brief
 * @date 2019-05-01
 *
 * @copyright Copyright (c) 2019 Triad National Security, LLC
 *
 */

#ifndef TAUSDSKVPLUGIN_PLUGIN_HPP_
#define TAUSDSKVPLUGIN_PLUGIN_HPP_

// Third Party Includes
#include <Profile/TauPlugin.h>

enum class PluginStatus { Success = 0, Fail = 1 };

namespace tausdskv {
class Plugin {
  public:
    static Plugin *GetInstance();

    template <typename EventData>
    using Callback = int (*)(EventData *);

    template <typename EventData, PluginStatus (Plugin::*MemberFunction)(EventData const &)>
    static Callback<EventData> Bind() {
        return [](EventData *data) -> int {
            return static_cast<int>((Plugin::GetInstance()->*MemberFunction)(*data));
        };
    }

    PluginStatus Dump(Tau_plugin_event_dump_data_t const &data);
    PluginStatus Mpit(Tau_plugin_event_mpit_data_t const &data);
    PluginStatus
    MetadataRegistrationComplete(Tau_plugin_event_metadata_registration_data_t const &data);
    PluginStatus PostInit(Tau_plugin_event_post_init_data_t const &data);
    PluginStatus PreEndOfExecution(Tau_plugin_event_pre_end_of_execution_data_t const &data);
    PluginStatus EndOfExecution(Tau_plugin_event_end_of_execution_data_t const &data);

    PluginStatus Send(Tau_plugin_event_send_data_t const &data);
    PluginStatus Recv(Tau_plugin_event_recv_data_t const &data);
    PluginStatus FunctionEntry(Tau_plugin_event_function_entry_data_t const &data);
    PluginStatus FunctionExit(Tau_plugin_event_function_exit_data_t const &data);
    PluginStatus AtomicEventTrigger(Tau_plugin_event_atomic_event_trigger_data_t const &data);

  private:
    Plugin() = default;
};

} // namespace tausdskv

#endif // TAUSDSKVPLUGIN_PLUGIN_HPP_