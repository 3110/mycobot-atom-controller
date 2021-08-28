#include <M5Atom.h>

#include "EspNowController.h"
#include "MovingMean.h"
#include "MyCobotCommand.h"
#include "common.h"

const uint8_t ESP_NOW_CHANNEL = 1;

const CRGB CONNECT_COLOR(0xf0, 0xf0, 0x00);
const CRGB GRIPPER_OPEN_COLOR(0x00, 0x00, 0xf0);
const CRGB GRIPPER_CLOSE_COLOR(0x00, 0xf0, 0x00);  // Red

#ifdef ENABLE_FADER_UNIT
#ifdef TARGET_M5ATOM
const uint8_t INPUT_PIN = 32;
#else
const uint8_t INPUT_PIN = 36;
#endif
const uint16_t FADER_MIN_VALUE = 0;
const uint16_t FADER_MAX_VALUE = 4096;
#endif

const uint16_t CLOSE_GRIPPER = 1300;
const uint16_t OPEN_GRIPPER = 2048;
const uint16_t GRIPPER_THRETHOLD =
    CLOSE_GRIPPER + (OPEN_GRIPPER - CLOSE_GRIPPER) / 2;

EspNowController controller(ESP_NOW_CHANNEL);
MyCobotCommandFactory factory;
bool isGripperOpened = false;

#ifdef ENABLE_FADER_UNIT
const size_t WINDOW_SIZE = 3;
MovingMean<WINDOW_SIZE> movingMean;

uint16_t rawADC = 0;
uint16_t encodeValue = 0;
uint16_t prevEncodeValue = 0;
#endif

SendAnglesCommand* initPosition =
    static_cast<SendAnglesCommand*>(factory.create(CommandID::SendAngles))
        ->setAngle(Joint::J1, 0.0)
        ->setAngle(Joint::J2, 65.0)
        ->setAngle(Joint::J3, 145.0)
        ->setAngle(Joint::J4, -150.0)
        ->setAngle(Joint::J5, 175.0)
        ->setAngle(Joint::J6, 100.0)
        ->setSpeed(50);

#ifdef ENABLE_FADER_UNIT
SetEncoder* setEncoder =
    static_cast<SetEncoder*>(factory.create(CommandID::SetEncoder))
        ->setJoint(Joint::Gripper)
        ->setValue(2048);
#else
SetGripperState* gripper =
    static_cast<SetGripperState*>(factory.create(CommandID::SetGripperState))
        ->open()
        ->setSpeed(0);  // In AtomMain 3.2, Only 0 is valid for the speed
#endif

uint8_t cmdBuf[MyCobotCommand::MAX_COMMAND_LEN] = {0};

void setup(void) {
    M5.begin(true, false, true);  // serial, I2C, Display
#ifdef ENABLE_FADER_UNIT
    pinMode(INPUT_PIN, INPUT);
#endif
    if (controller.begin()) {
        M5.dis.drawpix(0, CONNECT_COLOR);
        size_t n = initPosition->serialize(cmdBuf, sizeof(cmdBuf));
        controller.send(cmdBuf, n);
    }
}

void loop(void) {
    M5.update();
#ifdef ENABLE_FADER_UNIT
    rawADC = movingMean.update(analogRead(INPUT_PIN));
    encodeValue = map(rawADC, FADER_MIN_VALUE, FADER_MAX_VALUE, OPEN_GRIPPER,
                      CLOSE_GRIPPER);
    if (abs(encodeValue - prevEncodeValue) > 10) {
        prevEncodeValue = encodeValue;
        isGripperOpened = encodeValue > GRIPPER_THRETHOLD;
        setEncoder->setValue(encodeValue);
        size_t n = setEncoder->serialize(cmdBuf, sizeof(cmdBuf));
        controller.send(cmdBuf, n);
    }
    delay(100);
#else
    if (M5.Btn.wasPressed()) {
        size_t n = 0;
        if (isGripperOpened) {
            gripper->close();
            isGripperOpened = false;
        } else {
            gripper->open();
            isGripperOpened = true;
        }
        n = gripper->serialize(cmdBuf, sizeof(cmdBuf));
        controller.send(cmdBuf, n);
        delay(1000);
    }
#endif
}
