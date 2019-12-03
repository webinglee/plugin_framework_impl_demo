/**
 * @Copyright Dorabot Inc.
 * @date : 2019
 * @author : Weibin.li@dorabot.com
 * @brief :
 */

#include "arm.hh"
#include "arm_interface.hh"
#include "class_loader.hh"

int main(int argc, char *argv[])
{
  /*
  dr::DrArm arm;
  arm.init(nullptr, nullptr, "", dr::ArmAPIs::ARM_MH50);
  arm.enable_robot();
  */
  dr::ClassLoader<dr::IDrArm> loader;
  loader.load_library("libarm_gp180.so");
  auto arm = loader.create("dr::Gp180");
  arm->enable_robot();
  return 0;
}