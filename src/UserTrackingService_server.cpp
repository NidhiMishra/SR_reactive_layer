// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.
#pragma once
#include "UserTrackingService.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include "ThriftTools.hpp"
#include "ProtectedClient.h"
#include "AgentControl.h"
#include "Inputs_constants.h"
//#include "FaceService_server.cpp"
#include "reactiveLayer.h"
#include <string>
#include "Behavior.h"


using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

using namespace  ::imi;
using namespace ::BB;



class UserTrackingServiceHandler : virtual public UserTrackingServiceIf {
 public:
	 reactiveLayer *Decision;
	 int removeFrames;//Used for the kinect tracker that repeat the same frame...
	 int frame;
  UserTrackingServiceHandler(reactiveLayer * aDecision)
  {
	   Decision=aDecision;
	   removeFrames=0;
	   frame=0;
  }

  void userNew(const std::string& sensorID, const  ::imi::Microseconds timestamp, const int32_t userID) {
    // Your implementation goes here
	  Decision->userNew();
    printf("userNew\n");
  }

  void userLost(const std::string& sensorID, const  ::imi::Microseconds timestamp, const int32_t userID) {
    // Your implementation goes here
	  Decision->userLost();
    printf("userLost\n");
  }

#ifdef VH

  void userSkeletonChanged(const std::string& sensorID, const  ::imi::Microseconds timestamp, const int32_t userID, const  ::imi::Skeleton& position)
  {
	  float xyz[3];
	  //coordinateCorrection("Cyberglove", position.head.x,position.head.y, position.head.z, xyz);
	  
	  perceptualInput input;
	  input.sensorId="Kinect";
	  input.pos.x=position.head.x*100;
	  input.pos.y=position.head.y*100;
	  input.pos.z=position.head.z*100;
	  input.userID=userID;
	  Decision->updateActivationLevels(input);

// 	  if(userID==0)
// 		  std::cout<<"User 1 position is"<<position.head.x <<" "<<position.head.y<<" "<<position.head.z<<"\n";
// 	  if(userID==1)
// 			std::cout<<"User 2 position is"<<position.head.x <<" "<<position.head.y<<" "<<position.head.z<<"\n";
  }


#else
 void  userSkeletonChanged(const std::string& sensorID, const  ::imi::Microseconds timestamp, const int32_t userID, const  ::imi::Skeleton& position)
  {
 	  
// 	  coordinateCorrection("Cyberglove", position.head.x,position.head.y, position.head.z, xyz);
//  	  perceptualInput input;
// 	  input.sensorId="Kinect";
// 	  input.pos.x=xyz[0];
// 	  input.pos.y=xyz[1];
// 	  input.pos.z=xyz[2];
// 	  input.userID=userID;
// 	  Decision->updateActivationLevels(input);
// 
// 	  if(sensorID=="Kinect_v2")
// 
// 	  {
// 		  for(int i=0; i<position.size(); i++)
// 		  {
// 			  input.position.push_back(coordinateCorrection("Cyberglove", position.at(i).head.x,position.at(i).head.y, position.at(i).head.z));
// 		  }
// 	  }
// 	  Decision->updateActivationLevels(input);

  }


#endif


  void userImageChanged(const std::string& sensorID, const  ::imi::Microseconds timestamp, const int32_t userID, const  ::imi::Vec2& position) {
    // Your implementation goes here
    printf("userImageChanged\n");
  }
   
  void gestureStart(const std::string& sensorID, const  ::imi::Microseconds timestamp, const int32_t userID, const Gesture::type gesture,const  ::imi::Skeleton& position1) {
    // Your implementation goes here
    //printf("gestureStart\n");
	switch (gesture)
	{
		case 12:
			Decision->havingQuestionDetected(); 
			break;
		case 13:
			Decision->objectionDetected();
			break;
		case 14:
			Decision->praiseDetected();
			break;
		case 15:
			Decision->agreementDetected();
			break;
		case 16:
			Decision->confidentDetected();
			break;
		case 17:
			Decision->oncallDetected();
			break;
		case 18:
			Decision->stopDetected();
			break;
		case 19:
			Decision->successDetected();
			break;
		case 20:
			Decision->readDetected();
			break;
		case 21:
			Decision->writeDetected();
			break;
		case 22:
			Decision->drinkDetected();
			break; 
		case 4: //JASON
			imi::WorldPosition a;//=new imi::WorldPosition();
			float *xyz;//[3];
			//xyz=coordinateCorrection("Cyberglove",position1.wristr.x,position1.wristr.y,position1.wristr.z);
			a.x=position1.wristr.x;
			a.y=position1.wristr.y;
			a.z=position1.wristr.z;
			Decision->shakeDetected(a);
			break;
	}
  }

  void gestureStop(const std::string& sensorID, const  ::imi::Microseconds timestamp, const int32_t userID, const Gesture::type gesture) {
    // Your implementation goes here
    printf("gestureStop\n");
  }

  private:
	 /* float * coordinateCorrection(std::string senseorId, float xk,float yk, float zk,float *p)
	  {
		  float xyz[3];
		   p[0] = 0;
		   p[1] = 0;
		   p[2] = 0;
		   float xd;
		   float yd;
		   float zd;
		   float theta1 = 5*3.14/180;
		   float theta2=15*3.14/180;

		   xd = 1.83;// should be changed by the positions of the kinect based on the world coordinate
		   yd = 1.7;//-0.4;
		   zd = 1.1;//-0.8;
			if(senseorId=="Cyberglove")
			{
				//xyz[0] = x + xd;
				//xyz[1] = y*sin(theta) + z*cos(theta) + yd;
				//xyz[2] = -y*cos(theta) +z*sin(theta) + zd;

				p[0] = xk*sin(theta1) - yk*cos(theta1)*sin(theta2) + zk*cos(theta1)*cos(theta2) + xd-1.75;
				p[1] = xk*cos(theta1) + yk*sin(theta1)*sin(theta2) - zk*sin(theta1)*cos(theta2) + yd+0.6;
				p[2] = +yk*cos(theta2) + zk*sin(theta2) + zd;
			}
			//printf("CORRECTED :%f %f %f A \n",p[0],p[1],p[2]);
			return xyz;
	  }*/



	  imi::Skeleton coordinateCorrection(std::string senseorId, float xk,float yk, float zk)
	  {
		  imi::Skeleton result;
		  float xyz[3];
// 		   p[0] = 0;
// 		   p[1] = 0;
// 		   p[2] = 0;
		   float xd;
		   float yd;
		   float zd;
		   float theta1 = 5*3.14/180;
		   float theta2=10*3.14/180;

		   //xd = 0.75;// should be changed by the positions of the kinect based on the world coordinate
		   //yd = 2.45;//-0.4;
		   //zd = 0.75;//-0.8;
			if(senseorId=="Cyberglove")
			{
				//xyz[0] = x + xd;
				//xyz[1] = y*sin(theta) + z*cos(theta) + yd;
				//xyz[2] = -y*cos(theta) +z*sin(theta) + zd;

				xyz[0] = -xk*sin(theta1) + yk*cos(theta1)*sin(theta2) + zk*cos(theta1)*cos(theta2);
				xyz[1] = xk*cos(theta1) + yk*sin(theta1)*sin(theta2) + zk*sin(theta1)*cos(theta2) + 1.7;
				xyz[2] = yk*cos(theta2) - zk*sin(theta2) + 1.83;
				//p[0] = zk +0;
				//p[1] = xk + 1.7;
				//p[2] = yk + 1.83;
			}
 			//printf("user positon in the kinect :%f %f %f A \n",xk,yk,zk);
			//printf("user position in the world :%f %f %f A \n",p[0],p[1],p[2]);
			result.head.x=xyz[0];
			result.head.y=xyz[1];
			result.head.z=xyz[2];
			return result;
	  }


};
