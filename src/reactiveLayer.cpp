#include "reactiveLayer.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include "ThriftTools.hpp"
#include "SoundService.h"
#include "AgentFeedBackService_server.cpp"
#include <math.h>
#include <iostream>
#include "Behavior.h"
#include "LookAtPerson.h"
#include "LookAtSoundSource.h"
#include "BB_Bored.h"
#include "BB_LookAtSpeechSource.h"
#include "BB_ReactToSpeechSource.h"
#include "BB_AskToStop.h"
#include "BB_Answer.h"
#include "BB_Introduce.h"
#include "BB_Idle.h"
#include "BB_LookAtPerson.h"
#include "BB_ReactFace.h"
#include "BB_ReactGesture.h"
#include <boost/thread.hpp>
#include "BB_SpeakerID.h"
#include "BB_Facebook.h"
#include "BB_Skype.h"
#include "BB_ReactObject.h"

//In this version there is nothing clever. The aim is to group all the perception so that they do not conflict anymore.
#define INTERVAL 4 //This is used to wait 4 seconds in between actions for Xiao's demo

reactiveLayer::reactiveLayer(ProtectedClient<imi::AgentControlClient> *aNadine)//, ProtectedClient<imi::AgentControlClient> *aNao, ProtectedClient<imi::AgentControlClient> *aSophie, ProtectedClient<imi::FacebookClient> *aFacebook,ProtectedClient<imi::PictureServiceClient> *aPic )
{
	nadineClient=aNadine;
	initBehavior();
	//naoClient//=aNao;
	//sophieClient=aSophie;
	//facebook=aFacebook;
	//pic=aPic;
	alreadyGreeted="";
	lastSpeak=time(NULL);
	lastAction=time(NULL);
	lastTrack=time(NULL);
	lastEyeMovement=time(NULL);
	lastAbrupt=0;
	lastShake=0;//time(NULL);
	alone=true;
	currentUser="";
	previousPos.x=0;
	previousPos.y=0;
	previousPos.z=0;

	currentHeadPos.x=3;
	currentHeadPos.y=3;
	currentHeadPos.z=2;

	recogActive=false;
	
	ChatBot=false;

	//////////////////////////////////////////////////
	// variables for face

	currentFaceStatus=FaceStatus::DONOTHING;
	speakOnceAfterTypeName = false;
	prevSpeakTime = (time_t)0;
	previousStatue = imi::FaceStatus::DOREGISTERCOMPLETE;
	firstSee = true;

	robot=true;
	intro=true;
	Sleep(4000);

	frame=0;
	previousName="";

	std::string text="";
	//std::string processed="";
	//std::cout << "\npress 's' to start the puppet show anything else to skip it \n";
	

	if(text=="read")
	{
		nadineClient->getClient()->lookAtPosition(currentHeadPos);
		//nadineClient->getClient()->speak("<spurt audio=\"g0001_006\">cough cough</spurt>",10);
		//nadineClient->getClient()->playAnimation(Animation::ROCK);	
		//Sleep(500);	
		while(1)
		{

		nadineClient->getClient()->lookAtPosition(currentHeadPos);
		nadineClient->getClient()->speak("Hello, I am Nadine",10);
		nadineClient->getClient()->playAnimation(imi::Animation::WAVE_HAND);
		std::getline(std::cin,text);
		}

		
		std::getline(std::cin,text);


		nadineClient->getClient()->speak("Today, I will read a part of a paper from The Visual Computer",10);
		//Sleep(3500);


		std::getline(std::cin,text);


		nadineClient->getClient()->playAnimation(Animation::RELEASE_OBJECT);
		nadineClient->getClient()->playAnimation(Animation::RELEASE_OBJECT);
		nadineClient->getClient()->lookAtPosition(currentHeadPos);
		Sleep(2000);
		//This is volume 9

		nadineClient->getClient()->speak("<spurt audio=\"g0001_015\">cough cough</spurt>This looks interesting, it is a paper on digital entities aware of facebook",10);
		//Sleep(6500);


		std::getline(std::cin,text);

		//nadineClient->getClient()->speak("A virtual human is an effective interface for interacting with users and plays an important role in carrying out certain tasks.",10);
	//	Sleep(6500);	
		nadineClient->getClient()->speak("<spurt audio=\"g0001_037\">cough cough</spurt> This sounds like an excellent idea. Lets continue to read.",10);
		//Sleep(5000);
		//nadineClient->getClient()->speak("As social networking sites are getting more and more popular, we propose a Facebook aware virtual human.",10);
		nadineClient->getClient()->playAnimation(Animation::RELEASE_OBJECT);
		nadineClient->getClient()->playAnimation(Animation::RELEASE_OBJECT);
		nadineClient->getClient()->playAnimation(Animation::RELEASE_OBJECT);
		nadineClient->getClient()->playAnimation(Animation::RELEASE_OBJECT);
		nadineClient->getClient()->playAnimation(Animation::RELEASE_OBJECT);
		nadineClient->getClient()->playAnimation(Animation::RELEASE_OBJECT);
	

	}
}

void reactiveLayer::swissNet()
{	
	std::string text;
	nadineClient->getClient()->playAnimation(imi::Animation::WAVE_HAND);
	nadineClient->getClient()->speak("<voice emotion='happy'> <spurt audio=\"g0001_037\">cough cough</spurt> Nice to see you tonight Nadia!</voice>",10);
	std::getline(std::cin,text);

	nadineClient->getClient()->speak("yes.  I am a humanoid robot. My body and face can move thanks to twenty-seven pneumatic motors",10);
	Sleep(6000);
	nadineClient->getClient()->speak("<voice emotion='happy'>I am learning new things everyday. thanks to researchers  at the Institute for media innovation</voice>",10);
	nadineClient->getClient()->playAnimation(Animation::WHY);
	//Sleep(4000);
	//nadineClient->getClient()->speak(" My capacities are developed at the Institute for media innovation",10);


	std::getline(std::cin,text);

	nadineClient->getClient()->speak("Ok, I will show you what I can do.",10);
	Sleep(2500);


	nadineClient->getClient()->playAnimation(Animation::WHY);
	nadineClient->getClient()->speak("I can speak and display gestures",10);
	Sleep(2500);

	nadineClient->getClient()->speak("<voice emotion='happy'>I can smile</voice>",10);
	//nadineClient->getClient()->playAnimation(Animation::ROCK);
	Sleep(2000);
	nadineClient->getClient()->setFaceExpression(Facial_Expression::HAPPINESS,10);
	Sleep(3000);

	nadineClient->getClient()->speak("<voice emotion='cross'>or be angry </voice>",10);
	nadineClient->getClient()->playAnimation(Animation::NOD_NO);
	Sleep(4000);

	nadineClient->getClient()->speak("Today, I want to show you that I can recognize people",10);
	nadineClient->getClient()->playAnimation(Animation::OFFER);
	Sleep(2500);

}


void reactiveLayer::puppet()
{

	ProtectedClient<imi::AgentControlClient> naoClient("155.69.52.119",10013);//Demo backup computer
	ProtectedClient<imi::AgentControlClient> sophieClient("155.69.52.73",9090);
	//ProtectedClient<imi::AgentControlClient> sophieClient("172.22.160.83",9090);
	ProtectedClient<imi::FacebookClient>facebook("155.69.52.119",14000);
	ProtectedClient<imi::PictureServiceClient>pic("155.69.52.119",13001);

	nadineClient->getClient()->lookAtPosition(currentHeadPos);
	nadineClient->getClient()->speak("<spurt audio=\"g0001_006\">cough cough</spurt>",10);
	//nadineClient->getClient()->playAnimation(Animation::ROCK);	
	Sleep(500);	
	nadineClient->getClient()->lookAtPosition(currentHeadPos);
	nadineClient->getClient()->speak("Hello, I am Nadine",10);
	nadineClient->getClient()->playAnimation(imi::Animation::WAVE_HAND);
	Sleep(4000);

	(&sophieClient)->getClient()->speak("Hi, I am Sophie!",10);
	(&sophieClient)->getClient()->playAnimation(imi::Animation::WAVE_HAND);
	Sleep(2000);

	nadineClient->getClient()->lookAtPosition(currentHeadPos);
	nadineClient->getClient()->speak("and this is my little child nao",10);
	nadineClient->getClient()->playAnimation(Animation::OFFER);
	(&naoClient)->getClient()->playAnimation(imi::Animation::STAND_UP);
	Sleep(4000);
	//naoClient->getClient()->playAnimation(imi::Animation::STAND_UP);
	(&naoClient)->getClient()->speak("yeah, I am nao",10);
	(&naoClient)->getClient()->playAnimation(imi::Animation::WAVE_HAND);
	Sleep(3000);

	nadineClient->getClient()->lookAtPosition(currentHeadPos);
	nadineClient->getClient()->speak("I can speak and display gestures",10);
	nadineClient->getClient()->playAnimation(Animation::WHY);
	Sleep(2500);

	//sophieClient->getClient()->lookAtPosition(currentHeadPos);
	(&sophieClient)->getClient()->speak("me too",10);
	(&sophieClient)->getClient()->playAnimation(Animation::WHY);
	Sleep(1000);

	nadineClient->getClient()->lookAtPosition(currentHeadPos);
	nadineClient->getClient()->speak("<voice emotion='happy'>I can be happy</voice>",10);
	//nadineClient->getClient()->playAnimation(Animation::ROCK);
	Sleep(2000);
	nadineClient->getClient()->setFaceExpression(Facial_Expression::HAPPINESS,10);
	Sleep(3000);
	//nadineClient->getClient()->setFaceExpression(Facial_Expression::HAPPINESS,10);
	//Sleep(3000);
	nadineClient->getClient()->speak("<voice emotion='cross'>or angry </voice>",10);
	nadineClient->getClient()->playAnimation(Animation::NOD_NO);
	Sleep(1500);

	(&sophieClient)->getClient()->speak("hey, I can show better than you",10);
	Sleep(2500);
	(&sophieClient)->getClient()->speak("This is my best day smile",10);
	(&sophieClient)->getClient()->setFaceExpression(imi::Facial_Expression::HAPPINESS,10);
	Sleep(3000);
	(&sophieClient)->getClient()->setFaceExpression(imi::Facial_Expression::NEUTRAL,10);
	(&sophieClient)->getClient()->speak("I am so surprised to see so many visitors here",10);
	(&sophieClient)->getClient()->setFaceExpression(imi::Facial_Expression::SURPRISE,10);
	Sleep(2500);
	nadineClient->getClient()->speak("<spurt audio=\"g0001_050\">cough cough</spurt>",10);
	//nadineClient->getClient()->playAnimation(Animation::ROCK);		

	//nadineClient->getClient()->playAnimation(Animation::NOD_NO);
	//Sleep(1500);
	nadineClient->getClient()->lookAtPosition(currentHeadPos);
	//nadineClient->getClient()->speak("I was made in Japan and programmed in IMI BTC",10);
	//nadineClient->getClient()->playAnimation(Animation::ROCK);
	Sleep(2000);
	nadineClient->getClient()->speak("<spurt audio=\"g0001_037\">cough cough</spurt>",10);
	//nadineClient->getClient()->playAnimation(Animation::ROCK);	
	Sleep(500);	
	nadineClient->getClient()->speak("Look at what Nao can do",10);
	nadineClient->getClient()->playAnimation(Animation::OFFER);
	Sleep(5000);
	(&naoClient)->getClient()->speak("watch me I am coming now",10);
	//naoClient->getClient()->playAnimation(imi::Animation::STAND_UP);
	currentHeadPos.x=1.8;
	currentHeadPos.y=0;
	currentHeadPos.z=0;
	(&naoClient)->getClient()->moveToPosition(currentHeadPos);
	Sleep(8000);
	nadineClient->getClient()->speak("That is far enough nao",10);
	nadineClient->getClient()->playAnimation(Animation::OFFER);
	Sleep(1500);
	(&naoClient)->getClient()->speak("ok",10);
	(&naoClient)->getClient()->endMove();

	nadineClient->getClient()->speak("Can you take a picture and put it on your facebook?",10);
	nadineClient->getClient()->playAnimation(Animation::OFFER);
	Sleep(4000);
	(&naoClient)->getClient()->speak("yes mum",10);
	(&naoClient)->getClient()->playAnimation(imi::Animation::RELEASE_OBJECT);
	(&naoClient)->getClient()->speak("I am trying to find a good view point for the picture",10);
	Sleep(10000);
	(&naoClient)->getClient()->speak("Look at me and say cheers in 3",10);
	Sleep(100);
	(&naoClient)->getClient()->speak("2",10);
	Sleep(100);
	(&naoClient)->getClient()->speak("1",10);
	Sleep(100);
	(&naoClient)->getClient()->speak("Cheers!",10);
	std::string aPicture;
	(&pic)->getClient()->takePicture(aPicture);
	(&facebook)->getClient()->postMessageAndPicture("IMI/BTC Visitors",aPicture);
	(&naoClient)->getClient()->speak("Ok, I think it looks good. Yo can check my facebook. See ya!",10);
	Sleep(8000);
	(&sophieClient)->getClient()->speak("It is enough Nao! Sit down!",10);
	Sleep(2500);
	(&naoClient)->getClient()->playAnimation(imi::Animation::SIT_DOWN);
}


void reactiveLayer::initBehavior()
{
	behaviors=new std::vector<Behavior *>();//This is where all the behaviors are stored
	// this->behaviors->push_back(new LookAtPerson(nadineClient));
	//boost::thread gaze(&LookAtPerson::gaze,(LookAtPerson *)behaviors->at(0));
	//this->behaviors->push_back(new LookAtSoundSource(nadineClient));
	//this->behaviors->push_back(new BB_Bored(nadineClient));
	
	//this->behaviors->push_back(new BB_AskToStop(nadineClient));
	this->behaviors->push_back(new BB_Answer(nadineClient));
	this->behaviors->push_back(new BB_Introduce(nadineClient));
	this->behaviors->push_back(new BB_Idle(nadineClient));
	this->behaviors->push_back(new BB_LookAtPerson(nadineClient));
	this->behaviors->push_back(new BB_ReactToSpeechSource(nadineClient));
	this->behaviors->push_back(new BB_ReactFace(nadineClient));
	this->behaviors->push_back(new LookAtSpeechSource(nadineClient));
	this->behaviors->push_back(new BB_ReactGesture(nadineClient));
	//this->behaviors->push_back(new BB_SpeakerID(nadineClient));
	this->behaviors->push_back(new BB_Facebook(nadineClient));
	this->behaviors->push_back(new BB_Skype(nadineClient));
	//this->behaviors->push_back(new BB_ReactObject(nadineClient));
}

void reactiveLayer::connectChatBot()
{
	chatBot=new ProtectedClient<imi::SpeechRecognitionServiceClient>("localhost",imi::g_Inputs_constants.DEFAULT_CHATBOT_PORT); //12010
	ChatBot=true;
	for(std::vector<Behavior *>::iterator it=behaviors->begin();it!=behaviors->end();++it)
		(*it)->setChatBot(chatBot);
}

void reactiveLayer::sendInfoBot(int state)
{
	if(ChatBot)
		chatBot->getClient()->sentenceRecognized("ReactiveLayer",0,"A",0);
}


void reactiveLayer::updateActivationLevels(perceptualInput aInput)
{
	try{
	for(std::vector<Behavior *>::iterator it=behaviors->begin();it!=behaviors->end();++it)//Loop on all the elements of the behaviors Vector (cf initBehavior())
		(*it)->updateActivationLevel(aInput);
	}catch (...) {
		printf("Unexpected Error has happened\n");
	}
}

void reactiveLayer::executeWinner()
{	
	try{
		double activationWinner=0;
		Behavior *winner;
		perceptualInput behaviorInput;
		behaviorInput.behaviorType="none";
		behaviorInput.sensorId="";
		behaviorInput.userName="";
		while(1)
		{
			winner=NULL;
			activationWinner=0;
			for(std::vector<Behavior *>::iterator it=behaviors->begin();it!=behaviors->end();++it)//Loop on all the behaviors it is "iterator"
			{
				if((*it)->isActive()&&((*it)->getActivationLevel()>activationWinner))//First check if the behavior is active (go to isActive()), if yes, compare its activationLevel with the others.
				{
					winner=*it;
					activationWinner=(*it)->getActivationLevel();//To compare with the next active behaviors
				}
			}

			if(winner!=NULL)
			{
			
				if (lastWinner!=winner->getType()){
					std::cout<<"[Current Action]: "<<winner->getType()<<" with Activation: "<<winner->getActivationLevel()<<std::endl;
					lastWinner=winner->getType();
				}
				//std::cout<<"Action: "<<winner->getType()<<" with Activation: "<<winner->getActivationLevel()<<std::endl;
				updateActivationLevels(winner->execute());//execute
				//lastWinner=new Behavior(*winner);
				//lastWinner = malloc(sizeof(Behavior));
				//memcpy(lastWinner, winner, sizeof(Behavior));

				//behaviorInput.behaviorType=winner->getType();//Each behavior has a type. It is used in the next line
				//updateActivationLevels(behaviorInput);//At the moment, this is used to inhibit some conflicting behaviors (give penalty to their activation levels).
				//Sleep(30);
			}

		}
	}
	catch (...) {
		printf("Unexpected Error has happened\n");
	}
}

void reactiveLayer::updateTimeBehaviors()
{
	perceptualInput TimedInput;
	TimedInput.behaviorType="time";
	TimedInput.sensorId="TimePassed";
	TimedInput.userName="";
	int printTimer=0;
	while(1)
	{
		
		Sleep(1000);
		updateActivationLevels(TimedInput);
#ifndef GERMAN
		if(printTimer==5)
		{	
			
			for(std::vector<Behavior *>::iterator it=behaviors->begin();it!=behaviors->end();++it)
				(*it)->printActivation();
			printTimer=0;
			std::cout<<"\n";
		}
		printTimer++;
#endif
	}
}


void reactiveLayer::fakeInput()
{
	while(1)
	{
		std::string text;
		//std::string processed="";
		perceptualInput inputFake;
		inputFake.source.azimuth=0;
		inputFake.sensorId="Sound";
		inputFake.behaviorType="None";

		//std::cout << frame <<"\n";
		std::getline(std::cin,text);

		if(text=="I")
		{
			std::cout<<"Starting Introduction blabla \n";
			inputFake.source.azimuth=0;
			inputFake.sensorId="Introduce";
			inputFake.behaviorType="None";
			updateActivationLevels(inputFake);
		}
		if (text == "skype")
		{
			while (1)
			{
				//std::cout << "Starting skype \n";
				inputFake.source.azimuth = 0;
				inputFake.sensorId = "Skype";
				inputFake.behaviorType = "None";
				updateActivationLevels(inputFake);
				sleep(30);
				if( GetAsyncKeyState( VK_ESCAPE ) )
				{
					//text = "";
					break;
				}
			
			}
		}

		if (text == "demo")
		{
			//while (1)
			{
				//std::cout << "Starting skype \n";
				inputFake.source.azimuth = 0;
				inputFake.sensorId = "Demo";
				inputFake.behaviorType = "None";
				updateActivationLevels(inputFake);
				sleep(30);

				if( GetAsyncKeyState( VK_ESCAPE ) )
				{
					//text = "";
					break;
				}

			}
		}

	}
}

void reactiveLayer::personRecognized(std::string name, bool firstSee)
{
	bool newPerson = false;
	if (this->currentUser != name)
	{
		newPerson = true;
		this->currentUser = name;
	}
	if (this->previousStatue == FaceStatus::DOREGISTERCOMPLETE && this->currentUser == "unknown")
	{
		newPerson = true;
	}

	if (name != "unknown" && newPerson)
	{
		if (name == "Bertil")
		{
			name = "Professor Anderson";
		}
		if (firstSee)
		{
			if (name == "Nadia")
			{
				this->swissNet();
			}
			else
			{
				nadineClient->getClient()->speak("Hello, " + name + ". First time seeing you today.",10);
				Sleep(3000);
			}
			
		}
		else
		{
			nadineClient->getClient()->speak("Hello, " + name + ". Seeing you again! Welcome back.",10);
			Sleep(3000);
		}

	} 
	else
	if(name == "unknown" && newPerson)
	{
		nadineClient->getClient()->speak("Sorry I do not know you.",10);
		Sleep(2500);
		nadineClient->getClient()->speak("Please introduce yourself to me by going to the laptop and enter your name.",10);
		Sleep(5000);
	}
}


FaceStatus::type reactiveLayer::faceRecognized(std::string aName)
{
	int KFrame = 0;
	
	this->faceRecognizedFrame.push_back(aName);

	if (this->recognizedPerson.size() == 0)
		KFrame = 5;
	else
		KFrame = 5;

	if (this->faceRecognizedFrame.size() > 6*KFrame)
		this->faceRecognizedFrame.erase(this->faceRecognizedFrame.begin());
	else
	if (this->faceRecognizedFrame.size() < KFrame)
		return FaceStatus::DONOTHING;

	if (aName.empty())
	{
		if(this->previousStatue == FaceStatus::DOREGISTERCOMPLETE)
		{
			bool moreThanKUnknowFrame = true;
			for ( int i = this->faceRecognizedFrame.size()-KFrame ; i <  this->faceRecognizedFrame.size() ; i ++ )
				if (!this->faceRecognizedFrame[i].empty())
					moreThanKUnknowFrame = false;
			if (moreThanKUnknowFrame)
			{
				this->firstSee = false;
				this->personRecognized("unknown", this->firstSee);
				return FaceStatus::DOTYPENAME;
			}
		}
	}
	else
	{
		if ( this->recognizedPerson.find(aName) != this->recognizedPerson.end() )
		{
			this->firstSee = false;
			this->personRecognized(aName, this->firstSee);
		}
		else
		{
			this->recognizedPerson.insert(aName);
			this->firstSee = true;
			this->personRecognized(aName, this->firstSee);
		}
		return FaceStatus::DONOTHING;
	}

	//if((previousName==aName)&&(frame<100))
	//	frame++;
	//else
	//{
	//	previousName=aName;
	//	frame=0;
	//}

	////std::cout<<"Name is: "<<aName <<"\n";


	//if((frame>6)&&(checkTimeInterval(INTERVAL,lastSpeak)))
	//{
	//	if(aName!="")
	//	{
	//		if(alreadyGreeted.find(aName)== std::string::npos)
	//		{
	//			nadineClient->getClient()->speak("Hello "+aName+", first time seeing you today",10);
	//			nadineClient->getClient()->playAnimation(imi::Animation::WHY);
	//			alreadyGreeted+=aName;
	//			lastSpeak=time(NULL);
	//		}
	//		else
	//		{
	//			if(currentUser!=aName)
	//			{
	//				nadineClient->getClient()->speak("Hello, "+ aName + ". Seeing you again! Welcome back.",10);
	//				lastSpeak=time(NULL);
	//			}

	//		}
	//		currentUser=aName;
	//		return FaceStatus::DONOTHING;
	//	}
	//	else
	//	{
	//			nadineClient->getClient()->speak("Sorry I do not know you.", 10);
	//			Sleep(2500);
	//			nadineClient->getClient()->speak("Please introduce yourself to me by going to the laptop and enter your name.",10);
	//			Sleep(5000);
	//			currentFaceStatus=FaceStatus::DOTYPENAME;
	//			return FaceStatus::DOTYPENAME;
	//	}
	//}
		
}

FaceStatus::type reactiveLayer::updateFaceRecognitionStatus(const  ::imi::Microseconds timestamp, FaceStatus::type aStatus)
{
	if ( aStatus == FaceStatus::DOTYPENAMECOMPLETE )
	{
		if (!this->speakOnceAfterTypeName)
		{
			nadineClient->getClient()->speak("Please come back and stand inside the square and face the web cam.",10);
			Sleep(6000);
			this->speakOnceAfterTypeName = true;
			this->prevSpeakTime = timestamp;
			return FaceStatus::DOTYPENAMECOMPLETE;
		}
		else
		{
			nadineClient->getClient()->speak("We now start the registration. Please keep the current position for thirty seconds.",10);
			Sleep(7000);
			this->prevSpeakTime = timestamp;
			this->previousStatue = aStatus;
			this->speakOnceAfterTypeName = false;
			return FaceStatus::DOREGISTER;
		}
	}
	else
	if ( aStatus == FaceStatus::DOREGISTERCOMPLETE )
	{
		nadineClient->getClient()->speak("Registration complete.",10);
		Sleep(3000);
		this->previousStatue = aStatus;
		this->faceRecognizedFrame.clear();
	}
		
}



void reactiveLayer::facialExpressionDetected(const  ::imi::Facial_Expression::type exp)
{
	if (checkTimeInterval(INTERVAL/2,lastAction))
	{
		switch (exp)
		{
		case 4:
			if(checkTimeInterval(INTERVAL,lastSpeak))
			{
				nadineClient->getClient()->setFaceExpression(imi::Facial_Expression::HAPPINESS,10);
			}
			break;
		default:
			nadineClient->getClient()->setFaceExpression(imi::Facial_Expression::NEUTRAL,10);
		}
		lastAction=time(NULL);
		//lastSpeak=time(NULL);
	}
}

void reactiveLayer::shakeDetected(imi::WorldPosition aPosition)
{
	imi::WorldPosition a;//=new imi::WorldPosition();
	if(checkTimeInterval(15,lastShake))
	{
		lastAction=time(NULL);
		lastSpeak=time(NULL);
		
		if(aPosition.z<1.4)
		{
			recogActive=!recogActive;
			lastShake=time(NULL);
			if(recogActive)
			{
				if(intro)
				{
					nadineClient->getClient()->lookAtPosition(currentHeadPos);
					nadineClient->getClient()->speak("You want to shake hand with me. This is nice",10);
					nadineClient->getClient()->touchPosition(aPosition);
					//nadineClient->getClient()->playAnimation(Animation::ROCK);
				}
			}
			else
			{
				nadineClient->getClient()->lookAtPosition(currentHeadPos);
				nadineClient->getClient()->speak("<spurt audio=\"g0001_029\">cough cough</spurt> Ok, bye bye then",10);
				nadineClient->getClient()->touchPosition(aPosition);
				//nadineClient->getClient()->playAnimation(Animation::ROCK);
			}
		}
		else
		{
			nadineClient->getClient()->speak("You are too far Please come closer!",10);
			//nadineClient->getClient()->playAnimation(Animation::ROCK);
			lastShake=time(NULL)-10;
		}

	}
	else
	{
		std::cout<<"Shake received: Z="<<aPosition.z<<"\n";
	}
}

void reactiveLayer::abruptSoundDetected( AudioLocalization source)
{
	//nadineClient->getClient()->setFaceExpression(imi::Facial_Expression::SURPRISE,10);

	double angle=source.azimuth;
	imi::Vec3 pos;
	int scale=1;
	int direction=1;
	//if(source.azimuth>0)
		//direction=0;
	//pos.y=2;
	//pos.z=0;//.*cos(angle);
	//pos.x=pos.y/cos(angle);  //scale*2.*sin(angle);
	pos.y=2;//this is an assumption it should be read from kinerct
	pos.z=0;//
	pos.x=pos.y/tan(angle);  //scale*2.*sin(angle);

		if(checkTimeInterval(1,lastSpeak))
		{
			//nadineClient->getClient()->lookAtPosition(pos);
			//nadineClient->getClient()->playAnimation(Animation::WAVE_HAND);
			if(direction)//&&pos.x<=10)
			{
					//if(pos.x>20)
						//pos.x=0;pos.y=0.2;pos.z=3;
				nadineClient->getClient()->speak("something dropped",10);
				lastSpeak=time(NULL);
				lastTrack=time(NULL);
				Sleep(2500);
				lastSpeak=time(NULL);
				lastTrack=time(NULL);
				nadineClient->getClient()->lookAtPosition(pos);
				nadineClient->getClient()->speak("something dropped, I think it is here",10);
				//if(pos.x>=0&&pos.x<=3)
					
				//nadineClient->getClient()->playAnimation(Animation::ROCK);
				lastSpeak=time(NULL);
				lastTrack=time(NULL);
				//std::cout<<"Abrupt Sound Position received "<<pos.x<<" "<<pos.y<<" "<<pos.z<< "\n";
			}
			else//&&pos.x<=10)
			{
				//if(pos.x<10 && pos.x>0)
				//{
				nadineClient->getClient()->speak("something dropped",10);
				if(pos.x>=0&&pos.x<=3)
					nadineClient->getClient()->lookAtPosition(pos);
				//nadineClient->getClient()->playAnimation(Animation::ROCK);
				lastSpeak=time(NULL);
				lastTrack=time(NULL);
				std::cout<<"Abrupt Sound Position received "<<pos.x<<" "<<pos.y<<" "<<pos.z<< "\n";
			}//}
			lastAbrupt=time(NULL);
			
		}
		else
		{
			if(checkTimeInterval(1,lastSpeak))//Avoid her own voice being picked up
			{
				if(pos.x>=0&&pos.x<=3)
				{
					nadineClient->getClient()->lookAtPosition(pos);
					//nadineClient->getClient()->playAnimation(Animation::ROCK);
				}
			}
		}
	}

	
/*
void reactiveLayer::nonImpulsiveSoundDetected(AudioLocalization source)
{
	double angle=source.azimuth;
	imi::Vec3 pos;
	int scale=1;
	int direction=1;
	if(source.azimuth>0)
		direction=0;
	printf("Sound is sending voice with angle= %f \n",source.azimuth);
	pos.y=0;
	pos.z=2;//.*cos(angle);
	pos.x=pos.z/cos(angle);  //scale*2.*sin(angle);

	printf("Calculation is sending this to Nadine= %f %f %f \n",pos.x,pos.y,pos.z);
	
	if(checkTimeInterval(INTERVAL,lastSpeak))
	{
		if(direction)//&&pos.x<=10)
		{
			if(pos.x>20)
				pos.x=0;pos.y=0.2;pos.z=3;
		nadineClient->getClient()->speak("Phone ringing",10);
		nadineClient->getClient()->lookAtPosition(pos);
		nadineClient->getClient()->playAnimation(Animation::ROCK);
		lastSpeak=time(NULL);
		lastTrack=time(NULL);
		//std::cout<<" Non Impulsive Sound Position received "<<pos.x<<" "<<pos.y<<" "<<pos.z<< "\n";
		}
		else//&&pos.x<=10)
		{
			if(pos.x<0)
				pos.x=0;pos.y=4;pos.z=3;
		nadineClient->getClient()->speak("phone ringing",10);
		nadineClient->getClient()->lookAtPosition(pos);
		nadineClient->getClient()->playAnimation(Animation::ROCK);
		lastSpeak=time(NULL);
		lastTrack=time(NULL);
		}
		//std::cout<<" Non Impulsive Sound Position received "<<pos.x<<" "<<pos.y<<" "<<pos.z<< "\n";
		
	}
	
}*/

	
void reactiveLayer::nonImpulsiveSoundDetected(AudioLocalization source)
{
	double angle=source.azimuth;
	imi::Vec3 pos;
	int scale=1;
	int direction=1;
	//if(source.azimuth>0)
		//direction=0;
	printf("Sound is sending voice with angle= %f \n",source.azimuth);
	pos.y=2;
	pos.z=0;//.*cos(angle);
	pos.x=pos.y/tan(angle);  //scale*2.*sin(angle);

	printf("Calculation is sending this to Nadine= %f %f %f \n",pos.x,pos.y,pos.z);
	
	if(checkTimeInterval(INTERVAL,lastSpeak))
	{
		if(direction)//&&pos.x<=10)
		{
				//if(pos.x<10 && pos.x>0)
			//{
				//pos.x=0;pos.y=0.2;pos.z=3;
		nadineClient->getClient()->lookAtPosition(pos);
		//nadineClient->getClient()->speak("Phone ringing",10);
		//nadineClient->getClient()->playAnimation(Animation::ROCK);
		//lastSpeak=time(NULL);
		lastTrack=time(NULL);
		//std::cout<<" Non Impulsive Sound Position received "<<pos.x<<" "<<pos.y<<" "<<pos.z<< "\n";
					}//}
		else//&&pos.x<=10)
		{
			//if(pos.x<0)
		pos.x=0;pos.y=4;pos.z=3;
		//nadineClient->getClient()->speak("phone ringing",10);
		nadineClient->getClient()->lookAtPosition(pos);
		//nadineClient->getClient()->playAnimation(Animation::ROCK);
		lastSpeak=time(NULL);
		lastTrack=time(NULL);
		}
		//std::cout<<" Non Impulsive Sound Position received "<<pos.x<<" "<<pos.y<<" "<<pos.z<< "\n";
		
	}
	
}





void reactiveLayer::speechDetected(AudioLocalization source)
{
	double angle=source.azimuth;
	imi::Vec3 pos;
	int scale=1;
	int direction=1;
	//if(source.azimuth==0)
		//direction=0;
	//if(source.azimuth>3.14)
	//	direction=;
	printf("Sound is sending voice with angle= %f \n",source.azimuth);
	
	pos.y=2.5;//this is an assumption it should be read from kinerct
	pos.z=0;//
	pos.x=pos.y/tan(angle);  //scale*2.*sin(angle);

	printf("Sound user position in sound system x y z %f %f %f \n",pos.x,pos.y,pos.z);
	
	//printf("angle and cosangle= %f %f \n",angle,cos(angle));
	//printf("Calculation is sending this to Nadine= %f %f %f \n",pos.x,pos.y,pos.z);

	if(checkTimeInterval(2,lastSpeak))
	{
			//if(pos.x<10 && pos.x>0)
			//{
				//pos.x=0;pos.y=0.2;pos.z=3;
				nadineClient->getClient()->speak("Someone speaking",10);
				nadineClient->getClient()->lookAtPosition(pos);
				//nadineClient->getClient()->lookAtPosition([0,0.2,3]);
				//nadineClient->getClient()->speak("someone speaking",10);
				/*int randVar=rand() % 100;
				if(randVar<20)
				{
					nadineClient->getClient()->speak("That's interesting",10);
				}
				else
				{
					if(randVar<40)
					{
							nadineClient->getClient()->speak("Yes tell me",10);
					}
					else
					{
						if(randVar<60)
						{
							nadineClient->getClient()->speak("Yes, go on",10);
						}
						else
						{
							if(randVar<80)
							{
								nadineClient->getClient()->speak("<spurt audio=\"g0001_014\">cough cough</spurt>",10);
							}
							else
							{
								nadineClient->getClient()->speak("<spurt audio=\"g0001_025\">cough cough</spurt>",10);
							}
						}
					}
				}*/
				//nadineClient->getClient()->playAnimation(Animation::ROCK);
				lastTrack=time(NULL);
				lastSpeak=time(NULL);
	}
			//}
			
	
}





void reactiveLayer::skeletonDetected(double x, double y, double z)
{

	//if((abs(currentHeadPos.y-y)>0.1)||(abs(currentHeadPos.z-z)>0.1))
	//{
		//if(checkTimeInterval(INTERVAL,lastSpeak)&&checkTimeInterval(INTERVAL,lastAction)&&checkTimeInterval(15,lastShake))
		//{
			currentHeadPos.x=x;
			currentHeadPos.y=y;
			currentHeadPos.z=z;
			//std::cout<<"Time before starting: "<<time(NULL)<<"\n";
			nadineClient->getClient()->lookAtPosition(currentHeadPos);
			//std::cout<<"Time after lookAt: "<<time(NULL)<<"\n";
			//nadineClient->getClient()->playAnimation(Animation::ROCK);
			//std::cout<<"Time after anim: "<<time(NULL)<<"\n";
		//}
	//}
}
		

void reactiveLayer::skeletonDetectedSpecial(double x, double y, double z)
{
	if(robot)
		return;

	imi::Vec3 pos;
	pos.x=x*200;
	pos.y=y*200;
	pos.z=z*200;

	if(pos!=previousPos)
	{
		if(checkTimeInterval(INTERVAL,lastTrack)&&((pos.x!=0)||(pos.y!=0)||(pos.z!=0)))
		{
			nadineClient->getClient()->lookAtPosition(pos);
			previousPos=pos;
		}
	}
	
}

void reactiveLayer::userNew()
{
	alone=false;
	lastAction=time(NULL);
	lastSpeak=time(NULL);
	std::cout<<"nadineClient returning: "<<nadineClient->getClient()->speak("Hi, I am Nadine",10)<<"\n";
	std::cout<<"nadineClient Returning: "<<nadineClient->getClient()->playAnimation(Animation::WAVE_HAND)<<"\n";
}

void reactiveLayer::userLost()
{
	lastAction=time(NULL);
	lastSpeak=time(NULL);
	nadineClient->getClient()->speak("Please come back later!"+currentUser,10);
	nadineClient->getClient()->playAnimation(Animation::WAVE_HAND);
	alone=true;
	currentUser="";
}

void reactiveLayer::havingQuestionDetected()
{
	if (checkTimeInterval(INTERVAL,lastAction)&&recogActive)
	{
		lastAction=time(NULL);
		lastSpeak=time(NULL);
		//nadineClient->getClient()->setFaceExpression(Facial_Expression::NEUTRAL,10);
		nadineClient->getClient()->lookAtPosition(currentHeadPos);
		//nadineClient->getClient()->playAnimation(Animation::POINTING_YOU);
		//nadineClient->getClient()->speak("What is your question?",10);
		nadineClient->getClient()->speak("You are raising your hand",10);
		nadineClient->getClient()->playAnimation(Animation::POINTING_YOU);
	}
}

void reactiveLayer::objectionDetected()
{
	if (checkTimeInterval(INTERVAL,lastAction)&&(recogActive))
	{
		lastAction=time(NULL);
		lastSpeak=time(NULL);
		//nadineClient->getClient()->setFaceExpression(Facial_Expression::SADNESS,10);
		nadineClient->getClient()->lookAtPosition(currentHeadPos);
		//nadineClient->getClient()->playAnimation(imi::Animation::RIGHT_SWEEP);
		nadineClient->getClient()->speak("you disagree!",10);
		
		nadineClient->getClient()->playAnimation(imi::Animation::WHY);
	}
}
	/*	recogActive=!recogActive;
		if(recogActive)
		{
			if(!intro)
			{
				nadineClient->getClient()->speak("Ok, let's start",10);
				nadineClient->getClient()->playAnimation(imi::Animation::ROCK);
				intro=false;
			}
			else
			{
				nadineClient->getClient()->speak("Hello, I am Nadine a Companion robot. My capabilities are growing. Today I will show you that I can already understand gestures.",10);
				nadineClient->getClient()->playAnimation(imi::Animation::ROCK);
			}

		}
		else
		{
			nadineClient->getClient()->setFaceExpression(Facial_Expression::SADNESS,10);
			nadineClient->getClient()->speak("Ok, we can stop then",10);
			nadineClient->getClient()->playAnimation(imi::Animation::GRAB_OBJECT);
		}
	}
	lastAction=time(NULL);
}*/

void reactiveLayer::praiseDetected()
{
	if (checkTimeInterval(INTERVAL,lastAction)&&recogActive)
	{
		lastAction=time(NULL);
		lastSpeak=time(NULL);
		nadineClient->getClient()->lookAtPosition(currentHeadPos);
		//nadineClient->getClient()->playAnimation(imi::Animation::POINTING_YOU);
		//nadineClient->getClient()->speak("Thank you for your praise!",10);
		nadineClient->getClient()->speak("You are praising!",10);
		if(!robot)
		{
			nadineClient->getClient()->setFaceExpression(Facial_Expression::HAPPINESS,10);
			nadineClient->getClient()->playAnimation(imi::Animation::POINTING_YOU);
		}
		else
		{
			//nadineClient->getClient()->playAnimation(imi::Animation::NOD_YES);
			//nadineClient->getClient()->playAnimation(Animation::ROCK);

		}
	}
}

void reactiveLayer::agreementDetected()
{
	if (checkTimeInterval(INTERVAL,lastAction)&&recogActive)
	{
		lastAction=time(NULL);
		lastSpeak=time(NULL);
		nadineClient->getClient()->lookAtPosition(currentHeadPos);
		//nadineClient->getClient()->playAnimation(imi::Animation::POINTING_YOU);
		nadineClient->getClient()->speak("You agree",10);
		if(!robot)
		{
			nadineClient->getClient()->setFaceExpression(Facial_Expression::HAPPINESS,10);
			nadineClient->getClient()->playAnimation(imi::Animation::POINTING_YOU);
		}
		else
		{
			//nadineClient->getClient()->playAnimation(imi::Animation::NOD_YES);
			//nadineClient->getClient()->playAnimation(Animation::ROCK);
		}
	}
}

void reactiveLayer::confidentDetected()
{
	if (checkTimeInterval(INTERVAL,lastAction)&&recogActive)
	{
		lastAction=time(NULL);
		lastSpeak=time(NULL);
		//nadineClient->getClient()->setFaceExpression(Facial_Expression::HAPPINESS,10);
		nadineClient->getClient()->lookAtPosition(currentHeadPos);
		//nadineClient->getClient()->playAnimation(imi::Animation::POINTING_YOU);
		nadineClient->getClient()->speak("you are confident!"+currentUser,10);
		nadineClient->getClient()->playAnimation(imi::Animation::PAPER);
	}
}

void reactiveLayer::oncallDetected()
{
	if (checkTimeInterval(INTERVAL,lastAction)&&recogActive)
	{
		lastAction=time(NULL);
		lastSpeak=time(NULL);
		nadineClient->getClient()->lookAtPosition(currentHeadPos);
		//nadineClient->getClient()->speak("<spurt audio=\"g0001_004\">cough cough</spurt> <voice emotion='cross'> Please turn off your phone! </voice>",10);
		nadineClient->getClient()->speak("You are calling",10);
		if(!robot)
		{
			nadineClient->getClient()->setFaceExpression(Facial_Expression::ANGER,10);
			nadineClient->getClient()->playAnimation(imi::Animation::WHY);
		}
		else
		{
			//nadineClient->getClient()->playAnimation(imi::Animation::NOD_NO);
			//nadineClient->getClient()->playAnimation(Animation::ROCK);
		}
	}
}

void reactiveLayer::stopDetected()
{
	if (checkTimeInterval(INTERVAL,lastAction)&&recogActive)
	{
		lastAction=time(NULL);
		lastSpeak=time(NULL);
		nadineClient->getClient()->setFaceExpression(Facial_Expression::SADNESS,10);
		nadineClient->getClient()->lookAtPosition(currentHeadPos);
		//nadineClient->getClient()->playAnimation(imi::Animation::POINTING_YOU);
		//nadineClient->getClient()->speak("Why do you stop me? What is the reason?",10);
		//nadineClient->getClient()->playAnimation(imi::Animation::WHY);
		nadineClient->getClient()->speak("you are stopping me",10);
		nadineClient->getClient()->playAnimation(imi::Animation::ROCK);
	}
}

void reactiveLayer::successDetected()
{
	if (checkTimeInterval(INTERVAL,lastAction)&&recogActive)
	{
		lastAction=time(NULL);
		lastSpeak=time(NULL);
		nadineClient->getClient()->lookAtPosition(currentHeadPos);
		//nadineClient->getClient()->speak("Well done! You are successful!",10);
		nadineClient->getClient()->speak("You succeed",10);
		if(!robot)
		{
			nadineClient->getClient()->setFaceExpression(Facial_Expression::HAPPINESS,10);
			nadineClient->getClient()->playAnimation(imi::Animation::PAPER);
		}
		else
		{
			//nadineClient->getClient()->playAnimation(imi::Animation::NOD_YES);
			//nadineClient->getClient()->playAnimation(imi::Animation::ROCK);
		}
	}
}

void reactiveLayer::readDetected()
{
	if (checkTimeInterval(INTERVAL,lastAction)&&recogActive)
	{
		lastAction=time(NULL);
		lastSpeak=time(NULL);
		//nadineClient->getClient()->setFaceExpression(Facial_Expression::NEUTRAL,10);
		nadineClient->getClient()->lookAtPosition(currentHeadPos);
		//nadineClient->getClient()->playAnimation(imi::Animation::POINTING_YOU);
		//nadineClient->getClient()->speak("Please, take your time and read it carefully.",10);
		nadineClient->getClient()->speak("You are reading",10);
		//nadineClient->getClient()->playAnimation(imi::Animation::OFFER);
		//nadineClient->getClient()->playAnimation(imi::Animation::ROCK);
	}
}

void reactiveLayer::writeDetected()
{
	if (checkTimeInterval(INTERVAL,lastAction)&&recogActive)
	{
		lastAction=time(NULL);
		lastSpeak=time(NULL);
		//nadineClient->getClient()->setFaceExpression(Facial_Expression::NEUTRAL,10);
		nadineClient->getClient()->lookAtPosition(currentHeadPos);
		//nadineClient->getClient()->playAnimation(imi::Animation::POINTING_YOU);
		//nadineClient->getClient()->speak("If you need time for taking notes, I can slow my presentation.",10);
		nadineClient->getClient()->speak("You are writing",10);
		nadineClient->getClient()->playAnimation(imi::Animation::WHY);

	}
}

void reactiveLayer::drinkDetected()
{
	if (checkTimeInterval(INTERVAL,lastAction)&&recogActive)
	{
		lastAction=time(NULL);
		lastSpeak=time(NULL);
		nadineClient->getClient()->setFaceExpression(Facial_Expression::NEUTRAL,10);
		nadineClient->getClient()->lookAtPosition(currentHeadPos);
		//nadineClient->getClient()->playAnimation(imi::Animation::POINTING_YOU);
		//nadineClient->getClient()->speak("you can have a drink, no problem. I will continue as soon as you are done,",10);
		
		nadineClient->getClient()->speak("You are drinking",10);
		//nadineClient->getClient()->playAnimation(imi::Animation::OFFER);
		nadineClient->getClient()->playAnimation(imi::Animation::ROCK);
	}
}


bool reactiveLayer::checkTimeInterval(float anInterval,time_t aTime)
{
	time_t a=time(NULL);
	return difftime(a,aTime)>anInterval;
}


void reactiveLayer::updateGaze(double x, double y)
{
	//if(checkTimeInterval(0.3,lastEyeMovement))
	{
		imi::WorldPosition pos;
		pos.z=-1;
		if (x<140)
		{
			pos.x=10;
		}
		if(x>180)
		{
			pos.x=-10;
		}
		if(y<140)
		{
			pos.y=10;
		}
		if(y>180)
		{
			pos.y=-10;
		}
		std::cout<<"X received is "<<x<<" Y received is "<<y<<"\n";
		nadineClient->getClient()->lookAtPosition(pos);
		lastEyeMovement=time(NULL);
	}
}

reactiveLayer::~reactiveLayer(void)
{
}
