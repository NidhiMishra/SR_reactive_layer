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
#include "I2P_types.h"

namespace BB
{
	class BB_ReactAction : public Behavior
	{
	public:
		BB_ReactAction(ProtectedClient<imi::AgentControlClient> *aClient);
		~BB_ReactAction();
		bool updateActivationLevel(perceptualInput aInput);
		perceptualInput execute();
		void stopExecution();

	private:
		bool active;
		bool skype;	
		serverStarter *s1;
		std::vector<imi::UserAction> userActionToReport;
		std::vector<std::string> userNames;
		void checkDifferentActions(std::vector<imi::UserAction> inputActions);
		bool hasNew;
		void ReportAction();
		std::string getActionName(const ActionType::type action);
	};
}