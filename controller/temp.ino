// // ESP-NOW Peer info
// esp_now_peer_info_t peerInfo;
// esp_now_peer_info_t peerInfo2;



// // Register the send callback
//   esp_now_register_send_cb(OnDataSent);

//   // Register peer 1 and 2
//   memcpy(peerInfo.peer_addr, broadcastAddress, 6);
//   memcpy(peerInfo2.peer_addr, broadcastAddress2, 6);
//   // peerInfo.channel = 0;
//   peerInfo.encrypt = false;
//   // peerInfo2.channel = 1;
//   peerInfo2.encrypt = false;
//   // Register peer 2
//   // peerInfo.channel = 1;
//   // peerInfo.encrypt = false;

//   // Add peer

//   // esp_now_add_peer(slave1Address, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

//   if (esp_now_add_peer( & peerInfo) != ESP_OK) {
//     connectStatus = "No peer added";
//     connectError = HIGH;
//     return;
//   } else {
//     connectStatus = "ESP-NOW Ready";
//     connectError = LOW;
//   }
//   if (esp_now_add_peer( & peerInfo2) != ESP_OK) {
//     connectStatus = "No peer added";
//     connectError = HIGH;
//     return;
//   } else {
//     connectStatus = "ESP-NOW Ready";
//     connectError = LOW;
//   }
//   // timer.attach(2,send_sensor);
