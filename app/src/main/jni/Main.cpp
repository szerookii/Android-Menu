#include <list>
#include <vector>
#include <map>
#include <cstring>
#include <pthread.h>
#include <cstring>
#include <jni.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <dlfcn.h>

// imgui needed files
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include "libraries/imgui/imgui.h"
#include "libraries/imgui/imgui_internal.h"
#include "libraries/imgui/backends/imgui_impl_opengl3.h"
#include "libraries/imgui/backends/imgui_impl_android.h"

#include "utils/Utils.h"

#define targetLibName OBFUSCATE("libil2cpp.so")

// Variables
JNIEnv *globalEnv;
bool setup;
int glHeight, glWidth;

// Hooks

//

void* hack_thread(void*) {
    LOGI(OBFUSCATE("pthread created"));

    do {
        sleep(1);
    } while (!isLibraryLoaded(targetLibName));

    LOGI(OBFUSCATE("%s has been loaded"), (const char *) targetLibName);

    //hook((void *) getAbsoluteAddress(targetLibName, string2Offset("0x84C848")), (void *) hWalletModel_GetCurrency, (void **) &oWalletModel_GetCurrency);

    LOGI(OBFUSCATE("Initialized"));

    return nullptr;
}

// ImGui stuff
void setup_imgui() {
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    io.DisplaySize = ImVec2((float)glWidth, (float)glHeight);

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

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    // add our shit here
    ImGui::Begin("Niggerz");

    ImGui::Text("some useless text testing");

    ImGui::End();

    // Rendering
    ImGui::EndFrame();
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


    return oEglSwapBuffers(dpy, surface);
}

void* imgui_thread(void *) {
    auto addr = (uintptr_t)dlsym(RTLD_NEXT, "eglSwapBuffers");
    hook((void *)addr, (void *)hEglSwapBuffers, (void **)&oEglSwapBuffers);
    pthread_exit(nullptr);
}

__attribute__((constructor))
void lib_main() {
    pthread_t ptid;
    pthread_create(&ptid, NULL, imgui_thread, NULL);

    pthread_t modmenu;
    pthread_create(&modmenu, nullptr, hack_thread, nullptr);
}

JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    vm->GetEnv((void **) &globalEnv, JNI_VERSION_1_6);

    void *sym_input = DobbySymbolResolver(("/system/lib/libinput.so"), ("_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE"));
    if (NULL != sym_input) {
        // TODo : impl input from user lmao
    }

    return JNI_VERSION_1_6;
}
