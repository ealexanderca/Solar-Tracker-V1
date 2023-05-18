#include <math.h>
#include <NeoSWSerial.h>
#include <TinyGPS++.h>
#include <AccelStepper.h>

int count=0;
int directionh=-1;
int directionv=1;
int sh=2;
int sv=2;
int endh=A0;
int endv=A1;
static const int RXPin = 10, TXPin = 11;
static const uint32_t GPSBaud = 9600;

AccelStepper horizontal(AccelStepper::FULL4WIRE, 2, 3, 4, 5);
AccelStepper vertical(AccelStepper::FULL4WIRE, 6, 7, 8, 9);

TinyGPSPlus gps;
NeoSWSerial ss(RXPin, TXPin);
float year,month,day,hour,latitude,longitude,JD,JC,GMLS,GMAS,EEO,SEOC,STL,vary,EQT,SAL,MOE,OCorrect,ADecline,AHour,AZenith,AElevation,AAzimuth,TST,AEAdjusted;
void setup() {
  Serial.begin(9600);
  pinMode(endv, INPUT);
  pinMode(endh, INPUT);
  horizontal.setMaxSpeed(50);
  horizontal.setAcceleration(1000);
  vertical.setMaxSpeed(50);
  vertical.setAcceleration(1000);
  homes();
  ss.begin(9600);
}

void homes(){
  
  vrun=0,hrun=0;
  horizontal.move(-600*directionh);
  while(digitalRead(endh)){
    horizontal.run();
  }
  horizontal.stop();
  vertical.move(-800*directionv);
  while(digitalRead(endv)){
    vertical.run();
  }
  vertical.stop();
  horizontal.setCurrentPosition(0);
  vertical.setCurrentPosition(0);
  Serial.println("hi");
}

void hcontrol(){
  //if(AAzimuth>0&&AAzimuth<M_PI*35.0/18.0&&AAzimuth*200.0/M_PI*sh!=directionh*horizontal.currentPosition()){
    horizontal.moveTo(AAzimuth*100.0/M_PI*sh*directionh);
  //}
}

void vcontrol(){
  //if(AEAdjusted<M_PI/2&&AEAdjusted>0&&AEAdjusted*200.0/M_PI*sv!=directionv*vertical.currentPosition()){
    vertical.moveTo(AEAdjusted*100.0/M_PI*sv*directionv);
 // }
}

void calculate() {
  
    if(gps.altitude.meters()!=0){
      latitude = gps.location.lat();
      longitude = gps.location.lng();
      day = gps.date.day();
      month = gps.date.month();
      year = gps.date.year();
      hour = gps.time.hour()+gps.time.minute()/60.0+gps.time.second()/3600.0;
      
      JD =367*year-static_cast<int>((7*(year+static_cast<int>((month+9)/12)))/4)+static_cast<int>((275*month)/9)+day+1721013.5+hour/24-0.5*(100*year+month-190002.5>0?1:-1)+0.5;
      JC=(JD-2451545.0)/36525.0;
      GMLS=(280.46646+JC*(36000.76983+JC*0.0003032));
      while(static_cast<int>(GMLS)>=360){GMLS-=360.0;}
      GMLS*=M_PI/180.0;
      GMAS=(357.52911+JC*(35999.05029-0.0001537*JC))*M_PI/180.0;
      EEO=0.016708634-JC*(0.000042037+0.0000001267*JC);
      SEOC=sin(GMAS)*(1.914602-JC*(0.004817+0.000014*JC))+sin(2.0*GMAS)*(0.019993-0.000101*JC)+sin(3*GMAS)*0.000289;
      STL=SEOC*M_PI/180.0+GMLS;
      SAL=STL+(-0.00569-0.00478*sin((125.04-1934.136*JC)*M_PI/180.0))*M_PI/180.0;
      MOE=(23.0+(26.0+((21.448-JC*(46.815+JC*(0.00059-JC*0.001813))))/60.0)/60.0)*M_PI/180.0;
      OCorrect=MOE+M_PI/180.0*0.00256*cos(M_PI/180.0*(125.04-1934.136*JC));
      ADecline=asin(sin(OCorrect)*sin(SAL));
      vary=tan(OCorrect/2.0)*tan(OCorrect/2.0);
      EQT=4*(vary*sin(2.0*GMLS)-2.0*EEO*sin(GMAS)+4.0*EEO*vary*sin(GMAS)*cos(2.0*GMLS)-0.5*vary*vary*sin(4.0*GMLS)-1.25*EEO*EEO*sin(2.0*GMAS))*180.0/M_PI;
      TST=(hour/24.0*1440.0+EQT+4.0*longitude);
      while(static_cast<int>(TST)>=1440){TST-=1440.0;}
      while(static_cast<int>(TST)<0){TST+=1440.0;}
      AHour=(TST/4<0?TST/4.0+180.0:TST/4.0-180.0)*M_PI/180.0;
      AZenith=acos(sin(latitude*M_PI/180.0)*sin(ADecline)+cos(latitude*M_PI/180.0)*cos(ADecline)*cos(AHour));
      AElevation=M_PI/2.0-AZenith;
      AEAdjusted=AElevation;
      if(AElevation>17.0*M_PI/36.0){
      } else if (AElevation>M_PI/36.0){
        AEAdjusted+=(58.1/tan(AElevation)-0.07/pow(tan(AElevation),3)+0.000086/pow(tan(AElevation),5))*M_PI/180.0/3600.0;
      } else if (AElevation>-23.0*M_PI/7200.0){
        AEAdjusted+=(1735.0+AElevation*180.0/M_PI*(-518.2+AElevation*180.0/M_PI*(103.4+AElevation*180.0/M_PI*(-12.79+AElevation*180.0/M_PI*0.711))))*M_PI/180.0/3600.0;
      } else {
        AEAdjusted=0;
      }
      AAzimuth=(AHour>0?acos(((sin(latitude*M_PI/180.0)*cos(AZenith))-sin(ADecline))/(cos(latitude*M_PI/180.0)*sin(AZenith)))+M_PI:3.0*M_PI-acos(((sin(latitude*M_PI/180.0)*cos(AZenith))-sin(ADecline))/(cos(latitude*M_PI/180.0)*sin(AZenith))));
      while(static_cast<int>(AAzimuth)>=2*M_PI){AAzimuth-=2.0*M_PI;}
      while(static_cast<int>(AAzimuth)<0){AAzimuth+=2.0*M_PI;}
      Serial.println(latitude);
      Serial.println(longitude);
      Serial.println(AEAdjusted);
      Serial.println(AAzimuth);
      Serial.println(static_cast<int>(AAzimuth*200.0/2.0/M_PI*sh*directionh));
    };
}

void loop(){
  while (ss.available() > 0)
  gps.encode(ss.read());
  if(millis()/1000>=count){
    calculate();
    count++;
  }
  vcontrol();
  hcontrol();
  /*if((horizontal.currentPosition()!=0||vertical.currentPosition()!=0)&&AEAdjusted<0){
    homes();
  }*/
  vertical.run();
  horizontal.run();
}
