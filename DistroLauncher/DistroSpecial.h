#pragma once

namespace DistroSpecial
{
	// The name of the distribution. This will be displayed to the user via
	// wslconfig.exe and in other places. It must conform to the following
	// regular expression: ^[a-zA-Z0-9._-]+$
	//
	// WARNING: This value must not change between versions of your app,
	// otherwise users upgrading from older versions will see launch failures.
	const std::wstring Name = L"Alpine";

	// The title bar for the console window while the distribution is installing.
	const std::wstring WindowTitle = L"Alpine";

	// Command Lines
	const std::wstring commandLinePreAddUser[] = {
		L"/bin/chmod 755 /",  // Alpine tar.gz image has wrong permissions on the "/" aka. root folder, this prevents su from working, so only "install --root" is working.
		L"/sbin/apk --no-cache add shadow"  // Provides /usr/sbin/usermod
	}; 
	const std::wstring commandLineAddUser = L"/usr/sbin/adduser -g '' -D ";
	// const std::wstring commandLineAddUserToGroups = L"/usr/sbin/usermod -aG adm,cdrom,sudo,dip,plugdev ";
	const std::wstring commandLineAddUserToGroups = L"/usr/sbin/usermod -aG adm,floppy,cdrom,tape,wheel,ping ";
	const std::wstring commandLineDeleteUSer = L"/usr/sbin/deluser --remove-home ";
	const std::wstring commandLineQueryUID = L"/usr/bin/id -u ";
	const PCWSTR commandLineDeleteResolvConf = L"/bin/rm /etc/resolv.conf";
	const TCHAR UserlandDownloadURL[] = _T("http://dl-cdn.alpinelinux.org/alpine/v3.7/releases/x86_64/alpine-minirootfs-3.7.0-x86_64.tar.gz");
	const std::wstring UserlandChecksum = L"17e8275545aca7a6eaf4e43759a091d33bd9eb9cf11805118773dc940c8b94ac";
}
