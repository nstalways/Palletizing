//  This example runs on uno.
String inString;
#include <PWM.h>

int p_mg90s = 9;              // signal for mg90 Servo Motor -> 그리퍼 open / close 제어하는 모터
int p_hs311 = 10;             // 그리퍼 rotation 제어하는 모터

int p_zstepCw = 2;            // z축 모터의 경우 핀 두 개로 방향을 제어함.
int p_zstepCcw = 4;           // cw에 펄스를 인가하면 위로 이동, Ccw에 펄스를 인가하면 아래로 이동

//--------------- input parameter --------
//int tw_mg90s=13; //13(min), 19(neutral)->16, 25(max) // 13이면 완전 open(-90도), 25면 완전 close(+90도), 19면 그리퍼를 중간 위치로 이동
//int tw_hs311=19;  //7(min)->6, 19(neutral)->20, 31(max)->32 // 7이면 -90도로 회전, 31이면 +90도로 회전, 19면 중간 위치.
//unsigned int maxPulz=2000; //Input Parameter ~1cm/1000
//boolean Cw=LOW; // Cw가 LOW면 아래로 이동, HIGH면 위로 이동
//------------- End input parameter ----------- 얘들을 입력으로,

//unsigned int nPulz=0;

unsigned int nPul = 0;
boolean pulse_HL = HIGH;

int32_t frequency = 50; //pwm frequency (in Hz)

void setup()
{
  Serial.begin(9600); 
  pinMode(p_mg90s, OUTPUT);
  pinMode(p_hs311, OUTPUT);
  
  //initialize all timers except for 0, to save time keeping functions (ex. delay...)
  InitTimersSafe();
  //sets the frequency for the specified pin
  SetPinFrequencySafe(p_mg90s, frequency); //https://setoo0922.tistory.com/293<-기능설명, 교수님 주신 ppt 첫장보면 20ms(50hz)로 되어있는 그래서 하는 듯
  SetPinFrequencySafe(p_hs311, frequency); 

  pinMode(p_zstepCw, OUTPUT);
  pinMode(p_zstepCcw, OUTPUT);
 
}

void loop()
{
  unsigned int nPulz = 0;
// serial polling to get : finger: tw_mg90s_pw,  wrist: tw_hs311, zmotor: maxPulz, Cw 
// At serial date received, only single trial to set nPulz=0,
  while (Serial.available())
{
  // Reference: https://steemit.com/kr-arduino/@codingman/serial-string
  String inString = Serial.readStringUntil('\n'); //finger값,wrist값|z펄스수,z방향
  Serial.println(inString);

  int index_divide=inString.indexOf('|');
  int len=inString.length();

  String paramG=inString.substring(0, index_divide);
  String paramZ=inString.substring(index_divide+1,len);

  int lenG=paramG.length();
  int lenZ=paramZ.length();
  
  int index_Gf = paramG.indexOf(',');
  int index_Z = paramZ.indexOf(',');
  
//  int x = inString.substring(0, index_x).toInt();
//  int y = inString.substring(index_x+1, index_z).toInt();
//  int z = inString.substring(index_z+1, index_y).toInt();

  unsigned int fpower = (unsigned int)paramG.substring(0, index_Gf).toInt(); // finger의 움직이는 정도 25~13
  unsigned int wpower= (unsigned int)paramG.substring(index_Gf+1, lenG).toInt(); // , wrist는 19~31, 펄스 수
  unsigned int zpower= (unsigned int)paramZ.substring(0, index_Z).toInt(); //Z축 이동 펄스 수
  unsigned int updown = (unsigned int)paramZ.substring(index_Z+1,lenZ).toInt(); //  High일 때 아래로, Low일 때 위로 Z축 이동.
  

  // 시리얼 모니터에서 입력을 확인하는 부분
  Serial.print("fpower: ");
  Serial.print(fpower);
  Serial.print(" wpower: ");
  Serial.print(wpower);
  Serial.print(" zpower: ");
  Serial.print(zpower); 
  Serial.print(" up or down: ");
  Serial.print(updown);
  Serial.print("\n");
// end serial polling
  
  // direction 신호를 실제 인가하는 부분
  pwmWrite(p_hs311, wpower); //gripper wrist rotate
  for (nPul; nPulz<=zpower; nPulz++){
        if(nPulz<zpower) {
          if(updown==LOW) {
            digitalWrite(p_zstepCw,pulse_HL);
            } else {digitalWrite(p_zstepCcw,pulse_HL);} // Cw가 LOW면 앞쪽 코드 실행(Cw 핀에 펄스를 줌), Cw가 HIGH면 뒤쪽 코드 실행 (Ccw에 펄스를 줌)
        pulse_HL=!pulse_HL;
        delay(1);
      }  
    }
  
  //delay(1000);
  pwmWrite(p_mg90s, fpower); //gripper finger close
  //pwmWrite(p_hs311,0);
  }//z축 모터
  
}
