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

#include "src/hook/HookManager.h"
#include "src/utils/Utils.h"

void* init(void*) {
    LOGI("Injected, initializing...");

    hookMgr.init();

    return nullptr;
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    pthread_t ptid;
    pthread_create(&ptid, nullptr, init, nullptr);

    return JNI_VERSION_1_6;
}
