// These are the system timers. Using millis() function. Each timer has a NAME_int to define the length of time in ms.
// if current time - start time >= whatever_int then {}
// 
bool timer_lapsed(uint8_t PID){                             // timer. used for short interval scheduling 1sec- a few minutes

  if (PID == WATER_Zone1){
    if ((millis() - WATER_Zone1_lastRead_millis) >= WATER_Zone1_int){   // set to 2minutes
      //WATER_Zone1_lastRead_millis = millis();
      return true;
    }
    else {return false;}
  }
    
  if (PID == WATER_Zone2){
    if ((millis() - WATER_Zone2_lastRead_millis) >= WATER_Zone2_int){   // set to 2minutes
      //WATER_Zone2_lastRead_millis = millis();
      return true;
    }
    else {return false;}
  }

  if (PID == WATER_Zone3){
    if ((millis() - WATER_Zone3_lastRead_millis) >= WATER_Zone3_int){   // set to 2minutes
      //WATER_Zone3_lastRead_millis = millis();
      return true;
    }
    else {return false;}
  }

  if (PID == WATER_Zone4){
    if ((millis() - WATER_Zone4_lastRead_millis) >= WATER_Zone4_int){   // set to 2minutes
      //WATER_Zone4_lastRead_millis = millis();
      return true;
    }
    else {return false;}
  }

  if (PID == WATER_Zone5){
    if ((millis() - WATER_Zone5_lastRead_millis) >= WATER_Zone5_int){   // set to 2minutes
      //WATER_Zone5_lastRead_millis = millis();
      return true;
    }
    else {return false;}
  }

  if (PID == INHIBIT_Zone1){
    if ((millis() - INHIBIT_Zone1_lastRead_millis) >= INHIBIT_Zone1_int){    // set to 60sec
      //INHIBIT_Zone1_lastRead_millis = millis();
      return true;
    }
    else {return false;}
  }

  if (PID == INHIBIT_Zone2){
    if ((millis() - INHIBIT_Zone2_lastRead_millis) >= INHIBIT_Zone2_int){    // set to 60sec
      //INHIBIT_Zone1_lastRead_millis = millis();
      return true;
    }
    else {return false;}
  }

  if (PID == INHIBIT_Zone3){
    if ((millis() - INHIBIT_Zone3_lastRead_millis) >= INHIBIT_Zone3_int){    // set to 60sec
      //INHIBIT_Zone1_lastRead_millis = millis();
      return true;
    }
    else {return false;}
  }

  if (PID == INHIBIT_Zone4){
    if ((millis() - INHIBIT_Zone4_lastRead_millis) >= INHIBIT_Zone4_int){    // set to 60sec
      //INHIBIT_Zone1_lastRead_millis = millis();
      return true;
    }
    else {return false;}
  }

  if (PID == INHIBIT_Zone5){
    if ((millis() - INHIBIT_Zone5_lastRead_millis) >= INHIBIT_Zone5_int){    // set to 60sec
      //INHIBIT_Zone1_lastRead_millis = millis();
      return true;
    }
    else {return false;}
  }

  if (PID == FLUSH_water){
    if ((millis() - FLUSH_lastRead_millis) >= FLUSH_water_int){    // set to 60sec
      return true;
    }
    else {return false;}
  }

  if (PID == INHIBIT_flush){
    if ((millis() - INHIBIT_flush_lastRead_millis) >= INHIBIT_flush_int){    // 2 hours
      return true;
    }
    else {return false;}
  }

  if (PID == PRINT){
    if ((millis() - PRINT_lastRead_millis) >= PRINT_int){    // set to 60sec
      PRINT_lastRead_millis = millis();
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

    // Water Line purge before second daily watering. First watering doesn't need a flush
    if (flushinhibit == false){
      if ((UTC_hours == flushSchedule[0]) && (UTC_minutes == flushSchedule[1])){
        flushinhibit = true;
        INHIBIT_flush_lastRead_millis = millis();
        digitalWrite(flushPin, ON);
        digitalWrite(TESTpin, ON);      // testing flush inhibit
        FLUSH_lastRead_millis = millis();
      }  
    }
    // check for end of flush
    if (digitalRead(!flushPin)){           // true if LOW (active low)
      if (timer_lapsed(FLUSH_water)){
        digitalWrite(flushPin, OFF);
      }
    }
    // Check flush inhibit timer
    if (flushinhibit == true){
      if (timer_lapsed(INHIBIT_flush)){
        flushinhibit = false;
        digitalWrite(TESTpin,OFF);
      }
    }


    // ZONE 1 watering control
    if ((waterZone1ON == false) && (waterZone1Inhibit == false)){   // waterON = false if it's not time to water
      if (((UTC_hours == waterScheduleZone1[0]) && (UTC_minutes == waterScheduleZone1[1])) || 
         ((UTC_hours == waterScheduleZone1[2]) && (UTC_minutes == waterScheduleZone1[3]))) {
        waterZone1ON = true;                                        // waterON = time to water. Doesn't mean that watering is active
        waterZone1Inhibit = true;
        INHIBIT_Zone1_lastRead_millis = millis();                   // start the inhibit timer. Prevent early restart of watering time
      }
    }
    if ((waterZone1ON == true) && (wateringZone1ON == false)){      //waterON if inside watering window and wateringON is not active
        wateringZone1ON = true;                                     // wateringON is true when  watering is active
          digitalWrite(zone1WaterPin, ON);                          // turn Zone1 watering valve ON
        WATER_Zone1_lastRead_millis = millis();                     // init the wateringZone1 timer start value
    }
    // Check if Zone1 watering timer has elapsed
    if ((waterZone1ON == true) && (wateringZone1ON == true)){       // if Zone1 is already watering (valves ON)
      endwatering = timer_lapsed(WATER_Zone1);                      // endwatering true when watering time has expired
      if (endwatering == true){
        waterZone1ON = false;                                       // reset the waterON to false
        wateringZone1ON = false;                                    // turn off Zone1 water valve
          digitalWrite(zone1WaterPin, OFF);
      }
    }
    // Check if Zone1 watering inhibit timer has elapsed
    if (waterZone1Inhibit == true){          
      endinhibit = timer_lapsed(INHIBIT_Zone1);                     // endwatering true when watering time has expired
      if (endinhibit == true){
        waterZone1Inhibit = false;                                  // reset the waterON to false
      }
    }

    // ZONE 2 watering control
    if ((waterZone2ON == false) && (waterZone2Inhibit == false)){   // waterON = false if it's not time to water
      if (((UTC_hours == waterScheduleZone2[0]) && (UTC_minutes == waterScheduleZone2[1])) || 
         ((UTC_hours == waterScheduleZone2[2]) && (UTC_minutes == waterScheduleZone2[3]))) {
        waterZone2ON = true;                                        // waterON = time to water. Doesn't mean that watering is active
        waterZone2Inhibit = true;
        INHIBIT_Zone2_lastRead_millis = millis();                   // start the inhibit timer. Prevent early restart of watering time
      }
    }
    if ((waterZone2ON == true) && (wateringZone2ON == false)){      //waterON if inside watering window and wateringON is not active
        wateringZone2ON = true;                                     // wateringON is true when  watering is active
          digitalWrite(zone2WaterPin, ON);                          // turn Zone1 watering valve ON
        WATER_Zone2_lastRead_millis = millis();                     // init the wateringZone1 timer start value
    }
    // Check if Zone2 watering timer has elapsed
    if ((waterZone2ON == true) && (wateringZone2ON == true)){       // if Zone1 is already watering (valves ON)
      endwatering = timer_lapsed(WATER_Zone2);                      // endwatering true when watering time has expired
      if (endwatering == true){
        waterZone2ON = false;                                       // reset the waterON to false
        wateringZone2ON = false;                                    // turn off Zone1 water valve
          digitalWrite(zone2WaterPin, OFF);
      }
    }
    // Check if Zone2 watering inhibit timer has elapsed
    if (waterZone2Inhibit == true){          
      endinhibit = timer_lapsed(INHIBIT_Zone2);                     // endwatering true when watering time has expired
      if (endinhibit == true){
        waterZone2Inhibit = false;                                  // reset the waterON to false
      }
    }

    // ZONE 3 watering control
    if ((waterZone3ON == false) && (waterZone3Inhibit == false)){   // waterON = false if it's not time to water
      if (((UTC_hours == waterScheduleZone3[0]) && (UTC_minutes == waterScheduleZone3[1])) || 
         ((UTC_hours == waterScheduleZone3[2]) && (UTC_minutes == waterScheduleZone3[3]))) {
        waterZone3ON = true;                                        // waterON = time to water. Doesn't mean that watering is active
        waterZone3Inhibit = true;
        INHIBIT_Zone3_lastRead_millis = millis();                   // start the inhibit timer. Prevent early restart of watering time
      }
    }
    if ((waterZone3ON == true) && (wateringZone3ON == false)){      //waterON if inside watering window and wateringON is not active
        wateringZone3ON = true;                                     // wateringON is true when  watering is active
          digitalWrite(zone3WaterPin, ON);                          // turn Zone1 watering valve ON
        WATER_Zone3_lastRead_millis = millis();                     // init the wateringZone1 timer start value
    }
    // Check if Zone3 watering timer has elapsed
    if ((waterZone3ON == true) && (wateringZone3ON == true)){       // if Zone1 is already watering (valves ON)
      endwatering = timer_lapsed(WATER_Zone3);                      // endwatering true when watering time has expired
      if (endwatering == true){
        waterZone3ON = false;                                       // reset the waterON to false
        wateringZone3ON = false;                                    // turn off Zone1 water valve
          digitalWrite(zone3WaterPin, OFF);
      }
    }
    // Check if Zone3 watering inhibit timer has elapsed
    if (waterZone3Inhibit == true){          
      endinhibit = timer_lapsed(INHIBIT_Zone3);                     // endwatering true when watering time has expired
      if (endinhibit == true){
        waterZone3Inhibit = false;                                  // reset the waterON to false
      }
    }

    // ZONE 4 watering control
    if ((waterZone4ON == false) && (waterZone4Inhibit == false)){   // waterON = false if it's not time to water
      if (((UTC_hours == waterScheduleZone4[0]) && (UTC_minutes == waterScheduleZone4[1])) || 
         ((UTC_hours == waterScheduleZone4[2]) && (UTC_minutes == waterScheduleZone4[3]))) {
        waterZone4ON = true;                                        // waterON = time to water. Doesn't mean that watering is active
        waterZone4Inhibit = true;
        INHIBIT_Zone4_lastRead_millis = millis();                   // start the inhibit timer. Prevent early restart of watering time
      }
    }
    if ((waterZone4ON == true) && (wateringZone4ON == false)){      //waterON if inside watering window and wateringON is not active
        wateringZone4ON = true;                                     // wateringON is true when  watering is active
          digitalWrite(zone4WaterPin, ON);                          // turn Zone1 watering valve ON
        WATER_Zone4_lastRead_millis = millis();                     // init the wateringZone1 timer start value
    }
    // Check if Zone4 watering timer has elapsed
    if ((waterZone4ON == true) && (wateringZone4ON == true)){       // if Zone1 is already watering (valves ON)
      endwatering = timer_lapsed(WATER_Zone4);                      // endwatering true when watering time has expired
      if (endwatering == true){
        waterZone4ON = false;                                       // reset the waterON to false
        wateringZone4ON = false;                                    // turn off Zone1 water valve
          digitalWrite(zone4WaterPin, OFF);
      }
    }
    // Check if Zone4 watering inhibit timer has elapsed
    if (waterZone4Inhibit == true){          
      endinhibit = timer_lapsed(INHIBIT_Zone4);                     // endwatering true when watering time has expired
      if (endinhibit == true){
        waterZone4Inhibit = false;                                  // reset the waterON to false
      }
    }

    // ZONE 5 watering control
    if ((waterZone5ON == false) && (waterZone5Inhibit == false)){   // waterON = false if it's not time to water
      if (((UTC_hours == waterScheduleZone5[0]) && (UTC_minutes == waterScheduleZone5[1])) || 
         ((UTC_hours == waterScheduleZone5[2]) && (UTC_minutes == waterScheduleZone5[3]))) {
        waterZone5ON = true;                                        // waterON = time to water. Doesn't mean that watering is active
        waterZone5Inhibit = true;
        INHIBIT_Zone5_lastRead_millis = millis();                   // start the inhibit timer. Prevent early restart of watering time
      }
    }
    if ((waterZone5ON == true) && (wateringZone5ON == false)){      //waterON if inside watering window and wateringON is not active
        wateringZone5ON = true;                                     // wateringON is true when  watering is active
          digitalWrite(zone5WaterPin, ON);                          // turn Zone1 watering valve ON
        WATER_Zone5_lastRead_millis = millis();                     // init the wateringZone1 timer start value
    }
    // Check if Zone5 watering timer has elapsed
    if ((waterZone5ON == true) && (wateringZone5ON == true)){       // if Zone1 is already watering (valves ON)
      endwatering = timer_lapsed(WATER_Zone5);                      // endwatering true when watering time has expired
      if (endwatering == true){
        waterZone5ON = false;                                       // reset the waterON to false
        wateringZone5ON = false;                                    // turn off Zone1 water valve
          digitalWrite(zone5WaterPin, OFF);
      }
    }
    // Check if Zone5 watering inhibit timer has elapsed
    if (waterZone5Inhibit == true){          
      endinhibit = timer_lapsed(INHIBIT_Zone5);                     // endwatering true when watering time has expired
      if (endinhibit == true){
        waterZone5Inhibit = false;                                  // reset the waterON to false
      }
    }

}

void printData(void){
  Serial.print("TIME:   ");
  Serial.print(UTC_hours);
  Serial.print(":");
    if (UTC_minutes < 10) {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      Serial.print('0');
    }
  Serial.println(UTC_minutes);

  Serial.print("WaterZone1 is ");
  if (waterZone1ON == false){
    Serial.print("OFF");
  }
  else {Serial.print("ON");
  }
  Serial.println("");

  Serial.print("WaterZone2 is ");
  if (waterZone2ON == false){
    Serial.print("OFF");
  }
  else {Serial.print("ON");
  }
  Serial.println("");

  Serial.print("WaterZone3 is ");
  if (waterZone3ON == false){
    Serial.print("OFF");
  }
  else {Serial.print("ON");
  }
  Serial.println("");

  Serial.print("WaterZone4 is ");
  if (waterZone4ON == false){
    Serial.print("OFF");
  }
  else {Serial.print("ON");
  }
  Serial.println("");

  Serial.print("WaterZone5 is ");
  if (waterZone5ON == false){
    Serial.print("OFF");
  }
  else {Serial.print("ON");
  }
  
  Serial.println("");
}
















  
