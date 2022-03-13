/*******************************************************************************
 * Sketch name: HTML code for WLAN webpage
 * Description: Webpage code for Listing 7-1
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    7 - Wireless local area network
 ******************************************************************************/

String webcode(int LEDG, int LEDR, int counter)
{
  String page = "<!DOCTYPE html><html><head>";
  page +="<title>Local network</title>";
  page +="<style>";
  page +="body {margin-top:50px; font-family:Arial;";
  page +="font-size:20px; text-align:center}";
  page +=".btn {display:block; width:220px; margin:auto; padding:30px}";
  page +=".btn {font-size:30px; color:black; text-decoration:none}";
  page +=".on {background-color:SkyBlue}";
  page +=".off {background-color:LightSteelBlue}";
  page +=".zero {background-color:Thistle}";
  page +="td {font-size:30px; margin-top:50px; margin-bottom:5px}";
  page +="p {font-size:30px; margin-top:50px; margin-bottom:5px}";
  page +="</style></head>";
  page +="<body>";
  page +="<h1>ESP8266 local area network</h1>";
  page +="<table style='width:100%'><tr>";
  if(LEDG>0)
  {
    page +="<td>Green LED is ON now<a href='/LEDGurl' class='btn on'>";
    page +="Press to turn Green LED OFF</a></td>";
  }
  else
  {
    page +="<td>Green LED is OFF now<a href='/LEDGurl' class='btn off'>";
    page +="Press to turn Green LED ON</a></td>";
  }
  if(LEDR>0)
  {
    page +="<td>Red LED is ON now<a href='/LEDRurl' class='btn on' >";
    page +="Press to turn Red LED OFF</a></td>";
  }
  else
  {  
    page +="<td>Red LED is OFF now<a href='/LEDRurl' class='btn off'>";
    page +="Press to turn Red LED ON</a></td>";
  }
  page +="</tr></table>";
  page +="<p>Counter is "+String(counter)+" now<a href='/zeroUrl'";
  page +="class='btn zero'>Press to zero counter</a></p>";
  page +="</body></html>";
  return page;
}
