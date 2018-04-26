#pragma once
#pragma comment(lib, "crypt32.lib")

namespace ChecksumVerify
{
	bool Verify(std::wstring path, std::wstring sha256hash);
	std::string GetSHA256(std::wstring path);
}
