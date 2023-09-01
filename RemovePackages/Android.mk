LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := RemovePackages
LOCAL_MODULE_CLASS := APPS
LOCAL_MODULE_TAGS := optional
LOCAL_OVERRIDES_PACKAGES := \
   CarrierServices \
   Dialer \
   GoogleDialer \
   Stk \
   TeleService \
   Telecom \
   com.android.smspush \
   TelephonyProvider \
   TelephonyManager\
   com.google.android.apps.messaging \
   com.google.android.contacts \
   com.android.providers.contacts \
   com.android.server.telecom \
   com.android.mms.service \
   GoogleDialer \
   GoogleContacts \
   com.google.android.dialer.support \
   com.android.phone \
   com.android.telephony.TelephonyManager\
   com.android.phone.common \
   com.android.providers.telephony \
   DevicePersonalizationPrebuiltPixel2021 \
   messaging

LOCAL_UNINSTALLABLE_MODULE := true
LOCAL_CERTIFICATE := PRESIGNED
LOCAL_SRC_FILES := /dev/null
include $(BUILD_PREBUILT)
