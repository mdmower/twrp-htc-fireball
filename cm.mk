# Release name
PRODUCT_RELEASE_NAME := fireball

# Inherit some common CM stuff.
$(call inherit-product, vendor/cm/config/common_full_phone.mk)

# Inherit device configuration
$(call inherit-product, device/htc/fireball/device.mk)

## Device identifier. This must come after all inclusions
PRODUCT_DEVICE := fireball
PRODUCT_NAME := cm_fireball
PRODUCT_BRAND := htc
PRODUCT_MODEL := Incredible 4G LTE
PRODUCT_MANUFACTURER := HTC

PRODUCT_BUILD_PROP_OVERRIDES += \
    PRODUCT_NAME=fireball

TARGET_SCREEN_HEIGHT := 960
TARGET_SCREEN_WIDTH := 540
