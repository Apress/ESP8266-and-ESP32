/*******************************************************************************
 * Sketch name: AJAX code with JSON parsing
 * Description: Webpage code to display ThingSpeak data - combine with Listing 8-6
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
<p>Date: <span id = 'var1'>00 000 0000</span></p>
<p>Time: <span id = 'var2'>00:00:00</span></p>
<p>Temp is <span id = 'var3'>0</span>&degC<p>
<p>Humidity is <span id = 'var4'>0</span>%<p>

<script>
setInterval(APIreload, 30000);        // time in milliseconds
function APIreload()
{
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function()
  {
    if(this.readyState == 4 && this.status == 200)
    {
      var obj = JSON.parse(this.responseText);
      document.getElementById('var1').innerHTML = obj.var1;
      document.getElementById('var2').innerHTML = obj.var2;
      document.getElementById('var3').innerHTML = obj.var3;
      document.getElementById('var4').innerHTML = obj.var4;
    }
  };
  xhr.open('GET', 'API', true);
  xhr.send();
}
</script>
</body></html>
)";
