#include "LED.h"
const int lampuIjo = 5,
          lampuBiru = 5,
          buzzerPin = 12;

LED::LED(){
    pinMode(lampuBiru, OUTPUT);
    pinMode(lampuIjo, OUTPUT);
    pinMode(buzzerPin, OUTPUT);
}

LED::~LED(){}

void LED::hijau(int x){
    digitalWrite(lampuIjo, x);
}

void LED::biru(int x){
    digitalWrite(lampuBiru, x);
}

void LED::buzzer(int x){
    if(x == 1){
        digitalWrite(buzzer, HIGH);
        tone (buzzer, 1000);
    }elseif(x == 0){
        digitalWrite(buzzer, LOW);
        noTone(buzzer);
    }
}