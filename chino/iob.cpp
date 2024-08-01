// Copyright (c) SunnyCase. All rights reserved.
// Licensed under the Apache license. See LICENSE file in the project root for full license information.
#include <stdio.h>
#include <sys/cdefs.h>
#include <chino/os/ke_services.h>

using namespace chino;
using namespace chino::os;

static int
stdio_putc(char c, FILE *file)
{
    if (ke_services().write(STDOUT_FILENO, &c, 1) != -1) {
        return (unsigned char)c;
    } else {
        errno = ke_services().errno_();
        return -1;
    }
}

static int
stdio_getc(FILE *file)
{
    char c;
    auto ret = ke_services().read(STDIN_FILENO, &c, 1);
    return ret > 0 ? c : ret;
}

static int
stdio_flush(FILE *file)
{
    (void)file;
    return 0;
}

static FILE __stdio =
    FDEV_SETUP_STREAM(stdio_putc, stdio_getc, stdio_flush, _FDEV_SETUP_RW);

#ifdef __strong_reference
#define STDIO_ALIAS(x) __strong_reference(stdin, x);
#else
#define STDIO_ALIAS(x) FILE *const x = &__stdio;
#endif

FILE *const stdin = &__stdio;
STDIO_ALIAS(stdout);
STDIO_ALIAS(stderr);
