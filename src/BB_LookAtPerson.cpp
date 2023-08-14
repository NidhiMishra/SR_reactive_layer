#pragma once
#include <iostream>
#include "BB_LookAtPerson.h"
#include "AgentControl.h"
#include "ProtectedClient.h"
#include "Inputs_types.h"
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

using namespace ::std;

namespace BB
{
	User::User()
	{
		beingLooked = false;
		distance = 0;
		engaged = 1;
		ges_frame_count = 0;
		ges_wave = false;
	}

	BB_LookAtPerson::BB_LookAtPerson(ProtectedClient<imi::AgentControlClient> *aClient)
	{
		activationLevel = 0;
		threshold = 0.05;
		nadineClient = aClient;
		active = false;
		myBBType = "LookAtPerson";//TO DO: Remove these variable
		myOutput.behaviorType = myBBType;
		executing = false;
		disable = false;

		framesNum = GAZE_DURATION + 1;
		posToLookAt.x = 0;
		posToLookAt.y = 0;
		posToLookAt.z = 0;

		//initialisation for Kinect V1
		posUserOne.x = 0;
		posUserOne.y = 0;
		posUserOne.z = 0;
		posUserTwo.x = 0;
		posUserTwo.y = 0;
		posUserTwo.z = 0;
		lookingAt = 0;
		distance1 = 0;
		distance2 = 0;

		//timer to smile back
		lastSmile = time(NULL);

		users.resize(6);//Maximum number of users with Kinect V2
//		ges_success_id = 0;
	}

	BB_LookAtPerson::~BB_LookAtPerson()
	{
		while (users.size() > 0)//delete users that are gone
		{
			User *temp = users.back();
			users.pop_back();
			delete temp;
		}

	}

	bool BB_LookAtPerson::updateActivationLevel(perceptualInput aInput)
	{
		mtx.lock();
		if (aInput.sensorId == "Skype")
		{
			disable = true;
		}
		if (aInput.sensorId == "Demo")
		{
			disable = false;
		}
		if (aInput.sensorId == "KinectV2")
		{
			//std::cout<<"Start updating users vector \n";
			if (framesNum < GAZE_DURATION)
				framesNum++;

			int previousSize = users.size();
			while (users.size()<aInput.position.size())//Create new users
			{
				users.push_back(new User());
			}

			while (users.size()>aInput.position.size())//delete users that are gone
			{
				User *temp = users.back();
				users.pop_back();
				delete temp;
			}

			for (int i = 0; i < users.size(); i++)
			{
				if (i < previousSize)
				{ //Compute distance moved by each user. Update activationLevel
					//double tempDist = sqrt(pow(users.at(i)->position.x - aInput.position.at(i).head.x, 2) + pow(users.at(i)->position.y - aInput.position.at(i).head.y, 2) + pow(users.at(i)->position.z - aInput.position.at(i).head.z, 2));//TO DO: Update to consider Qantity of Motion from all joints
					//users.at(i)->distance += tempDist;
					double tempDist = computeQom(users.at(i)->skeleton, aInput.position.at(i));
					users.at(i)->distance += tempDist;
					//std::cout << "QOM is: " << tempDist << "\n";
					//activationLevel = activationLevel + 0.00 + tempDist * 10;
					
				}


				users.at(i)->position = aInput.position.at(i).head;
				users.at(i)->skeleton = aInput.position.at(i);

				//Update position fo next gaze
				if (users.at(i)->beingLooked)
				{
					posToLookAt = users.at(i)->position;
					activationLevel = threshold;
				}
			}

			if (framesNum >= GAZE_DURATION && users.size() > 0)//Check if the robot should change its focus
			{
				int maxDistance = 0;
				double tempDistance = 0;
				for (int i = 0; i<users.size(); i++)
				{
					users.at(i)->beingLooked = false;
					if (users.at(i)->distance * ((double)users.at(i)->engaged / GAZE_DURATION)>tempDistance)
					{
						maxDistance = i;
						tempDistance = users.at(i)->distance* ((double)users.at(i)->engaged / GAZE_DURATION);
					}
					users.at(i)->distance = 0;
					users.at(i)->engaged = 1;
				}
				posToLookAt = users.at(maxDistance)->position;
				users.at(maxDistance)->beingLooked = true;
				framesNum = 0;
			}

			/*for (int i = 0; i < users.size(); i++)
			{
				WaveRightSegment(aInput.position.at(i));
				if (ges_success_id == 1)
				{
					users.at(i)->ges_frame_count++;
				}
				else
				{
					users.at(i)->ges_frame_count = 0;
				}
				if (users.at(i)->ges_frame_count == 30)
				{
					users.at(i)->ges_frame_count = 0;

					std::cout << "User" << i << " Hand Gesture" << "\n";
				}
			}*/
		}

		if (aInput.sensorId == "KinectV2_Face")
		{
			if (aInput.faceStatus.size() != users.size())
			{//skip this frame
				mtx.unlock();
				return activationLevel > threshold;
			}

			for (int i = 0; i < users.size(); i++)
			{
				if (aInput.faceStatus.at(i).engaged == imi::FacePossibilities::YES)
				{
					users.at(i)->faceStatus.engaged = imi::FacePossibilities::YES;
					//users.at(i)->distance*1.05;//Give a bonus for each frame the users is looking at the robot
					users.at(i)->engaged++;
				}
				if (aInput.faceStatus.at(i).happy == imi::FacePossibilities::YES)
				{
					users.at(i)->faceStatus.happy = imi::FacePossibilities::YES;
					//std::cout << "BB User " << i << " is smiling \n";
				}
				if (aInput.faceStatus.at(i).mmoved == imi::FacePossibilities::YES)
					users.at(i)->faceStatus.mmoved = imi::FacePossibilities::YES;
			}
		}
	
		
	if(aInput.sensorId=="KinectV2_Gesture")
	{

		/*
		for(int i=0;i<users.size();i++)
		{
			WaveRightSegment(aInput.position.at(i));
			if(ges_success_id == 1)
				{
					users.at(i)->ges_frame_count++;
				}
			else
				{
					users.at(i)->ges_frame_count = 0;
				}
			if(users.at(i)->ges_frame_count == 30 )
			{ 
			users.at(i)->ges_frame_count=0;
						
			std::cout<< "User" << i << " Hand Gesture" <<"\n";
			//nadineClient->getClient()->playAnimation(imi::Animation::WAVE_HAND);
			//sleep(2000);
			}*/
		}

	//KinectV2_Gesture//


		if (aInput.sensorId == "Kinect") //Maintain Old kinect. TO DO: remove this once update is done in demo area 
		{
			if (framesNum < GAZE_DURATION)
				framesNum++;
			if (aInput.userID == 0)
			{
				distance1 = sqrt((aInput.pos.x - posUserOne.x)*(aInput.pos.x - posUserOne.x) + (aInput.pos.y - posUserOne.y)*(aInput.pos.y - posUserOne.y) + (aInput.pos.z - posUserOne.z)*(aInput.pos.z - posUserOne.z));
				activationLevel = activationLevel + 0.00 + distance1 * 10;

				posUserOne = aInput.pos;
			}

			if (aInput.userID == 1)
			{
				distance2 = sqrt((aInput.pos.x - posUserTwo.x)*(aInput.pos.x - posUserTwo.x) + (aInput.pos.y - posUserTwo.y)*(aInput.pos.y - posUserTwo.y) + (aInput.pos.z - posUserTwo.z)*(aInput.pos.z - posUserTwo.z));
				activationLevel = activationLevel + 0.00 + distance2 * 10;
				posUserTwo = aInput.pos;

			}

			if (lookingAt == 1)//currently looking at the other one
			{
				boost::variate_generator<boost::mt19937, boost::normal_distribution<> > generator(boost::mt19937(time(NULL)), boost::normal_distribution<>(1, (distance1 / (distance2 + 0.01) + 0.2)));
				if (abs(generator() > 1.5) && (framesNum > 20))
				{
					lookingAt = 0;
					posToLookAt = posUserOne;
					framesNum = 0;
				}
				else
				{
					posToLookAt = posUserTwo;
				}
			}
			else
			{
				boost::variate_generator<boost::mt19937, boost::normal_distribution<> > generator(boost::mt19937(time(NULL)), boost::normal_distribution<>(1, (distance2 / (distance1 + 0.01) + 0.2)));
				if (abs(generator() > 1.5) && (framesNum > 20))
				{
					lookingAt = 1;
					posToLookAt = posUserTwo;
					framesNum = 0;
				}
				else
				{
					posToLookAt = posUserOne;
				}
			}
		}

		if (aInput.behaviorType == "LookAtSound")
		{
			activationLevel  -= 3;
			framesNum = 0;

		}

		if (aInput.behaviorType == "LookAtSpeechSource")
		{
			if (aInput.userID < users.size())
			{
				for (int i = 0; i < users.size(); i++)
				{
					users.at(i)->beingLooked = false;
				}
				users.at(aInput.userID)->beingLooked = true;
				framesNum = -100;
			}
			activationLevel = 0;
		}


		if(aInput.behaviorType=="BB_Introduce" || aInput.behaviorType=="Answer")
		{
			activationLevel = 0;
		}

		if(aInput.behaviorType=="BB_Idle")
		{
			//std::cout<<"Penalty Given \n";
			activationLevel = -100;
		}

		if (!activationLevel >= threshold)
		{
			active = false;
		}
		active = activationLevel >= threshold;
		mtx.unlock();
		return active;
	}

	perceptualInput BB_LookAtPerson::execute()
	{
		//skype
        perceptualInput temp;
		if (disable)
		{
			activationLevel = 0;
			return temp;
		}

		//std::cout << "STARTING GAZE \n";
		mtx.lock();
		
		active = true;

		gaze();
		if (activationLevel > 0)
			activationLevel = 0;


		//std::cout << "FINISHED GAZE \n";
		mtx.unlock();
		
		return myOutput;
	}
/*int BB_LookAtPerson::WaveRightSegment(imi::Skeleton aSkel)
{
	cout << "I am in WaveRightSegment \n";
	
				if(( aSkel.handr.y > aSkel.elbowr.y) || (aSkel.handl.y > aSkel.elbowl.y))
					{
					if( (aSkel.handr.x > aSkel.elbowr.x) || (aSkel.handr.x <= aSkel.elbowr.x)  || (aSkel.handl.x > aSkel.elbowl.x) || (aSkel.handl.x <= aSkel.elbowl.x))
						{
							
							return ges_success_id = 1;
						}											
					}
				return ges_success_id = 0;
	
}*/


	void BB_LookAtPerson::stopExecution()
	{
		active = false;
	}

	void BB_LookAtPerson::gaze()
	{

#ifdef VH
		posToLookAt.x = posToLookAt.x * 100;
		posToLookAt.y = posToLookAt.y * 100;
		posToLookAt.z = posToLookAt.z * 100;
#endif

		nadineClient->getClient()->lookAtPosition(posToLookAt);

		for (int i = 0; i < users.size(); i++)
		{
			if (users.at(i)->beingLooked == true)
			{
				myOutput.userID = i;
				if ((users.at(i)->faceStatus.happy == imi::FacePossibilities::YES) && (difftime(time(NULL), lastSmile)>120))
				{
					//nadineClient->getClient()->setFaceExpression(imi::Facial_Expression::HAPPINESS, 10);
					std::cout << "I am smiling \n";
					lastSmile = time(NULL);
				}
				break;
			}

		}
	}

	double BB_LookAtPerson::computeQom(imi::Skeleton aSkel1, imi::Skeleton aSkel2)
	{
		double qom = 0;
		aSkel1.elbowl.x -= aSkel1.spines.x;
		aSkel1.elbowl.y -= aSkel1.spines.y;
		aSkel1.elbowl.z -= aSkel1.spines.z;
		aSkel1.wristl.x -= aSkel1.spines.x;
		aSkel1.wristl.y -= aSkel1.spines.y;
		aSkel1.wristl.z -= aSkel1.spines.z;
		aSkel1.shoulderl.x -= aSkel1.spines.x;
		aSkel1.shoulderl.y -= aSkel1.spines.y;
		aSkel1.shoulderl.z -= aSkel1.spines.z;
		aSkel1.handl.x -= aSkel1.spines.x;
		aSkel1.handl.y -= aSkel1.spines.y;
		aSkel1.handl.z -= aSkel1.spines.z;
		aSkel1.elbowr.x -= aSkel1.spines.x;
		aSkel1.elbowr.y -= aSkel1.spines.y;
		aSkel1.elbowr.z -= aSkel1.spines.z;
		aSkel1.wristr.x -= aSkel1.spines.x;
		aSkel1.wristr.y -= aSkel1.spines.y;
		aSkel1.wristr.z -= aSkel1.spines.z;
		aSkel1.shoulderr.x -= aSkel1.spines.x;
		aSkel1.shoulderr.y -= aSkel1.spines.y;
		aSkel1.shoulderr.z -= aSkel1.spines.z;
		aSkel1.handr.x -= aSkel1.spines.x;
		aSkel1.handr.y -= aSkel1.spines.y;
		aSkel1.handr.z -= aSkel1.spines.z;
		aSkel1.head.x -= aSkel1.spines.x;
		aSkel1.head.y -= aSkel1.spines.y;
		aSkel1.head.z -= aSkel1.spines.z;

		aSkel2.elbowl.x -= aSkel2.spines.x;
		aSkel2.elbowl.y -= aSkel2.spines.y;
		aSkel2.elbowl.z -= aSkel2.spines.z;
		aSkel2.wristl.x -= aSkel2.spines.x;
		aSkel2.wristl.y -= aSkel2.spines.y;
		aSkel2.wristl.z -= aSkel2.spines.z;
		aSkel2.shoulderl.x -= aSkel2.spines.x;
		aSkel2.shoulderl.y -= aSkel2.spines.y;
		aSkel2.shoulderl.z -= aSkel2.spines.z;
		aSkel2.handl.x -= aSkel2.spines.x;
		aSkel2.handl.y -= aSkel2.spines.y;
		aSkel2.handl.z -= aSkel2.spines.z;
		aSkel2.elbowr.x -= aSkel2.spines.x;
		aSkel2.elbowr.y -= aSkel2.spines.y;
		aSkel2.elbowr.z -= aSkel2.spines.z;
		aSkel2.wristr.x -= aSkel2.spines.x;
		aSkel2.wristr.y -= aSkel2.spines.y;
		aSkel2.wristr.z -= aSkel2.spines.z;
		aSkel2.shoulderr.x -= aSkel2.spines.x;
		aSkel2.shoulderr.y -= aSkel2.spines.y;
		aSkel2.shoulderr.z -= aSkel2.spines.z;
		aSkel2.handr.x -= aSkel2.spines.x;
		aSkel2.handr.y -= aSkel2.spines.y;
		aSkel2.handr.z -= aSkel2.spines.z;
		aSkel2.head.x -= aSkel2.spines.x;
		aSkel2.head.y -= aSkel2.spines.y;
		aSkel2.head.z -= aSkel2.spines.z;

		qom += computeDistance(aSkel1.elbowl, aSkel2.elbowl);
		qom += computeDistance(aSkel1.elbowr, aSkel2.elbowr);
		qom += computeDistance(aSkel1.wristl, aSkel2.wristl);
		qom += computeDistance(aSkel1.wristr, aSkel2.wristr);
		qom += computeDistance(aSkel1.handl, aSkel2.handl);
		qom += computeDistance(aSkel1.handr, aSkel2.handr);
		qom += computeDistance(aSkel1.shoulderl, aSkel2.shoulderl);
		qom += computeDistance(aSkel1.shoulderr, aSkel2.shoulderr);

		qom += computeDistance(aSkel1.head, aSkel2.head);

		return qom;
	}

	double BB_LookAtPerson::computeDistance(imi::Vec3 aVec3_1, imi::Vec3 aVec3_2)
	{

		return sqrt(pow(aVec3_1.x - aVec3_2.x, 2) + pow(aVec3_1.y - aVec3_2.y, 2) + pow(aVec3_1.z - aVec3_2.z, 2));
		;
	}
}