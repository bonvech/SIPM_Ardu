/***********************************************************

   This is a library for Microchip PAC193x
    
   © 2020 Microchip Technology Inc. and its subsidiaries.  
 
   Subject to your compliance with these terms, you may use Microchip
   software and any derivatives of this software. You must retain the above
   copyright notice with any redistribution of this software and the 
   following disclaimers. 
   It is your responsibility to comply with third party license terms 
   applicable to your use of third party software (including open source 
   software) that may accompany this Microchip software.
  
   THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
   EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING 
   ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS
   FOR A PARTICULAR PURPOSE.  
  
   IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
   INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
   WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP
   HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. 
   TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
   CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF
   FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

***********************************************************/

/***********************************************************

    Version 1.0.0

***********************************************************/

#include "Microchip_PAC1934_8.h"

Microchip_PAC193x::Microchip_PAC193x()
{
    rsense = RSENSE;
    I2C_ADDRESS = 0x10;
    errorCode = 0;
}


Microchip_PAC193x::Microchip_PAC193x(uint8_t Addr, uint32_t resistorValue=RSENSE) 
{
    rsense = resistorValue;
    I2C_ADDRESS = 0x10 + Addr;
    errorCode = 0;
}

/*Microchip_PAC193x::Microchip_PAC193x(uint32_t resistorValue) 
{
    rsense = resistorValue;
    errorCode = 0;
}
*/

void Microchip_PAC193x::SetAddress(uint8_t Addr)
{
    I2C_ADDRESS = Addr;
}


void Microchip_PAC193x::begin()
{
    Write8(PAC1934_NEG_PWR_ADDR, 0);
    Write8(PAC1934_CTRL_ADDR, 2);
    Write8(PAC1934_CHANNEL_DIS_ADDR, 0);
    Write8(PAC1934_SLOW_ADDR, 20); //14h

    Refresh();
    delay(125);
}


void Microchip_PAC193x::Read(uint8_t reg_address, int Nbytes, uint8_t *pBuffer) 
{
    int byteCount = Nbytes;
    Wire.beginTransmission (I2C_ADDRESS);
#if (ARDUINO >= 100)
    Wire.write(reg_address); 
#else
    Wire.send(reg_address); 
#endif

    errorCode = Wire.endTransmission(false); 
    if (errorCode != 0)
    {
        errorCode = (-1);
    }

    Wire.beginTransmission(I2C_ADDRESS); 
    Wire.requestFrom(I2C_ADDRESS, Nbytes); 
    
#if (ARDUINO >= 100)
    while(Wire.available() && (byteCount > 0))    // slave may send less than requested
    {
        *pBuffer = Wire.read();
        pBuffer++;
        byteCount--;
    }
#else
    while(Wire.available() && (byteCount > 0))    // slave may send less than requested
    {
        *pBuffer = Wire.receive();
        pBuffer++;
        byteCount--;
    }
#endif
}


uint8_t Microchip_PAC193x::Read8(uint8_t reg_address) 
{
    uint8_t buffer[1];
    uint8_t *ptemp;

    ptemp = (uint8_t *)&buffer[0];
    Read(reg_address, 1, buffer);

    return *ptemp;
}


uint16_t Microchip_PAC193x::Read16(uint8_t reg_address) 
{
    uint8_t buffer[2];
    uint16_t *ptemp;

    ptemp = (uint16_t *)&buffer[0];
    Read(reg_address, 2, buffer);

    return *ptemp;
}

uint32_t Microchip_PAC193x::Read24(uint8_t reg_address) 
{
    uint8_t buffer[4];
    uint8_t tmp;
    uint32_t *ptemp;

    ptemp = (uint32_t *)&buffer[0];
    Read(reg_address, 3, buffer);

    // reverse bytes in read buffer:
/*    tmp = buffer[3];
    buffer[3] = buffer[0];
    buffer[0] = tmp;
    tmp = buffer[2];
    buffer[2] = buffer[1];
    buffer[1] = tmp;
  */  
    
    return *ptemp;
}


uint32_t Microchip_PAC193x::Read32(uint8_t reg_address) 
{
    uint8_t buffer[4];
    uint32_t *ptemp;

    ptemp = (uint32_t *)&buffer[0];
    Read(reg_address, 4, buffer);

    return *ptemp;
}


uint64_t Microchip_PAC193x::Read64(uint8_t reg_address) 
{
    uint8_t buffer[8];
    uint64_t *ptemp;

    ptemp = (uint64_t *)&buffer[0];    
    Read(reg_address, 8, buffer);

    return *ptemp;
}

void Microchip_PAC193x::Write8(uint8_t reg_address, uint8_t data) 
{
    Wire.beginTransmission(I2C_ADDRESS); // start transmission to device 
#if (ARDUINO >= 100)
    Wire.write(reg_address); // sends register address to read from
    Wire.write(data);        // write data
#else
    Wire.send(reg_address);  // sends register address to read from
    Wire.send(data);         // write data
#endif
    errorCode = Wire.endTransmission();  // end transmission
    if (errorCode != 0){
        errorCode = (-2);
    }
}

int16_t Microchip_PAC193x::UpdateProductID()
{    
    errorCode = 0;
    ProductID = Read8(PAC1934_PRODUCT_ID_ADDR); 
    
    return errorCode;
}

int16_t Microchip_PAC193x::UpdateManufacturerID()
{    
    errorCode = 0;
    ManufacturerID = Read8(PAC1934_MANUFACTURER_ID_ADDR); 
    
    return errorCode;
}

int16_t Microchip_PAC193x::UpdateRevisionID()
{    
    errorCode = 0;
    RevisionID = Read8(PAC1934_REVISION_ID_ADDR); 
    
    return errorCode;
}

void Microchip_PAC193x::Refresh()
{
    Write8(PAC1934_REFRESH_CMD_ADDR, 1); //refresh
    delay(20);
}

void Microchip_PAC193x::RefreshG()
{
    Write8(PAC1934_REFRESH_G_CMD_ADDR, 0); //refreshG
    delay(20);
}


int16_t Microchip_PAC193x::UpdateVoltageRaw()
{
    uint16_t VbusRaw;
    
    errorCode = 0;
    Write8(PAC1934_REFRESH_V_CMD_ADDR, 1); //refreshV
    delay(2);

    for(int i = 0; i < 4; i++)
    {
        VbusRaw = Read16(PAC1934_VBUS1_ADDR + i);         
        VbusRaw = (VbusRaw << 8) | (VbusRaw >> 8);
        VoltageRaw[i] = VbusRaw;
    }

    return errorCode;
}


int16_t Microchip_PAC193x::UpdateVoltage()
{
    //float VbusReal;
    float VbusLsb;

    errorCode = 0;
    UpdateVoltageRaw();
    VbusLsb = 32000 / 65536.0;

    for(int i = 0; i < 4; i++)
    {
        //VbusReal = (float)VoltageRaw[i] * VbusLsb;
        Voltage[i] = (float)VoltageRaw[i] * VbusLsb;
        // -U power of SiPM 
        if((I2C_ADDRESS == 0x11) && (i == 3))
        {
           Voltage[i] = (Voltage[i] - 2500) * 12.048; 
        }
    }

    return errorCode;
}


int16_t Microchip_PAC193x::UpdateVsenseRaw()
{
    uint16_t VsRaw = 0;
    errorCode = 0;
    Write8(PAC1934_REFRESH_V_CMD_ADDR, 1); //refreshV
    delay(2);

    for(int i = 0; i < 4; i++)
    {
        VsRaw = Read16(PAC1934_VSENSE1_ADDR + i);
        VsenseRaw[i] = (VsRaw << 8) | (VsRaw >> 8);
    }
    
    return errorCode;
}


int16_t Microchip_PAC193x::UpdateVsense()
{
    float VsenseLsb = 100. / 65536.0;

    errorCode = 0;
    UpdateVsenseRaw();

    for(int i = 0; i < 4; i++)
    {
        Vsense[i] = (float)VsenseRaw[i] * VsenseLsb;
    }

    return errorCode;
}


int16_t Microchip_PAC193x::UpdateCurrent()
{
    errorCode = 0; 
    UpdateVsense();

    for(int i = 0; i < 4; i++)
    {
        Current[i] = (Vsense[i] / rsense) * 1000000; //mA
    }
    
    if (rsense <= 0){
        errorCode = (-3);
    }

    return errorCode;
}


int16_t Microchip_PAC193x::UpdatePowerRaw()
{
    uint32_t PowerRawtmp;
    errorCode = 0;
    
    Write8(PAC1934_REFRESH_V_CMD_ADDR, 1); //refreshV
    delay(2);

    for(int i = 0; i < 4; i++)
    {    
        PowerRawtmp = Read32(PAC1934_VPOWER1_ADDR + i);
        PowerRawtmp = ((PowerRawtmp << 8) & 0xFF00FF00) | ((PowerRawtmp >> 8) & 0xFF00FF); 
        PowerRaw[i] = (PowerRawtmp << 16) | (PowerRawtmp >> 16);
    }
    
    return errorCode;
}


int16_t Microchip_PAC193x::UpdatePower()
{
    uint32_t PowerRegScale = 0x10000000;
    double   PowerFSR = (3.2 * 1000000) / rsense; 
    errorCode = 0;
    
    UpdatePowerRaw();

    for(int i = 0; i < 4; i++)
    {    
        Power[i] = (((double)PowerRaw[i] / 16.) * PowerFSR) / PowerRegScale;
    }

    if (rsense <= 0){
        errorCode = (-3);
    }

    return errorCode;
}


int16_t Microchip_PAC193x::UpdatePowerAccRaw()
{
    uint64_t tmp = 0;
    errorCode = 0;
    
    Write8(PAC1934_REFRESH_V_CMD_ADDR, 1); //refreshV
    delay(2);

    tmp = Read24(PAC1934_ACC_COUNT_ADDR);
    //PowerRawtmp = ((PowerRawtmp << 8) & 0xFF00FF00) | ((PowerRawtmp >> 8) & 0xFF00FF); 
    //PowerRaw[i] = (PowerRawtmp << 16) | (PowerRawtmp >> 16);

    //tmp = ((tmp <<  8) & 0xFF00FF00FF00FF00ULL ) | ((tmp >> 8) & 0x00FF00FF00FF00FFULL );
    //tmp = ((tmp << 16) & 0xFFFF0000FFFF0000ULL ) | ((tmp >> 16) & 0x0000FFFF0000FFFFULL );
    PowerAccAllRaw = tmp;
      
    for(int i = 0; i < 4; i++)
    {    
        tmp = Read64(PAC1934_VPOWER1_ACC_ADDR + i);
        tmp = ((tmp <<  8) & 0xFF00FF00FF00FF00ULL ) | ((tmp >> 8) & 0x00FF00FF00FF00FFULL );
        tmp = ((tmp << 16) & 0xFFFF0000FFFF0000ULL ) | ((tmp >> 16) & 0x0000FFFF0000FFFFULL );
        tmp = ( tmp << 32) | (tmp >> 32); 
        PowerAccRaw[i] = tmp;
    }

    return errorCode;
}


int16_t Microchip_PAC193x::UpdatePowerAcc()
{
    uint32_t PowerRegScale = 0x10000000;
    double   PowerFSR = (3.2 * 1000000) / rsense;

    errorCode = 0;
    UpdatePowerAccRaw();

    PowerAccAll = (double)PowerAccAllRaw; // * PowerFSR / PowerRegScale;
    
    for(int i = 0; i < 4; i++)
    {    
        PowerAcc[i] = (double)PowerAccRaw[i] * PowerFSR / PowerRegScale;
    }

    if (rsense <= 0)
        errorCode = (-3);

    return errorCode;
}

int16_t Microchip_PAC193x::UpdateSampleRateLat()
{
    uint16_t sampleRateVal;
    uint8_t  sampleRateBits;

    errorCode = 0;
    sampleRateBits = Read8(PAC1934_CTRL_LAT_ADDR);
    sampleRateBits = ((sampleRateBits & 0xC0) >> 6);
    switch(sampleRateBits){
        case 0:
            sampleRateVal = 1024;
            break;
        case 1:
            sampleRateVal = 256;
            break;
        case 2:
            sampleRateVal = 64;
            break;
        case 3:
            sampleRateVal = 8;
            break;
    }

    SampleRateLat = sampleRateVal;

    return errorCode;
}


int16_t Microchip_PAC193x::setSampleRate(uint16_t value)
{
    uint8_t temp_read, temp_write;

    errorCode = 0;
    temp_read = Read8(PAC1934_CTRL_ADDR);

    switch(value){
        case 1024:
            temp_write = 0;
            break;
        case 256:
            temp_write = 1;
            break;
        case 64:
            temp_write = 2;
            break;
        case 8:
            temp_write = 3;
            break;
        default:
            temp_write = 0;
            errorCode = (-4);
            return errorCode;
    }

    temp_write = (uint8_t)((temp_write << 6) + (temp_read & 0x3F));
    Write8(PAC1934_CTRL_ADDR, temp_write);
    Refresh();
    delay(125);

    return errorCode;
}


int16_t Microchip_PAC193x::UpdateEnergy()
{
    errorCode = 0;
    UpdatePowerAcc();
    UpdateSampleRateLat();

    for(int i = 0; i < 4; i++)
    {    
        Energy[i] = (PowerAcc[i] / SampleRateLat) / 3.6;
    }

    return errorCode;
}


int16_t Microchip_PAC193x::UpdateOverflowAlert()
{
    uint8_t temp_read;
    
    errorCode = 0;
    temp_read = Read8(PAC1934_CTRL_ACT_ADDR);
    temp_read = (uint8_t)(temp_read & 0x01); // bit 0
    
    OverflowAlert = temp_read;
    
    return errorCode;
}
int16_t Microchip_PAC193x::UpdateSlowStatus(){
    uint8_t temp_read;
    
    errorCode = 0;
    temp_read = Read8(PAC1934_SLOW_ADDR);
    temp_read = (uint8_t)((temp_read & 0x80) >> 7); // bit 7
    
    SlowStatus = temp_read;
    
    return errorCode;
}
int16_t Microchip_PAC193x::UpdatePowerOnStatus(){
    uint8_t temp_read, temp_write;
    
    errorCode = 0;
    temp_read = Read8(PAC1934_SLOW_ADDR);
    temp_read = (uint8_t)(temp_read & 0x01); // bit 0
    temp_write = (uint8_t)(temp_read & 0xFE);
    Write8(PAC1934_SLOW_ADDR, temp_write);
    
    PowerOnStatus = temp_read;
    
    return errorCode;
}
