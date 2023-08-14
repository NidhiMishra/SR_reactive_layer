#pragma once
#include "Behavior.h"
#include "AgentControl.h"
#include "ProtectedClient.h"
#include "Inputs_types.h"
#include <boost/thread.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>
#include <vector>

namespace BB
{


		enum GESTURE
		{
			NONE=0,
			WAVE=1,
			SHAKE=2
		};


	class BB_ReactGesture : public Behavior
	{
	public:
		BB_ReactGesture(ProtectedClient<imi::AgentControlClient> *aClient);
		~BB_ReactGesture();
		//float getActivationLevel();
		bool updateActivationLevel(perceptualInput aInput);
		perceptualInput execute();
		
	private:

		bool active;
		time_t lastHello;//Time at which the robot smiled back
		time_t shakeHand;
		GESTURE lastDetected;


	};
}