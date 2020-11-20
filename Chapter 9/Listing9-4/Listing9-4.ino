/*******************************************************************************
 * Sketch name: AJAX code for height and distance measurement
 * Description: control servo motor and laser on webpage - AJAX code for Listing 9-3
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    9 - WebSocket
 ******************************************************************************/

char page[] PROGMEM = R"(
<!DOCTYPE html><html>
<head>
<meta name='viewport' content='width=device-width, initial-scale=1.0'>
<meta charset='UTF-8'>
<title>ESP8266</title>
<style>
html {text-align: center}
.slider {-webkit-appearance: none; height: 2px; background: DarkGrey}
img {width:25px; height:50px}
td {height:50px}
</style>
<title>WebSocket</title>
</head>
<!-- initiate WebSocket when webpage loaded-->
<body id='initialise'>
<h2>Servo control</h2>
<table align='center'><tr>
<td colspan='2'><input type='radio' id='r1' onclick='changeLaser()'>
     Change laser
<img id='bulb' src='https://www.w3schools.com/jsref/pic_bulboff.gif'>
<span id='laserId'>Off</span></td>
</tr>
<tr>
<!--autocomplete='off': returns slider to default mid-point position-->
<td><input autocomplete='on' type='range' min='20' max='90' value='20' 
    class='slider' id='FBSlider' oninput='sendFB()'></td>
<td><label id='FBId'>decrease - increase angle (20&deg)</label></td>
</tr>
<tr>
<td style='width:200px'>Height: <span id='vertical'>0</span> cm</td>
<td>Distance: <span id='horizontal'>0</span> cm</td>
</tr></table>

<script>
var FBVal = 20;
var laserVal = 0;
document.getElementById('initialise').onload = function()
{init()};

function init() 
{
  wskt = new WebSocket('ws://' + window.location.hostname + ':81/');
  wskt.onmessage = function(rx)
  {
    var obj = JSON.parse(rx.data);
    console.log(obj.var1);
    console.log(obj.var2);
    document.getElementById('vertical').innerHTML = obj.var1;
    document.getElementById('horizontal').innerHTML = obj.var2;
  };
}
function sendFB()
{
  FBVal = document.getElementById('FBSlider').value;
  document.getElementById('FBId').innerHTML = 'decrease - increase angle ('
    + FBVal.toString() + '&deg)';
  wskt.send(FBVal +','+ laserVal);
  console.log(FBVal);
  console.log(laserVal);
}

function changeLaser()
{
  laserVal = 1 - laserVal;
  if(laserVal == 1) {laserTag = 'On';}
  else {laserTag = 'Off';}
  document.getElementById('laserId').innerHTML = laserTag;
  document.getElementById('r1').checked=false;
  wskt.send(FBVal +',' + laserVal);
  var image = document.getElementById('bulb');
  if (image.src.match('bulboff')) {image.src =
                    'https://www.w3schools.com/js/pic_bulbon.gif';}
  else {image.src = 'https://www.w3schools.com/js/pic_bulboff.gif';}
}
</script>
</body></html>
)";
