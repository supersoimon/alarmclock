#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include <WiFi.h>
#include "time.h"

#define TFT_SCLK 0
#define TFT_MOSI 1 
#define TFT_RST 2
#define TFT_DC 3
#define TFT_CS 4
#define TFT_BL 5

const char* ssid     = "ssid";
const char* password = "password";

const char* time_zone = "EST5EDT,M3.2.0,M11.1.0"; // edit for your own time zone

class mescreen : public Adafruit_ST7789 {
  public:
    mescreen(int8_t cs, int8_t dc, int8_t mosi, int8_t sclk, int8_t rst) : 
    Adafruit_ST7789(cs, dc, mosi, sclk, rst) {}
    
    void setCustomOffsets(uint8_t col, uint8_t row) {
      _colstart = col;
      _rowstart = row;
    }
};

mescreen tft = mescreen(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

void setup() {
  Serial.begin(115200);
  tft.init(76, 284);
  tft.setCustomOffsets(82, 18);
  tft.invertDisplay(false); 
  tft.setRotation(1); 
  tft.fillScreen(ST77XX_BLACK);
  Serial.println("TFT Initialized!");
  tft.setCursor(0,0); 
  tft.setTextSize(5); 
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK); 


  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
        delay(500); 
  }
    
  configTzTime(time_zone, "pool.ntp.org", "time.nist.gov");
  
  struct tm timeinfo;
  while(!getLocalTime(&timeinfo)){
    delay(500);
  }  

}

void loop() {

  struct tm timeinfo;
  
  if(getLocalTime(&timeinfo)){
    int hour   = timeinfo.tm_hour; 
    int minute = timeinfo.tm_min;      
    
    char buffy[6]; 
    snprintf(buffy, sizeof(buffy), "%02d:%02d", hour, minute);
    String timeString = String(buffy);
    
    tft.fillScreen(ST77XX_BLACK); 
    tft.setCursor(0,0);
    tft.print(timeString);

  }

  delay(30000);


}
