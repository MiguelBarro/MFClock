# eProsimaClock

**eProsimaClock** is a simple example of an MFC application that uses MFC as static or dynamic library. Set by using the variable BUILD_SHARED_LIBS ON or OFF.

## Building procedure

The steps to provide an MFC application CMakeLists.txt of fastrtps support are the following:

- one must include the MFC as library by using `find_package(MFC REQUIRED)`.
- set the CMAKE_MFC_FLAG variable to 1 (static) or 2 (dynamic) in order to check the type of MFC linking.
- set target property WIN32_EXECUTABLE to change the application entry point to windows application.
- set target property MSVC_RUNTIME_LIBRARY to choose how to link to C-Runtime (either dynamically or statically). This kind of linking should match the MFC one in order to prevent memory leaks.

 Note that to profit from the MSVC_RUNTIME_LIBRARY and the CMAKE_MSVC_RUNTIME_LIBRARY we must be using CMake version 3.15.3 or higher and enable the policy CMP0091. This is done by doing:
 
 ```
    cmake_minimum_required(VERSION 3.15.3)
    cmake_policy(SET CMP0091 NEW)
 ```
One must have a proper fastrtps installation in order to link dynamically or statically the MFC application with it. Current fastrtps distributed binaries use a dynamic link to CRT runtime (either dynamic or static binaries). Right now is not CMake support for generating fastrtps binaries with static linking to CRT runtime. In order to workaround this limitation we can profit from CMake 3.15.3 capabilities. Passing to fastcdr, fastrtps and tiny cmake command line:

 ```
    -DBUILD_SHARED_LIBS=OFF -DCMAKE_MSVC_RUNTIME_LIBRARY="MultiThreaded$<$<CONFIG:Debug>:Debug>" -DCMAKE_POLICY_DEFAULT_CMP0091=NEW 
 ```  

## Example operation

The application generates a dialog with two displays and two buttons. The first button allows to publish or subscribe current counter count. The second button is only enable for publishing and starts and stops the counter. 

On publishing mode the GUID of the generated publisher is show and the counter tics are published when counter is running.

On subscribing mode the GUID of the current publisher is shown and the last correspondig published value. If there are several publishers the one with lower GUID is chosen as current one.