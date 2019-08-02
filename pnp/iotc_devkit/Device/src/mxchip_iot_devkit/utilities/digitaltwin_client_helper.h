﻿/************************************************************************************************
 * This code was automatically generated by Digital Twin Code Generator tool 0.3.8.
 * Changes to this file may cause incorrect behavior and will be lost if the code is regenerated.
 *
 * Generated Date: Thursday, August 1, 2019
 ***********************************************************************************************/

/**  @file digitaltwin_client_helper.h
*    @brief  DigitalTwin InterfaceClient helper functions.
*
*    @details  DIGITALTWIN_CLIENT_HELPER is wrapper of DigitalTwin device client and interface client. These helper functions can facilitate the
               usage of DigitalTwin client for receiving commands, reporting properties, updated read/write properties, and sending telemetry.
*/

#ifndef DIGITALTWIN_CLIENT_HELPER_H
#define DIGITALTWIN_CLIENT_HELPER_H

#include "iothub.h"
#include "digitaltwin_device_client_ll.h"
#include "digitaltwin_interface_client.h"
#include "iothub_device_client_ll.h"
#include "iothub_client_options.h"
#include "iothubtransportmqtt.h"
#include "azure_c_shared_utility/xlogging.h"
#include "azure_c_shared_utility/threadapi.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
* @brief State of DigitalTwin registration process.  We cannot proceed with DigitalTwin until we get into the state DIGITALTWIN_REGISTRATION_SUCCEEDED.
*/
typedef enum DIGITALTWIN_REGISTRATION_STATUS_TAG
{
    DIGITALTWIN_REGISTRATION_PENDING,
    DIGITALTWIN_REGISTRATION_SUCCEEDED,
    DIGITALTWIN_REGISTRATION_FAILED
} DIGITALTWIN_REGISTRATION_STATUS;

/**
* @brief    Initializes underlying IoTHub client, creates a device handle
*           with the specified connection string, and sets some options on this handle prior to beginning.
*/
DIGITALTWIN_DEVICE_CLIENT_LL_HANDLE DigitalTwinClientHelper_InitializeDeviceHandle(const char* connectionString, bool traceOn, const char * trustedCert);

/**
* @brief    Invokes DigitalTwin_DeviceClient_RegisterInterfacesAsync, which indicates to Azure IoT which DigitalTwin interfaces this device supports.
*           The DigitalTwin Handle *is not valid* until this operation has completed (as indicated by the callback DigitalTwinClientHelper_RegisterInterfacesAndWait being invoked)
*           This call will block until interfaces are successfully registered, we get a failure from server, or we timeout.
*/
DIGITALTWIN_CLIENT_RESULT DigitalTwinClientHelper_RegisterInterfacesAndWait(DIGITALTWIN_DEVICE_CLIENT_LL_HANDLE digitalTwinDeviceClientHandle, const char* deviceCapabilityModelUri, DIGITALTWIN_INTERFACE_CLIENT_HANDLE* interfaceClientHandles, int numInterfaceClientHandles);

/**
* @brief    A helper function that fills out a DIGITALTWIN_CLIENT_COMMAND_RESPONSE
*/
int DigitalTwinClientHelper_SetCommandResponse(DIGITALTWIN_CLIENT_COMMAND_RESPONSE* digitalTwinClientCommandResponseContext, const char* responseData, int status);

/**
* @brief    A helper function that fills out a DIGITALTWIN_CLIENT_ASYNC_COMMAND_UPDATE for a given command specified with 'commandName' parameter
*/
void DigitalTwinClientHelper_SetUpdateAsyncStatus(DIGITALTWIN_CLIENT_ASYNC_COMMAND_UPDATE *digitalTwinClientAsyncCommandUpdate, const char* commandName, const char* requestId, const char* propertyData, int statusCode);

/**
* @brief    De-initialized DigitalTwin client handle and IoT Hub device handle
*/
void DigitalTwinClientHelper_DeInitialize();

/**
* @brief check pending work from Azure IoT Hub
*/
void DigitalTwinClientHelper_Check();

#ifdef __cplusplus
}
#endif

#endif // DIGITALTWIN_CLIENT_HELPER_H
