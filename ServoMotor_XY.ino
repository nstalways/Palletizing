/*

 This example runs on mega and uno.
 */
// maxPul, dir_xmotor, ena_xmotor

int p_pul_xmotor=12;  //pulse for motor x : white
int p_dir_xmotor=8;  //direction for motor x : green
int p_ena_xmotor=13; //enable for motor x : yellow

unsigned int maxPul=1000; //Input Parameter
unsigned int nPul=0;
boolean dir_xmotor=LOW; // 
boolean ena_xmotor=HIGH; //

boolean pulse_HL=LOW;

boolean pulse_running=HIGH;
boolean xstepPermit=HIGH;

int32_t frequency = 50; //frequency (in Hz)

void setup()
{ 
  pinMode(p_pul_xmotor,OUTPUT);
  pinMode(p_dir_xmotor,OUTPUT);
  pinMode(p_ena_xmotor,OUTPUT);
  
}

void loop()
{

//serial polling 
// serial get (xmaxPul 100000 , xdir 0) -> maxPul=xmaxpul, dir_xmotor=xdir




  //end serial polling

if(dir_xmotor==HIGH){digitalWrite(p_dir_xmotor,HIGH);}
if(dir_xmotor==LOW){digitalWrite(p_dir_xmotor,LOW);}


if(xstepPermit==1){
  if(nPul<maxPul) {
    digitalWrite(p_pul_xmotor,pulse_HL);
    pulse_HL=!pulse_HL; 
  } else {digitalWrite(p_pul_xmotor,LOW);}
nPul++;   
}


if(nPul>=maxPul){ digitalWrite(p_ena_xmotor,HIGH); }
delay(1);       

}
