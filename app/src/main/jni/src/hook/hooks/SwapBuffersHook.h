#pragma once

#include "src/utils/Utils.h"
#include "Hook.h"

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include "libraries/imgui/imgui.h"
#include "libraries/imgui/imgui_internal.h"
#include "libraries/imgui/backends/imgui_impl_opengl3.h"
#include "libraries/imgui/backends/imgui_impl_android.h"

class SwapBuffersHook : public Hook {
public:
    void install();
};

bool setup = false;

void setup_imgui(EGLint w, EGLint h) {
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    io.IniFilename = NULL;
    io.DisplaySize = ImVec2((float)w, (float)h);

    ImGui_ImplAndroid_Init(nullptr);
    ImGui_ImplOpenGL3_Init();

    ImFontConfig font_cfg;
    font_cfg.SizePixels = 20.0f;
    io.Fonts->AddFontDefault(&font_cfg);

    ImGui::GetStyle().ScaleAllSizes(2.5f);
}

EGLBoolean (*oEglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
EGLBoolean hEglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
    EGLint w, h;
    eglQuerySurface(dpy, surface, EGL_WIDTH, &w);
    eglQuerySurface(dpy, surface, EGL_HEIGHT, &h);

    if (!setup) {
        setup_imgui(w, h);
        setup = true;
    }

    ImGuiIO &io = ImGui::GetIO();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame(w, h);
    ImGui::NewFrame();

    ImGui::Begin("Hello, world!");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    ImGui::EndFrame();

    return oEglSwapBuffers(dpy, surface);
}

void SwapBuffersHook::install() {
    this->hookSymbol("libEGL.so", "eglSwapBuffers",  (void*)hEglSwapBuffers, (void**)&oEglSwapBuffers);
}