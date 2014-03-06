LOCAL_PATH := $(call my-dir)
OPENCV_SDK := $(OPENCV_ROOT)/sdk/native

OPENCV_LIB_TYPE := STATIC
OPENCV_INSTALL_MODULES := on

# Thinning morphology
include $(CLEAR_VARS)
include $(OPENCV_SDK)/jni/OpenCV.mk
LOCAL_MODULE := thinning-morph  
LOCAL_CPP_FEATURES := exceptions rtti
#LOCAL_CFLAGS += -S
LOCAL_C_INCLUDES := $(OPENCV_SDK)/jni/include include
LOCAL_SRC_FILES := $(addprefix src/, main.cpp thinning_morph.cpp perf.cpp)
include $(BUILD_EXECUTABLE)
