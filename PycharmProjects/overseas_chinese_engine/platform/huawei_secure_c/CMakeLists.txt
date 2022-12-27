# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

include_directories(./include)

set(all_native_sources
    src/fscanf_s.c
    src/fwscanf_s.c
    src/gets_s.c
    src/memcpy_s.c
    src/memmove_s.c
    src/memset_s.c
    src/scanf_s.c
    src/securecutil.c
    src/secureinput_a.c
    src/secureinput_w.c
    src/secureprintoutput_a.c
    src/secureprintoutput_w.c
    src/snprintf_s.c
    src/sprintf_s.c
    src/sscanf_s.c
    src/strcat_s.c
    src/strcpy_s.c
    src/strncat_s.c
    src/strncpy_s.c
    src/strtok_s.c
    src/swprintf_s.c
    src/swscanf_s.c
    src/vfscanf_s.c
    src/vfwscanf_s.c
    src/vscanf_s.c
    src/vsnprintf_s.c
    src/vsprintf_s.c
    src/vsscanf_s.c
    src/vswprintf_s.c
    src/vswscanf_s.c
    src/vwscanf_s.c
    src/wcscat_s.c
    src/wcscpy_s.c
    src/wcsncat_s.c
    src/wcsncpy_s.c
    src/wcstok_s.c
    src/wmemcpy_s.c
    src/wmemmove_s.c
    src/wscanf_s.c
)

message(STATUS "huawei_secure_c build")

add_library(huawei_secure_c STATIC
    ${all_native_sources}
)

target_include_directories(huawei_secure_c
    PUBLIC .
)
