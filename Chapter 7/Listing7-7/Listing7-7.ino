/*******************************************************************************
 * Sketch name: AJAX code to periodically update counter
 * Description: Automated updating of counter on webpage (inclusion in Listing 7-6)
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    7 - Wireless local area network
 ******************************************************************************/

setInterval(reload, 1000);
function reload() 
{
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function()
  {
    if(this.readyState == 4 && this.status == 200)
    document.getElementById('counter').innerHTML = this.responseText;
  };
  xhr.open('GET', 'countUrl', true);
  xhr.send();
}
