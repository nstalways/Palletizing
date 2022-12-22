# [2022 CDP] Palletizing Robot<br>
Instance Segmentation 기술을 활용한 물체 적재 로봇<br><br>

## 개요<br>
4차 산업혁명의 확산세에 따라 산업현장, 공공기관, 서비스업 등을 포함한 사회 전반에 자동화 시스템이 들어서고 있다. 2022년 약 11개의 대기업은 4년간 905개 협력사의 스마트공장 구축을 발표했다. 뿐만 아니라 환경 미화, 재활용 분리와 같은 공공서비스 부분 역시 자동화 시스템의 도입을 추진하고 있다.<br><br>
인건비, 삶의 질과 같은 문제를 차지하고도 인구 절벽의 문제에 놓인 현대 사회에서 자동화 시스템의 저변 확대는 자명하다.  이에 우리 조에서는 공학적 방법론을 통해 여러 산업, 사회문제 상황에서 활용성과 범용성을 보일 수 있는 프로젝트로 **Instance Segmentation 분야의 Yolact 모델을 활용한 Palletizing Robot**의 개발을 진행했다.<br><br>

## 시스템 구조<br>
![image](https://user-images.githubusercontent.com/90047145/209156393-4aaba3ce-a4cd-4500-9e99-fd42dc8446b0.png)
- 전체적인 동작 흐름은 다음과 같다.<br>
> 1. 웹캠을 켠 상태에서 이미지를 캡처한다.<br>
> 1. 캡쳐한 이미지를 학습시킨 Yolact 모델에게 전달, Segmentation 결과를 예측한다.<br>
> 1. 예측한 결과를 바탕으로 하드웨어 제어에 필요한 수치들을 연산한다.<br>
> 1. 연산한 수치는 시리얼 통신을 기반으로 아두이노 보드에 전달된다.<br>
> 1. 수신한 데이터는 아두이노 보드에 업데이트한 코드의 흐름에 따라 모터를 제어한다. Object는 사용자가 지정한 위치에 적재되면서 동작을 마무리한다.<br>
> 1. 1-5번 과정을 계속해서 반복한다.<br>
<br>

## 실제 사진 및 동작 영상<br>

|정면 뷰|상단 뷰|영상|
|:--:|:--:|:--:|
|![image](https://user-images.githubusercontent.com/90047145/209161345-2f550812-a013-46a8-b5b1-c2b34968f9e6.png)|![image](https://user-images.githubusercontent.com/90047145/209161518-90dfad5a-f409-4c6c-8948-513def11adec.png)|[Palletizing Robot 동작 영상](https://blog.naver.com/qkrtndud2010/222963116724)|

<br>

## 결과 <br>
- 2022 전자공학과 Capstone Design Project **금상 수상**<br>

|상장|
|:-:|
|![image](https://user-images.githubusercontent.com/90047145/209162845-4fd40617-96a5-4262-a139-98355b45d877.png)|

