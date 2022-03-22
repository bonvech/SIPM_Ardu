#include <Microchip_PAC193x.h>
#include <Wire.h>
#include <AD5671R.h>

Microchip_PAC193x PAC;
AD5671R dac;

#define Addr AD5671R_DAC_A  // address of first of four possible DACs: A1=GND pin A0=GND pin
#define CH0  AD5671R_CH_0   // channel 0 of DAC
#define CH1  AD5671R_CH_1   // channel 1 of DAC
#define CH2  AD5671R_CH_2   // channel 2 of DAC
#define CH3  AD5671R_CH_3   // channel 3 of DAC
#define CH4  AD5671R_CH_4   // channel 4 of DAC
#define CH5  AD5671R_CH_5   // channel 5 of DAC
#define CH6  AD5671R_CH_6   // channel 6 of DAC
#define CH7  AD5671R_CH_7   // channel 7 of DAC

const uint8_t stateNormal = AD5671R_STATE_NORMAL;
const uint8_t stateNC     = AD5671R_STATE_TRISTATE;
const uint8_t WriteUpdate = AD5671R_CMD_WRITE_UPDATE_DAC_CH_N;
const uint8_t ReadBack = AD5671R_CMD_READBACK_ENABLE;
//const uint8_t state2gain  = AD5671R_CMD_INTERNAL_REFER_SETUP;
uint16_t dac_code[16];
int i=0;
int Vp=3277;

 
#define Temp_1   A0  // аналоговый вход датчика температуры светодиода 
#define DAC_A0   2
#define DAC_A1   3
#define DAC_RESET   9
#define DAC_RSTSEL  8
#define T_SEL 12


int TA0_1 = 4;
int TA2_3 = 5;
int TA4_5 = 6;
int TA6 = 7;
int val = 0;


void setup() {
    Wire.begin();
    PAC.begin();
    Serial.begin(9600);
    
    while (! Serial); // Wait until Serial is ready - Leonardo
    
    Serial.print("\n Product      ID: ");
    PAC.UpdateProductID();
    Serial.print(PAC.ProductID, HEX);
    
    Serial.print("\n Manufacturer ID: ");
    PAC.UpdateManufacturerID();
    Serial.print(PAC.ManufacturerID, HEX);
    
    Serial.print("\n Revision     ID: ");
    PAC.UpdateRevisionID();
    Serial.print(PAC.RevisionID, HEX);
    
    analogReference(EXTERNAL);
       
    pinMode(DAC_A0, OUTPUT);
    pinMode(DAC_A1, OUTPUT);
    
    digitalWrite(DAC_A0, LOW);  //DAC_A0
    digitalWrite(DAC_A1, LOW);  //DAC_A1
    
    pinMode(DAC_RESET, OUTPUT);
    pinMode(DAC_RSTSEL, OUTPUT);
    digitalWrite(DAC_RESET, HIGH); 
    digitalWrite(DAC_RSTSEL, LOW);
    
    pinMode(T_SEL, OUTPUT);
    
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);
    
    delay(2000);

    // --- DAC begin ---
    dac.begin();
    //All channels is enabled. (For turn off CH - stateNC)
    dac.setPowerState(Addr, stateNormal, stateNormal, stateNormal, stateNormal, stateNormal, stateNormal, stateNormal, stateNormal);
    dac.setGain(Addr,1,0); //DAC's outputs have gain х2 for voltages range 0..5V, Internal Reference.
    
    dac.setDAC(Addr, CH0, WriteUpdate, Vp); // 1235 -> 1,5В при gain=2
    dac.setDAC(Addr, CH1, WriteUpdate, Vp); // 3500 -> 4,25В при gain=2. 4,25 + 24,5 = 28,75В
    dac.setDAC(Addr, CH2, WriteUpdate, Vp);
    dac.setDAC(Addr, CH3, WriteUpdate, Vp);
    dac.setDAC(Addr, CH4, WriteUpdate, Vp);
    dac.setDAC(Addr, CH5, WriteUpdate, Vp);
    dac.setDAC(Addr, CH6, WriteUpdate, Vp);   
    dac.setDAC(Addr, CH7, WriteUpdate, 2048);  
}



void loop() {
    Serial.print("\n\nRead start:");
    
    PAC.UpdateVoltage();
    Serial.print("\n Voltage    (mV) = ");
    Serial.print(PAC.Voltage1, 2); Serial.print("\t");
    Serial.print(PAC.Voltage2, 2); Serial.print("\t");
    Serial.print(PAC.Voltage3, 2); Serial.print("\t");
    Serial.print(PAC.Voltage4, 2);
    
    PAC.UpdateVsense();
    Serial.print("\n Vsense     (mV) = ");
    Serial.print(PAC.Vsense,  5); Serial.print("\t");
    Serial.print(PAC.Vsense1, 5); Serial.print("\t");
    Serial.print(PAC.Vsense2, 5); Serial.print("\t");
    Serial.print(PAC.Vsense3, 5); Serial.print("\t");
    Serial.print(PAC.Vsense4, 5);
    
    PAC.UpdateCurrent();
    Serial.print("\n Current    (mA) = ");
    Serial.print(PAC.Current,  5); Serial.print("\t");
    Serial.print(PAC.Current1, 5); Serial.print("\t");
    Serial.print(PAC.Current2, 5); Serial.print("\t");
    Serial.print(PAC.Current3, 5); Serial.print("\t");
    Serial.print(PAC.Current4, 5);
    
    PAC.UpdatePowerRaw();
    Serial.print("\n Raw Power (HEX) = ");
    Serial.print(PAC.PowerRaw,  HEX); Serial.print("\t");
    Serial.print(PAC.PowerRaw1, HEX); Serial.print("\t");
    Serial.print(PAC.PowerRaw2, HEX); Serial.print("\t");
    Serial.print(PAC.PowerRaw3, HEX); Serial.print("\t");
    Serial.print(PAC.PowerRaw4, HEX);
    
    PAC.UpdatePower();
    Serial.print("\n Power      (mW) = ");
    Serial.print(PAC.Power,  5); Serial.print("\t");
    Serial.print(PAC.Power1, 5); Serial.print("\t");
    Serial.print(PAC.Power2, 5); Serial.print("\t");
    Serial.print(PAC.Power3, 5); Serial.print("\t");
    Serial.print(PAC.Power4, 5);
    
    PAC.UpdatePowerAcc() ;   
    Serial.print("\n Power Acc  (mW) = ");
    Serial.print(PAC.PowerAcc,  5); Serial.print("\t");
    Serial.print(PAC.PowerAcc1, 5); Serial.print("\t");
    Serial.print(PAC.PowerAcc2, 5); Serial.print("\t");
    Serial.print(PAC.PowerAcc3, 5); Serial.print("\t");
    Serial.print(PAC.PowerAcc4, 5);
    
    PAC.UpdateEnergy();
    Serial.print("\n Energy    (mWh) = ");
    Serial.print(PAC.Energy,  5); Serial.print("\t");
    Serial.print(PAC.Energy1, 5); Serial.print("\t");
    Serial.print(PAC.Energy2, 5); Serial.print("\t");
    Serial.print(PAC.Energy3, 5); Serial.print("\t");
    Serial.print(PAC.Energy4, 5);

    
    // --- read temperature
    digitalWrite(T_SEL, LOW);
    delay(3000);
    
    val = analogRead(Temp_1);     
    Serial.print("\nT1 = "); Serial.println(val);
    
    val = analogRead(A1); 
    Serial.print("T2 = ");   Serial.println(val);
               
    val = analogRead(A2); 
    Serial.print("T3 = ");   Serial.println(val);  
        
    val = analogRead(A3);
    Serial.print("T4 = ");   Serial.println(val);
    
    digitalWrite(T_SEL, HIGH);
    delay(3000);
    
    val = analogRead(A0);     
    Serial.print("T5 = ");  Serial.println(val);
    
    val = analogRead(A1); 
    Serial.print("T6 = ");  Serial.println(val);           
    
    val = analogRead(A2); 
    Serial.print("T7 = ");  Serial.println(val);
      
    val = analogRead(A3);
    Serial.print("T8 = ");  Serial.println(val);
    
    delay(5000);
}
