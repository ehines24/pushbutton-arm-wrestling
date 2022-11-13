#include <Servo.h>



// constants won't change. They're used here to set pin numbers:
const int player1 = 2;  // pin of player 2
const int player2 = 3;  // pin of player 1
const int LEDButtonPin = 7;  // pin of the led button


const int LEDPin = 12;    // pin of the LED

int buttonStateLED = 0;  // variable to store whether or not the LED button is pressed

Servo wrestling_arm;  // create servo object to control a servo
int angle = 90; // inital angle of the Servo (in the middle)
int player1_increment = 2; // The init value of increment for pressing the button (player1)
int player2_decrement = -2; // The init value of decrement for pressing the button (player2)
int dont_penalize = 1;

/* Check the current angle of the Servo and update it according to what player pressed the button.
  PARAMETERS
    int is_positive: 
      1: player 1 increments the Servo angle towards 180 degrees
      0: player 2 decrements the Servo angle towards 0 degrees
    int angle: 
      pass the value of the global variable angle
  RETURN VALUE 
    an integer indicating the new angle of the Servo, should be set to the global variable angle
*/
int check_game_button_state(int is_positive, int angle) {
  if (angle <= 0 && !is_positive) {
    return 0;
  }
  if (angle >= 180 && is_positive) {
    return 180;
  }
  is_positive ? angle += player1_increment : angle += player2_decrement;
  return angle;
}

/* Penalize the player when the LED is not illuminated by buffing the other player's button pushing power
  INPUT 
    int player
      player 1 or player 2
*/
void penalize(int player) {
  //return;
  player == 1 ? player2_decrement -= 1 : player1_increment += 1;
  Serial.println("Updating: ");
  Serial.println(player2_decrement);
  Serial.println(player1_increment);
}

/* These are the button handlers. When a player 1/2 button is pressed do the following:
  (1) Penalize the player if the LED isn't illuminated (buff the other player's button pushing power)
  (2) Reward the player for pressing the button by changing the Servo angle
*/
void button_handler_p1() {
  //Serial.println("Handling button press p1");
  Serial.println(dont_penalize);
  //if(dont_penalize) {penalize(1);}
  angle = check_game_button_state(1, angle);
  Serial.print(angle);
  Serial.print("\n");
}

void button_handler_p2() {
  //Serial.println("Handling button press p2");
  Serial.println(dont_penalize);
  //if(dont_penalize) {penalize(2);}
  angle = check_game_button_state(0, angle);
  Serial.print(angle);
  Serial.print("\n");
}


void setup() {
  // initialize the LED pin as an output:
  pinMode(LEDPin, OUTPUT);
  // initialize the players buttons an inputs:
  pinMode(player1, INPUT_PULLUP);
  pinMode(player2, INPUT_PULLUP);
  pinMode(LEDButtonPin, INPUT_PULLUP);
  // attach the interupts and trigger them when the button is released
  attachInterrupt(digitalPinToInterrupt(player1), button_handler_p1, FALLING);
  attachInterrupt(digitalPinToInterrupt(player2), button_handler_p2, FALLING);
  // attach the Servo and set it to 90 degrees (the middle)
  wrestling_arm.attach(9);
  wrestling_arm.write(angle);
  delay(2000);
  Serial.begin(9600);
}

void loop() {
  buttonStateLED = digitalRead(LEDButtonPin); // Check the LED button to see if it is pressed down
  // Turn the LED on/off and set the penalize flag to 0/1 (Penalize when LED isn't on)
  if(buttonStateLED == HIGH) {
    dont_penalize = 1;
    digitalWrite(LEDPin, 0);
  } else {
    dont_penalize = 0;
    digitalWrite(LEDPin, 1);
  }
  // Write the new angle variable to the Servo
  //Serial.println(do_penalize);
  wrestling_arm.write(angle);
  delay(10);
}
