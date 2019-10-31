#ifndef __ARM_GP180_HH__
#define __ARM_GP180_HH__

#include "arm_interface.hh"

namespace dr
{

class Gp180 : public IDrArm
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

#endif // __ARM_GP180_HH__
