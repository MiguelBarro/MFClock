#-----CMakeLists.txt------

cmake_minimum_required(VERSION 3.13.2)

project(eProsimaClock LANGUAGES CXX)

# incorporate MFC support
find_package(MFC REQUIRED)
set(CMAKE_MFC_FLAG 1)	# static library

#incorporate fastrtps support
find_package(fastrtps REQUIRED)

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

# MFC static link support
target_compile_options(eProsimaClock PUBLIC $<IF:$<CONFIG:Debug>,/MTd,/MT>) 
set_target_properties(eProsimaClock PROPERTIES WIN32_EXECUTABLE TRUE)

# fastrtps support
set(BUILD_SHARED_LIBS OFF) # fastrtps doesn't explicitly defined the kind of linking
target_link_libraries(eProsimaClock fastrtps)

#install the binaries
install(TARGETS eProsimaClock 
                CONFIGURATIONS Release
                RUNTIME DESTINATION installed/bin               COMPONENT Runtime
                ARCHIVE DESTINATION installed/lib               COMPONENT Development)

install(TARGETS eProsimaClock
                CONFIGURATIONS Debug
                RUNTIME DESTINATION installed/bin/Debug         COMPONENT Runtime
                ARCHIVE DESTINATION installed/lib/Debug         COMPONENT Development)


