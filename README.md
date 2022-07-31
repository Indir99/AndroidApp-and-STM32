## Android App for communicating with STM32F407 (HC-05/HC-06 module)

### Required equipment:

In this project we will use:

1. STM32F407G-DISC board
2. HC-05/HC-06 module
3. USB dongle for debugging

### Android Application

We have created a simple application to communicate with the hardware using the Bluetooth communication protocol.
The layout of the application is shown below:

<p align="center">
<img src="https://github.com/Indir99/AndroidApp-and-STM32/blob/master/images/app-photo.jpg" width="300" height="600" />
</[>

#### Button functions:

1. BLT ON -> Enable bluetooth
2. BLT OFF -> Disable bluetooth
3. CHECK IF HC IS PAIRED -> Scanning paired devices and trying to find HC-05/HC-06 (hardcoded for these two modules)
4. CONNECT TO HC MODULE -> Connecting (it will take a while)
5. TEST MESSAGE -> If connection is successful, sending "Text Message" on your device
6. LED1 -> sending "PD12#1" command on module which will turn on LED on peripheral PD12 on STM32 disco board
7. LED2, LED3, LED4 -> same as previous command

#### Quick guide

1. First, you have to pair your phone with your HC Modlue
2. In app, turn on bluetooth
3. Check if HC module is paired
4. Connect your devices for communication
5. Test app

### Hardware application

First, you have to connect your STM32 Disco board with usb dongle and HC module. In this case (check it in usart.c file):
1. USB dongle RX -> PA2
2. USB dongle TX -> PA3
3. HC module RX -> PD8
4. HC module TX -> PD9

After connecting, you need to launch the application. For debugging (not necessary) you can use picocom in linux terminal:
```
sudo picocom/dev/ttyUSB0 -b 115200
```
Reset the disco board. In Terminal you should see:
<p align="center">
<img src="https://github.com/Indir99/AndroidApp-and-STM32/blob/master/images/terminal1.png" width="535" height="136" />
</[>

Now, everything is ready, so you can communicate.

#### Quick guide part 2
1. First, you have to pair your phone with your HC Modlue
2. In app, turn on bluetooth
3. Check if HC module is paired
4. Connect your disco borad
5. Press "Connect to HC module" button (andorid app side) and wait to connect
6. Turn your leds

At the following link you can see how the app work:
```
https://www.dropbox.com/s/c3xesxhcbhvd5dd/STM32-LEDS.mp4?dl=0
```


