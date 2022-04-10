mkdir build

mkdir latest_app
cp bashutils/run.sh build/.
cp bashutils/debug.sh build/.
cp bashutils/ENV.sh build/.

cd build

source ENV.sh

$CMAKE .. -GNinja \
    -DVOSVisionCore_DIR="$VOS_SDK_ROOT/vos-api-android/lib/cmake/VOSVisionCore" \
    -DVOSPlatformCalibration_DIR="$VOS_SDK_ROOT/vos-api-android/lib/cmake/VOSPlatformCalibration" \
    -DVOSPlatformSensors_DIR="$VOS_SDK_ROOT/vos-api-android/lib/cmake/VOSPlatformSensors" \
    -DVOSSlamCore_DIR="$VOS_SDK_ROOT/vos-api-android/lib/cmake/VOSSlamCore" \
    -DVOSPlatformGraphics_DIR="$VOS_SDK_ROOT/vos-api-android/lib/cmake/VOSPlatformGraphics" \
    -DVOSAppKit_DIR="$VOS_SDK_ROOT/vos-app-kit-android/lib/cmake/VOSAppKit" \
    -DCMAKE_BUILD_TYPE=Release \
    -DANDROID_ABI=arm64-v8a \
    -DANDROID_PLATFORM=android-28 \
    -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK_HOME/build/cmake/android.toolchain.cmake
