# Solar Tracker V1
This is a 3D model and cad for a serial 2-DOF actuator that rotates in the azimuth and elevation.

## Components

- [Stepper Driver](https://www.droking.com/L298N-Dual-H-Bridge-Motor-Driver-Module-DC-5V-35V-2A-25W-Power-Supply-Module-Driver-Module-for-Step-motor-Smart-car-Robot-etc)
- [42x38 mm Stepper Motor](https://www.amazon.ca/SIMAX-3D-Creality-Printers-Extruder/dp/B07TY6B6DS/ref=d_pd_sbs_sccl_2_3/139-9931410-4958941?pd_rd_w=crJoH&content-id=amzn1.sym.c1849561-aea0-48eb-b576-ba648ed69471&pf_rd_p=c1849561-aea0-48eb-b576-ba648ed69471&pf_rd_r=2RBT1ZTY2THE9M7N53YA&pd_rd_wg=vJUM3&pd_rd_r=9bfae479-3045-4208-9504-5be424a80949&pd_rd_i=B07TY6B6DS&th=1)
- [GPS Module](https://www.amazon.ca/Robojax-U-blox-NEO-6M-Module-Antenna/dp/B073MFYL8B)
- [Arduino Uno](https://store.arduino.cc/products/arduino-uno-rev3)

## Code
The code in this script uses the positioning and time data from the GPS to calculate the angle of the sun. It does this through a open loop algorithm provided by the [NOAA](https://www.noaa.gov/) which I have programed into the calculate function in the script. If you follow the wiring diagram below you should be able to upload the code and get it running.

## Wiring Diagram
Below is a wiring diagram for how all the motors and controllers are hooked up

<img src=https://raw.githubusercontent.com/ealexanderca/Solar-Tracker-V1/main/extras/Wiring.png alt='missing'>