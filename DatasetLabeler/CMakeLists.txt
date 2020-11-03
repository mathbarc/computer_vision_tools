set( CMAKE_AUTOMOC ON )
set( CMAKE_INCLUDE_CURRENT_DIR ON )

find_package(Qt5Widgets REQUIRED)
find_package(OpenCV 4 COMPONENTS core highgui videoio imgcodecs imgproc calib3d)

if(${OpenCV_FOUND})
else()
        find_package(OpenCV 2 REQUIRED core highgui calib3d)
endif()

list(APPEND DATASETMARKER_INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${Qt5Widgets_INCLUDE_DIRS}
        ${UTILS_INCLUDE_DIR}
        ${OpenCV_INCLUDE_DIRS}
)

file(GLOB DATASETMARKER_SOURCES ./source/*.cpp)
file(GLOB DATASETMARKER_HEADERS ./include/*.hpp ./include/*.h)
file(GLOB DATASETMARKER_FORMS ./forms/*.ui)

include_directories(${DATASETMARKER_INCLUDE_DIRS})

qt5_wrap_ui(DATASETMARKER_WRAPPED_IUS ${DATASETMARKER_FORMS})
qt5_add_resources(DATASETMARKER_RESOURCE ${CMAKE_CURRENT_SOURCE_DIR}/resource.qrc)

add_executable(DatasetLabeler ${DATASETMARKER_SOURCES} ${DATASETMARKER_HEADERS} ${DATASETMARKER_WRAPPED_IUS} ${DATASETMARKER_RESOURCE})
target_link_libraries(DatasetLabeler Qt5::Widgets util ${OpenCV_LIBS})
add_dependencies(DatasetLabeler Qt5::Widgets util ${OpenCV_LIBS})

install(TARGETS DatasetLabeler RUNTIME DESTINATION "./bin/" COMPONENT tools)
