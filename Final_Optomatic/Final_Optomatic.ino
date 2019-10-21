

//////////////////////////////////////////////////////////////////////////////////////////
//
//    Demo code for the ADS1220 24-bit ADC breakout board
//
//    Author: Ashwin Whitchurch
//    Copyright (c) 2018 ProtoCentral
//
//    This example sequentially reads all 4 channels in continuous conversion mode
//
//    Arduino connections:
//
//  |ADS1220 pin label| Pin Function         |Arduino Connection|
//  |-----------------|:--------------------:|-----------------:|
//  | DRDY            | Data ready Output pin|  D6              |
//  | MISO            | Slave Out            | D12(D50 for Mega)|
//  | MOSI            | Slave In             | D11(D51 for Mega)|
//  | SCLK            | Serial Clock         | D13(D52 for Mega)|
//  | CS              | Chip Select          |  D7              |
//  | DVDD            | Digital VDD          |  +5V             |
//  | DGND            | Digital Gnd          |  Gnd             |
//  | AN0-AN3         | Analog Input         |  Analog Input    |
//  | AVDD            | Analog VDD           |  -               |
//  | AGND            | Analog Gnd           |  -               |
//
//    This software is licensed under the MIT License(http://opensource.org/licenses/MIT).
//
//   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
//   NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//   IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//   WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//   For information on how to use, visit https://github.com/Protocentral/Protocentral_ADS1220
//
/////////////////////////////////////////////////////////////////////////////////////////

#include "Protocentral_ADS1220.h"
#include <SPI.h>

#define PGA          1                 // Programmable Gain = 1
#define VREF         2.048            // Internal reference of 2.048V
#define VFSR         VREF/PGA
#define FULL_SCALE   (((long int)1<<23)-1)

#define ADS1220_CS_PIN    7
#define ADS1220_DRDY_PIN  6
int mux_channel_code_SE[4]={MUX_AIN0_AVSS,MUX_AIN1_AVSS,MUX_AIN2_AVSS,MUX_AIN3_AVSS,};
int peak_detector_enable_pin=27;
Protocentral_ADS1220 pc_ads1220;
int32_t adc_data;
long sampling(int channel, int samples)
{ 
  pc_ads1220.select_mux_channels(mux_channel_code_SE[4]);
  long long val=0;
  for(int i=0;i<samples;i++)
  {
    val+=pc_ads1220.Read_WaitForData();
  }
  val/=samples;
  return samples;
}
const int AD9850_clk=8;
const int AD9850_fqud=9;
const int AD9850_data=10;
const int AD9850_reset=11;

void init10khz()
{
  int64_t freqword=(10LL*((1LL<<32LL) -1LL));
  freqword= freqword*125000000LL;
  //freqword= freqword;
  for(int i=0;i<40;i++)
  {
    digitalWrite( AD9850_data, freqword&1);
    digitalWrite(AD9850_clk,LOW);
    digitalWrite(AD9850_clk,HIGH);
    digitalWrite(AD9850_clk,LOW);
    freqword>>=1LL;
  }
  digitalWrite(AD9850_fqud,LOW);
  digitalWrite(AD9850_fqud,HIGH);
  digitalWrite(AD9850_fqud,LOW);
}
void setup() {
  pinMode(AD9850_clk,OUTPUT);
  pinMode(AD9850_fqud,OUTPUT);
  pinMode(AD9850_data,OUTPUT);
  pinMode(AD9850_reset,OUTPUT);
  init10khz();
  Serial.begin(9600);

    pc_ads1220.begin(ADS1220_CS_PIN,ADS1220_DRDY_PIN);

    pc_ads1220.set_data_rate(DR_330SPS);
    pc_ads1220.set_pga_gain(PGA_GAIN_1);
  
    pc_ads1220.PGA_OFF();//turning off pga for single ended continous use
    pc_ads1220.Start_Conv();
    pc_ads1220.set_conv_mode_continuous();//Start continuous conversion mode
}

void loop() {
  if(Serial.available()
  {
    readMsg();
  }else
  {

}
