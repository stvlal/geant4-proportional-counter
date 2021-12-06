cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(spectrometer)

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})

file(GLOB sources ${PROJECT_SOURCE_DIR}/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/*.hh)

file(GLOB STL_FILES
	"*.stl"
)
file(GLOB MAC_FILES
	"*.mac"
)
file(GLOB C_FILES
	"*.C"
)

file(COPY ${STL_FILES} DESTINATION ${PROJECT_BINARY_DIR})
file(COPY ${MAC_FILES} DESTINATION ${PROJECT_BINARY_DIR})
file(COPY ${C_FILES} DESTINATION ${PROJECT_BINARY_DIR})

add_executable(my_spectrometer my_spectrometer.cc ${sources} ${headers})
target_link_libraries(my_spectrometer ${Geant4_LIBRARIES})
target_link_libraries(my_spectrometer cadmesh)

add_custom_target(spectrometer DEPENDS my_spectrometer)
