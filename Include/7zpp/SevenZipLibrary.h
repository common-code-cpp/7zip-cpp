#pragma once

#include "typedef.h"
#include "SevenZipException.h"
#include "CompressionFormat.h"

namespace SevenZip
{
	class EXPORT_7ZPP_API SevenZipLibrary
	{
	private:

		typedef UINT32 (WINAPI * CreateObjectFunc)( const GUID* clsID, const GUID* interfaceID, void** outObject );

		HMODULE				m_dll;
		CreateObjectFunc	m_func;

	public:

		SevenZipLibrary();
		~SevenZipLibrary();

		bool Load();
		bool Load( const TString& libraryPath );
		void Free();

		bool CreateObject( const GUID& clsID, const GUID& interfaceID, void** outObject ) const;
	};
}
