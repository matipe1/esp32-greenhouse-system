// Primera vez que compila define "DEVICE", si ya esta creada no la define más.
#ifndef DEVICE
#define DEVICE

#include <Arduino.h>
#include <DHT.h>
#include <Adafruit_SH110X.h>
#include <Wire.h>

class Device {

  public:
    Device(int w, int h, int reset, int pinDHT, int model);
    void begin();
    void showDisplay(const char *text);
    float readTemp();
    float readHum();
    // void loop();
    /* No implementamos el loop*/

  private:
    DHT _sensor;
    Adafruit_SH1106G _display;
};

#endif