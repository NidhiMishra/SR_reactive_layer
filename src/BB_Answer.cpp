#include <iostream>
#include "BB_Answer.h"
#include "Behavior.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include "ThriftTools.hpp"
#include "AgentControl.h"
#include "ProtectedClient.h"
#include "Inputs_types.h"
#include <string>
#include <algorithm>
namespace BB
{

	BB_Answer::BB_Answer(ProtectedClient<imi::AgentControlClient> *aClient)
	{
		threshold = 10;
		activationLevel = 0;
		myBBType = "Answer";
		nadineClient = aClient;
		previousBehavior = "";
		executing = false;
		lastTimeSpeak = time(NULL);
		lastSentence = "";
		speaking = false;
	}


	BB_Answer::~BB_Answer()
	{
	}


	bool BB_Answer::updateActivationLevel(perceptualInput aInput)
	{
		if (!mtx.try_lock())
			return activationLevel >= threshold;

		if (aInput.sensorId == "Speech")
		{
			if(aInput.sentence!="REPORTOBJ" && aInput.sentence!="TAKEPIC"){
				activationLevel += 10;
				toBeSpoken = aInput.sentence;
			}
		}

		mtx.unlock();
		return activationLevel >= threshold;
	}

	perceptualInput BB_Answer::execute()
	{
		perceptualInput temp;

		if (speaking)
		{
			return temp;
			activationLevel = 0;
		}

		mtx.lock();
#ifdef Chat
		chatBot->getClient()->sentenceRecognized("ReactiveLayer",0,"A",0);
#endif

		std::transform(toBeSpoken.begin(), toBeSpoken.end(), toBeSpoken.begin(), ::tolower);
#ifdef GERMAN
		if (toBeSpoken.find("nadien"))
		{
			nadineClient->getClient()->speak("<s> <usel variant='5'>" + toBeSpoken + "</usel> </s>", 10);
			std::cout << "ËXECUTING ANSWER INSIDE IF\n";
		}
#else
		if (toBeSpoken.find("german:::") != string::npos)
		{
			try{
			toBeSpoken = toBeSpoken.erase(0, 9);
			std::cout << "Speak German :" << toBeSpoken << "\n";
			nadineClient->getClient()->speak(toBeSpoken, 20);
			}
			catch(...){
				std::cout << "ERROR SPEAK GERMAN!!!\n";
			}
		}
		else if (toBeSpoken.find("french:::") != string::npos)
		{
			try{
			toBeSpoken = toBeSpoken.erase(0, 9);
			std::cout << "Speak French :" << toBeSpoken << "\n";
			nadineClient->getClient()->speak(toBeSpoken, 30);
			}
			catch(...){
				std::cout << "ERROR SPEAK FRENCH!!!\n";
			}
		}
		else
		{
			try{
			std::cout << "Speak English:" << toBeSpoken << "\n";
			nadineClient->getClient()->speak(toBeSpoken, 10);
			}
			catch(...){
				std::cout << "ERROR SPEAK ENGLISH!!!\n";
			}
		}

		
#endif
		activationLevel = 0;


#ifdef VH
		// Convert string to be spoken to upper case
		std::string tempStr = toBeSpoken;
		std::transform(tempStr.begin(), tempStr.end(), tempStr.begin(), ::toupper);
		// If "Yes" is detected in toBeSpoken, play nodding head animation
		if (tempStr.compare(0, 3, "YES") == 0)
		{
			nadineClient->getClient()->touchTarget("SIT_NOD_YES");
			std::cout << "Play Animation: SIT_NOD_YES \n";
		}
		// If "No" is detected in toBeSpoken, play shaking head animation
		else if (tempStr.compare(0, 2, "NO") == 0)
		{
			nadineClient->getClient()->touchTarget("SIT_NOD_NO");
			std::cout << "Play Animation: SIT_NOD_NO \n";
		}
		//If "I Don't know" or "Sorry, I don't know" is detected, play two arm gesture animation
		else if ((tempStr.compare(0, 12, "I DON'T KNOW") == 0) || (tempStr.compare(0, 19, "SORRY, I DON'T KNOW") == 0))
		{
			nadineClient->getClient()->touchTarget("SIT_TWO_ARM_GESTURE03");
			std::cout << "Play Animation: SIT_TWO_ARM_GESTURE03 \n";
		}
		//If "My Name is Nicole" is detected, play two arm gesture animation
		//else if(tempStr.compare(0,17,"MY NAME IS NICOLE")==0)
		//{
		//	nadineClient->getClient()->touchTarget("SIT_INTRODUCE");
		//	std::cout << "Play Animation: SIT_INTRODUCE \n";
		//}
		//If "The time is" is detected, play tell time animation
		else if ((tempStr.compare(0, 8, "THE TIME") == 0) || (tempStr.compare(0, 9, " THE TIME") == 0))
		{
			nadineClient->getClient()->touchTarget("SIT_TELL_TIME");
			std::cout << "Play Animation: SIT_TELL_TIME \n";
		}


		// If "YES, I CAN SMILE" is detected in toBeSpoken, play smiling animation 
		if (toBeSpoken == "YES, I CAN SMILE")
		{
			sleep(2000);
			nadineClient->getClient()->setFaceExpression(imi::Facial_Expression::HAPPINESS, 10);
		}
		// If "YES, I CAN WAVE MY HAND" is detected in toBeSpoken, play waving animation
		else if (toBeSpoken == "YES, I CAN WAVE MY HAND")
		{
			sleep(1000);
			nadineClient->getClient()->playAnimation(imi::Animation::WAVE_HAND);
			std::cout << "Play Animation: WAVE \n";
		}
#endif VH

		activationLevel = 0;


#ifdef Chat
		sleep(2000);
		chatBot->getClient()->sentenceRecognized("ReactiveLayer",0,"A",0);
#endif
		mtx.unlock();

		perceptualInput temp1;
		return temp1;

	}



}