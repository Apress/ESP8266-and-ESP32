/*******************************************************************************
 * Sketch name: Websocket webpage AJAX code
 * Description: update webpage with received structure data - see Listing 14-8
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
<title>ESP-NOW</title>
<style>
body {font-family:Arial}
</style></head>
<body id='initialise'>
<h2>ESP-NOW with ESP8266</h2>
<p>last message at <span id='timeNow'</span></p>
<p>received from <span id='MAC'> </span></p>
<p>counter <span id='count'>0</span></p>
<p>number <span id='value'>0</span></p>
<p>message <span id='text'> </span></p>
<script>
var wskt;
document.getElementById('initialise').onload = function() {init()};
function init()
{
  wskt = new WebSocket('ws://' + window.location.hostname + ':81/');
  wskt.onmessage = function(rx)
  {
    var obj = JSON.parse(rx.data);
    document.getElementById('MAC').innerHTML = obj.var1;
    document.getElementById('count').innerHTML = obj.var2;
    document.getElementById('value').innerHTML = obj.var3;
    document.getElementById('text').innerHTML = obj.var4;
    var dt = new Date();
    var tm = dt.toLocaleTimeString ('en-GB');
    document.getElementById('timeNow').innerHTML = tm;
  };
}
</script>
</body></html>
)";
