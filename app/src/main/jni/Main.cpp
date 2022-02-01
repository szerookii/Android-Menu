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

#include "Utils/Utils.h"
#include "Menu.h"

#define targetLibName OBFUSCATE("libil2cpp.so")

// Variables
JNIEnv *globalEnv;

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

//JNI calls
extern "C" {
JNIEXPORT jobjectArray
JNICALL
Java_dev_seyz_modmenu_FloatingModMenuService_getFeatureList(JNIEnv *env, jobject context) {
    jobjectArray ret;

    MakeToast(env, context, OBFUSCATE("Modded by Seyz"), Toast::LENGTH_LONG);

    const char *features[] = {
            OBFUSCATE("Toggle_Toggle"),
    };

    int Total_Feature = (sizeof features / sizeof features[0]);
    ret = (jobjectArray)
            env->NewObjectArray(Total_Feature, env->FindClass(OBFUSCATE("java/lang/String")),
                                env->NewStringUTF(""));

    for (int i = 0; i < Total_Feature; i++)
        env->SetObjectArrayElement(ret, i, env->NewStringUTF(features[i]));

    pthread_t ptid;
    pthread_create(&ptid, nullptr, antiLeech, nullptr);

    return (ret);
}

JNIEXPORT void JNICALL
Java_dev_seyz_modmenu_Preferences_Changes(JNIEnv *env, jclass clazz, jobject obj,
                                          jint featNum, jstring featName, jint value,
                                          jboolean boolean, jstring str) {
    LOGD(OBFUSCATE("Name: %d - %s | Value: = %d | Bool: = %d | Text: = %s"), featNum,
         env->GetStringUTFChars(featName, nullptr), value,
         boolean, str != nullptr ? env->GetStringUTFChars(str, nullptr) : "");

    switch (featNum) {
        case 0:
            break;
    }
}
}

__attribute__((constructor))
void lib_main() {
    pthread_t ptid;
    pthread_create(&ptid, nullptr, hack_thread, nullptr);
}

JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    vm->GetEnv((void **) &globalEnv, JNI_VERSION_1_6);
    return JNI_VERSION_1_6;
}
