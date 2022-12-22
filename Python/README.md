# **Instance Segmentation, CV algorithm, user-defined functions**<br>
## Instance Segmentation<br>
- 모델: Yolact [https://github.com/dbolya/yolact]<br>
- 데이터: 정육면체(직접 구축) [[데이터 링크]](https://drive.google.com/drive/folders/1mY1t2hW2ppF5HKe9G4--kyBVDySUKixd?usp=share_link)<br>
- Pretrained weight: [[weight 링크]](https://drive.google.com/drive/folders/1mY1t2hW2ppF5HKe9G4--kyBVDySUKixd?usp=share_link)<br>
- 실행을 위한 기본 setup<br>
> 1. Yolact github clone
> 1. Yolact 하위 폴더 중 data 폴더의 config.py를 수정해야 함. 이는 모델 학습/추론을 위해 **필수**이므로 반드시 확인해야 함. [[참고 링크]](https://ksg980920.tistory.com/2)
> 1. palletizing2.py 다운로드 후 Yolact 하위 폴더에 복사
> 1. pretrained weight 다운 후 weights 폴더에 넣어주기
> 1. Script를 통해 실행<br>
> ```python palletizing2.py --trained_model=weights/다운받은_weight_파일.pth --score_threshold=threshold_val(0.0~1.0) --top_k=integer(as you want)```<br>
> (추가로 필요한 argument는 Yolact github 참조)

## CV algorithm<br>
- cv2.findContours(): 모델이 예측한 값을 기반으로 윤곽선을 검출<br>
- cv2.minAreaRect(): 검출한 윤곽선에 외접하면서 면적이 가장 작은 직사각형을 구함<br>
- cv2.moments(): 검출한 윤곽선을 기반으로 물체의 중심점을 구함. 실제 하드웨어 제어에 필요한 값들을 연산할 때 사용<br>
- cv2.approxPolyDP(): 검출한 윤곽선의 근사된 다각형을 구함<br>
- cv2.drawContours(): 앞서 근사시킨 다각형을 기반으로 이미지에 윤곽선을 그림

## User-defined functions<br>
- calCenter(): cv2.moments()의 return 값을 기반으로 물체의 중심점을 구하는 함수<br>
- makePulse(): 물체의 중심점과 그리퍼의 중심점 사이 거리를 기반으로, 하드웨어 제어에 필요한 펄스 값을 연산하는 함수<br>
- Py2Ard(): 하드웨어의 전체적인 동작 흐름을 구현한 함수
