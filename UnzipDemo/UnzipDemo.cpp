// Unzip.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <numeric>
#include <7zpp/7zpp.h>
#include <7zpp/ProgressCallback.h>
#include <7zpp/ListCallback.h>

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
		//std::wcout << __FUNCTION__ << "," << (m_bytesCompleted * 100) / m_totalBytes << "%" << ",bytesCompleted:" << bytesCompleted << ",archivePath:" << archivePath.c_str() << ", bytesCompleted:" << bytesCompleted << std::endl;
	}

	/*
	Called When progress has reached 100%
	*/
	void OnDone(const SevenZip::TString& archivePath) override {
		//std::wcout << __FUNCTION__ << " archivePath:" << archivePath.c_str() << std::endl;
	}

	/*
	Called When single file progress has reached 100%, returns the filepath that completed
	*/
	void OnFileDone(const SevenZip::TString& archivePath, const SevenZip::TString& filePath, unsigned __int64 bytesCompleted) override {
		//std::wcout << __FUNCTION__ << ",archivePath:" << archivePath.c_str() << ",filePath:" << filePath.c_str() << ", bytesCompleted:" << bytesCompleted << std::endl;
	}

	/*
	Called to determine if it's time to abort the zip operation. Return true to abort the current operation.
	*/
	bool OnCheckBreak() override {
		//std::wcout << __FUNCTION__ << std::endl;
		return false;
	}

	/*
	Called When error, return true will retry
	*/
	bool OnError(const SevenZip::TString& fileAbPath, unsigned __int64 errorCode, int retryTime) override {
		std::wcout << __FUNCTION__<< "fileAbPath:" << fileAbPath 
			<< ", errorCode:" << errorCode << ",retryTime:" << retryTime << std::endl;
		if (retryTime > 10) {
			return false;
		}
		return true;
	}

private:
	unsigned __int64 m_totalBytes = 0;
	unsigned __int64 m_bytesCompleted = 0;
};

class ListCallBackOutput : SevenZip::ListCallback
{
public:
	~ListCallBackOutput() = default;

	virtual void OnFileFound(const SevenZip::intl::FileInfo& info)
	{
		m_files.push_back(info);
	}

	virtual void OnListingDone(const SevenZip::TString& path)
	{
	}

	const std::vector<SevenZip::FileInfo>& GetList() const { return m_files; }

protected:
	std::vector<SevenZip::FileInfo> m_files;
};

void GetItemNameCallback(const wchar_t *path, void * useData)
{
	std::vector<SevenZip::TString> *names = (std::vector<SevenZip::TString>*)useData;
	if (names) 
	{
		names->push_back(path);
	}
}

void GetOrigSizesCallback(size_t s, void* useData)
{
	std::vector<size_t>* sizes = (std::vector<size_t>*)useData;
	if (sizes)
	{
		sizes->push_back(s);
	}
}

int main()
{
#if 0
	try {
		ListCallBackOutput myListCallBack;
		SevenZip::SevenZipLibrary lib;
		bool b = lib.Load(L"C:\\Program Files\\7-Zip\\7z.dll");

		SevenZip::SevenZipLister lister(lib, L"D:\\projects\\demos\\Unzip\\AI Now-49c26.7z");
		lister.SetCompressionFormat(SevenZip::CompressionFormat::SevenZip);

		bool b1= lister.ListArchive(L"", (SevenZip::ListCallback*)&myListCallBack);

		for (const auto& info : myListCallBack.GetList())
		{
			if (info.FileName.find(_T("FastRestore64.dll")) != -1) {
				int i = 0;
				i++;
			}
			std::wcout
				<< info.FileName
#if 0
				<< L"\n Size: "
				<< info.Size
				<< L" Packed Size: "
				<< info.PackedSize
				<< L" Attributes: "
				<< info.Attributes
				<< L"\n Create Time: "
				//<< CTime(info.CreationTime).Format(_T("%F %T")).GetString()
				<< L" Access Time: "
				//<< CTime(info.LastAccessTime).Format(_T("%F %T")).GetString()
				<< L" Modify Time: "
				//<< CTime(info.LastWriteTime).Format(_T("%F %T")).GetString()
#endif
				<< std::endl;
		}
	}
	catch (SevenZip::SevenZipException& ex)
	{
		std::wcout << "ex:" << ex.GetMessageW().c_str() << std::endl;
	}
	return 0;

#endif

	try 
	{
		SevenZip::SevenZipLibrary lib;
		if (!lib.Load(L"C:\\Program Files\\Lenovo\\Lenovo IQ\\7Zip\\7z.dll")) {
			std::wcout << "load 7z.dll failed!!!!!" << std::endl;
			return -1;
		}

		//L"D:\\23.09.11.9383-b76eb4.7z", L"C:\\Program Files\\Cyberhaven\\23.09.11.9383-b76eb4"

		//SevenZip::TString file = L"D:\\projects\\demos\\Unzip\\SmartCache.7z";
		SevenZip::TString file = L"C:\\Users\\qwerr\\Desktop\\bin103.bin.7z";
		SevenZip::SevenZipExtractor extractor(lib, file);
		std::wcout << file.c_str() << std::endl;

		// Try to detect compression type
		//if (!extractor->DetectCompressionFormat())
		{
			extractor.SetCompressionFormat(SevenZip::CompressionFormat::SevenZip);
		}

		{
			std::vector<SevenZip::TString> names;
			extractor.GetItemsNamesEx(GetItemNameCallback, &names);
			for (int i = 0; i < names.size(); i++) {
				std::wcout << "to remove file:" << names[i] << std::endl;
			}
		}

		std::vector<size_t> sizes;
		extractor.GetOrigSizesEx(GetOrigSizesCallback, &sizes);
		std::wcout << "accumulate size:" << std::accumulate(sizes.begin(), sizes.end(), 0) << std::endl;

		// Change this function to suit
		ZipProgressCallback extractcallbackfunc;
		bool ret = extractor.ExtractArchive(L"D:\\projects\\thirdparty-source\\test\\", &extractcallbackfunc);
		if (!ret) {
			std::cerr << "ExtractArchive failed**************" << std::endl;
		}

		lib.Free();
	}
	catch (SevenZip::SevenZipException& ex)
	{
		std::wcout << "ex:" << ex.GetMessageW().c_str() << std::endl;
	}

	std::wcout << "Finished!\n";
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
