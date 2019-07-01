/* 
 * File:   LTC6803.h
 * Author: Ayodeji Ajayi
 * Comments:
 * Revision history: v1
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef LTC6803_H
#define	LTC6803_H

#ifndef CSDO
#define CSDO        LATBbits.LATB5   //Configure Chip select output pin here
#endif

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

/* LTC6803-2 Command Sets*/
#define  WRCFG          0x01
#define  RDCFG          0x02
#define  RDCV           0x04
#define  RDCVA          0x06
#define  RDCVB          0x08
#define  RDCVC          0x0A
#define  RDFLG          0x0C
#define  RDTMP          0x0E
#define  STCVAD_ALL     0x10
#define  STCVAD_CLR     0x1D
#define  STOWAD_ALL     0x20
#define  STTMPAD_ALL    0x30
#define  PLADC          0x40
#define  PLINT          0x50
#define  DAGN           0x52
#define  RDDGNR         0x54
#define  STCVDC_ALL     0x60
#define  STOWDC_ALL     0x70

/* LTC6803-2 Command PEC Sets*/
#define  WRCFG_PEC          0xC7
#define  RDCFG_PEC          0xCE
#define  RDCV_PEC           0xDC
#define  RDCVA_PEC          0xD2
#define  RDCVB_PEC          0xF8
#define  RDCVC_PEC          0xF6
#define  RDFLG_PEC          0xE4
#define  RDTMP_PEC          0xEA
#define  STCVAD_ALL_PEC     0xB0
#define  STCVAD_CLR_PEC     0x93
#define  STOWAD_ALL_PEC     0x20
#define  STTMPAD_ALL_PEC    0x50
#define  PLADC_PEC          0x07
#define  PLINT_PEC          0x77
#define  DAGN_PEC           0x79
#define  RDDGNR_PEC         0x6B
#define  STCVDC_ALL_PEC     0xE7
#define  STOWDC_ALL_PEC     0x97
/* */
#define  DEFAULT_CFG0       0xE0
#define  READ_SUCCESS       1
#define  READ_FAILED        0
#define  LTC_RES            1.5


/**
 *  Get PEC from a byte array
 * @param dataArray
 * @param length
 * @return PEC
 */
uint8_t LTC6803_GetPEC(uint8_t  *dataArray, int length);
/**
 * Get PEC from a single byte
 * @param data
 * @return PEC
 */
uint8_t LTC6803_GetSinglePEC(uint8_t data);
/**
 * Write configuration to a single LTC6803 device using its address
 * @param address
 * @param configuration
 */
void LTC6803_WriteConfiguration(uint8_t address, uint8_t * configuration);
/**
 * Write configuration to all LTC6803 devices on the bus
 * @param configuration
 */
void LTC6803_WriteConfigurationBC(uint8_t * configuration);
/**
 Starts LTC6803 cells voltage ADC conversion without polling
 */
void LTC6803_StartCellADCConversion();
/**
 * Starts LTC6803 temperature sensor ADC conversion
 */
void LTC6803_StartTempADCConversion();
/**
 * Read cell voltages from LTC6803 device using its address and writing to the pointer * data
 * @param address
 * @param data
 * @return 
 */   
void LTC6803_ReadCellVoltages(uint8_t address, uint8_t * cellRegisters);
/**
 * Read module temperature from LTC6803 device using its address and writing to the pointer * cellRegister
 * @param address
 * @param tempRegisters
 */
void LTC6803_ReadTemperature(uint8_t address, uint8_t * tempRegisters);
/**
 * Read configuration from LTC6803 device using its address and writing to the pointer * configuration
 * @param address
 * @param configuration
 * @return 
 */
bool LTC6803_ReadConfiguration(uint8_t address, uint8_t * configuration);
 

#endif	/* LTC6803_H */

