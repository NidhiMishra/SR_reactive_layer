#pragma once

#define NUMBER_OF_SOUNDS 15

#include "Behavior.h"
#include "AgentControl.h"
#include "ProtectedClient.h"
#include "Inputs_types.h"
#include <boost/random/normal_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>
#include "BB_LookAtPerson.h"

namespace BB
{
	class LookAtSpeechSource_VH : public Behavior
	{
	public:
		LookAtSpeechSource_VH(ProtectedClient<imi::AgentControlClient> *aClient);
		~LookAtSpeechSource_VH(void);
		//float getActivationLevel();
		bool updateActivationLevel(perceptualInput aInput);
		perceptualInput execute();

		//double randomValues[300];

		double rd;
		//User position for look at
		imi::Vec3 pos;
		double slope;
		imi::Vec3 kinectPos1;
		imi::Vec3 kinectPos2;
		bool talking; //The Agent is talking


		//std::vector<User *> users;

		imi::AudioLocalization lastSound;
		double previousSoundCount[NUMBER_OF_SOUNDS];

	private:
		time_t lastEx;
	};

}