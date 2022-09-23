#pragma once

#include "src/utils/Utils.h"
#include "Hook.h"

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include "libraries/imgui/imgui.h"
#include "libraries/imgui/imgui_internal.h"
#include "libraries/imgui/backends/imgui_impl_opengl3.h"
#include "libraries/ByNameModding/BNM.hpp"

int(*oScreen_get_height)();
int(*oScreen_get_width)();
int(*oInput_get_touchCount)();
UnityEngine_Touch_Fields(*oInput_GetTouch)(int index);


class SwapBuffersHook : public Hook {
public:
    void install();
};

bool setup = false;
bool clearMouse = true;

void setup_imgui() {
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    InitResolveFunc(oScreen_get_height, OBFUSCATE_BNM("UnityEngine.Screen::get_height"));
    InitResolveFunc(oScreen_get_width, OBFUSCATE_BNM("UnityEngine.Screen::get_width"));

    int height = oScreen_get_height();
    int width = oScreen_get_width();

    io.DisplaySize = ImVec2((float)width, (float)height);

    ImGui::StyleColorsDark();

    ImGui_ImplOpenGL3_Init("#version 100");

    ImFontConfig font_cfg;
    font_cfg.SizePixels = 22.0f;
    io.Fonts->AddFontDefault(&font_cfg);

    ImGui::GetStyle().ScaleAllSizes(3.0f);
}

EGLBoolean (*oEglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
EGLBoolean hEglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
    if (!setup) {
        setup_imgui();
        setup = true;
    }

    ImGuiIO &io = ImGui::GetIO();

    InitResolveFunc(oInput_get_touchCount, OBFUSCATE_BNM("UnityEngine.Input::get_touchCount"));
    int touchCount = oInput_get_touchCount();

    if (touchCount > 0) {
        InitResolveFunc(oInput_GetTouch, OBFUSCATE_BNM("UnityEngine.Input::GetTouch"));
        UnityEngine_Touch_Fields touch = oInput_GetTouch(0);
        float reverseY = io.DisplaySize.y - touch.m_Position.fields.y;

        switch (touch.m_Phase) {
            case TouchPhase::Began:
            case TouchPhase::Stationary:
                io.MousePos = ImVec2(touch.m_Position.fields.x, reverseY);
                io.MouseDown[0] = true;
                break;

            case TouchPhase::Ended:
            case TouchPhase::Canceled:
                io.MouseDown[0] = false;
                clearMouse = true;
                break;

            case TouchPhase::Moved:
                io.MousePos = ImVec2(touch.m_Position.fields.x, reverseY);
                break;

            default:
                break;
        }
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    /* MENU */

    ImGui::Begin("Unity");

    ImGui::Text("Just a test :)");

    ImGui::End();

    /* END MENU */

    ImGui::EndFrame();
    ImGui::Render();

    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (clearMouse) {
        io.MousePos = ImVec2(-1, -1);
        clearMouse = false;
    }

    return oEglSwapBuffers(dpy, surface);
}

void SwapBuffersHook::install() {
    this->hookSymbol("libEGL.so", "eglSwapBuffers",  (void*)hEglSwapBuffers, (void**)&oEglSwapBuffers);
}