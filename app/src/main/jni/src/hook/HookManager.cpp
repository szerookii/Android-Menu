#include "HookManager.h"

#include "../utils/Logger.h"

#include "hooks/SwapBuffersHook.h"
#include "hooks/NativeInjectEventHook.h"

HookManager hookMgr;

void HookManager::init() {
    LOGI("[HookManager] Initializing...");

    hooks.push_back(new SwapBuffersHook());
    hooks.push_back(new NativeInjectEventHook());

    for (auto & hook : hooks) {
        hook->install();
    }

    LOGI("[HookManager] Sucessfully installed hooks!");
}
