#include "stdafx.h"

#pragma comment(lib, "urlmon.lib")

// Adapted from:
// Creating a progress bar in C/C++ (or any other console app.)
// http://www.rosshemsley.co.uk/2011/02/creating-a-progress-bar-in-c-or-any-other-console-app/
void LoadBar(unsigned curr_val, unsigned max_val, unsigned bar_width = 20)
{
	if ((curr_val != max_val) && (curr_val % (max_val / 100) != 0))
		return;

	double   ratio = curr_val / (double)max_val;
	unsigned bar_now = (unsigned)(ratio * bar_width);

	std::tcout << _T("\r") << std::setw(3) << (unsigned)(ratio * 100.0) << _T("% [");
	for (unsigned curr_val = 0; curr_val < bar_now; ++curr_val)
		std::tcout << _T("=");
	for (unsigned curr_val = bar_now; curr_val < bar_width; ++curr_val)
		std::tcout << _T(" ");
	std::tcout << _T("]") << std::flush;
}

class CallbackHandler : public IBindStatusCallback
{
private:
	int m_percentLast;

public:
	CallbackHandler() : m_percentLast(0) {};

	// IUnknown

	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) {
		if (IsEqualIID(IID_IBindStatusCallback, riid) || IsEqualIID(IID_IUnknown, riid)) {
			*ppvObject = reinterpret_cast<void*>(this);
			return S_OK;
		};
		return E_NOINTERFACE;
	};

	ULONG STDMETHODCALLTYPE AddRef()
	{
		return 2UL;
	};

	ULONG STDMETHODCALLTYPE Release() {
		return 1UL;
	};

	// IBindStatusCallback

	HRESULT STDMETHODCALLTYPE OnStartBinding(DWORD /*dwReserved*/, IBinding* /*pib*/) {
		return E_NOTIMPL;
	};

	HRESULT STDMETHODCALLTYPE GetPriority(LONG* /*pnPriority*/) {
		return E_NOTIMPL;
	};

	HRESULT STDMETHODCALLTYPE OnLowResource(DWORD /*reserved*/) {
		return E_NOTIMPL;
	};

	HRESULT STDMETHODCALLTYPE OnProgress(ULONG   ulProgress, ULONG   ulProgressMax, ULONG   ulStatusCode, LPCWSTR /*szStatusText*/)
	{
		switch (ulStatusCode)
		{
		case BINDSTATUS_FINDINGRESOURCE:
			std::tcout << _T("Finding resource...") << std::endl;
			break;
		case BINDSTATUS_CONNECTING:
			std::tcout << _T("Connecting...") << std::endl;
			break;
		case BINDSTATUS_SENDINGREQUEST:
			std::tcout << _T("Sending request...") << std::endl;
			break;
		case BINDSTATUS_MIMETYPEAVAILABLE:
			std::tcout << _T("Mime type available") << std::endl;
			break;
		case BINDSTATUS_CACHEFILENAMEAVAILABLE:
			std::tcout << _T("Cache filename available") << std::endl;
			break;
		case BINDSTATUS_BEGINDOWNLOADDATA:
			std::tcout << _T("Begin download") << std::endl;
			break;
		case BINDSTATUS_DOWNLOADINGDATA:
		case BINDSTATUS_ENDDOWNLOADDATA:
		{
			int percent = (int)(100.0 * static_cast<double>(ulProgress)
				/ static_cast<double>(ulProgressMax));
			if (m_percentLast < percent)
			{
				LoadBar(percent, 100);
				m_percentLast = percent;
			}
			if (ulStatusCode == BINDSTATUS_ENDDOWNLOADDATA)
			{
				std::tcout << std::endl << _T("End download") << std::endl;
			}
		}
		break;

		default:
		{
			std::tcout << _T("Status code : ") << ulStatusCode << std::endl;
		}
		}
		// The download can be cancelled by returning E_ABORT here
		// of from any other of the methods.
		return S_OK;
	};

	HRESULT STDMETHODCALLTYPE OnStopBinding(HRESULT /*hresult*/, LPCWSTR /*szError*/) {
		return E_NOTIMPL;
	};

	HRESULT STDMETHODCALLTYPE GetBindInfo(DWORD* /*grfBINDF*/, BINDINFO* /*pbindinfo*/) {
		return E_NOTIMPL;
	};

	HRESULT STDMETHODCALLTYPE OnDataAvailable(DWORD /*grfBSCF*/, DWORD /*dwSize*/, FORMATETC* /*pformatetc*/, STGMEDIUM* /*pstgmed*/) {
		return E_NOTIMPL;
	};

	HRESULT STDMETHODCALLTYPE OnObjectAvailable(REFIID /*riid*/, IUnknown* /*punk*/) {
		return E_NOTIMPL;
	};
};

HRESULT DownloadUserland() {
	const TCHAR* url = DistroSpecial::UserlandDownloadURL;
	const TCHAR filePath[] = _T("install.tar.gz");

	std::tcout << "Downloading   : " << url << std::endl;
	std::tcout << "To local file : " << filePath << std::endl;

	char* buffer;
	// Get the current working directory:
	// Passing NULL as the buffer forces getcwd to allocate  
	// memory for the path, which allows the code to support file paths  
	// longer than _MAX_PATH, which are supported by NTFS.
	if ((buffer = _getcwd(NULL, 0)) == NULL) {
		perror("_getcwd error");
	} else {
		std::tcout << "The current working directory is: " << buffer << std::endl;
		free(buffer);
	}

	if (remove("install.tar.gz") != 0) {
		perror("Error deleting file");
	}
	else {
		puts("File successfully deleted");
	}

	// invalidate cache, so file is always downloaded from web site
	// (if not called, the file will be retieved from the cache if
	// it's already been downloaded.)
	//FIXME: DeleteUrlCacheEntry(url);

	CallbackHandler callbackHandler;
	IBindStatusCallback* pBindStatusCallback = NULL;
	callbackHandler.QueryInterface(IID_IBindStatusCallback, reinterpret_cast<void**>(&pBindStatusCallback));

	HRESULT hr = URLDownloadToFile(
		NULL,   // A pointer to the controlling IUnknown interface
		url,
		filePath,
		0,      // Reserved. Must be set to 0.
		pBindStatusCallback);
	callbackHandler.Release();
	if (SUCCEEDED(hr))
	{
		std::tcout << _T("Downloaded OK") << std::endl;
		return S_OK;
	}
	else
	{
		std::tcout << _T("An error occured : error code = 0x") << std::hex << hr << std::endl;
		return hr;
	}
};
