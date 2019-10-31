#include "arm_gp180.hh"

#include <iostream>

namespace dr
{

bool Gp180::init(const void *urdf, const void *config, const std::string &name)
{
  std::cout << "Gp180::init()" << std::endl;
  return true;
}

void Gp180::enable_robot()
{
  std::cout << "Gp180::enable_robot()" << std::endl;
}

void Gp180::disable_robot()
{
   std::cout << "Gp180::disable_robot()" << std::endl;
}

bool Gp180::reset_alarm()
{
  std::cout << "Gp180::reset_alarm()" << std::endl;
  return true;
}

bool Gp180::set_alarm(int ecode, const std::string &emsg, int subcode)
{
  std::cout << "Gp180::set_alarm()" << std::endl;
  return true;
}

bool Gp180::execute_trajectory(const void*)
{
  std::cout << "Gp180::execute_trajectory()" << std::endl;
  return true;
}

bool Gp180::queue_trajectory(const void*)
{
  std::cout << "Gp180::queue_trajectory()" << std::endl;
  return true;
}

bool Gp180::stop_trajectory()
{
  std::cout << "Gp180::stop_trajectory()" << std::endl;
  return true;
}

void* Gp180::get_joint_state()
{
  std::cout << "Gp180::stop_trajectory()" << std::endl;
  return NULL;
}

void* Gp180::get_traj_feedback()
{
  std::cout << "Gp180::get_traj_feedback()" << std::endl;
  return NULL;
}

} // namespace dr

#include "plugin_api.hh"

namespace dr
{

static int gp180_init(const char *name, void *opaque, DrPluginArm *handle)
{
  Gp180 *gp180 = NULL;
  gp180 = new Gp180;
  gp180->init(nullptr, nullptr, "");
  *handle = (DrPluginArm) gp180;
  return 0;
}

static int gp180_enable(DrPluginArm handle)
{
  if (handle)
  {
    Gp180 *gp180 = (Gp180*)handle;
    gp180->enable_robot();
    return 0;
  }
  return -1;
}

static int gp180_disable(DrPluginArm handle)
{
  if (handle)
  {
    Gp180 *gp180 = (Gp180*)handle;
    gp180->disable_robot();
    return 0;
  }
  return -1;
}

static int gp180_reset_alarm(DrPluginArm handle)
{
  if (handle)
  {
    Gp180 *gp180 = (Gp180*)handle;
    gp180->reset_alarm();
    return 0;
  }
  return -1;
}

static int gp180_set_alarm(DrPluginArm handle, int ecode, const char *emsg, int subcode)
{
  if (handle)
  {
    Gp180 *gp180 = (Gp180*)handle;
    gp180->set_alarm(ecode, emsg, subcode);
    return 0;
  }
  return -1;
}

static int gp180_execute_trajectory(DrPluginArm handle, const void *traj)
{
  if (handle)
  {
    Gp180 *gp180 = (Gp180*)handle;
    gp180->execute_trajectory(traj);
    return 0;
  }
  return -1;
}

static int gp180_queue_trajectory(DrPluginArm handle, const void *traj)
{
  if (handle)
  {
    Gp180 *gp180 = (Gp180*)handle;
    gp180->queue_trajectory(traj);
    return 0;
  }
  return -1;
}

static int gp180_stop_trajectory(DrPluginArm handle)
{
  if (handle)
  {
    Gp180 *gp180 = (Gp180*)handle;
    gp180->stop_trajectory();
    return 0;
  }
  return -1;
}

static int gp180_get_joint_state(DrPluginArm handle)
{
  if (handle)
  {
    Gp180 *gp180 = (Gp180*)handle;
    gp180->get_joint_state();
    return 0;
  }
  return -1;
}

static int gp180_get_traj_feedback(DrPluginArm handle)
{
  if (handle)
  {
    Gp180 *gp180 = (Gp180*)handle;
    gp180->get_traj_feedback();
    return 0;
  }
  return -1;
}
static int gp180_release(DrPluginArm handle)
{
  if (handle)
  {
    Gp180 *gp180 = (Gp180*)handle;
    delete gp180;
    return 0;
  }
  return -1;
}
#define ARM_GP180 0
static const Dr_Arm_Plugin_API_preview plugin_api = 
{
  ARM_GP180,
  gp180_init,
  gp180_enable,
  gp180_disable,
  gp180_reset_alarm,
  gp180_set_alarm,
  gp180_execute_trajectory,
  gp180_queue_trajectory,
  gp180_stop_trajectory,
  gp180_get_joint_state,
  gp180_get_traj_feedback,
  gp180_release
};

} // namespace dr

const Dr_Arm_Plugin_API_preview* dr_arm_plugin_init_v0(int requested_abi_version,
    int requested_api_version, void* reserved)
{
  (void) requested_abi_version;
  (void) requested_api_version;
  (void) reserved;
  std::cout << "gp180 plugin init" << std::endl;
  return &dr::plugin_api;
}