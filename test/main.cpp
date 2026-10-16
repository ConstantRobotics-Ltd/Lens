#include <array>
#include <climits>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <initializer_list>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include "ConfigReader.h"
#include "Lens.h"
#include "LensVersion.h"



namespace
{
/// Size of the encoded params with all fields, bytes.
constexpr int MAX_ENCODED_SIZE{201};
/// Size of the encoded params header (data type, version, mask), bytes.
constexpr int HEADER_SIZE{10};
/// Size of the encoded command, bytes.
constexpr int COMMAND_SIZE{11};
/// Position of the params mask in the encoded params.
constexpr int MASK_POS{3};
/// Size of the buffers used by the tests, bytes.
constexpr int BUFFER_SIZE{512};
/// The first and the last IDs of the commands and of the params.
constexpr int FIRST_COMMAND{static_cast<int>(cr::lens::LensCommand::ZOOM_TELE)};
constexpr int LAST_COMMAND{
    static_cast<int>(cr::lens::LensCommand::DETECT_HW_RANGES)};
constexpr int FIRST_PARAM{static_cast<int>(cr::lens::LensParam::ZOOM_POS)};
constexpr int LAST_PARAM{static_cast<int>(cr::lens::LensParam::CUSTOM_3)};
/// Number of the random masks in the encode / decode tests.
constexpr int RANDOM_MASKS_NUMBER{3000};
/// Number of the damaged samples in the damaged data test.
constexpr int DAMAGED_SAMPLES_NUMBER{30000};
/// Name of the file which is written by the JSON test.
const char* const JSON_FILE_NAME{"LensTestParams.json"};
static_assert(sizeof(int) == 4, "int must be 4 bytes");
static_assert(sizeof(float) == 4, "float must be 4 bytes");



/// State of the test random generator. The tests use own deterministic
/// generator: results must be reproducible on all platforms.
uint32_t g_randomState{20240915U};



/// Get next random number.
uint32_t nextRandom()
{
    g_randomState = g_randomState * 1664525U + 1013904223U;
    return g_randomState;
}



/// Get a random 32 bits pattern (the low bits of the generator are weak).
uint32_t randomBits()
{
    return (nextRandom() & 0xFFFF0000U) | (nextRandom() >> 16);
}



/// Get next random number in [0, limit).
int randomInt(int limit)
{
    return static_cast<int>((nextRandom() >> 8) % static_cast<uint32_t>(limit));
}



/// Get the bits of an int or float value.
template <class T>
uint32_t bitsOf(T value)
{
    static_assert(sizeof(T) == sizeof(uint32_t), "wrong type size");
    uint32_t bits{0};
    std::memcpy(&bits, &value, sizeof(bits));
    return bits;
}



/// Kind of a LensParams field.
enum class FieldKind
{
    INT,
    FLOAT,
    FLAG
};



/// Description of a LensParams field and of the flag of LensParamsMask which
/// selects it for encoding.
struct FieldDesc
{
    /// Name of the field.
    const char* name{""};
    /// Kind of the field.
    FieldKind kind{FieldKind::INT};
    /// Pointer to the int field.
    int cr::lens::LensParams::* intField{nullptr};
    /// Pointer to the float field.
    float cr::lens::LensParams::* floatField{nullptr};
    /// Pointer to the bool field.
    bool cr::lens::LensParams::* flagField{nullptr};
    /// Pointer to the flag of the mask.
    bool cr::lens::LensParamsMask::* maskFlag{nullptr};
};



/// Describe an int field.
FieldDesc intField(const char* name, int cr::lens::LensParams::* field,
                   bool cr::lens::LensParamsMask::* flag)
{
    FieldDesc desc;
    desc.name = name;
    desc.kind = FieldKind::INT;
    desc.intField = field;
    desc.maskFlag = flag;
    return desc;
}



/// Describe a float field.
FieldDesc floatField(const char* name, float cr::lens::LensParams::* field,
                     bool cr::lens::LensParamsMask::* flag)
{
    FieldDesc desc;
    desc.name = name;
    desc.kind = FieldKind::FLOAT;
    desc.floatField = field;
    desc.maskFlag = flag;
    return desc;
}



/// Describe a bool field.
FieldDesc flagField(const char* name, bool cr::lens::LensParams::* field,
                    bool cr::lens::LensParamsMask::* flag)
{
    FieldDesc desc;
    desc.name = name;
    desc.kind = FieldKind::FLAG;
    desc.flagField = field;
    desc.maskFlag = flag;
    return desc;
}



/// Get the fields in the order they must be stored in the encoded data. The
/// order is the part of the data format: the n-th field uses the bit
/// (0x80 >> (n % 8)) of the mask byte n / 8.
const std::array<FieldDesc, 50>& fields()
{
    using cr::lens::LensParams;
    using cr::lens::LensParamsMask;
    static const std::array<FieldDesc, 50> table{{
        intField("zoomPos", &LensParams::zoomPos, &LensParamsMask::zoomPos),
        intField("zoomHwPos", &LensParams::zoomHwPos,
                 &LensParamsMask::zoomHwPos),
        intField("focusPos", &LensParams::focusPos, &LensParamsMask::focusPos),
        intField("focusHwPos", &LensParams::focusHwPos,
                 &LensParamsMask::focusHwPos),
        intField("irisPos", &LensParams::irisPos, &LensParamsMask::irisPos),
        intField("irisHwPos", &LensParams::irisHwPos,
                 &LensParamsMask::irisHwPos),
        intField("focusMode", &LensParams::focusMode,
                 &LensParamsMask::focusMode),
        intField("filterMode", &LensParams::filterMode,
                 &LensParamsMask::filterMode),
        intField("afRoiX0", &LensParams::afRoiX0, &LensParamsMask::afRoiX0),
        intField("afRoiY0", &LensParams::afRoiY0, &LensParamsMask::afRoiY0),
        intField("afRoiX1", &LensParams::afRoiX1, &LensParamsMask::afRoiX1),
        intField("afRoiY1", &LensParams::afRoiY1, &LensParamsMask::afRoiY1),
        intField("zoomSpeed", &LensParams::zoomSpeed,
                 &LensParamsMask::zoomSpeed),
        intField("zoomHwSpeed", &LensParams::zoomHwSpeed,
                 &LensParamsMask::zoomHwSpeed),
        intField("zoomHwMaxSpeed", &LensParams::zoomHwMaxSpeed,
                 &LensParamsMask::zoomHwMaxSpeed),
        intField("focusSpeed", &LensParams::focusSpeed,
                 &LensParamsMask::focusSpeed),
        intField("focusHwSpeed", &LensParams::focusHwSpeed,
                 &LensParamsMask::focusHwSpeed),
        intField("focusHwMaxSpeed", &LensParams::focusHwMaxSpeed,
                 &LensParamsMask::focusHwMaxSpeed),
        intField("irisSpeed", &LensParams::irisSpeed,
                 &LensParamsMask::irisSpeed),
        intField("irisHwSpeed", &LensParams::irisHwSpeed,
                 &LensParamsMask::irisHwSpeed),
        intField("irisHwMaxSpeed", &LensParams::irisHwMaxSpeed,
                 &LensParamsMask::irisHwMaxSpeed),
        intField("zoomHwTeleLimit", &LensParams::zoomHwTeleLimit,
                 &LensParamsMask::zoomHwTeleLimit),
        intField("zoomHwWideLimit", &LensParams::zoomHwWideLimit,
                 &LensParamsMask::zoomHwWideLimit),
        intField("focusHwFarLimit", &LensParams::focusHwFarLimit,
                 &LensParamsMask::focusHwFarLimit),
        intField("focusHwNearLimit", &LensParams::focusHwNearLimit,
                 &LensParamsMask::focusHwNearLimit),
        intField("irisHwOpenLimit", &LensParams::irisHwOpenLimit,
                 &LensParamsMask::irisHwOpenLimit),
        intField("irisHwCloseLimit", &LensParams::irisHwCloseLimit,
                 &LensParamsMask::irisHwCloseLimit),
        floatField("focusFactor", &LensParams::focusFactor,
                   &LensParamsMask::focusFactor),
        flagField("isConnected", &LensParams::isConnected,
                  &LensParamsMask::isConnected),
        intField("afHwSpeed", &LensParams::afHwSpeed,
                 &LensParamsMask::afHwSpeed),
        floatField("focusFactorThreshold", &LensParams::focusFactorThreshold,
                   &LensParamsMask::focusFactorThreshold),
        intField("refocusTimeoutSec", &LensParams::refocusTimeoutSec,
                 &LensParamsMask::refocusTimeoutSec),
        flagField("afIsActive", &LensParams::afIsActive,
                  &LensParamsMask::afIsActive),
        intField("irisMode", &LensParams::irisMode, &LensParamsMask::irisMode),
        intField("autoAfRoiWidth", &LensParams::autoAfRoiWidth,
                 &LensParamsMask::autoAfRoiWidth),
        intField("autoAfRoiHeight", &LensParams::autoAfRoiHeight,
                 &LensParamsMask::autoAfRoiHeight),
        intField("autoAfRoiBorder", &LensParams::autoAfRoiBorder,
                 &LensParamsMask::autoAfRoiBorder),
        intField("afRoiMode", &LensParams::afRoiMode,
                 &LensParamsMask::afRoiMode),
        intField("extenderMode", &LensParams::extenderMode,
                 &LensParamsMask::extenderMode),
        intField("stabiliserMode", &LensParams::stabiliserMode,
                 &LensParamsMask::stabiliserMode),
        intField("afRange", &LensParams::afRange, &LensParamsMask::afRange),
        floatField("xFovDeg", &LensParams::xFovDeg, &LensParamsMask::xFovDeg),
        floatField("yFovDeg", &LensParams::yFovDeg, &LensParamsMask::yFovDeg),
        intField("logMode", &LensParams::logMode, &LensParamsMask::logMode),
        floatField("temperature", &LensParams::temperature,
                   &LensParamsMask::temperature),
        flagField("isOpen", &LensParams::isOpen, &LensParamsMask::isOpen),
        intField("type", &LensParams::type, &LensParamsMask::type),
        floatField("custom1", &LensParams::custom1, &LensParamsMask::custom1),
        floatField("custom2", &LensParams::custom2, &LensParamsMask::custom2),
        floatField("custom3", &LensParams::custom3, &LensParamsMask::custom3)
    }};
    return table;
}



/// Number of the fields.
constexpr int FIELDS_NUMBER{50};



/// Get the bits of a field: the bits of an int or float value, 0 or 1 for a
/// bool value.
uint32_t fieldBits(const cr::lens::LensParams& p, const FieldDesc& desc)
{
    switch (desc.kind)
    {
    case FieldKind::INT:
        return bitsOf(p.*(desc.intField));
    case FieldKind::FLOAT:
        return bitsOf(p.*(desc.floatField));
    case FieldKind::FLAG:
        return (p.*(desc.flagField)) ? 1U : 0U;
    }
    return 0U;
}



/// Set the bits of a field.
void setFieldBits(cr::lens::LensParams& p, const FieldDesc& desc, uint32_t bits)
{
    switch (desc.kind)
    {
    case FieldKind::INT:
        std::memcpy(&(p.*(desc.intField)), &bits, sizeof(bits));
        break;
    case FieldKind::FLOAT:
        std::memcpy(&(p.*(desc.floatField)), &bits, sizeof(bits));
        break;
    case FieldKind::FLAG:
        p.*(desc.flagField) = (bits & 1U) != 0U;
        break;
    }
}



/// Get size of a field in the encoded data, bytes.
int fieldSize(const FieldDesc& desc)
{
    return desc.kind == FieldKind::FLAG ? 1 : 4;
}



/// Get size of the encoded params for the mask, bytes.
int encodedSize(const cr::lens::LensParamsMask& mask)
{
    int size = HEADER_SIZE;
    for (const FieldDesc& desc : fields())
    {
        if (mask.*(desc.maskFlag))
        {
            size += fieldSize(desc);
        }
    }
    return size;
}



/// Get the position of a field in the encoded data. The field must be selected
/// by the mask.
int fieldPos(const cr::lens::LensParamsMask& mask, int index)
{
    int pos = HEADER_SIZE;
    for (int i = 0; i < index; ++i)
    {
        const FieldDesc& desc = fields()[static_cast<size_t>(i)];
        if (mask.*(desc.maskFlag))
        {
            pos += fieldSize(desc);
        }
    }
    return pos;
}



/// Set all flags of the mask.
void setMask(cr::lens::LensParamsMask& mask, bool value)
{
    for (const FieldDesc& desc : fields())
    {
        mask.*(desc.maskFlag) = value;
    }
}



/// Set random flags of the mask. The flag is set with the probability 1/2
/// (mode 0) or 1/8 (mode 1).
void randomMask(cr::lens::LensParamsMask& mask, int mode)
{
    for (const FieldDesc& desc : fields())
    {
        mask.*(desc.maskFlag) = randomInt(mode == 0 ? 2 : 8) == 0;
    }
}



/// Get the mask which is stored in the header of the encoded params.
cr::lens::LensParamsMask maskFromData(const uint8_t* data)
{
    cr::lens::LensParamsMask mask;
    for (int i = 0; i < FIELDS_NUMBER; ++i)
    {
        const int byte = data[MASK_POS + i / 8];
        mask.*(fields()[static_cast<size_t>(i)].maskFlag) =
                (byte & (0x80 >> (i % 8))) != 0;
    }
    return mask;
}



/// Fill all fields with random bit patterns (including NaN, infinity and
/// denormal floats), the init string and the FOV points.
void randomizeParams(cr::lens::LensParams& p)
{
    for (const FieldDesc& desc : fields())
    {
        setFieldBits(p, desc, randomBits());
    }
    p.initString = "init" + std::to_string(randomInt(1000));
    p.fovPoints.clear();
    const int points = randomInt(5);
    for (int i = 0; i < points; ++i)
    {
        cr::lens::FovPoint point;
        point.hwZoomPos = static_cast<int>(randomBits());
        point.xFovDeg = static_cast<float>(randomInt(180));
        point.yFovDeg = static_cast<float>(randomInt(180));
        p.fovPoints.push_back(point);
    }
}



/// Fill the fields with random small integer values: they are written to a
/// JSON file and read back exactly.
void randomizeParamsForJson(cr::lens::LensParams& p)
{
    for (const FieldDesc& desc : fields())
    {
        if (desc.kind == FieldKind::INT)
        {
            p.*(desc.intField) = randomInt(255);
        }
        else if (desc.kind == FieldKind::FLOAT)
        {
            p.*(desc.floatField) = static_cast<float>(randomInt(255));
        }
        else
        {
            p.*(desc.flagField) = randomInt(2) == 1;
        }
    }
    p.initString = "init" + std::to_string(randomInt(1000));
    p.fovPoints.clear();
    for (int i = 0; i < 5; ++i)
    {
        cr::lens::FovPoint point;
        point.hwZoomPos = randomInt(255);
        point.xFovDeg = static_cast<float>(randomInt(255));
        point.yFovDeg = static_cast<float>(randomInt(255));
        p.fovPoints.push_back(point);
    }
}



/// Compare the fields of two params objects (by bits), the init string and the
/// FOV points. Prints the name of each different field.
bool sameParams(const cr::lens::LensParams& a, const cr::lens::LensParams& b)
{
    bool same = true;
    for (const FieldDesc& desc : fields())
    {
        if (fieldBits(a, desc) != fieldBits(b, desc))
        {
            std::cout << "Mismatch in field " << desc.name << std::endl;
            same = false;
        }
    }
    if (a.initString != b.initString)
    {
        std::cout << "Mismatch in field initString" << std::endl;
        same = false;
    }
    if (a.fovPoints.size() != b.fovPoints.size())
    {
        std::cout << "Mismatch in size of fovPoints" << std::endl;
        return false;
    }
    for (size_t i = 0; i < a.fovPoints.size(); ++i)
    {
        if (a.fovPoints[i].hwZoomPos != b.fovPoints[i].hwZoomPos ||
            bitsOf(a.fovPoints[i].xFovDeg) != bitsOf(b.fovPoints[i].xFovDeg) ||
            bitsOf(a.fovPoints[i].yFovDeg) != bitsOf(b.fovPoints[i].yFovDeg))
        {
            std::cout << "Mismatch in fovPoints[" << i << "]" << std::endl;
            same = false;
        }
    }
    return same;
}



/// Check the result of decoding: the fields selected by the mask are equal to
/// the input, other fields are zero, the init string is empty and there are no
/// FOV points.
bool checkDecoded(const cr::lens::LensParams& in,
                  const cr::lens::LensParams& out,
                  const cr::lens::LensParamsMask& mask)
{
    bool ok = true;
    for (const FieldDesc& desc : fields())
    {
        const uint32_t expected =
                (mask.*(desc.maskFlag)) ? fieldBits(in, desc) : 0U;
        if (expected != fieldBits(out, desc))
        {
            std::cout << "Mismatch in field " << desc.name << std::endl;
            ok = false;
        }
    }
    if (!out.initString.empty() || !out.fovPoints.empty())
    {
        std::cout << "initString or fovPoints are not empty" << std::endl;
        ok = false;
    }
    return ok;
}



/// Print a failed check and return false.
bool fail(const char* message)
{
    std::cout << message << std::endl;
    return false;
}



/// Encode params with the mask into the buffer and return the size or -1.
int encodeParams(cr::lens::LensParams& p, uint8_t* buffer,
                 cr::lens::LensParamsMask* mask)
{
    int size = 0;
    if (!p.encode(buffer, BUFFER_SIZE, size, mask))
    {
        return -1;
    }
    return size;
}



/// Copy test: copy and move construction and assignment.
bool copyTest()
{
    cr::lens::LensParams in;
    randomizeParams(in);

    const cr::lens::LensParams copied(in);
    if (!sameParams(in, copied))
    {
        return fail("Copy constructor");
    }
    cr::lens::LensParams assigned;
    assigned = in;
    if (!sameParams(in, assigned))
    {
        return fail("Copy assignment");
    }
    cr::lens::LensParams source(in);
    const cr::lens::LensParams moved(std::move(source));
    if (!sameParams(in, moved))
    {
        return fail("Move constructor");
    }
    cr::lens::LensParams source2(in);
    cr::lens::LensParams moveAssigned;
    moveAssigned = std::move(source2);
    if (!sameParams(in, moveAssigned))
    {
        return fail("Move assignment");
    }
    const cr::lens::LensParams& self = assigned;
    assigned = self;
    if (!sameParams(in, assigned))
    {
        return fail("Self assignment");
    }

    // The copy is independent of the source.
    assigned.fovPoints.push_back(cr::lens::FovPoint());
    if (assigned.fovPoints.size() == in.fovPoints.size())
    {
        return fail("The copy shares FOV points with the source");
    }

    cr::lens::FovPoint point;
    point.hwZoomPos = 11;
    point.xFovDeg = 22.0f;
    point.yFovDeg = 33.0f;
    const cr::lens::FovPoint pointCopy(point);
    cr::lens::FovPoint pointAssigned;
    pointAssigned = point;
    if (pointCopy.hwZoomPos != 11 || pointAssigned.yFovDeg != 33.0f)
    {
        return fail("FovPoint copy");
    }
    return true;
}



/// Encode / decode params with all fields.
bool encodeDecodeParamsTest()
{
    cr::lens::LensParams in;
    randomizeParams(in);

    uint8_t data[BUFFER_SIZE];
    const int size = encodeParams(in, data, nullptr);
    std::cout << "Encoded data size: " << size << " bytes" << std::endl;
    if (size != MAX_ENCODED_SIZE)
    {
        return fail("Wrong size of encoded data");
    }

    // The same bytes are produced with the default mask.
    cr::lens::LensParamsMask mask;
    uint8_t data2[BUFFER_SIZE];
    const int size2 = encodeParams(in, data2, &mask);
    if (size2 != size ||
        std::memcmp(data, data2, static_cast<size_t>(size)) != 0)
    {
        return fail("Default mask and nullptr mask give different data");
    }

    // The header: data type, version.
    if (data[0] != 0x02 || data[1] != LENS_MAJOR_VERSION ||
        data[2] != LENS_MINOR_VERSION)
    {
        return fail("Wrong header");
    }

    // Decode into an object with other values.
    cr::lens::LensParams out;
    randomizeParams(out);
    if (!out.decode(data, size))
    {
        return fail("Can't decode data");
    }
    return checkDecoded(in, out, mask);
}



/// Encode / decode params with random masks.
bool encodeDecodeMaskTest()
{
    bool ok = true;
    for (int n = 0; n < RANDOM_MASKS_NUMBER; ++n)
    {
        cr::lens::LensParams in;
        randomizeParams(in);
        cr::lens::LensParamsMask mask;
        if (n == 0)
        {
            setMask(mask, false);
        }
        else
        {
            randomMask(mask, n % 2);
        }

        // Guard bytes after the encoded data must stay untouched.
        uint8_t data[BUFFER_SIZE];
        std::memset(data, 0xA5, sizeof(data));
        const int size = encodeParams(in, data, &mask);
        if (size != encodedSize(mask))
        {
            return fail("Wrong size of encoded data");
        }
        for (int i = size; i < BUFFER_SIZE; ++i)
        {
            if (data[i] != 0xA5)
            {
                return fail("Data written after the end of encoded data");
            }
        }

        cr::lens::LensParams out;
        randomizeParams(out);
        if (!out.decode(data, size))
        {
            return fail("Can't decode data");
        }
        if (!checkDecoded(in, out, mask))
        {
            ok = false;
            break;
        }
    }
    return ok;
}



/// Encode params with a single selected field for each field: checks the
/// position of the mask bit, the size and the decoded value.
bool singleFieldMaskTest()
{
    bool ok = true;
    for (int i = 0; i < FIELDS_NUMBER; ++i)
    {
        const FieldDesc& desc = fields()[static_cast<size_t>(i)];
        cr::lens::LensParams in;
        randomizeParams(in);
        cr::lens::LensParamsMask mask;
        setMask(mask, false);
        mask.*(desc.maskFlag) = true;

        uint8_t data[BUFFER_SIZE];
        const int size = encodeParams(in, data, &mask);
        if (size != HEADER_SIZE + fieldSize(desc))
        {
            std::cout << "Wrong size for field " << desc.name << std::endl;
            ok = false;
            continue;
        }

        // Only one bit of the mask is set.
        for (int byte = 0; byte < 7; ++byte)
        {
            const int expected = byte == i / 8 ? (0x80 >> (i % 8)) : 0;
            if (data[MASK_POS + byte] != expected)
            {
                std::cout << "Wrong mask for field " << desc.name << std::endl;
                ok = false;
            }
        }

        // The value is stored right after the header.
        const uint32_t bits = fieldBits(in, desc);
        uint32_t stored = 0;
        if (desc.kind == FieldKind::FLAG)
        {
            stored = data[HEADER_SIZE];
        }
        else
        {
            std::memcpy(&stored, &data[HEADER_SIZE], sizeof(stored));
        }
        if (stored != bits)
        {
            std::cout << "Wrong value for field " << desc.name << std::endl;
            ok = false;
        }

        cr::lens::LensParams out;
        if (!out.decode(data, size) || !checkDecoded(in, out, mask))
        {
            std::cout << "Wrong decoding of field " << desc.name << std::endl;
            ok = false;
        }
    }
    return ok;
}



/// Buffer boundaries of encode(...).
bool encodeBoundariesTest()
{
    cr::lens::LensParams in;
    randomizeParams(in);

    // Null pointer: the size is set to 0.
    int size = 123;
    if (in.encode(nullptr, BUFFER_SIZE, size) || size != 0)
    {
        return fail("nullptr data");
    }

    // The buffer is too small: nothing is written, the size is 0.
    const int smallSizes[] = {INT_MIN, -1, 0, 1, 10, 100, 200};
    for (const int bufferSize : smallSizes)
    {
        uint8_t data[BUFFER_SIZE];
        std::memset(data, 0x5A, sizeof(data));
        size = 123;
        if (in.encode(data, bufferSize, size) || size != 0)
        {
            std::cout << "bufferSize " << bufferSize << std::endl;
            return fail("Encoded into a small buffer");
        }
        for (const uint8_t byte : data)
        {
            if (byte != 0x5A)
            {
                return fail("Data written after a rejected call");
            }
        }
    }

    // The exact size: all bytes are written, nothing after them.
    uint8_t exact[MAX_ENCODED_SIZE + 1];
    std::memset(exact, 0x5A, sizeof(exact));
    size = 0;
    if (!in.encode(exact, MAX_ENCODED_SIZE, size) || size != MAX_ENCODED_SIZE)
    {
        return fail("Exact buffer size");
    }
    if (exact[MAX_ENCODED_SIZE] != 0x5A)
    {
        return fail("Data written after the end of the buffer");
    }

    // A smaller mask also needs the buffer for all params.
    cr::lens::LensParamsMask mask;
    setMask(mask, false);
    uint8_t data[BUFFER_SIZE];
    size = 123;
    if (in.encode(data, MAX_ENCODED_SIZE - 1, size, &mask) || size != 0)
    {
        return fail("Small buffer with a small mask");
    }
    return true;
}



/// Decoding of invalid data: nothing is changed in the object.
bool decodeInvalidDataTest()
{
    cr::lens::LensParams in;
    randomizeParams(in);
    uint8_t data[BUFFER_SIZE];
    std::memset(data, 0, sizeof(data));
    const int size = encodeParams(in, data, nullptr);
    const cr::lens::LensParamsMask all;

    cr::lens::LensParams target;
    randomizeParams(target);
    const cr::lens::LensParams snapshot(target);
    bool ok = true;

    // A rejected decode: the result is false and the object is not changed.
    auto rejected = [&](const uint8_t* bytes, int bytesSize, const char* what)
    {
        uint8_t copy[BUFFER_SIZE];
        std::memcpy(copy, bytes, sizeof(copy));
        if (target.decode(copy, bytesSize))
        {
            std::cout << what << " (size " << bytesSize << "): accepted"
                      << std::endl;
            return false;
        }
        if (!sameParams(target, snapshot))
        {
            std::cout << what << " (size " << bytesSize << "): object changed"
                      << std::endl;
            return false;
        }
        return true;
    };

    // Null pointer and wrong sizes.
    if (target.decode(nullptr, size) || !sameParams(target, snapshot))
    {
        ok = fail("nullptr data");
    }
    const int badSizes[] = {INT_MIN, -1, 0, 1, 9};
    for (const int badSize : badSizes)
    {
        ok = rejected(data, badSize, "Wrong size") && ok;
    }

    // Every truncation of valid data.
    for (int length = 0; length < size; ++length)
    {
        ok = rejected(data, length, "Truncated data") && ok;
    }

    // Every wrong value of the data type and of the version bytes.
    const int positions[] = {0, 1, 2};
    const uint8_t valid[] = {0x02, static_cast<uint8_t>(LENS_MAJOR_VERSION),
                             static_cast<uint8_t>(LENS_MINOR_VERSION)};
    for (const int pos : positions)
    {
        for (int value = 0; value < 256; ++value)
        {
            if (value == valid[pos])
            {
                continue;
            }
            uint8_t bad[BUFFER_SIZE];
            std::memcpy(bad, data, sizeof(bad));
            bad[pos] = static_cast<uint8_t>(value);
            ok = rejected(bad, size, "Wrong header byte") && ok;
        }
    }

    // Reserved bits of the mask (they don't belong to any field).
    for (int bit = 0; bit < 6; ++bit)
    {
        uint8_t bad[BUFFER_SIZE];
        std::memcpy(bad, data, sizeof(bad));
        bad[MASK_POS + 6] = static_cast<uint8_t>(bad[MASK_POS + 6] |
                                                 (1 << bit));
        ok = rejected(bad, size, "Reserved mask bit") && ok;
    }

    // Trailing bytes are ignored.
    for (int extra = 1; extra <= 8; ++extra)
    {
        cr::lens::LensParams out;
        if (!out.decode(data, size + extra) || !checkDecoded(in, out, all))
        {
            ok = fail("Data with trailing bytes");
        }
    }

    // A bool value is TRUE for any non zero byte and FALSE for zero.
    for (int index = 0; index < FIELDS_NUMBER; ++index)
    {
        const FieldDesc& desc = fields()[static_cast<size_t>(index)];
        if (desc.kind != FieldKind::FLAG)
        {
            continue;
        }
        for (const int value : {0, 1, 2, 0x7F, 0xFF})
        {
            uint8_t flagData[BUFFER_SIZE];
            std::memcpy(flagData, data, sizeof(flagData));
            flagData[fieldPos(all, index)] = static_cast<uint8_t>(value);
            cr::lens::LensParams out;
            const uint32_t expected = value != 0 ? 1U : 0U;
            if (!out.decode(flagData, size) ||
                fieldBits(out, desc) != expected)
            {
                std::cout << "Bool value " << value << " of " << desc.name
                          << std::endl;
                ok = false;
            }
        }
    }
    return ok;
}



/// Decoding of damaged data: deterministic result, no crash, a decoded object
/// can be encoded and decoded again.
bool damagedDataTest()
{
    int accepted = 0;
    bool ok = true;
    for (int n = 0; n < DAMAGED_SAMPLES_NUMBER && ok; ++n)
    {
        cr::lens::LensParams in;
        randomizeParams(in);
        cr::lens::LensParamsMask mask;
        randomMask(mask, n % 2);
        uint8_t data[BUFFER_SIZE];
        std::memset(data, 0, sizeof(data));
        const int size = encodeParams(in, data, &mask);

        // Replace a few bytes and change the size.
        const int changes = 1 + randomInt(6);
        for (int i = 0; i < changes; ++i)
        {
            data[randomInt(size)] = static_cast<uint8_t>(randomInt(256));
        }
        const int damagedSize = randomInt(size + 4);

        // Two objects with the same state must give the same result.
        cr::lens::LensParams first;
        randomizeParams(first);
        cr::lens::LensParams second(first);
        const cr::lens::LensParams snapshot(first);
        const bool firstResult = first.decode(data, damagedSize);
        const bool secondResult = second.decode(data, damagedSize);
        if (firstResult != secondResult || !sameParams(first, second))
        {
            ok = fail("Not deterministic result");
            break;
        }
        if (!firstResult)
        {
            if (!sameParams(first, snapshot))
            {
                ok = fail("Object changed after a rejected call");
            }
            continue;
        }
        ++accepted;

        // The decoded object is encoded and decoded again without changes.
        cr::lens::LensParamsMask decodedMask = maskFromData(data);
        uint8_t again[BUFFER_SIZE];
        const int againSize = encodeParams(first, again, &decodedMask);
        cr::lens::LensParams third;
        if (againSize != encodedSize(decodedMask) ||
            !third.decode(again, againSize) || !sameParams(first, third))
        {
            ok = fail("Decoded object is not encoded back");
        }
    }
    std::cout << "Damaged samples: " << DAMAGED_SAMPLES_NUMBER
              << ", accepted: " << accepted << std::endl;
    return ok;
}



/// Encode / decode commands and set param commands.
bool commandsTest()
{
    bool ok = true;
    for (int id = FIRST_COMMAND; id <= LAST_COMMAND; ++id)
    {
        const uint32_t argBits = randomBits();
        float arg = 0.0f;
        std::memcpy(&arg, &argBits, sizeof(arg));

        uint8_t data[BUFFER_SIZE];
        int size = 0;
        cr::lens::Lens::encodeCommand(
                    data, size, static_cast<cr::lens::LensCommand>(id), arg);
        if (size != COMMAND_SIZE || data[0] != 0x00 ||
            data[1] != LENS_MAJOR_VERSION || data[2] != LENS_MINOR_VERSION)
        {
            ok = fail("Wrong encoded command");
            continue;
        }

        cr::lens::LensParam paramId{cr::lens::LensParam::CUSTOM_3};
        cr::lens::LensCommand commandId{cr::lens::LensCommand::RESTART};
        float value = 0.0f;
        if (cr::lens::Lens::decodeCommand(
                    data, size, paramId, commandId, value) != 0 ||
            static_cast<int>(commandId) != id || bitsOf(value) != argBits ||
            paramId != cr::lens::LensParam::CUSTOM_3)
        {
            std::cout << "Command " << id << std::endl;
            ok = fail("Command is not decoded");
        }
    }

    for (int id = FIRST_PARAM; id <= LAST_PARAM; ++id)
    {
        const uint32_t valueBits = randomBits();
        float outValue = 0.0f;
        std::memcpy(&outValue, &valueBits, sizeof(outValue));

        uint8_t data[BUFFER_SIZE];
        int size = 0;
        cr::lens::Lens::encodeSetParamCommand(
                    data, size, static_cast<cr::lens::LensParam>(id), outValue);
        if (size != COMMAND_SIZE || data[0] != 0x01)
        {
            ok = fail("Wrong encoded set param command");
            continue;
        }

        cr::lens::LensParam paramId{cr::lens::LensParam::ZOOM_POS};
        cr::lens::LensCommand commandId{cr::lens::LensCommand::RESTART};
        float value = 0.0f;
        if (cr::lens::Lens::decodeCommand(
                    data, size, paramId, commandId, value) != 1 ||
            static_cast<int>(paramId) != id || bitsOf(value) != valueBits ||
            commandId != cr::lens::LensCommand::RESTART)
        {
            std::cout << "Param " << id << std::endl;
            ok = fail("Set param command is not decoded");
        }
    }
    return ok;
}



/// Decoding of invalid commands: the result is -1, output arguments are not
/// changed.
bool invalidCommandsTest()
{
    uint8_t valid[BUFFER_SIZE];
    std::memset(valid, 0, sizeof(valid));
    int size = 0;
    cr::lens::Lens::encodeCommand(
                valid, size, cr::lens::LensCommand::ZOOM_TO_POS, 1.5f);
    uint8_t validParam[BUFFER_SIZE];
    std::memset(validParam, 0, sizeof(validParam));
    cr::lens::Lens::encodeSetParamCommand(
                validParam, size, cr::lens::LensParam::ZOOM_POS, 1.5f);

    cr::lens::LensParam paramId{cr::lens::LensParam::IRIS_POS};
    cr::lens::LensCommand commandId{cr::lens::LensCommand::IRIS_STOP};
    float value = 7.0f;
    bool ok = true;

    // A rejected decode: the result is -1 and the output is not changed.
    auto rejected = [&](uint8_t* bytes, int bytesSize, const char* what)
    {
        const int result = cr::lens::Lens::decodeCommand(
                    bytes, bytesSize, paramId, commandId, value);
        if (result != -1 || paramId != cr::lens::LensParam::IRIS_POS ||
            commandId != cr::lens::LensCommand::IRIS_STOP || value != 7.0f)
        {
            std::cout << what << " (size " << bytesSize << ", result "
                      << result << ")" << std::endl;
            return false;
        }
        return true;
    };

    // Null pointer, wrong sizes: the size must be exactly 11 bytes.
    ok = rejected(nullptr, COMMAND_SIZE, "nullptr data") && ok;
    for (int badSize = -2; badSize <= COMMAND_SIZE + 9; ++badSize)
    {
        if (badSize != COMMAND_SIZE)
        {
            ok = rejected(valid, badSize, "Wrong size") && ok;
        }
    }
    ok = rejected(valid, INT_MAX, "Huge size") && ok;
    ok = rejected(valid, INT_MIN, "Negative size") && ok;

    // Wrong data type and version.
    for (int pos = 0; pos <= 2; ++pos)
    {
        for (int byte = 0; byte < 256; ++byte)
        {
            const uint8_t validBytes[] = {
                0x00, static_cast<uint8_t>(LENS_MAJOR_VERSION),
                static_cast<uint8_t>(LENS_MINOR_VERSION)};
            if (byte == validBytes[pos] || (pos == 0 && byte == 0x01))
            {
                continue;
            }
            uint8_t bad[BUFFER_SIZE];
            std::memcpy(bad, valid, sizeof(bad));
            bad[pos] = static_cast<uint8_t>(byte);
            ok = rejected(bad, COMMAND_SIZE, "Wrong header byte") && ok;
        }
    }

    // IDs outside the enum range for the command and for the set param.
    const int badIds[] = {INT_MIN, -1, 0, LAST_COMMAND + 1, LAST_PARAM + 1,
                          1000, INT_MAX};
    for (const int badId : badIds)
    {
        for (const uint8_t type : {uint8_t{0x00}, uint8_t{0x01}})
        {
            const int last = type == 0x00 ? LAST_COMMAND : LAST_PARAM;
            if (badId >= 1 && badId <= last)
            {
                continue;
            }
            uint8_t bad[BUFFER_SIZE];
            std::memcpy(bad, valid, sizeof(bad));
            bad[0] = type;
            std::memcpy(&bad[3], &badId, sizeof(badId));
            ok = rejected(bad, COMMAND_SIZE, "ID out of the range") && ok;
        }
    }

    // Null pointer of the encoders: the size is 0.
    size = 5;
    cr::lens::Lens::encodeCommand(
                nullptr, size, cr::lens::LensCommand::ZOOM_TELE);
    if (size != 0)
    {
        ok = fail("encodeCommand(nullptr)");
    }
    size = 5;
    cr::lens::Lens::encodeSetParamCommand(
                nullptr, size, cr::lens::LensParam::ZOOM_POS, 0.0f);
    if (size != 0)
    {
        ok = fail("encodeSetParamCommand(nullptr)");
    }
    return ok;
}



/// Version string of the library.
bool versionTest()
{
    const std::string expected = std::to_string(LENS_MAJOR_VERSION) + "." +
                                 std::to_string(LENS_MINOR_VERSION) + "." +
                                 std::to_string(LENS_PATCH_VERSION);
    std::cout << "Lens version: " << cr::lens::Lens::getVersion() << std::endl;
    if (cr::lens::Lens::getVersion() != expected ||
        expected != LENS_VERSION)
    {
        return fail("Version string is not equal to the version macros");
    }
    return true;
}



/// Remove a file at the end of the scope. The result of the removal is
/// reported.
class TempFile
{
public:
    explicit TempFile(const char* name) : m_name(name)
    {
    }

    TempFile(const TempFile&) = delete;
    TempFile(TempFile&&) = delete;
    TempFile& operator= (const TempFile&) = delete;
    TempFile& operator= (TempFile&&) = delete;

    ~TempFile()
    {
        if (!m_removed)
        {
            remove();
        }
    }

    /// Remove the file. Returns false (and prints a message) if the file
    /// stays.
    bool remove()
    {
        m_removed = true;
        if (std::remove(m_name) != 0)
        {
            std::cout << "Can't remove file " << m_name << std::endl;
            return false;
        }
        return true;
    }

private:
    /// Name of the file.
    const char* m_name;
    /// Flag: remove() was called.
    bool m_removed{false};
};



/// Read and write params to a JSON file and to a string.
bool jsonReadWriteTest()
{
    cr::lens::LensParams in;
    randomizeParamsForJson(in);

    // Write params to the file and read them back.
    bool ok = true;
    {
        TempFile file(JSON_FILE_NAME);
        cr::utils::ConfigReader inConfig;
        if (!inConfig.set(in, "lensParams") ||
            !inConfig.writeToFile(JSON_FILE_NAME))
        {
            return fail("Can't write config file");
        }
        cr::utils::ConfigReader outConfig;
        cr::lens::LensParams out;
        if (!outConfig.readFromFile(JSON_FILE_NAME))
        {
            return fail("Can't open config file");
        }
        if (!outConfig.get(out, "lensParams"))
        {
            return fail("Can't read params from file");
        }

        // The JSON file has only the fields of JSON_READABLE macro. Other
        // fields keep the default values.
        const cr::lens::LensParams defaults;
        const char* const notInJson[] = {
            "zoomPos", "zoomHwPos", "focusPos", "focusHwPos", "irisPos",
            "irisHwPos", "zoomSpeed", "zoomHwSpeed", "focusSpeed",
            "focusHwSpeed", "irisSpeed", "irisHwSpeed", "focusFactor",
            "isConnected", "afIsActive", "xFovDeg", "yFovDeg", "temperature",
            "isOpen"};
        for (const FieldDesc& desc : fields())
        {
            bool inJson = true;
            for (const char* name : notInJson)
            {
                if (std::strcmp(name, desc.name) == 0)
                {
                    inJson = false;
                }
            }
            const uint32_t expected =
                    inJson ? fieldBits(in, desc) : fieldBits(defaults, desc);
            if (fieldBits(out, desc) != expected)
            {
                std::cout << "Mismatch in field " << desc.name << std::endl;
                ok = false;
            }
        }
        if (out.initString != in.initString ||
            out.fovPoints.size() != in.fovPoints.size())
        {
            ok = fail("Mismatch in initString or fovPoints");
        }
        for (size_t i = 0; i < out.fovPoints.size() &&
                           i < in.fovPoints.size(); ++i)
        {
            if (out.fovPoints[i].hwZoomPos != in.fovPoints[i].hwZoomPos ||
                out.fovPoints[i].xFovDeg != in.fovPoints[i].xFovDeg ||
                out.fovPoints[i].yFovDeg != in.fovPoints[i].yFovDeg)
            {
                ok = fail("Mismatch in fovPoints");
            }
        }
        if (!file.remove())
        {
            ok = false;
        }
    }

    // Invalid and incomplete documents are rejected, the object is not
    // changed.
    cr::utils::ConfigReader config;
    config.set(in, "lensParams");
    std::string valid;
    if (!config.writeToString(valid))
    {
        return fail("Can't write config string");
    }
    std::vector<std::string> documents;
    documents.push_back("{\"lensParams\":5}");
    documents.push_back("{\"lensParams\":{}}");
    documents.push_back("{\"other\":{}}");
    documents.push_back("[]");
    const size_t keyPos = valid.find("\"initString\":");
    if (keyPos == std::string::npos)
    {
        return fail("No initString in the written config");
    }
    std::string missingKey = valid;
    missingKey.replace(keyPos, 12, "\"initStrinx\"");
    documents.push_back(missingKey);
    std::string wrongType = valid;
    wrongType.insert(keyPos + 13, "5,\"unused\":");
    documents.push_back(wrongType);
    for (const std::string& document : documents)
    {
        cr::utils::ConfigReader reader;
        cr::lens::LensParams target;
        randomizeParams(target);
        const cr::lens::LensParams snapshot(target);
        if (!reader.readFromString(document))
        {
            std::cout << "Can't parse: " << document.substr(0, 32)
                      << std::endl;
            ok = false;
            continue;
        }
        if (reader.get(target, "lensParams") || !sameParams(target, snapshot))
        {
            std::cout << "Accepted or changed: " << document.substr(0, 32)
                      << std::endl;
            ok = false;
        }
    }
    cr::utils::ConfigReader broken;
    if (broken.readFromString("{\"lensParams\":"))
    {
        ok = fail("Accepted a broken JSON text");
    }
    return ok;
}



/// Run one test and print the result.
bool runTest(const char* title, bool (*test)())
{
    std::cout << title << ":" << std::endl;
    const bool passed = test();
    std::cout << (passed ? "OK" : "ERROR") << std::endl << std::endl;
    return passed;
}
}



/// Entry point. Returns 0 if all tests passed.
int main()
{
    std::cout << "#####################################" << std::endl;
    std::cout << "#                                   #" << std::endl;
    std::cout << "# Lens data structures test         #" << std::endl;
    std::cout << "#                                   #" << std::endl;
    std::cout << "#####################################" << std::endl;
    std::cout << std::endl;

    bool passed = true;
    passed = runTest("Copy test", copyTest) && passed;
    passed = runTest("Encode/Decode params test",
                     encodeDecodeParamsTest) && passed;
    passed = runTest("Encode/Decode test with params mask",
                     encodeDecodeMaskTest) && passed;
    passed = runTest("Encode/Decode test with a single field mask",
                     singleFieldMaskTest) && passed;
    passed = runTest("Encode buffer boundaries test",
                     encodeBoundariesTest) && passed;
    passed = runTest("Decode invalid data test",
                     decodeInvalidDataTest) && passed;
    passed = runTest("Decode damaged data test", damagedDataTest) && passed;
    passed = runTest("Encode/Decode commands test", commandsTest) && passed;
    passed = runTest("Decode invalid commands test",
                     invalidCommandsTest) && passed;
    passed = runTest("Version test", versionTest) && passed;
    passed = runTest("JSON read/write test", jsonReadWriteTest) && passed;

    std::cout << (passed ? "ALL TESTS PASSED" : "SOME TESTS FAILED")
              << std::endl;
    return passed ? 0 : 1;
}
