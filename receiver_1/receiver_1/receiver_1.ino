#include <DMD32.h>  //--> DMD32 by Qudor-Engineer (KHUDHUR ALFARHAN) : https://github.com/Qudor-Engineer/DMD32

#include "fonts/SystemFont5x7.h"

#include "fonts/Arial_black_16.h"

// ESP_NOW_LIBRARIES_______
#include <esp_now.h>

#include <WiFi.h>


// dfplayermini lib___________________________________________________________________
#include "Arduino.h"

#include "DFRobotDFPlayerMini.h"

#define FPSerial Serial1
DFRobotDFPlayerMini myDFPlayer;

//----------------------------------------
// Fire up the DMD library as dmd.
#define DISPLAYS_ACROSS 1
#define DISPLAYS_DOWN 1
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

// Timer setup.
// create a hardware timer  of ESP32.
hw_timer_t * timer = NULL;

#define scoreboardID 1 //2

//_______________variables for scoreboard timer_________________________________________________________________

int mode; //1="Single",2="Dual" 
bool playA;

// for dual mode 
int startScoreBoard;
int stopScoreBoard;
int remainingTime = 0;
//________________________________________________________________________________
const unsigned int interval = 1000;
unsigned long previousMillis = 0;

//_______________________________________________________________________________
// uint8_t broadcastAddress[] = {
//   0xD4,
//   0x8A,
//   0xFC,
//   0xA6,
//   0xB5,
//   0x5C
// }; //mac add-transmitter
// // D4:8A:FC:A6:B5:5C =transmitter address
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; 

typedef struct struct_message_rcv {

  int remainTime;
  int mode; //1-"Single,2-"Dual
  //  for dual mode 
  int startSB;
  int stopSB;
  bool playAlarm;

}
struct_message_rcv;

// Create a structured object for received data
struct_message_rcv rcvData;

esp_now_peer_info_t peerInfo;

// Define ESP-NOW timeout value
#define SIGNAL_TIMEOUT 500
// Last Receive time
unsigned long lastRecvTime = 0;

//_________________________________________________IRAM_ATTR triggerScan()
//  Interrupt handler for Timer1 (TimerOne) driven DMD refresh scanning,
//  this gets called at the period set in Timer1.initialize();

void IRAM_ATTR triggerScan() {
  dmd.scanDisplayBySPI();
}
// //________________________________________________________________________________

//_______________Button handler functions(future webpage buttons) _________________________________________________________________
//________________________________________________________________________________VOID SETUP()
void setup() {

  // put your setup code here, to run once:

  //__________________________SOUND_SETUP____________
  FPSerial.begin(9600, SERIAL_8N1, /*rx =*/ 27, /*tx =*/ 26);

  Serial.begin(115200);
  sound_setup();

  //________________________________ESP_NOW_________________________________________________
  // Set ESP32 as a Wi-Fi Station

  WiFi.mode(WIFI_STA);

  // Disable WiFi sleep mode
  WiFi.setSleep(false);

  // Initilize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  // Initilize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    return;
  }

  // Register receive callback function
  esp_now_register_recv_cb(OnDataRecv);

  // Register the send callback
  // esp_now_register_send_cb(OnDataSent);

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer( & peerInfo) != ESP_OK) {
    return;
  }

  // Enable watchdog timer - true is "panic mode" to restart
  // esp_task_wdt_init(WDT_TIMEOUT, true);

  pinMode(2, OUTPUT);
  // P10__Display setup______________________


  delay(500);

  Serial.println();
  Serial.println("return the clock speed of the CPU.");
  // return the clock speed of the CPU.
  uint8_t cpuClock = ESP.getCpuFreqMHz();
  delay(500);

  Serial.println();
  Serial.println("Timer Begin");
  // Use 1st timer of 4.
  // devide cpu clock speed on its speed value by MHz to get 1us for each signal  of the timer.
  timer = timerBegin(2, cpuClock, true);
  delay(500);

  Serial.println();
  Serial.println("Attach triggerScan function to our timer.");
  // Attach triggerScan function to our timer.
  timerAttachInterrupt(timer, & triggerScan, true);
  delay(500);

  Serial.println();
  Serial.println("Set alarm to call triggerScan function.");
  // Set alarm to call triggerScan function.
  // Repeat the alarm (third parameter).
  timerAlarmWrite(timer, 1000, true);
  delay(500);

  Serial.println();
  Serial.println("Start an alarm.");
  // Start an alarm.
  timerAlarmEnable(timer);
  delay(500);
  // ________________________________main timer setup________________________________________________

  dmd.clearScreen(true); //true is normal (all pixels off), false is negative (all pixels on)

}
//________________________________________________________________________________

//_____________________VOID LOOP()___________________
int prevTime = 0;
void loop() {

  // put your main code here, to run repeatedly:

  // ________________________________
  // If you want to change the font, don't forget to use this line of code : dmd.selectFont . Example: dmd.selectFont(Arial_Black_16);

  //----------------------------------------Demo with "Arial_Black_16" font.
  dmd.clearScreen(true);
  dmd.selectFont(Arial_Black_16);
  // dmd.drawString(x, y, Text, Number of characters in text, GRAPHICS_NORMAL);

  switch (mode) {
    //single mode

  case 1: {

    int incoming = remainingTime;
    if (playA != 1) {

      updateDisplay(remainingTime);
    }

    if (prevTime != incoming && remainingTime > 0) {

      // updateDisplay(incoming);     

      prevTime = incoming;

    }
    if (remainingTime <= 0) {
      dmd.clearScreen(true);
    }

    break; //switch break of single mode
  }

  case 2: { //dual mode 
    int incoming;

    if (startScoreBoard == scoreboardID) {
      incoming = remainingTime;
      if (playA != 1) {

        updateDisplay(remainingTime);
      }

      if (prevTime != incoming && remainingTime > 0) {
        // prepareCount(incoming);
        prevTime = incoming;
      }
      if (remainingTime <= 0) {
        dmd.clearScreen(true);
      }
    } else if (stopScoreBoard == scoreboardID)

    {
      // do nothing
      dmd.clearScreen(true);
      remainingTime = 0;
    }

    break; //switch dual of single mode
  }

  } //switch break

  // // Reset watchdog timer
  // esp_task_wdt_reset();
  delay(1000);

} //loop end 

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<