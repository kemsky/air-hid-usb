/*
 ~ Copyright: (c) 2014. Turtsevich Alexander
 ~
 ~ Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.html
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <wchar.h>
#include <stdarg.h>

#ifdef _WIN32
    #include <malloc.h>
    #include <windows.h>
#else
    #include <sys/malloc.h>
#endif

#include "air.h"

#ifdef LOG_ENABLED
    #define DEBUG_PRINT(msg) print(msg)
    #define DEBUG_RESULT(method, result) printResult(method, result)
#else
    #define DEBUG_PRINT(msg)
    #define DEBUG_RESULT(method, result)
#endif

#ifdef __cplusplus
extern "C" {
#endif

        void print(const char * msg)
        {
            if(LOG_ENABLED)
            {
                FILE *file;
                char fname[] = LOG_FILE;
                file = fopen(fname,"a");

                if(file == NULL)
                {
                    return;
                }

                fprintf(file, "%s", msg);
                fprintf(file, "\n");

                fclose(file);
            }
        }

        void printResult(const char * method, FREResult result)
        {
            char msg [1024];

            if(FRE_OK == result)
            {
                sprintf(msg, "SUCCESS FRE_OK: %s", method);
            }
            else if(FRE_NO_SUCH_NAME == result)
            {
                sprintf(msg, "ERROR FRE_NO_SUCH_NAME: %s", method);
            }
            else if(FRE_INVALID_OBJECT == result)
            {
                sprintf(msg, "ERROR FRE_INVALID_OBJECT: %s", method);
            }
            else if(FRE_TYPE_MISMATCH == result)
            {
                sprintf(msg, "ERROR FRE_TYPE_MISMATCH: %s", method);
            }
            else if(FRE_ACTIONSCRIPT_ERROR == result)
            {
                sprintf(msg, "ERROR FRE_ACTIONSCRIPT_ERROR: %s", method);
            }
            else if(FRE_INVALID_ARGUMENT == result)
            {
                sprintf(msg, "ERROR FRE_INVALID_ARGUMENT: %s", method);
            }
            else if(FRE_INVALID_ARGUMENT == result)
            {
                sprintf(msg, "ERROR FRE_INVALID_ARGUMENT: %s", method);
            }
            else if(FRE_READ_ONLY == result)
            {
                sprintf(msg, "ERROR FRE_READ_ONLY: %s", method);
            }
            else if(FRE_WRONG_THREAD == result)
            {
                sprintf(msg, "ERROR FRE_WRONG_THREAD: %s", method);
            }
            else if(FRE_ILLEGAL_STATE == result)
            {
                sprintf(msg, "ERROR FRE_ILLEGAL_STATE: %s", method);
            }
            else if(FRE_INSUFFICIENT_MEMORY == result)
            {
                sprintf(msg, "ERROR FRE_INSUFFICIENT_MEMORY: %s", method);
            }

            print(msg);
        }

        /*
        * Private. Check strings before convertion.
        */
        bool is_not_empty_strw(const wchar_t * s)
        {
             return (s != NULL && wcslen(s) > 1 && wcscmp(s, L"") > 0);
        }

        /*
        * Private. Returns device handle stored in air extension context.
        */
        hid_device * get_device_handle(FREContext ctx)
        {
            hid_device * device = NULL;

            FREResult result = FREGetContextNativeData(ctx, (void **) &device);

            if(FRE_OK != result)
            {
                device = NULL;
            }
            DEBUG_RESULT("get_device_handle", result);
            return device;
        }

        FREObject get_null_string_as_empty(const char * resultString)
        {
            FREObject stringObject;
            FREResult result;

            if(resultString != NULL && strlen(resultString) > 1)
            {
               result = FRENewObjectFromUTF8((uint32_t)strlen(resultString), (const uint8_t*)resultString, &stringObject);
            }
            else
            {
               result = FRENewObjectFromUTF8((uint32_t)strlen(""), (const uint8_t*)"", &stringObject);
            }
            DEBUG_RESULT("FRENewObjectFromUTF8: get_null_string_as_empty", result);
            return stringObject;
        }

        void put_object_property_int32(FREObject object, const uint8_t * propertyName, uint32_t value)
        {
            char msg [1024];
            FREResult result;
            FREObject propertyValue;

            result = FRENewObjectFromUint32(value, &propertyValue);
            sprintf(msg, "FRENewObjectFromUint32: %s", propertyName);
            DEBUG_RESULT(msg, result);

            result = FRESetObjectProperty(object, propertyName, propertyValue, NULL);
            sprintf(msg, "FRESetObjectProperty: %s", propertyName);
            DEBUG_RESULT(msg, result);
        }

        void put_object_property_strw(FREObject object, const uint8_t * propertyName, const wchar_t * value)
        {
            if(is_not_empty_strw(value))
            {
                char msg [1024];
                char * string = (char *) malloc(8192);
                wcstombs(string,  value, 8192);
                FREObject propertyValue = get_null_string_as_empty(string);
                FREResult result = FRESetObjectProperty(object, propertyName, propertyValue, NULL);
                sprintf(msg, "FRESetObjectProperty: %s", propertyName);
                DEBUG_RESULT(msg, result);
                free(string);
            }
        }

        /*
        * Private. Returns as3 object (Boolean) for bool.
        */
        FREObject get_result_as_boolean(bool value)
        {
            FREObject resultObject;
            FREResult result = FRENewObjectFromBool(value, &resultObject);
            DEBUG_RESULT("FRENewObjectFromBool: get_result_as_boolean", result);
            return resultObject;
        }

        /*
        * Private. Returns as3 object (int) for int.
        */
        FREObject get_result_as_int32(int value)
        {
            FREObject resultObject;
            FREResult result = FRENewObjectFromInt32(value, &resultObject);
            DEBUG_RESULT("FRENewObjectFromInt32: get_result_as_int32", result);
            return resultObject;
        }

        /*
        * hid_enumerate (unsigned short vendor_id, unsigned short product_id)
        */
        FREObject hid_enumerateA(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
        {
            DEBUG_PRINT("hid_enumerateA");

            FREResult result;

            //input array
            FREObject resultArray = argv[0];

            uint32_t prodId;
            result = FREGetObjectAsUint32(argv[1], &prodId);
            DEBUG_RESULT("FREGetObjectAsUint32: prodId", result);

            uint32_t vendId;
            result = FREGetObjectAsUint32(argv[2], &vendId);
            DEBUG_RESULT("FREGetObjectAsUint32: vendId", result);

            char msg [1024];
            sprintf(msg, "hid_enumerateA(vendorId=%04hx, productId=%04hx)", vendId, prodId);
            DEBUG_PRINT(msg);

            struct hid_device_info *devs;
            devs = hid_enumerate(vendId, prodId);

            if(devs != NULL)
            {
                uint32_t deviceCounter = 0;
                struct hid_device_info *cur_dev;

                cur_dev = devs;

                while (cur_dev)
                {
                    DEBUG_PRINT("Device:");
                    sprintf(msg, "  vendorId=%04hx productId=%04hx", cur_dev->vendor_id, cur_dev->product_id);
                    DEBUG_PRINT(msg);
                    sprintf(msg, "  path: %s", cur_dev->path);
                    DEBUG_PRINT(msg);
                    sprintf(msg, "  serial_number: %ls",cur_dev->serial_number);
                    DEBUG_PRINT(msg);
                    sprintf(msg, "  Manufacturer: %ls", cur_dev->manufacturer_string);
                    DEBUG_PRINT(msg);
                    sprintf(msg, "  Product:      %ls", cur_dev->product_string);
                    DEBUG_PRINT(msg);
                    sprintf(msg, "  Release:      %hx", cur_dev->release_number);
                    DEBUG_PRINT(msg);
                    sprintf(msg, "  Interface:    %d",  cur_dev->interface_number);
                    DEBUG_PRINT(msg);

                    //create device Object
                    FREObject device;
                    uint8_t *className = (uint8_t*)"com.hidapi.HidDeviceInfo";

                    //Array of zero length is NOT standard C, but a GCC extension.
                    result = FRENewObject(className, 0, new FREObject [0], &device, NULL);

                    DEBUG_RESULT("FRENewObject: com.hidapi.HidDeviceInfo", result);

                    //set path property of device object
                    FREObject pathPropertyValue;

                    result = FRENewObjectFromUTF8((uint32_t)strlen(cur_dev->path), (const uint8_t*)cur_dev->path, &pathPropertyValue);
                    DEBUG_RESULT("FRENewObjectFromUTF8: pathPropertyValue", result);

                    result = FRESetObjectProperty(device, (const uint8_t*) "path", pathPropertyValue, NULL);
                    DEBUG_RESULT("FRESetObjectProperty: pathPropertyValue", result);

                    //set manufacturer_string property of device object
                    put_object_property_strw(device, (const uint8_t*) "manufacturer_string", cur_dev->manufacturer_string);

                    //set product_string property of device object
                    put_object_property_strw(device, (const uint8_t*) "product_string", cur_dev->product_string);

                    //set serial_number property of device object
                    put_object_property_strw(device, (const uint8_t*) "serial_number", cur_dev->serial_number);

                    //set productId property of device object
                    put_object_property_int32(device, (const uint8_t*) "productId", (uint32_t)cur_dev->product_id);

                    //set vendorId property of device object
                    put_object_property_int32(device, (const uint8_t*) "vendorId", (uint32_t)cur_dev->vendor_id);

                    //set release_number property of device object
                    put_object_property_int32(device, (const uint8_t*) "release_number", (uint32_t)cur_dev->release_number);

                    //set usage_page property of device object
                    put_object_property_int32(device, (const uint8_t*) "usage_page", (uint32_t)cur_dev->usage_page);

                    //set usage property of device object
                    put_object_property_int32(device, (const uint8_t*) "usage", (uint32_t)cur_dev->usage);

                    //set interface_number property of device object
                    put_object_property_int32(device, (const uint8_t*) "interface_number", (uint32_t)cur_dev->interface_number);

                    //add device to result array
                    result = FRESetArrayLength(resultArray, deviceCounter + 1);
                    DEBUG_RESULT("FRESetArrayLength", result);

                    result = FRESetArrayElementAt(resultArray, deviceCounter, device);
                    DEBUG_RESULT("FRESetArrayElementAt", result);

                    //increment device counter
                    deviceCounter++;
                    cur_dev = cur_dev->next;
                }
                //void
                hid_free_enumeration(devs);
            }
            DEBUG_PRINT("hid_enumerateA success");
            //no way to detect HIDAPI errors here...
            //hid_enumerate returns NULL both on error and when list is empty
            return NULL;
        }

        /*
        * hid_open (unsigned short vendor_id, unsigned short product_id, wchar_t *serial_number)
        */
        FREObject hid_openA(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
        {
            DEBUG_PRINT("hid_openA");

            FREResult result;

            uint32_t prodId;
            result = FREGetObjectAsUint32(argv[0], &prodId);
            DEBUG_RESULT("FREGetObjectAsUint32: prodId", result);

            uint32_t vendId;
            result = FREGetObjectAsUint32(argv[1], &vendId);
            DEBUG_RESULT("FREGetObjectAsUint32: vendId", result);

            FREObject serialObject = argv[2];
            uint32_t length;
            const uint8_t *serialString = 0;
            result = FREGetObjectAsUTF8(serialObject, &length, (const uint8_t**) &serialString);
            DEBUG_RESULT("FREGetObjectAsUTF8: serialString", result);

            if(FRE_OK == result)
            {
                char msg [1024];
                wchar_t * serial_number = NULL;
                hid_device * device = NULL;
                if(serialString != NULL)
                {
                    serial_number =(wchar_t *) malloc(8192);
                    mbtowc(serial_number,  (const char*)serialString, 8192);

                    sprintf(msg, "hid_openA: prodId=%d, vendId=%d, serial=%s", prodId, vendId, serialString);
                    DEBUG_PRINT(msg);

                    device = hid_open(vendId, prodId, NULL);
                    free(serial_number);
                }
                else
                {
                    sprintf(msg, "hid_openA: prodId=%d, vendId=%d, serial=NULL", prodId, vendId);
                    DEBUG_PRINT(msg);
                    device = hid_open(vendId, prodId, serial_number);
                }

                if(device != NULL)
                {
                    DEBUG_PRINT("hid_openA success");
                    result = FRESetContextNativeData(ctx, (void*) device);
                    DEBUG_RESULT("FRESetContextNativeData: device", result);

                    if(FRE_OK == result)
                    {
                        DEBUG_PRINT("hid_openA save handle success");
                        return get_result_as_boolean(true);
                    }
                    else
                    {
                        DEBUG_PRINT("hid_openA save handle error");
                    }
                }
                else
                {
                    DEBUG_PRINT("hid_openA error");
                }
            }
            else
            {
                DEBUG_PRINT("hid_openA -> FREGetObjectAsUTF8 error");
            }

            return get_result_as_boolean(false);
        }

        /*
        * hid_open_path (const char *path)
        */
        FREObject hid_open_pathA(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
        {
            DEBUG_PRINT("hid_open_pathA");

            FREResult result;
            char msg [1024];

            FREObject pathObject = argv[0];
            uint32_t length;
            const uint8_t *pathString = 0;
            result = FREGetObjectAsUTF8(pathObject, &length, (const uint8_t**) &pathString);
            DEBUG_RESULT("FREGetObjectAsUTF8: pathString", result);

            if(FRE_OK == result)
            {
                sprintf(msg, "hid_open_pathA: %s", pathString);
                DEBUG_PRINT(msg);

                hid_device * device = hid_open_path((const char*)pathString);
                if(device != NULL)
                {
                    DEBUG_PRINT("hid_open_pathA success");
                    result = FRESetContextNativeData(ctx, (void*) device);
                    DEBUG_RESULT("FRESetContextNativeData: device", result);

                    if(FRE_OK == result)
                    {
                        DEBUG_PRINT("hid_open_pathA save handle success");
                        return get_result_as_boolean(true);
                    }
                    else
                    {
                        DEBUG_PRINT("hid_open_pathA save handle error");
                    }
                }
                else
                {
                    DEBUG_PRINT("hid_open_pathA error");
                }
            }
            else
            {
                DEBUG_PRINT("hid_open_pathA -> FREGetObjectAsUTF8 error");
            }
            return get_result_as_boolean(false);
        }

        /*
        * hid_send_feature_report (hid_device *device, const unsigned char *data, size_t length)
        */
        FREObject hid_send_feature_reportA(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
        {
            DEBUG_PRINT("hid_send_feature_reportA");
            int res = -1;

            hid_device * device = get_device_handle(ctx);

            if(device != NULL)
            {
                DEBUG_PRINT("hid_send_feature_reportA device found");
                FREObject byteArrayObject = argv[0];
                FREByteArray byteArray;
                FREResult result = FREAcquireByteArray(byteArrayObject, &byteArray);
                DEBUG_RESULT("FREAcquireByteArray: byteArray", result);
                if(FRE_OK == result)
                {
                    res = hid_send_feature_report(device, (const unsigned char *)byteArray.bytes, (size_t)byteArray.length);
                    if(res >= 0)
                    {
                        DEBUG_PRINT("hid_send_feature_report success");
                    }
                    else
                    {
                        DEBUG_PRINT("hid_send_feature_report error");
                    }

                    result = FREReleaseByteArray(byteArrayObject);
                    DEBUG_RESULT("FREReleaseByteArray: byteArrayObject", result);
                }
            }
            else
            {
               DEBUG_PRINT("hid_send_feature_reportA: no device");
            }
            return get_result_as_int32(res);
        }

        /*
        * hid_get_feature_report (hid_device *device, unsigned char *data, size_t length)
        */
        FREObject hid_get_feature_reportA(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
        {
            DEBUG_PRINT("hid_get_feature_reportA");
            int res = -1;

            hid_device * device = get_device_handle(ctx);

            if(device != NULL)
            {
                DEBUG_PRINT("hid_get_feature_reportA device found");
                FREObject byteArrayObject = argv[0];
                FREByteArray byteArray;
                FREResult result = FREAcquireByteArray(byteArrayObject, &byteArray);
                DEBUG_RESULT("FREAcquireByteArray: byteArray", result);

                if(FRE_OK == result)
                {
                    res = hid_get_feature_report(device, (unsigned char *)byteArray.bytes, (size_t)byteArray.length);
                    if(res >= 0)
                    {
                        DEBUG_PRINT("hid_get_feature_reportA success");
                    }
                    else
                    {
                        DEBUG_PRINT("hid_get_feature_reportA error");
                    }

                    result = FREReleaseByteArray(byteArrayObject);
                    DEBUG_RESULT("FREReleaseByteArray: byteArrayObject", result);
                }
            }
            else
            {
                DEBUG_PRINT("hid_get_feature_reportA: no device");
            }
            return get_result_as_int32(res);
        }

        /*
        * hid_write (hid_device *device, const unsigned char *data, size_t length)
        */
        FREObject hid_writeA(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
        {
            DEBUG_PRINT("hid_writeA");
            int res = -1;

            hid_device * device = get_device_handle(ctx);

            if(device != NULL)
            {
                DEBUG_PRINT("hid_writeA device found");
                FREObject byteArrayObject = argv[0];
                FREByteArray byteArray;
                FREResult result = FREAcquireByteArray(byteArrayObject, &byteArray);
                DEBUG_RESULT("FREAcquireByteArray: byteArray", result);

                if(FRE_OK == result)
                {
                    res = hid_write(device, (const unsigned char *)byteArray.bytes, (size_t)byteArray.length);
                    if(res >= 0)
                    {
                        DEBUG_PRINT("hid_writeA success");
                    }
                    else
                    {
                        DEBUG_PRINT("hid_writeA error");
                    }

                    result = FREReleaseByteArray(byteArrayObject);
                    DEBUG_RESULT("FREReleaseByteArray: byteArrayObject", result);
                }
            }
            else
            {
               DEBUG_PRINT("hid_writeA: no device");
            }
            return get_result_as_int32(res);
        }

        /*
        * hid_read (hid_device *device, unsigned char *data, size_t length)
        */
        FREObject hid_readA(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
        {
            DEBUG_PRINT("hid_readA");
            int res = -1;

            hid_device * device = get_device_handle(ctx);

            if(device != NULL)
            {
                DEBUG_PRINT("hid_readA device found");
                FREObject byteArrayObject = argv[0];
                FREByteArray byteArray;
                FREResult result = FREAcquireByteArray(byteArrayObject, &byteArray);
                DEBUG_RESULT("FREAcquireByteArray: byteArray", result);

                if(FRE_OK == result)
                {
                    res = hid_read(device, (unsigned char *)byteArray.bytes, (size_t)byteArray.length);
                    if(res >= 0)
                    {
                        DEBUG_PRINT("hid_readA success");
                    }
                    else
                    {
                        DEBUG_PRINT("hid_readA error");
                    }

                    result = FREReleaseByteArray(byteArrayObject);
                    DEBUG_RESULT("FREReleaseByteArray: byteArrayObject", result);
                }
            }
            else
            {
                DEBUG_PRINT("hid_readA: no device");
            }
            return get_result_as_int32(res);
        }

        /*
        * hid_read_timeout (hid_device *dev, unsigned char *data, size_t length, int milliseconds)
        */
        FREObject hid_read_timeoutA(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
        {
            DEBUG_PRINT("hid_read_timeoutA");
            int res = -1;

            hid_device * device = get_device_handle(ctx);

            if(device != NULL)
            {
                DEBUG_PRINT("hid_read_timeoutA device found");
                FREObject byteArrayObject = argv[0];
                FREByteArray byteArray;
                FREResult result = FREAcquireByteArray(byteArrayObject, &byteArray);
                DEBUG_RESULT("FREAcquireByteArray: byteArray", result);

                int32_t millis;
                result = FREGetObjectAsInt32(argv[1], &millis);
                DEBUG_RESULT("FREGetObjectAsInt32: millis", result);

                if(FRE_OK == result)
                {
                    res = hid_read_timeout(device, (unsigned char *)byteArray.bytes, (size_t)byteArray.length, millis);
                    if(res >= 0)
                    {
                        DEBUG_PRINT("hid_read_timeoutA success");
                    }
                    else
                    {
                        DEBUG_PRINT("hid_read_timeoutA error");
                    }

                    result = FREReleaseByteArray(byteArrayObject);
                    DEBUG_RESULT("FREReleaseByteArray: byteArrayObject", result);
                }
            }
            else
            {
                DEBUG_PRINT("hid_read_timeoutA: no device");
            }
            return get_result_as_int32(res);
        }
        /*
        * hid_close (hid_device *device)
        */
        FREObject hid_closeA(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
        {
            DEBUG_PRINT("hid_closeA");

            hid_device * device = get_device_handle(ctx);

            if(device != NULL)
            {
                hid_close(device);

                FREResult result = FRESetContextNativeData(ctx, (void*) NULL);
                DEBUG_RESULT("FRESetContextNativeData: NULL", result);

                if(FRE_OK == result)
                {
                    DEBUG_PRINT("FRESetContextNativeData reset handle success");
                    return get_result_as_boolean(true);
                }
                else
                {
                    DEBUG_PRINT("FRESetContextNativeData reset handle error");
                }

                DEBUG_PRINT("hid_closeA success");
            }
            else
            {
                DEBUG_PRINT("hid_closeA: no device");
            }
            return get_result_as_boolean(false);
        }

        /*
        * hid_error (hid_device *device)
        */
        FREObject hid_errorA(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
        {
            DEBUG_PRINT("hid_errorA");

            hid_device * device = get_device_handle(ctx);

            FREResult result;
            FREObject errorStringObject;

            if(device != NULL)
            {
                const wchar_t * hidError = hid_error(device);
                if(is_not_empty_strw(hidError))
                {
                    char * errorString = (char *) malloc(8192);
                    wcstombs(errorString,  hidError, 8192);
                    result = FRENewObjectFromUTF8((uint32_t)strlen(errorString), (const uint8_t*)errorString, &errorStringObject);
                    free(errorString);
                }
                else
                {
                    char errorString[] = "";
                    result = FRENewObjectFromUTF8((uint32_t)strlen(errorString), (const uint8_t*)errorString, &errorStringObject);
                }
                DEBUG_RESULT("FRENewObjectFromUTF8: hidError", result);
                DEBUG_PRINT("hid_errorA success");
            }
            else
            {
                char errorString[] = "";
                result = FRENewObjectFromUTF8((uint32_t)strlen(errorString), (const uint8_t*)errorString, &errorStringObject);
                DEBUG_RESULT("FRENewObjectFromUTF8: errorString", result);
                DEBUG_PRINT("hid_errorA: no device");
            }
            return errorStringObject;
        }

        /*
        * hid_get_manufacturer_string (hid_device *device, wchar_t *string, size_t maxlen)
        */
        FREObject hid_get_manufacturer_stringA(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
        {
            DEBUG_PRINT("hid_get_manufacturer_stringA");

            hid_device * device = get_device_handle(ctx);

            FREObject stringObject = NULL;

            if(device != NULL)
            {
                wchar_t hidString[255];
                memset(hidString, 0, sizeof(hidString));

                int ret = hid_get_manufacturer_string(device, hidString, sizeof(hidString));
                if(ret >= 0)
                {
                    if(is_not_empty_strw(hidString))
                    {
                        char * resultString = (char *) malloc(8192);
                        wcstombs(resultString,  hidString, 8192);
                        stringObject = get_null_string_as_empty(resultString);
                        free(resultString);
                    }
                    else
                    {
                        char empty[] = "";
                        stringObject = get_null_string_as_empty(empty);
                    }
                }
                else
                {
                    DEBUG_PRINT("hid_get_manufacturer_stringA: error");
                }
                DEBUG_PRINT("hid_get_manufacturer_stringA success");
            }
            else
            {
                DEBUG_PRINT("hid_get_manufacturer_stringA: no device");
            }
            return stringObject;
        }

        /*
        * hid_get_manufacturer_string (hid_device *device, wchar_t *string, size_t maxlen)
        */
        FREObject hid_get_product_stringA(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
        {
            DEBUG_PRINT("hid_get_product_stringA");

            hid_device * device = get_device_handle(ctx);

            FREObject stringObject = NULL;

            if(device != NULL)
            {
                wchar_t hidString[255];
                memset(hidString, 0, sizeof(hidString));

                int ret = hid_get_product_string(device, hidString, sizeof(hidString));
                if(ret >= 0)
                {
                    if(is_not_empty_strw(hidString))
                    {
                        char * resultString = (char *) malloc(8192);
                        wcstombs(resultString,  hidString, 8192);
                        stringObject = get_null_string_as_empty(resultString);
                        free(resultString);
                    }
                    else
                    {
                        char empty[] = "";
                        stringObject = get_null_string_as_empty(empty);
                    }
                }
                else
                {
                    DEBUG_PRINT("hid_get_product_stringA: error");
                }
                DEBUG_PRINT("hid_get_product_stringA success");
            }
            else
            {
                DEBUG_PRINT("hid_get_product_stringA: no device");
            }
            return stringObject;
        }

        /*
        * hid_get_serial_number_string (hid_device *device, wchar_t *string, size_t maxlen)
        */
        FREObject hid_get_serial_number_stringA(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
        {
            DEBUG_PRINT("hid_get_serial_number_stringA");

            hid_device * device = get_device_handle(ctx);

            FREObject stringObject = NULL;

            if(device != NULL)
            {
                wchar_t hidString[255];
                memset(hidString, 0, sizeof(hidString));

                int ret = hid_get_serial_number_string(device, hidString, sizeof(hidString));
                if(ret >= 0)
                {
                    if(is_not_empty_strw(hidString))
                    {
                        char * resultString = (char *) malloc(8192);
                        wcstombs(resultString,  hidString, 8192);
                        stringObject = get_null_string_as_empty(resultString);
                        free(resultString);
                    }
                    else
                    {
                        char empty[] = "";
                        stringObject = get_null_string_as_empty(empty);
                    }
                }
                else
                {
                    DEBUG_PRINT("hid_get_serial_number_stringA: error");
                }
                DEBUG_PRINT("hid_get_serial_number_stringA success");
            }
            else
            {
                DEBUG_PRINT("hid_get_serial_number_stringA: no device");
            }
            return stringObject;
        }

        /*
        * hid_get_indexed_string (hid_device *device, int string_index, wchar_t *string, size_t maxlen)
        */
        FREObject hid_get_indexed_stringA(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
        {
            DEBUG_PRINT("hid_get_indexed_stringA");

            hid_device * device = get_device_handle(ctx);

            FREObject stringObject = NULL;

            if(device != NULL)
            {
                wchar_t hidString[512];
                memset(hidString, 0, sizeof(hidString));

                int32_t index;
                FREGetObjectAsInt32(argv[0], &index);

                int ret = hid_get_indexed_string(device, index, hidString, sizeof(hidString));
                if(ret >= 0)
                {
                    if(is_not_empty_strw(hidString))
                    {
                        char * resultString = (char *) malloc(8192);
                        wcstombs(resultString,  hidString, 8192);
                        stringObject = get_null_string_as_empty(resultString);
                        free(resultString);
                    }
                    else
                    {
                        char empty[] = "";
                        stringObject = get_null_string_as_empty(empty);
                    }
                }
                else
                {
                    DEBUG_PRINT("hid_get_indexed_stringA: error");
                }
                DEBUG_PRINT("hid_get_indexed_stringA success");
            }
            else
            {
                DEBUG_PRINT("hid_get_indexed_stringA: no device");
            }
            return stringObject;
        }

        /*
        * hid_set_nonblocking (hid_device *device, int nonblock)
        */
        FREObject hid_set_nonblockingA(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
        {
            DEBUG_PRINT("hid_set_nonblockingA");

            hid_device * device = get_device_handle(ctx);

            if(device != NULL)
            {
                int32_t nonblock;
                FREResult result = FREGetObjectAsInt32(argv[0], &nonblock);
                DEBUG_RESULT("FREGetObjectAsInt32: nonblock", result);

                int ret = hid_set_nonblocking(device, nonblock);
                if(ret >= 0)
                {
                    DEBUG_PRINT("hid_set_nonblockingA success");
                    return get_result_as_boolean(true);
                }
                else
                {
                    DEBUG_PRINT("hid_set_nonblockingA: error");
                }
            }
            else
            {
                DEBUG_PRINT("hid_set_nonblockingA: no device");
            }
            return get_result_as_boolean(false);
        }

        void contextInitializer(void* extData, const uint8_t* ctxType, FREContext ctx, uint32_t* numFunctions, const FRENamedFunction** functions)
        {
            char msg [1024];
            sprintf(msg, "contextInitializer: %s", (char *)ctxType);
            DEBUG_PRINT(msg);

            *numFunctions = 15;

            FRENamedFunction* func = (FRENamedFunction*) malloc(sizeof(FRENamedFunction) * (*numFunctions));

            func[0].name = (const uint8_t*) "hid_enumerateA";
            func[0].functionData = NULL;
            func[0].function = &hid_enumerateA;

            func[1].name = (const uint8_t*) "hid_open_pathA";
            func[1].functionData = NULL;
            func[1].function = &hid_open_pathA;

            func[2].name = (const uint8_t*) "hid_closeA";
            func[2].functionData = NULL;
            func[2].function = &hid_closeA;

            func[3].name = (const uint8_t*) "hid_send_feature_reportA";
            func[3].functionData = NULL;
            func[3].function = &hid_send_feature_reportA;

            func[4].name = (const uint8_t*) "hid_get_feature_reportA";
            func[4].functionData = NULL;
            func[4].function = &hid_get_feature_reportA;

            func[5].name = (const uint8_t*) "hid_errorA";
            func[5].functionData = NULL;
            func[5].function = &hid_errorA;

            func[6].name = (const uint8_t*) "hid_openA";
            func[6].functionData = NULL;
            func[6].function = &hid_openA;

            func[7].name = (const uint8_t*) "hid_writeA";
            func[7].functionData = NULL;
            func[7].function = &hid_writeA;

            func[8].name = (const uint8_t*) "hid_readA";
            func[8].functionData = NULL;
            func[8].function = &hid_readA;

            func[9].name = (const uint8_t*) "hid_read_timeoutA";
            func[9].functionData = NULL;
            func[9].function = &hid_read_timeoutA;

            func[10].name = (const uint8_t*) "hid_get_manufacturer_stringA";
            func[10].functionData = NULL;
            func[10].function = &hid_get_manufacturer_stringA;

            func[11].name = (const uint8_t*) "hid_get_product_stringA";
            func[11].functionData = NULL;
            func[11].function = &hid_get_product_stringA;

            func[12].name = (const uint8_t*) "hid_get_serial_number_stringA";
            func[12].functionData = NULL;
            func[12].function = &hid_get_serial_number_stringA;

            func[13].name = (const uint8_t*) "hid_get_indexed_stringA";
            func[13].functionData = NULL;
            func[13].function = &hid_get_indexed_stringA;

            func[14].name = (const uint8_t*) "hid_set_nonblockingA";
            func[14].functionData = NULL;
            func[14].function = &hid_set_nonblockingA;

            *functions = func;
        }

        void contextFinalizer(FREContext ctx)
        {
            DEBUG_PRINT("contextFinalizer");
            //close open device if any
            uint32_t argc = 1;
            FREObject argv[1];
            hid_closeA(ctx, NULL, argc, argv);
            return;
        }

        void initializer(void** extData, FREContextInitializer* ctxInitializer, FREContextFinalizer* ctxFinalizer)
        {
            DEBUG_PRINT("initializer");
            *ctxInitializer = &contextInitializer;
            *ctxFinalizer = &contextFinalizer;
        }

        //The runtime calls this function when it unloads an extension. However, the runtime does not guarantee that it will
        //unload the extension or call FREFinalizer().
        void finalizer(void* extData)
        {
            DEBUG_PRINT("finalizer");
            return;
        }
#ifdef __cplusplus
}
#endif