/*******************************************************************************
 * Sketch name: Streaming real-time images
 * Description: Tab with code to stream images to webpage, required by Listing 2-5
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    2 - Intranet camera
 ******************************************************************************/
 
#define Boundary "123456789000000000000987654321"
static const char* ContentType = "multipart/x-mixed-replace;boundary=" Boundary;
static const char* StreamBound = "\r\n--" Boundary  "\r\n";
static const char* StreamContent = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";
static esp_err_t stream_handler(httpd_req_t *req)
{
  camera_fb_t * frame = NULL; // as in Listing 2-1
  uint8_t * jpgBuffer = NULL; // JPEG buffer
  size_t jpgLength = 0; // length of JPEG buffer
  char * part_buf[64];
  esp_err_t res = ESP_OK; // error status
  res = httpd_resp_set_type(req, ContentType);
  if (res != ESP_OK) return res;
  while (true)
  {
    frame = esp_camera_fb_get(); // as in Listing 2-1
    if (!frame) // as in Listing 2-1
    {
      Serial.println("Camera capture failed");     // as in Listing 2-1
      res = ESP_FAIL;
    }
    else if (frame->width > 400)
    {
      jpgLength = frame->len; // set JPEG buffer length
      jpgBuffer = frame->buf; // set JPEG buffer
    }
    if (res == ESP_OK) // no error, stream image
    {
      size_t hlen = snprintf((char *)part_buf, 64, StreamContent, jpgLength);
      res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
    }
    if (res == ESP_OK) res = httpd_resp_send_chunk(req, (const char *)jpgBuffer, jpgLength);
    if (res == ESP_OK) res = httpd_resp_send_chunk(req, StreamBound, strlen (StreamBound));
    if (frame)
    {
      esp_camera_fb_return(frame); // as in Listing 2-1
      frame = NULL;
      jpgBuffer = NULL; // reset to NULL value
    }
    else if (jpgBuffer)
    {
      free(jpgBuffer); // reset to NULL value
      jpgBuffer = NULL;
    }
    if (res != ESP_OK) break;
  }
  return res;
}
