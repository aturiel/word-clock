/** The MIT License (MIT)

Copyright (c) 2018 David Payne

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/******************************************************************************
 * This is designed for the Wemos D1 ESP8266
 * Wemos D1 Mini:  https://amzn.to/2qLyKJd
 * MAX7219 Dot Matrix Module 4-in-1 Display For Arduino
 * Matrix Display:  https://amzn.to/2HtnQlD
 ******************************************************************************/
/******************************************************************************
 * NOTE: The settings here are the default settings for the first loading.  
 * After loading you will manage changes to the settings via the Web Interface.  
 * If you want to change settings again in the settings.h, you will need to 
 * erase the file system on the Wemos or use the “Reset Settings” option in 
 * the Web Interface.
 ******************************************************************************/
 
#include "FS.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
//#include <ESP8266mDNS.h>
#include <WiFiManager.h> // --> https://github.com/tzapu/WiFiManager
#include <ArduinoOTA.h>
#include <NTPClient.h> // --> https://github.com/arduino-libraries/NTPClient
#include <Adafruit_NeoPixel.h> // --> https://github.com/adafruit/Adafruit_NeoPixel

//******************************
// Start Settings
//******************************

const int WEBSERVER_PORT = 80;          // The port you can access this device on over HTTP
const boolean WEBSERVER_ENABLED = true; // Device will provide a web interface via http://[ip]:[port]/
boolean IS_BASIC_AUTH = false;          // Use Basic Authorization for Configuration security on Web Interface
char* www_username = "admin";           // User account for the Web Interface
char* www_password = "WJ8>9n#9Fz9";     // Password for the Web Interface

String timeDisplayTurnsOn = "";   // 24 Hour Format HH:MM -- Leave blank for always on. (ie 05:30)
String timeDisplayTurnsOff = "";  // 24 Hour Format HH:MM -- Leave blank for always on. Both must be set to work.
boolean isRainbowHour = false;    // Display rainbow animation on the hour
boolean isColorsPerMinute = true; // Update colors every minute
boolean isBuzzerEnabled = false;  // Buzzer enabled

boolean ENABLE_OTA = true;  // this will allow you to load firmware to the device over WiFi (see OTA for ESP8266)
String OTA_Password = "";   // Set an OTA password here -- leave blank if you don't want to be prompted for password

//******************************
// End Settings
//******************************
