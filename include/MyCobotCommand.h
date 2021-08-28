#ifndef MYCOBOT_COMMAND_H_
#define MYCOBOT_COMMAND_H_

#include <Arduino.h>

const size_t NUMBER_OF_JOINTS = 6;

enum class Joint {
    J1 = 0,
    J2,
    J3,
    J4,
    J5,
    J6,
    Gripper = 7,
};

enum class DataFrame {
    Header = 0xFE,
    Footer = 0xFA,
};

enum class DataFrameState {
    None,
    HeaderStart,
    HeaderEnd,
    CommandLength,
    Command,
    Data,
    Footer,
    Illegal,
};

struct CommandID {
    enum ID {
        GetVersion = 0x00,
        PowerOn = 0x10,
        PowerOff = 0x11,
        ReleaseAllServos = 0x13,
        IsControllerConnected = 0x14,
        ReadNextError = 0x15,
        SetFreeMode = 0x1A,
        IsFreeMode = 0x1B,
        GetAngles = 0x20,
        SendAngle = 0x21,
        SendAngles = 0x22,
        GetCoords = 0x23,
        SendCoord = 0x24,
        SendCoords = 0x25,
        Pause = 0x26,
        IsPaused = 0x27,
        Resume = 0x28,
        Stop = 0x29,
        IsInPosition = 0x2A,
        IsMoving = 0x2B,
        JogAngle = 0x30,
        JogCoord = 0x32,
        JogStop = 0x34,
        SetEncoder = 0x3A,
        GetEncoder = 0x3B,
        SetEncoders = 0x3C,
        GetEncoders = 0x3D,
        GetSpeed = 0x40,
        SetSpeed = 0x41,
        GetFeedOverride = 0x42,
        GetAcceleration = 0x44,
        GetJointMinAngle = 0x4A,
        GetJointMaxAngle = 0x4B,
        IsServoEnable = 0x50,
        IsAllServoEnable = 0x51,
        SetServoData = 0x52,
        GetServoData = 0x53,
        SetServoCalibration = 0x54,
        ReleaseServo = 0x56,
        FocusServo = 0x57,
        SetPinMode = 0x60,
        SetDigitalOutput = 0x61,
        GetDigitalInput = 0x62,
        SetPWMMode = 0x63,
        SetPWMOutput = 0x64,
        GetGripperValue = 0x65,
        SetGripperState = 0x66,
        SetGripperValue = 0x67,
        SetGripperIni = 0x68,
        IsGrippreMoving = 0x69,
        SetColor = 0x6A,
        SetBasicOutput = 0xA0,
        GetBasicInput = 0xA1,
    } id;

    const char *getName(void) const {
        switch (this->id) {
            case CommandID::GetVersion:
                return "Get Version";
            case CommandID::PowerOn:
                return "Power On";
            case CommandID::PowerOff:
                return "Power Off";
            case CommandID::ReleaseAllServos:
                return "Release All Servos";
            case CommandID::IsControllerConnected:
                return "In Controller Connected";
            case CommandID::ReadNextError:
                return "Read Next Error";
            case CommandID::SetFreeMode:
                return "Set Free Mode";
            case CommandID::IsFreeMode:
                return "Is Free Mode";
            case CommandID::GetAngles:
                return "Get Angles";
            case CommandID::SendAngle:
                return "Send Angle";
            case CommandID::SendAngles:
                return "Send Angles";
            case CommandID::GetCoords:
                return "Get Coords";
            case CommandID::SendCoord:
                return "Send Coord";
            case CommandID::SendCoords:
                return "Send Coords";
            case CommandID::Pause:
                return "Pause";
            case CommandID::IsPaused:
                return "Is Paused?";
            case CommandID::Resume:
                return "Resume";
            case CommandID::Stop:
                return "Stop";
            case CommandID::IsInPosition:
                return "Is in Position?";
            case CommandID::IsMoving:
                return "Is Moving?";
            case CommandID::JogAngle:
                return "Jog Angle";
            case CommandID::JogCoord:
                return "Jog Coord";
            case CommandID::JogStop:
                return "Jog Stop";
            case CommandID::SetEncoder:
                return "Set Encoder";
            case CommandID::GetEncoder:
                return "Get Encoder";
            case CommandID::SetEncoders:
                return "Set Encoders";
            case CommandID::GetEncoders:
                return "Get Encoders";
            case CommandID::GetSpeed:
                return "Get Speed";
            case CommandID::SetSpeed:
                return "Set Speed";
            case CommandID::GetFeedOverride:
                return "Get Feed Override";
            case CommandID::GetAcceleration:
                return "Get Acceleration";
            case CommandID::GetJointMinAngle:
                return "GetJointMinAngle";
            case CommandID::GetJointMaxAngle:
                return "GetJointMaxAngle";
            case CommandID::IsServoEnable:
                return "Is Servo Enable?";
            case CommandID::IsAllServoEnable:
                return "Is All Servo Enable?";
            case CommandID::SetServoData:
                return "Set Servo Data";
            case CommandID::GetServoData:
                return "Get Servo Data";
            case CommandID::SetServoCalibration:
                return "Set Servo Calibration";
            case CommandID::ReleaseServo:
                return "Release Servo";
            case CommandID::FocusServo:
                return "Focus Servo";
            case CommandID::SetPinMode:
                return "set Pin Mode";
            case CommandID::SetDigitalOutput:
                return "Set Digital Output";
            case CommandID::GetDigitalInput:
                return "Get Digital Input";
            case CommandID::SetPWMMode:
                return "Set PWM Mode";
            case CommandID::SetPWMOutput:
                return "Set PWM Output";
            case CommandID::GetGripperValue:
                return "Get Gripper Value";
            case CommandID::SetGripperState:
                return "Set Gripper State";
            case CommandID::SetGripperValue:
                return "Set Gripper Value";
            case CommandID::SetGripperIni:
                return "Set Gripper Ini";
            case CommandID::IsGrippreMoving:
                return "Is Gripper Moving?";
            case CommandID::SetColor:
                return "Set Color";
            case CommandID::SetBasicOutput:
                return "Set Basic Output";
            case CommandID::GetBasicInput:
                return "Get Basic Input";
            default:
                return nullptr;
        }
    }

    CommandID(enum ID id) : id(id) {
    }

    int getValue(void) const {
        return id;
    }

    bool operator==(const CommandID &v) {
        return id == v.id;
    }
};

class MyCobotCommand {
public:
    static const size_t MAX_COMMAND_LEN = 18;
    static const int READ_TIMEOUT_MS = 30;
    static const int ANGLE_COEFFICIENT = 100;

    MyCobotCommand(CommandID id);
    virtual ~MyCobotCommand(void);

    virtual const char *getName(void) const;

    inline CommandID getID(void) const {
        return ID;
    }

    bool serialize(HardwareSerial &s);
    bool deserialize(HardwareSerial &s);

    size_t serialize(uint8_t *buf, size_t len, size_t pos = 0);
    size_t deserialize(const uint8_t *buf, size_t len, size_t pos = 0);

    virtual void dump(HardwareSerial &s);

protected:
    inline size_t getCommandLength() const {
        return getDataLength() + 3;
    }
    int findHeader(const uint8_t *buf, size_t len, size_t pos = 0);
    bool isFoundHeader(HardwareSerial &s);

    virtual size_t serializeData(uint8_t *buf, size_t len, size_t pos = 0) = 0;
    virtual size_t deserializeData(const uint8_t *buf, size_t len,
                                   size_t pos = 0) = 0;
    virtual int8_t getDataLength(void) const = 0;

    size_t readInt8(HardwareSerial &s, int8_t &data);
    size_t readInt16(HardwareSerial &s, int16_t &data);
    size_t readFloat(HardwareSerial &s, float &data);
    size_t read(HardwareSerial &s, uint8_t *buf, size_t len, size_t pos = 0);

    size_t readInt8(int8_t &data, const uint8_t *buf, size_t len,
                    size_t pos = 0);
    size_t readInt16(int16_t &data, const uint8_t *buf, size_t len,
                     size_t pos = 0);
    size_t readFloat(float &data, const uint8_t *buf, size_t len,
                     size_t pos = 0);

    size_t writeInt8(int8_t data, uint8_t *buf, size_t len, size_t pos = 0);
    size_t writeInt16(int16_t data, uint8_t *buf, size_t len, size_t pos = 0);
    size_t writeFloat(float data, uint8_t *buf, size_t len, size_t pos = 0);

private:
    const CommandID ID;
};

class MyCobotCommandFactory {
public:
    MyCobotCommandFactory(void);
    virtual ~MyCobotCommandFactory(void);
    virtual MyCobotCommand *create(CommandID id);
};

class SendAnglesCommand : public MyCobotCommand {
public:
    SendAnglesCommand(void);
    virtual ~SendAnglesCommand(void);

    virtual float getAngle(Joint joint) const;
    virtual int8_t getSpeed(void) const;
    virtual SendAnglesCommand *setAngle(Joint joint, float angle);
    virtual SendAnglesCommand *setSpeed(int8_t speed);

protected:
    virtual size_t serializeData(uint8_t *buf, size_t len, size_t pos = 0);
    virtual size_t deserializeData(const uint8_t *buf, size_t len,
                                   size_t pos = 0);
    virtual int8_t getDataLength(void) const;

private:
    float angles[NUMBER_OF_JOINTS];
    int8_t speed;
};

class SetGripperState : public MyCobotCommand {
public:
    SetGripperState(void);
    virtual ~SetGripperState(void);

    virtual bool isOpened(void) const;
    virtual SetGripperState *open(void);
    virtual SetGripperState *close(void);
    virtual SetGripperState *setSpeed(int8_t speed);

protected:
    virtual size_t serializeData(uint8_t *buf, size_t len, size_t pos = 0);
    virtual size_t deserializeData(const uint8_t *buf, size_t len,
                                   size_t pos = 0);
    virtual int8_t getDataLength(void) const;

private:
    bool opened;
    int8_t speed;
};

class SetEncoder : public MyCobotCommand {
public:
    SetEncoder(void);
    virtual ~SetEncoder(void);

    virtual int16_t getValue(void) const;
    virtual Joint getJoint(void) const;
    virtual SetEncoder *setValue(int16_t value);
    virtual SetEncoder *setJoint(Joint joint);

protected:
    virtual size_t serializeData(uint8_t *buf, size_t len, size_t pos = 0);
    virtual size_t deserializeData(const uint8_t *buf, size_t len,
                                   size_t pos = 0);
    virtual int8_t getDataLength(void) const;

private:
    Joint joint;
    int16_t value;
};

#endif