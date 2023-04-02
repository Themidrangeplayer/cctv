#include <IRremote.h>
#include <SD.h>
#include <SPI.h>
#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal.h>

// initialize camera pins
Const int camera1 = 2;
Const int camera2 = 3;
Const int camera3 = 4;
Const int camera4 = 5;
Const int camera5 = 6;
Const int camera6 = 7;
Const int camera7 = 8;
Const int camera8 = 9;

// initialize ultrasonic sensor pins
Const int trigPin1 = 10;
Const int echoPin1 = 11;
Const int trigPin2 = 12;
Const int echoPin2 = 13;
Const int trigPin3 = A0;
Const int echoPin3 = A1;
Const int trigPin4 = A2;
Const int echoPin4 = A3;
Const int trigPin5 = A4;
Const int echoPin5 = A5;

// initialize microphone sensor pins
Int mic1 = A6;
Int mic2 = A7;
Int mic3 = 2;
Int mic4 = 3;
Int mic5 = 4;
Int mic6 = 5;
Int mic7 = 6;
Int mic8 = 7;

// initialize LED pin
Int ledPin = 13;

// initialize IR receiver pin
Int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
Decode_results results;

// initialize RTC
RTC_DS1307 rtc;

// initialize LCD screen
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

// initialize SD card pin
Const int chipSelect = 53;

Void setup() {
  // initialize camera pins as outputs
  pinMode(camera1, OUTPUT);
  pinMode(camera2, OUTPUT);
  pinMode(camera3, OUTPUT);
  pinMode(camera4, OUTPUT);
  pinMode(camera5, OUTPUT);
  pinMode(camera6, OUTPUT);
  pinMode(camera7, OUTPUT);
  pinMode(camera8, OUTPUT);

  // initialize ultrasonic sensor pins as inputs
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
  pinMode(trigPin4, OUTPUT);
  pinMode(echoPin4, INPUT);
  pinMode(trigPin5, OUTPUT);
  pinMode(echoPin5, INPUT);

  // initialize microphone sensor pins as inputs
  pinMode(mic1, INPUT);
  pinMode(mic2, INPUT);
  pinMode(mic3, INPUT);
  pinMode(mic4, INPUT);
  pinMode(mic5, INPUT);
  pinMode(mic6, INPUT);
  pinMode(mic7, INPUT);
  pinMode(mic8, INPUT);

  // initialize LED pin as output
  pinMode(ledPin, OUTPUT);

  // initialize IR receiver
  Irrecv.enableIRIn();

  // initialize RTC
  Wire.begin();
  Rtc.begin();

  // initialize LCD screen
  Lcd.begin(16, 2);

  // initialize SD card
  If (!SD.begin(chipSelect)) {
    Lcd.print(“SD card failed!”);
    Return;
  }
}

Void loop() {
  // get current time from RTC
  DateTime now = rtc.now();

  // check if current time is between 11:00 pm and 6:00 am
  If (now.hour() >= 23 || now.hour() < 
  If (motionDetected) {
    // start recording
    recordStart = millis();
    digitalWrite(ledPin, HIGH);
    for (int I = 0; I < numCameras; i++) {
      String filename = generateFilename(i);
      videoFile[i] = SD.open(filename, FILE_WRITE);
      cameras[i].startRecording(videoFile[i]);
    }
    // play sound on speaker
    Tone(speakerPin, 1000, 1000);
    Delay(1000);
  }
  
  // check if recording needs to be stopped
  If (millis() – recordStart >= recordTime) {
    For (int I = 0; I < numCameras; i++) {
      Cameras[i].stopRecording();
      videoFile[i].close();
    }
    digitalWrite(ledPin, LOW);
  }
}

Void loop() {
  // check if it’s within the designated time frame
  DateTime now = rtc.now();
  If (now.hour() >= startHour || now.hour() < endHour) {
    // check for motion
    checkMotion();
  } else {
    // turn off LED
    digitalWrite(ledPin, LOW);
  }
  
  // blink LED
  If (millis() – blinkStart >= blinkInterval) {
    blinkStart = millis();
    digitalWrite(ledPin, !digitalRead(ledPin));
  }
  
  // check for sound
  If (digitalRead(soundPin)) {
    // play sound on speaker
    Tone(speakerPin, 1000, 1000);
    Delay(1000);
  }
  
  // check for IR remote signal
  If (irrecv.decode(&results)) {
    If (results.value == irCode) {
      // stop recording and close files
      For (int I = 0; I < numCameras; i++) {
        Cameras[i].stopRecording();
        videoFile[i].close();
      }
      digitalWrite(ledPin, LOW);
    }
    Irrecv.resume();
  }
}
Void loop() {
  If (irrecv.decode(&results)) {
    // process IR code
    Switch (results.value) {
      Case IR_RECORD_CODE:
        // record a new IR code
        Serial.println(“Recording IR code…”);
        Irrecord();
        Serial.println(“Done.”);
        Break;
      Case IR_PLAY_CODE:
        // play back last recorded IR code
        Serial.println(“Playing back IR code…”);
        Irsend.sendRaw(lastCode, sizeof(lastCode) / sizeof(lastCode[0]), khz);
        Break;
    }
    Irrecv.resume();
  }

  // check if motion is detected
  Bool motionDetected = false;
  For (int I = 0; I < NUM_SENSORS; i++) {
    If (distance[i] < MAX_DISTANCE) {
      motionDetected = true;
      break;
    }
  }

  If (motionDetected) {
    Serial.println(“Motion detected!”);

    // turn on LED to indicate recording
    digitalWrite(LED_PIN, HIGH);

    // start recording
    String filename = getFilename();
    startRecording(filename);

    // check for loud sounds
    Bool loudSoundsDetected = false;
    For (int I = 0; I < NUM_MICROPHONES; i++) {
      If (microphoneValues[i] > LOUD_SOUND_THRESHOLD) {
        loudSoundsDetected = true;
        break;
      }
    }

    If (loudSoundsDetected) {
      Serial.println(“Loud sounds detected! Playing sound…”);

      // play loud sound through speaker
      Tone(SPEAKER_PIN, MAX_VOLUME);
      Delay(SOUND_DURATION);
      noTone(SPEAKER_PIN);
    }

    // wait for motion to stop
    While (motionDetected) {
      motionDetected = false;
      for (int I = 0; I < NUM_SENSORS; i++) {
        if (distance[i] < MAX_DISTANCE) {
          motionDetected = true;
          break;
        }
      }
      Delay(1000);  // wait 1 second before checking again
    }

    // stop recording
    stopRecording();

    // turn off LED
    digitalWrite(LED_PIN, LOW);
  }

  // blink LED to indicate program is running
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  delay(1000);
}
  //check if there is movement in any of the sensors
  For (int I = 0; I < NUM_SENSORS; i++) {
    If (distance[i] <= DETECTION_DISTANCE) {
      Detected[i] = true;
      anyDetected = true;
    } else {
      Detected[i] = false;
    }
  }

  //if any sensor detects movement, start recording
  If (anyDetected && !recording) {
    Serial.println(“Motion Detected! Recording started.”);
    Recording = true;
    startRecording();
  }

  //if no sensor detects movement, stop recording
  If (!anyDetected && recording) {
    Serial.println(“No Motion Detected! Recording stopped.”);
    stopRecording();
    recording = false;
  }
  
  //blink LED to indicate that cameras are recording
  If (recording) {
    digitalWrite(LED_PIN, (millis() / BLINK_INTERVAL) % 2);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  //check if loud sound is detected
  If (micReading >= MAX_SOUND_LEVEL) {
    Serial.println(“Loud sound detected! Playing alarm.”);
    playAlarm();
  }

  Delay(100); //wait for 100ms
}
    // check if there is any movement detected by ultrasonic sensors
    For(int i=0; i<5; i++) {
      If(distance[i] < 50) {
        motionDetected = true;
        break;
      }
    }
  
    // check if there is any loud sound detected by microphones
    For(int i=0; i<8; i++) {
      If(micReading[i] > 800) {
        loudSoundDetected = true;
        break;
      }
    }
  
    // if there is movement or loud sound detected, start recording video
    If(motionDetected || loudSoundDetected) {
      Serial.println(“Motion or loud sound detected, starting video recording…”);
  
      // blink LED to indicate that cameras are recording
      For(int i=0; i<8; i++) {
        digitalWrite(cameraLEDs[i], HIGH);
      }
      Delay(500);
      For(int i=0; i<8; i++) {
        digitalWrite(cameraLEDs[i], LOW);
      }
  
      // create new video file with current date and time
      String filename = “/recordings/” + String(year()) + “-“ + String(month()) + “-“ + String(day()) + “-“ + String(hour()) + “-“ + String(minute()) + “-“ + String(second()) + “.avi”;
  
      // start recording video on all cameras
      For(int i=0; i<8; i++) {
        Cameras[i].startRecording(filename, 20);
      }
  
      // record video until there is no more movement or loud sound detected
      While(motionDetected || loudSoundDetected) {
  
        // update ultrasonic sensor readings
        For(int i=0; i<5; i++) {
          Distance[i] = ultrasonic[i].ping_cm();
        }
  
        // update microphone readings
        For(int i=0; i<8; i++) {
          micReading[i] = analogRead(microphones[i]);
        }
  
        // check if there is any movement detected by ultrasonic sensors
        motionDetected = false;
        for(int i=0; i<5; i++) {
          if(distance[i] < 50) {
            motionDetected = true;
            break;
          }
        }
  
        // check if there is any loud sound detected by microphones
        loudSoundDetected = false;
        for(int i=0; i<8; i++) {
          if(micReading[i] > 800) {
            loudSoundDetected = true;
            break;
          }
        }
  
        // blink LED to indicate that cameras are still recording
        For(int i=0; i<8; i++) {
          digitalWrite(cameraLEDs[i], HIGH);
        }
        Delay(500);
        For(int i=0; i<8; i++) {
          digitalWrite(cameraLEDs[i], LOW);
        }
  
        // delay for 500ms before checking for movement or loud sound again
        Delay(500);
      }
  
      // stop recording video on all cameras
      For(int i=0; i<8; i++) {
        Cameras[i].stopRecording();
      }
  
      Serial.println(“Video recording stopped.”);
    }
  }
}
  // check if any of the ultrasonic sensors detect movement
  For(int i=0; i<5; i++){
    If(distance[i] < distanceThreshold){
      If(!motionDetected){
        // if motion is detected for the first time, start recording
        startRecording();
        motionDetected = true;
      }
      // update the last motion time
      lastMotionTime = millis();
      break;
    }
  }

  // check if the motion has stopped
  If(motionDetected && (millis() – lastMotionTime > motionStopTime)){
    stopRecording();
    motionDetected = false;
  }
  
  // check if the recording has exceeded maximum recording time
  If(recording && (millis() – recordingStartTime > maxRecordingTime)){
    stopRecording();
  }

  // check if it’s time to start the program
  If(currentTime >= startTime && currentTime <= endTime){
    // blink LED every 1 second
    If((millis() – ledBlinkTime) >= 1000){
      ledBlinkTime = millis();
      digitalWrite(ledPin, !digitalRead(ledPin));
    }
    // check if any sound is detected by the microphones
    If(checkSound()){
      // if loud sound is detected, play alarm sound
      Tone(buzzerPin, 5000, 500);
      Delay(500);
      noTone(buzzerPin);
    }
  }
  Else{
    digitalWrite(ledPin, LOW);
  }

  Delay(100);
}
Void loop() {
  // Check for loud sounds
  For (int I = 0; I < numMics; i++) {
    If (analogRead(micPins[i]) > loudnessThreshold) {
      digitalWrite(speakerPin, HIGH);
      delay(1000);
      digitalWrite(speakerPin, LOW);
      break;
    }
  }

  // Check for movement
  For (int I = 0; I < numSensors; i++) {
    Int distance = readDistance(sensorPins[i]);
    If (distance < maxDistance) {
      If (!isRecording) {
        startRecording();
      }
      lastMotionTime = millis();
    }
  }

  // Stop recording if there has been no motion for a while
  If (isRecording && millis() – lastMotionTime > stopRecordingDelay) {
    stopRecording();
  }

  // Blink LED to indicate that cameras are recording
  If (isRecording) {
    If (millis() – lastBlinkTime >= blinkInterval) {
      digitalWrite(ledPin, !digitalRead(ledPin));
      lastBlinkTime = millis();
    }
  } else {
    digitalWrite(ledPin, LOW);
  }

  // Check for IR remote commands
  If (irrecv.decode(&results)) {
    handleIRCommand(results.value);
    irrecv.resume();
  }
}
      If (irrecv.decode(&results)) {  // Process IR remote signals
        Irrecv.resume();
        Switch (results.value) {
          Case 0xFFA25D:  // Power button pressed
            If (!isRecording) {
              Serial.println(“Starting recording”);
              startRecording();
            } else {
              Serial.println(“Stopping recording”);
              stopRecording();
            }
            Break;
          Case 0xFFE21D:  // Stop button pressed
            stopRecording();
            break;
        }
      }

      // Check ultrasonic sensors for movement
      For (int I = 0; I < numSensors; i++) {
        Distance = measureDistance(i);

        If (distance <= triggerDistance) {
          If (!isRecording) {
            startRecording();
          }
          If (millis() – lastTriggered[i] >= triggerInterval) {
            Serial.print(“Motion detected by sensor “);
            Serial.print(I + 1);
            Serial.print(“, distance = “);
            Serial.println(distance);
            lastTriggered[i] = millis();
          }
        }
      }

      // Check microphone for loud sounds
      For (int I = 0; I < numMics; i++) {
        Int micLevel = analogRead(micPins[i]);
        If (micLevel > loudnessThreshold) {
          If (!isRecording) {
            startRecording();
          }
          If (millis() – lastLoudness >= loudnessInterval) {
            Serial.print(“Loud sound detected by mic “);
            Serial.print(I + 1);
            Serial.print(“, level = “);
            Serial.println(micLevel);
            lastLoudness = millis();
            playSound();
          }
        }
      }

      // Blink LED to indicate that cameras are recording
      If (isRecording) {
        If (millis() – lastBlink >= blinkInterval) {
          digitalWrite(ledPin, !digitalRead(ledPin));
          lastBlink = millis();
        }
      } else {
        digitalWrite(ledPin, LOW);
      }

      Delay(10);
    }
  }

  Void startRecording() {
    isRecording = true;
    time_t now = time(nullptr);
    String timestamp = String(now);
    SD.mkdir(“/recordings”);
    currentFilename = “/recordings/recording_” + timestamp + “.avi”;
    Serial.print(“Recording to “);
    Serial.println(currentFilename);
    For (int I = 0; I < numCameras; i++) {
      Cameras[i].startRecording(currentFilename);
    }
  }

  Void stopRecording() {
    isRecording = false;
    Serial.println(“Stopping recording”);
    For (int I = 0; I < numCameras; i++) {
      Cameras[i].stopRecording();
    }
  }

  Void playSound() {
    Tone(speakerPin, MAX_VOLUME);
    Delay(1000);
    noTone(speakerPin);
  }

  Float measureDistance(int sensorIndex) {
    digitalWrite(triggerPins[sensorIndex], LOW);
    delayMicroseconds(2);
    digitalWrite(triggerPins[sensorIndex], HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPins[sensorIndex], LOW);
    float duration = pulseIn(echoPins[sensorIndex], HIGH);
    return duration * 0.034 / 2;
  }
}
Void recordVideo(int camera) {
  Serial.println(“Recording video…”);
  digitalWrite(ledPin, HIGH); // turn on LED to indicate recording
  String filename = getFilename(camera);
  Char fileChar[filename.length() + 1];
  Filename.toCharArray(fileChar, filename.length() + 1);
  Char command[100];
  Sprintf(command, “ffmpeg -f video4linux2 -I /dev/video%d -t %d -y -c:v libx264 -preset ultrafast -crf 0 -c:a copy %s”, camera, videoLength, fileChar);
  Serial.println(command);
  System(command);
  digitalWrite(ledPin, LOW); // turn off LED
}

String getFilename(int camera) {
  Char filename[100];
  Sprintf(filename, “%04d%02d%02d_%02d%02d%02d_camera%d.mp4”, year(), month(), day(), hour(), minute(), second(), camera);
  Return String(filename);
}

Void loop() {
  // check for movement
  Int movement = checkMovement();
  If (movement) {
    // record video from each camera
    For (int I = 1; I <= numCameras; i++) {
      recordVideo(i);
    }
  }
  // check for loud sound
  Int loudSound = checkLoudSound();
  If (loudSound) {
    digitalWrite(speakerPin, HIGH);
    delay(500);
    digitalWrite(speakerPin, LOW);
  }
  // check if it’s time to turn on/off cameras
  Int currentHour = hour();
  If (currentHour >= startHour || currentHour < endHour) {
    digitalWrite(powerPin, HIGH); // turn on cameras
  } else {
    digitalWrite(powerPin, LOW); // turn off cameras
  }
  // blink LED to indicate functioning
  digitalWrite(ledPin, !digitalRead(ledPin));
  delay(1000);
}
Void loop() {
  // Check if it’s within the designated time period
  If (currentTime.hour() >= START_HOUR || currentTime.hour() < END_HOUR) {
    // Check for motion on all ultrasonic sensors
    For (int I = 0; I < NUM_SENSORS; i++) {
      If (digitalRead(sensorPins[i]) == HIGH) {
        // Record video footage from all cameras
        For (int j = 0; j < NUM_CAMERAS; j++) {
          If (!isRecording[j]) {
            isRecording[j] = true;
            startRecording(j);
          }
        }

        // Turn on LED to indicate recording is in progress
        digitalWrite(LEDPin, HIGH);

        // Record audio if there is a loud sound
        If (analogRead(micPin) > THRESHOLD) {
          Tone(speakerPin, MAX_VOLUME);
        }

        // Wait for motion to stop before stopping recording
        While (digitalRead(sensorPins[i]) == HIGH) {
          Delay(10);
        }

        // Stop recording video footage from all cameras
        For (int j = 0; j < NUM_CAMERAS; j++) {
          If (isRecording[j]) {
            isRecording[j] = false;
            stopRecording(j);
          }
        }

        // Turn off LED to indicate recording has stopped
        digitalWrite(LEDPin, LOW);

        // Stop audio recording
        noTone(speakerPin);

        // Generate a filename based on the current date and time
        String filename = String(currentTime.year()) + “-“ + String(currentTime.month()) + “-“ + String(currentTime.day()) + “_” + String(currentTime.hour()) + “-“ + String(currentTime.minute()) + “-“ + String(currentTime.second());

        // Create a new file for the recorded footage
        For (int j = 0; j < NUM_CAMERAS; j++) {
          If (isRecording[j]) {
            String filepath = “/” + cameras[j] + “/” + filename + “.avi”;
            If (SD.exists(filepath)) {
              SD.remove(filepath);
            }
            File[j] = SD.open(filepath, FILE_WRITE);
          }
        }

        // Wait for a short period before checking for motion again
        Delay(1000);
      }
    }
  }

  // Update current time
  currentTime = rtc.now();
}
    // stop recording and save file
    Void stopRecording() {
      // stop recording
      For (int I = 0; I < NUM_CAMERAS; i++) {
        Cameras[i].stopRecording();
      }
      // create filename with date and time
      Char filename[30];
      Sprintf(filename, “/%04d-%02d-%02d_%02d-%02d-%02d.h264”, year(), month(), day(), hour(), minute(), second());
      // create new file and open for writing
      Char filepath[40];
      Sprintf(filepath, “%s%s”, DIRECTORY_PATH, filename);
      currentFile = SD.open(filepath, FILE_WRITE);
      if (!currentFile) {
        Serial.println(“Failed to create file!”);
      }
      Else {
        Serial.println(“New file created: “ + String(filepath));
        currentFile.write(cameraNames, sizeof(cameraNames));
      }
    }

    // check if it’s nighttime
    Bool isNighttime() {
      Int currentHour = hour();
      Return (currentHour >= 23 || currentHour < 6);
    }

    Void loop() {
      If (isNighttime()) {
        // check for motion
        Bool isMotionDetected = false;
        For (int I = 0; I < NUM_ULTRASONIC_SENSORS; i++) {
          If (digitalRead(ultrasonicSensorPins[i]) == HIGH) {
            isMotionDetected = true;
            break;
          }
        }
        // check for sound
        Bool isLoudSoundDetected = false;
        For (int I = 0; I < NUM_MICROPHONES; i++) {
          If (microphones[i].detectLoudSound()) {
            isLoudSoundDetected = true;
            break;
          }
        }
        // start or continue recording
        If (isMotionDetected || isLoudSoundDetected) {
          If (!isRecording) {
            startRecording();
          }
          Else {
            Record();
          }
          // blink LED
          digitalWrite(LED_PIN, !digitalRead(LED_PIN));
        }
        // stop recording
        Else {
          If (isRecording) {
            stopRecording();
          }
          // turn off LED
          digitalWrite(LED_PIN, LOW);
        }
      }
      // delay for one second
      Delay(1000);
    }
      If (pirState == LOW) {
        // we have just turned off
        Serial.println(“Motion ended!”);
        digitalWrite(ledPin, LOW);
        // stop recording and close the file
        stopRecording();
      }
      pirState = digitalRead(pirPin);
    }
  }
}
Void loop() {
  // check for loud sounds
  For (int I = 0; I < numMics; i++) {
    Int micVal = analogRead(micPins[i]);
    If (micVal > loudnessThreshold) {
      Serial.print(“Loud sound detected on microphone “);
      Serial.println(i+1);
      // play sound on speaker
      digitalWrite(speakerPin, HIGH);
      delay(1000);
      digitalWrite(speakerPin, LOW);
    }
  }
  
  // check for movement using ultrasonic sensors
  For (int I = 0; I < numSensors; i++) {
    // send ultrasonic pulse
    digitalWrite(trigPins[i], LOW);
    delayMicroseconds(2);
    digitalWrite(trigPins[i], HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPins[i], LOW);
    
    // measure distance
    Long duration = pulseIn(echoPins[i], HIGH);
    Int distance = duration * 0.034 / 2;
    
    If (distance < sensorThreshold) {
      Serial.print(“Motion detected on sensor “);
      Serial.println(i+1);
      // start recording and turn on LED
      startRecording(i+1);
      digitalWrite(ledPin, HIGH);
      
      // wait for motion to stop before stopping recording
      While (distance < sensorThreshold) {
        Delay(500);
        Duration = pulseIn(echoPins[i], HIGH);
        Distance = duration * 0.034 / 2;
      }
      
      // turn off LED and stop recording
      Serial.println(“Motion ended!”);
      digitalWrite(ledPin, LOW);
      stopRecording();
    }
  }
}
Void startRecording(int sensorNum) {
  // generate filename based on sensor number and timestamp
  String filename = “SENSOR” + String(sensorNum) + “_” + String(millis()) + “.avi”;
  
  // start recording
  If (cam1.available()) {
    Serial.println(“Recording started on “ + filename);
    Cam1.startRecording(filename);
  }
}

Void stopRecording() {
  // stop recording
  If (cam1.recording()) {
    Serial.println(“Recording stopped”);
    Cam1.stopRecording();
  }
}

