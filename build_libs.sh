#!/bin/bash
gcc arm_mh50.cc arm_mh50.hh plugin_api.hh arm_interface.hh  -fPIC -shared -o libarm_mh50.so
gcc arm_gp180.cc arm_gp180.hh plugin_api.hh arm_interface.hh  -fPIC -shared -o libarm_gp180.so
sudo cp libarm* /usr/local/lib/
