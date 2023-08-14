#pragma once
#include "Behavior.h"
#include "AgentControl.h"
#include "ProtectedClient.h"
#include "Inputs_types.h"
#include <string>
#include <boost/thread.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>


namespace BB
{

	class BB_Introduce : public Behavior
	{
	public:
		BB_Introduce(ProtectedClient<imi::AgentControlClient> *aClient);
		~BB_Introduce(void);

		bool updateActivationLevel(perceptualInput aInput);
		perceptualInput execute();

		std::string previousBehavior;
		time_t lastTimeSpeak;
		std::string lastSentence;
		bool speaking;

		int rm;
		int user_dialogue_choice;


		//std::vector<User *> users;//used for new kinect
		time_t spoken;//Time at which the robot smiled back
		time_t lastIntro;

	private:
	
		void introduce();
		double countExec;
		bool introduced;

		bool scripted;
		int scriptCount;
	
		std::vector<std::string> allReadyDone;
		bool firstIntro;
		int lastLook;




	};

}