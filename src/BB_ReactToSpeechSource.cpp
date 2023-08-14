#pragma once

#define COUNTSOUND 2
#include <iostream>
#include "BB_ReactToSpeechSource.h"
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
	BB_ReactToSpeechSource::BB_ReactToSpeechSource(ProtectedClient<imi::AgentControlClient> *aClient)
	{
		myBBType = "ReactToSpeechSource";
		myOutput.behaviorType = "ReactToSpeechSource";
		lastSound = imi::AudioLocalization();
		activationLevel = 0;
		threshold = 1.5;
		nadineClient = aClient;
		pos.x = 0;
		pos.y = 2;
		pos.z = 1.8;

		lastTimeSpeak = time(NULL);
		speechDetected = time(NULL);
		flag = false;

		rd = 0;
		int lookCounter = 0;
		executing = false;
		talking = false;

		for (int i = 0; i < NUMBER_OF_SOUNDS; i++)
			previousSoundCount[i] = 0;
	}


	BB_ReactToSpeechSource::~BB_ReactToSpeechSource(void)
	{

	}

	bool BB_ReactToSpeechSource::updateActivationLevel(perceptualInput aInput)
	{
		if (!mtx.try_lock() || talking)
			return activationLevel >= threshold;

		if (aInput.sensorId == "Sound")
		{
			if (!talking)
			{
				pos = aInput.pos;
				bool equalPrevious = lastSound.soundtype == aInput.source.soundtype;
				switch (aInput.source.soundtype)
				{
				case imi::SoundClass::BOXDROP:
					lastSound = aInput.source;
					if (!equalPrevious)
						activationLevel += threshold;
					else
						activationLevel = 0;
					break;
				case imi::SoundClass::SILENCE:
					break;
				case imi::SoundClass::COUGH:
					lastSound = aInput.source;
					if (!equalPrevious)
						activationLevel = 2 * threshold / 3 + 0.01;
					else
						activationLevel += threshold / (3 * (1 + previousSoundCount[aInput.source.soundtype]));
					break;

				case imi::SoundClass::SPEECH_MULTI:
					if (!talking)
					{
						lastSound = aInput.source;
						speechDetected = time(NULL);
						flag = false;
						if (!equalPrevious)
							activationLevel = 2 * threshold / 3 + 0.01;
						else
							activationLevel += threshold / (3 * (1 + previousSoundCount[aInput.source.soundtype]));
					}
					break;

				default:
					lastSound = aInput.source;
					if (!equalPrevious)
						activationLevel = 2 * threshold / 3 + 0.01;
					else
						activationLevel += threshold / 3;
				}
			}
			else
			{
				activationLevel = 0;
			}
		}

		if (aInput.sensorId=="Speech")
		{
			lastTimeSpeak = time(NULL);
			flag = false;
		}

		if (aInput.sensorId == "TimePassed")
		{
			for (int i = 0; i<NUMBER_OF_SOUNDS; i++)
			{
				if (previousSoundCount[i]>0)
					previousSoundCount[i] -= COUNTSOUND / 60;
			}

			if(!(difftime(lastTimeSpeak, speechDetected)<-3) && flag==false)
			{
				previousSoundCount[imi::SoundClass::SPEECH_MULTI] = 0;
				activationLevel = 0;
				flag = true;
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

	perceptualInput BB_ReactToSpeechSource::execute()
	{
		//executing=true;
		/*std::cout<<"Looking at speech source \n";*/
		mtx.lock();

#ifdef VH //This makes the V.H look toward the left. It replaces the sound position to a fixed one for debugging
		pos.x=-100;
		pos.y=100;
		pos.z=100;
#endif
		if (!talking)
		{
			//nadineClient->getClient()->lookAtPosition(pos);
			switch (lastSound.soundtype)
			{
			case imi::SoundClass::SPEECH:
				//nadineClient->getClient()->speak("Please use the microphone", 10);
				break;
			case imi::SoundClass::IMPULSIVE_SOUND:
				nadineClient->getClient()->speak("Something dropped", 10);
				break;
			case imi::SoundClass::NON_IMPULSIVE_SOUND:
				nadineClient->getClient()->speak("phone ringing", 10);
				break;

			case imi::SoundClass::BOXDROP:
				nadineClient->getClient()->speak("Something dropped", 10);
				break;
			case imi::SoundClass::COUGH:
				nadineClient->getClient()->speak("Are you not well", 10);
				break;
			case imi::SoundClass::SPEECH_MULTI:
				nadineClient->getClient()->speak("You are speaking", 10);
				break;
			case imi::SoundClass::KEYS:
				nadineClient->getClient()->speak("Keys jiggling", 10);
				break;
			case imi::SoundClass::NAILS:
				nadineClient->getClient()->speak("You seem bored", 10);
				break;
			case imi::SoundClass::PEN:
				nadineClient->getClient()->speak("Pen tapping", 10);
				break;
			case imi::SoundClass::PHONE:
				nadineClient->getClient()->speak("Phone ringing", 10);
				break;
			case imi::SoundClass::SILENCE:
				break;
			}
			previousSoundCount[lastSound.soundtype] += COUNTSOUND;
		}
		activationLevel = 0;
		mtx.unlock();
		return myOutput;
	}
}


