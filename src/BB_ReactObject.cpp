#pragma once
#include <iostream>
#include "BB_ReactObject.h"
#include "AgentControl.h"
#include "ProtectedClient.h"
#include "Inputs_types.h"
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

using namespace ::std;

namespace BB
{
	BB_ReactObject::BB_ReactObject(ProtectedClient<imi::AgentControlClient> *aClient)
	{
		activationLevel = -30;
		threshold = 30;
		nadineClient = aClient;
		active = false;
		myBBType = "ReactObject";//TO DO: Remove these variable
		myOutput.behaviorType = myBBType;
		executing = false;
		skype = false;
		hasNew = false;
		talkAll = false;
		disable = false;
		permanentBlockedLabels[0]= "person";
		permanentBlockedLabels[1]= "sink";
	}

	BB_ReactObject::~BB_ReactObject()
	{

	}


	bool BB_ReactObject::updateActivationLevel(perceptualInput aInput)
	{
		mtx.lock();
		//std::cout << "LOCKED \n";

		if(aInput.behaviorType=="BB_Idle")
		{
			//std::cout<<"Penalty Given \n";
			activationLevel = -50;
			
		}

		if (aInput.sensorId == "Speech" && aInput.sentence == "REPORTOBJ")
		{
			activationLevel += 100;
			talkAll = true;
			
		}

		if (aInput.sensorId == "KinectForObject") // if object inputs happen
		{
			updateObjects(aInput.detectedObjects);
			if (hasNew){
				printf ("Update Obj Activation\n");
				activationLevel += 10;
			}
		}

		
		active = activationLevel >= threshold;
		mtx.unlock();
		//std::cout << "UNLOCKED \n";
		return active;
	}

	perceptualInput BB_ReactObject::execute()
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
		if (talkAll){
			printf("Report all objects!!!\n");
			reportAll(current_objects);
			talkAll=false;
			hasNew=false;
		}
		if (hasNew){
			reportNew(current_objects);
			hasNew=false;
		}

		activationLevel = -30;
		mtx.unlock();
		
		return myOutput;
	}


	void BB_ReactObject::stopExecution()
	{
		active = false;
	}

	void BB_ReactObject::initializeChecking(){
		for (vector<::imi::ObjectInfo>::iterator current_obj = current_objects.begin();
			current_obj != current_objects.end(); ++current_obj){
			current_obj->checked = false;
		}
	}

	void BB_ReactObject::updateNew(vector<::imi::ObjectInfo> & detectedObjects){
		for(vector<::imi::ObjectInfo>::iterator detectedObj = detectedObjects.begin();
		detectedObj != detectedObjects.end(); ++detectedObj){
			bool isNewFlag = true;
			for (vector<::imi::ObjectInfo>::iterator current_obj = current_objects.begin();
				current_obj != current_objects.end(); ++current_obj){
				// find if any in unchecked objects.
				if (!current_obj->checked && detectedObj->label == current_obj->label){
					// update matched objects
					current_obj->checked = true;
					current_obj->__set_coordinate(detectedObj->coordinate);
					current_obj->__set_prob(detectedObj->prob);
					isNewFlag = false;
					break;
				}
			}
			// add new object to current objects
			if (isNewFlag){
				::imi::ObjectInfo newObj;
				copyObj(newObj, *detectedObj);
				newObj.__set_checked(true);
				newObj.__set_isNew(false);
				if (!checkBlockedLabel(newObj.label)){
					newObj.__set_isNew(true);
					hasNew = true;
				}
				current_objects.push_back(newObj);
			}
		}
	}

	void BB_ReactObject::removeUncheckedObjects(vector<::imi::ObjectInfo> & Objects){
		for (vector<::imi::ObjectInfo>::iterator obj = Objects.begin();
			obj != Objects.end();){
			if (!obj->checked){
				obj=Objects.erase(obj); // obj refers to the next one already
			}
			else{
				++obj;
			}
		}
		
	}

	void BB_ReactObject::copyObj(::imi::ObjectInfo & newObj, ::imi::ObjectInfo & oldObj){
		newObj.__set_checked(oldObj.checked);
		newObj.__set_coordinate(oldObj.coordinate);
		newObj.__set_duplicated(oldObj.duplicated);
		newObj.__set_isNew(oldObj.isNew);
		newObj.__set_label(oldObj.label);
		newObj.__set_prob(oldObj.prob);
	}

	void BB_ReactObject::updateObjects(vector<::imi::ObjectInfo> & detectedObjects){
		initializeChecking();
		setFlexibleBlockedLabels();
		updateNew(detectedObjects);
		removeUncheckedObjects(current_objects);
		updateDuplication(current_objects);
	}

	/*
	void BB_ReactObject::updateDuplication(vector<::imi::ObjectInfo> & detectedObjects){
		detectedObjects.begin()->duplicated = false; // set the first one
		for (std::vector<::imi::ObjectInfo>::iterator obj = ++detectedObjects.begin();
			obj != detectedObjects.end(); ++obj) {
				obj->duplicated = false; // by default
				std::vector<::imi::ObjectInfo>::iterator mobj=obj; // check from obj to all objects before that position
				do {
					--mobj;
					if (obj != mobj && obj->label == mobj->label) {
						obj->duplicated = true;
						break;
					}
				}while (mobj!=detectedObjects.begin());
		}
	}*/

	
	void BB_ReactObject::updateDuplication(vector<::imi::ObjectInfo> & detectedObjects){
		const size_t numObj=detectedObjects.size();
		for (size_t idx=0; idx!= numObj; idx++) {
			::imi::ObjectInfo obj=detectedObjects.at(idx);
			if (idx==0){// set the first one
				obj.duplicated=false;
			}
			else{
				for (size_t j=idx-1; j>-1; j--){// check from obj to all objects before that position
					::imi::ObjectInfo mobj=detectedObjects.at(j);
					if (obj.label == mobj.label) {
						obj.duplicated = true;
						break;
					}
				}
			}
		}
	}
	
	/*
	void BB_ReactObject::updateDuplication(vector<::imi::ObjectInfo> & detectedObjects){
		const size_t numObj=detectedObjects.size();
		for (std::vector<::imi::ObjectInfo>::iterator obj = ++detectedObjects.begin();
			obj != detectedObjects.end(); ++obj) {
			for (std::vector<::imi::ObjectInfo>::iterator mobj = obj;
			mobj != detectedObjects.end(); ++mobj){
				if(!obj->duplicated && obj!=mobj && obj->label==mobj->label){
					obj->duplicated=true;
					mobj->duplicated=true;
					break;
				}
			}
		}
	}
	*/


	void BB_ReactObject::reportNew(vector<::imi::ObjectInfo> & current_objects){
		
		for (vector<::imi::ObjectInfo>::iterator current_obj = current_objects.begin();
			current_obj != current_objects.end(); ++current_obj){
			if (current_obj->isNew){
				if (current_obj->duplicated){
					string toSend="I see another " + current_obj->label;
					nadineClient->getClient()->speak(toSend, 10);
					printf("Report new objects!!!\n");
					std::cout<<toSend<<std::endl;
					std::cout<<"Probability: "<<current_obj->prob<<std::endl;
					sleep(2000);
				}
				else{
					string toSend="I see a " + current_obj->label;
					nadineClient->getClient()->speak("I see a " + current_obj->label, 10);
					printf("Report new objects!!!\n");
					std::cout<<toSend<<std::endl;
					std::cout<<"Probability: "<<current_obj->prob<<std::endl;
					sleep(2000);
				}
				current_obj->isNew = false;
			}
		}
		
	}


	bool BB_ReactObject::checkBlockedLabel(std::string label){
		bool flag = false;
		// check permanent blocked list
		const size_t size = 5;
		for (size_t ix = 0; ix != size; ++ix){
			if (label == permanentBlockedLabels[ix]){
				flag = true;
				break;
			}
		}
		// check flexible blocked list
		map<std::string, int>::iterator it = flexibleBlockedLabels.find(label);
		if (it!=flexibleBlockedLabels.end()){
			if (it->second > 0){
				flag = true;
				it->second-=1;
			}
		}
		
		return flag;

	}

	void BB_ReactObject::reportAll(vector<::imi::ObjectInfo> & Objects){
		map<std::string, int> objStat;
		for (vector<::imi::ObjectInfo>::iterator obj = Objects.begin(); obj != Objects.end(); ++obj){
			map<std::string, int>::iterator it = objStat.find(obj->label);
			if (it != objStat.end()){
				it->second+=1; // if found, increase the count
			}
			else{
				objStat[obj->label] = 1; // if not found, get one
			}
			obj->isNew = false;
		}

		std::string toReport="I see";
		for (map<std::string, int>::iterator iter = objStat.begin(); iter != objStat.end(); ++iter){
			if(iter->first=="person"){
				iter->second-=1;
				if (iter->second<=0){
					continue;
				}
			}
			
			stringstream convert;
			convert << iter->second;
			string numStr = convert.str();
			if (iter != objStat.end()){
				toReport = toReport + " " + numStr + " " + iter->first + ", ";
			}
			else{
				toReport = toReport + " and " + numStr + " " + iter->first + ".";
			}
		}
		// speak out
		nadineClient->getClient()->speak(toReport, 10);
	}

	void BB_ReactObject::setFlexibleBlockedLabels(){
		flexibleBlockedLabels["tv"]=2;
		flexibleBlockedLabels["chair"]=1;
		flexibleBlockedLabels["laptop"]=2;
	}

	
	

}