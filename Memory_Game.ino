#include <Vector.h> //libary for dynamic lists (essentially arraylists)

//setting max value for lists
const int MAX_ELEMENTS = 100; 
int storageArray_ran[MAX_ELEMENTS]; 
int storageArray_guess[MAX_ELEMENTS];

//initialize variables
int store_num;
int round_n;
int lives;
bool level_pass = true;
bool restart = true;
bool most_recent_cor = true;
double delay_time;
double mul;

//initialize lists
Vector<int> dynamicList_ran;
Vector<int> dynamicList_guess;

void setup(){
  Serial.begin(9600); //initialize serial console
  delay(1000);

  randomSeed(analogRead(A1)); //set random see

  //confirming max value for lists
  dynamicList_ran.setStorage(storageArray_ran);
  dynamicList_guess.setStorage(storageArray_guess);

  //inputs for buttons
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);

  pinMode(10, INPUT_PULLUP); //speed set

  //outputs for LEDs
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);



  //starting pattern
  for(int i = 6; i < 10; i++){
    digitalWrite(i, HIGH);
    delay(100);
    digitalWrite(i, LOW);
  }
  for(int i = 9; i >= 6; i--){
    digitalWrite(i, HIGH);
    delay(100);
    digitalWrite(i, LOW);
  }

  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);

  if(digitalRead(10) == 0){ //standard speed
    Serial.println("Starting Memory Game!");
    Serial.println("This is standard speed. The game will not get faster.");
    delay_time = 1100;
    mul = 1;
  }
  else{ //increase speed
    Serial.println("Starting Memory Game!");
    Serial.println("This is speed mode. The game will get faster after each correct answer.");
    delay_time = 1100;
    mul = 1.1;
  }

  lives = 3;
}

void wait_restart(){ //restart game function
  Serial.println("Click any button to restart.");

  while(!restart){
    if(digitalRead(2) == 0){
      delay(20);
      while(digitalRead(2) == 0){} 
      delay(250);
      restart = true;
    }
    if(digitalRead(3) == 0){
        delay(20);
        while(digitalRead(3) == 0){} 
        delay(250);
        restart = true;
    }
    if(digitalRead(4) == 0){
        delay(20);
        while(digitalRead(4) == 0){} 
        delay(250);
        restart = true;
    }
    if(digitalRead(5) == 0){
        delay(20);
        while(digitalRead(5) == 0){} 
        delay(250);
        restart = true;
    }
  }

  dynamicList_guess.clear();
  dynamicList_ran.clear();

  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);

  lives = 3;
  most_recent_cor = true;

  if(digitalRead(10) == 0){ //standard speed
    Serial.println("This is standard speed. The game will not get faster.");
    delay_time = 1100;
    mul = 1;
  }
  else{ //increase speed
    Serial.println("This is speed mode. The game will get faster after each correct answer.");
    delay_time = 1100;
    mul = 1.1;
  }

  Serial.println("Resarting Game!");

  for(int i = 6; i < 10; i++){
    digitalWrite(i, HIGH);
    delay(100);
    digitalWrite(i, LOW);
  }
  for(int i = 9; i >= 6; i--){
    digitalWrite(i, HIGH);
    delay(100);
    digitalWrite(i, LOW);
  }
}

void loop(){
  long randomNumber = random(6, 10); //create the random number 6-9
  delay(1000);

  if(most_recent_cor){ //add to list if user got correct answer
    dynamicList_ran.push_back(randomNumber);
  }
  round_n = dynamicList_ran.size();
  Serial.print("Round ");
  Serial.println(round_n);

  //light LEDs in seqeunce
  for(int i = 0; i < dynamicList_ran.size(); i++){
    digitalWrite(dynamicList_ran[i], HIGH);
    delay((unsigned int)delay_time);
    digitalWrite(dynamicList_ran[i], LOW);
    delay(200);
    if(most_recent_cor){
      delay_time /= mul; //this is where speed mode comes in
    }
  }
  
  //game logic: wait for a button press, check if that press was right or wrong
  while(dynamicList_guess.size() < dynamicList_ran.size() && level_pass){
    int pressed_pin = -1;

    if(digitalRead(2) == 0){
      delay(20); 
      while(digitalRead(2) == 0){} 
      delay(250);
      store_num = 6;
      Serial.println("Your Guess: RED");
      pressed_pin = store_num;
    }
    else if(digitalRead(3) == 0){
      delay(20); 
      while(digitalRead(3) == 0){} 
      delay(250);
      store_num = 7;
      Serial.println("Your Guess: GREEN");
      pressed_pin = store_num;
    }
    else if(digitalRead(4) == 0){
      delay(20); 
      while(digitalRead(4) == 0){} 
      delay(250);
      store_num = 8;
      Serial.println("Your Guess: YELLOW");
      pressed_pin = store_num;
    }
    else if(digitalRead(5) == 0){
      delay(20); 
      while(digitalRead(5) == 0){} 
      delay(250);
      store_num = 9;
      Serial.println("Your Guess: ORANGE");
      pressed_pin = store_num;
    }

    //when a pin is pressed, check that number with what the number should be
    if(pressed_pin != -1){
      dynamicList_guess.push_back(pressed_pin);
      int currentIndex = dynamicList_guess.size() - 1;
      
      if(dynamicList_guess[currentIndex] != dynamicList_ran[currentIndex]){
        level_pass = false; 
        break;
      }
    }
  }

  if(!level_pass){ //incorrect
    lives--;
    level_pass = true;
    most_recent_cor = false;
    Serial.println("Incorrect guess. You have ");
    Serial.print(lives);
    Serial.print(" lives");
    Serial.println("");
  }
  else{ //correct
    Serial.println("Correct!");
    most_recent_cor = true;
  }

  if(lives == 0){ //gameover if lives runout, call the restart function
    dynamicList_ran.clear();
    delay(2500);
    Serial.println("Game Over!");
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(9, HIGH);
    restart = false;
    wait_restart();
  }

  dynamicList_guess.clear(); //clear user inputs for new round
  delay(50); //delay to help program run
}