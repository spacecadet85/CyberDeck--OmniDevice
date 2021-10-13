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
    int mode = 0;
    if (getMode(fd)){
        printf("Power down single shot mode\n");
    }
    else {
        printf("Continuous conversion mode\n");
    }
    setMode(fd, mode);
    if (getMode(fd)){
        printf("Power down single shot mode\n");
    }
    else {
        printf("Continuous conversion mode\n");
    }
    mode = 1;
    setMode(fd, mode);
    if (getMode(fd)){
        printf("Power down single shot mode\n");
    }
    else {
        printf("Continuous conversion mode\n");
    }

    int mux = getMultiplexer(fd);
    int setMux = ADS1115_MUX_P0_N3;

    printf("mux value = %i\n", mux);
    setMultiplexer(fd, setMux);
    mux = getMultiplexer(fd);
    if(mux == ADS1115_MUX_P0_N3){
        printf("Mux was set correctly --> %i\n", mux);
    }
    else{
        printf("Mux was not set correctly --> %i", mux);
    }

    setMux = ADS1115_MUX_P1_N3;
    setMultiplexer(fd, setMux);
    mux = getMultiplexer(fd);
    if(mux == ADS1115_MUX_P1_N3){
        printf("Mux was set correctly --> %i\n", mux);
    }
    else{
        printf("Mux was not set correctly --> %i", mux);
    }

    setMux = ADS1115_MUX_P2_N3;
    setMultiplexer(fd, setMux);
    mux = getMultiplexer(fd);
    if(mux == ADS1115_MUX_P2_N3){
        printf("Mux was set correctly --> %i\n", mux);
    }
    else{
        printf("Mux was not set correctly --> %i", mux);
    }

    setMux = ADS1115_MUX_P0_NG;
    setMultiplexer(fd, setMux);
    mux = getMultiplexer(fd);
    if(mux == ADS1115_MUX_P0_NG){
        printf("Mux was set correctly --> %i\n", mux);
    }
    else{
        printf("Mux was not set correctly --> %i", mux);
    }

    setMux = ADS1115_MUX_P1_NG;
    setMultiplexer(fd, setMux);
    mux = getMultiplexer(fd);
    if(mux == ADS1115_MUX_P1_NG){
        printf("Mux was set correctly --> %i\n", mux);
    }
    else{
        printf("Mux was not set correctly --> %i", mux);
    }

    setMux = ADS1115_MUX_P2_NG;
    setMultiplexer(fd, setMux);
    mux = getMultiplexer(fd);
    if(mux == ADS1115_MUX_P2_NG){
        printf("Mux was set correctly --> %i\n", mux);
    }
    else{
        printf("Mux was not set correctly --> %i", mux);
    }

    setMux = ADS1115_MUX_P3_NG;
    setMultiplexer(fd, setMux);
    mux = getMultiplexer(fd);
    if(mux == ADS1115_MUX_P3_NG){
        printf("Mux was set correctly --> %i\n", mux);
    }
    else{
        printf("Mux was not set correctly --> %i", mux);
    }
    
    //gain test
    int gain = getGain(fd);
    printf("Default Gain ---> %i\n", gain);

    for(int i = 0; i <= 7; i++){
        setGain(fd, i);
        int gain = getGain(fd);
        if(gain == i){
            printf("Gain was set correctly --> %i\n", gain);
        }
        else{
            printf("Gain was not set correctly --> %i\n", gain);
        }

    }

    //test rate
    int rate = getRate(fd);
    printf("Default Rate ---> %i\n", rate);

    for(int i = 0; i <= 7; i++){
        setRate(fd, i);
        int rate = getRate(fd);
        if(rate == i){
            printf("Rate was set correctly --> %i\n", rate);
        }
        else{
            printf("Rate was not set correctly --> %i\n", rate);
        }

    }

    //comparator mode
    int compMode = getComparatorMode(fd);
    printf("Default comparator mode ---> %i\n", compMode);
    if(compMode == 0){
        compMode = 1;
    }
    else{
        compMode = 0;
    }
    setComparatorMode(fd, compMode);
    compMode = getComparatorMode(fd);
    printf("Comparator set to ---> %i\n", compMode);

    //comparator polarity
    int polMode = getComparatorPolarity(fd);
    printf("Default comparator polarity mode ---> %i\n", polMode);
    if(polMode == 0){
        polMode = 1;
    }
    else{
        polMode = 0;
    }
    setComparatorPolarity(fd, polMode);
    polMode = getComparatorPolarity(fd);
    printf("Comparator polarity set to ---> %i\n", polMode);

    //comparator latch enable
    int latchMode = getComparatorLatchEnabled(fd);
    printf("Default comparator latch mode ---> %i\n", latchMode);
    if(latchMode == 0){
        latchMode = 1;
    }
    else{
        latchMode = 0;
    }
    setComparatorLatchEnabled(fd, latchMode);
    latchMode = getComparatorLatchEnabled(fd);
    printf("Comparator latch set to ---> %i\n", latchMode);

    //resistor divider = 0.175
    int dividerGain = 1/0.175; 
    float milvolts = 0;
    float milvolts1 = 0;
    setComparatorMode(fd, ADS1115_COMP_MODE_HYSTERESIS);
    setComparatorLatchEnabled(fd, ADS1115_COMP_LAT_NON_LATCHING);
    setComparatorQueueMode(fd, ADS1115_COMP_QUE_DISABLE);
    //set rate
    setRate(fd, ADS1115_RATE_128);
    //set mode single shot
    setMode(fd, ADS1115_MODE_SINGLESHOT);
    //set gain to 1
    setGain(fd, ADS1115_PGA_4P096);
    //set rate
    setRate(fd, ADS1115_RATE_128);
    //setup mux P0-N1
    //get A0-A1
    //print
    milvolts = getMilliVolts(fd, ADS1115_MUX_P0_N1);
    milvolts = milvolts * dividerGain;
    printf("differential battery value ---> %f\n", milvolts);
    //setup mux P1-GND
    //get A1-GND
    milvolts = getMilliVolts(fd, ADS1115_MUX_P1_NG);
    milvolts1 = milvolts * dividerGain;
    printf("differential battery value ---> %f\n", milvolts);
    //set mux
    //get float of A0
    milvolts = getMilliVolts(fd, ADS1115_MUX_P0_NG);
    milvolts = milvolts * dividerGain;
    printf("differential battery value ---> %f\n", milvolts);

    milvolts = milvolts1 - milvolts;
    printf("A1-A0 ---> %f\n", milvolts);
    

    return 0;
}