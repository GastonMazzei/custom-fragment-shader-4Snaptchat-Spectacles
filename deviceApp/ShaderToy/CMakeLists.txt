cmake_minimum_required(VERSION 3.11)

project(ShaderToy VERSION 2.0.0 LANGUAGES C CXX)

# AppKit and all its usage requirements is discovered through a CMake
# configuration. Make sure that the AppKit config file can be found by CMake by
# setting an appopriate CMAKE_PREFIX_PATH or by setting VOSAppKit_DIR to point
# to the 'lib/cmake/VOSAppKit' folder of the AppKit bundle (where VOSAppKitConfig.cmake is
# located)
find_package(VOSAppKit REQUIRED)



#**************************************************
# Adding Curl!

# option 1: not working
#find_package( CURL REQUIRED )
#target_link_libraries( curl-demo CURL::libcurl )

# option 2: not working
#find_package(PkgConfig REQUIRED)
#pkg_check_modules(libcurl REQUIRED IMPORTED_TARGET libcurl>=7.17.0)

# option 3: not working
#set(CURL_LIBRARY "-lcurl") 
#find_package(CURL REQUIRED) 
#include_directories(${CURL_INCLUDE_DIR})

# other options: not working
#set(CMAKE_USE_OPENSSL OFF)
#set(CMAKE_USE_LIBSSH2 OFF)
#add_subdirectory(curl)

#**************************************************

# Add your application-specific source files here
app_kit_add_application(
    TARGET ShaderToy
    FILES
        "${CMAKE_CURRENT_LIST_DIR}/CubeGL.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/CubeGL.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/CubeDelegate.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/CubeDelegate.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/external/glad.c")


target_include_directories(ShaderToy PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/external")

# Define a decent level of warnings
target_compile_options(ShaderToy PRIVATE
    # Clang / AppleClang / GCC
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-Wall$<SEMICOLON>-Wextra>
    # MSVC
    $<$<CXX_COMPILER_ID:MSVC>:/W3>)

# This command actually does more than just 'linking', it pulls in all usage
# requirements for the app to use AppKit. In this case, this means adding
# AppKit's source files to this project, setting various compiler options, etc.
# AppKit itself doesn't provide a precompiled library. It comes with source
# files that your project needs to compile with its other sources. These
# sources are attached to the VOS::AppKit target via its INTERFACE_SOURCES
# property. They are automatically added to your project tree by 'linking'
# against the VOS::AppKit target with the following command.
target_link_libraries(ShaderToy PRIVATE VOS::AppKit)





# This CMake function is provided by AppKit to bundle your application
# It will generate a new target called 'CubeSampleBundle' that needs to be
# executed manually

app_kit_add_android_bundler(
    TARGET ShaderToy
    NAME RedShaderToy
    DATA_DIR assets_redcube
    VERSION ${PROJECT_VERSION}
    DOMAIN "com"
    ORG "snaptech")

app_kit_add_android_bundler(
    TARGET ShaderToy
    DEPENDER_TARGET AppKitBundle
    DATA_DIR assets
    VERSION ${PROJECT_VERSION}
    # DOMAIN/ORG are used for the bundle identifier, e.g. org.MyCompany.ShaderToy
THEME "@style/AppKitThemeAlwaysOn"
    DOMAIN "com"
    ORG "snaptech")

app_kit_add_hawaii_bundler(
    TARGET ShaderToy
    DEPENDER_TARGET AppKitBundle
    DATA_DIR assets
    VERSION ${PROJECT_VERSION})

app_kit_add_hawaii_bundler(
    TARGET ShaderToy
    NAME RedShaderToy
    DATA_DIR assets_redcube
    VERSION ${PROJECT_VERSION}
    DOMAIN "com"
    ORG "snaptech")

# Snap internal, can be ignored
if (TARGET AppKitBundle)
    app_kit_add_desktop_bundler(
        TARGET ShaderToy
        DEPENDER_TARGET AppKitBundle
        VERSION ${PROJECT_VERSION})
endif()
