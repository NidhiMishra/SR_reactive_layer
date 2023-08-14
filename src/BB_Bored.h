#pragma once
#include "Behavior.h"
#include "AgentControl.h"
#include "ProtectedClient.h"
#include "Inputs_types.h"
#include <string>

namespace BB
{

	class BB_Bored : public Behavior
	{
	public:
		BB_Bored(ProtectedClient<imi::AgentControlClient> *aClient);
		~BB_Bored(void);

		bool updateActivationLevel(perceptualInput aInput);
		perceptualInput execute();

		std::string previousBehavior;
		time_t lastTimeSpeak;
	};

}