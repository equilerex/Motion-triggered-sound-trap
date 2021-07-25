/*
  Button

  
  created 2021
  by Joosep koivistik   <http://www.koivistik.com>
 
*/

// constants won't change. They're used here to set pin numbers:
#define buttonPin 2     // the number of the pushbutton pin
#define backPin  8      // the number of the LED pin
#define playPin  6      // the number of the LED pin                                     // Data pin to connect to the strip.
#define motionPin 10  
#define ledPin 12
  
int clipLength = 20000;
int delayBetweenClips = 2000;
int buttonPressLength = 100;


// variables will change: buttonPressLength
bool playInProgress = true;  
bool firstRun = true;  
int motionDetected = 0;
int realLength = clipLength - buttonPressLength;

void pressPlayButton() {      
  //Serial.print("\n pressPlayButton");          
  digitalWrite(playPin, HIGH);   // +5v to MOSFET gate
  
  delay(buttonPressLength);               // 
  
  Serial.print("\n pressPlayButton"); 
  digitalWrite(playPin, LOW);    // 0v to MOSFET gate 
}

void pressBackButton() {      
  //Serial.print("\n pressBackButton");        
         
  digitalWrite(backPin, HIGH);   // +5v to MOSFET gate
  delay(buttonPressLength);               // 
  
  Serial.print("\n pressbackButton"); 
  digitalWrite(backPin, LOW);    // 0v to MOSFET gate 
} 

void setup() {
    // Initialize serial port for debugging.
  Serial.begin(115200);      

  
      // initialize the LED pin as an output:
      pinMode(backPin, OUTPUT);
      pinMode(playPin, OUTPUT);
      pinMode(ledPin, OUTPUT); 
}

void loop() {
    if (firstRun == true) { 
      playInProgress = true; 
      firstRun = false;                 
      // Soft startup to ease the flow of electrons.
      delay(1000);      
      Serial.print(realLength); 
      Serial.print("\n");       
      delay(4000);       
      digitalWrite(ledPin, HIGH);   // +5v to MOSFET gate
      pressBackButton(); 
      Serial.print("\n wait clip length   ");      
      delay(realLength);   
      pressPlayButton();   
      Serial.print("\n stop    ");   
      digitalWrite(ledPin, LOW);   // +5v to MOSFET gate
      Serial.print("\n wait delay   "); 
      delay(3000);  // add a buffer so the trap wont fire too often
    
      playInProgress = false; // allow for new 
    } else { 
     motionDetected = digitalRead(motionPin);  // read input value
        //Serial.print(motionDetected);    
      if (motionDetected == HIGH && playInProgress == false) { // check if motion detected and nothing is playing
        playInProgress = true; // lock the session
        Serial.print("\n motionDetected"); 
        digitalWrite(ledPin, HIGH);   // +5v to MOSFET gate
        pressPlayButton();
        Serial.print("\n wait clip length   ");      
        delay(realLength);   
    
        Serial.print("\n stop    "); 
        pressPlayButton();   
        
        digitalWrite(ledPin, LOW);   // +5v to MOSFET gate
        Serial.print("\n wait delay   "); 
        delay(delayBetweenClips);  // add a buffer so the trap wont fire too often
    
        playInProgress = false; // allow for new 
      } else {
          //Serial.println("no motion or one in progress"); 
      }  
      
    } 

}
