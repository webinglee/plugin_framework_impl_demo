/**
 * \Copyright Dorabot Inc.
 * \date : 2019
 * \author : weibin.li@dorabot.com
 * \brief : maintainer of the subclass to create
 */

#ifndef __DR_MANIFEST_HH__
#define __DR_MANIFEST_HH__

#include <map>
#include <set>
#include <memory>
#include <exception>

namespace dr
{
template <class B>
class IMetaObject
{
public:
	explicit IMetaObject(const char* name): name_(name) {}
	virtual ~IMetaObject()
	{
		for (typename object_set_t::iterator it = delete_objs_.begin();
      it != delete_objs_.end(); ++it)
		{
			delete *it;
		}
	}
  IMetaObject() = delete;
	IMetaObject(const IMetaObject&) = delete;
	IMetaObject& operator = (const IMetaObject&) = delete;

	const char* name() const
	{
		return name_.c_str();
	}
	virtual B* create() const = 0;
	virtual B& instance() const = 0;
	virtual bool can_create() const = 0;
	virtual void destroy(B* obj) const
	{
		typename object_set_t::iterator it = delete_objs_.find(obj);
		if (it != delete_objs_.end())
		{
			delete_objs_.erase(obj);
			delete obj;
		}
	}
	B* auto_delete(B* obj) const
	{
		if (this->can_create())
		{
			delete_objs_.insert(obj);
		}
		else throw std::runtime_error(std::string("Cannot take ownership of ") + name_);

		return obj;
	}
	virtual bool is_auto_delete(B* obj) const
	{
		return delete_objs_.find(obj) != delete_objs_.end();
	}
private:
	typedef std::set<B*> object_set_t;

	const std::string name_;
	mutable object_set_t delete_objs_;
};


template <class C, class B>
class MetaObject: public IMetaObject<B>
{
public:
	explicit MetaObject(const char* name): IMetaObject<B>(name) {}
	~MetaObject(){}

	B* create() const
	{
		return new C;
	}
	B& instance() const
	{
		throw std::runtime_error(std::string("Not a singleton. Use create() to create instances of ") + this->name());
	}
	bool can_create() const
	{
		return true;
	}
};


template <class C, class B>
class MetaSingleton: public IMetaObject<B>
{
public:
	explicit MetaSingleton(const char* name): IMetaObject<B>(name), object_(new C) {}
	~MetaSingleton() {}

	B* create() const
	{
		throw std::runtime_error(std::string("Cannot create instances of a singleton class. "\
      "Use instance() to obtain a ") + this->name());
	}
	bool can_create() const
	{
		return false;
	}
	B& instance() const
	{
		return *object_.get();
	}
	bool is_auto_delete(B*) const
	{
		return true;
	}

private:
	mutable std::unique_ptr<C> object_;
};

////////////////////////////////////Manifest///////////////////////////////////////////
class IManifest
{
public:
	virtual ~IManifest() {}
	virtual const char* class_name() const = 0;
};

template <class B>
class Manifest: public IManifest
{
public:
	typedef IMetaObject<B> meta_t;
	typedef std::map<std::string, const meta_t*> meta_map_t;

	class Iterator
	{
	public:
		Iterator(const typename meta_map_t::const_iterator& it)
		{
			it_ = it;
		}
		Iterator(const Iterator& it)
		{
			it_ = it.it_;
		}
		~Iterator() {}
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
		inline const meta_t* operator* () const
		{
			return it_->second;
		}
		inline const meta_t* operator-> () const
		{
			return it_->second;
		}
	private:
		typename meta_map_t::const_iterator it_;
	};

	Manifest() {}
	virtual ~Manifest()
	{
		clear();
	}

	Iterator find(const std::string& className) const
	{
		return Iterator(meta_map_.find(className));
	}

	Iterator begin() const
	{
		return Iterator(meta_map_.begin());
	}

	Iterator end() const
	{
		return Iterator(meta_map_.end());
	}

	bool insert(const meta_t* meta)
	{
		return meta_map_.insert(typename meta_map_t::value_type(meta->name(), meta)).second;
	}

	void clear()
	{
		for (typename meta_map_t::iterator it = meta_map_.begin(); it != meta_map_.end(); ++it)
		{
			delete it->second;
		}
		meta_map_.clear();
	}

	int size() const
	{
		return static_cast<int>(meta_map_.size());
	}

	bool empty() const
	{
		return meta_map_.empty();
	}

	const char* class_name() const
	{
		return typeid(*this).name();
	}

private:
	meta_map_t meta_map_;
};

}
#endif // __DR_MANIFEST_HH__
