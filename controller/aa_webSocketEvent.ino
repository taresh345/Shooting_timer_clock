
// when data is received  -do things defined in this function 
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

    switch (type) 
    {
      case WStype_DISCONNECTED:
        Serial.printf("[%u] Disconnected!\n", num);
        break;
      case WStype_CONNECTED: {
          IPAddress ip = websockets.remoteIP(num);
          Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        }
        break;
      case WStype_TEXT:
        Serial.printf("[%u] get Text: %s\n", num, payload);
        String message = String((char*)( payload));
        Serial.println(message);

      // deserialization of data received from websockets server usiing arduinojson library
      DynamicJsonDocument doc(200);
      // deserialize the data
      DeserializationError error = deserializeJson(doc, message);
      // parse the parameters we expect to receive (TO-DO: error handling)
      // Test if parsing succeeds.
  
    // on any type of error
    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }

      // if payload succesfully arrives . write here  to update variables.___________________
      if(doc["type"]){
        // Serial.println("pong recieved");
      }

    // if "Set/reset is pressed"
      if (doc["shootingTime"]){
        shootingTime = doc["shootingTime"];
        prepareTime = doc["prepareTime"];
        warningTime = doc["warningTime"];
        st = doc["shootingTime"];
        pt = doc["prepareTime"];
        wt = doc["warningTime"];
        reset= doc["Reset"];

      }

      // to get mode of the •scoreboard
      if (doc["mode"]){
        mode=doc["mode"];
      }


      // holds value of wether to start timer or not 
      if (doc["Start"] || doc["shootingTime"]){
      start =doc["Start"];
      // for dual mode 
      startScoreBoard=doc["startSB"];
      stopScoreBoard=doc["stopSB"];


      }
      // holds value of wether to start timer or not 
      if (doc["Stop"]){
      start =doc["Stop"];


      }

      // this function is executed whenever i receive data so reset times
      // these conditions are always true on dual mode 
      if(mode==2 && doc["startSB"] && doc["stopSB"])
      {
        shootingAlarm=1;
        EndAlarm=2;

      }




    }
  }