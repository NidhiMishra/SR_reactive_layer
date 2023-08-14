#pragma once
#include "Behavior.h"
#include "AgentControl.h"
#include "ProtectedClient.h"
#include "Inputs_types.h"
#include <string>

namespace BB
{

	class BB_Skype : public Behavior
	{
	public:
		BB_Skype(ProtectedClient<imi::AgentControlClient> *aClient);
		~BB_Skype(void);

		bool updateActivationLevel(perceptualInput aInput);
		perceptualInput execute();

		std::string previousBehavior;
		time_t lastTimeSpeak;
	};

}