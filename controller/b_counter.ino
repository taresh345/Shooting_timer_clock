// takes a number and counts it down 
void counter(int &Time)
{

    if (Time>=0) {       
  long unsigned current=millis();
        if(current-previousMillis>interval-50){
          previousMillis=current;
            // Serial.println(Time);
            Serial.print("timeStamp:");
            Serial.println(millis());


          xmitData.remainTime =Time;
          xmitData.mode = mode;
          xmitData.startSB=startScoreBoard; 
          xmitData.stopSB=stopScoreBoard;
          xmitData.playAlarm=playA;




    // Send message via ESP-NOW__"b_ondatasend" executes after this 
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&xmitData, sizeof(xmitData));
      // delay(50);// wont send data to second client without delay
    // esp_err_t result2 = esp_now_send(broadcastAddress2, (uint8_t *)&xmitData, sizeof(xmitData));
          Time--;



          }
          }

}