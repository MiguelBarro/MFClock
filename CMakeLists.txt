#-----CMakeLists.txt------

cmake_minimum_required(VERSION 3.15.3)

cmake_policy(SET CMP0091 NEW)

project(eProsimaClock LANGUAGES CXX)

# incorporate MFC static support
find_package(MFC REQUIRED)

set(CMAKE_MFC_FLAG 1)	# static library
set(BUILD_SHARED_LIBS OFF) # fastrtps doesn't explicitly defined the kind of linking

# incorporate fastrtps support
find_package(fastcdr REQUIRED) # idl types dependency
find_package(fastrtps REQUIRED)

set(IDL_TYPES 
        Type/eProsimaClockType.cxx
        Type/eProsimaClockTypePubSubTypes.cxx
    )

set(SOURCES 	eProsimaClock.cpp
		eProsimaClockDlg.cpp
		eProsimaClock.rc)
				
set(HEADERS	stdafx.h
		eProsimaClock.h
		eProsimaClockDlg.h
        Type/eProsimaClockType.h
        Type/eProsimaClockTypePubSubTypes.h
        )
				
set(PRECOMPILED stdafx.cpp)
set_source_files_properties( ${SOURCES} PROPERTIES COMPILE_OPTIONS "/Yu stdafx.h")
set_source_files_properties( ${PRECOMPILED} PROPERTIES COMPILE_OPTIONS "/Yc stdafx.h")

#create an executable
add_executable(eProsimaClock ${HEADERS} ${SOURCES} ${PRECOMPILED} ${IDL_TYPES})

set_target_properties(eProsimaClock PROPERTIES WIN32_EXECUTABLE TRUE
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# fastrtps support
target_link_libraries(eProsimaClock fastcdr fastrtps)

#install the binaries
install(TARGETS eProsimaClock 
                CONFIGURATIONS Release
                RUNTIME DESTINATION installed/bin               COMPONENT Runtime
                ARCHIVE DESTINATION installed/lib               COMPONENT Development)

install(TARGETS eProsimaClock
                CONFIGURATIONS Debug
                RUNTIME DESTINATION installed/bin/Debug         COMPONENT Runtime
                ARCHIVE DESTINATION installed/lib/Debug         COMPONENT Development)


