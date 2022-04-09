#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>


#define TRUE 1
#define FALSE 0

#define TG 1
#define TY 2
#define TR 3
#define PedestrianWaiting1 4

#define PR 1
#define PG 2
#define PedestrianWaiting 3

int T_state = TG;

int pass = FALSE;

int greenpass = FALSE;

int msg = FALSE;

int Button= FALSE;

int P_state = PR;

int msg1= FALSE;


void Traffic_light_function() {

  switch(T_state){

    case TG:
    	greenpass = TRUE;
    	msg = FALSE;
      printf("The traffic_green light is on.\n");
    	sleep(10);

            if(Button==TRUE){//1a
              T_state = PedestrianWaiting1;
            }//1a

        T_state = TY;


    case TY:
    	printf("The traffic_yellow light is on.\n");
      sleep(5);

    	if(greenpass==TRUE){//1a
          T_state = TR;
        }//1a

    	if(Button==TRUE){//1a
          T_state = TR;
        }//1a

    	if((greenpass==FALSE)&&(msg1==TRUE)){//1a
        	T_state= TG;
        }//1a



   case TR:
      printf("The traffic_red light is on.\n");

      Button = FALSE;
    	pass = TRUE;
      msg = TRUE;
    	greenpass = FALSE;
    	sleep(10);

    	T_state = TY;

    case PedestrianWaiting1:
      if(Button==TRUE){//1a
        T_state= TY;
      }//1a

  }
}



void Pedestrian_light_function(){

  switch (P_state){
  	case PR:
      printf("The Pedestrian_red light is on.\n");
    	pass = FALSE;
    	sleep(10);
      P_state = PG;

    	if(Button==TRUE){//1a
        	P_state = PedestrianWaiting;
        }//1a

    case PG:
    	printf("The Pedestrian_green light is on.\n");
      P_state = PR;
    	sleep(10);

    case PedestrianWaiting:
      if(pass ==TRUE){//1a
        P_state = PG;
      }//1a
  }

}

void* tloop(){
  while(TRUE){
    Traffic_light_function();
  }
}

void* ploop(){
  while(TRUE){
    Pedestrian_light_function();
  }
}


int main(int argc, char* argv[]){
  pthread_t thread1, thread2;

  if(pthread_create(&thread1,NULL,&tloop,NULL) != 0){
    return 1;
    printf("Error 1");
  }
  if(pthread_create(&thread2,NULL,&ploop,NULL) !=0){
    return 2;
    printf("Error 2");
  }

  if(pthread_join(thread1,NULL) !=0){
    return 3;
    printf("Error 3");
  }
  if(pthread_join(thread2,NULL) !=0){
    return 4;
    printf("Error 4");
  }

  //create server socket
  int server_socket;
  server_socket = socket(AF_INET,SOCK_STREAM,0);

  //define the sever address
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(9002);
  server_address.sin_addr.s_addr = INADDR_ANY;

  //bind the socket to our specified IP and port
  bind(server_socket,(struct sockaddr*) &server_address,sizeof(server_address));

  //listen for connections
  listen(server_socket,1);

  //accept connection
  int client_socket = accept(server_socket,NULL,NULL);

  for(;;){
  //send data to client
  send(client_socket,msg,sizeof(msg),0);
  //receive data from client
  recv(client_socket,&msg1,sizeof(msg1),0);
  }

  return 0;
}
