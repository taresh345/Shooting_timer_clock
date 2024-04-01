


// char webpage[] PROGMEM = R"=====(
// <!DOCTYPE html>

// <head>
//   <meta charset="utf-8">
//   <meta http-equiv="X-UA-Compatible" content="IE=edge">
//   <title></title>
//   <meta name="description" content="">
//   <meta name="viewport" content="width=device-width, initial-scale=1">
//   <link rel="stylesheet" href="">
// </head>


// <style>
//   input[type="number"] {
//     width: 200px;
//     /* Set the desired width */
//     height: 30px;
//     /* Set the desired height */
//     padding: 5px;
//     /* Add padding for better appearance */
//     box-sizing: border-box;
//     /* Include padding and border in the total width and height */
//     /* Add more styles as needed */
//     font-size: 30px;
//   }

//   /* Style the up and down buttons */
//   input[type="number"]::-webkit-inner-spin-button,
//   input[type="number"]::-webkit-outer-spin-button {
//     /* -webkit-appearance: none; */
//     margin: 0;
//   }

//   input[type="number"] {
//     -moz-appearance: textfield;
//   }

//   body {
//     font-family: Arial, sans-serif;
//     text-align: center;
//     background-color: rgb(127, 200, 190);

//   }

//   h1 {
//     color: #333;
//   }

//   button {
//     padding: 10px 20px;
//     margin: 5px;
//     cursor: pointer;
//     font-size: 25px;
//   }

//   #switch {
//     display: none;

//   }
// </style>



// <body>


//   <h1>INPUT SHOOTING TIME</h1>
//   <input type="number" value="180" id="shootingTime" max="999">

//   <h1>INPUT PREPARE TIME</h1>
//   <input type="number" value="10" id="prepareTime" max="30">

//   <h1>INPUT WARNING TIME</h1>
//   <input type="number" value="30" id="warningTime" min=0 >

//   <br />
//   <br />
//   <div id="warning">
//   </div>
//   <button onclick="setTimer()" id="setTimer">set</button>
//   <br />
//   <br />
//   <button onclick="StartTimer()" id="start_btn" disabled>start</button>
//   <button onclick="stopTimer()" id="stop_btn" disabled>stop</button>
//   <button onclick="resetTimer()">reset</button>
//   <br />
//   <br />
//   <button onclick="setSwitch()" id="switch">SWITCH</button>

//   <h2>Select mode</h2>
//   <button onclick="setMode(1)" id="single">Single</button>
//   <button onclick="setMode(2)" id="dual">Dual</button>

//   <!-- <button onclick="button_1_off()">off</button>
//      -->


//   <script src="" async defer></script>
// </body>


// <script>

//   // data sent once or on interrupt buttons
//   var timerMode = 1; //1-single ,2-dual
//   var set;
//   var shootingTime;
//   var prepareTime;
//   var warningTime;
//   // data sent repeatedly
//   // stop, reset is intterupt buttons
//   var start = 0;
//   var reset = 0;
//   var SWITCH;

//   var startSB = 1;
//   var stopSB = 2;

//   // _____________________________________


//   var connection = new WebSocket('ws://' + location.hostname + ':81/');

//   connection.onopen = function() {
//             console.log('WebSocket connection established');
//             startKeepAlive();
//         };

//   connection.onclose = function(event) {
//             console.log('WebSocket connection closed:', event);
//         };

//   connection.onerror = function(error) {
//             console.error('WebSocket error:', error);
//         };

// //________detect ping____________
// // Event listener to handle incoming messages


// function sendPing() {
//   // You can customize the ping message as needed
//   let type="pong";
  
//   let pongMessage = '{"type":"' + type + '"}';
//     connection.send(pongMessage);
//     console.log('Sent ping message to server');
// }

// // Function to start the keep-alive timer
// function startKeepAlive() {
//     // Set the interval for sending ping messages (e.g., every 30 seconds)
//     var keepAliveInterval = 30000; // 30 seconds
    
//     // Start the timer to send ping messages at regular intervals
//     setInterval(sendPing, keepAliveInterval);
//     console.log('Keep-alive timer started');
// }


//         //________________________________________



//   function setMode(Mode) {
//     timerMode = Mode;
//     console.log(`the mode is set as ${timerMode}`);
//     var timer_mode = '{"mode":"' + timerMode + '"}';
//     connection.send(timer_mode);
//     document.getElementById('single').disabled = true;
//     document.getElementById('dual').disabled = true;

//     if (timerMode == 2) {

//       document.getElementById('switch').style.display = 'inline-block';
//     }
//     else if (timerMode == 1) {
//       document.getElementById('switch').style.display = "none";

//     }

//   }



//   // ____________only sent once or on  button-press___________


//   function disable_input() {
//     document.getElementById('shootingTime').disabled = true;
//     document.getElementById('prepareTime').disabled = true;
//     document.getElementById('warningTime').disabled = true;

//   }
//   function enable_input() {
//     document.getElementById('shootingTime').disabled = false;
//     document.getElementById('prepareTime').disabled = false;
//     document.getElementById('warningTime').disabled = false;

//   }

//   function setTimer() {
//     // if this function is sent then reset the timer in each clock 
//     // by the new data sent
//     if (reset == 0) {
//       // reset 0 means reset button is not pressed
//       // reset is false ,show the set timers button
//       // first reset is 0 , then when set is pressed ,it becomes 
//       // disabled
//       document.getElementById("setTimer").disabled = true;
//       document.getElementById("start_btn").disabled = false;
//       document.getElementById("stop_btn").disabled = false;
//       disable_input();

//     }
//     else if (reset == 1) {

//       // reset 1 means reset button is pressed

//       document.getElementById("setTimer").disabled = false;

//       enable_input();


//     }

//     var Timer_data;
//     shootingTime = document.getElementById('shootingTime').value;
//     prepareTime = document.getElementById('prepareTime').value;
//     warningTime = document.getElementById('warningTime').value;

//       //constraints____
//   let shootingTimeInput = document.getElementById('shootingTime').value
//   let warningTimeInput = document.getElementById('warningTime')

//   warningTimeInput.addEventListener('input', function () { warningTimeInput.setAttribute('max', shootingTimeInput - 1) })


//     if (Number(shootingTime) > Number(warningTime)) {
//       Timer_data = '{"shootingTime":"' + shootingTime + '","prepareTime":"' + prepareTime + '","warningTime":"' + warningTime + '","timerMode":"' + timerMode + '","Reset":"' + reset + '"}';

//       // sending timer data only when set is pressed
//       // set is only available when "start"=0 
//       console.log(Timer_data);
//       connection.send(Timer_data);
//       document.getElementById('warning').innerHTML = "data sent"

//               // Get the reference to the <div> element
//         const warningDiv = document.getElementById('warning');

//     }
//     else {

//       document.getElementById('warning').innerHTML = "Warning! warning time must be smaller than shooting time"
//     }





//     //subtracting warningTime
//     // shootingTime=shootingTime-warningTime;




//   }

//   function StartTimer() {
//     //  start the timer  and send the start info
//     // cannot start while values are not set 
//     start = 1;
//     console.log(`start : ${start}`);
//     if (timerMode == 2) {
//       if (start == 1) {
//         document.getElementById("start_btn").disabled = true;

//       }
//       else if (start == 0) {
//         document.getElementById("start_btn").disabled = false;

//       }
//     }

//     var Start_timer = '{"Start":"' + start + '","startSB":"' + startSB + '","stopSB":"' + stopSB + '"}';
//     connection.send(Start_timer);

//   }
//   function stopTimer() {
//     // if stop button is pressed , it stops the current ongoing count!
//     // make start =0 for stoppping the count
//     // timer only counts if start =1 
//     start = 0;
//     console.log(`stop : ${start}`);
//     if (timerMode == 2) {

//       if (start == 0) {
//         document.getElementById("start_btn").disabled = false;

//       }
//     }

//     // store in the same start var in arduino 
//     var Stop_timer = '{"Stop":"' + start + '"}';
//     connection.send(Stop_timer);

//   }


//   function resetTimer() {
//     // stop the countdown 
//     console.log("reset the timer");
//     stopTimer();
//     // and reset the values to last filled values
//     // reset used to rollback totalTimein each machine
//     // reset , rollbacks the time  by sending again the data of Timer.reseting it.
//     reset = 1;
//     document.getElementById("start_btn").disabled = true;
//     document.getElementById("stop_btn").disabled = true;
//     // show the mode buttons to set mode again 
//     document.getElementById('single').disabled = false;
//     document.getElementById('dual').disabled = false;
//     setTimer();
//     reset = 0;
//     // make set visible
//     // inputs can be again filled 
//     // disbale stop

//   }

//   function setSwitch() {
//     // to reset the timer ,  repopulate the three
//     [startSB, stopSB] = [stopSB, startSB];
//     if (timerMode == 2) {
//       console.log(`startSB : ${startSB}`);
//       console.log(`stopSB : ${stopSB}`);
//     }

//     SWITCH = '{"shootingTime":"' + shootingTime + '","prepareTime":"' + prepareTime + '","warningTime":"' + warningTime + '",'
//       + '"startSB":"' + startSB + '","stopSB":"' + stopSB + '","Start":"' + start + '"}';

//     // sending timer data only when set is pressed
//     // set is only available when "start"=0 
//     console.log(SWITCH);
//     console.log(start);
//     connection.send(SWITCH);

//   }



// </script>


// </html>
// )=====";

