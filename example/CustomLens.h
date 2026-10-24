#pragma once
#include <cstdint>
#include <mutex>
#include <string>
#include "Lens.h"



namespace rapidpixel
{
/**
 * @brief Custom lens controller class. The class is an example of the Lens
 * interface implementation: it keeps the params, checks the values which
 * come from outside and doesn't control any lens hardware.
 *
 * @note All methods of the Lens interface are thread-safe: they can be called
 * from any thread.
 */
class CustomLens : public Lens
{
public:

    /**
     * @brief Class constructor.
     */
    CustomLens();

    /**
     * @brief Class destructor.
     */
    ~CustomLens() override;

    /**
     * @brief Copy constructor is deleted: the class owns a mutex.
     */
    CustomLens(const CustomLens&) = delete;

    /**
     * @brief Move constructor is deleted: the class owns a mutex.
     */
    CustomLens(CustomLens&&) = delete;

    /**
     * @brief Copy assignment operator is deleted: the class owns a mutex.
     */
    CustomLens& operator= (const CustomLens&) = delete;

    /**
     * @brief Move assignment operator is deleted: the class owns a mutex.
     */
    CustomLens& operator= (CustomLens&&) = delete;

    /**
     * @brief Get lens class version.
     * @return Lens class version string in format "Major.Minor.Patch".
     */
    static std::string getVersion();

    /**
     * @brief Open lens controller. Can be used instead initLens(...) method.
     * @param initString Init string. The example keeps it in the params.
     * Maximum size is 256 bytes.
     * @return TRUE if the lens controller is init or FALSE if the init
     * string is too long.
     */
    bool openLens(std::string initString) override;

    /**
     * @brief Init lens controller by structure. Can be used instead
     * openLens(...) method.
     * @param params Lens params. The values are checked: positions 0-65535,
     * speeds 0-100, modes 0-1 (logMode 0-3), refocusTimeoutSec 0-100000, ROI
     * coordinates >= 0, auto ROI size >= 8, focusFactorThreshold 0-100,
     * custom params must be finite, size of initString <= 256 bytes, number
     * of fovPoints <= 256.
     * @return TRUE if the lens controller is init or FALSE if the params are
     * not valid. If FALSE is returned the params of the controller are not
     * changed.
     */
    bool initLens(LensParams& params) override;

    /**
     * @brief Close connection.
     */
    void closeLens() override;

    /**
     * @brief Get lens open status.
     * @return TRUE if the lens is open or FALSE.
     */
    bool isLensOpen() override;

    /**
     * @brief Get lens connection status. Lens can be open but no response from
     * lens hardware.
     * @return TRUE if the lens has data exchange with lens hardware or FALSE.
     */
    bool isLensConnected() override;

    /**
     * @brief Set the lens controller param.
     * @param id Param ID.
     * @param value Param value. Float values are converted to int by
     * truncation. The value is checked against the range of the param (see
     * initLens(...)).
     * @return TRUE if the param was set or FALSE if the param is read only,
     * unknown or the value is not valid (NaN, infinity or out of the range).
     */
    bool setParam(LensParam id, float value) override;

    /**
     * @brief Get the lens controller param.
     * @param id Param ID.
     * @return Param value or -1 if the param doesn't exist.
     */
    float getParam(LensParam id) override;

    /**
     * @brief Get the lens controller params.
     * @param params Reference to LensParams object.
     */
    void getParams(LensParams& params) override;

    /**
     * @brief Execute command.
     * @param id Command ID.
     * @param arg Command argument. The arguments of ZOOM_TO_POS,
     * FOCUS_TO_POS and IRIS_TO_POS must be 0-65535.
     * @return TRUE if the command executed or FALSE if the command is unknown
     * or the argument is not valid.
     */
    bool executeCommand(LensCommand id, float arg = 0) override;

    /**
     * @brief Add video frame for auto focus purposes. The example doesn't
     * support autofocus algorithms: the frame is ignored.
     * @param frame Video frame object.
     */
    void addVideoFrame(rapidpixel::Frame& frame) override;

    /**
     * @brief Decode and execute command.
     * @param data Pointer to command data.
     * @param size Size of data.
     * @return TRUE if command decoded and executed or FALSE if not.
     */
    bool decodeAndExecuteCommand(uint8_t* data, int size) override;

private:

    /// Lens parameters structure (Default params). Protected by
    /// m_paramsMutex.
    LensParams m_params;
    /// Mutex to protect m_params.
    std::mutex m_paramsMutex;
};
}
