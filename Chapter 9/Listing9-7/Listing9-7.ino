/*******************************************************************************
 * Sketch name: AJAX code for real-time sensor graphics
 * Description: webpage code to update image and sensor data - use with Listing 9-6
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
<!-- padding top right bottom left-->
<style>
html {text-align: center}
.slider {-webkit-appearance: none; height: 2px; background: DarkGrey}
td {padding: 0px 0px 0px 25px}
img {width:253px; height:384px}
</style>
<title>ESP8266</title>
</head>
<body onload='javascript:init()'>
<table>
<tr><th></th><th><h2>BMP280 at <span id = 'timeNow'></span></h2></th></tr>
<tr>
<td rowspan='7'>
<img src=
'https://images.springer.com/sgw/books/medium/9781484263358.jpg'
  alt='book'></td>
<td>Temperature: <span id = 'temp'>0</span>&degC</td>
</tr>
<tr><td>Interval: <span id='interval'>1</span>s</td></tr>
<tr><td>
<canvas id = 'myCanvas' width = '445' height = '200' 
    style = 'border:1px solid DarkGrey'>
    Your browser does not support the canvas element.
</canvas>
</td></tr>
<tr><td>
<input autocomplete='off' type='range' min='1' max='100' value='25' class='slider' id='maxySlider' oninput='setMaxy()' >
<label id='maxyId'>Maximum: 25</label>
</td></tr>
<tr><td>
<input autocomplete='off' type='range' min='0' max='100' value='15' class='slider' id='minySlider' oninput='setMiny()' >
<label id='minyId'>Minimum: 15</label>
</td></tr>
<tr><td>
<input type='radio' id='r1' oninput='sendadd()'> Increase interval
</td></tr>
<tr><td>
<input type='radio' id='r2' oninput='sendsub()'> Decrease interval
</td></tr>
</table>
<script>
var canvas = document.getElementById('myCanvas');
var ctx = canvas.getContext('2d');
ctx.strokeStyle = 'red';
ctx.strokeRect(25, 20, 400, 160);
ctx.lineWidth = 1;
var y = 0;
var miny = 15;
var maxy = 25;
var timeval = 1;
var datay = [0];
var Ndata = 0;
var maxVal = 20;
var dt = 0;
var tm = 0;
function init() 
{
  webSocket = new WebSocket('ws://' + window.location.hostname + ':81/');
  webSocket.onmessage = function(rx)
  {
    var obj = JSON.parse(rx.data);
    document.getElementById('temp').innerHTML = obj.var1;
    document.getElementById('interval').innerHTML = obj.var2;
    Ndata++;
    if(Ndata>maxVal) datay.shift();
    datay.push(obj.var1);
    dt = new Date();
    tm = dt.toLocaleTimeString
      ('en-GB', {weekday: 'long', day: '2-digit', month: 'long'});
    document.getElementById('timeNow').innerHTML = tm;
    graph()
  };
}
function graph()
{
  ctx.clearRect(0, 0, 445, 200);
  ctx.strokeStyle = 'red';
  ctx.strokeRect(25, 20, 400, 160);
  for (i=0; i<6; i++) 
    ctx.fillText(Math.round(maxy-i*(maxy-miny)/5), 3, 25+31*i);
  if(Ndata<21) {for (i=0; i<21; i=i+2) 
    ctx.fillText(String(i), 27+19*i, 193);}
  if(Ndata>20) {for (i=0; i<21; i=i+2) 
    ctx.fillText(String(Ndata+i-20), 27+19*i, 193);}
  ctx.beginPath();
  y = 20+160*(maxy-datay[0])/(maxy-miny);
  if(y<20) y=20;
  if(y>180) y=180;
  ctx.moveTo(25, y);
  for(i=1; i<21; i++)
  {
    y = 20+160*(maxy-datay[i])/(maxy-miny);
    if(y<20) y=20;
    if(y>180) y=180;
    ctx.strokeStyle = 'blue';
    ctx.lineTo(25+20*i, y);
  }
  ctx.stroke();
}
function setMaxy()
{
  maxy = document.getElementById('maxySlider').value;
  document.getElementById('maxyId').innerHTML =
    'Maximum: ' + maxy.toString();
}
function setMiny()
{
  miny = document.getElementById('minySlider').value;
  document.getElementById('minyId').innerHTML =
    'Minimum: ' + miny.toString();
}
function sendadd()
{
  timeval = parseInt(document.getElementById('interval').innerHTML) + 1;
  document.getElementById('interval').innerHTML = timeval;
  document.getElementById('r1').checked=false;
  webSocket.send(timeval);
}
function sendsub()
{
  timeval = parseInt(document.getElementById('interval').innerHTML) - 1;
  if(timeval < 1) timeval = 1;
  document.getElementById('interval').innerHTML = timeval;
  document.getElementById('r2').checked=false;
  webSocket.send(timeval);
}
</script>
</body></html>
)";
