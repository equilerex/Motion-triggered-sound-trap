/*
  Motion triggered sound trap
  
  created 2021
  by Joosep koivistik   <http://www.koivistik.com>
  note: c++ by a javascript developer, excuse my lack of proper patterns :P
 
*/

// Pin definitions
#define backPin  8      // "back" button
#define playPin  6      // "play" button
#define motionPin 10    // Motion sensor
#define ledPin 12       // LED mosfet

// sound clip settings 
int clipLength = 20000;  // each audio clip should be exactly this length in milliseconds
int delayBetweenClips = 2000; // delay in between clips to avoid triggering too often
int buttonPressLength = 100; // length of the push button press on the soundcard (might need tweaking dependent on the sound card)
int resetAfterCount = 60; // the arduino and soundcard can go out of sync (maybe bad code, maybe hardware inconsistencies? not enough time to debug) so we re-sync the loop after this number of clips is played (pressing back button, playing a clip and stopping)




// runtime variables
bool firstRun = true;  
bool playInProgress = false;  
int motionDetected = 0;
int playsSinceReset = 0;
int realLength = clipLength - buttonPressLength; 

void setup() {
  // Initialize serial port for debugging.
  Serial.begin(115200);       
  Serial.print("\n Setup");     

  // initialize the LED pin as an output:
  pinMode(backPin, OUTPUT);
  pinMode(playPin, OUTPUT);
  pinMode(ledPin, OUTPUT); 
}

void loop() { 
    // on the very first run, we want to sync the arduino and the sound card's starting point.
    if (firstRun == true && playInProgress == false) { 
      
      Serial.print("\n firstRun"); 
      
      Serial.print("\n playsSinceReset: "); 
      Serial.print(playsSinceReset);  

      playInProgress = true;        
      // wait for the sound card to boot
      delay(1000);      
      Serial.print("\n Play length:");     
      Serial.print(realLength); 
      Serial.print("\n");       
      delay(3000);      

      syncArduinoAndSoundCard();
      
      firstRun = false;          
      Serial.print("\n Boot complete   "); 
    }

    motionDetected = digitalRead(motionPin);
   
    // play a fresh track
    if (firstRun == false && playInProgress == false && motionDetected == HIGH) { 
      Serial.print("\n Motion Detected");  
      
      Serial.print("\n playsSinceReset: "); 
      Serial.print(playsSinceReset);  


      // we want to re-sync arduino and the sound card every now and then
      if (playsSinceReset == resetAfterCount) { 
        syncArduinoAndSoundCard();
        playsSinceReset = 0;
      } else {
        playSounClip();
      }
      
    } 

}

void playSounClip() {
    
      Serial.print("\n play nr: "); 
      Serial.print(playsSinceReset); 
       // lock the session
      playInProgress = true;

      // light it up
      digitalWrite(ledPin, HIGH);
 
      // start the track
      pressPlayButton();

      // let it play for the length of the clip
      Serial.print("\n wait clip length   ");      
      delay(realLength);    

      // stop the track
      pressPlayButton();   

      // lights off
      digitalWrite(ledPin, LOW); 

      // add a buffer so the trap wont fire too often
      Serial.print("\n wait delay"); 
      delay(delayBetweenClips);  

      // unlock session
      playInProgress = false;
      playsSinceReset++;
      
      Serial.print("\n Next track prepped and ready to play");   
}

void syncArduinoAndSoundCard() {
      Serial.print("\n syncArduinoAndSoundCard");   
    // lock the session
      playInProgress = true;

      // light it up 
      digitalWrite(ledPin, HIGH); 
      // we press back button and let it play for the length of the track to make sure we are exactly at the beginning of a fresh track
      pressBackButton(); 
      Serial.print("\n wait clip length");      
      delay(realLength);   
      pressPlayButton();   
      Serial.print("\n first track prepped and ready to play");   
      digitalWrite(ledPin, LOW);
       // add a buffer 
      delay(3000); 
      playInProgress = false;
}

void pressPlayButton() {           
  digitalWrite(playPin, HIGH);
  delay(buttonPressLength);
  digitalWrite(playPin, LOW);
  Serial.print("\n Press Play Button"); 
}

void pressBackButton() {          
  digitalWrite(backPin, HIGH);
  delay(buttonPressLength);
  digitalWrite(backPin, LOW);
  Serial.print("\n press back Button"); 
} 
