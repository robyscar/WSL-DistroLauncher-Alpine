//
//    Copyright (C) Microsoft.  All rights reserved.
// Licensed under the terms described in the LICENSE file in the root of this project.
//

#include "stdafx.h"


namespace {
	HRESULT FormatMessageHelperVa(DWORD messageId, va_list vaList, std::wstring* message);
	HRESULT PrintMessageVa(DWORD messageId, va_list vaList);
}

std::wstring Helpers::CreateTemporaryDirectory() {
	std::wstring strTempPath = L"";
	wchar_t wchPath[MAX_PATH];
	if (GetTempPathW(MAX_PATH, wchPath))
		strTempPath = wchPath;

	fs::path dir(strTempPath);
	fs::path dir2(L"wsl_" + DistroSpecial::Name + L"_setup");
	fs::path full_path = dir / dir2;
	CreateDirectoryW(full_path.c_str(), NULL); // TODO: Check return status
	std::wcout << L"Temporary directory: " << full_path << std::endl;
	return full_path;
}

void Helpers::SetWorkingDirectory(std::wstring newPath) {
	SetCurrentDirectoryW(newPath.c_str());
}

std::wstring Helpers::GetUserInput(DWORD promptMsg, DWORD maxCharacters)
{
	Helpers::PrintMessage(promptMsg);
	size_t bufferSize = maxCharacters + 1;
	std::unique_ptr<wchar_t[]> inputBuffer(new wchar_t[bufferSize]);
	std::wstring input;
	if (wscanf_s(L"%s", inputBuffer.get(), (unsigned int)bufferSize) == 1) {
		input = inputBuffer.get();
	}

	// Throw away any additional chracters that did not fit in the buffer.
	wchar_t wch;
	do {
		wch = getwchar();

	} while ((wch != L'\n') && (wch != WEOF));

	return input;
}

void Helpers::PrintErrorMessage(HRESULT error)
{
	PWSTR buffer = nullptr;
	::FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
		nullptr,
		error,
		0,
		(PWSTR)&buffer,
		0,
		nullptr);

	Helpers::PrintMessage(MSG_ERROR_CODE, error, buffer);
	if (buffer != nullptr) {
		HeapFree(GetProcessHeap(), 0, buffer);
	}

	return;
}

HRESULT Helpers::PrintMessage(DWORD messageId, ...)
{
	va_list argList;
	va_start(argList, messageId);
	HRESULT hr = PrintMessageVa(messageId, argList);
	va_end(argList);
	return hr;
}

void Helpers::PromptForInput()
{
	Helpers::PrintMessage(MSG_PRESS_A_KEY);
	_getwch();
	return;
}

namespace {
	HRESULT FormatMessageHelperVa(DWORD messageId, va_list vaList, std::wstring* message)
	{
		PWSTR buffer = nullptr;
		DWORD written = ::FormatMessageW(FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_ALLOCATE_BUFFER,
			nullptr,
			messageId,
			0,
			(PWSTR)&buffer,
			10,
			&vaList);
		*message = buffer;
		if (buffer != nullptr) {
			HeapFree(GetProcessHeap(), 0, buffer);
		}

		return (written > 0) ? S_OK : HRESULT_FROM_WIN32(GetLastError());
	}

	HRESULT PrintMessageVa(DWORD messageId, va_list vaList)
	{
		std::wstring message;
		HRESULT hr = FormatMessageHelperVa(messageId, vaList, &message);
		if (SUCCEEDED(hr)) {
			wprintf(L"%ls", message.c_str());
		}

		return hr;
	}
}
