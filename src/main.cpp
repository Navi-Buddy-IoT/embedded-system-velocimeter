#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include "time.h"
#include "http-client.hpp"
#include "wifi-setup.hpp"
#include "velocity-sensor.hpp"



//declaraciones
#define DEVICE_ID 1

#define ALERT_LED 4

String authToken;

LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Hello, ESP32!");
  
  pinMode(ALERT_LED, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Starting...");
  delay(500);
  

  setupWiFi();
  authToken = login();
  setupSensor();

  delay(1000);
  configTime(-9000, -9000, "1.south-america.pool.ntp.org");

  lcd.clear();
}

void loop() {
  float velocity = measureVelocity();

  if(!isnan(velocity)){

    if(velocity >= 40){
    digitalWrite(ALERT_LED,HIGH);
    delay(300);
    digitalWrite(ALERT_LED,LOW);
    delay(300);
    }
    else{
      digitalWrite(ALERT_LED,LOW);
    }

    lcd.setCursor(1,0);
    lcd.print("Velocimetro");

    lcd.setCursor(0,1);
    lcd.print("Vlct: ");
    lcd.print(velocity);
    lcd.println(" Km/h      ");

    String requestBody = String("{\"velocity\":") + velocity + ",\"bicycleId\":" + DEVICE_ID + "}";
    Serial.print(requestBody);
    int responseCode = sendUpdateRequest(requestBody.c_str(), authToken.c_str(), DEVICE_ID);
    checkResponseCode(responseCode); 
  }
}

