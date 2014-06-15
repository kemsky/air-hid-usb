/*
 * Copyright: (c) 2012. Turtsevich Alexander
 *
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.html
 */
package com.hidapi
{
    import flash.events.StatusEvent;
    import flash.external.ExtensionContext;
    import flash.utils.ByteArray;

    import mx.logging.ILogger;
    import mx.logging.Log;

    /**
     * @private
     * Provides common methods to access/manipulate HID devices
     * direct mapping to air.cpp functions
     * @see http://www.signal11.us/oss/hidapi/hidapi/doxygen/html/group__API.html
     */
    internal class Hid
    {
        private static const CONTEXT:String = "com.hidapi.Hid";

        private static const log:ILogger = Log.getLogger(CONTEXT);

        /**
         * @private
         */
        private var _context:ExtensionContext;

        /**
         * Creates context
         * @param contextType not used
         */
        public function Hid(contextType:String = "Hid")
        {
            try
            {
                log.debug("Creating context: {0}, contextType: {1}", CONTEXT, contextType);
                _context = ExtensionContext.createExtensionContext(CONTEXT, contextType);
                if (_context == null)
                {
                    log.error("Context is null");
                }
                else
                {
                    log.debug("Context was created successfully");
                    _context.addEventListener(StatusEvent.STATUS, onStatus);
                    _context.actionScriptData = 1;
                }
            }
            catch(e:Error)
            {
                log.error("Failed to create context: {0}", e.getStackTrace());
            }
        }

        /*todo: create device detection based on RegisterDeviceNotification
                and hidden message-only window in dll
                FREResult FREDispatchStatusEventAsync(
                FREContext ctx,
                const uint8_t*  code,
                const uint8_t*  level
                );
                but it would be windows limited solution...
        */
        private function onStatus(event:StatusEvent):void
        {
            log.info("Status event received: level={1}, code={0}", event.code, event.level);
        }


        private function get contextCreated():Boolean
        {
            return _context != null;
        }

        /**
         * Enumerate the HID Devices.
         * @param productId HID device productId
         * @param vendorId  HID device vendorId
         * @param result array of HidDeviceInfo
         * @return operation status
         * @see com.hidapi.HidDeviceInfo
         */
        public function hid_enumerate(result:Array, productId:uint = 0x0, vendorId:uint = 0x0):Boolean
        {
            if(!contextCreated)
                return false;

            if (result == null)
            {
                log.warn("hid_enumerate result argument is null");
                result = [];
            }

            _context.call('hid_enumerateA', result, productId, vendorId);
            return true;
        }

        /**
         * Open a HID device using a Vendor ID (VID), Product ID (PID) and optionally a serial number.
         * @param productId The Vendor ID (VID) of the types of device to open
         * @param vendorId  The Product ID (PID) of the types of device to open
         * @param serial_number The Serial Number of the device to open (Optionally NULL)
         * @return operation status
         */
        public function hid_open(productId:uint, vendorId:uint, serial_number:String = null):Boolean
        {
            if(!contextCreated)
                return false;

            var status:Boolean = _context.call('hid_openA', productId, vendorId, serial_number) as Boolean;
            if (!status)
            {
                log.error("Error: hid_open(productId={0}, vendorId={1}, serial_number: {2})", productId, vendorId, serial_number);
                logError();
            }
            return status;
        }

        /**
         * Open a HID device using a path. This allows to select to open one of few devices with the same pid/vid.
         * @param path     The path name of the device to open
         * @return operation status
         */
        public function hid_open_path(path:String):Boolean
        {
            if(!contextCreated)
                return false;

            var status:Boolean = _context.call('hid_open_pathA', path) as Boolean;
            if (!status)
            {
                log.error("Error: hid_open_path(path={0})", path);
                logError();
            }
            return status;
        }

        /**
         * Close a HID device.
         * @return operation status
         */
        public function hid_close():Boolean
        {
            if(!contextCreated)
                return false;

            var status:Boolean = _context.call('hid_closeA') as Boolean;
            if (!status)
            {
                log.error("Error: hid_close()");
            }
            return status;
        }

        /**
         * This function returns a string containing the last error which occurred or NULL if none has occurred.
         * @return error message or "" (empty string) if there is no errors
         */
        public function hid_error():String
        {
            if(!contextCreated)
                return "Context is null";

            return _context.call('hid_errorA') as String;
        }

        /**
         * Send a Feature report to the device.
         * @param data
         * @return operation status
         */
        public function hid_send_feature_report(data:ByteArray):int
        {
            if(!contextCreated)
                return -1;

            var status:int = _context.call('hid_send_feature_reportA', data) as int;
            if (status < 0)
            {
                log.error("Error: hid_send_feature_report()");
                logError();
            }
            return status;
        }

        /**
         * Get a feature report from a HID device.
         * @param data
         * @return
         */
        public function hid_get_feature_report(data:ByteArray):int
        {
            if(!contextCreated)
                return -1;

            var status:int = _context.call('hid_get_feature_reportA', data) as int;
            if (status < 0)
            {
                log.error("Error: hid_send_feature_report()");
                logError();
            }
            return status;
        }

        /**
         * Write an Output report to a HID device
         * @param data
         * @return operation status
         */
        public function hid_write(data:ByteArray):int
        {
            if(!contextCreated)
                return -1;

            var status:int = _context.call('hid_writeA', data) as int;
            if (status < 0)
            {
                log.error("Error: hid_write()");
                logError();
            }
            return status;
        }

        /**
         * Get a feature report from a HID device.
         * @param data
         * @return operation status
         */
        public function hid_read(data:ByteArray):int
        {
            if(!contextCreated)
                return -1;

            var status:int = _context.call('hid_readA', data) as int;
            if (status < 0)
            {
                log.error("Error: hid_read()");
                logError();
            }
            return status;
        }

        /**
         * Read an Input report from a HID device with timeout.
         * @param data
         * @param millis timeout in milliseconds
         * @return operation status
         */
        public function hid_read_timeout(data:ByteArray, millis:int):int
        {
            if(!contextCreated)
                return -1;

            var status:int = _context.call('hid_read_timeoutA', data, millis) as int;
            if (status < 0)
            {
                log.error("Error: hid_read_timeoutA(timeout={0})", millis);
                logError();
            }
            return status;
        }

        /**
         * Get The Manufacturer String from a HID device.
         * @return string or null on error
         */
        public function hid_get_manufacturer_string():String
        {
            if(!contextCreated)
                return null;

            var result:String = _context.call('hid_get_manufacturer_stringA') as String;
            if(result == null)
            {
                log.error("Error: hid_get_manufacturer_stringA()");
                logError();
            }
            return result;
        }

        /**
         * Get The Product String from a HID device.
         * @return string or null on error
         */
        public function hid_get_product_string():String
        {
            if(!contextCreated)
                return null;

            var result:String = _context.call('hid_get_product_stringA') as String;
            if(result == null)
            {
                log.error("Error: hid_get_product_stringA()");
                logError();
            }
            return result;
        }

        /**
         * Get The Serial Number String from a HID device.
         * @return string or null on error
         */
        public function hid_get_serial_number_string():String
        {
            if(!contextCreated)
                return null;

            var result:String = _context.call('hid_get_serial_number_stringA') as String;
            if(result == null)
            {
                log.error("Error: hid_get_serial_number_stringA()");
                logError();
            }
            return result;
        }

        /**
         * Get a string from a HID device, based on its string index.
         * @param index string index
         * @return string  or null on error
         */
        public function hid_get_indexed_string(index:int):String
        {
            if(!contextCreated)
                return null;

            var result:String = _context.call('hid_get_indexed_stringA', index) as String;
            if(result == null)
            {
                log.error("Error: hid_get_indexed_stringA(index={0})", index);
                logError();
            }
            return result;
        }

        /**
         * Set the device handle to be non-blocking.In non-blocking mode calls to hid_read()
         * will return immediately with a value of 0 if there is no data to be read.
         * In blocking mode, hid_read() will wait (block) until there is data to read before returning.
         * Nonblocking can be turned on and off at any time.
         * @param nonblock string nonblock
         * @return string  or null on error
         */
        public function hid_set_nonblocking(nonblock:int):Boolean
        {
            if(!contextCreated)
                return false;

            var result:Boolean = _context.call('hid_set_nonblockingA', nonblock) as Boolean;
            if(result)
            {
                log.error("Error: hid_set_nonblockingA(nonblock={0})", nonblock);
                logError();
            }
            return result;
        }

        private function logError():void
        {
            var err:String = hid_error();
            if (err && err.length > 0)
            {
                log.error("HID error: {0}", err);
            }
        }

        /**
         * Performs clean-up
         */
        public function dispose():void
        {
            if (_context)
            {
                _context.dispose();
                //clean all references
                _context.removeEventListener(StatusEvent.STATUS, onStatus);
                _context = null;
            }
            else
            {
                log.error("Can not dispose: Context is null");
            }
        }
    }
}
