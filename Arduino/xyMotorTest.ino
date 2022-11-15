String inString;
// This example runs uno.
// `22.06.22 (수) 동작 확인하였음.

int p_pul_xmotor=12;  //pulse for motor x : white
int p_dir_xmotor=8;  //direction for motor x : green
int p_ena_xmotor=13; //enable for motor x : yellow
int p_pul_ymotor=4;  //pulse for motor y : white
int p_dir_ymotor=2;  //direction for motor y : green
int p_ena_ymotor=7; //enable for motor y : yellow

//-------- input parameter ------------ -> Serial 데이터로 받아옴.
//unsigned int maxPulx=1000; // 펄스 개수만큼 x 모터가 움직임.
//unsigned int maxPuly=100; // 펄스 개수만큼 y 모터가 움직임. (하지만 x 모터와 y 모터 종류가 달라서, 똑같은 pulse 개수일 때 y 모터가 더 많이 움직임.)
//boolean dir_xmotor=HIGH;  // dir이 HIGH면 오른쪽, LOW면 왼쪽
//boolean dir_ymotor=HIGH;  // 동일
//boolean ena_xmotor=HIGH; // enable. HIGH면 모터에 전류 공급을 차단하여 동작을 멈춤.
//boolean ena_ymotor=HIGH; // 동일
//-------- input parameter ------------

//unsigned int nPulx=0; // loop를 깨기 위해 선언된 변수
//unsigned int nPuly=0; // 동일

boolean pulse_HL=HIGH; // 펄스 신호를 만들기 위해 선언한 변수.

int32_t frequency = 50; //frequency (in Hz)int ny=0; // 주파수의 의미는 아직 잘 모름.

void setup()
{ 
  Serial.begin(9600);
  // 사용할 pin들을 setup
  pinMode(p_pul_xmotor,OUTPUT);
  pinMode(p_dir_xmotor,OUTPUT);
  pinMode(p_ena_xmotor,OUTPUT);
  
  pinMode(p_pul_ymotor,OUTPUT);
  pinMode(p_dir_ymotor,OUTPUT);
  pinMode(p_ena_ymotor,OUTPUT);
//  ena_xmotor=LOW; ena_ymotor=LOW; // ena = LOW일 때 모터가 동작하기 떄문에, 초기값은 LOW로 설정

  //serial 데이터로 제어하려면 아래 코드 필요.
  //Serial.begin(9600); // begin 내부 parameter는 baud rate로, 데이터의 송/수신 속도를 의미함. 일반적으로 9600을 설정.
}

// 모터를 제어하는 loop
void loop()
{
  unsigned int nPulx = 0;
  unsigned int nPuly = 0;
  //String inString;
  //serial polling 
  // serial get (xmaxPul 100000 , xdir 0) -> maxPul=xmaxpul, dir_xmotor=xdir
  // At serial date received, only single trial to set nPulx=0, nPuly=0
  while (Serial.available())
  { 
//     while (Serial.available()){
//      char wait = Serial.read();
//      inString.concat(wait);
//      Serial.print(inString);
//     }
    // Serial port로 데이터 읽어오기.
    // 입력 방식 -> maxPulX,dir_xmotor/ena_xmotor|maxPulY,dir_ymotor/ena_ymotor
    //String inString = Serial.readStringUntil('\n'); // x와 y 파라미터에 대한 값들을 모두 입력받아 inString에 저장.
    inString = Serial.readStringUntil('\n');
    Serial.println(inString);
    //Serial.write(inString);
    
    int index_divide = inString.indexOf('|'); // 기호 |를 기준으로 문자열을 나누기 위해 index를 가져옴.
    int len = inString.length(); // 문자열을 나누기 위해 index가 필요해서 선언.
  
    String paramX = inString.substring(0, index_divide); // x 모터에 들어갈 파라미터들이 담겨있는 문자열.
    String paramY = inString.substring(index_divide+1, len); // y 모터에 들어갈 파라미터들이 담겨있는 문자열.

    int lenX = paramX.length();
    int lenY = paramY.length();
    
    int index_maxPulx = paramX.indexOf(',');
    int index_dirX = paramX.indexOf('/');
  
    int index_maxPuly = paramY.indexOf(',');
    int index_dirY = paramY.indexOf('/');

    int x = paramX.substring(0, index_maxPulx).toInt();
    int y = paramX.substring(index_maxPulx+1, index_dirX).toInt();
    int z = paramX.substring(index_dirX+1, lenX).toInt();
    
    unsigned int maxPulx = (unsigned int) paramX.substring(0, index_maxPulx).toInt();
    boolean dir_xmotor = (boolean) paramX.substring(index_maxPulx+1, index_dirX).toInt();
    boolean ena_xmotor = (boolean) paramX.substring(index_dirX+1, lenX).toInt();
  
    unsigned int maxPuly = (unsigned int) paramY.substring(0, index_maxPuly).toInt();
    boolean dir_ymotor = (boolean) paramY.substring(index_maxPuly+1, index_dirY).toInt();
    boolean ena_ymotor = (boolean) paramY.substring(index_dirY+1, lenY).toInt();
    // Serial port로 데이터 읽어오기.

    // Serial 모니터에서 입력을 확인하는 부분
    Serial.write("Max Pulse for X motor: ");
    Serial.write(maxPulx);
    Serial.write(", X direction: ");
    Serial.write(dir_xmotor);
    Serial.write(", X enable: ");
    Serial.write(ena_xmotor);
    Serial.write("\n");

    Serial.write("Max Pulse for Y motor: ");
    Serial.write(maxPuly);
    Serial.write(", Y direction: ");
    Serial.write(dir_ymotor);
    Serial.write(", Y enable: ");
    Serial.write(ena_ymotor);
    Serial.write("\n");
    // Serial 모니터에서 입력을 확인하는 부분
    
    // dir_xmotor와 dir_ymotor는 먼저 할당.
    if(dir_xmotor==HIGH){digitalWrite(p_dir_xmotor,HIGH);} if(dir_xmotor==LOW){digitalWrite(p_dir_xmotor,LOW);}
    if(dir_ymotor==HIGH){digitalWrite(p_dir_ymotor,HIGH);} if(dir_ymotor==LOW){digitalWrite(p_dir_ymotor,LOW);}

    // 초기에는 x, y enable 모두 LOW로 줘야 모터가 구동하기 때문에, 일단 setting 해줌.
    digitalWrite(p_ena_xmotor, LOW);
    digitalWrite(p_ena_ymotor, LOW);

    // 조건문을 이용, 새로운 enable 입력이 0이면 모터가 다시 동작하게끔 LOW 신호 인가.
    if (ena_xmotor==0){
      digitalWrite(p_ena_xmotor, LOW);
    }
    if (ena_ymotor==0){
      digitalWrite(p_ena_ymotor, LOW);
    }
    
    while (1)
    {
     // when 'nPulx or nPuly < max' set enable and pulse ready
    if((nPulx<maxPulx)||(nPuly<maxPuly)) {pulse_HL=!pulse_HL; } // pusle_HL 신호를 x, y 모터에 공통으로 인가함.
    
    //x y pulse generation -> 앞선 조건문에 의해 설정된 pulse_HL 신호를 넘겨줌. 따라서 x, y 모터가 pulse 개수만큼 순차적으로 동작함.
    if(ena_xmotor==LOW){
      if(nPulx<maxPulx) {
        digitalWrite(p_pul_xmotor,pulse_HL);
      } else {digitalWrite(p_pul_xmotor,LOW);}
      nPulx++;   
    }
    
    if(ena_ymotor==LOW){
        if(nPuly<maxPuly) {
          digitalWrite(p_pul_ymotor,pulse_HL);
        } else {digitalWrite(p_pul_ymotor,LOW);}
        nPuly++;
    }
    
    // 조건을 만족하면, enable을 HIGH로 설정함으로써 모터에 전류 공급을 차단함.
    if(nPulx>=maxPulx){ ena_xmotor=HIGH; digitalWrite(p_ena_xmotor, ena_xmotor); }
    if(nPuly>=maxPuly){ ena_ymotor=HIGH; digitalWrite(p_ena_ymotor,ena_ymotor); }
    if((nPulx>=maxPulx)&&(nPuly>=maxPuly)){ break; } // x,y 모터 모두! maxPul을 넘으면 while문을 break함.
    
    delay(1);           
    } 
  }
}

// 동작 흐름 정리
// 1. void loop()는 아두이노에 올렸을 때 무한 반복한다.
// 2. while (Serial.available()){} 구문은 시리얼 포트로부터 입력된 데이터가 있으면 True, 없으면 False이다. 즉 입력된 데이터가 있을 때만 while 구문 안으로 들어간다.
// 3. 입력받은 시리얼 데이터를 split하여 변수에 저장한다.
// 4. 저장한 값을 바탕으로 while (1){} 구문에 들어가 모터를 제어한다. maxPulX, maxPulY만큼 반복하면 while (1){} 구문을 빠져나와 void loop를 반복한다.
// 4-1. delay()는 신호를 유지하는 역할을 한다. 현재 1ms로 설정되어있고, High 신호를 1ms 유지, 이후 Low 신호를 1ms 유지... 를 반복한다.
// 4-2. while (1) 구문을 break하지 않으면 모터 동작은 멈추나 while문은 계속 돌고있기 때문에 새로 입력받은 시리얼 데이터를 처리할 수 없다. 따라서 꼭 필요.
// 5. 입력받은 데이터를 통해 모터 제어가 끝나고 나면, 새로운 시리얼 데이터가 입력되기 전까지 void loop를 계속 돈다. 새로운 데이터를 입력받으면 2번-4번 과정을 반복한다.
