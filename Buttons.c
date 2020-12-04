#include <p32xxxx.h>
#include "Buttons.h"
#include "LCD.h"
#include "TemperatureSensor.h"

void Color() {
        if (BUTTON4) {
            RED = !RED;
            Delay(200);
        }
        
        if (BUTTON5) {
            GREEN = !GREEN;
            Delay(200);
        }
        if (BUTTON6) {
            BLUE = !BLUE;
            Delay(200);
        }
}