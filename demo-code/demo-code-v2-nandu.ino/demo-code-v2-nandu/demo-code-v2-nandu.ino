#include <Servo.h>
#include <stdio.h>

// try to use mutexes to solve the problem, but no mutex library because arduino weak as shit and we cannot have nice things in life.


// constants won't change. They're used here to set pin numbers:
const int player1 = 2;           // pin of player 2
const int player2 = 3;           // pin of player 1
const int LEDButtonPin = 7;      // pin of the led button
const int LEDgreen = 12;         // pin of the green LED
const int LEDred = 10;           //pin of the red LED
const int buzzerPin = 13;        //buzzer pin
int buttonStateLED = 0;          // variable to store whether or not the LED button is pressed
int player1State = 0;            // current state of the player buttons on/off
int player2State = 0;
Servo wrestling_arm;             // create servo object to control a servo
int angle = 90;                  // inital angle of the Servo. Tracks the game state. takes [0,180]. If it hits either value the game ends.
int player1_increment;           // The init value of increment for pressing the button (player1) //see setup
int player2_decrement;           // The init value of decrement for pressing the button (player2) // see setp
volatile int dont_penalize = 1;  //bool to be checked before applying penalty. If 0, apply penalty.
int penalty = 4; // Penalty incurred if you press the button when the light is off
int lastplayer1State = 0;       // last state of the player buttons on/off to prevent 
int lastplayer2State = 0;


/*void button_handler_p1() {
  //Serial.println("Handling button press p1");
  if (dont_penalize == 1) {
    //Serial.println("dont_penalize == 1");
    angle += player1_increment;

  } else if (dont_penalize == 0) {
    //Serial.println("dont_penalize == 0");
    player2_decrement -= penalty;
  }


  Serial.print("angle: ");
  Serial.print(angle);
  Serial.print(" strength: ");
  Serial.println(player1_increment);
}

void button_handler_p2() {
  //Serial.println("Handling button press p2");
  if (dont_penalize == 1) {
    //Serial.println("dont_penalize == 1");
    angle += player2_decrement;
  } else if (dont_penalize == 0) {
    //Serial.println("dont_penalize == 0");
    player1_increment += penalty;
  }

  Serial.print("angle: ");
  Serial.print(angle);
  Serial.print(" strength: ");
  Serial.println(player2_decrement);
}*/


// Play a sequence of n frequencies from a frequency array with delay_time ms in between
void musical(int freq_array[], int delay_time, int n) {
  for (int i = 0; i < n; i++) {
    tone(buzzerPin, freq_array[i]);
    delay(delay_time);
  }
  noTone(buzzerPin);
}

void player_victory(int player_number) {
  Serial.print("Player " + String(player_number) + " wins!\n");
  //maybe add a siren or some kind of visual alert that indicates a victory.
  digitalWrite(LEDgreen, 0);
  digitalWrite(LEDred, HIGH);  //to indicate visually that the game has ended
  if (player_number == 1) {
    int freq_array[5] = { 392, 440, 523.3, 587.3, 659.3 };
    musical(freq_array, 500, 5);
  } else {
    int freq_array[8] = { 392, 440, 523.3, 587.3, 659.3, 587.3, 523.3, 587.3 };
    musical(freq_array, 500, 8);
  }
  delay(4000);
  digitalWrite(LEDred, LOW);  //reset red light
  setup();
}

void setup() {
  Serial.print("Set to starting game position\n");
  angle = 90;  //starting angle in the middle. Equal game state for both players.
  player1_increment = 2;
  player2_decrement = -2;
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
  Serial.begin(9600);  //useful to print to the serial monitor
}

void loop() {
  // put your main code here, to run repeatedly:
  player1State = digitalRead(player1);
  player2State = digitalRead(player2);
  buttonStateLED = digitalRead(LEDButtonPin);  // Check the LED button to see if it is pressed down
  if (buttonStateLED == HIGH) {
    dont_penalize = 1;
    digitalWrite(LEDgreen, 1);
  } else {
    dont_penalize = 0;
    di
    digitalWrite(LEDgreen, 0);
  }
  /* The two game buttons are polled and the following happens:
  (1) Penalize the player if the LED isn't illuminated (buff the other player's button pushing power)
  (2) Reward the player for pressing the button by changing the Servo angle
*/
  if(player1State || !lastplayer1State) {
    if (dont_penalize == 1) {
      //Serial.println("dont_penalize == 1");
       angle += player1_increment;
       lastplayer1State = 1;
    } else if (dont_penalize == 0) {
      //Serial.println("dont_penalize == 0");
      player2_decrement -= penalty;
    }
    Serial.print("angle: ");
    Serial.print(angle);
    Serial.print(" strength: ");
    Serial.println(player1_increment); 
  } else {
    lastplayer1State = 0;
  }
  
  if(player2State) {
    if (dont_penalize == 1) {
      //Serial.println("dont_penalize == 1");
      lastplayer2State = 1;
      angle += player2_decrement;
    } else if (dont_penalize == 0) {
      //Serial.println("dont_penalize == 0");
      player1_increment += penalty;
    }    
    Serial.print("angle: ");
    Serial.print(angle);
    Serial.print(" strength: ");
    Serial.println(player2_decrement); 
  } else {
    lastplayer2State = 0;
  }
 


  
  //check victory condition
  if (angle <= 0) {
    player_victory(2);
  } else if (angle >= 180) {
    player_victory(1);
  }


  wrestling_arm.write(angle);
}