/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.
This is the responder unit for the person identification main.ino code. 
this responder.cpp unit has been 
***************designed by Dhruv Sheth* under GPL3+ License.**********************************
This project has been designed for Arduino touch free-challenge
Functioning: Usually, in malls or supermarkets, many people touch certain objects in aisles and it is
really hard to monitor each object and sanitise each aisle individually. Most of the times, manual
sanitisation is completed after each day and individual sanitisation of each aisle is not possible.
Sometimes it happens that the food-aisle is more contaminated and the clothes-aisle is less contaminated, hence
extra care is required for monitoring and sanitisatio of food-aisle. But such a monitoring solution is not
possible manually. Therefore, I decided to make an autonomous solution based on TinyML
deployed on Arduino Nano 33 BLE Sense. The person-identification model is highly accurate and uses
an arducam which can be used in 5mp or 2mp variants. In solutions that contain ultrasonic sensors, or
Lidar sensors, the readings or measurements are not accurate and can send false readings. Hence,
the person detction model uses person recogition technology based on TensorflowLite framework.
When, 1-50 people are detcted, a green light is switched on which means that the area is yet
secure. When, 51-75 people are detected, a yellow light is flashed which means that awareness is required.
When, 76-100 people are detected, a red light is flashed which means that people need to carefully touch objects and
only touch those objects which are required. When 101-110 people are detected, the UV sanitisation 
system is activated which sanitises the complete area and objects are secure again.
The count is again reset to 0 and the cycle is looped. The people vs Time graph is potrayed on 
the ThingSpeak web app where mall owners can track the time when the crowd is more or less and accordingly
maintain more awareness during peak timings.

==============================================================================*/

#if defined(ARDUINO) && !defined(ARDUINO_ARDUINO_NANO33BLE)
#define ARDUINO_EXCLUDE_CODE
#endif  // defined(ARDUINO) && !defined(ARDUINO_ARDUINO_NANO33BLE)
#ifndef ARDUINO_EXCLUDE_CODE
#include "detection_responder.h"
#include "Arduino.h"
#include ESP32_Servo.h
#include <WiFi.h>
#include <WiFiMulti.h>
WiFiMulti WiFiMulti;
const char* ssid     = "Yourssid"; // Your SSID (Name of your WiFi)
const char* password = "Wifipass"; //Your Wifi password
const char* host = "api.thingspeak.com";
String api_key = "RYYSAR2DUGXAMBIC"; // Your API Key provied by thingspeak
int count = 0;
const int LEDGREEN = 8;
const int LEDYELLOW = 5;
const int LEDRED = 3;
const int UV = 6;

void setup() 
{
  pinMode(LEDGREEN, OUTPUT);
  pinMode(LEDYELLOW, OUTPUT);
  pinMode(LEDRED, OUTPUT);
  pinMode(UV, OUTPUT);
  Serial.begin(9600);
    Connect_to_Wifi();
}


void loop(){

 // call function to send data to Thingspeak
  Send_Data();

  delay(10);
}

// Flash the blue LED after each inference
void RespondToDetection(tflite::ErrorReporter* error_reporter,
                        uint8_t person_score, uint8_t no_person_score) {
  static bool is_initialized = false;
  if (!is_initialized) {
    // Pins for the built-in RGB LEDs on the Arduino Nano 33 BLE Sense
    pinMode(LEDR, OUTPUT);
    pinMode(LEDG, OUTPUT);
    pinMode(LEDB, OUTPUT);
    is_initialized = true;
  }

  // Note: The RGB LEDs on the Arduino Nano 33 BLE
  // Sense are on when the pin is LOW, off when HIGH.

  // Switch the person/not person LEDs off
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDR, HIGH);

  // Flash the blue LED after every inference.
  digitalWrite(LEDB, LOW);
  delay(100);
  digitalWrite(LEDB, HIGH);

  // Switch on the green LED when a person is detected,
  // the red when no person is detected
  if (person_score > no_person_score) {
    digitalWrite(LEDG, LOW);
    digitalWrite(LEDR, HIGH);
    count++
  } else {
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDR, LOW);
  }

  TF_LITE_REPORT_ERROR(error_reporter, "Person score: %d No person score: %d",
                       person_score, no_person_score);
}


  if (constrain(count, 1, 50)) {
  digitalWrite(LEDGREEN, LOW); // This is a green LED which means area is not contaminated yet
}
  if (constrain(count, 51, 75)) {
  digitalWrite(LEDYELLOW, LOW); // This is a yellow led so that people are alert that more than 50 people
}
 if (constrain(count, 76,100)) {
   digitalWrite(LEDRED, LOW); // This is a red LED which warns people that this area is contaminated and one should take care while touching objects
}
 if (constrain(count, 101, 110)) {
   digitalWrite(UV, LOW); // This is an Ultraviolet Light which ensures that after 100 people have touched the aisle objects, it sanitises the area with UV light
 }
 if (count > 111) {
   (count = 0); // this resets the person count to 0 once the area has been sanitised by UV light
  } 
 } // In this way, a simple math function has been written by me on the arduino nano 33 ble sense which alerts people and sanitises areas
/* cc - Dhruv Sheth */ 

void Connect_to_Wifi()
{

  // We start by connecting to a WiFi network
  WiFiMulti.addAP(ssid, password);

  Serial.println();
  Serial.println();
  Serial.print("Wait for WiFi... ");

  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(10);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}

void Send_Data()
{

  Serial.println("Prepare to send data");

  // Use WiFiClient class to create TCP connections
  WiFiClient client;

  const int httpPort = 80;

  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  else
  {
    String data_to_send = api_key;
    data_to_send += "&field1=";
    data_to_send += (count);
    data_to_send += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + api_key + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(data_to_send.length());
    client.print("\n\n");
    client.print(data_to_send);

    delay(10);
  }

  client.stop();

}
#endif  // ARDUINO_EXCLUDE_CODE

