/**
 * \Copyright Dorabot Inc.
 * \date : 2019
 * \author : weibin.li@dorabot.com
 * \brief : For help export class
 */


#ifndef __DR_CLASS_LIBRARY_HELP_HH__
#define __DR_CLASS_LIBRARY_HELP_HH__

#include "manifest.hh"

extern "C" bool dr_build_manifest(void* manif);
typedef bool (*dr_build_manifest_func_t)(void* manif);
#define TO_STR(symbol) #symbol
#define BUILD_MANIFEST_STR TO_STR(dr_build_manifest)

//
// Macros to automatically implement dr_build_manifest
//
// usage:
//
// DR_BEGIN_MANIFEST(MyBaseClass)
//     DR_EXPORT_CLASS(MyFirstClass)
//     DR_EXPORT_CLASS(MySecondClass)
//		 DR_EXPORT_SINGLETON(MyThirdClass) // for SINGLETON
//     ...
// DR_END_MANIFEST
//
#define DR_BEGIN_MANIFEST_IMPL(fnName, base)                                    \
	bool fnName(void *manif)										                                  \
	{																				                                      \
    typedef base Base;                                                     			\
		typedef dr::Manifest<Base> base_manifest_t;									      					\
		dr::IManifest *manifest = static_cast<dr::IManifest *>(manif);        			\
    std::string required(typeid(base_manifest_t).name());							      		\
		std::string actual(manifest->class_name());							                    \
		if (required == actual)												                              \
		{																			                                      \
			base_manifest_t* base_manif = static_cast<base_manifest_t*>(manifest);    \


#define DR_BEGIN_MANIFEST(base)		\
	DR_BEGIN_MANIFEST_IMPL(dr_build_manifest, base)


#define DR_END_MANIFEST   \
			return true;	      \
		}					            \
		else return false;	  \
	}

#define DR_EXPORT_CLASS(cls)		\
  base_manif->insert(new dr::MetaObject<cls, Base>(#cls));

#define DR_EXPORT_SINGLETON(cls)	\
	base_manif->insert(new dr::MetaSingleton<cls, Base>(#cls));

#endif //__DR_CLASS_LIBRARY_HELP_HH__
