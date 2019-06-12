#include <Wire.h> //i2C 통신 라이브러리
#include <LiquidCrystal_I2C.h> 
#include <SoftwareSerial.h> // for bluetooth 5 Sensor

LiquidCrystal_I2C lcd(0x27,16,2); //LCD 접근 주소
 
#define BT_RXD A2
#define BT_TXD A1
SoftwareSerial bluetooth(BT_RXD, BT_TXD);

char bluetoothChar=""; // bluetooth 메세지 수신될때 입력
String bluetoothMessage=""; // bluetooth 메세지 저장용

String myStatus="Leave work."; // 현재 상태 표시용
String lastStatus=""; //현재 상태 저장용

String lastMessage=""; // 마지막 메시지 저장용
String myPhone=" 010-1234-5678 "; //전화번호 스트링

int sw=5;
int cnt=0;
 
void setup(){
  Serial.begin(9600);
  bluetooth.begin(9600);
  pinMode(sw, INPUT_PULLUP);
  lcd.init(); //LCD 초기화
  lcd.backlight();

  lcd.setCursor(1,0); //출력 커서 위치 첫번쨰
  lcd.print("Booting...");
}
 
void loop() //메인 Loop
{
  while(bluetooth.available()) {
    bluetoothChar = (char)bluetooth.read();
    bluetoothMessage += bluetoothChar;
    delay(5);
  }

  if(!bluetoothMessage.equals(""))  //bluetooth 입력부
  {
    Serial.println("input: "+bluetoothMessage);
    if(lastStatus.equals("")) {
      lastStatus = myStatus;
    }
    lastMessage = bluetoothMessage;
    bluetoothMessage="";
  } 

  if(!lastMessage.equals("")) { // lastMessage가 있으면 myPhone과 교차출력
    for(int i=0; i<30; i++) { // 0.1초 간격으로 스위치 입력을 감지
      if(digitalRead(sw)==LOW)
        break;
      delay(100);
    }
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(lastStatus);
    lcd.setCursor(0,1);
    lcd.print(lastMessage);
    for(int i=0; i<30; i++) {
      if(digitalRead(sw)==LOW)
        break;
      delay(100);
    }
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(lastStatus);
    lcd.setCursor(0,1);
    lcd.print(myPhone);
  }

  if(!myStatus.equals(""))
  {
    Serial.println(myStatus);
    lastStatus = myStatus;
    lcd.clear(); //LCD 초기화
    lcd.setCursor(0,0);
    lcd.print(myStatus);
    lcd.setCursor(0,1);
    lcd.print(myPhone);
    myStatus="";
  }


  if(digitalRead(sw)==LOW) {
    lastMessage = ""; //버튼 눌렀을때 lastMessage 초기화
    
    if(cnt==0) {
      myStatus="In the room.";
      cnt++;
      delay(500);
    }
    else if(cnt==1) {
      myStatus="Out of the room.";
      cnt++;
      delay(500);
    }
    else {
      myStatus="Leave work.";
      cnt=0;
      delay(500);
    }
  }
}
