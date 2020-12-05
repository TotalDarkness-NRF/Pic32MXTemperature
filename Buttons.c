#include "Thermometer.h"

 // TODO maybe have interupts instead, delay do to getting temperature

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

void ChangePrecision() {
    if(BUTTON3) {
        int precision = getPrecsion();
        if (precision >= 12) 
            setPrecision(9);
        else setPrecision(++precision);
         clearDisplay();
         char text[32];
         sprintf(text, "Precision set to%D", getPrecsion());
         displayString(text);
         Delay(1000);
    }
}

void ChangeTemperatureUnit() {
    
}

void ShowTime() {
    
}