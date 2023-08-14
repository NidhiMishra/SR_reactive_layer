#####################################################################
# i2p_applications cmake
#####################################################################

cmake_minimum_required(VERSION 2.6)

set(PROJECT_NAME "i2p_reactive_layer")
project(${PROJECT_NAME})

#setup where are the modules: for findKinectSDK.cmake
set(CMAKE_MODULE_PATH $ENV{I2P_MODULE}/i2pCmake)

if (WIN32)
    set(BOOST_ROOT "$ENV{IMI_LIBRARIES}/boost_1_49_0")
endif()

#dependencies for this project
#i2p: thrift boost libevent
include($ENV{I2P_MODULE}/i2pCmake/i2p.cmake)


FIND_PACKAGE(Boost REQUIRED)
#FIND_PACKAGE(KinectSDK REQUIRED)

#####################################################################
# Thrift
#####################################################################
FIND_PACKAGE(Thrift REQUIRED)
IF( NOT THRIFT_FOUND )
	message("Failed to find thrift!")
ELSE()
	include_directories(${THRIFT_INCLUDE_DIR})
ENDIF()

#####################################################################
# Generate Thrift stubs
#####################################################################
include_directories($ENV{I2P_MODULE}/i2pThrift/gen-cpp)
GENERATE_STUBS(I2P.thrift cpp)
GENERATE_STUBS(Control.thrift cpp)
GENERATE_STUBS(Inputs.thrift cpp)
GENERATE_STUBS(SocialNetwork.thrift cpp)
GENERATE_STUBS(Devices.thrift cpp)
GENERATE_STUBS(Feedback.thrift cpp )

GENERATE_STUBS(I2P.thrift py)
GENERATE_STUBS(Control.thrift py)
GENERATE_STUBS(Inputs.thrift py)
GENERATE_STUBS(SocialNetwork.thrift py)
GENERATE_STUBS(Devices.thrift py)

INCLUDE_DIRECTORIES(${THRIFT_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
INCLUDE_DIRECTORIES($ENV{I2P_MODULE}/i2pThrift/tools/cpp)
INCLUDE_DIRECTORIES($ENV{I2P_MODULE}/i2pThrift/gen-cpp)
INCLUDE_DIRECTORIES(${PROJECT_NAME})


LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

file(TO_CMAKE_PATH "$ENV{I2P_MODULE}/i2pThrift" I2P_MODULE_PATH)

set( I2P_SRCS
    ${I2P_MODULE_PATH}/gen-cpp/I2P_constants.cpp
    ${I2P_MODULE_PATH}/gen-cpp/I2P_constants.h
    ${I2P_MODULE_PATH}/gen-cpp/I2P_types.cpp
    ${I2P_MODULE_PATH}/gen-cpp/I2P_types.h
    ${I2P_MODULE_PATH}/gen-cpp/Inputs_constants.cpp
    ${I2P_MODULE_PATH}/gen-cpp/Inputs_constants.h
	${I2P_MODULE_PATH}/gen-cpp/Feedback_constants.cpp
    ${I2P_MODULE_PATH}/gen-cpp/Feedback_constants.h
	${I2P_MODULE_PATH}/gen-cpp/SocialNetwork_constants.cpp
    ${I2P_MODULE_PATH}/gen-cpp/SocialNetwork_constants.h
    ${I2P_MODULE_PATH}/gen-cpp/Inputs_types.cpp
    ${I2P_MODULE_PATH}/gen-cpp/Inputs_types.h
	${I2P_MODULE_PATH}/gen-cpp/UserTrackingService.cpp
    ${I2P_MODULE_PATH}/gen-cpp/UserTrackingService.h
	${I2P_MODULE_PATH}/gen-cpp/UserTrackingServiceNewKinect.cpp
	${I2P_MODULE_PATH}/gen-cpp/UserTrackingServiceNewKinect.h
	${I2P_MODULE_PATH}/gen-cpp/AgentControl.h
	${I2P_MODULE_PATH}/gen-cpp/AgentControl.cpp
	${I2P_MODULE_PATH}/gen-cpp/FaceService.h
	${I2P_MODULE_PATH}/gen-cpp/FaceService.cpp
	${I2P_MODULE_PATH}/gen-cpp/SoundService.h
	${I2P_MODULE_PATH}/gen-cpp/PictureService.cpp
	${I2P_MODULE_PATH}/gen-cpp/PictureService.h
	${I2P_MODULE_PATH}/gen-cpp/Facebook.cpp
	${I2P_MODULE_PATH}/gen-cpp/Facebook.h
	${I2P_MODULE_PATH}/gen-cpp/AgentFeedbackService.h
	${I2P_MODULE_PATH}/gen-cpp/AgentFeedbackService.cpp
	${I2P_MODULE_PATH}/gen-cpp/SpeechRecognitionService.h
	${I2P_MODULE_PATH}/gen-cpp/SpeechRecognitionService.cpp
	${I2P_MODULE_PATH}/gen-cpp/NewSpeechRecognitionService.h
	${I2P_MODULE_PATH}/gen-cpp/NewSpeechRecognitionService.cpp
	${I2P_MODULE_PATH}/gen-cpp/SoundService.cpp
	${I2P_MODULE_PATH}/gen-cpp/EmotionEngineService.h
	${I2P_MODULE_PATH}/gen-cpp/EmotionEngineService.cpp
	${I2P_MODULE_PATH}/gen-cpp/SpeakerIdentification.h
	${I2P_MODULE_PATH}/gen-cpp/SpeakerIdentification.cpp
	${I2P_MODULE_PATH}/gen-cpp/ObjectDetectionService.h
	${I2P_MODULE_PATH}/gen-cpp/ObjectDetectionService.cpp
    ${I2P_MODULE_PATH}/tools/cpp/ThriftTools.cpp
    ${I2P_MODULE_PATH}/tools/cpp/ThriftTools.hpp
    ${I2P_MODULE_PATH}/tools/cpp/ProtectedClient.h #this is a tool
	${I2P_MODULE_PATH}/gen-cpp/DialogueManagerService.h
	${I2P_MODULE_PATH}/gen-cpp/DialogueManagerService.cpp
	${I2P_MODULE_PATH}/gen-cpp/ActionRecognitionService.h
	${I2P_MODULE_PATH}/gen-cpp/ActionRecognitionService.cpp
)

set(i2p_reactive_layer_SRC
	src/UserTrackingService_server.cpp
	src/UserTrackingServiceNewKinect_server.cpp
	src/FaceService_server.cpp
	src/FaceService_server.h
	src/ObjectService_server.cpp
	src/ObjectService_server.h
	src/ActionRecognitionService_server.cpp
	src/ActionRecognitionService_server.h
	src/i2p_reactive_layer.cpp
	src/reactiveLayer.h
	src/reactiveLayer.cpp
	src/soundService_server.cpp
	src/AgentFeedbackService_server.cpp
	src/SpeechRecognitionService_server.cpp
	src/NewSpeechRecognitionService_server.cpp
	src/Behavior.cpp
	src/Behavior.h
	src/LookAtPerson.h
	src/LookAtPerson.cpp
	src/LookAtSoundSource.h
	src/LookAtSoundSource.cpp
	src/BB_Bored.h
	src/BB_Bored.cpp
	src/BB_LookAtSpeechSource.h
	src/BB_LookAtSpeechSource.cpp
	src/BB_AskToStop.h
	src/BB_AskToStop.cpp
	src/BB_Answer.h
	src/BB_Answer.cpp
	src/EmotionEngineService_server.cpp
	src/BB_Idle.cpp
	src/BB_Idle.h
	src/BB_Introduce.cpp
	src/BB_Introduce.h
	src/BB_LookAtPerson.cpp
	src/BB_LookAtPerson.h
	src/BB_LookAtSpeechSource.cpp
	src/BB_LookAtSpeechSource.h
	src/BB_ReactToSpeechSource.cpp
	src/BB_ReactToSpeechSource.h
	src/BB_ReactFace.h
	src/BB_ReactFace.cpp
	src/BB_ReactObject.h
	src/BB_ReactObject.cpp
	src/BB_ReactAction.h
	src/BB_ReactAction.cpp
	src/BB_ReactGesture.h
	src/BB_ReactGesture.cpp
	src/Server_Starter.h
	src/Server_Starter.cpp
	src/SpeakerIdentification_server.cpp
	src/BB_SpeakerID.h
	src/BB_SpeakerID.cpp
	src/BB_Facebook.h
	src/BB_Facebook.cpp
	src/Facebook_server.cpp
	src/BB_Skype.h
	src/BB_Skype.cpp
	src/global.h
	src/global.cpp
	)
	
SOURCE_GROUP( i2p FILES ${I2P_SRCS})

#####################################################################
# Common Tools from I2P
set( COMMON_HDRS
    ${I2P_MODULE_PATH}/tools/cpp/ThriftTools.hpp
)
set( COMMON_SRCS
    ${I2P_MODULE_PATH}/tools/cpp/ThriftTools.cpp
)

if(WIN32)
    set( COMMON_SRCS ${COMMON_SRCS} 
        ${I2P_MODULE_PATH}/tools/cpp/ThriftTools_Win32.cpp ) 
else(WIN32)
    set( COMMON_SRCS ${COMMON_SRCS} 
        ${I2P_MODULE_PATH}/tools/cpp/ThriftTools_Unix.cpp ) 
endif(WIN32)

SOURCE_GROUP( I2PCommonSources FILES ${COMMON_SRCS})
set(HDRS ${HDRS} ${COMMON_HDRS})
set(SRCS ${SRCS} ${COMMON_SRCS})

ADD_EXECUTABLE(${PROJECT_NAME}
    ${I2P_SRCS}
	${HDRS} ${SRCS} 
	${HDR} 
	${i2p_reactive_layer_SRC}
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
#libthrift
debug ${THRIFT_LIBRARY_DEBUG}
optimized ${THRIFT_LIBRARY_RELEASE}
${KinectSDK_LIBRARY}
)
