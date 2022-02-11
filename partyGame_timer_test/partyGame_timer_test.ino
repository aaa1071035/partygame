#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPLynq2zlkF"
#define BLYNK_DEVICE_NAME "partyGame"
/* Fill-in your Template ID (only if using Blynk.Cloud) */
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#include <FastLED.h>


//----------------------------------------------------------------
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YwfrQW-sPUIoYwtOWsk4mtLeCYF83ebt";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Barababa";
char pass[] = "c8763946";
//---------------------------------------------------------------

#define PNUM_LED 8
#define NUM_LED 16
#define p1_btn 34     //玩家1按鈕  (GREEN)
#define p2_btn 5        //玩家2按鈕  (BLUE)
#define p1_LED 18       //玩家1燈條
#define p2_LED 19       //玩家2燈條
#define boss_LED 21     //燈圈
#define music_1 13      //音樂1
#define music_2 32      //音樂2
#define music_3 14      //音樂3
#define music_4 27
#define music_5 26
#define music_6 25
#define music_P 4
#define spray_1 15      //噴霧1
#define spray_2 2       //噴霧2

byte p1_btnState;
byte p2_btnState;
int gameMode = 0; //遊戲模式設定
long int  timer;
byte bright;
byte bossMode;
byte stopp=0;
//RGB------
byte ledModeA = 0;
byte ledModeB = 0;
byte ledModeC = 0;
byte ledMode = 0;
byte rboss = 255;
byte gboss = 255;
byte bboss = 255;
byte r1 = 255;
byte g1 = 255;
byte b1 = 255;
byte r2 = 255;
byte g2 = 255;
byte b2 = 255;
//----------
byte v0; //Blynk Ｖ0腳位讀取
byte v1; //Blynk V1腳位讀取
byte v2; //Blynk V2腳位讀取
byte v3; //Blynk V3腳位讀取
byte v4; //Blynk V4腳位讀取
byte maxBright = 50; //Blynk V5拉條讀取


CRGB p1_leds[NUM_LED];
CRGB p2_leds[NUM_LED];
CRGB boss_leds[NUM_LED];

void getBTN() {          //取得按鈕狀態
  p1_btnState = digitalRead(p1_btn);
  p2_btnState = digitalRead(p2_btn);
}

void ledDefault() {    //燈圈燈條初始狀態
  for (byte i = 0; i < 16; i++)
  {
    p1_leds[i] = CRGB::Black;
    p2_leds[i] = CRGB::Black;
    boss_leds[i] = CRGB::Black;
    FastLED.show();
  }
}
void scanVirtualPin() //掃描BLYNK選擇關卡狀況
{
  blynkRun();
  FastLED.setBrightness(maxBright);
  if (v0 == 1)
  {
    gameMode = 1;
  }
  if (v1 == 1)
  {
    gameMode = 2;
  }
  if (v2 == 1)
  {
    gameMode = 3;
  }
  if (v3 == 1)
  {
    gameMode = 4;
  }
  if (v0 == 0 && v1 == 0 && v2 == 0 && v3 == 0)
  {
    gameMode = 0;
  }
  delay(50);
}

void blynkRun()  //執行BLYNK
{
  Blynk.run();
}

void Pause()  //暫停
{
  while (v4 == 1)  //如果暫停按住
  {
    defaultMusic();
    Serial.println("PAUSE");
    scanVirtualPin();
    if (v4 == 0) break;
  }
}

void scanGraph()
{
  Blynk.virtualWrite(V6, gameMode); //關卡統計圖
  if (gameMode == 3)
    Blynk.virtualWrite(V7, bossMode); //魔王統計圖
}

void defaultMusic()
{
  digitalWrite(music_P, 0);
  digitalWrite(music_1, 1);
  digitalWrite(music_2, 1);
  digitalWrite(music_3, 1);
  digitalWrite(music_4, 1);
  digitalWrite(music_5, 1);
  digitalWrite(music_6, 1);
}

void playMusic1()
{
  digitalWrite(music_P, 1);
  digitalWrite(music_1, 0);
  digitalWrite(music_2, 1);
  digitalWrite(music_3, 1);
  digitalWrite(music_4, 1);
  digitalWrite(music_5, 1);
  digitalWrite(music_6, 1);
}

void playMusic2()
{
  digitalWrite(music_P, 1);
  digitalWrite(music_1, 1);
  digitalWrite(music_2, 0);
  digitalWrite(music_3, 1);
  digitalWrite(music_4, 1);
  digitalWrite(music_5, 1);
  digitalWrite(music_6, 1);
}

void playMusic3()
{
  digitalWrite(music_P, 1);
  digitalWrite(music_1, 0);
  digitalWrite(music_2, 0);
  digitalWrite(music_3, 1);
  digitalWrite(music_4, 1);
  digitalWrite(music_5, 1);
  digitalWrite(music_6, 1);
}

void playMusic4()
{
  digitalWrite(music_P, 1);
  digitalWrite(music_1, 1);
  digitalWrite(music_2, 1);
  digitalWrite(music_3, 0);
  digitalWrite(music_4, 1);
  digitalWrite(music_5, 1);
  digitalWrite(music_6, 1);
}

void playMusic5()
{
  digitalWrite(music_P, 1);
  digitalWrite(music_1, 0);
  digitalWrite(music_2, 1);
  digitalWrite(music_3, 0);
  digitalWrite(music_4, 1);
  digitalWrite(music_5, 1);
  digitalWrite(music_6, 1);
}

void playMusic6()
{
  digitalWrite(music_P, 1);
  digitalWrite(music_1, 1);
  digitalWrite(music_2, 0);
  digitalWrite(music_3, 0);
  digitalWrite(music_4, 1);
  digitalWrite(music_5, 1);
  digitalWrite(music_6, 1);
}
//-----------Blynk腳位宣告---------------
BLYNK_WRITE(V0)   //反應模式
{
  v0 = param.asInt();
  if (v0 == 1) {
    v1 = 0;
    v2 = 0;
    v3 = 0;
  }
  Blynk.virtualWrite(V1, 0);
  Blynk.virtualWrite(V2, 0);
  Blynk.virtualWrite(V3, 0);
}
BLYNK_WRITE(V1)    //節奏模式
{
  v1 = param.asInt();
  if (v1 == 1) {
    v0 = 0;
    v2 = 0;
    v3 = 0;
  }
  Blynk.virtualWrite(V0, 0);
  Blynk.virtualWrite(V2, 0);
  Blynk.virtualWrite(V3, 0);
}
BLYNK_WRITE(V2)    //魔王模式
{
  v2 = param.asInt();
  if (v2 == 1) {
    v1 = 0;
    v0 = 0;
    v3 = 0;
  }
  Blynk.virtualWrite(V1, 0);
  Blynk.virtualWrite(V0, 0);
  Blynk.virtualWrite(V3, 0);
}
BLYNK_WRITE(V3)    //競速模式
{
  v3 = param.asInt();
  if (v3 == 1) {
    v1 = 0;
    v2 = 0;
    v0 = 0;
  }
  Blynk.virtualWrite(V1, 0);
  Blynk.virtualWrite(V2, 0);
  Blynk.virtualWrite(V0, 0);
}
BLYNK_WRITE(V4)  //暫停
{
  v4 = param.asInt();
}
BLYNK_WRITE(V5)
{
  maxBright = param.asInt();
  Serial.println(maxBright);
}
BLYNK_WRITE(V9)
{
  rboss = param.asInt();
}
BLYNK_WRITE(V10)
{
  gboss = param.asInt();
}
BLYNK_WRITE(V11)
{
  bboss = param.asInt();
}
BLYNK_WRITE(V12)
{
  r1 = param.asInt();
}
BLYNK_WRITE(V13)
{
  g1 = param.asInt();
}
BLYNK_WRITE(V14)
{
  b1 = param.asInt();
}
BLYNK_WRITE(V15)
{
  r2 = param.asInt();
}
BLYNK_WRITE(V16)
{
  g2 = param.asInt();
}
BLYNK_WRITE(V17)
{
  b2 = param.asInt();
}
BLYNK_WRITE(V18)
{
  ledModeA = param.asInt();
  if (ledModeA == 1)
  {
    Serial.println("1");
    ledMode = 1;
    ledModeB = 0;
    ledModeC = 0;
    Blynk.virtualWrite(V19, 0);
    Blynk.virtualWrite(V20, 0);
  }
}
BLYNK_WRITE(V19)
{
  ledModeB = param.asInt();
  if (ledModeB == 1)
  {
    Serial.println("2");
    ledMode = 2;
    ledModeA = 0;
    ledModeC = 0;
    Blynk.virtualWrite(V18, 0);
    Blynk.virtualWrite(V20, 0);
  }
}
BLYNK_WRITE(V20)
{
  ledModeC = param.asInt();
  if (ledModeC == 1)
  {
    Serial.println("3");
    ledMode = 3;
    ledModeB = 0;
    ledModeA = 0;
    Blynk.virtualWrite(V19, 0);
    Blynk.virtualWrite(V18, 0);
  }
}
//-------------------------------------

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, p1_LED>(p1_leds, PNUM_LED);
  FastLED.addLeds<NEOPIXEL, p2_LED>(p2_leds, PNUM_LED);
  FastLED.addLeds<NEOPIXEL, boss_LED>(boss_leds, NUM_LED);
  FastLED.setBrightness(50);
  for (byte a = 0; a < 16; a++)
  {
    boss_leds[a] = CRGB::Red;
    FastLED.show();
  }

  pinMode(p1_btn, INPUT);
  pinMode(p2_btn, INPUT);
  pinMode(music_1, OUTPUT);
  pinMode(music_2, OUTPUT);
  pinMode(music_3, OUTPUT);
  pinMode(music_4, OUTPUT);
  pinMode(music_5, OUTPUT);
  pinMode(music_6, OUTPUT);
  pinMode(music_P, OUTPUT);
  pinMode(spray_1, OUTPUT);
  pinMode(spray_2, OUTPUT);
  defaultMusic();
  Blynk.begin(auth, ssid, pass); //Blynk連結
  bright = 0;

}

void loop() {
  if ( stopp != 1)
  {
    defaultMusic();
    delay(100);
    Blynk.virtualWrite(V0, 0);
    Blynk.virtualWrite(V1, 0);
    Blynk.virtualWrite(V2, 0);
    Blynk.virtualWrite(V3, 0);
    Blynk.virtualWrite(V4, 0);
    Blynk.virtualWrite(V5, 0);
    Blynk.virtualWrite(V9, 255); rboss = 255;
    Blynk.virtualWrite(V10, 255); gboss = 255;
    Blynk.virtualWrite(V11, 255); bboss = 255;
    Blynk.virtualWrite(V12, 255); r1 = 255;
    Blynk.virtualWrite(V13, 255); g1 = 255;
    Blynk.virtualWrite(V14, 255); b1 = 255;
    Blynk.virtualWrite(V15, 255); r2 = 255;
    Blynk.virtualWrite(V16, 255); g2 = 255;
    Blynk.virtualWrite(V17, 255); b2 = 255;
    Blynk.virtualWrite(V18, 0);
    Blynk.virtualWrite(V19, 0);
    Blynk.virtualWrite(V20, 0);
    playMusic2();
    stopp = 1;
  }
  //--------------模式選擇-----------------
  while (gameMode == 0)
  {
    byte scan = 0;
    defaultMusic();
    Blynk.run();
    if (ledModeA == 0 && ledModeB == 0 && ledModeC == 0) ledMode = 0;
    if (v0 == 1 || v1 == 1 || v2 == 1 || v3 == 1)
    {
      break;
    }
    //-------------------呼吸燈-------------------------
    FastLED.setBrightness(maxBright);
    double star = maxBright / 1.2;
    switch (ledMode)
    {
      case 0:
        {
          FastLED.setBrightness(bright);
          for (byte i = 0; i < 16; i++) {
            fill_rainbow(boss_leds, 16, 0, 16);
            fill_rainbow(p1_leds, 8, 0, 32);
            fill_rainbow(p2_leds, 8, 0, 32);
            FastLED.show();
          }
          if (scan == 0)
          {
            bright++;
            if (bright == maxBright || bright == 255)
              scan = 1;
          }
          if (scan == 1)
          {
            bright--;
            if (bright == 0)
              scan = 0;
          }
          break;
        }
      case 1:     //呼吸燈
        {
          FastLED.setBrightness(bright);
          for (byte i = 0; i < NUM_LED; i++)
          {
            boss_leds[i] = CRGB(rboss, gboss, bboss);
            FastLED.show();
          }
          for (byte j = 0; j < PNUM_LED; j++)
          {
            p1_leds[j] = CRGB(r1, g1, b1);
            p2_leds[j] = CRGB(r2, g2, b2);
            FastLED.show();
          }
          if (scan == 0)
          {
            bright++;
            if (bright == maxBright || bright == 255)
              scan = 1;
          }
          if (scan == 1)
          {
            bright--;
            if (bright == 0)
              scan = 0;
          }
          break;
        }
      case 2:    //跑馬燈
        {
          byte z = 0;
          for (byte i = 0; i < NUM_LED; i++)
          {
            boss_leds[i] = CRGB(rboss, gboss, bboss);
            if (i < 8)
            {
              p1_leds[i] = CRGB(r1, g1, b1);
              p2_leds[i] = CRGB(r2, g2, b2);
            }
            if (i >= 8)
            {
              byte a = 1 + 2 * z;
              p1_leds[i - a] = CRGB(r1, g1, b1);
              p2_leds[i - a] = CRGB(r2, g2, b2);
              z++;
            }
            FastLED.show();
            delay(100);
            FastLED.clear();
          }
          break;
        }
      case 3:    //流星燈
        {
          FastLED.clear();
          byte z = 0;
          for (byte i = 0; i < NUM_LED; i++)
          {
            boss_leds[i] = CRGB(rboss, gboss, bboss);
            if (i < 8)
            {
              p1_leds[i] = CRGB(r1, g1, b1);
              p2_leds[i] = CRGB(r2, g2, b2);
              fadeToBlackBy(p1_leds, i, star);
              fadeToBlackBy(p2_leds, i, star);
            }
            if (i >= 8)
            {
              byte a = 1 + 2 * z;
              p1_leds[i - a] = CRGB(r1, g1, b1);
              p2_leds[i - a] = CRGB(r2, g2, b2);
              fadeToBlackBy(p1_leds, i - a, star);
              fadeToBlackBy(p2_leds, i - a, star);
              z++;
            }
            FastLED.show();
            fadeToBlackBy(boss_leds, i, star);
            delay(100);
          }
          break;
        }
    }
  }
  FastLED.setBrightness(maxBright);
  defaultMusic();
  //--------------------------------------
  // put your main code here, to run repeatedly:
  switch (gameMode) //遊戲模式
  {
    //--------------------------反應模式------------------------------------
    case 1:  //反應模式
      {
        playMusic1();
        scanVirtualPin();
        ledDefault();
        delay(500);
        byte ran = 0;
        byte start = 0;
        while (ran != 5)    //若亂數＝＝5開始競速
        {
          if (start == 2)
          {
            digitalWrite(music_1, 1);
            delay(100);
            playMusic1(); //播放第一首歌
            start = 0;
          }
          delay(100);
          start++;
          getBTN();
          scanVirtualPin();
          if (v4 == 1) Pause(); //暫停
          if (v1 == 1 || v2 == 1 || v3 == 1 || v0 == 0) break;
          for (byte i = 0; i < NUM_LED; i++) //黃燈等待
          {
            boss_leds[i] = CRGB(rboss, gboss, bboss);
            p1_leds[i] = CRGB(r1, g1, b1);
            p2_leds[i] = CRGB(r2, g2, b2);
            FastLED.show();
          }
          ran = random(0, 10);

          if (p1_btnState == 1 || p2_btnState == 1) //按住按鈕暫停遊戲
          {
            scanVirtualPin();
            if (v4 == 1) Pause(); //暫停
            if (v1 == 1 || v2 == 1 || v3 == 1 || v0 == 0) break;
            ran = 2;
          }
          delay(500);
          Serial.print(ran);
        }
        Serial.println("test");
        if (ran == 5)
        {
          Serial.println("aaaaaa");
          playMusic2(); //播放第二首歌
          Blynk.run();
          if (v4 == 1) Pause(); //暫停
          if (v1 == 1 || v2 == 1 || v3 == 1 || v0 == 0) break; //模式更換
          for (byte whiteLED = 0; whiteLED < NUM_LED; whiteLED++) //紅燈反應開始
          {
            boss_leds[whiteLED] = CRGB::Red;
            p1_leds[whiteLED] = CRGB::Red;
            p2_leds[whiteLED] = CRGB::Red;
            FastLED.show();
          }
          for (;;)
          {
            getBTN();
            scanVirtualPin();
            if (v4 == 1) Pause(); //暫停
            if (v1 == 1 || v2 == 1 || v3 == 1 || v0 == 0) break; //模式更換
            if (p1_btnState == 1)  //若玩家1先按則亮綠燈 玩家1獲勝
            {
              //-------------------------------------------------------------------------------------------------------------------------------------------------噴霧
              for (byte i = 0; i < NUM_LED; i++)
              {
                boss_leds[i] = CRGB(r1, g1, b1);
                p1_leds[i] = CRGB(r1, g1, b1);
                p2_leds[i] = CRGB(r1, g1, b1);
                FastLED.show();
              }
              delay(1000);
              break;
            }
            if (p2_btnState == 1) //若玩家2先按則亮藍燈 玩家2獲勝
            {
              //-------------------------------------------------------------------------------------------------------------------------------------------------噴霧
              for (byte i = 0; i < NUM_LED; i++)
              {
                boss_leds[i] = CRGB(r2, g2, b2);
                p1_leds[i] = CRGB(r2, g2, b2);
                p2_leds[i] = CRGB(r2, g2, b2);
                FastLED.show();
              }
              delay(1000);
              break;
            }
          }
        }
        break;
      }
    //------------------------------------------------------------------

    //--------------------------節奏模式------------------------------------
    case 2: //節奏模式
      {
        byte mode2_level = 1;
        byte r = 1;
        byte g = 1;
        byte b = 1;
        playMusic3();
        scanVirtualPin();
        if (v4 == 1) Pause(); //暫停
        if (v0 == 1 || v2 == 1 || v3 == 1 || v1 == 0) break; //模式更換
        ledDefault();
        delay(500);

        while (mode2_level != 0)
        {
          scanVirtualPin();
          if (v4 == 1) Pause(); //暫停
          if (v0 == 1 || v2 == 1 || v3 == 1 || v1 == 0) break; //模式更換
          switch (mode2_level) {
            case 1:   //lv1
              {
                //-------------------------------------------------------------------------MUSIC
                Serial.println("LV1");
                if (v4 == 1) Pause(); //暫停
                if (v0 == 1 || v2 == 1 || v3 == 1 || v1 == 0) break; //模式更換
                //-------------------------LEVEL 1----------------------------------
                for (byte whiteLED = 0; whiteLED < NUM_LED; whiteLED++)
                {
                  getBTN();
                  scanVirtualPin();
                  if (v4 == 1) Pause(); //暫停
                  if (v0 == 1 || v2 == 1 || v3 == 1 || v1 == 0) break; //模式更換
                  boss_leds[0] = CRGB(rboss, gboss, bboss);
                  FastLED.show();
                  //判斷
                  if (p1_btnState == 1) {
                    for (;;) //按住按鈕暫停遊戲
                    {
                      getBTN();
                      scanVirtualPin();
                      if (v4 == 1) Pause(); //暫停
                      if (v0 == 1 || v2 == 1 || v3 == 1 || v1 == 0) break; //模式更換
                      if (whiteLED == 0) //中了
                      {

                        for (byte i = 0; i < NUM_LED; i++)
                        {
                          boss_leds[i] = CRGB::Green;
                          FastLED.show();

                          if (p1_btnState == 0) {    //中了閃七彩霓虹燈進入LEVEL 2
                            playMusic2();
                            for (byte goal = 0; goal < NUM_LED; goal++)
                            {
                              byte r = random(0, 255);
                              byte g = random(0, 255);
                              byte b = random(0, 255);
                              boss_leds[goal] = CRGB(r, g, b);
                              FastLED.show();
                            }
                            delay(100);
                            boss_leds[i] = CRGB::Black;
                            mode2_level = 2;
                            whiteLED = 16;
                          }
                        }
                      }

                      else//沒中
                      {
                        playMusic4();
                        for (byte i = 0; i < NUM_LED; i++)
                        {
                          boss_leds[i] = CRGB::Red;
                          FastLED.show();
                        }
                      }
                      if (p1_btnState == 0) //放開按鈕繼續遊戲
                      {
                        for (byte i = 0; i < NUM_LED; i++)
                        {
                          boss_leds[i] = CRGB::Black;
                        }
                        break;
                      }
                    }
                  }
                  if (whiteLED == 15)
                  {
                    playMusic3(); //播放音樂3
                  }
                  boss_leds[whiteLED] = CRGB::White;
                  FastLED.show();
                  delay(100);
                  defaultMusic();
                  boss_leds[whiteLED] = CRGB::Black;
                }
                //------------------------------------------------------------------
                break;
              }
            case 2:  //lv2
              {
                //-------------------------------------------------------------------------------------------------------------------------------------------------MUSIC
                Serial.println("LV2");
                //--------------------------LEVEL 2---------------------------------
                for (byte whiteLED = 0; whiteLED < NUM_LED; whiteLED++)
                {
                  getBTN();
                  scanVirtualPin();
                  if (v4 == 1) Pause(); //暫停
                  if (v0 == 1 || v2 == 1 || v3 == 1 || v1 == 0) break; //模式更換
                  boss_leds[0] = CRGB(rboss, gboss, bboss);
                  FastLED.show();
                  //判斷
                  if (p1_btnState == 1) {
                    for (;;) //按住按鈕暫停遊戲
                    {
                      getBTN();
                      scanVirtualPin();
                      if (v4 == 1) Pause(); //暫停
                      if (v0 == 1 || v2 == 1 || v3 == 1 || v1 == 0) break; //模式更換
                      if (whiteLED == 0) //中了
                      {
                        for (byte i = 0; i < NUM_LED; i++)
                        {

                          boss_leds[i] = CRGB::Green;
                          FastLED.show();

                          if (p1_btnState == 0) {    //中了閃七彩霓虹燈進入LEVEL 3
                            playMusic2();
                            for (byte goal = 0; goal < NUM_LED; goal++)
                            {

                              r = random(0, 255);
                              g = random(0, 255);
                              b = random(0, 255);
                              boss_leds[goal] = CRGB(r, g, b);
                              FastLED.show();
                            }
                            delay(100);
                            boss_leds[i] = CRGB::Black;
                            mode2_level = 3;
                            whiteLED = 16;
                          }
                        }
                      }

                      else//沒中
                      {
                        playMusic4();
                        for (byte i = 0; i < NUM_LED; i++)
                        {
                          boss_leds[i] = CRGB::Red;
                          FastLED.show();
                          mode2_level = 1;
                          whiteLED = 16;
                        }
                      }
                      if (p1_btnState == 0) //放開按鈕繼續遊戲
                      {
                        for (byte i = 0; i < NUM_LED; i++)
                        {
                          boss_leds[i] = CRGB::Black;
                        }
                        break;
                      }
                    }
                  }
                  if (whiteLED == 15)
                  {
                    Serial.println("tt");
                    playMusic3(); //播放音樂3
                  }
                  boss_leds[whiteLED] = CRGB::White;
                  FastLED.show();
                  delay(50);
                  defaultMusic();
                  boss_leds[whiteLED] = CRGB::Black;
                }
                //------------------------------------------------------------------
                break;
              }
            case 3:  //lv3
              {
                //-------------------------------------------------------------------------------------------------------------------------------------------------MUSIC
                Serial.println("LV3");
                //--------------------------LEVEL 3---------------------------------
                for (byte whiteLED = 0; whiteLED < NUM_LED; whiteLED++)
                {
                  getBTN();
                  scanVirtualPin();
                  if (v4 == 1) Pause(); //暫停
                  if (v0 == 1 || v2 == 1 || v3 == 1 || v1 == 0) break; //模式更換
                  boss_leds[0] = CRGB(rboss, gboss, bboss);
                  FastLED.show();
                  //判斷
                  if (p1_btnState == 1) {
                    for (;;) //按住按鈕暫停遊戲
                    {
                      scanVirtualPin();
                      if (v4 == 1) Pause(); //暫停
                      if (v0 == 1 || v2 == 1 || v3 == 1 || v1 == 0) break; //模式更換
                      getBTN();
                      if (whiteLED == 0) //中了
                      {
                        for (byte i = 0; i < NUM_LED; i++)
                        {

                          boss_leds[i] = CRGB::Green;
                          FastLED.show();

                          if (p1_btnState == 0) {    //中了閃七彩霓虹燈進入LEVEL 4
                            playMusic2();
                            for (byte goal = 0; goal < NUM_LED; goal++)
                            {
                              r = random(0, 255);
                              g = random(0, 255);
                              b = random(0, 255);
                              boss_leds[goal] = CRGB(r, g, b);
                              FastLED.show();
                            }
                            delay(100);
                            boss_leds[i] = CRGB::Black;
                            mode2_level = 4;
                            whiteLED = 16;
                          }
                        }
                      }
                      else//沒中
                      {
                        playMusic4();
                        for (byte i = 0; i < NUM_LED; i++)
                        {
                          boss_leds[i] = CRGB::Red;
                          FastLED.show();
                          mode2_level = 2;
                          whiteLED = 16;
                        }
                      }
                      if (p1_btnState == 0) //放開按鈕繼續遊戲
                      {
                        for (byte i = 0; i < NUM_LED; i++)
                        {
                          boss_leds[i] = CRGB::Black;
                        }
                        scanVirtualPin();
                        break;
                      }
                    }
                  }
                  if (whiteLED == 15)
                  {
                    playMusic3(); //播放音樂3
                  }
                  if ((whiteLED - 8) < 0)
                  {
                    boss_leds[whiteLED] = CRGB::White;
                    FastLED.show();
                    delay(100);
                    defaultMusic();
                    boss_leds[whiteLED] = CRGB::Black;
                  }
                  else
                  {
                    delay(100);
                    defaultMusic();
                    boss_leds[whiteLED] = CRGB::Black;
                  }
                  scanVirtualPin();
                }
                //--------------------------------------------------------------------
                break;
              }
            case 4:  //lv4
              {
                scanVirtualPin();
                if (v4 == 1) Pause(); //暫停
                if (v0 == 1 || v2 == 1 || v3 == 1 || v1 == 0) break; //模式更換
                Serial.println("LV4");
                //--------------------------LEVEL 4---------------------------------
                byte ran = random(1, 15);
                for (byte whiteLED = 0; whiteLED < NUM_LED; whiteLED++)
                {
                  getBTN();
                  boss_leds[0] = CRGB(rboss, gboss, bboss);
                  FastLED.show();
                  //判斷
                  if (p1_btnState == 1) {
                    if (v4 == 1) Pause(); //暫停
                    if (v0 == 1 || v2 == 1 || v3 == 1 || v1 == 0) break; //模式更換
                    for (;;) //按住按鈕暫停遊戲
                    {
                      scanVirtualPin();
                      if (v4 == 1) Pause(); //暫停
                      if (v0 == 1 || v2 == 1 || v3 == 1 || v1 == 0) break; //模式更換
                      getBTN();
                      if (whiteLED == 0) //中了
                      {
                        for (byte i = 0; i < NUM_LED; i++)
                        {
                          boss_leds[i] = CRGB::Green;
                          FastLED.show();
                          if (p1_btnState == 0) {    //中了閃七彩霓虹燈進入LEVEL 4
                            playMusic2();
                            for (byte goal = 0; goal < NUM_LED; goal++)
                            {
                              r = random(0, 255);
                              g = random(0, 255);
                              b = random(0, 255);
                              boss_leds[goal] = CRGB(r, g, b);
                              FastLED.show();
                            }
                            delay(100);
                            boss_leds[i] = CRGB::Black;
                            mode2_level = 5;
                            whiteLED = 16;
                          }
                        }
                      }
                      else//沒中
                      {
                        playMusic4();
                        for (byte i = 0; i < NUM_LED; i++)
                        {
                          boss_leds[i] = CRGB::Red;
                          FastLED.show();
                          mode2_level = 3;
                          whiteLED = 16;
                        }
                      }
                      if (p1_btnState == 0) //放開按鈕繼續遊戲
                      {
                        for (byte i = 0; i < NUM_LED; i++)
                        {
                          boss_leds[i] = CRGB::Black;
                        }
                        break;
                      }
                    }
                  }
                  if (whiteLED == 15)
                  {
                    playMusic3(); //播放音樂3
                  }
                  if (whiteLED - ran < 0)
                  {
                    boss_leds[whiteLED] = CRGB::White;
                    FastLED.show();
                    delay(100);
                    defaultMusic();
                    boss_leds[whiteLED] = CRGB::Black;
                  }
                  else
                  {
                    delay(100);
                    defaultMusic();
                    boss_leds[whiteLED] = CRGB::Black;
                  }
                }
                //------------------------------------------------------------------
                break;
              }
            case 5: //game over
              {
                Serial.println("Complete");
                mode2_level = 0;
                break;
              }
          }
          if (gameMode == 5) break;
        }
        gameMode = 0;
        Blynk.virtualWrite(V1, 0);
        v1 = 0;
        scanVirtualPin();
        break;
      }
    //--------------------------------------------------------------------
    //--------------------------魔王模式------------------------------------
    case 3:
      {
        byte player1_HP;
        byte player2_HP;
        byte boss_HP;
        byte ran = 0;
        long int timermusic;
        playMusic5();
        timermusic = millis();
        scanVirtualPin();
        if (v4 == 1) Pause(); //暫停
        if (v0 == 1 || v1 == 1 || v3 == 1 || v2 == 0) break; //模式更換
        ledDefault();
        for (byte p1 = 0; p1 < 16; p1++) //玩家1初始血量
        {
          p1_leds[p1] = CRGB(r1, g1, b1);
          FastLED.show();
        }
        for (byte p2 = 0; p2 < 16; p2++) //玩家2初始血量
        {
          p2_leds[p2] = CRGB(r2, g2, b2);
          FastLED.show();
        }
        for (byte boss = 0; boss < 16; boss++) //boss初始血量
        {
          boss_leds[boss] = CRGB(rboss, gboss, bboss);
          FastLED.show();
        }
        getBTN();
        Serial.println(boss_HP);
        boss_HP = 160;    //初始boss血量
        player1_HP = 2;
        player2_HP = 2;
        while (boss_HP > 0)
        {
          if (millis() - timermusic >= 4000)
          {
            defaultMusic();
          }
          if (millis() - timermusic >= 6000) //每隔六秒播一次音樂
          {
            playMusic5();
            timermusic = millis();
          }
          scanVirtualPin();
          if (v4 == 1) Pause(); //暫停
          if (v0 == 1 || v1 == 1 || v3 == 1 || v2 == 0) break; //模式更換
          if (player1_HP <= 0 && player2_HP <= 0) //game over
          {
            playMusic4();
            break;
          }
          Serial.println("boss mode:");
          for (byte p1 = 15; p1 >= player1_HP * 8; p1--) //玩家1血量
          {
            p1_leds[p1] = CRGB::Black;
            FastLED.show();
          }
          for (byte p2 = 15; p2 >= player2_HP * 8; p2--) //玩家2血量
          {
            p2_leds[p2] = CRGB::Black;
            FastLED.show();
          }
          getBTN();

          bossMode = random(1, 4);
          delay(100);//boss隨機行動
          Serial.println(bossMode);
          scanVirtualPin();
          if (v4 == 1) Pause(); //暫停
          if (v0 == 1 || v1 == 1 || v3 == 1 || v2 == 0) break; //模式更換
          for (byte i = 0; i < NUM_LED; i++)
          {
            boss_leds[i] = CRGB::Black;
            FastLED.show();
          }
          switch (bossMode) //boss行動模式
          {
            //攻擊---------------------------------
            case 1:
              //-------------------------------------------------------------------------------------------------------------------------------------------------MUSIC
              timer = millis();
              while (millis() - timer <= 1000)
              {
                scanVirtualPin();
                if (v4 == 1) Pause(); //暫停
                if (v0 == 1 || v1 == 1 || v3 == 1 || v2 == 0) break; //模式更換
                for (byte boss = 0; boss < boss_HP / 10; boss++) //boss血量
                {
                  boss_leds[boss] = CRGB::Red;
                  FastLED.show();
                }
                if (millis() - timer >= 950)
                {
                  getBTN();
                  if (p1_btnState == 0) //玩家沒按按鈕被攻擊
                  {
                    player1_HP--;
                  }
                  if (p2_btnState == 0) //玩家沒按按鈕被攻擊
                  {
                    player2_HP--;
                  }
                  if (player1_HP == 255)
                  {
                    player1_HP = 0;
                  }
                  if (player2_HP == 255)
                  {
                    player2_HP = 0;
                  }
                  delay(40);
                }
              }
              break;

            //-------------------------------------
            //防禦---------------------------------
            case 2: //防禦
              //-------------------------------------------------------------------------------------------------------------------------------------------------MUSIC
              timer = millis();
              while (millis() - timer <= 2000)
              {
                for (byte boss = 0; boss < boss_HP / 10; boss++) //boss血量
                {
                  boss_leds[boss] = CRGB(rboss, gboss, bboss);
                  FastLED.show();
                }
                scanVirtualPin();
                if (v4 == 1) Pause(); //暫停
                if (v0 == 1 || v1 == 1 || v3 == 1 || v2 == 0) break; //模式更換
              }
              break;
            //------------------------------------
            //閒置-------------------------------------------------------
            case 3: //閒置
              //-------------------------------------------------------------------------------------------------------------------------------------------------MUSIC
              timer = millis();
              while (millis() - timer <= 3000)
              {
                for (byte boss = 0; boss < boss_HP / 10; boss++) //boss血量
                {
                  boss_leds[boss] = CRGB::Green;
                  FastLED.show();
                }
                scanVirtualPin();
                if (v4 == 1) Pause(); //暫停
                if (v0 == 1 || v1 == 1 || v3 == 1 || v2 == 0) break; //模式更換
                getBTN();
                Blynk.run();
                if (v4 == 1) Pause(); //暫停
                if (v0 == 1 || v1 == 1 || v3 == 1 || v2 == 0) break; //模式更換
                //玩家1攻擊-----------------------
                if (p1_btnState == 1 && player1_HP > 0)
                {
                  while (1) //按住按鈕暫停
                  {
                    Blynk.run();
                    if (v4 == 1) Pause(); //暫停
                    if (v0 == 1 || v1 == 1 || v3 == 1 || v2 == 0) break; //模式更換
                    delay(10);
                    getBTN();
                    if (p1_btnState == 0) //放開按鈕繼續
                    {
                      boss_HP--;
                      for (byte boss = 16; boss >= boss_HP / 10; boss--) //boss血量
                      {
                        boss_leds[boss] = CRGB::Black;
                        FastLED.show();
                      }
                      break;
                    }

                  }
                }
                //------------------------------
                //玩家2攻擊----------------------
                if (p2_btnState == 1 && player2_HP > 0)
                {
                  while (1) //按住按鈕暫停
                  {
                    Blynk.run();
                    if (v4 == 1) Pause(); //暫停
                    if (v0 == 1 || v1 == 1 || v3 == 1 || v2 == 0) break; //模式更換
                    delay(10);
                    getBTN();
                    if (p2_btnState == 0) //放開按鈕繼續
                    {
                      boss_HP--;
                      for (byte boss = 16; boss >= boss_HP / 10; boss--) //boss血量
                      {
                        boss_leds[boss] = CRGB::Black;
                        FastLED.show();
                      }
                      break;
                    }

                  }
                }
                //------------------------------
                if (boss_HP <= 0)
                {
                  playMusic2();
                  Serial.println("boss dead");
                  break;
                }

                delay(10);

                Blynk.run();
                if (v4 == 1) Pause(); //暫停
                if (v0 == 1 || v1 == 1 || v3 == 1 || v2 == 0) break; //模式更換
              }
              Serial.print("boss HP:");
              Serial.println(boss_HP);
              break;
          }
          scanVirtualPin();
          if (v4 == 1) Pause(); //暫停
          if (v0 == 1 || v1 == 1 || v3 == 1 || v2 == 0) break; //模式更換
          if (player1_HP <= 0 && player2_HP <= 0) //兩個玩家死亡
          {
            playMusic4();
            //-------------------------------------------------------------------------------------------------------------------------------------------------MUSIC
            for (byte i = 0; i < 2; i++)      //GameOver
            {
              //-------------------------------------------------------------------------------------------------------------------------------------------------噴霧
              for (byte die = 0; die < 16; die++)
              {
                p1_leds[die] = CRGB::Red;
                p2_leds[die] = CRGB::Red;
                FastLED.show();
              }
              delay(100);
              for (byte die = 0; die < 16; die++)
              {
                p1_leds[die] = CRGB::Black;
                p2_leds[die] = CRGB::Black;
              }
              scanVirtualPin();
              if (v4 == 1) Pause(); //暫停
              if (v0 == 1 || v1 == 1 || v3 == 1 || v2 == 0) break; //模式更換
            }
            Serial.println("Game Over");
            delay(5000);
            break;
          }
          scanVirtualPin();
          if (v4 == 1) Pause(); //暫停
          if (v0 == 1 || v1 == 1 || v3 == 1 || v2 == 0) break; //模式更換
        }
        break;
      }
    //-------------------------------------------------------
    //競速模式------------------------------------------------
    case 4:
      {
        byte p1_score;
        byte p2_score;
        Serial.println("-----------mode4 start---------");
        scanVirtualPin();
        playMusic6();
        timer = millis();
        if (v4 == 1) Pause(); //暫停
        if (v0 == 1 || v1 == 1 || v2 == 1 || v3 == 0) break; //模式更換
        ledDefault();
        delay(500);
        ledDefault();
        for (int i = 7; i <= 8; i++)
        {
          boss_leds[i] = CRGB::Red;
          FastLED.show();
        }
        p1_score = 0;  //初始分數
        p2_score = 0;
        while (p1_score < 100 && p2_score < 100) //當沒有任何玩家分數超過160
        {
          if (millis() - timer == 3000)
            defaultMusic();
          if (millis() - timer >= 6000) //每6秒播放一次
          {
            playMusic6();
            timer = millis();
          }
          Blynk.run();
          if (v4 == 1) Pause(); //暫停
          if (v0 == 1 || v1 == 1 || v2 == 1 || v3 == 0) break; //模式更換
          getBTN();
          if (p1_btnState == 1) //當玩家1按下按鈕
          {
            delay(30);
            getBTN();
            Blynk.run();
            if (v4 == 1) Pause(); //暫停
            if (v0 == 1 || v1 == 1 || v2 == 1 || v3 == 0) break; //模式更換
            if (p2_btnState == 1) //當玩家2按下按鈕
            {
              delay(30);
              getBTN();
              Blynk.run();
              if (v4 == 1) Pause(); //暫停
              if (v0 == 1 || v1 == 1 || v2 == 1 || v3 == 0) break; //模式更換
              if (p2_btnState == 0)  //放開按鈕玩家2得分
              {
                p2_score++;
                Serial.print("p2:");
                Serial.println(p2_score);
                Blynk.run();
                Blynk.run();
                if (v4 == 1) Pause(); //暫停
                if (v0 == 1 || v1 == 1 || v2 == 1 || v3 == 0) break; //模式更換
              }
            }
            if (p1_btnState == 0)  //放開按鈕玩家1得分
            {
              p1_score++;
              Serial.print("p1:");
              Serial.println(p1_score);
              Blynk.run();
              if (v4 == 1) Pause(); //暫停
              if (v0 == 1 || v1 == 1 || v2 == 1 || v3 == 0) break; //模式更換
            }
          }
          for (byte led = 0; led < p1_score / 7; led++) //玩家1得分顯示於燈條
          {
            p1_leds[led] = CRGB(r1, g1, b1);
            FastLED.show();
          }
          for (byte bigLED = 0; bigLED < p1_score / 13; bigLED++) //玩家1分數顯示於燈圈前半
          {
            boss_leds[bigLED] = CRGB (r1, g1, b1);
            FastLED.show();
          }

          if (p2_btnState == 1) //當玩家2按下按鈕
          {
            delay(50);
            getBTN();
            Blynk.run();
            if (v4 == 1) Pause(); //暫停
            if (v0 == 1 || v1 == 1 || v2 == 1 || v3 == 0) break; //模式更換
            if (p1_btnState == 1) //當玩家1按下按鈕
            {
              delay(50);
              getBTN();
              Blynk.run();
              if (v4 == 1) Pause(); //暫停
              if (v0 == 1 || v1 == 1 || v2 == 1 || v3 == 0) break; //模式更換
              if (p1_btnState == 0)  //放開按鈕玩家1得分
              {
                p1_score++;
                Serial.print("p1:");
                Serial.println(p1_score);
                Blynk.run();
                if (v4 == 1) Pause(); //暫停
                if (v0 == 1 || v1 == 1 || v2 == 1 || v3 == 0) break; //模式更換
              }
            }
            if (p2_btnState == 0)  //放開按鈕玩家2得分
            {
              p2_score++;
              Serial.print("p2:");
              Serial.println(p2_score);
              Blynk.run();
              if (v4 == 1) Pause(); //暫停
              if (v0 == 1 || v1 == 1 || v2 == 1 || v3 == 0) break; //模式更換
            }
          }
          for (byte led = 0; led < p2_score / 7; led++) //玩家2得分顯示於燈條
          {
            p2_leds[led] = CRGB(r2, g2, b2);
            FastLED.show();
          }
          for (byte bigLED = 16; bigLED >= 16 - (p2_score / 13); bigLED--) //玩家2分數顯示於燈圈後半
          {
            boss_leds[bigLED] = CRGB(r2, g2, b2);
            FastLED.show();
          }
          if (p1_score >= 160 || p2_score >= 160) //當有玩家勝利滅掉所有燈
          {
            Blynk.run();
            if (v4 == 1) Pause(); //暫停
            if (v0 == 1 || v1 == 1 || v2 == 1 || v3 == 0) break; //模式更換
            for (byte i = 0; i < 16; i++)
            {
              boss_leds[i] = CRGB::Black;
            }
          }

        }
        if (p1_score >= 100) //當玩家1勝利
        {
          //-------------------------------------------------------------------------------------------------------------------------------------------------噴霧
          for (byte i = 0; i < 16; i++)
          {
            boss_leds[i] = CRGB(r1, g1, b1);
            FastLED.show();
            delay(200);
          }
        }
        if (p2_score >= 100) //當玩家2勝利
        {
          //-------------------------------------------------------------------------------------------------------------------------------------------------噴霧
          for (byte i = 16; i >= 0 ; i--)
          {
            boss_leds[i] = CRGB(r2, g2, b2);
            FastLED.show();
            delay(200);
          }
        }
        break;
      }
    //------------------------------------------------------
    default:
      scanVirtualPin();
      break;
  }
  delay(100);
  defaultMusic();
}
