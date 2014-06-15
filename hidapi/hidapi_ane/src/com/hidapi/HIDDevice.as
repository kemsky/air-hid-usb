/*
 * Copyright: (c) 2014. Turtsevich Alexander
 *
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.html
 */
package com.hidapi
{
    import flash.utils.ByteArray;

    import mx.logging.ILogger;
    import mx.logging.Log;

    /**
     * @private
     */
    internal class HIDDevice implements IHIDDevice
    {
        private static const log:ILogger = Log.getLogger("com.hidapi.HIDDevice");

        private var hid:Hid = null;

        private var productId:uint = 0;
        private var vendorId:uint = 0;
        private var serial_number:String = null;
        private var path:String = null;

        private var _isOpened:Boolean = false;

        private var _deviceDescriptor:IHIDDeviceInfo = null;

        private var manager:HIDManager = null;

        private var notFoundError:String = null;

        public function HIDDevice(hid:Hid, path:String, productId:uint, vendorId:uint, serial_number:String, manager:HIDManager)
        {
            this.hid = hid;
            this.path = path;
            this.productId = productId;
            this.vendorId = vendorId;
            this.serial_number = serial_number;
            this.manager = manager;
        }

        /**
         * @inheritDoc
         */
        public function get isOpened():Boolean
        {
            return _isOpened && !isDisposed;
        }

        /**
         * @inheritDoc
         */
        public function set isOpened(value:Boolean):void
        {
            _isOpened = value;
        }

        public function get isDisposed():Boolean
        {
            return hid == null;
        }

        /**
         * @inheritDoc
         */
        public function open():Boolean
        {
            if (!isOpened)
            {
                var deviceInfos:Array = [];
                var result:Boolean = hid.hid_enumerate(deviceInfos, productId, vendorId);

                if (result)
                {
                    var devicesToOpen:Array = deviceInfos.filter(deviceFilter, null);//null if class method is used

                    var foundDevice:IHIDDeviceInfo = IHIDDeviceInfo(devicesToOpen.length > 0 ? devicesToOpen[0] : null);

                    if (foundDevice != null)
                    {
                        log.debug("opening device: {0}", foundDevice);
                        isOpened = hid.hid_open_path(foundDevice.path);
                        if (isOpened)
                        {
                            this._deviceDescriptor = foundDevice;
                            if (devicesToOpen.length > 1)
                            {
                                log.warn("Found {0} devices, but opened first", devicesToOpen.length);
                            }
                            notFoundError = null;
                        }
                    }
                    else
                    {
                        notFoundError = "Device is not found";
                        log.error(notFoundError);
                    }
                }
            }
            else
            {
                log.warn("Device has been already opened: {0}", this)
            }
            return isOpened;
        }

        /*
         * the same filter as HIDAPI has.
         */
        private function deviceFilter(deviceInfo:IHIDDeviceInfo, index:int, array:Array):Boolean
        {
            var isPathEqual:Boolean = path != null && path == deviceInfo.path;
            var isSerialEqual:Boolean = serial_number == null || serial_number == deviceInfo.serial_number;
            return isPathEqual || (path == null && productId == deviceInfo.productId && vendorId == deviceInfo.vendorId && isSerialEqual);
        }


        public function get deviceDescriptor():IHIDDeviceInfo
        {
            return _deviceDescriptor;
        }

        /**
         * @inheritDoc
         */
        public function sendFeatureReport(data:ByteArray):int
        {
            return isOpened ? hid.hid_send_feature_report(data)  : -1;
        }

        /**
         * @inheritDoc
         */
        public function getFeatureReport(data:ByteArray):int
        {
            return isOpened ? hid.hid_get_feature_report(data) : -1;
        }

        public function write(data:ByteArray):int
        {
            return isOpened ? hid.hid_write(data) : -1;
        }

        /**
         * @inheritDoc
         */
        public function read(data:ByteArray):int
        {
            return isOpened ? hid.hid_read(data) : -1;
        }

        /**
         * @inheritDoc
         */
        public function readTimeout(data:ByteArray, millis:int):int
        {
            return isOpened ? hid.hid_read_timeout(data, millis) : -1;
        }

        /**
         * @inheritDoc
         */
        public function getManufacturerString():String
        {
            return isOpened ? hid.hid_get_manufacturer_string() : null;
        }

        /**
         * @inheritDoc
         */
        public function getProductString():String
        {
            return isOpened ? hid.hid_get_product_string() : null;
        }

        /**
         * @inheritDoc
         */
        public function getSerialNumberString():String
        {
            return isOpened ? hid.hid_get_serial_number_string() : null;
        }

        /**
         * @inheritDoc
         */
        public function getIndexedString(index:int):String
        {
            return isOpened ? hid.hid_get_indexed_string(index) : null;
        }

        /**
         * @inheritDoc
         */
        public function close():Boolean
        {
            var result:Boolean = true;
            if (isOpened)
            {
                result = hid.hid_close();
                isOpened = !result;
                if(!isOpened)
                {
                    log.debug("Closed: {0}", deviceDescriptor);
                }
            }
            else
            {
                log.warn("Device has been already closed: {0}", this)
            }
            return result;
        }

        /**
         * @inheritDoc
         */
        public function getLastError():String
        {
            var result:String = notFoundError != null ? notFoundError : "";
            if (isOpened)
            {
                result = hid.hid_error();
            }
            else if (isDisposed)
            {
                result = "Context was disposed";
            }
            return result;
        }


        public function setNonblocking(nonblock:int):Boolean
        {
            return isOpened ? hid.hid_set_nonblocking(nonblock) : false;
        }

        /**
         * @inheritDoc
         */
        public function dispose():void
        {
            if (isOpened)
            {
                log.warn("Disposing opened device: {0}", this);
                close();
            }

            if (!isDisposed)
            {
                _deviceDescriptor = null;
                manager = null;
                hid.dispose();
                hid = null;
            }
            else
            {
                log.warn("Device has been already disposed: {0}", this);
            }
        }


        public function toString():String
        {
            return "HIDDevice{productId=0x" + productId.toString(16) +
                    ",\nvendorId=0x" + vendorId.toString(16) +
                    ",\nserial_number=" + String(serial_number) +
                    ",\npath=" + String(path) +
                    ",\nisOpened=" + String(_isOpened) + "}";
        }
    }
}
