#include "arm.hh"

int main(int argc, char *argv[])
{
  dr::DrArm arm;
  arm.init(nullptr, nullptr, "", dr::ArmAPIs::ARM_MH50);
  arm.enable_robot();
  return 0;
}