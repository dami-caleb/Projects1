#define TRUE 1
#define FALSE 0

#define PR 1
#define PG 2
#define PedestrianWaiting 3

int P_state = PR;

int push_button = 2;

int pass;

volatile int Button;

int msg1;

int Pin_P_red=9;
int Pin_P_green=8;

void LED(int ledPin, int status){
  digitalWrite(ledPin, status);
}

void setup(){


  	Serial.begin(96000);

    pinMode(push_button, INPUT_PULLUP);
  	attachInterrupt(digitalPinToInterrupt(push_button),button_func,CHANGE);// "RISING" because we are using an "active-high" push button


}


void loop(){

  switch (P_state){
  	case PR:
      LED(Pin_P_green,FALSE);
      LED(Pin_P_red, TRUE);

    	pass = FALSE;
      P_state = PG;
    	delay(10000);

    	if(Button==TRUE){//1a
        	P_state = PedestrianWaiting;
        }//1a



    case PG:
      LED(Pin_P_red,FALSE);
      LED(Pin_P_green, TRUE);

      P_state = PR;
    	delay(10000);

    	if(Button==1){//1a
        	P_state = PedestrianWaiting;
        }//1a

    case PedestrianWaiting:

    	if(Serial.available() > 0){//2a
        	pass = Serial.read();
          delay(500); //time to read
          	if(pass ==TRUE){//1a
            	P_state = PG;
            }//1a
        }//2a

  }

}

void button_func(){
	Button = TRUE;
  	Serial.write(Button);
    //delay(50); //time to send
}
//
void LED(int ledPin, int status){
  digitalWrite(ledPin, status);
}
