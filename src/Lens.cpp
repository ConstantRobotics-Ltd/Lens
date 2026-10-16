#include "Lens.h"
#include "LensVersion.h"
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <new>
#include <string>
#include <utility>



namespace
{
// Encoded data stores int and float fields as raw 4 bytes values.
static_assert(sizeof(int) == 4, "int must be 4 bytes");
static_assert(sizeof(float) == 4, "float must be 4 bytes");

/// Size of an int or float field in the encoded data, bytes.
constexpr int VALUE_SIZE{4};
/// Size of a bool field in the encoded data, bytes.
constexpr int FLAG_SIZE{1};
/// Number of the params which can be encoded: one for each field of the
/// params mask.
constexpr int FIELDS_NUMBER{50};
/// Number of bool fields (they use FLAG_SIZE bytes in the encoded data).
constexpr int FLAGS_NUMBER{3};
/// Number of int and float fields (they use VALUE_SIZE bytes).
constexpr int VALUES_NUMBER{FIELDS_NUMBER - FLAGS_NUMBER};
static_assert(sizeof(cr::lens::LensParamsMask) == FIELDS_NUMBER,
              "wrong number of params mask fields");

/// Data type of the encoded params (the first byte of the data).
constexpr uint8_t PARAMS_DATA_TYPE{0x02};
/// Position of the Major version in the encoded params. The Minor version
/// follows it.
constexpr int PARAMS_VERSION_POS{1};
/// Position of the params mask in the encoded params.
constexpr int PARAMS_MASK_POS{3};
/// Number of bits in a byte of the params mask.
constexpr int MASK_BYTE_BITS{8};
/// Bit of the first field of a mask byte: the most significant bit.
constexpr unsigned int MASK_FIRST_BIT{0x80U};
/// Size of the params mask: one bit for each field, the first field uses the
/// most significant bit of the first byte.
constexpr int PARAMS_MASK_SIZE{(FIELDS_NUMBER + MASK_BYTE_BITS - 1) /
                               MASK_BYTE_BITS};
/// Number of the bits of the last mask byte which belong to fields.
constexpr int LAST_MASK_BYTE_BITS{
    FIELDS_NUMBER - (PARAMS_MASK_SIZE - 1) * MASK_BYTE_BITS};
/// Bits of the last mask byte which don't belong to any field. They must be
/// zero: a set bit means that the data has a field which is unknown to the
/// decoder, so the layout of the data can't be recognized.
constexpr uint8_t PARAMS_RESERVED_MASK_BITS{
    static_cast<uint8_t>(0xFFU >> LAST_MASK_BYTE_BITS)};
/// Size of the encoded params header: data type, version and mask.
constexpr int PARAMS_HEADER_SIZE{PARAMS_MASK_POS + PARAMS_MASK_SIZE};
/// Maximum size of the encoded params (all fields encoded).
constexpr int PARAMS_MAX_SIZE{PARAMS_HEADER_SIZE +
                              VALUES_NUMBER * VALUE_SIZE +
                              FLAGS_NUMBER * FLAG_SIZE};
static_assert(PARAMS_MASK_SIZE == 7, "wrong size of params mask");
static_assert(PARAMS_RESERVED_MASK_BITS == 0x3F, "wrong reserved mask bits");
static_assert(PARAMS_MAX_SIZE == 201, "wrong size of encoded params");

/// Data type of the command (the first byte of the encoded command).
constexpr uint8_t COMMAND_DATA_TYPE{0x00};
/// Data type of the set param command (the first byte of the encoded command).
constexpr uint8_t SET_PARAM_DATA_TYPE{0x01};
/// Position of the Major version in the encoded command. The Minor version
/// follows it.
constexpr int COMMAND_VERSION_POS{1};
/// Position of the command or param ID in the encoded command.
constexpr int COMMAND_ID_POS{3};
/// Position of the command argument or param value in the encoded command.
constexpr int COMMAND_VALUE_POS{7};
/// Size of the encoded command and of the encoded set param command: data
/// type, version, ID and value. The size is fixed.
constexpr int COMMAND_SIZE{11};
static_assert(COMMAND_VALUE_POS + VALUE_SIZE == COMMAND_SIZE,
              "wrong size of encoded command");

/// The first and the last valid command ID (see LensCommand enum).
constexpr int FIRST_COMMAND_ID{
    static_cast<int>(cr::lens::LensCommand::ZOOM_TELE)};
constexpr int LAST_COMMAND_ID{
    static_cast<int>(cr::lens::LensCommand::DETECT_HW_RANGES)};
/// The first and the last valid param ID (see LensParam enum).
constexpr int FIRST_PARAM_ID{static_cast<int>(cr::lens::LensParam::ZOOM_POS)};
constexpr int LAST_PARAM_ID{static_cast<int>(cr::lens::LensParam::CUSTOM_3)};

/// Result of Lens::decodeCommand(...): command decoded.
constexpr int COMMAND_DECODED{0};
/// Result of Lens::decodeCommand(...): set param command decoded.
constexpr int SET_PARAM_DECODED{1};
/// Result of Lens::decodeCommand(...): error.
constexpr int DECODE_ERROR{-1};



/// Location of a LensParams field in the encoded data.
struct FieldRef
{
    /// Pointer to the field of a LensParams object.
    void* value{nullptr};
    /// Size of the field in the encoded data: VALUE_SIZE or FLAG_SIZE.
    int size{0};
};



/// Get the table of LensParams fields in the order they are stored in the
/// encoded data (the same order as the bits of the mask and the fields of
/// LensParamsMask).
std::array<FieldRef, FIELDS_NUMBER> makeFieldTable(cr::lens::LensParams& p)
{
    return {{
        {&p.zoomPos, VALUE_SIZE},
        {&p.zoomHwPos, VALUE_SIZE},
        {&p.focusPos, VALUE_SIZE},
        {&p.focusHwPos, VALUE_SIZE},
        {&p.irisPos, VALUE_SIZE},
        {&p.irisHwPos, VALUE_SIZE},
        {&p.focusMode, VALUE_SIZE},
        {&p.filterMode, VALUE_SIZE},
        {&p.afRoiX0, VALUE_SIZE},
        {&p.afRoiY0, VALUE_SIZE},
        {&p.afRoiX1, VALUE_SIZE},
        {&p.afRoiY1, VALUE_SIZE},
        {&p.zoomSpeed, VALUE_SIZE},
        {&p.zoomHwSpeed, VALUE_SIZE},
        {&p.zoomHwMaxSpeed, VALUE_SIZE},
        {&p.focusSpeed, VALUE_SIZE},
        {&p.focusHwSpeed, VALUE_SIZE},
        {&p.focusHwMaxSpeed, VALUE_SIZE},
        {&p.irisSpeed, VALUE_SIZE},
        {&p.irisHwSpeed, VALUE_SIZE},
        {&p.irisHwMaxSpeed, VALUE_SIZE},
        {&p.zoomHwTeleLimit, VALUE_SIZE},
        {&p.zoomHwWideLimit, VALUE_SIZE},
        {&p.focusHwFarLimit, VALUE_SIZE},
        {&p.focusHwNearLimit, VALUE_SIZE},
        {&p.irisHwOpenLimit, VALUE_SIZE},
        {&p.irisHwCloseLimit, VALUE_SIZE},
        {&p.focusFactor, VALUE_SIZE},
        {&p.isConnected, FLAG_SIZE},
        {&p.afHwSpeed, VALUE_SIZE},
        {&p.focusFactorThreshold, VALUE_SIZE},
        {&p.refocusTimeoutSec, VALUE_SIZE},
        {&p.afIsActive, FLAG_SIZE},
        {&p.irisMode, VALUE_SIZE},
        {&p.autoAfRoiWidth, VALUE_SIZE},
        {&p.autoAfRoiHeight, VALUE_SIZE},
        {&p.autoAfRoiBorder, VALUE_SIZE},
        {&p.afRoiMode, VALUE_SIZE},
        {&p.extenderMode, VALUE_SIZE},
        {&p.stabiliserMode, VALUE_SIZE},
        {&p.afRange, VALUE_SIZE},
        {&p.xFovDeg, VALUE_SIZE},
        {&p.yFovDeg, VALUE_SIZE},
        {&p.logMode, VALUE_SIZE},
        {&p.temperature, VALUE_SIZE},
        {&p.isOpen, FLAG_SIZE},
        {&p.type, VALUE_SIZE},
        {&p.custom1, VALUE_SIZE},
        {&p.custom2, VALUE_SIZE},
        {&p.custom3, VALUE_SIZE}
    }};
}



/// Get the table of LensParamsMask flags in the same order as the table of
/// LensParams fields.
std::array<const bool*, FIELDS_NUMBER> makeFlagTable(
        const cr::lens::LensParamsMask& m)
{
    return {{
        &m.zoomPos, &m.zoomHwPos, &m.focusPos, &m.focusHwPos, &m.irisPos,
        &m.irisHwPos, &m.focusMode, &m.filterMode, &m.afRoiX0, &m.afRoiY0,
        &m.afRoiX1, &m.afRoiY1, &m.zoomSpeed, &m.zoomHwSpeed,
        &m.zoomHwMaxSpeed, &m.focusSpeed, &m.focusHwSpeed, &m.focusHwMaxSpeed,
        &m.irisSpeed, &m.irisHwSpeed, &m.irisHwMaxSpeed, &m.zoomHwTeleLimit,
        &m.zoomHwWideLimit, &m.focusHwFarLimit, &m.focusHwNearLimit,
        &m.irisHwOpenLimit, &m.irisHwCloseLimit, &m.focusFactor,
        &m.isConnected, &m.afHwSpeed, &m.focusFactorThreshold,
        &m.refocusTimeoutSec, &m.afIsActive, &m.irisMode, &m.autoAfRoiWidth,
        &m.autoAfRoiHeight, &m.autoAfRoiBorder, &m.afRoiMode,
        &m.extenderMode, &m.stabiliserMode, &m.afRange, &m.xFovDeg,
        &m.yFovDeg, &m.logMode, &m.temperature, &m.isOpen, &m.type,
        &m.custom1, &m.custom2, &m.custom3
    }};
}



/// Get the index of the mask byte which contains the bit of a field.
int maskByteIndex(int fieldIndex)
{
    return fieldIndex / MASK_BYTE_BITS;
}



/// Get the bit of a field inside its mask byte. The first field of each mask
/// byte uses the most significant bit.
uint8_t maskBit(int fieldIndex)
{
    return static_cast<uint8_t>(
                MASK_FIRST_BIT >> (fieldIndex % MASK_BYTE_BITS));
}



/// Fill the header of an encoded command or set param command and the data
/// (ID and value). The buffer must be valid for COMMAND_SIZE bytes.
void encodeCommandData(
        uint8_t* data, uint8_t dataType, int id, float value)
{
    data[0] = dataType;
    data[COMMAND_VERSION_POS] = static_cast<uint8_t>(LENS_MAJOR_VERSION);
    data[COMMAND_VERSION_POS + 1] = static_cast<uint8_t>(LENS_MINOR_VERSION);
    std::memcpy(&data[COMMAND_ID_POS], &id, VALUE_SIZE);
    std::memcpy(&data[COMMAND_VALUE_POS], &value, VALUE_SIZE);
}
}



bool cr::lens::LensParams::encode(
        uint8_t* data, int bufferSize, int& size, LensParamsMask* mask)
{
    // The size is 0 unless the params are encoded.
    size = 0;

    // Check pointer and buffer size: the buffer must be able to keep all
    // params, regardless of the mask.
    if (data == nullptr || bufferSize < PARAMS_MAX_SIZE)
    {
        return false;
    }

    // Set mask: all params are encoded by default.
    const LensParamsMask defaultMask;
    const auto flags = makeFlagTable(mask == nullptr ? defaultMask : *mask);
    const auto fields = makeFieldTable(*this);

    // Encode header: data type, version, mask (set bit by bit below).
    data[0] = PARAMS_DATA_TYPE;
    data[PARAMS_VERSION_POS] = static_cast<uint8_t>(LENS_MAJOR_VERSION);
    data[PARAMS_VERSION_POS + 1] = static_cast<uint8_t>(LENS_MINOR_VERSION);
    std::memset(&data[PARAMS_MASK_POS], 0, PARAMS_MASK_SIZE);

    // Encode values of the selected params.
    int pos = PARAMS_HEADER_SIZE;
    for (int i = 0; i < FIELDS_NUMBER; ++i)
    {
        if (!*flags[static_cast<size_t>(i)])
        {
            continue;
        }

        uint8_t& maskByte = data[PARAMS_MASK_POS + maskByteIndex(i)];
        maskByte = static_cast<uint8_t>(maskByte | maskBit(i));

        const FieldRef& field = fields[static_cast<size_t>(i)];
        if (field.size == FLAG_SIZE)
        {
            data[pos] = *static_cast<const bool*>(field.value) ? 0x01 : 0x00;
        }
        else
        {
            std::memcpy(&data[pos], field.value, VALUE_SIZE);
        }
        pos += field.size;
    }

    size = pos;

    return true;
}



bool cr::lens::LensParams::decode(uint8_t* data, int dataSize)
{
    // Check pointer and data size (header must be complete).
    if (data == nullptr || dataSize < PARAMS_HEADER_SIZE)
    {
        return false;
    }

    // Check data type and version.
    if (data[0] != PARAMS_DATA_TYPE ||
        data[PARAMS_VERSION_POS] != LENS_MAJOR_VERSION ||
        data[PARAMS_VERSION_POS + 1] != LENS_MINOR_VERSION)
    {
        return false;
    }

    // Check that the mask has no bits of unknown fields.
    if ((data[PARAMS_MASK_POS + PARAMS_MASK_SIZE - 1] &
         PARAMS_RESERVED_MASK_BITS) != 0)
    {
        return false;
    }

    // Decode into a temporary object: this object is changed only if the
    // whole data is valid. The temporary object needs memory for the default
    // init string: an allocation failure is reported as a decoding failure.
    try
    {
        LensParams result;
        result.initString.clear();
        const auto fields = makeFieldTable(result);
        int pos = PARAMS_HEADER_SIZE;
        for (int i = 0; i < FIELDS_NUMBER; ++i)
        {
            const FieldRef& field = fields[static_cast<size_t>(i)];
            const bool encoded =
                    (data[PARAMS_MASK_POS + maskByteIndex(i)] &
                     maskBit(i)) != 0;
            if (!encoded)
            {
                // Params which are not encoded are zero.
                std::memset(field.value, 0, static_cast<size_t>(field.size));
                continue;
            }

            // Check that the value is inside the data. The position never
            // exceeds the data size.
            if (dataSize - pos < field.size)
            {
                return false;
            }
            if (field.size == FLAG_SIZE)
            {
                *static_cast<bool*>(field.value) = data[pos] != 0x00;
            }
            else
            {
                std::memcpy(field.value, &data[pos], VALUE_SIZE);
            }
            pos += field.size;
        }

        // The move assignment doesn't throw.
        *this = std::move(result);
    }
    catch (const std::bad_alloc&)
    {
        return false;
    }

    return true;
}



cr::lens::Lens::~Lens() = default;



std::string cr::lens::Lens::getVersion()
{
    return LENS_VERSION;
}



void cr::lens::Lens::encodeSetParamCommand(
        uint8_t* data, int& size, LensParam id, float value)
{
    // The size is 0 unless the command is encoded.
    size = 0;
    if (data == nullptr)
    {
        return;
    }

    encodeCommandData(data, SET_PARAM_DATA_TYPE, static_cast<int>(id), value);
    size = COMMAND_SIZE;
}



void cr::lens::Lens::encodeCommand(
        uint8_t* data, int& size, LensCommand id, float arg)
{
    // The size is 0 unless the command is encoded.
    size = 0;
    if (data == nullptr)
    {
        return;
    }

    encodeCommandData(data, COMMAND_DATA_TYPE, static_cast<int>(id), arg);
    size = COMMAND_SIZE;
}



int cr::lens::Lens::decodeCommand(uint8_t* data,
                                  int size,
                                  LensParam& paramId,
                                  LensCommand& commandId,
                                  float& value)
{
    // Check pointer and size: the size of the command is fixed.
    if (data == nullptr || size != COMMAND_SIZE)
    {
        return DECODE_ERROR;
    }

    // Check version.
    if (data[COMMAND_VERSION_POS] != LENS_MAJOR_VERSION ||
        data[COMMAND_VERSION_POS + 1] != LENS_MINOR_VERSION)
    {
        return DECODE_ERROR;
    }

    // Extract data. Output arguments are not changed until the ID is checked.
    int id = 0;
    float decodedValue = 0.0f;
    std::memcpy(&id, &data[COMMAND_ID_POS], VALUE_SIZE);
    std::memcpy(&decodedValue, &data[COMMAND_VALUE_POS], VALUE_SIZE);

    // Check command type and ID. An ID outside of the enum range must not be
    // passed to the lens controller implementation.
    if (data[0] == COMMAND_DATA_TYPE)
    {
        if (id < FIRST_COMMAND_ID || id > LAST_COMMAND_ID)
        {
            return DECODE_ERROR;
        }
        commandId = static_cast<LensCommand>(id);
        value = decodedValue;
        return COMMAND_DECODED;
    }
    if (data[0] == SET_PARAM_DATA_TYPE)
    {
        if (id < FIRST_PARAM_ID || id > LAST_PARAM_ID)
        {
            return DECODE_ERROR;
        }
        paramId = static_cast<LensParam>(id);
        value = decodedValue;
        return SET_PARAM_DECODED;
    }

    return DECODE_ERROR;
}
