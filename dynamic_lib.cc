/**
 * @Copyright Dorabot Inc.
 * @date : 2019
 * @author : Weibin.li@dorabot.com
 * @brief :
 */

#include "dynamic_lib.hh"

#include <dlfcn.h>
#include <string.h>
#include <errno.h>

namespace dr
{

DynamicLib::DynamicLib(const std::string &filename) : handle_(nullptr),
  filename_(filename)
{
  load(filename);
}

DynamicLib::~DynamicLib()
{
  release();
}

bool DynamicLib::is_loaded() const
{
  return handle_ != nullptr;
}

void* DynamicLib::get_symbol(const char *symbol_name) const
{
  return dlsym(handle_, symbol_name);
}

const std::string& DynamicLib::get_name() const
{
  return filename_;
}

void DynamicLib::load(const std::string &filename)
{
  handle_ = dlopen(filename.c_str(), RTLD_LAZY);
  if(handle_ == nullptr)
  {
    fprintf(stderr, "load %s failed. emsg[%s]\n", filename.c_str(), strerror(errno));
  }
}

void DynamicLib::release()
{
  if(handle_)
  {
    dlclose(handle_);
    handle_ = nullptr;
  }
}

}
