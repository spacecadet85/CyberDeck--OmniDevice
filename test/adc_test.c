/*
This code will test the functionality of the ads1115.c and .h files.

*/

#include "ads1115.h"
#include <stdio.h>
#include <sys/types.h> // open
#include <sys/stat.h>  // open
#include <fcntl.h>     // open
#include <unistd.h>    // read/write usleep
#include <stdlib.h>    // exit
#include <inttypes.h>  // uint8_t, etc
#include <linux/i2c-dev.h> 

int main(){
    //set up 
    int fd = initialize_ads1115(ADS1115_ADDRESS_ADDR_GND);
    //set and check mode
    if (getmode(fd)){
        printf("Power down single shot mode\n");
    }
    else {
        printf("Continuous conversion mode\n");
    }
    //check read
    int mux = getMultiplexer(fd);
    printf("mux value is %x\n",mux);
    //check write 
    //check all functions
    return(0);
}