#pragma once

#include "src/utils/Utils.h"
#include "Hook.h"

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include "libraries/imgui/imgui.h"
#include "libraries/imgui/imgui_internal.h"
#include "libraries/imgui/backends/imgui_impl_opengl3.h"
#include "libraries/imgui/backends/imgui_impl_android.h"
#include "libraries/ByNameModding/BNM.hpp"

int(*oScreen_get_height)();
int(*oScreen_get_width)();

class SwapBuffersHook : public Hook {
public:
    void install();
};

bool setup = false;

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
    ImGui_ImplAndroid_Init(nullptr);

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

    int height = oScreen_get_height();
    int width = oScreen_get_width();

    ImGuiIO &io = ImGui::GetIO();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame(width, height);
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

    return oEglSwapBuffers(dpy, surface);
}

void SwapBuffersHook::install() {
    this->hookSymbol("libEGL.so", "eglSwapBuffers",  (void*)hEglSwapBuffers, (void**)&oEglSwapBuffers);
}