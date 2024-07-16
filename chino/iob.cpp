// Copyright (c) SunnyCase. All rights reserved.
// Licensed under the Apache license. See LICENSE file in the project root for full license information.
#include <stdio.h>
#include <sys/cdefs.h>
#include <chino/os/kernel/ke_services.h>

using namespace chino;
using namespace chino::os;
using namespace chino::os::kernel;

static int
stdio_putc(char c, FILE *file)
{
    ke_services().write(0, &c, 1);
    return (unsigned char)c;
}

static int
stdio_getc(FILE *file)
{
    char c;
    auto ret = ke_services().read(0, &c, 1);
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
