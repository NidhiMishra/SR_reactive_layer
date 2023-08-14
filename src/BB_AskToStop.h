#pragma once
#include "Behavior.h"
#include "AgentControl.h"
#include "ProtectedClient.h"
#include "Inputs_types.h"
#include <string>

namespace BB
{

	class BB_AskToStop : public Behavior
	{
	public:
		BB_AskToStop(ProtectedClient<imi::AgentControlClient> *aClient);
		~BB_AskToStop(void);

		bool updateActivationLevel(perceptualInput aInput);
		perceptualInput execute();

		imi::Vec3 pos;
		std::string previousBehavior;
	};

	class BB_ReactGestures : public Behavior
	{
		BB_ReactGestures(ProtectedClient<imi::AgentControlClient> *aClient);
		~BB_ReactGestures(void);
		bool updateActivationLevel(perceptualInput aInput);
		perceptualInput execute();
	};

}