#pragma once

#define NUMBER_OF_SOUNDS 15

#include "Behavior.h"
#include "AgentControl.h"
#include "ProtectedClient.h"
#include "Inputs_types.h"
#include <boost/random/normal_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>

namespace BB
{

	class BB_ReactToSpeechSource : public Behavior
	{
	public:
		BB_ReactToSpeechSource(ProtectedClient<imi::AgentControlClient> *aClient);
		~BB_ReactToSpeechSource(void);
		//float getActivationLevel();
		bool updateActivationLevel(perceptualInput aInput);
		perceptualInput execute();

		//double randomValues[300];

		double rd;
		//User position for look at
		imi::Vec3 pos;
		imi::Vec3 kinectPos1;
		imi::Vec3 kinectPos2;
		bool talking; //The Agent is talking

		imi::AudioLocalization lastSound;
		double previousSoundCount[NUMBER_OF_SOUNDS];

		bool flag;
		time_t lastTimeSpeak;
		time_t speechDetected;
	};

}