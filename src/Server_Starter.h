#ifndef SERVER_STARTER
#define SERVER_STARTER
#include "ObjectService_server.h"
#include "UserTrackingService.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include "ThriftTools.hpp"
#include "ProtectedClient.h"
#include "AgentControl.h"
#include "Inputs_constants.h"
#include "SocialNetwork_constants.h"
#include "Feedback_constants.h"
#include "FaceService_server.h"
#include "UserTrackingService_server.cpp"
#include "I2P_types.h"
#include "FaceService.h"
#include "reactiveLayer.h"
#include "SoundService.h"
#include "SoundService_server.cpp"
#include "Facebook.h"
#include "PictureService.h"
#include "AgentFeedBackService_server.cpp"
#include "SpeechRecognitionService.h"
#include "SpeechRecognitionService_server.cpp"
#include "EmotionEngineService.h"
#include "EmotionEngineService_server.cpp"
#include "UserTrackingServiceNewKinect.h"
#include "UserTrackingServiceNewKinect_server.cpp"
#include "NewSpeechRecognitionService.h"
#include "NewSpeechRecognitionService_server.cpp"
#include "SpeakerIdentification.h"
#include "SpeakerIdentification_server.cpp"
#include "Facebook_server.cpp"
#include "ObjectDetectionService.h"
#include "ObjectService_server.h"
#include "ActionRecognitionService.h"
#include "ActionRecognitionService_server.h"


class serverStarter
{
	public:
		boost::shared_ptr<AgentFeedbackServiceHandler> feedbackHandler;
		boost::shared_ptr<NewSpeechRecognitionServiceHandler> handlerSpeech;
		boost::shared_ptr<FacebookHandler> handlerFaceBook;

		serverStarter();
		void gestureServer(reactiveLayer *aDecision);//Start a server for the Kinect
		void faceServer(reactiveLayer *aDecision);
		void objectServer(reactiveLayer *aDecision);
		void actionServer(reactiveLayer *aDecision);
		void soundServer(reactiveLayer *aDecision);
		void speechServer(reactiveLayer *aDecision);
		void feedbackServer(/*reactiveLayer *aDecision*/);
		void emotionServer(reactiveLayer *aDecision);
		void newKinectServer(reactiveLayer *aDecision);
		void newspeechServer(/*reactiveLayer *aDecision*/);
		void SepakerIdServer(reactiveLayer *aDecision);
		void newfacebookServer(/*reactiveLayer *aDecision*/);
};

#endif