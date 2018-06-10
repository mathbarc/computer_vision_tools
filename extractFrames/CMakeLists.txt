find_package(OpenCV 3 COMPONENTS core highgui videoio imgcodecs)

if(${OpenCV_FOUND})
else()
	find_package(OpenCV 2 REQUIRED core highgui)
endif()

set(EXTRACTFRAMES_INCLUDE_DIRS 
	${OpenCV_INCLUDE_DIRS}
)

file(GLOB EXTRACTFRAMES_SOURCES main.cpp)
include_directories(${EXTRACTFRAMES_INCLUDE_DIRS})

add_executable(extractFrames ${EXTRACTFRAMES_SOURCES})
target_link_libraries(extractFrames ${OpenCV_LIBS})

install(TARGETS extractFrames RUNTIME DESTINATION "./bin/" COMPONENT tools)
