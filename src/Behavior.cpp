#pragma once
#include <string>
#include <ctime>
#include "Behavior.h"
#include "AgentControl.h"
#include "ProtectedClient.h"
#include "Inputs_types.h"
#include <iostream>

namespace BB
{
	Behavior::Behavior()
	{
	}

	Behavior::Behavior(ProtectedClient<imi::AgentControlClient> *aClient)
	{
		threshold = 100;
		activationLevel = 0;
		nadineClient = aClient;
		myBBType = "";
		disable = false; //skype
		
	}
	Behavior::~Behavior()
	{

	}

	double Behavior::getActivationLevel()
	{
		return activationLevel;
	}

	bool Behavior::updateActivationLevel(perceptualInput aInput)
	{
		return false;
	}

	perceptualInput Behavior::execute()
	{
		perceptualInput temp;
		return temp;
	}
	bool Behavior::isActive()
	{
		return activationLevel >= threshold;
	}
	std::string Behavior::getType()
	{
		return myBBType;
	}

	void Behavior::printActivation()
	{
		std::cout << "Activation Level of " << myBBType << " " << activationLevel << "\n";
	}
	void Behavior::setChatBot(ProtectedClient<imi::SpeechRecognitionServiceClient> *aChatBot)
	{
		chatBot=aChatBot;
	}
	
	void Behavior::gaze(int duration, int index)
	{
		int i = 0;

		while (i < duration)
		{
			if (index < users.size())
			{
				imi::Vec3 temp = users.at(index)->position;
	#ifdef VH
				temp.x = posToLookAt.x * 100;
				temp.y = posToLookAt.y * 100;
				temp.z = posToLookAt.z * 100;
	#endif
				nadineClient->getClient()->lookAtPosition(temp);
			}
			sleep (100);
			i += 100;
		}
	}
}