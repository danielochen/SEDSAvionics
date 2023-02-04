# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/danielchen/esp/esp-idf/components/bootloader/subproject"
  "/Users/danielchen/Documents/ESP32/hello_world/build/bootloader"
  "/Users/danielchen/Documents/ESP32/hello_world/build/bootloader-prefix"
  "/Users/danielchen/Documents/ESP32/hello_world/build/bootloader-prefix/tmp"
  "/Users/danielchen/Documents/ESP32/hello_world/build/bootloader-prefix/src/bootloader-stamp"
  "/Users/danielchen/Documents/ESP32/hello_world/build/bootloader-prefix/src"
  "/Users/danielchen/Documents/ESP32/hello_world/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/danielchen/Documents/ESP32/hello_world/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/danielchen/Documents/ESP32/hello_world/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
