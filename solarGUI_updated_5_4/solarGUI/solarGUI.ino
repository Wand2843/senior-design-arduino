#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_LSM303_Accel.h>
#include <Adafruit_LIS2MDL.h>

#include <WiFi.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <EthernetClient.h>
#include <Ethernet.h>
#include <avr/dtostrf.h>

#include <SPI.h>
#include <math.h>
#include "Adafruit_GFX.h"
#include "Adafruit_RA8875.h"
#include "debouncer.h"

#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

// #include <WiFiNINA.h>

#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme;

IPAddress servaddress(52,5,226,201);

char ssid[] = "iPhone (3)";
char pass[] = "Minusone123!";

char user[] = "sql9596235";
char password[] = "7LCYhZVIwK";

char sqlQuery[] = "INSERT INTO sql9596235.tracked_sensors (temperature, pressure, altitude, humidity) VALUES (";
char deleteQuery[] = "DELETE FROM sql9596235.tracked_sensors";
char selectQuery[] = "SELECT * FROM sql9596235.control";

WiFiClient client;
MySQL_Connection conn((Client *)&client);
// bool dataBaseOnline;

IPAddress server_addr(52, 5, 226, 201);

// Library only supports hardware SPI at this time
// Connect SCLK to UNO Digital #13 (Hardware SPI clock)
// Connect MISO to UNO Digital #12 (Hardware SPI MISO)
// Connect MOSI to UNO Digital #11 (Hardware SPI MOSI)
#define RA8875_INT 8
#define RA8875_CS 10
#define RA8875_RESET 9

#define MOTORLEFT 14
#define MOTORLDELAY 2
#define MOTORRIGHT 15
#define MOTORRDELAY 3
#define LAUP 4
#define LADOWN 5
#define LAUPDELAY 6
#define LADOWNDELAY 7

#define MOTORDUTYCYCLE 5
#define LIACDUTYCYCLE 20
#define GATEDEALY 230

#define WIFISTATUSCYCLE 3000

#define ONESECOND 60000

#define LATITUDE 40

Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);
uint16_t tx, ty;
tsPoint_t _tsLCDPoints[3];
tsPoint_t _tsTSPoints[3];
tsMatrix_t _tsMatrix;

tmElements_t tm;

uint8_t battery_soc;             // percent
float battery_voltage;           // volts
float battery_charging_amps;     // amps
uint8_t battery_temperature;     // celcius
uint8_t controller_temperature;  // celcius
float load_voltage;              // volts
float load_amps;                 // amps
uint8_t load_watts;              // watts
float solar_panel_voltage;       // volts
float solar_panel_amps;          // amps
uint8_t solar_panel_watts;       // watts

float enviornment_temp;      //C
float enviornment_pressure;  //hPa
float enviornment_humidity;  //%
float enviornment_altitude;  //m

int manual;
int schedule;
int automatic;

Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);
float angle;

Adafruit_LIS2MDL mag = Adafruit_LIS2MDL(12345);
float heading;

float baseAngleAzi;
float baseAngleAlt;

float altAngleAim = 0;
float aziAngleAim = 0;

bool WIFISTAT;
bool MANUALMODE;
bool PHOTOTRACKING;
bool MOTORMOVE;
bool MOTORMOVEDELAY;
bool LIACMOVE;

int motorCount;
int laCount;




int statPageCount = 0;

int startTime = 10;
int endTime = 17;
int timeStepMin;

#define EEPROMLOCATION 100

// Use to force a recalibration
#define FORCE_CALIBRATION false

enum guiStates {
  SELF_CHECK,
  MAIN_MENU,
  DISPLAY_OFF,
  RECORD_MENU,
  SENSOR_MENU,
  TEST_MENU,
  WIFI_MENU
} state,
  prevState;

enum trackingStates {
  TRACKING_OFF,
  FIXED_PATH,
  SENSOR_TRACKING
} stState;

enum moveDirection {
  UP_LA,
  DOWN_LA,
  LEFT_M,
  RIGHT_M,
  RST_BOTH
} directions;

int startTickTime;
int startSensorTime;
int startWiFiTime;
int startClockTime;
int startPDTime;
int endTickTime;
int motorStartTime;
int motorStartTimeDelay;

int powerPress;

tsPoint_t raw;
tsPoint_t calibrated;
bool pressedOut;
int debTime = 250;
bool pollClock = false;

bool displayWiFi = false;

void setup() {

  Serial.begin(9600);
  Serial.println("RA8875 start");
  pollClock = false;

  powerPress = 0;
  motorCount = 0;
  laCount = 0;
  WIFISTAT = false;
  MANUALMODE = true;
  PHOTOTRACKING = false;
  MOTORMOVE = false;
  MOTORMOVEDELAY = false;
  LIACMOVE = false;
  stState = TRACKING_OFF;
  state = SELF_CHECK;
  prevState = state;
  unsigned BMEstatus;
  BMEstatus = bme.begin();
  if (!BMEstatus) {
    Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
    Serial.print("SensorID was: 0x");
    Serial.println(bme.sensorID(), 16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
  }
  // accelSetup();
  // magSetup();

  // setUpWifi(WIFISTAT);
  initDebouncer();

  pinMode(MOTORLEFT, OUTPUT);
  digitalWrite(MOTORLEFT, LOW);

  pinMode(MOTORRIGHT, OUTPUT);
  digitalWrite(MOTORRIGHT, LOW);

  pinMode(LADOWN, OUTPUT);
  digitalWrite(LADOWN, LOW);

  pinMode(LAUP, OUTPUT);
  digitalWrite(LAUP, LOW);

  pinMode(MOTORLDELAY, OUTPUT);
  digitalWrite(MOTORLDELAY, LOW);

  pinMode(MOTORRDELAY, OUTPUT);
  digitalWrite(MOTORRDELAY, LOW);

  pinMode(LADOWNDELAY, OUTPUT);
  digitalWrite(LADOWNDELAY, LOW);

  pinMode(LAUPDELAY, OUTPUT);
  digitalWrite(LAUPDELAY, LOW);


  

  /* Initialize the display using 'RA8875_480x80', 'RA8875_480x128', 'RA8875_480x272' or 'RA8875_800x480' */
  if (!tft.begin(RA8875_800x480)) {
    Serial.println("RA8875 Not Found!");
    while (1)
      ;
  }

  tft.displayOn(true);
  tft.GPIOX(true);                               // Enable TFT - display enable tied to GPIOX
  tft.PWM1config(true, RA8875_PWM_CLK_DIV1024);  // PWM output for backlight
  tft.PWM1out(255);

  Serial.println("Enabled the touch screen");
  pinMode(RA8875_INT, INPUT);
  digitalWrite(RA8875_INT, HIGH);
  tft.touchEnable(true);


  // #if defined(EEPROM_SUPPORTED)
  //   /* Start the calibration process */
  //   if (FORCE_CALIBRATION || tft.readCalibration(EEPROMLOCATION, &_tsMatrix) == false) {
  //     Serial.println("Calibration not found.  Calibrating..\n");
  //     tsCalibrate();
  //   } else
  //     Serial.println("Calibration found\n");
  // #else
  //   tsCalibrate();
  // #endif
  /* _tsMatrix should now be populated with the correct coefficients! */
  // Serial.println("Waiting for touch events ...");
  _tsMatrix.Divider = -396278;
  _tsMatrix.An = -329920;
  _tsMatrix.Bn = 4160;
  _tsMatrix.Cn = 8697440;
  _tsMatrix.Dn = -1344;
  _tsMatrix.En = -221376;
  _tsMatrix.Fn = 25864608;
  tft.fillScreen(RA8875_BLACK);
  writeTxt(0, 100, "Initializing, please wait", RA8875_WHITE, 1, 0);
  // drawTestMenu();
  // drawRecordMenu();
  //delay(5000);
  //screenOff();
  startTickTime = millis();
  startClockTime=millis();
  startPDTime=millis();
  startSensorTime=millis();
  // magPoll();
  // accelPoll();
  baseAngleAlt = angle;
  baseAngleAzi = heading;
  altAngleAim = baseAngleAlt;
  aziAngleAim = baseAngleAzi;
}



void loop() {
  // Serial.println("RA8875 start");
  // int sensorValue = analogRead(A7);
  // float voltage= sensorValue * (5.0 / 1023.0);
  // Serial.println(voltage);
  // put your main code here, to run repeatedly:
  // if (stState == FIXED_PATH || SENSOR_TRACKING) {
  //   panelMoveTracking(0, 0);
  // }
  // accelPoll();
  // magPoll();
  // bmeData();
  // Serial.println("bmeTemp:");
  // Serial.println(enviornment_temp);
  // Serial.println("bmePress:");
  // Serial.println(enviornment_pressure);
  // Serial.println("bmeHum:");
  // Serial.println(enviornment_humidity);
  // Serial.println("bmeAlt:");
  // Serial.println(enviornment_altitude);
  // Serial.println("accelAngle:");
  // Serial.println(angle);
  // Serial.println("magAngle:");
  // Serial.println(heading);
  // delay(100);
  endTickTime = millis();
  motorFlipFlop();
  trackingMove();
  sendDataFSM();
  if ((endTickTime - startTickTime) > debTime) {
    if (debTime = 1000) debTime = 100;
    pressedOut = tick_Debouncer(waitForTouchEvent(&raw));
    startTickTime = endTickTime;
  }

  if ((endTickTime - startClockTime) > ONESECOND) {
    pollClock = true;
    startClockTime=endTickTime;
  }

  if((endTickTime-startWiFiTime)>WIFISTATUSCYCLE){
    displayWiFi=true;
    startWiFiTime=millis();
  }

  /* Calcuate the real X/Y position based on the calibration matrix */
  calibrateTSPoint(&calibrated, &raw, &_tsMatrix);
  fsmTick();
}

void motorFlipFlop() {
  if (MOTORMOVE) {
    if ((directions == DOWN_LA || directions == UP_LA) && endTickTime - motorStartTime >= LIACDUTYCYCLE) {
      panelMove(directions);
      motorStartTime = millis();
    } else if ((directions == LEFT_M || directions == RIGHT_M) && (endTickTime - motorStartTime) >= MOTORDUTYCYCLE) {
      panelMove(directions);
      motorStartTime = millis();
      // Serial.println(MOTORDUTYCYCLE);
    }
  }
  if (MOTORMOVEDELAY) {
    if ((directions == DOWN_LA || directions == UP_LA) && endTickTime - motorStartTimeDelay >= LIACDUTYCYCLE + GATEDEALY) {
      panelMoveDelay(directions);
      motorStartTimeDelay = millis();
      if (MANUALMODE && state == MAIN_MENU) {
        drawDirectionArrows(1);
      }
    } else if ((directions == LEFT_M || directions == RIGHT_M) && (endTickTime - motorStartTimeDelay) >= MOTORDUTYCYCLE + GATEDEALY) {
      panelMoveDelay(directions);
      motorStartTimeDelay = millis();
      if (MANUALMODE && state == MAIN_MENU) {
        drawDirectionArrows(1);
      }
      // Serial.println(MOTORDUTYCYCLE);
    }
    // MOTORMOVE = false;
  }
}

void trackingMove() {
  if ((stState == SENSOR_TRACKING || stState == FIXED_PATH) && !MOTORMOVE && !MOTORMOVEDELAY) {
    RTC.read(tm);
    float timeP = getTimeP(tm.Hour, tm.Minute);
    float solarDeclination = getSolarDec(getJD(tmYearToCalendar(tm.Year), tm.Month, tm.Day));
    altAngleAim = getAlt(LATITUDE, solarDeclination, timeP);
    aziAngleAim = getAzi(LATITUDE, solarDeclination, timeP, altAngleAim);
    magPoll();
    accelPoll();
    if (heading < aziAngleAim + baseAngleAzi - 4) {
      panelMove(directions = RIGHT_M);
      panelMoveDelay(directions = RIGHT_M);
    } else if (heading < aziAngleAim + baseAngleAzi + 4) {
      panelMove(directions = LEFT_M);
      panelMoveDelay(directions = LEFT_M);
    } else if (angle < altAngleAim + baseAngleAlt - 4) {
      panelMove(directions = UP_LA);
      panelMoveDelay(directions = UP_LA);
    } else if (angle < altAngleAim + baseAngleAlt + 4) {
      panelMove(directions = DOWN_LA);
      panelMoveDelay(directions = DOWN_LA);
    }

    else if (stState == SENSOR_TRACKING /*and ref reads are not 3.3*/) {
      // else if(readLR is greater than 1.24+A){
      //     panelMove(directions = RIGHT_M);
      //   panelMoveDelay(directions = RIGHT_M);
      // }else if(readLR is less than 1.24-A){
      //   panelMove(directions = LEFT_M);
      //   panelMoveDelay(directions = LEFT_M);
      // }else if(readUD is greater than 1.24+A){
      //   panelMove(directions = UP_LA);
      //   panelMoveDelay(directions = UP_LA);
      // }else if(readUD is less than 1.24-A){
      //   panelMove(directions = DOWN_LA);
      //   panelMoveDelay(directions = DOWN_LA);
      // }
    }
  }
}

void fsmTick() {
  if (pressedOut)
    tft.fillCircle(calibrated.x, calibrated.y, 3, RA8875_RED);

  switch (state) {
    case SELF_CHECK:
      if (selfCheck()) {
        prevState = state;
        state = MAIN_MENU;

        drawMainMenu();
        wifiStatus();
        startWiFiTime=millis();
      }
      break;

    case MAIN_MENU:
      displayTime();



      //CCData();
      //Serial.println(battery_voltage);
      if(displayWiFi){
        wifiStatus();
        displayWiFi=false;
      }
      
      // if(WIFISTAT)writeTxt(160, 0, "Server Online ", RA8875_WHITE, 2, 1);
      // else writeTxt(160, 0, "Server Offline", RA8875_WHITE, 2, 1);
      if (calibrated.x > 627 && calibrated.x < (627 + 117) && calibrated.y > 76 && calibrated.y < (76 + 85) && pressedOut) {
        powerPress = powerPress + 1;
        // Serial.println(powerPress);
        if (powerPress == 8) {
          // Serial.println(powerPress);
          prevState = state;
          state = DISPLAY_OFF;
          displayOff();
          debTime = 1000;
          powerPress = 0;
        }
        pressedOut = false;
      }
      // delay(1000);

      else if (pressedOut) {
        powerPress = 0;

        if (calibrated.x > 145 && calibrated.x < (145 + 117) && calibrated.y > 107 && calibrated.y < (107 + 85) && pressedOut && !MOTORMOVE && !MOTORMOVEDELAY && MANUALMODE) {
          drawDirectionArrows(0);
          panelMove(directions = UP_LA);
          panelMoveDelay(directions = UP_LA);
          // MOTORMOVE = true;
          pressedOut = false;
        } else if (calibrated.x > 33 && calibrated.x < (33 + 117) && calibrated.y > 223 && calibrated.y < (223 + 85) && pressedOut && !MOTORMOVE && !MOTORMOVEDELAY && MANUALMODE) {
          drawDirectionArrows(0);
          panelMove(directions = LEFT_M);
          panelMoveDelay(directions = LEFT_M);
          // MOTORMOVE = true;
          pressedOut = false;
        } else if (calibrated.x > 258 && calibrated.x < (258 + 117) && calibrated.y > 223 && calibrated.y < (223 + 85) && pressedOut && !MOTORMOVE && !MOTORMOVEDELAY && MANUALMODE) {
          drawDirectionArrows(0);
          panelMove(directions = RIGHT_M);
          panelMoveDelay(directions = RIGHT_M);
          // MOTORMOVE = true;
          pressedOut = false;
        } else if (calibrated.x > 145 && calibrated.x < (145 + 117) && calibrated.y > 347 && calibrated.y < (347 + 85) && pressedOut && !MOTORMOVE && !MOTORMOVEDELAY && MANUALMODE) {
          drawDirectionArrows(0);
          panelMove(directions = DOWN_LA);
          panelMoveDelay(directions = DOWN_LA);
          // MOTORMOVE = true;
          pressedOut = false;
        } else if (calibrated.x > 0 && calibrated.x < (0 + 130) && calibrated.y > 0 && calibrated.y < (0 + 170) && pressedOut) {
          prevState = state;
          state = TEST_MENU;
          drawTestMenu();
          pressedOut = false;
        } else if (calibrated.x > 460 && calibrated.x < (460 + 117) && calibrated.y > 76 && calibrated.y < (76 + 85) && pressedOut) {
          prevState = state;
          state = SENSOR_MENU;
          statPageCount = 0;
          drawSensorList(statPageCount);
          pressedOut = false;
        } else if (calibrated.x > 460 && calibrated.x < (460 + 117) && calibrated.y > 210 && calibrated.y < (210 + 85) && pressedOut) {
          prevState = state;
          state = RECORD_MENU;
          drawRecordMenu();
          pressedOut = false;
        } else if (calibrated.x > 627 && calibrated.x < (627 + 117) && calibrated.y > 210 && calibrated.y < (210 + 85) && pressedOut) {

          prevState = state;
          state = WIFI_MENU;
          drawWiFiIcon(1);
          drawRstIcon(0, 0);
          drawStatsIcon(0);

          MANUALMODE = false;
          stState = FIXED_PATH;
          drawRecordIcon(2);
          drawAutoIcon(1);

          pressedOut = false;
          tft.fillRect(0, 101, 400, 380, RA8875_BLACK);
          writeTxt(0, 101, "Server overwrite,", RA8875_WHITE, 2, 0);
          writeTxt(0, 150, "press WiFi to", RA8875_WHITE, 2, 0);
          writeTxt(0, 200, "cancel.", RA8875_WHITE, 2, 0);
          // setUpWifi(true);
          // delay(500);  //simulate setup
        } else if (calibrated.x > 460 && calibrated.x < (460 + 117) && calibrated.y > 340 && calibrated.y < (340 + 85) && pressedOut) {
          switch (stState) {
            case TRACKING_OFF:
              MANUALMODE = false;
              drawDirectionArrows(0);
              panelMove(directions = RST_BOTH);
              delay(1000);  //simulate panel move
              MANUALMODE = true;
              drawDirectionArrows(1);
              break;
            case FIXED_PATH:
              drawRstIcon(2, 1);
              stState = SENSOR_TRACKING;
              break;
            case SENSOR_TRACKING:
              drawRstIcon(1, 1);
              stState = FIXED_PATH;
              break;
          }
          pressedOut = false;
        } else if (calibrated.x > 627 && calibrated.x < (627 + 117) && calibrated.y > 340 && calibrated.y < (340 + 85) && pressedOut) {
          switch (stState) {
            case TRACKING_OFF:
              drawAutoIcon(1);
              drawRstIcon(1, 1);
              MANUALMODE = false;
              drawDirectionArrows(0);
              stState = FIXED_PATH;
              break;
            case FIXED_PATH:
              drawAutoIcon(0);
              drawRstIcon(0, 1);
              MANUALMODE = true;
              drawDirectionArrows(1);
              stState = TRACKING_OFF;
              break;
            case SENSOR_TRACKING:
              drawAutoIcon(0);
              drawRstIcon(0, 1);
              MANUALMODE = true;
              drawDirectionArrows(1);
              stState = TRACKING_OFF;
              break;
          }
          pressedOut = false;
        }
      }
      break;
    case WIFI_MENU:
      displayTime();
      if (WIFISTAT) writeTxt(160, 17, "Server Online ", RA8875_WHITE, 2, 1);
      else writeTxt(160, 17, "Server Offline", RA8875_WHITE, 2, 1);
      if (calibrated.x > 627 && calibrated.x < (627 + 117) && calibrated.y > 76 && calibrated.y < (76 + 85) && pressedOut) {
        powerPress = powerPress + 1;
        // Serial.println(powerPress);
        if (powerPress == 8) {
          // Serial.println(powerPress);
          prevState = state;
          state = DISPLAY_OFF;
          displayOff();
          debTime = 1000;
          powerPress = 0;
        }
        pressedOut = false;
      } else if (pressedOut) {
        powerPress = 0;
        if (calibrated.x > 627 && calibrated.x < (627 + 117) && calibrated.y > 210 && calibrated.y < (210 + 85) && pressedOut) {
          // WIFISTAT = false;
          prevState = state;
          state = MAIN_MENU;
          // setUpWifi(false);
          drawWiFiIcon(0);
          drawRstIcon(0, 1);
          drawStatsIcon(1);

          MANUALMODE = true;
          stState = TRACKING_OFF;

          drawAutoIcon(0);

          pressedOut = false;
          tft.fillRect(0, 101, 400, 380, RA8875_BLACK);
          writeTxt(440, 295, "Re-Connect", RA8875_BLACK, 1, 1);
          drawRecordIcon(0);
          drawDirectionArrows(1);
          // delay(500);  //simulate setup
        } else if (calibrated.x > 460 && calibrated.x < (460 + 117) && calibrated.y > 210 && calibrated.y < (210 + 85) && pressedOut) {
          WIFISTAT = wifiConnect();
          Serial.println(WIFISTAT);
          if (WIFISTAT) writeTxt(160, 17, "Server Online ", RA8875_WHITE, 2, 1);
          pressedOut = false;
        }
      }
      break;
    case SENSOR_MENU:
      displayTime();

      if ((endTickTime - startSensorTime) > 3000) {
        drawSensorTable(statPageCount);
        startSensorTime=endTickTime;
      }

      if (calibrated.x > 640 && calibrated.x < 800 && calibrated.y > 65 && calibrated.y < 65 + 70 * 2 && statPageCount > 0 && pressedOut) {
        statPageCount--;
        tft.fillRect(0, 70, 640, 410, RA8875_BLACK);
        for (int i = 1; i < 6; i++) { tft.fillRect(0, 65 + 70 * i, 640, 5, RA8875_WHITE); }
        drawSensorTable(statPageCount);
        // delay(100);
        pressedOut = false;
      } else if (calibrated.x > 640 && calibrated.x < 800 && calibrated.y > 65 + 70 * 2 && calibrated.y < 65 + 70 * 4 && statPageCount < 2 && pressedOut) {
        statPageCount++;
        tft.fillRect(0, 70, 640, 410, RA8875_BLACK);
        for (int i = 1; i < 6; i++) { tft.fillRect(0, 65 + 70 * i, 640, 5, RA8875_WHITE); }
        // delay(100);
        drawSensorTable(statPageCount);
        pressedOut = false;
      } else if (calibrated.x > 640 && calibrated.x < 800 && calibrated.y > 65 + 70 * 4 && calibrated.y < 800 && pressedOut) {
        prevState = state;
        state = MAIN_MENU;
        drawMainMenu();
        pressedOut = false;
      }
      break;
    case TEST_MENU:
      if (calibrated.x > 0 && calibrated.x < (0 + 160) && calibrated.y > 360 && calibrated.y < (360 + 120) && pressedOut) {
        prevState = state;
        state = MAIN_MENU;
        drawMainMenu();
        pressedOut = false;
      }
      break;
    case DISPLAY_OFF:
      if (tft.touched() && pressedOut) {
        debTime = 1000;
        displayOn();
        state = prevState;
        prevState = DISPLAY_OFF;
        // delay(1000);
        pressedOut = false;
      }
      break;
    case RECORD_MENU:
      if (calibrated.x > 230 && calibrated.x < 230 + 120 && calibrated.y > 100 && calibrated.y < 210 && startTime > 0 && pressedOut) {
        startTime--;
        if (startTime < 10) {
          String tempString = "0";
          tempString.concat(startTime);
          char tempCArr[10];
          tempString.toCharArray(tempCArr, 10);
          writeTxt(265, 230, tempCArr, RA8875_WHITE, 2, 1);
        } else {
          String tempString = "";
          tempString.concat(startTime);
          char tempCArr[10];
          tempString.toCharArray(tempCArr, 10);
          writeTxt(265, 230, tempCArr, RA8875_WHITE, 2, 1);
        }
        pressedOut = false;
      } else if (calibrated.x > 230 && calibrated.x < 230 + 120 && calibrated.y > 290 && calibrated.y < 400 && startTime < endTime - 1 && pressedOut) {
        startTime++;
        if (startTime < 10) {
          String tempString = "0";
          tempString.concat(startTime);
          char tempCArr[10];
          tempString.toCharArray(tempCArr, 10);
          writeTxt(265, 230, tempCArr, RA8875_WHITE, 2, 1);
        } else {
          String tempString = "";
          tempString.concat(startTime);
          char tempCArr[10];
          tempString.toCharArray(tempCArr, 10);
          writeTxt(265, 230, tempCArr, RA8875_WHITE, 2, 1);
        }
        pressedOut = false;
      } else if (calibrated.x > 600 && calibrated.x < 600 + 120 && calibrated.y > 100 && calibrated.y < 210 && endTime > startTime + 1 && pressedOut) {
        endTime--;
        if (endTime < 10) {
          String tempString = "0";
          tempString.concat(endTime);
          char tempCArr[10];
          tempString.toCharArray(tempCArr, 10);
          writeTxt(635, 230, tempCArr, RA8875_WHITE, 2, 1);
        } else {
          String tempString = "";
          tempString.concat(endTime);
          char tempCArr[10];
          tempString.toCharArray(tempCArr, 10);
          writeTxt(635, 230, tempCArr, RA8875_WHITE, 2, 1);
        }
        pressedOut = false;
      } else if (calibrated.x > 600 && calibrated.x < 600 + 120 && calibrated.y > 290 && calibrated.y < 400 && endTime < 24 && pressedOut) {
        endTime++;
        if (endTime < 10) {
          String tempString = "0";
          tempString.concat(endTime);
          char tempCArr[10];
          tempString.toCharArray(tempCArr, 10);
          writeTxt(635, 230, tempCArr, RA8875_WHITE, 2, 1);
        } else {
          String tempString = "";
          tempString.concat(endTime);
          char tempCArr[10];
          tempString.toCharArray(tempCArr, 10);
          writeTxt(635, 230, tempCArr, RA8875_WHITE, 2, 1);
        }
        pressedOut = false;
      } else if (calibrated.x > 0 && calibrated.x < 0 + 160 && calibrated.y > 360 && calibrated.y < 480 && pressedOut) {
        prevState = state;
        state = MAIN_MENU;
        drawMainMenu();
        pressedOut = false;
      }
      break;
  }
}

void panelMoveTracking(int hori, int verti) {}

bool solarTracking() {
  Serial.println("start solar tracking");
  return true;
}
bool selfCheck() {
  //check voltage reading
  int voltage = 5;
  return voltage == 5;
}


// boolean motorSignalOn = false;
void panelMove(moveDirection direc) {
  if (MOTORMOVE == false) {
    switch (direc) {
      case UP_LA:
        MOTORMOVE = true;
        motorStartTime = millis();
        // Serial.println("linear actuator move up");
        digitalWrite(LAUP, HIGH);
        laCount++;
        break;
      case DOWN_LA:
        MOTORMOVE = true;
        motorStartTime = millis();
        // Serial.println("linear actuator move down");
        digitalWrite(LADOWN, HIGH);
        laCount--;
        break;
      case LEFT_M:
        // Serial.println("motor move left");
        MOTORMOVE = true;
        motorStartTime = millis();
        digitalWrite(MOTORLEFT, HIGH);
        motorCount--;
        break;
      case RIGHT_M:
        MOTORMOVE = true;
        motorStartTime = millis();
        digitalWrite(MOTORRIGHT, HIGH);
        // Serial.println("motor move right");
        motorCount++;
        break;
      case RST_BOTH:
        //Serial.println("rest panel to front");
        //move the panel until compass reads certain value
        break;
    }
  } else {
    switch (direc) {
      case UP_LA:
        MOTORMOVE = false;
        digitalWrite(LAUP, LOW);
        //Serial.println("linear actuator move up STOP");
        break;
      case DOWN_LA:
        MOTORMOVE = false;
        digitalWrite(LADOWN, LOW);
        // Serial.println("linear actuator move down STOP");
        break;
      case LEFT_M:
        MOTORMOVE = false;
        digitalWrite(MOTORLEFT, LOW);
        // Serial.println("motor move left STOP");
        break;
      case RIGHT_M:
        MOTORMOVE = false;
        digitalWrite(MOTORRIGHT, LOW);
        // Serial.println("motor move right STOP");
        break;
      case RST_BOTH:
        // Serial.println("rest panel to front");

        break;
    }
  }
}

void panelMoveDelay(moveDirection direc) {
  if (MOTORMOVEDELAY == false) {
    switch (direc) {
      case UP_LA:
        MOTORMOVEDELAY = true;
        motorStartTimeDelay = millis();
        // Serial.println("linear actuator move up");
        digitalWrite(LAUPDELAY, HIGH);
        // laCount++;
        break;
      case DOWN_LA:
        MOTORMOVEDELAY = true;
        motorStartTimeDelay = millis();
        // Serial.println("linear actuator move down");
        digitalWrite(LADOWNDELAY, HIGH);
        // laCount--;
        break;
      case LEFT_M:
        // Serial.println("motor move left");
        MOTORMOVEDELAY = true;
        motorStartTimeDelay = millis();
        digitalWrite(MOTORLDELAY, HIGH);
        // motorCount--;
        break;
      case RIGHT_M:
        MOTORMOVEDELAY = true;
        motorStartTimeDelay = millis();
        digitalWrite(MOTORRDELAY, HIGH);
        // Serial.println("motor move right");
        // motorCount++;
        break;
      case RST_BOTH:
        //Serial.println("rest panel to front");
        //move the panel until compass reads certain value
        break;
    }
  } else {
    switch (direc) {
      case UP_LA:
        MOTORMOVEDELAY = false;
        digitalWrite(LAUPDELAY, LOW);
        //Serial.println("linear actuator move up STOP");
        break;
      case DOWN_LA:
        MOTORMOVEDELAY = false;
        digitalWrite(LADOWNDELAY, LOW);
        // Serial.println("linear actuator move down STOP");
        break;
      case LEFT_M:
        MOTORMOVEDELAY = false;
        digitalWrite(MOTORLDELAY, LOW);
        // Serial.println("motor move left STOP");
        break;
      case RIGHT_M:
        MOTORMOVEDELAY = false;
        digitalWrite(MOTORRDELAY, LOW);
        // Serial.println("motor move right STOP");
        break;
      case RST_BOTH:
        // Serial.println("rest panel to front");

        break;
    }
  }
}

void drawMainMenu() {
  tft.fillScreen(RA8875_BLACK);
  tft.fillRect(0, 65, 800, 5, RA8875_WHITE);
  //test icon
  tft.fillRect(0, 0, 130, 70, RA8875_WHITE);
  writeTxt(0, 0, "TEST", RA8875_BLACK, 3, 0);

  drawDirectionArrows(1);
  pollClock=true;
  displayTime();
  drawPowerIcon();
  drawStatsIcon(1);
  drawRecordIcon(0);
  drawWiFiIcon(0);
  drawRstIcon(0, 1);
  drawAutoIcon(0);
}

void drawTestMenu() {
  tft.fillScreen(RA8875_BLACK);
  writeTxt(0, 0, "TEST RESULT", RA8875_WHITE, 3, 0);
  tft.fillRect(0, 65, 800, 5, RA8875_WHITE);
  String outString = "Error:  ";
  outString.concat("pressure sensor offline, ");
  outString.concat("compass offline, ");
  outString.concat("clock offline, ");
  outString.concat("linear actuator offline, ");
  outString.concat("motor offline, ");
  outString.concat("compass error, ");
  outString.concat("thermameter offline, ");
  char output[200];
  outString.toCharArray(output, 200);
  writeTxt(0, 100, output, RA8875_WHITE, 1, 0);
  tft.fillRect(0, 360, 160, 120, RA8875_WHITE);
  tft.fillRect(0, 365, 155, 115, RA8875_BLACK);
  writeTxt(25, 390, "HOME", RA8875_WHITE, 2, 0);
}

void drawSensorList(int page) {
  tft.fillScreen(RA8875_BLACK);
  writeTxt(0, 0, "SENSOR LIST", RA8875_WHITE, 3, 0);
  tft.fillRect(0, 65, 800, 5, RA8875_WHITE);
  displayTime();
  //buttons
  tft.fillRect(640, 65, 5, 415, RA8875_WHITE);
  tft.fillRect(640, 65 + 70 * 2, 160, 5, RA8875_WHITE);
  tft.fillRect(640, 65 + 70 * 4, 160, 5, RA8875_WHITE);

  writeTxt(660, 380, "HOME", RA8875_WHITE, 3, 0);
  tft.fillTriangle(720, 90, 680, 170, 760, 170, RA8875_WHITE);
  tft.fillTriangle(720, 320, 680, 240, 760, 240, RA8875_WHITE);
  tft.fillRect(0, 70, 640, 410, RA8875_BLACK);
  for (int i = 1; i < 6; i++) { tft.fillRect(0, 65 + 70 * i, 640, 5, RA8875_WHITE); }
  drawSensorTable(page);
}

void drawSensorTable(int page) {
  if (page == 0) {


    bmeData();

    char myTemperature[8];
    char myPressure[8];
    char myAltitude[8];
    char myHumidity[8];

    char myBatteryPercentage[8];
    char myBatteryVoltage[8];
    

    float battery_percentage_float;

    battery_percentage_float = battery_soc;

    dtostrf(enviornment_temp, 6, 2, myTemperature);
    dtostrf(enviornment_pressure, 6, 2, myPressure);
    dtostrf(enviornment_altitude, 6, 2, myAltitude);
    dtostrf(enviornment_humidity, 6, 2, myHumidity);

    dtostrf(battery_percentage_float, 6, 2, myBatteryPercentage);
    dtostrf(battery_voltage, 6, 2, myBatteryVoltage);

    String env_temp = "Environment Temperature:" + String(myTemperature) + " Degrees F";
    String env_pressure = "Environment Pressure:" + String(myPressure) + " inches/hg";
    String env_altitude = "Environment Altitude: " + String(myAltitude) + " m";
    String env_humidity = "Environment Humidity:" + String(myHumidity) + " %";

    String battery_percentage_string = "Battery Percentage:" + String(myBatteryPercentage) + " %";
    String battery_voltage_string = "Battery Voltage:" + String(myBatteryVoltage) + " V";

    char charTempArray[50];
    char charPressureArray[50];
    char charAltitudeArray[50];
    char charHumidityArray[50];

    char charBatteryPercentageArray[50];
    char charBatteryVoltageArray[50];

    env_temp.toCharArray(charTempArray, 50);
    env_pressure.toCharArray(charPressureArray, 50);
    env_altitude.toCharArray(charAltitudeArray, 50);
    env_humidity.toCharArray(charHumidityArray, 50);

    battery_percentage_string.toCharArray(charBatteryPercentageArray, 50);
    battery_voltage_string.toCharArray(charBatteryVoltageArray, 50);

    writeTxt(0, 80, charTempArray, RA8875_WHITE, 1, 1);
    writeTxt(0, 80 + 70, charPressureArray, RA8875_WHITE, 1, 1);
    writeTxt(0, 80 + 70 * 2, charAltitudeArray, RA8875_WHITE, 1, 1);
    writeTxt(0, 80 + 70 * 3, charHumidityArray, RA8875_WHITE, 1, 1);
    writeTxt(0, 80 + 70 * 4, charBatteryPercentageArray, RA8875_WHITE, 1, 1);
    writeTxt(0, 80 + 70 * 5, charBatteryVoltageArray, RA8875_WHITE, 1, 1);

    CCData();

  } else if (page == 1) {

/*
uint8_t battery_temperature;     // celcius
uint8_t controller_temperature;  // celcius
float load_voltage;              // volts
float load_amps;                 // amps
uint8_t load_watts;              // watts
float solar_panel_voltage;       // volts
float solar_panel_amps;          // amps
uint8_t solar_panel_watts;       // watts
*/

    bmeData();

    char myBatteryChargingAmps[8];
    char myBatteryTemperature[8];

    dtostrf(battery_charging_amps, 6, 2, myBatteryChargingAmps);
    dtostrf(battery_temperature, 6, 2, myBatteryTemperature);

    String battery_charging_amps_string = "Battery Amps:" + String(myBatteryChargingAmps) + " A";
    String battery_temperature_string = "Battery Temperature:" + String(myBatteryTemperature) + " Degrees C";

    char charBatteryChargingAmpsArray[50];
    char charBatteryTemperatureArray[50];

    battery_charging_amps_string.toCharArray(charBatteryChargingAmpsArray, 50);
    battery_temperature_string.toCharArray(charBatteryTemperatureArray, 50);


    writeTxt(0, 80, charBatteryChargingAmpsArray, RA8875_WHITE, 1, 1);
    writeTxt(0, 80 + 70 * 1, charBatteryTemperatureArray, RA8875_WHITE, 1, 1);
    writeTxt(0, 80 + 70 * 2, "Charging Current: 0A", RA8875_WHITE, 1, 1);
    writeTxt(0, 80 + 70 * 3, "How long did this take?", RA8875_WHITE, 1, 1);
    writeTxt(0, 80 + 70 * 4, "One night", RA8875_WHITE, 1, 1);
    writeTxt(0, 80 + 70 * 5, "When did I start? Sunday 12 AM", RA8875_WHITE, 1, 1);

    CCData();

  } else if (page == 2) {

    writeTxt(0, 80, "Did I sleep? ", RA8875_WHITE, 1, 1);
    writeTxt(0, 80 + 70 * 1, "No!", RA8875_WHITE, 1, 1);

  }
}

void drawRecordMenu() {
  tft.fillScreen(RA8875_BLACK);
  writeTxt(0, 0, "RECORD Schedule", RA8875_WHITE, 3, 0);
  tft.fillRect(0, 65, 800, 5, RA8875_WHITE);
  displayTime();
  writeTxt(15, 230, "START AT", RA8875_WHITE, 2, 0);
  tft.fillRect(230, 100, 120, 300, RA8875_WHITE);
  tft.fillRect(235, 105, 110, 290, RA8875_BLACK);
  tft.fillRect(230, 210, 120, 5, RA8875_WHITE);
  tft.fillRect(230, 290, 120, 5, RA8875_WHITE);
  tft.fillTriangle(290, 120, 240, 180, 340, 180, RA8875_WHITE);
  tft.fillTriangle(290, 380, 240, 320, 340, 320, RA8875_WHITE);
  if (startTime < 10) {
    String tempString = "0";
    tempString.concat(startTime);
    char tempCArr[10];
    tempString.toCharArray(tempCArr, 10);
    writeTxt(265, 230, tempCArr, RA8875_WHITE, 2, 1);
  } else {
    String tempString = "";
    tempString.concat(startTime);
    char tempCArr[10];
    tempString.toCharArray(tempCArr, 10);
    writeTxt(265, 230, tempCArr, RA8875_WHITE, 2, 1);
  }
  int offset = 370;
  writeTxt(65 + offset, 230, "END AT", RA8875_WHITE, 2, 0);
  tft.fillRect(230 + offset, 100, 120, 300, RA8875_WHITE);
  tft.fillRect(235 + offset, 105, 110, 290, RA8875_BLACK);
  tft.fillRect(230 + offset, 210, 120, 5, RA8875_WHITE);
  tft.fillRect(230 + offset, 290, 120, 5, RA8875_WHITE);
  tft.fillTriangle(290 + offset, 120, 240 + offset, 180, 340 + offset, 180, RA8875_WHITE);
  tft.fillTriangle(290 + offset, 380, 240 + offset, 320, 340 + offset, 320, RA8875_WHITE);
  if (endTime < 10) {
    String tempString = "0";
    tempString.concat(endTime);
    char tempCArr[10];
    tempString.toCharArray(tempCArr, 10);
    writeTxt(265 + offset, 230, tempCArr, RA8875_WHITE, 2, 1);
  } else {
    String tempString = "";
    tempString.concat(endTime);
    char tempCArr[10];
    tempString.toCharArray(tempCArr, 10);
    writeTxt(265 + offset, 230, tempCArr, RA8875_WHITE, 2, 1);
  }
  tft.fillRect(0, 360, 160, 120, RA8875_WHITE);
  tft.fillRect(0, 365, 155, 115, RA8875_BLACK);
  writeTxt(25, 390, "HOME", RA8875_WHITE, 2, 0);
}

void displayOff() {
  tft.sleep(true);
}

void displayOn() {
  tft.displayOn(true);
}

void displayTime() {
  if (pollClock && RTC.read(tm)) {
    String time = " ";
    time = time + tm.Month;
    time = time + "/";
    time = time + tm.Day;
    time = time + "/";
    time = time + tmYearToCalendar(tm.Year);
    time = time + " ";
    time = time + tm.Hour;
    time = time + ":";
    time = time + tm.Minute;
    char output[50];
    time.toCharArray(output, 50);
    writeTxt(530, 30, output, RA8875_WHITE, 1, 1);
    pollClock = false;
  }
}

void writeTxt(int x, int y, char *string, uint16_t foreColor, uint8_t size, int refresh) {
  tft.textMode();
  tft.textEnlarge(size);
  tft.textSetCursor(x, y);
  if (refresh == 0)
    tft.textTransparent(foreColor);
  else
    tft.textColor(foreColor, RA8875_BLACK);
  tft.textWrite(string);

  tft.graphicsMode();
}

void drawDirectionArrows(int on) {
  if (on == 1) {
    tft.fillRoundRect(145, 107, 117, 85, 20, RA8875_WHITE);
    tft.fillRoundRect(33, 223, 117, 85, 20, RA8875_WHITE);
    tft.fillRoundRect(258, 223, 117, 85, 20, RA8875_WHITE);
    tft.fillRoundRect(145, 347, 117, 85, 20, RA8875_WHITE);

    tft.fillTriangle(67, 266, 110, 240, 110, 286, RA8875_BLACK);
    tft.fillTriangle(204, 132, 173, 159, 235, 159, RA8875_BLACK);
    tft.fillTriangle(345, 266, 302, 240, 302, 286, RA8875_BLACK);
    tft.fillTriangle(204, 409, 173, 382, 235, 382, RA8875_BLACK);
  } else {
    tft.fillRoundRect(145, 107, 117, 85, 20, 0x7BEF);
    tft.fillRoundRect(33, 223, 117, 85, 20, 0x7BEF);
    tft.fillRoundRect(258, 223, 117, 85, 20, 0x7BEF);
    tft.fillRoundRect(145, 347, 117, 85, 20, 0x7BEF);

    tft.fillTriangle(67, 266, 110, 240, 110, 286, RA8875_BLACK);
    tft.fillTriangle(204, 132, 173, 159, 235, 159, RA8875_BLACK);
    tft.fillTriangle(345, 266, 302, 240, 302, 286, RA8875_BLACK);
    tft.fillTriangle(204, 409, 173, 382, 235, 382, RA8875_BLACK);
  }
}

void drawPowerIcon() {
  tft.fillRoundRect(627, 76, 117, 85, 20, RA8875_WHITE);
  writeTxt(648, 161, "POWER", RA8875_WHITE, 1, 0);


  tft.fillCircle(685, 120, 25, RA8875_BLACK);
  tft.fillCircle(685, 120, 20, RA8875_WHITE);
  tft.fillRect(680, 95, 10, 10, RA8875_WHITE);
  tft.fillRect(683, 85, 5, 30, RA8875_BLACK);
}
void drawStatsIcon(int on) {
  if (on == 1) {
    tft.fillRoundRect(460, 76, 117, 85, 20, RA8875_WHITE);
    writeTxt(470, 85, "SENSOR", RA8875_BLACK, 1, 0);
    writeTxt(475, 115, "STATS", RA8875_BLACK, 1, 0);
    writeTxt(479, 161, "STATS", RA8875_WHITE, 1, 0);
  } else {
    tft.fillRoundRect(460, 76, 117, 85, 20, 0x7BEF);
    writeTxt(470, 85, "SENSOR", RA8875_WHITE, 1, 0);
    writeTxt(475, 115, "STATS", RA8875_WHITE, 1, 0);
    writeTxt(479, 161, "STATS", RA8875_WHITE, 1, 0);
  }
}

void drawRecordIcon(int on) {
  tft.fillRoundRect(460, 210, 117, 85, 20, RA8875_WHITE);
  writeTxt(473, 295, "RECORD", RA8875_WHITE, 1, 0);
  if (on == 1) {
    tft.fillCircle(518, 252, 25, RA8875_RED);
  } else if (on == 0) {
    tft.fillCircle(518, 252, 25, RA8875_BLACK);
  } else if (on == 2) {
    tft.fillRoundRect(460, 210, 117, 85, 20, RA8875_WHITE);
    writeTxt(485, 220, "RE", RA8875_BLACK, 3, 0);
    writeTxt(440, 295, "Re-Connect", RA8875_WHITE, 1, 1);
  }
}

void drawWiFiIcon(int on) {
  tft.fillRoundRect(627, 210, 117, 85, 20, RA8875_WHITE);
  if (on == 1) {
    writeTxt(620, 295, "WiFi (ON)", RA8875_WHITE, 1, 1);
    tft.fillRect(650, 255, 20, 35, RA8875_BLACK);
    tft.fillRect(675, 235, 20, 55, RA8875_BLACK);
    tft.fillRect(700, 215, 20, 75, RA8875_BLACK);
  } else if (on == 0) {
    writeTxt(620, 295, "WiFi(OFF)", RA8875_WHITE, 1, 1);
    tft.fillRect(650, 280, 20, 10, RA8875_BLACK);
    tft.fillRect(675, 280, 20, 10, RA8875_BLACK);
    tft.fillRect(700, 280, 20, 10, RA8875_BLACK);
  }
}

void drawRstIcon(int state, int on) {
  if (on == 1) {
    if (state == 0) {
      tft.fillRoundRect(460, 340, 117, 85, 20, RA8875_WHITE);
      writeTxt(477, 350, "PANEL", RA8875_BLACK, 1, 0);
      writeTxt(477, 380, "RESET", RA8875_BLACK, 1, 0);
      writeTxt(480, 427, "RESET", RA8875_WHITE, 1, 1);
    } else if (state == 1) {
      tft.fillRoundRect(460, 340, 117, 85, 20, 0x7BEF);
      writeTxt(475, 350, "LIGHT", RA8875_WHITE, 1, 0);
      writeTxt(475, 380, "SENSOR", RA8875_WHITE, 1, 0);
      writeTxt(480, 427, "(OFF)", RA8875_WHITE, 1, 1);
    } else if (state == 2) {
      tft.fillRoundRect(460, 340, 117, 85, 20, RA8875_WHITE);
      writeTxt(475, 350, "LIGHT", RA8875_BLACK, 1, 0);
      writeTxt(475, 380, "SENSOR", RA8875_BLACK, 1, 0);
      writeTxt(480, 427, "(ON )", RA8875_WHITE, 1, 1);
    }
  } else if (on == 0) {
    tft.fillRoundRect(460, 340, 117, 85, 20, 0x7BEF);
    writeTxt(477, 350, "PANEL", RA8875_WHITE, 1, 0);
    writeTxt(477, 380, "RESET", RA8875_WHITE, 1, 0);
    writeTxt(480, 427, "RESET", RA8875_WHITE, 1, 1);
  }
}

void drawAutoIcon(int on) {
  if (on == 1) {
    tft.fillRoundRect(627, 340, 117, 85, 20, RA8875_WHITE);
    tft.fillRect(653, 375, 8, 20, RA8875_BLACK);
    tft.fillRect(710, 375, 8, 20, RA8875_BLACK);
    tft.fillRect(660, 365, 51, 42, RA8875_BLACK);
    tft.fillRect(682, 350, 7, 20, RA8875_BLACK);
    tft.fillRect(671, 407, 30, 10, RA8875_BLACK);
    tft.fillCircle(685, 350, 8, RA8875_BLACK);
    tft.fillCircle(672, 380, 7, RA8875_WHITE);
    tft.fillCircle(698, 380, 7, RA8875_WHITE);
    tft.fillRect(673, 395, 25, 8, RA8875_WHITE);
    writeTxt(620, 427, "AUTO (ON)", RA8875_WHITE, 1, 1);
  } else if (on == 0) {
    tft.fillRoundRect(627, 340, 117, 85, 20, 0x7BEF);
    tft.fillRect(653, 375, 8, 20, RA8875_WHITE);
    tft.fillRect(710, 375, 8, 20, RA8875_WHITE);
    tft.fillRect(660, 365, 51, 42, RA8875_WHITE);
    tft.fillRect(682, 350, 7, 20, RA8875_WHITE);
    tft.fillRect(671, 407, 30, 10, RA8875_WHITE);
    tft.fillCircle(685, 350, 8, RA8875_WHITE);
    tft.fillCircle(672, 380, 7, 0x7BEF);
    tft.fillCircle(698, 380, 7, 0x7BEF);
    tft.fillRect(673, 395, 25, 8, 0x7BEF);
    writeTxt(620, 427, "AUTO(OFF)", RA8875_WHITE, 1, 1);
  }
}



int calibrateTSPoint(tsPoint_t *displayPtr, tsPoint_t *screenPtr, tsMatrix_t *matrixPtr) {
  int retValue = 0;

  if (matrixPtr->Divider != 0) {
    displayPtr->x = ((matrixPtr->An * screenPtr->x) + (matrixPtr->Bn * screenPtr->y) + matrixPtr->Cn) / matrixPtr->Divider;

    displayPtr->y = ((matrixPtr->Dn * screenPtr->x) + (matrixPtr->En * screenPtr->y) + matrixPtr->Fn) / matrixPtr->Divider;
  } else {
    return -1;
  }

  return (retValue);
}

bool waitForTouchEvent(tsPoint_t *point) {
  /* Clear the touch data object and placeholder variables */
  memset(point, 0, sizeof(tsPoint_t));

  /* Clear any previous interrupts to avoid false buffered reads */
  uint16_t x, y;
  tft.touchRead(&x, &y);
  delay(1);

  /* Wait around for a new touch event (INT pin goes low) */
  // while (digitalRead(RA8875_INT)) {
  // }

  /* Make sure this is really a touch event */
  if (tft.touched()) {
    tft.touchRead(&x, &y);
    point->x = x;
    point->y = y;
    return true;
    // Serial.print("Touch: ");
    // Serial.print(point->x);
    // Serial.print(", ");
    // Serial.println(point->y);


  } else {
    point->x = 0;
    point->y = 0;
    return false;
  }
}

tsPoint_t renderCalibrationScreen(uint16_t x, uint16_t y, uint16_t radius) {
  tft.fillScreen(RA8875_WHITE);
  tft.drawCircle(x, y, radius, RA8875_RED);
  tft.drawCircle(x, y, radius + 2, 0x8410); /* 50% Gray */

  // Wait for a valid touch events
  tsPoint_t point = { 0, 0 };

  /* Keep polling until the TS event flag is valid */
  bool valid = false;
  while (!valid) {
    waitForTouchEvent(&point);
    if (point.x || point.y) {
      valid = true;
    }
  }

  return point;
}

void tsCalibrate(void) {
  tsPoint_t data;

  /* --------------- Welcome Screen --------------- */
  Serial.println("Starting the calibration process");
  data = renderCalibrationScreen(tft.width() / 2, tft.height() / 2, 5);
  delay(250);

  /* ----------------- First Dot ------------------ */
  // 10% over and 10% down
  data = renderCalibrationScreen(tft.width() / 10, tft.height() / 10, 5);
  _tsLCDPoints[0].x = tft.width() / 10;
  _tsLCDPoints[0].y = tft.height() / 10;
  _tsTSPoints[0].x = data.x;
  _tsTSPoints[0].y = data.y;
  // Serial.print("Point 1 - LCD");
  // Serial.print(" X: ");
  // Serial.print(_tsLCDPoints[0].x);
  // Serial.print(" Y: ");
  // Serial.print(_tsLCDPoints[0].y);
  // Serial.print(" TS X: ");
  // Serial.print(_tsTSPoints[0].x);
  // Serial.print(" Y: ");
  // Serial.println(_tsTSPoints[0].y);
  delay(250);

  /* ---------------- Second Dot ------------------ */
  // 50% over and 90% down
  data = renderCalibrationScreen(tft.width() / 2, tft.height() - tft.height() / 10, 5);
  _tsLCDPoints[1].x = tft.width() / 2;
  _tsLCDPoints[1].y = tft.height() - tft.height() / 10;
  _tsTSPoints[1].x = data.x;
  _tsTSPoints[1].y = data.y;
  // Serial.print("Point 2 - LCD");
  // Serial.print(" X: ");
  // Serial.print(_tsLCDPoints[1].x);
  // Serial.print(" Y: ");
  // Serial.print(_tsLCDPoints[1].y);
  // Serial.print(" TS X: ");
  // Serial.print(_tsTSPoints[1].x);
  // Serial.print(" Y: ");
  // Serial.println(_tsTSPoints[1].y);
  delay(250);

  /* ---------------- Third Dot ------------------- */
  // 90% over and 50% down
  data = renderCalibrationScreen(tft.width() - tft.width() / 10, tft.height() / 2, 5);
  _tsLCDPoints[2].x = tft.width() - tft.width() / 10;
  _tsLCDPoints[2].y = tft.height() / 2;
  _tsTSPoints[2].x = data.x;
  _tsTSPoints[2].y = data.y;
  // Serial.print("Point 3 - LCD");
  // Serial.print(" X: ");
  // Serial.print(_tsLCDPoints[2].x);
  // Serial.print(" Y: ");
  // Serial.print(_tsLCDPoints[2].y);
  // Serial.print(" TS X: ");
  // Serial.print(_tsTSPoints[2].x);
  // Serial.print(" Y: ");
  // Serial.println(_tsTSPoints[2].y);
  delay(250);

  /* Clear the screen */
  tft.fillScreen(RA8875_WHITE);

  // Do matrix calculations for calibration and store to EEPROM
  setCalibrationMatrix(&_tsLCDPoints[0], &_tsTSPoints[0], &_tsMatrix);
}

int setCalibrationMatrix(tsPoint_t *displayPtr, tsPoint_t *screenPtr, tsMatrix_t *matrixPtr) {
  int retValue = 0;

  matrixPtr->Divider = ((screenPtr[0].x - screenPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) - ((screenPtr[1].x - screenPtr[2].x) * (screenPtr[0].y - screenPtr[2].y));

  if (matrixPtr->Divider == 0) {
    retValue = -1;
  } else {
    matrixPtr->An = ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) - ((displayPtr[1].x - displayPtr[2].x) * (screenPtr[0].y - screenPtr[2].y));

    matrixPtr->Bn = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].x - displayPtr[2].x)) - ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].x - screenPtr[2].x));

    matrixPtr->Cn = (screenPtr[2].x * displayPtr[1].x - screenPtr[1].x * displayPtr[2].x) * screenPtr[0].y + (screenPtr[0].x * displayPtr[2].x - screenPtr[2].x * displayPtr[0].x) * screenPtr[1].y + (screenPtr[1].x * displayPtr[0].x - screenPtr[0].x * displayPtr[1].x) * screenPtr[2].y;

    matrixPtr->Dn = ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].y - screenPtr[2].y)) - ((displayPtr[1].y - displayPtr[2].y) * (screenPtr[0].y - screenPtr[2].y));

    matrixPtr->En = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].y - displayPtr[2].y)) - ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].x - screenPtr[2].x));

    matrixPtr->Fn = (screenPtr[2].x * displayPtr[1].y - screenPtr[1].x * displayPtr[2].y) * screenPtr[0].y + (screenPtr[0].x * displayPtr[2].y - screenPtr[2].x * displayPtr[0].y) * screenPtr[1].y + (screenPtr[1].x * displayPtr[0].y - screenPtr[0].x * displayPtr[1].y) * screenPtr[2].y;

#if defined(EEPROM_SUPPORTED)
    // Write the calibration matrix to the EEPROM
    tft.writeCalibration(EEPROMLOCATION, matrixPtr);
#endif
  }
  Serial.println(matrixPtr->Divider);
  Serial.println(matrixPtr->An);
  Serial.println(matrixPtr->Bn);
  Serial.println(matrixPtr->Cn);
  Serial.println(matrixPtr->Dn);
  Serial.println(matrixPtr->En);
  Serial.println(matrixPtr->Fn);
  return (retValue);
}

void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color) {

  int16_t i, j, byteWidth = (w + 7) / 8;
  uint8_t byte;

  for (j = 0; j < h; j++) {
    for (i = 0; i < w; i++) {
      if (i & 7) byte <<= 1;
      else byte = pgm_read_byte(bitmap + j * byteWidth + i / 8);
      if (byte & 0x80) tft.drawPixel(x + i, y + j, color);
    }
  }
}

//pull data from charge controller
void CCData() {
  Wire.requestFrom(8, 27);  // request 27 bytes from slave device #8
  while (Wire.available()) {
    battery_soc = Wire.read();
    Wire.readBytes((byte *)&battery_voltage, sizeof(float));
    Wire.readBytes((byte *)&battery_charging_amps, sizeof(float));
    battery_temperature = Wire.read();
    controller_temperature = Wire.read();
    Wire.readBytes((byte *)&load_voltage, sizeof(float));
    Wire.readBytes((byte *)&load_amps, sizeof(float));
    load_watts = Wire.read();
    Wire.readBytes((byte *)&solar_panel_voltage, sizeof(float));
    Wire.readBytes((byte *)&solar_panel_amps, sizeof(float));
    solar_panel_watts = Wire.read();
  }
}

void bmeData() {
  enviornment_temp = ((bme.readTemperature() * 1.8) + 32);
  enviornment_pressure = (bme.readPressure() / 100.0F) * 0.030;
  enviornment_altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  enviornment_humidity = bme.readHumidity();
}

void accelSetup() {
  if (!accel.begin()) {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while (1)
      ;
  }
  /* Display some basic information on this sensor */
  //displaySensorDetails();
  accel.setRange(LSM303_RANGE_4G);
  //Serial.print("Range set to: ");
  lsm303_accel_range_t new_range = accel.getRange();
  switch (new_range) {
    case LSM303_RANGE_2G:
      //Serial.println("+- 2G");
      break;
    case LSM303_RANGE_4G:
      //Serial.println("+- 4G");
      break;
    case LSM303_RANGE_8G:
      //Serial.println("+- 8G");
      break;
    case LSM303_RANGE_16G:
      //Serial.println("+- 16G");
      break;
  }
  accel.setMode(LSM303_MODE_NORMAL);
  //Serial.print("Mode set to: ");
  lsm303_accel_mode_t new_mode = accel.getMode();
  switch (new_mode) {
    case LSM303_MODE_NORMAL:
      //Serial.println("Normal");
      break;
    case LSM303_MODE_LOW_POWER:
      //Serial.println("Low Power");
      break;
    case LSM303_MODE_HIGH_RESOLUTION:
      //Serial.println("High Resolution");
      break;
  }
}


void accelPoll() {
  sensors_event_t event;
  accel.getEvent(&event);
  float Pi = 3.14159;
  angle = (atan2(event.acceleration.y, event.acceleration.z) * 180) / Pi;
}

void magSetup() {
  if (!mag.begin()) {
    /* There wasa problem detecting the LIS2MDL ... check your connections */
    Serial.println("Ooops, no LIS2MDL detected ... Check your wiring!");
    while (1)
      ;
  }
}

void magPoll() {
  sensors_event_t event;
  mag.getEvent(&event);

  float Pi = 3.14159;

  // Calculate the angle of the vector y,x
  float offsety = 65 + event.magnetic.y;
  float offsetx = 60 + event.magnetic.x;
  heading = ((atan2(offsety, offsetx)) * 180) / Pi;
}

float getAlt(float lat, float sd, float timeP) {
  float hourAngle = getHourAngle(timeP);
  return degrees(asin(cos(radians(lat)) * cos(radians(sd)) * cos(radians(hourAngle)) + sin(radians(lat)) * sin(radians(sd))));
}

float getAzi(float lat, float sd, float timeP, float alt) {
  float hourAngle = getHourAngle(timeP);
  float azi2;
  if (timeP < 12) {
    azi2 = (180.0 - degrees(asin(cos(radians(sd)) * sin(radians(hourAngle)) / cos(radians(alt)))));
  } else {
    azi2 = (180.0 - degrees(asin(cos(radians(sd)) * sin(radians(hourAngle)) / cos(radians(alt)))) - 360.0);
  }
  if (cos(radians(hourAngle)) >= ((tan(radians(sd))) / (tan(radians(lat))))) {
    return (degrees(asin(cos(radians(sd)) * sin(radians(hourAngle)) / cos(radians(alt)))));
  } else {
    return azi2;
  }
}

float getHourAngle(float timeInput) {
  return (12 - timeInput) * 15;
}

float getSolarDec(float jD) {
  return 23.45 * sin(radians(360.0 / 365 * (jD - 81)));
}

float getTimeP(int h, int m) {
  return (float)h + ((float)m) / 60;
}

float getJD(int y, int m, int d) {
  if (y % 400 != 0) {
    switch (m) {
      case 1:
        return d;
        break;
      case 2:
        return 31 + d;
        break;
      case 3:
        return 59 + d;
        break;
      case 4:
        return 90 + d;
        break;
      case 5:
        return 120 + d;
        break;
      case 6:
        return 151 + d;
        break;
      case 7:
        return 181 + d;
        break;
      case 8:
        return 212 + d;
        break;
      case 9:
        return 243 + d;
        break;
      case 10:
        return 273 + d;
        break;
      case 11:
        return 304 + d;
        break;
      case 12:
        return 334 + d;
        break;
    }
  } else {
    switch (m) {
      case 1:
        return d;
        break;
      case 2:
        return 31 + d;
        break;
      case 3:
        return 60 + d;
        break;
      case 4:
        return 91 + d;
        break;
      case 5:
        return 121 + d;
        break;
      case 6:
        return 152 + d;
        break;
      case 7:
        return 182 + d;
        break;
      case 8:
        return 213 + d;
        break;
      case 9:
        return 244 + d;
        break;
      case 10:
        return 274 + d;
        break;
      case 11:
        return 305 + d;
        break;
      case 12:
        return 335 + d;
        break;
    }
  }
}

void wifiStatus(){
  int status = WiFi.status();
  if(status ==WL_CONNECTED){
    writeTxt(160, 17, "Server Online ", RA8875_WHITE, 2, 1);
  }else{
    writeTxt(160, 17, "Server Offline", RA8875_WHITE, 2, 1);
  }
}



bool wifiConnect() {
  int wifiStartTime = millis();
  writeTxt(0, 250, "Connecting  ", RA8875_WHITE, 2, 1);
  int status = WiFi.begin(ssid, pass);  //attempt to establish a WiFi connection at the given network

  if (status != WL_CONNECTED) {

    Serial.println("Couldn't get a wifi connection");  //Tell the user that the system could not establish a connection
    while (true) {
      if (millis() - wifiStartTime > 5000) {
        WiFi.end();
        writeTxt(0, 250, "Connect fail    ", RA8875_WHITE, 2, 1);
        
        return false;
      }
    }  //loop until a connection is established

  }

  else {

    Serial.println("Connected to network");  //Tell user the system was able to connect to the internet
    IPAddress ip = WiFi.localIP();           //Retrieve the ip address of the system
    Serial.print("My IP address is: ");      //Print out the ip address to the serial monitor
    Serial.println(ip);                      //Print out the ip address to the serial monitor
    
    conn.connect(server_addr, 3306, user, password);
    
    writeTxt(0, 250, "Connect Done", RA8875_WHITE, 2, 1);
    // bmeData();
    // startPDTime=endTickTime;
    // pushSensorData(enviornment_temp,enviornment_pressure,enviornment_humidity,enviornment_altitude);
    // // Serial.println(endTickTime);
    // // Serial.println(startPDTime);
    // endTickTime=millis();
    // startPDTime=endTickTime;
    
    return true;
  }
  return false;
}

void wifiCloseConnection() {

  WiFi.end();
}

void sendDataFSM(){
  if(WIFISTAT&&((endTickTime-startPDTime)>60000)){
    bmeData();

    Serial.println(enviornment_temp);
    Serial.println(enviornment_pressure);
    Serial.println(enviornment_humidity);
    Serial.println(enviornment_altitude);

    char bmeTemperature[8];
    char bmePressure[8];
    char bmeAltitude[8];
    char bmeHumidity[8];

    dtostrf(enviornment_temp, 6, 2, bmeTemperature);
    dtostrf(enviornment_pressure, 6, 2, bmePressure);
    dtostrf(enviornment_altitude, 6, 2, bmeAltitude);
    dtostrf(enviornment_humidity, 6, 2, bmeHumidity);

    Serial.println(bmeTemperature);
    Serial.println(bmePressure);

    String newQuery = String(sqlQuery) + String(bmeTemperature) + "," + String(bmePressure) + "," + String(bmeAltitude) + "," + String(bmeHumidity) + ")";
    Serial.println(newQuery);

    char charArray[150];
    newQuery.toCharArray(charArray, 150);
    Serial.println(charArray);

    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
    // Execute the query
    cur_mem->execute(deleteQuery);
    cur_mem->execute(charArray);
    // Note: since there are no results, we do not need to read any data
    // Deleting the cursor also frees up memory used
    delete cur_mem;

    selectControlData();

    startPDTime=endTickTime;
    
  }
}

void selectControlData() {

    row_values *row = NULL;

    MySQL_Cursor *cur_mem1 = new MySQL_Cursor(&conn);

    cur_mem1->execute(selectQuery);

    column_names *columns = cur_mem1->get_columns();

    do {
    row = cur_mem1->get_next_row();
    if (row != NULL) {
      manual = atol(row->values[0]);
      schedule = atol(row->values[1]);
      automatic = atol(row->values[2]);
    }
  } while (row != NULL);
  // Deleting the cursor also frees up memory used
    delete cur_mem1;

    Serial.println(manual);
    Serial.println(schedule);
    Serial.println(automatic);

}

