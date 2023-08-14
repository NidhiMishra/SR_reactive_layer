#include <iostream>
#include "BB_Facebook.h"
#include "Behavior.h"

#include "AgentControl.h"
#include "ProtectedClient.h"
#include "Inputs_types.h"
#include <string>

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>


namespace BB
{
	BB_Facebook::BB_Facebook(ProtectedClient<imi::AgentControlClient> *aClient)
	{
		threshold = 2;
		activationLevel = 0;
		myBBType = "BB_Facebook";
		myOutput.behaviorType = myBBType;
		nadineClient = aClient;
		previousBehavior = "";
		executing = false;
		lastTimeSpeak = time(NULL);
		lastIntro = time(NULL);
		lastSentence = "";
		speaking = false;
		user_dialogue_choice = -20;
		users.resize(6);
		countExec = 0;
		introduced = false;
		firstIntro = false;
		lastLook = -1;

		scriptCount = -1;
		scripted = false;

		//s1 = new serverStarter();
		//boost::thread NewSpeechThread(&serverStarter::newspeechServer, s1);
		//aFacebook = new ProtectedClient<imi::FacebookClient>("localhost", imi::g_SocialNetwork_constants.DEFAULT_FACEBOOK_SERVICE_PORT);

		s1 = new serverStarter();
		//boost::thread NewSpeechThread(&serverStarter::newspeechServer, s1);
		boost::thread FacebookServer(&serverStarter::newfacebookServer, s1);
		//introduce();

	}

	BB_Facebook::~BB_Facebook()
	{
	}


	bool BB_Facebook::updateActivationLevel(perceptualInput aInput)
	{
		//if (!mtx.try_lock())
		//	return activationLevel >= threshold;

		if (aInput.sensorId == "Facebook")
		{
			activationLevel += 10;
			lastSentence = aInput.sentence;

		}

		if (aInput.sensorId == "Speech")
		{
			if (aInput.sentence == "TAKEPIC")
			{
				activationLevel += 10;
				lastSentence = aInput.sentence;
			}
		}
		return activationLevel >= threshold;
	}

	perceptualInput BB_Facebook::execute()
	{
		std::streampos size;
		char * memblock;
		std::string *picture, *pic2;

		mtx.lock();
		nadineClient->getClient()->speak("Yes, I can", 10);
		sleep(2000);
		nadineClient->getClient()->speak("I will post your picture on facebook", 10);
		sleep(4000);
		nadineClient->getClient()->speak("Three", 10);
		sleep(1000);
		nadineClient->getClient()->speak("Two", 10);
		sleep(1000);
		nadineClient->getClient()->speak("One", 10);
		sleep(1000);
		nadineClient->getClient()->speak("Say Cheese...", 10);

		std::string ret, pass = "hi";

		//s1->handlerSpeech->sendNewString(temp);

		s1->handlerFaceBook->takepic("YES");
		//aFacebook->getClient()->takePicture("PICTURE");
		sleep(1000);
		nadineClient->getClient()->speak("You can check my facebook page!", 10);

		/*
		std::ifstream file("C:/Users/IMI-Demo/Desktop/IMI/i2p_bb_reactive_layer_V1.1/test.jpg", std::ios::in | std::ios::binary | std::ios::ate);
		if (file.is_open())
		{
		size = file.tellg();
		memblock = new char[size];
		file.seekg(0, std::ios::beg);
		file.read(memblock, size);
		file.close();
		picture = new std::string();// (memblock);
		std::cout << "Size is: " << size << " string size is: " << picture->size() << "\n";

		for (int i = 0; i < size; i++)
		*picture = *picture + memblock[i];
		//aFacebook->getClient()->postMessageAndPicture("test picture", *picture);
		aFacebook->getClient()->postPicture(*picture);
		delete[] memblock;
		delete picture;
		}
		else std::cout << "Unable to open file";
		*/
		activationLevel = 0;
		mtx.unlock();
		return myOutput;

	}
}