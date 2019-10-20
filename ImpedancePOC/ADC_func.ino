//    Arduino connections:
//
//  |ADS1220 pin label| Pin Function         |Arduino Connection|
//  |-----------------|:--------------------:|-----------------:|
//  | DRDY            | Data ready Output pin|  D6              |
//  | MISO            | Slave Out            |  D12             |
//  | MOSI            | Slave In             |  D11             |
//  | SCLK            | Serial Clock         |  D13             |
//  | CS              | Chip Select          |  D7              |
//  | DVDD            | Digital VDD          |  +5V             |
//  | DGND            | Digital Gnd          |  Gnd             |
//  | AN0-AN3         | Analog Input         |  Analog Input    |
//  | AVDD            | Analog VDD           |  -               |
//  | AGND            | Analog Gnd           |  -               |

#include <Protocentral_ADS1220.h>
#include <SPI.h>

#define VREF 2.048            // Internal reference of 2.048V
#define FSR (((long int)1<<23)-1)

#define ADS1220_CS_PIN    7
#define ADS1220_DRDY_PIN  6

Protocentral_ADS1220 pc_ads1220;
int32_t adc_data;
int PGA = 1;                 // Programmable Gain = 1
float VFSR = VREF/PGA;

void setup(){
  
}

void loop(){
  
}

int init_adc(){   //Run this inside setup()
    pc_ads1220.begin(ADS1220_CS_PIN,ADS1220_DRDY_PIN);

    pc_ads1220.set_data_rate(DR_1000SPS);
    pc_ads1220.set_pga_gain(PGA_GAIN_1);

    pc_ads1220.set_conv_mode_single_shot(); //Set Single shot mode
}

long read_val_adc(int a){
  if(a > -1 && a < 5){
    adc_data = 0;
    if(a == 0){
      for(int i = 0; i< 1000; i++){ //1000 * delay(1) leads to averaging for 1s
        adc_data += pc_ads1220.Read_SingleShot_SingleEnded_WaitForData(MUX_SE_CH0);
        delay(1);
      }
      return adc_data/1000;
    }
    else if(a==1){
      for(int i = 0; i< 1000; i++){
        adc_data += pc_ads1220.Read_SingleShot_SingleEnded_WaitForData(MUX_SE_CH1);
        delay(1);
      }
      return adc_data/1000;
    }
    else if(a==2){
      for(int i = 0; i< 1000; i++){
        adc_data += pc_ads1220.Read_SingleShot_SingleEnded_WaitForData(MUX_SE_CH2);
        delay(1);
      }
      return adc_data/1000;
    }
    else if(a==3){
      for(int i = 0; i< 1000; i++){
        adc_data += pc_ads1220.Read_SingleShot_SingleEnded_WaitForData(MUX_SE_CH3);
        delay(1);
      }
      return adc_data/1000;
    }
  }
  else{
    return -1;
  }
}

int set_gain(int a){
  PGA = a;
  pc_ads1220.set_pga_gain(PGA);
  return 0;
}
