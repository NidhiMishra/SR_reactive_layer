#pragma once
#include <iostream>
#include "LookAtPerson.h"
#include "AgentControl.h"
#include "ProtectedClient.h"
#include "Inputs_types.h"
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

namespace BB
{

	LookAtPerson::LookAtPerson(ProtectedClient<imi::AgentControlClient> *aClient)
	{
		activationLevel = 0;
		threshold = 0.05;
		nadineClient = aClient;
		active = false;
		myBBType = "LookAtPerson";
		executing = false;

		framesNum = 0;

		posUserOne.x = 0;
		posUserOne.y = 0;
		posUserOne.z = 0;

		posUserTwo.x = 0;
		posUserTwo.y = 0;
		posUserTwo.z = 0;

		posToLookAt.x = 0;
		posToLookAt.y = 0;
		posToLookAt.z = 0;

		lookingAt = 0;


		distance1 = 0;
		distance2 = 0;
	}

	LookAtPerson::~LookAtPerson()
	{

	}
	// float LookAtPerson::getActivationLevel()
	// {
	// 	return activationLevel;
	// }
	bool LookAtPerson::updateActivationLevel(perceptualInput aInput)
	{
		mtx.lock();

		if (aInput.sensorId == "Kinect")
		{
			if (framesNum < 100)
				framesNum++;
			if (aInput.userID == 0)
			{
				distance1 = sqrt((aInput.pos.x - posUserOne.x)*(aInput.pos.x - posUserOne.x) + (aInput.pos.y - posUserOne.y)*(aInput.pos.y - posUserOne.y) + (aInput.pos.z - posUserOne.z)*(aInput.pos.z - posUserOne.z));
				activationLevel = activationLevel + 0.00 + distance1 * 10;
				//	std::cout<<"distance user 0 is: "<<distance1<<"\n";
				posUserOne = aInput.pos;
			}

			if (aInput.userID == 1)
			{
				distance2 = sqrt((aInput.pos.x - posUserTwo.x)*(aInput.pos.x - posUserTwo.x) + (aInput.pos.y - posUserTwo.y)*(aInput.pos.y - posUserTwo.y) + (aInput.pos.z - posUserTwo.z)*(aInput.pos.z - posUserTwo.z));
				activationLevel = activationLevel + 0.00 + distance2 * 10;
				posUserTwo = aInput.pos;
				//	std::cout<<"distance user 2 is: "<<distance1<<"\n";
			}

			if (lookingAt == 1)//currently looking at the other one
			{
				boost::variate_generator<boost::mt19937, boost::normal_distribution<> > generator(boost::mt19937(time(NULL)), boost::normal_distribution<>(1, (distance1 / (distance2 + 0.01) + 0.2)));
				if (abs(generator() > 1.5) && (framesNum > 20))
				{
					lookingAt = 0;
					posToLookAt = posUserOne;
					framesNum = 0;

				}
				else
				{
					posToLookAt = posUserTwo;
				}
			}
			else
			{
				boost::variate_generator<boost::mt19937, boost::normal_distribution<> > generator(boost::mt19937(time(NULL)), boost::normal_distribution<>(1, (distance2 / (distance1 + 0.01) + 0.2)));
				if (abs(generator() > 1.5) && (framesNum > 20))
				{
					lookingAt = 1;
					posToLookAt = posUserTwo;
					framesNum = 0;
				}
				else
				{
					posToLookAt = posUserOne;
				}
			}



		}
		if (aInput.behaviorType == "LookAtSound")// && !executing)
		{
			//std::cout<<"inhibiting \n";
			activationLevel -= 3;

		}
		if (aInput.behaviorType == "LookAtSpeechSource")// && !executing)
		{
			activationLevel -= 1;
		}
		/*std::cout<<"Kinect Activation Level is: "<<activationLevel<<"\n";*/
		if (!activationLevel >= threshold)
		{
			active = false;
		}
		active = activationLevel >= threshold;
		mtx.unlock();
		return active;
	}

	perceptualInput LookAtPerson::execute()
	{
		mtx.lock();
		//executing=true;
		active = true;
		//std::cout<<"Looking Kinect \n";
		gaze();
		if (activationLevel > 0)
			activationLevel = 0;
		//executing=false;
		mtx.unlock();

		perceptualInput temp;
		return temp;
	}

	void LookAtPerson::stopExecution()
	{
		active = false;
	}

	void LookAtPerson::gaze()
	{
		nadineClient->getClient()->lookAtPosition(posToLookAt);
		/*std::cout<<"position for look at person is: "<<pos.x<<" "<<pos.y<<" "<<pos.z<<"\n";*/
	}
};