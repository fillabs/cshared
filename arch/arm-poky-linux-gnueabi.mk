SDKTARGETSYSROOT := /opt/poky-craton2/2.2.1/sysroots/cortexa7t2hf-neon-poky-linux-gnueabi
export PATH := $(PATH):/opt/poky-craton2/2.2.1/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi
CFG += ATLK
cflags := -feliminate-unused-debug-types -march=armv7ve -mthumb -mfpu=neon -mfloat-abi=hard -mcpu=cortex-a7 --sysroot=$(SDKTARGETSYSROOT) -Wno-parentheses
LDFLAGS := -Wl,-O1 -Wl,--hash-style=gnu -Wl,--as-needed
defines += 'target_type_get()=1' TARGET_TYPE_SECTON=1