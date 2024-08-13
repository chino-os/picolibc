// Copyright (c) SunnyCase. All rights reserved.
// Licensed under the Apache license. See LICENSE file in the project root for full license information.
#include "chino/os/hal/chips/wch/ch582/chip.h"
#include <stdio.h>
#include <chino/os/ke_services.h>
#include <unistd.h>
#include <sys/file.h>
#include <chino/os/ioapi.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>

using namespace chino;
using namespace chino::os;

#ifndef CHINO_CRT_KERNEL
i_ke_services &
os::ke_services() noexcept
{
    return *reinterpret_cast<i_ke_services *>(hal::chip_t::ke_services_address);
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

int
socket(int domain, int type, int protocol)
{
    return ke_services().socket(domain, type, protocol);
}

int
setsockopt(int sockfd, int level, int option, const void *value,
           socklen_t value_len)
{
    return ke_services().setsockopt(sockfd, level, option, value, value_len);
}

ssize_t
sendto(int sockfd, const void *buf, size_t len, int flags,
       const struct sockaddr *to, socklen_t tolen)
{
    return ke_services().sendto(sockfd, buf, len, flags, to, tolen);
}

ssize_t
recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *from,
         socklen_t *fromlen)
{
    return ke_services().recvfrom(sockfd, buf, len, flags, from, fromlen);
}

char *
inet_ntoa_r(struct in_addr in, char *buf, size_t bufflen)
{
    unsigned char *ptr = (unsigned char *)&in.s_addr;
    snprintf(buf, bufflen, "%u.%u.%u.%u", ptr[0], ptr[1], ptr[2], ptr[3]);
    return buf;
}

char *
inet_ntoa(struct in_addr in)
{
    static char buffer[INET_ADDRSTRLEN];
    return inet_ntoa_r(in, buffer, sizeof(buffer));
}

const char *
inet_ntop(int af, const void *__restrict__ src, char *__restrict__ dst,
          socklen_t size)
{
    if (size == 0) {
        return nullptr;
    }

    switch (af) {
    case AF_INET: {
        {
            const char *formatted =
                inet_ntoa(*static_cast<const in_addr *>(src));
            if (formatted && strlen(formatted) < static_cast<size_t>(size)) {
                return ::strcpy(dst, formatted);
            }
        }
        return nullptr;
    case AF_INET6: {
        char tmp[INET6_ADDRSTRLEN] = { 0 };
        const uint16_t *src16 = static_cast<const uint16_t *>(src);
        int full_size =
            ::snprintf(tmp, sizeof(tmp), "%x:%x:%x:%x:%x:%x:%x:%x",
                       ntohs(src16[0]), ntohs(src16[1]), ntohs(src16[2]),
                       ntohs(src16[3]), ntohs(src16[4]), ntohs(src16[5]),
                       ntohs(src16[6]), ntohs(src16[7]));
        if (full_size < static_cast<int>(size)) {
            return ::strcpy(dst, tmp);
        }
        return nullptr;
    }
    }
    }
    return nullptr;
}

static int
inet_ipv4_pton(const char *src, void *dest)
{
    size_t srcoffset;
    size_t numoffset;
    int value;
    int ndots;
    uint8_t ch;
    char numstr[4];
    uint8_t *ip;

    memset(dest, 0, sizeof(struct in_addr));

    ip = (uint8_t *)dest;
    srcoffset = 0;
    numoffset = 0;
    ndots = 0;

    for (;;) {
        ch = (uint8_t)src[srcoffset++];

        if (ch == '.' || ch == '\0') {
            if (ch == '.' && ndots >= 4) {
                /* Too many dots */

                break;
            }

            if (numoffset < 1) {
                /* Empty numeric string */

                break;
            }

            numstr[numoffset] = '\0';
            numoffset = 0;

            value = atoi(numstr);
            if (value < 0 || value > 255) {
                /* Out of range value */

                break;
            }

            ip[ndots] = (uint8_t)value;

            if (ch == '\0') {
                if (ndots != 3) {
                    /* Not enough dots */

                    break;
                }

                /* Return 1 if the conversion succeeds */

                return 1;
            }

            ndots++;
        } else if (ch >= '0' && ch <= '9') {
            numstr[numoffset++] = ch;
            if (numoffset >= 4) {
                /* Number is too long */

                break;
            }
        } else {
            /* Illegal character */

            break;
        }
    }

    /* Return zero if there is any problem parsing the input */

    return 0;
}

int
inet_ipv6_pton(const char *src, void *dest)
{
    size_t srcoffset;
    size_t numoffset;
    long value;
    int nsep;
    int nrsep;
    uint8_t ch;
    char numstr[5];
    uint8_t ip[sizeof(struct in6_addr)];
    uint8_t rip[sizeof(struct in6_addr)];
    bool rtime;

    memset(dest, 0, sizeof(struct in6_addr));

    srcoffset = 0;
    numoffset = 0;
    nsep = 0;
    nrsep = 0;
    rtime = false;

    for (;;) {
        ch = (uint8_t)src[srcoffset++];

        if (ch == ':' || ch == '\0') {
            if (ch == ':' && (nsep + nrsep) >= 8) {
                /* Too many separators */

                break;
            }

            if (ch != '\0' && numoffset < 1) {
                /* Empty numeric string */

                if (rtime && nrsep > 1) {
                    /* dup simple */

                    break;
                }

                numoffset = 0;
                rtime = true;
                continue;
            }

            numstr[numoffset] = '\0';
            numoffset = 0;

            value = strtol(numstr, NULL, 16);
            if (value < 0 || value > 0xffff) {
                /* Out of range value */

                break;
            }

            if (!rtime) {
                ip[(nsep << 1) + 0] = (uint8_t)((value >> 8) & 0xff);
                ip[(nsep << 1) + 1] = (uint8_t)((value >> 0) & 0xff);
                nsep++;
            } else {
                rip[(nrsep << 1) + 0] = (uint8_t)((value >> 8) & 0xff);
                rip[(nrsep << 1) + 1] = (uint8_t)((value >> 0) & 0xff);
                nrsep++;
            }

            if (ch == '\0' /* || ch == '/' */) {
                if ((nsep <= 1 && nrsep <= 0) || (nsep + nrsep) < 1 ||
                    (nsep + nrsep) > 8) {
                    /* Separator count problem */

                    break;
                }

                if (nsep > 0) {
                    memcpy(dest, &ip[0], nsep << 1);
                }

                if (nrsep > 0) {
                    memcpy((uint8_t *)dest + (16 - (nrsep << 1)), &rip[0],
                           nrsep << 1);
                }

                /* Return 1 if the conversion succeeds */

                return 1;
            }
        } else if ((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') ||
                   (ch >= 'A' && ch <= 'F')) {
            numstr[numoffset++] = ch;
            if (numoffset >= 5) {
                /* Numeric string is too long */

                break;
            }
        } else {
            /* Illegal character */

            break;
        }
    }

    /* Return zero if there is any problem parsing the input */

    return 0;
}

int
inet_pton(int af, const char *__restrict__ src, void *__restrict__ dest)
{
    switch (af) {
    case AF_INET:
        return inet_ipv4_pton(src, dest);
    case AF_INET6:
        return inet_ipv6_pton(src, dest);

    default:
        errno = EAFNOSUPPORT;
        return -1;
    }
}
