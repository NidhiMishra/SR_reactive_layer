#pragma once
#include "Behavior.h"
#include "AgentControl.h"
#include "ProtectedClient.h"
#include "Inputs_types.h"
#include <boost/thread.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>

namespace BB
{

	class LookAtPerson : public Behavior
	{
	public:
		LookAtPerson(ProtectedClient<imi::AgentControlClient> *aClient);
		~LookAtPerson();;
		//float getActivationLevel();
		bool updateActivationLevel(perceptualInput aInput);
		perceptualInput execute();
		void gaze();
		void stopExecution();

		//User position for look at
		imi::Vec3 posUserOne;
		imi::Vec3 posUserTwo;
		
		bool active;
		double distance1;
		double distance2;
		int lookingAt;
		int framesNum;

	};
};
