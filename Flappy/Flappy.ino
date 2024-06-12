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

CRGB leds[NUM_LEDS];
volatile int counter = 24; // Anfangsposition des Vogels
volatile bool reverse = false;
volatile bool isGameOver = true;
volatile int score;
volatile int timer2_interval = 1000;
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

  if(isGameOver == false && millis() - previousMillis > timer2_interval){
    previousMillis = millis();
    shift();
    // world.shift();
  }

  world.print(leds);

  // Game over wenn der Vogel die Balken berührt
  if(leds[counter] != CRGB::Black){
    lcd.setCursor(0,1);
    lcd.print("GAME OVER!");
    isGameOver = true;    
  }
  leds[counter] = CRGB::Brown;
  FastLED.show();
  delay(10);
}

void reset(){
  if(isGameOver){
    counter = 24;
    world.reset();
    isGameOver = false;
    score = 0;
  }
}

void flyUp(){
  reverse = (digitalRead(IN_PIN) == HIGH);
}

void flap(){
  if(isGameOver){
    return;
  }
  leds[counter]=CRGB::Black;

  // Vogelposition erhöht bzw. verkleinert sich
  if(reverse){
    counter--;
  } else{
    counter++;
  }
  
  // Vogel bleibt in der zweiten Spalte
  if(counter < 16){
    counter = 16;
  } else if(counter >= 31){
    counter = 31;
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
  Serial.println("Shifting");
  if(!isGameOver){
    world.shift();
  }

}




