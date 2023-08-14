#include <iostream>
#include "BB_Introduce.h"
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

namespace BB
{
	BB_Introduce::BB_Introduce(ProtectedClient<imi::AgentControlClient> *aClient)
	{
		threshold = 2;
		activationLevel = 0;
		myBBType = "BB_Introduce";
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
		disable = false;

		//introduce();

	}

	BB_Introduce::~BB_Introduce()
	{
	}


	bool BB_Introduce::updateActivationLevel(perceptualInput aInput)
	{
		//if (!mtx.try_lock())
		//	return activationLevel >= threshold;

		if (aInput.sensorId == "Introduce")
		{
			activationLevel += 10;
			lastSentence = aInput.sentence;
			scripted = true;
		}
		if (aInput.sensorId == "Skype")
		{
			disable=true;
		}
		if (aInput.sensorId == "Demo")
		{
			disable=false;
		}
		if (aInput.sensorId == "Feedback")
		{
			speaking = aInput.behaviorType == "Start";
			std::cout << "Speaking has been updated to: " << speaking << "\n";
		}

		if (aInput.sensorId == "KinectV2")
		{
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
				users.at(i)->skeleton = aInput.position.at(i);
				users.at(i)->position = users.at(i)->skeleton.head;
				if (users.at(i)->beingLooked)
				{
					posToLookAt = users.at(i)->skeleton.head;
					lastLook = i;
				}
			}
		}
		
		if (aInput.sensorId == "Speech")
		{
			if (activationLevel>0)
			{
				activationLevel = 0;
				introduced = true;
				firstIntro = true;
			}
		}

		if (aInput.behaviorType == "ReactFace")
		{
			firstIntro = true;
			introduced = true;
		}

		if (aInput.behaviorType == "ReactToSpeechSource")
		{
			firstIntro = true;
			introduced = true;
			activationLevel = 0;
		}

		if (aInput.behaviorType == "ReactFace")
		{
			activationLevel += threshold;
		}


		if (aInput.behaviorType == "LookAtPerson")
		{
			int engaged = 0;

			if (aInput.userID < users.size())
			{
				for (int i = 0; i < users.size(); i++)
				{
					users.at(i)->beingLooked = false;
					if (users.at(i)->faceStatus.engaged == imi::FacePossibilities::YES)
						engaged++;
				}
				users.at(aInput.userID)->beingLooked = true;
				posToLookAt = users.at(aInput.userID)->skeleton.head;
			}
			//activationLevel += 0.01*(1+engaged);
			activationLevel += 0.05*(1+engaged);
		}

		if (aInput.sensorId =="KinectV2_Face")
		{
// 			while (users.size()<aInput.faceStatus.size())//Create new users
// 			{
// 				users.push_back(new User());
// 			}
// 
// 			while (users.size()>aInput.faceStatus.size())//delete users that are gone
// 			{
// 				User *temp = users.back();
// 				users.pop_back();
// 				delete temp;
// 			}

			if (users.size() == aInput.faceStatus.size())
			{

				for (int i = 0; i < users.size(); i++)
				{
					users.at(i)->faceStatus = aInput.faceStatus.at(i);
				}
			}
		}

		if (aInput.sensorId == "TimePassed")
		{
			if (users.size() == 0)
			{
				countExec = 1;
				activationLevel = 0;
				threshold = 2;
				introduced = false;
				allReadyDone.clear();
				//std::cout << "ALL USERS ARE GONE \n";
			}
			else
			{
				if (countExec > 1)
					countExec = countExec - 0.1;
			}
		}

		if (aInput.behaviorType == myOutput.behaviorType)
		{
			boost::variate_generator<boost::mt19937, boost::normal_distribution<> > generator(boost::mt19937(time(NULL)), boost::normal_distribution<>(1, (countExec++*10)));
			activationLevel = -abs(generator());
		}
	//	mtx.unlock();
		return activationLevel >= threshold;
	}

	perceptualInput BB_Introduce::execute()
	{
		perceptualInput temp;
		if (disable)
		{
			activationLevel = 0;
			return temp;
		}
		if (speaking)
		{
			perceptualInput temp;
			return temp;
		}

		mtx.lock();

#ifdef VH
		nadineClient->getClient()->touchTarget("SIT_DOWN");
#endif
		if (scripted)
		{
			switch (scriptCount)
			{
				case -1:
					nadineClient->getClient()->speak("Let me look up first", 10);
					break;
				case 0:
					nadineClient->getClient()->touchTarget("Laptop_Waving_03_x2(Lookup)");
					sleep(500);
					nadineClient->getClient()->speak("Hi! I am Nadine. What is your name?",10);
					break;
				case 1:
					nadineClient->getClient()->speak("Thank you. You look nice too. I was born in Japan but I grew up in IMI.", 10);
					sleep(5500);
					nadineClient->getClient()->touchTarget("Laptop_OneArmGesture_04_(Lookup)");
					nadineClient->getClient()->speak("IMI is the Institute for Media Innovation in NTU. What about you?", 10);
					break;
				case 2:
					nadineClient->getClient()->speak("Well Edgar", 10);
					sleep(1000);
					nadineClient->getClient()->touchTarget("LOOKUP_TwoArmGesture");
					nadineClient->getClient()->speak("I am a social robot but I cannot walk.", 10);
					break;
				case 3:
					nadineClient->getClient()->touchTarget("Laptop_OneArmGesture_02_(Lookup)");
					nadineClient->getClient()->speak("Please do. We can always talk via the internet.", 10);
					
					sleep(5000);
					nadineClient->getClient()->touchTarget("Laptop_TwoArmGesture_02_(Lookup)");
					nadineClient->getClient()->speak("Look, I have my own laptop.", 10); 
						
					break;
				case 4:
					nadineClient->getClient()->touchTarget("nod");
					sleep(300);
					nadineClient->getClient()->speak("Yes!", 10);
					break;
				case 5:
					nadineClient->getClient()->touchTarget("Laptop_Waving_03_x1(Lookup)");
					sleep(300);
					nadineClient->getClient()->speak("Nice meeting you Edgar.", 10);
					break;
			}
			if (scriptCount < 5)
				scriptCount++;
			else
				scriptCount = 0;

			perceptualInput temp;
			activationLevel = 0;
			mtx.unlock();
			return temp;

		}



#ifdef Chat
		chatBot->getClient()->sentenceRecognized("ReactiveLayer",0,"A",0);
#endif
		if (!firstIntro || (!introduced && difftime(time(NULL), lastIntro) > 60))
		{
			std::cout << "AGENT INTRO !!!" < "\n";
			firstIntro = true;
			introduce();
		}
		else
		{
			for (int i = 0; i < users.size(); i++)
			{
				if (users.at(i)->beingLooked == true)
				{
					std::string color="";
#ifdef GERMAN
					switch (users.at(i)->faceStatus.color)
					{
					case 0:
						color = "rot";
						break;
					case 1:
						color = "orange";
						break;
					case 2:
						color = "gelb";
						break;
					case 3:
						color = "gruen";
						break;
					case 4:
						color = "blaugruen";
						break;
					case 5:
						color = "blau";
						break;
					case 6:
						color = "violett";
						break;
					case 7:
						color = "magenta";
						break;
					case 8:
						color = "weiss";
						break;
					case 9:
						color = "schwarz";
						break;
					default:
						color = "";
					}
#else
					switch (users.at(i)->faceStatus.color)
					{
					case 0:
						color = "red";
						break;
					case 1:
						color = "orange";
						break;
					case 2:
						color = "yellow";
						break;
					case 3:
						color = "green";
						break;
					case 4:
						color = "cyan";
						break;
					case 5:
						color = "blue";
						break;
					case 6:
						color = "violet";
						break;
					case 7:
						color = "magenta";
						break;
					case 8:
						color = "white";
						break;
					case 9:
						color = "black";
						break;
					default:
						color = "";
					}
#endif
					if (color != "" && color!="black")
					{
						bool found = false;
						for (int i = 0; i < allReadyDone.size(); i++)
						{
							if (color == allReadyDone.at(i))
								found = true;
						}
						if (!found)
						{
#ifdef GERMAN
							//nadineClient->getClient()->speak("Sie sind in " + color + " heute.", 10);
#else
							//nadineClient->getClient()->speak("It seems that you are in " + color + " today. Aren't you?", 10);
#endif
							
							//spoken = time(NULL);
							//std::cout << "AGENT SPEAKING !!!" < "\n";
							allReadyDone.push_back(color);
						}
					}
					break;
				}
			}
		}
// 		mtx.lock();
// 		activationLevel = 0;

#ifdef VH
		//int randVal = rand() % 6 + 2;
		int randVal = rand() % 10 + 4;
		std::string Result;          // string which will contain the result

		std::ostringstream convert;   // stream used for the conversion

		convert << randVal;      // insert the textual representation of 'Number' in the characters in the stream

		Result = convert.str(); // set 'Result' to the contents of the stream
		if (randVal == 4)
			nadineClient->getClient()->touchTarget("SIT_IDLE_WATCH");
		else if (randVal == 5)
			//if (randVal<5)
			nadineClient->getClient()->touchTarget("SIT_IDLE0" + Result);
		else if (randVal == 7)
			nadineClient->getClient()->touchTarget("SIT_IDLE0" + Result);
#endif

 		mtx.unlock();
#ifdef Chat
		chatBot->getClient()->sentenceRecognized("ReactiveLayer",0,"A",0);
#endif
		return myOutput;

	}

	void BB_Introduce::introduce()
	{

#ifndef GERMAN
		lastIntro = time(NULL);
		introduced = true;
		threshold = 5;
#ifdef MEMORY
		return;
#endif
		//nadineClient->getClient()->playAnimation(imi::Animation::WAVE_HAND);
		//gaze(1000);
		//nadineClient->getClient()->speak("Hello, Welcome to the museum!", 10);
		//gaze(2500);
#ifndef JAKARTA
		nadineClient->getClient()->speak("Hello, my name is Nadine", 10);
		gaze(2600, lastLook);
#else
		nadineClient->getClient()->speak("Pagii! Nama saya Nicole", 10);
		gaze(2600, lastLook);
#endif
		//sleep(2600);
		//nadineClient->getClient()->playAnimation(imi::Animation::OFFER);
		//nadineClient->getClient()->speak("I am your companion of the future", 10);
		//sleep(3000);
		//gaze(3000, lastLook);
// 		nadineClient->getClient()->speak("play games, and do some gestures and facial expressions.", 10);
// 		gaze(4500);
// 		nadineClient->getClient()->speak("I am so excited to see you all", 10);
// 		gaze(2500);
// 		nadineClient->getClient()->playAnimation(imi::Animation::RIGHT_SWEEP);
// 		nadineClient->getClient()->speak("and I’m looking forward for some fun and interaction with you!", 10);
// 		gaze(3000);

#else
		
		lastIntro = time(NULL);
		introduced = true;
		threshold = 5;

		nadineClient->getClient()->touchTarget("Laptop_Waving_02_(Lookup)");
		nadineClient->getClient()->speak("Liebe Besucherinnen und Besucher", 10);
		
		gaze(2600,lastLook);

		nadineClient->getClient()->speak("<s> mein Name ist  <usel variant='5'> Nadien </usel> </s>", 10);
		gaze(2500, lastLook);

		nadineClient->getClient()->touchTarget("Laptop_TwoArmGesture_(Lookup)");
		nadineClient->getClient()->speak("Ich gehoere zu den wenigen humanoiden Roboterklonen auf der Welt", 10);
		gaze(4500, lastLook);

		nadineClient->getClient()->speak("und bin der erste meiner Art in Deutschland.", 10);
		gaze(4000, lastLook);

		//nadineClient->getClient()->touchTarget("Laptop_OneArmGesture_04_(Lookup)");
		nadineClient->getClient()->speak("Ich bin nach dem Vorbild von Nadia Magnenat Thalmann gestaltet.", 10);
		gaze(3800, lastLook);

		nadineClient->getClient()->touchTarget("Laptop_OneArmGesture_03_(Lookup)");
		nadineClient->getClient()->speak("Derzeit wird noch an mir geforscht.", 10);
		gaze(2500, lastLook);

		nadineClient->getClient()->touchTarget("Laptop_TwoArmGesture_(Lookup)");
		nadineClient->getClient()->speak("Eine freie Konversation mit den Menschen fellt mir auf Deutsch noch etwas schwer.", 10);
		gaze(5000, lastLook);

		nadineClient->getClient()->speak("Aber vielleicht sehen wir uns ja in ein paar Jahren wieder und koennen weiterreden.", 10);
		gaze(5000, lastLook);


		nadineClient->getClient()->touchTarget("Laptop_OneArmGesture_04_(Lookup)");
		nadineClient->getClient()->speak("Jetzt wuensche ich Ihnen erst einmal viel Spass,", 10);
		gaze(3000, lastLook);
		
		nadineClient->getClient()->speak("<s> etwas ueber die virtuellen Welten der <usel variant='7'> compjuter animation </usel> </s> und simulation zu erfahren", 10);
		gaze(6000, lastLook);

#endif
	}
}