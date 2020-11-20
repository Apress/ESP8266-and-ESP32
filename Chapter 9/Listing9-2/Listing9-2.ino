/*******************************************************************************
 * Sketch name: WebSocket webpage AJAX code
 * Description: webpage code to display server-client conversation for Listing 9-1
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    9 - WebSocket
 ******************************************************************************/

char page[] PROGMEM = R"(
<!DOCTYPE html><html>
<head><title>ESP8266</title>
<style>
body {font-family:Arial}
td {vertical-align: top;}
textarea {font-family:Arial; width:300px; height:50px;}
input[type=submit] {background-color:yellow;}
</style></head>
<body id='initialise'>
<h2>WebSocket</h2>
<table><tr>
<td>transmit text</td>
<td>receive text (click to clear)</td>
</tr><tr>
<td><form action='javascript:sendText()'>
<textarea id='txText'></textarea><br>
<input type='submit' value="send text">
</form></td>
<td><textarea id='rxText' onfocus='this.value=""'></textarea><br></td>
</tr></table>

<script>
var wskt;
document.getElementById('initialise').onload = function() {init()};

function init()              // open WebSocket
{
  wskt = new WebSocket('ws://' + window.location.hostname + ':81/');
  wskt.onmessage = function(rx)
  {                 // client receive message
    var obj = rx.data;
    document.getElementById('rxText').value = obj;
  };
}
function sendText()           // client transmit message
{
  wskt.send(document.getElementById('txText').value);
  document.getElementById('txText').value = "";
}
</script>
</body></html>
)";
