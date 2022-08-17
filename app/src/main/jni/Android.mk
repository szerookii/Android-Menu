LOCAL_PATH := $(call my-dir)
MAIN_LOCAL_PATH := $(call my-dir)

# we include libdobby too
LOCAL_MODULE := libdobby
LOCAL_SRC_FILES := $(LOCAL_PATH)/libraries/$(TARGET_ARCH_ABI)/libdobby.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := ModMenu

# Code optimization
LOCAL_CFLAGS := -w -s -Wno-error=format-security -fvisibility=hidden -fpermissive -fexceptions
LOCAL_CPPFLAGS := -w -s -Wno-error=format-security -fvisibility=hidden -Werror -std=c++17
LOCAL_CPPFLAGS += -Wno-error=c++11-narrowing -fpermissive -Wall -fexceptions
LOCAL_LDFLAGS += -Wl,--gc-sections,--strip-all,-llog
LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv2
LOCAL_ARM_MODE := arm

LOCAL_C_INCLUDES += $(MAIN_LOCAL_PATH)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/libraries/imgui
LOCAL_C_INCLUDES += $(LOCAL_PATH)/libraries/backends

LOCAL_STATIC_LIBRARIES := libdobby

# Here you add the cpp file
LOCAL_SRC_FILES := Main.cpp \
	utils/Unity/MonoString.cpp \
	libraries/imgui/imgui.cpp \
	libraries/imgui/imgui_draw.cpp \
	libraries/imgui/imgui_demo.cpp \
	libraries/imgui/imgui_widgets.cpp \
	libraries/imgui/imgui_tables.cpp \
	libraries/imgui/backends/imgui_impl_opengl3.cpp \
	libraries/imgui/backends/imgui_impl_android.cpp \

include $(BUILD_SHARED_LIBRARY)
