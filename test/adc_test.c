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

    for(int i = 0; i < 7=; i++){
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
}