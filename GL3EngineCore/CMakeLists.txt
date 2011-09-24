cmake_minimum_required(VERSION 2.8)
project(GL3EngineEZR)

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

option(GLM_PATH "GLM Path" "")

add_definitions(-DUNIX)

#Optimization / Debug
if(RELEASE)
    set(CMAKE_CXX_FLAGS "-O3 -msse4 -mssse3") #Optimized
else()
    set(CMAKE_CXX_FLAGS "-O0 -g3") #Debug
endif()

#Set c++0x
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")

#Warnings
if(WARNINGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -Woverloaded-virtual -Wuninitialized")
endif()
execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
if (GCC_VERSION VERSION_GREATER 4.6 OR GCC_VERSION VERSION_EQUAL 4.6)
	message(STATUS "GCC Version >= 4.6")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wsuggest-attribute=const")
else()
	message(STATUS "GCC Version < 4.6, try to upgrade")      
endif()


find_package(OpenGL)
if(NOT OPENGL_FOUND)
	message("ERROR: OpenGL not found")
endif(NOT OPENGL_FOUND)

#check for opengl3 headers
function(download_gl_header GL_HEADER)
    if(NOT EXISTS ${CMAKE_SOURCE_DIR}/src/Renderer/GL3/${GL_HEADER})
        execute_process(
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/Renderer/GL3
            COMMAND wget http://www.opengl.org/registry/api/${GL_HEADER}
    )
    endif()
endfunction(download_gl_header)

if(EXISTS /usr/include/GL3 AND NOT LIBLUB_WINDOWS)
	message("OpenGL 3 Headers in /usr/include")
else()
	message("No OpenGL 3 Headers found in /usr! Using local ones")
	if(NOT EXISTS ${CMAKE_SOURCE_DIR}/GL3)
		execute_process(COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/GL3)
	endif()
	set(GL_HEADERS gl3.h glext.h glxext.h)
	foreach(GL_HEADER ${GL_HEADERS})
		    download_gl_header(${GL_HEADER})
    endforeach(GL_HEADER)

	include_directories(GL3)
endif()

#Check for nvidia libGL in ubuntu
if(EXISTS "/usr/lib/nvidia-current/libGL.so")
	message("Taking Ubuntu Nvidia libGL")
	SET(OPENGL_LIB /usr/lib/nvidia-current/libGL.so)
else()
	message("Taking system libGL")
	#SET(OPENGL_LIB GL)
	#set(OPENGL_LIB opengl32)
	set(OPENGL_LIB ${OPENGL_LIBRARY})
endif()

include_directories(${GLM_PATH})

file(GLOB GLOB_RESULT *.cpp)
set(DEMO_SOURCES ${GLOB_RESULT})

add_executable(gl3engineezr ${DEMO_SOURCES})
target_link_libraries(gl3engineezr GL glfw assimp)
