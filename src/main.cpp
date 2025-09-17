// Librerías necesarias para el proyecto
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>

// --- Definiciones de pines y constantes ---
#define DHTPIN 5       // Pin al que se conecta el sensor DHT22
#define DHTTYPE DHT22  // Tipo de sensor
#define POTENCIOMETRO_PIN 32   // Pin del potenciómetro
#define LED_VENTILACION_PIN 4  // Pin del LED para ventilación
#define LED_RIEGO_PIN 12       // Pin del LED para riego
#define SCREEN_WIDTH 128       // Ancho del display OLED
#define SCREEN_HEIGHT 64       // Alto del display OLED
#define BUTTON_PIN 14

// --- Inicialización de objetos ---
DHT dht(DHTPIN, DHTTYPE); // Objeto para el sensor DHT22
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); // Objeto para el OLED

// --- Variables globales ---
int estadoVentilacion = 0;      // 0 = Apagado, 1 = Encendido
int estadoRiego = 0;            // 0 = Apagado, 1 = Encendido
int umbralHumedadMin;           // Umbral de humedad mínima para el riego
unsigned long tiempoUltimoRiego = 0; // Para controlar el tiempo del LED de riego intermitente
const long intervaloRiego = 500;    // Intervalo de parpadeo del LED de riego (500 ms)

// --- Prototipos de funciones ---
void generarUmbralAleatorio();
void controlarRiego();

// --- Setup: Se ejecuta una vez al inicio del programa ---
void setup() {
  Serial.begin(115200); // Inicia la comunicación serial
  dht.begin(); // Inicia el sensor DHT

  // Configura los pines de los LEDs como salida
  pinMode(LED_VENTILACION_PIN, OUTPUT);
  pinMode(LED_RIEGO_PIN, OUTPUT);

  // Inicia el display OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Error al iniciar el display OLED."));
    for (;;); // No continuar si el OLED falla
  }
  display.clearDisplay(); // Limpia el display
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Genera el umbral de humedad aleatorio al inicio y lo muestra
  generarUmbralAleatorio();
  Serial.println("--- Sistema Iniciado ---");
  Serial.println("Umbral de humedad minima generado: " + String(umbralHumedadMin) + " %");

  pinMode(BUTTON_PIN, INPUT_PULLUP); // activamos resistencia interna
}

// --- Loop: Se ejecuta repetidamente ---
void loop() {
  // Esperar unos segundos entre mediciones
  delay(2000);

  // Leer la humedad y temperatura
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Leer el valor del potenciómetro y mapearlo a un rango de temperatura (0 a 40°C)
  int valorPotenciometro = analogRead(POTENCIOMETRO_PIN);
  float tempReferencia = map(valorPotenciometro, 0, 4095, 0, 40);

  // --- Lógica de control de ventilación ---
  if (t > tempReferencia) {
    if (estadoVentilacion == 0) {
      Serial.println("Ventilacion activada. Temperatura > Referencia.");
      estadoVentilacion = 1;
    }
    digitalWrite(LED_VENTILACION_PIN, HIGH); // Enciende el LED
  } else {
    if (estadoVentilacion == 1) {
      Serial.println("Ventilacion desactivada.");
      estadoVentilacion = 0;
    }
    digitalWrite(LED_VENTILACION_PIN, LOW); // Apaga el LED
  }

  // --- Lógica de control de riego ---
  controlarRiego();

  // --- Visualización en el OLED ---
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("T: ");
  display.print(t);
  display.print("C  H: ");
  display.print(h);
  display.println(" %");
  display.println("-----------------");
  display.print("Temp Ref: ");
  display.print(tempReferencia);
  display.println(" C");
  display.print("Hum Min: ");
  display.print(umbralHumedadMin);
  display.println(" %");
  display.println("-----------------");
  display.print("Ventilacion: ");
  display.println(estadoVentilacion == 1 ? "ON" : "OFF");
  display.print("Riego: ");
  display.println(estadoRiego == 1 ? "ON" : "OFF");
  display.display();

  // --- Imprimir en el monitor serial ---
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.print(" C | Humedad: ");
  Serial.print(h);
  Serial.print(" % | Temp Referencia: ");
  Serial.print(tempReferencia);
  Serial.print(" C | Humedad Min.: ");
  Serial.print(umbralHumedadMin);
  Serial.print(" % | Ventilacion: ");
  Serial.print(estadoVentilacion == 1 ? "ON" : "OFF");
  Serial.print(" | Riego: ");
  Serial.println(estadoRiego == 1 ? "ON" : "OFF");

  if (digitalRead(BUTTON_PIN) == LOW) {
    Serial.println("Boton presionado!");
    delay(200); // pequeño anti-rebote
  }
}

// --- Funciones de control ---

// Genera un umbral de humedad aleatorio entre 40% y 60%
void generarUmbralAleatorio() {
  randomSeed(micros() + analogRead(34)); // Usa micros() y un pin analógico para una semilla verdaderamente aleatoria
  umbralHumedadMin = random(40, 61); // Genera un número entre 40 y 60
}

// Controla el LED de riego de forma intermitente
void controlarRiego() {
  float humedad = dht.readHumidity();
  unsigned long tiempoActual = millis();

  if (humedad < umbralHumedadMin) {
    // Si la humedad es baja, activar el riego
    if (estadoRiego == 0) { // Si el estado cambia a encendido
      estadoRiego = 1;
    }
    // Controla el parpadeo del LED
    if (tiempoActual - tiempoUltimoRiego >= intervaloRiego) {
      digitalWrite(LED_RIEGO_PIN, !digitalRead(LED_RIEGO_PIN)); // Cambia el estado del LED
      tiempoUltimoRiego = tiempoActual;
    }
  } else {
    // Si la humedad es suficiente, apagar el riego
    if (estadoRiego == 1) { // Si el estado cambia a apagado
      estadoRiego = 0;
    }
    digitalWrite(LED_RIEGO_PIN, LOW);
  }
}