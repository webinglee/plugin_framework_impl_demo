/**
 * @Copyright Dorabot Inc.
 * @date : 2019
 * @author : Weibin.li@dorabot.com
 * @brief :
 */

#include "arm.hh"

int main(int argc, char *argv[])
{
  dr::DrArm arm;
  arm.init(nullptr, nullptr, "", dr::ArmAPIs::ARM_MH50);
  arm.enable_robot();
  return 0;
}