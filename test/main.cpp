#include <iostream>
#include "Lens.h"



/// Link namesapces.
using namespace cr::lens;
using namespace std;



/// Copy test.
bool copyTest();

/// Encode/decode params test.
bool encodeDecodeParamsTest();

/// Encode/decode test with params mask.
bool encodeDecodeParamsWithMaskTest();


/// Encode/decode commands test.
bool encodeDecodeCommandsTest();

/// JSON read/write test.
bool jsonReadWriteTest();



/// Entry point.
int main(void)
{
    cout << "#####################################" << endl;
    cout << "#                                   #" << endl;
    cout << "# Lens data structures test         #" << endl;
    cout << "#                                   #" << endl;
    cout << "#####################################" << endl;
    cout << endl;

    cout << "Copy test:" << endl;
    if (copyTest())
        cout << "OK" << endl;
    else
        cout << "ERROR" << endl;
    cout << endl;

    cout << "Encode/Decode params test:" << endl;
    if (encodeDecodeParamsTest())
        cout << "OK" << endl;
    else
        cout << "ERROR" << endl;
    cout << endl;

    cout << "Encode/Decode test with params mask:" << endl;
    if (encodeDecodeParamsWithMaskTest())
        cout << "OK" << endl;
    else
        cout << "ERROR" << endl;
    cout << endl;

    cout << "Encode/Decode commands test:" << endl;
    if (encodeDecodeCommandsTest())
        cout << "OK" << endl;
    else
        cout << "ERROR" << endl;
    cout << endl;

    cout << "JSON read/write test:" << endl;
    if (jsonReadWriteTest())
        cout << "OK" << endl;
    else
        cout << "ERROR" << endl;
    cout << endl;

    return 1;
}



// Copy test.
bool copyTest()
{
    // Prepare random params.
    LensParams in;
    in.initString = "dfhglsjirhuhjfb";
    in.zoomPos = rand() % 255;
    in.zoomHwPos = rand() % 255;
    in.focusPos = rand() % 255;
    in.focusHwPos = rand() % 255;
    in.irisPos = rand() % 255;
    in.irisHwPos = rand() % 255;
    in.focusMode = rand() % 255;
    in.filterMode = rand() % 255;
    in.afRoiX0 = rand() % 255;
    in.afRoiY0 = rand() % 255;
    in.afRoiX1 = rand() % 255;
    in.afRoiY1 = rand() % 255;
    in.zoomSpeed = rand() % 255;
    in.zoomHwSpeed = rand() % 255;
    in.zoomHwMaxSpeed = rand() % 255;
    in.focusSpeed = rand() % 255;
    in.focusHwSpeed = rand() % 255;
    in.focusHwMaxSpeed = rand() % 255;
    in.irisSpeed = rand() % 255;
    in.irisHwSpeed = rand() % 255;
    in.irisHwMaxSpeed = rand() % 255;
    in.zoomHwTeleLimit = rand() % 255;
    in.zoomHwWideLimit = rand() % 255;
    in.focusHwFarLimit = rand() % 255;
    in.focusHwNearLimit = rand() % 255;
    in.irisHwOpenLimit = rand() % 255;
    in.irisHwCloseLimit = rand() % 255;
    in.focusFactor = rand() % 255;
    in.isConnected = true;
    in.afHwSpeed = rand() % 255;
    in.focusFactorThreshold = rand() % 255;
    in.refocusTimeoutSec = rand() % 255;
    in.afIsActive = true;
    in.irisMode = rand() % 255;
    in.autoAfRoiWidth = rand() % 255;
    in.autoAfRoiHeight = rand() % 255;
    in.autoAfRoiBorder = rand() % 255;
    in.afRoiMode = rand() % 255;
    in.extenderMode = rand() % 255;
    in.stabiliserMode = rand() % 255;
    in.afRange = rand() % 255;
    in.xFovDeg = rand() % 255;
    in.yFovDeg = rand() % 255;
    in.logMode = rand() % 255;
    in.temperature = rand() % 255;
    in.isOpen = true;
    in.type = rand() % 255;
    in.custom1 = rand() % 255;
    in.custom2 = rand() % 255;
    in.custom3 = rand() % 255;
    for (int i = 0; i < 5; ++i)
    {
        FovPoint pt;
        pt.hwZoomPos = rand() % 255;
        pt.xFovDeg = rand() % 255;
        pt.yFovDeg = rand() % 255;
        in.fovPoints.push_back(pt);
    }

    // Copy params.
    LensParams out = in;

    // Compare params.
    bool result = true;
    if (in.initString != out.initString)
    {
        cout << "in.initString" << endl;
        result = false;
    }
    if (in.zoomPos != out.zoomPos)
    {
        cout << "in.zoomPos" << endl;
        result = false;
    }
    if (in.zoomHwPos != out.zoomHwPos)
    {
        cout << "in.zoomHwPos" << endl;
        result = false;
    }
    if (in.focusPos != out.focusPos)
    {
        cout << "in.focusPos" << endl;
        result = false;
    }
    if (in.focusHwPos != out.focusHwPos)
    {
        cout << "in.focusHwPos" << endl;
        result = false;
    }
    if (in.irisPos != out.irisPos)
    {
        cout << "in.irisPos" << endl;
        result = false;
    }
    if (in.irisHwPos != out.irisHwPos)
    {
        cout << "in.irisHwPos" << endl;
        result = false;
    }
    if (in.focusMode != out.focusMode)
    {
        cout << "in.focusMode" << endl;
        result = false;
    }
    if (in.filterMode != out.filterMode)
    {
        cout << "in.filterMode" << endl;
        result = false;
    }
    if (in.afRoiX0 != out.afRoiX0)
    {
        cout << "in.afRoiX0" << endl;
        result = false;
    }
    if (in.afRoiY0 != out.afRoiY0)
    {
        cout << "in.afRoiY0" << endl;
        result = false;
    }
    if (in.afRoiX1 != out.afRoiX1)
    {
        cout << "in.afRoiX1" << endl;
        result = false;
    }
    if (in.afRoiY1 != out.afRoiY1)
    {
        cout << "in.afRoiY1" << endl;
        result = false;
    }
    if (in.zoomSpeed != out.zoomSpeed)
    {
        cout << "in.zoomSpeed" << endl;
        result = false;
    }
    if (in.zoomHwSpeed != out.zoomHwSpeed)
    {
        cout << "in.zoomHwSpeed" << endl;
        result = false;
    }
    if (in.zoomHwMaxSpeed != out.zoomHwMaxSpeed)
    {
        cout << "in.zoomHwMaxSpeed" << endl;
        result = false;
    }
    if (in.focusSpeed != out.focusSpeed)
    {
        cout << "in.focusSpeed" << endl;
        result = false;
    }
    if (in.focusHwSpeed != out.focusHwSpeed)
    {
        cout << "in.focusHwSpeed" << endl;
        result = false;
    }
    if (in.focusHwMaxSpeed != out.focusHwMaxSpeed)
    {
        cout << "in.focusHwMaxSpeed" << endl;
        result = false;
    }
    if (in.irisSpeed != out.irisSpeed)
    {
        cout << "in.irisSpeed" << endl;
        result = false;
    }
    if (in.irisHwSpeed != out.irisHwSpeed)
    {
        cout << "in.irisHwSpeed" << endl;
        result = false;
    }
    if (in.irisHwMaxSpeed != out.irisHwMaxSpeed)
    {
        cout << "in.irisHwMaxSpeed" << endl;
        result = false;
    }
    if (in.zoomHwTeleLimit != out.zoomHwTeleLimit)
    {
        cout << "in.zoomHwTeleLimit" << endl;
        result = false;
    }
    if (in.zoomHwWideLimit != out.zoomHwWideLimit)
    {
        cout << "in.zoomHwWideLimit" << endl;
        result = false;
    }
    if (in.focusHwFarLimit != out.focusHwFarLimit)
    {
        cout << "in.focusHwFarLimit" << endl;
        result = false;
    }
    if (in.focusHwNearLimit != out.focusHwNearLimit)
    {
        cout << "in.focusHwNearLimit" << endl;
        result = false;
    }
    if (in.irisHwOpenLimit != out.irisHwOpenLimit)
    {
        cout << "in.irisHwOpenLimit" << endl;
        result = false;
    }
    if (in.irisHwCloseLimit != out.irisHwCloseLimit)
    {
        cout << "in.irisHwCloseLimit" << endl;
        result = false;
    }
    if (in.focusFactor != out.focusFactor)
    {
        cout << "in.focusFactor" << endl;
        result = false;
    }
    if (in.isConnected != out.isConnected)
    {
        cout << "in.isConnected" << endl;
        result = false;
    }
    if (in.afHwSpeed != out.afHwSpeed)
    {
        cout << "in.afHwSpeed" << endl;
        result = false;
    }
    if (in.focusFactorThreshold != out.focusFactorThreshold)
    {
        cout << "in.focusFactorThreshold" << endl;
        result = false;
    }
    if (in.refocusTimeoutSec != out.refocusTimeoutSec)
    {
        cout << "in.refocusTimeoutSec" << endl;
        result = false;
    }
    if (in.afIsActive != out.afIsActive)
    {
        cout << "in.afIsActive" << endl;
        result = false;
    }
    if (in.irisMode != out.irisMode)
    {
        cout << "in.irisMode" << endl;
        result = false;
    }
    if (in.autoAfRoiWidth != out.autoAfRoiWidth)
    {
        cout << "in.autoAfRoiWidth" << endl;
        result = false;
    }
    if (in.autoAfRoiHeight != out.autoAfRoiHeight)
    {
        cout << "in.autoAfRoiHeight" << endl;
        result = false;
    }
    if (in.autoAfRoiBorder != out.autoAfRoiBorder)
    {
        cout << "in.autoAfRoiBorder" << endl;
        result = false;
    }
    if (in.afRoiMode != out.afRoiMode)
    {
        cout << "in.afRoiMode" << endl;
        result = false;
    }
    if (in.extenderMode != out.extenderMode)
    {
        cout << "in.extenderMode" << endl;
        result = false;
    }
    if (in.stabiliserMode != out.stabiliserMode)
    {
        cout << "in.stabiliserMode" << endl;
        result = false;
    }
    if (in.afRange != out.afRange)
    {
        cout << "in.afRange" << endl;
        result = false;
    }
    if (in.xFovDeg != out.xFovDeg)
    {
        cout << "in.xFovDeg" << endl;
        result = false;
    }
    if (in.yFovDeg != out.yFovDeg)
    {
        cout << "in.yFovDeg" << endl;
        result = false;
    }
    if (in.logMode != out.logMode)
    {
        cout << "in.logMode" << endl;
        result = false;
    }
    if (in.temperature != out.temperature)
    {
        cout << "in.temperature" << endl;
        result = false;
    }
    if (in.isOpen != out.isOpen)
    {
        cout << "in.isOpen" << endl;
        result = false;
    }
    if (in.type != out.type)
    {
        cout << "in.type" << endl;
        result = false;
    }
    if (in.custom1 != out.custom1)
    {
        cout << "in.custom1" << endl;
        result = false;
    }
    if (in.custom2 != out.custom2)
    {
        cout << "in.custom2" << endl;
        result = false;
    }
    if (in.custom3 != out.custom3)
    {
        cout << "in.custom3" << endl;
        result = false;
    }
    if (in.fovPoints.size() != out.fovPoints.size())
    {
        cout << "in.fovPoints.size()" << endl;
        result = false;
    }
    for (int i = 0; i < in.fovPoints.size(); ++i)
    {
        if (in.fovPoints[i].hwZoomPos != out.fovPoints[i].hwZoomPos)
        {
            cout << "in.fovPoints[" << i << "].hwZoomPos" << endl;
            result = false;
        }
        if (in.fovPoints[i].xFovDeg != out.fovPoints[i].xFovDeg)
        {
            cout << "in.fovPoints[" << i << "].xFovDeg" << endl;
            result = false;
        }
        if (in.fovPoints[i].yFovDeg != out.fovPoints[i].yFovDeg)
        {
            cout << "in.fovPoints[" << i << "].yFovDeg" << endl;
            result = false;
        }
    }

    return result;
}



// Encode/decode test.
bool encodeDecodeParamsTest()
{
    // Prepare random params.
    LensParams in;
    in.initString = "dfhglsjirhuhjfb";
    in.zoomPos = rand() % 255;
    in.zoomHwPos = rand() % 255;
    in.focusPos = rand() % 255;
    in.focusHwPos = rand() % 255;
    in.irisPos = rand() % 255;
    in.irisHwPos = rand() % 255;
    in.focusMode = rand() % 255;
    in.filterMode = rand() % 255;
    in.afRoiX0 = rand() % 255;
    in.afRoiY0 = rand() % 255;
    in.afRoiX1 = rand() % 255;
    in.afRoiY1 = rand() % 255;
    in.zoomSpeed = rand() % 255;
    in.zoomHwSpeed = rand() % 255;
    in.zoomHwMaxSpeed = rand() % 255;
    in.focusSpeed = rand() % 255;
    in.focusHwSpeed = rand() % 255;
    in.focusHwMaxSpeed = rand() % 255;
    in.irisSpeed = rand() % 255;
    in.irisHwSpeed = rand() % 255;
    in.irisHwMaxSpeed = rand() % 255;
    in.zoomHwTeleLimit = rand() % 255;
    in.zoomHwWideLimit = rand() % 255;
    in.focusHwFarLimit = rand() % 255;
    in.focusHwNearLimit = rand() % 255;
    in.irisHwOpenLimit = rand() % 255;
    in.irisHwCloseLimit = rand() % 255;
    in.focusFactor = rand() % 255;
    in.isConnected = true;
    in.afHwSpeed = rand() % 255;
    in.focusFactorThreshold = rand() % 255;
    in.refocusTimeoutSec = rand() % 255;
    in.afIsActive = true;
    in.irisMode = rand() % 255;
    in.autoAfRoiWidth = rand() % 255;
    in.autoAfRoiHeight = rand() % 255;
    in.autoAfRoiBorder = rand() % 255;
    in.afRoiMode = rand() % 255;
    in.extenderMode = rand() % 255;
    in.stabiliserMode = rand() % 255;
    in.afRange = rand() % 255;
    in.xFovDeg = rand() % 255;
    in.yFovDeg = rand() % 255;
    in.logMode = rand() % 255;
    in.temperature = rand() % 255;
    in.isOpen = true;
    in.type = rand() % 255;
    in.custom1 = rand() % 255;
    in.custom2 = rand() % 255;
    in.custom3 = rand() % 255;
    for (int i = 0; i < 5; ++i)
    {
        FovPoint pt;
        pt.hwZoomPos = rand() % 255;
        pt.xFovDeg = rand() % 255;
        pt.yFovDeg = rand() % 255;
        in.fovPoints.push_back(pt);
    }

    // Encode data.
    uint8_t data[1024];
    int size = 0;
    in.encode(data, 1024, size);

    cout << "Encoded data size: " << size << " bytes" << endl;

    // Decode data.
    LensParams out;
    if (!out.decode(data, size))
    {
        cout << "Can't decode data" << endl;
        return false;
    }

    // Compare params.
    bool result = true;
    if (out.initString != "")
    {
        cout << "in.initString" << endl;
        result = false;
    }
    if (in.zoomPos != out.zoomPos)
    {
        cout << "in.zoomPos" << endl;
        result = false;
    }
    if (in.zoomHwPos != out.zoomHwPos)
    {
        cout << "in.zoomHwPos" << endl;
        result = false;
    }
    if (in.focusPos != out.focusPos)
    {
        cout << "in.focusPos" << endl;
        result = false;
    }
    if (in.focusHwPos != out.focusHwPos)
    {
        cout << "in.focusHwPos" << endl;
        result = false;
    }
    if (in.irisPos != out.irisPos)
    {
        cout << "in.irisPos" << endl;
        result = false;
    }
    if (in.irisHwPos != out.irisHwPos)
    {
        cout << "in.irisHwPos" << endl;
        result = false;
    }
    if (in.focusMode != out.focusMode)
    {
        cout << "in.focusMode" << endl;
        result = false;
    }
    if (in.filterMode != out.filterMode)
    {
        cout << "in.filterMode" << endl;
        result = false;
    }
    if (in.afRoiX0 != out.afRoiX0)
    {
        cout << "in.afRoiX0" << endl;
        result = false;
    }
    if (in.afRoiY0 != out.afRoiY0)
    {
        cout << "in.afRoiY0" << endl;
        result = false;
    }
    if (in.afRoiX1 != out.afRoiX1)
    {
        cout << "in.afRoiX1" << endl;
        result = false;
    }
    if (in.afRoiY1 != out.afRoiY1)
    {
        cout << "in.afRoiY1" << endl;
        result = false;
    }
    if (in.zoomSpeed != out.zoomSpeed)
    {
        cout << "in.zoomSpeed" << endl;
        result = false;
    }
    if (in.zoomHwSpeed != out.zoomHwSpeed)
    {
        cout << "in.zoomHwSpeed" << endl;
        result = false;
    }
    if (in.zoomHwMaxSpeed != out.zoomHwMaxSpeed)
    {
        cout << "in.zoomHwMaxSpeed" << endl;
        result = false;
    }
    if (in.focusSpeed != out.focusSpeed)
    {
        cout << "in.focusSpeed" << endl;
        result = false;
    }
    if (in.focusHwSpeed != out.focusHwSpeed)
    {
        cout << "in.focusHwSpeed" << endl;
        result = false;
    }
    if (in.focusHwMaxSpeed != out.focusHwMaxSpeed)
    {
        cout << "in.focusHwMaxSpeed" << endl;
        result = false;
    }
    if (in.irisSpeed != out.irisSpeed)
    {
        cout << "in.irisSpeed" << endl;
        result = false;
    }
    if (in.irisHwSpeed != out.irisHwSpeed)
    {
        cout << "in.irisHwSpeed" << endl;
        result = false;
    }
    if (in.irisHwMaxSpeed != out.irisHwMaxSpeed)
    {
        cout << "in.irisHwMaxSpeed" << endl;
        result = false;
    }
    if (in.zoomHwTeleLimit != out.zoomHwTeleLimit)
    {
        cout << "in.zoomHwTeleLimit" << endl;
        result = false;
    }
    if (in.zoomHwWideLimit != out.zoomHwWideLimit)
    {
        cout << "in.zoomHwWideLimit" << endl;
        result = false;
    }
    if (in.focusHwFarLimit != out.focusHwFarLimit)
    {
        cout << "in.focusHwFarLimit" << endl;
        result = false;
    }
    if (in.focusHwNearLimit != out.focusHwNearLimit)
    {
        cout << "in.focusHwNearLimit" << endl;
        result = false;
    }
    if (in.irisHwOpenLimit != out.irisHwOpenLimit)
    {
        cout << "in.irisHwOpenLimit" << endl;
        result = false;
    }
    if (in.irisHwCloseLimit != out.irisHwCloseLimit)
    {
        cout << "in.irisHwCloseLimit" << endl;
        result = false;
    }
    if (in.focusFactor != out.focusFactor)
    {
        cout << "in.focusFactor" << endl;
        result = false;
    }
    if (in.isConnected != out.isConnected)
    {
        cout << "in.isConnected" << endl;
        result = false;
    }
    if (in.afHwSpeed != out.afHwSpeed)
    {
        cout << "in.afHwSpeed" << endl;
        result = false;
    }
    if (in.focusFactorThreshold != out.focusFactorThreshold)
    {
        cout << "in.focusFactorThreshold" << endl;
        result = false;
    }
    if (in.refocusTimeoutSec != out.refocusTimeoutSec)
    {
        cout << "in.refocusTimeoutSec" << endl;
        result = false;
    }
    if (in.afIsActive != out.afIsActive)
    {
        cout << "in.afIsActive" << endl;
        result = false;
    }
    if (in.irisMode != out.irisMode)
    {
        cout << "in.irisMode" << endl;
        result = false;
    }
    if (in.autoAfRoiWidth != out.autoAfRoiWidth)
    {
        cout << "in.autoAfRoiWidth" << endl;
        result = false;
    }
    if (in.autoAfRoiHeight != out.autoAfRoiHeight)
    {
        cout << "in.autoAfRoiHeight" << endl;
        result = false;
    }
    if (in.autoAfRoiBorder != out.autoAfRoiBorder)
    {
        cout << "in.autoAfRoiBorder" << endl;
        result = false;
    }
    if (in.afRoiMode != out.afRoiMode)
    {
        cout << "in.afRoiMode" << endl;
        result = false;
    }
    if (in.extenderMode != out.extenderMode)
    {
        cout << "in.extenderMode" << endl;
        result = false;
    }
    if (in.stabiliserMode != out.stabiliserMode)
    {
        cout << "in.stabiliserMode" << endl;
        result = false;
    }
    if (in.afRange != out.afRange)
    {
        cout << "in.afRange" << endl;
        result = false;
    }
    if (in.xFovDeg != out.xFovDeg)
    {
        cout << "in.xFovDeg" << endl;
        result = false;
    }
    if (in.yFovDeg != out.yFovDeg)
    {
        cout << "in.yFovDeg" << endl;
        result = false;
    }
    if (in.logMode != out.logMode)
    {
        cout << "in.logMode" << endl;
        result = false;
    }
    if (in.temperature != out.temperature)
    {
        cout << "in.temperature" << endl;
        result = false;
    }
    if (in.isOpen != out.isOpen)
    {
        cout << "in.isOpen" << endl;
        result = false;
    }
    if (in.type != out.type)
    {
        cout << "in.type" << endl;
        result = false;
    }
    if (in.custom1 != out.custom1)
    {
        cout << "in.custom1" << endl;
        result = false;
    }
    if (in.custom2 != out.custom2)
    {
        cout << "in.custom2" << endl;
        result = false;
    }
    if (in.custom3 != out.custom3)
    {
        cout << "in.custom3" << endl;
        result = false;
    }

    return result;
}



// Encode/decode commands test.
bool encodeDecodeCommandsTest()
{
    // Encode command.
    uint8_t data[1024];
    int size = 0;
    float outValue = (float)(rand() % 20);
    Lens::encodeCommand(data, size, LensCommand::AF_STOP, outValue);

    // Decode command.
    LensCommand commandId;
    LensParam paramId;
    float value = 0.0f;
    if (Lens::decodeCommand(data, size, paramId, commandId, value) != 0)
    {
        cout << "Command not decoded" << endl;
        return false;
    }

    // Checkk ID and value.
    if (commandId != LensCommand::AF_STOP)
    {
        cout << "not a LensCommand::AF_STOP" << endl;
        return false;
    }
    if (value != outValue)
    {
        cout << "not equal value" << endl;
        return false;
    }

    // Encode param.
    outValue = (float)(rand() % 20);
    Lens::encodeSetParamCommand(data, size, LensParam::AF_ROI_X0, outValue);

    // Decode command.
    value = 0.0f;
    if (Lens::decodeCommand(data, size, paramId, commandId, value) != 1)
    {
        cout << "Set param command not decoded" << endl;
        return false;
    }

    // Checkk ID and value.
    if (paramId != LensParam::AF_ROI_X0)
    {
        cout << "not a LensParam::AF_ROI_X0" << endl;
        return false;
    }
    if (value != outValue)
    {
        cout << "not equal value" << endl;
        return false;
    }

    return true;
}



/// JSON read/write test.
bool jsonReadWriteTest()
{
    // Prepare random params.
    LensParams in;
    in.initString = "dfhglsjirhuhjfb";
    in.zoomPos = rand() % 255;
    in.zoomHwPos = rand() % 255;
    in.focusPos = rand() % 255;
    in.focusHwPos = rand() % 255;
    in.irisPos = rand() % 255;
    in.irisHwPos = rand() % 255;
    in.focusMode = rand() % 255;
    in.filterMode = rand() % 255;
    in.afRoiX0 = rand() % 255;
    in.afRoiY0 = rand() % 255;
    in.afRoiX1 = rand() % 255;
    in.afRoiY1 = rand() % 255;
    in.zoomSpeed = rand() % 255;
    in.zoomHwSpeed = rand() % 255;
    in.zoomHwMaxSpeed = rand() % 255;
    in.focusSpeed = rand() % 255;
    in.focusHwSpeed = rand() % 255;
    in.focusHwMaxSpeed = rand() % 255;
    in.irisSpeed = rand() % 255;
    in.irisHwSpeed = rand() % 255;
    in.irisHwMaxSpeed = rand() % 255;
    in.zoomHwTeleLimit = rand() % 255;
    in.zoomHwWideLimit = rand() % 255;
    in.focusHwFarLimit = rand() % 255;
    in.focusHwNearLimit = rand() % 255;
    in.irisHwOpenLimit = rand() % 255;
    in.irisHwCloseLimit = rand() % 255;
    in.focusFactor = rand() % 255;
    in.isConnected = true;
    in.afHwSpeed = rand() % 255;
    in.focusFactorThreshold = rand() % 255;
    in.refocusTimeoutSec = rand() % 255;
    in.afIsActive = true;
    in.irisMode = rand() % 255;
    in.autoAfRoiWidth = rand() % 255;
    in.autoAfRoiHeight = rand() % 255;
    in.autoAfRoiBorder = rand() % 255;
    in.afRoiMode = rand() % 255;
    in.extenderMode = rand() % 255;
    in.stabiliserMode = rand() % 255;
    in.afRange = rand() % 255;
    in.xFovDeg = rand() % 255;
    in.yFovDeg = rand() % 255;
    in.logMode = rand() % 255;
    in.temperature = rand() % 255;
    in.isOpen = true;
    in.type = rand() % 255;
    in.custom1 = rand() % 255;
    in.custom2 = rand() % 255;
    in.custom3 = rand() % 255;
    for (int i = 0; i < 5; ++i)
    {
        FovPoint pt;
        pt.hwZoomPos = rand() % 255;
        pt.xFovDeg = rand() % 255;
        pt.yFovDeg = rand() % 255;
        in.fovPoints.push_back(pt);
    }

    // Write params to file.
    cr::utils::ConfigReader inConfig;
    inConfig.set(in, "lensParams");
    inConfig.writeToFile("TestLensParams.json");

    // Read params from file.
    cr::utils::ConfigReader outConfig;
    if(!outConfig.readFromFile("TestLensParams.json"))
    {
        cout << "Can't open config file" << endl;
        return false;
    }

    LensParams out;
    if(!outConfig.get(out, "lensParams"))
    {
        cout << "Can't read params from file" << endl;
        return false;
    }

    // Compare params.
    bool result = true;
    if (in.initString != out.initString)
    {
        cout << "in.initString" << endl;
        result = false;
    }
    if (in.focusMode != out.focusMode)
    {
        cout << "in.focusMode" << endl;
        result = false;
    }
    if (in.filterMode != out.filterMode)
    {
        cout << "in.filterMode" << endl;
        result = false;
    }
    if (in.afRoiX0 != out.afRoiX0)
    {
        cout << "in.afRoiX0" << endl;
        result = false;
    }
    if (in.afRoiY0 != out.afRoiY0)
    {
        cout << "in.afRoiY0" << endl;
        result = false;
    }
    if (in.afRoiX1 != out.afRoiX1)
    {
        cout << "in.afRoiX1" << endl;
        result = false;
    }
    if (in.afRoiY1 != out.afRoiY1)
    {
        cout << "in.afRoiY1" << endl;
        result = false;
    }
    if (in.zoomHwMaxSpeed != out.zoomHwMaxSpeed)
    {
        cout << "in.zoomHwMaxSpeed" << endl;
        result = false;
    }
    if (in.focusHwMaxSpeed != out.focusHwMaxSpeed)
    {
        cout << "in.focusHwMaxSpeed" << endl;
        result = false;
    }
    if (in.irisHwMaxSpeed != out.irisHwMaxSpeed)
    {
        cout << "in.irisHwMaxSpeed" << endl;
        result = false;
    }
    if (in.zoomHwTeleLimit != out.zoomHwTeleLimit)
    {
        cout << "in.zoomHwTeleLimit" << endl;
        result = false;
    }
    if (in.zoomHwWideLimit != out.zoomHwWideLimit)
    {
        cout << "in.zoomHwWideLimit" << endl;
        result = false;
    }
    if (in.focusHwFarLimit != out.focusHwFarLimit)
    {
        cout << "in.focusHwFarLimit" << endl;
        result = false;
    }
    if (in.focusHwNearLimit != out.focusHwNearLimit)
    {
        cout << "in.focusHwNearLimit" << endl;
        result = false;
    }
    if (in.irisHwOpenLimit != out.irisHwOpenLimit)
    {
        cout << "in.irisHwOpenLimit" << endl;
        result = false;
    }
    if (in.irisHwCloseLimit != out.irisHwCloseLimit)
    {
        cout << "in.irisHwCloseLimit" << endl;
        result = false;
    }
    if (in.afHwSpeed != out.afHwSpeed)
    {
        cout << "in.afHwSpeed" << endl;
        result = false;
    }
    if (in.focusFactorThreshold != out.focusFactorThreshold)
    {
        cout << "in.focusFactorThreshold" << endl;
        result = false;
    }
    if (in.refocusTimeoutSec != out.refocusTimeoutSec)
    {
        cout << "in.refocusTimeoutSec" << endl;
        result = false;
    }
    if (in.irisMode != out.irisMode)
    {
        cout << "in.irisMode" << endl;
        result = false;
    }
    if (in.autoAfRoiWidth != out.autoAfRoiWidth)
    {
        cout << "in.autoAfRoiWidth" << endl;
        result = false;
    }
    if (in.autoAfRoiHeight != out.autoAfRoiHeight)
    {
        cout << "in.autoAfRoiHeight" << endl;
        result = false;
    }
    if (in.autoAfRoiBorder != out.autoAfRoiBorder)
    {
        cout << "in.autoAfRoiBorder" << endl;
        result = false;
    }
    if (in.afRoiMode != out.afRoiMode)
    {
        cout << "in.afRoiMode" << endl;
        result = false;
    }
    if (in.extenderMode != out.extenderMode)
    {
        cout << "in.extenderMode" << endl;
        result = false;
    }
    if (in.stabiliserMode != out.stabiliserMode)
    {
        cout << "in.stabiliserMode" << endl;
        result = false;
    }
    if (in.afRange != out.afRange)
    {
        cout << "in.afRange" << endl;
        result = false;
    }
    if (in.logMode != out.logMode)
    {
        cout << "in.logMode" << endl;
        result = false;
    }
    if (in.type != out.type)
    {
        cout << "in.type" << endl;
        result = false;
    }
    if (in.custom1 != out.custom1)
    {
        cout << "in.custom1" << endl;
        result = false;
    }
    if (in.custom2 != out.custom2)
    {
        cout << "in.custom2" << endl;
        result = false;
    }
    if (in.custom3 != out.custom3)
    {
        cout << "in.custom3" << endl;
        result = false;
    }
    if (in.fovPoints.size() != out.fovPoints.size())
    {
        cout << "in.fovPoints.size()" << endl;
        result = false;
    }
    for (int i = 0; i < in.fovPoints.size(); ++i)
    {
        if (in.fovPoints[i].hwZoomPos != out.fovPoints[i].hwZoomPos)
        {
            cout << "in.fovPoints[" << i << "].hwZoomPos" << endl;
            result = false;
        }
        if (in.fovPoints[i].xFovDeg != out.fovPoints[i].xFovDeg)
        {
            cout << "in.fovPoints[" << i << "].xFovDeg" << endl;
            result = false;
        }
        if (in.fovPoints[i].yFovDeg != out.fovPoints[i].yFovDeg)
        {
            cout << "in.fovPoints[" << i << "].yFovDeg" << endl;
            result = false;
        }
    }

    return result;
}




/// Encode/decode test with params mask.
bool encodeDecodeParamsWithMaskTest()
{
    // Prepare random params.
    LensParams in;
    in.initString = "dfhglsjirhuhjfb";
    in.zoomPos = rand() % 255;
    in.zoomHwPos = rand() % 255;
    in.focusPos = rand() % 255;
    in.focusHwPos = rand() % 255;
    in.irisPos = rand() % 255;
    in.irisHwPos = rand() % 255;
    in.focusMode = rand() % 255;
    in.filterMode = rand() % 255;
    in.afRoiX0 = rand() % 255;
    in.afRoiY0 = rand() % 255;
    in.afRoiX1 = rand() % 255;
    in.afRoiY1 = rand() % 255;
    in.zoomSpeed = rand() % 255;
    in.zoomHwSpeed = rand() % 255;
    in.zoomHwMaxSpeed = rand() % 255;
    in.focusSpeed = rand() % 255;
    in.focusHwSpeed = rand() % 255;
    in.focusHwMaxSpeed = rand() % 255;
    in.irisSpeed = rand() % 255;
    in.irisHwSpeed = rand() % 255;
    in.irisHwMaxSpeed = rand() % 255;
    in.zoomHwTeleLimit = rand() % 255;
    in.zoomHwWideLimit = rand() % 255;
    in.focusHwFarLimit = rand() % 255;
    in.focusHwNearLimit = rand() % 255;
    in.irisHwOpenLimit = rand() % 255;
    in.irisHwCloseLimit = rand() % 255;
    in.focusFactor = rand() % 255;
    in.isConnected = true;
    in.afHwSpeed = rand() % 255;
    in.focusFactorThreshold = rand() % 255;
    in.refocusTimeoutSec = rand() % 255;
    in.afIsActive = true;
    in.irisMode = rand() % 255;
    in.autoAfRoiWidth = rand() % 255;
    in.autoAfRoiHeight = rand() % 255;
    in.autoAfRoiBorder = rand() % 255;
    in.afRoiMode = rand() % 255;
    in.extenderMode = rand() % 255;
    in.stabiliserMode = rand() % 255;
    in.afRange = rand() % 255;
    in.xFovDeg = rand() % 255;
    in.yFovDeg = rand() % 255;
    in.logMode = rand() % 255;
    in.temperature = rand() % 255;
    in.isOpen = true;
    in.type = rand() % 255;
    in.custom1 = rand() % 255;
    in.custom2 = rand() % 255;
    in.custom3 = rand() % 255;
    for (int i = 0; i < 5; ++i)
    {
        FovPoint pt;
        pt.hwZoomPos = rand() % 255;
        pt.xFovDeg = rand() % 255;
        pt.yFovDeg = rand() % 255;
        in.fovPoints.push_back(pt);
    }

    // Prepare mask.
    LensParamsMask mask;
    mask.zoomPos = true;
    mask.zoomHwPos = false;
    mask.focusPos = true;
    mask.focusHwPos = false;
    mask.irisPos = true;
    mask.irisHwPos = false;
    mask.focusMode = true;
    mask.filterMode = false;
    mask.afRoiX0 = true;
    mask.afRoiY0 = false;
    mask.afRoiX1 = true;
    mask.afRoiY1 = false;
    mask.zoomSpeed = true;
    mask.zoomHwSpeed = false;
    mask.zoomHwMaxSpeed = true;
    mask.focusSpeed = false;
    mask.focusHwSpeed = true;
    mask.focusHwMaxSpeed = false;
    mask.irisSpeed = true;
    mask.irisHwSpeed = false;
    mask.irisHwMaxSpeed = true;
    mask.zoomHwTeleLimit = false;
    mask.zoomHwWideLimit = true;
    mask.focusHwFarLimit = false;
    mask.focusHwNearLimit = true;
    mask.irisHwOpenLimit = false;
    mask.irisHwCloseLimit = true;
    mask.focusFactor = false;
    mask.isConnected = true;
    mask.afHwSpeed = false;
    mask.focusFactorThreshold = true;
    mask.refocusTimeoutSec = false;
    mask.afIsActive = true;
    mask.irisMode = false;
    mask.autoAfRoiWidth = true;
    mask.autoAfRoiHeight = false;
    mask.autoAfRoiBorder = true;
    mask.afRoiMode = false;
    mask.extenderMode = true;
    mask.stabiliserMode = false;
    mask.afRange = true;
    mask.xFovDeg = false;
    mask.yFovDeg = true;
    mask.logMode = false;
    mask.temperature = true;
    mask.isOpen = false;
    mask.type = true;
    mask.custom1 = false;
    mask.custom2 = true;
    mask.custom3 = false;

    // Encode data.
    uint8_t data[1024];
    int size = 0;
    in.encode(data, 1024, size, &mask);

    cout << "Encoded data size: " << size << " bytes" << endl;

    // Decode data.
    LensParams out;
    if (!out.decode(data, size))
    {
        cout << "Can't decode data" << endl;
        return false;
    }

    // Compare params.
    bool result = true;
    if (out.initString != "")
    {
        cout << "in.initString" << endl;
        result = false;
    }
    if (in.zoomPos != out.zoomPos)
    {
        cout << "in.zoomPos" << endl;
        result = false;
    }
    if (0 != out.zoomHwPos)
    {
        cout << "in.zoomHwPos" << endl;
        result = false;
    }
    if (in.focusPos != out.focusPos)
    {
        cout << "in.focusPos" << endl;
        result = false;
    }
    if (0 != out.focusHwPos)
    {
        cout << "in.focusHwPos" << endl;
        result = false;
    }
    if (in.irisPos != out.irisPos)
    {
        cout << "in.irisPos" << endl;
        result = false;
    }
    if (0 != out.irisHwPos)
    {
        cout << "in.irisHwPos" << endl;
        result = false;
    }
    if (in.focusMode != out.focusMode)
    {
        cout << "in.focusMode" << endl;
        result = false;
    }
    if (0 != out.filterMode)
    {
        cout << "in.filterMode" << endl;
        result = false;
    }
    if (in.afRoiX0 != out.afRoiX0)
    {
        cout << "in.afRoiX0" << endl;
        result = false;
    }
    if (0 != out.afRoiY0)
    {
        cout << "in.afRoiY0" << endl;
        result = false;
    }
    if (in.afRoiX1 != out.afRoiX1)
    {
        cout << "in.afRoiX1" << endl;
        result = false;
    }
    if (0 != out.afRoiY1)
    {
        cout << "in.afRoiY1" << endl;
        result = false;
    }
    if (in.zoomSpeed != out.zoomSpeed)
    {
        cout << "in.zoomSpeed" << endl;
        result = false;
    }
    if (0 != out.zoomHwSpeed)
    {
        cout << "in.zoomHwSpeed" << endl;
        result = false;
    }
    if (in.zoomHwMaxSpeed != out.zoomHwMaxSpeed)
    {
        cout << "in.zoomHwMaxSpeed" << endl;
        result = false;
    }
    if (0 != out.focusSpeed)
    {
        cout << "in.focusSpeed" << endl;
        result = false;
    }
    if (in.focusHwSpeed != out.focusHwSpeed)
    {
        cout << "in.focusHwSpeed" << endl;
        result = false;
    }
    if (0 != out.focusHwMaxSpeed)
    {
        cout << "in.focusHwMaxSpeed" << endl;
        result = false;
    }
    if (in.irisSpeed != out.irisSpeed)
    {
        cout << "in.irisSpeed" << endl;
        result = false;
    }
    if (0 != out.irisHwSpeed)
    {
        cout << "in.irisHwSpeed" << endl;
        result = false;
    }
    if (in.irisHwMaxSpeed != out.irisHwMaxSpeed)
    {
        cout << "in.irisHwMaxSpeed" << endl;
        result = false;
    }
    if (0 != out.zoomHwTeleLimit)
    {
        cout << "in.zoomHwTeleLimit" << endl;
        result = false;
    }
    if (in.zoomHwWideLimit != out.zoomHwWideLimit)
    {
        cout << "in.zoomHwWideLimit" << endl;
        result = false;
    }
    if (0 != out.focusHwFarLimit)
    {
        cout << "in.focusHwFarLimit" << endl;
        result = false;
    }
    if (in.focusHwNearLimit != out.focusHwNearLimit)
    {
        cout << "in.focusHwNearLimit" << endl;
        result = false;
    }
    if (0 != out.irisHwOpenLimit)
    {
        cout << "in.irisHwOpenLimit" << endl;
        result = false;
    }
    if (in.irisHwCloseLimit != out.irisHwCloseLimit)
    {
        cout << "in.irisHwCloseLimit" << endl;
        result = false;
    }
    if (0 != out.focusFactor)
    {
        cout << "in.focusFactor" << endl;
        result = false;
    }
    if (in.isConnected != out.isConnected)
    {
        cout << "in.isConnected" << endl;
        result = false;
    }
    if (0 != out.afHwSpeed)
    {
        cout << "in.afHwSpeed" << endl;
        result = false;
    }
    if (in.focusFactorThreshold != out.focusFactorThreshold)
    {
        cout << "in.focusFactorThreshold" << endl;
        result = false;
    }
    if (0 != out.refocusTimeoutSec)
    {
        cout << "in.refocusTimeoutSec" << endl;
        result = false;
    }
    if (in.afIsActive != out.afIsActive)
    {
        cout << "in.afIsActive" << endl;
        result = false;
    }
    if (0 != out.irisMode)
    {
        cout << "in.irisMode" << endl;
        result = false;
    }
    if (in.autoAfRoiWidth != out.autoAfRoiWidth)
    {
        cout << "in.autoAfRoiWidth" << endl;
        result = false;
    }
    if (0 != out.autoAfRoiHeight)
    {
        cout << "in.autoAfRoiHeight" << endl;
        result = false;
    }
    if (in.autoAfRoiBorder != out.autoAfRoiBorder)
    {
        cout << "in.autoAfRoiBorder" << endl;
        result = false;
    }
    if (0 != out.afRoiMode)
    {
        cout << "in.afRoiMode" << endl;
        result = false;
    }
    if (in.extenderMode != out.extenderMode)
    {
        cout << "in.extenderMode" << endl;
        result = false;
    }
    if (0 != out.stabiliserMode)
    {
        cout << "in.stabiliserMode" << endl;
        result = false;
    }
    if (in.afRange != out.afRange)
    {
        cout << "in.afRange" << endl;
        result = false;
    }
    if (0.0f != out.xFovDeg)
    {
        cout << "in.xFovDeg" << endl;
        result = false;
    }
    if (in.yFovDeg != out.yFovDeg)
    {
        cout << "in.yFovDeg" << endl;
        result = false;
    }
    if (0 != out.logMode)
    {
        cout << "in.logMode" << endl;
        result = false;
    }
    if (in.temperature != out.temperature)
    {
        cout << "in.temperature" << endl;
        result = false;
    }
    if (false != out.isOpen)
    {
        cout << "in.isOpen" << endl;
        result = false;
    }
    if (in.type != out.type)
    {
        cout << "in.type" << endl;
        result = false;
    }
    if (0.0f != out.custom1)
    {
        cout << "in.custom1" << endl;
        result = false;
    }
    if (in.custom2 != out.custom2)
    {
        cout << "in.custom2" << endl;
        result = false;
    }
    if (0.0f != out.custom3)
    {
        cout << "in.custom3" << endl;
        result = false;
    }

    return result;
}















