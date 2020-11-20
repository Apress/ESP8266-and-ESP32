/*******************************************************************************
 * Sketch name: AJAX code for webpage with ESP32-CAM photo
 * Description: Webpage code for Listing 2-3
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    2 - Intranet camera
 ******************************************************************************/
 
char page[] PROGMEM = R"(
<!DOCTYPE HTML><html><head>
<title>ESP32-CAM</title>
<style>
body {text-align:center; font-size: 25px;}
.vert {margin-bottom: 10%}
.hori {margin-bottom: 0%}
.btn {background-color:White; font-size: 25px}
table {margin: auto}
td {padding: 10px}
</style></head>
<body>
<h2>ESP32-CAM</h2>
<div id='buttons'>
<table><tr>
<td><button onclick='newPhoto()' class='btn'>New photo</button></td>
<td><button onclick='turn()' class='btn'>Rotate</button></td>
</tr></table></div>
<img src='/photoURL' id='photo' width='80%'>
<script>
function newPhoto()
{
  var xhr = new XMLHttpRequest();
  xhr.open('GET', '/photoURL', true);
  xhr.send();
  location.reload();
}
var deg = 0;
function turn()
{
  deg = deg + 90;
  var img = document.getElementById('photo');
  img.style.transform = 'rotate(' + deg + 'deg)';
  if((deg/90)%2 == 1)
  document.getElementById('buttons').className = 'vert';
  else document.getElementById('buttons').className = 'hori';
}
</script>
</body></html>
)";
