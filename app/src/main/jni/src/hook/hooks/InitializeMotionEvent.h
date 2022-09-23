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

void(*oinitializeMotionEvent)(AInputEvent*, void*, void*);
void hinitializeMotionEvent(AInputEvent* _this, void* ex_ab, void* ex_ac) {
    oinitializeMotionEvent(_this, ex_ab, ex_ac);
    ImGui_ImplAndroid_HandleInputEvent(_this);
}

void InitializeMotionEvent::install() {
    this->hookSymbol("/system/lib/libinput.so", "_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE", (void*)hinitializeMotionEvent, (void**)&oinitializeMotionEvent);
}
