/*
 * Copyright (C) 2019 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "vendor.mokee.touch@1.0-service.onclite"

#include <android-base/logging.h>
#include <binder/ProcessState.h>
#include <hidl/HidlTransportSupport.h>

#include "KeyDisabler.h"
#include "GloveMode.h"
#include "TouchscreenGesture.h"

using android::OK;
using android::sp;
using android::status_t;
using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;

using ::vendor::mokee::touch::V1_0::IKeyDisabler;
using ::vendor::mokee::touch::V1_0::implementation::KeyDisabler;
using ::vendor::mokee::touch::V1_0::IGloveMode;
using ::vendor::mokee::touch::V1_0::implementation::GloveMode;
using ::vendor::mokee::touch::V1_0::ITouchscreenGesture;
using ::vendor::mokee::touch::V1_0::implementation::TouchscreenGesture;

int main() {	
	android::sp<ITouchscreenGesture> gestureService = new TouchscreenGesture();
    android::hardware::configureRpcThreadpool(1, true /*callerWillJoin*/);
    sp<KeyDisabler> keyDisabler;
    sp<GloveMode> gloveMode;
    status_t status;

    LOG(INFO) << "Touch HAL service is starting.";

    keyDisabler = new KeyDisabler();
    if (keyDisabler == nullptr) {
        LOG(ERROR) << "Can not create an instance of Touch HAL KeyDisabler Iface, exiting.";
        goto shutdown;
    }

    gloveMode = new GloveMode();
    if (gloveMode == nullptr) {
        LOG(ERROR) << "Can not create an instance of Touch HAL GloveMode Iface, exiting.";
        goto shutdown;
    }

    configureRpcThreadpool(1, true /*callerWillJoin*/);

    status = keyDisabler->registerAsService();
    if (status != OK) {
        LOG(ERROR) << "Could not register service for Touch HAL KeyDisabler Iface ("
                   << status << ")";
        goto shutdown;
    }

    status = gloveMode->registerAsService();
    if (status != OK) {
        LOG(ERROR) << "Could not register service for Touch HAL GloveMode Iface ("
                   << status << ")";
        goto shutdown;
    }

    if (gestureService->registerAsService() != android::OK) {
        LOG(ERROR) << "Cannot register touchscreen gesture HAL service.";
        return 1;
    }

    LOG(INFO) << "Touch HAL service is ready.";
    joinRpcThreadpool();
    // Should not pass this line

shutdown:
    // In normal operation, we don't expect the thread pool to shutdown
    LOG(ERROR) << "Touch HAL service is shutting down.";
