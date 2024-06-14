# mc-beleg-flappy-bird

## Überblick

Wir haben im Rahmen des Moduls "Programmierung von Microcontrollern" (I-223) bei Prof. Dr.-Ing. Jörg Vogt ein Programm für den Microcontoller "Arduino Uno Rev3" geschrieben, mit dem man eine abgewandelte Version des Spiels "Flappy Bird" von Dong Nguyen spielen kann.

_Autoren:_ Lotte Richter (lotte.richter@stud.htw-dresden.de) & Niklas Hempel (niklas.hempel@stud.htw-dresden.de)
HTW Dresden

## Spielablauf

Das Spiel beginnt, wenn man Button 2 drückt. Dann fängt der Vogel in der Mitte seiner Spalte an zu fliegen und es erscheinen langsam die Balken, durch die man den Vogel steuern muss.

Man kann den Vogel steuern, indem man den Button 1 drückt bzw. gedrückt hält. Wenn man den Button 1 drückt, fliegt der Vogel linear nach oben und wenn der Button 1 nicht gedrückt ist, fliegt der Vogel linear nach unten (Schwerkraft).

Nun besteht die Aufgabe für den Spielenden dabei, den Vogel durch die Lücken in den Balken zu steuern. Auf dem LCD-Display des Microcontrollers wird immer parallel der Spielstand(Score) angezeigt. Jedemal wenn man den Vogel erfolgreich durch einen Balken steuert erhöht sich der Score um Eins.

Mit fortschreitender Zeit des Spieles wird die Geschwindigkeit mit der sich die Balken auf den Vogel zu bewegen schneller und damit wird es schwieriger den Vogel durch die Lücken zu manövrieren.

Das Spiel ist vorbei, wenn der Vogel gegen eine Balken fliegt. Dann wird die Animation auf dem LED-Feld gestoppt und auf dem LCD-Display wird der Score angezeigt.

Wenn man Button 2 drückt, wird das Spiel erneut gestartet.

## Installation

Um das Programm auf den Arduino zu übertragen, muss der Arduino an den Computer angeschlossen werden und die Arduino IDE gestartet werden.

Dann muss der Sketch `Flappy.ino` in der Arduino IDE geöffnet werden. In der Arduino IDE muss unter `Werkzeuge -> Board` der richtige Board-Typ ausgewählt werden. In unserem Fall ist das `Arduino Uno`.

Anschließend muss unter `Werkzeuge -> Port` der richtige Port ausgewählt werden, an dem der Arduino angeschlossen ist.

Zum Übertragen des Sketches auf den Arduino muss der Upload-Button in der Arduino IDE gedrückt werden.

## Hardware

- Arduino UNO Rev.3 mit DIP-Fassung
- 16\*2 LCD HD44780
- 16x16 Neopixel Matrix WS2812B
- 3 x Taster

In folgendem Bild wird der Aufbau unseres Microcontrollers dargestellt. Wir nutzen das 16x16 Matrix-LED-Fels WS2812B um das Spiel anzuzeigen, also der Vogel und die Balken durch die er durch muss werden darauf abgebildet. Das LCD-Display nutzen wir, um den jeweiligen Spielstand anzuzeigen und von den Tastern nutzen wir nur die zwei äußersten Buttons (Button 1 & Button 2).

## Programmierung

Zur Entwicklung des Programms wurde die [Arduino IDE](https://www.arduino.cc/en/software) mit der Programmiersprache C++ verwendet.
Zusätzlich zu den Standardbibliotheken von Arduino wurden folgende Bibliotheken verwendet:

- [Bounce2](https://github.com/thomasfredericks/Bounce2): Entprellen der Taster
- [LiquidCrystal](https://github.com/arduino-libraries/LiquidCrystal): Ansteuerung des LCD-Displays
- [FastLED](https://github.com/FastLED/FastLED): Ansteuerung der LED-Matrix
- [TimerOne](https://github.com/PaulStoffregen/TimerOne): Timer

Neben diesen externen Bibliotheken haben wir auch eigene Klassen und Funktionen geschrieben, um das Spiel zu realisieren. Die wichtigsten Klassen und Funktionen sind:

- `Bird`: Klasse, die den Vogel repräsentiert. Sie enthält die Position des Vogels und Methoden, um den Vogel zu bewegen.
- `World`: Klasse, die die Welt des Spiels repräsentiert, d.h. die Balken und die Lücken zwischen den Balken. Sie enthält Methoden, um die Balken zu bewegen.

Beide Klassen haben eine Methode `print(CRGB *leds)`, die die Objekte auf der LED-Matrix zeichnet.

Vereint werden die Klassen im Hauptprogramm `Flappy.ino`. Üblich für Arduino-Projekte sind die Funktionen `setup()` und `loop()`, in denen die Initialisierung und die Hauptschleife des Programms stattfinden.

Nachfolgend wird der Aufbau des Programms in den einzelnen Dateien beschrieben:

### Welt (world.hpp, world.cpp)

Die Welt, in der sich der Vogel bewegt, wird über die Klasse `World` realisiert. Sie enthält die Balken, die sich auf den Vogel zubewegen, und die Lücken zwischen den Balken, durch die der Vogel fliegen muss. Die Klasse `World` hat folgende Attribute:

| Attribut | Typ     | Beschreibung                                                                      |
| -------- | ------- | --------------------------------------------------------------------------------- |
| distance | int     | Abstand zwischen zwei aufeinanderfolgenden Balken                                 |
| gap      | int     | Größe der Lücke innerhalb eines Balkens                                           |
| state    | int     | Wird dazu genutzt, aller `distance` (s.o.) Ticks einen neuen Balken zu generieren |
| world    | int[][] | Matrix, die die Position der Balken speichert                                     |

Neben den Attributen enthält die Klasse `World` folgende Methoden:

| Methode                       | Beschreibung                                                             |
| ----------------------------- | ------------------------------------------------------------------------ |
| World(int gap, int distance)  | Konstruktor, initialisiert die Attribute der Klasse und die Matrix mit 0 |
| void shift()                  | Verschiebt die Balken um eine Position nach links                        |
| void print(CRGB \*leds) const | Überträgt die interne Weltmatrix auf die LED-Matrix                      |
| void reset()                  | Setzt die Welt zurück, d.h. löscht alle Balken und setzt state zurück    |

Nachfolgend möchten wir die Methode `void print(CRGB *leds) const` genauer erläutern.

```cpp
void World::print(CRGB *leds) const {
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 16; j++) {
      int index;
      if (i % 2 == 0) { // even row runs forward
        index = j + (i * 16);
      } else {
        // odd row runs backward
        index = (i * 16) + 15 - j;
      }
      if (this->world[i][j] == 1) {
        leds[index] = CRGB::Green;
      } else {
        leds[index] = CRGB::Black;
      }
    }
  }
}
```

Die Methode iteriert erwartungsgemäß über die Weltmatrix und setzt die entsprechenden LEDs auf der LED-Matrix auf grün, wenn sich an der entsprechenden Position ein Balken befindet, und auf schwarz, wenn sich an der entsprechenden Position kein Balken befindet. Zu beachten ist jedoch, dass am Ende einer Zeile nicht zum Beginn der nächsten Zeile gesprungen wird, sondern die Balken in der nächsten Zeile in umgekehrter Reihenfolge gezeichnet werden. Dies ist notwendig, da die LED-Matrix in einer Zick-Zack-Form angesprochen wird. Dies nennt man auch "serpentinenförmige Ansteuerung" oder "Bustrophedon".

```cpp
//     0 >  1 >  2 >  3 >  4 > 5 > 6 > 7 > 8
//                                         |
//                                         |
//      16 < 15 < 14 < 13 < 12 < 11 < 10 < 9
//     |
//     |
//     17 > 18 > 19 > 20 > 21 > 22 > 23 > 24
//                                         |
//                                        usw.
```

### Vogel (bird.hpp, bird.cpp)

Der Vogel wird über die Klasse `Bird` realisiert und hat folgende Attribute:

| Attribut          | Typ  | Beschreibung                                                                 |
| ----------------- | ---- | ---------------------------------------------------------------------------- |
| position          | int  | Position des Vogels innerhalb einer Spalte der Matrix                        |
| min_position      | int  | Minimale Position des Vogels                                                 |
| max_position      | int  | Maximale Position des Vogels                                                 |
| starting_position | int  | Startposition des Vogels                                                     |
| do_fly_up         | bool | Gibt an, ob der Vogel fliegt (oder fällt)                                    |
| prevent_falling   | bool | Gibt an, ob der Vogel nicht fallen soll obwohl er es laut `do_fly_up` sollte |

Zusätzlich dazu besitzt die Klasse folgende Methoden:

| Methode                                                         | Beschreibung                                           |
| --------------------------------------------------------------- | ------------------------------------------------------ |
| Bird(int min_position, int max_position, int starting_position) | Konstruktor, initialisiert die Attribute der Klasse    |
| void flyUp(bool button_pressed)                                 | Setzt `do_fly_up` auf `button_pressed`                 |
| void flap()                                                     | Bewegt den Vogel um eine Position nach oben oder unten |
| void print(CRGB \*leds) const                                   | Überträgt die Position des Vogels auf die LED-Matrix   |
| void hide(CRGB \*leds) const                                    | Löscht die Position des Vogels von der LED-Matrix      |
| int getPosition() const                                         | Gibt die aktuelle Position des Vogels zurück           |

Exemplarisch möchten wir die Methode `void flyUp(bool button_pressed)` genauer erläutern.

```cpp
void Bird::flyUp(bool button_pressed) {
  // Bird stops raising when button is released but should not immediately
  // fall down
  bool old_do_fly_up = do_fly_up;
  this->do_fly_up = button_pressed;
  if (!old_do_fly_up && this->do_fly_up) {
    this->prevent_falling = true;
  }
}
```

Das Hauptprogramm übergibt den aktuellen Zustand des Buttons an die Methode `flyUp`. Wenn der Button gedrückt ist, wird `do_fly_up` auf `true` gesetzt, andernfalls auf `false`. Wenn der Button losgelassen wird, d.h. der Vogel wird aufhören zu steigen und soll wieder fallen, wird `prevent_falling` auf `true` gesetzt, damit der Vogel nicht sofort fällt, wenn der Button losgelassen wird. Dies verbessert das Spielerlebnis.

### Hauptprogramm (Flappy.ino)

Das Hauptprogramm `Flappy.ino` enthält die Initialisierung des Spiels und die Hauptschleife.

#### Initialisierung

Am Anfang des Programms definieren wir diverse Konstanten, die für das Spiel relevant sind, z.B. die Größe der Lücke in den Balken, den Abstand zwischen den Balken etc.

```cpp
#define LED_PIN 11
#define START_PIN 2 // Reset Button
#define IN_PIN 3    // Vogel Button
#define NUM_LEDS 256
#define BRIGHTNESS 50
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define FLAP_TIMER_INTERVAL 150000 // Timer: 0.15s
#define MIN_BIRD_POSITION 16
#define MAX_BIRD_POSITION 31
#define STARTING_BIRD_POSITION 24
#define DEFAULT_WORLD_SPEED 1000 // Timer: 1s
#define WORLD_GAP 5
#define WORLD_DISTANCE 4
```

Danach legen wir die relevanten Objekte an, z.B. die LED-Matrix, das LCD-Display, den Vogel und die Welt.

```cpp
CRGB leds[NUM_LEDS];
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
Bounce2::Button button = Bounce2::Button();
World world(WORLD_GAP, WORLD_DISTANCE);
Bird bird(MIN_BIRD_POSITION, MAX_BIRD_POSITION, STARTING_BIRD_POSITION);
```

und zu guter Letzt noch die restlichen Variablen, die für das Spiel relevant sind.

```cpp
unsigned long previousMillis = 0;
volatile int score = 0;
volatile int world_timer_interval = DEFAULT_WORLD_SPEED;
volatile bool is_game_over = true;
```

Nach dieser Anfangskonfiguration folgt die `setup()`-Funktion, in der zuerst die benötigten Pins für die Buttons, die LED-Matrix und das LCD-Display initialisiert werden. Danach wird ein Timer angelegt, der den Takt des Vogels setzt und die Funktion `flyUp` aufruft.
Des Weiteren gab es ursprünglich einen zweiten Timer aus der Bibliothek [arduino-timer](https://github.com/contrem/arduino-timer), der den Takt der Welt setzt. Dieser Timer wurde jedoch entfernt, da er oftmals grundlos stoppte. Als Alternative verwenden wir nun die Funktion `millis()` und fragen in jedem Schleifendurchlauf ab, ob genug Zeit vergangen ist, um die Welt eine Spalte weiter zu schieben. Die Buttons werden nicht in der Hauptschleife abgefragt, sondern es wird mittels Interrupts auf Änderungen reagiert.

```cpp
void setup(){
  /*
  * Skipped for brevity:
  *   Initialisierung der Pins
  *   Initialisierung der LED-Matrix
  *   Initialisierung des LCD-Displays
  *   Initialisierung des Timers
  *   Initialisierung für entprellten Button
  */
  // Setup Interrupt für Button
  attachInterrupt(digitalPinToInterrupt(IN_PIN), flyUp,CHANGE); // Vogelsteuerung
  attachInterrupt(digitalPinToInterrupt(START_PIN), reset, FALLING); // Reset
}
```

#### Hauptschleife

```cpp
void loop() {
  if (is_game_over) {
    return;
  }

  // Button entprellen
  button.update();

  lcd.setCursor(0, 0);
  lcd.clear();
  lcd.print("SCORE: ");
  lcd.print(score);

  if (millis() - previousMillis > world_timer_interval) {
    previousMillis = millis();
    shift_world();
  }

  world.print(leds);
  bird.print(leds);

  // Game over wenn der Vogel die Balken berührt
  if (leds[bird.getPosition()] != CRGB::Black) {
    lcd.setCursor(0, 1);
    lcd.print("GAME OVER!");
    is_game_over = true;
  }

  FastLED.show();
  delay(10);
}
```

In der Hauptschleife findet der zuvor beschrieben Timer seinen Einsatz. Gleichzeitig mit dem Versetzen der Welt wird mit jedem vierten Balken, der erfolgreich überwunden wurde, die Geschwindigkeit der Spielwelt erhöht. Anschließend werden die Welt und der Vogel mit den beschriebenen `print`-Funktionen und `FastLED.show()` auf der LED-Matrix gezeichnet.

```cpp
void shift_world() {
  if (is_game_over) {
    return;
  }

  if (leds[0] != CRGB::Black || leds[15] != CRGB::Black) {
    score++;

    // Geschwindigkeitserhöhung der Balken
    if ((score % 4) == 0 && score < 9 * 4) {
      world_timer_interval -= 100;
    }
  }

  world.shift();
}
```

## Ausblick

Folgende Aspekte des Projekts kann man weiterentwickeln/verbessern:

- 16x16 LED-Matrix auf mehr LEDs erweitern
  - Die 16x16 LEDs sind vergleichsweise fürs menschliche Auge ziemlich wenig und man kann keine schönen Flugkurven darstellen, stattdessen ist das Spiel sehr stockend. Man könnte ein LED-Feld nutzen, welches mehr LEDs hat, wodurch die Animation etwas angenehmer wird. Dazu müssten u.a. derzeit noch feste Werte für die Höhe und Breite der Matrix z.B. per Konstruktorparameter übergeben werden.
- Ergonomischerer Button
  - Wenn man das Spiel eine Weile spielt, wird der Buttonklick mit der Zeit relativ unangenehm, da man Kraft aufwenden muss, um den Button ordentlich zu drücken. Wenn man eine Button nutzen würde, der einfacher zu drücken ist, würde das das Spielerlebnis des Spielenden um einiges verbessern.
- Schwierigkeit mit der Zeit erhöhen
  - Derzeit wird das Spiel mit der Zeit schwieriger zu spielen, da die Geschwindigkeit sich erhöht, mit der die Balken auf den Vogel zu kommen. Es gibt noch weitere Funktionen, die man implementieren könnte, damit das Spiel mit der Zeit an Schwierigkeit zunimmt:
    - Lücke in den Balken verringern: Man könnte die Lücke in den Balken mit fortlaufender Spielzeit verringern, damit es anspruchsvoller wird, das Spiel zu spielen.
    - Abstand zwischen den Balken verringern: Man könnte den Abstand zwischen den Balken mit fortlaufender Spielzeit verringern.
    - Abstand zwischen den Lücken aufeinanderfolgender Balken vergrößern: Je größer der Abstand zwischen den Lücken zweier Balken ist, desto schwieriger wird es sie rechtzeitig zu überwinden.

```

```
