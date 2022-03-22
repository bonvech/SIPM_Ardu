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
    //Serial.print("\n Voltage    (mV) = ");
    //Serial.print(PAC.Voltage);
    Serial.print("\n Voltage1    (mV) = ");
    Serial.print(PAC.Voltage1);
    Serial.print("\n Voltage2    (mV) = ");
    Serial.print(PAC.Voltage2);
    Serial.print("\n Voltage3    (mV) = ");
    Serial.print(PAC.Voltage3);
    Serial.print("\n Voltage4    (mV) = ");
    Serial.print(PAC.Voltage4);

      
    PAC.UpdateVsense();
    Serial.print("\n Vsense1     (mV) = ");
    Serial.print(PAC.Vsense1,6);
    Serial.print("\n Vsense2     (mV) = ");
    Serial.print(PAC.Vsense2,6);
    Serial.print("\n Vsense3     (mV) = ");
    Serial.print(PAC.Vsense3,6);
    Serial.print("\n Vsense4     (mV) = ");
    Serial.print(PAC.Vsense4,6);
    
    PAC.UpdateCurrent();
    Serial.print("\n Current     (mA) = ");
    Serial.print(PAC.Current,  5); Serial.print("\t");
    Serial.print(PAC.Current1, 5); Serial.print("\t");
    Serial.print(PAC.Current2, 5); Serial.print("\t");
    Serial.print(PAC.Current3, 5); Serial.print("\t");
    Serial.print(PAC.Current4, 5);

    
  //  Serial.print("\n Raw Power (HEX) = ");
  //    PAC.UpdatePowerRaw();
  //    Serial.print(PAC.PowerRaw, HEX);
  //    Serial.print("\n Power      (mW) = ");
  //    PAC.UpdatePower();
  //    Serial.print(PAC.Power,6);
  //    Serial.print("\n Power Acc  (mW) = ");
  //    PAC.UpdatePowerAcc() ;
  //    Serial.print(PAC.PowerAcc,6);
  //    Serial.print("\n Energy    (mWh) = ");
  //    PAC.UpdateEnergy();
  //    Serial.print(PAC.Energy,6);
  //
  delay(2000);

}
