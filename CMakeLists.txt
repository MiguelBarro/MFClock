#-----CMakeLists.txt------

cmake_minimum_required(VERSION 3.14)

project(eProsimaClock LANGUAGES CXX)

find_package(MFC REQUIRED)

set(CMAKE_MFC_FLAG 1)	# static library

set(SOURCES 	eProsimaClock.cpp
		eProsimaClockDlg.cpp
		eProsimaClock.rc)
				
set(HEADERS	stdafx.h
		eProsimaClock.h
		eProsimaClock.h)
				
set(PRECOMPILED stdafx.cpp)
set_source_files_properties( ${SOURCES} PROPERTIES COMPILE_OPTIONS "/Yu stdafx.h")
set_source_files_properties( ${PRECOMPILED} PROPERTIES COMPILE_OPTIONS "/Yc stdafx.h")

#create an executable
add_executable(eProsimaClock ${HEADERS} ${SOURCES} ${PRECOMPILED})

target_compile_options(eProsimaClock PUBLIC $<IF:$<CONFIG:Debug>,/MTd,/MT>) 

set_target_properties(eProsimaClock PROPERTIES WIN32_EXECUTABLE TRUE)

#install the binaries
install(TARGETS eProsimaClock 
                CONFIGURATIONS Release
                RUNTIME DESTINATION installed/bin               COMPONENT Runtime
                ARCHIVE DESTINATION installed/lib               COMPONENT Development)

install(TARGETS eProsimaClock
                CONFIGURATIONS Debug
                RUNTIME DESTINATION installed/bin/Debug         COMPONENT Runtime
                ARCHIVE DESTINATION installed/lib/Debug         COMPONENT Development)


