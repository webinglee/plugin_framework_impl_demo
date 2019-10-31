#include "arm_mh50.hh"

#include <iostream>

namespace dr
{

bool Mh50::init(const void *urdf, const void *config, const std::string &name)
{
  std::cout << "Mh50::init()" << std::endl;
  return true;
}

void Mh50::enable_robot()
{
  std::cout << "Mh50::enable_robot()" << std::endl;
}

void Mh50::disable_robot()
{
   std::cout << "Mh50::disable_robot()" << std::endl;
}

bool Mh50::reset_alarm()
{
  std::cout << "Mh50::reset_alarm()" << std::endl;
  return true;
}

bool Mh50::set_alarm(int ecode, const std::string &emsg, int subcode)
{
  std::cout << "Mh50::set_alarm()" << std::endl;
  return true;
}

bool Mh50::execute_trajectory(const void*)
{
  std::cout << "Mh50::execute_trajectory()" << std::endl;
  return true;
}

bool Mh50::queue_trajectory(const void*)
{
  std::cout << "Mh50::queue_trajectory()" << std::endl;
  return true;
}

bool Mh50::stop_trajectory()
{
  std::cout << "Mh50::stop_trajectory()" << std::endl;
  return true;
}

void* Mh50::get_joint_state()
{
  std::cout << "Mh50::stop_trajectory()" << std::endl;
  return NULL;
}

void* Mh50::get_traj_feedback()
{
  std::cout << "Mh50::get_traj_feedback()" << std::endl;
  return NULL;
}

} // namespace dr

#include "plugin_api.hh"

namespace dr
{

static int mh50_init(const char *name, void *opaque, DrPluginArm *handle)
{
  Mh50 *mh50 = NULL;
  mh50 = new Mh50;
  mh50->init(nullptr, nullptr, "");
  *handle = (DrPluginArm) mh50;
  return 0;
}

static int mh50_enable(DrPluginArm handle)
{
  if (handle)
  {
    Mh50 *mh50 = (Mh50*)handle;
    mh50->enable_robot();
    return 0;
  }
  return -1;
}

static int mh50_disable(DrPluginArm handle)
{
  if (handle)
  {
    Mh50 *mh50 = (Mh50*)handle;
    mh50->disable_robot();
    return 0;
  }
  return -1;
}

static int mh50_reset_alarm(DrPluginArm handle)
{
  if (handle)
  {
    Mh50 *mh50 = (Mh50*)handle;
    mh50->reset_alarm();
    return 0;
  }
  return -1;
}

static int mh50_set_alarm(DrPluginArm handle, int ecode, const char *emsg, int subcode)
{
  if (handle)
  {
    Mh50 *mh50 = (Mh50*)handle;
    mh50->set_alarm(ecode, emsg, subcode);
    return 0;
  }
  return -1;
}

static int mh50_execute_trajectory(DrPluginArm handle, const void *traj)
{
  if (handle)
  {
    Mh50 *mh50 = (Mh50*)handle;
    mh50->execute_trajectory(traj);
    return 0;
  }
  return -1;
}

static int mh50_queue_trajectory(DrPluginArm handle, const void *traj)
{
  if (handle)
  {
    Mh50 *mh50 = (Mh50*)handle;
    mh50->queue_trajectory(traj);
    return 0;
  }
  return -1;
}

static int mh50_stop_trajectory(DrPluginArm handle)
{
  if (handle)
  {
    Mh50 *mh50 = (Mh50*)handle;
    mh50->stop_trajectory();
    return 0;
  }
  return -1;
}

static int mh50_get_joint_state(DrPluginArm handle)
{
  if (handle)
  {
    Mh50 *mh50 = (Mh50*)handle;
    mh50->get_joint_state();
    return 0;
  }
  return -1;
}

static int mh50_get_traj_feedback(DrPluginArm handle)
{
  if (handle)
  {
    Mh50 *mh50 = (Mh50*)handle;
    mh50->get_traj_feedback();
    return 0;
  }
  return -1;
}

static int mh50_release(DrPluginArm handle)
{
  if (handle)
  {
    Mh50 *mh50 = (Mh50*)handle;
    delete mh50;
    return 0;
  }
  return -1;
}

#define ARM_MH50 1
static const Dr_Arm_Plugin_API_preview plugin_api = 
{
  ARM_MH50,
  mh50_init,
  mh50_enable,
  mh50_disable,
  mh50_reset_alarm,
  mh50_set_alarm,
  mh50_execute_trajectory,
  mh50_queue_trajectory,
  mh50_stop_trajectory,
  mh50_get_joint_state,
  mh50_get_traj_feedback,
  mh50_release
};

} // namespace dr

const Dr_Arm_Plugin_API_preview* dr_arm_plugin_init_v0(int requested_abi_version,
    int requested_api_version, void* reserved)
{
  (void) requested_abi_version;
  (void) requested_api_version;
  (void) reserved;
  std::cout << "mh50 plugin init" << std::endl;
  return &dr::plugin_api;
}