#ifdef ESP8266#include <ESP8266WiFi.h>

#include <ESP8266mDNS.h>

#elif defined(ESP32)
#include <WiFi.h>

#include <ESPmDNS.h>

#else
#error "Board not found"
#endif
// _________________________________________________________________________
#include <WebSocketsServer.h>

#include <ArduinoJson.h>
 // creating  a async webserver-esp  
#include <ESPAsyncWebServer.h>

// Include Libraries for ESP-NOW Communications to send data to receivers
#include <esp_now.h>


// _____pin_definitions___________________________________________________________________

#define led 2

// _____web_controller_page__________________________________________________________________
#include "new_webpage.h"
 // #include "webpage.h"
// #include "callback.ino"

//_______const values of global var________
int st;
int pt;
int wt;

// ____________variables global____

int shootingTime;
int prepareTime;
int warningTime;
int reset;
int start; //to start timer or not (0,1)
int mode; //1="Single" 2="Dual"

int warningTimeExecutions;

//  alarm values
int prepareAlarm = 2;
int shootingAlarm = 1;
int EndAlarm = 2;

bool playA = 0;

//  for dual mode 
int startScoreBoard;
int stopScoreBoard;

const unsigned int interval = 1000;
unsigned long previousMillis = 0;
// enum class TimerState { Idle, Prepare, Shoot, Warn };
// TimerState state = TimerState::Idle;


// MAC Address of transmitter - edit as required
//MAC of receivers 
// uint8_t broadcastAddress[] = {
//   0xA8,
//   0x42,
//   0xE3,
//   0xAB,
//   0xB7,
//   0x38
// }; // A8:42:E3:AB:B7:38-receiver1
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; 
uint8_t broadcastAddress2[] = {
  0xE4,
  0x65,
  0xB8,
  0x20,
  0x8E,
  0x38
}; // { 0xE4, 0x65, 0xB8, 0x20, 0x8E, 0x38 }-receiver

// Create a structured object for sent data
typedef struct struct_message_xmit {

  int remainTime;
  int mode; //1="Single" 2="Dual"
  //  for dual mode 
  int startSB;
  int stopSB;
  bool playAlarm;

}
struct_message_xmit;

// Create a structured object for sent data
struct_message_xmit xmitData;

// ESP-NOW Peer info
esp_now_peer_info_t peerInfo;
// esp_now_peer_info_t peerInfo2;

// Variable for connection error  - HIGH is error state
volatile bool connectError = LOW;

// Variable for connection status string
String connectStatus = "NO INFO";

// ___objects_for_webserver_controller_____________
AsyncWebServer server(80); // server port 80
WebSocketsServer websockets(81);

// on pages that are not defined
void notFound(AsyncWebServerRequest * request) {
  request -> send(404, "text/plain", "Page Not found");
}

void setup(void) {

  //_for_websockets server________________________
  Serial.begin(115200);

  WiFi.softAP("Tareshnetwork", ""); //ssid name, password.
  Serial.println("softap");
  Serial.println("");
  Serial.println(WiFi.softAPIP());

  // setting name of webserver
  if (MDNS.begin("taresh")) { //esp.local/
    Serial.println("MDNS responder started");
  }
  pinMode(led, OUTPUT);

  server.on("/", [](AsyncWebServerRequest * request) {

    request -> send_P(200, "text/html", webpage);
  });

  server.onNotFound(notFound);

  server.begin(); //  start webserver
  websockets.begin();
  // when data is received from the web server 
  websockets.onEvent(webSocketEvent);


  // ____________________webserver_end________________

  // Set ESP32 as a Wi-Fi Station____for ESP-NOW______
  // WiFi.mode(WIFI_STA);
  WiFi.mode(WIFI_AP_STA);

  // Disable WiFi Sleep mode
  WiFi.setSleep(false);

  // Initilize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    connectStatus = "ESP-NOW Error";
    connectError = HIGH;
    return;
  } else {
    connectStatus = "ESP-NOW OK";
    connectError = LOW;
  }

  // Register the send callback
  esp_now_register_send_cb(OnDataSent);

  // peerInfo.channel = 0;
  // peerInfo.encrypt = false;
  // Register peer 1 and 2
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  //adding peer
  if (!esp_now_is_peer_exist(broadcastAddress))
  {
    esp_now_add_peer(&peerInfo);
  }
  // memcpy(peerInfo.peer_addr, broadcastAddress2, 6);
  // //adding peer
  //   if (esp_now_add_peer(&peerInfo) != ESP_OK){
  //   Serial.println("Failed to add peer");
  //   return;
  // }

  // // Add peer
  // if (esp_now_add_peer( & peerInfo) != ESP_OK) {
  //   connectStatus = "No peer added";
  //   connectError = HIGH;
  //   return;
  // } else {
  //   connectStatus = "ESP-NOW Ready";
  //   connectError = LOW;
  // }
  // if (esp_now_add_peer( & peerInfo2) != ESP_OK) {
  //   connectStatus = "No peer added";
  //   connectError = HIGH;
  //   return;
  // } else {
  //   connectStatus = "ESP-NOW Ready";
  //   connectError = LOW;
  // }
  // timer.attach(2,send_sensor);

}

void loop(void) {
  websockets.loop();

  switch (mode) {
    //single mode
  case 1:

    if (start == 1) {
      digitalWrite(led, true);
      //play alarm two times before preparetime

      if (prepareAlarm > 0) {
        playA = 1;
        counter(prepareAlarm);
      } else if (prepareTime >= 0) {
        playA = 0;

        counter(prepareTime);
        // currtime = prepareTime;

      } else if (shootingAlarm > 0) {
        playA = 1;
        counter(shootingAlarm);

      } else if (shootingTime > warningTime) {
        playA = 0;
        counter(shootingTime);
      }

       else if (warningTime >= 0) {
      
        // Check if warningTime has decreased since the previous iteration
        if (warningTime < wt) {
            // Increment the counter only when warningTime decreases
            warningTimeExecutions++;
        }

        // Check if it's the first or second execution
        if (warningTimeExecutions <= 1) {
            playA = 1; // Set playA to 1 for the first two decrements of warningTime
            Serial.println("playA set to 1");
        } else {
            playA = 0; // Reset playA to 0 after the first two decrements
        }         
         if (shootingTime >= warningTime) {
            shootingTime = -1;
          }

        
        counter(warningTime);

      } 
      
      else if (EndAlarm >= 0) {
        playA = 1;
        counter(EndAlarm);
      } else if (EndAlarm == 0) {
        playA = 0;
        counter(EndAlarm);
      }

    } else if (start == 0) {
      digitalWrite(led, false);
      if (reset == 1) {
        prepareAlarm = 2;
        shootingAlarm = 1;
        EndAlarm = 2;
        warningTimeExecutions=0;

      }

    }

    break;

  case 2:

    // Serial.print("Start :");
    // Serial.println(start);

    if (start == 1) {
      digitalWrite(led, true);

      if (shootingAlarm > 0) {
        playA = 1;
        counter(shootingAlarm);

      } else if (prepareTime >= 0) {
        playA = 0;
        counter(prepareTime);

      } else if (shootingTime > warningTime) {
        playA = 0;
        counter(shootingTime);

      } else if (warningTime >= 0) {
         // Check if warningTime has decreased since the previous iteration
        if (warningTime < wt) {
            // Increment the counter only when warningTime decreases
            warningTimeExecutions++;
        }

        // Check if it's the first or second execution
        if (warningTimeExecutions <= 1) {
            playA = 1; // Set playA to 1 for the first two decrements of warningTime
            // Serial.println("playA set to 1");
        } else {
            playA = 0; // Reset playA to 0 after the first two decrements
        }   

        //for the first time when flow enters this loop .
        if (shootingTime >= warningTime) {
          shootingTime = -1;
        }
      
          
        counter(warningTime);

      } else if (EndAlarm >= 0) {
        playA = 1;
        counter(EndAlarm);
      } else if (EndAlarm == 0) {
        playA = 0;
        counter(EndAlarm);
      }

//for automatic switching ---
      // else if (warningTime<=0){
      //   //changing scoreboard
      //   int send=0;
      //   int temp=startScoreBoard;
      //   startScoreBoard=stopScoreBoard;
      //   stopScoreBoard=temp;
      //   // reset alarm values  
      //   shootingAlarm=1;
      //   EndAlarm=2;

      //   //reseting the values
      //   prepareTime=pt;
      //   shootingTime=st;
      //   warningTime=wt;
      //   counter(send);

      // }

    } else if (start == 0) {
      digitalWrite(led, false);
      shootingAlarm = 1;
      EndAlarm = 2;
      warningTimeExecutions=0;

    }

    break;

  }

  // sending data tor receiver_____________
  // // Format structured data
  delay(50); //important for webockets to work 

}

// this is to broadcast the signal to webpage/webserver 
// void send_sensor()
// {
//     // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
//   float h = dht.readHumidity();
//   // Read temperature as Celsius (the default)
//   float t = dht.readTemperature();

//     if (isnan(h) || isnan(t) ) {
//     Serial.println(F("Failed to read from DHT sensor!"));
//     return;
//   }
//   // JSON_Data = {"temp":t,"hum":h}
//   String JSON_Data = "{\"temp\":";
//          JSON_Data += t;
//          JSON_Data += ",\"hum\":";
//          JSON_Data += h;
//          JSON_Data += "}";
//    Serial.println(JSON_Data);

  // websockets.broadcastTXT(JSON_Data);
// }