#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#include <WiFi.h>
#include <IOXhop_FirebaseESP32.h>


#define FIREBASE_HOST "loveboxtest.firebaseio.com"                         // the project name address from firebase id
#define FIREBASE_AUTH "jqCBe0X42WOZh5Q6Q2Gp8NkNKsOhhpD3aOeCPxVn"                    // the secret key generated from firebase
#define WIFI_SSID ""                                          // input your home or public wifi name 
#define WIFI_PASSWORD "" 
#define SCREEN_WIDTH 320  
#define SCREEN_HEIGHT 240

String fireStatus = "";
String fireStatusNew = "";
  int16_t x, y;
  uint16_t w, h;


const int freq = 5000;
const int ledChannel = 0;
const int resolution = 10;


// extern uint8_t heart[];

#define LED_NOTIF 4
#define BUTTON_PIN 12
#define MOTOR_PIN 34

#define TFT_MISO 25
#define TFT_LED 5
#define TFT_SCK 19
#define TFT_MOSI 23
#define TFT_DC 21
#define TFT_RESET 18
#define TFT_CS 22

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS,TFT_DC,TFT_MOSI,TFT_SCK,TFT_RESET,TFT_MISO);

void setup() {
  // put your setup code here, to run once:

  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_NOTIF, OUTPUT);

  pinMode(MOTOR_PIN,OUTPUT);

Serial.begin(9600);
  delay(1000);
  //pinMode(2, OUTPUT);                
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                  
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

    fireStatus = Firebase.getString("message");
    Serial.print(fireStatus);
    

  pinMode(TFT_LED,OUTPUT);
  digitalWrite(TFT_LED,HIGH);
  Serial.begin(9600);
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_PURPLE);
  tft.setTextWrap(true);
  tft.setTextSize(3);
  tft.getTextBounds(fireStatus, 0, 0, &x, &y, &w, &h);
  Serial.println(x);
  Serial.println(y);
  Serial.println(w);
  Serial.println(h);

  tft.setCursor(SCREEN_WIDTH/2 - w/2,SCREEN_HEIGHT/2 - h/2);

  tft.println(fireStatus);
 // tft.drawBitmap(50,50,heart,50,50,ILI9341_WHITE);


}

int ledState = 0;

void loop() {
  // put your main code here, to run repeatedly:
 
  int buttonState = digitalRead(BUTTON_PIN);

if(ledState) {
  digitalWrite(MOTOR_PIN,HIGH);
  delay(1300);
  digitalWrite(MOTOR_PIN, LOW);
}
  if (!ledState || !buttonState)
    fireStatusNew = Firebase.getString("message");

  if (fireStatus.compareTo(fireStatusNew) != 0) {
    if (buttonState) {
      digitalWrite(LED_NOTIF, HIGH);
      ledState = 1;
    } else {
      digitalWrite(LED_NOTIF, LOW);
      ledState = 0;
    }
     
      fireStatus = fireStatusNew;
      tft.fillScreen(ILI9341_PURPLE);
      tft.getTextBounds(fireStatus, 0, 0, &x, &y, &w, &h);
  
     tft.setCursor(SCREEN_WIDTH/2 - w/2,SCREEN_HEIGHT/2 - h/2);
      Serial.println(h);
    tft.println(fireStatus);
  } else if (!buttonState){ 
    digitalWrite(LED_NOTIF, LOW);
    Firebase.setString("messageStatus","Seen");
   
    }
  delay(5000);
}
       
