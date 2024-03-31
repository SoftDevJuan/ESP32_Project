
String ApiHost = "http://192.168.1.7:4000";
const int JSON_DOC_SIZE = 4096;




// Struct to represent our component  record


struct RFID {
  char rfidnumber[30];
  char acceso[15];
  
};




struct RFID rfid[20];



void processResponse(int httpCode, HTTPClient& http)
{
   if (httpCode > 0) {
     // Serial.printf("Codigo de respuesta: %d\t", httpCode);
      if (httpCode == HTTP_CODE_OK) {
         String payload = http.getString();
         Serial.println(payload);
         if(strstr(payload.c_str(),"null"))
            Serial.println("No existe el ID en la DB");
         else
          //Serial.println("Transaccion realizada con exito");
          Serial.print(".");   

      }
   }
   else {
      Serial.printf("Request failed, error: %s\n", http.errorToString(httpCode).c_str());
   }
   http.end();
}




//////////////////////crea un nuevo componente///////////////////////////////////////////////////

void registrarIntento(struct RFID nuevoRfid) {
    HTTPClient http;
    http.begin(ApiHost + "/api/registrarintento/");
    http.addHeader("Content-Type", "application/json");
    
    String jsonComponente = "";
    StaticJsonDocument<300> jsonDoc;
    jsonDoc["rfidnumber"] = nuevoRfid.rfidnumber;
    serializeJson(jsonDoc, jsonComponente);
    
    int httpCode = http.POST(jsonComponente);
    
    if (httpCode > 0) {
        if (httpCode == HTTP_CODE_OK) {
            // Procesar respuesta JSON
            String payload = http.getString();
            Serial.println("Respuesta del servidor:");
            Serial.println(payload);
            
            // Parsear JSON
            DynamicJsonDocument jsonBuffer(900);
            deserializeJson(jsonBuffer, payload);
           
            // Obtener el valor del campo "acceso"
            const char* acceso = jsonBuffer["acceso"];
            String accesoString(acceso);
         
            
            Serial.println("se guardo registro en la base de datos");
        } else {
            Serial.print("Error al registrar. Código de estado HTTP: ");
            Serial.println(httpCode);
        }
        
    } else {
        Serial.println("Error al realizar la solicitud HTTP vlv");
    }
     
    http.end();
}
  
///////////////////////////// consultar acceso////////////////////////////////////////////////////////////////////////
///////////////////////// obtiene un componente///////////////////////////////////////

struct RFID  GetAcceso(String rfidnumber)
{
  RFID rfd;
  Serial.print(rfidnumber);
   HTTPClient http;
   http.useHTTP10(true);
   http.addHeader("Content-Type", "application/json");
   http.begin(ApiHost + "/api/consultaRFID/" + rfidnumber);
   int httpCode = http.GET();
   String payload = http.getString();
   
  processResponse(httpCode, http);
  StaticJsonDocument<JSON_DOC_SIZE> doc;
   http.end();
  // Parse response
  //DeserializationError error = deserializeJson(doc, http.getStream());
  
 DeserializationError error = deserializeJson(doc, payload); 
  if (error) {
    Serial.print("deserializeJson() failed en el get de un componente: ");
    Serial.println(error.c_str());
    Serial.println(payload);
    return rfd;
  }
  
  if (doc.isNull()){
    Serial.println("doc is Null");
    return rfd;  // or {}
  }



  
  //Serial.println(payload);     
int i=0;
      for (JsonObject obj : doc.as<JsonArray>()) {
        // Copiar los valores del JSON a la estructura COMPONENTE
        strlcpy(rfd.acceso, obj["acceso"], sizeof(rfd.acceso));
       
  // Serial.println(component_id);
    
    Serial.print("el objeto:");
    Serial.println(rfd.acceso);
    


    return rfd;
  }
   
  return rfd;
}




