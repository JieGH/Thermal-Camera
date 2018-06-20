/***************************************************
  This project may can not be completed by the help from Adafruit and other online resources.

  MIT license

****************************************************/

/***************************************************
  This is our GFX example for the Adafruit ILI9341 Breakout and Shield
  ----> http://www.adafruit.com/products/1651

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/


#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <XPT2046_Touchscreen.h>
#include <Wire.h>
#include <Adafruit_AMG88xx.h> 
#include <Fonts/FreeSans9pt7b.h> //advanced fonts for tft text draw 
#include <Fonts/FreeMonoBold12pt7b.h> // same here 

#define TFT_DC D4 
#define TFT_CS D8

#define CS_PIN  D3 
#define TIRQ_PIN  3 // for touch function of the screen
//ON WEMOS D1 MINI (or conpatible) RX PIN，using RX when running out of pins only 

int mintemp = 18;
int maxtemp = 28;
#define MINTEMP mintemp
#define MAXTEMP maxtemp

const uint16_t camColors[] = {0x480F,
                              0x400F, 0x400F, 0x400F, 0x4010, 0x3810, 0x3810, 0x3810, 0x3810, 0x3010, 0x3010,
                              0x3010, 0x2810, 0x2810, 0x2810, 0x2810, 0x2010, 0x2010, 0x2010, 0x1810, 0x1810,
                              0x1811, 0x1811, 0x1011, 0x1011, 0x1011, 0x0811, 0x0811, 0x0811, 0x0011, 0x0011,
                              0x0011, 0x0011, 0x0011, 0x0031, 0x0031, 0x0051, 0x0072, 0x0072, 0x0092, 0x00B2,
                              0x00B2, 0x00D2, 0x00F2, 0x00F2, 0x0112, 0x0132, 0x0152, 0x0152, 0x0172, 0x0192,
                              0x0192, 0x01B2, 0x01D2, 0x01F3, 0x01F3, 0x0213, 0x0233, 0x0253, 0x0253, 0x0273,
                              0x0293, 0x02B3, 0x02D3, 0x02D3, 0x02F3, 0x0313, 0x0333, 0x0333, 0x0353, 0x0373,
                              0x0394, 0x03B4, 0x03D4, 0x03D4, 0x03F4, 0x0414, 0x0434, 0x0454, 0x0474, 0x0474,
                              0x0494, 0x04B4, 0x04D4, 0x04F4, 0x0514, 0x0534, 0x0534, 0x0554, 0x0554, 0x0574,
                              0x0574, 0x0573, 0x0573, 0x0573, 0x0572, 0x0572, 0x0572, 0x0571, 0x0591, 0x0591,
                              0x0590, 0x0590, 0x058F, 0x058F, 0x058F, 0x058E, 0x05AE, 0x05AE, 0x05AD, 0x05AD,
                              0x05AD, 0x05AC, 0x05AC, 0x05AB, 0x05CB, 0x05CB, 0x05CA, 0x05CA, 0x05CA, 0x05C9,
                              0x05C9, 0x05C8, 0x05E8, 0x05E8, 0x05E7, 0x05E7, 0x05E6, 0x05E6, 0x05E6, 0x05E5,
                              0x05E5, 0x0604, 0x0604, 0x0604, 0x0603, 0x0603, 0x0602, 0x0602, 0x0601, 0x0621,
                              0x0621, 0x0620, 0x0620, 0x0620, 0x0620, 0x0E20, 0x0E20, 0x0E40, 0x1640, 0x1640,
                              0x1E40, 0x1E40, 0x2640, 0x2640, 0x2E40, 0x2E60, 0x3660, 0x3660, 0x3E60, 0x3E60,
                              0x3E60, 0x4660, 0x4660, 0x4E60, 0x4E80, 0x5680, 0x5680, 0x5E80, 0x5E80, 0x6680,
                              0x6680, 0x6E80, 0x6EA0, 0x76A0, 0x76A0, 0x7EA0, 0x7EA0, 0x86A0, 0x86A0, 0x8EA0,
                              0x8EC0, 0x96C0, 0x96C0, 0x9EC0, 0x9EC0, 0xA6C0, 0xAEC0, 0xAEC0, 0xB6E0, 0xB6E0,
                              0xBEE0, 0xBEE0, 0xC6E0, 0xC6E0, 0xCEE0, 0xCEE0, 0xD6E0, 0xD700, 0xDF00, 0xDEE0,
                              0xDEC0, 0xDEA0, 0xDE80, 0xDE80, 0xE660, 0xE640, 0xE620, 0xE600, 0xE5E0, 0xE5C0,
                              0xE5A0, 0xE580, 0xE560, 0xE540, 0xE520, 0xE500, 0xE4E0, 0xE4C0, 0xE4A0, 0xE480,
                              0xE460, 0xEC40, 0xEC20, 0xEC00, 0xEBE0, 0xEBC0, 0xEBA0, 0xEB80, 0xEB60, 0xEB40,
                              0xEB20, 0xEB00, 0xEAE0, 0xEAC0, 0xEAA0, 0xEA80, 0xEA60, 0xEA40, 0xF220, 0xF200,
                              0xF1E0, 0xF1C0, 0xF1A0, 0xF180, 0xF160, 0xF140, 0xF100, 0xF0E0, 0xF0C0, 0xF0A0,
                              0xF080, 0xF060, 0xF040, 0xF020, 0xF800,
                             };

XPT2046_Touchscreen ts(CS_PIN, TIRQ_PIN);  // Param 2 - Touch IRQ Pin - interrupt enabled polling

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

Adafruit_AMG88xx amg;

//float pixels[AMG88xx_PIXEL_ARRAY_SIZE];
uint16_t displayPixelWidth, displayPixelHeight;

#define AMG_COLS 8
#define AMG_ROWS 8
float pixels[AMG_COLS * AMG_ROWS];

#define INTERPOLATED_COLS 24
#define INTERPOLATED_ROWS 24


float get_point(float *p, uint8_t rows, uint8_t cols, int8_t x, int8_t y);
void set_point(float *p, uint8_t rows, uint8_t cols, int8_t x, int8_t y, float f);
void get_adjacents_1d(float *src, float *dest, uint8_t rows, uint8_t cols, int8_t x, int8_t y);
void get_adjacents_2d(float *src, float *dest, uint8_t rows, uint8_t cols, int8_t x, int8_t y);
float cubicInterpolate(float p[], float x);
float bicubicInterpolate(float p[], float x, float y);
void interpolate_image(float *src, uint8_t src_rows, uint8_t src_cols,
                       float *dest, uint8_t dest_rows, uint8_t dest_cols);
int xx;
int yy;
int zz;

void setup() {
  Serial.begin(115200);

  ts.begin();
  while (!Serial && (millis() <= 1000));

  tft.begin();
  tft.setRotation(3); // I have to retate the screen because of my mistakes on case modeling
  tft.fillScreen(ILI9341_BLACK);
  displayPixelWidth = tft.width() / 8;
  displayPixelHeight = tft.height() / 8;
  Serial.println(F("AMG88xx pixels"));

  bool status;
  // default settings
  status = amg.begin();
  if (!status) {
    Serial.println("Could not find a valid AMG88xx sensor, check wiring!");
    while (1);
  }

  Serial.println("-- Hi --");
  Serial.println();
  tft.setFont(&FreeSans9pt7b);
  tft.setTextSize(1);
  tft.drawRect(246, 3, 72, 120, tft.color565(255, 255, 255));//highest lowest window
  tft.drawRect(246, 130, 72, 35, tft.color565(255, 255, 255));//range
  tft.drawRect(246, 170, 72, 62, tft.color565(255, 255, 255));//updown
  tft.setCursor(250, 20);
  tft.println("H.");
  tft.setCursor(250, 50);
  tft.println("L.");
  tft.setCursor(250, 80);
  tft.println("A.");
  tft.setCursor(250, 110);
  tft.println("D.");
  delay(100); // let sensor boot up
  // draw the user interface to display highest, lowest, average and the temperture differential of two peak temp.
}

void loop(void) {
  getxyz(); // get the position of your touch
  drawAMG();  //draw the heat map on TFT
  tftPrintKeyTemp(); // display the three 4 key temperture value
  resetset(); // reset the autoset, see below 
  autoset(); // auto set the heat map temp. range based on the highest and lowest temp. that senser detected
  tftRange(); // display heat map temp. range 
  UpDownB();  // the plus and minus button that change temp. range
  Cleanxyz(); // set the position value (xyz) to zero, see UpDownB()
  // the temp. range in here does not affect the temp. range that the senser can dectect
}

void getxyz() {
  if (ts.touched()) {
    TS_Point p = ts.getPoint();
    Serial.print("Pressure = ");
    Serial.print(p.z);
    Serial.print(", x = ");
    Serial.print(p.x);
    Serial.print(", y = ");
    Serial.print(p.y);

    xx = p.x;
    yy = p.y;
    zz = p.z;
    Serial.println();

  }
}

void drawAMG(void) { //slower but runding
  //read all the pixels
  amg.readPixels(pixels);
  float dest_2d[INTERPOLATED_ROWS * INTERPOLATED_COLS];

  int32_t t = millis();
  interpolate_image(pixels, AMG_ROWS, AMG_COLS, dest_2d, INTERPOLATED_ROWS, INTERPOLATED_COLS);

  uint16_t boxsize = min(tft.width() / INTERPOLATED_COLS, tft.height() / INTERPOLATED_COLS);

  drawpixels(dest_2d, INTERPOLATED_ROWS, INTERPOLATED_COLS, boxsize, boxsize, false);
}

void drawpixels(float *p, uint8_t rows, uint8_t cols, uint8_t boxWidth, uint8_t boxHeight, boolean showVal) {
  int colorTemp;
  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols; x++) {
      float val = get_point(p, rows, cols, x, y);
      if (val >= MAXTEMP) colorTemp = MAXTEMP;
      else if (val <= MINTEMP) colorTemp = MINTEMP;
      else colorTemp = val;

      uint8_t colorIndex = map(colorTemp, MINTEMP, MAXTEMP, 0, 255);
      colorIndex = constrain(colorIndex, 0, 255);
      //draw the pixels!
      uint16_t color;
      color = val * 2;
      tft.fillRect(0 + boxWidth * x, boxHeight * y, boxWidth, boxHeight, camColors[colorIndex]);

      if (showVal) {
        tft.setCursor(300, 220);
        tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(1);
        tft.print(val, 1);
      }
    }
  }
}

float largest(float pixels[AMG_COLS * AMG_ROWS], int n)
{
  int i;
  // Initialize maximum element
  float max = pixels[0];

  // Traverse array elements from second and
  // compare every element with current max
  for (i = 0; i < n; i++)
    if (pixels[i] > max)
      max = pixels[i];
  return max;
}

float lowest(float pixels[AMG_COLS * AMG_ROWS], int n)
{
  int i;
  // Initialize maximum element
  float mini = pixels[0];

  // Traverse array elements from second and
  // compare every element with current max
  for (i = 0; i < n; i++)
    if (pixels[i] < mini)
      mini = pixels[i];
  return mini;
}

float sum(float pixels[AMG_COLS * AMG_ROWS], int n)
{
  int i;
  float sumnum = 0;
  // Initialize maximum element
  for (i = 0; i < n; i++) {
    sumnum = sumnum + pixels[i];
  }
  return sumnum;
}

void tftPrintKeyTemp (void) {
  tft.fillRect(268, 5, 48, 110, ILI9341_BLACK);

  float average = sum(pixels, 64) / 64;
  float diff = largest(pixels, 63) - lowest(pixels, 63);

  tft.setCursor(268, 20);
  tft.println(largest(pixels, 63));
  tft.setCursor(268, 50);
  tft.println(lowest(pixels, 63));
  tft.setCursor(268, 80);
  tft.println(average);
  tft.setCursor(268, 110);
  tft.println(diff);
}

void tftRange (void) {
  tft.fillRect(248, 132, 65, 30, ILI9341_BLACK);//range
  tft.setCursor(250, 153);
  tft.println(MINTEMP);
  tft.setCursor(290, 153);
  tft.println(MAXTEMP);
}


void UpDownB(void) {
  tft.setFont(&FreeMonoBold12pt7b);

  tft.setCursor(256, 195);
  tft.println("+");
  tft.setCursor(295, 195);
  tft.println("+");
  tft.setCursor(256, 222);
  tft.println("-");
  tft.setCursor(295, 222);
  tft.println("-");

  tft.setFont(&FreeSans9pt7b);

  if (xx > 609 && xx < 906) {
    if (yy > 2830 && yy < 3295)
    {
      Serial.println("up1");
      mintemp++ ;
    }
  }

  if (xx > 260 && xx < 474) {
    if (yy > 2926 && yy < 3279)
    {
      Serial.println("up2");
      maxtemp++ ;
    }
  }

  if (xx > 627 && xx < 915) {
    if (yy > 3358 && yy < 3642)
    {
      Serial.println("down1");
      mintemp -- ;
    }
  }

  if (xx > 216 && xx < 474) {
    if (yy > 3320 && yy < 3636)
    {
      Serial.println("down2");
      maxtemp--;
    }
  }
}

void Cleanxyz(void) {
  xx = 0;
  yy = 0;
}
void autoset(void) {
  if (xx > 1260 && xx < 3560) {
    if (yy > 449 && yy < 1844)
    {
      Serial.println("autoset");
      mintemp = lowest(pixels, 63);
      maxtemp = largest(pixels, 63);
    }
  }
}


void resetset(void) {
  if (xx > 1196 && xx < 3544) {
    if (yy > 2341 && yy < 3544)
    {
      Serial.println("resetset");
      mintemp = 18;
      maxtemp = 28;
    }
  }
}