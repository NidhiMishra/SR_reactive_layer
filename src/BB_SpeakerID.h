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
#include "Server_Starter.h"
#include <string>
namespace BB
{
	class BB_SpeakerID : public Behavior
	{
	public:
		BB_SpeakerID(ProtectedClient<imi::AgentControlClient> *aClient);
		~BB_SpeakerID();
		//float getActivationLevel();
		bool updateActivationLevel(perceptualInput aInput);
		perceptualInput execute();
		
		void stopExecution();

	private:
		std::string toGreet;
		bool active;
	};
}