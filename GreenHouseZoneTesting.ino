
// A0-A4  Water pots 1-5
// 13     RUN LED

//************************************************************************************************************
// IMPORTANT NOTES

// SAMD5x can sink/sourse 8mA
// Using analog pins A0-A4 for zone watering

//************************************************************************************************************

#define zone1WaterPin   A0                   
#define zone2WaterPin   A1
#define zone3WaterPin   A2
#define zone4WaterPin   A3
#define zone5WaterPin   A4

#define flushPin        A5

#define LEDpin          13

#define OFF HIGH                          // Active LOW inputs on the external relay board
#define ON  LOW                           // Active LOW inputs on the external relay board

// for the timer and time
#define WATER_Zone1 0x03
unsigned long WATER_Zone1_int = 120000;           // 2 minute watering DURATION timer. Water ON for 2 minutes
#define WATER_Zone2 0x04
unsigned long WATER_Zone2_int = 120000;           // 2 minute watering DURATION timer. Water ON for 2 minutes
#define WATER_Zone3 0x05
unsigned long WATER_Zone3_int = 120000;           // 2 minute watering DURATION timer. Water ON for 2 minutes
#define WATER_Zone4 0x06
unsigned long WATER_Zone4_int = 120000;           // 2 minute watering DURATION timer. Water ON for 2 minutes
#define WATER_Zone5 0x07
unsigned long WATER_Zone5_int = 120000;           // NOT USED. 2 minute watering DURATION timer. Water ON for 2 minutes
#define RUNNING     0x08
unsigned long RUNNING_int   = 250;          // LED toggle only indicates prgram running

// init the timers                       
unsigned long WATER_Zone1_lastRead_millis;
unsigned long WATER_Zone2_lastRead_millis;
unsigned long WATER_Zone3_lastRead_millis;
unsigned long WATER_Zone4_lastRead_millis;
unsigned long WATER_Zone5_lastRead_millis;
unsigned long RUNNING_lastRead_millis;

// Variables to hold current time from decodeTime()
int UTC_hours   = 25;                    // init to 25 to prevent actions until a valid NTP time is received. 25 is invalid time.
int UTC_minutes = 65;                    // same as above
int UTC_seconds = 0;                     //


int waterSchedule[]   {6,0,18,55};       // 24 hour clock. {H,M,H,M.....}

bool waterZone1ON        = false;             // true if it is time to water
bool waterZone2ON        = false;             // true if it is time to water
bool waterZone3ON        = false;             // true if it is time to water
bool waterZone4ON        = false;             // true if it is time to water
bool waterZone5ON        = false;             // true if it is time to water

bool wateringZone1ON     = false;             // true if watering is active... i.e. water valves are open
bool wateringZone2ON     = false;             // true if watering is active... i.e. water valves are open
bool wateringZone3ON     = false;             // true if watering is active... i.e. water valves are open
bool wateringZone4ON     = false;             // true if watering is active... i.e. water valves are open
bool wateringZone5ON     = false;             // Not used. true if watering is active... i.e. water valves are open



bool  crcFAIL = false;                  // CRC used in serial communication with RPi.  NOT USED currently

// memory for communication from RPi
uint8_t   RPirecBlock[16];                              // Data block received from RPi. 15 bytes for data, one byte for CRC
uint8_t   RPirecCRC;                                    // CRC included with received data block (from RPi)
uint8_t   calcCRC;                                      // calculated CRC8 of the data received from RPi

void setup() {

  // Open serial communications
  Serial.begin(9600);

  pinMode(zone1WaterPin, OUTPUT);                   // pin A0 as GPIO OUTPUT
  digitalWrite(zone1WaterPin, OFF);
  pinMode(zone2WaterPin, OUTPUT);                   // pin A1
  digitalWrite(zone2WaterPin, OFF);
  pinMode(zone3WaterPin, OUTPUT);                   // pin A2
  digitalWrite(zone3WaterPin, OFF);
  pinMode(zone4WaterPin, OUTPUT);                   // pin A3
  digitalWrite(zone4WaterPin, OFF);
  pinMode(zone5WaterPin, OUTPUT);                   // pin A4
  digitalWrite(zone5WaterPin, OFF);
  
  pinMode(flushPin, OUTPUT);                        // pin A5
  digitalWrite(flushPin, OFF);

  pinMode(LEDpin, OUTPUT);                          // pin 13
  digitalWrite(LEDpin, LOW);

// Init the timers
unsigned long current_millis;
current_millis = millis();
WATER_Zone1_lastRead_millis       = current_millis;
WATER_Zone2_lastRead_millis       = current_millis;
WATER_Zone3_lastRead_millis       = current_millis;
WATER_Zone4_lastRead_millis       = current_millis;
WATER_Zone5_lastRead_millis       = current_millis;
RUNNING_lastRead_millis           = current_millis;
}

void loop() {

  // TOGGLE the run state LED
  if (timer_lapsed(RUNNING) == true){
    digitalWrite(LEDpin, !digitalRead(LEDpin));
  }

  receiveRPiData();
  
  // check to see if it's time to water
  waterPots();

}
