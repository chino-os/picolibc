// Copyright (c) SunnyCase. All rights reserved.
// Licensed under the Apache license. See LICENSE file in the project root for full license information.
#include <stdint.h>

#ifdef WIN32
#include <Windows.h>
#include <isa_availability.h>

#ifndef _CRTALLOC
#define _CRTALLOC(x) __attribute__((section(x)))
#endif

extern "C" {
int __isa_available = 0;
int __favor = 0;
int _fltused;

#if defined(_M_AMD64)
size_t __memset_nt_threshold = 0x2000000;
size_t __memset_fast_string_threshold = 0x80000;
#endif

int
atexit(void(__cdecl *)())
{
    return 0;
}

void
abort()
{
    while (1)
        ;
}

void
__chkstk()
{
}

void
guard_check_icall()
{
}

int
_CrtDbgReport(_In_ int _ReportType, _In_opt_z_ char const *_FileName,
              _In_ int _Linenumber, _In_opt_z_ char const *_ModuleName,
              _In_opt_z_ char const *_Format, ...)
{
    return 0;
}

void __cdecl _invalid_parameter(_In_opt_z_ wchar_t const *,
                                _In_opt_z_ wchar_t const *,
                                _In_opt_z_ wchar_t const *, _In_ unsigned int,
                                _In_ uintptr_t)
{
}

void(__cdecl *__guard_check_icall_fptr)() = guard_check_icall;
void(__cdecl *__guard_dispatch_icall_fptr)() = guard_check_icall;

int __cdecl _purecall()
{
    while (1)
        ;
}

void __cdecl __std_terminate()
{
    while (1)
        ;
}

/* Thread Local Storage index for this .EXE or .DLL */

ULONG _tls_index = 0;

/* Special symbols to mark start and end of Thread Local Storage area. */

#pragma data_seg(".tls")

#if defined(_M_X64)
_CRTALLOC(".tls")
#endif /* defined (_M_X64) */
char _tls_start = 0;

#pragma data_seg(".tls$ZZZ")

#if defined(_M_X64)
_CRTALLOC(".tls$ZZZ")
#endif /* defined (_M_X64) */
char _tls_end = 0;

#pragma data_seg()

/* Start section for TLS callback array examined by the OS loader code.
 * If dynamic TLS initialization is used, then a pointer to __dyn_tls_init
 * will be placed in .CRT$XLC by inclusion of tlsdyn.obj.  This will cause
 * the .CRT$XD? array of individual TLS variable initialization callbacks
 * to be walked.
 */

_CRTALLOC(".CRT$XLA") PIMAGE_TLS_CALLBACK __xl_a = 0;

/* NULL terminator for TLS callback array.  This symbol, __xl_z, is never
 * actually referenced anywhere, but it must remain.  The OS loader code
 * walks the TLS callback array until it finds a NULL pointer, so this makes
 * sure the array is properly terminated.
 */

_CRTALLOC(".CRT$XLZ") PIMAGE_TLS_CALLBACK __xl_z = 0;

#ifdef _WIN64

_CRTALLOC(".rdata$T")
extern const IMAGE_TLS_DIRECTORY64 _tls_used = {
    (ULONGLONG)&_tls_start, // start of tls data
    (ULONGLONG)&_tls_end, // end of tls data
    (ULONGLONG)&_tls_index, // address of tls_index
    (ULONGLONG)(&__xl_a + 1), // pointer to call back array
    (ULONG)0, // size of tls zero fill
    {
        (ULONG)0 // characteristics
    }
};

#else /* _WIN64 */

_CRTALLOC(".rdata$T")
extern const IMAGE_TLS_DIRECTORY _tls_used = {
    (ULONG)(ULONG_PTR)&_tls_start, // start of tls data
    (ULONG)(ULONG_PTR)&_tls_end, // end of tls data
    (ULONG)(ULONG_PTR)&_tls_index, // address of tls_index
    (ULONG)(ULONG_PTR)(&__xl_a + 1), // pointer to call back array
    (ULONG)0, // size of tls zero fill
    (ULONG)0 // characteristics
};

#endif /* _WIN64 */

int main(int argc, char *argv[]) __weak_symbol;

int __cdecl mainCRTStartup()
{
    int __arg_c = 0;
    char **__arg_v = nullptr;
    const int exitCode = main(__arg_c, __arg_v);
    return exitCode;
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
                   _In_ LPSTR lpCmdLine, _In_ int nShowCmd) __weak_symbol;

int __cdecl WinMainCRTStartup(void)
{
    return mainCRTStartup();
}

#include <unistd.h>
#include <errno.h>

char heap_[1024 * 1024 * 4];

//static char *brk = heap_;

//void *
//sbrk(ptrdiff_t incr)
//{
//    if (incr < 0) {
//        if ((size_t)(brk - heap_) < (size_t)(-incr)) {
//            errno = ENOMEM;
//            return (void *)-1;
//        }
//    } else {
//        if ((size_t)(heap_ + sizeof(heap_) - brk) < (size_t)incr) {
//            errno = ENOMEM;
//            return (void *)-1;
//        }
//    }
//    void *ret = brk;
//    brk += incr;
//    return ret;
//}
}
#endif
