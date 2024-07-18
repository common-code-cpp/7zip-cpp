// stdafx.cpp : source file that includes just the standard includes
// SevenZip++.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include "7zpp.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

SevenZip::SevenZipLibrary* CreateSevenZipLibrary() {
	return new SevenZip::SevenZipLibrary();
}

void DestroySevenZipLibrary(SevenZip::SevenZipLibrary* pSevenZipLibrary) {
	if (pSevenZipLibrary) {
		delete pSevenZipLibrary;
		pSevenZipLibrary = NULL;
	}
}

SevenZip::SevenZipExtractor* CreateSevenZipExtractor(const SevenZip::SevenZipLibrary& library, const SevenZip::TString& archivePath) {
	return new SevenZip::SevenZipExtractor(library, archivePath);
}

void DestroySevenZipExtractor(SevenZip::SevenZipExtractor* pSevenZipExtractor) {
	if (pSevenZipExtractor) {
		delete pSevenZipExtractor;
		pSevenZipExtractor = NULL;
	}
}