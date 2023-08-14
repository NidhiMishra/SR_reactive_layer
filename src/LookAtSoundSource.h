#pragma once
#include "Behavior.h"
#include "AgentControl.h"
#include "ProtectedClient.h"
#include "Inputs_types.h"
#include <boost/random/normal_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>

namespace BB
{

	class LookAtSoundSource : public Behavior
	{
	public:
		LookAtSoundSource(ProtectedClient<imi::AgentControlClient> *aClient);
		~LookAtSoundSource(void);
		//float getActivationLevel();
		bool updateActivationLevel(perceptualInput aInput);
		perceptualInput execute();

		//double randomValues[300];

		time_t lastbigSound;
		double rd;
		//User position for look at
		imi::Vec3 pos;
		bool talking;
	};
}