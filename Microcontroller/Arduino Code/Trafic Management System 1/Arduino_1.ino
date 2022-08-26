//Master for I2C communication

#include <Wire.h> //This header file makes I2C communication in Arduino possible

#define SLAVE_ADDRESS 1
#define ANSWERSIZE 2

#define TRUE 1
#define FALSE 0

#define TG 1
#define TY 2
#define TR 3
#define PedestrianWaiting1 4





int T_state = TG;

int pass;

int greenpass;

int msg;

int Pin_T_red=13;
int Pin_T_yellow =12;
int Pin_T_green = 11;

int Button;




void setup(){
  Wire.begin();
  Serial.begin(9600);
}

void loop(){
  delay(50);

  switch(T_state){

    case TG:
        LED(Pin_T_red,FALSE);
        LED(Pin_T_yellow,FALSE);

        LED(Pin_T_green,TRUE);
        greenpass = TRUE;
        T_state = TY;

        //write data to slave
        msg = FALSE;
        Wire.beginTransmission(SLAVE_ADDRESS);
        Wire.write(msg);
        Wire.endTransmission();

        delay(10000);

        if(Serial.available()>0){//2a

              //read serial data
            Button = Serial.read();
            delay(500); //time to read

            if(Button==TRUE){//1a
              T_state = PedestrianWaiting1;
            }//1a






        }//2a

    case TY:
        LED(Pin_T_green,FALSE);
        LED(Pin_T_red,FALSE);

        //read data from slave
        Wire.requestFrom(SLAVE_ADDRESS,ANSWERSIZE);
        while (Wire.available()){
          int msg1 = Wire.read();
      }


        if(greenpass==TRUE){//1a
          LED(Pin_T_yellow,TRUE);
          T_state = TR;
          delay(5000);
        }//1a

        if(Button==TRUE){//1a
          LED(Pin_T_yellow,TRUE);
          T_state = TR;
          delay(5000);
        }//1a

        if((greenpass==FALSE)&&(msg1==TRUE)){//1a
            LED(Pin_T_yellow,TRUE);
            T_state= TG;
            delay(5000)
        }//1a



   case TR:
        LED(Pin_T_yellow,FALSE);
        LED(Pin_T_green,FALSE);

        LED(Pin_T_red,TRUE);

        pass = TRUE;
        Serial.write(pass);
        delay(500); //time to send
        Button =0;
        greenpass = FALSE;

        //write data to slave
        msg = TRUE;
        Wire.beginTransmission(SLAVE_ADDRESS);
        Wire.write(msg);
        Wire.endTransmission();

        delay(10000);

        T_state = TY;



    case PedestrianWaiting1:
      if(Button==TRUE){//1a
        T_state= TY;
      }//1a

  }





}
//
void LED(int ledPin, int status){
  digitalWrite(ledPin, status);
}
