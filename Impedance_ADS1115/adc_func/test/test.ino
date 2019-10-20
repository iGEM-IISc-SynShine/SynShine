#include <Wire.h>
#include <Adafruit_ADS1015.h>

float Multiplier = 0.1875F;

Adafruit_ADS1115 ads;

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////// Connections to be made ///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

// ADS1115      |   Arduino
// Vdd          |   5V
// Gnd          |   Gnd
// SDA          |   SDA
// SCL          |   SCL
// ADDR         |   GND


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////REMEMBER THIS. CAUTION ADVICED./////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

  // The ADC input range (or gain) can be changed via the following
  // functions, but be careful never to exceed VDD +0.3V max, or to
  // exceed the upper and lower limits if you adjust the input range!
  // Setting these values incorrectly may destroy your ADC!
  //                                                                ADS1015  ADS1115
  //                                                                -------  -------
  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

int init_adc()
{
  ads.begin();
}

long read_val_adc(int a) // If a = 1, gives V_(01) else if a = 2, gives V_(23)
{
  if(a == 1)
  {
    return (long)ads.readADC_Differential_0_1();
  }
  else if(a == 2)
  {
    return (long)ads.readADC_Differential_2_3();
  }
  else
  {
    return -1;
  }
}

int set_gain(int a)
{
  //                                            Maximum Voltage              Least Count / Multiplier
  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V        a = 1      0.1875mV (default)
  // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V        a = 2      0.125mV
  // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V        a = 3      0.0625mV
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V        a = 4      0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V        a = 5      0.015625mV
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V        a = 6      0.0078125mV

  if(a == 1){
    ads.setGain(GAIN_TWOTHIRDS);
    Multiplier = 0.1875F;
  }
  else if(a == 2){
    ads.setGain(GAIN_ONE);
    Multiplier = 0.125F;
  }
  else if(a == 3){
    ads.setGain(GAIN_TWO);
    Multiplier = 0.0625F;
  }
  else if(a == 4){
    ads.setGain(GAIN_FOUR);
    Multiplier = 0.03125F;
  }
  else if(a == 5){
    ads.setGain(GAIN_EIGHT);
    Multiplier = 0.015625F;
  }
  else if(a == 6){
    ads.setGain(GAIN_SIXTEEN);
    Multiplier = 0.0078125F;
  }
  else{
    return -1;
  }
  return 0;
}

long adc_reading(double* gain, double* V_true){
  int expc_gain = 1;                //The index of the set_gain function
  //long max_vol[5] = {4.096, 2.048, 1.024, 0.512, 0.256};
  
  long val = read_val_adc(1); // Reading V_01
  if(val<0){
    val*=-1;
  }
  
  long threshold = ((1<<13));
  
  while(expc_gain<5){
    set_gain(expc_gain);
    if(val>threshold){
      break;
    }
    expc_gain++;
    val = read_val_adc(1); //Reading V_01
    if(val<0){
      val*=-1;
    }
  }
/*  
  if(expc_gain==7){
    expc_gain = 1;
    set_gain(expc_gain);
  }
*/  
  long avg = 0;
  for(int i=0; i<100; i++){
    avg+=read_val_adc(1);
  }
  *V_true = Multiplier*avg/100.0;
  avg /= 100;
  if(expc_gain == 1){
    avg*=24; 
  }
  else{
    avg = (avg<<(6-expc_gain));
  }
  if(expc_gain == 1){
    *gain = 0.66;
  }
  else{
    long k = 1<<(expc_gain-2);
    *gain = (double)k;
  }
  set_gain(1);
  return avg;
  //Serial.println(avg);
}

void setup(void)
{
  Serial.begin(9600);
  init_adc();
}

void loop(void)
{
  double gain;
  double y;
  long x = adc_reading(&gain,&y);
  Serial.print("Gain = "); Serial.print(gain); Serial.print(" 1.Differential: "); Serial.print(x); Serial.print("("); Serial.print(y); Serial.println("mV)");
  delay(1000);
}
