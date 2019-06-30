#include <pigpio.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <signal.h>


#define PWM_PIN 12 
#define PWM_DUTY 500000 

static volatile int done = 0;

void catch(int sig)
{
  done = 1; 
}

int main(int nargs, char ** args) 
{
  unsigned freq = 1000; 
  int ok; 

  if (nargs > 1) freq = atoi(args[1]); 
	
  printf("Using %u Hz\n", freq); 


  if (gpioInitialise() < 0) 
  {
    fprintf(stderr,"pigpio initialization failed :(.\n"); 
    return 1; 
  
  }

  //register exit handler 
  atexit(gpioTerminate); 

  ok = gpioHardwarePWM(PWM_PIN, freq, PWM_DUTY); 
  if (ok) 
  {
    fprintf(stderr,"gpioHardwarePWM(%u,%u,%u) returned %d\n", PWM_PIN,freq,PWM_DUTY, ok); 
    return 1; 
  }


  signal(SIGINT, catch); 
  printf("Initialization successful! Ctrl-c to stop.\n"); 



  while(!done) 
  {
    sleep(1); 
  }


  ok = gpioHardwarePWM(PWM_PIN, 0, PWM_DUTY); 
  if (ok)
  {
    fprintf(stderr, "Hmm, had trouble turning PWM off?"); 
  }
  printf("\ngoodbye!!\n"); 

  return 0; 
}
