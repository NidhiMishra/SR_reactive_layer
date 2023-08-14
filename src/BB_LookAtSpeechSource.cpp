#pragma once

#define COUNTSOUND 2
#include <iostream>
#include "BB_LookAtSpeechSource.h"
#include "Behavior.h"
#include "AgentControl.h"
#include "ProtectedClient.h"
#include "Inputs_types.h"
#include <boost/random/normal_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>
#include <math.h>
//#define DEBUG
namespace BB
{

	LookAtSpeechSource::LookAtSpeechSource(ProtectedClient<imi::AgentControlClient> *aClient)
	{
		myBBType = "LookAtSpeechSource";
		myOutput.behaviorType = myBBType;
		lastSound = imi::AudioLocalization();
		activationLevel = 0;
		threshold = 1.5;
		nadineClient = aClient;
		pos.x = 0;
		pos.y = 2;
		pos.z = 1.8;


		slope = 0;

		rd = 0;
		int lookCounter = 0;
		executing = false;
		talking = false;
		disable = false;

		for (int i = 0; i < NUMBER_OF_SOUNDS; i++)
			previousSoundCount[i] = 0;

	}


	LookAtSpeechSource::~LookAtSpeechSource(void)
	{

	}

	bool LookAtSpeechSource::updateActivationLevel(perceptualInput aInput)
	{
		if (!mtx.try_lock() || talking)
			return activationLevel >= threshold;
		if (aInput.sensorId == "Skype")
		{
			disable = true;
		}
		if (aInput.sensorId == "Demo")
		{
			disable = false;
		}
		if (aInput.sensorId == "KinectV2")
		{
			int previousSize = users.size();
			while (users.size()<aInput.position.size())
			{
				users.push_back(new User());
			}

			while (users.size()>aInput.position.size())
			{
				User *temp = users.back();
				users.pop_back();
				delete temp;
			}

			for (int i = 0; i < users.size(); i++)
			{
				users.at(i)->position = aInput.position.at(i).head;
			}
		}


		if (aInput.sensorId == "SoundKinectV2")
		{
			//Compute the position of the sound in the sound system
			int maxDistance = 0;

			slope = 1 / tan(aInput.source.azimuth);

			if(!talking)
				activationLevel += threshold;
		}

		if (aInput.sensorId == "TimePassed")
		{
			for (int i = 0; i<NUMBER_OF_SOUNDS; i++)
			{
				if (previousSoundCount[i]>0)
					previousSoundCount[i] -= COUNTSOUND / 60;
			}
			//std::cout<<"The number of speech is: "<<previousSoundCount[imi::SoundClass::SPEECH_MULTI]<<"\n";
		}

		if (aInput.sensorId == "feedback")
		{
			if (aInput.behaviorType == "Start")
			{
				talking = true;
				activationLevel = 0;
			}
			if (aInput.behaviorType == "End")
			{
				talking = false;
				activationLevel = 0;
			}
		}

		mtx.unlock();
		return activationLevel >= threshold;
	}

	perceptualInput LookAtSpeechSource::execute()
	{
		//skype
		perceptualInput temp;
		if (disable)
		{
			activationLevel = 0;
			return temp;
		}

		mtx.lock();
		double tempSlope = 0;
		double minDist = 100;
		int index = -1;

		/*#ifdef VH
				pos.x*=100;
				pos.y*=100;
				pos.z*=100;
		#endif*/
		if (!talking)
		{

			//Determine which user to look
			for (int i = 0; i < users.size(); i++)
			{
				if (abs(slope - (users.at(i)->position.y / users.at(i)->position.x)) < minDist)
				{
					index = i;
					minDist = abs(slope - (users.at(i)->position.y / users.at(i)->position.x));
				}
			}

			if (index >= 0)
			{
				pos = users.at(index)->position;

#ifdef VH
				pos.x = pos.x * 100;
				pos.y = pos.y * 100;
				pos.z = pos.z * 100;
#endif
				myOutput.userID = index;
				nadineClient->getClient()->lookAtPosition(pos);
			}
			previousSoundCount[lastSound.soundtype] += COUNTSOUND;
		}
		activationLevel = 0;
		mtx.unlock();

		return myOutput;
	}


};