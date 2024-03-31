
String ApiHost = "http://192.168.1.73:3000";
const int JSON_DOC_SIZE = 4096;




// Struct to represent our component  record


struct PUERTA {
  char numero[30];
  char status[30];
  char acceso[15];
  char alarma[15];
  char activacion[15];
};




struct PUERTA puerta[20];



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





  
///////////////////////////// consultar acceso////////////////////////////////////////////////////////////////////////

struct PUERTA  GetEstado(String numeroPuerta)
{
  PUERTA gate;
   HTTPClient http;
   http.useHTTP10(true);
   http.addHeader("Content-Type", "application/json");
   http.begin(ApiHost + "/api/consultaPuerta/"+ numeroPuerta);
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
    return gate;
  }
  
  if (doc.isNull()){
    Serial.println("doc is Null");
    return gate;  // or {}
  }



  
  //Serial.println(payload);     
int i=0;
      for (JsonObject obj : doc.as<JsonArray>()) {
        // Copiar los valores del JSON a la estructura COMPONENTE
        strlcpy(gate.acceso, obj["acceso"], sizeof(gate.acceso));
        strlcpy(gate.status, obj["status"], sizeof(gate.status));
        strlcpy(gate.activacion, obj["activacion"], sizeof(gate.activacion));
       
  // Serial.println(component_id);
    
    Serial.print("estado del acceso:");
    Serial.println(gate.acceso);
    Serial.println(gate.status);
    


    return gate;
  }
   
  return gate;
}


////////////////////consultar alarma ////////////////////////////////////////////////////////////////////////////////////////////////
struct PUERTA  getAlarma(String numeroPuerta)
{
  PUERTA gate;
   HTTPClient http;
   http.useHTTP10(true);
   http.addHeader("Content-Type", "application/json");
   http.begin(ApiHost + "/api/alarma/"+ numeroPuerta);
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
    return gate;
  }
  
  if (doc.isNull()){
    Serial.println("doc is Null");
    return gate;  // or {}
  }



  
  //Serial.println(payload);     
int i=0;
      for (JsonObject obj : doc.as<JsonArray>()) {
        // Copiar los valores del JSON a la estructura COMPONENTE
        
        strlcpy(gate.activacion, obj["activacion"], sizeof(gate.activacion));
        strlcpy(gate.alarma, obj["alarma"], sizeof(gate.alarma));

       
  // Serial.println(component_id);
    
    Serial.print("estado de la alarma:");
    Serial.println(gate.alarma);
    Serial.println(gate.activacion);
    


    return gate;
  }
   
  return gate;
}



////////////////////////////////////////////////controlar puerta //////////////////////////////////////////////////////////////////
struct PUERTA  getEstadoPuerta(String numeroPuerta)
{
  PUERTA gate;
   HTTPClient http;
   http.useHTTP10(true);
   http.addHeader("Content-Type", "application/json");
   http.begin(ApiHost + "/api/controlarPuerta/"+ numeroPuerta);
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
    return gate;
  }
  
  if (doc.isNull()){
    Serial.println("doc is Null");
    return gate;  // or {}
  }



  
  //Serial.println(payload);     
int i=0;
      for (JsonObject obj : doc.as<JsonArray>()) {
        // Copiar los valores del JSON a la estructura COMPONENTE
        strlcpy(gate.acceso, obj["acceso"], sizeof(gate.acceso));
        strlcpy(gate.activacion, obj["activacion"], sizeof(gate.activacion));
       
  // Serial.println(component_id);
    
    Serial.print("estado de la puerta:");
    Serial.println(gate.acceso);
    Serial.println(gate.activacion);
    


    return gate;
  }
   
  return gate;
}




//////////////////////////////////////////////////patch para cerrar puerta ////////////////////////////////////////////////////////

void cerrarPuerta(String numeroPuerta, struct PUERTA puerta) {
   
    HTTPClient http;
    http.useHTTP10(true);
    http.begin(ApiHost + "/api/cerrarPuerta/" + numeroPuerta);
    http.addHeader("Content-Type", "application/json");

    // Crear el JSON del componente
    String jsonPuerta = "";
    StaticJsonDocument<300> jsonDoc;
    jsonDoc["status"] = puerta.status;
    jsonDoc["acceso"] = puerta.acceso;
    
    serializeJson(jsonDoc, jsonPuerta);
    Serial.println(jsonPuerta);

    // Realizar la solicitud PUT
    int httpCode = http.PATCH(jsonPuerta);

    // Verificar el código de estado de la respuesta HTTP
    if (httpCode > 0) {
        // Si el código de estado es 200 (OK), mostramos un mensaje indicando que la operación fue exitosa
        if (httpCode == HTTP_CODE_OK) {
            Serial.println("se cerro la puerta de manera local");
        } else {
            // Si el código de estado no es 200, mostramos un mensaje indicando que ha ocurrido un error
            Serial.print("error al cerrar la puerta. Código de estado HTTP: ");
            Serial.println(httpCode);
        }
    } else {
        // Si no se pudo establecer una conexión con el servidor, mostramos un mensaje de error
        Serial.println("Error al conectar con el servidor");
    }

    // Procesar la respuesta HTTP
    //processResponse(httpCode, http);

    // Finalizar la conexión HTTP
    http.end();
}




/////////////////////// borra un registro /////////////////////////////////////////////////////////
void borrar(String numeroPuerta) {
    HTTPClient http;
    http.begin(ApiHost + "/api/borrar/" + numeroPuerta);

    // Enviamos una solicitud DELETE
    int httpCode = http.sendRequest("DELETE");

    // Verificamos el código de estado de la respuesta HTTP
    if (httpCode > 0) {
        // Si el código de estado es 200 (OK), mostramos un mensaje indicando que el componente ha sido eliminado
        if (httpCode == HTTP_CODE_OK) {
            Serial.println("El componente ha sido eliminado exitosamente");
        } else {
            // Si el código de estado no es 200, mostramos un mensaje indicando que ha ocurrido un error
            Serial.print("Error al eliminar el componente. Código de estado HTTP: ");
            Serial.println(httpCode);
        }
    } else {
        // Si no se pudo establecer una conexión con el servidor, mostramos un mensaje de error
        Serial.println("Error al conectar con el servidor");
    }

    // Procesamos la respuesta HTTP
    processResponse(httpCode, http);

    // Finalizamos la conexión HTTP
    http.end();
}




