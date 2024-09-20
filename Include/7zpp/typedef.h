#pragma once

#ifdef SHARED_LIBRARAY_7ZPP
#ifdef EXPORT_7ZPP
#define EXPORT_7ZPP_API __declspec(dllexport)
#else
#define EXPORT_7ZPP_API __declspec(dllimport)
#endif
#else
#define EXPORT_7ZPP_API
#endif

typedef void (*ItemNameCallback)(const wchar_t*, void*);
typedef void (*OrigSizeCallback)(size_t, void*);
