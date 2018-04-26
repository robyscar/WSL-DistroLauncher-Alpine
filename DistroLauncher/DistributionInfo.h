//
//    Copyright (C) Microsoft.  All rights reserved.
// Licensed under the terms described in the LICENSE file in the root of this project.
//

#pragma once

namespace DistributionInfo
{
	// Create and configure a user account.
	bool CreateUser(std::wstring_view userName);

	// Query the UID of the user account.
	ULONG QueryUid(std::wstring_view userName);
}