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

#define GAZE_DURATION 100 //Every 100 frames, robot decides which users to look at

namespace BB
{
	class BB_LookAtPerson : public Behavior
	{
	public:
		BB_LookAtPerson(ProtectedClient<imi::AgentControlClient> *aClient);
		~BB_LookAtPerson();
		//float getActivationLevel();
		bool updateActivationLevel(perceptualInput aInput);
		perceptualInput execute();
		void gaze();
		void stopExecution();

	private:

		double computeQom(imi::Skeleton aSkel1, imi::Skeleton aSkel2);// return the quantity of movements from the last two frames
		double computeDistance(imi::Vec3 aVec3_1, imi::Vec3 aVec3_2);


		time_t lastSmile;//Time at which the robot smiled back

		bool active;
		int framesNum;
		int numberUsers;

		//for old kinect
		double distance1;
		double distance2;
		imi::Vec3 posUserOne;
		imi::Vec3 posUserTwo;
		int lookingAt;

		//int WaveRightSegment(imi::Skeleton aSkel);
		//int ges_success_id;

	};
}