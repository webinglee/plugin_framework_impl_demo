/**
 * @Copyright Dorabot Inc.
 * @date : 2019
 * @author : Weibin.li@dorabot.com
 * @brief : Dynamic load specific so filename
 */

#ifndef __DYNAMIC_LIB_HH__
#define __DYNAMIC_LIB_HH__

#include <string>
#include <memory>

namespace dr
{

class DynamicLib
{
public:
  DynamicLib(const std::string &filename);
  ~DynamicLib();
  bool is_loaded() const;
  void* get_symbol(const char *symbol_name) const;
  const std::string& get_name() const;

  DynamicLib(const DynamicLib&) = delete;
  DynamicLib& operator=(const DynamicLib &) = delete;

private:
  std::unique_ptr<struct DynamicLibImpl> impl_;
};

}

#endif // __DYNAMIC_LIB_HH__
