#include <p32xxxx.h>
#include "Buttons.h"
#include "LCD.h"

void Color() { // TODO maybe have interupts instead, delay do to getting temperature
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