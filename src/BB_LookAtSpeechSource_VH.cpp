#pragma once

#define COUNTSOUND 2
#include <iostream>
#include "BB_LookAtSpeechSource_VH.h"
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

	LookAtSpeechSource_VH::LookAtSpeechSource_VH(ProtectedClient<imi::AgentControlClient> *aClient)
	{
		myBBType = "LookAtSpeechSource_VH";
		myOutput.behaviorType = myBBType;
		lastSound = imi::AudioLocalization();
		activationLevel = 0;
		threshold = 1.5;
		nadineClient = aClient;
		pos.x = 0;
		pos.y = 2;
		pos.z = 1.8;

		lastEx = time(NULL);
		slope = 0;

		rd = 0;
		int lookCounter = 0;
		executing = false;
		talking = false;

		for (int i = 0; i < NUMBER_OF_SOUNDS; i++)
			previousSoundCount[i] = 0;

	}


	LookAtSpeechSource_VH::~LookAtSpeechSource_VH(void)
	{

	}

	bool LookAtSpeechSource_VH::updateActivationLevel(perceptualInput aInput)
	{
		if (!mtx.try_lock() || talking)
			return activationLevel >= threshold;

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

			if ((!talking))// && (difftime(time(NULL), lastEx)>0.5))
			{
#ifndef VH
				activationLevel += threshold;
#else
				activationLevel += threshold / 2;
#endif
			}
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

		if (aInput.sensorId == "Feedback")
		{
			if (aInput.behaviorType == "Start")
			{
				talking = true;
				activationLevel = 0;
				std::cout << "Speaking Start \n";
			}
			if (aInput.behaviorType == "End")
			{
				talking = false;
				activationLevel = 0;
				std::cout << "Speaking End \n";
			}
		}

		if (aInput.behaviorType == "BB_Introduce" || aInput.behaviorType == "Answer")
		{
#ifndef VH
			activationLevel -=threshold*100;
#endif
		}


		mtx.unlock();
		return activationLevel >= threshold;
	}

	perceptualInput LookAtSpeechSource_VH::execute()
	{
		mtx.lock();
		double tempSlope = 0;
		double minDist = 100;
		int index = -1;

		if (!talking)
		{

			//Determine which user to look
			for (int i = 0; i < users.size(); i++)
			{
	
				if (abs(slope - (users.at(i)->position.y / users.at(i)->position.x)) < minDist)
				{
					index = i;
					minDist = abs(slope - (users.at(i)->position.y / users.at(i)->position.x));
					//std::cout << slope << " LOOK AT SPEECH Looking at USER:" << i << " DIST IS: " << abs(slope - (users.at(i)->position.y / users.at(i)->position.x)) << "\n";
				}
			}

			if (index >= 0)
			{
				pos = users.at(index)->position;
				myOutput.userID = index;
				//nadineClient->getClient()->lookAtPosition(pos);
				//std::cout << "LOOK AT SPEECH Looking at USER:" << index << " X: " << pos.x << " Y : " << pos.y << " Z : " << pos.z << "\n";
			}
			previousSoundCount[lastSound.soundtype] += COUNTSOUND;
		}
		activationLevel =  -3 * threshold;//To react less often WESTGATE
		mtx.unlock();

		return myOutput;
	}

	 
};

