#include "BB_Bored.h"
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

	BB_Bored::BB_Bored(ProtectedClient<imi::AgentControlClient> *aClient)
	{
		threshold = 1;//0.5
		activationLevel = 0;
		myBBType = "Bored";
		nadineClient = aClient;
		previousBehavior = "";
		executing = false;
		lastTimeSpeak = time(NULL);
	}


	BB_Bored::~BB_Bored()
	{
	}



	bool BB_Bored::updateActivationLevel(perceptualInput aInput)
	{
		if (!mtx.try_lock())
			return activationLevel >= threshold;

		if (aInput.sensorId == "TimePassed")
		{
			//if(activationLevel>=0)
			activationLevel += 0.01;
			//else
			//activationLevel+=1;
		}
		else
		{
			if (aInput.behaviorType == "LookAtSpeechSource" && activationLevel >= -10)
				activationLevel = activationLevel - 0.1;

		}
		if (aInput.behaviorType == "LookAtSound" && activationLevel >= -10)
			activationLevel = activationLevel - 1;


		if (aInput.sensorId == "sound")
		{
			lastTimeSpeak = time(NULL);
		}

		//previousBehavior=aInput.behaviorType; 
		//}
		/*std::cout << "Boredom Level is: "<<activationLevel<<"\n";*/
		mtx.unlock();
		return activationLevel >= threshold;
	}


	perceptualInput BB_Bored::execute()
	{
		mtx.lock();

		if (difftime(time(NULL), lastTimeSpeak) > 0.5)
		{
			int i = rand() % 6;
			switch (i)
			{
			case 0:
				nadineClient->getClient()->speak("This is so boring....", 10);
				break;
			case 1:
				nadineClient->getClient()->speak("l wish I could go for a walk", 10);
				break;
			case 2:
				nadineClient->getClient()->speak("This is not really exciting", 10);
				break;
			case 3:
				nadineClient->getClient()->speak("<spurt audio=\"g0001_037\">cough cough</spurt>", 10);
				break;
			case 4:
				nadineClient->getClient()->speak("<spurt audio=\"g0001_044\">cough cough</spurt>", 10);
				break;
			case 5:
				nadineClient->getClient()->speak("<spurt audio=\"g0001_041\">cough cough</spurt>", 10);
				break;
			case 6:
				nadineClient->getClient()->speak("<spurt audio=\"g0001_040\">cough cough</spurt>", 10);
				break;
			}

			activationLevel = 0;
			//executing=false;
		}
		mtx.unlock();

		perceptualInput temp;
		return temp;
	}
}