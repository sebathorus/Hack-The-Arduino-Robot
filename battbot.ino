
#include <ArduinoRobot.h>
#include <Servo.h>

int motor;
int irsensorval, irsensornumber;
int backlight = 0;
int deliv_pos=0;//delivery position - the tube where batteries are droped
int x = 100;
int val_eject = 10;
int mhp = 79;//mobile_hatch_pos
int sensor1,sensor2,sensor3;
int aa=0;//initialize AA and AAA counter
int aaa=0;

Servo hatch;
Servo mobile_hatch;
Servo eject;

void setup() 
{
  // initialize the robot
  Robot.begin();
  //Serial.begin(9600);
  pinMode(TK0, INPUT);
  pinMode(TK1, INPUT);
  pinMode(TK2, INPUT);
  // initialize the robot's screen
  Robot.beginTFT();
  eject.attach(TKD1);
  hatch.attach(TKD2);
  mobile_hatch.attach(TKD3);
}

void batt_drop()
    {
      irsensornumber = 1;
 while(irsensornumber<=deliv_pos)
  {
   motor = -90;
   Robot.updateIR(); // update the IR array
   irsensorval = Robot.IRarray[irsensornumber];
   while(irsensorval<650)
    {
      delay(50);
      Robot.updateIR(); // update the IR array
      irsensorval = Robot.IRarray[irsensornumber];
      Robot.motorsWrite(motor, backlight);
      
    }
    delay(50);
    Robot.motorsWrite(0, backlight);
    //delay(100);
    irsensornumber ++;
  }
  for(x=100;x>=40;x--)
    {
     hatch.write(x);
     delay(40);
    }
  for(x=40;x<145;x++)
    {
     hatch.write(x);
     delay(40);
    }
  for(x=145;x>=100;x--)
    {
     hatch.write(x);
     delay(40);
    }
 delay(2000);  
irsensornumber--;
Robot.updateIR();

while(irsensornumber>=0)//return to zero
  {
   motor = 90;
   Robot.updateIR(); // update the IR array
   irsensorval = Robot.IRarray[irsensornumber];
   while(irsensorval<650)
    {
      delay(50);
      Robot.updateIR(); // update the IR array
      irsensorval = Robot.IRarray[irsensornumber];
      Robot.motorsWrite(motor, backlight);
      
    }
    Robot.motorsWrite(0, backlight);
    //delay(100);
    irsensornumber--;
   }//end return to zero
  }

void release_batt()//AA and AAA batteries are released from selector
     {
     for(mhp=79;mhp<130;mhp++)
      {
       mobile_hatch.write(mhp);
       delay(40);
      }
     for(mhp=130;mhp>=79;mhp--)
      {
       mobile_hatch.write(mhp);
       delay(40);
      }
     }
     
void eject_batt()//bigger batteries are dropped directly to bin
     {
     for(val_eject=10;val_eject<80;val_eject++)
      {
       eject.write(val_eject);
       delay(40);
      }
     for(val_eject=80;val_eject>=10;val_eject--)
      {
       eject.write(val_eject);
       delay(40);
      }
     }

void loop() 
{
 deliv_pos = 0;
 hatch.write(x);
 eject.write(val_eject);
 mobile_hatch.write(mhp);
 
 sensor1 = Robot.digitalRead(TK0);
 sensor2 = Robot.digitalRead(TK1);
 sensor3 = Robot.digitalRead(TK2);
    
 while((Robot.digitalRead(TK0)==0) && (Robot.digitalRead(TK1)==0) && (Robot.digitalRead(TK2)==0))
   {
     sensor1 = Robot.digitalRead(TK0);
     sensor2 = Robot.digitalRead(TK1);
     sensor3 = Robot.digitalRead(TK2);
     //Serial.println(sensor1); 
     //Serial.println(sensor2);
     //Serial.println(sensor3);
     Robot.stroke(0, 0, 0); // choose the color black
     Robot.text("READY !!!", 40, 80);
     delay(1000);
     Robot.stroke(255, 255, 255);// choose the color white
     Robot.text("READY !!!", 40, 80);
   }
 if(sensor1==1)
   {
    //Serial.println("Eject big battery");
    eject_batt();
    
   }
 if(sensor2==1)
   {
    release_batt();
    aa++;
    if(aa<=7) deliv_pos = 1;
    if((aa>7)&&(aa<=14)) deliv_pos = 2;
    if(aa>14)
       {
        deliv_pos =1;
        aa=0;
       }
    /*Serial.println("Battery is AA.");
    Serial.println(aa);*/
   }
 if(sensor3==1)
   {
    release_batt();
    aaa++;
    if(aaa<=7) deliv_pos = 3;
    if((aaa>7)&&(aaa<=14)) deliv_pos = 4;
    if(aaa>14)
       {
        deliv_pos =3;
        aaa=0;
       }
   /*Serial.println("Battery is AAA.");
   Serial.println(aaa);*/
 }
 
 Robot.stroke(0, 0, 0); // choose the color black
 if(sensor2==1) Robot.text("Battery type is AA", 0, 0);  // print the text
 if(sensor3==1) Robot.text("Battery type is AAA", 0, 0);
 
 if(deliv_pos>0) batt_drop();
 
 Robot.stroke(255, 255, 255);
 if(sensor2==1) Robot.text("Battery type is AA", 0, 0);  // print the text
 if(sensor3==1) Robot.text("Battery type is AAA", 0, 0);
}
