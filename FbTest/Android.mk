LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	fbTest.cpp


LOCAL_C_INCLUDES := \
	$(LOCAL_PATH) \

LOCAL_SHARED_LIBRARIES := \
	liblog \
	libcutils \
	libutils \

LOCAL_MODULE:= fbTest
LOCAL_MODULE_TAGS := optional


# move to vendor/bin folder
LOCAL_PROPRIETARY_MODULE := true

LOCAL_CFLAGS 		+= -O0 -g
LOCAL_CPPFLAGS 		+= -O0 -g
LOCAL_STRIP_MODULE	:= false
include $(BUILD_EXECUTABLE)

