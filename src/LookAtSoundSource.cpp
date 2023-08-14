#pragma once
#include <iostream>
#include "LookAtSoundSource.h"
#include "Behavior.h"
#include "AgentControl.h"
#include "ProtectedClient.h"
#include "Inputs_types.h"
#include <boost/random/normal_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>
#include <math.h>
#define DEBUG

namespace BB
{

	LookAtSoundSource::LookAtSoundSource(ProtectedClient<imi::AgentControlClient> *aClient)
	{
		myBBType = "LookAtSound";
		activationLevel = 0;
		threshold = 3;
		nadineClient = aClient;
		pos.x = 0;
		pos.y = 2.5;
		pos.z = 0;
		rd = 0;
		int lookCounter = 0;
		executing = false;
		talking = false;
		lastbigSound = time(NULL);
		//boost::variate_generator<boost::mt19937, boost::normal_distribution<> > generator(boost::mt19937(time(NULL)), boost::normal_distribution<>(-6,0.2));
		// 	for(int i=0;i<300;i++)
		// 	{
		// 		randomValues[i]=generator();
		// 	}
	}


	LookAtSoundSource::~LookAtSoundSource(void)
	{

	}


	// float LookAtSoundSource::getActivationLevel()
	// {
	// 	return activationLevel; 
	// }

	bool LookAtSoundSource::updateActivationLevel(perceptualInput aInput)
	{
		if (!mtx.try_lock() || talking)
			return activationLevel >= threshold;




		if (aInput.sensorId == "Sound")
		{
			if (aInput.source.soundtype == imi::SoundClass::IMPULSIVE_SOUND)
			{
				if (!talking)
					activationLevel += 3;

				pos = aInput.pos;
				lastbigSound = time(NULL);
			}
		}
		/*if(aInput.sensorId=="Kinect" && aInput.userID==0)
		{
		if(aInput.pos.y>1.2)
		pos.y=aInput.pos.y;
		pos.z=aInput.pos.z;
		}*/
		if (aInput.sensorId == "TimePassed")
		{
			if (rd > 0)
				rd = rd - 0.01;
			if (activationLevel < 0)
			{
				//	mtx.lock();
				activationLevel += 0.5;

			}
		}
		if (aInput.behaviorType == myBBType)
		{
			boost::variate_generator<boost::mt19937, boost::normal_distribution<> > generator(boost::mt19937(time(NULL)), boost::normal_distribution<>(-rd, 0.2));
			activationLevel += -abs(generator());
			if (rd < 200)
				rd += 1;

			std::cout << "Sound Activation Level is: " << activationLevel;
		}
		/*std::cout<<"Sound Activation Level is: "<<activationLevel<<" rd is: "<<rd<<"\n";*/

		if (aInput.sensorId == "feedback")
		{
			if (aInput.behaviorType == "Start")
				talking = true;
			if (aInput.behaviorType == "End")
				talking = false;
		}

		mtx.unlock();
		return activationLevel >= threshold;
	}

	perceptualInput LookAtSoundSource::execute()
	{
		mtx.lock();
		if (difftime(time(NULL), lastbigSound) < 2)
		{
			//executing=true;
			/*std::cout<<"Looking at sound \n";*/
#ifdef VH
			pos.x *= 100;
			pos.y *= 100;
			pos.z *= 100;
#endif
			if (!talking)
				nadineClient->getClient()->lookAtPosition(pos);
			activationLevel = 0;
		}
		mtx.unlock();

		perceptualInput temp;
		return temp;
		//executing=false;
		//Sleep(1000);

	}


}