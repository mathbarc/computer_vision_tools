set( CMAKE_AUTOMOC ON )
set( CMAKE_INCLUDE_CURRENT_DIR ON )

find_package(Qt5Widgets REQUIRED)
find_package(OpenCV 3 COMPONENTS core highgui videoio imgcodecs imgproc calib3d)

if(${OpenCV_FOUND})
else()
        find_package(OpenCV 2 REQUIRED core highgui calib3d)
endif()

set(CAMERAMARKER_INCLUDE_DIRS 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${Qt5Widgets_INCLUDE_DIRS}
        ${OpenCV_INCLUDE_DIRS}
)

file(GLOB CAMERAMARKER_SOURCES ./source/*.cpp)
file(GLOB CAMERAMARKER_HEADERS ./include/*.hpp ./include/*.h)
file(GLOB CAMERAMARKER_FORMS ./forms/*.ui)

include_directories(${CAMERAMARKER_INCLUDE_DIRS})

qt5_wrap_ui(CAMERAMARKER_WRAPPED_IUS ${CAMERAMARKER_FORMS})
qt5_add_resources(CAMERAMARKER_RESOURCE ${CMAKE_CURRENT_SOURCE_DIR}/resource.qrc)

add_executable(CameraMarker ${CAMERAMARKER_SOURCES} ${CAMERAMARKER_HEADERS} ${CAMERAMARKER_WRAPPED_IUS} ${CAMERAMARKER_RESOURCE})
target_link_libraries(CameraMarker Qt5::Widgets ${OpenCV_LIBS})
add_dependencies(CameraMarker Qt5::Widgets ${OpenCV_LIBS})

install(TARGETS CameraMarker RUNTIME DESTINATION "./bin/" COMPONENT tools)
