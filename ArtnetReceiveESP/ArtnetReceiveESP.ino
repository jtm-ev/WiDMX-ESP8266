#include <Artnet.h>

//WS2812 specific - testing only
#include <WS2812FX.h>
WS2812FX ws2812fx  = WS2812FX(1, 5, NEO_GRBW + NEO_KHZ800);

const char* ssid     = "yourssid";
const char* password = "yourpassword";

float t = 0;

int intensity_r = 0;
float fade_r = 0;

int intensity_g = 0;
float fade_g = 0;

int intensity_b = 0;
float fade_b = 0;

Artnet artnet;

void setup()
{
  //Serial.begin(115200);
  IPAddress ip(192, 168, 178, 210);
  IPAddress gateway(192, 168, 178, 1);
  //Serial.print(F("Setting static ip to : "));
  //Serial.println(ip);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.config(ip, gateway, subnet);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    //Serial.print(".");
  }

  //Serial.println("");
  //Serial.print("Connected to ");
  //Serial.println(ssid);
  //Serial.print("IP address: ");
  //Serial.println(WiFi.localIP());

  artnet.begin();

  //WS2812 specific - testing only
  ws2812fx.init();
  ws2812fx.setBrightness(255);
  ws2812fx.setMode(FX_MODE_STATIC);
  ws2812fx.start();
}

void loop()
{
  float value_r = (cos(t - PI) * (0.5 - (0.5 * intensity_r) / 255) + 0.5) * 255 + (intensity_r / 2);
  value_r = value_r * (fade_r / 255.0);

  float value_g = (cos(t - PI) * (0.5 - (0.5 * intensity_g) / 255) + 0.5) * 255 + (intensity_g / 2);
  value_g = value_g * (fade_g / 255.0);

  float value_b = (cos(t - PI) * (0.5 - (0.5 * intensity_b) / 255) + 0.5) * 255 + (intensity_b / 2);
  value_b = value_b * (fade_b / 255.0);
  
  ws2812fx.setPixelColor(0, value_r, value_g, value_b);
  ws2812fx.show();
  if (artnet.read() == ART_DMX)
  {
    // print out data of Channel 0, universe 0
    if (artnet.getUniverse() == 0)
    {
      intensity_r = artnet.getDmxFrame()[0];
      fade_r = artnet.getDmxFrame()[1];

      intensity_g = artnet.getDmxFrame()[2];
      fade_g = artnet.getDmxFrame()[3];

      intensity_b = artnet.getDmxFrame()[4];
      fade_b = artnet.getDmxFrame()[5];
      //WS2812 specific - testing only
      //ws2812fx.setPixelColor(0, artnet.getDmxFrame()[0], artnet.getDmxFrame()[1], artnet.getDmxFrame()[2]);
      //ws2812fx.show();
      //Serial.println(artnet.getDmxFrame()[0]);
    }
  }
  t += 0.0015;
  if (t >= 2 * PI) {
    t = 0;
  }

}
