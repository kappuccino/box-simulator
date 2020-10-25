#include <iostream>
#include <stdio.h>
#include "Serial.h"
#include "SimConnect.h"

bool quit{ false };
HANDLE hSimConnect{ NULL };

const char* port = R"(\\.\COM3)";


/*enum DEFINITION_ID {
	DEF_READ_ALL
};*/

// request id
/*enum REQUEST_ID {
	REQ_ID,
};
*/

enum DATA_DEFINE_ID {
	DEFINITION_1,
};

enum DATA_REQUEST_ID {
	REQUEST_1,
};




struct Struct1
{
	double  vel_x;
	double  vel_y;
	double  vel_z;
	double  acc_x;
	double  acc_y;
	double  acc_z;
	double  orient_x;
	double  orient_y;
	double  orient_z;
	double  rotspd_x;
	double  rotspd_y;
	double  rotspd_z;
};


// events
static enum EVENT_ID {
	EVENT_ALT_INC,
	EVENT_ALT_DEC,
	EVENT_ALT_HOLD,
	EVENT_VS_INC,
	EVENT_VS_DEC,
	EVENT_VS_SELECT,
	EVENT_HEADING_BUG_INC,
	EVENT_HEADING_BUG_DEC,
	EVENT_HEADING_BUG_HOLD,
	EVENT_SPEED_INC,
	EVENT_SPEED_DEC,
	EVENT_SPEED_HOLD,
	EVENT_ZOOM_IN,
	EVENT_ZOOM_OUT,
	SIM_START,
	SIM_STOP,
};


void CALLBACK MyDispatchProc(SIMCONNECT_RECV* pData, DWORD  cbData, void* pContext) {
	HRESULT hr;
	switch (pData->dwID) {
	case SIMCONNECT_RECV_ID_EVENT:
	{
		// event received 
		SIMCONNECT_RECV_EVENT* evt = static_cast<SIMCONNECT_RECV_EVENT*>(pData);
		switch (evt->uEventID)
		{
		case SIM_START: {
			// sim is running
			// Now the sim is running, request information on the user aircraft
			hr = SimConnect_RequestDataOnSimObjectType(hSimConnect, REQUEST_1, DEFINITION_1, 0, SIMCONNECT_SIMOBJECT_TYPE_USER);
			std::cout << "Sim start" << std::endl;
			break;
		}
		case SIM_STOP: {
			// sim stop
			std::cout << "Sim stop" << std::endl;
			break;
		}

		default:
			printf("unknow event id! %1d\n", evt->uEventID);
			break;
		}
	}

	case SIMCONNECT_RECV_ID_SIMOBJECT_DATA_BYTYPE:
	{
		SIMCONNECT_RECV_SIMOBJECT_DATA_BYTYPE* pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA_BYTYPE*)pData;

		switch (pObjData->dwRequestID)
		{

			/*case REQ_ID:
			{
				printf("known request id: %ld\n", pObjData->dwRequestID);
				break;
			}*/

		case REQUEST_1:
		{
			Struct1* pS = (Struct1*)&pObjData->dwData;
			//hr = SimConnect_RequestDataOnSimObjectType(hSimConnect, REQUEST_1, DEFINITION_1, 0, SIMCONNECT_SIMOBJECT_TYPE_USER);

			//printf("known request id: %ld\n", pObjData->dwRequestID);
			printf("HEADING: %lf\n", pS->orient_z * (180.0 / 3.141592653589793238463));
			break;
		}

		default:
			printf("Unknown request id: %ld\n", pObjData->dwRequestID);
			break;
		}
	}

	case SIMCONNECT_RECV_ID_QUIT:
	{
		//std::cout << "SIMCONNECT_RECV_ID_QUIT recieved" << std::endl;
		break;
	}
	}


};

int main() {

	bool simConnected{ false };
	//int retryDelay{ 0 };
	HRESULT result;

	// FS connexion
	std::cout << "Try to connect to MSFS...\n";

	result = SimConnect_Open(&hSimConnect, "fsbridge", NULL, 0, 0, 0);
	if (result != 0) {
		std::cout << "Failed to connect to MSFS" << std::endl;
		return 1;
	}
	std::cout << "Connected to MSFS" << std::endl;

	/*if (result == 0) { // connected
		std::cout << "Connected to MSFS" << std::endl;

		// subscribe
		result = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "VELOCITY BODY Z", "meters per second squared");
		result = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "VELOCITY BODY X", "meters per second squared");
		result = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "VELOCITY BODY Y", "meters per second squared");

		result = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "ACCELERATION BODY Z", "meters per second squared");
		result = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "ACCELERATION BODY X", "meters per second squared");
		result = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "ACCELERATION BODY Y", "meters per second squared");

		result = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "PLANE BANK DEGREES", "radians");
		result = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "PLANE PITCH DEGREES", "radians");
		result = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "PLANE HEADING DEGREES TRUE", "radians");

		result = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "ROTATION VELOCITY BODY Z", "radians per second");
		result = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "ROTATION VELOCITY BODY X", "radians per second");
		result = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "ROTATION VELOCITY BODY Y", "radians per second");

		// Request an event when the simulation starts
		result = SimConnect_SubscribeToSystemEvent(hSimConnect, SIM_START, "SimStart");
		result = SimConnect_RequestDataOnSimObjectType(hSimConnect, REQUEST_1, DEFINITION_1, 0, SIMCONNECT_SIMOBJECT_TYPE_USER);

		// bind dispatcher
		while (true) {
			result = SimConnect_RequestDataOnSimObjectType(hSimConnect, REQUEST_1, DEFINITION_1, 0, SIMCONNECT_SIMOBJECT_TYPE_USER);
			result = SimConnect_CallDispatch(hSimConnect, MyDispatchProc, NULL);
			if (result != 0) {
				{
					std::cout << "Dispatcher detached" << std::endl;
					return 0;
				}
			}
			Sleep(500);
		}
	}
	else {
		std::cout << "Failed to connect to MSFS" << std::endl;
		return 1;
	}*/

	// Vertical speed
	result = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_VS_INC, "AP_VS_VAR_INC");
	result = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_VS_DEC, "AP_VS_VAR_DEC");
	//result = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_VS_SELECT, "VSI_BUG_SELECT");
	//result = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "AUTOPILOT VERTICAL HOLD VAR", "feet/minute");

	// Altitude
	result = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_ALT_INC, "AP_ALT_VAR_INC");
	result = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_ALT_DEC, "AP_ALT_VAR_DEC");
	result = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_ALT_HOLD, "AP_PANEL_ALTITUDE_HOLD");

	// heading
	result = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_HEADING_BUG_INC, "HEADING_BUG_INC");
	result = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_HEADING_BUG_DEC, "HEADING_BUG_DEC");
	result = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_HEADING_BUG_HOLD, "AP_PANEL_HEADING_HOLD");

	// speed
	result = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_SPEED_INC, "AP_SPD_VAR_INC");
	result = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_SPEED_DEC, "AP_SPD_VAR_DEC");
	result = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_SPEED_HOLD, "KEY_AP_PANEL_SPEED_HOLD");

	// zoom map
	result = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_ZOOM_IN, "GPS_ZOOMIN_BUTTON");
	result = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_ZOOM_OUT, "GPS_ZOOMOUT_BUTTON");





	// arduino connexion
	//const int DATA_LENGHT = 1;
	Serial* serialConn;
	std::cout << "Serial connexion...";
	serialConn = new Serial(port);

	if (serialConn->isConnected()) {
		std::cout << "OK" << std::endl;
		std::string cmd;
		while (true) {
			cmd.clear();
			bool hasRead = serialConn->readCommand(&cmd);
			if (hasRead) {
				std::cout << "RX: " << cmd << '"' << std::endl;

				// parse command
				int pos = cmd.find(':') + 1;

				std::string sender = cmd.substr(0, static_cast<int>(pos - 1));
				std::string action = cmd.substr(pos);

				//std::cout << "sender: '"<< sender << "' action: '" << action << "'" << std::endl;

				// exec commands
				// can't use switch with string

				// encoder 1: vertical speed
				if (sender.compare("E1")==0)
				{
					if (action.compare("u") == 0) {
						result = SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_VS_INC, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
					}
					else if (action.compare("d")==0) {
						result = SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_VS_DEC, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);

					}
					/*
					// vs hold
					else
					{
						//SIMCONNECT_DATA
						
						int hold{ 2000 };
						result = SimConnect_SetDataOnSimObject(hSimConnect, DEFINITION_1, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(hold),&hold);

					}*/
				}
				// encoder 2: altitude
				else if (sender.compare("E2")==0) {
					if (action.compare("u") == 0) {
						result = SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_ALT_INC, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
					}
					else if (action.compare("d")==0)
					{
						result = SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_ALT_DEC, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
					}
					else {
						result = SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_ALT_HOLD, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
					}
				}

				// encoder 3: heading
				else if (sender.compare("E3") == 0) {
					if (action.compare("u") == 0) {
						result = SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_HEADING_BUG_INC, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
					}
					else if (action.compare("d") == 0) {
						result = SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_HEADING_BUG_DEC, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);

					}
					else if (action.compare("b") == 0) {
						result = SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_HEADING_BUG_HOLD, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
					}
				}

				// encoder 4: speed
				else if (sender.compare("E4") == 0) {
					if (action.compare("u") == 0) {
						result = SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_SPEED_INC, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
					}
					else if (action.compare("d") == 0) {
						result = SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_SPEED_DEC, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);

					}
					else if (action.compare("b") == 0) {
						result = SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_SPEED_HOLD, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
					}
				}

				// encoder 5: map zoom
				else if (sender.compare("E5")) {
					if (action.compare("u") == 0) {
						result = SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_ZOOM_IN, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
					}
					else if (action.compare("d") == 0) {
						result = SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_ZOOM_OUT, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
					}
				}

				// default
				else {
					std::cout << "Error unknow command - sender: '" << sender << "' action: '" << action << "'" << std::endl;
				}

				std::cout << "result from transmit event: " << result << std::endl;
			}
			else {
				std::cerr << "ERROR reading data\n";
				break;
			}
		}
	}
	return 0;
}
