/*******************************************************************************
 * Sketch name: AJAX request with GPS position data
 * Description: display GPS latitude and longitude values -  see Listing 12-3
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    12 - GPS tracking app with Google Maps
 ******************************************************************************/

char page[] PROGMEM = R"(
<!DOCTYPE html><html>
<head><title>ESP8266</title>
<meta charset="UTF-8">
</head>
<body>
<h2>GPS</h2>
<p>Latitude: <span id = 'latId'>0</span></p>
<p>Longitude: <span id = 'longId'>0</span></p>
<p>Altitude: <span id = 'altId'>0</span> m</p>
<p>Speed: <span id = 'speedId'>0</span> kph<p>
<p>Counter: <span id = 'countId'>0</span></p>

<script>
setInterval(reload, 5000);        // reload function called every 5s
function reload()
{
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function()
  {
    if(this.readyState == 4 && this.status == 200) 
    {
      var obj = JSON.parse(this.responseText);
      document.getElementById('latId').innerHTML = obj.var1;
      document.getElementById('longId').innerHTML = obj.var2;
      document.getElementById('altId').innerHTML = obj.var3;
      document.getElementById('speedId').innerHTML = obj.var4;
      console.log(obj.var3);
    }
  };
  xhr.open('GET', '/GPSurl', true);
  xhr.send();
}

setInterval(reload2, 1000);
function reload2()        // reload2 function called every 1s
{             // to obtain countId from /countUrl
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function()
  {
    if(this.readyState == 4 && this.status == 200) 
    {document.getElementById('countId').innerHTML = this.responseText;
    console.log(this.responseText);}
  };
  xhr.open('GET', '/countUrl', true);
  xhr.send();
}
</script>
</body></html>
)";
