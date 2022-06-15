find_package(OpenCV 4 COMPONENTS core highgui videoio imgcodecs)

if(${OpenCV_FOUND})
else()
        find_package(OpenCV 2 COMPONENTS core highgui REQUIRED)
endif()


find_package(PCL COMPONENTS visualization REQUIRED)
find_package(Boost COMPONENTS filesystem REQUIRED)

set(OPENNI_CAPTURE_INCLUDE_DIRS 
	${OpenCV_INCLUDE_DIRS}
	${Boost_INCLUDE_DIRS}
        ${PCL_INCLUDE_DIRS}
)

file(GLOB OPENNI_CAPTURE_SOURCES main.cpp)
include_directories(${OPENNI_CAPTURE_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

add_executable(openni-capture ${OPENNI_CAPTURE_SOURCES})
target_link_libraries(openni-capture ${OpenCV_LIBS} ${Boost_LIBRARIES} ${PCL_LIBRARIES})

install(TARGETS openni-capture RUNTIME DESTINATION "./bin/" COMPONENT tools)
