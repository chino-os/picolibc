// Copyright (c) SunnyCase. All rights reserved.
// Licensed under the Apache license. See LICENSE file in the project root for full license information.
#include <stdio.h>
#include <chino/os/kernel/ke_services.h>
#include <unistd.h>
#include <sys/file.h>

using namespace chino;
using namespace chino::os;
using namespace chino::os::kernel;

int
open(const char *pathname, int flags, ...)
{
    auto fd = ke_services().open(pathname, flags, 0666);
    if (fd != -1) {
        return fd;
    } else {
        errno = ke_services().errno_();
        return -1;
    }
}

int
close(int fd)
{
    if (fd <= STDERR_FILENO)
        return 0;
    if (ke_services().close(fd) != -1) {
        return 0;
    } else {
        errno = ke_services().errno_();
        return -1;
    }
}

ssize_t
read(int fd, void *buf, size_t count)
{
    auto ret = ke_services().read(fd, buf, count);
    if (ret != -1) {
        return ret;
    } else {
        errno = ke_services().errno_();
        return -1;
    }
}

ssize_t
write(int fd, const void *buf, size_t count)
{
    auto ret = ke_services().write(fd, buf, count);
    if (ret != -1) {
        return ret;
    } else {
        errno = ke_services().errno_();
        return -1;
    }
}
