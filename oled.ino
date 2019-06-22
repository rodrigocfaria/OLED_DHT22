#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>
#include <DHT.h>

U8G2_SSD1306_128X64_NONAME_2_SW_I2C u8g2(U8G2_R0, A5, A4);
DHT dht(8, DHT22);

void setup()
{
  u8g2.begin();
  Serial.begin(9600);
  dht.begin();
}

float get_temp()
{
  float t = dht.readTemperature();
  if(isnan(t)) {return -999;}
  else {return t;}
}

float get_hum()
{
  float h = dht.readHumidity();
  if(isnan(h)) {return -999;}
  else {return h;}
}

void get_integer(char *string, char *integer)
{
  for(int i=0;i<strlen(string);i++)
  {
    if(string[i] != '.') 
    {
      integer[i] = string[i];
    }
    else
    {
    integer[i] = '\0';
    return;
    }
  }
}

void get_decimal(char *string, char *decimal)
{
  for(int i=0;i<strlen(string);i++)
  {
    if(string[i] == '.') 
    {
      decimal[0] = string[i+1];
      decimal[1] = '\0';
      return;
    }
  }
}


float current_temp, current_hum;
int text_width, dot_width, decimal_width, integer_width;
char current_title[16] = "               "; char title[16];

void loop()
{
  current_temp = get_temp();
  current_hum = get_hum();

  /*DISPLAY INSTRUCTIONS*/
  u8g2.firstPage();
  do
  {
    char buf[6], integer[4], decimal[2];

    /*TEMP*/
    if(current_temp != -999)
    {
      dtostrf(current_temp, 5, 1, buf);
      get_integer(buf, integer);
      get_decimal(buf, decimal);
      u8g2.setFont(u8g2_font_logisoso28_tf);
      u8g2.drawStr(0,64,integer);
      text_width = u8g2.getStrWidth(integer);
      
      u8g2.setFont(u8g2_font_logisoso16_tn);
      dot_width = u8g2.getStrWidth(".");
      u8g2.drawStr(text_width + 1,52,".");
      u8g2.drawStr(text_width + 2 + dot_width, 52, decimal);
      
      u8g2.setFont(u8g2_font_8x13B_tf);
      u8g2.drawUTF8(text_width + 2, 64, "°C");
      
    }
    /*HUM*/
    if(current_hum != -999)
    { 
      dtostrf(current_hum, 5, 1, buf);
      get_integer(buf, integer);
      get_decimal(buf, decimal);

      u8g2.setFont(u8g2_font_logisoso16_tn);
      decimal_width = u8g2.getStrWidth(decimal);
      u8g2.drawStr(128 - decimal_width, 52, decimal);
      u8g2.drawStr(128 - decimal_width - dot_width - 1, 52, ".");

      u8g2.setFont(u8g2_font_8x13B_tf);
      u8g2.drawUTF8(128 - decimal_width, 64, "%");

      u8g2.setFont(u8g2_font_logisoso28_tf);
      integer_width = u8g2.getStrWidth(integer);
      u8g2.drawStr(128 - decimal_width - integer_width - dot_width - 2, 64, integer);


    }

/* //Get title of the music from foobar   
    if(Serial.available())
    {
    strcpy(title," ");
    String s;
    s = Serial.readStringUntil('\n');
    s.toCharArray(title, 16);
    strcpy(current_title, title);
    u8g2.setFont(u8g2_font_logisoso16_tf);
    u8g2.drawStr(0, 16, title);
    Serial.println(title);
    }
    else
    {
    u8g2.setFont(u8g2_font_logisoso16_tf);
    u8g2.drawStr(0, 16, current_title);
    }
*/


  } while (u8g2.nextPage());
  /*END OF DISPLAY INSTRUCTIONS*/

  delay(1000);
}


