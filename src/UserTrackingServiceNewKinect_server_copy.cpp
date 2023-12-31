// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "UserTrackingServiceNewKinect.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include "reactiveLayer.h"
#include <string>
#include "Behavior.h"
#include <iostream>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

using namespace  ::imi;
using namespace ::BB;

class UserTrackingServiceNewKinectHandler : virtual public UserTrackingServiceNewKinectIf {
 public:
	 reactiveLayer *Decision;
	 int frameNum;
  UserTrackingServiceNewKinectHandler() {
    // Your initialization goes here
  }

  UserTrackingServiceNewKinectHandler(reactiveLayer * aDecision)
  {
	  Decision=aDecision;
	  frameNum = 0;
  }

  void userNew(const std::string& sensorID, const  ::imi::Microseconds timestamp, const int32_t userID) {
    // Your implementation goes here
    printf("userNew\n");
  }

  void userLost(const std::string& sensorID, const  ::imi::Microseconds timestamp, const int32_t userID) {
    // Your implementation goes here
    printf("userLost\n"); 
  }

  void MultiUserSkeletonChanged(const std::string& sensorID, const  ::imi::Microseconds timestamp, const  ::imi::skel& skelposition) {
    // Your implementation goes here
	
	if(frameNum>200)
		frameNum=0;

	if(frameNum++%2==0)
		return;

	perceptualInput input;
	input.sensorId="KinectV2";

	if(sensorID=="KinectV2")
	{
		for(int i=0; i<skelposition.size(); i++)
		{
			if (skelposition.at(i).head.y != 0)
			{
				//input.position.push_back(coordinateCorrection("Cyberglove", skelposition.at(i).head.x,skelposition.at(i).head.y, skelposition.at(i).head.z));
				std::cout << "Coordinate Received: " << skelposition.at(i).head.x <<" "<< skelposition.at(i).head.y << " "<<skelposition.at(i).head.z << "\n";
				input.position.push_back(skelposition.at(i));
			}
		}
	}

	Decision->updateActivationLevels(input);
  }

  void MultiUserFaceFeatures(const std::string& sensorID, const  ::imi::Microseconds timestamp, const  ::imi::face& faceposition)
  {
	  perceptualInput input;
	  input.sensorId="KinectV2_Face";

	  for(int i=0;i<faceposition.size();i++)
	  {
			input.faceStatus.push_back(faceposition.at(i));
		 // std::cout << "I think user "<<i<<" is: "<<faceposition.at(i).Face_rec_ID<<"\n";
	  }
	 // std::cout << "\n";
	 Decision->updateActivationLevels(input);
  }

  void userImageChanged(const std::string& sensorID, const  ::imi::Microseconds timestamp, const int32_t userID, const  ::imi::Vec2& position) {
    // Your implementation goes here
    printf("userImageChanged\n");
  }

  void gestureStart(const std::string& sensorID, const  ::imi::Microseconds timestamp, const  ::imi::ges& gesposition)
  {
    // Your implementation goes here
    perceptualInput input; 
	input.sensorId="KinectV2_Gesture";

	for (int i = 0; i < gesposition.size(); i++)
	{
		input.gesture.push_back(gesposition.at(i));
		if (input.gesture.at(i).Right_wavehand == imi::GesPossibilities::YES)
			std::cout << "GESTURE DETECTED \n";
	}

	 Decision->updateActivationLevels(input);
  }

  void gestureStop(const std::string& sensorID, const  ::imi::Microseconds timestamp, const int32_t userID, const Gesture::type gesture) {
    // Your implementation goes here
    printf("gestureStop\n");
  }

  void gestureHandsShakingStart(const std::string& sensorID, const  ::imi::Microseconds timestamp, const  ::imi::gesHandShake& gesHandposition) {
	  // Your implementation goes here
	  perceptualInput input;
	  input.sensorId = "KinectV2_Gesture_Shake";
	  for (int i = 0; i < gesHandposition.size(); i++)
	  {
		  input.shake.push_back(gesHandposition.at(i));
		  if (gesHandposition.at(i).Right_HandShake == imi::GesPossibilities::YES)
			  printf("gestureHandsShakingStart\n");
	  }
	  Decision->updateActivationLevels(input);
  }


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


	  //temporarily override
	  result.head.x=xk;
	  result.head.y=yk;
	  result.head.z=zk;

	  return result;
  }


};


