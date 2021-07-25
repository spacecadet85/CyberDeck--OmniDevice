#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h> // open
#include <sys/stat.h>  // open
#include <fcntl.h>     // open
#include <unistd.h>    // read/write usleep
#include <stdlib.h>    // exit
#include <inttypes.h>  // uint8_t, etc
#include <linux/i2c-dev.h> // I2C bus definitions
#include "ads1115.h"


//set up comms with ads1115
void initialize(int fd, uint8_t address){
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

bool isConversionReady(int fd){
    uint16_t osBit;
    //read config
    osBit = readReg(fd, ADS1115_RA_CONFIG);
    //get status bit
    osBit = osBit >> ADS1115_CFG_OS_BIT;
    return osBit;
}

//if the ad chip fails this will hang
bool pollConversion(int fd){
    bool conversion = false;
    while(conversion == false){
        conversion = isConversionReady(fd);
    }
    return true;
}
/*/ Differential
int16_t getConversionP0N1(int fd){
    setMultiplexer(fd, ADS1115_MUX_P0_N1);
    return getConversion(fd);
}

int16_t getConversionP0N3(int fd){
    setMultiplexer(fd, ADS1115_MUX_P0_N3 );
    return getConversion(fd);
}

int16_t getConversionP1N3(int fd){
    setMultiplexer(fd, ADS1115_MUX_P1_N3);
    return getConversion(fd);
}

int16_t getConversionP2N3(int fd){
    setMultiplexer(fd, ADS1115_MUX_P2_N3);
    return getConversion(fd);
}

// Single-ended
int16_t getConversionP0GND(int fd){
    setMultiplexer(fd,ADS1115_MUX_P0_NG);
    return getConversion(fd);
}

int16_t getConversionP1GND(int fd){
    setMultiplexer(fd,ADS1115_MUX_P1_NG);
    return getConversion(fd);
}

int16_t getConversionP2GND(int fd){
    setMultiplexer(fd,ADS1115_MUX_P2_NG);
    return getConversion(fd);
}

int16_t getConversionP3GND(int fd){
    setMultiplexer(fd,ADS1115_MUX_P3_NG);
    return getConversion(fd);
}
*/
int16_t getConversion(int fd){
    //check to see if in singleshot mode
    if(getMode(fd)){
        //if so trigger conversion and poll
        triggerConversion(fd);
        pollConversion(fd);
    }
    //read when ready
    return readReg(fd, ADS1115_RA_CONVERSION);
}

float getMilliVolts(int fd, uint8_t mux){
    //set mux 
    setMultiplexer(fd, mux);
    //get pga
    //get conversion and convert
    uint8_t pga = getGain(fd);
    switch (pga){
        case ADS1115_PGA_6P144:
            return (getConversion(fd) * ADS1115_MV_6P144); 
            break;
        case ADS1115_PGA_4P096:
            return (getConversion(fd) * ADS1115_MV_4P096);
            break;
        case ADS1115_PGA_2P048:
            return (getConversion(fd) * ADS1115_MV_2P048);
            break;
        case ADS1115_PGA_1P024:
            return (getConversion(fd) * ADS1115_MV_1P024);
            break;
        case ADS1115_PGA_0P512:
            return (getConversion(fd) * ADS1115_MV_0P512);
            break;
        case ADS1115_PGA_0P256:
            return (getConversion(fd) * ADS1115_MV_0P256);
            break;
        case ADS1115_PGA_0P256B:
            return (getConversion(fd) * ADS1115_MV_0P256B);
            break;
        case ADS1115_PGA_0P256C:
            return (getConversion(fd) * ADS1115_MV_0P256C);
            break;
    }
}
//float getMvPerCount();

void triggerConversion(int fd){
    uint16_t trigg;
    uint8_t buff[3];
    trigg = readReg(fd, ADS1115_RA_CONFIG);
    buff[0] = ADS1115_RA_CONFIG;
    buff[1] = ((trigg >> 8)) & 0xff;
    buff[1] = buff[1] | (1 << 7);
    buff[2] = ((trigg >> 0) & 0xff);
    writeReg(fd, buff);
}

uint8_t getMultiplexer(int fd){
    uint16_t mux;
    //read config 
    mux = readReg(fd, ADS1115_RA_CONFIG);
    //get multiplexer status
    mux = mux >> (ADS1115_CFG_MUX_BIT - ADS1115_CFG_MUX_LENGTH + 1);
    //shift bits, filter out OS bit and return (msB gets cut off)
    mux = mux & 0b0111;
    return mux;
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
    buff[1] = buff[1] | (mux << 4);// mux bits are 14, 13,and 12
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
    buff[1] = buff[1] | (gain << 1);
    buff[2] = ((config >>0) & 0xff);
    //write to gain bits
    writeReg(fd, buff);
}   

//reads the gain bits
uint8_t getGain(int fd){
    uint16_t gain;
    //read config
    gain = readReg(fd, ADS1115_RA_CONFIG);
    //get gain
    gain = gain >> (ADS1115_CFG_PGA_BIT - ADS1115_CFG_PGA_LENGTH +1);
    //shift bits and filter
    gain = gain & 0b0111;
    return gain;
}

bool getMode(int fd){
    uint16_t mode;
    //read config
    mode = readReg(fd, ADS1115_RA_CONFIG);
    //get mose
    mode = mode >> (ADS1115_CFG_MODE_BIT);
    //shift bits and filter
    mode = mode & 0x0001;
    if (mode == 0){
        return 0;
    }
    else{
        return 1;
    }
}

void setMode(int fd, uint8_t mode){
    uint16_t config;
    uint8_t buff[3];
    //get current config
    config = readReg(fd, ADS1115_RA_CONFIG);
    //mask bits
    buff[0] = ADS1115_RA_CONFIG;
    buff[1] = ((config >> 8) & 0xff);
    buff[1] = buff[1] | (mode << 0);
    buff[2] = ((config >>0) & 0xff);
    //write to gain bits
    writeReg(fd, buff);
}

uint8_t getRate(int fd){
    uint16_t rate;
    //read config
    rate = readReg(fd, ADS1115_RA_CONFIG);
    //get gain
    rate = rate >> (ADS1115_CFG_DR_BIT - ADS1115_CFG_DR_LENGTH + 1);
    //shift bits and filter
    rate = rate & 0b0111;
    return rate;
}

void setRate(int fd, uint8_t rate){
    uint16_t config;
    uint8_t buff[3];
    //get current config
    config = readReg(fd, ADS1115_RA_CONFIG);
    //mask bits
    buff[0] = ADS1115_RA_CONFIG;
    buff[1] = ((config >> 8) & 0xff);
    buff[1] = buff[1] | (rate << 0);
    buff[2] = ((config >>0) & 0xff);
    //write to gain bits
    writeReg(fd, buff);
}

bool getComparatorMode(int fd){
    uint16_t compMode;
    //read config
    compMode = readReg(fd, ADS1115_RA_CONFIG);
    //get mode
    compMode = compMode >> (ADS1115_CFG_COMP_MODE_BIT);
    //shift bits and filter
    compMode = compMode & 0x0001;
    if (compMode == 0){
        return 0;
    }
    else{
        return 1;
    }
}

void setComparatorMode(int fd, uint8_t mode){
    uint16_t config;
    uint8_t buff[3];
    //get current config
    config = readReg(fd, ADS1115_RA_CONFIG);
    //mask bits
    buff[0] = ADS1115_RA_CONFIG;
    buff[1] = ((config >> 8) & 0xff);
    buff[1] = buff[1] | (mode << 0);
    buff[2] = ((config >>0) & 0xff);
    //write to gain bits
    writeReg(fd, buff);
}

bool getComparatorPolarity(int fd){
    uint16_t compPol;
    //read config
    compPol = readReg(fd, ADS1115_RA_CONFIG);
    //get mode
    compPol = compPol >> (ADS1115_CFG_COMP_POL_BIT);
    //shift bits and filter
    compPol = compPol & 0x0001;
    if (compPol == 0){
        return 0;
    }
    else{
        return 1;
    }
}

void setComparatorPolarity(int fd, uint8_t polarity){
    uint16_t config;
    uint8_t buff[3];
    //get current config
    config = readReg(fd, ADS1115_RA_CONFIG);
    //mask bits
    buff[0] = ADS1115_RA_CONFIG;
    buff[1] = ((config >> 8) & 0xff);
    buff[2] = ((config >> 0) & 0xff);
    buff[2] = buff[2] | (polarity << 3);
    //write to gain bits
    writeReg(fd, buff);
}

bool getComparatorLatchEnabled(int fd){
    uint16_t latch;
    //read config
    latch = readReg(fd, ADS1115_RA_CONFIG);
    //get mode
    latch = latch >> (ADS1115_CFG_COMP_LAT_BIT);
    //shift bits and filter
    latch = latch & 0x0001;
    if (latch == 0){
        return 0;
    }
    else{
        return 1;
    }
}

void setComparatorLatchEnabled(int fd, uint8_t enabled){
    uint16_t config;
    uint8_t buff[3];
    //get current config
    config = readReg(fd, ADS1115_RA_CONFIG);
    //mask bits
    buff[0] = ADS1115_RA_CONFIG;
    buff[1] = ((config >> 8) & 0xff);
    buff[2] = ((config >> 0) & 0xff);
    buff[2] = buff[2] | (enabled << 3);
    //write to gain bits
    writeReg(fd, buff);
}

uint8_t getComparatorQueueMode(int fd){
    uint16_t queue;
    //read config
    queue = readReg(fd, ADS1115_RA_CONFIG);
    //get gain
    //queue = queue >> (ADS1115_CFG_DR_BIT - ADS1115_CFG_DR_LENGTH + 1);
    //shift bits and filter
    queue = queue & 0b0011;
    return queue;
}

void setComparatorQueueMode(int fd, uint8_t mode){
    uint16_t config;
    uint8_t buff[3];
    //get current config
    config = readReg(fd, ADS1115_RA_CONFIG);
    //mask bits
    buff[0] = ADS1115_RA_CONFIG;
    buff[1] = ((config >> 8) & 0xff);
    buff[2] = ((config >> 0) & 0xff);
    buff[2] = buff[2] | (mode << 0);
    //write to gain bits
    writeReg(fd, buff);
}
/*
void beginConversion(int fd){
    uint16_t config;
    uint8_t buff[3];
    //get current config
    config = readReg(fd, ADS1115_RA_CONFIG);
    //mask bits
    buff[0] = ADS1115_RA_CONFIG;
    buff[1] = ((config >> 8) & 0xff);
    buff[2] = ((config >> 0) & 0xff);
    buff[1] = buff[1] | (1 << 8);
    //write to gain bits
    writeReg(fd, buff); 
}
*/

void setConversionReadyPinMode(int fd, uint8_t pinMode){
    uint16_t config;
    uint8_t buff[3];
    //get current config
    config = readReg(fd, ADS1115_RA_CONFIG);
    //mask bits
    buff[0] = ADS1115_RA_CONFIG;
    buff[1] = ((config >> 8) & 0xff);
    buff[2] = ((config >> 0) & 0xff);
    buff[2] = buff[2] | (pinMode << 0);
    //write to gain bits
    writeReg(fd, buff);
}

int16_t getLowThreshold(){
    int16_t lowTheshold;
    //read config
    lowTheshold = readReg(fd, ADS1115_RA_LO_THRESH);
    return lowTheshold;
}

void setLowThreshold(int fd, int16_t threshold){

}

/*
int16_t getHighThreshold(){
    uint16_t highTheshold;

    //read config
    highTheshold = readReg(fd, ADS1115_RA_HI_THRESH);
    return highTheshold;
}

void setHighThreshold(int16_t threshold){

}
*/

void writeReg(int fd, uint8_t inputBuf[3]){
    if (write(fd, inputBuf, 3) != 3) {
        perror("Write to register 1");
        exit(-1);
    }
    return;
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