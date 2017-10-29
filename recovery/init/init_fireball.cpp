/*
   Copyright (c) 2013, The Linux Foundation. All rights reserved.

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

#include <blkid/blkid.h>
#include <stdlib.h>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>
#include <unistd.h>

#include <string>

#include "log.h"
#include "property_service.h"
#include "util.h"
#include "vendor_init.h"

/*
 *  The original partitions for data and internal storage are by-name/userdata
 *  and by-name/fat, respectively. Some unofficial AOSP ROMs swap these two
 *  partitions to grant more room to the data partition. Detect whether these
 *  partitions have been swapped during init and if true:
 *  1) apply new device name 'fireballx'
 *  2) swap fstabs
 */

#define BLK_PART_ORIG_DATA "/dev/block/platform/msm_sdcc.1/by-name/userdata"
#define BLK_PART_ORIG_STOR "/dev/block/platform/msm_sdcc.1/by-name/fat"

void property_override(char const prop[], char const value[]) {
    prop_info *pi;

    pi = (prop_info*) __system_property_find(prop);
    if (pi)
        __system_property_update(pi, value, strlen(value));
    else
        __system_property_add(prop, strlen(prop), value, strlen(value));
}

void vendor_load_properties() {
    char *detected_fs_type_data = NULL;
    /* char *detected_fs_type_stor = NULL; */

    std::string platform = property_get("ro.board.platform");
    if (platform != ANDROID_TARGET) {
        property_override("ro.product.device", "fireball");
        property_override("ro.build.product", "fireball");
        return;
    }

    detected_fs_type_data = blkid_get_tag_value(NULL, "TYPE", BLK_PART_ORIG_DATA);
    if (detected_fs_type_data == NULL) {
        /*
         * This may be encountered if by-name/userdata is encrypted
         * when the standard partition scheme is in use. Apply the
         * default device name.
         */
        property_override("ro.product.device", "fireball");
        property_override("ro.build.product", "fireball");
        return;
    }

    /* detected_fs_type_stor = blkid_get_tag_value(NULL, "TYPE", BLK_PART_ORIG_STOR); */
    /*
     * Do not fail if the fstype of by-name/fat cannot be detected.
     * This may be encountered if the block device is encrypted in
     * the swapped partition scheme.
     *
     * FIXME: Until a more reliable check can be performed, we rely
     * on the fstype of by-name/userdata to tell us if the swapped
     * partition scheme is active.
     */

#if 0
    if (strcmp(detected_fs_type_data, "vfat") == 0 &&
            (strcmp(detected_fs_type_stor, "ext4") == 0 ||
             strcmp(detected_fs_type_stor, "f2fs") == 0)) {
#else
    if (strcmp(detected_fs_type_data, "vfat") == 0) {
#endif
        property_override("ro.product.device", "fireballx");
        property_override("ro.build.product", "fireballx");
        symlink((const char *)("/etc/recovery.fstab.fbx"),
                (const char *)("/etc/twrp.fstab"));
        ERROR("Found swapped partition scheme; fireballx device name applied");
    } else {
        property_override("ro.product.device", "fireball");
        property_override("ro.build.product", "fireball");
        ERROR("Found standard partition scheme; fireball device name applied");
    }
}
