/**
 * @file control.h
 * @author Andrew Gaspar (agaspar@lanl.gov)
 * @brief Declares internal plugin routines
 * @date 2019-05-29
 *
 * @copyright Copyright (c) 2019 Triad National Security, LLC
 */

#ifndef TAU_SDSKEYVAL_CONTROL_H_
#define TAU_SDSKEYVAL_CONTROL_H_

extern int TAU_SDSKEYVAL_PLUGIN_VERSION_MAJOR;
extern int TAU_SDSKEYVAL_PLUGIN_VERSION_MINOR;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

int tausdskeyval_control_set_dump_name(char const *dump_name);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // TAU_SDSKEYVAL_CONTROL_H_