import time, serial

def calCenter(M): # Max x, Min x, Max y, Min y
    # 중심 좌표 계산
    cx = round(M['m10'] / M['m00'])
    cy = round(M['m01'] / M['m00'])

    return (cx, cy)

def makePulse(centerpoint, angle):
    grip_x, grip_y = 265, 512

    x, y = centerpoint[0], centerpoint[1]
    params = {'x_left':67.5, 'x_left_center':30, 'x_right_center':55, 'x_right':72,
                'y_upper':25.5, 'y_center':23.5, 'y_lower':21,
                    'z':0.1207}

    ######## X, Y 모터 제어 코드 ########
    # 위치별 파라미터 setting
    ratio_x, ratio_y = x / 550, y / 550

    param_key_x, param_key_y = '', ''
    if ratio_x <= 0.45:
        param_key_x = 'x_left'
    elif 0.45 < ratio_x < 0.5:
        param_key_x = 'x_left_center'
    elif 0.5 <= ratio_x < 0.55:
        param_key_x = 'x_right_center'
    else:
        param_key_x = 'x_right'
    
    if ratio_y <= 0.4:
        param_key_y = 'y_upper'
    elif 0.4 < ratio_y <= 0.6:
        param_key_y = 'y_center'
    else:
        param_key_y = 'y_lower'
    
    print(f'[Selected Keys(X, Y)]: {param_key_x}: {params[param_key_x]}, {param_key_y}: {params[param_key_y]}')

    move_x = grip_x - x
    move_y = grip_y - y

    # 펄스 값 연산
    pulse_x, pulse_y = round(abs(move_x) * params[param_key_x]), round(abs(move_y) * params[param_key_y])

    # 제어 방향 설정
    dir_x = 1 if move_x >= 0 else 0
    dir_y = 0 if move_y >= 0 else 1

    # enable 설정
    ena_x, ena_y = 0, 0
    ######################################
    
    ######## 그리퍼, Z 모터 제어 코드 ########
    grip = 25 # 그리퍼 닫기
    
    if round(angle) < 5 or round(angle) >= 85: # 회전에 필요한 값 연산
        wrist = 20
    else:
        wrist = 20 - round(angle * params['z'])
    
    pulse_z, dir_z = 5500, 0
    ##########################################

    return (pulse_y, dir_y, ena_y, pulse_x, dir_x, ena_x, grip, wrist, pulse_z, dir_z)

def Py2Ard(cnt, info, baudrate):
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

    print(f'[물체 인식] xy_motor: {xy_data}, grip_z_motor: {grip_z_data}')
    
    # 시리얼 포트 오픈
    ard1 = serial.Serial('COM3', baudrate) # x,y 모터 포트
    ard2 = serial.Serial('COM4', baudrate) # z 모터 포트

    ard1.timeout, ard2.timeout = 1, 1
    
    # 로봇 이동을 위한 초기 데이터 전송
    time.sleep(2)
    ard1.write(xy_data.encode())
    time.sleep(8) # 4 -> 8 -> 16
    ard2.write(grip_z_data.encode())
    time.sleep(7) # z축 모터의 속도가 매우 느려서 24초라는 시간을 고정적으로 기다림. 알고리즘을 통해 가변적으로 변경 가능

    # 지정한 자리로 돌아가기 위한 조건문
    if xdir==0:
        xpul=xpul+3000
        xdir=1
    elif xdir==1:
        if xpul>3000: #더많이가면
            xpul=xpul-3000
            xdir=0
        else:
            xpul=3000-xpul
            xdir=1
    if ydir==0:
        if ypul>2000:
            ypul=ypul-2000
            ydir=1
        else:
            ypul=2000-ypul
            ydir=0
    elif ydir==1:
        ypul=ypul+2000
        ydir=0

    wpow = 20
    zdir = 1

    # 돌아가기 위한 데이터 병합
    x_data = str(xpul)+','+str(xdir)+'/'+str(xena)
    y_data = str(ypul)+','+str(ydir)+'/'+str(yena)
    xy_data = x_data+'|'+y_data

    gripper_data = str(fpow)+','+str(wpow)
    z_data = str(zpow)+','+str(zdir)
    grip_z_data = gripper_data+'|'+z_data

    print(f'[지정 위치로 돌아갈 때] xy_motor: {xy_data}, grip_z_motor: {grip_z_data}')

    # 지정 위치로 돌아가는 데이터 전송
    time.sleep(2)
    ard2.write(grip_z_data.encode())
    time.sleep(7)
    ard1.write(xy_data.encode())
    time.sleep(6)

    # 적재
    param_x = (cnt // 3)
    param_y = (cnt % 3)
    
    xpul, xdir, xena = 2500 * param_x, 1, 0
    ypul, ydir, yena = 8000 * param_y, 1, 0
    x_data = str(xpul) + ',' + str(xdir) + '/' + str(xena)
    y_data = str(ypul) + ',' + str(ydir) + '/' + str(yena)
    xy_data = x_data + '|' + y_data

    fpow, zdir = 13, 0 # 내려가기 위한 데이터
    gripper_data = str(fpow) + ',' + str(wpow)
    z_data = str(zpow) + ',' + str(zdir)
    grip_z_data = gripper_data + '|' + z_data

    print(f'[적재_down] xy_motor: {xy_data}, grip_z_motor: {grip_z_data}')
    time.sleep(2)
    ard1.write(xy_data.encode())
    time.sleep(4)
    ard2.write(grip_z_data.encode())
    time.sleep(7)

    zdir = 1 # 올라오기 위한 데이터
    z_data = str(zpow) + ',' + str(zdir)
    grip_z_data = gripper_data + '|' + z_data

    print(f'[적재_up] xy_motor: {xy_data}, grip_z_motor: {grip_z_data}')
    time.sleep(2)
    ard2.write(grip_z_data.encode())
    time.sleep(7)

    # 원래 자리로 돌아오는 데이터 선언
    xpul, xdir = 3000 + 2500 * param_x, 0
    ypul = abs(8000 * param_y - 2000)
    ydir = 0 if ypul > 2000 else 1

    # 원래 자리로 돌아오는 데이터 병합
    x_data = str(xpul)+','+str(xdir)+'/'+str(xena)
    y_data = str(ypul)+','+str(ydir)+'/'+str(yena)
    xy_data = x_data+'|'+y_data

    print(f'[초기 위치] xy_motor: {xy_data}, grip_z_motor: {grip_z_data}')
    
    # 최종 데이터
    time.sleep(2)
    ard1.write(xy_data.encode())
    time.sleep(7) # 2 > 8

    ard1.close()
    ard2.close()
