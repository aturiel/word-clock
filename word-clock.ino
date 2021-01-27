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

/**********************************************
  Edit Settings.h for personalization
***********************************************/

#include "Settings.h"
#define VERSION "2.15"

#define HOSTNAME "WORD-CLOCK-"
#define CONFIG "/conf.txt"
#define BUZZER_PIN  D5

/* Useful Constants */
#define SECS_PER_MIN  (60UL)
#define SECS_PER_HOUR (3600UL)
#define SECS_PER_DAY  (SECS_PER_HOUR * 24L)

/* Useful Macros for getting elapsed time */
#define numberOfSeconds(_time_) (_time_ % SECS_PER_MIN)
#define numberOfMinutes(_time_) ((_time_ / SECS_PER_MIN) % SECS_PER_MIN)
#define numberOfHours(_time_) (( _time_% SECS_PER_DAY) / SECS_PER_HOUR)
#define elapsedDays(_time_) ( _time_ / SECS_PER_DAY)

//declairing prototypes
void configModeCallback (WiFiManager *myWiFiManager);
int8_t getWifiQuality();

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

String wordClockSimulated = "";
//
long lastEpoch = 0;
long firstEpoch = 0;

long displayOffEpoch = 0;
boolean displayOn = true;

ESP8266WebServer server(WEBSERVER_PORT);
ESP8266HTTPUpdateServer serverUpdater;

static const char WEB_ACTIONS1[] PROGMEM = "<a class='w3-bar-item w3-button' href='/'><i class='fas fa-home'></i> Home</a>"
    "<a class='w3-bar-item w3-button' href='/configure'><i class='fas fa-cog'></i> Configure</a>";

static const char WEB_ACTIONS2[] PROGMEM = "<a class='w3-bar-item w3-button' href='/pull'><i class='fas fa-cloud-download-alt'></i> Refresh Data</a>"
    "<a class='w3-bar-item w3-button' href='/display'>";

static const char WEB_ACTION3[] PROGMEM = "</a><a class='w3-bar-item w3-button' href='#' onClick='confirm(\"Are you sure your want to reboot?\") && window.location.replace(\"/reboot\");'><i class='fas fa-power-off'></i> Reboot</a>"
    "<a class='w3-bar-item w3-button' href='/systemreset' onclick='return confirm(\"Do you want to reset to default settings?\")'><i class='fas fa-undo'></i> Reset Settings</a>"
    "<a class='w3-bar-item w3-button' href='/forgetwifi' onclick='return confirm(\"Do you want to forget to WiFi connection?\")'><i class='fas fa-wifi'></i> Forget WiFi</a>"
    "<a class='w3-bar-item w3-button' href='/update'><i class='fas fa-wrench'></i> Firmware Update</a>";
//                     "<a class='w3-bar-item w3-button' href='https://github.com/Qrome/marquee-scroller' target='_blank'><i class='fas fa-question-circle'></i> About</a>";
//                     "<a class='w3-bar-item w3-button' href='/reboot'><i class='fas fa-power-off'></i> Reboot</a>"

static const char CHANGE_FORM3[] PROGMEM = "<hr><p><input name='isBasicAuth' class='w3-check w3-margin-top' type='checkbox' %IS_BASICAUTH_CHECKED%> Use Security Credentials for Configuration Changes</p>"
    "<p><label>Word-Clock User ID (for this web interface)</label><input class='w3-input w3-border w3-margin-bottom' type='text' name='userid' value='%USERID%' maxlength='20'></p>"
    "<p><label>Word-Clock Password </label><input class='w3-input w3-border w3-margin-bottom' type='password' name='stationpassword' value='%STATIONPASSWORD%'></p>"
    "<p><button class='w3-button w3-block w3-green w3-section w3-padding' type='submit'>Save</button></p></form>"
    "<script>function isNumberKey(e){var h=e.which?e.which:event.keyCode;return!(h>31&&(h<48||h>57))}</script>";


// Change the externalLight to the pin you wish to use if other than the Built-in LED
int externalLight = LED_BUILTIN; // LED_BUILTIN is is the built in LED on the Wemos

// Which pin on the Arduino is connected to the NeoPixels?
#define LED_PIN     D4

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT  64

// NeoPixel brightness, 0 (min) to 255 (max)
#define BRIGHTNESS 50 // Set BRIGHTNESS to about 1/5 (max = 255)

// NeoPixel library
Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int8_t nowHour;
int8_t nowMinute;
int8_t minuteRounded;
int8_t hourCompensated;

int TOTAL_ROWS = 8;
int TOTAL_COLS = 8;
int display[8][8] = {0};
char text[8][8] = {
  {'D', 'I', 'O', 'N', 'C', 'E', 'Z', 'S'},
  {'N', 'U', 'S', 'I', 'E', 'T', 'V', 'E'},
  {'C', 'I', 'N', 'O', 'C', 'H', 'O', 'I'},
  {'C', 'U', 'A', 'T', 'R', 'E', 'O', 'S'},
  {'Y', 'M', 'E', 'N', 'O', 'S', 'M', 'E'},
  {'C', 'U', 'A', 'R', 'T', 'O', 'D', 'I'},
  {'V', 'E', 'I', 'N', 'T', 'I', 'E', 'A'},
  {'D', 'C', 'I', 'N', 'E', 'Z', 'C', 'O'},
};

//
void setup() {
  // Initialize digital pin for LED
  pinMode(externalLight, OUTPUT);
  digitalWrite (externalLight, HIGH); // Switch off LED

  // Init leds to turn off
  leds.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  leds.show(); // Turn OFF all pixels ASAP
  leds.setBrightness(BRIGHTNESS);

  Serial.begin (115200);

  //New Line to clear from start garbage
  Serial.println();

  // start-up chime
  /*
    tone(BUZZER_PIN, 415, 500);
    delay(500 * 1.3);
    tone(BUZZER_PIN, 466, 500);
    delay(500 * 1.3);
    tone(BUZZER_PIN, 370, 1000);
    delay(1000 * 1.3);
    noTone(BUZZER_PIN);
  */

  ledsIntro();
  leds.show(); // Turn OFF all pixels ASAP

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  // Uncomment for testing wifi manager
  //wifiManager.resetSettings();
  wifiManager.setAPCallback(configModeCallback);

  //Custom Station (client) Static IP Configuration - Set custom IP for your Network (IP, Gateway, Subnet mask)
  //wifiManager.setSTAStaticIPConfig(IPAddress(192,168,0,99), IPAddress(192,168,0,1), IPAddress(255,255,255,0));

  String hostname(HOSTNAME);
  hostname += String(ESP.getChipId(), HEX);
  if (!wifiManager.autoConnect((const char *)hostname.c_str())) {// new addition
    delay(3000);
    WiFi.disconnect(true);
    ESP.reset();
    delay(5000);
  } else {
    timeClient.begin();
  }

  // print the received signal strength:
  Serial.print("Signal Strength (RSSI): ");
  Serial.print(getWifiQuality());
  Serial.println("%");

  if (ENABLE_OTA) {
    ArduinoOTA.onStart([]() {
      Serial.println("Start");
    });
    ArduinoOTA.onEnd([]() {
      Serial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });
    ArduinoOTA.setHostname((const char *)hostname.c_str());
    if (OTA_Password != "") {
      ArduinoOTA.setPassword(((const char *)OTA_Password.c_str()));
    }
    ArduinoOTA.begin();
  }

  if (WEBSERVER_ENABLED) {
    server.on("/", displayHomeData);
    server.on("/pull", handlePull);
    server.on("/systemreset", handleSystemReset);
    server.on("/forgetwifi", handleForgetWifi);
    server.on("/configure", handleConfigure);
    server.on("/display", handleDisplay);
    server.on("/reboot", handleReboot);
    server.onNotFound(redirectHome);
    serverUpdater.setup(&server, "/update", www_username, www_password);
    // Start the server
    server.begin();
    Serial.println("Server started");
    // Print the IP address
    String webAddress = "http://" + WiFi.localIP().toString() + ":" + String(WEBSERVER_PORT) + "/";
    Serial.println("Use this URL : " + webAddress);
    Serial.println(" v" + String(VERSION) + "  IP:" + WiFi.localIP().toString() + "  ");
  } else {
    Serial.println("Web Interface is Disabled");
  }

  flashLED(1, 500);
}

//************************************************************
// Main Looop
//************************************************************
void loop() {

  static int last = 0;

  //Get some updated data to serve
  if ((getMinutesFromLastRefresh() >= minutesBetweenDataRefresh) || lastEpoch == 0) {
    updateData();
  }

  checkDisplay(); // this will see if we need to turn it on or off for night mode.

  if ((millis() - last) > 5000) {
    last = millis();
    nowHour = timeClient.getHours() % 12;
    nowMinute = timeClient.getMinutes();
    //daysOfTheWeek[timeClient.getDay()]);
    //timeClient.getSeconds());

    // round minutes always to five minutes after
    int roundMinutes = nowMinute % 5;
    if ( roundMinutes != 0) {
      if (roundMinutes > 2) {
        roundMinutes = 5 - roundMinutes;
      } else {
        roundMinutes *= -1;
      }
    }
    minuteRounded = nowMinute + roundMinutes;

    if (minuteRounded > 30) {
      hourCompensated = (nowHour + 1) % 12;
    } else {
      hourCompensated = nowHour;
    }

    if(displayOn) {
      displayWordTime();
    }
  }

  //
  if (WEBSERVER_ENABLED) {
    server.handleClient();
  }

  if (ENABLE_OTA) {
    ArduinoOTA.handle();
  }
}

boolean athentication() {
  if (IS_BASIC_AUTH) {
    return server.authenticate(www_username, www_password);
  }
  return true; // Authentication not required
}

void handlePull() {
  updateData();
}

void displayHomeData() {
  String html = "";

  server.sendHeader("Cache-Control", "no-cache, no-store");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/html", "");
  sendHeader();

  Serial.println(timeClient.getFormattedTime());
  html += "<p>" + timeClient.getFormattedTime() + "</p>";

  html += "<p>" + String(nowHour) + ":" + String(nowMinute) + "</p>";
  html += "<p> rounded minute " + String(minuteRounded) + "</p>";
  html += "<p> compensated hour " + String(hourCompensated) + "</p>";
  html += "<hr>";
  html += "<code>" + wordClockSimulated + "</code>";

  server.sendContent(String(html)); // spit out what we got

  sendFooter();
  server.sendContent("");
  server.client().stop();
  digitalWrite(externalLight, HIGH);
}

void handleSystemReset() {
  if (!athentication()) {
    return server.requestAuthentication();
  }
  
  Serial.println("Reset System Configuration");
  
  if (SPIFFS.remove(CONFIG)) {
    redirectHome();
    ESP.restart();
  }
}

void handleForgetWifi() {
  if (!athentication()) {
    return server.requestAuthentication();
  }
  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  redirectHome();
  WiFiManager wifiManager;
  wifiManager.resetSettings();
  ESP.restart();
}

void handleConfigure() {
  if (!athentication()) {
    return server.requestAuthentication();
  }
  digitalWrite(externalLight, LOW);
  String html = "";

  server.sendHeader("Cache-Control", "no-cache, no-store");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/html", "");

  sendHeader();

  String form = FPSTR(CHANGE_FORM3);

  String isUseSecurityChecked = "";
  if (IS_BASIC_AUTH) {
    isUseSecurityChecked = "checked='checked'";
  }
  form.replace("%IS_BASICAUTH_CHECKED%", isUseSecurityChecked);
  form.replace("%USERID%", String(www_username));
  form.replace("%STATIONPASSWORD%", String(www_password));

  server.sendContent(form); // Send the second chunk of Data

  sendFooter();

  server.sendContent("");
  server.client().stop();
  digitalWrite(externalLight, HIGH);
}

void handleReboot() {
  ESP.restart();
}

void handleDisplay() {
  if (!athentication()) {
    return server.requestAuthentication();
  }
  enableDisplay(!displayOn);
}

//***********************************************************************
void updateData() {
  Serial.println("Updating Time...");
  timeClient.update();

  lastEpoch = now();
  
  if (firstEpoch == 0) {
    firstEpoch = now();
    Serial.println("firstEpoch is: " + String(firstEpoch));
  }

}

void redirectHome() {
  // Send them back to the Root Directory
  server.sendHeader("Location", String("/"), true);
  server.sendHeader("Cache-Control", "no-cache, no-store");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send(302, "text/plain", "");
  server.client().stop();
  delay(1000);
}

void sendHeader() {
  String html = "<!DOCTYPE HTML>";
  html += "<html><head><title>Word-Clock</title><link rel='icon' href='data:;base64,='>";
  html += "<meta http-equiv='Content-Type' content='text/html; charset=UTF-8' />";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<link rel='stylesheet' href='https://www.w3schools.com/w3css/4/w3.css'>";
  html += "<link rel='stylesheet' href='https://www.w3schools.com/lib/w3-theme-blue-grey.css'>";
  html += "<link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.8.1/css/all.min.css'>";
  html += "<style>";
  html += "div.word-clock {text-align: center;}";
  html += "span.letter {font:'monospaced'; font-size: 150%; color:lightgrey; padding: 2px 4px;}";
  html += "span.selected {font-weight: bold; color:black;}";
  html += "</style>";
  html += "</head><body>";
  server.sendContent(html);
  html = "<nav class='w3-sidebar w3-bar-block w3-card' style='margin-top:88px' id='mySidebar'>";
  html += "<div class='w3-container w3-theme-d2'>";
  html += "<span onclick='closeSidebar()' class='w3-button w3-display-topright w3-large'><i class='fas fa-times'></i></span>";
  html += "<div class='w3-padding'>Menu</div></div>";
  server.sendContent(html);

  server.sendContent(FPSTR(WEB_ACTIONS1));
  server.sendContent(FPSTR(WEB_ACTIONS2));
  if (displayOn) {
    server.sendContent("<i class='fas fa-eye-slash'></i> Turn Display OFF");
  } else {
    server.sendContent("<i class='fas fa-eye'></i> Turn Display ON");
  }
  server.sendContent(FPSTR(WEB_ACTION3));

  html = "</nav>";
  html += "<header class='w3-top w3-bar w3-theme'><button class='w3-bar-item w3-button w3-xxxlarge w3-hover-theme' onclick='openSidebar()'><i class='fas fa-bars'></i></button><h2 class='w3-bar-item'>Word-Clock</h2></header>";
  html += "<script>";
  html += "function openSidebar(){document.getElementById('mySidebar').style.display='block'}function closeSidebar(){document.getElementById('mySidebar').style.display='none'}closeSidebar();";
  html += "</script>";
  html += "<br><div class='w3-container w3-large' style='margin-top:88px'>";
  server.sendContent(html);
}

void sendFooter() {
  int8_t rssi = getWifiQuality();
  Serial.print("Signal Strength (RSSI): ");
  Serial.print(rssi);
  Serial.println("%");
  String html = "<br><br><br>";
  html += "</div>";
  html += "<footer class='w3-container w3-bottom w3-theme w3-margin-top'>";
  html += "<i class='far fa-paper-plane'></i> Version: " + String(VERSION) + "<br>";
  html += "<i class='far fa-clock'></i> Next Update: " + getTimeTillUpdate() + "<br>";
  html += "<i class='fas fa-rss'></i> Signal Strength: ";
  html += String(rssi) + "%";
  html += "</footer>";
  html += "</body></html>";
  server.sendContent(html);
}

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  Serial.println("Wifi Manager");
  Serial.println("Please connect to AP");
  Serial.println(myWiFiManager->getConfigPortalSSID());
  Serial.println("To setup Wifi Configuration");
  Serial.println("Please Connect to AP: " + String(myWiFiManager->getConfigPortalSSID()));
}

void flashLED(int number, int delayTime) {
  for (int inx = 0; inx < number; inx++) {
    tone(BUZZER_PIN, 440, delayTime);
    delay(delayTime);
    digitalWrite(externalLight, LOW);
    delay(delayTime);
    digitalWrite(externalLight, HIGH);
    delay(delayTime);
  }
  noTone(BUZZER_PIN);
}

// converts the dBm to a range between 0 and 100%
int8_t getWifiQuality() {
  int32_t dbm = WiFi.RSSI();
  if (dbm <= -100) {
    return 0;
  } else if (dbm >= -50) {
    return 100;
  } else {
    return 2 * (dbm + 100);
  }
}

String getTimeTillUpdate() {
  String rtnValue = "";

  long timeToUpdate = (((minutesBetweenDataRefresh * 60) + lastEpoch) - now());

  int hours = numberOfHours(timeToUpdate);
  int minutes = numberOfMinutes(timeToUpdate);
  int seconds = numberOfSeconds(timeToUpdate);

  rtnValue += String(hours) + ":";
  if (minutes < 10) {
    rtnValue += "0";
  }
  rtnValue += String(minutes) + ":";
  if (seconds < 10) {
    rtnValue += "0";
  }
  rtnValue += String(seconds);

  return rtnValue;
}

int getMinutesFromLastRefresh() {
  int minutes = (now() - lastEpoch) / 60;
  return minutes;
}

int getMinutesFromLastDisplay() {
  int minutes = (now() - displayOffEpoch) / 60;
  return minutes;
}

void enableDisplay(boolean enable) {
  displayOn = enable;
  if (enable) {
    if (getMinutesFromLastDisplay() >= minutesBetweenDataRefresh) {
      // The display has been off longer than the minutes between refresh -- need to get fresh data
      lastEpoch = 0; // this should force a data pull
      displayOffEpoch = 0;  // reset
    }
    //matrix.shutdown(false);
    Serial.println("Display was turned ON: " + now());
  } else {
    //matrix.shutdown(true);
    Serial.println("Display was turned OFF: " + now());
    displayOffEpoch = lastEpoch;
  }
}

// Toggle on and off the display if user defined times
void checkDisplay() {
  if (timeDisplayTurnsOn == "" || timeDisplayTurnsOff == "") {
    return; // nothing to do
  }

  String currentTime = String(nowHour) + ":" + String(nowMinute);

  if (currentTime == timeDisplayTurnsOn && !displayOn) {
    Serial.println("Time to turn display on: " + currentTime);
    flashLED(1, 500);
    enableDisplay(true);
  }

  if (currentTime == timeDisplayTurnsOff && displayOn) {
    Serial.println("Time to turn display off: " + currentTime);
    flashLED(2, 500);
    enableDisplay(false);
  }
}

String decodeHtmlString(String msg) {
  String decodedMsg = msg;
  // Restore special characters that are misformed to %char by the client browser
  decodedMsg.replace("+", " ");
  decodedMsg.replace("%21", "!");
  decodedMsg.replace("%22", "");
  decodedMsg.replace("%23", "#");
  decodedMsg.replace("%24", "$");
  decodedMsg.replace("%25", "%");
  decodedMsg.replace("%26", "&");
  decodedMsg.replace("%27", "'");
  decodedMsg.replace("%28", "(");
  decodedMsg.replace("%29", ")");
  decodedMsg.replace("%2A", "*");
  decodedMsg.replace("%2B", "+");
  decodedMsg.replace("%2C", ",");
  decodedMsg.replace("%2F", "/");
  decodedMsg.replace("%3A", ":");
  decodedMsg.replace("%3B", ";");
  decodedMsg.replace("%3C", "<");
  decodedMsg.replace("%3D", "=");
  decodedMsg.replace("%3E", ">");
  decodedMsg.replace("%3F", "?");
  decodedMsg.replace("%40", "@");
  decodedMsg.toUpperCase();
  decodedMsg.trim();
  return decodedMsg;
}

void ledsIntro() {
  rainbow(15);
  //colorWipe(leds.Color( 0, 0, 0, 0), 1);
}

// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// leds.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
  for (int i = 0; i < leds.numPixels(); i++) {  // For each pixel in leds...
    leds.setPixelColor(i, color);               //  Set pixel's color (in RAM)
    leds.show();                                //  Update strip to match
    delay(wait);                                //  Pause for a moment
  }
}


void rainbow(uint8_t wait) {
  uint16_t i, j;
  for (j = 0; j < 256; j++) {
    for (i = 0; i < leds.numPixels(); i++) {
      leds.setPixelColor(i, Wheel((i + j) & 255));
    }
    leds.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return leds.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return leds.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return leds.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void displayWordTime() {
  // Reset array to zeroes
  for (int8_t row = 0; row < TOTAL_ROWS; row++) {
    for (int8_t col = 0; col < TOTAL_COLS; col++) {
      display[row][col] = 0;
      leds.setPixelColor(row * TOTAL_COLS + col, 0, 0, 0);
    }
  }

  // Hour
  if (hourCompensated == 1) {
    // "UNA"
    display[1][1] = 1;
    display[2][2] = 1;
    display[3][2] = 1;
  } else if (hourCompensated == 2) {
    // "DOS"
    display[3][0] = 1;
    display[3][2] = 1;
    display[3][7] = 1;
  } else if (hourCompensated == 3) {
    // "TRES"
    display[3][3] = 1;
    display[3][4] = 1;
    display[3][5] = 1;
    display[3][7] = 1;
  } else if (hourCompensated == 4) {
    // "CUATRO"
    display[3][0] = 1;
    display[3][1] = 1;
    display[3][2] = 1;
    display[3][3] = 1;
    display[3][4] = 1;
    display[3][6] = 1;
  } else if (hourCompensated == 5) {
    // "CINCO"
    display[2][0] = 1;
    display[2][1] = 1;
    display[2][2] = 1;
    display[2][4] = 1;
    display[2][6] = 1;
  } else if (hourCompensated == 6) {
    // "SEIS"
    display[0][7] = 1;
    display[1][7] = 1;
    display[2][7] = 1;
    display[3][7] = 1;
  } else if (hourCompensated == 7) {
    // "SIETE"
    display[1][2] = 1;
    display[1][3] = 1;
    display[1][4] = 1;
    display[1][5] = 1;
    display[1][7] = 1;
  } else if (hourCompensated == 8) {
    // "OCHO"
    display[2][3] = 1;
    display[2][4] = 1;
    display[2][5] = 1;
    display[2][6] = 1;
  } else if (hourCompensated == 9) {
    // "NUEVE"
    display[1][0] = 1;
    display[1][1] = 1;
    display[1][4] = 1;
    display[1][6] = 1;
    display[1][7] = 1;
  } else if (hourCompensated == 10) {
    // "DIEZ"
    display[0][0] = 1;
    display[0][1] = 1;
    display[0][5] = 1;
    display[0][6] = 1;
  } else if (hourCompensated == 11) {
    // "ONCE"
    display[0][2] = 1;
    display[0][3] = 1;
    display[0][4] = 1;
    display[0][5] = 1;
  } else if (hourCompensated == 12 || hourCompensated == 0) {
    // "DOCE"
    display[0][0] = 1;
    display[0][2] = 1;
    display[0][4] = 1;
    display[0][5] = 1;
  }

  if (minuteRounded == 0 || minuteRounded == 60) {
    // NADA
  } else if (minuteRounded > 30) {
    // MENOS
    display[4][1] = 1;
    display[4][2] = 1;
    display[4][3] = 1;
    display[4][4] = 1;
    display[4][5] = 1;
  } else {
    // Y
    display[4][0] = 1;
  }

  if (minuteRounded == 30) {
    // MEDIA
    display[4][6] = 1;
    display[4][7] = 1;
    display[5][6] = 1;
    display[5][7] = 1;
    display[6][7] = 1;
  } else if (minuteRounded == 5 || minuteRounded == 55) {
    // CINCO
    display[7][1] = 1;
    display[7][2] = 1;
    display[7][3] = 1;
    display[7][6] = 1;
    display[7][7] = 1;
  } else if (minuteRounded == 10 || minuteRounded == 50) {
    // 10 y 50 DIEZ
    display[7][0] = 1;
    display[7][2] = 1;
    display[7][4] = 1;
    display[7][5] = 1;
  } else if (minuteRounded == 15 || minuteRounded == 45) {
    // 15 y 45 CUARTO
    display[5][0] = 1;
    display[5][1] = 1;
    display[5][2] = 1;
    display[5][3] = 1;
    display[5][4] = 1;
    display[5][5] = 1;
  } else if (minuteRounded == 20 || minuteRounded == 40) {
    // 20 y 40 VEINTE
    display[6][0] = 1;
    display[6][1] = 1;
    display[6][2] = 1;
    display[6][3] = 1;
    display[6][4] = 1;
    display[6][6] = 1;
  } else if (minuteRounded == 25 || minuteRounded == 35) {
    // 25 y 35 VEINTICINCO
    display[6][0] = 1;
    display[6][1] = 1;
    display[6][2] = 1;
    display[6][3] = 1;
    display[6][4] = 1;
    display[6][5] = 1;
    display[7][1] = 1;
    display[7][2] = 1;
    display[7][3] = 1;
    display[7][6] = 1;
    display[7][7] = 1;
  }

  wordClockSimulated = debugOutput();

  for (int8_t row = 0; row < TOTAL_ROWS; ++row) {
    for (int8_t col = 0; col < TOTAL_COLS; ++col) {
      if (display[row][col] == 1) {
        // Dim LEDs between 11pm and 6am
        //if (hour() >= 6 && hour() <= 22)
          //leds.setPixelColor(row * TOTAL_COLS + col, 175, 175, 175);
          int magicNumber = (row + col) *12 + nowHour*2 + nowMinute*2;
          leds.setPixelColor(row * TOTAL_COLS + col, Wheel(magicNumber & 255));
        //else
          //leds.setPixelColor(row * TOTAL_COLS + col, 25, 25, 25);
        //  leds.setPixelColor(row * TOTAL_COLS + col, Wheel((row + col + nowHour + nowMinute) & 255));
        //}
      }
    }
  }
  leds.show();
}

String debugOutput() {
  String result = "<div class='word-clock'>";

  for (int8_t row = 0; row < TOTAL_ROWS; row++) {
    result += "<div class='word-clock-row'>";
    for (int8_t col = 0; col < TOTAL_COLS; col++) {
      if (display[row][col] == 1) {
        result += "<span class='selected letter'>" + String(text[row][col]) + "</span>";
      } else {
        result += "<span class='letter'>" + String(text[row][col]) + "</span>";
      }
    }
    result += "</div>";
  }
  result += "</div>";
  return result;
}