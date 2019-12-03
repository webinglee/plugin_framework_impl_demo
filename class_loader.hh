/**
 * \Copyright Dorabot Inc.
 * \date : 2019
 * \author : weibin.li@dorabot.com
 * \brief : ClassLoader dynamically load shared object for export class
 */

#ifndef __DR_CLASS_LOADER_HH__
#define __DR_CLASS_LOADER_HH__

#include <mutex>
#include "dynamic_lib.hh"
#include "class_library_helper.hh"

namespace dr
{

template <class Base>
class ClassLoader
{
public:
	typedef IMetaObject<Base> base_meta_t;
	typedef Manifest<Base> base_manifest_t;

	struct LibraryInfo
	{
		DynamicLib *dlib;
    const base_manifest_t *manifest;
		int ref;
	};
	typedef std::map<std::string, LibraryInfo> library_map_t;

  class Iterator
	{
	public:
		typedef std::pair<std::string, const base_manifest_t*> Pair;

		Iterator(const typename library_map_t::const_iterator& it) : it_(it) {}
		Iterator(const Iterator& it) : it_(it.it_) {}
		~Iterator() = default;

		Iterator& operator= (const Iterator& it)
		{
			it_ = it.it_;
			return *this;
		}
		inline bool operator== (const Iterator& it) const
		{
			return it_ == it.it_;
		}
		inline bool operator!= (const Iterator& it) const
		{
			return it_ != it.it_;
		}
		Iterator& operator++ ()
		{
			++it_;
			return *this;
		}
		Iterator operator++ (int)
		{
			Iterator result(it_);
			++it_;
			return result;
		}
		inline const Pair* operator* () const
		{
			pair_.first  = it_->first;
			pair_.second = it_->second.manifest;
			return &pair_;
		}
		inline const Pair* operator-> () const
		{
			pair_.first  = it_->first;
			pair_.second = it_->second.manifest;
			return &pair_;
		}

	private:
		typename library_map_t::const_iterator it_;
		mutable Pair pair_;
	};

	ClassLoader() = default;
	virtual ~ClassLoader()
  {
    for (typename library_map_t::const_iterator it = libs_.begin(); it != libs_.end(); ++it)
		{
			delete it->second.manifest;
      delete it->second.dlib;
		}
  }

	void load_library(const std::string &path)
	{
		std::lock_guard<std::mutex> lock(mtx_);
		typename library_map_t::iterator it = libs_.find(path);
		if (it == libs_.end())
		{
			LibraryInfo li{nullptr, nullptr, 1};
			try
			{
				li.dlib = new DynamicLib(path);
				li.manifest = new base_manifest_t();
				if (li.dlib->is_loaded())
				{
					auto build_manifest = (dr_build_manifest_func_t) li.dlib->get_symbol(BUILD_MANIFEST_STR);
					if (build_manifest && build_manifest(const_cast<base_manifest_t *>(li.manifest)))
						libs_[path] = li;
					else if(build_manifest == nullptr)
            throw std::runtime_error(std::string("No manifest in ") + path);
          else
						throw std::runtime_error(std::string("Manifest class mismatch in ") + path);
				}
				else throw std::runtime_error(std::string("Load library of ") + path + "failed");
			}
			catch (...)
			{
        delete li.dlib;
				delete li.manifest;
				throw;
			}
		}
		else
		{
			++it->second.ref;
		}
	}

	void unload_library(const std::string &path)
	{
		std::lock_guard<std::mutex> lock(mtx_);
		typename library_map_t::iterator it = libs_.find(path);
		if (it != libs_.end())
		{
			if (--it->second.ref == 0)
			{
        delete it->second.dlib;
				delete it->second.manifest;
				libs_.erase(it);
			}
		}
	}

  std::unique_ptr<Base> create(const std::string &class_name) const
	{
		return std::unique_ptr<Base>(class_for(class_name).create());
	}
	Base& instance(const std::string &class_name) const
	{
		return class_for(class_name).instance();
	}
	bool can_create(const std::string &class_name) const
	{
		return class_for(class_name).can_create();
	}
	bool is_auto_delete(const std::string &class_name, Base *obj) const
	{
		return class_for(class_name).is_auto_delete(obj);
	}
  bool is_library_loaded(const std::string& path) const
	{
		return find_manifest(path) != 0;
	}

	Iterator begin() const
	{
		std::lock_guard<std::mutex> lock(mtx_);
		return Iterator(libs_.begin());
	}
	Iterator end() const
	{
		std::lock_guard<std::mutex> lock(mtx_);
		return Iterator(libs_.end());
	}
  const base_meta_t* find_class(const std::string& class_name) const
	{
		std::lock_guard<std::mutex> lock(mtx_);
		for (typename library_map_t::const_iterator it = libs_.begin(); it != libs_.end(); ++it)
		{
			auto manif = it->second.manifest;
			typename base_manifest_t::Iterator itm = manif->find(class_name);
			if (itm != manif->end())
				return *itm;
		}
		return 0;
	}
	const base_meta_t& class_for(const std::string &class_name) const
	{
		auto meta = find_class(class_name);
		if (meta)
			return *meta;
		else
			throw std::runtime_error("Not Found of " + class_name);
	}
	const base_manifest_t* find_manifest(const std::string& path) const
	{
    std::lock_guard<std::mutex> lock(mtx_);
		typename library_map_t::const_iterator it = libs_.find(path);
		if (it != libs_.end())
			return it->second.manifest;
		else
			return nullptr;
	}
	const base_manifest_t& manifest_for(const std::string& path) const
	{
		auto manif = find_manifest(path);
		if (manif)
			return *manif;
		else
			throw std::runtime_error("Not Found of " + path);
	}

protected:
  Base* create_raw(const std::string &class_name) const
	{
		return class_for(class_name).create();
	}
  void destroy(const std::string &class_name, Base *obj) const
	{
		class_for(class_name).destroy(obj);
	}

private:
	library_map_t libs_;
	mutable std::mutex mtx_;
};

}

#endif //__DR_CLASS_LOADER_HH__
