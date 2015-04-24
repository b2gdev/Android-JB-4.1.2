#
# Sierra Wireless User Tags
#
# Add all Sierra specific modules to the list of modules grandfathered to use a
# user tag.  Technically, we are not supposed to add new modules to the list of
# grandfathered modules, but this is the easiest way to manage Sierra specific
# modules.  
#
#
# Copyright (C) 2011 Sierra Wireless, Inc.
#

# This file will be included by build/core/user_tags.mk, so assume the variable
# exists, and we are just adding to it.
GRANDFATHERED_USER_MODULES += \
	libswigpsqmi \
	libswigpsat \
	SierraImgMgr \
	SierraFwDl77xx \
	libswims \
	libswiqmiapi \
	slqssdk \
	swiqmi_test \
	libsierra-ril \
	swirt_server \
	libswirt_ril \
	libswirt_server \
	swirt_client \
	SierraSARTool \
	SierraDMLog \
	libsierraat-ril \
	swisdk \
	swifwdnld \
	libswisdkapi \
	com.sierra.logs \
	ash
