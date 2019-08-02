﻿/************************************************************************************************
 * This code was automatically generated by Digital Twin Code Generator tool 0.3.8.
 * Changes to this file may cause incorrect behavior and will be lost if the code is regenerated.
 *
 * Generated Date: Thursday, August 1, 2019
 ***********************************************************************************************/

#include "sensors_interface.h"

#define MAX_MASSAGE_SIZE 256

//
// Application state associated with this interface. In particular it contains 
// the DIGITALTWIN_INTERFACE_CLIENT_HANDLE used for responses in callbacks along with properties set
// and representations of the properrty update and command callbacks invoked on given interface
//
typedef struct SENSORS_INTERFACE_STATE_TAG
{
    DIGITALTWIN_INTERFACE_CLIENT_HANDLE interfaceClientHandle;

} SENSORS_INTERFACE_STATE;

static SENSORS_INTERFACE_STATE appState;

// SensorsInterface_TelemetryCallback is invoked when a DigitalTwin telemetry message is either successfully delivered to the service or else fails.
static void SensorsInterface_TelemetryCallback(DIGITALTWIN_CLIENT_RESULT digitalTwinTelemetryStatus, void* userContextCallback)
{
    if (digitalTwinTelemetryStatus == DIGITALTWIN_CLIENT_OK)
    {
        SendTelemetry_Succeeded_Callback("Sensors", (const char*)userContextCallback);
        LogInfo("SENSORS_INTERFACE: DigitalTwin successfully delivered telemetry message for <%s>", (const char*)userContextCallback);
    }
    else
    {
        SendTelemetry_Error_Callback("Sensors", (const char*)userContextCallback);
        LogError("SENSORS_INTERFACE: DigitalTwin failed to deliver telemetry message for <%s>, error=<%s> ", (const char*)userContextCallback, MU_ENUM_TO_STRING(DIGITALTWIN_CLIENT_RESULT, digitalTwinTelemetryStatus));
    }
}

// SendTelemetry is a helper funcion which is periodically invoked by the caller to send telemetry
DIGITALTWIN_CLIENT_RESULT SensorsInterface_SendTelemetry_Internal(DIGITALTWIN_INTERFACE_CLIENT_HANDLE interfaceHandle, const char* telemetryName, const char* telemetryData)
{
    DIGITALTWIN_CLIENT_RESULT result;

    result = DigitalTwin_InterfaceClient_SendTelemetryAsync(interfaceHandle, telemetryName, telemetryData,
        SensorsInterface_TelemetryCallback, (void*)telemetryName);

    if (result != DIGITALTWIN_CLIENT_OK)
    {
        LogError("SENSORS_INTERFACE:: DigitalTwin_InterfaceClient_SendTelemetryAsync failed for sending %s", telemetryName);
    }
    else
    {
        LogInfo("SENSORS_INTERFACE:: DigitalTwin_InterfaceClient_SendTelemetryAsync successfully sent %s", telemetryName);
    }

    DigitalTwinClientHelper_Check();
    return result;
}

DIGITALTWIN_CLIENT_RESULT SensorsInterface_Telemetry_SendAll()
{
    DIGITALTWIN_CLIENT_RESULT result;

    // NOTE: Future versions of SDK will support ability to send mulitiple telemetries in a single
    // send.  For now, one at a time is sufficient albeit less effecient.
    if (((result = SensorsInterface_Telemetry_SendHumidity()) != DIGITALTWIN_CLIENT_OK) ||
        ((result = SensorsInterface_Telemetry_SendTemperature()) != DIGITALTWIN_CLIENT_OK) ||
        ((result = SensorsInterface_Telemetry_SendPressure()) != DIGITALTWIN_CLIENT_OK) ||
        ((result = SensorsInterface_Telemetry_SendMagnetometer()) != DIGITALTWIN_CLIENT_OK) ||
        ((result = SensorsInterface_Telemetry_SendGyroscope()) != DIGITALTWIN_CLIENT_OK) ||
        ((result = SensorsInterface_Telemetry_SendAccelerometer()) != DIGITALTWIN_CLIENT_OK) )

    {
        LogError("SENSORS_INTERFACE: failed to send telemetries");
    }
    else
    {
        LogInfo("SENSORS_INTERFACE: Queing of all telemetries to be sent has succeeded");
    }

    return result;
}

DIGITALTWIN_CLIENT_RESULT SensorsInterface_Telemetry_SendHumidity()
{
    if (appState.interfaceClientHandle == NULL)
    {
        LogError("SENSORS_INTERFACE: interfaceClientHandle is required to be initialized before sending telemetries");
    }

    char payloadBuffer[MAX_MASSAGE_SIZE];
    if (Sensors_SerializeHumidityTelemetry(payloadBuffer, MAX_MASSAGE_SIZE))
    {
        return SensorsInterface_SendTelemetry_Internal(appState.interfaceClientHandle, SensorsInterface_HumidityTelemetry, payloadBuffer);
    }
    else
    {
        LogError("SENSORS_INTERFACE: serialize temeletry message humidity failed.");
        return DIGITALTWIN_CLIENT_ERROR;
    }
}

DIGITALTWIN_CLIENT_RESULT SensorsInterface_Telemetry_SendTemperature()
{
    if (appState.interfaceClientHandle == NULL)
    {
        LogError("SENSORS_INTERFACE: interfaceClientHandle is required to be initialized before sending telemetries");
    }

    char payloadBuffer[MAX_MASSAGE_SIZE];
    if (Sensors_SerializeTemperatureTelemetry(payloadBuffer, MAX_MASSAGE_SIZE))
    {
        return SensorsInterface_SendTelemetry_Internal(appState.interfaceClientHandle, SensorsInterface_TemperatureTelemetry, payloadBuffer);
    }
    else
    {
        LogError("SENSORS_INTERFACE: serialize temeletry message temperature failed.");
        return DIGITALTWIN_CLIENT_ERROR;
    }
}

DIGITALTWIN_CLIENT_RESULT SensorsInterface_Telemetry_SendPressure()
{
    if (appState.interfaceClientHandle == NULL)
    {
        LogError("SENSORS_INTERFACE: interfaceClientHandle is required to be initialized before sending telemetries");
    }

    char payloadBuffer[MAX_MASSAGE_SIZE];
    if (Sensors_SerializePressureTelemetry(payloadBuffer, MAX_MASSAGE_SIZE))
    {
        return SensorsInterface_SendTelemetry_Internal(appState.interfaceClientHandle, SensorsInterface_PressureTelemetry, payloadBuffer);
    }
    else
    {
        LogError("SENSORS_INTERFACE: serialize temeletry message pressure failed.");
        return DIGITALTWIN_CLIENT_ERROR;
    }
}

DIGITALTWIN_CLIENT_RESULT SensorsInterface_Telemetry_SendMagnetometer()
{
    if (appState.interfaceClientHandle == NULL)
    {
        LogError("SENSORS_INTERFACE: interfaceClientHandle is required to be initialized before sending telemetries");
    }

    char payloadBuffer[MAX_MASSAGE_SIZE];
    if (Sensors_SerializeMagnetometerTelemetry(payloadBuffer, MAX_MASSAGE_SIZE))
    {
        return SensorsInterface_SendTelemetry_Internal(appState.interfaceClientHandle, SensorsInterface_MagnetometerTelemetry, payloadBuffer);
    }
    else
    {
        LogError("SENSORS_INTERFACE: serialize temeletry message magnetometer failed.");
        return DIGITALTWIN_CLIENT_ERROR;
    }
}

DIGITALTWIN_CLIENT_RESULT SensorsInterface_Telemetry_SendGyroscope()
{
    if (appState.interfaceClientHandle == NULL)
    {
        LogError("SENSORS_INTERFACE: interfaceClientHandle is required to be initialized before sending telemetries");
    }

    char payloadBuffer[MAX_MASSAGE_SIZE];
    if (Sensors_SerializeGyroscopeTelemetry(payloadBuffer, MAX_MASSAGE_SIZE))
    {
        return SensorsInterface_SendTelemetry_Internal(appState.interfaceClientHandle, SensorsInterface_GyroscopeTelemetry, payloadBuffer);
    }
    else
    {
        LogError("SENSORS_INTERFACE: serialize temeletry message gyroscope failed.");
        return DIGITALTWIN_CLIENT_ERROR;
    }
}

DIGITALTWIN_CLIENT_RESULT SensorsInterface_Telemetry_SendAccelerometer()
{
    if (appState.interfaceClientHandle == NULL)
    {
        LogError("SENSORS_INTERFACE: interfaceClientHandle is required to be initialized before sending telemetries");
    }

    char payloadBuffer[MAX_MASSAGE_SIZE];
    if (Sensors_SerializeAccelerometerTelemetry(payloadBuffer, MAX_MASSAGE_SIZE))
    {
        return SensorsInterface_SendTelemetry_Internal(appState.interfaceClientHandle, SensorsInterface_AccelerometerTelemetry, payloadBuffer);
    }
    else
    {
        LogError("SENSORS_INTERFACE: serialize temeletry message accelerometer failed.");
        return DIGITALTWIN_CLIENT_ERROR;
    }
}

// SensorsInterface_InterfaceRegisteredCallback is invoked when this interface
// is successfully or unsuccessfully registered with the service, and also when the interface is deleted.
static void SensorsInterface_InterfaceRegisteredCallback(DIGITALTWIN_CLIENT_RESULT dtInterfaceStatus, void* userInterfaceContext)
{
    LogInfo("SensorsInterface_InterfaceRegisteredCallback with status=<%s>, userContext=<%p>", MU_ENUM_TO_STRING(DIGITALTWIN_CLIENT_RESULT, dtInterfaceStatus), userInterfaceContext);
    if (dtInterfaceStatus == DIGITALTWIN_CLIENT_OK)
    {
        // Once the interface is registered, send our reported properties to the service.  
        // It *IS* safe to invoke most DigitalTwin API calls from a callback thread like this, though it 
        // is NOT safe to create/destroy/register interfaces now.
        LogInfo("SENSORS_INTERFACE: Interface successfully registered.");
    }
    else if (dtInterfaceStatus == DIGITALTWIN_CLIENT_ERROR_INTERFACE_UNREGISTERING)
    {
        // Once an interface is marked as unregistered, it cannot be used for any DigitalTwin SDK calls.
        LogInfo("SENSORS_INTERFACE: Interface received unregistering callback.");
    }
    else
    {
        LogError("SENSORS_INTERFACE: Interface received failed, status=<%s>.", MU_ENUM_TO_STRING(DIGITALTWIN_CLIENT_RESULT, dtInterfaceStatus));
    }
}

//
// Create DigitalTwin interface cllient handle
//
DIGITALTWIN_INTERFACE_CLIENT_HANDLE SensorsInterface_Create()
{
    DIGITALTWIN_INTERFACE_CLIENT_HANDLE interfaceHandle;
    DIGITALTWIN_CLIENT_RESULT result;

    memset(&appState, 0, sizeof(SENSORS_INTERFACE_STATE));

    if ((result = DigitalTwin_InterfaceClient_Create(SensorsInterfaceId,  SensorsComponentName, SensorsInterface_InterfaceRegisteredCallback, (void*)&appState, &interfaceHandle)) != DIGITALTWIN_CLIENT_OK)
    {
        LogError("SENSORS_INTERFACE: Unable to allocate interface client handle for interfaceId=<%s>, componentName=<%s>, error=<%s>", SensorsInterfaceId, SensorsComponentName, MU_ENUM_TO_STRING(DIGITALTWIN_CLIENT_RESULT, result));
        interfaceHandle = NULL;
    }

    else
    {
        LogInfo("SENSORS_INTERFACE: Created DIGITALTWIN_INTERFACE_CLIENT_HANDLE successfully for interfaceId=<%s>, componentName=<%s>, handle=<%p>", SensorsInterfaceId, SensorsComponentName, interfaceHandle);
        appState.interfaceClientHandle = interfaceHandle;
    }

    return interfaceHandle;
}

void SensorsInterface_Close(DIGITALTWIN_INTERFACE_CLIENT_HANDLE digitalTwinInterfaceClientHandle)
{
    // On shutdown, in general the first call made should be to DigitalTwin_InterfaceClient_Destroy.
    // This will block if there are any active callbacks in this interface, and then
    // mark the underlying handle such that no future callbacks shall come to it.
    DigitalTwin_InterfaceClient_Destroy(digitalTwinInterfaceClientHandle);

    // After DigitalTwin_InterfaceClient_Destroy returns, it is safe to assume
    // no more callbacks shall arrive for this interface and it is OK to free
    // resources callbacks otherwise may have needed.

}
