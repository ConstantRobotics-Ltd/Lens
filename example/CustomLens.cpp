#include "CustomLens.h"
#include "CustomLensVersion.h"
#include <cstddef>
#include <cstdint>
#include <limits>
#include <mutex>
#include <utility>
#include "Frame.h"
#include "Lens.h"



namespace
{
/// Limit of float values which can be converted to int. 2^31 is exactly
/// representable as float, so (-LIMIT <= value < LIMIT) is the exact range
/// of int.
constexpr float INT_RANGE_LIMIT{2147483648.0f};

/// The whole range of int: the params whose values depend on the lens
/// hardware are not limited by the example.
constexpr int ANY_MIN{std::numeric_limits<int>::min()};
constexpr int ANY_MAX{std::numeric_limits<int>::max()};
/// Range of the user space positions of zoom, focus and iris: 0 (full wide,
/// near or close) - 65535 (full tele, far or open).
constexpr int POSITION_MIN{0};
constexpr int POSITION_MAX{65535};
/// Range of the speeds of zoom, focus and iris, percent of the max hardware
/// speed.
constexpr int SPEED_MIN{0};
constexpr int SPEED_MAX{100};
/// Range of the params which have two states (focus, filter and iris modes,
/// AF ROI mode, extender and stabiliser modes): 0 or 1.
constexpr int MODE_MIN{0};
constexpr int MODE_MAX{1};
/// Range of the logging mode: 0 - disable, 1 - only file, 2 - only terminal,
/// 3 - file and terminal.
constexpr int LOG_MODE_MIN{0};
constexpr int LOG_MODE_MAX{3};
/// Maximum value of the timeout for automatic refocus, seconds.
constexpr int REFOCUS_TIMEOUT_MAX{100000};
/// Minimum size of the autofocus ROI (width and height) when the ROI position
/// is detected automatically, pixels.
constexpr int AUTO_AF_ROI_SIZE_MIN{8};
/// Range of the threshold for changes of focus factor to start refocus,
/// percent.
constexpr float FOCUS_FACTOR_THRESHOLD_MIN{0.0f};
constexpr float FOCUS_FACTOR_THRESHOLD_MAX{100.0f};
/// Range of the custom params: any finite float.
constexpr float CUSTOM_MIN{std::numeric_limits<float>::lowest()};
constexpr float CUSTOM_MAX{std::numeric_limits<float>::max()};
/// Maximum size of the init string, bytes.
constexpr std::size_t MAX_INIT_STRING_SIZE{256};
/// Maximum number of the points to calculate field of view.
constexpr std::size_t MAX_FOV_POINTS{256};



/// Convert float param value to int. A conversion of NaN or of a value outside
/// of the int range is undefined behavior, so such values are rejected. The
/// negation of the whole condition is intentional: a comparison with NaN is
/// false, so NaN is rejected.
bool toInt(float value, int& result)
{
    if (!(value >= -INT_RANGE_LIMIT && value < INT_RANGE_LIMIT))
    {
        return false;
    }
    result = static_cast<int>(value);
    return true;
}



/// Check that the value is inside [minValue, maxValue].
bool inRange(int value, int minValue, int maxValue)
{
    return value >= minValue && value <= maxValue;
}



/// Check that the value is inside [minValue, maxValue]. The check is false for
/// NaN.
bool inRange(float value, float minValue, float maxValue)
{
    return value >= minValue && value <= maxValue;
}



/// Assign the param value to the target if the value is an int inside
/// [minValue, maxValue].
bool assignInt(int& target, float value, int minValue, int maxValue)
{
    int result = 0;
    if (!toInt(value, result) || !inRange(result, minValue, maxValue))
    {
        return false;
    }
    target = result;
    return true;
}



/// Assign the param value to the target if the value is inside
/// [minValue, maxValue] (NaN is rejected).
bool assignFloat(float& target, float value, float minValue, float maxValue)
{
    if (!inRange(value, minValue, maxValue))
    {
        return false;
    }
    target = value;
    return true;
}



/// Check the values of the params which are set by the user.
bool isValid(const rapidpixel::LensParams& p)
{
    return p.initString.size() <= MAX_INIT_STRING_SIZE &&
           p.fovPoints.size() <= MAX_FOV_POINTS &&
           inRange(p.zoomPos, POSITION_MIN, POSITION_MAX) &&
           inRange(p.focusPos, POSITION_MIN, POSITION_MAX) &&
           inRange(p.irisPos, POSITION_MIN, POSITION_MAX) &&
           inRange(p.zoomSpeed, SPEED_MIN, SPEED_MAX) &&
           inRange(p.focusSpeed, SPEED_MIN, SPEED_MAX) &&
           inRange(p.irisSpeed, SPEED_MIN, SPEED_MAX) &&
           inRange(p.focusMode, MODE_MIN, MODE_MAX) &&
           inRange(p.filterMode, MODE_MIN, MODE_MAX) &&
           inRange(p.irisMode, MODE_MIN, MODE_MAX) &&
           inRange(p.afRoiMode, MODE_MIN, MODE_MAX) &&
           inRange(p.extenderMode, MODE_MIN, MODE_MAX) &&
           inRange(p.stabiliserMode, MODE_MIN, MODE_MAX) &&
           inRange(p.logMode, LOG_MODE_MIN, LOG_MODE_MAX) &&
           inRange(p.refocusTimeoutSec, 0, REFOCUS_TIMEOUT_MAX) &&
           inRange(p.afRoiX0, 0, ANY_MAX) &&
           inRange(p.afRoiY0, 0, ANY_MAX) &&
           inRange(p.afRoiX1, 0, ANY_MAX) &&
           inRange(p.afRoiY1, 0, ANY_MAX) &&
           inRange(p.autoAfRoiWidth, AUTO_AF_ROI_SIZE_MIN, ANY_MAX) &&
           inRange(p.autoAfRoiHeight, AUTO_AF_ROI_SIZE_MIN, ANY_MAX) &&
           inRange(p.autoAfRoiBorder, 0, ANY_MAX) &&
           inRange(p.focusFactorThreshold, FOCUS_FACTOR_THRESHOLD_MIN,
                   FOCUS_FACTOR_THRESHOLD_MAX) &&
           inRange(p.custom1, CUSTOM_MIN, CUSTOM_MAX) &&
           inRange(p.custom2, CUSTOM_MIN, CUSTOM_MAX) &&
           inRange(p.custom3, CUSTOM_MIN, CUSTOM_MAX);
}
}



rapidpixel::CustomLens::CustomLens()
{
    // Reset connection flags.
    m_params.isOpen = false;
    m_params.isConnected = false;
}



rapidpixel::CustomLens::~CustomLens() = default;



std::string rapidpixel::CustomLens::getVersion()
{
    return CUSTOM_LENS_VERSION;
}



bool rapidpixel::CustomLens::openLens(std::string initString)
{
    // Check the size of the init string.
    if (initString.size() > MAX_INIT_STRING_SIZE)
    {
        return false;
    }

    const std::lock_guard<std::mutex> lock(m_paramsMutex);

    // Save the init string and set connection flags.
    m_params.initString = std::move(initString);
    m_params.isOpen = true;
    m_params.isConnected = true;

    return true;
}



bool rapidpixel::CustomLens::initLens(rapidpixel::LensParams& params)
{
    // Check params: they are copied only if all values are valid.
    if (!isValid(params))
    {
        return false;
    }

    const std::lock_guard<std::mutex> lock(m_paramsMutex);

    // Copy params.
    m_params = params;

    // Set connection flags.
    m_params.isOpen = true;
    m_params.isConnected = true;

    return true;
}



void rapidpixel::CustomLens::closeLens()
{
    const std::lock_guard<std::mutex> lock(m_paramsMutex);

    // Reset connection flags.
    m_params.isOpen = false;
    m_params.isConnected = false;
}



bool rapidpixel::CustomLens::isLensOpen()
{
    const std::lock_guard<std::mutex> lock(m_paramsMutex);

    return m_params.isOpen;
}



bool rapidpixel::CustomLens::isLensConnected()
{
    const std::lock_guard<std::mutex> lock(m_paramsMutex);

    return m_params.isConnected;
}



bool rapidpixel::CustomLens::setParam(rapidpixel::LensParam id, float value)
{
    const std::lock_guard<std::mutex> lock(m_paramsMutex);

    // Check parameter ID and save the value if it is valid.
    switch (id)
    {
    case rapidpixel::LensParam::ZOOM_POS:
    {
        return assignInt(m_params.zoomPos, value, POSITION_MIN, POSITION_MAX);
    }
    case rapidpixel::LensParam::ZOOM_HW_POS:
    {
        return assignInt(m_params.zoomHwPos, value, ANY_MIN, ANY_MAX);
    }
    case rapidpixel::LensParam::FOCUS_POS:
    {
        return assignInt(m_params.focusPos, value, POSITION_MIN, POSITION_MAX);
    }
    case rapidpixel::LensParam::FOCUS_HW_POS:
    {
        return assignInt(m_params.focusHwPos, value, ANY_MIN, ANY_MAX);
    }
    case rapidpixel::LensParam::IRIS_POS:
    {
        return assignInt(m_params.irisPos, value, POSITION_MIN, POSITION_MAX);
    }
    case rapidpixel::LensParam::IRIS_HW_POS:
    {
        return assignInt(m_params.irisHwPos, value, ANY_MIN, ANY_MAX);
    }
    case rapidpixel::LensParam::FOCUS_MODE:
    {
        return assignInt(m_params.focusMode, value, MODE_MIN, MODE_MAX);
    }
    case rapidpixel::LensParam::FILTER_MODE:
    {
        return assignInt(m_params.filterMode, value, MODE_MIN, MODE_MAX);
    }
    case rapidpixel::LensParam::AF_ROI_X0:
    {
        return assignInt(m_params.afRoiX0, value, 0, ANY_MAX);
    }
    case rapidpixel::LensParam::AF_ROI_Y0:
    {
        return assignInt(m_params.afRoiY0, value, 0, ANY_MAX);
    }
    case rapidpixel::LensParam::AF_ROI_X1:
    {
        return assignInt(m_params.afRoiX1, value, 0, ANY_MAX);
    }
    case rapidpixel::LensParam::AF_ROI_Y1:
    {
        return assignInt(m_params.afRoiY1, value, 0, ANY_MAX);
    }
    case rapidpixel::LensParam::ZOOM_SPEED:
    {
        return assignInt(m_params.zoomSpeed, value, SPEED_MIN, SPEED_MAX);
    }
    case rapidpixel::LensParam::ZOOM_HW_SPEED:
    {
        return assignInt(m_params.zoomHwSpeed, value, ANY_MIN, ANY_MAX);
    }
    case rapidpixel::LensParam::ZOOM_HW_MAX_SPEED:
    {
        return assignInt(m_params.zoomHwMaxSpeed, value, ANY_MIN, ANY_MAX);
    }
    case rapidpixel::LensParam::FOCUS_SPEED:
    {
        return assignInt(m_params.focusSpeed, value, SPEED_MIN, SPEED_MAX);
    }
    case rapidpixel::LensParam::FOCUS_HW_SPEED:
    {
        return assignInt(m_params.focusHwSpeed, value, ANY_MIN, ANY_MAX);
    }
    case rapidpixel::LensParam::FOCUS_HW_MAX_SPEED:
    {
        return assignInt(m_params.focusHwMaxSpeed, value, ANY_MIN, ANY_MAX);
    }
    case rapidpixel::LensParam::IRIS_SPEED:
    {
        return assignInt(m_params.irisSpeed, value, SPEED_MIN, SPEED_MAX);
    }
    case rapidpixel::LensParam::IRIS_HW_SPEED:
    {
        return assignInt(m_params.irisHwSpeed, value, ANY_MIN, ANY_MAX);
    }
    case rapidpixel::LensParam::IRIS_HW_MAX_SPEED:
    {
        return assignInt(m_params.irisHwMaxSpeed, value, ANY_MIN, ANY_MAX);
    }
    case rapidpixel::LensParam::ZOOM_HW_TELE_LIMIT:
    {
        return assignInt(m_params.zoomHwTeleLimit, value, ANY_MIN, ANY_MAX);
    }
    case rapidpixel::LensParam::ZOOM_HW_WIDE_LIMIT:
    {
        return assignInt(m_params.zoomHwWideLimit, value, ANY_MIN, ANY_MAX);
    }
    case rapidpixel::LensParam::FOCUS_HW_FAR_LIMIT:
    {
        return assignInt(m_params.focusHwFarLimit, value, ANY_MIN, ANY_MAX);
    }
    case rapidpixel::LensParam::FOCUS_HW_NEAR_LIMIT:
    {
        return assignInt(m_params.focusHwNearLimit, value, ANY_MIN, ANY_MAX);
    }
    case rapidpixel::LensParam::IRIS_HW_OPEN_LIMIT:
    {
        return assignInt(m_params.irisHwOpenLimit, value, ANY_MIN, ANY_MAX);
    }
    case rapidpixel::LensParam::IRIS_HW_CLOSE_LIMIT:
    {
        return assignInt(m_params.irisHwCloseLimit, value, ANY_MIN, ANY_MAX);
    }
    case rapidpixel::LensParam::FOCUS_HW_AF_SPEED:
    {
        return assignInt(m_params.afHwSpeed, value, ANY_MIN, ANY_MAX);
    }
    case rapidpixel::LensParam::FOCUS_FACTOR_THRESHOLD:
    {
        return assignFloat(m_params.focusFactorThreshold, value,
                           FOCUS_FACTOR_THRESHOLD_MIN,
                           FOCUS_FACTOR_THRESHOLD_MAX);
    }
    case rapidpixel::LensParam::REFOCUS_TIMEOUT_SEC:
    {
        return assignInt(m_params.refocusTimeoutSec, value, 0,
                         REFOCUS_TIMEOUT_MAX);
    }
    case rapidpixel::LensParam::IRIS_MODE:
    {
        return assignInt(m_params.irisMode, value, MODE_MIN, MODE_MAX);
    }
    case rapidpixel::LensParam::AUTO_AF_ROI_WIDTH:
    {
        return assignInt(m_params.autoAfRoiWidth, value, AUTO_AF_ROI_SIZE_MIN,
                         ANY_MAX);
    }
    case rapidpixel::LensParam::AUTO_AF_ROI_HEIGHT:
    {
        return assignInt(m_params.autoAfRoiHeight, value, AUTO_AF_ROI_SIZE_MIN,
                         ANY_MAX);
    }
    case rapidpixel::LensParam::AUTO_AF_ROI_BORDER:
    {
        return assignInt(m_params.autoAfRoiBorder, value, 0, ANY_MAX);
    }
    case rapidpixel::LensParam::AF_ROI_MODE:
    {
        return assignInt(m_params.afRoiMode, value, MODE_MIN, MODE_MAX);
    }
    case rapidpixel::LensParam::EXTENDER_MODE:
    {
        return assignInt(m_params.extenderMode, value, MODE_MIN, MODE_MAX);
    }
    case rapidpixel::LensParam::STABILIZER_MODE:
    {
        return assignInt(m_params.stabiliserMode, value, MODE_MIN, MODE_MAX);
    }
    case rapidpixel::LensParam::AF_RANGE:
    {
        return assignInt(m_params.afRange, value, ANY_MIN, ANY_MAX);
    }
    case rapidpixel::LensParam::LOG_MODE:
    {
        return assignInt(m_params.logMode, value, LOG_MODE_MIN, LOG_MODE_MAX);
    }
    case rapidpixel::LensParam::TYPE:
    {
        return assignInt(m_params.type, value, ANY_MIN, ANY_MAX);
    }
    case rapidpixel::LensParam::CUSTOM_1:
    {
        return assignFloat(m_params.custom1, value, CUSTOM_MIN, CUSTOM_MAX);
    }
    case rapidpixel::LensParam::CUSTOM_2:
    {
        return assignFloat(m_params.custom2, value, CUSTOM_MIN, CUSTOM_MAX);
    }
    case rapidpixel::LensParam::CUSTOM_3:
    {
        return assignFloat(m_params.custom3, value, CUSTOM_MIN, CUSTOM_MAX);
    }
    case rapidpixel::LensParam::FOCUS_FACTOR:
    case rapidpixel::LensParam::IS_CONNECTED:
    case rapidpixel::LensParam::AF_IS_ACTIVE:
    case rapidpixel::LensParam::X_FOV_DEG:
    case rapidpixel::LensParam::Y_FOV_DEG:
    case rapidpixel::LensParam::TEMPERATURE:
    case rapidpixel::LensParam::IS_OPEN:
    default:
    {
        // Read only params and unknown IDs.
        return false;
    }
    }
}



float rapidpixel::CustomLens::getParam(rapidpixel::LensParam id)
{
    const std::lock_guard<std::mutex> lock(m_paramsMutex);

    // Check parameter ID.
    switch (id)
    {
    case rapidpixel::LensParam::ZOOM_POS:
    {
        return static_cast<float>(m_params.zoomPos);
    }
    case rapidpixel::LensParam::ZOOM_HW_POS:
    {
        return static_cast<float>(m_params.zoomHwPos);
    }
    case rapidpixel::LensParam::FOCUS_POS:
    {
        return static_cast<float>(m_params.focusPos);
    }
    case rapidpixel::LensParam::FOCUS_HW_POS:
    {
        return static_cast<float>(m_params.focusHwPos);
    }
    case rapidpixel::LensParam::IRIS_POS:
    {
        return static_cast<float>(m_params.irisPos);
    }
    case rapidpixel::LensParam::IRIS_HW_POS:
    {
        return static_cast<float>(m_params.irisHwPos);
    }
    case rapidpixel::LensParam::FOCUS_MODE:
    {
        return static_cast<float>(m_params.focusMode);
    }
    case rapidpixel::LensParam::FILTER_MODE:
    {
        return static_cast<float>(m_params.filterMode);
    }
    case rapidpixel::LensParam::AF_ROI_X0:
    {
        return static_cast<float>(m_params.afRoiX0);
    }
    case rapidpixel::LensParam::AF_ROI_Y0:
    {
        return static_cast<float>(m_params.afRoiY0);
    }
    case rapidpixel::LensParam::AF_ROI_X1:
    {
        return static_cast<float>(m_params.afRoiX1);
    }
    case rapidpixel::LensParam::AF_ROI_Y1:
    {
        return static_cast<float>(m_params.afRoiY1);
    }
    case rapidpixel::LensParam::ZOOM_SPEED:
    {
        return static_cast<float>(m_params.zoomSpeed);
    }
    case rapidpixel::LensParam::ZOOM_HW_SPEED:
    {
        return static_cast<float>(m_params.zoomHwSpeed);
    }
    case rapidpixel::LensParam::ZOOM_HW_MAX_SPEED:
    {
        return static_cast<float>(m_params.zoomHwMaxSpeed);
    }
    case rapidpixel::LensParam::FOCUS_SPEED:
    {
        return static_cast<float>(m_params.focusSpeed);
    }
    case rapidpixel::LensParam::FOCUS_HW_SPEED:
    {
        return static_cast<float>(m_params.focusHwSpeed);
    }
    case rapidpixel::LensParam::FOCUS_HW_MAX_SPEED:
    {
        return static_cast<float>(m_params.focusHwMaxSpeed);
    }
    case rapidpixel::LensParam::IRIS_SPEED:
    {
        return static_cast<float>(m_params.irisSpeed);
    }
    case rapidpixel::LensParam::IRIS_HW_SPEED:
    {
        return static_cast<float>(m_params.irisHwSpeed);
    }
    case rapidpixel::LensParam::IRIS_HW_MAX_SPEED:
    {
        return static_cast<float>(m_params.irisHwMaxSpeed);
    }
    case rapidpixel::LensParam::ZOOM_HW_TELE_LIMIT:
    {
        return static_cast<float>(m_params.zoomHwTeleLimit);
    }
    case rapidpixel::LensParam::ZOOM_HW_WIDE_LIMIT:
    {
        return static_cast<float>(m_params.zoomHwWideLimit);
    }
    case rapidpixel::LensParam::FOCUS_HW_FAR_LIMIT:
    {
        return static_cast<float>(m_params.focusHwFarLimit);
    }
    case rapidpixel::LensParam::FOCUS_HW_NEAR_LIMIT:
    {
        return static_cast<float>(m_params.focusHwNearLimit);
    }
    case rapidpixel::LensParam::IRIS_HW_OPEN_LIMIT:
    {
        return static_cast<float>(m_params.irisHwOpenLimit);
    }
    case rapidpixel::LensParam::IRIS_HW_CLOSE_LIMIT:
    {
        return static_cast<float>(m_params.irisHwCloseLimit);
    }
    case rapidpixel::LensParam::FOCUS_FACTOR:
    {
        return m_params.focusFactor;
    }
    case rapidpixel::LensParam::IS_CONNECTED:
    {
        return m_params.isConnected ? 1.0f : 0.0f;
    }
    case rapidpixel::LensParam::FOCUS_HW_AF_SPEED:
    {
        return static_cast<float>(m_params.afHwSpeed);
    }
    case rapidpixel::LensParam::FOCUS_FACTOR_THRESHOLD:
    {
        return m_params.focusFactorThreshold;
    }
    case rapidpixel::LensParam::REFOCUS_TIMEOUT_SEC:
    {
        return static_cast<float>(m_params.refocusTimeoutSec);
    }
    case rapidpixel::LensParam::AF_IS_ACTIVE:
    {
        return m_params.afIsActive ? 1.0f : 0.0f;
    }
    case rapidpixel::LensParam::IRIS_MODE:
    {
        return static_cast<float>(m_params.irisMode);
    }
    case rapidpixel::LensParam::AUTO_AF_ROI_WIDTH:
    {
        return static_cast<float>(m_params.autoAfRoiWidth);
    }
    case rapidpixel::LensParam::AUTO_AF_ROI_HEIGHT:
    {
        return static_cast<float>(m_params.autoAfRoiHeight);
    }
    case rapidpixel::LensParam::AUTO_AF_ROI_BORDER:
    {
        return static_cast<float>(m_params.autoAfRoiBorder);
    }
    case rapidpixel::LensParam::AF_ROI_MODE:
    {
        return static_cast<float>(m_params.afRoiMode);
    }
    case rapidpixel::LensParam::EXTENDER_MODE:
    {
        return static_cast<float>(m_params.extenderMode);
    }
    case rapidpixel::LensParam::STABILIZER_MODE:
    {
        return static_cast<float>(m_params.stabiliserMode);
    }
    case rapidpixel::LensParam::AF_RANGE:
    {
        return static_cast<float>(m_params.afRange);
    }
    case rapidpixel::LensParam::X_FOV_DEG:
    {
        return m_params.xFovDeg;
    }
    case rapidpixel::LensParam::Y_FOV_DEG:
    {
        return m_params.yFovDeg;
    }
    case rapidpixel::LensParam::LOG_MODE:
    {
        return static_cast<float>(m_params.logMode);
    }
    case rapidpixel::LensParam::TEMPERATURE:
    {
        return m_params.temperature;
    }
    case rapidpixel::LensParam::IS_OPEN:
    {
        return m_params.isOpen ? 1.0f : 0.0f;
    }
    case rapidpixel::LensParam::TYPE:
    {
        return static_cast<float>(m_params.type);
    }
    case rapidpixel::LensParam::CUSTOM_1:
    {
        return m_params.custom1;
    }
    case rapidpixel::LensParam::CUSTOM_2:
    {
        return m_params.custom2;
    }
    case rapidpixel::LensParam::CUSTOM_3:
    {
        return m_params.custom3;
    }
    default:
    {
        return -1.0f;
    }
    }
}



void rapidpixel::CustomLens::getParams(rapidpixel::LensParams& params)
{
    const std::lock_guard<std::mutex> lock(m_paramsMutex);

    params = m_params;
}



bool rapidpixel::CustomLens::executeCommand(rapidpixel::LensCommand id, float arg)
{
    const std::lock_guard<std::mutex> lock(m_paramsMutex);

    // Check command ID.
    switch (id)
    {
    case rapidpixel::LensCommand::ZOOM_TELE:
    case rapidpixel::LensCommand::ZOOM_WIDE:
    case rapidpixel::LensCommand::ZOOM_STOP:
    case rapidpixel::LensCommand::FOCUS_FAR:
    case rapidpixel::LensCommand::FOCUS_NEAR:
    case rapidpixel::LensCommand::FOCUS_STOP:
    case rapidpixel::LensCommand::IRIS_OPEN:
    case rapidpixel::LensCommand::IRIS_CLOSE:
    case rapidpixel::LensCommand::IRIS_STOP:
    case rapidpixel::LensCommand::RESTART:
    case rapidpixel::LensCommand::DETECT_HW_RANGES:
    {
        // The example doesn't have lens hardware: the command is accepted.
        return true;
    }
    case rapidpixel::LensCommand::ZOOM_TO_POS:
    {
        // Setting the position is equivalent to the ZOOM_TO_POS command.
        return assignInt(m_params.zoomPos, arg, POSITION_MIN, POSITION_MAX);
    }
    case rapidpixel::LensCommand::FOCUS_TO_POS:
    {
        return assignInt(m_params.focusPos, arg, POSITION_MIN, POSITION_MAX);
    }
    case rapidpixel::LensCommand::IRIS_TO_POS:
    {
        return assignInt(m_params.irisPos, arg, POSITION_MIN, POSITION_MAX);
    }
    case rapidpixel::LensCommand::AF_START:
    {
        m_params.afIsActive = true;
        return true;
    }
    case rapidpixel::LensCommand::AF_STOP:
    {
        m_params.afIsActive = false;
        return true;
    }
    default:
    {
        return false;
    }
    }
}



void rapidpixel::CustomLens::addVideoFrame(rapidpixel::Frame&)
{
}



bool rapidpixel::CustomLens::decodeAndExecuteCommand(uint8_t* data, int size)
{
    // Decode command. The decoder checks the pointer, the size, the version
    // and the IDs.
    LensCommand commandId{LensCommand::ZOOM_TELE};
    LensParam paramId{LensParam::ZOOM_POS};
    float value{0.0f};
    const int result = Lens::decodeCommand(
                data, size, paramId, commandId, value);

    // Execute the decoded command.
    if (result == 0)
    {
        return executeCommand(commandId, value);
    }
    if (result == 1)
    {
        return setParam(paramId, value);
    }

    return false;
}
