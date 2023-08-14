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
	class BB_ReactObject : public Behavior
	{
	public:
		BB_ReactObject(ProtectedClient<imi::AgentControlClient> *aClient);
		~BB_ReactObject();
		bool updateActivationLevel(perceptualInput aInput);
		perceptualInput execute();
		void stopExecution();

	private:
		bool active;
		bool skype;	
		serverStarter *s1;
		std::vector< ::imi::ObjectInfo> current_objects;
		void updateObjects(std::vector< ::imi::ObjectInfo>& Object);
		void initializeChecking();
		void updateNew(vector<::imi::ObjectInfo> & Objects);
		void updateDuplication(vector<::imi::ObjectInfo> & Objects);
		void reportNew(vector<::imi::ObjectInfo> & Objects);
		void removeUncheckedObjects(vector<::imi::ObjectInfo> & Objects);
		void copyObj(::imi::ObjectInfo & newObj, ::imi::ObjectInfo & oldObj);
		std::string  permanentBlockedLabels [5];
		std::map<std::string,int> flexibleBlockedLabels;
		//std::vector<std::string> blockedObjectLabels;
		bool checkBlockedLabel(std::string label);
		bool hasNew;
		void reportAll(vector<::imi::ObjectInfo> & Objects);
		bool talkAll;
		void setFlexibleBlockedLabels();
	};
}