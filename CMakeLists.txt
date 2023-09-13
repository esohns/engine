include (${CMAKE_CURRENT_SOURCE_DIR}/VersionConfig.cmake)

cmake_minimum_required (VERSION 2.8)

# build type
#variable_watch (CMAKE_BUILD_TYPE)
# *NOTE*: iff the user specifies -DCMAKE_BUILD_TYPE on the command line, take
#         their definition and dump it in the cache along with proper
#         documentation, otherwise set CMAKE_BUILD_TYPE to Debug prior to
#         calling PROJECT()
set (BUILD_TYPE_CACHE_STRING "choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel.")
if (DEFINED CMAKE_BUILD_TYPE)
 set (CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING ${BUILD_TYPE_CACHE_STRING})
elseif (NOT CMAKE_CONFIGURATION_TYPES)
 set (CMAKE_BUILD_TYPE Debug CACHE STRING ${BUILD_TYPE_CACHE_STRING})
endif ()

project (engine
         LANGUAGES CXX C)

if (UNIX)
 set (NEWLINE_STYLE UNIX)
elseif (WIN32)
 set (NEWLINE_STYLE WIN32)
else ()
 set (NEWLINE_STYLE UNIX)
endif ()

# project options
if (WIN32)
 if (CMAKE_BUILD_TYPE STREQUAL Debug)
  add_definitions (-D_ITERATOR_DEBUG_LEVEL=2)
 else ()
  add_definitions (-D_ITERATOR_DEBUG_LEVEL=0)
 endif (CMAKE_BUILD_TYPE STREQUAL Debug)
endif (WIN32)
if (CMAKE_BUILD_TYPE STREQUAL Debug)
 if (CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options (-g)
  add_compile_options (-O0)
  add_compile_options (-fno-inline)
 endif (CMAKE_COMPILER_IS_GNUCXX)
 add_definitions (-D_DEBUG)
#  set_property (SOURCE PROPERTY COMPILE_DEFINITIONS _DEBUG)
elseif (CMAKE_BUILD_TYPE STREQUAL Release)
 if (CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options (-O3)
 endif (CMAKE_COMPILER_IS_GNUCXX)
 add_definitions (-DACE_NDEBUG)
#  set_property (SOURCE PROPERTY COMPILE_DEFINITIONS ACE_NDEBUG)
endif ()

option (TRACING "enable tracing" OFF)
if (TRACING)
 add_definitions (-DENGINE_NTRACE=0)
#  set_property (SOURCE PROPERTY COMPILE_DEFINITIONS COMMON_NTRACE=0)
else ()
 add_definitions (-DENGINE_NTRACE=1)
#  set_property (SOURCE PROPERTY COMPILE_DEFINITIONS COMMON_NTRACE=1)
endif ()

#include (valgrind)

set (PACKAGE ${PROJECT_NAME})
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake.in
                ${CMAKE_CURRENT_BINARY_DIR}/config.h
                @ONLY
                NEWLINE_STYLE ${NEWLINE_STYLE})
add_definitions (-DHAVE_CONFIG_H)

configure_file (resource.rc.in
                resource.rc)

set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/../Common/scripts;${CMAKE_CURRENT_SOURCE_DIR}/../Common/src/gl/scripts")
include (glew)
#include (ffmpeg)
#if (NOT FFMPEG_SUPPORT)
# message (FATAL_ERROR "could not find ffmpeg, aborting")
#else ()
# message (STATUS "found ffmpeg")
#endif (NOT FFMPEG_SUPPORT)
include (ImageMagick)
if (NOT IMAGEMAGICK_SUPPORT)
 message (FATAL_ERROR "could not find ImageMagick, aborting")
else ()
 message (STATUS "found ImageMagick")
endif (NOT IMAGEMAGICK_SUPPORT)

include (ACE)
if (NOT ACE_LIBRARY)
 message (FATAL_ERROR "could not find ${ACE_LIB_FILE} (was: \"${ACE_LIBRARY}\"), aborting")
else ()
 message (STATUS "Found ACE library \"${ACE_LIBRARY}\"")
endif (NOT ACE_LIBRARY)

string (REPLACE ${CMAKE_SOURCE_DIR} "" BUILD_PATH_SUFFIX ${CMAKE_BINARY_DIR})
string (SUBSTRING ${BUILD_PATH_SUFFIX} 1 -1 BUILD_PATH_SUFFIX)
#message (STATUS "BUILD_PATH_SUFFIX \"${BUILD_PATH_SUFFIX}\"")
#string (FIND ${BUILD_PATH_SUFFIX} "/" SLASH_LOCATION REVERSE)
#string (SUBSTRING ${BUILD_PATH_SUFFIX} 0 ${SLASH_LOCATION} BUILD_PATH_SUFFIX)
#message (STATUS "BUILD_PATH_SUFFIX \"${BUILD_PATH_SUFFIX}\"")
if (UNIX)
 set (Common_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../Common/${BUILD_PATH_SUFFIX}/${CMAKE_BUILD_TYPE}")
endif (UNIX)
find_package (Common 0.0.2
              REQUIRED COMPONENTS lib
              CONFIG
              PATHS ${CMAKE_CURRENT_SOURCE_DIR}/..
              PATH_SUFFIXES Common/${BUILD_PATH_SUFFIX}
              NO_DEFAULT_PATH)
if (NOT EXISTS ${Common_DIR})
 message (FATAL_ERROR "could not find package \"Common\", aborting")
endif (NOT EXISTS ${Common_DIR})
if (UNIX)
 set (COMMON_ROOT_DIR "${Common_DIR}/../../.." )
elseif (WIN32)
 set (COMMON_ROOT_DIR "${Common_DIR}/../.." )
endif ()
#message (STATUS "\"COMMON_ROOT_DIR\": ${COMMON_ROOT_DIR}")

if (DEFINED ENV{LIB_ROOT})
 set (PGE_DIR "$ENV{LIB_ROOT}/olcPixelGameEngine" )
endif (DEFINED ENV{LIB_ROOT})
find_path (PGE_INCLUDE_DIR
           olcPixelGameEngine.h
           HINTS ${PGE_DIR})

if (WIN32)
# set (CMAKE_BUILD_TYPE $<CONFIG> CACHE STRING "build type" FORCE)
endif (WIN32)
if ($<CONFIG> STREQUAL "Debug" OR
    $<CONFIG> STREQUAL "RelWithDebInfo" OR
    ${CMAKE_BUILD_TYPE} STREQUAL "Debug" OR
    ${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
 set (LIB_FILE_SUFFIX "d" CACHE STRING "lib file suffix" FORCE)
else ()
 set (LIB_FILE_SUFFIX "" CACHE STRING "lib file suffix" FORCE)
endif ()

if (DEFINED ENV{LIB_ROOT})
 set (LIBNOISE_DIR "$ENV{LIB_ROOT}/libnoise" )
endif (DEFINED ENV{LIB_ROOT})
set (LIBNOISE_INCLUDES "${LIBNOISE_DIR}/include")
if (UNIX)
 set (LIBNOISE_LIBRARY "libnoise.so")
 find_library (LIBNOISE_LIB ${LIBNOISE_LIBRARY}
               PATHS ${LIBNOISE_DIR}
               PATH_SUFFIXES build/gcc
               DOC "libnoise library"
               REQUIRED
               NO_DEFAULT_PATH)
elseif (WIN32)
 set (LIBNOISE_LIBRARY "noise.lib")
 find_library (LIBNOISE_LIB ${LIBNOISE_LIBRARY}
               PATHS ${LIBNOISE_DIR}
               PATH_SUFFIXES build/msvc/${CMAKE_BUILD_TYPE}
               DOC "libnoise library"
               REQUIRED
               NO_DEFAULT_PATH)
endif () 
set (LIBNOISE_LIB_DIR "${LIBNOISE_DIR}/build/msvc/${CMAKE_BUILD_TYPE}")

if (UNIX)
 set (GLUT_LIB_FILE libglut.so)
 find_library (GLUT_LIBRARY ${GLUT_LIB_FILE}
               PATHS /usr /usr/lib
               PATH_SUFFIXES lib64 x86_64-linux-gnu
               DOC "searching for ${GLUT_LIB_FILE}"
               REQUIRED)
 if (GLUT_LIBRARY)
  message (STATUS "Found Glut library \"${GLUT_LIBRARY}\"")
  set (GLUT_FOUND TRUE)
#   set (GLUT_INCLUDE_DIR "/usr/include")
  set (GLUT_LIBRARIES "${GLUT_LIBRARY}")
 else ()
  message (WARNING "could not find ${GLUT_LIB_FILE}, continuing")
 endif (GLUT_LIBRARY)
elseif (WIN32)
 set (GLUT_LIB_FILE freeglut.lib)
 set (BIN_PATH_SUFFIX "bin")
 set (LIB_PATH_BASE "freeglut")
 set (LIB_PATH_SUFFIX "lib")
 if (${CMAKE_VS_PLATFORM_TOOLSET_HOST_ARCHITECTURE} STREQUAL "x64")
  set (BIN_PATH_SUFFIX "${BIN_PATH_SUFFIX}/x64")
  set (LIB_PATH_BASE "${LIB_PATH_BASE}/lib")
  set (LIB_PATH_SUFFIX "x64")
 endif (${CMAKE_VS_PLATFORM_TOOLSET_HOST_ARCHITECTURE} STREQUAL "x64")
 find_library (GLUT_LIBRARY ${GLUT_LIB_FILE}
               PATHS $ENV{LIB_ROOT}/${LIB_PATH_BASE}
               PATH_SUFFIXES ${LIB_PATH_SUFFIX}
               DOC "searching for ${GLUT_LIB_FILE}")
 if (GLUT_LIBRARY)
  message (STATUS "Found Glut library \"${GLUT_LIBRARY}\"")
  set (GLUT_INCLUDE_DIRS "$ENV{LIB_ROOT}/freeglut/include")
  set (GLUT_LIBRARIES "${GLUT_LIBRARY}")
  set (GLUT_LIB_DIR "$ENV{LIB_ROOT}/freeglut/${BIN_PATH_SUFFIX}")
 else ()
  message (WARNING "could not find ${GLUT_LIB_FILE}, continuing")
 endif (GLUT_LIBRARY)
endif () 
add_definitions (-DGLUT_SUPPORT)

if (UNIX)
 include (FindPkgConfig)
 pkg_check_modules (PKG_ASSIMP assimp)
 if (PKG_ASSIMP_FOUND)
  if (CMAKE_SYSTEM_NAME MATCHES "Linux")
   include (OS)
   if (${LSB_RELEASE_ID_SHORT} MATCHES "Fedora")
# *WORKAROUND*: 'prefix' variable must be prepended to 'includedir'
    pkg_get_variable (PKG_ASSIMP_PREFIX assimp prefix)
    set (ASSIMP_INCLUDE_DIRS "${PKG_ASSIMP_PREFIX}/${PKG_ASSIMP_INCLUDE_DIRS}")
   elseif (${LSB_RELEASE_ID_SHORT} MATCHES "Ubuntu")
# *WORKAROUND*: there seems to be a problem on Ubuntu
    set (ASSIMP_INCLUDE_DIRS "/usr/include/assimp")
   endif ()
  endif (CMAKE_SYSTEM_NAME MATCHES "Linux")
  set (ASSIMP_LIBRARIES ${PKG_ASSIMP_LIBRARIES})
 endif (PKG_ASSIMP_FOUND)
# find_library (ASSIMP_LIBRARY libassimp.so.3
#               DOC "searching for libassimp")
# if (NOT ASSIMP_LIBRARY)
#  message (FATAL_ERROR "could not find libassimp, aborting")
# endif (NOT ASSIMP_LIBRARY)
elseif (WIN32)
 if (NOT ASSIMP_FOUND)
  set (ASSIMP_INCLUDE_DIRS "$ENV{LIB_ROOT}/assimp/include;$ENV{LIB_ROOT}/assimp/cmake/include")
  set (LIB_WITH_SUFFIX "assimp-vc${MSVC_TOOLSET_VERSION}-mt")
  if (CMAKE_BUILD_TYPE STREQUAL "Debug" OR
      CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
   set (LIB_WITH_SUFFIX "${LIB_WITH_SUFFIX}d")
  set (LIB_WITH_SUFFIX "${LIB_WITH_SUFFIX}.lib")
  endif (CMAKE_BUILD_TYPE STREQUAL "Debug" OR
         CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
  set (ASSIMP_LIBRARIES "$ENV{LIB_ROOT}/assimp/cmake/lib/${CMAKE_BUILD_TYPE}/${LIB_WITH_SUFFIX}")
  set (ASSIMP_LIB_DIR "$ENV{LIB_ROOT}/assimp/cmake/bin/${CMAKE_BUILD_TYPE}")
 endif (NOT ASSIMP_FOUND)
endif ()

if (UNIX)
# include (FindPkgConfig)
# pkg_check_modules (PKG_LIBPNG libpng)
 find_library (PNG_LIBRARY libpng16.so.16
               DOC "searching for libpng")
 if (NOT PNG_LIBRARY)
  message (WARNING "could not find libpng, continuing")
 else ()
  message (STATUS "Found libpng \"${PNG_LIBRARY}\"")
  set (PNG_LIBRARIES "${PNG_LIBRARY}")
 endif (NOT PNG_LIBRARY)
elseif (WIN32)
 if (NOT PNG_FOUND)
  set (PNG_INCLUDE_DIRS "$ENV{LIB_ROOT}/libpng/build;$ENV{LIB_ROOT}/libpng")
  set (PNG_LIBRARIES "$ENV{LIB_ROOT}/libpng/build/${CMAKE_BUILD_TYPE}/libpng16${LIB_FILE_SUFFIX}.lib")
  set (PNG_LIB_DIR "$ENV{LIB_ROOT}/zlib/build/${CMAKE_BUILD_TYPE};$ENV{LIB_ROOT}/libpng/build/${CMAKE_BUILD_TYPE}")
 endif (NOT PNG_FOUND)
endif ()

if (UNIX)
 pkg_check_modules (PKG_FFMPEG libavcodec libavformat libswscale libavutil)
 if (PKG_FFMPEG_FOUND)
  set (ffmpeg_INCLUDE_DIRS "${PKG_FFMPEG_INCLUDE_DIRS}")
  set (ffmpeg_LIBRARIES "${PKG_FFMPEG_LIBRARIES}")
 endif (PKG_FFMPEG_FOUND)
elseif (WIN32)
 if (NOT FFMPEG_FOUND)
  if (EXISTS $ENV{FFMPEG_ROOT})
   set (FFMPEG_ROOT $ENV{FFMPEG_ROOT})
  else ()
   set (FFMPEG_ROOT $ENV{LIB_ROOT}/ffmpeg)
  endif (EXISTS $ENV{FFMPEG_ROOT})
  find_library (FFMPEG_LIBRARY_AVCODEC avcodec.lib
                PATHS ${FFMPEG_ROOT}
                PATH_SUFFIXES libavcodec lib
                DOC "searching for avcodec.lib"
                NO_DEFAULT_PATH)
  find_library (FFMPEG_LIBRARY_AVFORMAT avformat.lib
                PATHS ${FFMPEG_ROOT}
                PATH_SUFFIXES libavformat lib
                DOC "searching for avformat.lib"
                NO_DEFAULT_PATH)
  find_library (FFMPEG_LIBRARY_SWSCALE swscale.lib
                PATHS ${FFMPEG_ROOT}
                PATH_SUFFIXES libswscale lib
                DOC "searching for swscale.lib"
                NO_DEFAULT_PATH)
  find_library (FFMPEG_LIBRARY_AVUTIL avutil.lib
                PATHS ${FFMPEG_ROOT}
                PATH_SUFFIXES libavutil lib
                DOC "searching for avutil.lib"
                NO_DEFAULT_PATH)
  if (NOT FFMPEG_LIBRARY_AVCODEC)
   message (WARNING "could not find avcodec.lib, continuing")
  else ()
   message (STATUS "Found avcodec.lib library \"${FFMPEG_LIBRARY_AVCODEC}\"")
  endif (NOT FFMPEG_LIBRARY_AVCODEC)
  if (NOT FFMPEG_LIBRARY_AVFORMAT)
   message (WARNING "could not find avformat.lib, continuing")
  else ()
   message (STATUS "Found avformat.lib library \"${FFMPEG_LIBRARY_AVFORMAT}\"")
  endif (NOT FFMPEG_LIBRARY_AVFORMAT)
  if (NOT FFMPEG_LIBRARY_SWSCALE)
   message (WARNING "could not find swscale.lib, continuing")
  else ()
   message (STATUS "Found swscale.lib library \"${FFMPEG_LIBRARY_SWSCALE}\"")
  endif (NOT FFMPEG_LIBRARY_SWSCALE)
  if (NOT FFMPEG_LIBRARY_AVUTIL)
   message (WARNING "could not find avutil.lib, continuing")
  else ()
   message (STATUS "Found avutil.lib library \"${FFMPEG_LIBRARY_AVUTIL}\"")
  endif (NOT FFMPEG_LIBRARY_AVUTIL)
  if (FFMPEG_LIBRARY_AVCODEC AND FFMPEG_LIBRARY_AVFORMAT AND FFMPEG_LIBRARY_SWSCALE AND FFMPEG_LIBRARY_AVUTIL)
   set (ffmpeg_LIBRARIES "${FFMPEG_LIBRARY_AVCODEC};${FFMPEG_LIBRARY_AVFORMAT};${FFMPEG_LIBRARY_SWSCALE};${FFMPEG_LIBRARY_AVUTIL}")
   if (EXISTS $ENV{FFMPEG_ROOT})
    set (ffmpeg_INCLUDE_DIRS "${FFMPEG_ROOT}")
    set (ffmpeg_LIB_DIR "${FFMPEG_ROOT}/libavcodec;${FFMPEG_ROOT}/libavformat;${FFMPEG_ROOT}/libswscale;${FFMPEG_ROOT}/libswresample;${FFMPEG_ROOT}/libavutil" CACHE INTERNAL "")
#    message (STATUS "ffmpeg_LIB_DIR: ${ffmpeg_LIB_DIR}")
   else ()
    set (ffmpeg_INCLUDE_DIRS "${FFMPEG_ROOT}/include")
    set (ffmpeg_LIB_DIR "${FFMPEG_ROOT}")
   endif (EXISTS $ENV{FFMPEG_ROOT})
  endif (FFMPEG_LIBRARY_AVCODEC AND FFMPEG_LIBRARY_AVFORMAT AND FFMPEG_LIBRARY_SWSCALE AND FFMPEG_LIBRARY_AVUTIL)
 endif (NOT FFMPEG_FOUND)
endif ()

if (UNIX)
 find_library (BOX2D_LIBRARY liquidfun
               PATHS $ENV{LIB_ROOT}/liquidfun/build/gcc/Box2D
               PATH_SUFFIXES Debug
               DOC "searching for liquidfun"
               NO_DEFAULT_PATH)
 if (NOT BOX2D_LIBRARY)
  message (WARNING "could not find libliquidfun.a, continuing")
 endif (NOT BOX2D_LIBRARY)
 if (BOX2D_LIBRARY)
  message (STATUS "found Box2D")
  set (BOX2D_FOUND TRUE)
  set (BOX2D_INCLUDE_DIR "$ENV{LIB_ROOT}/liquidfun/liquidfun/Box2D")
  set (BOX2D_LIBRARIES ${BOX2D_LIBRARY})
#  set (BOX2D_LIB_DIR "$ENV{LIB_ROOT}/liquidfun/build/msvc/Box2D/Debug")
 endif (BOX2D_LIBRARY)
elseif (WIN32)
 find_library (BOX2D_LIBRARY liquidfun.lib
               PATHS $ENV{LIB_ROOT}/liquidfun/build/msvc/Box2D
               PATH_SUFFIXES Debug
               DOC "searching for liquidfun.lib"
               NO_DEFAULT_PATH)
 if (NOT BOX2D_LIBRARY)
  message (WARNING "could not find liquidfun.lib, continuing")
 endif (NOT BOX2D_LIBRARY)
 if (BOX2D_LIBRARY)
  message (STATUS "found Box2D")
  set (BOX2D_FOUND TRUE)
  set (BOX2D_INCLUDE_DIR "$ENV{LIB_ROOT}/liquidfun/liquidfun/Box2D")
  set (BOX2D_LIBRARIES ${BOX2D_LIBRARY})
#  set (BOX2D_LIB_DIR "$ENV{LIB_ROOT}/liquidfun/build/msvc/Box2D/Debug")
 endif (BOX2D_LIBRARY)
endif ()

# user interface
option (GUI_SUPPORT "enable graphical user interface support" ON)
if (GUI_SUPPORT)
 add_definitions (-DGUI_SUPPORT)
 set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${COMMON_ROOT_DIR}/src/ui/scripts")
 include (PreUIFramework)
 include (SetUIFramework)
 if (GTK_SUPPORT)
  include (SetGTKVersion)
 endif (GTK_SUPPORT)
endif (GUI_SUPPORT)

#set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${COMMON_ROOT_DIR}/src/gl/scripts")
include (glm)

############## includes ###############
include_directories (${BOX2D_INCLUDE_DIR})
include_directories (${PGE_INCLUDE_DIR})
include_directories (${PGE_INCLUDE_DIR}/extensions)
include_directories (${LIBNOISE_INCLUDES})
include_directories (${GLUT_INCLUDE_DIRS})
include_directories (${GLEW_INCLUDE_DIRS})
include_directories (${GTK_INCLUDE_DIRS})

include_directories (${ACE_INCLUDE_DIR})

include_directories (${COMMON_ROOT_DIR})
include_directories (${COMMON_ROOT_DIR}/src)
include_directories (${COMMON_ROOT_DIR}/src/gl)
include_directories (${COMMON_ROOT_DIR}/src/log)
include_directories (${COMMON_ROOT_DIR}/src/math)
include_directories (${COMMON_ROOT_DIR}/src/timer)
include_directories (${COMMON_ROOT_DIR}/src/ui)
include_directories (${COMMON_ROOT_DIR}/src/ui/gtk)

include_directories (${CMAKE_CURRENT_BINARY_DIR})

############## next target ###############
set (engine_EXE_SRCS
     ${CMAKE_CURRENT_BINARY_DIR}/config.h
     ${CMAKE_CURRENT_BINARY_DIR}/resource.rc
#     ${CMAKE_CURRENT_BINARY_DIR}/ImportToLibreOffice.dir/Debug/resource.res
     camera.cpp
     camera.h
     engine_common.h
     defines.h
     main.cpp
     pge.cpp
     pge.h
     pge_2.cpp
     pge_2.h
     glut_3.cpp
     glut_3.h
     glut_4.cpp
     glut_4.h
     pge_5.cpp
     pge_5.h
     pge_6.cpp
     pge_6.h
     pge_6_2.cpp
     pge_6_2.h
     pge_7.cpp
     pge_7.h
     pge_8.cpp
     pge_8.h
     pge_9.cpp
     pge_9.h
     pge_10.cpp
     pge_10.h
     pge_11.cpp
     pge_11.h
     pge_12.cpp
     pge_12.h
     pge_13.cpp
     pge_13.h
     pge_14.cpp
     pge_14.h
     pge_15.cpp
     pge_15.h
     glut_16.cpp
     glut_16.h
     pge_17.cpp
     pge_17.h
     glut_18.cpp
     glut_18.h
     pge_19.cpp
     pge_19.h
     pge_20.cpp
     pge_20.h
     pge_21.cpp
     pge_21.h
     pge_22.cpp
     pge_22.h
     pge_23.cpp
     pge_23.h
     pge_24.cpp
     pge_24.h
     glut_25.cpp
     glut_25.h
     pge_26.cpp
     pge_26.h
     pge_27.cpp
     pge_27.h
     pge_28.cpp
     pge_28.h
     pge_29.cpp
     pge_29.h
     stdafx.h)

#cmake_policy (SET CMP0065 OLD) # do not add flags to export symbols from executables without the ENABLE_EXPORTS target property
set (EXECUTABLE engine)
add_executable (${EXECUTABLE}
                ${engine_EXE_SRCS})

set (engine_2_EXE_SRCS
     ${CMAKE_CURRENT_BINARY_DIR}/config.h
     ${CMAKE_CURRENT_BINARY_DIR}/resource.rc
#     ${CMAKE_CURRENT_BINARY_DIR}/ImportToLibreOffice.dir/Debug/resource.res
     camera.cpp
     camera.h
     engine_common.h
     defines_2.h
     main_2.cpp
     pge_30.cpp
     pge_30.h
     pge_31.cpp
     pge_31.h
     pge_32.cpp
     pge_32.h
     pge_33.cpp
     pge_33.h
     pge_34.cpp
     pge_34.h
     pge_35.cpp
     pge_35.h
     pge_36.cpp
     pge_36.h
     pge_37.cpp
     pge_37.h
     pge_38.cpp
     pge_38.h
     pge_39.cpp
     pge_39.h
     pge_40.cpp
     pge_40.h
     pge_41.cpp
     pge_41.h
     pge_42.cpp
     pge_42.h
     pge_43.cpp
     pge_43.h
     pge_44.cpp
     pge_44.h
     pge_45.cpp
     pge_45.h
     pge_46.cpp
     pge_46.h
     pge_47.cpp
     pge_47.h
     pge_48.cpp
     pge_48.h
     pge_49.cpp
     pge_49.h
     pge_50.cpp
     pge_50.h
     pge_51.cpp
     pge_51.h
     pge_52.cpp
     pge_52.h
     pge_53.cpp
     pge_53.h
     pge_54.cpp
     pge_54.h
     pge_55.cpp
     pge_55.h
     pge_56.cpp
     pge_56.h
     pge_57.cpp
     pge_57.h
     pge_58.cpp
     pge_58.h
     pge_59.cpp
     pge_59.h
     stdafx.h)

set (EXECUTABLE_2 engine_2)
add_executable (${EXECUTABLE_2}
                ${engine_2_EXE_SRCS})

set (engine_3_EXE_SRCS
     ${CMAKE_CURRENT_BINARY_DIR}/config.h
     ${CMAKE_CURRENT_BINARY_DIR}/resource.rc
#     ${CMAKE_CURRENT_BINARY_DIR}/ImportToLibreOffice.dir/Debug/resource.res
     camera.cpp
     camera.h
     engine_common.h
     defines_3.h
     main_3.cpp
     pge_60.cpp
     pge_60.h
     pge_61.cpp
     pge_61.h
     pge_62.cpp
     pge_62.h
     pge_63.cpp
     pge_63.h
     pge_64.cpp
     pge_64.h
     pge_65.cpp
     pge_65.h
     pge_66.cpp
     pge_66.h
     pge_67.cpp
     pge_67.h
     pge_68.cpp
     pge_68.h
     pge_69.cpp
     pge_69.h
     pge_70.cpp
     pge_70.h
     pge_71.cpp
     pge_71.h
     pge_72.cpp
     pge_72.h
     pge_73.cpp
     pge_73.h
     pge_74.cpp
     pge_74.h
     pge_75.cpp
     pge_75.h
     pge_76.cpp
     pge_76.h
     pge_77.cpp
     pge_77.h
     pge_78.cpp
     pge_78.h
     pge_79.cpp
     pge_79.h
     pge_80.cpp
     pge_80.h
     pge_81.cpp
     pge_81.h
     pge_82.cpp
     pge_82.h
     pge_83.cpp
     pge_83.h
     pge_84.cpp
     pge_84.h
     pge_85.cpp
     pge_85.h
     pge_86.cpp
     pge_86.h
     pge_87.cpp
     pge_87.h
     pge_88.cpp
     pge_88.h
     pge_89.cpp
     pge_89.h
     stdafx.h)

set (EXECUTABLE_3 engine_3)
add_executable (${EXECUTABLE_3}
                ${engine_3_EXE_SRCS})

set (engine_4_EXE_SRCS
     ${CMAKE_CURRENT_BINARY_DIR}/config.h
     ${CMAKE_CURRENT_BINARY_DIR}/resource.rc
#     ${CMAKE_CURRENT_BINARY_DIR}/ImportToLibreOffice.dir/Debug/resource.res
     camera.cpp
     camera.h
     engine_common.h
     defines_4.h
     main_4.cpp
     pge_90.cpp
     pge_90.h
     pge_91.cpp
     pge_91.h
     pge_92.cpp
     pge_92.h
     pge_93.cpp
     pge_93.h
     pge_94.cpp
     pge_94.h
     pge_95.cpp
     pge_95.h
     pge_96.cpp
     pge_96.h
     pge_97.cpp
     pge_97.h
     pge_98.cpp
     pge_98.h
     pge_99.cpp
     pge_99.h
     pge_100.cpp
     pge_100.h
     pge_101.cpp
     pge_101.h
     pge_102.cpp
     pge_102.h
     pge_103.cpp
     pge_103.h
     pge_104.cpp
     pge_104.h
     pge_105.cpp
     pge_105.h
     pge_106.cpp
     pge_106.h
     pge_107.cpp
     pge_107.h
     pge_108.cpp
     pge_108.h
     pge_109.cpp
     pge_109.h
     pge_110.cpp
     pge_110.h
     pge_111.cpp
     pge_111.h
     pge_112.cpp
     pge_112.h
     pge_113.cpp
     pge_113.h
     pge_114.cpp
     pge_114.h
     pge_115.cpp
     pge_115.h
     pge_116.cpp
     pge_116.h
     pge_117.cpp
     pge_117.h
     pge_118.cpp
     pge_118.h
     pge_119.cpp
     pge_119.h
     stdafx.h)

set (EXECUTABLE_4 engine_4)
add_executable (${EXECUTABLE_4}
                ${engine_4_EXE_SRCS})

set (engine_5_EXE_SRCS
     ${CMAKE_CURRENT_BINARY_DIR}/config.h
     ${CMAKE_CURRENT_BINARY_DIR}/resource.rc
#     ${CMAKE_CURRENT_BINARY_DIR}/ImportToLibreOffice.dir/Debug/resource.res
     camera.cpp
     camera.h
     engine_common.h
     defines_5.h
     main_5.cpp
     opensimplexnoise.h
     pge_120.cpp
     pge_120.h
     pge_121.cpp
     pge_121.h
     pge_122.cpp
     pge_122.h
     pge_123.cpp
     pge_123.h
     pge_124.cpp
     pge_124.h
     pge_125.cpp
     pge_125.h
     pge_126.cpp
     pge_126.h
     pge_127.cpp
     pge_127.h
     pge_128.cpp
     pge_128.h
     pge_129.cpp
     pge_129.h
     pge_130.cpp
     pge_130.h
     pge_131.cpp
     pge_131.h
     glut_132.cpp
     glut_132.h
     pge_133.cpp
     pge_133.h
     pge_134.cpp
     pge_134.h
     pge_135.cpp
     pge_135.h
     pge_136.cpp
     pge_136.h
     pge_137.cpp
     pge_137.h
     pge_138.cpp
     pge_138.h
     pge_139.cpp
     pge_139.h
     stdafx.h)

set (EXECUTABLE_5 engine_5)
add_executable (${EXECUTABLE_5}
                ${engine_5_EXE_SRCS})

set (engine_6_EXE_SRCS
     ${CMAKE_CURRENT_BINARY_DIR}/config.h
     ${CMAKE_CURRENT_BINARY_DIR}/resource.rc
#     ${CMAKE_CURRENT_BINARY_DIR}/ImportToLibreOffice.dir/Debug/resource.res
     camera.cpp
     camera.h
     engine_common.h
     defines_6.h
     main_6.cpp
     pge_140.cpp
     pge_140.h
     pge_141.cpp
     pge_141.h
     pge_142.cpp
     pge_142.h
     pge_143.cpp
     pge_143.h
     pge_144.cpp
     pge_144.h
     pge_145.cpp
     pge_145.h
     pge_146.cpp
     pge_146.h
     pge_147.cpp
     pge_147.h
     pge_148.cpp
     pge_148.h
     pge_149.cpp
     pge_149.h
     pge_150.cpp
     pge_150.h
     pge_151.cpp
     pge_151.h
     pge_152.cpp
     pge_152.h
     pge_153.cpp
     pge_153.h
     pge_154.cpp
     pge_154.h
     pge_155.cpp
     pge_155.h
     pge_156.cpp
     pge_156.h
     pge_157.cpp
     pge_157.h
     pge_158.cpp
     pge_158.h
     pge_159.cpp
     pge_159.h
     stdafx.h)

set (EXECUTABLE_6 engine_6)
add_executable (${EXECUTABLE_6}
                ${engine_6_EXE_SRCS})

set (engine_7_EXE_SRCS
     ${CMAKE_CURRENT_BINARY_DIR}/config.h
     ${CMAKE_CURRENT_BINARY_DIR}/resource.rc
#     ${CMAKE_CURRENT_BINARY_DIR}/ImportToLibreOffice.dir/Debug/resource.res
     camera.cpp
     camera.h
     engine_common.h
     defines_7.h
     main_7.cpp
     pge_160.cpp
     pge_160.h
     pge_161.cpp
     pge_161.h
     pge_162.cpp
     pge_162.h
     pge_163.cpp
     pge_163.h
     pge_164.cpp
     pge_164.h
     pge_165.cpp
     pge_165.h
     pge_166.cpp
     pge_166.h
     pge_167.cpp
     pge_167.h
     pge_168.cpp
     pge_168.h
     glut_169.cpp
     glut_169.h
     pge_170.cpp
     pge_170.h
     pge_171.cpp
     pge_171.h
     pge_172.cpp
     pge_172.h
     pge_173.cpp
     pge_173.h
     pge_174.cpp
     pge_174.h
     pge_175.cpp
     pge_175.h
     pge_176.cpp
     pge_176.h
     pge_177.cpp
     pge_177.h
     pge_178.cpp
     pge_178.h
     pge_179.cpp
     pge_179.h
     stdafx.h)

set (EXECUTABLE_7 engine_7)
add_executable (${EXECUTABLE_7}
                ${engine_7_EXE_SRCS})

set (engine_8_EXE_SRCS
     ${CMAKE_CURRENT_BINARY_DIR}/config.h
     ${CMAKE_CURRENT_BINARY_DIR}/resource.rc
#     ${CMAKE_CURRENT_BINARY_DIR}/ImportToLibreOffice.dir/Debug/resource.res
     camera.cpp
     camera.h
     engine_common.h
     defines_8.h
     main_8.cpp
     pge_180.cpp
     pge_180.h
     pge_181.cpp
     pge_181.h
     pge_182.cpp
     pge_182.h
     pge_183.cpp
     pge_183.h
     pge_184.cpp
     pge_184.h
     pge_185.cpp
     pge_185.h
     pge_186.cpp
     pge_186.h
     stdafx.h)

set (EXECUTABLE_8 engine_8)
add_executable (${EXECUTABLE_8}
                ${engine_8_EXE_SRCS})

if (UNIX)
 target_link_libraries (${EXECUTABLE} X11)
 target_link_libraries (${EXECUTABLE} GL)
 target_link_libraries (${EXECUTABLE} png)

 target_link_libraries (${EXECUTABLE_2} X11)
 target_link_libraries (${EXECUTABLE_2} GL)
 target_link_libraries (${EXECUTABLE_2} png)

 target_link_libraries (${EXECUTABLE_3} X11)
 target_link_libraries (${EXECUTABLE_3} GL)
 target_link_libraries (${EXECUTABLE_3} png)

 target_link_libraries (${EXECUTABLE_4} X11)
 target_link_libraries (${EXECUTABLE_4} GL)
 target_link_libraries (${EXECUTABLE_4} png)

 target_link_libraries (${EXECUTABLE_5} X11)
 target_link_libraries (${EXECUTABLE_5} GL)
 target_link_libraries (${EXECUTABLE_5} png)

 target_link_libraries (${EXECUTABLE_6} X11)
 target_link_libraries (${EXECUTABLE_6} GL)
 target_link_libraries (${EXECUTABLE_6} png)

 target_link_libraries (${EXECUTABLE_7} X11)
 target_link_libraries (${EXECUTABLE_7} GL)
 target_link_libraries (${EXECUTABLE_7} png)

 target_link_libraries (${EXECUTABLE_8} X11)
 target_link_libraries (${EXECUTABLE_8} GL)
 target_link_libraries (${EXECUTABLE_8} png)
endif ()
target_link_libraries (${EXECUTABLE} ${BOX2D_LIBRARIES})
target_link_libraries (${EXECUTABLE} ${LIBNOISE_LIB})
target_link_libraries (${EXECUTABLE} ${GLUT_LIBRARIES})
target_link_libraries (${EXECUTABLE} ${GTK_LIBRARIES})

target_link_libraries (${EXECUTABLE} ${ACE_LIBRARY})

target_link_libraries (${EXECUTABLE} Common)
target_link_libraries (${EXECUTABLE} CommonError)
target_link_libraries (${EXECUTABLE} CommonLog)
target_link_libraries (${EXECUTABLE} CommonOpenGL)
target_link_libraries (${EXECUTABLE} CommonTimer)
target_link_libraries (${EXECUTABLE} CommonUI)
target_link_libraries (${EXECUTABLE} CommonUI_GTK)

target_link_libraries (${EXECUTABLE_2} ${BOX2D_LIBRARIES})
target_link_libraries (${EXECUTABLE_2} ${LIBNOISE_LIB})
target_link_libraries (${EXECUTABLE_2} ${GLUT_LIBRARIES})
target_link_libraries (${EXECUTABLE_2} ${GTK_LIBRARIES})

target_link_libraries (${EXECUTABLE_2} ${ACE_LIBRARY})

target_link_libraries (${EXECUTABLE_2} Common)
target_link_libraries (${EXECUTABLE_2} CommonError)
target_link_libraries (${EXECUTABLE_2} CommonLog)
target_link_libraries (${EXECUTABLE_2} CommonOpenGL)
target_link_libraries (${EXECUTABLE_2} CommonTimer)
target_link_libraries (${EXECUTABLE_2} CommonUI)
target_link_libraries (${EXECUTABLE_2} CommonUI_GTK)

target_link_libraries (${EXECUTABLE_3} ${BOX2D_LIBRARIES})
target_link_libraries (${EXECUTABLE_3} ${LIBNOISE_LIB})
target_link_libraries (${EXECUTABLE_3} ${GLUT_LIBRARIES})
target_link_libraries (${EXECUTABLE_3} ${GTK_LIBRARIES})

target_link_libraries (${EXECUTABLE_3} ${ACE_LIBRARY})

target_link_libraries (${EXECUTABLE_3} Common)
target_link_libraries (${EXECUTABLE_3} CommonError)
target_link_libraries (${EXECUTABLE_3} CommonLog)
target_link_libraries (${EXECUTABLE_3} CommonOpenGL)
target_link_libraries (${EXECUTABLE_3} CommonTimer)
target_link_libraries (${EXECUTABLE_3} CommonUI)
target_link_libraries (${EXECUTABLE_3} CommonUI_GTK)

target_link_libraries (${EXECUTABLE_4} ${BOX2D_LIBRARIES})
target_link_libraries (${EXECUTABLE_4} ${LIBNOISE_LIB})
target_link_libraries (${EXECUTABLE_4} ${GLUT_LIBRARIES})
target_link_libraries (${EXECUTABLE_4} ${GTK_LIBRARIES})

target_link_libraries (${EXECUTABLE_4} ${ACE_LIBRARY})

target_link_libraries (${EXECUTABLE_4} Common)
target_link_libraries (${EXECUTABLE_4} CommonError)
target_link_libraries (${EXECUTABLE_4} CommonLog)
target_link_libraries (${EXECUTABLE_4} CommonOpenGL)
target_link_libraries (${EXECUTABLE_4} CommonTimer)
target_link_libraries (${EXECUTABLE_4} CommonUI)
target_link_libraries (${EXECUTABLE_4} CommonUI_GTK)

target_link_libraries (${EXECUTABLE_5} ${BOX2D_LIBRARIES})
target_link_libraries (${EXECUTABLE_5} ${LIBNOISE_LIB})
target_link_libraries (${EXECUTABLE_5} ${GLUT_LIBRARIES})
target_link_libraries (${EXECUTABLE_5} ${GTK_LIBRARIES})

target_link_libraries (${EXECUTABLE_5} ${ACE_LIBRARY})

target_link_libraries (${EXECUTABLE_5} Common)
target_link_libraries (${EXECUTABLE_5} CommonError)
target_link_libraries (${EXECUTABLE_5} CommonLog)
target_link_libraries (${EXECUTABLE_5} CommonOpenGL)
target_link_libraries (${EXECUTABLE_5} CommonTimer)
target_link_libraries (${EXECUTABLE_5} CommonUI)
target_link_libraries (${EXECUTABLE_5} CommonUI_GTK)

target_link_libraries (${EXECUTABLE_6} ${BOX2D_LIBRARIES})
target_link_libraries (${EXECUTABLE_6} ${LIBNOISE_LIB})
target_link_libraries (${EXECUTABLE_6} ${GLUT_LIBRARIES})
target_link_libraries (${EXECUTABLE_6} ${GTK_LIBRARIES})

target_link_libraries (${EXECUTABLE_6} ${ACE_LIBRARY})

target_link_libraries (${EXECUTABLE_6} Common)
target_link_libraries (${EXECUTABLE_6} CommonError)
target_link_libraries (${EXECUTABLE_6} CommonLog)
target_link_libraries (${EXECUTABLE_6} CommonOpenGL)
target_link_libraries (${EXECUTABLE_6} CommonTimer)
target_link_libraries (${EXECUTABLE_6} CommonUI)
target_link_libraries (${EXECUTABLE_6} CommonUI_GTK)

target_link_libraries (${EXECUTABLE_7} ${BOX2D_LIBRARIES})
target_link_libraries (${EXECUTABLE_7} ${LIBNOISE_LIB})
target_link_libraries (${EXECUTABLE_7} ${GLUT_LIBRARIES})
target_link_libraries (${EXECUTABLE_7} ${GLEW_LIBRARIES})
target_link_libraries (${EXECUTABLE_7} ${GTK_LIBRARIES})

target_link_libraries (${EXECUTABLE_7} ${ACE_LIBRARY})

target_link_libraries (${EXECUTABLE_7} Common)
target_link_libraries (${EXECUTABLE_7} CommonError)
target_link_libraries (${EXECUTABLE_7} CommonLog)
target_link_libraries (${EXECUTABLE_7} CommonOpenGL)
target_link_libraries (${EXECUTABLE_7} CommonTimer)
target_link_libraries (${EXECUTABLE_7} CommonUI)
target_link_libraries (${EXECUTABLE_7} CommonUI_GTK)

target_link_libraries (${EXECUTABLE_8} ${BOX2D_LIBRARIES})
target_link_libraries (${EXECUTABLE_8} ${LIBNOISE_LIB})
target_link_libraries (${EXECUTABLE_8} ${GLUT_LIBRARIES})
target_link_libraries (${EXECUTABLE_8} ${GLEW_LIBRARIES})
target_link_libraries (${EXECUTABLE_8} ${GTK_LIBRARIES})

target_link_libraries (${EXECUTABLE_8} ${ACE_LIBRARY})

target_link_libraries (${EXECUTABLE_8} Common)
target_link_libraries (${EXECUTABLE_8} CommonError)
target_link_libraries (${EXECUTABLE_8} CommonLog)
target_link_libraries (${EXECUTABLE_8} CommonOpenGL)
target_link_libraries (${EXECUTABLE_8} CommonTimer)
target_link_libraries (${EXECUTABLE_8} CommonUI)
target_link_libraries (${EXECUTABLE_8} CommonUI_GTK)

if (UNIX)
 target_link_options (${EXECUTABLE_5} PUBLIC "$<$<CXX_COMPILER_ID:GNU>:-Wl,--allow-multiple-definition>")
 target_link_options (${EXECUTABLE_6} PUBLIC "$<$<CXX_COMPILER_ID:GNU>:-Wl,--allow-multiple-definition>")
 target_link_options (${EXECUTABLE_7} PUBLIC "$<$<CXX_COMPILER_ID:GNU>:-Wl,--allow-multiple-definition>")
elseif (WIN32)
 set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /STACK:20000000")
 target_link_options (${EXECUTABLE_5} PUBLIC "$<$<CXX_COMPILER_ID:MSVC>:-FORCE:MULTIPLE>")
 target_link_options (${EXECUTABLE_6} PUBLIC "$<$<CXX_COMPILER_ID:MSVC>:-FORCE:MULTIPLE>")
 target_link_options (${EXECUTABLE_7} PUBLIC "$<$<CXX_COMPILER_ID:MSVC>:-FORCE:MULTIPLE>")
endif ()

# add PCH
target_precompile_headers (${EXECUTABLE}
                           PRIVATE stdafx.h)
target_precompile_headers (${EXECUTABLE_2}
                           REUSE_FROM ${EXECUTABLE})
target_precompile_headers (${EXECUTABLE_3}
                           REUSE_FROM ${EXECUTABLE})
target_precompile_headers (${EXECUTABLE_4}
                           REUSE_FROM ${EXECUTABLE})
target_precompile_headers (${EXECUTABLE_5}
                           REUSE_FROM ${EXECUTABLE})
target_precompile_headers (${EXECUTABLE_6}
                           REUSE_FROM ${EXECUTABLE})
target_precompile_headers (${EXECUTABLE_7}
                           REUSE_FROM ${EXECUTABLE})
target_precompile_headers (${EXECUTABLE_8}
                           REUSE_FROM ${EXECUTABLE})

install (TARGETS ${EXECUTABLE} ${EXECUTABLE_2} ${EXECUTABLE_3} ${EXECUTABLE_4} ${EXECUTABLE_5} ${EXECUTABLE_6} ${EXECUTABLE_7} ${EXECUTABLE_8}
         RUNTIME
         DESTINATION bin
         COMPONENT applications)
if (WIN32)
 install(FILES
         ${ACE_LIB_DIR}/ACE${LIB_FILE_SUFFIX}.dll
         DESTINATION bin
         COMPONENT binaries)
 if (CMAKE_BUILD_TYPE STREQUAL Debug)
 install(FILES
         "C:/Windows/System32/msvcp140d.dll"
         "C:/Windows/System32/vcruntime140d.dll"
         DESTINATION bin
         COMPONENT runtime)
 endif (CMAKE_BUILD_TYPE STREQUAL Debug)
endif (WIN32)

list (APPEND CMAKE_MODULE_PATH "$ENV{LIB_ROOT}/cmake-modules")
#include(UseBackportedModules)
cmake_policy (SET CMP0026 OLD)
cmake_policy (SET CMP0053 OLD)
include (CreateLaunchers)

set (ARGS "-l -m1")
set (ARGS_2 "-l -m30")
set (ARGS_3 "-l -m60")
set (ARGS_4 "-l -m90")
set (ARGS_5 "-l -m120")
set (ARGS_6 "-l -m140")
set (ARGS_7 "-l -m160")
set (ARGS_8 "-l -m180")
if (CMAKE_BUILD_TYPE STREQUAL Debug)
 set (ARGS "${ARGS} -t")
 set (ARGS "${ARGS_2} -t")
 set (ARGS "${ARGS_3} -t")
 set (ARGS "${ARGS_4} -t")
 set (ARGS "${ARGS_5} -t")
 set (ARGS "${ARGS_6} -t")
 set (ARGS "${ARGS_7} -t")
 set (ARGS "${ARGS_8} -t")
endif (CMAKE_BUILD_TYPE STREQUAL Debug)
file (TO_CMAKE_PATH ${LIBNOISE_LIB_DIR} LIBNOISE_PATH)
file (TO_CMAKE_PATH "${GLUT_LIB_DIR}" GLUT_PATH)
file (TO_CMAKE_PATH "${GLEW_LIB_DIR}" GLEW_PATH)
file (TO_CMAKE_PATH ${ACE_LIB_DIR} ACE_PATH)
file (TO_CMAKE_PATH "${ASSIMP_LIB_DIR}" ASSIMP_PATH)
file (TO_CMAKE_PATH "${PNG_LIB_DIR}" PNG_PATH)
file (TO_CMAKE_PATH "${ffmpeg_LIB_DIR}" FFMPEG_PATH)
file (TO_CMAKE_PATH "${ImageMagick_LIB_DIR}" IMAGEMAGICK_PATH)
file (TO_CMAKE_PATH "${GTK_LIB_DIRS}" GTK_PATH)
guess_runtime_library_dirs (RUNTIME_LIBRARY_DIRS)
set (RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS}
                          ${ACE_PATH}
                          ${LIBNOISE_PATH}
                          ${GLUT_PATH}
                          ${GLEW_PATH}
                          ${ASSIMP_PATH}
                          ${PNG_PATH}
                          ${FFMPEG_PATH}
                          ${IMAGEMAGICK_PATH}
                          ${GTK_PATH})
if (UNIX)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE}" COMMAND)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_2}" COMMAND_2)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_3}" COMMAND_3)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_4}" COMMAND_4)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_5}" COMMAND_5)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_6}" COMMAND_6)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_7}" COMMAND_7)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_8}" COMMAND_8)
elseif (WIN32)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE}.exe" COMMAND)
 string (REPLACE "\\" "\\\\" COMMAND ${COMMAND})
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_2}.exe" COMMAND_2)
 string (REPLACE "\\" "\\\\" COMMAND_2 ${COMMAND_2})
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_3}.exe" COMMAND_3)
 string (REPLACE "\\" "\\\\" COMMAND_3 ${COMMAND_3})
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_4}.exe" COMMAND_4)
 string (REPLACE "\\" "\\\\" COMMAND_4 ${COMMAND_4})
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_5}.exe" COMMAND_5)
 string (REPLACE "\\" "\\\\" COMMAND_5 ${COMMAND_5})
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_6}.exe" COMMAND_6)
 string (REPLACE "\\" "\\\\" COMMAND_6 ${COMMAND_6})
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_7}.exe" COMMAND_7)
 string (REPLACE "\\" "\\\\" COMMAND_7 ${COMMAND_7})
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_8}.exe" COMMAND_8)
 string (REPLACE "\\" "\\\\" COMMAND_8 ${COMMAND_8})
 set (ENVIRONMENT_MAGICK "MAGICK_CODER_MODULE_PATH=${IMAGEMAGICK_PATH}\\\\modules\\\\coders")
endif ()
create_target_launcher (${EXECUTABLE}
                        COMMAND ${COMMAND}
                        ARGS ${ARGS}
#                        FORWARD_ARGS
                        RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        ENVIRONMENT ${ENVIRONMENT_MAGICK})
create_target_launcher (${EXECUTABLE_2}
                        COMMAND ${COMMAND_2}
                        ARGS ${ARGS_2}
#                        FORWARD_ARGS
                        RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        ENVIRONMENT ${ENVIRONMENT_MAGICK})
create_target_launcher (${EXECUTABLE_3}
                        COMMAND ${COMMAND_3}
                        ARGS ${ARGS_3}
#                        FORWARD_ARGS
                        RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        ENVIRONMENT ${ENVIRONMENT_MAGICK})
create_target_launcher (${EXECUTABLE_4}
                        COMMAND ${COMMAND_4}
                        ARGS ${ARGS_4}
#                        FORWARD_ARGS
                        RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        ENVIRONMENT ${ENVIRONMENT_MAGICK})
create_target_launcher (${EXECUTABLE_5}
                        COMMAND ${COMMAND_5}
                        ARGS ${ARGS_5}
#                        FORWARD_ARGS
                        RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        ENVIRONMENT ${ENVIRONMENT_MAGICK})
create_target_launcher (${EXECUTABLE_6}
                        COMMAND ${COMMAND_6}
                        ARGS ${ARGS_6}
#                        FORWARD_ARGS
                        RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        ENVIRONMENT ${ENVIRONMENT_MAGICK})
create_target_launcher (${EXECUTABLE_7}
                        COMMAND ${COMMAND_7}
                        ARGS ${ARGS_7}
#                        FORWARD_ARGS
                        RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        ENVIRONMENT ${ENVIRONMENT_MAGICK})
create_target_launcher (${EXECUTABLE_8}
                        COMMAND ${COMMAND_8}
                        ARGS ${ARGS_8}
#                        FORWARD_ARGS
                        RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        ENVIRONMENT ${ENVIRONMENT_MAGICK})

set (CPACK_GENERATOR "NSIS")
set (CPACK_PACKAGE_INSTALL_DIRECTORY ${PROJECT_NAME})
set (CPACK_PACKAGE_INSTALL_REGISTRY_KEY ${PROJECT_NAME})
set (CPACK_PACKAGE_NAME ${PROJECT_NAME})
set (CPACK_PACKAGE_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_MICRO})
set (CPACK_PACKAGE_VERSION_PATCH "0")
set (CPACK_PACKAGE_VENDOR "")
#set (CPACK_IGNORE_FILES "\\.psd$;/CVS/;/\\.svn/;/\\.git/;\\.swp$;/CMakeLists.txt.user;\\.#;/#;\\.tar.gz$;/CMakeFiles/;CMakeCache.txt;\\.qm$;/build/;\\.diff$;.DS_Store'")
set (CPACK_SOURCE_GENERATOR "TGZ")
set (CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
set (CPACK_SOURCE_IGNORE_FILES ${CPACK_IGNORE_FILES})
#set (CPACK_NSIS_MODIFY_PATH ON)
set (CPACK_NSIS_INSTALL_ROOT "C:")
set (CPACK_NSIS_INSTALLED_ICON_NAME "${PROJECT_NAME}.ico")
set (CPACK_NSIS_HELP_LINK ${PACKAGE_URL})
set (CPACK_NSIS_URL_INFO_ABOUT ${PACKAGE_URL})
set (CPACK_NSIS_CONTACT ${PACKAGE_BUGREPORT})

set (CPACK_NSIS_DEFINES "RequestExecutionLevel user")

#set (CPACK_NSIS_EXTRA_PREINSTALL_COMMANDS "
#  InstallDir 'C:\\\\importToLibreOffice'
#")

include (CPack)
