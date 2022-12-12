**Instance Segmentation, CV algorithm, user-defined functions**<br>
1. Instance Segmentation<br>
- 모델: Yolact [https://github.com/dbolya/yolact]<br>
- 데이터: 정육면체(직접 구축) [[데이터 링크]](https://drive.google.com/drive/folders/1mY1t2hW2ppF5HKe9G4--kyBVDySUKixd?usp=share_link)<br>
- Pretrained weight: [[weight 링크]](https://drive.google.com/drive/folders/1mY1t2hW2ppF5HKe9G4--kyBVDySUKixd?usp=share_link)<br>
- 실행<br>
Yolact clone -> palletizing2.py 다운로드 후 Yolact 하위 폴더로 이동 -> pretrained weight 다운 후 weights 폴더에 넣어주기 -> palletizing2.py 실행
2. CV algorithm<br>
- cv2.findContours(): 모델이 예측한 값을 기반으로 윤곽선을 검출<br>
- cv2.minAreaRect(): 검출한 윤곽선에 외접하면서 면적이 가장 작은 직사각형을 구함<br>
- cv2.moments(): 검출한 윤곽선을 기반으로 물체의 중심점을 구함. 실제 하드웨어 제어에 필요한 값들을 연산할 때 사용<br>
- cv2.approxPolyDP(): 검출한 윤곽선의 근사된 다각형을 구함<br>
- cv2.drawContours(): 앞서 근사시킨 다각형을 기반으로 이미지에 윤곽선을 그림
3. User-defined functions<br>
- calCenter(): cv2.moments()의 return 값을 기반으로 물체의 중심점을 구하는 함수<br>
- makePulse(): 물체의 중심점과 그리퍼의 중심점 사이 거리를 기반으로, 하드웨어 제어에 필요한 펄스 값을 연산하는 함수<br>
- Py2Ard(): 하드웨어의 전체적인 동작 흐름을 구현한 함수
