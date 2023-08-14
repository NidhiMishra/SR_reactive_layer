#pragma once
#include <iostream>
#include "BB_ReactAction.h"
#include "AgentControl.h"
#include "ProtectedClient.h"
#include "Inputs_types.h"
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include "global.h"

using namespace ::std;

namespace BB
{
	BB_ReactAction::BB_ReactAction(ProtectedClient<imi::AgentControlClient> *aClient)
	{
		activationLevel = -30;
		threshold = 30;
		nadineClient = aClient;
		active = false;
		myBBType = "ReactAction";//TO DO: Remove these variable
		myOutput.behaviorType = myBBType;
		executing = false;
		skype = false;
		disable = false;
		users.resize(6);//Maximum number of users with Kinect V2
		hasNew=false;
		
	}

	BB_ReactAction::~BB_ReactAction()
	{
		while (users.size() > 0)//delete users that are gone
		{
			User *temp = users.back();
			users.pop_back();
			delete temp;
		}
	}


	bool BB_ReactAction::updateActivationLevel(perceptualInput aInput)
	{
		mtx.lock();
		//std::cout << "LOCKED \n";

		if(aInput.behaviorType=="BB_Idle")
		{
			//std::cout<<"Penalty Given \n";
			activationLevel = -50;
			
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
		}

		if (aInput.sensorId == "KinectV2_Face")
		{
			if (aInput.faceStatus.size() != users.size())
			{//skip this frame
				mtx.unlock();
				return activationLevel >= threshold;
			}
			for (int i = 0; i < users.size(); i++)
			{
				users.at(i)->name = aInput.faceStatus.at(i).Face_rec_ID;
			}
		}

		if (aInput.sensorId == "KinectForAction") // if action inputs are detected
		{
			if (aInput.detectedAction.size()!=users.size()) 
			{
				//skip this frame
				mtx.unlock();
				return activationLevel >= threshold;
			}
			if (!hasNew){ // update actions only after current actions have been reported.
				checkDifferentActions(aInput.detectedAction);
			}
			if (hasNew){
				activationLevel += 10;
			}

		}

		
		active = activationLevel >= threshold;
		mtx.unlock();
		//std::cout << "UNLOCKED \n";
		return active;
	}

	perceptualInput BB_ReactAction::execute()
	{

		mtx.lock();
		/*
		if (disable)
		{	

			perceptualInput temp;
			activationLevel = -30;
			mtx.unlock();
			return temp;
		}
		*/

		// YOUR IMPLEMENTATION IS HERE
		if (hasNew){
			ReportAction();
			// clear variables
			hasNew=false;
			userNames.clear();
			userActionToReport.clear();
		}
		
		activationLevel = -30;
		mtx.unlock();
		
		return myOutput;
	}


	void BB_ReactAction::stopExecution()
	{
		active = false;
	}


	void BB_ReactAction::checkDifferentActions(std::vector<imi::UserAction> inputActions){
		for (int i = 0; i < inputActions.size(); i++){
			imi::UserAction inputAct=inputActions.at(i);
			std::vector<ActionType::type> user_actions=users.at(i)->actions.actions;
			imi::UserAction new_actions;
			for (int j=0; j< inputAct.actions.size();j++){
				imi::ActionType::type input_act=inputAct.actions.at(j);
				bool newFlag=true; // check if input actions are new.
				for (int k=0; k< user_actions.size(); k++){
					if (input_act == user_actions.at(k)){
						newFlag=false;
						break;
					}
				}
				if (newFlag){ // add new actions to report
					new_actions.actions.push_back(input_act);
					hasNew=true;
				}
			}
			if (new_actions.actions.size()>0){ // if has new actions
				std::string name=users.at(i)->name;
				if (name=="" || name == "Unknown"){
					name="Someone";
				}
				userNames.push_back(name);
				userActionToReport.push_back(new_actions);
			}
			users.at(i)->actions = inputAct; // update user actions

		}
	}
	
	void BB_ReactAction::ReportAction() {
		// Report Current Action
		std::stringstream sToSend;
		sToSend << "I see ";
		for (int i=0; i<userActionToReport.size(); i++){
			if (userNames.at(i) == global::focusingUser){
				sToSend << "you are ";
			}
			else{
				sToSend << userNames.at(i) << "is ";
			}
			std::vector <ActionType::type> mActions=userActionToReport.at(i).actions;
			std::vector <std::string> mActionNames;
			for (int j=0; j<mActions.size();j++){
				std::string mName= getActionName(mActions.at(i));
				if (mName!=""){ // remove undefined actions or walking
					mActionNames.push_back(mName);
				}

			}
			for (int j=0; j<mActionNames.size();j++){
				std::string actionName=mActionNames.at(i);
				if (mActions.size()==1){
					sToSend << actionName;
				}
				else if (j==mActions.size()-1){ // last action
					sToSend << "and " << actionName;
				}
				else{
					sToSend << actionName << ", ";
				}
			}
			sToSend << ". ";	
		}
		std::string toReport = sToSend.str(); 
		// speak out
		nadineClient->getClient()->speak(toReport, 10);
		
	}

	std::string BB_ReactAction::getActionName(const ActionType::type action){
		switch (action)
		{
			case imi::ActionType::DRINKING:
			{
				return "drinking";
			}

			case imi::ActionType::ANSWER_PHONE:
			{
				return "answering a phone call";
			}

			case imi::ActionType::CHECK_WATCH:
			{
				return "looking at watch or phone";
			}

			case imi::ActionType::EATING:
			{
				return "eating";
			}

			case imi::ActionType::GIVING:
			{
				return "giving something to me";
			}

			case imi::ActionType::HAND_SHAKE:
			{
				return "shaking hand with me";
			}

			case imi::ActionType::TAKE_PHOTO:
			{
				return "taking photos";
			}

			case imi::ActionType::WALKING:
			{
				return "";
			}
		
			default:
			{
				printf("Error: Action Undefined\n");
				return "";
			}
		}

	}
	
	

}