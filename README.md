Copyright (C) 2012 The LineageOS Project
 
  Device configuration for Xiaomi Redmi 7
 =========================================
 
  The Xiaomi Redmi 7 (codenamed "Onclite") is an 
budget smartphone from Xiaomi.
 
  It was announced and released in April 2019.
 
  ## Device specifications
 
  Basic   | Spec Sheet
-------:|:-------------------------
CPU     | Octa-core (4x1.8 GHz Kryo 250 Gold & 4x1.8 GHz Kryo 250 Silver)
Chipset | Qualcomm SDM632 Snapdragon 632 (14 nm)
GPU     | Adreno 506
Shipped Android Version | 9.0
Memory | 64 GB, 4 GB RAM or 32 GB, 3 GB RAM
MicroSD | microSD, up to 512 GB (dedicated slot)
Battery | Non-removable Li-Ion 4000 mAh battery
Display | 6.26 inches, 97.8 cm2 (~81.5% screen-to-body ratio)
Rear Camera  | 12 MP, f/1.8, 1.25µm, PDAF
Front Camera  |  8 MP, f/2.0, 1.12µm

![Xiaomi Redmi 7](https://i.gadgets360cdn.com/products/large/1552901002_635_redmi_7.jpg?downsize=770:*&output-quality=70&output-format=webp "Xiaomi Redmi 7")

## Compile

Add these projects to .repo/manifest.xml:

```xml
  <project name="krot-melles/android_device_xiaomi_onclite" path="device/xiaomi/onclite" remote="github" />
  <project name="krot-melles/android_vendor_xiaomi_onclite" path="vendor/xiaomi/onclite" remote="github" />
  <project name="krot-melles/android_kernel_xiaomi_onclite" path="kernel/xiaomi/onclite" remote="github" />
```

Finally execute these:

```
. build/envsetup.sh
lunch lineage_onclite-eng
brunch onclite
```

 
 

 
                

 
 
  
