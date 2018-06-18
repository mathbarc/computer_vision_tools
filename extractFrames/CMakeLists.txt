find_package(OpenCV 3 COMPONENTS core highgui videoio imgcodecs)

if(${OpenCV_FOUND})
else()
        find_package(OpenCV 2 REQUIRED core highgui REQUIRED)
endif()

find_package(Boost COMPONENTS filesystem REQUIRED)

set(EXTRACTFRAMES_INCLUDE_DIRS 
	${OpenCV_INCLUDE_DIRS}
        ${Boost_INCLUDE_DIRS}
)

file(GLOB EXTRACTFRAMES_SOURCES main.cpp)
include_directories(${EXTRACTFRAMES_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

add_executable(extractFrames ${EXTRACTFRAMES_SOURCES})
target_link_libraries(extractFrames ${OpenCV_LIBS} ${Boost_LIBRARIES})

install(TARGETS extractFrames RUNTIME DESTINATION "./bin/" COMPONENT tools)
