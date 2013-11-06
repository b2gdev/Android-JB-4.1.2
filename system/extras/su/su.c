/*
**
** Copyright 2008, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License"); 
** you may not use this file except in compliance with the License. 
** You may obtain a copy of the License at 
**
**     http://www.apache.org/licenses/LICENSE-2.0 
**
** Unless required by applicable law or agreed to in writing, software 
** distributed under the License is distributed on an "AS IS" BASIS, 
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
** See the License for the specific language governing permissions and 
** limitations under the License.
*/

#define LOG_TAG "su"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

#include <unistd.h>
#include <time.h>

#include <pwd.h>

#include <private/android_filesystem_config.h>

static int permissionDenied()
{
        printf("su: permission denied\n");
        return 1;
}

static int executionFailure(char *context)
{
        fprintf(stderr, "su: %s. Error:%s\n", context, strerror(errno));
        return -errno;

}

int main(int argc, char **argv)
{
	
	if(setgid(0) || setuid(0))
		return permissionDenied();
     
    char *exec_args[argc + 1];
	exec_args[argc] = NULL;
	exec_args[0] = "sh";
	int i;
	for (i = 1; i < argc; i++)
	{
		exec_args[i] = argv[i];
	}
	execv("/system/usr/bin/sh", exec_args);
	return executionFailure("sh");                           
}
