#include "ads1115.h"
#include <stdio.h>
#include <sys/types.h> // open
#include <sys/stat.h>  // open
#include <fcntl.h>     // open
#include <unistd.h>    // read/write usleep
#include <stdlib.h>    // exit
#include <inttypes.h>  // uint8_t, etc
#include <linux/i2c-dev.h>
#include <signals.h>
#include <string.h>
#include <time.h>

#define FileName "voltageLog.csv"

//ADC setup function to set the rate mode and gain and initialize comms
void ads1115_setup(uint8_t rate, uint8_t mode, uint8_t gain, int fd, int address){
  if(initialize_ads1115(fd, address)){
    printf("Comms with ads1115 established.");
  } //initialize the ADC
  setmode(fd, mode);
  setrate(fd, rate);
  setGain(fd, gain);
  if (getMode(fd)){
        printf("Power down single shot mode\n");
    }
    else {
        printf("Continuous conversion mode\n");
    }
  int set_gain = getGain(fd);
  switch(set_gain){
    case ADS1115_PGA_6P144:
      printf("Gain is set at 2/3");
      break;
    case ADS1115_PGA_4P096:
      printf("Gain is set at 1");
      break;
    case ADS1115_PGA_2P048:
      printf("Gain is set at 2");
      break;
    case ADS1115_PGA_1P024:
      printf("Gain is set at 4");
      break;
    case ADS1115_PGA_0P512:
      printf("Gain is set at 8");
      break;
    case ADS1115_PGA_0P256:
      printf("Gain is set at 16");
      break;
    case ADS1115_PGA_0P256B:
      printf("Gain is set at 16");
      break;
    case ADS1115_PGA_0P256C:
      printf("Gain is set at 16");
      break;
  }
  int set_rate = getRate(fd)l;
  switch(set_rate){
    case ADS1115_RATE_8:
      printf("Gain is set at 8 SPS");
      break;
    case ADS1115_RATE_16:
      printf("Gain is set at 16 SPS");
      break;
    case ADS1115_RATE_32:
      printf("Gain is set at 32 SPS");
      break;
    case ADS1115_RATE_64:
      printf("Gain is set at 64 SPS");
      break;
    case ADS1115_RATE_128:
      printf("Gain is set at 128 SPS");
      break;
    case ADS1115_RATE_250:
      printf("Gain is set at 250 SPS");
      break;
    case ADS1115_RATE_475:
      printf("Gain is set at 475 SPS");
      break;
    case ADS1115_RATE_860:
      printf("Gain is set at 860 SPS");
      break;
  }
}

void report_and_exit(const char* msg) {
  perror(msg);
  exit(-1); /* EXIT_FAILURE */
}

int main() {

  time_t logInterval = 1000; //log rate in ms
  time_t now_time = 0;
  time_t last_time = 0;

  struct flock lock;
  lock.l_type = F_WRLCK;    /* read/write (exclusive versus shared) lock */
  lock.l_whence = SEEK_SET; /* base for seek offsets */
  lock.l_start = 0;         /* 1st byte in file */
  lock.l_len = 0;           /* 0 here means 'until EOF' */
  lock.l_pid = getpid();    /* process id */

  int voltage_fd;
   //initialize the ADC, defualt rate 128 sps, continous mode, 1x gain, default address
   ads1115_setup(ADS1115_RATE_128, ADS1115_MODE_CONTINUOUS, ADS1115_PGA_4P096, voltage_fd, ADS1115_ADDRESS_ADDR_GND);

   //open file and insert header if not already inserted. 


  int fd; /* file descriptor to identify a file within a process */
  if ((fd = open(FileName, O_RDWR | O_CREAT, 0666)) < 0)  /* -1 signals an error */
    report_and_exit("open failed...");

  if (fcntl(fd, F_SETLKW, &lock) < 0) /** F_SETLK doesn't block, F_SETLKW does **/
    report_and_exit("fcntl failed to get lock...");
  else {
    write(fd, DataString, strlen(DataString)); /* populate data file */
    fprintf(stderr, "Process %d has written to data file...\n", lock.l_pid);
  }

  /* Now release the lock explicitly. */
  lock.l_type = F_UNLCK;
  if (fcntl(fd, F_SETLK, &lock) < 0)
    report_and_exit("explicit unlocking failed...");

  close(fd); /* close the file: would unlock if needed */

  while(1){
    //get time
    now_time = time(NULL); 
    //char string_header[] = {};
    //check interval
    if ((now_time - last_time) > logInterval ){
      //if interval is met get all possible voltages
      int voltage[8] = {0,0,0,0,0,0,0,0};
      for( int i = 0, i <= 7, i++){
        //set mux and get voltage 
        voltage[i] = getMilliVolts(voltage_fd, i);
      }

      //open file
      if ((fd = open(FileName, O_RDWR | O_CREAT, 0666)) < 0)  /* -1 signals an error */
        report_and_exit("open failed...");
        
      if (fcntl(fd, F_SETLKW, &lock) < 0) /** F_SETLK doesn't block, F_SETLKW does **/
        report_and_exit("fcntl failed to get lock...");
      else {
        write(fd, DataString, strlen(DataString)); /* populate data file */
        fprintf(stderr, "Process %d has written to data file...\n", lock.l_pid);
      }
      //open file
      //store value
      //close file 

    }
    else{

    }
    
    
  }
  return 0;  /* terminating the process would unlock as well */
}