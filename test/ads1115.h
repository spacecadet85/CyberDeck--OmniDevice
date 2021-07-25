#ifndef ADS1115_H
#define ADS1115_H

#include <stdint.h>

#define ADS1115_ADDRESS_ADDR_GND    0x48 // address pin low (GND)
#define ADS1115_ADDRESS_ADDR_VDD    0x49 // address pin high (VCC)
#define ADS1115_ADDRESS_ADDR_SDA    0x4A // address pin tied to SDA pin
#define ADS1115_ADDRESS_ADDR_SCL    0x4B // address pin tied to SCL pin
#define ADS1115_DEFAULT_ADDRESS     ADS1115_ADDRESS_ADDR_GND

#define ADS1115_RA_CONVERSION       0x00
#define ADS1115_RA_CONFIG           0x01
#define ADS1115_RA_LO_THRESH        0x02
#define ADS1115_RA_HI_THRESH        0x03

#define ADS1115_CFG_OS_BIT          15
#define ADS1115_CFG_MUX_BIT         14
#define ADS1115_CFG_MUX_LENGTH      3
#define ADS1115_CFG_PGA_BIT         11
#define ADS1115_CFG_PGA_LENGTH      3
#define ADS1115_CFG_MODE_BIT        8
#define ADS1115_CFG_DR_BIT          7
#define ADS1115_CFG_DR_LENGTH       3
#define ADS1115_CFG_COMP_MODE_BIT   4
#define ADS1115_CFG_COMP_POL_BIT    3
#define ADS1115_CFG_COMP_LAT_BIT    2
#define ADS1115_CFG_COMP_QUE_BIT    1
#define ADS1115_CFG_COMP_QUE_LENGTH 2


#define ADS1115_MUX_P0_N1           0x00 // default
#define ADS1115_MUX_P0_N3           0x01
#define ADS1115_MUX_P1_N3           0x02
#define ADS1115_MUX_P2_N3           0x03
#define ADS1115_MUX_P0_NG           0x04
#define ADS1115_MUX_P1_NG           0x05
#define ADS1115_MUX_P2_NG           0x06
#define ADS1115_MUX_P3_NG           0x07

#define ADS1115_PGA_6P144           0x00
#define ADS1115_PGA_4P096           0x01
#define ADS1115_PGA_2P048           0x02 // default
#define ADS1115_PGA_1P024           0x03
#define ADS1115_PGA_0P512           0x04

#define ADS1115_PGA_0P256           0x05
#define ADS1115_PGA_0P256B          0x06
#define ADS1115_PGA_0P256C          0x07

#define ADS1115_MV_6P144            0.187500
#define ADS1115_MV_4P096            0.125000
#define ADS1115_MV_2P048            0.062500 // default
#define ADS1115_MV_1P024            0.031250
#define ADS1115_MV_0P512            0.015625
#define ADS1115_MV_0P256            0.007813
#define ADS1115_MV_0P256B           0.007813 
#define ADS1115_MV_0P256C           0.007813

#define ADS1115_MODE_CONTINUOUS     0x00
#define ADS1115_MODE_SINGLESHOT     0x01 // default

#define ADS1115_RATE_8              0x00
#define ADS1115_RATE_16             0x01
#define ADS1115_RATE_32             0x02
#define ADS1115_RATE_64             0x03
#define ADS1115_RATE_128            0x04 // default
#define ADS1115_RATE_250            0x05
#define ADS1115_RATE_475            0x06
#define ADS1115_RATE_860            0x07

#define ADS1115_COMP_MODE_HYSTERESIS    0x00 // default
#define ADS1115_COMP_MODE_WINDOW        0x01

#define ADS1115_COMP_POL_ACTIVE_LOW     0x00 // default
#define ADS1115_COMP_POL_ACTIVE_HIGH    0x01

#define ADS1115_COMP_LAT_NON_LATCHING   0x00 // default
#define ADS1115_COMP_LAT_LATCHING       0x01

#define ADS1115_COMP_QUE_ASSERT1    0x00
#define ADS1115_COMP_QUE_ASSERT2    0x01
#define ADS1115_COMP_QUE_ASSERT4    0x02
#define ADS1115_COMP_QUE_DISABLE    0x03 // default

//------------------------------------------------------------------------------
//ads1115.h 
//------------------------------------------------------------------------------

//ADS1115();
//ADS1115(uint8_t address);

void initialize_ads1115(int fd, uint8_t address);
bool testConnection();

// SINGLE SHOT utilities
bool pollConversion(int fd);
void triggerConversion(int fd);

// Read the current CONVERSION register
int16_t getConversion(int fd);

/*/ Differential
int16_t getConversionP0N1(int fd);
int16_t getConversionP0N3(int fd);
int16_t getConversionP1N3(int fd);
int16_t getConversionP2N3(int fd);

// Single-ended
int16_t getConversionP0GND(int fd);
int16_t getConversionP1GND(int fd);
int16_t getConversionP2GND(int fd);
int16_t getConversionP3GND(int fd);
*/



// Utility
float getMilliVolts(int fd, uint8_t mux);
//float getMvPerCount();

// CONFIG register
bool isConversionReady(int fd);
uint8_t getMultiplexer(int fd);
void setMultiplexer(int fd, uint8_t mux);
uint8_t getGain(int fd);
void setGain(int fd, uint8_t gain);
bool getMode(int fd);
void setMode(int fd, uint8_t mode);
uint8_t getRate(int fd);
void setRate(int fd, uint8_t rate);
bool getComparatorMode(int fd);
void setComparatorMode(int fd, uint8_t mode);
bool getComparatorPolarity(int fd);
void setComparatorPolarity(int fd, uint8_t polarity);
bool getComparatorLatchEnabled(int fd);
void setComparatorLatchEnabled(int fd, uint8_t enabled);
uint8_t getComparatorQueueMode(int fd);
void setComparatorQueueMode(int fd, uint8_t mode);
void setConversionReadyPinMode(int fd, uint8_t pinMode);


// *_THRESH registers
int16_t getLowThreshold();
void setLowThreshold(int fd, int16_t threshold);
int16_t getHighThreshold();
void setHighThreshold(int16_t threshold);

// DEBUG
void showConfigRegister();

void writeReg(int fd, uint8_t inputBuf[3]);
int16_t readReg(int fd, uint8_t regAddress);


#endif