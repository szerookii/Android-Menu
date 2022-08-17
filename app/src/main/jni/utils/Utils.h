#ifndef UTILS
#define UTILS

#include <jni.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <string>
#include <cstdlib>
#include <sys/mman.h>

#include <include/dobby.h>

#include "Logger.h"
#include "Resolver.h"
#include "Unity/Unity.h"

typedef unsigned long DWORD;
void hook(void *orig_fcn, void* new_fcn, void **orig_fcn_ptr);

static uintptr_t libBase;

bool isGameLibLoaded = false;

DWORD findLibrary(const char *library) {
    char filename[0xFF] = {0},
            buffer[1024] = {0};
    FILE *fp = NULL;
    DWORD address = 0;

    sprintf(filename, OBFUSCATE("/proc/self/maps"));

    fp = fopen(filename, OBFUSCATE("rt"));
    if (fp == NULL) {
        perror(OBFUSCATE("fopen"));
        goto done;
    }

    while (fgets(buffer, sizeof(buffer), fp)) {
        if (strstr(buffer, library)) {
            address = (DWORD) strtoul(buffer, NULL, 16);
            goto done;
        }
    }

    done:

    if (fp) {
        fclose(fp);
    }

    return address;
}

DWORD getAbsoluteAddress(const char *libraryName, DWORD relativeAddr) {
    libBase = findLibrary(libraryName);
    if (libBase == 0)
        return 0;
    return (reinterpret_cast<DWORD>(libBase + relativeAddr));
}

bool isLibraryLoaded(const char *libraryName) {
    char line[512] = {0};
    FILE *fp = fopen(OBFUSCATE("/proc/self/maps"), OBFUSCATE("rt"));
    if (fp != NULL) {
        while (fgets(line, sizeof(line), fp)) {
            std::string a = line;
            if (strstr(line, libraryName)) {
                isGameLibLoaded = true;
                return true;
            }
        }
        fclose(fp);
    }
    return false;
}

uintptr_t string2Offset(const char *c) {
    int base = 16;

    if (sizeof(uintptr_t) == sizeof(unsigned long)) {
        return strtoul(c, nullptr, base);
    }

    return strtoull(c, nullptr, base);
}

void hook(void *orig_fcn, void* new_fcn, void **orig_fcn_ptr)
{
#if defined(__aarch64__)
    auto pagesize = sysconf(_SC_PAGE_SIZE);
    auto addr = ((uintptr_t)orig_fcn) & (-pagesize);
    mprotect((void*)addr, pagesize, PROT_READ | PROT_WRITE | PROT_EXEC);
    DobbyHook((void*)orig_fcn, (void*)new_fcn, (void**)&orig_fcn_ptr);
#else
    DobbyHook(orig_fcn, new_fcn, orig_fcn_ptr);
#endif
}
#endif
