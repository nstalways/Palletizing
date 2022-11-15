# Palletizing Robot 동작을 위한 함수들
import time
import serial

def calCenter(M): # Max x, Min x, Max y, Min y
    # 중심 좌표 계산
    cx = round(M['m10'] / M['m00'])
    cy = round(M['m01'] / M['m00'])

    return (cx, cy)

# OpenCV의 이미지 좌표는 (x = row(height), y = col(width))
# Matplotlib의 이미지 좌표는 (x = col(width), y = row(height))
# 펄스 계산에 사용하는 좌표는 cv2 좌표이기 때문에, x 모터에 move_x를, y 모터에 move_y를 주면 됨.
# dir_x=0이면 위(왼쪽)로, 1이면 아래(오른쪽) 이동
# dir_y=0이면 앞쪽으로, 1이면 뒤쪽으로 이동

def makePulse(centerpoint, angle):    
    grip_x, grip_y = 265, 512 # gripper의 중심점

    x, y = centerpoint[0], centerpoint[1] # 인식한 물체의 중심점(x:col, y:row)
    print(f'cx, cy: {x}, {y}')

    move_x = grip_x - x # x 모터 펄스 (gripper의 x 좌표 - 인식한 물체의 x좌표 = x 거리)
    move_y = grip_y - y # y 모터 펄스 (gripper의 y 좌표 - 인식한 물체의 y좌표 = y 거리)

    pulse_y = round(abs(move_y) * 13.63) # 13.63은 반복 실험을 통해 구한 파라미터(모터 변경 시 값도 변경해야함)
    if move_y >= 0: # 위쪽(+y 방향)으로 이동
        dir_y = 0
    else: # 아래쪽(-y 방향)으로 이동
        dir_y = 1
    ena_y = 0 # enable은 0(=LOW)일 때 동작하므로 초기값은 0으로 set

    # 
    if move_x >= 0:
        pulse_x = round(abs(move_x)*1.93)
        dir_x = 0
    else:
        pulse_x = round(abs(move_x)*1.88)
        dir_x = 1
    ena_x = 0
    
    grip = 25 # 닫기
    if round(angle) < 5 or round(angle) >= 85:
        wrist = 20
    else:
        wrist = 20 - round(angle*0.1207) # 움직여야할 펄스 개수, 20은 rotation 중립
    pulse_z = 22000 # 이만큼 움직이는데 23초 소요(고정값)
    dir_z = 0

    # 이미지 영역에서 Y축으로 이동하려면 하드웨어 영역에서 X축 모터를 제어해야하고,
    # 이미지 영역에서 X축으로 이동하려면 하드웨어 영역에서 Y축 모터를 제어해야하기 때문에, 반대로 값을 넘겨줌.

    return (pulse_y, dir_y, ena_y, pulse_x, dir_x, ena_x, grip, wrist, pulse_z, dir_z)

def Py2Ard(info, baudrate):
    # 데이터 파싱
    xpul, xdir, xena = info[0], info[1], info[2]
    ypul, ydir, yena = info[3], info[4], info[5]
    
    fpow, wpow, zpow, zdir = info[6], info[7], info[8], info[9]
    
    # 아두이노로 전송하기 위해 파싱한 데이터 다시 병합
    x_data = str(xpul)+','+str(xdir)+'/'+str(xena)
    y_data = str(ypul)+','+str(ydir)+'/'+str(yena)
    xy_data = x_data+'|'+y_data

    gripper_data = str(fpow)+','+str(wpow)
    z_data = str(zpow)+','+str(zdir)
    grip_z_data = gripper_data+'|'+z_data

    print(xy_data)
    print(grip_z_data)

    # 시리얼 포트 오픈
    ard1 = serial.Serial('COM6', baudrate)
    ard2 = serial.Serial('COM7', baudrate)

    ard1.timeout, ard2.timeout = 1, 1
    
    # 로봇 이동을 위한 초기 데이터 전송
    time.sleep(2)
    ard1.write(xy_data.encode())
    time.sleep(4)
    ard2.write(grip_z_data.encode())
    time.sleep(24) # z축 모터의 속도가 매우 느려서 24초라는 시간을 고정적으로 기다림. 알고리즘을 통해 가변적으로 변경 가능

    # 지정한 자리로 돌아가기 위한 조건문
    if xdir==0:
        xpul=xpul+3500
        xdir=1
    elif xdir==1:
        if xpul>3500: #더많이가면
            xpul=xpul-3500
            xdir=0
        else:
            xpul=3500-xpul
            xdir=1
    if ydir==0:
        if ypul>600:
            ypul=ypul-600
            ydir=1
        else:
            ypul=600-ypul
            ydir=0
    elif ydir==1:
        ypul=ypul+600
        ydir=0

    wpow=20
    zdir=1

    # 돌아가기 위한 데이터 병합
    x_data = str(xpul)+','+str(xdir)+'/'+str(xena)
    y_data = str(ypul)+','+str(ydir)+'/'+str(yena)
    xy_data = x_data+'|'+y_data

    gripper_data = str(fpow)+','+str(wpow)
    z_data = str(zpow)+','+str(zdir)
    grip_z_data = gripper_data+'|'+z_data

    # 지정 위치로 돌아가는 데이터 전송
    time.sleep(2)
    ard2.write(grip_z_data.encode())
    time.sleep(24)
    ard1.write(xy_data.encode())
    time.sleep(4) # 변경 예정

    # 원래 자리로 돌아오는 데이터 선언
    xpul, xdir = 3500, 0
    ypul, ydir = 600, 1

    fpow, zpow = 13, 0

    # 원래 자리로 돌아오는 데이터 병합
    x_data = str(xpul)+','+str(xdir)+'/'+str(xena)
    y_data = str(ypul)+','+str(ydir)+'/'+str(yena)
    xy_data = x_data+'|'+y_data

    gripper_data = str(fpow)+','+str(wpow)
    z_data = str(zpow)+','+str(zdir)
    grip_z_data = gripper_data+'|'+z_data    

    print(xy_data)
    print(grip_z_data)
    # 마지막 데이터 전송
    time.sleep(2)
    ard2.write(grip_z_data.encode())
    time.sleep(24)
    ard1.write(xy_data.encode())
    time.sleep(2)