/*******************************************************************************
 * Sketch name: AJAX code for XML HTTP request
 * Description: Example of an XML HTTP request (used in Listings 7-5 and 7-6)
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    7 - Wireless local area network
 ******************************************************************************/

<script>              // start of JavaScript
var xhr = new XMLHttpRequest();     // XMLHttpRequest object
xhr.onreadystatechange = function()
{
  if(this.readyState == 4 && this.status == 200)  // [1] if request successful
                  // [2] update variable
  document.getElementById(variable).innerHTML = this.responseText;
};
xhr.open('GET', URL, true);         // [3] at URL
xhr.send();
</script>             // end of JavaScript
