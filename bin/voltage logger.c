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
//#define DataString "Now is the winter of our discontent\nMade glorious summer by this sun of York\n"

void report_and_exit(const char* msg) {
  perror(msg);
  exit(-1); /* EXIT_FAILURE */
}

int main() {

  time_t logInterval = 1000; //log rate in ms
  time_t now_time;
  time_t last_time = 0;

  struct flock lock;
  lock.l_type = F_WRLCK;    /* read/write (exclusive versus shared) lock */
  lock.l_whence = SEEK_SET; /* base for seek offsets */
  lock.l_start = 0;         /* 1st byte in file */
  lock.l_len = 0;           /* 0 here means 'until EOF' */
  lock.l_pid = getpid();    /* process id */

  int votlage_fd = initialize_ads1115(ADS1115_ADDRESS_ADDR_GND); //initialize the ADC
  int fd; /* file descriptor to identify a file within a process */
  if ((fd = open(FileName, O_RDWR | O_CREAT, 0666)) < 0)  /* -1 signals an error */
    report_and_exit("open failed...");

  if (fcntl(fd, F_SETLK, &lock) < 0) /** F_SETLK doesn't block, F_SETLKW does **/
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
    //check interval
    if ((now_time - last_time) > logInterval ){
      //if interval met get volatge
      //get lock 
      //open file
      //store value
      //close file 

    }
    else{

    }
    
    
  }
  return 0;  /* terminating the process would unlock as well */
}