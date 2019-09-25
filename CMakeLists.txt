#-----CMakeLists.txt------

cmake_minimum_required(VERSION 3.13)

project(TskClock LANGUAGES CXX)

find_package(MFC REQUIRED)

set(CMAKE_MFC_FLAG 2)	# dynamic library

set(SOURCES 	TskClock.cpp
				TskClockDlg.cpp
				TskClock.rc)
				
set(HEADERS		stdafx.h
				TskClock.h
				TskClock.h)
				
set(PRECOMPILED stdafx.cpp)
set_source_files_properties( ${SOURCES} PROPERTIES COMPILE_OPTIONS "/Yu stdafx.h")
set_source_files_properties( ${PRECOMPILED} PROPERTIES COMPILE_OPTIONS "/Yc stdafx.h")

#create an executable
add_executable(TskClock ${HEADERS} ${SOURCES} ${PRECOMPILED})

set_target_properties(TskClock PROPERTIES WIN32_EXECUTABLE TRUE)

#install the binaries
install(TARGETS TskClock 
                CONFIGURATIONS Release
                RUNTIME DESTINATION installed/bin               COMPONENT Runtime
                ARCHIVE DESTINATION installed/lib               COMPONENT Development)

install(TARGETS TskClock
                CONFIGURATIONS Debug
                RUNTIME DESTINATION installed/bin/Debug         COMPONENT Runtime
                ARCHIVE DESTINATION installed/lib/Debug         COMPONENT Development)


