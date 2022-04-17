# Sets the minimum version of CMake required to build your native library.
# This ensures that a certain set of CMake features is available to
# your build.

cmake_minimum_required(VERSION 3.10.2)

project("JNI")


add_library( # Specifies the name of the library.
        Jnilib

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        test.cpp
        )

find_library( # Defines the name of the path variable that stores the
        # location of the NDK library.
        log-lib
        # Specifies the name of the NDK library that
        # CMake needs to locate.
        log)


target_link_libraries( # Specifies the target library.
        Jnilib
        ${log-lib})


