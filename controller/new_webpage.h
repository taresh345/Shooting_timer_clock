


char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>

<head>
  <meta charset="utf-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <title></title>
  <meta name="description" content="">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="">
</head>


<style>
/* ID styles */

#profile button {
	margin-top: 50px;
    padding: 20px 70px;
}
#switch {
  display: none;
}

#displayTimerINFO {
  display: none;
  flex-direction: row; /* Display columns */
  justify-content: space-between; /* Distribute space evenly between columns */
  margin-bottom: 5px;
  border: 2px solid #000000; /* Add border */
  flex: auto;
}

/* Class styles */
.noshow {
  display: none;
}

.show {
  display: inline-block;
}

.displayBox {
  border-right: 2px solid #000000; /* Add border */
  /* padding: 2px; */
  height: 10%;
}

.displayInfo {
  font-size: 3em;
}

/* Tag styles */
body {
  font-family: Arial, sans-serif;
  text-align: center;
  background-color: rgb(127, 0, 190);
  height: 100vh;
}

form {
  text-align: center;
}

input[type="number"] {
  width: 200px;
  /* Set the desired width */
  height: 50px;
  /* Set the desired height */
  padding: 10px;
  /* Add padding for better appearance */
  box-sizing: border-box;
  /* Include padding and border in the total width and height */
  /* Add more styles as needed */
  font-size: 60px;
  margin-bottom: 10px;
  /* Add a bit of space below each label */
}

h1 {
  color: #333;
}

button {
  padding: 10px 30px;
  margin: 5px;
  cursor: pointer;
  font-size: 25px;
  border-radius: 25px;
}

label {
  display: block;
  /* Ensures each label occupies its own line */
  font-weight: bold;
  /* Makes labels bold for emphasis */
  margin: 5px;
  /* Add a bit of space below each label */
}

/* for reset timervalues button  */
#resetTimerValues {
  position: fixed;
  bottom: 10px; /* Adjust this value as needed */
  left: 50%; /* Position it horizontally in the center */
  transform: translateX(-50%); /* Center horizontally */
}

/* for active scoreboard metric  */
#displayActiveScoreboard {
  display: none;
  flex-direction: column; /* Display columns */
  justify-content: space-between;
  margin-bottom: 3.5em;
	border: 2px solid #000000; /* Add border */
}






</style>


<body>
	<div class="show" id="profile">
	  <button id="standardSingleRound" onclick="standardSingleRound()">Standard <br> Single Round </button>
	  <br>
	  <button id="standardDualRound" onclick="standardDualRound()">Standard <br> Dual Round </button>
	  <br>
	  <button id="customRound" onclick="customRound()">Custom <br> Round </button>
	</div>
  
	<form id="TimerForm" class="noshow">
	  <label for="prepareTime">INPUT PREPARE TIME</label>
	  <input type="number" value="10" id="prepareTime" class="show" min="0" max="30">
	  <br />
	  <label for="shootingTime">INPUT SHOOTING TIME</label>
	  <input type="number" value="180" id="shootingTime" class="show" min="0" max="999">
	  <br />
	  <label for="warningTime">INPUT WARNING TIME</label>
	  <input type="number" value="30" id="warningTime" class="show" min="0">
	  <br />
	  <legend>
		<h2>Select mode</h2>
	  </legend>
	  <input type="radio" id="single" name="mode" value="1" checked />
	  <label for="single">SINGLE</label>
	  <input type="radio" id="dual" name="mode" value="2" />
	  <label for="dual">DUAL</label>
	  <button type="submit" class="show" id="setTimer">> Set Timer Values</button>
	</form>
	<br />
	<button onclick="setProfile()" class="noshow" id="setProfile">
	  <= Back to <br />Standard Profiles
	</button>
  
	<div id="warning" class="show"></div>
  
	<div id="controller_btns" class="noshow">
	  <!-- display shooting data of timer -->
	  <div class="show" id="displayTimerINFO">
		<div class="displayBox" id="prepareTimeDisplay">
		  <h3> PREPARE TIME IS : </h3>
		  <div class="displayInfo" id="prepareTimeDisplayValue"></div>
		</div>
		<div class="displayBox" id="shootingTimeDisplay">
		  <h3> SHOOTING TIME IS : </h3>
		  <div class="displayInfo" id="shootingTimeDisplayValue"></div>
		</div>
		<div class="displayBox" id="warningTimeDisplay">
		  <h3> WARNING TIME IS : </h3>
		  <div class="displayInfo" id="warningTimeDisplayValue"></div>
		</div>
	  </div>
	  <!-- display shooting data of timer -->
	  <!-- display activeScoreboard in case of dual mode -->
	  <div class="noshow" id="displayActiveScoreboard">
		<h3>ACTIVE SCOREBOARD:</h3>
		<div class="displayInfo" id="activeScoreboardDisplayValue"></div>
	  </div>
  
	  <button onclick="StartTimer()" class="show" id="start_btn">start</button>
	  <button onclick="stopTimer()" class="show" id="stop_btn">stop</button>
	  <button onclick="resetTimer()" class="show" id="reset_btn">reset</button>
	  <br />
	  <br />
	  <button onclick="setSwitch()" class="show" id="switch">SWITCH</button>
	  <button onclick="resetTimerValues()" class="show" id="resetTimerValues">
		<= Back to <br>Custom Inputs
	  </button>
	</div>
	<!-- end of controller btns  -->
	<script src="" async defer></script>
  </body>
  

<script>

 // data sent once or on interrupt buttons
var timerMode = 1; //1-single ,2-dual
var set;
var shootingTime;
var prepareTime;
var warningTime;
// data sent repeatedly
// stop, reset is intterupt buttons
var start = 0;
var reset = 0;
var SWITCH;

var startSB = 1;
var stopSB = 2;

// _____________________________________


var connection = new WebSocket('ws://' + location.hostname + ':81/');

connection.onopen = function() {
	console.log('WebSocket connection established');
	// startKeepAlive();
};

connection.onclose = function(event) {
	console.log('WebSocket connection closed:', event);
};

connection.onerror = function(error) {
	console.error('WebSocket error:', error);
};

//________detect ping____________
// Event listener to handle incoming messages


function sendPing() {
	// You can customize the ping message as needed
	let type = "pong";

	let pongMessage = '{"type":"' + type + '"}';
	connection.send(pongMessage);
	console.log('Sent ping message to server');
}

// Function to start the keep-alive timer

function startKeepAlive() {
	// Set the interval for sending ping messages (e.g., every 30 seconds)
	var keepAliveInterval = 30000; // 30 seconds

	// Start the timer to send ping messages at regular intervals
	setInterval(sendPing, keepAliveInterval);
	console.log('Keep-alive timer started');
}



//___________form data validation _____________________________
function dataWarning(inputValue, inputTag) {
	// Validate the input
	if (inputValue >= document.getElementById(inputTag).min && inputValue <= Number(document.getElementById(inputTag).max)) {
		document.getElementById('warning').innerText = 'Input is valid: ' + inputValue;
		return true
	} else {
		document.getElementById('warning').innerText = 'Input is invalid.....';
		return false

	}

}

//for realtime change of input shooting time:
let shooting = document.getElementById("shootingTime")
shooting.addEventListener('input', function(event) {
	document.getElementById('warningTime').setAttribute('max', event.target.value - 1)

})


//setting max value of warningTime  shootingTime. for no conflicts

let form = document.getElementById('TimerForm');

form.addEventListener('submit', function(event) {
	event.preventDefault(); // Prevent form submission
	//handling timerMode
	const selectedMode = document.querySelector('input[name="mode"]:checked').value;
	setMode(selectedMode);


	//handling displayInfo
	let prepareInput = document.getElementById('prepareTime').value;
	let shootingInput = document.getElementById('shootingTime').value;
	let warningInput = document.getElementById('warningTime').value;


	document.getElementById('warningTime').setAttribute('max', shootingInput - 1)

	// Validate the input
	if (dataWarning(prepareInput, 'prepareTime') &&
		dataWarning(shootingInput, 'shootingTime') &&
		dataWarning(warningInput, 'warningTime')
	) {
		document.getElementById('warning').innerText = 'data valid: Press "Set Timer Values"';
		setTimer();

		//remove form settings if its validated
		document.getElementById('TimerForm').className = "noshow"
		document.getElementById('warning').className = "noshow";

		//remove back button on the form page 
		document.getElementById('setProfile').className = "noshow"


		//show control buttons 
		document.getElementById('controller_btns').className = "show";

		//column display of data 
		document.getElementById('prepareTimeDisplayValue').innerHTML = prepareInput;
		document.getElementById('warningTimeDisplayValue').innerHTML = warningInput;
		document.getElementById('shootingTimeDisplayValue').innerHTML = shootingInput;
		document.getElementById('displayTimerINFO').style.display = "flex"


		//if mode is 2 display the activescoreboard metric 
		if (selectedMode == "2") {

			document.getElementById('displayActiveScoreboard').style.display = "flex";
			document.getElementById('displayActiveScoreboard').className = "show";
			document.getElementById('activeScoreboardDisplayValue').innerHTML = startSB;

		} else if (selectedMode == "1") {
			document.getElementById('displayActiveScoreboard').style.display = "none";
			document.getElementById('displayActiveScoreboard').className = "noshow";
		}


	} else {
		document.getElementById('warning').innerText = 'invalid Timings';

	}




});


//____formData validation done ______________




function setMode(Mode) {
	timerMode = Mode;
	console.log(`the mode is set as ${timerMode}`);
	var timer_mode = '{"mode":"' + timerMode + '"}';
	connection.send(timer_mode);
	document.getElementById('single').disabled = true;
	document.getElementById('dual').disabled = true;

	if (timerMode == 2) {

		document.getElementById('switch').style.display = 'inline-block';
	} else if (timerMode == 1) {
		document.getElementById('switch').style.display = "none";

	}

}



// ____________only sent once or on  button-press___________




function setTimer() {
	var Timer_data;
	shootingTime = document.getElementById('shootingTime').value;
	prepareTime = document.getElementById('prepareTime').value;
	warningTime = document.getElementById('warningTime').value;



	//constraints____
	Timer_data = '{"shootingTime":"' + shootingTime + '","prepareTime":"' + prepareTime + '","warningTime":"' + warningTime + '","timerMode":"' + timerMode + '","Reset":"' + reset + '"}';

	// sending timer data only when set is pressed
	// set is only available when "start"=0 
	console.log(Timer_data);
	connection.send(Timer_data);

	//subtracting warningTime
	// shootingTime=shootingTime-warningTime;

}

function StartTimer() {
	//  start the timer  and send the start info
	// cannot start while values are not set 
	start = 1;
	console.log(`start : ${start}`);
	// if (timerMode == 2) {
	//   if (start == 1) {
	//     document.getElementById("start_btn").disabled = true;

	//   }
	//   else if (start == 0) {
	//     document.getElementById("start_btn").disabled = false;

	//   }
	// }

	var Start_timer = '{"Start":"' + start + '","startSB":"' + startSB + '","stopSB":"' + stopSB + '"}';
	connection.send(Start_timer);

}

function stopTimer() {
	// if stop button is pressed , it stops the current ongoing count!
	// make start =0 for stoppping the count
	// timer only counts if start =1 
	start = 0;
	console.log(`stop : ${start}`);
	// if (timerMode == 2) {

	//   if (start == 0) {
	//     document.getElementById("start_btn").disabled = false;

	//   }
	// }

	// store in the same start var in arduino 
	var Stop_timer = '{"Stop":"' + start + '"}';
	connection.send(Stop_timer);

}

function resetTimerValues() {
	stopTimer();
  resetTimer();
	document.getElementById('TimerForm').className = "show";
	document.getElementById('setProfile').className = "show";
	document.getElementById('controller_btns').className = "noshow";
	// Reset radio buttons with the name attribute "mode"

	document.getElementById("single").disabled = false;
	document.getElementById("dual").disabled = false;




}


function resetTimer() {
	// stop the countdown 
	console.log("reset the timer");
	stopTimer();
	// and reset the values to last filled values
	// reset used to rollback totalTimein each machine
	// reset , rollbacks the time  by sending again the data of Timer.reseting it.
	reset = 1;

	setTimer();
	reset = 0;
	// make set visible
	// inputs can be again filled 
	// disbale stop

}

function setSwitch() {
	// to reset the timer ,  repopulate the three

	[startSB, stopSB] = [stopSB, startSB];
	if (timerMode == 2) {
		console.log(`startSB : ${startSB}`);
		console.log(`stopSB : ${stopSB}`);
	}

	SWITCH = '{"shootingTime":"' + shootingTime + '","prepareTime":"' + prepareTime + '","warningTime":"' + warningTime + '",' +
		'"startSB":"' + startSB + '","stopSB":"' + stopSB + '","Start":"' + start + '"}';

	// sending timer data only when set is pressed
	// set is only available when "start"=0 
	console.log(SWITCH);
	console.log(start);
	connection.send(SWITCH);
	//update active scoreboard value
	document.getElementById('activeScoreboardDisplayValue').innerHTML = startSB;
}

//after clicking standardRound
function standardSingleRound(){
	//globalValues to send to scoreboard
	document.getElementById('shootingTime').value=180;
	document.getElementById('prepareTime').value=10;
	document.getElementById('warningTime').value=30;
	shootingTime =document.getElementById('shootingTime').value;
	prepareTime =document.getElementById('prepareTime').value;
	warningTime = document.getElementById('warningTime').value;

	//set timer mode to single 
	document.getElementById("single").checked=true;
	const selectedMode = document.querySelector('input[name="mode"]:checked').value;

	setMode(selectedMode);
	setTimer();
	
	//handling displayInfo
	let prepareInput = document.getElementById('prepareTime').value;
	let shootingInput = document.getElementById('shootingTime').value;
	let warningInput = document.getElementById('warningTime').value;

	//show control buttons 
	
	//column display of data 
	document.getElementById('prepareTimeDisplayValue').innerHTML = prepareInput;
	document.getElementById('warningTimeDisplayValue').innerHTML = warningInput;
	document.getElementById('shootingTimeDisplayValue').innerHTML = shootingInput;
	document.getElementById('displayTimerINFO').style.display = "flex"
	
	
	document.getElementById('controller_btns').className = "show";

	document.getElementById('profile').className="noshow";

	//if mode is 1 do not display the activescoreboard metric 
	
	document.getElementById('displayActiveScoreboard').style.display = "none";
	document.getElementById('displayActiveScoreboard').className = "noshow";


}

function standardDualRound(){
	//globalValues to send to scoreboard
		document.getElementById('shootingTime').value=30;
		document.getElementById('prepareTime').value=0;
		document.getElementById('warningTime').value=10;

	//for sending variables through "setTimer()" function
		shootingTime =document.getElementById('shootingTime').value;
		prepareTime =document.getElementById('prepareTime').value;
		warningTime = document.getElementById('warningTime').value;

	//for sending variables through "setMode()" function
	//handling timerMode
		document.getElementById("dual").checked=true;
		const selectedMode = document.querySelector('input[name="mode"]:checked').value;

	//sending the values 
		setMode(selectedMode);
		setTimer();
		
	//handling displayInfo
		let prepareInput = document.getElementById('prepareTime').value;
		let shootingInput = document.getElementById('shootingTime').value;
		let warningInput = document.getElementById('warningTime').value;
	
	//column display of data 
		document.getElementById('prepareTimeDisplayValue').innerHTML = prepareInput;
		document.getElementById('warningTimeDisplayValue').innerHTML = warningInput;
		document.getElementById('shootingTimeDisplayValue').innerHTML = shootingInput;
		document.getElementById('displayTimerINFO').style.display = "flex"

	//display the active scoreboard metric 
		document.getElementById('displayActiveScoreboard').style.display = "flex";
		document.getElementById('displayActiveScoreboard').className = "show";
		document.getElementById('activeScoreboardDisplayValue').innerHTML = startSB;
	
	//showing controller elements 
		document.getElementById('controller_btns').className = "show";
		document.getElementById('profile').className="noshow";
}

//after clicking "Custom Round"
function customRound(){

	document.getElementById('profile').className="noshow";
	document.getElementById('TimerForm').className="show";
	document.getElementById('setProfile').className="show";
}

//after clicking "back on Form Page"
function setProfile(){
	document.getElementById('profile').className="show";
	document.getElementById('setProfile').className="noshow";
	document.getElementById('TimerForm').className="noshow";

}

</script>


</html>
)=====";

