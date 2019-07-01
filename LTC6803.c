
#include "LTC6803.h"
#include "mcc_generated_files/mcc.h"
//------------------------------------------------------------------------------
uint8_t LTC6803_GetPEC(uint8_t  *dataArray, int length)
{
    uint8_t PEC = 0x41, data, IN0, IN1, IN2;
    
    for (int byte = 0; byte < length; byte++)
    {
        data = dataArray[byte];
        for (int Bit = 7; Bit >=0 ; Bit--)
        {
            IN0 = (((data >> Bit) & 0x01) ^ ((PEC >> 7) & 0x01) & 0x01);
            IN1 = ((PEC & 0x01) ^ IN0) & 0x01;
            IN2 = (((PEC >> 1) & 0x01) ^ IN0) & 0x01;
            PEC = (PEC << 1) & 0xF8;
            PEC |= ((IN2 << 2) | (IN1 << 1)) | (IN0 & 0x01);
        }
    }
    return PEC;
}
//------------------------------------------------------------------------------
uint8_t LTC6803_GetSinglePEC(uint8_t data)
{
    uint8_t PEC = 0x41, IN0, IN1, IN2;
    for (int Bit = 7; Bit >=0 ; Bit--)
    {
        IN0 = (((data >> Bit) & 0x01) ^ ((PEC >> 7) & 0x01) & 0x01);
        IN1 = ((PEC & 0x01) ^ IN0) & 0x01;
        IN2 = (((PEC >> 1) & 0x01) ^ IN0) & 0x01;
        PEC = (PEC << 1) & 0xF8;
        PEC |= ((IN2 << 2) | (IN1 << 1)) | (IN0 & 0x01);
    }
    return PEC;
}
//------------------------------------------------------------------------------
void LTC6803_WriteConfiguration(uint8_t address, uint8_t * configuration)
{
    char dummy[8], configurationPEC, addressPEC;
    configurationPEC = LTC6803_GetPEC(configuration, 6);
    addressPEC = LTC6803_GetSinglePEC((address & 0x0F) | 0x80);
    
    CSDO_SetLow();
    SPI_Exchange8bit((address & 0x0F) | 0x80);
    SPI_Exchange8bit(addressPEC);
    SPI_Exchange8bit(WRCFG);
    SPI_Exchange8bit(WRCFG_PEC);
    for (int i = 0; i < 6; i++)
    {
        SPI_Exchange8bit(configuration[i]);
    }
    SPI_Exchange8bit(configurationPEC);
    CSDO_SetHigh();
}
//------------------------------------------------------------------------------
void LTC6803_WriteConfigurationBC(uint8_t * configuration)
{
    char dummy[8], configurationPEC;
    uint8_t byte_num;
    configurationPEC = LTC6803_GetPEC(configuration,6);
    
    CSDO_SetLow();
    SPI_Exchange8bit(WRCFG);
    SPI_Exchange8bit(WRCFG_PEC);
    byte_num = SPI_Exchange8bitBuffer(&configuration,6, &dummy);
    SPI_Exchange8bit(configurationPEC);
    CSDO_SetHigh();
}
//------------------------------------------------------------------------------
void LTC6803_StartCellADCConversion()
{   
    CSDO_SetLow();
    SPI_Exchange8bit(STCVAD_ALL);
    SPI_Exchange8bit(STCVAD_ALL_PEC);
    __delay_ms(13);
    CSDO_SetHigh();
}
//------------------------------------------------------------------------------
void LTC6803_StartTempADCConversion()
{   
    CSDO_SetLow();
    SPI_Exchange8bit(STTMPAD_ALL);
    SPI_Exchange8bit(STTMPAD_ALL_PEC);
    __delay_ms(13);
    CSDO_SetHigh();
}
//------------------------------------------------------------------------------
void LTC6803_ReadCellVoltages(uint8_t address, uint8_t * cellRegisters)
{
    uint8_t addressPEC = LTC6803_GetSinglePEC((address & 0x0F) | 0x80);
    uint8_t byteNum;
    
    CSDO_SetLow();
    SPI_Exchange8bit((address & 0x0F) | 0x80);
    SPI_Exchange8bit(addressPEC);
    SPI_Exchange8bit(RDCV);
    SPI_Exchange8bit(RDCV_PEC);
    for (int i = 0; i < 19; i++)
    {
        cellRegisters[i]= SPI_Exchange8bit(0xFF);
    }
    CSDO_SetHigh();
}
//------------------------------------------------------------------------------
void LTC6803_ReadTemperature(uint8_t address, uint8_t * tempRegisters)
{
    uint8_t addressPEC = LTC6803_GetSinglePEC((address & 0x0F) | 0x80);
    uint8_t byteNum;
    
    CSDO_SetLow();
    SPI_Exchange8bit((address & 0x0F) | 0x80);
    SPI_Exchange8bit(addressPEC);
    SPI_Exchange8bit(RDTMP);
    SPI_Exchange8bit(RDTMP_PEC);
    for (int i = 0; i < 6; i++)
    {
        tempRegisters[i]= SPI_Exchange8bit(0xFF);
    }
    CSDO_SetHigh();
}
//------------------------------------------------------------------------------

bool LTC6803_ReadConfiguration(uint8_t address, uint8_t * configuration)
{
    volatile uint8_t addressPEC, configurationPEC; 
    addressPEC = LTC6803_GetSinglePEC((address & 0x0F) | 0x80);
    CSDO_SetLow();
    SPI_Exchange8bit((address & 0x0F) | 0x80);
    SPI_Exchange8bit(addressPEC);
    SPI_Exchange8bit(RDCFG);
    SPI_Exchange8bit(RDCFG_PEC);
    
    for (int i = 0; i < 7; i++)
    {
        configuration[i]= SPI_Exchange8bit(0xFF);
    }
    CSDO_SetHigh();
    configurationPEC = LTC6803_GetPEC(configuration,6);
    if (configurationPEC != configuration[6])
    {
        return READ_FAILED;
    }else
    {
        return READ_SUCCESS;
    }
    
}   