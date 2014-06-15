/*
 * Copyright: (c) 2012. Turtsevich Alexander
 *
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.html
 */
package com.hidapi
{
    /**
     * HID device descriptor
     * its public because instances are created in native code.
     * @private
     */
    public class HidDeviceInfo implements IHIDDeviceInfo
    {
        private var _manufacturer_string:String = null;

        private var _product_string:String = null;

        private var _path:String = null;

        private var _productId:uint = 0;

        private var _vendorId:uint = 0;

        private var _serial_number:String = null;

        private var _release_number:uint = 0;

        private var _usage_page:uint = 0;

        private var _usage:uint = 0;

        private var _interface_number:int = 0;

        public function HidDeviceInfo()
        {
        }

        /**
         * @inheritDoc
         */
        public function get manufacturer_string():String
        {
            return _manufacturer_string;
        }

        public function set manufacturer_string(value:String):void
        {
            _manufacturer_string = value;
        }

        /**
         * @inheritDoc
         */
        public function get product_string():String
        {
            return _product_string;
        }

        public function set product_string(value:String):void
        {
            _product_string = value;
        }

        /**
         * @inheritDoc
         */
        public function get path():String
        {
            return _path;
        }

        public function set path(value:String):void
        {
            _path = value;
        }

        /**
         * @inheritDoc
         */
        public function get productId():uint
        {
            return _productId;
        }

        public function set productId(value:uint):void
        {
            _productId = value;
        }

        /**
         * @inheritDoc
         */
        public function get vendorId():uint
        {
            return _vendorId;
        }

        public function set vendorId(value:uint):void
        {
            _vendorId = value;
        }

        /**
         * @inheritDoc
         */
        public function get serial_number():String
        {
            return _serial_number;
        }

        public function set serial_number(value:String):void
        {
            _serial_number = value;
        }

        /**
         * @inheritDoc
         */
        public function get release_number():uint
        {
            return _release_number;
        }

        public function set release_number(value:uint):void
        {
            _release_number = value;
        }

        /**
         * @inheritDoc
         */
        public function get usage_page():uint
        {
            return _usage_page;
        }

        public function set usage_page(value:uint):void
        {
            _usage_page = value;
        }

        /**
         * @inheritDoc
         */
        public function get usage():uint
        {
            return _usage;
        }

        public function set usage(value:uint):void
        {
            _usage = value;
        }

        /**
         * @inheritDoc
         */
        public function get interface_number():int
        {
            return _interface_number;
        }

        public function set interface_number(value:int):void
        {
            _interface_number = value;
        }

        public function toString():String
        {
            return "IHIDDeviceInfo{manufacturer_string=" + String(manufacturer_string) +
                    ",\nproduct_string=" +  String(product_string) +
                    ",\npath=" + String(path) +
                    ",\nproductId=" + String(productId.toString(16)) +
                    ",\nvendorId=" + String(vendorId.toString(16)) +
                    ",\nserial_number=" + String(serial_number) +
                    ",\nrelease_number=" + String(release_number) +
                    ",\nusage_page=" + String(usage_page) +
                    ",\nusage=" + String(usage) +
                    ",\ninterface_number=" + String(interface_number) +
                    "}";
        }
    }
}
