[![Build status](https://ci.appveyor.com/api/projects/status/11i6cncafgey8lt3?svg=true)](https://ci.appveyor.com/project/getnamo/7zip-cpp)

# 7zip-cpp
A fork of [SevenZip++](http://bitbucket.org/cmcnab/sevenzip/wiki/Home) for modern builds. Uses cmake to generate build files for desired Visual Studio version, see [setup section for instructions](https://github.com/getnamo/7zip-cpp#using-git-and-cmake).

Uses latest lzma1801 SDK

#### Notes
Originally from:
[http://bitbucket.org/cmcnab/sevenzip/wiki/Home](http://bitbucket.org/cmcnab/sevenzip/wiki/Home)


This is a C++ wrapper for accessing the 7-zip COM-like API in 7z.dll and 7za.dll. This code is heavily based off of the Client7z sample found in the [LZMA SDK](http://www.7-zip.org/sdk.html).

The project itself is a static library.

### Basic Usage

To use, first load the 7z DLL into memory:

```cpp
#include <7zpp/7zpp.h>

SevenZip::SevenZipLibrary lib;
lib.Load();
```

If the appropriate 7z DLL is not in your path you may wish to specify it explicitly in the call to load. Note you may have the 64-bit version installed but are trying to load it from a 32-bit executable; keep that in mind if you encounter errors.

```cpp
lib.Load(_T("path\\to\\7za.dll"));
```

Then create and use a compressor:

```cpp
SevenZip::SevenZipCompressor compressor(lib, archiveName);
compressor.SetCompressionFormat(SevenZip::CompressionFormat::Zip);
compressor.UseAbsolutePaths(false);
compressor.AddFile(targetFile);
compressor.AddDirectory(targetDir);
compressor.DoCompress(callbackfunc);
```

Or an extractor:

```cpp
SevenZip::SevenZipExtractor extractor(lib, archiveName);

// Try to detect compression type
if (!extractor.DetectCompressionFormat())
{
	extractor.SetCompressionFormat(SevenZip::CompressionFormat::Zip);
}

...

// Change this function to suit
SevenZip::ProgressCallBack *extractcallbackfunc = nullptr;

...

extractor.ExtractArchive(destination, extractcallbackfunc);
```

Or a lister:

```cpp
class ListCallBackOutput : SevenZip::ListCallback
{
	virtual void OnFileFound(WCHAR* path, ULONGLONG size)
	{
		std::wcout
			<< path
			<< L" "
			<< size
			<< std::endl;
	}
};

...

SevenZip::SevenZipLister lister(lib, archiveName);

// Try to detect compression type
if (!lister.DetectCompressionFormat())
{
	lister.SetCompressionFormat(SevenZip::CompressionFormat::Zip);
}

ListCallBackOutput myListCallBack;


lister.ListArchive((SevenZip::ListCallback *)&myListCallBack);
```

Note:  Most of the functions now return a boolean to indicate if it worked
instead of throwing an exception.

Otherwise, don't forget to wrap the operations in a try/catch block to handle errors:

```cpp
...
catch (SevenZip::SevenZipException& ex)
{
    std::cerr << ex.GetMessage() << std::endl;
}
...
```

## Setup and Installation

### Using git and cmake
1. Ensure you have [cmake](https://cmake.org/download/) and [git](https://git-scm.com/downloads) installed. Navigate to folder of choice.
2. Open a powershell window and type ```git clone https://github.com/getnamo/7zip-cpp.git --recursive```
3. Navigate into the newly cloned project ```cd 7zip-cpp```
4. (Optional for Test app only) Download and build [Boost](https://www.boost.org/users/download/)
5. Example build with cmake using powershell
```plain 
cd build
cmake -G "Visual Studio 15 2017 Win64" ../
cmake --build ../build --config Release
```
6. Commands from 5 will build win64 Release target and output will be found in ```build/Release/7zpp.lib```

### How to use this library in my project
Add project into your cmakelists 

```plain
add_subdirectory(${pathto7zip-cpp} ${PROJECT_SOURCE_DIR}/build/build7zpp)
target_include_directories(${my_project} INTERFACE ${pathto7zip-cpp}/Include)
target_link_libraries(${my_project} 7zpp)
add_dependencies(${my_project}  7zpp) #might not be necessary
```

### Test Requirements

In order to compile the tests,you must have boost libraries in your path or specify the location where cmake can find them

1. Assuming you're in the project directory
2. Download and build [Boost](https://www.boost.org/users/download/)
```plain
cd build
cmake ../ -DBOOST_ROOT="My boost location"
```
2. Then finally `cmake --build ../build` to build


## Known Issues

~~Only 1 test unit is failing.~~

## Contributing

Read Contributing.md

### Branches

- Devel branch is the bleeding edge, but it should still work.
- Devel-XXX branches are current topics.
- Master branch is the latest stable version.
- More branch information is in Contributing.md.

```plain 
git clone git@github.com:common-code-cpp/7zip-cpp.git --recursive

cd  7zip-cpp
cd build
cmake -G "Visual Studio 17 2022" -A x64 ../
cmake --build ../build --config Debug
cmake --build ../build --config Release


cmake -G "Visual Studio 17 2022" -A ARM64 ../
cmake --build ../build --config Debug
cmake --build ../build --config Release
```


静态库修改：
字符集：使用unicode字符集
运行库：MTD/MT

unzip demo:
```plain
// Unzip.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <numeric>
#include <7zpp/7zpp.h>
#include <7zpp/ProgressCallback.h>

class ZipProgressCallback : public SevenZip::ProgressCallback {
public:

	/*
	Called at beginning
	*/
	void OnStartWithTotal(const SevenZip::TString& archivePath, unsigned __int64 totalBytes) override {
		m_totalBytes = totalBytes;
		std::wcout << __FUNCTION__ << " archivePath:" << archivePath.c_str() << ", totalBytes:" << totalBytes << std::endl;
	}

	/*
	Called Whenever progress has updated with a bytes complete
	*/
	void OnProgress(const SevenZip::TString& archivePath, unsigned __int64 bytesCompleted) override {
		m_bytesCompleted += bytesCompleted;
		std::wcout << __FUNCTION__ << "," << (m_bytesCompleted * 100) / m_totalBytes << "%" << ",bytesCompleted:" << bytesCompleted << ",archivePath:" << archivePath.c_str() << ", bytesCompleted:" << bytesCompleted << std::endl;
	}

	/*
	Called When progress has reached 100%
	*/
	void OnDone(const SevenZip::TString& archivePath) override {
		std::wcout << __FUNCTION__ << " archivePath:" << archivePath.c_str() << std::endl;
	}

	/*
	Called When single file progress has reached 100%, returns the filepath that completed
	*/
	void OnFileDone(const SevenZip::TString& archivePath, const SevenZip::TString& filePath, unsigned __int64 bytesCompleted) override {
		std::wcout << __FUNCTION__ << ",archivePath:" << archivePath.c_str() << ",filePath:" << filePath.c_str() << ", bytesCompleted:" << bytesCompleted << std::endl;
	}

	/*
	Called to determine if it's time to abort the zip operation. Return true to abort the current operation.
	*/
	bool OnCheckBreak() override {
		std::wcout << __FUNCTION__ << std::endl;
		return false;
	}

private:
	unsigned __int64 m_totalBytes = 0;
	unsigned __int64 m_bytesCompleted = 0;
};

int main()
{
	try {
		SevenZip::SevenZipLibrary* lib = CreateSevenZipLibrary();
		if (!lib) {
			std::wcout << "loadCreateSevenZipLibrary failed!!!!!" << std::endl;
			return -1;
		}

		if (!lib->Load(L"C:\\Program Files\\7-Zip\\7z.dll")) {
			std::wcout << "load 7z.dll failed!!!!!" << std::endl;
			return -1;
		}

		SevenZip::TString file = L"C:\\Users\\qwerr\\Desktop\\bin103.bin.7z";
		SevenZip::SevenZipExtractor* extractor = CreateSevenZipExtractor(*lib, file);
		std::wcout << file.c_str() << std::endl;
		if (!extractor) {
			std::wcout << "CreateSevenZipExtractor failed!!!!!" << std::endl;
			return -1;
		}

		// Try to detect compression type
		//if (!extractor->DetectCompressionFormat())
		{
			extractor->SetCompressionFormat(SevenZip::CompressionFormat::SevenZip);
		}

		std::vector<size_t> sizes = extractor->GetOrigSizes();
		std::wcout << "accumulate size:" << std::accumulate(sizes.begin(), sizes.end(), 0) << std::endl;

		// Change this function to suit
		SevenZip::ProgressCallback* extractcallbackfunc = new ZipProgressCallback();
		extractor->ExtractArchive(L"C:\\log\\test\\", extractcallbackfunc);

		DestroySevenZipExtractor(extractor);
		lib->Free();
		DestroySevenZipLibrary(lib);
		delete extractcallbackfunc;
	}
	catch (SevenZip::SevenZipException& ex)
	{
		std::wcout << "ex:" << ex.GetMessageW().c_str() << std::endl;
	}

    std::wcout << "Finished!\n";
}
```
