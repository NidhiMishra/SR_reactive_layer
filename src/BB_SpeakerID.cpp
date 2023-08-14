#pragma once
#include <iostream>
#include "BB_SpeakerID.h"
#include "AgentControl.h"
#include "ProtectedClient.h"
#include "Inputs_types.h"
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

using namespace ::std;

namespace BB
{
	BB_SpeakerID::BB_SpeakerID(ProtectedClient<imi::AgentControlClient> *aClient)
	{
		activationLevel = 0;
		threshold = 20;
		nadineClient = aClient;
		active = false;
		myBBType = "SpeakerID";//TO DO: Remove these variable
		myOutput.behaviorType = myBBType;
		executing = false;
		toGreet = "";
	}

	BB_SpeakerID::~BB_SpeakerID()
	{

	}

	bool BB_SpeakerID::updateActivationLevel(perceptualInput aInput)
	{
		mtx.lock();
		//std::cout << "LOCKED \n";


		if (aInput.sensorId == "SpeakerIdentification")
		{
			toGreet = aInput.userName;
			activationLevel = 20;
		}


		if (!activationLevel >= threshold)
		{
			active = false;
		}
		active = activationLevel >= threshold;
		mtx.unlock();
		//std::cout << "UNLOCKED \n";
		return active;
	}

	perceptualInput BB_SpeakerID::execute()
	{
		mtx.lock();
		//std::cout << "LOCKED_EXE \n";
		myOutput.userName = toGreet;
#ifdef GERMAN
		nadineClient->getClient()->speak("Ich glaube sie heissen " + temp, 10);
#else
		nadineClient->getClient()->speak("I think that " + toGreet + " is speaking", 10);
#endif
		activationLevel = 0;
		mtx.unlock();
		//std::cout << "UNLOCKED_EXE \n";
		return myOutput;
	}
}