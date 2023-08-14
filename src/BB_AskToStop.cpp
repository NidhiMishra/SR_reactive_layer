#include "BB_AskToStop.h"
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
	BB_ReactGestures::BB_ReactGestures(ProtectedClient<imi::AgentControlClient> *aClient)
	{
		nadineClient = aClient;
		activationLevel = 0;
		threshold = 1;
	}

	perceptualInput BB_ReactGestures::execute()
	{
		perceptualInput temp;
		return temp;
	}

	bool BB_ReactGestures::updateActivationLevel(perceptualInput aInput)
	{
		return activationLevel > threshold;
	}

	BB_ReactGestures::~BB_ReactGestures(void)
	{

	}

	BB_AskToStop::BB_AskToStop(ProtectedClient<imi::AgentControlClient> *aClient)
	{
		threshold = 3;
		activationLevel = 0;
		myBBType = "AskToStop";
		nadineClient = aClient;
		previousBehavior = "";
		executing = false;
		pos.x = 0;
		pos.y = 0;
		pos.z = 0;
	}


	BB_AskToStop::~BB_AskToStop()
	{
	}



	bool BB_AskToStop::updateActivationLevel(perceptualInput aInput)
	{
		if (!mtx.try_lock())
			return activationLevel >= threshold;

		if (aInput.sensorId == "TimePassed")
		{
			if (activationLevel >= 0)
				activationLevel -= 0.01;
			//else
			//activationLevel+=1;
		}

		// 	if(aInput.sensorId=="Kinect")
		// 	{
		// 		pos.y=aInput.pos.y;
		// 		pos.z=aInput.pos.z;
		// 	}


		if (aInput.sensorId == "Sound")
		{
			if (aInput.source.soundtype == imi::SoundClass::IMPULSIVE_SOUND)
			{
				activationLevel += 0.5;
				//pos.x=pos.y/tan(aInput.source.azimuth); 
				pos = aInput.pos;

			}
		}





		if (aInput.behaviorType == "LookAtSound")
			activationLevel = activationLevel + 1;

		//previousBehavior=aInput.behaviorType; 
		//}
		/*std::cout << "Boredom Level is: "<<activationLevel<<"\n";*/
		mtx.unlock();
		return activationLevel >= threshold;
	}


	perceptualInput BB_AskToStop::execute()
	{
		mtx.lock();
		int i = rand() % 6;
		nadineClient->getClient()->lookAtPosition(pos);
		switch (i)
		{
		case 0:
			nadineClient->getClient()->speak("This is so boring....", 10);
			break;
		case 1:
			nadineClient->getClient()->speak("Could you please stop doing so much noise", 10);
			break;
		case 2:
			nadineClient->getClient()->playAnimation(imi::Animation::NOD_NO);
			nadineClient->getClient()->speak("It is enough now!", 10);
			break;
		case 3:
			nadineClient->getClient()->playAnimation(imi::Animation::NOD_NO);
			nadineClient->getClient()->speak("This is annoying", 10);
			break;
		case 4:
			nadineClient->getClient()->speak("I am sorry but could you please stop being so noisy?", 10);
			break;
		case 5:
			nadineClient->getClient()->speak("this is starting to be tiring", 10);
			break;
		case 6:
			nadineClient->getClient()->speak("please stop doing so much noise", 10);
			break;
		}

		activationLevel = 0;
		//executing=false;
		mtx.unlock();

		perceptualInput temp;
		return temp;
	}
}