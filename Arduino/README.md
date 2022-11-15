**Palletizing Robot을 제어하기 위해 작성한 아두이노 코드**<br>
1. xyMotorTest.ino<br>
- Palletizing Robot의 x, y 축 모터를 제어하기 위한 코드<br>
- 동작 원리는 코드 하단 주석에 작성되어 있음.<br>
- 22.11.16) X, Y 모터 변경에 따라 제어값 변동 발생. 실험을 통해 적정 펄스 개수를 찾아 코드 변경 필요<br>

2. ServoMotor_PWM_test.ino<br>
- Palletizing Robot의 z축 모터를 제어하기 위한 코드<br>
- 해당 코드를 사용하기 위해선 PWM 라이브러리를 설치해야 함.<br>
- 동작 원리는 코드 하단 주석에 작성되어 있음.<br>
