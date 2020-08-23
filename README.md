# self-sanitisation-person-detection
This project has been designed for Arduino touch free-challenge Functioning: Usually, in malls or supermarkets, many people touch certain objects in aisles and it is really hard to monitor each object and sanitise each aisle individually. Most of the times, manual sanitisation is completed after each day and individual sanitisation of each aisle is not possible. Sometimes it happens that the food-aisle is more contaminated and the clothes-aisle is less contaminated, hence extra care is required for monitoring and sanitisatio of food-aisle. But such a monitoring solution is not possible manually. Therefore, I decided to make an autonomous solution based on TinyML deployed on Arduino Nano 33 BLE Sense. The person-identification model is highly accurate and uses an arducam which can be used in 5mp or 2mp variants. In solutions that contain ultrasonic sensors, or Lidar sensors, the readings or measurements are not accurate and can send false readings. Hence, the person detction model uses person recogition technology based on TensorflowLite framework. When, 1-50 people are detcted, a green light is switched on which means that the area is yet secure. When, 51-75 people are detected, a yellow light is flashed which means that awareness is required. When, 76-100 people are detected, a red light is flashed which means that people need to carefully touch objects and only touch those objects which are required. When 101-110 people are detected, the UV sanitisation  system is activated which sanitises the complete area and objects are secure again. The count is again reset to 0 and the cycle is looped. The people vs Time graph is potrayed on  the ThingSpeak web app where mall owners can track the time when the crowd is more or less and accordingly maintain more awareness during peak timings.


**Labels in the Model trained:**

``` C
#include "model_settings.h"

const char* kCategoryLabels[kCategoryCount] = {
    "unused",
    "person",
    "notperson",
};
 ```
  
 # Circuit Diagram
 ![alt text](https://hackster.imgix.net/uploads/attachments/1007722/arduino_ble_with_battery_bb_Wq6pNrk8fW.png?auto=compress%2Cformat&w=680&h=510&fit=max
)

![alt text](https://github.com/dhruvsheth-ai/Person-queuing-system-arduino33/blob/master/Pinout-NANOsense_latest-1.jpg)
