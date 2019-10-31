
#include "backend.hh"
#include <assert.h>
#include <iostream>
#include <list>

#include "dynamic_lib.hh"
#include "plugin_api.hh"

namespace dr
{

class PluginBackend : public IBackend
{
public:
  std::unique_ptr<DynamicLib> lib_;
  const Dr_Arm_Plugin_API_preview *plugin_api_;

  PluginBackend(std::unique_ptr<DynamicLib> &lib) : lib_(std::move(lib)), plugin_api_(NULL)
  {
    const char* init_name = "dr_arm_plugin_init_v0";
    FN_dr_arm_plugin_init_t fn_init = reinterpret_cast<FN_dr_arm_plugin_init_t>(lib_->get_symbol(init_name));
    if (fn_init)
    {
      plugin_api_ = fn_init(ABI_VERSION, API_VERSION, NULL);
      if (!plugin_api_)
      {
          std::cout << "Dr_Arm: plugin is incompatible: " << lib->get_name() << std::endl;
          return;
      }
      /*
      if (plugin_api_->api_header.opencv_version_major != CV_VERSION_MAJOR ||
          plugin_api_->api_header.opencv_version_minor != CV_VERSION_MINOR)
      {
          std::cout << "Dr_Arm: wrong Dorabot version used by plugin '"
            << plugin_api_->api_header.api_description << "': " <<
              cv::format("%d.%d, OpenCV version is '" CV_VERSION "'",
              plugin_api_->api_header.opencv_version_major, plugin_api_->api_header.opencv_version_minor))
          plugin_api_ = NULL;
          return;
      }
      // TODO Preview: add compatibility API/ABI checks
      std::cout << "DrArm: loaded plugin '" << plugin_api_->api_header.api_description << "'";
      */
    }
    else
    {
      std::cout << "DrArm: plugin is incompatible, missing init function: '"
        << init_name << "', file: " << lib->get_name();
    }
  }
  std::unique_ptr<IDrArm> create_arm() const override;
};

class PluginBackendFactory : public IBackendFactory
{
  public:
    PluginBackendFactory(int api_id, const char *base_name) :
      api_id_(api_id), base_name_(base_name), initialized_(false)
    {
    }
    std::shared_ptr<IBackend> get_backend() const override
    {
      if (!initialized_)
      {
        const_cast<PluginBackendFactory*>(this)->init_backend();
      }
      return backend_;
    }
  protected:
    void init_backend()
    {
      try {
        if (!initialized_)
          load_plugin();
      }
      catch(...)
      {
        std::cout << "Dr ARM: exception during plugin loading: " << base_name_ << ". SKIP" << std::endl;
      }
      initialized_ = true;
    }
    void load_plugin();
  private:
    int api_id_;
    const char* base_name_;
    std::shared_ptr<PluginBackend> backend_;
    bool initialized_;
};

std::list<std::string> get_candidates(const char *base_name)
{
  (void) base_name;
  return {"libarm_gp180.so", "libarm_mh50.so"};
}

void PluginBackendFactory::load_plugin()
{
  for (const std::string &plugin : get_candidates(base_name_))
  {
    auto lib = std::make_unique<DynamicLib> (plugin);
    if (!lib->is_loaded())
      continue;
    try {
      auto plugin_backend = std::make_shared<PluginBackend>(lib);
      if (plugin_backend && plugin_backend->plugin_api_)
      {
        if (plugin_backend->plugin_api_->arm_api != api_id_)
        {
          std::cout << "DR ARM: plugin '" << plugin << "': unexpected backend ID: "
           << plugin_backend->plugin_api_->arm_api << " vs " << api_id_ << " (expected)" << std::endl;
        }
        else
        {
          backend_ = plugin_backend;
          return;
        }
      }
    }
    catch (...)
    {
      std::cout << "DR ARM: exception during plugin initialization: " << plugin << ". SKIP" << std::endl;
    }
  }
}

class PluglinArm : public IDrArm
{
  public:
    static std::unique_ptr<PluglinArm> create(const Dr_Arm_Plugin_API_preview *plugin_api,
      const void *urdf_model, const void *config, const std::string &name)
    {
      assert(plugin_api);
      DrPluginArm arm = nullptr;
      if (plugin_api->Arm_init)
      {
        assert(plugin_api->Arm_release);
        if (plugin_api->Arm_init(0, 0, &arm) == 0)
        {
          assert(arm);
          return std::make_unique<PluglinArm>(plugin_api, arm);
        }
      }
      return nullptr;
    }

    PluglinArm(const Dr_Arm_Plugin_API_preview* plugin_api, DrPluginArm arm)
      : plugin_api_(plugin_api), arm_(arm)
    {
        assert(plugin_api_);
        assert(arm_);
    }

    ~PluglinArm()
    {
        assert(plugin_api_->Arm_release);
        if (0 != plugin_api_->Arm_release(arm_))
            std::cout << "DR ARM: Can't release capture by plugin '" << "api description" << "'";
        arm_ = NULL;
    }
    bool init(const void*, const void*, const std::string&) override
    {
      return true;
    }
    void enable_robot() override
    {
      if (plugin_api_->Arm_enable)
        plugin_api_->Arm_enable(arm_);
    }
    void disable_robot() override
    {
      if (plugin_api_->Arm_enable)
        plugin_api_->Arm_disable(arm_);
    }
    bool reset_alarm() override
    {
      if (plugin_api_->Arm_reset_alarm)
        plugin_api_->Arm_reset_alarm(arm_);
      return true;
    }
    bool set_alarm(int ecode, const std::string &emsg, int subcode) override
    {
      if (plugin_api_->Arm_set_alarm)
        plugin_api_->Arm_set_alarm(arm_, ecode, emsg.c_str(), subcode);
      return true;
    }
    bool execute_trajectory(const void* traj) override
    {
      if (plugin_api_->Arm_execute_trajectory)
        plugin_api_->Arm_execute_trajectory(arm_, traj);
      return true;
    }
    bool queue_trajectory(const void* traj) override
    {
      if (plugin_api_->Arm_queue_trajectory)
        plugin_api_->Arm_queue_trajectory(arm_, traj);
      return true;
    }
    bool stop_trajectory() override
    {
      if (plugin_api_->Arm_stop_trajectory)
        plugin_api_->Arm_stop_trajectory(arm_);
      return true;
    }
    void* get_joint_state() override
    {
      if (plugin_api_->Arm_get_joint_state)
        plugin_api_->Arm_get_joint_state(arm_);
      return nullptr;
    }
    void* get_traj_feedback() override
    {
      if (plugin_api_->Arm_get_traj_feedback)
        plugin_api_->Arm_get_traj_feedback(arm_);
      return nullptr;
    }
  private:
    const Dr_Arm_Plugin_API_preview *plugin_api_;
    DrPluginArm arm_;
};

std::unique_ptr<IDrArm> PluginBackend::create_arm() const
{
  try
  {
    if (plugin_api_)
      return PluglinArm::create(plugin_api_, 0, 0, "");
  }
  catch (...)
  {
    std::cout << "DR ARM: can't init file capture: " << std::endl;
  }
  return nullptr;
}

std::unique_ptr<IBackendFactory> create_plugin_backend_factory(int id, const char *base_name)
{
  return std::make_unique<PluginBackendFactory> (id, base_name);
}

} // end of namespace dr
