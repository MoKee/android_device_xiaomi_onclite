/*
   Copyright (c) 2016, The CyanogenMod Project
   Copyright (C) 2019 The LineageOS Project.
   Copyright (C) 2020-2021 The MoKee Open Source Project

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


#include <fcntl.h>
#include <stdlib.h>
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

using android::init::property_set;
using std::string;

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
		// set different Davlik heap properties for 3 GB
    if (sys.totalram > 2048ull * 1024 * 1024) {
        // from - phone-xhdpi-2048-dalvik-heap.mk
        heapstartsize = "8m";
        heapgrowthlimit = "192m";
        heapsize = "512m";
        heaptargetutilization = "0.75";
        heapminfree = "512k";
        heapmaxfree = "8m";
    } else {
        // from go_defaults_common.prop
        // from - phone-xhdpi-2048-dalvik-heap.mk
        heapstartsize = "8m";
        heapgrowthlimit = "128m";
        heapsize = "256m";
        heaptargetutilization = "0.75";
        heapminfree = "512k";
        heapmaxfree = "8m";
    }
	}

	// set rest of Go tweaks for 2 GB
    if (sys.totalram < 2048ull * 1024 * 1024) {
        // set lowram options and enable traced by default
        property_set("ro.config.low_ram", "true");
        property_set("persist.traced.enable", "true");
        property_set("ro.statsd.enable", "true");
        // set threshold to filter unused apps
        property_set("pm.dexopt.downgrade_after_inactive_days", "10");
        // set the compiler filter for shared apks to quicken
        property_set("pm.dexopt.shared", "quicken");
		// set swap options
		property_set("ro.vendor.qti.config.swap", "true");
    }
}

void vendor_load_properties()
{
    check_device();

    property_set("dalvik.vm.heapstartsize", heapstartsize);
    property_set("dalvik.vm.heapgrowthlimit", heapgrowthlimit);
    property_set("dalvik.vm.heapsize", heapsize);
    property_set("dalvik.vm.heaptargetutilization", heaptargetutilization);
    property_set("dalvik.vm.heapminfree", heapminfree);
    property_set("dalvik.vm.heapmaxfree", heapmaxfree);
				
    string boot_cert = android::base::GetProperty("ro.boot.product.cert", "");

    if (boot_cert == "M1810F6LG" || boot_cert == "M1810F6LH" || boot_cert == "M1810F6LI"
            || boot_cert == "M1810F6LE" || boot_cert == "M1810F6LT" || boot_cert == "M1810F6LC")
        load_props("onclite", "Redmi 7");
    else
        load_props("onc", "Redmi Y3");

}
