#ifndef LED_H
#define LED_H
#include <Arduino.h>

class LED {
    public:
        LED();
        ~LED();
        void hijau(int x);
        void biru(int x);
        void buzzer(int x);
};
#endif