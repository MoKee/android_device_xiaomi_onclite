/*
   Copyright (c) 2016, The CyanogenMod Project
   Copyright (C) 2019-2020 The LineageOS Project
   Copyright (C) 2020 The MoKee Open Source Project

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.
   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include <sys/sysinfo.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "vendor_init.h"
#include "property_service.h"
#include "android/log.h"
#include <android-base/properties.h>

#include <string>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

char const *heapstartsize;
char const *heapgrowthlimit;
char const *heapsize;
char const *heapminfree;
char const *heapmaxfree;
char const *heaptargetutilization;

using android::init::property_override;
using std::string;

void property_override(char const prop[], char const value[], bool add = true)
{
    auto pi = (prop_info *) __system_property_find(prop);

    if (pi != nullptr) {
        __system_property_update(pi, value, strlen(value));
    } else if (add) {
        __system_property_add(prop, strlen(prop), value, strlen(value));
    }
}

void load_props(string device, string model) {
    string RO_PROP_SOURCES[] = { "", "odm.", "system.", "vendor." };

    for (const string &source : RO_PROP_SOURCES) {
        property_override(string("ro.product.") + source + string("name"), device);
        property_override(string("ro.product.") + source + string("device"), device);
        property_override(string("ro.product.") + source + string("model"), model);
    }
}

void check_device()
{
    struct sysinfo sys;

    sysinfo(&sys);

    if (sys.totalram > 3072ull * 1024 * 1024) {
        // from - phone-xxhdpi-4096-dalvik-heap.mk
        heapstartsize = "8m";
        heapgrowthlimit = "256m";
        heapsize = "512m";
        heaptargetutilization = "0.6";
        heapminfree = "8m";
        heapmaxfree = "16m";
    } else {
        // from - phone-xhdpi-2048-dalvik-heap.mk
        heapstartsize = "8m";
        heapgrowthlimit = "192m";
        heapsize = "512m";
        heaptargetutilization = "0.75";
        heapminfree = "512k";
        heapmaxfree = "8m";
    }
}

/* From Magisk@jni/magiskhide/hide_utils.c */
static const char *snet_prop_key[] = {
	"ro.boot.vbmeta.device_state",
	"ro.boot.verifiedbootstate",
	"ro.boot.flash.locked",
	"ro.boot.selinux",
	"ro.boot.veritymode",
	"ro.boot.warranty_bit",
	"ro.warranty_bit",
	"ro.debuggable",
	"ro.secure",
	"ro.build.type",
	"ro.build.tags",
	"ro.build.selinux",
	NULL
};

static const char *snet_prop_value[] = {
	"locked",
	"green",
	"1",
	"enforcing",
	"enforcing",
	"0",
	"0",
	"0",
	"1",
	"user",
	"release-keys",
	"1",
	NULL
};

void set_avoid_gfxaccel_config() {
    struct sysinfo sys;
    sysinfo(&sys);

    if (sys.totalram <= 2048ull * 1024 * 1024) {
        // Reduce memory footprint
        property_override("ro.config.avoid_gfx_accel", "true");
    }
}

static void workaround_snet_properties() {

	// Hide all sensitive props
	for (int i = 0; snet_prop_key[i]; ++i) {
		property_override(snet_prop_key[i], snet_prop_value[i]);
	}
}

void vendor_load_properties()
{
    check_device();

    property_override("dalvik.vm.heapstartsize", heapstartsize);
    property_override("dalvik.vm.heapgrowthlimit", heapgrowthlimit);
    property_override("dalvik.vm.heapsize", heapsize);
    property_override("dalvik.vm.heaptargetutilization", heaptargetutilization);
    property_override("dalvik.vm.heapminfree", heapminfree);
    property_override("dalvik.vm.heapmaxfree", heapmaxfree);
    
    property_override("ro.control_privapp_permissions", "log");

    string boot_cert = android::base::GetProperty("ro.boot.product.cert", "");

    if (boot_cert == "M1810F6LG" || boot_cert == "M1810F6LH" || boot_cert == "M1810F6LI"
            || boot_cert == "M1810F6LE" || boot_cert == "M1810F6LT" || boot_cert == "M1810F6LC")
        load_props("onclite", "Redmi 7");
    else
        load_props("onc", "Redmi Y3");
	
	    // Workaround SafetyNet
    workaround_snet_properties();
}
