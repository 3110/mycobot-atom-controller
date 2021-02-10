#include <M5Atom.h>
#include "m5atom/EspNowController.h"

const uint8_t ESP_NOW_CHANNEL = 3;
const uint8_t GRIPPER_OPEN_CMD[] = {0xFE, 0xFE, 4, 0x66, 0, 50, 0xFA};
const uint8_t GRIPPER_CLOSE_CMD[] = {0xFE, 0xFE, 4, 0x66, 1, 50, 0xFA};
const CRGB CONNECT_COLOR(0xf0, 0xf0, 0x00);
const CRGB GRIPPER_OPEN_COLOR(0x00, 0x00, 0xf0);
const CRGB GRIPPER_CLOSE_COLOR(0x00, 0xf0, 0x00); // Red

EspNowController controller(ESP_NOW_CHANNEL);
bool is_gripper_open = false;

void setup(void)
{
    M5.begin(true, false, true); // serial, I2C, Display
    if (controller.begin())
    {
        M5.dis.drawpix(0, CONNECT_COLOR);
    }
}

void loop(void)
{
    M5.update();

    if (M5.Btn.wasPressed())
    {
        is_gripper_open = !is_gripper_open;
        if (is_gripper_open)
        {
            controller.send(GRIPPER_OPEN_CMD, sizeof(GRIPPER_OPEN_CMD));
            M5.dis.drawpix(0, GRIPPER_OPEN_COLOR);
        }
        else
        {
            controller.send(GRIPPER_CLOSE_CMD, sizeof(GRIPPER_CLOSE_CMD));
            M5.dis.drawpix(0, GRIPPER_CLOSE_COLOR);
        }
    }
}
