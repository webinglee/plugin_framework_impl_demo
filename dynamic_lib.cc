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

struct DynamicLibImpl
{
  DynamicLibImpl(const std::string &filename) : filename_(filename)
  {
    load();
  }
  ~DynamicLibImpl()
  {
    release();
  }

  void load()
  {
    handle_ = dlopen(filename_.c_str(), RTLD_LAZY);
    if(handle_ == nullptr)
    {
      fprintf(stderr, "load %s failed. emsg[%s]\n", filename_.c_str(), dlerror());
    }
  }

  void release()
  {
    if(handle_)
    {
      dlclose(handle_);
      handle_ = nullptr;
    }
  }

  bool is_loaded() const
  {
    return handle_ != nullptr;
  }

  void* get_symbol(const char *symbol_name) const
  {
    return dlsym(handle_, symbol_name);
  }

  const std::string& get_name() const
  {
    return filename_;
  }
private:
  void *handle_;
  std::string filename_;
};

DynamicLib::DynamicLib(const std::string &filename) : impl_(new DynamicLibImpl(filename)) {}

DynamicLib::~DynamicLib() = default;

bool DynamicLib::is_loaded() const
{
  return impl_->is_loaded();
}

void* DynamicLib::get_symbol(const char *symbol_name) const
{
  return impl_->get_symbol(symbol_name);
}

const std::string& DynamicLib::get_name() const
{
  return impl_->get_name();
}

} // end of namespace dr
