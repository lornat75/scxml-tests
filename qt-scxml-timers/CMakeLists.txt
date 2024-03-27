cmake_minimum_required(VERSION 3.22)

set (project-name qt-scxml-timers)
project (project-name)

# set up our program
add_executable(${project-name})

if (WIN32)
  # we use q55 in windows
  find_package(Qt5 COMPONENTS Scxml Core REQUIRED)
  # generate state machine code for WIN32/mamba (workaround bug in qt_add_statecharts)
  message (STATUS "Using workaround for qt_add_statecharts, calling ${Qt5Scxml_QSCXMLC_EXECUTABLE}")
  execute_process(COMMAND ${Qt5Scxml_QSCXMLC_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/MyStateMachine.scxml")
else ()
  # we use q6 in linux
  find_package(Qt6 COMPONENTS Scxml Core REQUIRED)
  # generate code and state machine in linux
  qt_add_statecharts(${project-name} "${CMAKE_CURRENT_SOURCE_DIR}/MyStateMachine.scxml")
endif()


# declare our source files
target_sources(${project-name} PRIVATE main.cpp ${CMAKE_CURRENT_BINARY_DIR}/MyStateMachine.cpp)
target_include_directories(${project-name} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})


if (WIN32)
	target_link_libraries(${project-name} PRIVATE Qt5::Core Qt5::Scxml)
else ()
	target_link_libraries(${project-name} PRIVATE Qt6::Core Qt6::Scxml)
endif()
	
