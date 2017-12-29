# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)
project(face3d)
set(OPEN_CV_ROOT /Users/zhangcheng/OpenSourceHub/OpenCV-android-sdk )
include_directories(${OPEN_CV_ROOT}/sdk/native/jni/include)

# build native_app_glue as a static lib
include_directories(${ANDROID_NDK}/sources/android/native_app_glue)
add_library(app-glue STATIC
            ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

set(ndk_helper_dir ${ANDROID_NDK}/sources/android/ndk_helper)
set(ndk_helper_SRCS ${ndk_helper_dir}/gl3stub.c
                    ${ndk_helper_dir}/GLContext.cpp
                    ${ndk_helper_dir}/shader.cpp
                    ${ndk_helper_dir}/vecmath.cpp
                    ${ndk_helper_dir}/gestureDetector.cpp
                    ${ndk_helper_dir}/interpolator.cpp
                    ${ndk_helper_dir}/tapCamera.cpp
                    ${ndk_helper_dir}/JNIHelper.cpp
                    ${ndk_helper_dir}/perfMonitor.cpp)
include_directories(${ndk_helper_dir}
                    ${ANDROID_NDK}/sources/android/cpufeatures)




include_directories(include)
include_directories(eos/3rdparty/eigen)
include_directories(glm)
include_directories(load3dmodel/common)
include_directories(load3dmodel/modelAssimp)

include(dlib/cmake)
include(eos/cmake)



add_library(opencv_java3 SHARED IMPORTED)
add_library(assimp STATIC IMPORTED)
add_library(IrrXML STATIC IMPORTED)

set_target_properties(opencv_java3
                    PROPERTIES IMPORTED_LOCATION
                   ${PROJECT_SOURCE_DIR}/../../../src/main/jniLibs/${ANDROID_ABI}/libopencv_java3.so)
set_target_properties(assimp
                    PROPERTIES IMPORTED_LOCATION
                   ${PROJECT_SOURCE_DIR}/../../../src/main/jniLibs/${ANDROID_ABI}/libassimp.a)
set_target_properties(IrrXML
                    PROPERTIES IMPORTED_LOCATION
                   ${PROJECT_SOURCE_DIR}/../../../src/main/jniLibs/${ANDROID_ABI}/libIrrXML.a)


add_library( # Sets the name of the library.
             face3d

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
              ${ndk_helper_SRCS}

             load3dmodel/common/myJNIHelper.h
             load3dmodel/common/myJNIHelper.cpp

             load3dmodel/common/myShader.cpp
             load3dmodel/common/misc.cpp
             load3dmodel/common/myGLCamera.cpp
             load3dmodel/common/myGLFunctions.cpp
             load3dmodel/common/assimpLoader.cpp

             load3dmodel/modelAssimp/modelAssimp.cpp

             face3d.cpp




             )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )
find_library(jnigraphics-lib
             jnigraphics)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.


                       face3d
                       opencv_java3
                       assimp
                       IrrXML
                       dlib
                       eos
                       EGL
                       GLESv2
                       app-glue
                       android
                       ${log-lib}
                       ${jnigraphics-lib}
                       )