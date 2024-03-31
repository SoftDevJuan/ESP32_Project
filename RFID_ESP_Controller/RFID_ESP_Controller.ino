
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <ESP32Servo.h>
#include "config.h"  // Sustituir con datos de tu red
#include "API.hpp"
#include "ESP32_Utils.hpp"
#define LED_BUILTIN 33
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN    5  // Pin GPIO para el pin SS (SDA) del módulo RC522
#define RST_PIN   2 // Pin GPIO para el pin RST del módulo RC522 (puedes elegir otro si lo prefieres)

const int boton1 = 14;
const int boton2 = 12;
const int buzzer = 27;
const int ledRojo = 32;
const int servoPin = 15;
MFRC522 mfrc522(SS_PIN, RST_PIN); // Crea el objeto MFRC522


int pinLed = LED_BUILTIN;
struct RFID *rafid, rfd;
Servo motorcito;
StaticJsonDocument<JSON_DOC_SIZE> doc;




///////////////////////////////////////////////////////////////////////
void setup() 
{

   Serial.begin(115200);
    motorcito.attach(servoPin);
    pinMode(pinLed, OUTPUT);
    pinMode(boton1, INPUT_PULLUP);
    pinMode(boton2, INPUT_PULLUP);
    pinMode(buzzer, OUTPUT);
    pinMode(ledRojo, OUTPUT);
    delay(300);
    Serial.println("Conectandose al servidor..");
    ConnectWiFi_STA();
    SPI.begin();           // Inicializa SPI bus
    mfrc522.PCD_Init();    // Inicializa el lector RFID
    Serial.print("inicializando porgrama");
  for(int i = 0; i <= 10;i++){
    Serial.print(".");
    delay(500);
    
  }
  Serial.println("Bienvenido");

}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void loop() 
{

  int statusBoton1 = digitalRead(boton1);
  int statusBoton2 = digitalRead(boton2);
  String rfidString1 = "a2c3b4f100";
  String rfidString2 = "e37a3295";

  if(statusBoton1 == LOW){
    strlcpy(rfd.rfidnumber, "a2c3b4f100", sizeof(rfd.rfidnumber));
    registrarIntento(rfd);
    rfd = GetAcceso(rfidString1);
    String accesoString = String(rfd.acceso);
     // Evaluar el valor de acceso
            if (accesoString.equals("Permitido")) {
                Serial.println("Acceso Permitido. Realizar acción A");
                // Realizar acción A
                digitalWrite(pinLed, HIGH);
                tone(buzzer, 500);
                delay(1000);
                digitalWrite(pinLed, LOW);
                noTone(buzzer);
                delay(200);
                motorcito.write(180);
                delay(2000);
                motorcito.write(0);
                delay(200);
               

                
            } else if (accesoString.equals("Denegado")) {
                Serial.println("Acceso Denegado. Realizar acción B");
                // Realizar acción B
                digitalWrite(ledRojo, HIGH);
                tone(buzzer, 1000);
                delay(1000);
                digitalWrite(ledRojo, LOW);
                noTone(buzzer);


            } else {
                Serial.println("Respuesta no reconocida");
                Serial.println(accesoString);
                // Realizar acción C (si es necesario)
                digitalWrite(ledRojo, HIGH);
                tone(buzzer, 1000);
                delay(1000);
                digitalWrite(ledRojo, LOW);
                noTone(buzzer);
            }
  }else if(statusBoton2 == LOW){
    strlcpy(rfd.rfidnumber, "e37a3295", sizeof(rfd.rfidnumber));
    registrarIntento(rfd);
    
    rfd = GetAcceso(rfidString2);
    String accesoString = String(rfd.acceso);
     // Evaluar el valor de acceso
            if (accesoString.equals("Permitido")) {
                Serial.println("Acceso Permitido. Realizar acción A");
                // Realizar acción A
                digitalWrite(pinLed, HIGH);
                tone(buzzer, 500);
                delay(1000);
                digitalWrite(pinLed, LOW);
                noTone(buzzer);
                delay(200);
                motorcito.write(180);
                delay(2000);
                motorcito.write(0);
                delay(200);
                
            } else if (accesoString.equals("Denegado")) {
                Serial.println("Acceso Denegado. Realizar acción B");
                // Realizar acción B
                digitalWrite(ledRojo, HIGH);
                tone(buzzer, 1000);
                delay(1000);
                digitalWrite(ledRojo, LOW);
                noTone(buzzer);


            } else {
                Serial.println("Respuesta no reconocida");
                Serial.println(accesoString);
                // Realizar acción C (si es necesario)
                digitalWrite(ledRojo, HIGH);
                tone(buzzer, 1000);
                delay(1000);
                digitalWrite(ledRojo, LOW);
                noTone(buzzer);
            }

  }


}


/*if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Muestra el UID de la tarjeta
    Serial.print("UID de la tarjeta: ");
    String rfidString = ""; // Variable para almacenar el UID como string
    
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        if (mfrc522.uid.uidByte[i] < 0x10) {
            rfidString += "0"; // Asegura que los bytes menores a 0x10 tengan un cero delante
        }
        rfidString += String(mfrc522.uid.uidByte[i], HEX); // Convierte el byte a hexadecimal y lo añade a la cadena
    }
    
    Serial.println(rfidString);
    delay(1000);
    Serial.println("registrando..");
    
    // Envía el UID como string a la función registrarIntento
    strlcpy(rfd.rfidnumber, rfidString.c_str(), sizeof(rfd.rfidnumber));
    registrarIntento(rfd);
    GetAcceso(rfidString);
    String accesoString = rfd.acceso;
     // Evaluar el valor de acceso
            if (accesoString.equals("Permitido")) {
                Serial.println("Acceso Permitido. Realizar acción A");
                // Realizar acción A
            } else if (accesoString.equals("Denegado")) {
                Serial.println("Acceso Denegado. Realizar acción B");
                // Realizar acción B
            } else {
                Serial.println("Respuesta no reconocida");
                Serial.println(accesoString);
                // Realizar acción C (si es necesario)
            }
}*/




