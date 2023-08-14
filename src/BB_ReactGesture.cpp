#pragma once
#include <iostream>
#include "BB_ReactGesture.h"
#include "AgentControl.h"
#include "ProtectedClient.h"
#include "Inputs_types.h"
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

using namespace ::std;

namespace BB
{
	BB_ReactGesture::BB_ReactGesture(ProtectedClient<imi::AgentControlClient> *aClient)
	{
		activationLevel = 0;
		threshold = 1;
		nadineClient = aClient;
		active = false;
		myBBType = "ReactGesture";//TO DO: Remove these variable
		myOutput.behaviorType = myBBType;
		executing = false;
		lastDetected = GESTURE::NONE;

		lastHello = time(NULL);
		shakeHand = time(NULL);
		srand(NULL);
		disable = false;
	}

	BB_ReactGesture::~BB_ReactGesture()
	{

	}

	bool BB_ReactGesture::updateActivationLevel(perceptualInput aInput)
	{
		mtx.lock();
		//std::cout << "LOCKED \n";

		if (aInput.sensorId == "KinectV2_Gesture")
		{
			//std::cout << "Starting LOOP \n";
			for (int i = 0; i < aInput.gesture.size(); i++)
			{
				if ((aInput.gesture.at(i).Right_wavehand == imi::GesPossibilities::YES) || (aInput.gesture.at(i).Left_wavehand == imi::GesPossibilities::YES))
				{
					if (difftime(time(NULL), lastHello)>5)
					{
						activationLevel += 1;
						lastDetected = GESTURE::WAVE;
					}

				}
			}
		}
		if (aInput.sensorId == "KinectV2_Gesture_Shake")
		{
			for (int i = 0; i < aInput.shake.size(); i++)
			{
				if (aInput.shake.at(i).Right_HandShake == imi::GesPossibilities::YES)
				{
					if (difftime(time(NULL), shakeHand)>10)
					{
						activationLevel += 1;
						lastDetected = GESTURE::SHAKE;
					}
				}
			}
		}

		active = activationLevel >= threshold;
		mtx.unlock();
		//std::cout << "UNLOCKED \n";
		return active;
	}

	perceptualInput BB_ReactGesture::execute()
	{
		mtx.lock();
		
		activationLevel = 0;

		if (lastDetected == GESTURE::WAVE)
		{
			lastHello = time(NULL);
			int i = rand() % 3;
			string temp[3];
			temp[0] = "1";
			temp[1] = "2";
			temp[2] = "3";
			nadineClient->getClient()->touchTarget("Laptop_Waving_03_x" + temp[i] + "(Lookup)");
		}
		else
		{
			if (lastDetected == GESTURE::SHAKE)
			{
				shakeHand = time(NULL);
				//nadineClient->getClient()->touchTarget("ShakeHands_FaceLeft");
				sleep(1000);
				//nadineClient->getClient()->speak("Nice to see you", 10);
				//sleep(1000);
			}
		}
		lastDetected = GESTURE::NONE;
		mtx.unlock();
		//std::cout << "UNLOCKED_EXE \n";
		return myOutput;
	}



	

	}