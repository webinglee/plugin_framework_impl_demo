/**
 * @Copyright Dorabot Inc.
 * @date : 2019
 * @author : Weibin.li@dorabot.com
 * @brief :
 */

#ifndef __DYNAMIC_LIB_HH__
#define __DYNAMIC_LIB_HH__

#include <string>

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
  void load(const std::string &filename);
  void release();

  void *handle_;
  std::string filename_;
};

}

#endif // __DYNAMIC_LIB_HH__
