/*******************************************************************************
 * Sketch name: AJAX request with BMP280 and LED
 * Description: Webpage code for revised Listing 8-1
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    8 - Updating a webpage
 ******************************************************************************/

char page[] PROGMEM = R"(
<!DOCTYPE html><html>
<head><title>ESP8266</title></head>
<body>
<h2>BMP280</h2>
<p>Temperature: <span id = 'tempId'>0</span>&degC</p>
<p>Counter: <span id = 'countId'>0</span></p>
<p>LED is <span id = 'LEDid'> </span><p>

<script>
setInterval(reload, 5000);      // time in milliseconds
function reload()         // reload function called every 5s
{             // to get tempId from tempUrl
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function()
  {
    if(this.readyState == 4 && this.status == 200) 
    document.getElementById('tempId').innerHTML = this.responseText;
  };
  xhr.open('GET', 'tempUrl', true);
  xhr.send();
}

setInterval(LEDreload, 1000);
function LEDreload()        // LEDreload function called every 1s
{             // to get countId from countUrl
  var xhr = new XMLHttpRequest();   // and LEDid from LEDurl
  xhr.onreadystatechange = function()
  {
    if(this.readyState == 4 && this.status == 200) 
    document.getElementById('countId').innerHTML = this.responseText;
  };
  xhr.open('GET', 'countUrl', true);
  xhr.send();
  
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function()
  {
    if(this.readyState == 4 && this.status == 200) 
    document.getElementById('LEDid').innerHTML = this.responseText;
  };
  xhr.open('GET', 'LEDurl', true);
  xhr.send();
}
</script>
</body></html>
)";
