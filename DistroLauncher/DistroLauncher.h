#pragma once

// Commandline arguments: 
#define ARG_CONFIG              L"config"
#define ARG_CONFIG_DEFAULT_USER L"--default-user"
#define ARG_INSTALL             L"install"
#define ARG_INSTALL_ROOT        L"--root"
#define ARG_RUN                 L"run"
#define ARG_RUN_C               L"-c"

static HRESULT InstallDistribution(bool createUser);
static HRESULT SetDefaultUser(std::wstring_view userName);
