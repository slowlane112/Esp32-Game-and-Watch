cmake_minimum_required(VERSION 3.16)

if(MODEL)
	string(TOLOWER "${MODEL}" MODEL)
endif()

if(MODEL STREQUAL "single_screen_2_button")
	add_compile_definitions(MODEL_SINGLE_SCREEN_2_BUTTON=1)
	set(model_message "Single Screen 2 Button")
elseif(MODEL STREQUAL "multi_screen_2_button")
	add_compile_definitions(MODEL_MULTI_SCREEN_2_BUTTON=1)
	set(model_message "Multi Screen 2 Button")
	
elseif(MODEL STREQUAL "multi_screen_dpad")
	add_compile_definitions(MODEL_MULTI_SCREEN_DPAD=1)
	set(model_message "Multi Screen Dpad")
		
else()
	add_compile_definitions(MODEL_SINGLE_SCREEN_4_BUTTON=1)
	set(model_message "Single Screen 4 Button (Default)")
endif()

message(STATUS "Model Build Target: ${model_message}")

if(DISPLAY)
    string(TOLOWER "${DISPLAY}" DISPLAY)
endif()

if(DISPLAY STREQUAL "st7789")
	add_compile_definitions(USE_ST7789=1)
	set(display_message "ST7789")
else()
	add_compile_definitions(USE_ILI9341=1)
	set(display_message "ILI9341 (Default)")
endif()

message(STATUS "Display Build Target: ${display_message}")

set(EXTRA_COMPONENT_DIRS ../components)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(gandw_handheld)
