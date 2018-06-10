find_package(OpenCV 3 COMPONENTS core highgui videoio imgcodecs imgproc calib3d)

if(${OpenCV_FOUND})
else()
        find_package(OpenCV 2 REQUIRED core highgui calib3d)
endif()

set(BACKGROUND_MODEL_INCLUDE_DIRS 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${Qt5Widgets_INCLUDE_DIRS}
        ${OpenCV_INCLUDE_DIRS}
)

file(GLOB BACKGROUND_MODEL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)
file(GLOB BACKGROUND_MODEL_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)


include_directories(${BACKGROUND_MODEL_INCLUDE_DIRS}) 

add_executable(BackGroundModel ${BACKGROUND_MODEL_SOURCES} ${BACKGROUND_MODEL_HEADERS})
target_link_libraries(BackGroundModel ${OpenCV_LIBS})

install(TARGETS BackGroundModel RUNTIME DESTINATION "./bin/" COMPONENT tools)
