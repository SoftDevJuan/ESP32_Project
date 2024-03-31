
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


const int buzzer = 25;
const int ledRojo = 19;
const int servoPin = 27;



int pinLed = LED_BUILTIN;
struct PUERTA *puertas, gate;
Servo motorcito;
StaticJsonDocument<JSON_DOC_SIZE> doc;




///////////////////////////////////////////////////////////////////////
void setup() 
{

   Serial.begin(115200);
    motorcito.attach(servoPin);
    pinMode(pinLed, OUTPUT);
    pinMode(ledRojo, OUTPUT);
    delay(300);
    Serial.println("Conectandose al servidor..");
    ConnectWiFi_STA();
    Serial.print("inicializando porgrama");
  for(int i = 0; i <= 10;i++){
    Serial.print(".");
    delay(500);
    
  }
  Serial.println("");
  Serial.println("Bienvenido");

}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void loop() 
{

  gate = GetEstado("1"); //este metodo consulta a la api si hay un acceso a la puerta
  String puertaString = String(gate.status); // es para saber si la puerta esta abierta o cerrada
  String activacionString = String(gate.activacion); //es para saber si fue por gafete o por la app

  if(puertaString.equals("true") && activacionString.equals("local")){ //aqui valida si fue por gafete, sino fue por gafete no hace nada eso es en otro metodo
      digitalWrite(pinLed, HIGH); // prende el led
      motorcito.write(180); //abre la puerta
      delay(3000); // espera 3 segundos
      motorcito.write(0); // cierra la puerta
      delay(200);
                 //ACTUALIZAR UN COMPONENTE (ALGUNOS CAMPOS)
                 //aqui manada a decirle a la db que la puerta ya se cerro y el acceso ahora se deniega hasta que haya otro gafete
      strlcpy(gate.status, "false",15);
      strlcpy(gate.acceso, "false",15);
      strlcpy(gate.activacion, "local",15);
      cerrarPuerta("1", gate);
      delay(300);


    }else if(puertaString.equals("false") && activacionString.equals("local")){
      digitalWrite(ledRojo, HIGH);
      delay(4000);
      digitalWrite(ledRojo, LOW);
    }



//////////////////////////////////////////////////////////////////////////////////////////////////////
  gate = getAlarma("1"); // este metodo consulta a la api si la alarma esta activa
  String alarmaString = String(gate.alarma); // recibe si esta activa o no
 //valida que se haya activado desde la app

  if(alarmaString.equals("true") && activacionString.equals("remota")){ //valida si esta activa y fue desde la app se enciende en el ESP
      digitalWrite(ledRojo, HIGH); // enciende el led de la alarma
      //aqui va el codigo del buzzer nomas que ya no tengo buzzer tengo que ir a comprar
      delay(200);
      
    }else if (alarmaString.equals("true") && activacionString.equals("remota")){
      digitalWrite(ledRojo, LOW); // en caso de que ya el estado de la alarma se apagado se paga el led y el buzzer
       //aqui va el codigo del buzzer nomas que ya no tengo buzzer tengo que ir a comprar
    }




/////////////////////////////////////////////////////////////////////////////////////////////////////////
    gate = getEstadoPuerta("1"); // obtiene si la puerta esta abierta o cerrada desde la app

  if(puertaString.equals("true") && activacionString.equals("remota")){ // si la puerta fue abierta de manera remota se abre y no se cierra
      motorcito.write(180);// se queda abierta hasta que se mande a cerrar desde la app tambien
      delay(200);
      
    }else if(puertaString.equals("false") && activacionString.equals("remota")){
       motorcito.write(0);//aqui en caso de que se mande a cerrar desde la app
      delay(200);
    }

    
  







}//loop


