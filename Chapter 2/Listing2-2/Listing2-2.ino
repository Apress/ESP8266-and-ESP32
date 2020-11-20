/*******************************************************************************
 * Sketch name: Camera configuration instructions tab
 * Description: Tab containing camera pin numbers and configuration details
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    2 - Intranet camera
 ******************************************************************************/

camera_config_t config;        // store camera configuration parameters
void configCamera()
{
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = 5;
  config.pin_d1 = 18;
  config.pin_d2 = 19;           // GPIO pin numbers
  config.pin_d3 = 21;
  config.pin_d4 = 36;
  config.pin_d5 = 39;
  config.pin_d6 = 34;
  config.pin_d7 = 35;
  config.pin_xclk = 0;
  config.pin_pclk = 22;
  config.pin_vsync = 25;
  config.pin_href = 23;
  config.pin_sscb_sda = 26;
  config.pin_sscb_scl = 27;
  config.pin_pwdn = 32;
  config.pin_reset = -1;
  config.xclk_freq_hz = 20000000;       // clock speed of 20MHz
  config.pixel_format = PIXFORMAT_JPEG;     // JPEG file format
  config.frame_size = FRAMESIZE_SVGA;     // SVGA 800600 pixels
  config.jpeg_quality = 10;         // image quality index
  config.fb_count = 1;            // frame buffer count
  esp_err_t err = esp_camera_init(&config);   // initialize camera
  if (err != ESP_OK)
  {
    Serial.print("Camera initialise failed with error");
    Serial.println(err);
    return;
  }
}
