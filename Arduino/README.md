**Palletizing Robot을 제어하기 위해 작성한 아두이노 코드**<br>
1. xyMotorTest.ino<br>
- Palletizing Robot의 x, y 축 모터를 제어하기 위한 코드<br>
- 동작 원리는 코드 하단 주석에 작성되어 있음.<br>
- <span style="color:red">X, Y 모터 변경에 따라 제어값에 변동이 생겼음. 실험을 통해 값 변경 필요</span>

2. ServoMotor_PWM_test.ino<br>
- Palletizing Robot의 z축 모터를 제어하기 위한 코드<br>
- 해당 코드를 사용하기 위해선 PWM 라이브러리를 설치해야 함.<br>
