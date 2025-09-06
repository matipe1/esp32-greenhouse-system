#include "Device.h"

// Variables globales
#define LED 23
#define POT 32
Device device(128, 64, -1, 14, DHT22);

void setup() {
  Serial.begin(9600);
  device.begin();
  pinMode(LED, OUTPUT);
}

void loop() {
  char text[64]; // Array de letras (string)

  float h = device.readHum();
  float t = device.readTemp();
  int valor = analogRead(POT); // Devuelve bits (2^12 posibles valores = 0-4096)
  analogWrite(LED, map(valor, 0, 4095, 0, 255)); // Mapeo debido a que podemos tener esos valores. Salida 8 bits
  float tempRef = 15 + (valor / 4095.0) * (35 - 15);

  sprintf(text, "Temp: %.1f C\nHum: %.1f %%\nValor de ref: %d \nVoltios: %.2f", t, h, tempRef, valor * 3.3 /4096);
  device.showDisplay(text);
}