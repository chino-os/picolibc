// Copyright (c) SunnyCase. All rights reserved.
// Licensed under the Apache license. See LICENSE file in the project root for full license information.
#include <stdio.h>
#include <sys/cdefs.h>
#include <chino/os/kernel/ke_services.h>
#include <termios.h>
#include <sys/ioctl.h>

using namespace chino;
using namespace chino::os;
using namespace chino::os::kernel;

int
tcgetattr(int fd, struct termios *termiosp)
{
    return ioctl(fd, TCGETS, (uintptr_t)termiosp);
}
