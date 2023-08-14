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
		emotion.x = 0;
		emotion.y = 0;
		emotion.z = 0;
		mood.x = 0;
		mood.y = 0;
		mood.z = 0;
		rm = 0;
		user_dialogue_choice = -20;

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
			activationLevel += 10;
			lastSentence = aInput.sentence;
			user_dialogue_choice = TransferSentence(lastSentence);
		}

		if (aInput.sensorId == "Feedback")
		{
			speaking = aInput.behaviorType == "Start";
		}

		if (aInput.sensorId == "Emotion")
		{
			emotion = aInput.emotion;
			mood = aInput.mood;
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
		int rm = rand() % 3;
		int response_animation = -1;
		imi::Animation::type a;
		float mood_max = mood.x;
		char auto_reply[500];
		if (-0.2 <= mood_max &&mood_max < 0){
			if (user_dialogue_choice == 1){
				// float rm=random(3);
				if (rm == 0)
				{
					strcpy_s(auto_reply, "<spurt audio=\"g0001_039\">cough cough</spurt> <voice emotion='cross'>I am sorry for that, I hope you can change your mind.</voice>");
					response_animation = 10;
				}
				else if (rm == 1)
				{
					strcpy_s(auto_reply, "<voice emotion='cross'> To be frankly, I can not agree with you.</voice>");
					response_animation = 18;
				}
				else
				{
					strcpy_s(auto_reply, "<spurt audio=\"g0001_050\">cough cough</spurt> <voice emotion='cross'> Ok, let us talk something else.</voice>");
					response_animation = 14; // PAPER
					// out<<"Current vh mood is: "<< mood.Position[0]<<" "<<mood.Position[1]<<" "<<mood.Position[2]<<endl;
				}
			}
			else{
				//	float rm=random(3);
				if (rm == 0)
					strcpy_s(auto_reply, "It sounds good.");
				else if (rm == 1)
					strcpy_s(auto_reply, "<spurt audio=\"g0001_013\">cough cough</spurt> I feel much better.");
				else
					strcpy_s(auto_reply, "<spurt audio=\"g0001_014\">cough cough</spurt>You are right.");
				response_animation = 9; // ROCK
				// out<<"Current vh mood is: "<< mood.Position[0]<<" "<<mood.Position[1]<<" "<<mood.Position[2]<<endl;
			}
		}
		else if (-0.4 <= mood_max && mood_max < -0.2){
			if (user_dialogue_choice == 1){
				// float rm=random(3);
				if (rm == 0)
					strcpy_s(auto_reply, "<voice emotion='cross'>I am unhappy, why you say such bad words to me!</voice>");
				else if (rm == 1)
					strcpy_s(auto_reply, "<voice emotion='cross'>What is wrong with you today.</voice>");
				else
					strcpy_s(auto_reply, "<voice emotion='cross'>I am a little angry.</voice>");

				response_animation = 13; // POINTING_YOU
				//  out<<"Current vh mood is: "<< mood.Position[0]<<" "<<mood.Position[1]<<" "<<mood.Position[2]<<endl;
			}
			else{
				//	float rm=random(3);
				if (rm == 0)
					strcpy_s(auto_reply, "Really? You think so?");
				else if (rm == 1)
					strcpy_s(auto_reply, "It sounds better.");
				else
					strcpy_s(auto_reply, "<spurt audio=\"g0001_010\">cough cough</spurt> Ok.");

				response_animation = 16; // OFFER
				// out<<"Current vh mood is: "<< mood.Position[0]<<" "<<mood.Position[1]<<" "<<mood.Position[2]<<endl;
			}
		}

		else if (-0.7 <= mood_max && mood_max < -0.4){
			if (user_dialogue_choice == 1){
				//float rm=random(3);
				if (rm == 0)
					strcpy_s(auto_reply, "<voice emotion='cross'>I am sad, I hope we can stop this conversation! </voice>");
				else if (rm == 1)
					strcpy_s(auto_reply, "<voice emotion='cross'>Stop here.</voice>");
				else
					strcpy_s(auto_reply, "<voice emotion='cross'>Let us stop.</voice>");

				response_animation = 14; // RIGHT_SWEEP
				// out<<"Current vh mood is: "<< mood.Position[0]<<" "<<mood.Position[1]<<" "<<mood.Position[2]<<endl;
			}
			else{
				//float rm=random(3);
				if (rm == 0)
					strcpy_s(auto_reply, "<spurt audio=\"g0001_050\">cough cough</spurt> stop kidding me");
				else if (rm == 1)
					strcpy_s(auto_reply, "Good words without deeds are rushes and reeds");
				else
					strcpy_s(auto_reply, "bla bla, you talk too much");

				response_animation = 14; // OFFER
				//out<<"Current vh mood is: "<< mood.Position[0]<<" "<<mood.Position[1]<<" "<<mood.Position[2]<<endl;
			}
		}
		else if (-1 <= mood_max && mood_max < -0.7){
			if (user_dialogue_choice == 1){
				//float rm=random(3);
				if (rm == 0)
					strcpy_s(auto_reply, "<voice emotion='cross'>How you dare to say so, get out of here!</voice>");
				else if (rm == 1)
					strcpy_s(auto_reply, "<voice emotion='cross'><spurt audio=\"g0001_038\">cough cough</spurt>go to hell!</voice>");
				else
					strcpy_s(auto_reply, "<voice emotion='cross'>Sorry, I have something else to do.</voice>");

				response_animation = 17; // RIGHT_STRONG_SWEEP
				// out<<"Current vh mood is: "<< mood.Position[0]<<" "<<mood.Position[1]<<" "<<mood.Position[2]<<endl;
			}
			else{
				//float rm=random(3);
				if (rm == 0)
					strcpy_s(auto_reply, "<voice emotion='cross'>Let us have no more of this, I do not believe you!</voice>");
				else if (rm == 1)
					strcpy_s(auto_reply, "<voice emotion='cross'>I cannot say the same for you</voice>");
				else
					strcpy_s(auto_reply, "<voice emotion='cross'> you are not.</voice>");

				response_animation = 18; // RIGHT_SWEEP

				// out<<"Current vh mood is: "<< mood.Position[0]<<" "<<mood.Position[1]<<" "<<mood.Position[2]<<endl;
			}
		}
		else if (0 <= mood_max && mood_max < 0.2){
			if (user_dialogue_choice == 1){
				//float rm=random(3);
				if (rm == 0)
					strcpy_s(auto_reply, "<spurt audio=\"g0001_014\">cough cough</spurt> It sounds bad!");
				else if (rm == 1)
					strcpy_s(auto_reply, "<spurt audio=\"g0001_020\">cough cough</spurt> Stop to say that");
				else
					strcpy_s(auto_reply, "<spurt audio=\"g0001_026\">cough cough</spurt>Sorry?");

				response_animation = 10; // PAPER
				// out<<"Current vh mood is: "<< mood.Position[0]<<" "<<mood.Position[1]<<" "<<mood.Position[2]<<endl;
			}
			else{
				//float rm=random(3);
				if (rm == 0)
					strcpy_s(auto_reply, "<voice emotion='happy'>Oh, thank you.</voice>");
				else if (rm == 1)
					strcpy_s(auto_reply, "<voice emotion='happy'>Really? Thank you</voice>");
				else
					strcpy_s(auto_reply, "<voice emotion='happy'>The same to you!</voice>");


				int randVar = rand() % 100;
				if (randVar < 33)
					response_animation = 12; // RIGHT_SWEEP
				else
				{
					if (randVar < 66)
						response_animation = 19;
					else
						response_animation = 16;
				}
				// out<<"Current vh mood is: "<< mood.Position[0]<<" "<<mood.Position[1]<<" "<<mood.Position[2]<<endl;
			}
		}
		else if (0.2 <= mood_max && mood_max < 0.4){
			if (user_dialogue_choice == 1){
				// float rm=random(3);
				if (rm == 0)
					strcpy_s(auto_reply, "Really? You think so? ");
				else if (rm == 1)
					strcpy_s(auto_reply, "Do not fool me");
				else
					strcpy_s(auto_reply, "Excuse me?");

				response_animation = 16; // OFFER
				// out<<"Current vh mood is: "<< mood.Position[0]<<" "<<mood.Position[1]<<" "<<mood.Position[2]<<endl;
			}
			else{
				//float rm=random(3);
				if (rm == 0)
					strcpy_s(auto_reply, "<voice emotion='happy'>You are so nice,my friend!</voice>");
				else if (rm == 1)
					strcpy_s(auto_reply, "<voice emotion='happy'>So nice you are!</voice>");
				else
					strcpy_s(auto_reply, "<voice emotion='happy'>You are so kind.</voice>");

				int randVar = rand() % 100;
				if (randVar < 33)
					response_animation = 12; // RIGHT_SWEEP
				else
				{
					if (randVar < 66)
						response_animation = 19;
					else
						response_animation = 16;
				}// POINTING_YOU
				// out<<"Current vh mood is: "<< mood.Position[0]<<" "<<mood.Position[1]<<" "<<mood.Position[2]<<endl;
			}
		}
		else if (0.4 <= mood_max && mood_max < 0.7){
			if (user_dialogue_choice == 1){
				//float rm=random(3);
				if (rm == 0)
					strcpy_s(auto_reply, "Do not say so, we are good friends!");
				else if (rm == 1)
					strcpy_s(auto_reply, "Good friend does not say such word");
				else
					strcpy_s(auto_reply, "Come on.");

				response_animation = 14; // WHY
				// out<<"Current vh mood is: "<< mood.Position[0]<<" "<<mood.Position[1]<<" "<<mood.Position[2]<<endl;
			}
			else{
				//float rm=random(3);
				int randVar = rand() % 100;
				if (randVar < 33)
					response_animation = 12; // RIGHT_SWEEP
				else
				{
					if (randVar < 66)
						response_animation = 19;
					else
						response_animation = 16;
				}//
				if (rm == 0)
					strcpy_s(auto_reply, "<voice emotion='happy'>I am happy, I am enjoying our conversation so much! <spurt audio=\"g0001_010\">cough cough</spurt></voice>");
				else if (rm == 1)
				{
					strcpy_s(auto_reply, "<voice emotion='happy'><spurt audio=\"g0001_037\">cough cough</spurt>It is my great pleasure to hear that. It makes me feel good.</voice>");
					response_animation = 19;
				}
				else
					strcpy_s(auto_reply, "<voice emotion='happy'><spurt audio=\"g0001_028\">cough cough</spurt>You are really kind. I really like you.</voice>");


				//  out<<"Current vh mood is: "<< mood.Position[0]<<" "<<mood.Position[1]<<" "<<mood.Position[2]<<endl;
			}
		}

		else {
			if (user_dialogue_choice == 1){
				//float rm=random(3);
				if (rm == 0)
					strcpy_s(auto_reply, "<voice emotion='calm'>But I like you, my friend!</voice>");
				else if (rm == 1)
					strcpy_s(auto_reply, "<voice emotion='calm'>Whatever you say, we are good friend!</voice>");
				else
					strcpy_s(auto_reply, "<voice emotion='calm'>Come on please.</voice>");

				response_animation = 100; // ROCK
				//  out<<"Current vh mood is: "<< mood.Position[0]<<" "<<mood.Position[1]<<" "<<mood.Position[2]<<endl;
			}
			else{
				//float rm=random(2);
				if (rm == 0)
					strcpy_s(auto_reply, "<voice emotion='happy'><spurt audio=\"g0001_025\">cough cough</spurt>I am so so happy. You are such a kind person</voice>");
				else
					strcpy_s(auto_reply, "<voice emotion='happy'> I have never felt so happy before <spurt audio=\"g0001_010\">cough cough</spurt></voice>");

				int randVar = rand() % 100;
				if (randVar < 33)
					response_animation = 12; // RIGHT_SWEEP
				else
				{
					if (randVar < 66)
						response_animation = 19;
					else
						response_animation = 16;
				}//
			}
		}

		if (user_dialogue_choice == 4){
			if (mood.x<-0.3){
				strcpy_s(auto_reply, "<voice emotion='cross'>Sorry, I am not really in the mood for talking</voice>");
			}
			else
			if (mood.x>0.3)
			{
				strcpy_s(auto_reply, "<voice emotion='happy'> Hi, nice to meet you! Shall we talk a bit </voice>");
				response_animation = 3;
			}
			else{
				strcpy_s(auto_reply, "Hi, nice to meet you!");
				response_animation = 3;
			}
		}


		else if (user_dialogue_choice == 5){
			if (mood.x<-0.3){
				strcpy_s(auto_reply, "<voice emotion='cross'> <spurt audio=\"g0001_009\">cough cough</spurt>You do not have to come back!</voice>");
			}
			else
			if (mood.x>0.3){
				strcpy_s(auto_reply, "<voice emotion='happy'> <spurt audio=\"g0001_010\">cough cough</spurt>I really hope we can talk again soon!");
			}
			else
				strcpy_s(auto_reply, "Ok, see you next time!");
			response_animation = 3;
			//user_speech_input=0;
			//glutDestroyWindow(main_window);
			//GLUI_Master.close_all();
			//exit(0);
		}
		else if (user_dialogue_choice == 6){
			strcpy_s(auto_reply, "Ok!");
			response_animation = 14;

			//user_speech_input=0;
		}

		//stimuli_input=false;
		user_dialogue_choice = 0;

		switch (response_animation){
		case 3: a = imi::Animation::WAVE_HAND; break;
			//case 9: a = imi::Animation::ROCK; break;
			//case 10: a = imi::Animation::PAPER; break;
		case 12: a = imi::Animation::RIGHT_SWEEP; break;
			//case 13: a = imi::Animation::POINTING_YOU; break;
		case 14: a = imi::Animation::WHY; break;
			//case 15: a = imi::Animation::HAND_ON_HIP; break;
		case 16: a = imi::Animation::OFFER; break;
			//case 17: a = imi::Animation::RIGHT_STRONG_SWEEP; break;
		case 18: a = imi::Animation::NOD_NO; break;
		case 19: a = imi::Animation::NOD_YES; break;
		default:a = imi::Animation::ROCK; break;
		}

		nadineClient->getClient()->playAnimation(a);
		nadineClient->getClient()->speak(auto_reply, 10);

		activationLevel = 0;
		mtx.unlock();

		perceptualInput temp1;
		return temp1;

	}




	int BB_Answer::TransferSentence(const std::string& phrase)
	{
		if (phrase == "I don't like to go with you" || phrase == "You look ugly today" || phrase == "You are so mean" ||
			phrase == "Your voice sounds Sleep(y" || phrase == "You are so boring" || phrase == "I don't like to talk with you")
			return 1;
		else if (phrase == "I like to go with you so much" || phrase == "You look pretty today" || phrase == "You are so nice" ||
			phrase == "Your voice sounds charming" || phrase == "You are so interesting" || phrase == "You are my best friend")
			return 2;
		else if (phrase == "What's your plan today" || phrase == "Shall we go swimming" || phrase == "would you like to go swimming" || phrase == "Let's go swimming")
			return 3;
		else if (phrase == "Hi Nadine")
			return 4;
		else if (phrase == "Goodbye")
			return 5;
		else if (phrase == "Let's talk about today's plan")
			return 6;
	}


}