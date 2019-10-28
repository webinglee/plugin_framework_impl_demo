/**
 * @Copyright Dorabot Inc.
 * @date : 2019
 * @author : Weibin.li@dorabot.com
 * @brief :
 */

#ifndef __ARM_INTERFACE_HH__
#define __ARM_INTERFACE_HH__

#include <string>

namespace dr
{

class IDrArm
{
public:
  IDrArm() = default;
  virtual ~IDrArm() = default;
  virtual bool init(const void *urdf_model, const void *config, const std::string &name) = 0;
  virtual void enable_robot() = 0;
  virtual void disable_robot() = 0;
  virtual bool reset_alarm() = 0;
  virtual bool set_alarm(int ecode, const std::string &emsg, int subcode) = 0;
  virtual bool execute_trajectory(const void*) = 0;
  virtual bool queue_trajectory(const void*) = 0;
  virtual bool stop_trajectory() = 0;
  virtual void* get_joint_state() = 0;
  virtual void* get_traj_feedback() = 0;
};

} // namespace dr

#endif //__ARM_INTERFACE_HH__
