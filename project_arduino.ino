#include <Wire.h> //i2C 통신 라이브러리
#include <LiquidCrystal_I2C.h> 
#include <SoftwareSerial.h> //bluetooth 5

LiquidCrystal_I2C lcd(0x27,16,2); //LCD 접근 주소
 
#define BT_RXD A2
#define BT_TXD A1
SoftwareSerial bluetooth(BT_RXD, BT_TXD);
String messageString="";
String mystatus="";
String lastStatus="";
String lastMessage="";
char messagechar="";
String myPhone=" 010-1234-5678 ";
int cnt=0;
 
void setup(){
  Serial.begin(9600);
  bluetooth.begin(9600);
  pinMode(5, INPUT_PULLUP);
  lcd.init(); //LCD 초기화
  lcd.backlight();

  lcd.setCursor(1,0); //출력 커서 위치 첫번쨰
  lcd.print("Booting...");
}
 
void loop() //메인 Loop
{
  while(bluetooth.available()) {
    messagechar = (char)bluetooth.read();
    messageString += messagechar;
    delay(5);
  }

  if(!messageString.equals(""))  //bluetooth 입력부
  {
    Serial.println("input: "+messageString);
    lastMessage = messageString;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(lastStatus);
    lcd.setCursor(0,1);
    lcd.print(messageString);
    messageString="";
  }

  /*if(!lastMessage.equals("")) //lastMeassage와 myPhone 교차 출력부
  {
    lcd.setCursor(0,1);
  }*/

  if(!mystatus.equals(""))
  {
    Serial.println(mystatus);
    lastStatus = mystatus;
    lcd.clear(); //LCD 초기화
    lcd.setCursor(0,0);
    //lcd.autoscroll();
    lcd.print(mystatus);
    lcd.setCursor(0,1);
//    lcd.print(myPhone);
    mystatus="";
  }


  if(digitalRead(5)==LOW) {
    //lastMessage = ""; //버튼 눌렀을때 last Message 초기화
    
    if(cnt==0) {
      mystatus="In the room.";
      cnt++;
      delay(500);
    }
    else if(cnt==1) {
      mystatus="Out of the room.";
      cnt++;
      delay(500);
    }
    else {
      mystatus="Leave work.";
      cnt=0;
      delay(500);
    }
  }
}
