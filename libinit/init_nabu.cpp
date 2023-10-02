/*
 * Copyright (C) 2020 LineageOS Project
 * Copyright (C) 2023 Yogesh Nimangre
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <vector>
#include <string>
#include <fstream>

#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

#include <android-base/properties.h>
#include <android-base/logging.h>

#include "property_service.h"
#include "vendor_init.h"

#include <fs_mgr_dm_linear.h>

struct device_variant
{
  const std::string model;
  const std::string device_name;
  const std::string build_fingerprint;
  const std::string build_description;
};

using dv = device_variant;

void property_override(char const prop[], char const value[], bool add = true)
{
  prop_info *pi;
  pi = (prop_info *)__system_property_find(prop);
  if (pi)
    __system_property_update(pi, value, strlen(value));
  else if (add)
    __system_property_add(prop, strlen(prop), value, strlen(value));
}

void set_device_properties(const dv &variant)
{
  const std::array<std::string, 7> props_order = {
      std::string(""), "bootimage.", "odm.", "product.",
      "system.", "system_ext.", "vendor."};

  const auto set_ro_build_prop = [](
                                     const std::string &source, const std::string &prop,
                                     const std::string &value, bool product = true)
  {
    property_override(
        std::string((product) ? std::string("ro.product." + source + prop)
                              : std::string("ro." + source + prop))
            .c_str(),
        value.c_str());
  };

  for (auto &props : props_order)
  {
    set_ro_build_prop(props, "fingerprint", variant.build_fingerprint, false);
    set_ro_build_prop(props, "model", variant.model);
    set_ro_build_prop(props, "name", variant.device_name);
    // common for all variants is the market name.
    set_ro_build_prop(props, "marketname", "Xiaomi Pad 5");
  }

  property_override("ro.build.description", variant.build_description.c_str());
  property_override("bluetooth.device.default_name", variant.model.c_str());
  property_override("vendor.usb.product_string", variant.model.c_str());
}

static const char *build_keys_props[] =
{
    "ro.build.tags",
    "ro.odm.build.tags",
    "ro.product.build.tags",
    "ro.system.build.tags",
    "ro.system_ext.build.tags",
    "ro.vendor.build.tags",
    nullptr};

void witch_nabu()
{

  dv variants[2] = {
      {"21051182G", "nabu_global", "Xiaomi/nabu_global/nabu:12/RKQ1.200826.002/V13.1.4.0.SKXMIXM:user/release-keys",
       "nabu_global-user 12 SKQ1.220303.001 V13.1.4.0.SKXMIXM release-keys"},
      {"21051182C", "nabu", "Xiaomi/nabu/nabu:13/RKQ1.200826.002/V14.0.4.0.TKXCNXM:user/release-keys",
       "nabu-user 13 TKQ1.221013.002 V14.0.4.0.TKXCNXM release-keys"},
  };

  set_device_properties(((::android::base::GetProperty("ro.boot.hwc", std::string("GLOBAL")) == "GLOBAL")) ? variants[0] : variants[1]);

  property_override("ro.boot.hardware.revision", ::android::base::GetProperty("ro.boot.hwversion", "").c_str());
}

void vendor_load_properties() {
  witch_nabu();

    /* Spoof Build keys */
    for (int i = 0; build_keys_props[i]; ++i)
    {
        property_override(build_keys_props[i], "release-keys");
    }

#ifdef __ANDROID_RECOVERY__
  std::string buildtype = ::android::base::GetProperty("ro.build.type", "userdebug");
  if (buildtype != "user") {
    property_override("ro.debuggable", "1");
    property_override("ro.adb.secure.recovery", "0");
  }
#endif
}
