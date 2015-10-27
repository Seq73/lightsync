#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <RF24/RF24.h>
#include <unistd.h>
#include <time.h>

using namespace std;
RF24 radio(RPI_V2_GPIO_P1_15, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_8MHZ);
const uint64_t pipes[3]={0xF0F0F0F0E1LL,0xF0F0F0F0D2LL,0xF0F0F0F0F0LL};

int main(int argc, char** argv){
  radio.begin();
  radio.enableDynamicPayloads();
  radio.setRetries(15,15);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MAX) ;
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);
  radio.openReadingPipe(2,pipes[2]);
  radio.printDetails();

  float milliseconds;
  while (1){
    timeval curTime;
    gettimeofday(&curTime, NULL);
    milliseconds=curTime.tv_usec/1000;
    if(milliseconds<=1)break;
  }
//printf("Millis: %f\n",milliseconds);

  time_t timer;
  time(&timer);
  char send_payload[29];
  sprintf(send_payload,"time_t:%li:%li",timer,timegm(localtime(&timer)));

  radio.stopListening();

  printf("Payload: %s size: %i",send_payload,sizeof(send_payload));
  radio.write(send_payload,sizeof(send_payload));
}
