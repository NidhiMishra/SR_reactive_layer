#pragma once
#include <string>
#include <ctime>
#include "AgentControl.h"
#include "DialogueManagerService.h"
#include "ProtectedClient.h"
#include "Inputs_types.h"
#include <boost/thread/mutex.hpp>
#include "SpeechRecognitionService.h"
#include "Inputs_constants.h"
//#define VH //comment this if you are using the robot
//#define Chat
//#define GERMAN
//#define JAKARTA
#define MEMORY

namespace BB
{
	struct perceptualInput //Data structure used to communicate the inputs to the behaviors
	{
		std::string sensorId;
		std::string userName;
		std::string behaviorType;
		std::string sentence;
		imi::Vec3 pos;
		imi::Vec3 emotion;
		imi::Vec3 mood;
		imi::skel position;
		imi::ges gesture;
		imi::gesHandShake shake;
		imi::AudioLocalization source;
		int32_t userID;
		imi::face faceStatus;
		std::vector< ::imi::ObjectInfo>  detectedObjects;
		std::vector<imi::UserAction> detectedAction;

	};

	struct User
	{
		User();
		imi::Vec3 position;
		double distance;
		bool beingLooked;
		imi::Facefeatures faceStatus;
		int engaged;//count the number of frames a user was engaged
		imi::Skeleton skeleton;
		int32_t ges_frame_count;
		bool ges_wave;
		std::string name;
		imi::UserAction actions;
	};


	class Behavior
	{
	public:

		Behavior(ProtectedClient<imi::AgentControlClient> *aClient);
		Behavior();
		virtual ~Behavior();
		virtual bool updateActivationLevel(perceptualInput aInput);//All the behaviors need to re-implement this function: Update each behavior with input.
		virtual perceptualInput execute();//All the behaviors need to re-implement this function. Function called each time a behavior is selected as winner
		double getActivationLevel();
		std::string getType();
		void printActivation();
		bool isActive();
		void setChatBot(ProtectedClient<imi::SpeechRecognitionServiceClient> *aChatBot);

	protected:
		bool disable;
		bool executing;
		double activationLevel;
		double threshold;
		std::string myBBType;
		ProtectedClient<imi::AgentControlClient> *nadineClient;
		ProtectedClient<imi::SpeechRecognitionServiceClient> *chatBot;
		
		boost::mutex mtx;
		perceptualInput myOutput;
		std::vector<User *> users;//used for new kinect
		imi::Vec3 posToLookAt;
		void gaze(int duration, int index);
	};

}