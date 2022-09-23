#pragma once

#include <jni.h>

#include "src/utils/Utils.h"
#include "Hook.h"

#include "libraries/imgui/imgui.h"
#include "libraries/imgui/imgui_internal.h"
#include "libraries/imgui/backends/imgui_impl_android.h"

class NativeInjectEventHook : public Hook {
public:
    void install();
};

void(*oNativeInjectEvent)(void*, void*, void*);
void hNativeInjectEvent(void* _this, void* ex_ab, void* ex_ac) {
    oNativeInjectEvent(_this, ex_ab, ex_ac);
    ImGui_ImplAndroid_HandleInputEvent((AInputEvent*)_this);
}

void NativeInjectEventHook::install() {
    //this->hookSymbol("/system/lib/libinput.so", "_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE",  (void*)oNativeInjectEvent, (void**)&oNativeInjectEvent); // Subway Surfers sig, update it urself
}
