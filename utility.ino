void generatingPulse(int numPin, int numPulse, int pulseSpeed){
  for (int i = 0; i <= numPulse; i++){
    digitalWrite(numPin, HIGH); delay(int(pulseSpeed/2));
    digitalWrite(numPin, LOW); delay(int(pulseSpeed/2));  
    }
    }

void setup() {
  // put your setup code here, to run once:
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
   generatingPulse(9, 5, 200); // x coordinate
   generatingPulse(10, 5, 210); // y coordinate
   generatingPulse(11, 5, 220); // z coordinate
//   exit(0);
}
