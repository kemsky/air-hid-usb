/*
 * Copyright: (c) 2014. Turtsevich Alexander
 *
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.html
 */
package com.hidapi
{
    /**
     * HIDManager interface
     * @see com.hidapi.IHIDDevice
     * @see com.hidapi.HIDFactory
     */
    public interface IHIDManager
    {
        /**
         * Get a HID device instance using a Vendor ID (VID), Product ID (PID) and optionally a serial number.
         * @param productId The Vendor ID (VID) of the types of device to open
         * @param vendorId  The Product ID (PID) of the types of device to open
         * @param serial_number The Serial Number of the device to open (Optionally NULL)
         * @return device instance
         * @see com.hidapi.IHIDDevice
         */
        function getDevice(productId:uint, vendorId:uint, serial_number:String = null):IHIDDevice;

        /**
         * Get a HID device instance using a path. This allows to select to open one of few devices with the same pid/vid.
         * @param path     The path name of the device to open
         * @return device instance
         * @see com.hidapi.IHIDDevice
         */
        function getDeviceByPath(path:String):IHIDDevice;

        /**
         * Enumerate the HID Devices.
         * @param productId HID device productId
         * @param vendorId  HID device vendorId
         * @return array of IHidDeviceInfo if no errors happened, null in other case.
         * @see com.hidapi.IHIDDeviceInfo
         */
        function getDeviceList(productId:uint = 0x0, vendorId:uint = 0x0):Array;
    }
}
