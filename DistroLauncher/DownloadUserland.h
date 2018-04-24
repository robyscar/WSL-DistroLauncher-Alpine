#pragma once

void LoadBar(unsigned curr_val, unsigned max_val, unsigned bar_width = 20);
HRESULT DownloadUserland();
class CallbackHandler : public IBindStatusCallback {
private:
	int m_percentLast;
public:
	CallbackHandler() : m_percentLast(0) {};
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject);
	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();
	HRESULT STDMETHODCALLTYPE OnStartBinding(DWORD /*dwReserved*/, IBinding* /*pib*/);
	HRESULT STDMETHODCALLTYPE GetPriority(LONG* /*pnPriority*/);
	HRESULT STDMETHODCALLTYPE OnLowResource(DWORD /*reserved*/);
	HRESULT STDMETHODCALLTYPE OnProgress(ULONG   ulProgress, ULONG   ulProgressMax, ULONG   ulStatusCode, LPCWSTR /*szStatusText*/);
	HRESULT STDMETHODCALLTYPE OnStopBinding(HRESULT /*hresult*/, LPCWSTR /*szError*/);
	HRESULT STDMETHODCALLTYPE GetBindInfo(DWORD*    /*grfBINDF*/, BINDINFO* /*pbindinfo*/);
	HRESULT STDMETHODCALLTYPE OnDataAvailable(DWORD /*grfBSCF*/, DWORD /*dwSize*/, FORMATETC* /*pformatetc*/, STGMEDIUM* /*pstgmed*/);
	HRESULT STDMETHODCALLTYPE OnObjectAvailable(REFIID /*riid*/, IUnknown* /*punk*/);
};
