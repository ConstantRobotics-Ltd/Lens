#include "Lens.h"
#include "LensVersion.h"



/// Copy operator.
cr::lens::FovPoint &cr::lens::FovPoint::operator= (const cr::lens::FovPoint &src)
{
    // Check yourself.
    if (this == &src)
        return *this;

    // Copy params.
    hwZoomPos = src.hwZoomPos;
    xFovDeg = src.xFovDeg;
    yFovDeg = src.yFovDeg;

    return *this;
}



/// Copy operator.
cr::lens::LensParams &cr::lens::LensParams::operator= (const cr::lens::LensParams &src)
{
    // Check yourself.
    if (this == &src)
        return *this;

    // Copy params.
    initString = src.initString;
    zoomPos = src.zoomPos;
    zoomHwPos = src.zoomHwPos;
    focusPos = src.focusPos;
    focusHwPos = src.focusHwPos;
    irisPos = src.irisPos;
    irisHwPos = src.irisHwPos;
    focusMode = src.focusMode;
    filterMode = src.filterMode;
    afRoiX0 = src.afRoiX0;
    afRoiY0 = src.afRoiY0;
    afRoiX1 = src.afRoiX1;
    afRoiY1 = src.afRoiY1;
    zoomSpeed = src.zoomSpeed;
    zoomHwSpeed = src.zoomHwSpeed;
    zoomHwMaxSpeed = src.zoomHwMaxSpeed;
    focusSpeed = src.focusSpeed;
    focusHwSpeed = src.focusHwSpeed;
    focusHwMaxSpeed = src.focusHwMaxSpeed;
    irisSpeed = src.irisSpeed;
    irisHwSpeed = src.irisHwSpeed;
    irisHwMaxSpeed = src.irisHwMaxSpeed;
    zoomHwTeleLimit = src.zoomHwTeleLimit;
    zoomHwWideLimit = src.zoomHwWideLimit;
    focusHwFarLimit = src.focusHwFarLimit;
    focusHwNearLimit = src.focusHwNearLimit;
    irisHwOpenLimit = src.irisHwOpenLimit;
    irisHwCloseLimit = src.irisHwCloseLimit;
    focusFactor = src.focusFactor;
    isConnected = src.isConnected;
    afHwSpeed = src.afHwSpeed;
    focusFactorThreshold = src.focusFactorThreshold;
    refocusTimeoutSec = src.refocusTimeoutSec;
    afIsActive = src.afIsActive;
    irisMode = src.irisMode;
    autoAfRoiWidth = src.autoAfRoiWidth;
    autoAfRoiHeight = src.autoAfRoiHeight;
    autoAfRoiBorder = src.autoAfRoiBorder;
    afRoiMode = src.afRoiMode;
    extenderMode = src.extenderMode;
    stabiliserMode = src.stabiliserMode;
    afRange = src.afRange;
    xFovDeg = src.xFovDeg;
    yFovDeg = src.yFovDeg;
    logMode = src.logMode;
    temperature = src.temperature;
    isOpen = src.isOpen;
    type = src.type;
    custom1 = src.custom1;
    custom2 = src.custom2;
    custom3 = src.custom3;
    fovPoints = src.fovPoints;

    return *this;
}



/// Encode params.
void cr::lens::LensParams::encode(uint8_t* data, int& size)
{
    // Encode version.
    int pos = 0;
    data[pos] = 0x02; pos += 1;
    data[pos] = LENS_MAJOR_VERSION; pos += 1;
    data[pos] = LENS_MINOR_VERSION; pos += 1;

    // Encode data.
    memcpy(&data[pos], &zoomPos, 4); pos += 4;
    memcpy(&data[pos], &zoomHwPos, 4); pos += 4;
    memcpy(&data[pos], &focusPos, 4); pos += 4;
    memcpy(&data[pos], &focusHwPos, 4); pos += 4;
    memcpy(&data[pos], &irisPos, 4); pos += 4;
    memcpy(&data[pos], &irisHwPos, 4); pos += 4;
    memcpy(&data[pos], &focusMode, 4); pos += 4;
    memcpy(&data[pos], &filterMode, 4); pos += 4;
    memcpy(&data[pos], &afRoiX0, 4); pos += 4;
    memcpy(&data[pos], &afRoiY0, 4); pos += 4;
    memcpy(&data[pos], &afRoiX1, 4); pos += 4;
    memcpy(&data[pos], &afRoiY1, 4); pos += 4;
    memcpy(&data[pos], &zoomSpeed, 4); pos += 4;
    memcpy(&data[pos], &zoomHwSpeed, 4); pos += 4;
    memcpy(&data[pos], &zoomHwMaxSpeed, 4); pos += 4;
    memcpy(&data[pos], &focusSpeed, 4); pos += 4;
    memcpy(&data[pos], &focusHwSpeed, 4); pos += 4;
    memcpy(&data[pos], &focusHwMaxSpeed, 4); pos += 4;
    memcpy(&data[pos], &irisSpeed, 4); pos += 4;
    memcpy(&data[pos], &irisHwSpeed, 4); pos += 4;
    memcpy(&data[pos], &irisHwMaxSpeed, 4); pos += 4;
    memcpy(&data[pos], &zoomHwTeleLimit, 4); pos += 4;
    memcpy(&data[pos], &zoomHwWideLimit, 4); pos += 4;
    memcpy(&data[pos], &focusHwFarLimit, 4); pos += 4;
    memcpy(&data[pos], &focusHwNearLimit, 4); pos += 4;
    memcpy(&data[pos], &irisHwOpenLimit, 4); pos += 4;
    memcpy(&data[pos], &irisHwCloseLimit, 4); pos += 4;
    memcpy(&data[pos], &focusFactor, 4); pos += 4;
    data[pos] = isConnected == true ? 0x01 : 0x00; pos += 1;
    memcpy(&data[pos], &afHwSpeed, 4); pos += 4;
    memcpy(&data[pos], &focusFactorThreshold, 4); pos += 4;
    memcpy(&data[pos], &refocusTimeoutSec, 4); pos += 4;
    data[pos] = afIsActive == true ? 0x01 : 0x00; pos += 1;
    memcpy(&data[pos], &irisMode, 4); pos += 4;
    memcpy(&data[pos], &autoAfRoiWidth, 4); pos += 4;
    memcpy(&data[pos], &autoAfRoiHeight, 4); pos += 4;
    memcpy(&data[pos], &autoAfRoiBorder, 4); pos += 4;
    memcpy(&data[pos], &afRoiMode, 4); pos += 4;
    memcpy(&data[pos], &extenderMode, 4); pos += 4;
    memcpy(&data[pos], &stabiliserMode, 4); pos += 4;
    memcpy(&data[pos], &afRange, 4); pos += 4;
    memcpy(&data[pos], &xFovDeg, 4); pos += 4;
    memcpy(&data[pos], &yFovDeg, 4); pos += 4;
    memcpy(&data[pos], &logMode, 4); pos += 4;
    memcpy(&data[pos], &temperature, 4); pos += 4;
    data[pos] = isOpen == true ? 0x01 : 0x00; pos += 1;
    memcpy(&data[pos], &type, 4); pos += 4;
    memcpy(&data[pos], &custom1, 4); pos += 4;
    memcpy(&data[pos], &custom2, 4); pos += 4;
    memcpy(&data[pos], &custom3, 4); pos += 4;

    size = pos;
}



/// Decode params.
bool cr::lens::LensParams::decode(uint8_t* data)
{
    // Check header.
    if (data[0] != 0x02)
        return false;

    // Check version.
    if (data[1] != LENS_MAJOR_VERSION || data[2] != LENS_MINOR_VERSION)
        return false;

    // Decode data.
    int pos = 3;
    memcpy(&zoomPos, &data[pos], 4); pos += 4;
    memcpy(&zoomHwPos, &data[pos], 4); pos += 4;
    memcpy(&focusPos, &data[pos], 4); pos += 4;
    memcpy(&focusHwPos, &data[pos], 4); pos += 4;
    memcpy(&irisPos, &data[pos], 4); pos += 4;
    memcpy(&irisHwPos, &data[pos], 4); pos += 4;
    memcpy(&focusMode, &data[pos], 4); pos += 4;
    memcpy(&filterMode, &data[pos], 4); pos += 4;
    memcpy(&afRoiX0, &data[pos], 4); pos += 4;
    memcpy(&afRoiY0, &data[pos], 4); pos += 4;
    memcpy(&afRoiX1, &data[pos], 4); pos += 4;
    memcpy(&afRoiY1, &data[pos], 4); pos += 4;
    memcpy(&zoomSpeed, &data[pos], 4); pos += 4;
    memcpy(&zoomHwSpeed, &data[pos], 4); pos += 4;
    memcpy(&zoomHwMaxSpeed, &data[pos], 4); pos += 4;
    memcpy(&focusSpeed, &data[pos], 4); pos += 4;
    memcpy(&focusHwSpeed, &data[pos], 4); pos += 4;
    memcpy(&focusHwMaxSpeed, &data[pos], 4); pos += 4;
    memcpy(&irisSpeed, &data[pos], 4); pos += 4;
    memcpy(&irisHwSpeed, &data[pos], 4); pos += 4;
    memcpy(&irisHwMaxSpeed, &data[pos], 4); pos += 4;
    memcpy(&zoomHwTeleLimit, &data[pos], 4); pos += 4;
    memcpy(&zoomHwWideLimit, &data[pos], 4); pos += 4;
    memcpy(&focusHwFarLimit, &data[pos], 4); pos += 4;
    memcpy(&focusHwNearLimit, &data[pos], 4); pos += 4;
    memcpy(&irisHwOpenLimit, &data[pos], 4); pos += 4;
    memcpy(&irisHwCloseLimit, &data[pos], 4); pos += 4;
    memcpy(&focusFactor, &data[pos], 4); pos += 4;
    isConnected = data[pos] == 0x00 ? false : true; pos += 1;
    memcpy(&afHwSpeed, &data[pos], 4); pos += 4;
    memcpy(&focusFactorThreshold, &data[pos], 4); pos += 4;
    memcpy(&refocusTimeoutSec, &data[pos], 4); pos += 4;
    afIsActive = data[pos] == 0x00 ? false : true; pos += 1;
    memcpy(&irisMode, &data[pos], 4); pos += 4;
    memcpy(&autoAfRoiWidth, &data[pos], 4); pos += 4;
    memcpy(&autoAfRoiHeight, &data[pos], 4); pos += 4;
    memcpy(&autoAfRoiBorder, &data[pos], 4); pos += 4;
    memcpy(&afRoiMode, &data[pos], 4); pos += 4;
    memcpy(&extenderMode, &data[pos], 4); pos += 4;
    memcpy(&stabiliserMode, &data[pos], 4); pos += 4;
    memcpy(&afRange, &data[pos], 4); pos += 4;
    memcpy(&xFovDeg, &data[pos], 4); pos += 4;
    memcpy(&yFovDeg, &data[pos], 4); pos += 4;
    memcpy(&logMode, &data[pos], 4); pos += 4;
    memcpy(&temperature, &data[pos], 4); pos += 4;
    isOpen = data[pos] == 0x00 ? false : true; pos += 1;
    memcpy(&type, &data[pos], 4); pos += 4;
    memcpy(&custom1, &data[pos], 4); pos += 4;
    memcpy(&custom2, &data[pos], 4); pos += 4;
    memcpy(&custom3, &data[pos], 4); pos += 4;

    initString = "";
    fovPoints.clear();

    return true;
}



/// Get controller version.
std::string cr::lens::Lens::getVersion()
{
    return LENS_VERSION;
}



/// Encode set param command.
void cr::lens::Lens::encodeSetParamCommand(uint8_t* data,
                                           int& size,
                                           cr::lens::LensParam id,
                                           float value)
{
    // Fill header.
    data[0] = 0x01;
    data[1] = LENS_MAJOR_VERSION;
    data[2] = LENS_MINOR_VERSION;

    // Fill data.
    int paramId = (int)id;
    memcpy(&data[3], &paramId, 4);
    memcpy(&data[7], &value, 4);
    size = 11;
}



/// Encode command.
void cr::lens::Lens::encodeCommand(uint8_t* data,
                                   int& size,
                                   cr::lens::LensCommand id,
                                   float arg)
{
    // Fill header.
    data[0] = 0x00;
    data[1] = LENS_MAJOR_VERSION;
    data[2] = LENS_MINOR_VERSION;

    // Fill data.
    int commandId = (int)id;
    memcpy(&data[3], &commandId, 4);
    memcpy(&data[7], &arg, 4);
    size = 11;
}



/// Decode command.
int cr::lens::Lens::decodeCommand(uint8_t* data,
                                  int size,
                                  cr::lens::LensParam& paramId,
                                  cr::lens::LensCommand& commandId,
                                  float& value)
{
    // Check size.
    if (size != 11)
        return -1;

    // Check version.
    if (data[1] != LENS_MAJOR_VERSION || data[2] != LENS_MINOR_VERSION)
        return -1;

    // Extract data.
    int id = 0;
    memcpy(&id, &data[3], 4);
    memcpy(&value, &data[7], 4);

    // Check command type.
    if (data[0] == 0x00)
    {
        commandId = (LensCommand)id;
        return 0;
    }
    else if (data[0] == 0x01)
    {
        paramId = (LensParam)id;
        return 1;
    }

    return -1;
}



