#pragma once
#include <iostream>
#include <fstream>
#include "BB_ReactFace.h"
#include "AgentControl.h"
#include "ProtectedClient.h"
#include "Inputs_types.h"
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include "global.h"

using namespace ::std;

namespace BB
{
	BB_ReactFace::BB_ReactFace(ProtectedClient<imi::AgentControlClient> *aClient)
	{
		activationLevel = -30;
		threshold = 20;
		nadineClient = aClient;
		active = false;
		myBBType = "ReactFace";//TO DO: Remove these variable
		myOutput.behaviorType = myBBType;
		executing = false;
		//greeted.push_back("Unknown");
		skype = false;
		disable = false;
		for (int i = 0; i < 6;i++)
			previousFace[i] = "Unknown";

		toGreet = -1;
		users.resize(6);//Maximum number of users with Kinect V2
		loadNumUser(); // set number of users
//		ges_success_id = 0;
#ifndef MEMORY
		s1 = new serverStarter();
		boost::thread NewSpeechThread(&serverStarter::newspeechServer, s1);
#else
		dialogueManager = new ProtectedClient<imi::DialogueManagerServiceClient>("localhost", imi::g_Inputs_constants.DEFAULT_DIALOGUE_MANAGER_PORT);
#endif
		global::focusingUser="";

	}

	

	BB_ReactFace::~BB_ReactFace()
	{
		while (users.size() > 0)//delete users that are gone
		{
			User *temp = users.back();
			users.pop_back();
			delete temp;
		}

	}

	void BB_ReactFace::loadNumUser(){
		try{
			ifstream myfile ("NumUser.txt");

			if (myfile.is_open())
			{
				std::string line;
				getline (myfile,line);
				cout<<"Load Number of User: "<<line<<std::endl;
				numberUsers=atoi(line.c_str());
			
				myfile.close();
			}
		}
		catch(...){
			printf("Unexpected error in loadNumUser");
		}
	}

	void BB_ReactFace::writeNumUser(){
		try{
		  ofstream myfile ("NumUser.txt");
		  if (myfile.is_open())
		  {
			myfile << numberUsers;
			myfile.close();
		  }
		}
		catch(...){
			printf("Unexpected error in writeNumUser");
		}
	}

	bool BB_ReactFace::updateActivationLevel(perceptualInput aInput)
	{
		try{
		mtx.lock();
		//std::cout << "LOCKED \n";
		if (aInput.sensorId == "Skype")
		{
			skype = true;
			try{
				dialogueManager->getClient()->sendInput(imi::InputType::USER_NAME, "skype , M");
			}
			catch(...){
				std::cout<<"ERROR IN SENDINPUT!!! \n";
			}
		}
		if (aInput.sensorId == "KinectV2")
		{
			skype = false;
			int change = 0;
			int previousSize = users.size();
			while (users.size()<aInput.position.size())//Create new users
			{
				users.push_back(new User());
				change++;
				numberUsers++;
				writeNumUser();
				std::cout<<"Number of Users is: "<<numberUsers<<std::endl;
				dialogueManager->getClient()->sendNumUser(numberUsers);
			
			}

			while (users.size()>aInput.position.size())//delete users that are gone
			{
				User *temp = users.back();
				users.pop_back();
				delete temp;
				change++;
			}

			for (int i = 0; i < users.size(); i++)
			{
				users.at(i)->position = aInput.position.at(i).head;
			}
			if (change != 0)
			{
				for (int i = 0; i < 6; i++)
					faces[i].clear();
			}
#ifndef MEMORY
			if (users.size()==0)
				s1->handlerSpeech->sendNewString("");
#endif
		}

		if (aInput.sensorId == "KinectV2_Face")
		{
			if (aInput.faceStatus.size() != users.size())
			{//skip this frame
				mtx.unlock();
				//std::cout << "UNLOCKED \n";
				return activationLevel > threshold;
			}
			//std::cout << "Starting LOOP \n";
			for (int i = 0; i < users.size(); i++)
			{
				bool already = false;
				users.at(i)->name = aInput.faceStatus.at(i).Face_rec_ID;
				//std::cout << "Starting INNERLOOP "<<i<<"\n";
				for (int j = 0; j < greeted.size(); j++)
				{
					if (greeted.at(j) == users.at(i)->name)
					{
						already = true;
						break;
					}
					//std::cout << j << "\n";
				}
				if ((previousFace[i] == users.at(i)->name) && (users.at(i)->name != ""))
				{
					toGreet = i;
					faces[i].push_back(users.at(i)->name);
					if (!already)
					{
#ifdef MEMORY
						if (greeted.size() < 2)
#endif
							//cout <<"Face Recog sending: "<< users.at(i)->name << "\n";
							activationLevel += 20;
					}
				}
				if (users.at(i)->name != "")
				{
					previousFace[i] = users.at(i)->name;
				}
			}
			//std::cout << "Finished LOOP \n";
		}
	
		if (aInput.behaviorType == "LookAtSpeechSource")
		{
			//activationLevel = 0;
		}

		if (aInput.sensorId == "SpeakerIdentification")
		{
#ifndef MEMORY
			s1->handlerSpeech->sendNewString(aInput.userName);
#endif
			std::cout << "Penalty Given \n";
			activationLevel = -30;
			greeted.clear();
			greeted.push_back("aInput.userName");
			for (int i = 0; i < 6; i++)
				faces[i].clear();
		}

		if(aInput.behaviorType=="BB_Introduce" || aInput.behaviorType=="Answer" || aInput.sensorId=="SpeakerIdentification")
		{
			//activationLevel = 0;
		}

		if(aInput.behaviorType=="BB_Idle")
		{
			//std::cout<<"Penalty Given \n";
			activationLevel = -30;
			greeted.clear();
			//greeted.push_back("Unknown");
			for (int i = 0; i < 6;i++)
				faces[i].clear();

#ifdef MEMORY
			try{
				dialogueManager->getClient()->sendInput(imi::InputType::USER_NAME,"");
				global::focusingUser="";
			}
			catch(...){
				std::cout<<"ERROR IN SENDINPUT!!! \n";
			}
#endif
		}

		if (!activationLevel >= threshold)
		{
			active = false;
		}
		active = activationLevel >= threshold;
		mtx.unlock();
		//std::cout << "UNLOCKED \n";
		return active;
	}
	catch(...){
		printf("Error in ReactFace_updateActivationLevel");
		return false;
	}
	}

	perceptualInput BB_ReactFace::execute()
	{
		try{
		if (skype)
		{
			try{
				dialogueManager->getClient()->sendInput(imi::InputType::USER_NAME, "skype , M");
			}
			catch(...){
				std::cout<<"ERROR IN SENDINPUT!!! \n";
			}
			activationLevel = 0;
			return myOutput;
		}

		mtx.lock();
	//	std::cout << "EXECUTING FACE REACTION \n";
		if (toGreet >= users.size() || toGreet<0)
		{
			activationLevel = 0;
			mtx.unlock();
			return myOutput;
		}

		string temp = findName(toGreet);
#ifndef MEMORY
		if (temp != "Unknown" && !alreadyGreeted(temp))
#else
		if (!alreadyGreeted(temp))
#endif
		{			
			greeted.push_back(temp);
#ifdef GERMAN
				nadineClient->getClient()->speak("Ich glaube sie heissen " + temp, 10);
#else
#ifdef MEMORY
				
				if (skype){
					try{
						dialogueManager->getClient()->sendInput(imi::InputType::USER_NAME, "skype");
					}
					catch(...){
						std::cout<<"ERROR IN SENDINPUT!!! \n";
					}
				}
				else{
					try{
						dialogueManager->getClient()->sendInput(imi::InputType::USER_NAME, temp);
						global::focusingUser=temp;
					}
					catch(...){
						std::cout<<"ERROR IN SENDINPUT!!! \n";
					}
				}
#else
				s1->handlerSpeech->sendNewString(temp);
				nadineClient->getClient()->speak("I think that you are " + temp, 10);
#endif
				
#endif
/*
#ifndef MEMORY
			s1->handlerSpeech->sendNewString("");
#endif
*/
			
			//sleep(3000);
			//gaze(3000,toGreet);
		}
		faces[toGreet].clear();
		toGreet = -1;
		activationLevel = -30;
		mtx.unlock();
		//std::cout << "UNLOCKED_EXE \n";
		//sleep(3000);
		return myOutput;
	}
	
	catch(...){
		printf("Error in ReactFace_execute");
		return myOutput;
	}
	}


	void BB_ReactFace::stopExecution()
	{
		active = false;
	}


	string BB_ReactFace::findName(int index)
	{
		string temp = "";
		map <std::string, int> names;

		for (int i = 0; i < faces[index].size(); i++)
		{
			if (names.find(faces[index].at(i)) != names.end())
			{
				names.at(faces[index].at(i))++;
			}
			else
			{
				names.insert(std::pair<std::string, int>(faces[index].at(i), 1));
			}
		}

		std::map<std::string, int>::iterator res = names.begin();
		for (std::map<std::string, int>::iterator it = names.begin(); it != names.end(); ++it)
		{
			if (it->second > res->second)
				res = it;
		}
		
		return res->first;

	}

	bool BB_ReactFace::alreadyGreeted(std::string aName)
	{
		bool res = false;
		for (int i = 0; i < greeted.size(); i++)
		{
			if (greeted.at(i) == aName)
			{
				res = true;
				break;
			}
		}
		return res;
	}
	

	}