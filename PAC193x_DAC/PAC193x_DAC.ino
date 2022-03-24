/***********************************************************/

#include "Microchip_PAC1934_8.h"
#include <Wire.h>
//#include <AD5671R.h>

Microchip_PAC193x PAC[2];
int k = 0;        //< counter for PCA devices
int count = 0;    //< counter for ferresh cycle


void setup() 
{
    Wire.begin();
    Serial.begin(9600);
    while (! Serial); // Wait until Serial is ready - Leonardo

    PAC[0].begin();
    PAC[1].begin();
    PAC[0].SetAddress(0x10);
    PAC[1].SetAddress(0x11);
    PAC[0].setSampleRate(8);
    PAC[1].setSampleRate(8);
        
    k = 0;
    PAC[k].UpdateProductID();
    Serial.print("\n Product      ID: ");
    Serial.print(PAC[k].ProductID, HEX);
    
    PAC[k].UpdateManufacturerID();
    Serial.print("\n Manufacturer ID: ");
    Serial.print(PAC[k].ManufacturerID, HEX);
    
    PAC[k].UpdateRevisionID();
    Serial.print("\n Revision     ID: ");
    Serial.print(PAC[k].RevisionID, HEX);

    PAC[0].RefreshG();
    PAC[1].RefreshG();
}


void loop() 
{
    Serial.print("\n\nRead start:");

    count += 1;
    if(count > 10)
    {
        count = 0;
        PAC[0].RefreshG();
        PAC[1].RefreshG();
        Serial.print("\n\n REFRESH\n\n");
    }

    for(k = 0; k < 2; k++)
    {
        PAC[k].UpdateVoltage();    
        //Serial.print(PAC.Voltage);
        Serial.print("\n Voltage     (mV) = ");
        for(int i=0; i < 4; i++)
        {
            Serial.print(PAC[k].Voltage[i], 3);   Serial.print("\t");
        }
    } 
      
    for(k = 0; k < 2; k++)
    {
        PAC[k].UpdateVsense();
        Serial.print("\n Vsense      (mV) = ");
        for(int i = 0; i < 4; i++)
        {
            Serial.print(PAC[k].Vsense[i], 6);  Serial.print("\t");
        }
    }
    
    for(k = 0; k < 2; k++)
    {
        PAC[k].UpdateCurrent();
        Serial.print("\n Current     (mA) = ");
        for(int i = 0; i < 4; i++)
        {
            Serial.print(PAC[k].Current[i],  6);   Serial.print("\t");
        }
    }
    
/*    for(k = 0; k < 2; k++)
    {
        PAC[k].UpdatePowerRaw();
        Serial.print("\n Raw Power  (HEX) = ");
        for(int i = 0; i < 4; i++)
        {
            //Serial.print(PAC.PowerRaw[i], HEX);  Serial.print("\t");
            Serial.print(PAC[k].PowerRaw[i], BIN);  Serial.print("\t");
        }
    }
*/  
    for(k = 0; k < 2; k++)
    {
        Serial.print("\n Power       (mW) = ");
        PAC[k].UpdatePower();
        for(int i = 0; i < 4; i++)
        {
            Serial.print(PAC[k].Power[i], 6);     Serial.print("\t");
        }
    }
    
    for(k = 0; k < 2; k++)
    {
        Serial.print("\n Power Acc   (mW) = ");
        PAC[k].UpdatePowerAcc() ;
        for(int i = 0; i < 4; i++)
        {
            Serial.print(PAC[k].PowerAcc[i], 6);  Serial.print("\t");
        }
        //Serial.print(PAC[k].PowerAccAll, 6); 
        Serial.print(uint32_t(PAC[k].PowerAccAll), HEX); 
    }

    for(k = 0; k < 2; k++)
    {
        Serial.print("\n Energy     (mWh) = ");
        PAC[k].UpdateEnergy();
        for(int i = 0; i < 4; i++)
        { 
            Serial.print(PAC[k].Energy[i], 6);  Serial.print("\t");
        }
    }
  
    delay(3000); // 3000 = 3 sec
}
