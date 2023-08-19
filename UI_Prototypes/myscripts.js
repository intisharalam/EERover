var isArrowKeyPressed = false;
var isLetterKeyPressed = false;
var xhttp = new XMLHttpRequest();

// Motor control functions
function up() {
    event.stopPropagation();
    event.preventDefault();
    xhttp.open("GET", "/up");
    xhttp.send();
    console.log("UP!")
}
function down() {
    event.stopPropagation();
    event.preventDefault();
    xhttp.open("GET", "/down");
    xhttp.send();
    console.log("DOWN!");
}
function left() {
    event.stopPropagation();
    event.preventDefault();
    xhttp.open("GET", "/left");
    xhttp.send();
    console.log("LEFT!");
}
function right() {
    event.stopPropagation();
    event.preventDefault();
    xhttp.open("GET", "/right");
    xhttp.send();
    console.log("RIGHT!");
}
function stop() {
    event.stopPropagation();
    event.preventDefault();
    xhttp.open("GET", "/stop");
    xhttp.send();
}


// Sensor Button handlers
function sendButtonClick(sensor) {
    event.stopPropagation();
    event.preventDefault();

    console.log(sensor + " button clicked!");

    var ipAddress = window.location.hostname;
    var url = "http://" + ipAddress + "/" + sensor;

    setTimeout(function () {
        fetch(url)
          .then(response => {
            if (response.ok) {
              response.text().then(value => addDataToList(sensor, value));
            } else {
              throw new Error("Request failed with status " + response.status);
            }
          })
          .catch(error => {
            console.error("Error:", error);
            // Handle the error here, such as displaying an error message to the user
          });
      }, 500); // Delay of 500 milliseconds
}

function addDataToList(sensor, value) {
    var olElement = document.getElementById(sensor);
    var liElement = document.createElement('li');
    liElement.textContent = value;
    olElement.appendChild(liElement);
}


function incrServoY() {
    event.stopPropagation();
    event.preventDefault();
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "/IncrY");
    xhttp.send();
    console.log("Y incremented!");
}

function decrServoY() {
    event.stopPropagation();
    event.preventDefault();
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "/DecrY");
    xhttp.send();
    console.log("Y decremented!");
}

function incrServoZ() {
    event.stopPropagation();
    event.preventDefault();
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "/IncrZ");
    xhttp.send();
    console.log("Z incremented!");
}

function decrServoZ() {
    event.stopPropagation();
    event.preventDefault();
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "/DecrZ");
    xhttp.send();
    console.log("Z decremented!");
}

var radBtn = document.createElement("button");
var irBtn = document.createElement("button");
var poleBtn = document.createElement("button");
radBtn.textContent = "Radio";
radBtn.className = "btn2 top";
irBtn.textContent = "IR";
irBtn.className = "btn2";
poleBtn.textContent = "Pole";
poleBtn.className = "btn2 bott";
radBtn.addEventListener('touchstart', function () { sendButtonClick("Radio"); });
radBtn.addEventListener('mousedown', function () { sendButtonClick("Radio"); });
irBtn.addEventListener('touchstart', function () { sendButtonClick("IR"); });
irBtn.addEventListener('mousedown', function () { sendButtonClick("IR"); });
poleBtn.addEventListener('touchstart', function () { sendButtonClick("Pole"); });
poleBtn.addEventListener('mousedown', function () { sendButtonClick("Pole"); });
var senCtrlDiv = document.querySelector(".SenCtrl");
senCtrlDiv.appendChild(radBtn);
senCtrlDiv.appendChild(irBtn);
senCtrlDiv.appendChild(poleBtn);
var arrUpBtn = document.createElement("button");
var arrDownBtn = document.createElement("button");
var arrRightBtn = document.createElement("button");
var arrLeftBtn = document.createElement("button");
var fillDiv1 = document.createElement("div");
var fillDiv2 = document.createElement("div");
var fillDiv3 = document.createElement("div");
var fillBtn = document.createElement("button")
arrUpBtn.className = "btn1";
arrDownBtn.className = "btn1";

arrRightBtn.className = "btn1";
arrLeftBtn.className = "btn1";
fillBtn.textContent = "ROVER";
var arrowIconUp = document.createElement("i");
var arrowIconLeft = document.createElement("i");
var arrowIconRight = document.createElement("i");
var arrowIconDown = document.createElement("i");
arrowIconUp.className = "arrow up";
arrowIconLeft.className = "arrow left";
arrowIconRight.className = "arrow right";
arrowIconDown.className = "arrow down";
arrUpBtn.appendChild(arrowIconUp);
arrDownBtn.appendChild(arrowIconDown);
arrRightBtn.appendChild(arrowIconRight);
arrLeftBtn.appendChild(arrowIconLeft);
arrUpBtn.addEventListener('touchstart', up);
arrUpBtn.addEventListener('mousedown', up);
arrDownBtn.addEventListener('touchstart', down);
arrDownBtn.addEventListener('mousedown', down);
arrLeftBtn.addEventListener('touchstart', left);
arrLeftBtn.addEventListener('mousedown', left);
arrRightBtn.addEventListener('touchstart', right);
arrRightBtn.addEventListener('mousedown', right);
var rovCtrlDiv = document.querySelector(".RovCtrl");
rovCtrlDiv.appendChild(fillDiv1);
rovCtrlDiv.appendChild(arrUpBtn);
rovCtrlDiv.appendChild(fillDiv2);
rovCtrlDiv.appendChild(arrLeftBtn);
rovCtrlDiv.appendChild(fillBtn);
rovCtrlDiv.appendChild(arrRightBtn);
rovCtrlDiv.appendChild(fillDiv3);
rovCtrlDiv.appendChild(arrDownBtn);

var upBtn = document.createElement("button");
var downBtn = document.createElement("button");
var advBtn = document.createElement("button");
var revBtn = document.createElement("button");
var fillDiv4 = document.createElement("div");
var fillDiv5 = document.createElement("div");
var fillDiv6 = document.createElement("div");
var fillBtn1 = document.createElement("button")
upBtn.textContent = "UP";
downBtn.textContent = "DOWN";
advBtn.textContent = "ADV";
revBtn.textContent = "REV";
fillBtn1.textContent = "ARM";
upBtn.addEventListener('touchstart', incrServoY);
upBtn.addEventListener('mousedown', incrServoY);
downBtn.addEventListener('touchstart', decrServoY);
downBtn.addEventListener('mousedown', decrServoY);
advBtn.addEventListener('touchstart', incrServoZ);
advBtn.addEventListener('mousedown', incrServoZ);
revBtn.addEventListener('touchstart', decrServoZ);
revBtn.addEventListener('mousedown', decrServoZ);
var armCtrlDiv = document.querySelector(".ArmCtrl");
var armCtrlDiv = document.querySelector(".ArmCtrl");
armCtrlDiv.appendChild(fillDiv4);
armCtrlDiv.appendChild(upBtn);
armCtrlDiv.appendChild(fillDiv5);
armCtrlDiv.appendChild(advBtn);
armCtrlDiv.appendChild(fillBtn1);

armCtrlDiv.appendChild(revBtn);
armCtrlDiv.appendChild(fillDiv6);
armCtrlDiv.appendChild(downBtn);

var isArrowKeyPressed = false;

var isArrowKeyPressed = false;

document.addEventListener("keydown", function (event) {
    var keyCode = event.keyCode;

    if (keyCode === 37) {
        if (!isArrowKeyPressed) {
            incrServoZ();
            isArrowKeyPressed = true;
        }
    } else if (keyCode === 38) {
        if (!isArrowKeyPressed) {
            incrServoY();
            isArrowKeyPressed = true;
        }
    } else if (keyCode === 39) {
        if (!isArrowKeyPressed) {
            decrServoZ();
            isArrowKeyPressed = true;
        }
    } else if (keyCode === 40) {
        if (!isArrowKeyPressed) {
            decrServoY();
            isArrowKeyPressed = true;
        }
    } else if (keyCode === 90 || keyCode === 88 || keyCode === 67) {
        if (!isArrowKeyPressed) {
            var buttonName;
            if (keyCode === 90) {
                buttonName = "Radio";
            } else if (keyCode === 88) {
                buttonName = "IR";
            } else if (keyCode === 67) {
                buttonName = "Pole";
            }
            sendButtonClick(buttonName);
            isArrowKeyPressed = true;
        }
    } else if (keyCode === 65) {
        if (!isArrowKeyPressed) {
            left();
            isArrowKeyPressed = true;
        }
    } else if (keyCode === 87) {
        if (!isArrowKeyPressed) {
            up();
            isArrowKeyPressed = true;
        }
    } else if (keyCode === 68) {
        if (!isArrowKeyPressed) {
            right();
            isArrowKeyPressed = true;
        }
    } else if (keyCode === 83) {
        if (!isArrowKeyPressed) {
            down();
            isArrowKeyPressed = true;
        }
    }
});

document.addEventListener("keyup", function (event) {
    var keyCode = event.keyCode;

    if (
        keyCode === 37 ||
        keyCode === 38 ||
        keyCode === 39 ||
        keyCode === 40 ||
        keyCode === 65 ||
        keyCode === 87 ||
        keyCode === 68 ||
        keyCode === 83 ||
        keyCode === 90 ||
        keyCode === 88 ||
        keyCode === 67
    ) {
        isArrowKeyPressed = false;
        stop();
    }
});



var buttons = document.getElementsByClassName("btn1");
for (var i = 0; i < buttons.length; i++) {
    var button = buttons[i]; button.addEventListener("mouseup", function (event) { event.preventDefault(); stop(); });
    button.addEventListener("touchend", function (event) { event.preventDefault(); stop(); });
}