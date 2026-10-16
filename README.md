![lens_web_logo](./static/lens_web_logo.png)



# **Lens interface C++ library**

**v4.4.5**



# Table of contents

- [Overview](#overview)
- [Versions](#versions)
- [Library files](#library-files)
- [Lens interface class description](#lens-interface-class-description)
  - [Class declaration](#lens-class-declaration)
  - [getVersion method](#getversion-method)
  - [openLens method](#openlens-method)
  - [initLens method](#initlens-method)
  - [closeLens method](#closelens-method)
  - [isLensOpen method](#islensopen-method)
  - [isLensConnected method](#islensconnected-method)
  - [setParam method](#setparam-method)
  - [getParam method](#getparam-method)
  - [getParams method](#getparams-method)
  - [executeCommand method](#executecommand-method)
  - [addVideoFrame method](#addvideoframe-method)
  - [encodeSetParamCommand method](#encodesetparamcommand-method)
  - [encodeCommand method](#encodecommand-method)
  - [decodeCommand method](#decodecommand-method)
  - [decodeAndExecuteCommand method](#decodeandexecutecommand-method)
- [Data structures](#data-structures)
  - [LensCommand enum](#lenscommand-enum)
  - [LensParam enum](#lensparam-enum)
- [LensParams class description](#lensparams-class-description)
  - [LensParams class declaration](#lensparams-class-declaration)
  - [Serialize lens params](#serialize-lens-params)
  - [Deserialize lens params](#deserialize-lens-params)
  - [Read params from JSON file and write to JSON file](#read-params-from-json-file-and-write-to-json-file)
- [Build and connect to your project](#build-and-connect-to-your-project)
  - [Build stand alone](#build-stand-alone)
  - [CMake options](#cmake-options)
  - [Connect to your project](#connect-to-your-project)
- [Portability](#portability)
- [How to make custom implementation](#how-to-make-custom-implementation)



# Overview

**Lens** C++ library provides standard interface as well defines data structures and rules for different lens controllers. **Lens** class allows you to create a single set of functions and commands for different lens controllers. In this way the **Lens** class creates a program abstraction from the lens hardware features. **Lens** interface class doesn't do anything, just provides interface and provides methods to encode / decode commands and encode / decode params. Different lens controller classes inherit interface form **Lens** C++ class. **Lens.h** file contains list of data structures ([LensCommand](#lenscommand-enum) enum, [LensParam](#lensparam-enum) enum and [LensParams](#lensparams-class-description) class) and [Lens](#lens-interface-class-description) class declaration. [LensCommand](#lenscommand-enum) enum contains IDs of commands supported by [Lens](#lens-interface-class-description) class. [LensParam](#lensparam-enum) enum contains IDs of params supported by **Lens** class. [LensParams](#lensparams-class-description) class contains fields for lens parameters values and provides methods to encode/decode and read/write lens parameters from JSON file. All lens controllers should include params and commands listed in **Lens.h** file. Lens interface class depends on [Frame](https://rapidpixel.constantrobotics.com/docs/Service/Frame.html) class (describes video frame and video frame data structures, necessary for autofocus functions) and [ConfigReader](https://rapidpixel.constantrobotics.com/docs/Service/ConfigReader.html) library (provides methods to read / write JSON config files). The repository doesn't contain these libraries: they must be placed next to the **Lens** repository (see [Build and connect to your project](#build-and-connect-to-your-project) section). It uses C++17 standard.



# Versions

**Table 1** - Library versions.

| Version | Release date | What's new                                                   |
| ------- | ------------ | ------------------------------------------------------------ |
| 1.0.0   | 21.10.2022   | First version                                                |
| 2.0.0   | 17.03.2023   | - Sub-repository **LensControllerDataStrctures** excluded.<br/>- Data structures moved to Lens.h file.<br/>- New parameters IDs added.<br/>- Description updated. |
| 3.0.0   | 23.04.2023   | - Methods signatures changed.                                |
| 3.1.0   | 08.05.2023   | - Added new parameters.                                      |
| 3.2.0   | 10.05.2023   | - Parameters list changed.                                   |
| 4.0.0   | 18.06.2023   | - Added **LensParams** class which contains lens controller parameters.<br/>- Added new methods to encode/decode lens control command.<br/>- Added new lens parameters.<br/>- Added tests. |
| 4.0.1   | 28.06.2023   | - ConfigReader submodule updated.<br />- Frame submodule updated. |
| 4.0.2   | 29.06.2023   | - Documentation updated.                                     |
| 4.0.3   | 01.07.2023   | - Documentation updated.<br />- Lens class comments in source code updated. |
| 4.1.0   | 11.07.2023   | - Added LensParamsMask for lens params masking.<br />- encode(...) method of LensParams class updated.<br />- Documentation updated. |
| 4.2.0   | 22.09.2023   | - Updated encode(...) and decode(...) methods of LensParams.<br />- Added decodeAndExecuteCommand(...) method.<br />- Added example of lens controller implementation. |
| 4.3.0   | 26.09.2023   | - getParams method updated.                                  |
| 4.3.1   | 13.11.2023   | - Frame class updated.                                       |
| 4.4.1   | 13.12.2023   | - Virtual destructor added.<br />- Frame class updated.      |
| 4.4.2   | 25.03.2024   | - Frame class updated.<br />- ConfigReader class updated.<br />- Documentation updated. |
| 4.4.3   | 21.05.2024   | - Frame class updated.<br />- ConfigReader class updated.<br />- Documentation updated. |
| 4.4.4   | 22.07.2024   | - Frame class updated.<br />- ConfigReader class updated.<br />- CMake updated. |
| 4.4.5   | 16.10.2026   | - Sub-repositories excluded: **Frame** and **ConfigReader** must be placed next to the library repository.<br />- Methods **encode(...)**, **decode(...)**, **encodeCommand(...)**, **encodeSetParamCommand(...)** and **decodeCommand(...)** reject nullptr arguments.<br />- Method **decode(...)** doesn't change the object if it returns FALSE, rejects data with unknown mask bits and decodes data with empty mask.<br />- Method **decodeCommand(...)** checks command and parameter IDs and doesn't change output arguments if it returns -1.<br />- Method **encode(...)** sets size to 0 if it returns FALSE.<br />- Fixed description of **focusFactorThreshold** and **autoAfRoiHeight** parameters.<br />- Compiler warning options and security options added to CMake files.<br />- Example of custom lens controller is thread-safe, checks values and sets **FOCUS_HW_FAR_LIMIT** parameter correctly.<br />- Test application updated.<br />- Documentation updated. |



# Library files

The library supplied by source code only. The user would be given a set of files in the form of a CMake project (repository). The repository structure is shown below:

```xml
CMakeLists.txt -------------- Main CMake file of the library.
README.md ------------------- Documentation of the library.
example --------------------- Folder with an example of custom lens controller.
    CMakeLists.txt ---------- CMake file for example of custom lens controller.
    CustomLens.cpp ---------- C++ implementation file.
    CustomLens.h ------------ Header with class declaration.
    CustomLensVersion.h ----- Header file which includes class version.
    CustomLensVersion.h.in -- CMake service file to generate version file.
static ---------------------- Folder with images of the documentation.
test ------------------------ Folder with test application.
    CMakeLists.txt ---------- CMake file for test application.
    main.cpp ---------------- Source code file of test application.
src ------------------------- Folder with source code of the library.
    CMakeLists.txt ---------- CMake file of the library.
    Lens.cpp ---------------- C++ implementation file.
    Lens.h ------------------ Header file which includes Lens class declaration.
    LensVersion.h ----------- Header file which includes version of the library.
    LensVersion.h.in -------- CMake service file to generate version file.
```



# Lens interface class description



## Lens class declaration

**Lens** interface class declared in **Lens.h** file. Class declaration:

```cpp
namespace cr
{
namespace lens
{
/// Lens controller interface class.
class Lens
{
public:

    /// Class destructor.
    virtual ~Lens();

    /// Get lens class version.
    static std::string getVersion();

    /// Open lens controller.
    virtual bool openLens(std::string initString) = 0;

    /// Init lens controller by set of parameters.
    virtual bool initLens(LensParams& params) = 0;

    /// Close connection.
    virtual void closeLens() = 0;

    /// Get lens open status.
    virtual bool isLensOpen() = 0;

    /// Get lens connection status.
    virtual bool isLensConnected() = 0;

    /// Set the lens controller param.
    virtual bool setParam(LensParam id, float value) = 0;

    /// Get the lens controller param.
    virtual float getParam(LensParam id) = 0;

    /// Get the lens controller params.
    virtual void getParams(LensParams& params) = 0;

    /// Execute command.
    virtual bool executeCommand(LensCommand id, float arg = 0) = 0;

    /// Add video frame for auto focus purposes.
    virtual void addVideoFrame(cr::video::Frame& frame) = 0;

    /// Encode set param command.
    static void encodeSetParamCommand(
            uint8_t* data, int& size, LensParam id, float value);

    /// Encode command.
    static void encodeCommand(
            uint8_t* data, int& size, LensCommand id, float arg = 0.0f);

    /// Decode command.
    static int decodeCommand(uint8_t* data,
                             int size,
                             LensParam& paramId,
                             LensCommand& commandId,
                             float& value);

    /// Decode and execute command.
    virtual bool decodeAndExecuteCommand(uint8_t* data, int size) = 0;

protected:

    /// Default constructor. The class can be used as a base only.
    Lens() = default;

    /// Copy constructor.
    Lens(const Lens& src) = default;

    /// Move constructor.
    Lens(Lens&& src) = default;

    /// Copy assignment operator.
    Lens& operator= (const Lens& src) = default;

    /// Move assignment operator.
    Lens& operator= (Lens&& src) = default;
};
}
}
```



## getVersion method

The **getVersion()** method returns string of current class version. Particular lens controller can have it's own **getVersion()** method. Method declaration:

```cpp
static std::string getVersion();
```

Method can be used without **Lens** class instance:

```cpp
std::cout << "Lens class version: " << cr::lens::Lens::getVersion() << std::endl;
```

Console output:

```bash
Lens class version: 4.4.5
```



## openLens method

The **openLens(...)** method initializes lens controller by initialization string. This method can be used instead of [initLens(...)](#initlens-method) method. **openLens(...)** method usually establish communication with lens equipment and set all other lens parameters by default. Method declaration:

```cpp
virtual bool openLens(std::string initString) = 0;
```

| Parameter  | Value                                                        |
| ---------- | ------------------------------------------------------------ |
| initString | Initialization string. Particular lens controller can have unique init string format. But it is recommended to use '**;**' symbol to divide parts of initialization string. Recommended initialization string format for controllers which uses serial port: **"/dev/ttyUSB0;9600;100"** ("/dev/ttyUSB0" - serial port name, "9600" - baudrate, "100" - serial port read timeout). |

**Returns:** TRUE if the lens controller is initialized or FALSE if not.



## initLens method

The **initLens(...)** method initializes lens controller by set of parameters. This method can be used instead of [openLens(...)](#openlens-method) method ([LensParams](#lensparams-class-description) class includes **initString**) when you need initialize lens controller with not default parameters. Method declaration:

```cpp
virtual bool initLens(LensParams& params) = 0;
```

| Parameter | Value                                                        |
| --------- | ------------------------------------------------------------ |
| params    | Parameters ([LensParams](#lensparams-class-description) class). LensParams class includes initString which used in **openLens(...)** method (see description of [LensParams](#lensparams-class-description) class). |

**Returns:** TRUE if the lens controller initialized or FALSE if not.



## closeLens method

The **closeLens()** method closes connection to lens equipment. Method declaration:

```cpp
virtual void closeLens() = 0;
```



## isLensOpen method

The **isLensOpen()** method returns lens initialization status. Open status shows if the lens controller initialized but doesn't show if lens controller has communication with lens equipment. For example, if lens has serial port lens controller connects to serial port (opens serial port file in OS) but lens can be not active (no power). In this case open status just shows that lens controller has opened serial port. Method declaration:

```cpp
virtual bool isLensOpen() = 0;
```

**Returns:** TRUE is the lens controller is initialized or FALSE if not.



## isLensConnected method

The **isLensConnected()** method returns lens connection status. Connection status shows if the lens controller has data exchange with lens equipment. For example, if lens has serial port lens controller connects to serial port (opens serial port file in OS) but lens can be not active (no power). In this case connection status shows that lens controller doesn't have data exchange with lens equipment (method will return FALSE). If lens controller has data exchange with lens equipment the method will return TRUE. If lens controller not initialize the connection status always FALSE. Method declaration:

```cpp
virtual bool isLensConnected() = 0;
```

**Returns:** TRUE if the lens controller has data exchange with lens equipment or FALSE if not.



## setParam method

The **setParam(...)** method sets new lens parameters value. The particular implementation of the lens controller must provide thread-safe **setParam(...)** method call. This means that the **setParam(...)** method can be safely called from any thread. Method declaration:

```cpp
virtual bool setParam(LensParam id, float value) = 0;
```

| Parameter | Description                                                  |
| --------- | ------------------------------------------------------------ |
| id        | Lens parameter ID according to [LensParam](#lensparam-enum) enum. |
| value     | Lens parameter value. Value depends on parameter ID (see description of [LensParam enum](#lensparam-enum) enum). |

**Returns:** TRUE if the parameter was set or FALSE if not.



## getParam method

The **getParam(...)** method returns lens parameter value. The particular implementation of the lens controller must provide thread-safe **getParam(...)** method call. This means that the **getParam(...)** method can be safely called from any thread. Method declaration:

```cpp
virtual float getParam(LensParam id) = 0;
```

| Parameter | Description                                                |
| --------- | ---------------------------------------------------------- |
| id        | Lens parameter ID according to [LensParam](#lensparam-enum) enum. |

**Returns:** parameter value or **-1** of the parameters doesn't exist (not supported) in particular lens controller.



## getParams method

The **getParams(...)** method return all lens parameters. The particular implementation of the lens controller must provide thread-safe **getParams(...)** method call. This means that the **getParams(...)** method can be safely called from any thread. Method declaration:

```cpp
    virtual void getParams(LensParams& params) = 0;
```

| Parameter | Description                                                |
| --------- | ---------------------------------------------------------- |
| params    | Reference to LensParams object to store params. |



## executeCommand method

The **executeCommand(...)** method executes lens action command. The particular implementation of the lens controller must provide thread-safe **executeCommand(...)** method call. This means that the **executeCommand(...)** method can be safely called from any thread. Method declaration:

```cpp
virtual bool executeCommand(LensCommand id, float arg = 0) = 0;
```

| Parameter | Description                                                  |
| --------- | ------------------------------------------------------------ |
| id        | Lens command ID according to [LensCommand](#lenscommand-enum) enum. |
| arg       | Lens command argument. Value depends on command ID (see description of [LensCommand](#lenscommand-enum) enum). |

**Returns:** TRUE is the action command was executed (accepted by lens controller) or FALSE if not.



## addVideoFrame method

The **addVideoFrame(...)** method designed to copy video frame data to lens controller to perform autofocus algorithm. Particular lens controller may not support autofocus algorithms. To perform autofocus lens controller calculates focus factor in autofocus ROI (focus factor can be obtained with [getParam(...)](#getparam-method) method and parameters **FOCUS_FACTOR**). To calculate focus factor lens controller needs video frame. If particular lens controller supports autofocus algorithms the method **addVideoFrame(...)** should be called for each captured video frame. Method declaration:

```cpp
virtual void addVideoFrame(cr::video::Frame& frame) = 0;
```

| Parameter | Description                                                  |
| --------- | ------------------------------------------------------------ |
| frame     | Video [Frame](https://rapidpixel.constantrobotics.com/docs/Service/Frame.html) object. |



## encodeSetParamCommand method

The **encodeSetParamCommand(...)** static method designed to encode command to change any remote lens parameter. To control a lens remotely, the developer has to develop his own protocol and according to it encode the command and deliver it over the communication channel. To simplify this, the **Lens** class contains static methods for encoding the control command. The **Lens** class provides two types of commands: a parameter change command (SET_PARAM) and an action command (COMMAND). **encodeSetParamCommand(...)** designed to encode SET_PARAM command. Method declaration:

```cpp
static void encodeSetParamCommand(uint8_t* data, int& size, LensParam id, float value);
```

| Parameter | Description                                                  |
| --------- | ------------------------------------------------------------ |
| data      | Pointer to data buffer for encoded command. Must have size >= 11. Must not be nullptr. |
| size      | Size of encoded data. Will be 11 bytes (0 if **data** is nullptr). |
| id        | Parameter ID according to [LensParam](#lensparam-enum) enum. |
| value     | Parameter value.                                             |

**encodeSetParamCommand(...)** is static and used without **Lens** class instance. This method used on client side (control system). Example:

```cpp
// Buffer for encoded data.
uint8_t data[11];
// Size of encoded data.
int size = 0;
// Parameter value.
float value = 10.0f;
// Encode command.
cr::lens::Lens::encodeSetParamCommand(
    data, size, cr::lens::LensParam::AF_ROI_X0, value);
```



## encodeCommand method

The **encodeCommand(...)** static method designed to encode lens action command. To control a lens remotely, the developer has to develop his own protocol and according to it encode the command and deliver it over the communication channel. To simplify this, the **Lens** class contains static methods for encoding the control command. The **Lens** class provides two types of commands: a parameter change command (SET_PARAM) and an action command (COMMAND). **encodeCommand(...)** designed to encode COMMAND command (action command). Method declaration:

```cpp
static void encodeCommand(uint8_t* data, int& size, LensCommand id, float arg = 0.0f);
```

| Parameter | Description                                                  |
| --------- | ------------------------------------------------------------ |
| data      | Pointer to data buffer for encoded command. Must have size >= 11. Must not be nullptr. |
| size      | Size of encoded data. Will be 11 bytes (0 if **data** is nullptr). |
| id        | Command ID according to [LensCommand](#lenscommand-enum) enum. |
| arg       | Command argument value (value depends on command ID).        |

**encodeCommand(...)** is static and used without **Lens** class instance. This method used on client side (control system). Encoding example:

```cpp
// Buffer for encoded data.
uint8_t data[11];
// Size of encoded data.
int size = 0;
// Command argument value.
float arg = 1000.0f;
// Encode command.
cr::lens::Lens::encodeCommand(
    data, size, cr::lens::LensCommand::ZOOM_TO_POS, arg);
```



## decodeCommand method

The **decodeCommand(...)** static method designed to decode command on lens controller side. To control a lens remotely, the developer has to develop his own protocol and according to it decode the command on lens controller side. To simplify this, the **Lens** interface class contains static method to decode input command (commands should be encoded by [encodeSetParamCommand(...)](#encodesetparamcommand-method) or [encodeCommand(...)](#encodecommand-method) methods). The **Lens** class provides two types of commands: a parameter change command (SET_PARAM) and an action command (COMMAND). Method declaration:

```cpp
static int decodeCommand(uint8_t* data, int size, LensParam& paramId, LensCommand& commandId, float& value);
```

| Parameter | Description                                                  |
| --------- | ------------------------------------------------------------ |
| data      | Pointer to input command. Must not be nullptr.               |
| size      | Size of command. Must be exactly 11 bytes.                   |
| paramId   | Lens parameter ID according to [LensParam](#lensparam-enum) enum. After decoding SET_PARAM command the method will return parameter ID. The argument is not changed in other cases. |
| commandId | Lens command ID according to [LensCommand](#lenscommand-enum) enum. After decoding COMMAND the method will return command ID. The argument is not changed in other cases. |
| value     | Lens parameter value (after decoding SET_PARAM command) or lens command argument (after decoding COMMAND). The value is not checked against any range: lens controller must validate it before use. The argument is changed only if the command was decoded. |

**Returns:** **0** - in case decoding COMMAND, **1** - in case decoding SET_PARAM command or **-1** in case errors: **data** is nullptr, **size** is not 11, version (Major and Minor) of the command is not equal to the version of the library, the command type is unknown, the command ID or the parameter ID is outside of the range of [LensCommand](#lenscommand-enum) or [LensParam](#lensparam-enum) enum. If the method returns **-1** the output arguments are not changed.



## decodeAndExecuteCommand method

The **decodeAndExecuteCommand(...)** method decodes and executes command on lens controller side encoded by [encodeSetParamCommand(...)](#encodesetparamcommand-method) or [encodeCommand(...)](#encodecommand-method) methods. The particular implementation of the lens controller must provide thread-safe **decodeAndExecuteCommand(...)** method call. This means that the **decodeAndExecuteCommand(...)** method can be safely called from any thread. Method declaration:

```cpp
virtual bool decodeAndExecuteCommand(uint8_t* data, int size) = 0;
```

| Parameter | Description                                                  |
| --------- | ------------------------------------------------------------ |
| data      | Pointer to input command.                                    |
| size      | Size of command. Must be 11 bytes for SET_PARAM and COMMAND. |

**Returns:** TRUE if command decoded (SET_PARAM or COMMAND) and executed (action command or set param command) or FALSE if the data is not a valid command (see [decodeCommand(...)](#decodecommand-method) method) or the lens controller didn't accept the command.

Example of internal implementation of **decodeAndExecuteCommand(...)** method:

```cpp
// Decode and execute command.
cr::lens::LensCommand commandId{cr::lens::LensCommand::ZOOM_TELE};
cr::lens::LensParam paramId{cr::lens::LensParam::ZOOM_POS};
float value{0.0f};
switch (cr::lens::Lens::decodeCommand(data, size, paramId, commandId, value))
{
// COMMAND.
case 0:
    return executeCommand(commandId, value);
// SET_PARAM.
case 1:
    return setParam(paramId, value);
// Error.
default:
    return false;
}
```



# Data structures



## LensCommand enum

Enum declaration:

```cpp
namespace cr
{
namespace lens
{
/**
 * @brief Lens commands enum.
 *
 * The IDs are consecutive and start from 1. Lens::decodeCommand(...) accepts
 * the IDs from ZOOM_TELE to the last enumerator (DETECT_HW_RANGES). A command
 * added to the enum must be placed at the end, and the check in Lens.cpp must
 * use the last enumerator.
 */
enum class LensCommand
{
    /// Move zoom tele (in). Command doesn't have arguments. User should be able
    /// to set zoom movement speed via lens parameters.
    ZOOM_TELE = 1,
    /// Move zoom wide (out). Command doesn't have arguments. User should be
    /// able to set zoom movement speed via lens parameters.
    ZOOM_WIDE,
    /// Move zoom to position. Lens controller should have zoom range from
    /// 0 (full wide) to 65535 (full tele) regardless of the hardware value of
    /// the zoom position. If the minimum and maximum zoom position limits are
    /// set by the user in the lens parameters, the range of the hardware zoom
    /// position must be scaled to the user space 0-65535 range.
    /// Command argument: zoom position 0-65535. User should be able to set zoom
    /// movement speed via lens parameters.
    ZOOM_TO_POS,
    /// Stop zoom moving including stop zoom to position command.
    ZOOM_STOP,
    /// Move focus far. Command doesn't have arguments. User should be able to
    /// set focus movement speed via lens parameters.
    FOCUS_FAR,
    /// Move focus near. Command doesn't have arguments. User should be able to
    /// set focus movement speed via lens parameters.
    FOCUS_NEAR,
    /// Move focus to position. Lens controller should have focus range from 0
    /// (full near) to 65535 (full far) regardless of the hardware value of the
    /// focus position. If the minimum and maximum focus position limits are
    /// set by the user in the lens parameters, the range of the hardware focus
    /// position must be scaled to the 0-65535 user space range.
    /// Command argument: focus position 0-65535. User should be able to set
    /// focus movement speed via lens parameters.
    FOCUS_TO_POS,
    /// Stop focus moving including stop focus to position command.
    FOCUS_STOP,
    /// Move iris open. Command doesn't have arguments. User should be able to
    /// set iris movement speed via lens parameters.
    IRIS_OPEN,
    /// Move iris close. Command doesn't have arguments. User should be able
    /// to set iris movement speed via lens parameters.
    IRIS_CLOSE,
    /// Move iris to position. Lens controller should have iris range
    /// from 0 (full close) to 65535 (full far) regardless of the hardware
    /// value of the iris position. If the minimum and maximum iris position
    /// limits are set by the user in the lens parameters, the range of the
    /// hardware iris position must be scaled to the 0-65535 user space range.
    /// Command argument: iris position 0-65535. User should be able to set
    /// iris movement speed via lens parameters.
    IRIS_TO_POS,
    /// Stop iris moving including stop iris to position command.
    /// Command doesn't have arguments.
    IRIS_STOP,
    /// Start autofocus.  Command doesn't have arguments.
    AF_START,
    /// Stop autofocus.  Command doesn't have arguments.
    AF_STOP,
    /// Restart lens controller.
    RESTART,
    /// Detect zoom and focus hardware ranges. After execution this command the
    /// lens controller should automatically set at least parameters
    /// (LensParam enum): ZOOM_HW_TELE_LIMIT, ZOOM_HW_WIDE_LIMIT,
    /// FOCUS_HW_FAR_LIMIT and FOCUS_HW_NEAR_LIMIT.
    DETECT_HW_RANGES
};
}
}
```

**Table 2** - Lens commands description. Some commands may be unsupported by particular lens controller.

| Command          | Description                                                  |
| ---------------- | ------------------------------------------------------------ |
| ZOOM_TELE        | Move zoom tele (in). Command doesn't have arguments. User should be able to set zoom movement speed via lens parameters. |
| ZOOM_WIDE        | Move zoom wide (out). Command doesn't have arguments. User should be able to set zoom movement speed via lens parameters. |
| ZOOM_TO_POS      | Move zoom to position. Lens controller should have zoom range from 0 (full wide) to 65535 (full tele) regardless of the hardware value of the zoom position. If the minimum and maximum zoom position limits are set by the user in the lens parameters, the range of the hardware zoom position must be scaled to the user space 0-65535 range. Command argument: zoom position 0-65535. User should be able to set zoom movement speed via lens parameters. |
| ZOOM_STOP        | Stop zoom moving including stop zoom to position command.    |
| FOCUS_FAR        | Move focus far. Command doesn't have arguments. User should be able to set focus movement speed via lens parameters. |
| FOCUS_NEAR       | Move focus near. Command doesn't have arguments. User should be able to set focus movement speed via lens parameters. |
| FOCUS_TO_POS     | Move focus to position. Lens controller should have focus range from 0 (full near) to 65535 (full far) regardless of the hardware value of the focus position. If the minimum and maximum focus position limits are set by the user in the lens parameters, the range of the hardware focus position must be scaled to the 0-65535 user space range. Command argument: focus position 0-65535. User should be able to set focus movement speed via lens parameters. |
| FOCUS_STOP       | Stop focus moving including stop focus to position command.  |
| IRIS_OPEN        | Move iris open. Command doesn't have arguments. User should be able to set iris movement speed via lens parameters. |
| IRIS_CLOSE       | Move iris close. Command doesn't have arguments. User should be able to set iris movement speed via lens parameters. |
| IRIS_TO_POS      | Move iris to position. Lens controller should have iris range from 0 (full close) to 65535 (full far) regardless of the hardware value of the iris position. If the minimum and maximum iris position limits are set by the user in the lens parameters, the range of the hardware iris position must be scaled to the 0-65535 user space range. Command argument: iris position 0-65535. User should be able to set iris movement speed via lens parameters. |
| IRIS_STOP        | Stop iris moving including stop iris to position command. Command doesn't have arguments. |
| AF_START         | Start autofocus. Command doesn't have arguments.            |
| AF_STOP          | Stop autofocus. Command doesn't have arguments.             |
| RESTART          | Restart lens controller.                                     |
| DETECT_HW_RANGES | Detect zoom and focus hardware ranges. After execution this command the lens controller should automatically set at least parameters ([LensParam](#lensparam-enum) enum): ZOOM_HW_TELE_LIMIT, ZOOM_HW_WIDE_LIMIT, FOCUS_HW_FAR_LIMIT and FOCUS_HW_NEAR_LIMIT. |



## LensParam enum

Enum declaration:

```cpp
namespace cr
{
namespace lens
{
/**
 * @brief Lens params enum.
 *
 * The IDs are consecutive and start from 1. Lens::decodeCommand(...) accepts
 * the IDs from ZOOM_POS to the last enumerator (CUSTOM_3). A param added to
 * the enum must be placed at the end, and the check in Lens.cpp must use the
 * last enumerator.
 */
enum class LensParam
{
    /// Zoom position. Setting a parameter is equivalent to the command
    /// ZOOM_TO_POS. Lens controller should have zoom range from 0 (full wide)
    /// to 65535 (full tele) regardless of the hardware value of the zoom
    /// position. If the minimum and maximum zoom position limits are set by
    /// the user in the lens parameters, the range of the hardware zoom position
    /// must be scaled to the user space 0-65535 range. Parameter value: zoom
    /// position 0-65535. User should be able to set zoom movement speed via
    /// lens parameters.
    ZOOM_POS = 1,
    /// Hardware zoom position. Parameter value depends on implementation and
    /// lens hardware.
    ZOOM_HW_POS,
    /// Focus position. Setting a parameter is equivalent to the command
    /// FOCUS_TO_POS. Lens controller should have focus range from 0 (full near)
    /// to 65535 (full far) regardless of the hardware value of the focus
    /// position. If the minimum and maximum focus position limits are set by
    /// the user in the lens parameters, the range of the hardware focus
    /// position must be scaled to the 0-65535 user space range. Parameter
    /// value: focus position 0-65535. User should be able to set focus movement
    /// speed via lens parameters.
    FOCUS_POS,
    /// Hardware focus position. Parameter value depends on implementation and
    /// lens hardware.
    FOCUS_HW_POS,
    /// Iris position. Setting a parameter is equivalent to the command
    /// IRIS_TO_POS. Lens controller should have iris range from 0 (full close)
    /// to 65535 (full far) regardless of the hardware value of the iris
    /// position. If the minimum and maximum iris position limits are set by the
    /// user in the lens parameters, the range of the hardware iris position
    /// must be scaled to the 0-65535 user space range. Parameter value: iris
    /// position 0-65535. User should be able to set iris movement speed via
    /// lens parameters.
    IRIS_POS,
    /// Hardware iris position. Parameter value depends on particular lens
    /// controller.
    IRIS_HW_POS,
    /// Focus mode. Parameter value depends on implementation but it is
    /// recommended to keep default values: 0 - Manual focus control,
    /// 1 - Auto focus control.
    FOCUS_MODE,
    /// Filter mode. Parameter value depends on implementation but it is
    /// recommended to keep default values: 0 - Filter on, 1 - Filter off.
    FILTER_MODE,
    /// Autofocus ROI top-left corner horizontal position in pixels.
    /// Autofocus ROI is rectangle.
    AF_ROI_X0,
    /// Autofocus ROI top-left corner vertical position in pixels.
    /// Autofocus ROI is rectangle.
    AF_ROI_Y0,
    /// Autofocus ROI bottom-right corner horizontal position in pixels.
    /// Autofocus ROI is rectangle.
    AF_ROI_X1,
    /// Autofocus ROI bottom-right corner vertical position in pixels.
    /// Autofocus ROI is rectangle.
    AF_ROI_Y1,
    /// Zoom speed. Lens controller should have zoom speed range from 0 to
    /// 100% of max hardware zoom speed (parameter ZOOM_HW_MAX_SPEED).
    /// If the user sets a new parameter value of the ZOOM_HW_SPEED the
    /// parameter ZOOM_SPEED must be updated automatically. Formula for
    /// calculating speed:
    /// ZOOM_SPEED = ( ZOOM_HW_SPEED / ZOOM_HW_MAX_SPEED) * 100.
    ZOOM_SPEED,
    /// Zoom hardware speed. Value depends on implementation and lens hardware.
    /// The value of the parameters must be <= ZOOM_HW_MAX_SPEED parameter.
    /// If the user sets a new parameter value of the ZOOM_SPEED parameter
    /// the parameter ZOOM_HW_SPEED must be updated automatically.
    /// Formula for calculating hardware speed:
    /// ZOOM_HW_SPEED = ( ZOOM_SPEED / 100 ) * ZOOM_HW_MAX_SPEED.
    ZOOM_HW_SPEED,
    /// Maximum zoom hardware speed. Value depends on implementation.
    /// If user sets new ZOOM_HW_MAX_SPEED value the parameters
    /// ZOOM_SPEED must be updated automatically. If new value of
    /// ZOOM_HW_MAX_SPEED parameter will be less than ZOOM_HW_SPEED the
    /// parameter ZOOM_HW_SPEED must be reduced automatically.
    ZOOM_HW_MAX_SPEED,
    /// Focus speed. Lens controller should have focus speed range from 0 to
    /// 100% of max hardware focus speed (parameter FOCUS_HW_MAX_SPEED).
    /// If the user sets a new parameter value of the FOCUS_HW_SPEED the
    /// parameter FOCUS_SPEED must be updated automatically. Formula for
    /// calculating speed: FOCUS_SPEED = ( FOCUS_HW_SPEED / FOCUS_HW_MAX_SPEED)
    /// * 100.
    FOCUS_SPEED,
    /// Focus hardware speed. Value depends on on implementation and lens
    /// hardware. The value of the parameters must be <= FOCUS_HW_MAX_SPEED
    /// parameter. If the user sets a new parameter value of the FOCUS_SPEED
    /// parameter the parameter FOCUS_HW_SPEED must be updated automatically.
    /// Formula for calculating hardware speed:
    /// FOCUS_HW_SPEED = ( FOCUS_SPEED / 100 ) * FOCUS_HW_MAX_SPEED.
    FOCUS_HW_SPEED,
    /// Maximum focus hardware speed. Value depends on implementation.
    /// If user sets new FOCUS_HW_MAX_SPEED value the parameters
    /// FOCUS_SPEED and FOCUS_HW_SPEED must be updated by lens controller
    /// automatically. If new value of FOCUS_HW_MAX_SPEED parameter will be
    /// less than FOCUS_HW_SPEED the parameter FOCUS_HW_SPEED must be reduced
    /// automatically.
    FOCUS_HW_MAX_SPEED,
    /// Iris speed. Lens controller should have iris speed range from 0 to 100%
    /// of max hardware iris speed (parameter IRIS_HW_MAX_SPEED). If the user
    /// sets a new parameter value of the IRIS_HW_SPEED the parameter IRIS_SPEED
    /// must be updated automatically. Formula for calculating speed:
    /// IRIS_SPEED = ( IRIS_HW_SPEED / IRIS_HW_MAX_SPEED) * 100.
    IRIS_SPEED,
    /// Iris hardware speed. Value depends on implementation and lens hardware.
    /// The value of the parameters must be <= IRIS_HW_MAX_SPEED parameter.
    /// If the user sets a new parameter value of the IRIS_SPEED parameter
    /// the parameter IRIS_HW_SPEED must be updated automatically. Formula
    /// for calculating hardware speed:
    /// IRIS_HW_SPEED = ( IRIS_SPEED / 100 ) * IRIS_HW_MAX_SPEED.
    IRIS_HW_SPEED,
    /// Maximum iris hardware speed. Value depends on implementation. If user
    /// sets new IRIS_HW_MAX_SPEED value the parameters IRIS_SPEED and
    /// IRIS_HW_SPEED must be updated automatically. If new value of
    /// IRIS_HW_MAX_SPEED parameter will be less than IRIS_HW_SPEED the
    /// parameter IRIS_HW_SPEED must be reduced automatically.
    IRIS_HW_MAX_SPEED,
    /// Zoom hardware tele limit. Value depends on implementation and lens
    /// hardware. Lens controller should control zoom position. Lens controller
    /// should stop zoom moving if hardware zoom position will be out of limits.
    ZOOM_HW_TELE_LIMIT,
    /// Zoom hardware wide limit. Value depends on implementation and lens
    /// hardware. Lens controller should control zoom position. Lens controller
    /// should stop zoom moving if hardware zoom position will be out of limits.
    ZOOM_HW_WIDE_LIMIT,
    /// Focus hardware far limit. Value depends on on implementation and lens
    /// hardware. Lens controller should control focus position. Lens controller
    /// should stop focus moving if hardware focus position will be out of
    /// limits.
    FOCUS_HW_FAR_LIMIT,
    /// Focus hardware near limit. Value depends on implementation and lens
    /// hardware. Lens controller should control focus position. Lens controller
    /// should stop focus moving if hardware focus position will be out of
    /// limits.
    FOCUS_HW_NEAR_LIMIT,
    /// Iris hardware open limit. Value depends on implementation and lens
    /// hardware. Lens controller should control iris position. Lens controller
    /// should stop iris moving if hardware iris position will be out of limits.
    IRIS_HW_OPEN_LIMIT,
    /// Iris hardware close limit. Value depends on implementation and lens
    /// hardware. Lens controller should control iris position. Lens controller
    /// should stop iris moving if hardware iris position will be out of limits.
    IRIS_HW_CLOSE_LIMIT,
    /// Focus factor if it was calculated. If not calculated must be -1.
    /// Value depends on particular lens controller.
    FOCUS_FACTOR,
    /// Lens connection status. Connection status shows if the lens controller
    /// has data exchange with lens equipment. For example, if lens has serial
    /// port lens controller connects to serial port
    /// (opens serial port file in OS) but lens can be not active (no power).
    /// In this case connection status shows that lens controller doesn't have
    /// data exchange with lens equipment (the value is 0). If lens controller
    /// has data exchange with lens equipment the value is 1. If lens controller
    /// not initialize the connection status always FALSE. Value: 0 - not
    /// connected. 1 - connected.
    IS_CONNECTED,
    /// Focus hardware speed in autofocus mode. Value depends on implementation
    /// and lens hardware.
    FOCUS_HW_AF_SPEED,
    /// Threshold for changes of focus factor to start refocus. Value:
    /// 0% - no check, 100% - changing x2.
    FOCUS_FACTOR_THRESHOLD,
    /// Timeout for automatic refocus in seconds. Value:
    /// 0 - no automatic refocus, 100000 - maximum value.
    REFOCUS_TIMEOUT_SEC,
    /// Flag about active autofocus algorithm. Value: 0 - autofocus not working,
    /// 1 - working.
    AF_IS_ACTIVE,
    /// Iris mode. Value depends on implementation but it is recommended to keep
    /// default values: 0 - manual iris control, 1 - auto iris control.
    IRIS_MODE,
    /// ROI width (pixels) for autofocus algorithm when lens controller detects
    /// ROI position automatically. Value: from 8 to (video frame width -
    /// AUTO_AF_ROI_BORDER * 2).
    AUTO_AF_ROI_WIDTH,
    /// ROI height (pixels) for autofocus algorithm when lens controller
    /// detects ROI position automatically. Value: from 8 to
    /// (video frame height - AUTO_AF_ROI_BORDER * 2).
    AUTO_AF_ROI_HEIGHT,
    /// Video frame border size (along vertical and horizontal axes).
    /// Value: border size from 0 to video frame
    /// min(video frame width/height) / 2.
    AUTO_AF_ROI_BORDER,
    /// AF ROI mode (write/read). Value: 0 - Manual position, 1 - Auto position.
    AF_ROI_MODE,
    /// Lens extender mode. Value depends on implementation but it is
    /// recommended to keep default values: 0 - no extender, 1 - x2 extender.
    EXTENDER_MODE,
    /// Lens stabilization mode. Value depends on implementation but it is
    /// recommended to keep default values: 0 - no stabilization,
    /// 1 - stabilization.
    STABILIZER_MODE,
    /// Autofocus range. Value depends on implementation.
    AF_RANGE,
    /// Current horizontal Field of view, degree. Field of view calculated by
    /// lens controller according to initial params or by reading directly from
    /// lens hardware.
    X_FOV_DEG,
    /// Current vertical Field of view, degree. Field of view calculated by lens
    /// controller according to initial params or by reading directly from lens
    /// hardware.
    Y_FOV_DEG,
    /// Logging mode. Values: 0 - Disable, 1 - Only file, 2 - Only terminal,
    /// 3 - File and terminal.
    LOG_MODE,
    /// Lens temperature, degree.
    TEMPERATURE,
    /// Lens controller initialization status. Open status shows if the lens
    /// controller initialized or not but doesn't show if lens controller has
    /// communication with lens equipment. For example, if lens has serial port
    /// lens controller connects to serial port (opens serial port file in OS)
    /// but lens can be not active (no power). In this case open status just
    /// shows that lens controller has opened serial port. Values: 0 - not open
    /// (not initialized), 1 - open (initialized).
    IS_OPEN,
    /// Lens type. Value depends on implementation. Type allows to lens
    /// initialize necessary parameters for particular lens hardware.
    TYPE,
    /// Lens custom parameter. Value depends on particular lens controller.
    /// Custom parameters used when particular lens equipment has specific
    /// unusual parameter.
    CUSTOM_1,
    /// Lens custom parameter. Value depends on particular lens controller.
    /// Custom parameters used when particular lens equipment has specific
    /// unusual parameter.
    CUSTOM_2,
    /// Lens custom parameter. Value depends on particular lens controller.
    /// Custom parameters used when particular lens equipment has specific
    /// unusual parameter.
    CUSTOM_3
};
}
}
```

**Table 3** - Lens params description. Some params may be unsupported by particular lens controller.

| Parameter              | Access       | Description                                                  |
| ---------------------- | ------------ | ------------------------------------------------------------ |
| ZOOM_POS               | read / write | Zoom position. Setting a parameter is equivalent to the command ZOOM_TO_POS ([LensCommand](#lenscommand-enum) enum). Lens controller should have zoom range from 0 (full wide) to 65535 (full tele) regardless of the hardware value of the zoom position. If the minimum and maximum zoom position limits are set by the user in the lens parameters, the range of the hardware zoom position must be scaled to the user space 0-65535 range. Parameter value: zoom position 0-65535. User should be able to set zoom movement speed via lens parameters. |
| ZOOM_HW_POS            | read / write | Hardware zoom position. Parameter value depends on implementation and lens hardware. |
| FOCUS_POS              | read / write | Focus position. Setting a parameter is equivalent to the command FOCUS_TO_POS ([LensCommand](#lenscommand-enum) enum). Lens controller should have focus range from 0 (full near) to 65535 (full far) regardless of the hardware value of the focus position. If the minimum and maximum focus position limits are set by the user in the lens parameters, the range of the hardware focus position must be scaled to the 0-65535 user space range. Parameter value: focus position 0-65535. User should be able to set focus movement speed via lens parameters. |
| FOCUS_HW_POS           | read / write | Hardware focus position. Parameter value depends on implementation and lens hardware. |
| IRIS_POS               | read / write | Iris position. Setting a parameter is equivalent to the command IRIS_TO_POS ([LensCommand](#lenscommand-enum) enum). Lens controller should have iris range from 0 (full close) to 65535 (full far) regardless of the hardware value of the iris position. If the minimum and maximum iris position limits are set by the user in the lens parameters, the range of the hardware iris position must be scaled to the 0-65535 user space range. Parameter value: iris position 0-65535. User should be able to set iris movement speed via lens parameters. |
| IRIS_HW_POS            | read / write | Hardware iris position. Parameter value depends on particular lens controller. |
| FOCUS_MODE             | read / write | Focus mode. Parameter value depends on implementation but it is recommended to keep default values: 0 - Manual focus control, 1 - Auto focus control. |
| FILTER_MODE            | read / write | Filter mode. Parameter value depends on implementation but it is recommended to keep default values: 0 - Filter on, 1 - Filter off. |
| AF_ROI_X0              | read / write | Autofocus ROI top-left corner horizontal position in pixels. Autofocus ROI is rectangle. |
| AF_ROI_Y0              | read / write | Autofocus ROI top-left corner vertical position in pixels. Autofocus ROI is rectangle. |
| AF_ROI_X1              | read / write | Autofocus ROI bottom-right corner horizontal position in pixels. Autofocus ROI is rectangle. |
| AF_ROI_Y1              | read / write | Autofocus ROI bottom-right corner vertical position in pixels. Autofocus ROI is rectangle. |
| ZOOM_SPEED             | read / write | Zoom speed. Lens controller should have zoom speed range from 0 to 100% of max hardware zoom speed (parameter ZOOM_HW_MAX_SPEED). If the user sets a new parameter value of the ZOOM_HW_SPEED the parameter ZOOM_SPEED must be updated automatically. Formula for calculating speed: ZOOM_SPEED = ( ZOOM_HW_SPEED / ZOOM_HW_MAX_SPEED) * 100. |
| ZOOM_HW_SPEED          | read / write | Zoom hardware speed. Value depends on implementation and lens hardware. The value of the parameters must be <= ZOOM_HW_MAX_SPEED parameter. If the user sets a new parameter value of the ZOOM_SPEED parameter the parameter ZOOM_HW_SPEED must be updated automatically. Formula for calculating hardware speed: ZOOM_HW_SPEED = ( ZOOM_SPEED / 100 ) * ZOOM_HW_MAX_SPEED. |
| ZOOM_HW_MAX_SPEED      | read / write | Maximum zoom hardware speed. Value depends on implementation. If user sets new ZOOM_HW_MAX_SPEED value the parameter ZOOM_SPEED must be updated automatically. If new value of ZOOM_HW_MAX_SPEED parameter will be less than ZOOM_HW_SPEED the parameter ZOOM_HW_SPEED must be reduced automatically. |
| FOCUS_SPEED            | read / write | Focus speed. Lens controller should have focus speed range from 0 to 100% of max hardware focus speed (parameter FOCUS_HW_MAX_SPEED). If the user sets a new parameter value of the FOCUS_HW_SPEED the parameter FOCUS_SPEED must be updated automatically. Formula for calculating speed: FOCUS_SPEED = ( FOCUS_HW_SPEED / FOCUS_HW_MAX_SPEED) * 100. |
| FOCUS_HW_SPEED         | read / write | Focus hardware speed. Value depends on on implementation and lens hardware. The value of the parameters must be <= FOCUS_HW_MAX_SPEED parameter. If the user sets a new parameter value of the FOCUS_SPEED parameter the parameter FOCUS_HW_SPEED must be updated automatically. Formula for calculating hardware speed: FOCUS_HW_SPEED = ( FOCUS_SPEED / 100 ) * FOCUS_HW_MAX_SPEED. |
| FOCUS_HW_MAX_SPEED     | read / write | Maximum focus hardware speed. Value depends on implementation. If user sets new FOCUS_HW_MAX_SPEED value the parameter FOCUS_SPEED must be updated by lens controller automatically. If new value of FOCUS_HW_MAX_SPEED parameter will be less than FOCUS_HW_SPEED the parameter FOCUS_HW_SPEED must be reduced automatically. |
| IRIS_SPEED             | read / write | Iris speed. Lens controller should have iris speed range from 0 to 100% of max hardware iris speed (parameter IRIS_HW_MAX_SPEED). If the user sets a new parameter value of the IRIS_HW_SPEED the parameter IRIS_SPEED must be updated automatically. Formula for calculating speed: IRIS_SPEED = ( IRIS_HW_SPEED / IRIS_HW_MAX_SPEED) * 100. |
| IRIS_HW_SPEED          | read / write | Iris hardware speed. Value depends on implementation and lens hardware. The value of the parameters must be <= IRIS_HW_MAX_SPEED parameter. If the user sets a new parameter value of the IRIS_SPEED parameter the parameter IRIS_HW_SPEED must be updated automatically. Formula for calculating hardware speed: IRIS_HW_SPEED = ( IRIS_SPEED / 100 ) * IRIS_HW_MAX_SPEED. |
| IRIS_HW_MAX_SPEED      | read / write | Maximum iris hardware speed. Value depends on implementation. If user sets new IRIS_HW_MAX_SPEED value the parameter IRIS_SPEED must be updated automatically. If new value of IRIS_HW_MAX_SPEED parameter will be less than IRIS_HW_SPEED the parameter IRIS_HW_SPEED must be reduced automatically. |
| ZOOM_HW_TELE_LIMIT     | read / write | Zoom hardware tele limit. Value depends on implementation and lens hardware. Lens controller should control zoom position. Lens controller should stop zoom moving if hardware zoom position will be out of limits. |
| ZOOM_HW_WIDE_LIMIT     | read / write | Zoom hardware wide limit. Value depends on implementation and lens hardware. Lens controller should control zoom position. Lens controller should stop zoom moving if hardware zoom position will be out of limits. |
| FOCUS_HW_FAR_LIMIT     | read / write | Focus hardware far limit. Value depends on on implementation and lens hardware. Lens controller should control focus position. Lens controller should stop focus moving if hardware focus position will be out of limits. |
| FOCUS_HW_NEAR_LIMIT    | read / write | Focus hardware near limit. Value depends on implementation and lens hardware. Lens controller should control focus position. Lens controller should stop focus moving if hardware focus position will be out of limits. |
| IRIS_HW_OPEN_LIMIT     | read / write | Iris hardware open limit. Value depends on implementation and lens hardware. Lens controller should control iris position. Lens controller should stop iris moving if hardware iris position will be out of limits. |
| IRIS_HW_CLOSE_LIMIT    | read / write | Iris hardware close limit. Value depends on implementation and lens hardware. Lens controller should control iris position. Lens controller should stop iris moving if hardware iris position will be out of limits. |
| FOCUS_FACTOR           | read only    | Focus factor if it was calculated. If not calculated must be -1. Value depends on particular lens controller. |
| IS_CONNECTED           | read only    | Lens connection status. Connection status shows if the lens controller has data exchange with lens equipment. For example, if lens has serial port lens controller connects to serial port (opens serial port file in OS) but lens can be not active (no power). In this case connection status shows that lens controller doesn't have data exchange with lens equipment (method will return 0). If lens controller has data exchange with lens equipment the method will return 1. If lens controller not initialized the connection status always 0. Value: 0 - not connected. 1 - connected. |
| FOCUS_HW_AF_SPEED      | read / write | Focus hardware speed in autofocus mode. Value depends on implementation and lens hardware. |
| FOCUS_FACTOR_THRESHOLD | read / write | Focus factor changes threshold to start refocus. Value: 0% - no check, 100% - changing x2. |
| REFOCUS_TIMEOUT_SEC    | read / write | Timeout for automatic refocus in seconds. Value: 0 - no automatic refocus, 100000 - maximum value. |
| AF_IS_ACTIVE           | read only    | Flag about active autofocus algorithm. Value: 0 - autofocus not working, 1 - working. |
| IRIS_MODE              | read / write | Iris mode. Value depends on implementation but it is recommended to keep default values: 0 - manual iris control, 1 - auto iris control. |
| AUTO_AF_ROI_WIDTH      | read / write | ROI width (pixels) for autofocus algorithm when lens controller detects ROI position automatically. Value: from 8 to (video frame width - AUTO_AF_ROI_BORDER * 2). |
| AUTO_AF_ROI_HEIGHT     | read / write | ROI height (pixels) for autofocus algorithm when lens controller detects ROI position automatically. Value: from 8 to (video frame height - AUTO_AF_ROI_BORDER * 2). |
| AUTO_AF_ROI_BORDER     | read / write | Video frame border size (along vertical and horizontal axes). Value: border size from 0 to minimum of video frame width / 2 and height / 2. |
| AF_ROI_MODE            | read / write | AF ROI mode. Value: 0 - Manual position, 1 - Auto position. |
| EXTENDER_MODE          | read / write | Lens extender mode. Value depends on implementation but it is recommended to keep default values: 0 - no extender, 1 - x2 extender. |
| STABILIZER_MODE        | read / write | Lens stabilization mode. Value depends on implementation but it is recommended to keep default values: 0 - no stabilization, 1 - stabilization. |
| AF_RANGE               | read / write | Autofocus range. Value depends on implementation and lens hardware. |
| X_FOV_DEG              | read only    | Current horizontal Field of view, degree. Field of view calculated by lens controller according to initial params or by reading directly from lens hardware. |
| Y_FOV_DEG              | read only    | Current vertical Field of view, degree. Field of view calculated by lens controller according to initial params or by reading directly from lens hardware. |
| LOG_MODE               | read / write | Logging mode. Values: 0 - Disable, 1 - Only file, 2 - Only terminal, 3 - File and terminal. |
| TEMPERATURE            | read only    | Lens temperature, degree.                                    |
| IS_OPEN                | read only    | Lens controller initialization status. Open status shows if the lens controller initialized or not but doesn't show if lens controller has communication with lens equipment. For example, if lens has serial port lens controller connects to serial port (opens serial port file in OS) but lens can be not active (no power). In this case open status just shows that lens controller has opened serial port. Values: 0 - not open (not initialized), 1 - open (initialized). |
| TYPE                   | read / write | Lens type. Value depends on implementation. Type allows lens initialize necessary parameters for particular lens hardware. |
| CUSTOM_1               | read / write | Lens custom parameter. Value depends on particular lens controller. Custom parameters used when particular lens equipment has specific unusual parameter. |
| CUSTOM_2               | read / write | Lens custom parameter. Value depends on particular lens controller. Custom parameters used when particular lens equipment has specific unusual parameter. |
| CUSTOM_3               | read / write | Lens custom parameter. Value depends on particular lens controller. Custom parameters used when particular lens equipment has specific unusual parameter. |



# LensParams class description

**LensParams** class used for lens controller initialization ([initLens(...)](#initlens-method) method) or to get all actual lens parameters ([getParams(...)](#getparams-method) method). Also **LensParams** provides structure to write/read params from JSON files (**JSON_READABLE** macro) and provides methods to encode and decode params.



## LensParams class declaration

**LensParams** interface class declared in **Lens.h** file. Class declaration:

```cpp
namespace cr
{
namespace lens
{
/**
 * @brief Field of view point class.
 *
 * @note The class is not thread-safe: an object must not be accessed from
 * several threads at the same time without external synchronization.
 */
class FovPoint
{
public:
    /// Hardware zoom pos.
    int hwZoomPos{0};
    /// Horizontal field of view, degree.
    float xFovDeg{0.0f};
    /// Vertical field of view, degree.
    float yFovDeg{0.0f};

    JSON_READABLE(FovPoint, hwZoomPos, xFovDeg, yFovDeg)

    /**
     * @brief Default constructor.
     */
    FovPoint() = default;

    /**
     * @brief Copy constructor.
     * @param src Source object.
     */
    FovPoint(const FovPoint& src) = default;

    /**
     * @brief Move constructor.
     * @param src Source object.
     */
    FovPoint(FovPoint&& src) = default;

    /**
     * @brief Copy assignment operator.
     * @param src Source object.
     * @return Reference to this object.
     */
    FovPoint& operator= (const FovPoint& src) = default;

    /**
     * @brief Move assignment operator.
     * @param src Source object.
     * @return Reference to this object.
     */
    FovPoint& operator= (FovPoint&& src) = default;
};

/**
 * @brief Lens params class.
 *
 * @note The class is not thread-safe: an object must not be accessed from
 * several threads at the same time without external synchronization.
 */
class LensParams
{
public:
    /// Initialization string. Particular lens controller can have unique init
    /// string format. But it is recommended to use '**;**' symbol to divide
    /// parts of initialization string. Recommended initialization string format
    /// for controllers which uses serial port: "/dev/ttyUSB0;9600;100"
    /// ("/dev/ttyUSB0" - serial port name, "9600" - baudrate, "100" - serial
    /// port read timeout).
    std::string initString{"/dev/ttyUSB0;9600;20"};
    /// Zoom position. Setting a parameter is equivalent to the command
    /// ZOOM_TO_POS. Lens controller should have zoom range from 0 (full wide)
    /// to 65535 (full tele) regardless of the hardware value of the zoom
    /// position. If the minimum and maximum zoom position limits are set by
    /// the user in the lens parameters, the range of the hardware zoom position
    /// must be scaled to the user space 0-65535 range. Parameter value: zoom
    /// position 0-65535. User should be able to set zoom movement speed via
    /// lens parameters.
    int zoomPos{0};
    /// Hardware zoom position. Parameter value depends on implementation and
    /// lens hardware.
    int zoomHwPos{0};
    /// Focus position. Setting a parameter is equivalent to the command
    /// FOCUS_TO_POS. Lens controller should have focus range from 0 (full near)
    /// to 65535 (full far) regardless of the hardware value of the focus
    /// position. If the minimum and maximum focus position limits are set by
    /// the user in the lens parameters, the range of the hardware focus
    /// position must be scaled to the 0-65535 user space range. Parameter
    /// value: focus position 0-65535. User should be able to set focus movement
    /// speed via lens parameters.
    int focusPos{0};
    /// Hardware focus position. Parameter value depends on implementation and
    /// lens hardware.
    int focusHwPos{0};
    /// Iris position. Setting a parameter is equivalent to the command
    /// IRIS_TO_POS. Lens controller should have iris range from 0 (full close)
    /// to 65535 (full far) regardless of the hardware value of the iris
    /// position. If the minimum and maximum iris position limits are set by the
    /// user in the lens parameters, the range of the hardware iris position
    /// must be scaled to the 0-65535 user space range. Parameter value: iris
    /// position 0-65535. User should be able to set iris movement speed via
    /// lens parameters.
    int irisPos{0};
    /// Hardware iris position. Parameter value depends on implementation.
    int irisHwPos{0};
    /// Focus mode. Parameter value depends on implementation but it is
    /// recommended to keep default values: 0 - Manual focus control,
    /// 1 - Auto focus control.
    int focusMode{0};
    /// Filter mode. Parameter value depends on implementation but it is
    /// recommended to keep default values: 0 - Filter on, 1 - Filter off.
    int filterMode{0};
    /// Autofocus ROI top-left corner horizontal position in pixels.
    /// Autofocus ROI is rectangle.
    int afRoiX0{0};
    /// Autofocus ROI top-left corner vertical position in pixels.
    /// Autofocus ROI is rectangle.
    int afRoiY0{0};
    /// Autofocus ROI bottom-right corner horizontal position in pixels.
    /// Autofocus ROI is rectangle.
    int afRoiX1{0};
    /// Autofocus ROI bottom-right corner vertical position in pixels.
    /// Autofocus ROI is rectangle.
    int afRoiY1{0};
    /// Zoom speed. Lens controller should have zoom speed range from 0 to
    /// 100% of max hardware zoom speed (parameter ZOOM_HW_MAX_SPEED). If the
    /// user sets a new parameter value of the ZOOM_HW_SPEED the parameter
    /// ZOOM_SPEED must be updated automatically. Formula for calculating speed:
    /// ZOOM_SPEED = ( ZOOM_HW_SPEED / ZOOM_HW_MAX_SPEED) * 100.
    int zoomSpeed{50};
    /// Zoom hardware speed. Value depends on implementation and lens hardware.
    /// The value of the parameters must be <= ZOOM_HW_MAX_SPEED parameter.
    /// If the user sets a new parameter value of the ZOOM_SPEED parameter
    /// the parameter ZOOM_HW_SPEED must be updated automatically. Formula for
    /// calculating hardware speed:
    /// ZOOM_HW_SPEED = ( ZOOM_SPEED / 100 ) * ZOOM_HW_MAX_SPEED.
    int zoomHwSpeed{50};
    /// Maximum zoom hardware speed. Value depends on implementation. If user
    /// sets new ZOOM_HW_MAX_SPEED value the parameters ZOOM_SPEED must be
    /// updated automatically. If new value of ZOOM_HW_MAX_SPEED parameter will
    /// be less than ZOOM_HW_SPEED the parameter ZOOM_HW_SPEED must be reduced
    /// automatically.
    int zoomHwMaxSpeed{50};
    /// Focus speed. Lens controller should have focus speed range from 0 to
    /// 100% of max hardware focus speed (parameter FOCUS_HW_MAX_SPEED). If the
    /// user sets a new parameter value of the FOCUS_HW_SPEED the parameter
    /// FOCUS_SPEED must be updated automatically. Formula for calculating
    /// speed: FOCUS_SPEED = ( FOCUS_HW_SPEED / FOCUS_HW_MAX_SPEED) * 100.
    int focusSpeed{50};
    /// Focus hardware speed. Value depends on on implementation and lens
    /// hardware. The value of the parameters must be <= FOCUS_HW_MAX_SPEED
    /// parameter. If the user sets a new parameter value of the FOCUS_SPEED
    /// parameter the parameter FOCUS_HW_SPEED must be updated automatically.
    /// Formula for calculating hardware speed:
    /// FOCUS_HW_SPEED = ( FOCUS_SPEED / 100 ) * FOCUS_HW_MAX_SPEED.
    int focusHwSpeed{50};
    /// Maximum focus hardware speed. Value depends on implementation. If user
    /// sets new FOCUS_HW_MAX_SPEED value the parameters FOCUS_SPEED and
    /// FOCUS_HW_SPEED must be updated by lens controller automatically.
    /// If new value of FOCUS_HW_MAX_SPEED parameter will be less than
    /// FOCUS_HW_SPEED the parameter FOCUS_HW_SPEED must be reduced
    /// automatically.
    int focusHwMaxSpeed{50};
    /// Iris speed. Lens controller should have iris speed range from 0 to 100%
    /// of max hardware iris speed (parameter IRIS_HW_MAX_SPEED). If the user
    /// sets a new parameter value of the IRIS_HW_SPEED the parameter IRIS_SPEED
    /// must be updated automatically. Formula for calculating speed:
    /// IRIS_SPEED = ( IRIS_HW_SPEED / IRIS_HW_MAX_SPEED) * 100.
    int irisSpeed{50};
    /// Iris hardware speed. Value depends on implementation and lens hardware.
    /// The value of the parameters must be <= IRIS_HW_MAX_SPEED parameter.
    /// If the user sets a new parameter value of the IRIS_SPEED parameter the
    /// parameter IRIS_HW_SPEED must be updated automatically. Formula for
    /// calculating hardware speed:
    /// IRIS_HW_SPEED = ( IRIS_SPEED / 100 ) * IRIS_HW_MAX_SPEED.
    int irisHwSpeed{50};
    /// Maximum iris hardware speed. Value depends on implementation. If user
    /// sets new IRIS_HW_MAX_SPEED value the parameters IRIS_SPEED and
    /// IRIS_HW_SPEED must be updated automatically. If new value of
    /// IRIS_HW_MAX_SPEED parameter will be less than IRIS_HW_SPEED the
    /// parameter IRIS_HW_SPEED must be reduced automatically.
    int irisHwMaxSpeed{50};
    /// Zoom hardware tele limit. Value depends on implementation and lens
    /// hardware. Lens controller should control zoom position. Lens controller
    /// should stop zoom moving if hardware zoom position will be out of limits.
    int zoomHwTeleLimit{65535};
    /// Zoom hardware wide limit. Value depends on implementation and lens
    /// hardware. Lens controller should control zoom position. Lens controller
    /// should stop zoom moving if hardware zoom position will be out of limits.
    int zoomHwWideLimit{0};
    /// Focus hardware far limit. Value depends on on implementation and lens
    /// hardware. Lens controller should control focus position. Lens controller
    /// should stop focus moving if hardware focus position will be out of
    /// limits.
    int focusHwFarLimit{65535};
    /// Focus hardware near limit. Value depends on implementation and lens
    /// hardware. Lens controller should control focus position. Lens controller
    /// should stop focus moving if hardware focus position will be out of
    /// limits.
    int focusHwNearLimit{0};
    /// Iris hardware open limit. Value depends on implementation and lens
    /// hardware. Lens controller should control iris position. Lens controller
    /// should stop iris moving if hardware iris position will be out of limits.
    int irisHwOpenLimit{65535};
    /// Iris hardware close limit. Value depends on implementation and lens
    /// hardware. Lens controller should control iris position. Lens controller
    /// should stop iris moving if hardware iris position will be out of limits.
    int irisHwCloseLimit{0};
    /// Focus factor if it was calculated. If not calculated must be -1.
    /// Value depends on particular lens controller.
    float focusFactor{0.0f};
    /// Lens connection status. Connection status shows if the lens controller
    /// has data exchange with lens equipment. For example, if lens has serial
    /// port lens controller connects to serial port (opens serial port file
    /// in OS) but lens can be not active (no power). In this case connection
    /// status shows that lens controller doesn't have data exchange with lens
    /// equipment. If lens controller has data exchange with lens equipment
    /// the value is TRUE. If lens controller not initialize the connection
    /// status always FALSE. Value: false - not connected. true - connected.
    bool isConnected{false};
    /// Focus hardware speed in autofocus mode. Value depends on implementation
    /// and lens hardware.
    int afHwSpeed{50};
    /// Threshold for changes of focus factor to start refocus. Value:
    /// 0% - no check, 100% - changing x2.
    float focusFactorThreshold{0.0f};
    /// Timeout for automatic refocus in seconds. Value:
    /// 0 - no automatic refocus, 100000 - maximum value.
    int refocusTimeoutSec{0};
    /// Flag about active autofocus algorithm. Value: 0 - autofocus not working,
    /// 1 - working.
    bool afIsActive{false};
    /// Iris mode. Value depends on implementation but it is recommended to keep
    /// default values: 0 - manual iris control, 1 - auto iris control.
    int irisMode{0};
    /// ROI width (pixels) for autofocus algorithm when lens controller detects
    /// ROI position automatically. Value: from 8 to (video frame width -
    /// AUTO_AF_ROI_BORDER * 2).
    int autoAfRoiWidth{150};
    /// ROI height (pixels) for autofocus algorithm when lens controller detects
    /// ROI position automatically. Value: from 8 to (video frame height -
    /// AUTO_AF_ROI_BORDER * 2).
    int autoAfRoiHeight{150};
    /// Video frame border size (along vertical and horizontal axes).
    /// Value: border size from 0 to video
    /// frame min(video frame width/height) / 2.
    int autoAfRoiBorder{100};
    /// AF ROI mode (write/read). Value: 0 - Manual position, 1 - Auto position.
    int afRoiMode{0};
    /// Lens extender mode. Value depends on implementation but it is
    /// recommended to keep default values: 0 - no extender, 1 - x2 extender.
    int extenderMode{0};
    /// Lens stabilization mode. Value depends on implementation but it is
    /// recommended to keep default values: 0 - no stabilization,
    /// 1 - stabilization.
    int stabiliserMode{0};
    /// Autofocus range. Value depends on implementation.
    int afRange{0};
    /// Current horizontal Field of view, degree. Field of view calculated by
    /// lens controller according to initial params or by reading directly from
    /// lens hardware.
    float xFovDeg{1.0f};
    /// Current vertical Field of view, degree. Field of view calculated by lens
    /// controller according to initial params or by reading directly from lens
    /// hardware.
    float yFovDeg{1.0f};
    /// Logging mode. Values: 0 - Disable, 1 - Only file, 2 - Only terminal,
    /// 3 - File and terminal.
    int logMode{0};
    /// Lens temperature, degree (read only).
    float temperature{0.0f};
    /// Lens controller initialization status. Open status shows if the lens
    /// controller initialized or not but doesn't show if lens controller has
    /// communication with lens equipment. For example, if lens has serial port
    /// lens controller connects to serial port (opens serial port file in OS)
    /// but lens can be not active (no power). In this case open status just
    /// shows that lens controller has opened serial port.
    /// Values: false - not open (not initialized), true - open (initialized).
    bool isOpen{false};
    /// Lens type. Value depends on implementation. Type allows to lens
    /// initialize necessary parameters for particular lens hardware.
    int type{0};
    /// Lens custom parameter. Value depends on particular lens controller.
    /// Custom parameters used when particular lens equipment has specific
    /// unusual parameter.
    float custom1{0.0f};
    /// Lens custom parameter. Value depends on particular lens controller.
    /// Custom parameters used when particular lens equipment has specific
    /// unusual parameter.
    float custom2{0.0f};
    /// Lens custom parameter. Value depends on particular lens controller.
    /// Custom parameters used when particular lens equipment has specific
    /// unusual parameter.
    float custom3{0.0f};
    /// List of points to calculate field of view. Lens controller should
    /// calculate FOV table according to given list of points using
    /// approximation.
    std::vector<FovPoint> fovPoints{std::vector<FovPoint>()};

    JSON_READABLE(LensParams, initString, focusMode, filterMode,
                  afRoiX0, afRoiY0, afRoiX1, afRoiY1, zoomHwMaxSpeed,
                  focusHwMaxSpeed, irisHwMaxSpeed, zoomHwTeleLimit,
                  zoomHwWideLimit, focusHwFarLimit, focusHwNearLimit,
                  irisHwOpenLimit, irisHwCloseLimit, afHwSpeed,
                  focusFactorThreshold, refocusTimeoutSec, irisMode,
                  autoAfRoiWidth, autoAfRoiHeight, autoAfRoiBorder,
                  afRoiMode, extenderMode, stabiliserMode, afRange,
                  logMode, type, custom1, custom2, custom3, fovPoints)

    /**
     * @brief Default constructor.
     */
    LensParams() = default;

    /**
     * @brief Copy constructor.
     * @param src Source object.
     */
    LensParams(const LensParams& src) = default;

    /**
     * @brief Move constructor.
     * @param src Source object.
     */
    LensParams(LensParams&& src) = default;

    /**
     * @brief Copy assignment operator.
     * @param src Source object.
     * @return Reference to this object.
     */
    LensParams& operator= (const LensParams& src) = default;

    /**
     * @brief Move assignment operator.
     * @param src Source object.
     * @return Reference to this object.
     */
    LensParams& operator= (LensParams&& src) = default;

    /**
     * @brief Encode (serialize) params.
     *
     * The method doesn't encode initString and fovPoints. The encoded data
     * consists of the header (data type, version Major and Minor, params mask
     * of 7 bytes: one bit for each field of LensParamsMask) and the values of
     * the selected params in the order of the LensParamsMask fields. Values of
     * int and float fields use 4 bytes in the byte order of the host, values
     * of bool fields use 1 byte.
     *
     * @param data Pointer to data buffer. Must not be nullptr and must be
     * valid for bufferSize bytes.
     * @param bufferSize Size of the data buffer, bytes. Must be >= 201 (the
     * size of the data with all params encoded).
     * @param size Size of the encoded data, bytes. Set to 0 if the params
     * are not encoded.
     * @param mask Pointer to params mask. If nullptr all params are encoded.
     * @return TRUE if params encoded or FALSE if not (data is nullptr or
     * bufferSize < 201).
     */
    bool encode(uint8_t* data, int bufferSize, int& size,
                LensParamsMask* mask = nullptr);

    /**
     * @brief Decode (deserialize) params encoded by encode(...) method.
     *
     * The method doesn't decode initString and fovPoints: initString is set
     * to empty string and fovPoints is cleared. The method checks data type,
     * version (Major and Minor) and params mask, and checks that each encoded
     * value is inside the data before reading it. Params which are not
     * encoded are set to 0 (FALSE for bool params). Bytes after the last
     * encoded value are ignored. Values of params are not checked against any
     * range: the lens controller must validate them before use.
     *
     * @param data Pointer to encoded data. Must not be nullptr and must be
     * valid for dataSize bytes.
     * @param dataSize Size of the encoded data, bytes.
     * @return TRUE if params decoded or FALSE if not. If FALSE is returned
     * the object is not changed.
     */
    bool decode(uint8_t* data, int dataSize);
};
}
}
```

**Table 4** - LensParams class fields description is equivalent to **LensParam** enum description.

| Field                | type     | Description                                                  |
| -------------------- | -------- | ------------------------------------------------------------ |
| initString           | string   | Initialization string (equivalent in initialization string format in [openLens(...)](#openlens-method) method). Particular lens controller can have unique init string format. But it is recommended to use '**;**' symbol to divide parts of initialization string. Recommended initialization string format for controllers which uses serial port: **"/dev/ttyUSB0;9600;100"** ("/dev/ttyUSB0" - serial port name, "9600" - baudrate, "100" - serial port read timeout). |
| zoomPos              | int      | Zoom position. Lens controller should have zoom range from 0 (full wide) to 65535 (full tele) regardless of the hardware value of the zoom position. If the minimum and maximum zoom position limits are set by the user in the lens parameters, the range of the hardware zoom position must be scaled to the user space 0-65535 range. Parameter value: zoom position 0-65535. User should be able to set zoom movement speed via lens parameters. |
| zoomHwPos            | int      | Hardware zoom position. Parameter value depends on implementation and lens hardware. |
| focusPos             | int      | Focus position. Lens controller should have focus range from 0 (full near) to 65535 (full far) regardless of the hardware value of the focus position. If the minimum and maximum focus position limits are set by the user in the lens parameters, the range of the hardware focus position must be scaled to the 0-65535 user space range. Parameter value: focus position 0-65535. User should be able to set focus movement speed via lens parameters. |
| focusHwPos           | int      | Hardware focus position. Parameter value depends on implementation and lens hardware. |
| irisPos              | int      | Iris position. Lens controller should have iris range from 0 (full close) to 65535 (full far) regardless of the hardware value of the iris position. If the minimum and maximum iris position limits are set by the user in the lens parameters, the range of the hardware iris position must be scaled to the 0-65535 user space range. Parameter value: iris position 0-65535. User should be able to set iris movement speed via lens parameters. |
| irisHwPos            | int      | Hardware iris position. Parameter value depends on implementation. |
| focusMode            | int      | Focus mode. Parameter value depends on implementation but it is recommended to keep default values: 0 - Manual focus control, 1 - Auto focus control. |
| filterMode           | int      | Filter mode. Parameter value depends on implementation but it is recommended to keep default values: 0 - Filter on, 1 - Filter off. |
| afRoiX0              | int      | Autofocus ROI top-left corner horizontal position in pixels. Autofocus ROI is rectangle. |
| afRoiY0              | int      | Autofocus ROI top-left corner vertical position in pixels. Autofocus ROI is rectangle. |
| afRoiX1              | int      | Autofocus ROI bottom-right corner horizontal position in pixels. Autofocus ROI is rectangle. |
| afRoiY1              | int      | Autofocus ROI bottom-right corner vertical position in pixels. Autofocus ROI is rectangle. |
| zoomSpeed            | int      | Zoom speed. Lens controller should have zoom speed range from 0 to 100% of max hardware zoom speed (parameter zoomHwMaxSpeed). If the user sets a new parameter value of the ZOOM_HW_SPEED (with [setParam(...)](#setparam-method) method) the parameter zoomSpeed must be updated automatically. Formula for calculating speed: zoomSpeed = ( zoomHwSpeed / zoomHwMaxSpeed) * 100. |
| zoomHwSpeed          | int      | Zoom hardware speed. Value depends on implementation and lens hardware. The value of the parameters must be <= zoomHwMaxSpeed parameter. If the user sets a new parameter value of the ZOOM_SPEED (with [setParam(...)](#setparam-method) method) parameter the parameter zoomHwSpeed must be updated automatically. Formula for calculating hardware speed: zoomHwSpeed = ( zoomSpeed / 100 ) * zoomHwMaxSpeed. |
| zoomHwMaxSpeed       | int      | Maximum zoom hardware speed. Value depends on implementation. If user sets new ZOOM_HW_MAX_SPEED value (with [setParam(...)](#setparam-method) method) the parameter zoomSpeed must be updated automatically. If new value of zoomHwMaxSpeed parameter will be less than zoomHwSpeed the parameter zoomHwSpeed must be reduced automatically. |
| focusSpeed           | int      | Focus speed. Lens controller should have focus speed range from 0 to 100% of max hardware focus speed (parameter focusHwMaxSpeed). If the user sets a new parameter value of the FOCUS_HW_SPEED (with [setParam(...)](#setparam-method) method) the parameter focusSpeed must be updated automatically. Formula for calculating speed: focusSpeed = ( focusHwSpeed / focusHwMaxSpeed) * 100. |
| focusHwSpeed         | int      | Focus hardware speed. Value depends on on implementation and lens hardware. The value of the parameters must be <= focusHwMaxSpeed parameter. If the user sets a new parameter value of the FOCUS_SPEED parameter (with [setParam(...)](#setparam-method) method) the parameter focusHwSpeed must be updated automatically. Formula for calculating hardware speed: focusHwSpeed = ( focusSpeed / 100 ) * focusHwMaxSpeed. |
| focusHwMaxSpeed      | int      | Maximum focus hardware speed. Value depends on implementation. If user sets new FOCUS_HW_MAX_SPEED value (with [setParam(...)](#setparam-method) method) the parameter focusSpeed must be updated by lens controller automatically. If new value of focusHwMaxSpeed parameter will be less than focusHwSpeed the parameter focusHwSpeed must be reduced automatically. |
| irisSpeed            | int      | Iris speed. Lens controller should have iris speed range from 0 to 100% of max hardware iris speed (parameter irisHwMaxSpeed). If the user sets a new parameter value of the IRIS_HW_SPEED (with [setParam(...)](#setparam-method) method) the parameter irisSpeed must be updated automatically. Formula for calculating speed: irisSpeed = ( irisHwSpeed / irisHwMaxSpeed) * 100. |
| irisHwSpeed          | int      | Iris hardware speed. Value depends on implementation and lens hardware. The value of the parameters must be <= IRIS_HW_MAX_SPEED parameter. If the user sets a new parameter value of the IRIS_SPEED (with [setParam(...)](#setparam-method) method) the parameter irisHwSpeed must be updated automatically. Formula for calculating hardware speed: irisHwSpeed = ( irisSpeed / 100 ) * irisHwMaxSpeed. |
| irisHwMaxSpeed       | int      | Maximum iris hardware speed. Value depends on implementation. If user sets new IRIS_HW_MAX_SPEED (with [setParam(...)](#setparam-method) method) the parameter irisSpeed must be updated automatically. If new value of irisHwMaxSpeed parameter will be less than irisHwSpeed the parameter irisHwSpeed must be reduced automatically. |
| zoomHwTeleLimit      | int      | Zoom hardware tele limit. Value depends on implementation and lens hardware. Lens controller should control zoom position. Lens controller should stop zoom moving if hardware zoom position will be out of limits. |
| zoomHwWideLimit      | int      | Zoom hardware wide limit. Value depends on implementation and lens hardware. Lens controller should control zoom position. Lens controller should stop zoom moving if hardware zoom position will be out of limits. |
| focusHwFarLimit      | int      | Focus hardware far limit. Value depends on on implementation and lens hardware. Lens controller should control focus position. Lens controller should stop focus moving if hardware focus position will be out of limits. |
| focusHwNearLimit     | int      | Focus hardware near limit. Value depends on implementation and lens hardware. Lens controller should control focus position. Lens controller should stop focus moving if hardware focus position will be out of limits. |
| irisHwOpenLimit      | int      | Iris hardware open limit. Value depends on implementation and lens hardware. Lens controller should control iris position. Lens controller should stop iris moving if hardware iris position will be out of limits. |
| irisHwCloseLimit     | int      | Iris hardware close limit. Value depends on implementation and lens hardware. Lens controller should control iris position. Lens controller should stop iris moving if hardware iris position will be out of limits. |
| focusFactor          | float    | Focus factor if it was calculated. If not calculated must be -1. Value depends on particular lens controller. |
| isConnected          | bool     | Lens connection status. Connection status shows if the lens controller has data exchange with lens equipment. For example, if lens has serial port lens controller connects to serial port (opens serial port file in OS) but lens can be not active (no power). In this case connection status shows that lens controller doesn't have data exchange with lens equipment. If lens controller has data exchange with lens equipment the parameter will be TRUE. If lens controller not initialize the connection status always FALSE. Value: false - not connected. true - connected. |
| afHwSpeed            | int      | Focus hardware speed in autofocus mode. Value depends on implementation and lens hardware. |
| focusFactorThreshold | float    | Threshold for changes of focus factor to start refocus. Value: 0% - no check, 100% - changing x2. |
| refocusTimeoutSec    | int      | Timeout for automatic refocus in seconds. Value: 0 - no automatic refocus, 100000 - maximum value. |
| afIsActive           | bool     | Flag about active autofocus algorithm. Value: 0 - autofocus not working, 1 - working. |
| irisMode             | int      | Iris mode. Value depends on implementation but it is recommended to keep default values: 0 - manual iris control, 1 - auto iris control. |
| autoAfRoiWidth       | int      | ROI width (pixels) for autofocus algorithm when lens controller detects ROI position automatically. Value: from 8 to (video frame width - autoAfRoiBorder * 2). |
| autoAfRoiHeight      | int      | ROI height (pixels) for autofocus algorithm when lens controller detects ROI position automatically. Value: from 8 to (video frame height - autoAfRoiBorder * 2). |
| autoAfRoiBorder      | int      | Video frame border size (along vertical and horizontal axes). Value: border size from 0 to minimum of video frame width / 2 and height / 2. |
| afRoiMode            | int      | AF ROI mode. Value: 0 - Manual position, 1 - Auto position. |
| extenderMode         | int      | Lens extender mode. Value depends on implementation but it is recommended to keep default values: 0 - no extender, 1 - x2 extender. |
| stabiliserMode       | int      | Lens stabilization mode. Value depends on implementation but it is recommended to keep default values: 0 - no stabilization, 1 - stabilization. |
| afRange              | int      | Autofocus range. Value depends on implementation.            |
| xFovDeg              | float    | Current horizontal Field of view, degree. Field of view calculated by lens controller according to initial params or by reading directly from lens hardware. |
| yFovDeg              | float    | Current vertical Field of view, degree. Field of view calculated by lens controller according to initial params or by reading directly from lens hardware. |
| logMode              | int      | Logging mode. Values: 0 - Disable, 1 - Only file, 2 - Only terminal, 3 - File and terminal. |
| temperature          | float    | Lens temperature, degree.                                    |
| isOpen               | bool     | Lens controller initialization status. Open status shows if the lens controller initialized or not but doesn't show if lens controller has communication with lens equipment. For example, if lens has serial port lens controller connects to serial port (opens serial port file in OS) but lens can be not active (no power). In this case open status just shows that lens controller has opened serial port. Values: FALSE - not open (not initialized), TRUE - open (initialized). |
| type                 | int      | Lens type. Value depends on implementation. Type allows to lens initialize necessary parameters for particular lens hardware. |
| custom1              | float    | Lens custom parameter. Value depends on particular lens controller. Custom parameters used when particular lens equipment has specific unusual parameter. |
| custom2              | float    | Lens custom parameter. Value depends on particular lens controller. Custom parameters used when particular lens equipment has specific unusual parameter. |
| custom3              | float    | Lens custom parameter. Value depends on particular lens controller. Custom parameters used when particular lens equipment has specific unusual parameter. |
| fovPoints            | FovPoint | List of points to calculate field of view. Lens controller should calculate FOV table according to given list of points using approximation (if provided by user). Each point includes (**FovPoint** class):<br />- **hwZoomPos** - hardware zoom position.<br />- **xFovDeg** - horizontal FOV, degree for hwZoomPos.<br />- **yFovDeg** - vertical FOV, degree for hwZoomPos. |

**Note:** *LensParams class fields listed in Table 4 **must** reflect params set/get by methods setParam(...) and getParam(...).*



## Serialize lens params

[LensParams](#lensparams-class-description) class provides method **encode(...)** to serialize lens params (fields of [LensParams](#lensparams-class-description) class, see Table 4). Serialization of lens params necessary in case when you need to send lens params via communication channels. Method doesn't encode **initString** string field and **fovPoints**. Method provides options to exclude particular parameters from serialization. To do this method inserts binary mask (7 bytes) where each bit represents particular parameter and **decode(...)** method recognizes it. Method declaration:

```cpp
bool encode(uint8_t* data, int bufferSize, int& size, LensParamsMask* mask = nullptr);
```

| Parameter  | Value                                                        |
| ---------- | ------------------------------------------------------------ |
| data       | Pointer to data buffer. Must not be nullptr.                 |
| bufferSize | Data buffer size. Buffer size must be >= 201 bytes (the size of data with all parameters) regardless of the mask. |
| size       | Size of encoded data. Set to 0 if the parameters are not encoded. |
| mask       | Parameters mask - pointer to **LensParamsMask** structure. **LensParamsMask** (declared in **Lens.h** file) determines flags for each field (parameter) declared in [LensParams](#lensparams-class-description) class. If the user wants to exclude any parameters from serialization, he can put a pointer to the mask. If the user wants to exclude a particular parameter from serialization, he should set the corresponding flag of the **LensParamsMask** structure to **false**. If **mask** is nullptr all parameters are encoded. |

**Returns:** TRUE if the parameters were encoded or FALSE if not (**data** is nullptr or **bufferSize** is less than 201). If the method returns FALSE **size** is set to 0 and nothing is written to the buffer.

The encoded data consists of: data type (1 byte, 0x02), version Major and Minor (2 bytes), mask (7 bytes) and values of the selected parameters. The n-th field of **LensParamsMask** uses the bit (0x80 >> (n % 8)) of the mask byte n / 8 (fields are numbered from 0 in the order of the structure declaration). Values follow the mask in the order of the **LensParamsMask** fields. Values of int and float fields use 4 bytes in the byte order of the host (see [Portability](#portability) section), values of bool fields use 1 byte (0 or 1). The size of encoded data is from 10 bytes (no parameters) to 201 bytes (all parameters).

**LensParamsMask** structure declaration:

```cpp
/**
 * @brief Lens params mask structure.
 *
 * Each flag enables serialization of the LensParams field with the same
 * name (see LensParams::encode(...)).
 */
typedef struct LensParamsMask
{
    bool zoomPos{true};  ///< Serialize zoomPos.
    bool zoomHwPos{true};  ///< Serialize zoomHwPos.
    bool focusPos{true};  ///< Serialize focusPos.
    bool focusHwPos{true};  ///< Serialize focusHwPos.
    bool irisPos{true};  ///< Serialize irisPos.
    bool irisHwPos{true};  ///< Serialize irisHwPos.
    bool focusMode{true};  ///< Serialize focusMode.
    bool filterMode{true};  ///< Serialize filterMode.
    bool afRoiX0{true};  ///< Serialize afRoiX0.
    bool afRoiY0{true};  ///< Serialize afRoiY0.
    bool afRoiX1{true};  ///< Serialize afRoiX1.
    bool afRoiY1{true};  ///< Serialize afRoiY1.
    bool zoomSpeed{true};  ///< Serialize zoomSpeed.
    bool zoomHwSpeed{true};  ///< Serialize zoomHwSpeed.
    bool zoomHwMaxSpeed{true};  ///< Serialize zoomHwMaxSpeed.
    bool focusSpeed{true};  ///< Serialize focusSpeed.
    bool focusHwSpeed{true};  ///< Serialize focusHwSpeed.
    bool focusHwMaxSpeed{true};  ///< Serialize focusHwMaxSpeed.
    bool irisSpeed{true};  ///< Serialize irisSpeed.
    bool irisHwSpeed{true};  ///< Serialize irisHwSpeed.
    bool irisHwMaxSpeed{true};  ///< Serialize irisHwMaxSpeed.
    bool zoomHwTeleLimit{true};  ///< Serialize zoomHwTeleLimit.
    bool zoomHwWideLimit{true};  ///< Serialize zoomHwWideLimit.
    bool focusHwFarLimit{true};  ///< Serialize focusHwFarLimit.
    bool focusHwNearLimit{true};  ///< Serialize focusHwNearLimit.
    bool irisHwOpenLimit{true};  ///< Serialize irisHwOpenLimit.
    bool irisHwCloseLimit{true};  ///< Serialize irisHwCloseLimit.
    bool focusFactor{true};  ///< Serialize focusFactor.
    bool isConnected{true};  ///< Serialize isConnected.
    bool afHwSpeed{true};  ///< Serialize afHwSpeed.
    bool focusFactorThreshold{true};  ///< Serialize focusFactorThreshold.
    bool refocusTimeoutSec{true};  ///< Serialize refocusTimeoutSec.
    bool afIsActive{true};  ///< Serialize afIsActive.
    bool irisMode{true};  ///< Serialize irisMode.
    bool autoAfRoiWidth{true};  ///< Serialize autoAfRoiWidth.
    bool autoAfRoiHeight{true};  ///< Serialize autoAfRoiHeight.
    bool autoAfRoiBorder{true};  ///< Serialize autoAfRoiBorder.
    bool afRoiMode{true};  ///< Serialize afRoiMode.
    bool extenderMode{true};  ///< Serialize extenderMode.
    bool stabiliserMode{true};  ///< Serialize stabiliserMode.
    bool afRange{true};  ///< Serialize afRange.
    bool xFovDeg{true};  ///< Serialize xFovDeg.
    bool yFovDeg{true};  ///< Serialize yFovDeg.
    bool logMode{true};  ///< Serialize logMode.
    bool temperature{true};  ///< Serialize temperature.
    bool isOpen{true};  ///< Serialize isOpen.
    bool type{true};  ///< Serialize type.
    bool custom1{true};  ///< Serialize custom1.
    bool custom2{true};  ///< Serialize custom2.
    bool custom3{true};  ///< Serialize custom3.
} LensParamsMask;
```

Example without parameters mask:

```cpp
// Encode data.
cr::lens::LensParams in;
in.logMode = 3;
uint8_t data[1024];
int size = 0;
if (!in.encode(data, 1024, size))
{
    std::cout << "Can't encode data" << std::endl;
    return false;
}
std::cout << "Encoded data size: " << size << " bytes" << std::endl;
```

Example with parameters mask:

```cpp
// Prepare params.
cr::lens::LensParams in;
in.logMode = 3;

// Prepare mask.
cr::lens::LensParamsMask mask;
mask.logMode = false; // Exclude logMode. Others by default.

// Encode.
uint8_t data[1024];
int size = 0;
if (!in.encode(data, 1024, size, &mask))
{
    std::cout << "Can't encode data" << std::endl;
    return false;
}
std::cout << "Encoded data size: " << size << " bytes" << std::endl;
```



## Deserialize lens params

**LensParams** class provides method **decode(...)** to deserialize lens params (fields of **LensParams** class, see Table 4). Deserialization of lens params necessary in case when you need to receive lens params via communication channels. Method automatically recognizes which parameters were serialized by **encode(...)** method. Method doesn't decode fields: **initString** and **fovPoints** (after decoding **initString** is empty and **fovPoints** is empty). Parameters which were not serialized are set to 0 (**false** for bool parameters). Method checks that the data has all values selected by the mask before it changes the object. Bytes after the last value are ignored. Values of the parameters are not checked against any range: lens controller must validate them before use. Method declaration:

```cpp
bool decode(uint8_t* data, int dataSize);
```

| Parameter | Value                   |
| --------- | ----------------------- |
| data      | Pointer to data buffer. Must not be nullptr. |
| dataSize  | Size of data, bytes.    |

**Returns:** TRUE if data decoded (deserialized) or FALSE if not: **data** is nullptr, **dataSize** is less than 10 (size of the header), data type or version (Major and Minor) is wrong, the mask has bits of unknown fields (the last 6 bits of the last mask byte must be 0) or the data doesn't have all values selected by the mask. If the method returns FALSE the object is not changed.

Example:

```cpp
// Encode data.
cr::lens::LensParams in;
uint8_t data[1024];
int size = 0;
if (!in.encode(data, 1024, size))
{
    std::cout << "Can't encode data" << std::endl;
    return false;
}
std::cout << "Encoded data size: " << size << " bytes" << std::endl;

// Decode data.
cr::lens::LensParams out;
if (!out.decode(data, size))
{
    std::cout << "Can't decode data" << std::endl;
    return false;
}
```



## Read params from JSON file and write to JSON file

**Lens** interface class library depends on **ConfigReader** library which provides method to read params from JSON file and to write params to JSON file. Example of writing and reading params to JSON file:

```cpp
// Prepare params.
cr::lens::LensParams in;
for (int i = 0; i < 5; ++i)
{
    cr::lens::FovPoint pt;
    pt.hwZoomPos = 100 * i;
    pt.xFovDeg = 10.0f + static_cast<float>(i);
    pt.yFovDeg = 5.0f + static_cast<float>(i);
    in.fovPoints.push_back(pt);
}

// Write params to file.
cr::utils::ConfigReader inConfig;
if (!inConfig.set(in, "lensParams") ||
    !inConfig.writeToFile("TestLensParams.json"))
{
    std::cout << "Can't write config file" << std::endl;
    return false;
}

// Read params from file.
cr::utils::ConfigReader outConfig;
cr::lens::LensParams out;
if (!outConfig.readFromFile("TestLensParams.json") ||
    !outConfig.get(out, "lensParams"))
{
    std::cout << "Can't read params from file" << std::endl;
    return false;
}
```

**TestLensParams.json** will look like:

```json
{
    "lensParams": {
        "initString": "/dev/ttyUSB0;9600;20",
        "focusMode": 0,
        "filterMode": 0,
        "afRoiX0": 0,
        "afRoiY0": 0,
        "afRoiX1": 0,
        "afRoiY1": 0,
        "zoomHwMaxSpeed": 50,
        "focusHwMaxSpeed": 50,
        "irisHwMaxSpeed": 50,
        "zoomHwTeleLimit": 65535,
        "zoomHwWideLimit": 0,
        "focusHwFarLimit": 65535,
        "focusHwNearLimit": 0,
        "irisHwOpenLimit": 65535,
        "irisHwCloseLimit": 0,
        "afHwSpeed": 50,
        "focusFactorThreshold": 0.0,
        "refocusTimeoutSec": 0,
        "irisMode": 0,
        "autoAfRoiWidth": 150,
        "autoAfRoiHeight": 150,
        "autoAfRoiBorder": 100,
        "afRoiMode": 0,
        "extenderMode": 0,
        "stabiliserMode": 0,
        "afRange": 0,
        "logMode": 0,
        "type": 0,
        "custom1": 0.0,
        "custom2": 0.0,
        "custom3": 0.0,
        "fovPoints": [
            {
                "hwZoomPos": 0,
                "xFovDeg": 10.0,
                "yFovDeg": 5.0
            },
            {
                "hwZoomPos": 100,
                "xFovDeg": 11.0,
                "yFovDeg": 6.0
            },
            {
                "hwZoomPos": 200,
                "xFovDeg": 12.0,
                "yFovDeg": 7.0
            },
            {
                "hwZoomPos": 300,
                "xFovDeg": 13.0,
                "yFovDeg": 8.0
            },
            {
                "hwZoomPos": 400,
                "xFovDeg": 14.0,
                "yFovDeg": 9.0
            }
        ]
    }
}
```



# Build and connect to your project



## Build stand alone

The repository doesn't contain sub-repositories. The libraries **Lens** depends on (**Frame** and **ConfigReader**) must be cloned into the same folder as **Lens**. The folder structure must be:

```bash
<work folder>
    Lens
    Frame
    ConfigReader
```

Typical commands to build the library, the test application and the example (use the URLs of the repositories provided to you):

```bash
git clone <Lens repository URL>
git clone <Frame repository URL>
git clone <ConfigReader repository URL>
cd Lens
mkdir build
cd build
cmake ..
make
```

On Windows use `cmake --build . --config Release` instead of `make`. If **Lens** is the top-level CMake project, the main CMakeLists.txt file adds the folders ../Frame and ../ConfigReader (in this order) before the library itself. If a folder doesn't exist CMake stops with an error.



## CMake options

**Table 5** - CMake options of the repository. In the options below ${PARENT} is the prefix defined by the top-level project (empty if **Lens** is the top-level project).

| Option                              | Default                                           | Description                                                  |
| ----------------------------------- | ------------------------------------------------- | ------------------------------------------------------------ |
| ${PARENT}_LENS                      | ON                                                | Build the library.                                           |
| ${PARENT}_LENS_TEST                 | ON (stand alone), OFF (included in other project) | Build the test application.                                  |
| ${PARENT}_LENS_EXAMPLE              | ON (stand alone), OFF (included in other project) | Build the example of custom lens controller.                 |
| ${PARENT}_SUBMODULE_CACHE_OVERWRITE | ON                                                | Forced rewriting of the options above in the CMake cache. The top-level project sets it to OFF when it includes **Lens** as a sub-repository. |
| CMAKE_BUILD_TYPE                    | Release (if not specified)                        | Build type.                                                  |



## Connect to your project

If you want to connect **Lens** library to your CMake project as source code you can make follow. For example, if your repository has structure:

```bash
CMakeLists.txt
src
    CMakeLists.txt
    yourLib.h
    yourLib.cpp
```

Create folder **3rdparty** in your repository and clone (or copy) the repositories of **Frame**, **ConfigReader** and **Lens** there. Structure of your repository with the libraries:

```bash
CMakeLists.txt
src
    CMakeLists.txt
    yourLib.h
    yourLib.cpp
3rdparty
    Frame
    ConfigReader
    Lens
```

When **Lens** is included in another project it doesn't add the libraries it depends on: your project must add **Frame** and **ConfigReader** by itself. If they are not added the compiler stops with the error `Frame.h: No such file or directory`. Create CMakeLists.txt file in **3rdparty** folder. CMakeLists.txt should contain:

```cmake
cmake_minimum_required(VERSION 3.13)

################################################################################
## 3RD-PARTY
## dependencies for the project
################################################################################
project(3rdparty LANGUAGES CXX)

################################################################################
## SETTINGS
## basic 3rd-party settings before use
################################################################################
# To inherit the top-level architecture when the project is used as a submodule.
SET(PARENT ${PARENT}_YOUR_PROJECT_3RDPARTY)
# Disable self-overwriting of parameters inside included subdirectories.
SET(${PARENT}_SUBMODULE_CACHE_OVERWRITE OFF CACHE BOOL "" FORCE)

################################################################################
## CONFIGURATION
## 3rd-party submodules configuration
################################################################################
SET(${PARENT}_SUBMODULE_FRAME                           ON  CACHE BOOL "" FORCE)
if (${PARENT}_SUBMODULE_FRAME)
    SET(${PARENT}_FRAME                                 ON  CACHE BOOL "" FORCE)
    SET(${PARENT}_FRAME_TEST                            OFF CACHE BOOL "" FORCE)
endif()

SET(${PARENT}_SUBMODULE_CONFIG_READER                   ON  CACHE BOOL "" FORCE)
if (${PARENT}_SUBMODULE_CONFIG_READER)
    SET(${PARENT}_CONFIG_READER                         ON  CACHE BOOL "" FORCE)
    SET(${PARENT}_CONFIG_READER_EXAMPLES                OFF CACHE BOOL "" FORCE)
    SET(${PARENT}_CONFIG_READER_TESTS                   OFF CACHE BOOL "" FORCE)
endif()

SET(${PARENT}_SUBMODULE_LENS                            ON  CACHE BOOL "" FORCE)
if (${PARENT}_SUBMODULE_LENS)
    SET(${PARENT}_LENS                                  ON  CACHE BOOL "" FORCE)
    SET(${PARENT}_LENS_TEST                             OFF CACHE BOOL "" FORCE)
    SET(${PARENT}_LENS_EXAMPLE                          OFF CACHE BOOL "" FORCE)
endif()

################################################################################
## INCLUDING SUBDIRECTORIES
## Adding subdirectories according to the 3rd-party configuration
################################################################################
if (${PARENT}_SUBMODULE_FRAME)
    add_subdirectory(Frame)
endif()

if (${PARENT}_SUBMODULE_CONFIG_READER)
    add_subdirectory(ConfigReader)
endif()

if (${PARENT}_SUBMODULE_LENS)
    add_subdirectory(Lens)
endif()
```

File **3rdparty/CMakeLists.txt** adds folders **Frame**, **ConfigReader** and **Lens** (in this order: **Lens** is added after the libraries it depends on) to your project and excludes test applications and examples from compiling (by default test application and example excluded from compiling if **Lens** included as sub-repository). Structure of your repository will be:

```bash
CMakeLists.txt
src
    CMakeLists.txt
    yourLib.h
    yourLib.cpp
3rdparty
    CMakeLists.txt
    Frame
    ConfigReader
    Lens
```

Next you need include folder 3rdparty in main **CMakeLists.txt** file of your repository. Add string at the end of your main **CMakeLists.txt**:

```cmake
add_subdirectory(3rdparty)
```

Next you have to include Lens library in your **src/CMakeLists.txt** file:

```cmake
target_link_libraries(${PROJECT_NAME} Lens)
```

Done!



# Portability

Encoded data (**LensParams::encode(...)**, **Lens::encodeCommand(...)** and **Lens::encodeSetParamCommand(...)** methods) stores int and float values as raw 4 bytes in the byte order of the host. The library checks at compile time that int and float have 4 bytes. Data must be exchanged between hosts with the same byte order. The library was built and tested only on little-endian CPUs (x86-64 and AArch64).

The library doesn't use SIMD instructions or CPU specific code. The library, the example and the test application were built without warnings and the test application was run with MSVC 2022 (x86-64), GCC 12 and 13 (x86-64), Clang 15 and 18 (x86-64) and with GCC 13, Clang 15 and Clang 18 cross compilers for AArch64 (the test application was run under emulation).



# How to make custom implementation

The **Lens** class provides only an interface, data structures, and methods for encoding and decoding commands and params. To create your own implementation of the lens controller, you must include the Lens repository in your project (see [Build and connect to your project](#build-and-connect-to-your-project) section). The catalogue **example** (see [Library files](#library-files) section) includes an example of the design of the custom lens controller. You must implement all the methods of the Lens interface class. The methods **setParam(...)**, **getParam(...)**, **getParams(...)**, **executeCommand(...)** and **decodeAndExecuteCommand(...)** must be thread-safe: the example protects the parameters with a mutex. The example checks the values which come from outside (see the description of **initLens(...)** and **setParam(...)** methods in **CustomLens.h**) and doesn't control any lens hardware. Custom lens class declaration:

```cpp
class CustomLens : public Lens
{
public:

    /// Class constructor.
    CustomLens();

    /// Class destructor.
    ~CustomLens() override;

    /// Copy constructor is deleted: the class owns a mutex.
    CustomLens(const CustomLens&) = delete;

    /// Move constructor is deleted: the class owns a mutex.
    CustomLens(CustomLens&&) = delete;

    /// Copy assignment operator is deleted: the class owns a mutex.
    CustomLens& operator= (const CustomLens&) = delete;

    /// Move assignment operator is deleted: the class owns a mutex.
    CustomLens& operator= (CustomLens&&) = delete;

    /// Get lens class version.
    static std::string getVersion();

    /// Open lens controller.
    bool openLens(std::string initString) override;

    /// Init lens controller by structure.
    bool initLens(LensParams& params) override;

    /// Close connection.
    void closeLens() override;

    /// Get lens open status.
    bool isLensOpen() override;

    /// Get lens connection status.
    bool isLensConnected() override;

    /// Set the lens controller param.
    bool setParam(LensParam id, float value) override;

    /// Get the lens controller param.
    float getParam(LensParam id) override;

    /// Get the lens controller params.
    void getParams(LensParams& params) override;

    /// Execute command.
    bool executeCommand(LensCommand id, float arg = 0) override;

    /// Add video frame for auto focus purposes.
    void addVideoFrame(cr::video::Frame& frame) override;

    /// Decode and execute command.
    bool decodeAndExecuteCommand(uint8_t* data, int size) override;

private:

    /// Lens parameters structure (Default params). Protected by
    /// m_paramsMutex.
    LensParams m_params;
    /// Mutex to protect m_params.
    std::mutex m_paramsMutex;
};
```