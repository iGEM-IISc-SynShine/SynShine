// based on libraries created by
// Gonzalo Recio (grsynth.com)
// Robo Ulbricht (arduinoslovakia.eu/page/ad9850-generator-signalov)

//interchangeable pins
const int AD9850_clk=4;
const int AD9850_fqud=7;
const int AD9850_data=8;
const int AD9850_reset=12;

//Frequency of your crystal oscillator (CLKIN input pin 9 in datasheet), measured in MHz.
// This reference frequency must be higher than 1MHz.
long long XTAL_MHZ=125000000;

//Relationship value between actual frequency and 32-bit word sent in the serial streaming, basically the 32bit number for which we get 125 Mhz
long long FREQ_FACTOR=4294967295;

class AD9850
{
  int W_CLK;
  int FQ_UD;
  int DATA;
  int RESET;
  
public:
  AD9850(int givenW_CLK, int givenFQ_UD, int givenDATA, int givenRESET);

  int init();
  void doReset();
  void osc(long long  Freq,double phase);
  //void sweepUp(double minFreq, double maxFreq, double inc, int cyclesPerDelay);
  //void sweepDown(double minFreq, double maxFreq, double inc, int cyclesPerDelay);
  //void sweepLoop(double minFreq, double maxFreq, double inc, int cyclesPerDelay);
  void powerDown();

  long currentf;
};

////////////////////////////////////////////////////////////////////////////////////

#define _DEBUG 0

//Constructor for the class AD9850
AD9850::AD9850(int givenW_CLK, int givenFQ_UD, int givenDATA, int givenRESET)
{
W_CLK = givenW_CLK;
FQ_UD = givenFQ_UD;
DATA  = givenDATA;
RESET = givenRESET;
currentf = 0;

pinMode(W_CLK, OUTPUT);
pinMode(FQ_UD, OUTPUT);
pinMode(DATA, OUTPUT);
pinMode(RESET, OUTPUT);
}

int AD9850::init()
{
digitalWrite(W_CLK, LOW);
digitalWrite(FQ_UD, LOW);
digitalWrite(DATA, LOW);
digitalWrite(RESET, LOW);
#if _DEBUG
Serial.println("All the pins lowered.");
#endif
return 0;
}

/* Reset operation for the AD9850. This function must be called before using AD9850_Osc
 * first time in the execution (check page 12 on datasheet)
 */
void AD9850::doReset()
{
digitalWrite(W_CLK, LOW);
digitalWrite(W_CLK, HIGH);
digitalWrite(W_CLK, LOW);

digitalWrite(RESET, LOW);
digitalWrite(RESET, HIGH);
digitalWrite(RESET, LOW);

digitalWrite(FQ_UD, LOW);
digitalWrite(FQ_UD, HIGH);
digitalWrite(FQ_UD, LOW);

osc(0,0);
currentf = 0;

#if _DEBUG
Serial.println("Reset succesfull.");
#endif
}

/* Sets the DDS sine and square oscillator to the detailed "frequency" and "phase" variables.
 * "frequency" will be turned into a 32 bit word, so the frequency resolution of 0.0291 Hz
 * with a 125 MHz reference clock. "phase" will be a 5 bit word instead so the resolution is
 * 11.5 degrees, or pi/32 radians.
 */
void AD9850::osc(long long frequency, double phase)
{
#if _DEBUG
Serial.print("Changing frequency to: ");
Serial.println(frequency);
#endif

long y=(long)(frequency*FREQ_FACTOR)/XTAL_MHZ;
while(phase>360)
  phase-=360;
long z=phase/11.5;
        
#if _DEBUG
Serial.print("Frequency word: ");
Serial.println(y);
#endif
  
int i;
        
//Frequency 32-bit word
for (i=0;i<32;i++){
  digitalWrite(DATA, (y>>i) & 0x01);
  digitalWrite(W_CLK, HIGH);
  digitalWrite(W_CLK, LOW);
  }
  
//control bit #1, control bit #2 and Power off, all to low
digitalWrite(DATA, LOW);
digitalWrite(W_CLK, HIGH);
digitalWrite(W_CLK, LOW);
digitalWrite(W_CLK, HIGH);
digitalWrite(W_CLK, LOW);
digitalWrite(W_CLK, HIGH);
digitalWrite(W_CLK, LOW);
  
//phase 5-bit word
for (i=0;i<5;i++){
  digitalWrite(DATA, (z>>i) & 0x01);
  digitalWrite(W_CLK, HIGH);
  digitalWrite(W_CLK, LOW);
  }

digitalWrite(FQ_UD, HIGH);
digitalWrite(FQ_UD, LOW);
}

/* Enables power down mode. This method is used for a quick "all outputs" disable.
 * The effect is the same as AD9850_Osc(0,0), but it takes less clock cycles
 */
void AD9850::powerDown()
{
#if _DEBUG
Serial.println("Changing mode to power down.");
#endif
int PDword=0x04;
int i;
digitalWrite(FQ_UD, HIGH);
digitalWrite(FQ_UD, LOW);

for (i=0;i<8;i++){
  if((PDword>>i)&0x01)
    digitalWrite(DATA, HIGH);
  else
    digitalWrite(DATA, LOW);
  digitalWrite(W_CLK, HIGH);
  digitalWrite(W_CLK, LOW);
  }

digitalWrite(FQ_UD, HIGH);
digitalWrite(FQ_UD, LOW);
}


// create instance

AD9850 device(AD9850_clk,AD9850_fqud,AD9850_data,AD9850_reset);


// commands created as per protocol

int setval(AD9850 device, long f)
{
  device.currentf = f;
  device.osc(device.currentf, 0);

  return 0;
}



int set_sine(AD9850 device, int a)
{
  if (a)
  {
    device.osc(device.currentf, 0);
  }

  else
  {
    device.powerDown();
  }
}

void setup()
{
Serial.begin(9600);
//Serial.println("AD9850 serial demo");
 
device.init();
device.doReset();
setval(device,10000);
}


void loop()
{

 
}
