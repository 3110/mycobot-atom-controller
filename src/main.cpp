#include "main.h"

const uint8_t ESP_NOW_CHANNEL = 1;
const uint8_t GRIPPER_OPEN_CMD[] = {0xFE, 0xFE, 4, 0x66, 0, 100, 0xFA};
const uint8_t GRIPPER_CLOSE_CMD[] = {0xFE, 0xFE, 4, 0x66, 1, 100, 0xFA};
const uint8_t GRIPPER_INITIAL_POSITION_CMD[] = {0xFE, 0xFE, 0x0F,
                                                0x22,
                                                0, 0,
                                                0, 0,
                                                0, 0,
                                                0, 0,
                                                0, 0,
                                                0x11, 0x94,
                                                0x64,
                                                0xFA};
const uint8_t GRIPPER_FREE_POSITION_CMD[] = {0xFE, 0xFE, 0x0F,
                                             0x22,
                                             0xff, 0x0a,
                                             0x1e, 0xf7,
                                             0xcc, 0x20,
                                             0xcf, 0xa9,
                                             0xfe, 0xf0,
                                             0x11, 0x94,
                                             0x64,
                                             0xFA};

const CRGB CONNECT_COLOR(0xf0, 0xf0, 0x00);
const CRGB GRIPPER_OPEN_COLOR(0x00, 0x00, 0xf0);
const CRGB GRIPPER_CLOSE_COLOR(0x00, 0xf0, 0x00); // Red

EspNowController controller(ESP_NOW_CHANNEL);
size_t pos = 0;

Command COMMANDS[] = {
    GRIPPER_INITIAL_POSITION,
    GRIPPER_CLOSE,
    GRIPPER_OPEN,
    GRIPPER_CLOSE,
    GRIPPER_OPEN,
    GRIPPER_FREE_POSITION,
    GRIPPER_CLOSE,
    GRIPPER_OPEN,
    GRIPPER_CLOSE,
    GRIPPER_OPEN,
};

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
        sendCommand(COMMANDS[pos]);
        delay(1000);
    }
}

void sendCommand(Command cmd)
{
    M5.dis.drawpix(0, CONNECT_COLOR);
    switch (cmd)
    {
    case GRIPPER_INITIAL_POSITION:
        controller.send(GRIPPER_INITIAL_POSITION_CMD, sizeof(GRIPPER_INITIAL_POSITION_CMD));
        break;
    case GRIPPER_FREE_POSITION:
        controller.send(GRIPPER_FREE_POSITION_CMD, sizeof(GRIPPER_FREE_POSITION_CMD));
        break;
    case GRIPPER_CLOSE:
        M5.dis.drawpix(0, GRIPPER_CLOSE_COLOR);
        controller.send(GRIPPER_CLOSE_CMD, sizeof(GRIPPER_CLOSE_CMD));
        break;
    case GRIPPER_OPEN:
        M5.dis.drawpix(0, GRIPPER_OPEN_COLOR);
        controller.send(GRIPPER_OPEN_CMD, sizeof(GRIPPER_OPEN_CMD));
        break;
    default:
        SERIAL_PRINTF("Unknown Command: %d", COMMANDS[pos]);
        SERIAL_PRINTLN();
        break;
    }
    pos = (pos + 1) % MAX_NUMBER_OF_COMMANDS;
}
