#-----CMakeLists.txt------

cmake_minimum_required(VERSION 3.15.3)

cmake_policy(SET CMP0091 NEW)

project(eProsimaClock LANGUAGES CXX)

# incorporate MFC static support
find_package(MFC REQUIRED)

if(BUILD_SHARED_LIBS)
    set(CMAKE_MFC_FLAG 2)	# dynamic library
else()
    set(CMAKE_MFC_FLAG 1)	# static library  
endif()

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

if(BUILD_SHARED_LIBS)
    set_target_properties(eProsimaClock PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
else()
    set_target_properties(eProsimaClock PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

set_target_properties(eProsimaClock PROPERTIES WIN32_EXECUTABLE TRUE)

# Properties that change bin names depending on current config.
set_target_properties(eProsimaClock PROPERTIES MINSIZEREL_POSTFIX m )
set_target_properties(eProsimaClock PROPERTIES RELWITHDEBINFO_POSTFIX rd )
set_target_properties(eProsimaClock PROPERTIES DEBUG_POSTFIX d )

# fastrtps support
target_link_libraries(eProsimaClock fastcdr fastrtps)

#install the binaries
install(TARGETS eProsimaClock 
                RUNTIME DESTINATION bin               COMPONENT Runtime
                ARCHIVE DESTINATION lib               COMPONENT Development)
