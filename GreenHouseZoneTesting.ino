
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
#define PROBE       0X01
unsigned long PROBE_int       = 60000;
#define PRINT       0x02
unsigned long PRINT_int       = 1000;

#define WATER_Zone1 0x03
unsigned long WATER_Zone1_int   = 60000;           // mSec how long the watering valves are open Zone1
#define WATER_Zone2 0x04
unsigned long WATER_Zone2_int   = 60000;           // mSec how long the watering valves are open Zone2
#define WATER_Zone3 0x05
unsigned long WATER_Zone3_int   = 60000;           // mSec how long the watering valves are open Zone3
#define WATER_Zone4 0x06
unsigned long WATER_Zone4_int   = 60000;           // mSec how long the watering valves are open Zone4
#define WATER_Zone5 0x07
unsigned long WATER_Zone5_int   = 60000;           // mSec how long the watering valves are open Zone5
#define INHIBIT_Zone1 0x8
unsigned long INHIBIT_Zone1_int = 120000;          // 120000ms insures that the timers do not reset within the watering window
#define INHIBIT_Zone2 0x09
unsigned long INHIBIT_Zone2_int = 120000;
#define INHIBIT_Zone3 0x0A
unsigned long INHIBIT_Zone3_int = 120000;
#define INHIBIT_Zone4 0x0B
unsigned long INHIBIT_Zone4_int = 120000;
#define INHIBIT_Zone5 0x0C
unsigned long INHIBIT_Zone5_int = 120000;

#define RUNNING     0x0D
unsigned long RUNNING_int     = 250;              // LED toggle only indicates prgram running


// timers
unsigned long INHIBIT_Zone1_lastRead_millis;
unsigned long INHIBIT_Zone2_lastRead_millis;
unsigned long INHIBIT_Zone3_lastRead_millis;
unsigned long INHIBIT_Zone4_lastRead_millis;
unsigned long INHIBIT_Zone5_lastRead_millis;

unsigned long WATER_Zone1_lastRead_millis;
unsigned long WATER_Zone2_lastRead_millis;
unsigned long WATER_Zone3_lastRead_millis;
unsigned long WATER_Zone4_lastRead_millis;
unsigned long WATER_Zone5_lastRead_millis;

unsigned long PRINT_lastRead_millis  ;
unsigned long RUNNING_lastRead_millis;

// Variables to hold current time from decodeTime()
int UTC_hours   = 25;                    // init to 25 to prevent actions until a valid NTP time is received. 25 is invalid time.
int UTC_minutes = 65;                    // same as above
int UTC_seconds = 0;                     //


int waterScheduleZone1[]   {6,0,17,0};        // watering times. [h,m,h,m]. Each zone has two watering start times (2x per day)
int waterScheduleZone2[]   {6,1,17,1};
int waterScheduleZone3[]   {6,2,17,2};
int waterScheduleZone4[]   {6,3,17,3};
int waterScheduleZone5[]   {6,4,17,4};

bool waterZone1Inhibit  = false;
bool waterZone2Inhibit  = false;
bool waterZone3Inhibit  = false;
bool waterZone4Inhibit  = false;
bool waterZone5Inhibit  = false;

bool waterZone1ON        = false;             // true if it is time to water
bool waterZone2ON        = false;             // true if it is time to water
bool waterZone3ON        = false;             // true if it is time to water
bool waterZone4ON        = false;             // true if it is time to water
bool waterZone5ON        = false;             // true if it is time to water

bool wateringZone1ON     = false;             // true if watering is active... i.e. water valves are open
bool wateringZone2ON     = false;             // true if watering is active... i.e. water valves are open
bool wateringZone3ON     = false;             // true if watering is active... i.e. water valves are open
bool wateringZone4ON     = false;             // true if watering is active... i.e. water valves are open
bool wateringZone5ON     = false;             // true if watering is active... i.e. water valves are open

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
PRINT_lastRead_millis             = current_millis;
RUNNING_lastRead_millis           = current_millis;
}

void loop() {

  // TOGGLE the run state LED
  if (timer_lapsed(RUNNING) == true){
    digitalWrite(LEDpin, !digitalRead(LEDpin));
  }

  receiveRPiData();
  
  waterPots();

  if (timer_lapsed(PRINT) == true) {  // print Time and Temp data to Serial
    printData();
  }


}
