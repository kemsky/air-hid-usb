/*
 * Copyright: (c) 2012. Turtsevich Alexander
 *
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.html
 */
package com.hidapi
{
    /**
     * HID device descriptor
     * @see com.hidapi.IHIDManager
     */
    public interface IHIDDeviceInfo
    {
        /**
         * Manufacturer String.
         */
        function get manufacturer_string():String;

        /**
         * Product string.
         */
        function get product_string():String;

        /**
         * Platform-specific device path.
         */
        function get path():String;

        /**
         * Device Product ID.
         */
        function get productId():uint;

        /**
         * Device Vendor ID.
         */
        function get vendorId():uint;

        /**
         * Serial Number.
         */
        function get serial_number():String;

        /**
         * Device Release Number in binary-coded decimal,
         * also known as Device Version Number.
         */
        function get release_number():uint;

        /**
         * Usage Page for this Device/Interface
         * (Windows/Mac only).
         */
        function get usage_page():uint;

        /**
         * Usage for this Device/Interface
         * (Windows/Mac only).
         */
        function get usage():uint;

        /**
         * The USB interface which this logical device
         * represents. Valid on both Linux implementations
         * in all cases, and valid on the Windows implementation
         * only if the device contains more than one interface.
         */
        function get interface_number():int;
    }
}
