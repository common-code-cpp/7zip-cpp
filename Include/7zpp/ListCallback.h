#pragma once

#include "typedef.h"
#include "SevenZipLibrary.h"
#include "CompressionFormat.h"


namespace SevenZip
{
	using namespace intl;

	class EXPORT_7ZPP_API ListCallback
	{
	public:
		/*
		Called for each file found in the archive. Size in bytes.
		*/
		virtual void OnFileFound(const FileInfo& fileInfo) {}

		/*
		Called when all the files have been listed
		*/
		virtual void OnListingDone(const TString& path) {}
	};
}
