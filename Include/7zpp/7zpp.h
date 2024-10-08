#pragma once

#include "typedef.h"
#include "SevenZipCompressor.h"
#include "SevenZipExtractor.h"
#include "SevenZipLister.h"

// Version of this library
#define SEVENZIP_VERSION L"0.3.0.20180422"
#define SEVENZIP_BRANCH L"master"

#ifdef __cplusplus 
extern "C"
{
#endif // __cplusplus
	EXPORT_7ZPP_API SevenZip::SevenZipLibrary* CreateSevenZipLibrary();
	EXPORT_7ZPP_API void DestroySevenZipLibrary(SevenZip::SevenZipLibrary*);

	EXPORT_7ZPP_API SevenZip::SevenZipExtractor* CreateSevenZipExtractor(const SevenZip::SevenZipLibrary& library, const SevenZip::TString& archivePath);
	EXPORT_7ZPP_API void DestroySevenZipExtractor(SevenZip::SevenZipExtractor*);
#ifdef __cplusplus
}
#endif // __cplusplus
