
// Callback function executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {

  if (len == 0) {
    // No data received, error condition
    
    Serial.println("no data received");


  }

  memcpy(&rcvData, incomingData, sizeof(rcvData));

  // Serial.print("startSB= ");
  // Serial.println(rcvData.startSB);
  // Serial.print("stopSB= ");
  // Serial.println(rcvData.stopSB);
  // Serial.print("remainingtime= ");
  // Serial.println(rcvData.remainTime);
  Serial.print("timeStamp:");
  Serial.println(millis());





  // Pass received values to local variables
remainingTime= rcvData.remainTime ; 
mode=rcvData.mode;
startScoreBoard=rcvData.startSB; 
stopScoreBoard=rcvData.stopSB;
playA=rcvData.playAlarm;


  // Serial.println("playAlarm= ");
  // Serial.println(rcvData.playAlarm);

// dummy
if (!rcvData.remainTime){
  dmd.clearScreen( true );  
}

  if (rcvData.playAlarm==1){
    // will only play when playAlarm is True,
    //sound_alarm takes input on how many times  to sound the alarm
    sound_Alarm();
    
  }

   




}