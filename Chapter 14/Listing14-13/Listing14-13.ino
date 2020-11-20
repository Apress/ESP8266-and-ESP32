/*******************************************************************************
 * Sketch name: Websocket and AJAX code for webpage
 * Description: update webpage with received message - see Listing 14-12
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    14 - ESP-NOW and LoRa communication
 ******************************************************************************/

char page[] PROGMEM = R"(
<!DOCTYPE html><html>
<head>
<meta name='viewport' content='width=device-width, initial-scale=1.0'>
<meta charset='UTF-8'>
<title>ESP8266</title>
<style>
body {font-family:Arial}
</style></head>
<body id='initialise'>
<h2>LoRa and WebSocket</h2>
<p>last message at <span id='timeNow'</span></p>
<p>received <span id='rxText'> </span></p>
<p>RSSI <span id='RSSI'>0</span> dBm</p>
<p>SNR <span id='SNR'>0</span> dB</p>
<script>
var wskt;
document.getElementById('initialise').onload = function() {init()};
function init()            // open WebSocket
{
  wskt = new WebSocket('ws://' + window.location.hostname + ':81/');
  wskt.onmessage = function(rx)
  {
    var obj = JSON.parse(rx.data);
    document.getElementById('rxText').innerHTML = obj.var1;
    document.getElementById('RSSI').innerHTML = obj.var2;
    document.getElementById('SNR').innerHTML = obj.var3;
    var dt = new Date();
    var tm = dt.toLocaleTimeString ('en-GB');
    document.getElementById('timeNow').innerHTML = tm;
  };
}
</script>
</body></html>
)";
