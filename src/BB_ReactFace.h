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

namespace BB
{
	class BB_ReactFace : public Behavior
	{
	public:
		BB_ReactFace(ProtectedClient<imi::AgentControlClient> *aClient);
		~BB_ReactFace();
		//float getActivationLevel();
		bool updateActivationLevel(perceptualInput aInput);
		perceptualInput execute();
		
		void stopExecution();

	private:

		std::string findName(int index);
		bool alreadyGreeted(std::string aName);

		bool active;
		int framesNum;
		int numberUsers;

		//for old kinect
		double distance1;
		double distance2;
		imi::Vec3 posUserOne;
		imi::Vec3 posUserTwo;
		int lookingAt;

		ProtectedClient<imi::DialogueManagerServiceClient> *dialogueManager;

		bool skype;

		int toGreet;
		std::vector<std::string> greeted;

		std::string previousFace[6];
		std::vector<std::string>faces[6];
		//int WaveRightSegment(imi::Skeleton aSkel);
		//int ges_success_id;
		serverStarter *s1;
		void loadNumUser();
		void writeNumUser();
	};
}