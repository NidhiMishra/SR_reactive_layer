#include "BB_Skype.h"
#include "Behavior.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include "ThriftTools.hpp"
#include "AgentControl.h"
#include "ProtectedClient.h"
#include "Inputs_types.h"
#include <string>

namespace BB
{

	BB_Skype::BB_Skype(ProtectedClient<imi::AgentControlClient> *aClient)
	{
		threshold = 1;//0.5
		//threshold = 0.5;
		activationLevel = 0;
		myBBType = "BB_Skype";
		nadineClient = aClient;
		previousBehavior = "";
		executing = false;
		lastTimeSpeak = time(NULL);
	}


	BB_Skype::~BB_Skype()
	{
	}



	bool BB_Skype::updateActivationLevel(perceptualInput aInput)
	{
		mtx.lock();


		if (aInput.sensorId == "Skype")
		{
			//threshold = 1;
			activationLevel += 100;
		}


		mtx.unlock();
		return activationLevel >= threshold;
	}


	perceptualInput BB_Skype::execute()
	{
		perceptualInput temp;
		//if (!disable)
		{
		//	activationLevel = 0;
		//	return temp;
		}

		mtx.lock();

		imi::WorldPosition pos;
		pos.x = 0.2;
		pos.y = -0.4;
		pos.z = 2;
		nadineClient->getClient()->lookAtPosition(pos);
		
		//sleep(500);
		activationLevel = 0;
		mtx.unlock();

		//perceptualInput temp;
		return temp;
	}
}