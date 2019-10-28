/**
 * @Copyright Dorabot Inc.
 * @date : 2019
 * @author : Weibin.li@dorabot.com
 * @brief :
 */

#ifndef _PLUGIN_API_HPP_
#define _PLUGIN_API_HPP_


// increase for backward-compatible changes, e.g. add new function
// Main API <= Plugin API -> plugin is compatible
#define API_VERSION 0 // preview
// increase for incompatible changes, e.g. remove function argument
// Main ABI == Plugin ABI -> plugin is compatible
#define ABI_VERSION 0 // preview

#ifdef __cplusplus
extern "C" {
#endif


typedef struct DrArm_t* DrPluginArm;

typedef struct Dr_Arm_Plugin_API_preview
{
    //Dr_API_Header api_header;

    /** dr arm ID (ArmAPIs)
    @note API-ENTRY 1, API-Version == 0
     */
    int arm_api;
    int (*Arm_init)(const char *name, void *opaque, DrPluginArm *handle);
    int (*Arm_enable)(DrPluginArm handle);
    int (*Arm_disable)(DrPluginArm handle);
    int (*Arm_reset_alarm)(DrPluginArm handle);
    int (*Arm_set_alarm)(DrPluginArm handle, int ecode, const char *emsg, int subcode);
    int (*Arm_execute_trajectory)(DrPluginArm handle, const void *traj);
    int (*Arm_queue_trajectory)(DrPluginArm handle, const void *traj);
    int (*Arm_stop_trajectory)(DrPluginArm handle);
    int (*Arm_get_joint_state)(DrPluginArm handle);
    int (*Arm_get_traj_feedback)(DrPluginArm handle);
    int (*Arm_release)(DrPluginArm handle);
} Dr_Arm_Plugin_API_preview;


const Dr_Arm_Plugin_API_preview* dr_arm_plugin_init_v0(int requested_abi_version, int requested_api_version, void* reserved);

typedef const Dr_Arm_Plugin_API_preview* (*FN_dr_arm_plugin_init_t)
        (int requested_abi_version, int requested_api_version, void* reserved);

#ifdef __cplusplus
}
#endif

#endif // _PLUGIN_API_HPP_
