/**
 * @file plugin.hpp
 * @author Andrew Gaspar (agaspar@lanl.gov)
 * @brief TAU SDSKV Plugin
 * @date 2019-05-01
 *
 * @copyright Copyright (c) 2019 Triad National Security, LLC
 */

#ifndef TAU_SDSKEYVAL_PLUGIN_HPP_
#define TAU_SDSKEYVAL_PLUGIN_HPP_

// STL Includes
#include <map>
#include <memory>
#include <optional>
#include <shared_mutex>
#include <string_view>
#include <vector>

// Third Party Includes
#include <Profile/TauPlugin.h>

// Internal Includes
#include <margo.hpp>
#include <sdskv_client.hpp>

enum class PluginStatus { Success = 0, Fail = 1 };

namespace tau_sdskeyval {
class Plugin {
  public:
    static void InitializeInstance(int argc, char **argv);
    static Plugin *GetInstance();
    static int GlobalEndOfExecution(Tau_plugin_event_end_of_execution_data_t *data);

    explicit Plugin(std::string_view addr_str,
                    std::string_view server_address_str,
                    std::string_view db_name);

    template <typename EventData>
    using Callback = int (*)(EventData *);

    template <typename EventData, PluginStatus (Plugin::*MemberFunction)(EventData const &)>
    static Callback<EventData> Bind() {
        return [](EventData *data) -> int {
            auto instance = Plugin::GetInstance();

            if (instance) {
                return static_cast<int>((instance->*MemberFunction)(*data));
            } else {
                return 1;
            }
        };
    }

    void SetPrefix(std::string_view prefix);

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
    std::string prefix_;

    struct FunctionState {
        unsigned long total_time = 0;
        std::optional<unsigned long> last_start = std::nullopt;
        uint64_t count = 0;
        uint32_t depth = 0;
        uint32_t unpaired_exits;
    };

    struct ThreadState {
        std::map<std::string, FunctionState> functions;
    };

    ThreadState &GetThreadState(int tid);

    /**
     * @brief Guards thread_state_
     */
    std::unique_ptr<std::shared_mutex> mutex_ = std::make_unique<std::shared_mutex>();

    /**
     * @brief Holds per-thread state.
     *
     * @details
     * `ThreadState` is wrapped in a unique pointer so that when thread_state_ grows, outstanding
     * references are not invalidated.
     */
    std::vector<std::unique_ptr<ThreadState>> thread_state_;

    // mid_ must come before client_

    MargoInstance mid_;
    SdskvClient client_;
    MargoAddress server_address_;
    SdskvProviderHandle kvph_;
    sdskv_database_id_t db_id_;
};

} // namespace tau_sdskeyval

#endif // TAU_SDSKEYVAL_PLUGIN_HPP_