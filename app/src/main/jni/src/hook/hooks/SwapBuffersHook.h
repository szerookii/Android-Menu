#pragma once

#include "src/utils/Utils.h"
#include "Hook.h"

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include "libraries/imgui/imgui.h"
#include "libraries/imgui/imgui_internal.h"
#include "libraries/imgui/backends/imgui_impl_opengl3.h"

int glHeight, glWidth;
bool setup = false;

class SwapBuffersHook : public Hook {
public:
    void install();
};

void setup_imgui() {
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    io.DisplaySize = ImVec2((float)glWidth, (float)glHeight);

    ImGui::StyleColorsDark();

    ImGui_ImplOpenGL3_Init("#version 100");

    ImFontConfig font_cfg;
    font_cfg.SizePixels = 22.0f;
    io.Fonts->AddFontDefault(&font_cfg);

    ImGui::GetStyle().ScaleAllSizes(3.0f);
}

EGLBoolean (*oEglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
EGLBoolean hEglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
    eglQuerySurface(dpy, surface, EGL_WIDTH, &glWidth);
    eglQuerySurface(dpy, surface, EGL_HEIGHT, &glHeight);

    if (!setup) {
        setup_imgui();
        setup = true;
    }

    ImGuiIO &io = ImGui::GetIO();

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

    return oEglSwapBuffers(dpy, surface);
}

void SwapBuffersHook::install() {
    this->hookSymbol("libEGL.so", "eglSwapBuffers",  (void*)hEglSwapBuffers, (void**)&oEglSwapBuffers);
}