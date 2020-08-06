<# 

.SYNOPSIS

Build & install eProsima Clock example

.DESCRIPTION

Install-ClockEprosima uses CMake setup with the given parameters to build & install this example and all its
dependencies  

.PARAMETER InstallDir

Directory where the example and its dependencies will be installed

.PARAMETER BuildDir

Directory where the example and its dependencies will be cloned and built 

.PARAMETER DebugBuild 

Use the flag -DebugBuild to generate a debug build. 

.PARAMETER SharedLibs
Specify if static or dynamic libraries should be generated 

.EXAMPLE

This will use /temp/example as workspace to generate a debug build with static
libraries. eProsimaClock will be statically link to C++ runtime and MFC.

> Install-ClockEprosima /temp/example/install /temp/example/build -DebugBuild

.EXAMPLE

This will install the example in Program Files and build in a temporary dir a
Release version of the example using C++ runtime and MFC as dynamic libraries. 

> Install-ClockEprosima $Env:ProgramFiles\eProsimaClock $Env:TMP\eProsimaClock -SharedLibs

.LINK 

https://github.com/MiguelBarro/MFClock.git

.LINK 

https://fast-dds.docs.eprosima.com/en/latest/

.LINK

https://www.eprosima.com/

#>

[CmdletBinding()]
Param(

[Parameter(Position=0, Mandatory=$true)]
[ValidateScript({Test-Path $_ -PathType Container -IsValid })] 
[Alias("Install")]
[String]
# directory where the example will be installed
$InstallDir,

[Parameter(Position=1)]
[ValidateScript({Test-Path $_ -PathType Container -IsValid })] 
[Alias("Build")]
[String]
# directory where the example will be build 
$BuildDir="$Env:TMP\eProsima" + (New-Guid),

[Switch]
# use the flag -DebugBuild to generate a debug build. 
$DebugBuild,

[Switch]
# specify if static or dynamic libraries should be generated 
$SharedLibs
    
)

# create the install path if it doesn't exists
if (!(Test-Path $InstallDir -PathType Container)) { mkdir $InstallDir }

# create the install path if it doesn't exists
if (!(Test-Path $BuildDir -PathType Container)) { mkdir $BuildDir }

# rig SharedLibs
$build_shared = "-DBUILD_SHARED_LIBS=$(if($SharedLibs) {"ON"} else {"OFF"})"

# rig config
$build_config = $(if($DebugBuild) {"Debug"} else {"Release"})

# rig runtime, note foonathan prevents us from using $<${BUILD_SHARED_LIBS}:DLL> generator expression
$runtime = "-DCMAKE_MSVC_RUNTIME_LIBRARY=`"MultiThreaded$<$<CONFIG:Debug>:Debug>$(if($SharedLibs){"DLL"})`""

# keep the current dir
pushd
cd $BuildDir

# fastcdr build ----------------------------- 
git clone https://github.com/eProsima/Fast-CDR.git

Join-Path $BuildDir Fast-CDR/build -OutVariable fastcdr_build

cmake.exe -DCMAKE_INSTALL_PREFIX="$InstallDir" $build_shared $runtime -DCMAKE_POLICY_DEFAULT_CMP0091=NEW -Ax64 "-B$fastcdr_build" Fast-CDR 

cmake --build $fastcdr_build --config $build_config --target install 

# foonatham memory --------------------------
git clone --recurse-submodules --branch v0.6-2 https://github.com/foonathan/memory.git

Join-Path $BuildDir memory/build -OutVariable memory_build

# foonathan is always a static lib
cmake -DCMAKE_INSTALL_PREFIX="$InstallDir" -DBUILD_SHARED_LIBS=OFF $runtime -DCMAKE_POLICY_DEFAULT_CMP0091=NEW -Ax64 "-B$memory_build" memory 

cmake --build $memory_build --config $build_config --target install 

# fastdds -----------------------------------
git clone https://github.com/eProsima/Fast-DDS.git 

Join-Path $BuildDir Fast-DDS/build -OutVariable fastdds_build

cmake -DCMAKE_INSTALL_PREFIX="$InstallDir" $build_shared $runtime -DCMAKE_POLICY_DEFAULT_CMP0091=NEW -DTHIRDPARTY=ON -DCMAKE_PREFIX_PATH="$InstallDir" -DNO_TLS=ON -DSECURITY=OFF -DCOMPILE_EXAMPLES=OFF -DEPROSIMA_BUILD_TESTS=OFF "-DINTERNAL_DEBUG=$(if($DebugBuild){"ON"})" -Ax64 "-B$fastdds_build" Fast-DDS 

cmake --build $fastdds_build --config $build_config --target install 

# eProsimaClock -----------------------------
git clone https://github.com/MiguelBarro/MFClock.git

Join-Path $BuildDir MFClock/build -OutVariable clock_build

# note that $runtime is not required here because the CMakeLists.txt handles it
# through the BUILD_SHARED_LIBS variable. That's the reason why we pass
# $build_shared to the command line. This variable is used as a flag to hint
# how we want fastdds to be linked (that doesn't match the meaning on CMake).
# This way we mimic how fastdds is build which simplifies setup.

cmake -DCMAKE_INSTALL_PREFIX="$InstallDir" $build_shared -DCMAKE_PREFIX_PATH="$InstallDir" "-B$clock_build" -Ax64 MFClock 

cmake --build $clock_build --config $build_config --target install 

# restore the original dir
popd
