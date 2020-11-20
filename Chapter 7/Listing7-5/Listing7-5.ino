/*******************************************************************************
 * Sketch name: AJAX code for updating a variable
 * Description: Example of an AJAX function (used in Listing 7-6)
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    7 - Wireless local area network
 ******************************************************************************/

function sendData(butn)
{
  if(butn == 'Red LED' || butn == 'Green LED')
  {
    var state = document.getElementById(butn).className;
    state = (state == 'btn on' ? 'btn off' : 'btn on');
    text =  (state == 'btn on' ? butn + ' OFF' : butn + ' ON');
    document.getElementById(butn).className = state;
    document.getElementById(butn).innerHTML = 'Press to turn ' + text;
  }
  var URL, variab, text;
   
  else if(butn == 'Green LED')
  {
    URL = 'LEDGurl';
    variab = 'LEDG';
  }
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function(butn)
  {
    if (this.readyState == 4 && this.status == 200)
    document.getElementById(variab).innerHTML = this.responseText;
  };
  xhr.open('GET', URL, true);
  xhr.send();
}
