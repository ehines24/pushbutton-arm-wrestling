#include <Servo.h>

// constants won't change. They're used here to set pin numbers:
const int player1 = 2;  // pin of player 2
const int player2 = 3;  // pin of player 1
const int LEDButtonPin = 7;  // pin of the led button
const int LEDgreen = 12;    // pin of the green LED
const int LEDred =10; //pin of the red LED
const int buzzerPin = 13; //buzzer pin 
int buttonStateLED = 0;  // variable to store whether or not the LED button is pressed
Servo wrestling_arm;  // create servo object to control a servo
int angle = 90; // inital angle of the Servo. Tracks the game state. takes [0,180]. If it hits either value the game ends.
int player1_increment; // The init value of increment for pressing the button (player1) //see setup
int player2_decrement; // The init value of decrement for pressing the button (player2) // see setp
int dont_penalize = 1; //bool to be checked before applying penalty. If 0, apply penalty. 



/* These are the button handlers. When a player 1 or 2 button is pressed do the following:
  (1) Penalize the player if the LED isn't illuminated (buff the other player's button pushing power)
  (2) Reward the player for pressing the button by changing the Servo angle
*/
void button_handler_p1() {
  //Serial.println("Handling button press p1");
  if (dont_penalize == 1){
    //Serial.println("dont_penalize == 1");
    angle+=player1_increment;
  }
  else if (dont_penalize == 0){
    //Serial.println("dont_penalize == 0");
    player2_decrement -= 4;    
  }
   
  
  //Serial.print(angle);
  //Serial.print("\n");
}

void button_handler_p2() {
  //Serial.println("Handling button press p2");
  if (dont_penalize == 1){
    //Serial.println("dont_penalize == 1");
    angle+=player2_decrement;
  }
  else if (dont_penalize == 0){
    //Serial.println("dont_penalize == 0");
    player1_increment += 4;
  } 
  
  //Serial.print(angle);
  //Serial.print("\n");
}


void player_victory(String player_number){
  Serial.print("Player " + player_number + " wins!\n");
  //maybe add a siren or some kind of visual alert that indicates a victory.
  digitalWrite(LEDgreen, 0);
  digitalWrite(LEDred, HIGH);//to indicate visually that the game has ended
  tone(buzzerPin, 440); // A4
  delay(1000);
  noTone(buzzerPin);
  delay(4000);
  digitalWrite(LEDred, LOW);//reset red light
  setup();
}

void setup() {
  Serial.print("Set to starting game position\n");
  angle = 90; //starting angle in the middle. Equal game state for both players.
  player1_increment = 2;
  player2_decrement = -2.75;
  // initialize the LED pin as an output:
  pinMode(LEDgreen, OUTPUT);
  // initialize the LED pin as an output:
  pinMode(LEDred, OUTPUT);
  //initialize buzzer pin as an output. 
  pinMode(buzzerPin, OUTPUT);
  // initialize the players buttons an inputs:
  pinMode(player1, INPUT_PULLUP);
  pinMode(player2, INPUT_PULLUP);
  pinMode(LEDButtonPin, INPUT_PULLUP);
  // attach the interupts and trigger them when the button is released
  attachInterrupt(digitalPinToInterrupt(player1), button_handler_p1, FALLING);
  attachInterrupt(digitalPinToInterrupt(player2), button_handler_p2, FALLING);
  // attach the Servo and set it to 90 degrees (the middle)(I.E starting position for the game)
  wrestling_arm.attach(9);
  wrestling_arm.write(angle);
  delay(10);
  Serial.begin(9600); //useful to print to the serial monitor
}

void loop() {
  // put your main code here, to run repeatedly:
  buttonStateLED = digitalRead(LEDButtonPin); // Check the LED button to see if it is pressed down

  if(buttonStateLED == HIGH) {
    dont_penalize = 1;
    digitalWrite(LEDgreen, 1);
  } else {
    dont_penalize = 0;
    digitalWrite(LEDgreen, 0);
  }

  //check victory condition
  if(angle <= 0){
    player_victory("2");
  }
  else if (angle >= 180){
    player_victory("1");    
  }

  wrestling_arm.write(angle);

}
