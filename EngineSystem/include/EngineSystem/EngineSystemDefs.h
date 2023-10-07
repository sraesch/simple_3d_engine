#pragma once

#if defined(_MSC_VER)
#    define DLL_EXPORT __declspec(dllexport)
#    define DLL_IMPORT __declspec(dllimport)
#elif defined(__GNUC__) || defined(__clang__)
#    define DLL_EXPORT __attribute__((visibility("default")))
#    define DLL_IMPORT __attribute__((visibility("default")))
#else
#    define DLL_EXPORT
#    define DLL_IMPORT
#endif

#if defined(utils_EXPORTS)
#    define ENGINE_SYSTEM_API DLL_EXPORT
#else
#    define ENGINE_SYSTEM_API DLL_IMPORT
#endif
