void sound_setup(){

     Serial.println();
      Serial.println(F("DFRobot DFPlayer Mini Demo"));
      Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));


      if (!myDFPlayer.begin(FPSerial, /*isACK = */true, /*doReset = */true)) {  //Use serial to communicate with mp3.
      Serial.println(F("Unable to begin:"));
      Serial.println(F("1.Please recheck the connection!"));
      Serial.println(F("2.Please insert the SD card!"));
      while(true){
        delay(0); // Code to compatible with ESP8266 watch dog.
      }
    }
    Serial.println(F("DFPlayer Mini online."));

    //volume
      myDFPlayer.volume(20);  //Set volume value. From 0 to 30





}