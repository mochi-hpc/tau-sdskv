/**
 * @file lib.cpp
 * @author Andrew Gaspar (agaspar@lanl.gov)
 * @brief Implements the control library for tau_sdskeyval_plugin
 * @date 2019-05-29
 *
 * @copyright Copyright (c) 2019 Triad National Security, LLC
 */

// STL Includes
#include <iostream>
#include <optional>
#include <utility>

// System Includes
#include <dlfcn.h>

// Internal Includes
#include <tau-sdskeyval-plugin/control.h>

// Local Includes
#include <tau-sdskeyval/plugin.h>
#include <version.hpp>

#define LOAD_SYMBOL(so, symbol_name) (decltype(symbol_name) *)dlsym((so), #symbol_name)

class TauSdskeyvalPlugin {
  public:
    static TauSdskeyvalPlugin *FindPlugin();

    void SetDumpName(char const *dump_name) { set_dump_name_(dump_name); }

  private:
    explicit TauSdskeyvalPlugin(decltype(tausdskeyval_control_set_dump_name) set_dump_name)
        : set_dump_name_(set_dump_name) {}

    decltype(tausdskeyval_control_set_dump_name) *set_dump_name_;
};

TauSdskeyvalPlugin *TauSdskeyvalPlugin::FindPlugin() {
    static std::optional<TauSdskeyvalPlugin> plugin = std::nullopt;
    static void *so = nullptr;

    if (plugin) return &plugin.value();

    // Load library once and leak the handle
    if (!so) so = dlopen("libtau_sdskeyval_plugin.so", RTLD_NOLOAD | RTLD_LAZY);
    if (!so) {
        std::cerr << "tau_sdskeyval: ERROR: Could not find libtau_sdskeyval_plugin.so" << std::endl;
        return nullptr;
    }

    auto major = LOAD_SYMBOL(so, TAU_SDSKEYVAL_PLUGIN_VERSION_MAJOR);
    auto minor = LOAD_SYMBOL(so, TAU_SDSKEYVAL_PLUGIN_VERSION_MINOR);
    auto set_dump_name = LOAD_SYMBOL(so, tausdskeyval_control_set_dump_name);

    if (!(major && minor && set_dump_name)) {
        std::cerr
            << "tau_sdskeyval: ERROR: Could not find required symbols in libtau_sdskeyval_plugin.so"
            << std::endl;
        return nullptr;
    }

    // Make sure the control and plugin are compatible versions
    if (std::make_pair(tau_sdskeyval::control::VersionMajor,
                       tau_sdskeyval::control::VersionMinor) != std::make_pair(*major, *minor)) {
        std::cerr << "tau_sdskeyval: ERROR: The control library (v"
                  << tau_sdskeyval::control::VersionMajor << "."
                  << tau_sdskeyval::control::VersionMinor
                  << ") is not compatible with the plugin library (v" << *major << "." << *minor
                  << ")" << std::endl;
        return nullptr;
    }

    plugin = TauSdskeyvalPlugin(set_dump_name);

    return &plugin.value();
}

extern "C" int tausdskeyval_set_dump_name(char const *dump_name) {
    if (auto plugin = TauSdskeyvalPlugin::FindPlugin()) {
        plugin->SetDumpName(dump_name);
        return 0;
    } else {
        return 1;
    }
}