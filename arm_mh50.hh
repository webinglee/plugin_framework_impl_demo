/**
 * @Copyright Dorabot Inc.
 * @date : 2019
 * @author : Weibin.li@dorabot.com
 * @brief :
 */

#ifndef __ARM_MH50_HH__
#define __ARM_MH50_HH__

#include "arm_interface.hh"

namespace dr
{

class Mh50 : public IDrArm
{
public:
  bool init(const void *urdf_model, const void *config, const std::string &name) override;
  void enable_robot() override;
  void disable_robot() override;
  bool reset_alarm() override;
  bool set_alarm(int ecode, const std::string &emsg, int subcode) override;
  bool execute_trajectory(const void*) override;
  bool queue_trajectory(const void*) override;
  bool stop_trajectory() override;
  void* get_joint_state() override;
  void* get_traj_feedback() override;
};

}

#endif // __ARM_MH50_HH__
