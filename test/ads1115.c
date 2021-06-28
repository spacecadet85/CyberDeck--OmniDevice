#include <stdio.h>
#include <sys/types.h> // open
#include <sys/stat.h>  // open
#include <fcntl.h>     // open
#include <unistd.h>    // read/write usleep
#include <stdlib.h>    // exit
#include <inttypes.h>  // uint8_t, etc
#include <linux/i2c-dev.h> // I2C bus definitions
#include "ads1115.h"


//set up comms with ads1115
void initialize(uint8_t address, int fd){
    if ((fd = open("/dev/i2c-1", O_RDWR)) < 0) {
        printf("Error: Couldn't open device! %d\n", fd);
        exit (1);
    }

    // connect to ads1115 as i2c slave
    if (ioctl(fd, I2C_SLAVE, address) < 0) {
        printf("Error: Couldn't find device on address!\n");
        exit (1);
    }
}

/*might not need this function
bool isConversionReady(int fd){
    uint8_t readBuff[2];
    do {
        if (read(fd, writeBuf, 2) != 2) {
        perror("Read conversion");
        exit(-1);
        }
    }
    while ((writeBuf[0] & 0x80) == 0);
    return 
}
*/

uint8_t getMultiplexer(int fd){
    uint16_t mux;
    //read config 
    mux = readReg(fd, ADS1115_RA_CONFIG);
    //get multiplexer status
    mux = mux >> (ADS1115_CFG_MUX_BIT - ADS1115_CFG_MUX_LENGTH + 1);
    //shift bits, filter out OS bit and return (msB gets cut off)
    mux = mux & 0b0111;
    return val
}
//needs work
void setMultiplexer(int fd, uint8_t mux){
    uint16_t config;
    uint8_t buff[3];
    //get current config reg value for masking
    config = readReg(fd, ADS1115_RA_CONFIG);
    //mask bits
    buff[0] = ADS1115_RA_CONFIG;
    buff[1] = ((config >> 8) & 0xff);
    buff[1] = buff[1] | ((mux << 4));// mux bits are 14, 13,and 12
    buff[2] = ((config >> 0) & 0xff);//bits 7-0 
    //write to multiplexer bits only
    writeReg(fd, buff);
}
//needs work
//sets the gain bits
void setGain(int fd, uint8_t gain){
    uint16_t config;
    uint8_t buff[3];
    //get current config
    config = readReg(fd, ADS1115_RA_CONFIG);
    //mask bits
    buff[0] = ADS1115_RA_CONFIG;
    buff[1] = ((config >> 8) & 0xff);
    buff[1] = buff[1] | (gain << 1)
    //write to gain bits
}   

//reads the gain bits
void getGain(){

}

bool getMode(){

}

bool setMode(){

}

uint8_t getRate(){

}

uint8_t setRate(){

}

bool getComparatorMode(){

}

void setComparatorMode(bool mode){

}

bool getComparatorPolarity(){

}

void setComparatorPolarity(bool polarity){

}

bool getComparatorLatchEnabled(){

}

void setComparatorLatchEnabled(bool enabled){

}

uint8_t getComparatorQueueMode(){

}

void setComparatorQueueMode(uint8_t mode){

}

void setConversionReadyPinMode(){

}

int16_t getLowThreshold(){

}

void setLowThreshold(int16_t threshold){

}

int16_t getHighThreshold(){

}

void setHighThreshold(int16_t threshold){

}


int writeReg(int fd, uint8_t inputBuf[3]){
    if (write(fd, inputBuf, 3) != 3) {
        perror("Write to register 1");
        exit(-1);
    }
    return 1;
}

int16_t readReg(int fd, uint8_t regAddress){
    uint8_t reg[2];
    //send slave address byte (write), write to pointer reg
    if (write(fd, regAddress, 1) != 1) {
        perror("Write register select");
        exit(-1);
    }
  
    // send slave adress (read), read two bytes
    if (read(fd, reg, 2) != 2) {
        perror("Read conversion");
        exit(-1);
    }
    
    //convert dispaly results and return
    int val = reg[0] << 8 | reg[1];
    return val;
}