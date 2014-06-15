/*
 * Copyright: (c) 2014. Turtsevich Alexander
 *
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.html
 */
package com.hidapi
{
    import flash.utils.ByteArray;

    /**
     * HID device instance, wraps all HIDAPI methods.
     * @see com.hidapi.IHIDDeviceInfo
     * @see http://www.signal11.us/oss/hidapi/hidapi/doxygen/html/group__API.html HIDAPI Documentation
     */
    public interface IHIDDevice
    {
        /**
         * Open device. <b>Device should be disposed even if it failed to open</b>
         * @return operation status
         */
        function open():Boolean;

        /**
         * Returns true if device is opened (open called), false in other case
         * @return true if device is opened, false in other case
         */
        function get isOpened():Boolean;

        /**
         * Return device info or null if device has not been opened (or open call failed).
         * @see com.hidapi.IHIDDeviceInfo
         */
        function get deviceDescriptor():IHIDDeviceInfo;

        /**
         * Return true if device is disposed, false in other case
         * @return true if device is disposed, false in other case
         */
        function get isDisposed():Boolean;

        /**
         * Close device
         * @return operation status
         */
        function close():Boolean;

        /**
         * This function returns a string containing the last error which occurred or NULL if none has occurred.
         * @return error message or "" (empty string) if there were no errors
         */
        function getLastError():String;

        /**
         * Close device (if not closed) and disposes context.
         * Device can not be used anymore after this operation.
         */
        function dispose():void;

        /**
         * Send a Feature report to the device.
         * @param data
         * @return bytes processed or -1 on error
         */
        function sendFeatureReport(data:ByteArray):int;

        /**
         * Get a feature report from a HID device.
         * @param data
         * @return bytes processed or -1 on error
         */
        function getFeatureReport(data:ByteArray):int;


        /**
         * Write an Output report to a HID device
         * @param data
         * @return bytes processed or -1 on error
         */
        function write(data:ByteArray):int;

        /**
         * Get a feature report from a HID device.
         * @param data
         * @return bytes processed or -1 on error
         */
        function read(data:ByteArray):int;

        /**
         * Read an Input report from a HID device with timeout.
         * @param data
         * @param millis timeout in milliseconds
         * @return bytes processed or -1 on error
         */
        function readTimeout(data:ByteArray, millis:int):int;

        /**
         * Get The Manufacturer String from a HID device.
         * @return string or null on error
         */
        function getManufacturerString():String;

        /**
         * Get The Product String from a HID device.
         * @return string or null on error
         */
        function getProductString():String;

        /**
         * Get The Serial Number String from a HID device.
         * @return string or null on error
         */
        function getSerialNumberString():String;

        /**
         * Get a string from a HID device, based on its string index.
         * @param index string index
         * @return string or null on error
         */
        function getIndexedString(index:int):String;

        /**
         * Set the device handle to be non-blocking.In non-blocking mode calls to hid_read()
         * will return immediately with a value of 0 if there is no data to be read.
         * In blocking mode, hid_read() will wait (block) until there is data to read before returning.
         * Nonblocking can be turned on and off at any time.
         * @param nonblock string nonblock
         * @return operation status
         */
        function setNonblocking(nonblock:int):Boolean;
    }
}
