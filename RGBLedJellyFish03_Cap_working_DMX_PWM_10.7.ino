 #include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();


//  HI MATT HERE IS MY CHANGE


#include <Wire.h>
#include <SPI.h>
#include <Adafruit_CAP1188.h>

#include <Conceptinetics.h>
#define DMX_MASTER_CHANNELS 4
#define RXEN_PIN            2
DMX_Master        dmx_master ( DMX_MASTER_CHANNELS, RXEN_PIN );
Adafruit_CAP1188 cap = Adafruit_CAP1188();

int nowLight[3] = {0,0,0};     // contains 8 bit values to send DMX
int r = 0;
int g = 1;
int b = 2;
int dmxCh = 1;
//float grandMaster;
int gmPin = 3;
int pwmPins = 8;  //Number of PWM Pins used
int selectedKey = -1;
int touched=0;
int color = 8;
int colorCh = 3;
int pwmnum;
int currentColor;
int newColor;
int storedColor[8][3] = {
  //Red  Green Blue
  {255,  0,    0  },  
  {0,    255,  0  },
  {0,    0,    255},
  {255,  130,  20  },
  {255,  51,    255},
  {255,  255,  102},
  {255,  255,  255},
  {127,    0,    255, }//{67,    43,    200, } <- Nice Color
};

void setup(){
    //pinMode(sensePin[pin],INPUT_PULLUP);
    pinMode(17, INPUT);      // for GM Pot
    cap.begin(0x28);        //initilizes capacative driver board
    pwm.begin();
    pwm.setPWMFreq(1600);  // This is the maximum PWM frequency
    for(int i=0; i < pwmPins; i++){    
      pwm.setPWM(i,0,0);                  //Preset all PWM pins to LOW    
    }
  dmx_master.enable ();
}

int lowLevel = 16;
int highLevel = 128;
int numberOfLeds = 8;

void loop(){
   //grandMaster = map(analogRead(gmPin),0,1023,0,100);
    capSense();
    throbUp(28);
    throbDown(6);
    throbUp(28);
    throbDown(2);      // fade time
    sendColor(180);    //input fade time
    
    dmxThrobDown(3);  
    capSense();
    throbUp(28);
    throbDown(6);
    throbUp(28);
    throbDown(2);
    dmxThrobUp(3);
}  

void throbUp(int t){
for(int i = lowLevel; i < highLevel; i+= t){
  for(int j = 0; j < numberOfLeds; j++){
    capSense();
    if(j != selectedKey){
      pwm.setPWM(j,0,i);
      }
     //pwm.setPWM(selectedKey,0,1024 - i*16); 
    }
  }
}

void throbDown(int t){
for(int i = highLevel; i > lowLevel; i-= t){
  for(int j = 0; j< numberOfLeds; j++){
    capSense();
    //sendColor(180);    //
    if(j != selectedKey){
    pwm.setPWM(j,0,i);
    }
    //pwm.setPWM(selectedKey,0,1024-(i*16)); 
  }
}
}

void dmxThrobDown(int t){
for(int x = 1000; x >= 500; x-=t){  
    makeLight(nowLight[r]*.001*x,nowLight[g]*.001*x,nowLight[b]*.001*x);
    //delay(1);
    capSense();
    }
}
    
void dmxThrobUp(int t){
for(int x = 500; x <= 1000; x+=t){
    makeLight(nowLight[r]*.001*x,nowLight[g]*.001*x,nowLight[b]*.001*x);
    //delay(1);
    capSense();
  }
}

void sendColor(int t){
if(selectedKey != currentColor){
      for(int dim = 0; dim < 255; dim++){
        //capSense();
          for(int i = 0; i<3; i++){
            if(storedColor[selectedKey][i] > nowLight[i]){
              nowLight[i] == nowLight[i]++;
              }if(storedColor[selectedKey][i] < nowLight[i]){
                nowLight[i] = nowLight[i]--;
                }
          }makeLight(nowLight[r],nowLight[g],nowLight[b]);
        }currentColor = selectedKey;
      }
}
    
int capSense(){
  touched = cap.touched();
  if(touched >= 1 && touched < 2){
    // // // Serial.println("1");
    selectedKey = 0;
  }if(touched >= 2 && touched < 4){
    // // // Serial.println("2");
      selectedKey = 1;
  }if(touched >= 4 && touched < 8){
    // // Serial.println("3");
      selectedKey = 2;
  }if(touched >= 8 && touched < 16){
    // // Serial.println("4");
      selectedKey = 3;
  }if(touched >= 16 && touched < 32){
    // // Serial.println("5");
      selectedKey = 4;
  }if(touched >= 32 && touched < 64){
    // // Serial.println("6");
      selectedKey = 5;
  }if(touched >= 64 && touched < 128){
    // // Serial.println("7");
      selectedKey = 6;
  }if(touched >= 128 && touched < 255){
    // // Serial.println("8");
      selectedKey = 7;
  }
  pwm.setPWM(selectedKey,0,1000);
  return selectedKey;                
}


void pulseLight(){
    for(int x = 100; x > 60; x--){
    makeLight(nowLight[r]*(x/100),nowLight[g]*(x/100),nowLight[b]*(x/100));
    }
delay(250);
for(int x = 60; x > 100; x++){
    makeLight(nowLight[r]*(x/100),nowLight[g]*(x/100),nowLight[b]*(x/100));
    }
  delay(1000);
}
  
void makeLight(int outR, int outG, int outB){
  //  outR = outR*(grandMaster/100);
  //  outG = outG*(grandMaster/100);
  //  outB = outB*(grandMaster/100);
    dmx_master.setChannelValue(dmxCh+0,outR);
    dmx_master.setChannelValue(dmxCh+1,outG);
    dmx_master.setChannelValue(dmxCh+2,outB);
    delay(10);
}
