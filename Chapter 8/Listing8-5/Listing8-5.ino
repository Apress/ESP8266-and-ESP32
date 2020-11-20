/*******************************************************************************
 * Sketch name: AJAX code with JSON parsing
 * Description: Webpage code with JSON for revised Listing 8-1
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
<p>Temperature: <span id = tempId>0</span>&degC</p>
<p>Counter: <span id = 'var1'>0</span></p>
<p>LED is <span id = 'var2'> </span><p>

<script>
setInterval(reload, 5000);        // time in milliseconds
function reload()           // update the temperature every 5s
{
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function()
  {
    if(this.readyState == 4 && this.status == 200) 
    {document.getElementById('tempId').innerHTML = this.responseText;}
  };
  xhr.open('GET', '/tempUrl', true);
  xhr.send();
}

setInterval(countLEDreload, 1000);
function countLEDreload()         // update the counter and
{                 // LED state every second
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function()
  {
    if(this.readyState == 4 && this.status == 200)
    {                   // parse JSON text
      var obj = JSON.parse(this.responseText);
      document.getElementById('var1').innerHTML = obj.var1;
      document.getElementById('var2').innerHTML = obj.var2;
    }
  };
  xhr.open('GET', '/countLEDurl', true);
  xhr.send();
}
</script>
</body></html>
)";
