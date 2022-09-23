#pragma once

#include <jni.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <string>
#include <cstdlib>
#include <sys/mman.h>

#include "Unity/Unity.h"
#include "Logger.h"
#include "xorstr.h"

struct LibraryInfo {
    void* start_address;
    void* end_address;
    intptr_t size;
    std::string name;
};

class Utils {
public:
    static LibraryInfo findLibrary(const char *module_name){
        LibraryInfo library_info{};
        char line[512], mod_name[64];

        FILE *fp = fopen("/proc/self/maps", "rt");
        if (fp != NULL)
        {
            while (fgets(line, sizeof(line), fp))
            {
                if (strstr(line, module_name))
                {
                    sscanf(line, "%llx-%llx %*s %*ld %*s %*d %s",
                           (long long unsigned *)&library_info.start_address,
                           (long long unsigned *)&library_info.end_address, mod_name);

                    library_info.size = (uintptr_t)library_info.end_address - (uintptr_t)library_info.start_address;

                    if (library_info.name.empty())
                        library_info.name = std::string(mod_name);

                    break;
                }
            }
            fclose(fp);
        }
        return library_info;
    }

    static uintptr_t getAbsoluteAddress(const char *libraryName, uintptr_t relativeAddr) {
        return reinterpret_cast<uintptr_t>(findLibrary(libraryName).start_address) + relativeAddr;
    }

    static bool isLibraryLoaded(const char *libraryName) {
        char line[512] = {0};
        FILE *fp = fopen("/proc/self/maps", "rt");

        if (fp != NULL) {
            while (fgets(line, sizeof(line), fp)) {
                if (strstr(line, libraryName)){
                    return true;
                }
            }

            fclose(fp);
        }

        return false;
    }

    static uintptr_t string2Offset(const char *c) {
        return strtoul(c, nullptr, 16);
    }

    static uintptr_t findPattern(uint8_t* start, const size_t length, const char* pattern) {
        const char* pat = pattern;
        uint8_t* first_match = 0;

        for (auto current_byte = start; current_byte < (start + length); ++current_byte) {
            if (*pat == '?' || *current_byte == strtoul(pat, NULL, 16)) {
                if (!first_match)
                    first_match = current_byte;

                if (!pat[2])
                    return (uintptr_t)first_match;

                pat += *(uint16_t*)pat == 16191 || *pat != '?' ? 3 : 2;
            }
            else if (first_match) {
                current_byte = first_match;
                pat = pattern;
                first_match = 0;
            }
        } return 0;
    }

    static uintptr_t findPatternInModule(const char* lib_name, const char* pattern) {
        LibraryInfo lib_info = findLibrary(lib_name);

        return findPattern((uint8_t*)lib_info.start_address, lib_info.size, pattern);
    }

    static bool hook(void *orig_fcn, void* new_fcn, void **orig_fcn_ptr)
    {
#if defined(__aarch64__)
        auto pagesize = sysconf(_SC_PAGE_SIZE);
        auto addr = ((uintptr_t)orig_fcn) & (-pagesize);
        mprotect((void*)addr, pagesize, PROT_READ | PROT_WRITE | PROT_EXEC);
        return DobbyHook((void*)orig_fcn, (void*)new_fcn, (void**)&orig_fcn_ptr);
#else
        return DobbyHook(orig_fcn, new_fcn, orig_fcn_ptr);
#endif
    }
};