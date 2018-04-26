#include "stdafx.h"
#define BUFSIZE 1024
#define HASHLEN  32 // 256/8 A sha256 hash requires 256 bits or 32 bytes

namespace ChecksumVerify {
	bool Verify(std::wstring path, std::wstring sha256hash) {
		std::string calculatedHash = GetSHA256(path);
		std::wstring calculatedHash_w(calculatedHash.begin(), calculatedHash.end());
		std::wstring userlandChecksum_w(DistroSpecial::UserlandChecksum);

		std::tcout << std::endl;

		int countIsEqual = 0;
		for (DWORD i = 0; i < DistroSpecial::UserlandChecksum.length(); i++)
		{
			if (DistroSpecial::UserlandChecksum[i] == calculatedHash_w[i]) {
				countIsEqual += 1;
			}
		}
		if (DistroSpecial::UserlandChecksum.length() == countIsEqual && DistroSpecial::UserlandChecksum.length() > 0) {
			std::tcout << "Verifying Hash: OK" << std::endl;
			return true;
		} else {
			std::tcout << "Verifying Hash: Failed" << std::endl;
			return false;
		}
	}
	std::string GetSHA256(std::wstring path) {
		DWORD dwStatus = 0;
		BOOL bResult = FALSE;
		HCRYPTPROV hProv = 0;
		HCRYPTHASH hHash = 0;
		HANDLE hFile = NULL;
		BYTE rgbFile[BUFSIZE];
		DWORD cbRead = 0;
		BYTE rgbHash[HASHLEN];
		DWORD cbHash = 0;
		CHAR rgbDigits[] = "0123456789abcdef";
		LPCWSTR filenameLPCWSTR = L"install.tar.gz";
		const char *filename = "install.tar.gz";
		char sha256sum[65];
		sha256sum[64] = 0; // Strings are null terminated, so the last character needs to be '\0'

		hFile = CreateFile(filenameLPCWSTR, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);

		if (INVALID_HANDLE_VALUE == hFile) {
			dwStatus = GetLastError();
			printf("Error opening file %s\nError: %d\n", filename, dwStatus);
		}

		// Get handle to the crypto provider
		if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
			dwStatus = GetLastError();
			printf("CryptAcquireContext failed: %d\n", dwStatus);
		}

		if (!CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash)) {
			dwStatus = GetLastError();
			printf("CryptAcquireContext failed: %d\n", dwStatus);
		}

		while (bResult = ReadFile(hFile, rgbFile, BUFSIZE, &cbRead, NULL)) {
			if (0 == cbRead)
			{
				break;
			}

			if (!CryptHashData(hHash, rgbFile, cbRead, 0))
			{
				dwStatus = GetLastError();
				printf("CryptHashData failed: %d\n", dwStatus);
			}
		}

		if (!bResult)
		{
			dwStatus = GetLastError();
			printf("ReadFile failed: %d\n", dwStatus);
		}

		cbHash = HASHLEN;
		if (CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0))
		{
			printf("Hash of file %s is: ", filename);
			for (DWORD i = 0; i < cbHash; i++)
			{
				printf("%c%c", rgbDigits[rgbHash[i] >> 4], rgbDigits[rgbHash[i] & 0xf]);
				sha256sum[i*2] = rgbDigits[rgbHash[i] >> 4];
				sha256sum[i*2+1] = rgbDigits[rgbHash[i] & 0xf];
			}
			std::tcout << std::endl;
		}
		else
		{
			dwStatus = GetLastError();
			printf("CryptGetHashParam failed: %d\n", dwStatus);
		}

		CryptDestroyHash(hHash);
		CryptReleaseContext(hProv, 0);
		CloseHandle(hFile);
		return sha256sum;
	}
}
