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
int delayBetweenClips = 20000;
int buttonPressLength = 100;


// variables will change: buttonPressLength
bool playInProgress = true;  
bool firstRun = true;  
int motionDetected = 0;
int realLength = clipLength - buttonPressLength;
int firstRunLength = clipLength - (buttonPressLength*2);


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
  
  Serial.print("\n pressPlayButton"); 
  digitalWrite(backPin, LOW);    // 0v to MOSFET gate 
} 

void setup() {
    // Initialize serial port for debugging.
  Serial.begin(115200);      

  
  playInProgress = true;        
                           
  // Soft startup to ease the flow of electrons.
  delay(1000);      
      Serial.print(realLength); 

  Serial.print("\n ss");       
      
  Serial.print("\n ss");       
      Serial.print(firstRunLength);    
  // initialize the LED pin as an output:
  pinMode(backPin, OUTPUT);
  pinMode(playPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  delay(4000);      
 // Try to sync up with the audio module
  pressBackButton();
  delay(400);     
  pressBackButton();
  delay(400);     
  pressBackButton();
  delay(100);     
  pressPlayButton();
  Serial.print("\n boot stop"); 
  delay(3000);       
  playInProgress = false;   
}

void loop() {
  motionDetected = digitalRead(motionPin);  // read input value
    //Serial.print(motionDetected);    
  if (motionDetected == HIGH && playInProgress == false) { // check if motion detected and nothing is playing
    Serial.print("\n motionDetected"); 
    Serial.print(motionDetected);      
    digitalWrite(ledPin, HIGH);   // +5v to MOSFET gate
    playInProgress = true; // lock the session
    
    Serial.print("\n play  ");    
    pressPlayButton();
    Serial.print("\n wait clip length   ");    
    if (firstRun == true) {
      firstRun = false;
      delay(firstRunLength);   
    } else {
       delay(realLength);   
    }
    digitalWrite(ledPin, LOW);   // +5v to MOSFET gate
    Serial.print("\n stop    "); 
    pressPlayButton();   
    Serial.print("\n wait delay   "); 
    delay(delayBetweenClips);  // add a buffer so the trap wont fire too often

    playInProgress = false; // allow for new 
  } else {
      //Serial.println("no motion or one in progress"); 
  }
  // read the state of the pushbutton value:
 // buttonState = digitalRead(buttonPin);

}
