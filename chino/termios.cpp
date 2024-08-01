// Copyright (c) SunnyCase. All rights reserved.
// Licensed under the Apache license. See LICENSE file in the project root for full license information.
#include <stdio.h>
#include <sys/cdefs.h>
#include <chino/os/ke_services.h>
#include <termios.h>
#include <sys/ioctl.h>

using namespace chino;
using namespace chino::os;

int
tcgetattr(int fd, struct termios *termiosp)
{
    return ioctl(fd, TCGETS, (uintptr_t)termiosp);
}

int
tcsetattr(int fd, int options, const struct termios *termiosp)
{
    if (options != TCSANOW) {
        tcdrain(fd);
    }

    if (options == TCSAFLUSH) {
        tcflush(fd, TCIFLUSH);
    }
    return ioctl(fd, TCSETS, (uintptr_t)termiosp);
}

int
tcdrain(int fd)
{
    return ioctl(fd, TCDRN, (unsigned long)0);
}

int
tcflush(int fd, int cmd)
{
    return ioctl(fd, TCFLSH, (unsigned long)cmd);
}

speed_t
cfgetspeed(const struct termios *termiosp)
{
    return termiosp->c_speed;
}

int
cfsetspeed(struct termios *termiosp, speed_t speed)
{
    termiosp->c_speed = speed;
    return 0;
}
