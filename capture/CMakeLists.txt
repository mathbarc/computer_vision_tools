find_package(OpenCV 4 COMPONENTS core highgui videoio imgcodecs)

if(${OpenCV_FOUND})
else()
        find_package(OpenCV 2 COMPONENTS core highgui REQUIRED)
endif()

find_package(Boost COMPONENTS filesystem REQUIRED)

set(CAPTURE_INCLUDE_DIRS 
	${OpenCV_INCLUDE_DIRS}
	${Boost_INCLUDE_DIRS}
)

file(GLOB CAPTURE_SOURCES main.cpp)
include_directories(${CAPTURE_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

add_executable(capture ${CAPTURE_SOURCES})
target_link_libraries(capture ${OpenCV_LIBS} ${Boost_LIBRARIES})

install(TARGETS capture RUNTIME DESTINATION "./bin/" COMPONENT tools)
