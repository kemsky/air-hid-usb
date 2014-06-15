/*
 ~ Copyright: (c) 2014. Turtsevich Alexander
 ~
 ~ Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.html
 */

#include "hidapi.h"

#define LOG_ENABLED false

#ifdef _WIN32
    //force using mingw <stdint.h>, see FlashRuntimeExtensions.h
    #undef WIN32
    #include "FlashRuntimeExtensions.h"
    #define EXPORT __declspec(dllexport)

    #define LOG_FILE "c:/hidapi.log"
#else
    #include <Adobe AIR/Adobe AIR.h>
    #define EXPORT __attribute__((visibility("default")))

    #define LOG_FILE "/hidapi.log"
#endif

#ifdef __cplusplus
extern "C" {
#endif

    EXPORT void initializer(void** extData, FREContextInitializer* ctxInitializer, FREContextFinalizer* ctxFinalizer);
    EXPORT void finalizer(void* extData);

#ifdef __cplusplus
}
#endif
