﻿/************************************************************************************************
 * This code was automatically generated by Digital Twin Code Generator tool 0.3.9.
 * Changes to this file may cause incorrect behavior and will be lost if the code is regenerated.
 *
 * Generated Date: Monday, August 5, 2019
 ***********************************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "digitaltwin_client_helper.h"

// Amount to sleep between querying state from the register interface loop
static const int RegisterInterfacePollSleep = 100;

// Maximum amount of times we'll poll for interface being ready.  Current defaults between this and
// 'RegisterInterfacePollSleep' means we'll wait up to a minute for interface registration.
static const int RegisterInterfaceMaxPolls = 600;

static IOTHUB_DEVICE_CLIENT_LL_HANDLE deviceHandle;
static DIGITALTWIN_DEVICE_CLIENT_LL_HANDLE digitalTwinDeviceClientHandle;

bool DigitalTwinClientHelper_SetOption(const char* optionName, const void* value)
{
    if (deviceHandle == NULL || optionName == NULL || value == NULL)
    {
        return false;
    }

    IOTHUB_CLIENT_RESULT iothubClientResult;

    if ((iothubClientResult = IoTHubDeviceClient_LL_SetOption(deviceHandle, optionName, value)) != IOTHUB_CLIENT_OK)
    {
        LogError("Failed to set option %s, error=%d", optionName, iothubClientResult);
        IoTHubDeviceClient_LL_Destroy(deviceHandle);
        deviceHandle = NULL;

        IoTHub_Deinit();
        return false;
    }
    else
    {
        return true;
    }
}

DIGITALTWIN_DEVICE_CLIENT_LL_HANDLE DigitalTwinClientHelper_InitializeDeviceHandle(const char* connectionString, bool traceOn, const char * trustedCert)
{
    DIGITALTWIN_CLIENT_RESULT result;
    bool urlEncodeOn = true;
    deviceHandle = NULL;

    if (IoTHub_Init() != 0)
    {
        LogError("IoTHub_Init failed");
    }
    else
    {
        // First, we create a standard IOTHUB_DEVICE_HANDLE handle for DigitalTwin to consume.
        if ((deviceHandle = IoTHubDeviceClient_LL_CreateFromConnectionString(connectionString, MQTT_Protocol)) == NULL)
        {
            LogError("Failed to create device handle");
            return NULL;
        }

        DigitalTwinClientHelper_SetOption(OPTION_LOG_TRACE, &traceOn);
        DigitalTwinClientHelper_SetOption("product_info", "pnp_codegen_v0.3.9");

        // Setting OPTION_AUTO_URL_ENCODE_DECODE is required by callers only for private-preview.  
        // https://github.com/Azure/azure-iot-sdk-c-pnp/issues/2 tracks making underlying DigitalTwin set this automatically.
        DigitalTwinClientHelper_SetOption(OPTION_AUTO_URL_ENCODE_DECODE, &urlEncodeOn);

        if (trustedCert != NULL)
        {
            DigitalTwinClientHelper_SetOption("TrustedCerts", trustedCert);
        }

        if ((result = DigitalTwin_DeviceClient_LL_CreateFromDeviceHandle(deviceHandle, &digitalTwinDeviceClientHandle)) != DIGITALTWIN_CLIENT_OK)
        {
            LogError("DigitalTwin_DeviceClient_LL_CreateFromDeviceHandle failed, error=<%s>", MU_ENUM_TO_STRING(DIGITALTWIN_CLIENT_RESULT, result));
        }
        else
        {
            // Never log the complete connection string , as this contains information
            // that could compromise security of the device.
            LogInfo("Successfully created DigitalTwin device with connectionString=<****>, deviceHandle=<%p>", deviceHandle);
        }

        if (deviceHandle == NULL)
        {
            LogError("Could not allocate IoTHub Device handle");
            IoTHub_Deinit();
        }
    }

    return digitalTwinDeviceClientHandle;
}

// DigitalTwinClientHelper_InterfacesRegisteredCallback is invoked when the interfaces have been registered or failed.
// The userContextCallback pointer is set to whether we succeeded or failed and checked by thread blocking
// for registration to complete.
void DigitalTwinClientHelper_InterfacesRegisteredCallback(DIGITALTWIN_CLIENT_RESULT digitalTwinInterfaceStatus, void *userContextCallback)
{
    DIGITALTWIN_REGISTRATION_STATUS* digitalTwinRegistrationStatus = (DIGITALTWIN_REGISTRATION_STATUS*)userContextCallback;

    if (digitalTwinInterfaceStatus == DIGITALTWIN_CLIENT_OK)
    {
        LogInfo("Interface registration callback invoked, interfaces have been successfully registered");
        *digitalTwinRegistrationStatus = DIGITALTWIN_REGISTRATION_SUCCEEDED;
    }
    else
    {
        LogError("Interface registration callback invoked with an error=<%s>", MU_ENUM_TO_STRING(DIGITALTWIN_CLIENT_RESULT, digitalTwinInterfaceStatus));
        *digitalTwinRegistrationStatus = DIGITALTWIN_REGISTRATION_FAILED;
    }
}

DIGITALTWIN_CLIENT_RESULT DigitalTwinClientHelper_RegisterInterfacesAndWait(DIGITALTWIN_DEVICE_CLIENT_LL_HANDLE digitalTwinDeviceHandle, const char* deviceCapabilityModelUri, DIGITALTWIN_INTERFACE_CLIENT_HANDLE* interfaceClientHandles, int numInterfaceClientHandles)
{
    DIGITALTWIN_CLIENT_RESULT result;
    DIGITALTWIN_REGISTRATION_STATUS digitalTwinRegistrationStatus = DIGITALTWIN_REGISTRATION_PENDING;

    // Give DigitalTwin interfaces to register. DigitalTwin_DeviceClient_LL_RegisterInterfacesAsync returns immediately
    if ((result = DigitalTwin_DeviceClient_LL_RegisterInterfacesAsync(digitalTwinDeviceHandle, deviceCapabilityModelUri, interfaceClientHandles, numInterfaceClientHandles, DigitalTwinClientHelper_InterfacesRegisteredCallback, &digitalTwinRegistrationStatus)) != DIGITALTWIN_CLIENT_OK)
    {
        LogError("DigitalTwin_DeviceClient_LL_RegisterInterfacesAsync failed, error=<%s>", MU_ENUM_TO_STRING(DIGITALTWIN_CLIENT_RESULT, result));
    }
    else
    {
        // After registration, we do a simple polling algorithm to check for whether the callback
        // DigitalTwinClientHelper_InterfacesRegisteredCallback has changed digitalTwinRegistrationStatus. 
        // Since we can't do any other DigitalTwin operations at this point, we have to block here.
        for (int i = 0; (i < RegisterInterfaceMaxPolls) && (digitalTwinRegistrationStatus == DIGITALTWIN_REGISTRATION_PENDING); i++)
        {
            DigitalTwin_DeviceClient_LL_DoWork(digitalTwinDeviceHandle);
            ThreadAPI_Sleep(RegisterInterfacePollSleep);
        }

        if (digitalTwinRegistrationStatus == DIGITALTWIN_REGISTRATION_SUCCEEDED)
        {
            LogInfo("DigitalTwin interfaces successfully registered");
            result = DIGITALTWIN_CLIENT_OK;
        }
        else if (digitalTwinRegistrationStatus == DIGITALTWIN_REGISTRATION_PENDING)
        {
            LogError("Timed out attempting to register DigitalTwin interfaces");
            result = DIGITALTWIN_CLIENT_ERROR;
        }
        else
        {
            LogError("Error registering DigitalTwin interfaces");
            result = DIGITALTWIN_CLIENT_ERROR;
        }
    }

    return result;
}

int DigitalTwinClientHelper_SetCommandResponse(DIGITALTWIN_CLIENT_COMMAND_RESPONSE* commandResponse, const char* responseData, int status)
{
    memset(commandResponse, 0, sizeof(*commandResponse));
    commandResponse->version = DIGITALTWIN_CLIENT_COMMAND_RESPONSE_VERSION_1;
    commandResponse->status = status;
    int result = 0;

    if (responseData != NULL)
    {
        size_t responseLen = strlen(responseData);

        // Allocate a copy of the response data to return to the invoker.
        // takes responsibility for freeing this data.
        if (mallocAndStrcpy_s((char**)&commandResponse->responseData, responseData) != 0)
        {
            LogError("ENVIRONMENTAL_SENSOR_INTERFACE: Unable to allocate response data");
            commandResponse->status = 500;
            result = MU_FAILURE;
        }
        else
        {
            commandResponse->responseDataLen = responseLen;
            result = 0;
        }
    }

    return result;
}

void DigitalTwinClientHelper_SetUpdateAsyncStatus(DIGITALTWIN_CLIENT_ASYNC_COMMAND_UPDATE *asyncCommandUpdate, const char* commandName, const char* requestId, const char* propertyData, int status)
{
    memset(asyncCommandUpdate, 0, sizeof(DIGITALTWIN_CLIENT_ASYNC_COMMAND_UPDATE));
    asyncCommandUpdate->version = DIGITALTWIN_CLIENT_ASYNC_COMMAND_UPDATE_VERSION_1;
    asyncCommandUpdate->commandName = commandName;
    asyncCommandUpdate->requestId = requestId;
    asyncCommandUpdate->propertyData = propertyData;
    asyncCommandUpdate->statusCode = status;
}

void DigitalTwinClientHelper_DeInitialize()
{
    if (digitalTwinDeviceClientHandle != NULL)
    {
        DigitalTwin_DeviceClient_LL_Destroy(digitalTwinDeviceClientHandle);
    }

    if ((digitalTwinDeviceClientHandle == NULL) && (deviceHandle != NULL))
    {
        // Only destroy the deviceHandle directly if we've never created a digitalTwinDeviceClientHandle
        // (digitalTwinDeviceClientHandle de facto takes ownership of deviceHandle once its created).
        IoTHubDeviceClient_LL_Destroy(deviceHandle);
    }

    if (deviceHandle != NULL)
    {
        IoTHub_Deinit();
    }
}

void DigitalTwinClientHelper_Check()
{
    DigitalTwin_DeviceClient_LL_DoWork(digitalTwinDeviceClientHandle);
    ThreadAPI_Sleep(100);
}
