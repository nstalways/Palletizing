/*
 This example runs on mega and uno.
 */
// maxPul, dir_xmotor, ena_xmotor

int p_pul_xmotor=12;  //pulse for motor x : white
int p_dir_xmotor=8;  //direction for motor x : green
int p_ena_xmotor=13; //enable for motor x : yellow

//unsigned int maxPul=1000; //Input Parameter
//unsigned int nPul=0;
//boolean dir_xmotor=LOW; // 
//boolean ena_xmotor=HIGH; //

boolean pulse_HL=LOW;

boolean pulse_running=HIGH;
boolean xstepPermit=HIGH;

int32_t frequency = 50; //frequency (in Hz)

void setup()
{ 
  Serial.begin(9600);
  pinMode(p_pul_xmotor,OUTPUT);
  pinMode(p_dir_xmotor,OUTPUT);
  pinMode(p_ena_xmotor,OUTPUT);
  
}

void loop()
{
//루프 자체는 계속해서 도는 중.
//serial polling 
// serial get (xmaxPul 100000 , xdir 0) -> maxPul=xmaxpul, dir_xmotor=xdir
unsigned int nPul = 0;
//boolean ena_xmotor = LOW;

while (Serial.available())
{
  // Reference: https://steemit.com/kr-arduino/@codingman/serial-string
  String inString = Serial.readStringUntil('\n');

  int index_x = inString.indexOf(',');
  int index_y = inString.length();
  int index_z = inString.indexOf('/');
  
  int x = inString.substring(0, index_x).toInt();
  int y = inString.substring(index_x+1, index_z).toInt();
  int z = inString.substring(index_z+1, index_y).toInt();

  unsigned int maxPul = (unsigned int)x; // 펄스 개수
  boolean dir_xmotor = (boolean)y; // 방향. 1이면 오른쪽, 0이면 왼쪽
  boolean ena_xmotor = (boolean)z; // enable=Low일 때 동작, High일 때 종료.
  
  if (ena_xmotor==0){
    digitalWrite(p_ena_xmotor, LOW);
  } // enable이 0이면 모터가 다시 동작하게끔 LOW 신호 인가

  // 시리얼 모니터에서 입력을 확인하는 부분
  Serial.print("Max Pulse: ");
  Serial.print(maxPul);
  Serial.print(" X direction: ");
  Serial.print(dir_xmotor);
  Serial.print(" X enable: ");
  Serial.print(ena_xmotor);
  Serial.print("\n");

  // X 방향 모터 제어하는 부분
  for (nPul; nPul<=maxPul; nPul++){
    // direction 신호를 실제 인가하는 부분
    if(dir_xmotor==HIGH){digitalWrite(p_dir_xmotor,HIGH);}
    if(dir_xmotor==LOW){digitalWrite(p_dir_xmotor,LOW);}
    
    // xstepPermit의 값에 따라 pulse를 생성함.
    if(xstepPermit==1){
      if(nPul<maxPul) {
        digitalWrite(p_pul_xmotor,pulse_HL);
        pulse_HL=!pulse_HL; 
      } else {digitalWrite(p_pul_xmotor,LOW);}
    }
    
    // maxPul을 넘는 순간, enable에 HIGH 신호를 인가하면서 모터를 정지시킴(모터에 전류를 주지 않는다는 의미)
    if(nPul>=maxPul){
      if (ena_xmotor==1){
       digitalWrite(p_ena_xmotor,HIGH); 
      }
    }
    delay(1);
    
  }

}
}
