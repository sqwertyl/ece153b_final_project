#include "NUNCHUK.h"

static enum DIR direction = NONE;
static bool Z_button_pressed = false;

void parse_Nunchuk(uint8_t *data) {
    if (data[0] > 0) {
        if (data[1] > 200) {
            direction = FORWARD;
        else if (data[1] < 40)
            direction = BACKWARD;
        else if (data[0] > 200)
            direction = RIGHT;
        else if (data[0] < 40)
            direction = LEFT;
        else
            direction = NONE;
    }

    Z_button_pressed = (data[5] >> 0) & 1) ? false : true;

}

enum DIR get_Direction(void) { return direction; }

bool get_Z_pressed(void) { return Z_button_pressed; }