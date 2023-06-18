#pragma once
#include <string>
#include <cstdint>
#include "Frame.h"
#include "ConfigReader.h"



namespace cr
{
namespace lens
{



/// Field of view point class.
class FovPoint
{
public:
    /// Hardware zoom pos.
    int hwZoomPos{0};
    /// Horizontal field of view, degree.
    float xFovDeg{0.0f};
    /// Vertical field of view, degree.
    float yFovDeg{0.0f};

    JSON_READABLE(FovPoint,
                  hwZoomPos,
                  xFovDeg,
                  yFovDeg);

    /**
     * @brief operator =
     * @param src Source object.
     * @return FovPoint obect.
     */
    FovPoint& operator= (const FovPoint& src);
};



/// Lens params structure.
class LensParams
{
public:
    /// Initialization string. Formats depends on implementation.
    std::string initString{"/dev/ttyUSB0;9600;20"};
    /// Zoom position (write/read). Value:
    /// zoom position 0(full wide) - 65535(full tele).
    int zoomPos{0};
    /// Hardware zoom position (write/read). Argument:
    /// zoom position. Value depends on particular lens controller.
    int zoomHwPos{0};
    /// Focus position (write/read). Value:
    /// focus position 0(full near) - 65535(full far).
    int focusPos{0};
    /// Hardware focus position (write/read). Value:
    /// focus position. Value depends on particular lens controller.
    int focusHwPos{0};
    /// Iris position (write/read). Value:
    /// iris position 0(full close) - 65535(full open).
    int irisPos{0};
    /// Hardware iris position (write/read). Value:
    /// iris position. Value depends on particular lens controller.
    int irisHwPos{0};
    /// Focus mode (write/read). Value:
    /// Value depends on particular lens controller.
    int focusMode{0};
    /// Filter mode (write/read).  Value:
    /// Value depends on particular lens controller.
    int filterMode{0};
    /// Auto focus ROI top-left coordinate (write/read). Value:
    /// ROI top-left horizontal coordinate in pixels.
    int afRoiX0{0};
    /// Auto focus ROI top-left coordinate (write/read). Value:
    /// ROI top-left vertical coordinate in pixels.
    int afRoiY0{0};
    /// Auto focus ROI bottom-right coordinate (write/read). Value:
    /// ROI bottom-right horizontal coordinate in pixels.
    int afRoiX1{0};
    /// Auto focus ROI bottom-right coordinate (write/read). Value:
    /// ROI bottom-right vertical coordinate in pixels.
    int afRoiY1{0};
    /// Zoom speed (write/read). Value:
    /// zoom speed 0 to 100 %.
    int zoomSpeed{50};
    /// Hardware zoom speed (write/read). Value:
    /// zoom speed. Value depends on particular lens controller.
    int zoomHwSpeed{50};
    /// Max hardware zoom speed (write/read). Value:
    /// zoom speed. Value depends on particular lens controller.
    int zoomHwMaxSpeed{50};
    /// Focus speed (write/read). Value:
    /// focus speed 0 to 100 %.
    int focusSpeed{50};
    /// Hardware focus speed (write/read). Value:
    /// focus speed. Value depends on particular lens controller.
    int focusHwSpeed{50};
    /// Max hardware focus speed (write/read). Argument:
    /// focus speed. Value depends on particular lens controller.
    int focusHwMaxSpeed{50};
    /// Iris speed (write/read). Value:
    /// iris speed 0 to 100 %.
    int irisSpeed{50};
    /// Hardware iris speed (write/read). Value:
    /// iris speed. Value depends on particular lens controller.
    int irisHwSpeed{50};
    /// Max hardware iris speed (write/read). Value:
    /// iris speed. Value depends on particular lens controller.
    int irisHwMaxSpeed{50};
    /// Hardware zoom tele limit (write/read). Value:
    /// hardware zoom position. Value depends on particular lens controller.
    int zoomHwTeleLimit{65535};
    /// Hardware zoom wide limit (write/read). Value:
    /// hardware zoom position. Value depends on particular lens controller.
    int zoomHwWideLimit{0};
    /// Hardware focus far limit (write/read). Value:
    /// hardware focus position. Value depends on particular lens controller.
    int focusHwFarLimit{65535};
    /// Hardware focus near limit (write/read). Value:
    /// hardware focus position. Value depends on particular lens controller.
    int focusHwNearLimit{0};
    /// Hardware iris open limit (write/read). Value:
    /// hardware iris position. Value depends on particular lens controller.
    int irisHwOpenLimit{65535};
    /// Hardware iris close limit (write/read). Value:
    /// hardware iris position. Value depends on particular lens controller.
    int irisHwCloseLimit{0};
    /// Focus factor value (read only). Value:
    /// Value depends on particular lens controller.
    float focusFactor{0.0f};
    /// Connection status (read only). Value:
    /// 0 - not connected, 1 - connected.
    bool isConnected{false};
    /// Hardware focus speed in AF mode (write/read). Value:
    /// hardware focus speed. Value depends on particular lens controller.
    int afHwSpeed{50};
    /// Threshold for focus factor to start refocus (write/read). Value:
    /// threshold %: 0 - no check, 100 - changing x2.
    float focusFactorThreshold{0.0f};
    /// Refocus timeout, sec (write/read). Value:
    /// 0 - no refocus, to 100000 sec.
    int refocusTimeoutSec{0};
    /// AF process mode (read only). Value: 0 - not active, 1 - active.
    bool afIsActive{false};
    /// Iris mode. (write/read). Value:
    /// Value depends on particular lens controller.
    int irisMode{0};
    /// Auto ROI width (write/read). Value: 0 to video frame size, pxl.
    int autoAfRoiWidth{150};
    /// Auto ROI height (write/read). Value: 0 to video frame size, pxl.
    int autoAfRoiHeight{150};
    /// Auto ROI frame border in pixels (write/read). Value:
    /// border size from 0 to video frame min(width/height) / 2.
    int autoAfRoiBorder{100};
    /// AF ROI mode (write/read). Value:
    /// 0 - Manual position, 1 - Auto position.
    int afRoiMode{0};
    /// Optical extender mode (write/read). Value:
    /// Value depends on particular lens controller. Default: 0 -Off, 1 -On.
    int extenderMode{0};
    /// Stabilizer mode (write/read). Value:
    /// 0 - Off, 1 - On.
    int stabiliserMode{0};
    /// AF range (write/read). Value:
    /// Value depends on particular lens controller.
    int afRange{0};
    /// Horizontal fiels of view, degree (read only).
    float xFovDeg{1.0f};
    /// Vertical fiels of view, degree (read only).
    float yFovDeg{1.0f};
    /// Logging mode.
    /// Default values:
    /// 0 - Disable.
    /// 1 - Only file.
    /// 2 - Only terminal.
    /// 3 - File and terminal.
    int logMode{0};
    /// Lens temperature, degree (read only).
    float temperature{0.0f};
    /// Open status: 1 - lens control port open, 0 - not open.
    bool isOpen{false};
    /// Lens type service value (write/read). Value depends on implementation.
    int type{0};
    /// Lens custom param 1. Value depends on implementation.
    float custom1;
    /// Lens custom param 2. Value depends on implementation.
    float custom2;
    /// Lens custom param 3. Value depends on implementation.
    float custom3;
    /// List of field of view points.
    std::vector<FovPoint> fovPoints{std::vector<FovPoint>()};

    JSON_READABLE(LensParams,
                  initString,
                  focusMode,
                  filterMode,
                  afRoiX0,
                  afRoiY0,
                  afRoiX1,
                  afRoiY1,
                  zoomHwMaxSpeed,
                  focusHwMaxSpeed,
                  irisHwMaxSpeed,
                  zoomHwTeleLimit,
                  zoomHwWideLimit,
                  focusHwFarLimit,
                  focusHwNearLimit,
                  irisHwOpenLimit,
                  irisHwCloseLimit,
                  afHwSpeed,
                  focusFactorThreshold,
                  refocusTimeoutSec,
                  irisMode,
                  autoAfRoiWidth,
                  autoAfRoiHeight,
                  autoAfRoiBorder,
                  afRoiMode,
                  extenderMode,
                  stabiliserMode,
                  afRange,
                  logMode,
                  type,
                  custom1,
                  custom2,
                  custom3,
                  fovPoints);

    /**
     * @brief operator =
     * @param src Source object.
     * @return LensParams obect.
     */
    LensParams& operator= (const LensParams& src);

    /**
     * @brief Encode params. The method doesn't encode initString and fovPoints.
     * @param data Pointer to data buffer.
     * @param size Size of data.
     */
    void encode(uint8_t* data, int& size);

    /**
     * @brief Decode params. The method doesn't decode initString and fovPoints.
     * @param data Pointer to data.
     * @return TRUE is params decoded or FALSE if not.
     */
    bool decode(uint8_t* data);
};



/// Lens commands enum.
enum class LensCommand
{
    /// Zoom tele. No arguments.
    ZOOM_TELE = 1,
    /// Zoom wide. No arguments.
    ZOOM_WIDE,
    /// Zoom to position. Argument:
    /// zoom position 0(full wide) - 65535(full tele).
    ZOOM_TO_POS,
    /// Stop zoom. No arguments.
    ZOOM_STOP,
    /// Focus far. No arguments.
    FOCUS_FAR,
    /// Focus near. No arguments.
    FOCUS_NEAR,
    /// Focus to position. Argument:
    /// focus position 0(full near) - 65535(full far).
    FOCUS_TO_POS,
    /// Focus stop. No arguments.
    FOCUS_STOP,
    /// Iris open. No arguments.
    IRIS_OPEN,
    /// Iris close. No arguments.
    IRIS_CLOSE,
    /// Iris to position. Argument:
    /// iris position 0(full close) - 65535(full open).
    IRIS_TO_POS,
    /// Iris stop. No arguments.
    IRIS_STOP,
    /// Autofocus start. No arguments.
    AF_START,
    /// Autofocus stop. No arguments.
    AF_STOP,
    /// Restart lens controller. No arguments.
    RESTART,
    /// Do zoom and focus hardware range detection.
    DETECT_HW_RANGES
};



/// Lens params.
enum class LensParam
{
    /// Zoom position (write/read). Value:
    /// zoom position 0(full wide) - 65535(full tele).
    ZOOM_POS = 1,
    /// Hardware zoom position (write/read). Argument:
    /// zoom position. Value depends on particular lens controller.
    ZOOM_HW_POS,
    /// Focus position (write/read). Value:
    /// focus position 0(full near) - 65535(full far).
    FOCUS_POS,
    /// Hardware focus position (write/read). Value:
    /// focus position. Value depends on particular lens controller.
    FOCUS_HW_POS,
    /// Iris position (write/read). Value:
    /// iris position 0(full close) - 65535(full open).
    IRIS_POS,
    /// Hardware iris position (write/read). Value:
    /// iris position. Value depends on particular lens controller.
    IRIS_HW_POS,
    /// Focus mode (write/read). Value:
    /// Value depends on particular lens controller.
    FOCUS_MODE,
    /// Filter mode (write/read).  Value:
    /// Value depends on particular lens controller.
    FILTER_MODE,
    /// Auto focus ROI top-left coordinate (write/read). Value:
    /// ROI top-left horizontal coordinate in pixels.
    AF_ROI_X0,
    /// Auto focus ROI top-left coordinate (write/read). Value:
    /// ROI top-left vertical coordinate in pixels.
    AF_ROI_Y0,
    /// Auto focus ROI bottom-right coordinate (write/read). Value:
    /// ROI bottom-right horizontal coordinate in pixels.
    AF_ROI_X1,
    /// Auto focus ROI bottom-right coordinate (write/read). Value:
    /// ROI bottom-right vertical coordinate in pixels.
    AF_ROI_Y1,
    /// Zoom speed (write/read). Value:
    /// zoom speed 0 to 100 %.
    ZOOM_SPEED,
    /// Hardware zoom speed (write/read). Value:
    /// zoom speed. Value depends on particular lens controller.
    ZOOM_HW_SPEED,
    /// Max hardware zoom speed (write/read). Value:
    /// zoom speed. Value depends on particular lens controller.
    ZOOM_HW_MAX_SPEED,
    /// Focus speed (write/read). Value:
    /// focus speed 0 to 100 %.
    FOCUS_SPEED,
    /// Hardware focus speed (write/read). Value:
    /// focus speed. Value depends on particular lens controller.
    FOCUS_HW_SPEED,
    /// Max hardware focus speed (write/read). Argument:
    /// focus speed. Value depends on particular lens controller.
    FOCUS_HW_MAX_SPEED,
    /// Iris speed (write/read). Value:
    /// iris speed 0 to 100 %.
    IRIS_SPEED,
    /// Hardware iris speed (write/read). Value:
    /// iris speed. Value depends on particular lens controller.
    IRIS_HW_SPEED,
    /// Max hardware iris speed (write/read). Value:
    /// iris speed. Value depends on particular lens controller.
    IRIS_HW_MAX_SPEED,
    /// Hardware zoom tele limit (write/read). Value:
    /// hardware zoom position. Value depends on particular lens controller.
    ZOOM_HW_TELE_LIMIT,
    /// Hardware zoom wide limit (write/read). Value:
    /// hardware zoom position. Value depends on particular lens controller.
    ZOOM_HW_WIDE_LIMIT,
    /// Hardware focus far limit (write/read). Value:
    /// hardware focus position. Value depends on particular lens controller.
    FOCUS_HW_FAR_LIMIT,
    /// Hardware focus near limit (write/read). Value:
    /// hardware focus position. Value depends on particular lens controller.
    FOCUS_HW_NEAR_LIMIT,
    /// Hardware iris open limit (write/read). Value:
    /// hardware iris position. Value depends on particular lens controller.
    IRIS_HW_OPEN_LIMIT,
    /// Hardware iris close limit (write/read). Value:
    /// hardware iris position. Value depends on particular lens controller.
    IRIS_HW_CLOSE_LIMIT,
    /// Focus factor value (read only). Value:
    /// Value depends on particular lens controller.
    FOCUS_FACTOR,
    /// Connection status (read only). Value:
    /// 0 - not connected, 1 - connected.
    IS_CONNECTED,
    /// Hardware focus speed in AF mode (write/read). Value:
    /// hardware focus speed. Value depends on particular lens controller.
    FOCUS_HW_AF_SPEED,
    /// Threshold for focus factor to start refocus (write/read). Value:
    /// threshold %: 0 - no check, 100 - changing x2.
    FOCUS_FACTOR_THRESHOLD,
    /// Refocus timeout, sec (write/read). Value:
    /// 0 - no refocus, to 100000 sec.
    REFOCUS_TIMEOUT_SEC,
    /// AF process mode (read only). Value: 0 - not active, 1 - active.
    AF_IS_ACTIVE,
    /// Iris mode. (write/read). Value:
    /// Value depends on particular lens controller.
    IRIS_MODE,
    /// Auto ROI width (write/read). Value: 0 to video frame size, pxl.
    AUTO_AF_ROI_WIDTH,
    /// Auto ROI height (write/read). Value: 0 to video frame size, pxl.
    AUTO_AF_ROI_HEIGHT,
    /// Auto ROI frame border in pixels (write/read). Value:
    /// border size from 0 to video frame min(width/height) / 2.
    AUTO_AF_ROI_BORDER,
    /// AF ROI mode (write/read). Value:
    /// 0 - Manual position, 1 - Auto position.
    AF_ROI_MODE,
    /// Optical extender mode (write/read). Value:
    /// Value depends on particular lens controller. Default: 0 -Off, 1 -On.
    EXTENDER_MODE,
    /// Stabilizer mode (write/read). Value:
    /// 0 - Off, 1 - On.
    STABILIZER_MODE,
    /// AF range (write/read). Value:
    /// Value depends on particular lens controller.
    AF_RANGE,
    /// Horizontal Field of view, degree (read only).
    X_FOV_DEG,
    /// Vertical Field of view, degree (read only).
    Y_FOV_DEG,
    /// Logging mode.
    /// Default values:
    /// 0 - Disable.
    /// 1 - Only file.
    /// 2 - Only terminal.
    /// 3 - File and terminal.
    LOG_MODE,
    /// Lens temperature, degree.
    TEMPERATURE,
    /// Open status: 1 - lens control port open, 0 - not open.
    IS_OPEN,
    /// Lens type. Value depends on implementation.
    TYPE,
    /// Lens custom param 1. Value depends on implementation.
    CUSTOM_1,
    /// Lens custom param 2. Value depends on implementation.
    CUSTOM_2,
    /// Lens custom param 3. Value depends on implementation.
    CUSTOM_3
};



/**
 * @brief Lens controller interface class.
 */
class Lens
{
public:

    /**
     * @brief Get lens class version.
     * @return Lens class version string in format "Major.Minor.Patch".
     */
    static std::string getVersion();

    /**
     * @brief Open lens controller. Can be used instead initLens(...) method.
     * @param initString Init string. Format depends on lens controller.
     * @return TRUE if the lens controller is init or FALSE.
     */
    virtual bool openLens(std::string initString) = 0;

    /**
     * @brief Init lens controller by structure. Can be used instead
     * openLens(...) method.
     * @param initString Init string. Format depends on lens controller.
     * @return TRUE if the lens controller is init or FALSE.
     */
    virtual bool initLens(LensParams& params) = 0;

    /**
     * @brief Close connection.
     */
    virtual void closeLens() = 0;

    /**
     * @brief Get lens open status.
     * @return TRUE if the lens is open or FALSE.
     */
    virtual bool isLensOpen() = 0;

    /**
     * @brief Get lens connection status. Lens can be open but no response from
     * lens hardware.
     * @return TRUE if the lens is open or FALSE.
     */
    virtual bool isLensConnected() = 0;

    /**
     * @brief Set the lens controller param.
     * @param id Param ID.
     * @param value Param value.
     * @return TRUE if the property set or FALSE.
     */
    virtual bool setParam(LensParam id, float value) = 0;

    /**
     * @brief Get the lens controller param.
     * @param id Param ID.
     * @return float Param value or -1 of the param not exists.
     */
    virtual float getParam(LensParam id) = 0;

    /**
     * @brief Get the lens controller paramw.
     * @param id Param ID.
     * @return Lens params structure.
     */
    virtual LensParams getParams() = 0;

    /**
     * @brief Execute command.
     * @param id Command ID.
     * @param arg Command argument.
     * @return TRUE if the command executed or FALSE.
     */
    virtual bool executeCommand(LensCommand id, float arg = 0) = 0;

    /**
     * @brief Add video frame for auto focus purposes. Some lens controllers
     * may not support this functions.
     * @param frame Video frame object.
     */
    virtual void addVideoFrame(cr::video::Frame& frame) = 0;

    /**
     * @brief Encode set param command.
     * @param data Pointer to data buffer. Must have size >= 11.
     * @param size Size of encoded data.
     * @param id Lens parameter id.
     * @param value Lens parameter value.
     */
    static void encodeSetParamCommand(
            uint8_t* data, int& size, LensParam id, float value);

    /**
     * @brief Encode command.
     * @param data Pointer to data buffer. Must have size >= 11.
     * @param size Size of encoded data.
     * @param id Lens command ID.
     * @param arg Lens command argument.
     */
    static void encodeCommand(
            uint8_t* data, int& size, LensCommand id, float arg = 0.0f);

    /**
     * @brief Decode command.
     * @param data Pointer to command data.
     * @param size Size of data.
     * @param paramId Output command ID.
     * @param commandId Output command ID.
     * @param value Param or command value.
     * @return 0 - command decoded, 1 - set param command decoded, -1 - error.
     */
    static int decodeCommand(uint8_t* data,
                             int size,
                             LensParam& paramId,
                             LensCommand& commandId,
                             float& value);
};
}
}
