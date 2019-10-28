/**
 * @Copyright Dorabot Inc.
 * @date : 2019
 * @author : Weibin.li@dorabot.com
 * @brief :
 */

#include "arm.hh"

#include <assert.h>
#include <iostream>

#include "arm_interface.hh"
#include "backend.hh"

namespace dr
{

DrArm::DrArm()
{
}

DrArm::~DrArm()
{
}

bool DrArm::init(const void *urdf_model, const void *config, const std::string &name, int api)
{
  auto factory = create_plugin_backend_factory(api, nullptr);
  assert(factory);
  static auto backend = factory->get_backend();
  if (backend)
  {
    try
    {
      iarm_ = backend->create_arm();
      if (iarm_)
        return true;
    }
    catch(...)
    {
      std::cout << "create arm happen exception!" << std::endl;
    }
  }
  return false;
}

void DrArm::enable_robot()
{
   if (iarm_)
    iarm_->enable_robot();
}

void DrArm::disable_robot()
{
  if (iarm_)
    iarm_->disable_robot();
}

bool DrArm::reset_alarm()
{
  if (iarm_)
    return iarm_->reset_alarm();
  return false;
}

bool DrArm::set_alarm(int &ecode, const std::string &emsg, int subcode)
{
  if (iarm_)
    return iarm_->set_alarm(ecode, emsg, subcode);
  return false;
}

bool DrArm::execute_trajectory(const void *traj)
{
  if (iarm_)
    return iarm_->execute_trajectory(traj);
  return false;
}

bool DrArm::queue_trajectory(const void *traj)
{
  if (iarm_)
    return iarm_->queue_trajectory(traj);
  return false;
}

bool DrArm::stop_trajectory()
{
  if (iarm_)
    return iarm_->stop_trajectory();
  return false;
}

void* DrArm::get_joint_state()
{
  if (iarm_)
    return iarm_->get_joint_state();
  return nullptr;
}

void* DrArm::get_traj_feedback()
{
  if (iarm_)
    return iarm_->get_traj_feedback();
  return nullptr;
}

}
