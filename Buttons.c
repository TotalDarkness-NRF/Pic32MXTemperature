#include "Thermometer.h"

char text[32];

void Color() {
        if (BUTTON4) {
            RED = !RED;
            Delay(500);
        }
        if (BUTTON5) {
            GREEN = !GREEN;
            Delay(500);
        }
        if (BUTTON6) {
            BLUE = !BLUE;
            Delay(500);
        }
}

void ChangePrecision() {
    if (BUTTON2) {
        int precision = getPrecision();
        precision = precision >= 12 ? 9 : ++precision;
        setPrecision(precision);
        clearDisplay();
        sprintf(text, "Precision set to");
        displayString(text);
        sprintf(text, "%d", precision);
        displayStringStart(24, text);
        Delay(1000);
        clearDisplay();
    }
}

void ChangeTemperatureUnit() {
    if (BUTTON3) {
        char unit = getUnit();
        if (unit == 'F') unit = 'K';
        else if (unit == 'C') unit = 'F';
        else unit = 'C';
        setUnit(unit);
        clearDisplay();
        if (unit == 'K') sprintf(text, "Temperature set to Kelvin");
        else if (unit == 'F') sprintf(text, "Temperature set to Fahrenheit");
        else sprintf(text, "Temperature set to Celsius");
        displayString(text);
        Delay(1000);
        clearDisplay();
    }
}

int ChangeViewMode(int viewMode) {
    if (BUTTON1) {
        viewMode = viewMode ? 0:1;
        clearDisplay();
        sprintf(text, "Changing");
        displayStringStart(4, text);
        sprintf(text, "View Mode");
        displayStringStart(20, text);
        Delay(1000);
        clearDisplay();
    }
    return viewMode;
}