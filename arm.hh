/**
 * @Copyright Dorabot Inc.
 * @date : 2019
 * @author : Weibin.li@dorabot.com
 * @brief :
 */

#ifndef __DR_ARM_HH__
#define __DR_ARM_HH__

#include <memory>
#include <string>

namespace dr
{

enum ArmAPIs
{
  ARM_GP180,
  ARM_MH50,
};

class IDrArm;

class DrArm
{
public:
  DrArm();
  ~DrArm();
  bool init(const void *urdf_model, const void *config, const std::string &name, int api);
  void enable_robot();
  void disable_robot();
  bool reset_alarm();
  bool set_alarm(int &ecode, const std::string &emsg, int subcode);
  bool execute_trajectory(const void*);
  bool queue_trajectory(const void*);
  bool stop_trajectory();
  void* get_joint_state();
  void* get_traj_feedback();
private:
  std::unique_ptr<IDrArm> iarm_;
};

} // namespace dr

#endif //__DR_ARM_HH__
