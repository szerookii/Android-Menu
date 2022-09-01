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

int(*oNativeInjectEvent)(JNIEnv*, jobject, jobject);
int hNativeInjectEvent(JNIEnv* env, jobject _this, jobject inputEvent) {
    /*
    jclass motion_event = env->FindClass(("android/view/MotionEvent"));

    if(!motion_event){
        LOGI("Can't find MotionEvent!");
        return oNativeInjectEvent(env, _this, inputEvent);
    }

    if(env->IsInstanceOf(inputEvent, motion_event)){
        jmethodID get_action = env->GetMethodID(motion_event, ("getActionMasked"), ("()I"));
        jmethodID get_x = env->GetMethodID(motion_event, ("getX"), ("()F"));
        jmethodID get_y = env->GetMethodID(motion_event, ("getY"), ("()F"));

        ImGui_ImplAndroid_HandleInputEvent(env->CallFloatMethod(inputEvent, get_x), env->CallFloatMethod(inputEvent, get_y), env->CallIntMethod(inputEvent, get_action));

        ImGuiIO &io = ImGui::GetIO();
        if (io.WantCaptureMouse)
            return true;
    }

    jclass key_event = env->FindClass("android/view/KeyEvent");

    if(!key_event){
        LOGI("Can't find KeyEvent!");
        return oNativeInjectEvent(env, _this, inputEvent);
    }

    if(env->IsInstanceOf(inputEvent, key_event)){
        jmethodID get_unicode_char = env->GetMethodID(key_event, ("getUnicodeChar"), ("()I"));
        jmethodID get_meta_state = env->GetMethodID(key_event, ("getMetaState"), ("()I"));

        ImGuiIO &io = ImGui::GetIO();
        io.AddInputCharacter(env->CallIntMethod(inputEvent, get_unicode_char,env->CallIntMethod(inputEvent, get_meta_state)));
    }
    */

    return oNativeInjectEvent(env, _this, inputEvent);
}

void NativeInjectEventHook::install() {
    this->hookPattern("libunity.so", "2D E9 F0 41 84 B0 16 46 DF F7 FE FF",  (void*)oNativeInjectEvent, (void**)&oNativeInjectEvent); // Subway Surfers sig, update it urself
}
