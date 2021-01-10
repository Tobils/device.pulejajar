/**
 * dev.suhada@gmail.com
 * Bogor, 27 Dec, 2020
 **/
#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "sdkconfig.h"
#include <Arduino.h>
#include <WiFi.h>
#include <ESP32Ping.h>
#include <HTTPClient.h>
#include <WiFiUdp.h>
#include "time.h"
#include "string.h"

const char *serverName = "http://192.168.8.103:8000/api/v1/device";
String deviceToken = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiIyNzEwMTk5NiIsIm5hbWUiOiJwYXNhaW5zLW1hbnRhZi1wdWxlamFqYXIifQ.teHVLP8-xvxx5IcPlq3jtvFxO5FijmTO1UXaFYKdcx0";
const char *ssid = "XLGO-7218";
const char *password = "91609300";

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 25200;
const int daylightOffset_sec = 3600;

unsigned long lastTime = 0;
unsigned long timerDelay = 3000;

String response;
String _date;
String _time;
byte _day;
byte _month;
int _year;
int _hour;
int _min;
int _sec;

/** connect to wifi **/
void connectToWifi()
{
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Connecting to wifi ...");
  }

  Serial.println("Connected to wifi");
  delay(100);
}

/** get date time */
void getDateTime()
{

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return;
  }

  _day = timeinfo.tm_mday;
  _month = timeinfo.tm_mon + 1;
  _year = timeinfo.tm_year + 1900;
  _hour = timeinfo.tm_hour;
  _min = timeinfo.tm_min;
  _sec = timeinfo.tm_sec;

  _date = String(_year) + ":" + String(_month) + ":" + String(_day);

  String tmp_hour, tmp_min, tmp_sec;
  if (_hour < 10)
  {
    tmp_hour = "0" + String(_hour);
  }
  else
  {
    tmp_hour = String(_hour);
  }

  if (_min < 10)
  {
    tmp_min = "0" + String(_min);
  }
  else
  {
    tmp_min = String(_min);
  }
  if (_sec < 10)
  {
    tmp_sec = "0" + String(_sec);
  }
  else
  {
    tmp_sec = String(_sec);
  }

  _time = tmp_hour + ":" + tmp_min + ":" + tmp_sec;
};

/* read sensor suhu */
double getSuhu()
{
  return random(20.00, 30.00);
}

/* read sensor tds */
double getTds()
{
  return random(150.00, 300.00);
}

/* read sensor ph */
double getpH()
{
  return random(5.00, 8.00);
}

/* read sensor debit */
double getDebit()
{
  return random(205.00, 800.00);
}

/**
 * send data suhu, tds, ph dan debit
 * **/
void postData(double suhu, double tds, double ph, double debit)
{
  HTTPClient api;
  api.begin(serverName);
  api.addHeader("Content-Type", "application/json");
  String requestBody = "{\"meta\":{\"token\":\"" + deviceToken + "\"}, \"payload\":{\"data_sensor\":{\"suhu\":\"" + suhu + "\",\"tds\":\"" + tds + "\",\"ph\":\"" + ph + "\",\"debit\":\"" + debit + "\"}, \"date\":\"" + _date + "\", \"time\":\"" + _time + "\"}}";
  int responseCode = api.POST(requestBody.c_str());

  if (responseCode < 0)
  {
    Serial.println("DATA GAGAL DIKIRIM");
  }

  response = api.getString();
  Serial.print("HTTP Response code: ");
  Serial.println(responseCode);
  Serial.print("\n\n<=============REQUEST BODY============>\n");
  Serial.println(requestBody);
  Serial.print("<=========================> \n\n");
  Serial.print("\n\n<=============RESPONSE============>\n");
  Serial.println(response);
  Serial.print("<=========================> \n\n");
  api.end();
}

void setup()
{
  Serial.begin(115200);
  connectToWifi();
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  delay(100);
}

void loop()
{
  if ((millis() - lastTime) > timerDelay)
  {
    bool success = Ping.ping("www.google.com", 3);
    if (!success)
    {
      Serial.println("Ping failed");
      connectToWifi();
    }
    else
    {
      /* ambil date, time dan data sensor */
      getDateTime();
      double suhu = getSuhu();
      double tds = getTds();
      double ph = getpH();
      double debit = getDebit();
      postData(suhu, tds, ph, debit);
    }
  }
  delay(5000);
}
