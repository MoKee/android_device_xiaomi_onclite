#
# Copyright (C) 2019-2020 The LineageOS Project
# Copyright (C) 2020 The MoKee Open Source Project
#
# SPDX-License-Identifier: Apache-2.0
#

# Inherit from those products. Most specific first.
$(call inherit-product, $(SRC_TARGET_DIR)/product/core_64_bit.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base_telephony.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/product_launched_with_p.mk)

# Inherit from onclite device
$(call inherit-product, device/xiaomi/onclite/device.mk)

# Inherit some MoKee stuff.
$(call inherit-product, vendor/mokee/config/common_full_phone.mk)

# Device identifier. This must come after all inclusions.
PRODUCT_NAME := mokee_onclite
PRODUCT_DEVICE := onclite
PRODUCT_BRAND := Xiaomi
PRODUCT_MODEL := Redmi 7
PRODUCT_MANUFACTURER := Xiaomi

BUILD_FINGERPRINT := "google/redfin/redfin:11/RQ1A.210105.003/7005429:user/release-keys"

PRODUCT_PROPERTY_OVERRIDES += \
    ro.build.fingerprint=$(BUILD_FINGERPRINT)

PRODUCT_BUILD_PROP_OVERRIDES += \
    PRIVATE_BUILD_DESC="redfin-user 11 RQ1A.210105.003 7005429 release-keys"

PRODUCT_GMS_CLIENTID_BASE := android-xiaomi
