#pragma once

#include <jni.h>

#include "src/utils/Utils.h"
#include "Hook.h"

#include "libraries/imgui/imgui.h"
#include "libraries/imgui/imgui_internal.h"
#include "libraries/imgui/backends/imgui_impl_android.h"

class InitializeMotionEvent : public Hook {
public:
    void install();
};

void(*oinitializeMotionEvent)(void*, void*, void*);
void hinitializeMotionEvent(void* _this, void* ex_ab, void* ex_ac) {
    oinitializeMotionEvent(_this, ex_ab, ex_ac);
    ImGui_ImplAndroid_HandleInputEvent((AInputEvent*)_this);
}

void InitializeMotionEvent::install() {
    void* sym_input = DobbySymbolResolver(("/system/lib/libinput.so"), ("_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE"));

    if (NULL != sym_input){
        DobbyHook((void*)sym_input, (void*)hinitializeMotionEvent, (void **)&oinitializeMotionEvent);
    }
}
