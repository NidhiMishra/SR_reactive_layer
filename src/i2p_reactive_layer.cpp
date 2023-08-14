#ifndef REACTIVE_LAYER
#define REACTIVE_LAYER
#include "UserTrackingService.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include "Server_Starter.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include "ThriftTools.hpp"
#include "ProtectedClient.h"
#include "AgentControl.h"
#include "Inputs_constants.h"

int main(int argc, char **argv)
{
	
	std::cout << "**********************************************************************************************\n";
	std::cout << "This software belongs to the Institute for Media Innovation, Nanyang Technological University. \n";
	std::cout<<"The main developers are Aryel Beck, Zhang Zhijun,  Rubha Shri Narayanan. \n";
	std::cout << "**********************************************************************************************\n \n \n";
	sleep(500);
	//ProtectedClient<imi::AgentControlClient> Nadine("155.69.52.11",9090);//This is a Client for the AgentServer running on the Robot Computer
	//ProtectedClient<imi::AgentControlClient> Nadine("155.69.54.53",9090);//Aryel backup computer
	ProtectedClient<imi::AgentControlClient> Nadine("localhost",9090);

	serverStarter *s1=new serverStarter();//This object will be used to start the different server. each server run on a thread. 


	boost::thread feedbackThread(&serverStarter::feedbackServer,s1);//Open a server for the feedback of the agent. This needs to be done at the beginning otherwise the Agent cannot start properly. The Agent Control needs to connect to this feedback server
	
	reactiveLayer *decision=new reactiveLayer(&Nadine);//This is the object that will decide what to do ("decision")

	boost::thread NewKinectTracker(&serverStarter::newKinectServer,s1,decision);

	//boost::thread ObjectDetectThread(&serverStarter::objectServer,s1,decision);//Open a server for the Object Detection

	//boost::thread ActionRecognitionThread(&serverStarter::actionServer,s1,decision);//Open a server for the Action Recognition

	//boost::thread kinectServerThread(&serverStarter::gestureServer,s1,decision);//Open a server for the Kinect

	//boost::thread FaceRecoThread(&serverStarter::faceServer,s1,decision);//Open a server for the Face Recognition

	boost::thread SoundLocaThread(&serverStarter::soundServer,s1,decision);//Open a server for the sound localization

//#ifndef MEMORY
	boost::thread SpeechThread(&serverStarter::speechServer,s1,decision);//Open a server for the sound localization
//#endif

	boost::thread EmotionThread(&serverStarter::emotionServer,s1,decision);//Open a server for the Emotion Engine

	boost::thread SpeakerIdThread(&serverStarter::SepakerIdServer, s1, decision);//Open a server for the Emotion Engine

	//boost::thread NewSpeechThread(&serverStarter::newspeechServer, s1, decision);

	boost::thread FacebookServer(&serverStarter::newfacebookServer, s1);

	s1->feedbackHandler->addDecision(decision);//This tells the feedback server where the "decision" is.


	boost::thread executeLoop(&reactiveLayer::executeWinner,decision);//Starting the loop that execute the active behavior.
	boost::thread timePass(&reactiveLayer::updateTimeBehaviors,decision);//This is used to update the behavior that have time dependency.

	boost::thread keyboardInput(&reactiveLayer::fakeInput,decision);

#ifdef Chat
	decision->connectChatBot();
	decision->sendInfoBot(0);
#endif

	


	//kinectServerThread.join();
	NewKinectTracker.join();
	//ObjectDetectThread.join();
	//ActionRecognitionThread.join();
	//FaceRecoThread.join();
	SoundLocaThread.join();
	
//#ifndef MEMORY
	SpeechThread.join();
//#endif
	feedbackThread.join();
	EmotionThread.join();
	SpeakerIdThread.join();
	executeLoop.join();
	timePass.join();
	return 0;
}
#endif