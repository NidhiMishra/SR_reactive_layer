#pragma once
#include "AgentControl.h"
#include "SoundService.h"
#include "Facebook.h"
#include "PictureService.h"
#include "ProtectedClient.h"
#include "Inputs_types.h"
#include <string>
#include <ctime>
#include "Behavior.h"
#include <vector>
#include "SpeechRecognitionService.h"


using namespace ::imi;
using namespace ::BB;

struct perception
{
	enum type
	{
		FACERECOGNIZED=0,
		SMILEDETECTED=1,
		SHAKEDETECTED=2
	};
};



class reactiveLayer
{
public:
	reactiveLayer(ProtectedClient<imi::AgentControlClient> *aNadine);//, ProtectedClient<imi::AgentControlClient> *aNao, ProtectedClient<imi::AgentControlClient> *aSophie, ProtectedClient<imi::FacebookClient> *aFacebook,ProtectedClient<imi::PictureServiceClient> *aPic );
	~reactiveLayer(void);

	//These functions are for the Behavior based decision
	void perceptionReceived();
	void initBehavior();
	void updateActivationLevels(perceptualInput aInput);
	void executeWinner();
	void updateTimeBehaviors();
	void fakeInput();

	void puppet();//Start the puppet show

	//these Functions are for the original demo. They are kept for backup.
	void updateGaze(double x, double y);
	FaceStatus::type faceRecognized(std::string aName);
	FaceStatus::type updateFaceRecognitionStatus(const  ::imi::Microseconds timestamp, FaceStatus::type aStatus);
	void facialExpressionDetected(const  ::imi::Facial_Expression::type exp);
	void shakeDetected(imi::WorldPosition aPostion);
	void abruptSoundDetected(imi::AudioLocalization source);
	void speechDetected(imi::AudioLocalization source);
	void nonImpulsiveSoundDetected(imi::AudioLocalization source);
	void havingQuestionDetected();
	void objectionDetected();
	void praiseDetected();
	void agreementDetected();
	void confidentDetected();
	void oncallDetected();
	void stopDetected();
	void successDetected();
	void readDetected();
	void writeDetected();
	void drinkDetected();
	void skeletonDetected(double x, double y, double z);
	void skeletonDetectedSpecial(double x, double y, double z); //This is a special case for kinect1. 
	void userNew();
	void userLost();
	void swissNet();
	void connectChatBot();
	void sendInfoBot(int state);



	bool ChatBot;
	std::string alreadyGreeted;
	ProtectedClient<imi::AgentControlClient> *nadineClient;
	/*ProtectedClient<imi::AgentControlClient> *naoClient;
	ProtectedClient<imi::AgentControlClient> *sophieClient;

	ProtectedClient<imi::FacebookClient> *facebook;
	ProtectedClient<imi::PictureServiceClient> *pic;*/

	FaceStatus::type currentFaceStatus;

	std::vector<Behavior *>  *behaviors; //This is where all the behaviors are stored
	
	ProtectedClient<imi::SpeechRecognitionServiceClient> *chatBot;

private:
	bool checkTimeInterval(float anInterval1,time_t aTime);
	time_t lastAction;
	time_t lastSpeak;
	time_t lastTrack;
	time_t lastShake;
	time_t lastAbrupt;
	time_t lastEyeMovement;
	imi::Vec3 previousPos;
	imi::Vec3 currentHeadPos;
	bool alone;//Agent is alone in the space
	std::string currentUser;
	bool recogActive;
	bool robot;//Reactive layer is connected to Nadine =true, VH = false
	bool intro;
	int frame;
	std::string previousName;

	bool speakOnceAfterTypeName;
	time_t prevSpeakTime;
	std::set<std::string> recognizedPerson;
	imi::FaceStatus::type previousStatue;
	std::vector<std::string> faceRecognizedFrame;
	bool firstSee;
	std::string lastWinner;


	// helper function for face
	void personRecognized(std::string name, bool firstSee);
};

