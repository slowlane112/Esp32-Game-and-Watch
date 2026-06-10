cmake_minimum_required(VERSION 3.16)

if(MODEL)
	string(TOLOWER "${MODEL}" MODEL)
endif()

if(MODEL STREQUAL "cardputer_multi_screen")
	add_compile_definitions(MODEL_CARDPUTER_MULTI_SCREEN=1)
	set(model_message "Cardputer Multi Screen")
else()
	add_compile_definitions(MODEL_CARDPUTER=1)
	set(model_message "Cardputer (Default)")
endif()

message(STATUS "Model Build Target: ${model_message}")

set(EXTRA_COMPONENT_DIRS ../components)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(gandw_cardputer)

