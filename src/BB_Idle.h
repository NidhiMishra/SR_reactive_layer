#pragma once
#include "Behavior.h"
#include "AgentControl.h"
#include "ProtectedClient.h"
#include "Inputs_types.h"
#include <string>

namespace BB
{

	class BB_Idle : public Behavior
	{
	public:
		BB_Idle(ProtectedClient<imi::AgentControlClient> *aClient);
		~BB_Idle(void);

		bool updateActivationLevel(perceptualInput aInput);
		perceptualInput execute();

		int previousSize;

	};

}