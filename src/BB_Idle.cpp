#include <iostream>
#include "BB_Idle.h"
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

	BB_Idle::BB_Idle(ProtectedClient<imi::AgentControlClient> *aClient)
	{
		threshold = 15;
		activationLevel = 15;
		myBBType = "BB_Idle";
		nadineClient = aClient;
		executing = false;
		users.resize(6);
		previousSize=0;
		disable = false;
	}


	BB_Idle::~BB_Idle()
	{
	}


	bool BB_Idle::updateActivationLevel(perceptualInput aInput)
	{
		mtx.lock();
		if (aInput.sensorId == "Skype")
		{
			disable = true;
		}
		if (aInput.sensorId == "Demo")
		{
			disable = false;
		}
		if (aInput.sensorId == "TimePassed")
		{
				if(users.size()==0)
				{
					activationLevel+=6;
				}
				else //Something interesting going on
				{
					activationLevel=-10;
				}
			previousSize=users.size();
		}
		
		if(aInput.behaviorType=="BB_Introduce" || aInput.behaviorType=="Answer")
		{//robot should no go idle
			if(activationLevel>0)
				activationLevel = -10;
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
				users.at(i)->position = aInput.position.at(i).head;
			}
		}




		mtx.unlock();
		return activationLevel >= threshold;
	}

	perceptualInput BB_Idle::execute()
	{
		//skype
		perceptualInput temp;
		if (disable)
		{
			activationLevel = 0;
			return temp;
		}

		mtx.lock();
		int randVal=0;

		activationLevel = 0;
#ifndef VH
		nadineClient->getClient()->playAnimation(imi::Animation::RELEASE_OBJECT);
#else
		nadineClient->getClient()->touchTarget("READING");
		randVal=rand()%100+2;
		std::string Result;          // string which will contain the result
		std::ostringstream convert;   // stream used for the conversion
		convert << randVal;      // insert the textual representation of 'Number' in the characters in the stream
		Result = convert.str(); // set 'Result' to the contents of the stream

		if(randVal<5)
			nadineClient->getClient()->touchTarget("READING0" + Result);


#endif

		mtx.unlock();

	//	perceptualInput temp;
		temp.behaviorType=myBBType;
		return temp;

	}


}