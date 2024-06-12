#include <TimerOne.h>
#include <FastLED.h>
#include <LiquidCrystal.h>
#include <Bounce2.h>

#include "world.hpp"

#define LED_PIN 11
#define START_PIN 2 // Reset Button
#define IN_PIN 3 // Vogel
#define NUM_LEDS 256
#define BRIGHTNESS 50
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define TIMER 150000 // Timer: 0.15s
#define MIN_BIRD_POSITION 16
#define MAX_BIRD_POSITION 31
#define STARTING_BIRD_POSITION 24
#define DEFAULT_WORLD_SPEED 1000

CRGB leds[NUM_LEDS];
volatile int counter = 24; // Anfangsposition des Vogels
volatile bool do_fly_up = false;
volatile bool is_game_over = true;
volatile int score;
volatile int timer2_interval = DEFAULT_WORLD_SPEED;
volatile bool prevent_falling = false;
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

Bounce2::Button button = Bounce2::Button();

World world(5, 4);

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(IN_PIN, INPUT_PULLUP); // set digital pin 3 as input
  pinMode(START_PIN, INPUT_PULLUP);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  Timer1.initialize(TIMER);
  Timer1.attachInterrupt(flap);

  // Setup Button
  button.attach(IN_PIN, INPUT_PULLUP);
  button.interval(100); // Entprellzeit
  button.setPressedState(LOW);

  // Setup Interrupt für Button
  attachInterrupt(digitalPinToInterrupt(IN_PIN), flyUp,CHANGE); // Vogelsteuerung
  attachInterrupt(digitalPinToInterrupt(START_PIN), reset, FALLING); // Reset
  Serial.begin(9600);
  lcd.begin(16,2);
}

void loop() {
  static unsigned long previousMillis;

  // Button entprellen
  button.update();

  lcd.setCursor(0,0);
  lcd.clear();
  lcd.print("SCORE: ");
  lcd.print(score);

  if(is_game_over == false && millis() - previousMillis > timer2_interval){
    previousMillis = millis();
    shift();
  }

  world.print(leds);

  // Game over wenn der Vogel die Balken berührt
  if(leds[counter] != CRGB::Black){
    lcd.setCursor(0,1);
    lcd.print("GAME OVER!");
    is_game_over = true;    
  }
  leds[counter] = CRGB::Brown;
  FastLED.show();
  delay(10);
}

void reset(){
  if(is_game_over){
    world.reset();
    counter = STARTING_BIRD_POSITION;
    timer2_interval = DEFAULT_WORLD_SPEED;
    is_game_over = false;
    score = 0;
  }
}

void flyUp(){
  bool old_do_fly_up = do_fly_up;
  do_fly_up = (digitalRead(IN_PIN) == HIGH);
  if((old_do_fly_up == false) && (do_fly_up == true)){
    prevent_falling = true;
  }
}

void flap(){
  if(is_game_over){
    return;
  }
  leds[counter]=CRGB::Black;

  // Vogelposition erhöht bzw. verkleinert sich
  if(prevent_falling == true){
    prevent_falling = false;
  }else{
    if(do_fly_up){
      counter--;
    } else{
      counter++;
    }
  }
  
  // Vogel bleibt in der zweiten Spalte
  if(counter < MIN_BIRD_POSITION){
    counter = MIN_BIRD_POSITION;
  } else if(counter >= MAX_BIRD_POSITION){
    counter = MAX_BIRD_POSITION;
  }
}

void shift(){
  if(leds[0]!=CRGB::Black || leds[15]!=CRGB::Black){
    score++;
    
    // Geschwindigkeitserhöhung der Balken
    if((score%4)==0 && score < 7*4){
      timer2_interval -= 100;
    }
  }
  
  if(!is_game_over){
    world.shift();
  }
}




