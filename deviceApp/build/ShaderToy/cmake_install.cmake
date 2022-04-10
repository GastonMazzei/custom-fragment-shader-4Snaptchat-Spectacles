# Install script for directory: /home/m4zz31/Snapchat/Dev/spectacles-native-sdk/ShaderToy-Cube/ShaderToy

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/home/m4zz31/Android/Sdk/ndk/23b/toolchains/llvm/prebuilt/linux-x86_64/bin/llvm-objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xRedShaderToyComponentx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/libs/arm64-v8a/libShaderToy.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/libs/arm64-v8a/libShaderToy.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/libs/arm64-v8a/libShaderToy.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/libs/arm64-v8a" TYPE SHARED_LIBRARY FILES "/home/m4zz31/Snapchat/Dev/spectacles-native-sdk/ShaderToy-Cube/build/ShaderToy/libShaderToy.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/libs/arm64-v8a/libShaderToy.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/libs/arm64-v8a/libShaderToy.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/home/m4zz31/Android/Sdk/ndk/23b/toolchains/llvm/prebuilt/linux-x86_64/bin/llvm-strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/libs/arm64-v8a/libShaderToy.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xRedShaderToyComponentx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xRedShaderToyComponentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/./apk" TYPE FILE FILES
    "/home/m4zz31/Snapchat/Dev/spectacles-native-sdk/ShaderToy-Cube/build/ShaderToy/apk_bundle_RedShaderToy/apk/keystore.properties"
    "/home/m4zz31/Snapchat/Dev/spectacles-native-sdk/ShaderToy-Cube/build/ShaderToy/apk_bundle_RedShaderToy/apk/build.gradle"
    "/home/m4zz31/Snapchat/Dev/spectacles-native-sdk/ShaderToy-Cube/build/ShaderToy/apk_bundle_RedShaderToy/apk/AndroidManifest.xml"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xRedShaderToyComponentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/./aar" TYPE FILE FILES
    "/home/m4zz31/Snapchat/Dev/spectacles-native-sdk/ShaderToy-Cube/build/ShaderToy/apk_bundle_RedShaderToy/aar/build.gradle"
    "/home/m4zz31/Snapchat/Dev/spectacles-native-sdk/ShaderToy-Cube/build/ShaderToy/apk_bundle_RedShaderToy/aar/AndroidManifest.xml"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xRedShaderToyComponentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/assets" TYPE DIRECTORY FILES "/home/m4zz31/Snapchat/Dev/spectacles-native-sdk/ShaderToy-Cube/ShaderToy/assets_redcube/")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xRedShaderToyComponentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/assets" TYPE FILE RENAME "calibration.json" FILES "/home/m4zz31/Snapchat/Dev/spectacles-native-sdk/vos-app-kit-android/resources/hermosa-calibration.json")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xRedShaderToyComponentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/assets" TYPE FILE RENAME "calibration-480x564.json" FILES "/home/m4zz31/Snapchat/Dev/spectacles-native-sdk/vos-app-kit-android/resources/hermosa-calibration-480x564.json")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xRedShaderToyComponentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/libs/arm64-v8a" TYPE FILE FILES
    "/home/m4zz31/Snapchat/Dev/spectacles-native-sdk/vos-api-android/lib/libVOSVisionCore.so"
    "/home/m4zz31/Snapchat/Dev/spectacles-native-sdk/vos-api-android/lib/libVOSPlatformCalibration.so"
    "/home/m4zz31/Snapchat/Dev/spectacles-native-sdk/vos-api-android/lib/libVOSSlamCore.so"
    "/home/m4zz31/Snapchat/Dev/spectacles-native-sdk/vos-api-android/lib/libVOSPlatformGraphics.so"
    "/home/m4zz31/Snapchat/Dev/spectacles-native-sdk/vos-api-android/lib/libVOSVisionCore.so"
    "/home/m4zz31/Snapchat/Dev/spectacles-native-sdk/vos-api-android/lib/libVOSPlatformCalibration.so"
    "/home/m4zz31/Snapchat/Dev/spectacles-native-sdk/vos-api-android/lib/libVOSSlamCore.so"
    "/home/m4zz31/Snapchat/Dev/spectacles-native-sdk/vos-api-android/lib/libVOSPlatformGraphics.so"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xShaderToyComponentx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/libs/arm64-v8a/libShaderToy.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/libs/arm64-v8a/libShaderToy.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/libs/arm64-v8a/libShaderToy.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/libs/arm64-v8a" TYPE SHARED_LIBRARY FILES "/home/m4zz31/Snapchat/Dev/spectacles-native-sdk/ShaderToy-Cube/build/ShaderToy/libShaderToy.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/libs/arm64-v8a/libShaderToy.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/libs/arm64-v8a/libShaderToy.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/home/m4zz31/Android/Sdk/ndk/23b/toolchains/llvm/prebuilt/linux-x86_64/bin/llvm-strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/libs/arm64-v8a/libShaderToy.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xShaderToyComponentx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xShaderToyComponentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/./apk" TYPE FILE FILES
    "/home/m4zz31/Snapchat/Dev/spectacles-native-sdk/ShaderToy-Cube/build/ShaderToy/apk_bundle_ShaderToy/apk/keystore.properties"
    "/home/m4zz31/Snapchat/Dev/spectacles-native-sdk/ShaderToy-Cube/build/ShaderToy/apk_bundle_ShaderToy/apk/build.gradle"
    "/home/m4zz31/Snapchat/Dev/spectacles-native-sdk/ShaderToy-Cube/build/ShaderToy/apk_bundle_ShaderToy/apk/AndroidManifest.xml"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xShaderToyComponentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/./aar" TYPE FILE FILES
    "/home/m4zz31/Snapchat/Dev/spectacles-native-sdk/ShaderToy-Cube/build/ShaderToy/apk_bundle_ShaderToy/aar/build.gradle"
    "/home/m4zz31/Snapchat/Dev/spectacles-native-sdk/ShaderToy-Cube/build/ShaderToy/apk_bundle_ShaderToy/aar/AndroidManifest.xml"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xShaderToyComponentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/assets" TYPE DIRECTORY FILES "/home/m4zz31/Snapchat/Dev/spectacles-native-sdk/ShaderToy-Cube/ShaderToy/assets/")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xShaderToyComponentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/assets" TYPE FILE RENAME "calibration.json" FILES "/home/m4zz31/Snapchat/Dev/spectacles-native-sdk/vos-app-kit-android/resources/hermosa-calibration.json")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xShaderToyComponentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/assets" TYPE FILE RENAME "calibration-480x564.json" FILES "/home/m4zz31/Snapchat/Dev/spectacles-native-sdk/vos-app-kit-android/resources/hermosa-calibration-480x564.json")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xShaderToyComponentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/libs/arm64-v8a" TYPE FILE FILES
    "/home/m4zz31/Snapchat/Dev/spectacles-native-sdk/vos-api-android/lib/libVOSVisionCore.so"
    "/home/m4zz31/Snapchat/Dev/spectacles-native-sdk/vos-api-android/lib/libVOSPlatformCalibration.so"
    "/home/m4zz31/Snapchat/Dev/spectacles-native-sdk/vos-api-android/lib/libVOSSlamCore.so"
    "/home/m4zz31/Snapchat/Dev/spectacles-native-sdk/vos-api-android/lib/libVOSPlatformGraphics.so"
    "/home/m4zz31/Snapchat/Dev/spectacles-native-sdk/vos-api-android/lib/libVOSVisionCore.so"
    "/home/m4zz31/Snapchat/Dev/spectacles-native-sdk/vos-api-android/lib/libVOSPlatformCalibration.so"
    "/home/m4zz31/Snapchat/Dev/spectacles-native-sdk/vos-api-android/lib/libVOSSlamCore.so"
    "/home/m4zz31/Snapchat/Dev/spectacles-native-sdk/vos-api-android/lib/libVOSPlatformGraphics.so"
    )
endif()

