# mc-beleg-flappy-bird

## Überblick

Wir haben im Rahmen des Moduls "Programmierung von Microcontrollern" (I-223) bei Prof. Dr.-Ing. Jörg Vogt ein Programm für den Microcontoller "Arduino Uno Rev3" geschrieben, mit dem man eine abgewandelte Version des Spiels "Flappy Bird" von Dong Nguyen spielen kann.

*Autoren:* Lotte Richter (lotte.richter@stud.htw-dresden.de) & Niklas Hempel (niklas.hempel@stud.htw-dresden.de)
HTW Dresden

## Spielablauf

Das Spiel beginnt, wenn man Button 2 drückt. Dann fängt der Vogel in der Mitte seiner Spalte an zu fliegen und es erscheinen langsam die Balken, durch die man den Vogel steuern muss. 

Man kann den Vogel steuern, indem man den Button 1 drückt bzw. gedrückt hält. Wenn man den Button 1 drückt, fliegt der Vogel linear nach oben und wenn der Button 1 nicht gedrückt ist, fliegt der Vogel linear nach unten (Schwerkraft).

Nun besteht die Aufgabe für den Spielenden dabei, den Vogel durch die Lücken in den Balken zu steuern. Auf dem LCD-Display des Microcontrollers wird immer parallel der Spielstand(Score) angezeigt. Jedemal wenn man den Vogel erfolgreich durch einen Balken steuert erhöht sich der Score um Eins.

Mit fortschreitender Zeit des Spieles wird die Geschwindigkeit mit der sich die Balken auf den Vogel zu bewegen schneller und damit wird es schwieriger den Vogel durch die Lücken zu manövrieren.

Das Spiel ist vorbei, wenn der Vogel gegen eine Balken fliegt. Dann wird die Animation auf dem LED-Feld gestoppt und auf dem LCD-Display wird der Score angezeigt.

Wenn man Button 2 drückt, wird das Spiel erneut gestartet.

## Hardware

* Arduino UNO Rev.3 mit DIP-Fassung
* 16*2 LCD HD44780
* 16x16 Neopixel Matrix WS2812B
* 3 x Taster

In folgendem Bild wird der Aufbau unseres Microcontrollers dargestellt. Wir nutzen das 16x16 Matrix-LED-Fels WS2812B um das Spiel anzuzeigen, also der Vogel und die Balken durch die er durch muss werden darauf abgebildet. Das LCD-Display nutzen wir, um den jeweiligen Spielstand anzuzeigen und von den Tastern nutzen wir nur die zwei äußersten Buttons (Button 1 & Button 2).


## Programmierung

## Ausblick
Folgende Aspekte des Projekts kann man weiterentwickeln/verbessern:
* 16x16 LED-Matrix auf mehr LEDs erweitern
    * Die 16x16 LEDs sind vergleichsweise fürs menschliche Auge ziemlich wenig und man kann keine schönen Flugkurven darstellen, stattdessen ist das Spiel sehr stockend. Man könnte ein LED-Feld nutzen, welches mehr LEDs hat, wodurch die Animation etwas angenehmer wird.
* Ergonomischerer Button
    * Wenn man das Spiel eine Weile spielt, wird der Buttonklick mit der Zeit relativ unangenehm, da man Kraft aufwenden muss, um den Button ordentlich zu drücken. Wenn man eine Button nutzen würde, der einfacher zu drücken ist, würde das das Spielerlebnis des Spielenden um einiges verbessern.
* Schwierigkeit mit der Zeit erhöhen
    * Derzeit wird das Spiel mit der Zeit schwieriger zu spielen, da die Geschwindigkeit sich erhöht, mit der die Balken auf den Vogel zu kommen. Es gibt noch weitere Funktionen, die man implementieren könnte, damit das Spiel mit der Zeit an Schwierigkeit zunimmt:
      * Lücke in den Balken verringern: Man könnte die Lücke in den Balken mit fortlaufender Spielzeit verringern, damit es anspruchsvoller wird, das Spiel zu spielen.
      * Abstand zwischen den Balken verringern: Man könnte den Abstand zwischen den Balken mit fortlaufender Spielzeit verringern.
      * Abstand zwischen den Lücken aufeinanderfolgender Balken vergrößern: Je größer der Abstand zwischen den Lücken zweier Balken ist, desto schwieriger wird es sie rechtzeitig zu überwinden.
