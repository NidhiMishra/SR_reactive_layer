#pragma once
#include "Behavior.h"
#include "AgentControl.h"
#include "ProtectedClient.h"
#include "Inputs_types.h"
#include <string>

namespace BB
{

	class BB_Answer : public Behavior
	{
	public:
		BB_Answer(ProtectedClient<imi::AgentControlClient> *aClient);
		~BB_Answer(void);

		bool updateActivationLevel(perceptualInput aInput);
		perceptualInput execute();

		std::string previousBehavior;
		time_t lastTimeSpeak;
		std::string lastSentence;
		bool speaking;

		imi::Vec3 emotion;
		imi::Vec3 mood;

		int rm;
		int user_dialogue_choice;
	private:
		int TransferSentence(const std::string& phrase);
	};

}