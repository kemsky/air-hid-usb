air-hid-usb
===========

AIR native extension, wrapper for HIDAPI

General Information

1. Requirements (build): Java runtime, Gradle, MinGW (Windows), MinGW-Utils  (Windows, mingw-util-0.3 since 0.4 is broken), GNU utilities for Win32 (Windows), XCode (Mac OS X)

2. Building extension for both Windows and Mac OS does not make sense, because native installer must be used anyway. This is why extension is build separately for each platform.

3. See application\extensions\unpacked\readme.txt for debug info.

4. HIDAPI info http://www.signal11.us/oss/hidapi/

5. [http://bananas.at.tut.by/hidapi/index.html Documentation]

6. [https://bugs.adobe.com/jira/browse/FP-247 https://bugs.adobe.com/jira/browse/FP-247]

7. [http://code.google.com/p/air-hid-usb/source/browse/trunk/history.txt Version history]

Known Issues

You might experience problems debugging your application using FB on Mac OSX - http://forums.adobe.com/message/4869042, Issue 7 .

Example

    ```ActionScript
              var manager:IHIDManager = HIDFactory.getHIDManager();

              var deviceInfos:Array = manager.getDeviceList(0x8001, 0xA00F);//0x8001, 0xA00F
              var wmouse:HidDeviceInfo;
              for each (var hidDeviceInfo:HidDeviceInfo in deviceInfos)
              {
                  if (hidDeviceInfo.interface_number == 1)
                  {
                      wmouse = hidDeviceInfo;
                      break;
                  }
              }
              if (wmouse)
              {
                  log.info("getDeviceByPath {0}", wmouse);
                  var device:IHIDDevice = manager.getDeviceByPath(wmouse.path);
                  if (device.open())
                  {
                      log.info("getManufacturerString: {0}", device.getManufacturerString());
                      log.info("getProductString: {0}", device.getProductString());
                      log.info("getSerialNumberString: {0}", device.getSerialNumberString());
                      device.close();
                  }
                  device.dispose();
              }
    ```

Demo Windows 8

[http://air-hid-usb.googlecode.com/files/windows.png]

Demo Mac OS 10.6.7

[http://air-hid-usb.googlecode.com/files/macos.png]
