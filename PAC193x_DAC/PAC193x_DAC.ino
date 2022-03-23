/***********************************************************/

#include "Microchip_PAC1934_8.h"
#include <Wire.h>
//#include <AD5671R.h>

Microchip_PAC193x PAC(0x01);
//AD5671R dac;


void setup() 
{
    Wire.begin();
    PAC.begin();
    
    Serial.begin(9600);
    while (! Serial); // Wait until Serial is ready - Leonardo
    
    PAC.UpdateProductID();
    Serial.print("\n Product      ID: ");
    Serial.print(PAC.ProductID, HEX);
    
    PAC.UpdateManufacturerID();
    Serial.print("\n Manufacturer ID: ");
    Serial.print(PAC.ManufacturerID, HEX);
    
    PAC.UpdateRevisionID();
    Serial.print("\n Revision     ID: ");
    Serial.print(PAC.RevisionID, HEX);
}


void loop() 
{
    Serial.print("\n\nRead start:");

    PAC.UpdateVoltage();    
    //Serial.print(PAC.Voltage);
    Serial.print("\n Voltage     (mV) = ");
    for(int i=0; i < 4; i++)
    {
        Serial.print(PAC.Voltage[i]);   Serial.print("\t");
    } 
      
    PAC.UpdateVsense();
    Serial.print("\n Vsense      (mV) = ");
    for(int i = 0; i < 4; i++)
    {
        Serial.print(PAC.Vsense[i], 6);  Serial.print("\t");
    }
    
    PAC.UpdateCurrent();
    Serial.print("\n Current     (mA) = ");
    for(int i = 0; i < 4; i++)
    {
        Serial.print(PAC.Current[i],  6);   Serial.print("\t");
    }
    
    PAC.UpdatePowerRaw();
    Serial.print("\n Raw Power (HEX) = ");
    for(int i = 0; i < 4; i++)
    {
        Serial.print(PAC.PowerRaw[i], HEX);  Serial.print("\t");
    }
  
    Serial.print("\n Power      (mW) = ");
    PAC.UpdatePower();
    for(int i = 0; i < 4; i++)
    {
        Serial.print(PAC.Power[i], 6);     Serial.print("\t");
    }
    
    Serial.print("\n Power Acc  (mW) = ");
    PAC.UpdatePowerAcc() ;
    for(int i = 0; i < 4; i++)
    {
        Serial.print(PAC.PowerAcc[i], 6);  Serial.print("\t");
    }

    Serial.print("\n Energy    (mWh) = ");
    PAC.UpdateEnergy();
    for(int i = 0; i < 4; i++)
    {    
        Serial.print(PAC.Energy[i], 6);  Serial.print("\t");
    }
  
    delay(3000);
}
