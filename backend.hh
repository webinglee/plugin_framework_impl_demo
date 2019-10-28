/**
 * @Copyright Dorabot Inc.
 * @date : 2019
 * @author : Weibin.li@dorabot.com
 * @brief :
 */

#ifndef __BACKEND_HH__
#define __BACKEND_HH__

#include <memory>
#include "arm_interface.hh"

namespace dr
{

class IBackend
{
public:
  virtual ~IBackend() {}
  virtual std::unique_ptr<IDrArm> create_arm() const = 0;
};

class IBackendFactory
{
public:
  virtual ~IBackendFactory() {}
  virtual std::shared_ptr<IBackend> get_backend() const = 0;
};

std::unique_ptr<IBackendFactory> create_plugin_backend_factory(int id, const char *base_name);

}

#endif // __BACKEND_HH__
