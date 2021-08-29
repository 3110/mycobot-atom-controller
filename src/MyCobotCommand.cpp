#include "MyCobotCommand.h"

MyCobotCommand::MyCobotCommand(CommandID id) : ID(id) {
}

MyCobotCommand::~MyCobotCommand(void) {
}

void MyCobotCommand::dump(HardwareSerial &s) const {
    uint8_t buf[MAX_COMMAND_LEN] = {0};
    size_t n = serialize(buf, sizeof(buf));
    for (int i = 0; i < n; ++i) {
        s.printf("%02x ", buf[i]);
    }
    s.println();
}

const char *MyCobotCommand::getName(void) const {
    return ID.getName();
}

bool MyCobotCommand::serialize(HardwareSerial &s) const {
    uint8_t buf[MAX_COMMAND_LEN] = {0};
    if (serialize(buf, sizeof(buf)) == getCommandLength()) {
        return s.write(buf, getCommandLength()) == getCommandLength();
    } else {
        return false;
    }
}

bool MyCobotCommand::deserialize(HardwareSerial &s) {
    int8_t c = 0;
    if (!isFoundHeader(s)) {
        return false;
    }
    if (readInt8(s, c) == 0 || c != ID.getValue()) {
        return false;
    }
    // TODO:
    return false;
}

size_t MyCobotCommand::serialize(uint8_t *buf, size_t len, size_t pos) const {
    if (buf == nullptr || pos + getCommandLength() >= len) {
        return 0;
    }
    size_t n = 0;
    n += writeInt8(static_cast<int8_t>(DataFrame::Header), buf, len, pos + n);
    n += writeInt8(static_cast<int8_t>(DataFrame::Header), buf, len, pos + n);
    n += writeInt8(getDataLength() + 2, buf, len, pos + n);
    n += writeInt8(ID.getValue(), buf, len, pos + n);
    n += serializeData(buf, len, pos + n);
    n += writeInt8(static_cast<int8_t>(DataFrame::Footer), buf, len, pos + n);
    return n;
}

size_t MyCobotCommand::deserialize(const uint8_t *buf, size_t len, size_t pos) {
    if (buf == nullptr || pos >= len) {
        return 0;
    }
    if (findHeader(buf, len, pos)) {
    } else {
    }
    return 0;
}

int MyCobotCommand::findHeader(const uint8_t *buf, size_t len,
                               size_t pos) const {
    if (buf == nullptr || pos >= len) {
        return -1;
    }
    bool startHeader = false;
    for (; pos < len; ++pos) {
        if (startHeader) {
            if (buf[pos] == static_cast<uint8_t>(DataFrame::Header)) {
                return pos + 1;
            }
            startHeader = false;
        } else {
            if (buf[pos] == static_cast<uint8_t>(DataFrame::Header)) {
                startHeader = true;
            }
        }
    }
    return -1;
}

bool MyCobotCommand::isFoundHeader(HardwareSerial &s) const {
    bool startHeader = false;
    int c = 0;
    while (true) {
        c = s.read();
        if (c == -1) {
            return false;
        }
        if (startHeader) {
            if (c == static_cast<int>(DataFrame::Header)) {
                return true;
            }
            startHeader = false;
        } else {
            if (c == static_cast<int>(DataFrame::Header)) {
                startHeader = true;
            }
        }
    }
    return false;
}

size_t MyCobotCommand::readInt8(HardwareSerial &s, int8_t &data) const {
    unsigned long start = millis();
    int c = 0;
    while (millis() - start < READ_TIMEOUT_MS) {
        c = s.read();
        if (c != -1) {
            data = c;
            return 1;
        }
    }
    return 0;
}

size_t MyCobotCommand::readInt16(HardwareSerial &s, int16_t &data) const {
    uint8_t buf[2] = {0};
    size_t n = read(s, buf, sizeof(buf));
    if (n == sizeof(buf)) {
        return readInt16(data, buf, n);
    }
    return 0;
}

size_t MyCobotCommand::readFloat(HardwareSerial &s, float &data) const {
    int16_t v = 0;
    size_t n = readInt16(s, v);
    if (n > 0) {
        data = static_cast<float>(v) / ANGLE_COEFFICIENT;
    }
    return n;
}

size_t MyCobotCommand::read(HardwareSerial &s, uint8_t *buf, size_t len,
                            size_t pos) const {
    if (buf == nullptr || pos + s.available() >= len) {
        return 0;
    }
    int8_t c = 0;
    size_t n = 0;
    while (pos + n < len) {
        if (readInt8(s, c) == 0) {
            break;
        }
        buf[pos + n] = c;
        ++n;
    }
    return n;
}

size_t MyCobotCommand::readInt8(int8_t &data, const uint8_t *buf, size_t len,
                                size_t pos) const {
    if (buf == nullptr || pos >= len) {
        return 0;
    }
    data = buf[pos];
    return 1;
}

size_t MyCobotCommand::readInt16(int16_t &data, const uint8_t *buf, size_t len,
                                 size_t pos) const {
    if (buf == nullptr || pos + 1 >= len) {
        return 0;
    }
    data = (buf[pos] << 8) | buf[pos + 1];
    return 2;
}

size_t MyCobotCommand::readFloat(float &data, const uint8_t *buf, size_t len,
                                 size_t pos) const {
    int16_t v = 0;
    size_t n = readInt16(v, buf, len, pos);
    if (n > 0) {
        data = v / static_cast<float>(ANGLE_COEFFICIENT);
    }
    return n;
}

size_t MyCobotCommand::writeInt8(int8_t data, uint8_t *buf, size_t len,
                                 size_t pos) const {
    if (buf == nullptr || pos >= len) {
        return 0;
    }
    buf[pos] = data;
    return sizeof(int8_t);
}

size_t MyCobotCommand::writeInt16(int16_t data, uint8_t *buf, size_t len,
                                  size_t pos) const {
    if (buf == nullptr || pos + 1 >= len) {
        return 0;
    }
    size_t n = 0;
    n += writeInt8(highByte(data), buf, len, pos + n);
    n += writeInt8(lowByte(data), buf, len, pos + n);
    return n;
}

size_t MyCobotCommand::writeFloat(float data, uint8_t *buf, size_t len,
                                  size_t pos) const {
    if (buf == nullptr || pos + 1 >= len) {
        return 0;
    }
    return writeInt16(data * ANGLE_COEFFICIENT, buf, len, pos);
}

MyCobotCommandFactory::MyCobotCommandFactory(void) {
}

MyCobotCommandFactory::~MyCobotCommandFactory(void) {
}

MyCobotCommand *MyCobotCommandFactory::create(CommandID id) {
    switch (id.getValue()) {
        case CommandID::GetVersion:
            return nullptr;
        case CommandID::PowerOn:
            return nullptr;
        case CommandID::PowerOff:
            return nullptr;
        case CommandID::ReleaseAllServos:
            return nullptr;
        case CommandID::IsControllerConnected:
            return nullptr;
        case CommandID::ReadNextError:
            return nullptr;
        case CommandID::SetFreeMode:
            return nullptr;
        case CommandID::IsFreeMode:
            return nullptr;
        case CommandID::GetAngles:
            return nullptr;
        case CommandID::SendAngle:
            return nullptr;
        case CommandID::SendAngles:
            return new SendAnglesCommand();
        case CommandID::GetCoords:
            return nullptr;
        case CommandID::SendCoord:
            return nullptr;
        case CommandID::SendCoords:
            return nullptr;
        case CommandID::Pause:
            return nullptr;
        case CommandID::IsPaused:
            return nullptr;
        case CommandID::Resume:
            return nullptr;
        case CommandID::Stop:
            return nullptr;
        case CommandID::IsInPosition:
            return nullptr;
        case CommandID::IsMoving:
            return nullptr;
        case CommandID::JogAngle:
            return nullptr;
        case CommandID::JogCoord:
            return nullptr;
        case CommandID::JogStop:
            return nullptr;
        case CommandID::SetEncoder:
            return new SetEncoder();
        case CommandID::GetEncoder:
            return nullptr;
        case CommandID::SetEncoders:
            return nullptr;
        case CommandID::GetEncoders:
            return nullptr;
        case CommandID::GetSpeed:
            return nullptr;
        case CommandID::SetSpeed:
            return nullptr;
        case CommandID::GetFeedOverride:
            return nullptr;
        case CommandID::GetAcceleration:
            return nullptr;
        case CommandID::GetJointMinAngle:
            return nullptr;
        case CommandID::GetJointMaxAngle:
            return nullptr;
        case CommandID::IsServoEnable:
            return nullptr;
        case CommandID::IsAllServoEnable:
            return nullptr;
        case CommandID::SetServoData:
            return nullptr;
        case CommandID::GetServoData:
            return nullptr;
        case CommandID::SetServoCalibration:
            return nullptr;
        case CommandID::ReleaseServo:
            return nullptr;
        case CommandID::FocusServo:
            return nullptr;
        case CommandID::SetPinMode:
            return nullptr;
        case CommandID::SetDigitalOutput:
            return nullptr;
        case CommandID::GetDigitalInput:
            return nullptr;
        case CommandID::SetPWMMode:
            return nullptr;
        case CommandID::SetPWMOutput:
            return nullptr;
        case CommandID::GetGripperValue:
            return nullptr;
        case CommandID::SetGripperState:
            return new SetGripperState();
        case CommandID::SetGripperValue:
            return nullptr;
        case CommandID::SetGripperIni:
            return nullptr;
        case CommandID::IsGrippreMoving:
            return nullptr;
        case CommandID::SetColor:
            return nullptr;
        case CommandID::SetBasicOutput:
            return nullptr;
        case CommandID::GetBasicInput:
            return nullptr;
        default:
            return nullptr;
    }
}

SendAnglesCommand::SendAnglesCommand(void)
    : MyCobotCommand(CommandID::SendAngles), angles{0}, speed(0) {
}

SendAnglesCommand::~SendAnglesCommand(void) {
}

size_t SendAnglesCommand::serializeData(uint8_t *buf, size_t len,
                                        size_t pos) const {
    if (buf == nullptr || pos + getDataLength() >= len) {
        return 0;
    }
    size_t n = 0;
    for (int i = 0; i < NUMBER_OF_JOINTS; ++i) {
        n += writeFloat(this->angles[i], buf, len, pos + n);
    }
    n += writeInt8(this->speed, buf, len, pos + n);
    return n;
}

size_t SendAnglesCommand::deserializeData(const uint8_t *buf, size_t len,
                                          size_t pos) {
    if (buf == nullptr || pos + getDataLength() >= len) {
        return 0;
    }
    size_t n = 0;
    for (int i = 0; i < NUMBER_OF_JOINTS; ++i) {
        n += readFloat(this->angles[i], buf, len, pos + n);
    }
    n += readInt8(this->speed, buf, len, pos + n);
    return n;
}

int8_t SendAnglesCommand::getDataLength(void) const {
    return NUMBER_OF_JOINTS * 2 + 1;
}

float SendAnglesCommand::getAngle(Joint joint) const {
    return this->angles[static_cast<int>(joint)];
}

int8_t SendAnglesCommand::getSpeed(void) const {
    return this->speed;
}

SendAnglesCommand *SendAnglesCommand::setAngle(Joint joint, float angle) {
    this->angles[static_cast<int>(joint)] = angle;
    return this;
}

SendAnglesCommand *SendAnglesCommand::setSpeed(int8_t speed) {
    this->speed = speed;
    return this;
}

SetGripperState::SetGripperState(void)
    : MyCobotCommand(CommandID::SetGripperState), opened(false), speed(0) {
}

SetGripperState::~SetGripperState(void) {
}

bool SetGripperState::isOpened(void) const {
    return this->opened;
}

SetGripperState *SetGripperState::open(void) {
    this->opened = true;
    return this;
}

SetGripperState *SetGripperState::close(void) {
    this->opened = false;
    return this;
}

SetGripperState *SetGripperState::setSpeed(int8_t speed) {
    this->speed = speed;
    return this;
}

size_t SetGripperState::serializeData(uint8_t *buf, size_t len,
                                      size_t pos) const {
    if (buf == nullptr || pos + getDataLength() >= len) {
        return 0;
    }
    size_t n = 0;
    n += writeInt8(this->opened ? 0 : 1, buf, len, pos + n);
    n += writeInt8(this->speed, buf, len, pos + n);
    return n;
}

size_t SetGripperState::deserializeData(const uint8_t *buf, size_t len,
                                        size_t pos) {
    if (buf == nullptr || pos + getDataLength() >= len) {
        return 0;
    }
    size_t n = 0;
    int8_t c = 0;
    n += readInt8(c, buf, len, pos + n);
    if (n == 0) {
        return 0;
    }
    if (c == 0) {
        this->opened = true;
    } else if (c == 1) {
        this->opened = false;
    } else {
        return 0;
    }
    n += readInt8(c, buf, len, pos + n);
    if (n == 1) {
        return 0;
    }
    if (c > 0) {
        this->speed = c;
    }
    return n;
}

int8_t SetGripperState::getDataLength(void) const {
    return 2;
}

SetEncoder::SetEncoder(void)
    : MyCobotCommand(CommandID::SetEncoder), joint(Joint::Gripper), value(0) {
}

SetEncoder::~SetEncoder(void) {
}

int16_t SetEncoder::getValue(void) const {
    return this->value;
}

Joint SetEncoder::getJoint(void) const {
    return this->joint;
}

SetEncoder *SetEncoder::setValue(int16_t value) {
    this->value = value;
    return this;
}

SetEncoder *SetEncoder::setJoint(Joint joint) {
    this->joint = joint;
    return this;
}

size_t SetEncoder::serializeData(uint8_t *buf, size_t len, size_t pos) const {
    if (buf == nullptr || pos + getDataLength() >= len) {
        return 0;
    }
    size_t count = 0;
    size_t n = 0;
    n = writeInt8(static_cast<int8_t>(this->joint), buf, len, pos + count);
    if (n == 0) {
        return 0;
    }
    count += n;
    n = writeInt16(this->value, buf, len, pos + count);
    if (n == 0) {
        return 0;
    }
    count += n;
    return count;
}

size_t SetEncoder::deserializeData(const uint8_t *buf, size_t len, size_t pos) {
    if (buf == nullptr || pos + getDataLength() >= len) {
        return 0;
    }
    size_t count = 0;
    size_t n = 0;
    int8_t joint = 0;
    int16_t value = 0;
    n = readInt8(joint, buf, len, pos + count);
    if (n == 0) {
        return 0;
    }
    count += n;
    n = readInt16(value, buf, len, pos + count);
    if (n == 0) {
        return 0;
    }
    count += n;

    this->joint = static_cast<Joint>(joint);
    this->value = value;
    return count;
}

int8_t SetEncoder::getDataLength(void) const {
    return 3;
}
