// STL Includes
#include <cstring>
#include <memory>

// Third Party Includes
#include <Profile/TauPlugin.h>

#include <TAU.h>

// Local Includes
#include <plugin.hpp>
#include <tau-sdskv/control.h>

using namespace tausdskv;

extern "C" int Tau_plugin_init_func(int argc, char **argv) {
    // TAU_SoS

    Plugin::InitializeInstance(argc, argv);

    Tau_plugin_callbacks_t cb;
    Tau_util_init_tau_plugin_callbacks(&cb);

    cb.Dump = Plugin::Bind<Tau_plugin_event_dump_data_t, &Plugin::Dump>();
    cb.Mpit = Plugin::Bind<Tau_plugin_event_mpit_data_t, &Plugin::Mpit>();
    cb.MetadataRegistrationComplete = Plugin::Bind<Tau_plugin_event_metadata_registration_data_t,
                                                   &Plugin::MetadataRegistrationComplete>();
    cb.PostInit = Plugin::Bind<Tau_plugin_event_post_init_data_t, &Plugin::PostInit>();
    cb.PreEndOfExecution =
        Plugin::Bind<Tau_plugin_event_pre_end_of_execution_data_t, &Plugin::PreEndOfExecution>();
    cb.EndOfExecution = Plugin::GlobalEndOfExecution;

    cb.Send = Plugin::Bind<Tau_plugin_event_send_data_t, &Plugin::Send>();
    cb.Recv = Plugin::Bind<Tau_plugin_event_recv_data_t, &Plugin::Recv>();
    cb.FunctionEntry =
        Plugin::Bind<Tau_plugin_event_function_entry_data_t, &Plugin::FunctionEntry>();
    cb.FunctionExit = Plugin::Bind<Tau_plugin_event_function_exit_data_t, &Plugin::FunctionExit>();
    cb.AtomicEventTrigger =
        Plugin::Bind<Tau_plugin_event_atomic_event_trigger_data_t, &Plugin::AtomicEventTrigger>();

    Tau_util_plugin_register_callbacks(&cb);

    return 0;
}

extern "C" int tausdskv_set_dump_name(char const *dump_name) { return 0; }