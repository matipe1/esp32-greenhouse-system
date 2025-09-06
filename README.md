# Introducción

---

Se utilizará un entorno simulado para el ESP32 mediante extensiones de VSCode. Estas son: **Wokwi**, **PlatformIO** y **C/C++**

Proyecto llevado a cabo con un microcontrolador ESP32, consiste en un **Sistema de control automático para un invernadero**, en donde se usan los siguientes componentes:

- **Sensores**
    
    Dentro del invernadero se utilizarán los sensores **DHT22** (para la temperatura y humedad) y un **Potenciómetro** (valor de referencia de temperatura).
    
- **Actuadores**
    
    Se utilizarán **LEDs** para determinar cuando el sensor de temperatura determine que se superó el valor de referencia brindado por el potenciómetro, indicando que el sistema de ventilación se activó (led encendido).
    
    Por otro lado, en caso de que el valor de humedad caiga por debajo de un umbral (generado en un rango entre 40% y 60%), se utilizará un led, que encienda y apague intermitentemente, simulando riego automático.
    
- **Interfaz**
    
    **Pantalla OLED (ssd1306)** en donde se mostrarán los siguientes valores:
    
    ```
    * Valor de referencia de temperatura (potenciómetro)
    * Umbral mínimo generado aleatoriamente (valor mínimo)
    * Valor real de temperatura
    * Valor real de humedad
    ```
    
- **Comunicación**
    
    Utilicé un Puerto en Serie para loguear los eventos, con una velocidad de 9600 baudios. Esta es la velocidad en que se van enviando los “mensajes”, tomados por los sensores, a la PC o monitor serial.
