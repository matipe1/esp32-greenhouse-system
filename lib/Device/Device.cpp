#include "Device.h"

Device::Device(int w, int h, int reset, int pinDHT, int model): // Ahora la lista de los objetos que queremos inicializar
  _sensor(pinDHT, model),
  _display(w, h, &Wire, reset)
{

}

void Device::begin(){
  _display.begin(0X3C, true);
  _display.setTextSize(1);
  _display.setTextColor(SH110X_WHITE);
  _sensor.begin();
}

void Device::showDisplay(const char *text)
{
  _display.clearDisplay();
  _display.setCursor(0, 0);
  _display.print(text);
  _display.display();
}

float Device::readTemp()
{
  return _sensor.readTemperature();
}

float Device::readHum()
{
  return _sensor.readHumidity();
}