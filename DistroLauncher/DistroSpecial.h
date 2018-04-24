#pragma once

namespace DistroSpecial
{
	// Command Lines
	const std::wstring commandLineAddUser = L"/usr/sbin/adduser --quiet --gecos '' ";
	const std::wstring commandLineAddUserToGroups = L"/usr/sbin/usermod -aG adm,cdrom,sudo,dip,plugdev ";
	const std::wstring commandLineDeleteUSer = L"/usr/sbin/deluser ";
	const std::wstring commandLineQueryUID = L"/usr/bin/id -u ";
	const PCWSTR commandLineDeleteResolvConf = L"/bin/rm /etc/resolv.conf";
	const TCHAR UserlandDownloadURL[] = _T("http://dl-cdn.alpinelinux.org/alpine/v3.7/releases/x86_64/alpine-minirootfs-3.7.0-x86_64.tar.gz");
	const std::wstring UserlandChecksum = L"17e8275545aca7a6eaf4e43759a091d33bd9eb9cf11805118773dc940c8b94ac";
}
