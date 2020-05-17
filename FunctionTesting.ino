// These are the system timers. Using millis() function. Each timer has a NAME_int to define the length of time in ms.
// if current time - start time >= whatever_int then {}
// 
bool timer_lapsed(uint8_t PID){                             // timer. used for short interval scheduling 1sec- a few minutes

  if (PID == WATER_Zone1){
    if ((millis() - WATER_Zone1_lastRead_millis) >= WATER_Zone1_int){   // set to 2minutes
      WATER_Zone1_lastRead_millis = millis();
      return true;
    }
    else {return false;}
    }
    
  if (PID == WATER_Zone2){
    if ((millis() - WATER_Zone2_lastRead_millis) >= WATER_Zone2_int){   // set to 2minutes
      WATER_Zone2_lastRead_millis = millis();
      return true;
    }
    else {return false;}
    }

  if (PID == WATER_Zone3){
    if ((millis() - WATER_Zone3_lastRead_millis) >= WATER_Zone3_int){   // set to 2minutes
      WATER_Zone3_lastRead_millis = millis();
      return true;
    }
    else {return false;}
    }

  if (PID == WATER_Zone4){
    if ((millis() - WATER_Zone4_lastRead_millis) >= WATER_Zone4_int){   // set to 2minutes
      WATER_Zone4_lastRead_millis = millis();
      return true;
    }
    else {return false;}
    }

  if (PID == WATER_Zone5){
    if ((millis() - WATER_Zone5_lastRead_millis) >= WATER_Zone5_int){   // set to 2minutes
      WATER_Zone5_lastRead_millis = millis();
      return true;
    }
    else {return false;}
    }


  if (PID == RUNNING){
    if ((millis() - RUNNING_lastRead_millis) >= RUNNING_int){        // set to .5 sec
      RUNNING_lastRead_millis = millis();
      return true;
    }
    else {return false;}
    }
}

// Read data block from RPi
void receiveRPiData(void){
  
  int inCount = Serial.available();
  
  if (inCount < 6) {
    return;
    }     //return if less than 6 (plus CR?) bytes are avaialable
  
  if (inCount > 6) { 
    Serial.println("Input Buffer Overflow in function receiveRPiData()");
    // flush the input buffer
    while (Serial.available()) {
      Serial.read();
    }
    return; // if 
  }
  
  if (inCount == 6){    // read and store the buffer contents
      for (int i=0; i < 6; i++){
      RPirecBlock[i]=Serial.read();
      }
  }
      
  if ((RPirecBlock[0] == 72) && (RPirecBlock[2] == 77) && (RPirecBlock[4] == 83)) {
    // IF Data received conforms to format HxMxSx... or 72-"H", 77="M" and 83="S"
    // Is a verification of the received data
    // DATA block received verified. Set Time.
    UTC_hours   = RPirecBlock[1];
    UTC_minutes = RPirecBlock[3];
    UTC_seconds = RPirecBlock[5];      
  }
}


void waterPots(void){
  bool endwatering;               // endwatering true when watering time/duration has expired
                                   
    // ZONE 1 watering control
    if (waterZone1ON == false){              // waterON = false if it's not time to water
      if ((UTC_hours == waterSchedule[0]) && (UTC_minutes == waterSchedule[1])){
        waterZone1ON = true;                 // waterON = time to water. Doesn't mean that watering is active
      }
    }
    if ((waterZone1ON == true) && (wateringZone1ON == false)){   //waterON if inside watering window and wateringON is not active
        wateringZone1ON = true;                                  // wateringON is true when  watering is active
          digitalWrite(zone1WaterPin, ON);                       // turn Zone1 watering valve ON
        WATER_Zone1_lastRead_millis = millis();                  // init the wateringZone1 timer start value
    }
    // Check if Zone1 watering timer has elapsed
    if ((waterZone1ON == true) && (wateringZone1ON == true)){          // if Zone1 is already watering (valves ON)
      endwatering = timer_lapsed(WATER_Zone1);               // endwatering true when watering time has expired
      if (endwatering == true){
        waterZone1ON = false;                                          // reset the waterON to false
        wateringZone1ON = false;                                  // turn off Zone1 water valve
          digitalWrite(zone1WaterPin, OFF);
      }
    }

                                   
    // ZONE 2 watering control
    if (waterZone2ON == false){              // waterON = false if it's not time to water
      if ((UTC_hours == waterSchedule[0]) && (UTC_minutes == waterSchedule[1])){
        waterZone2ON = true;                 // waterON = time to water. Doesn't mean that watering is active
      }
    }
    if ((waterZone2ON == true) && (wateringZone2ON == false)){   //waterON if inside watering window and wateringON is not active
        wateringZone2ON = true;                                  // wateringON is true when  watering is active
          digitalWrite(zone2WaterPin, ON);                       // turn Zone1 watering valve ON
        WATER_Zone2_lastRead_millis = millis();                  // init the wateringZone1 timer start value
    }
    // Check if Zone2 watering timer has elapsed
    if ((waterZone2ON == true) && (wateringZone2ON == true)){          // if Zone1 is already watering (valves ON)
      endwatering = timer_lapsed(WATER_Zone2);               // endwatering true when watering time has expired
      if (endwatering == true){
        waterZone2ON = false;                                          // reset the waterON to false
        wateringZone2ON = false;                                  // turn off Zone1 water valve
          digitalWrite(zone2WaterPin, OFF);
      }
    }

                                   
    // ZONE 3 watering control
    if (waterZone3ON == false){              // waterON = false if it's not time to water
      if ((UTC_hours == waterSchedule[0]) && (UTC_minutes == waterSchedule[1])){
        waterZone3ON = true;                 // waterON = time to water. Doesn't mean that watering is active
      }
    }
    if ((waterZone3ON == true) && (wateringZone3ON == false)){   //waterON if inside watering window and wateringON is not active
        wateringZone3ON = true;                                  // wateringON is true when  watering is active
          digitalWrite(zone3WaterPin, ON);                       // turn Zone1 watering valve ON
        WATER_Zone3_lastRead_millis = millis();                  // init the wateringZone1 timer start value
    }
    // Check if Zone3 watering timer has elapsed
    if ((waterZone3ON == true) && (wateringZone3ON == true)){          // if Zone1 is already watering (valves ON)
      endwatering = timer_lapsed(WATER_Zone3);               // endwatering true when watering time has expired
      if (endwatering == true){
        waterZone3ON = false;                                          // reset the waterON to false
        wateringZone3ON = false;                                  // turn off Zone1 water valve
          digitalWrite(zone3WaterPin, OFF);
      }
    }

                                   
    // ZONE 4 watering control
    if (waterZone4ON == false){              // waterON = false if it's not time to water
      if ((UTC_hours == waterSchedule[0]) && (UTC_minutes == waterSchedule[1])){
        waterZone4ON = true;                 // waterON = time to water. Doesn't mean that watering is active
      }
    }
    if ((waterZone4ON == true) && (wateringZone4ON == false)){   //waterON if inside watering window and wateringON is not active
        wateringZone4ON = true;                                  // wateringON is true when  watering is active
          digitalWrite(zone4WaterPin, ON);                       // turn Zone1 watering valve ON
        WATER_Zone4_lastRead_millis = millis();                  // init the wateringZone1 timer start value
    }
    // Check if Zone4 watering timer has elapsed
    if ((waterZone4ON == true) && (wateringZone4ON == true)){          // if Zone1 is already watering (valves ON)
      endwatering = timer_lapsed(WATER_Zone4);               // endwatering true when watering time has expired
      if (endwatering == true){
        waterZone4ON = false;                                          // reset the waterON to false
        wateringZone4ON = false;                                  // turn off Zone1 water valve
          digitalWrite(zone4WaterPin, OFF);
      }
    }
                                   
    // ZONE 5 watering control
    if (waterZone5ON == false){              // waterON = false if it's not time to water
      if ((UTC_hours == waterSchedule[0]) && (UTC_minutes == waterSchedule[1])){
        waterZone5ON = true;                 // waterON = time to water. Doesn't mean that watering is active
      }
    }
    if ((waterZone5ON == true) && (wateringZone5ON == false)){   //waterON if inside watering window and wateringON is not active
        wateringZone5ON = true;                                  // wateringON is true when  watering is active
          digitalWrite(zone5WaterPin, ON);                       // turn Zone1 watering valve ON
        WATER_Zone5_lastRead_millis = millis();                  // init the wateringZone1 timer start value
    }
    // Check if Zone5 watering timer has elapsed
    if ((waterZone5ON == true) && (wateringZone5ON == true)){          // if Zone1 is already watering (valves ON)
      endwatering = timer_lapsed(WATER_Zone5);               // endwatering true when watering time has expired
      if (endwatering == true){
        waterZone5ON = false;                                          // reset the waterON to false
        wateringZone5ON = false;                                  // turn off Zone1 water valve
          digitalWrite(zone5WaterPin, OFF);
      }
    }
}

void printData(void){}
