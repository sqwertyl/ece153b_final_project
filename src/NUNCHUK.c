#include "NUNCHUK.h"

static enum DIR direction = NONE;
static uint8_t Z_button_pressed = 0;
static uint8_t C_button_pressed = 0;

void parse_Nunchuk(uint8_t *data) {
    if (data[0] > 0) {
        if (data[1] > 200)
            direction = FORWARD;
        else if (data[1] < 40)
            direction = BACKWARD;
        //else if (data[0] > 200)
        //    direction = RIGHT;
       // else if (data[0] < 40)
       //    direction = LEFT;
        else
            direction = NONE;
    }

    Z_button_pressed = (data[5] >> 0 & 1) ? 0 : 1;
	C_button_pressed = (data[5] >> 1 & 1) ? 0 : 1;

}

enum DIR get_Direction(void) { return direction; }

uint8_t get_Z_pressed(void) { return Z_button_pressed; }

uint8_t get_C_pressed(void) { return C_button_pressed; }
