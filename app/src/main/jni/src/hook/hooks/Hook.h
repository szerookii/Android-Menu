#pragma once

#include "../../../include/dobby.h"
#include "src/utils/Utils.h"

class Hook {
public:
    virtual void install() {};

    bool hookPattern(const char* libName, const char* pattern, void* pDetour, void** ppOriginal) {
        do {
            sleep(1);
        } while (!Utils::isLibraryLoaded(libName));

        uintptr_t addr = Utils::findPatternInModule(libName, pattern);

        if (addr) {
            if (Utils::hook((void*)addr, pDetour, ppOriginal) != 0) {
                LOGI("[Hook] Successfully hooked function");
                return true;
            } else {
                LOGE("[Hook] Failed to hook!");
                return false;
            }
        }

        return false;
    }

    bool hookSymbol(const char* libName, const char* symbolName, void* pDetour, void** ppOriginal) {
        do {
            sleep(1);
        } while (!Utils::isLibraryLoaded(libName));

        void* addr = DobbySymbolResolver(libName, symbolName);

        if (addr != nullptr) {
            return this->hookAddress((void*)addr, pDetour, ppOriginal);
        }

        return false;
    }

    bool hookAddress(void* addr, void* pDetour, void** ppOriginal) {
        if (Utils::hook((void*)addr, pDetour, ppOriginal) != 0) {
            LOGI("[Hook] Successfully hooked function");
            return true;
        } else {
            LOGE("[Hook] Failed to hook!");
            return false;
        }
    }
};