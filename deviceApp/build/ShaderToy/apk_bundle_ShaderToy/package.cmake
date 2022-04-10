if(WIN32)
    set(GRADLEW /home/m4zz31/Snapchat/Dev/spectacles-native-sdk/vos-app-kit-android/resources\\gradlew.bat --no-daemon)
else()
    set(GRADLEW /home/m4zz31/Snapchat/Dev/spectacles-native-sdk/vos-app-kit-android/resources/gradlew)
endif()
if(ANDROID_AAR_ARCHIVE)
    set(GRADLE_WORKDIR "${CPACK_TEMPORARY_DIRECTORY}/aar")
else()
    set(GRADLE_WORKDIR "${CPACK_TEMPORARY_DIRECTORY}/apk")
endif()
execute_process(
    COMMAND ${GRADLEW} assembleRelease
    WORKING_DIRECTORY ${GRADLE_WORKDIR}
    RESULT_VARIABLE RETCODE
)
if (RETCODE)
    message(FATAL_ERROR "error building with gradle")
endif()
