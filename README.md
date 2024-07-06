# Device Driver for Step Motor
- This is for operating step motor by using Hardware Timer(hr_timer) 

## Raspbian Kernel Version
6.1.21-v7l+

## Operating System
Raspbian 32bit Legacy

## Hardware 
- Raspberrypi 4 ( RAM : 8 GB ) 
- Step Motor: [ULN2003 + Motor Module](https://www.devicemart.co.kr/goods/view?no=1327608)

### Pin Map 
| Pin Number|Purpose|
|----|--------------------------|
|GPIO12|GlassMotor IN1|
|GPIO16|GlassMotor IN2|
|GPIO20|GlassMotor IN3|
|GPIO21|GlassMotor IN4|
|GPIO24|ShieldMotor IN1|
|GPIO25|ShieldMotor IN2|
|GPIO19|ShieldMotor IN3|
|GPIO1|ShieldMotor IN4|
|GPIO23|TiltMotor IN1|
|GPIO18|TiltMotor IN2|
|GPIO15|TiltMotor IN3|
|GPIO14|TiltMotor IN4|

## Demonstration
### 시연 영상
<br></br>
[![시연 영상](https://img.youtube.com/vi/NBITtxIuxkI/0.jpg)](https://www.youtube.com/watch?v=NBITtxIuxkI)

클릭하여 시연 영상을 시청할 수 있습니다.



## Kernel


### Usage(English) 

1. When installing Raspbian, add "arm_64bit = 0" to the config.txt file. If you do not do this, the kernel will be updated to 64-bit.

2. Enter root mode
```bash
sudo su
```

3. To check the current kernel version on a Linux system, including Raspbian, you can open a terminal and enter the following command
```bash
uname -r
```

4. Update Linux kernel headers
```bash
sudo agt-get install linux-headers
```

5. Update the Linux kernel on Raspberry Pi
```bash
sudo apt install raspberrypi-kernel-headers
```

6. Enter 'make' in the 'Drivers' folder(directory). This is for Driver building, driver permission change, driver kernel registration.
```bash
make
```

7. Enter 'ls -al' in the '/dev/' folder to check the permissions of the driver.
```bash
ls -al
```


8. Enter 'make' in the 'test' directory
```bash
make
```

9. Enter './test' in the 'test' folder to test the operation of the motor driver.
```bash
./test
```

10. If the Motor Spec does not show 2ms after completing the above steps, allow overclocking in the config.txt file by increasing the CPU frequency from 700Hz to 800Hz (up to a maximum of 1000Hz).



### Usage(Korean)

1. 라즈비안 설치 시 config.txt 파일에 arm_64bit = 0 을 적어준다.
   이를 진행하지 않을 경우 64비트 커널로 업데이트가 된다. 

2. 'sudo su' 을 입력한다.
```bash
sudo su
```

3. 'uname -r'를 입력해서 현재 커널 버젼을 확인한다. ( 프로젝트 커널 버젼: 6.1.21-v7l+)
```bash
uname -r
```

4. 리눅스 커널 헤더 업데이트를 한다.
```bash
sudo agt-get install linux-headers
```

5. 라즈베리 파이의 리눅스 커널 업데이트를 한다.
```bash
sudo apt install raspberrypi-kernel-headers
```

6.  Drivers 폴더에서 'make'를 입력한다. => 드라이버 빌드, 드라이버 권한 변경, 드라이버 커널 등록
```bash
make
```

7. /dev/폴더에서 'ls -al'을 입력해서 드라이버의 권한을 확인한다.
```bash
ls -al
```

8. test 폴더에서 'make'를 입력한다.

```bash
make
```

9. test 폴더에서 './test'를 입력하고 모터 드라이버의 동작을 확인한다.
```bash
./test
```

10. 위 단계를 진행했을 때 2ms의 Motor Spec을 보이지 않는다면 config.txt파일에 CPU Frequency를 700Hz에서 800Hz로 Overclocking을 허용해준다. ( 최대 1000Hz )



# Driver

Match Raspberry Pi Board Pin Number with Step Motor in GlassDriver.c(TiltMotor,ShileMotor etc) code
<br></br>
![디브_하드웨어](https://github.com/NahyunEE/DeviceDriver_StepMotor/assets/50420981/82a2105c-faa8-4e00-862c-1ec4813f7028)

# Prerequisites
[Linux Driver Tutirial](https://embetronicx.com/linux-device-driver-tutorials/)

# Reference
https://github.com/NahyunEE/smart_sunroof



## License
MIT License
