#include "Server_Starter.h"

serverStarter::serverStarter()
		{

		}

void serverStarter::gestureServer(reactiveLayer *aDecision)//Start a server for the Kinect
		{
			int port1 = imi::g_Inputs_constants.DEFAULT_USERTRACKING_SERVICE_PORT;//Set the port that the server will listen to.
			boost::shared_ptr<UserTrackingServiceHandler> handler(new UserTrackingServiceHandler(aDecision));//initiate the server object
			imi::createServer<UserTrackingServiceHandler, UserTrackingServiceProcessor>(handler, port1);//start to serve with a multi threaded server
		}
void serverStarter::faceServer(reactiveLayer *aDecision)
		{
			int port2 = imi::g_Inputs_constants.DEFAULT_FACE_SERVICE_PORT;
			boost::shared_ptr<FaceServiceHandler> handler(new FaceServiceHandler(aDecision));
			imi::createServer<FaceServiceHandler, FaceServiceProcessor>(handler, port2);
		}

void serverStarter::objectServer(reactiveLayer *aDecision)
		{
			int port2 = imi::g_Inputs_constants.DEFAULT_OBJECT_SERVICE_PORT;
			boost::shared_ptr<ObjectServiceHandler> handler(new ObjectServiceHandler(aDecision));
			imi::createServer<ObjectServiceHandler, ObjectDetectionServiceProcessor>(handler, port2);
		}

void serverStarter::actionServer(reactiveLayer *aDecision)
		{
			int port2 = imi::g_Inputs_constants.DEFAULT_ACTION_RECOGNITION_SERVICE_PORT;
			boost::shared_ptr<ActionRecognitionServiceHandler> handler(new ActionRecognitionServiceHandler(aDecision));
			imi::createServer<ActionRecognitionServiceHandler, ActionRecognitionServiceProcessor>(handler, port2);
		}

void serverStarter::soundServer(reactiveLayer *aDecision)
		{
			int port3 = imi::g_Inputs_constants.DEFAULT_SOUND_SERVICE_PORT;
			boost::shared_ptr<SoundServiceHandler> handler(new SoundServiceHandler(aDecision));
			imi::createServer<SoundServiceHandler, SoundServiceProcessor>(handler, port3);
		}
void serverStarter::speechServer(reactiveLayer *aDecision)
		{
			int port3 = imi::g_Inputs_constants.DEFAULT_SPEECHRECOGNITION_SERVICE_PORT;
			boost::shared_ptr<SpeechRecognitionServiceHandler> handler(new SpeechRecognitionServiceHandler(aDecision));
			imi::createServer<SpeechRecognitionServiceHandler, SpeechRecognitionServiceProcessor>(handler, port3);
		}


void serverStarter::feedbackServer(/*reactiveLayer *aDecision*/)
		{
			int port3 = imi::g_Feedback_constants.DEFAULT_AGENT_FEEDBACK_SERVICE_PORT;
			feedbackHandler = (boost::shared_ptr<AgentFeedbackServiceHandler>)new AgentFeedbackServiceHandler(/*aDecision*/);
			imi::createServer<AgentFeedbackServiceHandler, AgentFeedbackServiceProcessor>(feedbackHandler, port3);
		}

void serverStarter::emotionServer(reactiveLayer *aDecision)
		{
			int port = imi::g_Inputs_constants.DEFAULT_EMOTIONENGINE_SERVICE_PORT;
			boost::shared_ptr<EmotionEngineServiceHandler>handler(new EmotionEngineServiceHandler(aDecision));
			imi::createServer<EmotionEngineServiceHandler, EmotionEngineServiceProcessor>(handler, port);
		}

void serverStarter::newKinectServer(reactiveLayer *aDecision)
		{
			int port = imi::g_Inputs_constants.DEFAULT_USERTRACKING_KINECT2_SERVICE_PORT;
			boost::shared_ptr<UserTrackingServiceNewKinectHandler>handler(new UserTrackingServiceNewKinectHandler(aDecision));
			imi::createServer<UserTrackingServiceNewKinectHandler, UserTrackingServiceNewKinectProcessor>(handler, port);
		}
void serverStarter::newspeechServer(/*reactiveLayer *aDecision*/)
		{
			int port3 = imi::g_Inputs_constants.DEFAULT_SPEECHRECOGNITION_SERVICE_PORT_NEW;
			handlerSpeech = (boost::shared_ptr<NewSpeechRecognitionServiceHandler>)(new NewSpeechRecognitionServiceHandler());
			imi::createServer<NewSpeechRecognitionServiceHandler, NewSpeechRecognitionServiceProcessor>(handlerSpeech, port3);
		}
void serverStarter::SepakerIdServer(reactiveLayer *aDecision)
{
	int port = imi::g_Inputs_constants.DEFAULT_SPEAKERIDENTIFICATION_PORT;
	boost::shared_ptr<SpeakerIdentificationHandler>handler(new SpeakerIdentificationHandler(aDecision));
	imi::createServer<SpeakerIdentificationHandler, SpeakerIdentificationProcessor>(handler, port);
}

void serverStarter::newfacebookServer()
{
	int port = imi::g_SocialNetwork_constants.DEFAULT_FACEBOOK_SERVICE_PORT;
	handlerFaceBook = (boost::shared_ptr<FacebookHandler>)(new FacebookHandler());
	imi::createServer<FacebookHandler, FacebookProcessor>(handlerFaceBook, port);
}