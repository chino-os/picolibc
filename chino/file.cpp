// Copyright (c) SunnyCase. All rights reserved.
// Licensed under the Apache license. See LICENSE file in the project root for full license information.
#include <stdio.h>
#include <chino/os/ke_services.h>
#include <unistd.h>
#include <sys/file.h>
#include <chino/os/ioapi.h>
#include <sys/ioctl.h>

using namespace chino;
using namespace chino::os;

#ifndef CHINO_CRT_KERNEL
i_ke_services &
os::ke_services() noexcept
{
    return *reinterpret_cast<i_ke_services *>(ke_services_address);
}
#endif

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

int
ioctl(int fd, int req, ...)
{
    va_list ap;
    va_start(ap, req);
    auto arg = va_arg(ap, void *);
    auto ret = ke_services().ioctl(fd, req, arg);
    va_end(ap);

    if (ret != -1) {
        return ret;
    } else {
        errno = ke_services().errno_();
        return -1;
    }
}

result<async_io_result *>
os::wait_queued_io() noexcept
{
    return ke_services().wait_queued_io();
}

result<void>
os::read_async(int fd, std::span<std::byte> buffer, size_t offset,
               async_io_result &result) noexcept
{
    return ke_services().read_async(fd, buffer, offset, result);
}
