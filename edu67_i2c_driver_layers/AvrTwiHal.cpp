#include "AvrTwiHal.h"

#if defined(ARDUINO_ARCH_AVR) && defined(I2C_BACKEND_IT)

#include <util/twi.h> // TW_STATUS and the names of all TWI status codes

namespace {

// The ISR is a plain function: it needs to find the object.
AvrTwiHal* pInstanceS = nullptr;

// Writing TWCR with TWINT=1 CLEARS the flag and lets the hardware do the next step.
// Which step is chosen by the other bits:
constexpr uint8_t TWCR_NEXT  = _BV(TWINT) | _BV(TWEN) | _BV(TWIE);              // send TWDR / receive, then NACK
constexpr uint8_t TWCR_ACK   = _BV(TWINT) | _BV(TWEN) | _BV(TWIE) | _BV(TWEA);  // receive, then ACK (want more)
constexpr uint8_t TWCR_START = _BV(TWINT) | _BV(TWEN) | _BV(TWIE) | _BV(TWSTA); // (repeated) START
constexpr uint8_t TWCR_STOP  = _BV(TWINT) | _BV(TWEN) | _BV(TWSTO);             // STOP; no interrupt after it
constexpr uint8_t TWCR_FREE  = _BV(TWINT) | _BV(TWEN);                          // let go of the bus

// Stops the compiler from moving memory writes past this point.
inline void compilerBarrier() { __asm__ __volatile__("" ::: "memory"); }

} // anonymous namespace


ISR(TWI_vect)
{
    pInstanceS->handleInterrupt();
}


AvrTwiHal::AvrTwiHal() :
    operationM(Operation::IDLE),
    address8bitM(0),
    registerBytesM{0, 0},
    registerIndexM(0),
    pRxDataM(nullptr),
    pTxDataM(nullptr),
    dataSizeM(0),
    dataIndexM(0),
    errorM(I2cResult::OK),
    pCallbacksM(nullptr)
{
    EDU_ASSERT(pInstanceS == nullptr);
    pInstanceS = this;
}

void AvrTwiHal::begin(uint32_t clockHzP, bool internalPullupsP)
{
    pinMode(SDA, internalPullupsP ? INPUT_PULLUP : INPUT);
    pinMode(SCL, internalPullupsP ? INPUT_PULLUP : INPUT);

    // SCL = F_CPU / (16 + 2 * TWBR * prescaler); prescaler = 1.
    // 16 MHz: 100 kHz -> TWBR = 72, 400 kHz -> TWBR = 12.
    TWSR = 0;
    TWBR = static_cast<uint8_t>(((F_CPU / clockHzP) - 16) / 2);
    TWCR = _BV(TWEN);
}

bool AvrTwiHal::memReadIt(
    uint8_t deviceAddress8bitP,
    uint16_t registerAddressP,
    uint8_t registerAddressSizeP,
    uint8_t* pDataP,
    uint16_t dataSizeP)
{
    pRxDataM = pDataP;
    pTxDataM = nullptr;
    return start(Operation::MEM_READ, deviceAddress8bitP, registerAddressP, registerAddressSizeP, dataSizeP);
}

bool AvrTwiHal::memWriteIt(
    uint8_t deviceAddress8bitP,
    uint16_t registerAddressP,
    uint8_t registerAddressSizeP,
    const uint8_t* pDataP,
    uint16_t dataSizeP)
{
    pRxDataM = nullptr;
    pTxDataM = pDataP;
    return start(Operation::MEM_WRITE, deviceAddress8bitP, registerAddressP, registerAddressSizeP, dataSizeP);
}

bool AvrTwiHal::start(
    Operation operationP,
    uint8_t deviceAddress8bitP,
    uint16_t registerAddressP,
    uint8_t registerAddressSizeP,
    uint16_t dataSizeP)
{
    if (operationM != Operation::IDLE)
    {
        errorM = I2cResult::BUSY;
        return false;
    }

    // The STOP of the previous transfer takes a few microseconds. The hardware
    // clears TWSTO when it is on the wire. If it never does, the bus is stuck.
    for (uint16_t i = 0; (TWCR & _BV(TWSTO)) && (i < 1000); ++i)
    {
    }
    if (TWCR & _BV(TWSTO))
    {
        errorM = I2cResult::BUSY;
        return false;
    }

    address8bitM      = deviceAddress8bitP;
    registerBytesM[0] = static_cast<uint8_t>(registerAddressP >> 8);   // MSB first...
    registerBytesM[1] = static_cast<uint8_t>(registerAddressP & 0xFF);
    registerIndexM    = (registerAddressSizeP == 2) ? 0 : 1;          // ...or only the LSB
    dataSizeM         = dataSizeP;
    dataIndexM        = 0;
    errorM            = I2cResult::OK;
    operationM        = operationP;

    // Everything above must be in memory before the first interrupt can come.
    compilerBarrier();
    TWCR = TWCR_START;
    return true;
}

void AvrTwiHal::handleInterrupt()
{
    switch (TW_STATUS)
    {
    // --- Address phase: always WRITE first, to send the register address ---
    case TW_START:
        TWDR = address8bitM | TW_WRITE;
        TWCR = TWCR_NEXT;
        break;

    case TW_MT_SLA_ACK:  // device answered its address
    case TW_MT_DATA_ACK: // device accepted a byte
        if (registerIndexM < 2)
        {
            TWDR = registerBytesM[registerIndexM++];
            TWCR = TWCR_NEXT;
        }
        else if (operationM == Operation::MEM_READ)
        {
            // Register address sent. Turn the bus around WITHOUT a STOP.
            TWCR = TWCR_START;
        }
        else if (dataIndexM < dataSizeM)
        {
            TWDR = pTxDataM[dataIndexM++];
            TWCR = TWCR_NEXT;
        }
        else
        {
            TWCR = TWCR_STOP;
            finish(I2cResult::OK);
        }
        break;

    // --- Read phase ---
    case TW_REP_START:
        TWDR = address8bitM | TW_READ;
        TWCR = TWCR_NEXT;
        break;

    case TW_MR_SLA_ACK:
        // The master ACKs every byte except the LAST one: the NACK tells
        // the device "stop sending". So for a 1-byte read: NACK right away.
        TWCR = (dataSizeM > 1) ? TWCR_ACK : TWCR_NEXT;
        break;

    case TW_MR_DATA_ACK:
        pRxDataM[dataIndexM++] = TWDR;
        TWCR = ((dataIndexM + 1) < dataSizeM) ? TWCR_ACK : TWCR_NEXT;
        break;

    case TW_MR_DATA_NACK: // the last byte
        pRxDataM[dataIndexM++] = TWDR;
        TWCR = TWCR_STOP;
        finish(I2cResult::OK);
        break;

    // --- Errors ---
    case TW_MT_SLA_NACK:
    case TW_MR_SLA_NACK:
        TWCR = TWCR_STOP;
        finish(I2cResult::NACK_ADDRESS);
        break;

    case TW_MT_DATA_NACK:
        TWCR = TWCR_STOP;
        finish(I2cResult::NACK_DATA);
        break;

    case TW_MT_ARB_LOST: // same code as TW_MR_ARB_LOST
        TWCR = TWCR_FREE;
        finish(I2cResult::ARBITRATION_LOST);
        break;

    case TW_BUS_ERROR:
    default:
        TWCR = TWCR_STOP;
        finish(I2cResult::BUS_ERROR);
        break;
    }
}

void AvrTwiHal::finish(I2cResult resultP)
{
    const Operation finished = operationM;
    errorM     = resultP;
    operationM = Operation::IDLE;

    CallbacksIf* const pCallbacks = pCallbacksM;
    if (pCallbacks == nullptr)
    {
        return;
    }
    if (resultP != I2cResult::OK)
    {
        pCallbacks->errorCallback();
    }
    else if (finished == Operation::MEM_READ)
    {
        pCallbacks->memRxCpltCallback();
    }
    else
    {
        pCallbacks->memTxCpltCallback();
    }
}

void AvrTwiHal::abort()
{
    const CriticalSection lock;
    // Switching the TWI off and on resets its state machine.
    TWCR = 0;
    operationM = Operation::IDLE;
    TWCR = _BV(TWEN);
}

void AvrTwiHal::setCallbacks(CallbacksIf& rCallbacksP)
{
    const CriticalSection lock;
    EDU_ASSERT(pCallbacksM == nullptr);
    pCallbacksM = &rCallbacksP;
}

void AvrTwiHal::resetCallbacks(CallbacksIf& rCallbacksP)
{
    const CriticalSection lock;
    EDU_ASSERT(pCallbacksM == &rCallbacksP);
    pCallbacksM = nullptr;
}

I2cResult AvrTwiHal::getError() const
{
    return errorM;
}

#endif
