set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
include (VersionConfig)

string (REPLACE ${CMAKE_SOURCE_DIR} "" BUILD_PATH_SUFFIX ${CMAKE_BINARY_DIR})
string (SUBSTRING ${BUILD_PATH_SUFFIX} 1 -1 BUILD_PATH_SUFFIX)
if (UNIX)
 set (Common_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../Common/${BUILD_PATH_SUFFIX}/${CMAKE_BUILD_TYPE}")
endif (UNIX)
find_package (Common
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

set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${COMMON_ROOT_DIR}/scripts")
include (CPU)
include (OS)
include (compiler_init)

cmake_minimum_required (VERSION 2.8)

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
#  add_definitions (-D_ITERATOR_DEBUG_LEVEL=2)
 else ()
#  add_definitions (-D_ITERATOR_DEBUG_LEVEL=0)
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

set (PACKAGE ${PROJECT_NAME})
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake.in
                ${CMAKE_CURRENT_BINARY_DIR}/engine_config.h
                @ONLY
                NEWLINE_STYLE ${NEWLINE_STYLE})
add_definitions (-DHAVE_CONFIG_H)

include (libraries_init)

configure_file (resource.rc.in
                resource.rc)

include (FindPkgConfig)
set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${COMMON_ROOT_DIR}/src/gl/scripts")
include (assimp)
include (glew)
include (glut)

#set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${COMMON_ROOT_DIR}/scripts")
include (ffmpeg)
include (ImageMagick)

#set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${COMMON_ROOT_DIR}/scripts")
include (libnoise)

set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${COMMON_ROOT_DIR}/src/image/scripts")
include (png)
include (stb)

include (box2d)
include (pge)

include (ACE)

set (PATH_SUFFIX "Release")
if (DEFINED CMAKE_BUILD_TYPE)
 set (PATH_SUFFIX ${CMAKE_BUILD_TYPE})
endif (DEFINED CMAKE_BUILD_TYPE)

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
include_directories (${BOX2D_INCLUDE_DIRS})
include_directories (${PGE_INCLUDE_DIR})
include_directories (${PGE_INCLUDE_DIR}/extensions)
include_directories (${LIBNOISE_INCLUDE_DIRS})
include_directories (${GLUT_INCLUDE_DIRS})
include_directories (${GLEW_INCLUDE_DIRS})
include_directories (${GTK_INCLUDE_DIRS})
include_directories (${ImageMagick_INCLUDE_DIRS})

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
     ${CMAKE_CURRENT_BINARY_DIR}/engine_config.h
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
     ${CMAKE_CURRENT_BINARY_DIR}/engine_config.h
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
     ${CMAKE_CURRENT_BINARY_DIR}/engine_config.h
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
     ${CMAKE_CURRENT_BINARY_DIR}/engine_config.h
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
     ${CMAKE_CURRENT_BINARY_DIR}/engine_config.h
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
     ${CMAKE_CURRENT_BINARY_DIR}/engine_config.h
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
     ${CMAKE_CURRENT_BINARY_DIR}/engine_config.h
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
     ${CMAKE_CURRENT_BINARY_DIR}/engine_config.h
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
     pge_187.cpp
     pge_187.h
     pge_188.cpp
     pge_188.h
     pge_189.cpp
     pge_189.h
     pge_190.cpp
     pge_190.h
     pge_191.cpp
     pge_191.h
     pge_192.cpp
     pge_192.h
     pge_193.cpp
     pge_193.h
     pge_194.cpp
     pge_194.h
     pge_195.cpp
     pge_195.h
     pge_196.cpp
     pge_196.h
     pge_197.cpp
     pge_197.h
     pge_198.cpp
     pge_198.h
     pge_199.cpp
     pge_199.h
     stdafx.h)

set (EXECUTABLE_8 engine_8)
add_executable (${EXECUTABLE_8}
                ${engine_8_EXE_SRCS})

set (engine_9_EXE_SRCS
     ${CMAKE_CURRENT_BINARY_DIR}/engine_config.h
     ${CMAKE_CURRENT_BINARY_DIR}/resource.rc
#     ${CMAKE_CURRENT_BINARY_DIR}/ImportToLibreOffice.dir/Debug/resource.res
     camera.cpp
     camera.h
     engine_common.h
     defines_9.h
     main_9.cpp
     pge_200.cpp
     pge_200.h
     pge_201.cpp
     pge_201.h
     pge_202.cpp
     pge_202.h
     pge_203.cpp
     pge_203.h
     pge_204.cpp
     pge_204.h
     pge_205.cpp
     pge_205.h
     pge_206.cpp
     pge_206.h
     pge_207.cpp
     pge_207.h
     pge_208.cpp
     pge_208.h
     pge_209.cpp
     pge_209.h
     pge_210.cpp
     pge_210.h
     pge_211.cpp
     pge_211.h
     pge_212.cpp
     pge_212.h
     pge_213.cpp
     pge_213.h
     pge_214.cpp
     pge_214.h
     pge_215.cpp
     pge_215.h
     pge_216.cpp
     pge_216.h
     pge_217.cpp
     pge_217.h
     pge_218.cpp
     pge_218.h
     pge_219.cpp
     pge_219.h
     stdafx.h)

set (EXECUTABLE_9 engine_9)
add_executable (${EXECUTABLE_9}
                ${engine_9_EXE_SRCS})

set (engine_10_EXE_SRCS
     ${CMAKE_CURRENT_BINARY_DIR}/engine_config.h
     ${CMAKE_CURRENT_BINARY_DIR}/resource.rc
#     ${CMAKE_CURRENT_BINARY_DIR}/ImportToLibreOffice.dir/Debug/resource.res
     camera.cpp
     camera.h
     engine_common.h
     defines_10.h
     main_10.cpp
     glut_220.cpp
     glut_220.h
     pge_221.cpp
     pge_221.h
     glut_222.cpp
     glut_222.h
     pge_223.cpp
     pge_223.h
     pge_224.cpp
     pge_224.h
     pge_225.cpp
     pge_225.h
     pge_226.cpp
     pge_226.h
     glut_227.cpp
     glut_227.h
     glut_228.cpp
     glut_228.h
     glut_229.cpp
     glut_229.h
     pge_230.cpp
     pge_230.h
     pge_231.cpp
     pge_231.h
     glut_232.cpp
     glut_232.h
     glut_233.cpp
     glut_233.h
     pge_234.cpp
     pge_234.h
     pge_235.cpp
     pge_235.h
     glut_236.cpp
     glut_236.h
     glut_237.cpp
     glut_237.h
     glut_238.cpp
     glut_238.h
     glut_239.cpp
     glut_239.h
     stdafx.h)

set (EXECUTABLE_10 engine_10)
add_executable (${EXECUTABLE_10}
                ${engine_10_EXE_SRCS})

set (engine_11_EXE_SRCS
     ${CMAKE_CURRENT_BINARY_DIR}/engine_config.h
     ${CMAKE_CURRENT_BINARY_DIR}/resource.rc
#     ${CMAKE_CURRENT_BINARY_DIR}/ImportToLibreOffice.dir/Debug/resource.res
     camera.cpp
     camera.h
     engine_common.h
     defines_11.h
     main_11.cpp
     glut_240.cpp
     glut_240.h
     glut_241.cpp
     glut_241.h
     pge_242.cpp
     pge_242.h
     pge_243.cpp
     pge_243.h
     pge_244.cpp
     pge_244.h
     pge_245.cpp
     pge_245.h
     glut_246.cpp
     glut_246.h
     pge_247.cpp
     pge_247.h
     glut_248.cpp
     glut_248.h
     pge_249.cpp
     pge_249.h
     pge_250.cpp
     pge_250.h
     glut_251.cpp
     glut_251.h
     pge_252.cpp
     pge_252.h
     glut_253.cpp
     glut_253.h
     pge_254.cpp
     pge_254.h
     pge_255.cpp
     pge_255.h
     pge_256.cpp
     pge_256.h
     pge_257.cpp
     pge_257.h
     glut_258.cpp
     glut_258.h
     pge_259.cpp
     pge_259.h
     stdafx.h)

set (EXECUTABLE_11 engine_11)
add_executable (${EXECUTABLE_11}
                ${engine_11_EXE_SRCS})

set (engine_12_EXE_SRCS
     ${CMAKE_CURRENT_BINARY_DIR}/engine_config.h
     ${CMAKE_CURRENT_BINARY_DIR}/resource.rc
#     ${CMAKE_CURRENT_BINARY_DIR}/ImportToLibreOffice.dir/Debug/resource.res
     camera.cpp
     camera.h
     engine_common.h
     defines_12.h
     main_12.cpp
     pge_260.cpp
     pge_260.h
     pge_261.cpp
     pge_261.h
     pge_262.cpp
     pge_262.h
     pge_263.cpp
     pge_263.h
     glut_264.cpp
     glut_264.h
     pge_265.cpp
     pge_265.h
     pge_266.cpp
     pge_266.h
     glut_267.cpp
     glut_267.h
     pge_268.cpp
     pge_268.h
     glut_269.cpp
     glut_269.h
     pge_270.cpp
     pge_270.h
     glut_271.cpp
     glut_271.h
     glut_272.cpp
     glut_272.h
     glut_273.cpp
     glut_273.h
     glut_274.cpp
     glut_274.h
     glut_275.cpp
     glut_275.h
     pge_276.cpp
     pge_276.h
     glut_277.cpp
     glut_277.h
     glut_278.cpp
     glut_278.h
     glut_279.cpp
     glut_279.h
     stdafx.h)

set (EXECUTABLE_12 engine_12)
add_executable (${EXECUTABLE_12}
                ${engine_12_EXE_SRCS})

set (engine_13_EXE_SRCS
     ${CMAKE_CURRENT_BINARY_DIR}/engine_config.h
     ${CMAKE_CURRENT_BINARY_DIR}/resource.rc
#     ${CMAKE_CURRENT_BINARY_DIR}/ImportToLibreOffice.dir/Debug/resource.res
     camera.cpp
     camera.h
     engine_common.h
     defines_13.h
     main_13.cpp
     pge_280.cpp
     pge_280.h
     glut_281.cpp
     glut_281.h
     glut_282.cpp
     glut_282.h
     glut_283.cpp
     glut_283.h
     glut_284.cpp
     glut_284.h
     glut_285.cpp
     glut_285.h
     glut_286.cpp
     glut_286.h
     glut_287.cpp
     glut_287.h
     pge_288.cpp
     pge_288.h
     pge_289.cpp
     pge_289.h
     pge_290.cpp
     pge_290.h
     pge_291.cpp
     pge_291.h
     pge_292.cpp
     pge_292.h
     glut_293.cpp
     glut_293.h
     glut_294.cpp
     glut_294.h
     glut_295.cpp
     glut_295.h
     glut_296.cpp
     glut_296.h
     glut_297.cpp
     glut_297.h
     glut_298.cpp
     glut_298.h
     pge_299.cpp
     pge_299.h
     stdafx.h)

set (EXECUTABLE_13 engine_13)
add_executable (${EXECUTABLE_13}
                ${engine_13_EXE_SRCS})

set (engine_14_EXE_SRCS
     ${CMAKE_CURRENT_BINARY_DIR}/engine_config.h
     ${CMAKE_CURRENT_BINARY_DIR}/resource.rc
#     ${CMAKE_CURRENT_BINARY_DIR}/ImportToLibreOffice.dir/Debug/resource.res
     camera.cpp
     camera.h
     engine_common.h
     defines_14.h
     main_14.cpp
     pge_300.cpp
     pge_300.h
     pge_301.cpp
     pge_301.h
     glut_302.cpp
     glut_302.h
     pge_303.cpp
     pge_303.h
     glut_304.cpp
     glut_304.h
     pge_305.cpp
     pge_305.h
     pge_306.cpp
     pge_306.h
     pge_307.cpp
     pge_307.h
     glut_308.cpp
     glut_308.h
     pge_309.cpp
     pge_309.h
     glut_310.cpp
     glut_310.h
     glut_311.cpp
     glut_311.h
     glut_312.cpp
     glut_312.h
     glut_313.cpp
     glut_313.h
     pge_314.cpp
     pge_314.h
     glut_315.cpp
     glut_315.h
     glut_316.cpp
     glut_316.h
     glut_317.cpp
     glut_317.h
     glut_318.cpp
     glut_318.h
     pge_319.cpp
     pge_319.h
     stdafx.h)

set (EXECUTABLE_14 engine_14)
add_executable (${EXECUTABLE_14}
                ${engine_14_EXE_SRCS})

set (engine_15_EXE_SRCS
     ${CMAKE_CURRENT_BINARY_DIR}/engine_config.h
     ${CMAKE_CURRENT_BINARY_DIR}/resource.rc
#     ${CMAKE_CURRENT_BINARY_DIR}/ImportToLibreOffice.dir/Debug/resource.res
     camera.cpp
     camera.h
     engine_common.h
     defines_15.h
     main_15.cpp
     pge_320.cpp
     pge_320.h
     glut_321.cpp
     glut_321.h
     glut_322.cpp
     glut_322.h
     pge_323.cpp
     pge_323.h
     pge_324.cpp
     pge_324.h
     pge_325.cpp
     pge_325.h
     pge_326.cpp
     pge_326.h
     glut_327.cpp
     glut_327.h
     glut_328.cpp
     glut_328.h
     glut_329.cpp
     glut_329.h
     glut_330.cpp
     glut_330.h
     glut_331.cpp
     glut_331.h
     pge_332.cpp
     pge_332.h
     glut_333.cpp
     glut_333.h
     pge_334.cpp
     pge_334.h
     glut_335.cpp
     glut_335.h
     pge_336.cpp
     pge_336.h
     pge_337.cpp
     pge_337.h
     glut_338.cpp
     glut_338.h
     pge_339.cpp
     pge_339.h
     stdafx.h)

set (EXECUTABLE_15 engine_15)
add_executable (${EXECUTABLE_15}
                ${engine_15_EXE_SRCS})

set (engine_16_EXE_SRCS
     ${CMAKE_CURRENT_BINARY_DIR}/engine_config.h
     ${CMAKE_CURRENT_BINARY_DIR}/resource.rc
#     ${CMAKE_CURRENT_BINARY_DIR}/ImportToLibreOffice.dir/Debug/resource.res
     camera.cpp
     camera.h
     engine_common.h
     defines_16.h
     main_16.cpp
     pge_340.cpp
     pge_340.h
     pge_341.cpp
     pge_341.h
     pge_342.cpp
     pge_342.h
     pge_343.cpp
     pge_343.h
     pge_344.cpp
     pge_344.h
     glut_345.cpp
     glut_345.h
     pge_346.cpp
     pge_346.h
     glut_347.cpp
     glut_347.h
     glut_348.cpp
     glut_348.h
     pge_349.cpp
     pge_349.h
     pge_350.cpp
     pge_350.h
     glut_351.cpp
     glut_351.h
     pge_352.cpp
     pge_352.h
     pge_353.cpp
     pge_353.h
     pge_354.cpp
     pge_354.h
     glut_355.cpp
     glut_355.h
     pge_356.cpp
     pge_356.h
     glut_357.cpp
     glut_357.h
     glut_358.cpp
     glut_358.h
     pge_359.cpp
     pge_359.h
     stdafx.h)

set (EXECUTABLE_16 engine_16)
add_executable (${EXECUTABLE_16}
                ${engine_16_EXE_SRCS})

set (engine_17_EXE_SRCS
     ${CMAKE_CURRENT_BINARY_DIR}/engine_config.h
     ${CMAKE_CURRENT_BINARY_DIR}/resource.rc
#     ${CMAKE_CURRENT_BINARY_DIR}/ImportToLibreOffice.dir/Debug/resource.res
     camera.cpp
     camera.h
     engine_common.h
     defines_17.h
     main_17.cpp
     glut_360.cpp
     glut_360.h
     pge_361.cpp
     pge_361.h
     pge_362.cpp
     pge_362.h
     glut_363.cpp
     glut_363.h
     pge_364.cpp
     pge_364.h
     pge_365.cpp
     pge_365.h
     glut_366.cpp
     glut_366.h
     pge_367.cpp
     pge_367.h
     pge_368.cpp
     pge_368.h
     glut_369.cpp
     glut_369.h
     glut_370.cpp
     glut_370.h
     glut_371.cpp
     glut_371.h
     pge_372.cpp
     pge_372.h
     glut_373.cpp
     glut_373.h
     glut_374.cpp
     glut_374.h
     glut_375.cpp
     glut_375.h
     pge_376.cpp
     pge_376.h
     glut_377.cpp
     glut_377.h
     pge_378.cpp
     pge_378.h
     glut_379.cpp
     glut_379.h
     pge_380.cpp
     pge_380.h
     pge_381.cpp
     pge_381.h
     glut_382.cpp
     glut_382.h
     glut_383.cpp
     glut_383.h
     pge_384.cpp
     pge_384.h
     pge_385.cpp
     pge_385.h
     glut_386.cpp
     glut_386.h
     pge_387.cpp
     pge_387.h
     glut_388.cpp
     glut_388.h
     glut_389.cpp
     glut_389.h
     stdafx.h)

set (EXECUTABLE_17 engine_17)
add_executable (${EXECUTABLE_17}
                ${engine_17_EXE_SRCS})

set (engine_18_EXE_SRCS
     ${CMAKE_CURRENT_BINARY_DIR}/engine_config.h
     ${CMAKE_CURRENT_BINARY_DIR}/resource.rc
#     ${CMAKE_CURRENT_BINARY_DIR}/ImportToLibreOffice.dir/Debug/resource.res
     camera.cpp
     camera.h
     engine_common.h
     defines_18.h
     main_18.cpp
     glut_390.cpp
     glut_390.h
     glut_391.cpp
     glut_391.h
     glut_392.cpp
     glut_392.h
     pge_393.cpp
     pge_393.h
     glut_394.cpp
     glut_394.h
     pge_395.cpp
     pge_395.h
     pge_396.cpp
     pge_396.h
     pge_397.cpp
     pge_397.h
     glut_398.cpp
     glut_398.h
     pge_399.cpp
     pge_399.h
     pge_400.cpp
     pge_400.h
     pge_401.cpp
     pge_401.h
     pge_402.cpp
     pge_402.h
     pge_403.cpp
     pge_403.h
     pge_404.cpp
     pge_404.h
     pge_405.cpp
     pge_405.h
     pge_406.cpp
     pge_406.h
     glut_407.cpp
     glut_407.h
     glut_408.cpp
     glut_408.h
     pge_409.cpp
     pge_409.h
     stdafx.h)

set (EXECUTABLE_18 engine_18)
add_executable (${EXECUTABLE_18}
                ${engine_18_EXE_SRCS})

set (engine_19_EXE_SRCS
     ${CMAKE_CURRENT_BINARY_DIR}/engine_config.h
     ${CMAKE_CURRENT_BINARY_DIR}/resource.rc
#     ${CMAKE_CURRENT_BINARY_DIR}/ImportToLibreOffice.dir/Debug/resource.res
     camera.cpp
     camera.h
     engine_common.h
     defines_19.h
     main_19.cpp
     glut_410.cpp
     glut_410.h
     pge_411.cpp
     pge_411.h
     glut_412.cpp
     glut_412.h
     glut_413.cpp
     glut_413.h
     pge_414.cpp
     pge_414.h
     glut_415.cpp
     glut_415.h
     pge_416.cpp
     pge_416.h
     glut_417.cpp
     glut_417.h
     glut_418.cpp
     glut_418.h
     pge_419.cpp
     pge_419.h
     pge_420.cpp
     pge_420.h
     pge_421.cpp
     pge_421.h
     pge_422.cpp
     pge_422.h
     glut_423.cpp
     glut_423.h
     pge_424.cpp
     pge_424.h
     glut_425.cpp
     glut_425.h
     glut_426.cpp
     glut_426.h
     pge_427.cpp
     pge_427.h
     glut_428.cpp
     glut_428.h
     pge_429.cpp
     pge_429.h
     stdafx.h)

set (EXECUTABLE_19 engine_19)
add_executable (${EXECUTABLE_19}
                ${engine_19_EXE_SRCS})

set (engine_20_EXE_SRCS
     ${CMAKE_CURRENT_BINARY_DIR}/engine_config.h
     ${CMAKE_CURRENT_BINARY_DIR}/resource.rc
#     ${CMAKE_CURRENT_BINARY_DIR}/ImportToLibreOffice.dir/Debug/resource.res
     camera.cpp
     camera.h
     engine_common.h
     defines_20.h
     main_20.cpp
     glut_430.cpp
     glut_430.h
     glut_431.cpp
     glut_431.h
     pge_432.cpp
     pge_432.h
     glut_433.cpp
     glut_433.h
     glut_434.cpp
     glut_434.h
     pge_435.cpp
     pge_435.h
     glut_436.cpp
     glut_436.h
     pge_437.cpp
     pge_437.h
     pge_438.cpp
     pge_438.h
     glut_439.cpp
     glut_439.h
     pge_440.cpp
     pge_440.h
     glut_441.cpp
     glut_441.h
     glut_442.cpp
     glut_442.h
     pge_443.cpp
     pge_443.h
     glut_444.cpp
     glut_444.h
     glut_445.cpp
     glut_445.h
     glut_446.cpp
     glut_446.h
     pge_447.cpp
     pge_447.h
     glut_448.cpp
     glut_448.h
     glut_449.cpp
     glut_449.h
     stdafx.h)

set (EXECUTABLE_20 engine_20)
add_executable (${EXECUTABLE_20}
                ${engine_20_EXE_SRCS})

set (engine_21_EXE_SRCS
     ${CMAKE_CURRENT_BINARY_DIR}/engine_config.h
     ${CMAKE_CURRENT_BINARY_DIR}/resource.rc
#     ${CMAKE_CURRENT_BINARY_DIR}/ImportToLibreOffice.dir/Debug/resource.res
     camera.cpp
     camera.h
     engine_common.h
     defines_21.h
     main_21.cpp
     glut_450.cpp
     glut_450.h
     glut_451.cpp
     glut_451.h
     pge_452.cpp
     pge_452.h
     glut_453.cpp
     glut_453.h
     glut_454.cpp
     glut_454.h
     pge_455.cpp
     pge_455.h
     glut_456.cpp
     glut_456.h
     glut_457.cpp
     glut_457.h
     glut_458.cpp
     glut_458.h
     glut_459.cpp
     glut_459.h
     glut_460.cpp
     glut_460.h
     pge_461.cpp
     pge_461.h
     pge_462.cpp
     pge_462.h
     pge_463.cpp
     pge_463.h
     pge_464.cpp
     pge_464.h
     glut_465.cpp
     glut_465.h
     glut_466.cpp
     glut_466.h
     glut_467.cpp
     glut_467.h
     glut_468.cpp
     glut_468.h
     glut_469.cpp
     glut_469.h
     stdafx.h)

set (EXECUTABLE_21 engine_21)
add_executable (${EXECUTABLE_21}
                ${engine_21_EXE_SRCS})

set (engine_22_EXE_SRCS
     ${CMAKE_CURRENT_BINARY_DIR}/engine_config.h
     ${CMAKE_CURRENT_BINARY_DIR}/resource.rc
#     ${CMAKE_CURRENT_BINARY_DIR}/ImportToLibreOffice.dir/Debug/resource.res
     camera.cpp
     camera.h
     engine_common.h
     defines_22.h
     main_22.cpp
     glut_470.cpp
     glut_470.h
     glut_471.cpp
     glut_471.h
     glut_472.cpp
     glut_472.h
     glut_473.cpp
     glut_473.h
     glut_474.cpp
     glut_474.h
     glut_475.cpp
     glut_475.h
     glut_476.cpp
     glut_476.h
     glut_477.cpp
     glut_477.h
     glut_478.cpp
     glut_478.h
     glut_479.cpp
     glut_479.h
     glut_480.cpp
     glut_480.h
     glut_481.cpp
     glut_481.h
     glut_482.cpp
     glut_482.h
     glut_483.cpp
     glut_483.h
     glut_484.cpp
     glut_484.h
     glut_485.cpp
     glut_485.h
     glut_486.cpp
     glut_486.h
     glut_487.cpp
     glut_487.h
     glut_488.cpp
     glut_488.h
     glut_489.cpp
     glut_489.h
     stdafx.h)

set (EXECUTABLE_22 engine_22)
add_executable (${EXECUTABLE_22}
                ${engine_22_EXE_SRCS})

set (engine_23_EXE_SRCS
     ${CMAKE_CURRENT_BINARY_DIR}/engine_config.h
     ${CMAKE_CURRENT_BINARY_DIR}/resource.rc
#     ${CMAKE_CURRENT_BINARY_DIR}/ImportToLibreOffice.dir/Debug/resource.res
     camera.cpp
     camera.h
     engine_common.h
     defines_23.h
     main_23.cpp
     glut_490.cpp
     glut_490.h
     glut_491.cpp
     glut_491.h
     glut_492.cpp
     glut_492.h
     glut_493.cpp
     glut_493.h
     glut_494.cpp
     glut_494.h
     glut_495.cpp
     glut_495.h
     glut_496.cpp
     glut_496.h
     glut_497.cpp
     glut_497.h
     glut_498.cpp
     glut_498.h
     glut_499.cpp
     glut_499.h
     glut_500.cpp
     glut_500.h
     glut_501.cpp
     glut_501.h
     glut_502.cpp
     glut_502.h
     glut_503.cpp
     glut_503.h
     glut_504.cpp
     glut_504.h
     glut_505.cpp
     glut_505.h
     glut_506.cpp
     glut_506.h
     glut_507.cpp
     glut_507.h
     glut_508.cpp
     glut_508.h
     glut_509.cpp
     glut_509.h
     stdafx.h)

set (EXECUTABLE_23 engine_23)
add_executable (${EXECUTABLE_23}
                ${engine_23_EXE_SRCS})

set (engine_24_EXE_SRCS
     ${CMAKE_CURRENT_BINARY_DIR}/engine_config.h
     ${CMAKE_CURRENT_BINARY_DIR}/resource.rc
#     ${CMAKE_CURRENT_BINARY_DIR}/ImportToLibreOffice.dir/Debug/resource.res
     camera.cpp
     camera.h
     engine_common.h
     defines_24.h
     main_24.cpp
     glut_510.cpp
     glut_510.h
     glut_511.cpp
     glut_511.h
     glut_512.cpp
     glut_512.h
     glut_513.cpp
     glut_513.h
     glut_514.cpp
     glut_514.h
     glut_515.cpp
     glut_515.h
     glut_516.cpp
     glut_516.h
     glut_517.cpp
     glut_517.h
     glut_518.cpp
     glut_518.h
     glut_519.cpp
     glut_519.h
     glut_520.cpp
     glut_520.h
     glut_521.cpp
     glut_521.h
     glut_522.cpp
     glut_522.h
     glut_523.cpp
     glut_523.h
     glut_524.cpp
     glut_524.h
     glut_525.cpp
     glut_525.h
     glut_526.cpp
     glut_526.h
     glut_527.cpp
     glut_527.h
     glut_528.cpp
     glut_528.h
     glut_529.cpp
     glut_529.h
     stdafx.h)

set (EXECUTABLE_24 engine_24)
add_executable (${EXECUTABLE_24}
                ${engine_24_EXE_SRCS})

set (engine_25_EXE_SRCS
     ${CMAKE_CURRENT_BINARY_DIR}/engine_config.h
     ${CMAKE_CURRENT_BINARY_DIR}/resource.rc
#     ${CMAKE_CURRENT_BINARY_DIR}/ImportToLibreOffice.dir/Debug/resource.res
     camera.cpp
     camera.h
     engine_common.h
     defines_25.h
     main_25.cpp
     glut_530.cpp
     glut_530.h
     glut_531.cpp
     glut_531.h
     glut_532.cpp
     glut_532.h
     glut_533.cpp
     glut_533.h
     glut_534.cpp
     glut_534.h
     glut_535.cpp
     glut_535.h
     glut_536.cpp
     glut_536.h
     glut_537.cpp
     glut_537.h
     glut_538.cpp
     glut_538.h
     glut_539.cpp
     glut_539.h
     glut_540.cpp
     glut_540.h
     glut_541.cpp
     glut_541.h
     glut_542.cpp
     glut_542.h
     glut_543.cpp
     glut_543.h
     glut_544.cpp
     glut_544.h
     glut_545.cpp
     glut_545.h
     glut_546.cpp
     glut_546.h
     glut_547.cpp
     glut_547.h
     glut_548.cpp
     glut_548.h
     glut_549.cpp
     glut_549.h
     stdafx.h)

set (EXECUTABLE_25 engine_25)
add_executable (${EXECUTABLE_25}
                ${engine_25_EXE_SRCS})

set (engine_26_EXE_SRCS
     ${CMAKE_CURRENT_BINARY_DIR}/engine_config.h
     ${CMAKE_CURRENT_BINARY_DIR}/resource.rc
#     ${CMAKE_CURRENT_BINARY_DIR}/ImportToLibreOffice.dir/Debug/resource.res
     camera.cpp
     camera.h
     engine_common.h
     defines_26.h
     main_26.cpp
     glut_550.cpp
     glut_550.h
     glut_551.cpp
     glut_551.h
     glut_552.cpp
     glut_552.h
     glut_553.cpp
     glut_553.h
     glut_554.cpp
     glut_554.h
     glut_555.cpp
     glut_555.h
     glut_556.cpp
     glut_556.h
     glut_557.cpp
     glut_557.h
     glut_558.cpp
     glut_558.h
     glut_559.cpp
     glut_559.h
     glut_560.cpp
     glut_560.h
     glut_561.cpp
     glut_561.h
     glut_562.cpp
     glut_562.h
     glut_563.cpp
     glut_563.h
     glut_564.cpp
     glut_564.h
     glut_565.cpp
     glut_565.h
     glut_566.cpp
     glut_566.h
     glut_567.cpp
     glut_567.h
     glut_568.cpp
     glut_568.h
     glut_569.cpp
     glut_569.h
     stdafx.h)

set (EXECUTABLE_26 engine_26)
add_executable (${EXECUTABLE_26}
                ${engine_26_EXE_SRCS})

set (engine_27_EXE_SRCS
     ${CMAKE_CURRENT_BINARY_DIR}/engine_config.h
     ${CMAKE_CURRENT_BINARY_DIR}/resource.rc
#     ${CMAKE_CURRENT_BINARY_DIR}/ImportToLibreOffice.dir/Debug/resource.res
     camera.cpp
     camera.h
     engine_common.h
     defines_27.h
     main_27.cpp
     glut_570.cpp
     glut_570.h
     glut_571.cpp
     glut_571.h
     glut_572.cpp
     glut_572.h
     glut_573.cpp
     glut_573.h
     glut_574.cpp
     glut_574.h
     glut_575.cpp
     glut_575.h
     glut_576.cpp
     glut_576.h
     glut_577.cpp
     glut_577.h
     glut_578.cpp
     glut_578.h
     glut_579.cpp
     glut_579.h
     glut_580.cpp
     glut_580.h
     glut_581.cpp
     glut_581.h
     glut_582.cpp
     glut_582.h
     glut_583.cpp
     glut_583.h
     glut_584.cpp
     glut_584.h
     glut_585.cpp
     glut_585.h
     glut_586.cpp
     glut_586.h
     glut_587.cpp
     glut_587.h
     glut_588.cpp
     glut_588.h
     glut_589.cpp
     glut_589.h
     stdafx.h)

set (EXECUTABLE_27 engine_27)
add_executable (${EXECUTABLE_27}
                ${engine_27_EXE_SRCS})

set (engine_28_EXE_SRCS
     ${CMAKE_CURRENT_BINARY_DIR}/engine_config.h
     ${CMAKE_CURRENT_BINARY_DIR}/resource.rc
#     ${CMAKE_CURRENT_BINARY_DIR}/ImportToLibreOffice.dir/Debug/resource.res
     camera.cpp
     camera.h
     engine_common.h
     defines_28.h
     main_28.cpp
     glut_590.cpp
     glut_590.h
     glut_591.cpp
     glut_591.h
     glut_592.cpp
     glut_592.h
     glut_593.cpp
     glut_593.h
     glut_594.cpp
     glut_594.h
     glut_595.cpp
     glut_595.h
     glut_596.cpp
     glut_596.h
     glut_597.cpp
     glut_597.h
     glut_598.cpp
     glut_598.h
     glut_599.cpp
     glut_599.h
     glut_600.cpp
     glut_600.h
     glut_601.cpp
     glut_601.h
     glut_602.cpp
     glut_602.h
     glut_603.cpp
     glut_603.h
     glut_604.cpp
     glut_604.h
     glut_605.cpp
     glut_605.h
     glut_606.cpp
     glut_606.h
     glut_607.cpp
     glut_607.h
     glut_608.cpp
     glut_608.h
     glut_609.cpp
     glut_609.h
     stdafx.h)

set (EXECUTABLE_28 engine_28)
add_executable (${EXECUTABLE_28}
                ${engine_28_EXE_SRCS})

set (engine_29_EXE_SRCS
     ${CMAKE_CURRENT_BINARY_DIR}/engine_config.h
     ${CMAKE_CURRENT_BINARY_DIR}/resource.rc
#     ${CMAKE_CURRENT_BINARY_DIR}/ImportToLibreOffice.dir/Debug/resource.res
     camera.cpp
     camera.h
     engine_common.h
     defines_29.h
     main_29.cpp
     glut_610.cpp
     glut_610.h
     glut_611.cpp
     glut_611.h
     glut_612.cpp
     glut_612.h
     glut_613.cpp
     glut_613.h
     glut_614.cpp
     glut_614.h
     glut_615.cpp
     glut_615.h
     glut_616.cpp
     glut_616.h
     glut_617.cpp
     glut_617.h
     glut_618.cpp
     glut_618.h
     glut_619.cpp
     glut_619.h
     glut_620.cpp
     glut_620.h
     glut_621.cpp
     glut_621.h
     glut_622.cpp
     glut_622.h
     glut_623.cpp
     glut_623.h
     glut_624.cpp
     glut_624.h
     glut_625.cpp
     glut_625.h
     glut_626.cpp
     glut_626.h
     glut_627.cpp
     glut_627.h
     glut_628.cpp
     glut_628.h
     glut_629.cpp
     glut_629.h
     stdafx.h)

set (EXECUTABLE_29 engine_29)
add_executable (${EXECUTABLE_29}
                ${engine_29_EXE_SRCS})

set (engine_30_EXE_SRCS
     ${CMAKE_CURRENT_BINARY_DIR}/engine_config.h
     ${CMAKE_CURRENT_BINARY_DIR}/resource.rc
#     ${CMAKE_CURRENT_BINARY_DIR}/ImportToLibreOffice.dir/Debug/resource.res
     camera.cpp
     camera.h
     engine_common.h
     defines_30.h
     main_30.cpp
     glut_630.cpp
     glut_630.h
     glut_631.cpp
     glut_631.h
     glut_632.cpp
     glut_632.h
     glut_633.cpp
     glut_633.h
     glut_634.cpp
     glut_634.h
     glut_635.cpp
     glut_635.h
     glut_636.cpp
     glut_636.h
     glut_637.cpp
     glut_637.h
     glut_638.cpp
     glut_638.h
     glut_639.cpp
     glut_639.h
     glut_640.cpp
     glut_640.h
     glut_641.cpp
     glut_641.h
     glut_642.cpp
     glut_642.h
     glut_643.cpp
     glut_643.h
     glut_644.cpp
     glut_644.h
     glut_645.cpp
     glut_645.h
     glut_646.cpp
     glut_646.h
     glut_647.cpp
     glut_647.h
     glut_648.cpp
     glut_648.h
     glut_649.cpp
     glut_649.h
     stdafx.h)

set (EXECUTABLE_30 engine_30)
add_executable (${EXECUTABLE_30}
                ${engine_30_EXE_SRCS})

set (engine_31_EXE_SRCS
     ${CMAKE_CURRENT_BINARY_DIR}/engine_config.h
     ${CMAKE_CURRENT_BINARY_DIR}/resource.rc
#     ${CMAKE_CURRENT_BINARY_DIR}/ImportToLibreOffice.dir/Debug/resource.res
     camera.cpp
     camera.h
     engine_common.h
     defines_31.h
     main_31.cpp
     glut_650.cpp
     glut_650.h
     glut_651.cpp
     glut_651.h
     glut_652.cpp
     glut_652.h
     glut_653.cpp
     glut_653.h
     glut_654.cpp
     glut_654.h
     glut_655.cpp
     glut_655.h
     glut_656.cpp
     glut_656.h
     glut_657.cpp
     glut_657.h
     glut_658.cpp
     glut_658.h
     glut_659.cpp
     glut_659.h
     glut_660.cpp
     glut_660.h
     glut_661.cpp
     glut_661.h
     glut_662.cpp
     glut_662.h
     glut_663.cpp
     glut_663.h
     glut_664.cpp
     glut_664.h
     glut_665.cpp
     glut_665.h
     glut_666.cpp
     glut_666.h
     glut_667.cpp
     glut_667.h
     glut_668.cpp
     glut_668.h
     glut_669.cpp
     glut_669.h
     stdafx.h)

set (EXECUTABLE_31 engine_31)
add_executable (${EXECUTABLE_31}
                ${engine_31_EXE_SRCS})

set (engine_32_EXE_SRCS
     ${CMAKE_CURRENT_BINARY_DIR}/engine_config.h
     ${CMAKE_CURRENT_BINARY_DIR}/resource.rc
#     ${CMAKE_CURRENT_BINARY_DIR}/ImportToLibreOffice.dir/Debug/resource.res
     camera.cpp
     camera.h
     engine_common.h
     defines_32.h
     main_32.cpp
     glut_670.cpp
     glut_670.h
     glut_671.cpp
     glut_671.h
     glut_672.cpp
     glut_672.h
     glut_673.cpp
     glut_673.h
     glut_674.cpp
     glut_674.h
     glut_675.cpp
     glut_675.h
     glut_676.cpp
     glut_676.h
     glut_677.cpp
     glut_677.h
     glut_678.cpp
     glut_678.h
     glut_679.cpp
     glut_679.h
     glut_680.cpp
     glut_680.h
     glut_681.cpp
     glut_681.h
     glut_682.cpp
     glut_682.h
     glut_683.cpp
     glut_683.h
     glut_684.cpp
     glut_684.h
     glut_685.cpp
     glut_685.h
     glut_686.cpp
     glut_686.h
     glut_687.cpp
     glut_687.h
     glut_688.cpp
     glut_688.h
     glut_689.cpp
     glut_689.h
     stdafx.h)

set (EXECUTABLE_32 engine_32)
add_executable (${EXECUTABLE_32}
                ${engine_32_EXE_SRCS})

set (engine_33_EXE_SRCS
     ${CMAKE_CURRENT_BINARY_DIR}/engine_config.h
     ${CMAKE_CURRENT_BINARY_DIR}/resource.rc
#     ${CMAKE_CURRENT_BINARY_DIR}/ImportToLibreOffice.dir/Debug/resource.res
     camera.cpp
     camera.h
     engine_common.h
     defines_33.h
     main_33.cpp
     glut_690.cpp
     glut_690.h
     glut_691.cpp
     glut_691.h
     glut_692.cpp
     glut_692.h
     glut_693.cpp
     glut_693.h
     glut_694.cpp
     glut_694.h
     glut_695.cpp
     glut_695.h
     glut_696.cpp
     glut_696.h
     glut_697.cpp
     glut_697.h
     glut_698.cpp
     glut_698.h
     glut_699.cpp
     glut_699.h
     glut_700.cpp
     glut_700.h
     glut_701.cpp
     glut_701.h
     glut_702.cpp
     glut_702.h
     glut_703.cpp
     glut_703.h
     glut_704.cpp
     glut_704.h
     glut_705.cpp
     glut_705.h
     glut_706.cpp
     glut_706.h
     glut_707.cpp
     glut_707.h
     glut_708.cpp
     glut_708.h
     glut_709.cpp
     glut_709.h
     stdafx.h)

set (EXECUTABLE_33 engine_33)
add_executable (${EXECUTABLE_33}
                ${engine_33_EXE_SRCS})

set (engine_34_EXE_SRCS
     ${CMAKE_CURRENT_BINARY_DIR}/engine_config.h
     ${CMAKE_CURRENT_BINARY_DIR}/resource.rc
#     ${CMAKE_CURRENT_BINARY_DIR}/ImportToLibreOffice.dir/Debug/resource.res
     camera.cpp
     camera.h
     engine_common.h
     defines_34.h
     main_34.cpp
     glut_710.cpp
     glut_710.h
     glut_711.cpp
     glut_711.h
     glut_712.cpp
     glut_712.h
     glut_713.cpp
     glut_713.h
     glut_714.cpp
     glut_714.h
     glut_715.cpp
     glut_715.h
     glut_716.cpp
     glut_716.h
     glut_717.cpp
     glut_717.h
     glut_718.cpp
     glut_718.h
     glut_719.cpp
     glut_719.h
     glut_720.cpp
     glut_720.h
     glut_721.cpp
     glut_721.h
     glut_722.cpp
     glut_722.h
     glut_723.cpp
     glut_723.h
     glut_724.cpp
     glut_724.h
     glut_725.cpp
     glut_725.h
     glut_726.cpp
     glut_726.h
     glut_727.cpp
     glut_727.h
     glut_728.cpp
     glut_728.h
     glut_729.cpp
     glut_729.h
     stdafx.h)

set (EXECUTABLE_34 engine_34)
add_executable (${EXECUTABLE_34}
                ${engine_34_EXE_SRCS})

set (engine_35_EXE_SRCS
     ${CMAKE_CURRENT_BINARY_DIR}/engine_config.h
     ${CMAKE_CURRENT_BINARY_DIR}/resource.rc
#     ${CMAKE_CURRENT_BINARY_DIR}/ImportToLibreOffice.dir/Debug/resource.res
     camera.cpp
     camera.h
     engine_common.h
     defines_35.h
     main_35.cpp
     glut_730.cpp
     glut_730.h
     glut_731.cpp
     glut_731.h
     glut_732.cpp
     glut_732.h
     glut_733.cpp
     glut_733.h
     glut_734.cpp
     glut_734.h
     glut_735.cpp
     glut_735.h
     glut_736.cpp
     glut_736.h
     glut_737.cpp
     glut_737.h
     glut_738.cpp
     glut_738.h
     glut_739.cpp
     glut_739.h
     glut_740.cpp
     glut_740.h
     glut_741.cpp
     glut_741.h
     glut_742.cpp
     glut_742.h
     glut_743.cpp
     glut_743.h
     glut_744.cpp
     glut_744.h
     glut_745.cpp
     glut_745.h
     glut_746.cpp
     glut_746.h
     glut_747.cpp
     glut_747.h
     glut_748.cpp
     glut_748.h
     glut_749.cpp
     glut_749.h
     stdafx.h)

set (EXECUTABLE_35 engine_35)
add_executable (${EXECUTABLE_35}
                ${engine_35_EXE_SRCS})

set (engine_36_EXE_SRCS
     ${CMAKE_CURRENT_BINARY_DIR}/engine_config.h
     ${CMAKE_CURRENT_BINARY_DIR}/resource.rc
#     ${CMAKE_CURRENT_BINARY_DIR}/ImportToLibreOffice.dir/Debug/resource.res
     camera.cpp
     camera.h
     engine_common.h
     defines_36.h
     main_36.cpp
     glut_750.cpp
     glut_750.h
     glut_751.cpp
     glut_751.h
     glut_752.cpp
     glut_752.h
     glut_753.cpp
     glut_753.h
     glut_754.cpp
     glut_754.h
     glut_755.cpp
     glut_755.h
     glut_756.cpp
     glut_756.h
     glut_757.cpp
     glut_757.h
     glut_758.cpp
     glut_758.h
     glut_759.cpp
     glut_759.h
     glut_760.cpp
     glut_760.h
     glut_761.cpp
     glut_761.h
     glut_762.cpp
     glut_762.h
     glut_763.cpp
     glut_763.h
     glut_764.cpp
     glut_764.h
     glut_765.cpp
     glut_765.h
     glut_766.cpp
     glut_766.h
     glut_767.cpp
     glut_767.h
     glut_768.cpp
     glut_768.h
     glut_769.cpp
     glut_769.h
     stdafx.h)

set (EXECUTABLE_36 engine_36)
add_executable (${EXECUTABLE_36}
                ${engine_36_EXE_SRCS})

set (engine_37_EXE_SRCS
     ${CMAKE_CURRENT_BINARY_DIR}/engine_config.h
     ${CMAKE_CURRENT_BINARY_DIR}/resource.rc
#     ${CMAKE_CURRENT_BINARY_DIR}/ImportToLibreOffice.dir/Debug/resource.res
     camera.cpp
     camera.h
     engine_common.h
     defines_37.h
     main_37.cpp
     glut_770.cpp
     glut_770.h
     glut_771.cpp
     glut_771.h
     glut_772.cpp
     glut_772.h
     glut_773.cpp
     glut_773.h
     glut_774.cpp
     glut_774.h
     glut_775.cpp
     glut_775.h
     glut_776.cpp
     glut_776.h
     glut_777.cpp
     glut_777.h
     glut_778.cpp
     glut_778.h
     glut_779.cpp
     glut_779.h
     glut_780.cpp
     glut_780.h
     glut_781.cpp
     glut_781.h
     glut_782.cpp
     glut_782.h
     glut_783.cpp
     glut_783.h
     glut_784.cpp
     glut_784.h
     glut_785.cpp
     glut_785.h
     glut_786.cpp
     glut_786.h
     glut_787.cpp
     glut_787.h
     glut_788.cpp
     glut_788.h
     glut_789.cpp
     glut_789.h
     stdafx.h)

set (EXECUTABLE_37 engine_37)
add_executable (${EXECUTABLE_37}
                ${engine_37_EXE_SRCS})

set (engine_38_EXE_SRCS
     ${CMAKE_CURRENT_BINARY_DIR}/engine_config.h
     ${CMAKE_CURRENT_BINARY_DIR}/resource.rc
#     ${CMAKE_CURRENT_BINARY_DIR}/ImportToLibreOffice.dir/Debug/resource.res
     camera.cpp
     camera.h
     engine_common.h
     defines_38.h
     main_38.cpp
     glut_790.cpp
     glut_790.h
     glut_791.cpp
     glut_791.h
     glut_792.cpp
     glut_792.h
     glut_793.cpp
     glut_793.h
     glut_794.cpp
     glut_794.h
     glut_795.cpp
     glut_795.h
     glut_796.cpp
     glut_796.h
     glut_797.cpp
     glut_797.h
     glut_798.cpp
     glut_798.h
     glut_799.cpp
     glut_799.h
     glut_800.cpp
     glut_800.h
     glut_801.cpp
     glut_801.h
     glut_802.cpp
     glut_802.h
     glut_803.cpp
     glut_803.h
     glut_804.cpp
     glut_804.h
     glut_805.cpp
     glut_805.h
     glut_806.cpp
     glut_806.h
     glut_807.cpp
     glut_807.h
     glut_808.cpp
     glut_808.h
     glut_809.cpp
     glut_809.h
     stdafx.h)

set (EXECUTABLE_38 engine_38)
add_executable (${EXECUTABLE_38}
                ${engine_38_EXE_SRCS})

set (engine_39_EXE_SRCS
     ${CMAKE_CURRENT_BINARY_DIR}/engine_config.h
     ${CMAKE_CURRENT_BINARY_DIR}/resource.rc
#     ${CMAKE_CURRENT_BINARY_DIR}/ImportToLibreOffice.dir/Debug/resource.res
     camera.cpp
     camera.h
     engine_common.h
     defines_39.h
     main_39.cpp
     glut_810.cpp
     glut_810.h
     glut_811.cpp
     glut_811.h
     glut_812.cpp
     glut_812.h
     glut_813.cpp
     glut_813.h
     glut_814.cpp
     glut_814.h
     glut_815.cpp
     glut_815.h
     glut_816.cpp
     glut_816.h
     glut_817.cpp
     glut_817.h
     glut_818.cpp
     glut_818.h
     stdafx.h)

set (EXECUTABLE_39 engine_39)
add_executable (${EXECUTABLE_39}
                ${engine_39_EXE_SRCS})

set_target_properties (${EXECUTABLE_9} PROPERTIES CXX_STANDARD 17)
set_target_properties (${EXECUTABLE_10} PROPERTIES CXX_STANDARD 17)
set_target_properties (${EXECUTABLE_11} PROPERTIES CXX_STANDARD 17)
set_target_properties (${EXECUTABLE_13} PROPERTIES CXX_STANDARD 17)
if (WIN32 AND HOST_CPU_HAS_AVX2)
# target_compile_options (${EXECUTABLE_9} PRIVATE $<$CXX_COMPILER_ID:MSVC>:/arch:AVX2>)
 target_compile_options (${EXECUTABLE_9} PRIVATE $<$<BOOL:${MSVC}>:/arch:AVX2>)
endif (WIN32 AND HOST_CPU_HAS_AVX2)

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

 target_link_libraries (${EXECUTABLE_9} X11)
 target_link_libraries (${EXECUTABLE_9} GL)
 target_link_libraries (${EXECUTABLE_9} png)

 target_link_libraries (${EXECUTABLE_10} X11)
 target_link_libraries (${EXECUTABLE_10} GL)
 target_link_libraries (${EXECUTABLE_10} png)

 target_link_libraries (${EXECUTABLE_11} X11)
 target_link_libraries (${EXECUTABLE_11} GL)
 target_link_libraries (${EXECUTABLE_11} png)

 target_link_libraries (${EXECUTABLE_12} X11)
 target_link_libraries (${EXECUTABLE_12} GL)
 target_link_libraries (${EXECUTABLE_12} png)

 target_link_libraries (${EXECUTABLE_13} X11)
 target_link_libraries (${EXECUTABLE_13} GL)
 target_link_libraries (${EXECUTABLE_13} png)

 target_link_libraries (${EXECUTABLE_14} X11)
 target_link_libraries (${EXECUTABLE_14} GL)
 target_link_libraries (${EXECUTABLE_14} png)

 target_link_libraries (${EXECUTABLE_15} X11)
 target_link_libraries (${EXECUTABLE_15} GL)
 target_link_libraries (${EXECUTABLE_15} png)

 target_link_libraries (${EXECUTABLE_16} X11)
 target_link_libraries (${EXECUTABLE_16} GL)
 target_link_libraries (${EXECUTABLE_16} png)

 target_link_libraries (${EXECUTABLE_17} X11)
 target_link_libraries (${EXECUTABLE_17} GL)
 target_link_libraries (${EXECUTABLE_17} png)

 target_link_libraries (${EXECUTABLE_18} X11)
 target_link_libraries (${EXECUTABLE_18} GL)
 target_link_libraries (${EXECUTABLE_18} png)

 target_link_libraries (${EXECUTABLE_19} X11)
 target_link_libraries (${EXECUTABLE_19} GL)
 target_link_libraries (${EXECUTABLE_19} png)

 target_link_libraries (${EXECUTABLE_20} X11)
 target_link_libraries (${EXECUTABLE_20} GL)
 target_link_libraries (${EXECUTABLE_20} png)

 target_link_libraries (${EXECUTABLE_21} X11)
 target_link_libraries (${EXECUTABLE_21} GL)
 target_link_libraries (${EXECUTABLE_21} png)

 target_link_libraries (${EXECUTABLE_22} X11)
 target_link_libraries (${EXECUTABLE_22} GL)
 target_link_libraries (${EXECUTABLE_22} png)

 target_link_libraries (${EXECUTABLE_23} X11)
 target_link_libraries (${EXECUTABLE_23} GL)
 target_link_libraries (${EXECUTABLE_23} png)

 target_link_libraries (${EXECUTABLE_24} X11)
 target_link_libraries (${EXECUTABLE_24} GL)
 target_link_libraries (${EXECUTABLE_24} png)

 target_link_libraries (${EXECUTABLE_25} X11)
 target_link_libraries (${EXECUTABLE_25} GL)
 target_link_libraries (${EXECUTABLE_25} png)

 target_link_libraries (${EXECUTABLE_26} X11)
 target_link_libraries (${EXECUTABLE_26} GL)
 target_link_libraries (${EXECUTABLE_26} png)

 target_link_libraries (${EXECUTABLE_27} X11)
 target_link_libraries (${EXECUTABLE_27} GL)
 target_link_libraries (${EXECUTABLE_27} png)

 target_link_libraries (${EXECUTABLE_28} X11)
 target_link_libraries (${EXECUTABLE_28} GL)
 target_link_libraries (${EXECUTABLE_28} png)

 target_link_libraries (${EXECUTABLE_29} X11)
 target_link_libraries (${EXECUTABLE_29} GL)
 target_link_libraries (${EXECUTABLE_29} png)

 target_link_libraries (${EXECUTABLE_30} X11)
 target_link_libraries (${EXECUTABLE_30} GL)
 target_link_libraries (${EXECUTABLE_30} png)

 target_link_libraries (${EXECUTABLE_31} X11)
 target_link_libraries (${EXECUTABLE_31} GL)
 target_link_libraries (${EXECUTABLE_31} png)

 target_link_libraries (${EXECUTABLE_32} X11)
 target_link_libraries (${EXECUTABLE_32} GL)
 target_link_libraries (${EXECUTABLE_32} png)

 target_link_libraries (${EXECUTABLE_33} X11)
 target_link_libraries (${EXECUTABLE_33} GL)
 target_link_libraries (${EXECUTABLE_33} png)

 target_link_libraries (${EXECUTABLE_34} X11)
 target_link_libraries (${EXECUTABLE_34} GL)
 target_link_libraries (${EXECUTABLE_34} png)

 target_link_libraries (${EXECUTABLE_35} X11)
 target_link_libraries (${EXECUTABLE_35} GL)
 target_link_libraries (${EXECUTABLE_35} png)

 target_link_libraries (${EXECUTABLE_36} X11)
 target_link_libraries (${EXECUTABLE_36} GL)
 target_link_libraries (${EXECUTABLE_36} png)

 target_link_libraries (${EXECUTABLE_37} X11)
 target_link_libraries (${EXECUTABLE_37} GL)
 target_link_libraries (${EXECUTABLE_37} png)

 target_link_libraries (${EXECUTABLE_38} X11)
 target_link_libraries (${EXECUTABLE_38} GL)
 target_link_libraries (${EXECUTABLE_38} png)

 target_link_libraries (${EXECUTABLE_39} X11)
 target_link_libraries (${EXECUTABLE_39} GL)
 target_link_libraries (${EXECUTABLE_39} png)
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

target_link_libraries (${EXECUTABLE_9} ${BOX2D_LIBRARIES})
target_link_libraries (${EXECUTABLE_9} ${LIBNOISE_LIB})
target_link_libraries (${EXECUTABLE_9} ${GLUT_LIBRARIES})
target_link_libraries (${EXECUTABLE_9} ${GLEW_LIBRARIES})
target_link_libraries (${EXECUTABLE_9} ${GTK_LIBRARIES})

target_link_libraries (${EXECUTABLE_9} ${ACE_LIBRARY})

target_link_libraries (${EXECUTABLE_9} Common)
target_link_libraries (${EXECUTABLE_9} CommonError)
target_link_libraries (${EXECUTABLE_9} CommonLog)
target_link_libraries (${EXECUTABLE_9} CommonOpenGL)
target_link_libraries (${EXECUTABLE_9} CommonTimer)
target_link_libraries (${EXECUTABLE_9} CommonUI)
target_link_libraries (${EXECUTABLE_9} CommonUI_GTK)

target_link_libraries (${EXECUTABLE_10} ${BOX2D_LIBRARIES})
target_link_libraries (${EXECUTABLE_10} ${LIBNOISE_LIB})
target_link_libraries (${EXECUTABLE_10} ${GLUT_LIBRARIES})
target_link_libraries (${EXECUTABLE_10} ${GLEW_LIBRARIES})
target_link_libraries (${EXECUTABLE_10} ${GTK_LIBRARIES})

target_link_libraries (${EXECUTABLE_10} ${ACE_LIBRARY})

target_link_libraries (${EXECUTABLE_10} Common)
target_link_libraries (${EXECUTABLE_10} CommonError)
target_link_libraries (${EXECUTABLE_10} CommonLog)
target_link_libraries (${EXECUTABLE_10} CommonOpenGL)
target_link_libraries (${EXECUTABLE_10} CommonTimer)
target_link_libraries (${EXECUTABLE_10} CommonUI)
target_link_libraries (${EXECUTABLE_10} CommonUI_GTK)

target_link_libraries (${EXECUTABLE_11} ${BOX2D_LIBRARIES})
target_link_libraries (${EXECUTABLE_11} ${LIBNOISE_LIB})
target_link_libraries (${EXECUTABLE_11} ${GLUT_LIBRARIES})
target_link_libraries (${EXECUTABLE_11} ${GLEW_LIBRARIES})
target_link_libraries (${EXECUTABLE_11} ${GTK_LIBRARIES})

target_link_libraries (${EXECUTABLE_11} ${ACE_LIBRARY})

target_link_libraries (${EXECUTABLE_11} Common)
target_link_libraries (${EXECUTABLE_11} CommonError)
target_link_libraries (${EXECUTABLE_11} CommonLog)
target_link_libraries (${EXECUTABLE_11} CommonOpenGL)
target_link_libraries (${EXECUTABLE_11} CommonTimer)
target_link_libraries (${EXECUTABLE_11} CommonUI)
#target_link_libraries (${EXECUTABLE_11} CommonUI_GTK)

target_link_libraries (${EXECUTABLE_12} ${BOX2D_LIBRARIES})
target_link_libraries (${EXECUTABLE_12} ${LIBNOISE_LIB})
target_link_libraries (${EXECUTABLE_12} ${GLUT_LIBRARIES})
target_link_libraries (${EXECUTABLE_12} ${GLEW_LIBRARIES})
target_link_libraries (${EXECUTABLE_12} ${GTK_LIBRARIES})

target_link_libraries (${EXECUTABLE_12} ${ACE_LIBRARY})

target_link_libraries (${EXECUTABLE_12} Common)
target_link_libraries (${EXECUTABLE_12} CommonError)
target_link_libraries (${EXECUTABLE_12} CommonLog)
target_link_libraries (${EXECUTABLE_12} CommonOpenGL)
target_link_libraries (${EXECUTABLE_12} CommonTimer)
target_link_libraries (${EXECUTABLE_12} CommonUI)
#target_link_libraries (${EXECUTABLE_12} CommonUI_GTK)

target_link_libraries (${EXECUTABLE_13} ${BOX2D_LIBRARIES})
target_link_libraries (${EXECUTABLE_13} ${LIBNOISE_LIB})
target_link_libraries (${EXECUTABLE_13} ${GLUT_LIBRARIES})
target_link_libraries (${EXECUTABLE_13} ${GLEW_LIBRARIES})
target_link_libraries (${EXECUTABLE_13} ${GTK_LIBRARIES})

target_link_libraries (${EXECUTABLE_13} ${ACE_LIBRARY})

target_link_libraries (${EXECUTABLE_13} Common)
target_link_libraries (${EXECUTABLE_13} CommonError)
target_link_libraries (${EXECUTABLE_13} CommonLog)
target_link_libraries (${EXECUTABLE_13} CommonOpenGL)
target_link_libraries (${EXECUTABLE_13} CommonTimer)
target_link_libraries (${EXECUTABLE_13} CommonUI)
#target_link_libraries (${EXECUTABLE_13} CommonUI_GTK)

target_link_libraries (${EXECUTABLE_14} ${BOX2D_LIBRARIES})
target_link_libraries (${EXECUTABLE_14} ${LIBNOISE_LIB})
target_link_libraries (${EXECUTABLE_14} ${GLUT_LIBRARIES})
target_link_libraries (${EXECUTABLE_14} ${GLEW_LIBRARIES})
target_link_libraries (${EXECUTABLE_14} ${GTK_LIBRARIES})

target_link_libraries (${EXECUTABLE_14} ${ACE_LIBRARY})

target_link_libraries (${EXECUTABLE_14} Common)
target_link_libraries (${EXECUTABLE_14} CommonError)
target_link_libraries (${EXECUTABLE_14} CommonLog)
target_link_libraries (${EXECUTABLE_14} CommonOpenGL)
target_link_libraries (${EXECUTABLE_14} CommonTimer)
target_link_libraries (${EXECUTABLE_14} CommonUI)
#target_link_libraries (${EXECUTABLE_14} CommonUI_GTK)

target_link_libraries (${EXECUTABLE_15} ${BOX2D_LIBRARIES})
target_link_libraries (${EXECUTABLE_15} ${LIBNOISE_LIB})
target_link_libraries (${EXECUTABLE_15} ${GLUT_LIBRARIES})
target_link_libraries (${EXECUTABLE_15} ${GLEW_LIBRARIES})
target_link_libraries (${EXECUTABLE_15} ${GTK_LIBRARIES})

target_link_libraries (${EXECUTABLE_15} ${ACE_LIBRARY})

target_link_libraries (${EXECUTABLE_15} Common)
target_link_libraries (${EXECUTABLE_15} CommonError)
target_link_libraries (${EXECUTABLE_15} CommonLog)
target_link_libraries (${EXECUTABLE_15} CommonOpenGL)
target_link_libraries (${EXECUTABLE_15} CommonTimer)
target_link_libraries (${EXECUTABLE_15} CommonUI)
#target_link_libraries (${EXECUTABLE_15} CommonUI_GTK)

target_link_libraries (${EXECUTABLE_16} ${BOX2D_LIBRARIES})
target_link_libraries (${EXECUTABLE_16} ${LIBNOISE_LIB})
target_link_libraries (${EXECUTABLE_16} ${GLUT_LIBRARIES})
target_link_libraries (${EXECUTABLE_16} ${GLEW_LIBRARIES})
target_link_libraries (${EXECUTABLE_16} ${GTK_LIBRARIES})

target_link_libraries (${EXECUTABLE_16} ${ACE_LIBRARY})

target_link_libraries (${EXECUTABLE_16} Common)
target_link_libraries (${EXECUTABLE_16} CommonError)
target_link_libraries (${EXECUTABLE_16} CommonLog)
target_link_libraries (${EXECUTABLE_16} CommonOpenGL)
target_link_libraries (${EXECUTABLE_16} CommonTimer)
target_link_libraries (${EXECUTABLE_16} CommonUI)
#target_link_libraries (${EXECUTABLE_16} CommonUI_GTK)

target_link_libraries (${EXECUTABLE_17} ${BOX2D_LIBRARIES})
target_link_libraries (${EXECUTABLE_17} ${LIBNOISE_LIB})
target_link_libraries (${EXECUTABLE_17} ${GLUT_LIBRARIES})
target_link_libraries (${EXECUTABLE_17} ${GLEW_LIBRARIES})
target_link_libraries (${EXECUTABLE_17} ${GTK_LIBRARIES})

target_link_libraries (${EXECUTABLE_17} ${ACE_LIBRARY})

target_link_libraries (${EXECUTABLE_17} Common)
target_link_libraries (${EXECUTABLE_17} CommonError)
target_link_libraries (${EXECUTABLE_17} CommonLog)
target_link_libraries (${EXECUTABLE_17} CommonOpenGL)
target_link_libraries (${EXECUTABLE_17} CommonTimer)
target_link_libraries (${EXECUTABLE_17} CommonUI)
#target_link_libraries (${EXECUTABLE_17} CommonUI_GTK)

target_link_libraries (${EXECUTABLE_18} ${BOX2D_LIBRARIES})
target_link_libraries (${EXECUTABLE_18} ${LIBNOISE_LIB})
target_link_libraries (${EXECUTABLE_18} ${GLUT_LIBRARIES})
target_link_libraries (${EXECUTABLE_18} ${GLEW_LIBRARIES})
target_link_libraries (${EXECUTABLE_18} ${GTK_LIBRARIES})

target_link_libraries (${EXECUTABLE_18} ${ACE_LIBRARY})

target_link_libraries (${EXECUTABLE_18} Common)
target_link_libraries (${EXECUTABLE_18} CommonError)
target_link_libraries (${EXECUTABLE_18} CommonLog)
target_link_libraries (${EXECUTABLE_18} CommonOpenGL)
target_link_libraries (${EXECUTABLE_18} CommonTimer)
target_link_libraries (${EXECUTABLE_18} CommonUI)
#target_link_libraries (${EXECUTABLE_18} CommonUI_GTK)

target_link_libraries (${EXECUTABLE_19} ${BOX2D_LIBRARIES})
target_link_libraries (${EXECUTABLE_19} ${LIBNOISE_LIB})
target_link_libraries (${EXECUTABLE_19} ${GLUT_LIBRARIES})
target_link_libraries (${EXECUTABLE_19} ${GLEW_LIBRARIES})
target_link_libraries (${EXECUTABLE_19} ${GTK_LIBRARIES})

target_link_libraries (${EXECUTABLE_19} ${ACE_LIBRARY})

target_link_libraries (${EXECUTABLE_19} Common)
target_link_libraries (${EXECUTABLE_19} CommonError)
target_link_libraries (${EXECUTABLE_19} CommonLog)
target_link_libraries (${EXECUTABLE_19} CommonOpenGL)
target_link_libraries (${EXECUTABLE_19} CommonTimer)
target_link_libraries (${EXECUTABLE_19} CommonUI)
#target_link_libraries (${EXECUTABLE_19} CommonUI_GTK)

target_link_libraries (${EXECUTABLE_20} ${BOX2D_LIBRARIES})
target_link_libraries (${EXECUTABLE_20} ${LIBNOISE_LIB})
target_link_libraries (${EXECUTABLE_20} ${GLUT_LIBRARIES})
target_link_libraries (${EXECUTABLE_20} ${GLEW_LIBRARIES})
target_link_libraries (${EXECUTABLE_20} ${GTK_LIBRARIES})

target_link_libraries (${EXECUTABLE_20} ${ACE_LIBRARY})

target_link_libraries (${EXECUTABLE_20} Common)
target_link_libraries (${EXECUTABLE_20} CommonError)
target_link_libraries (${EXECUTABLE_20} CommonLog)
target_link_libraries (${EXECUTABLE_20} CommonOpenGL)
target_link_libraries (${EXECUTABLE_20} CommonTimer)
target_link_libraries (${EXECUTABLE_20} CommonUI)
target_link_libraries (${EXECUTABLE_20} CommonUI_GTK)

target_link_libraries (${EXECUTABLE_21} ${BOX2D_LIBRARIES})
target_link_libraries (${EXECUTABLE_21} ${LIBNOISE_LIB})
target_link_libraries (${EXECUTABLE_21} ${GLUT_LIBRARIES})
target_link_libraries (${EXECUTABLE_21} ${GLEW_LIBRARIES})
#target_link_libraries (${EXECUTABLE_21} ${GTK_LIBRARIES})

target_link_libraries (${EXECUTABLE_21} ${ACE_LIBRARY})

target_link_libraries (${EXECUTABLE_21} Common)
target_link_libraries (${EXECUTABLE_21} CommonError)
target_link_libraries (${EXECUTABLE_21} CommonLog)
target_link_libraries (${EXECUTABLE_21} CommonOpenGL)
target_link_libraries (${EXECUTABLE_21} CommonTimer)
target_link_libraries (${EXECUTABLE_21} CommonUI)
target_link_libraries (${EXECUTABLE_21} CommonUI_GTK)

target_link_libraries (${EXECUTABLE_22} ${BOX2D_LIBRARIES})
target_link_libraries (${EXECUTABLE_22} ${LIBNOISE_LIB})
target_link_libraries (${EXECUTABLE_22} ${GLUT_LIBRARIES})
target_link_libraries (${EXECUTABLE_22} ${GLEW_LIBRARIES})
#target_link_libraries (${EXECUTABLE_22} ${GTK_LIBRARIES})

target_link_libraries (${EXECUTABLE_22} ${ACE_LIBRARY})

target_link_libraries (${EXECUTABLE_22} Common)
target_link_libraries (${EXECUTABLE_22} CommonError)
target_link_libraries (${EXECUTABLE_22} CommonLog)
target_link_libraries (${EXECUTABLE_22} CommonOpenGL)
target_link_libraries (${EXECUTABLE_22} CommonTimer)
target_link_libraries (${EXECUTABLE_22} CommonUI)
target_link_libraries (${EXECUTABLE_22} CommonUI_GTK)

target_link_libraries (${EXECUTABLE_23} ${BOX2D_LIBRARIES})
target_link_libraries (${EXECUTABLE_23} ${LIBNOISE_LIB})
target_link_libraries (${EXECUTABLE_23} ${GLUT_LIBRARIES})
target_link_libraries (${EXECUTABLE_23} ${GLEW_LIBRARIES})
#target_link_libraries (${EXECUTABLE_23} ${GTK_LIBRARIES})

target_link_libraries (${EXECUTABLE_23} ${ACE_LIBRARY})

target_link_libraries (${EXECUTABLE_23} Common)
target_link_libraries (${EXECUTABLE_23} CommonError)
target_link_libraries (${EXECUTABLE_23} CommonLog)
target_link_libraries (${EXECUTABLE_23} CommonOpenGL)
target_link_libraries (${EXECUTABLE_23} CommonTimer)
target_link_libraries (${EXECUTABLE_23} CommonUI)
target_link_libraries (${EXECUTABLE_23} CommonUI_GTK)

target_link_libraries (${EXECUTABLE_24} ${BOX2D_LIBRARIES})
target_link_libraries (${EXECUTABLE_24} ${LIBNOISE_LIB})
target_link_libraries (${EXECUTABLE_24} ${GLUT_LIBRARIES})
target_link_libraries (${EXECUTABLE_24} ${GLEW_LIBRARIES})
#target_link_libraries (${EXECUTABLE_24} ${GTK_LIBRARIES})

target_link_libraries (${EXECUTABLE_24} ${ACE_LIBRARY})

target_link_libraries (${EXECUTABLE_24} Common)
target_link_libraries (${EXECUTABLE_24} CommonError)
target_link_libraries (${EXECUTABLE_24} CommonLog)
target_link_libraries (${EXECUTABLE_24} CommonOpenGL)
target_link_libraries (${EXECUTABLE_24} CommonTimer)
target_link_libraries (${EXECUTABLE_24} CommonUI)
target_link_libraries (${EXECUTABLE_24} CommonUI_GTK)

target_link_libraries (${EXECUTABLE_25} ${BOX2D_LIBRARIES})
target_link_libraries (${EXECUTABLE_25} ${LIBNOISE_LIB})
target_link_libraries (${EXECUTABLE_25} ${GLUT_LIBRARIES})
target_link_libraries (${EXECUTABLE_25} ${GLEW_LIBRARIES})
#target_link_libraries (${EXECUTABLE_25} ${GTK_LIBRARIES})

target_link_libraries (${EXECUTABLE_25} ${ACE_LIBRARY})

target_link_libraries (${EXECUTABLE_25} Common)
target_link_libraries (${EXECUTABLE_25} CommonError)
target_link_libraries (${EXECUTABLE_25} CommonLog)
target_link_libraries (${EXECUTABLE_25} CommonOpenGL)
target_link_libraries (${EXECUTABLE_25} CommonTimer)
target_link_libraries (${EXECUTABLE_25} CommonUI)
target_link_libraries (${EXECUTABLE_25} CommonUI_GTK)

target_link_libraries (${EXECUTABLE_26} ${BOX2D_LIBRARIES})
target_link_libraries (${EXECUTABLE_26} ${LIBNOISE_LIB})
target_link_libraries (${EXECUTABLE_26} ${GLUT_LIBRARIES})
target_link_libraries (${EXECUTABLE_26} ${GLEW_LIBRARIES})
#target_link_libraries (${EXECUTABLE_26} ${GTK_LIBRARIES})

target_link_libraries (${EXECUTABLE_26} ${ACE_LIBRARY})

target_link_libraries (${EXECUTABLE_26} Common)
target_link_libraries (${EXECUTABLE_26} CommonError)
target_link_libraries (${EXECUTABLE_26} CommonLog)
target_link_libraries (${EXECUTABLE_26} CommonOpenGL)
target_link_libraries (${EXECUTABLE_26} CommonTimer)
target_link_libraries (${EXECUTABLE_26} CommonUI)
target_link_libraries (${EXECUTABLE_26} CommonUI_GTK)

target_link_libraries (${EXECUTABLE_27} ${BOX2D_LIBRARIES})
target_link_libraries (${EXECUTABLE_27} ${LIBNOISE_LIB})
target_link_libraries (${EXECUTABLE_27} ${GLUT_LIBRARIES})
target_link_libraries (${EXECUTABLE_27} ${GLEW_LIBRARIES})
#target_link_libraries (${EXECUTABLE_27} ${GTK_LIBRARIES})

target_link_libraries (${EXECUTABLE_27} ${ACE_LIBRARY})

target_link_libraries (${EXECUTABLE_27} Common)
target_link_libraries (${EXECUTABLE_27} CommonError)
target_link_libraries (${EXECUTABLE_27} CommonLog)
target_link_libraries (${EXECUTABLE_27} CommonOpenGL)
target_link_libraries (${EXECUTABLE_27} CommonTimer)
target_link_libraries (${EXECUTABLE_27} CommonUI)
target_link_libraries (${EXECUTABLE_27} CommonUI_GTK)

target_link_libraries (${EXECUTABLE_28} ${BOX2D_LIBRARIES})
target_link_libraries (${EXECUTABLE_28} ${LIBNOISE_LIB})
target_link_libraries (${EXECUTABLE_28} ${GLUT_LIBRARIES})
target_link_libraries (${EXECUTABLE_28} ${GLEW_LIBRARIES})
#target_link_libraries (${EXECUTABLE_28} ${GTK_LIBRARIES})

target_link_libraries (${EXECUTABLE_28} ${ACE_LIBRARY})

target_link_libraries (${EXECUTABLE_28} Common)
target_link_libraries (${EXECUTABLE_28} CommonError)
target_link_libraries (${EXECUTABLE_28} CommonLog)
target_link_libraries (${EXECUTABLE_28} CommonOpenGL)
target_link_libraries (${EXECUTABLE_28} CommonTimer)
target_link_libraries (${EXECUTABLE_28} CommonUI)
target_link_libraries (${EXECUTABLE_28} CommonUI_GTK)

target_link_libraries (${EXECUTABLE_29} ${BOX2D_LIBRARIES})
target_link_libraries (${EXECUTABLE_29} ${LIBNOISE_LIB})
target_link_libraries (${EXECUTABLE_29} ${GLUT_LIBRARIES})
target_link_libraries (${EXECUTABLE_29} ${GLEW_LIBRARIES})
#target_link_libraries (${EXECUTABLE_29} ${GTK_LIBRARIES})

target_link_libraries (${EXECUTABLE_29} ${ACE_LIBRARY})

target_link_libraries (${EXECUTABLE_29} Common)
target_link_libraries (${EXECUTABLE_29} CommonError)
target_link_libraries (${EXECUTABLE_29} CommonLog)
target_link_libraries (${EXECUTABLE_29} CommonOpenGL)
target_link_libraries (${EXECUTABLE_29} CommonTimer)
target_link_libraries (${EXECUTABLE_29} CommonUI)
target_link_libraries (${EXECUTABLE_29} CommonUI_GTK)

target_link_libraries (${EXECUTABLE_30} ${BOX2D_LIBRARIES})
target_link_libraries (${EXECUTABLE_30} ${LIBNOISE_LIB})
target_link_libraries (${EXECUTABLE_30} ${GLUT_LIBRARIES})
target_link_libraries (${EXECUTABLE_30} ${GLEW_LIBRARIES})
#target_link_libraries (${EXECUTABLE_30} ${GTK_LIBRARIES})

target_link_libraries (${EXECUTABLE_30} ${ACE_LIBRARY})

target_link_libraries (${EXECUTABLE_30} Common)
target_link_libraries (${EXECUTABLE_30} CommonError)
target_link_libraries (${EXECUTABLE_30} CommonLog)
target_link_libraries (${EXECUTABLE_30} CommonOpenGL)
target_link_libraries (${EXECUTABLE_30} CommonTimer)
target_link_libraries (${EXECUTABLE_30} CommonUI)
target_link_libraries (${EXECUTABLE_30} CommonUI_GTK)

target_link_libraries (${EXECUTABLE_31} ${BOX2D_LIBRARIES})
target_link_libraries (${EXECUTABLE_31} ${LIBNOISE_LIB})
target_link_libraries (${EXECUTABLE_31} ${GLUT_LIBRARIES})
target_link_libraries (${EXECUTABLE_31} ${GLEW_LIBRARIES})
#target_link_libraries (${EXECUTABLE_31} ${GTK_LIBRARIES})

target_link_libraries (${EXECUTABLE_31} ${ACE_LIBRARY})

target_link_libraries (${EXECUTABLE_31} Common)
target_link_libraries (${EXECUTABLE_31} CommonError)
target_link_libraries (${EXECUTABLE_31} CommonLog)
target_link_libraries (${EXECUTABLE_31} CommonOpenGL)
target_link_libraries (${EXECUTABLE_31} CommonTimer)
target_link_libraries (${EXECUTABLE_31} CommonUI)
target_link_libraries (${EXECUTABLE_31} CommonUI_GTK)

target_link_libraries (${EXECUTABLE_32} ${BOX2D_LIBRARIES})
target_link_libraries (${EXECUTABLE_32} ${LIBNOISE_LIB})
target_link_libraries (${EXECUTABLE_32} ${GLUT_LIBRARIES})
target_link_libraries (${EXECUTABLE_32} ${GLEW_LIBRARIES})
#target_link_libraries (${EXECUTABLE_32} ${GTK_LIBRARIES})

target_link_libraries (${EXECUTABLE_32} ${ACE_LIBRARY})

target_link_libraries (${EXECUTABLE_32} Common)
target_link_libraries (${EXECUTABLE_32} CommonError)
target_link_libraries (${EXECUTABLE_32} CommonLog)
target_link_libraries (${EXECUTABLE_32} CommonOpenGL)
target_link_libraries (${EXECUTABLE_32} CommonTimer)
target_link_libraries (${EXECUTABLE_32} CommonUI)
target_link_libraries (${EXECUTABLE_32} CommonUI_GTK)

target_link_libraries (${EXECUTABLE_33} ${BOX2D_LIBRARIES})
target_link_libraries (${EXECUTABLE_33} ${LIBNOISE_LIB})
target_link_libraries (${EXECUTABLE_33} ${GLUT_LIBRARIES})
target_link_libraries (${EXECUTABLE_33} ${GLEW_LIBRARIES})
#target_link_libraries (${EXECUTABLE_33} ${GTK_LIBRARIES})

target_link_libraries (${EXECUTABLE_33} ${ACE_LIBRARY})

target_link_libraries (${EXECUTABLE_33} Common)
target_link_libraries (${EXECUTABLE_33} CommonError)
target_link_libraries (${EXECUTABLE_33} CommonLog)
target_link_libraries (${EXECUTABLE_33} CommonOpenGL)
target_link_libraries (${EXECUTABLE_33} CommonTimer)
target_link_libraries (${EXECUTABLE_33} CommonUI)
target_link_libraries (${EXECUTABLE_33} CommonUI_GTK)

target_link_libraries (${EXECUTABLE_34} ${BOX2D_LIBRARIES})
target_link_libraries (${EXECUTABLE_34} ${LIBNOISE_LIB})
target_link_libraries (${EXECUTABLE_34} ${GLUT_LIBRARIES})
target_link_libraries (${EXECUTABLE_34} ${GLEW_LIBRARIES})
#target_link_libraries (${EXECUTABLE_34} ${GTK_LIBRARIES})

target_link_libraries (${EXECUTABLE_34} ${ACE_LIBRARY})

target_link_libraries (${EXECUTABLE_34} Common)
target_link_libraries (${EXECUTABLE_34} CommonError)
target_link_libraries (${EXECUTABLE_34} CommonLog)
target_link_libraries (${EXECUTABLE_34} CommonOpenGL)
target_link_libraries (${EXECUTABLE_34} CommonTimer)
target_link_libraries (${EXECUTABLE_34} CommonUI)
target_link_libraries (${EXECUTABLE_34} CommonUI_GTK)

target_link_libraries (${EXECUTABLE_35} ${BOX2D_LIBRARIES})
target_link_libraries (${EXECUTABLE_35} ${LIBNOISE_LIB})
target_link_libraries (${EXECUTABLE_35} ${GLUT_LIBRARIES})
target_link_libraries (${EXECUTABLE_35} ${GLEW_LIBRARIES})
#target_link_libraries (${EXECUTABLE_35} ${GTK_LIBRARIES})

target_link_libraries (${EXECUTABLE_35} ${ACE_LIBRARY})

target_link_libraries (${EXECUTABLE_35} Common)
target_link_libraries (${EXECUTABLE_35} CommonError)
target_link_libraries (${EXECUTABLE_35} CommonLog)
target_link_libraries (${EXECUTABLE_35} CommonOpenGL)
target_link_libraries (${EXECUTABLE_35} CommonTimer)
target_link_libraries (${EXECUTABLE_35} CommonUI)
target_link_libraries (${EXECUTABLE_35} CommonUI_GTK)

target_link_libraries (${EXECUTABLE_36} ${BOX2D_LIBRARIES})
target_link_libraries (${EXECUTABLE_36} ${LIBNOISE_LIB})
target_link_libraries (${EXECUTABLE_36} ${GLUT_LIBRARIES})
target_link_libraries (${EXECUTABLE_36} ${GLEW_LIBRARIES})
#target_link_libraries (${EXECUTABLE_36} ${GTK_LIBRARIES})

target_link_libraries (${EXECUTABLE_36} ${ACE_LIBRARY})

target_link_libraries (${EXECUTABLE_36} Common)
target_link_libraries (${EXECUTABLE_36} CommonError)
target_link_libraries (${EXECUTABLE_36} CommonLog)
target_link_libraries (${EXECUTABLE_36} CommonOpenGL)
target_link_libraries (${EXECUTABLE_36} CommonTimer)
target_link_libraries (${EXECUTABLE_36} CommonUI)
target_link_libraries (${EXECUTABLE_36} CommonUI_GTK)

target_link_libraries (${EXECUTABLE_37} ${BOX2D_LIBRARIES})
target_link_libraries (${EXECUTABLE_37} ${LIBNOISE_LIB})
target_link_libraries (${EXECUTABLE_37} ${GLUT_LIBRARIES})
target_link_libraries (${EXECUTABLE_37} ${GLEW_LIBRARIES})
#target_link_libraries (${EXECUTABLE_37} ${GTK_LIBRARIES})

target_link_libraries (${EXECUTABLE_37} ${ACE_LIBRARY})

target_link_libraries (${EXECUTABLE_37} Common)
target_link_libraries (${EXECUTABLE_37} CommonError)
target_link_libraries (${EXECUTABLE_37} CommonLog)
target_link_libraries (${EXECUTABLE_37} CommonOpenGL)
target_link_libraries (${EXECUTABLE_37} CommonTimer)
target_link_libraries (${EXECUTABLE_37} CommonUI)
target_link_libraries (${EXECUTABLE_37} CommonUI_GTK)

target_link_libraries (${EXECUTABLE_38} ${BOX2D_LIBRARIES})
target_link_libraries (${EXECUTABLE_38} ${LIBNOISE_LIB})
target_link_libraries (${EXECUTABLE_38} ${GLUT_LIBRARIES})
target_link_libraries (${EXECUTABLE_38} ${GLEW_LIBRARIES})
#target_link_libraries (${EXECUTABLE_38} ${GTK_LIBRARIES})

target_link_libraries (${EXECUTABLE_38} ${ACE_LIBRARY})

target_link_libraries (${EXECUTABLE_38} Common)
target_link_libraries (${EXECUTABLE_38} CommonError)
target_link_libraries (${EXECUTABLE_38} CommonLog)
target_link_libraries (${EXECUTABLE_38} CommonOpenGL)
target_link_libraries (${EXECUTABLE_38} CommonTimer)
target_link_libraries (${EXECUTABLE_38} CommonUI)
target_link_libraries (${EXECUTABLE_38} CommonUI_GTK)

target_link_libraries (${EXECUTABLE_39} ${BOX2D_LIBRARIES})
target_link_libraries (${EXECUTABLE_39} ${LIBNOISE_LIB})
target_link_libraries (${EXECUTABLE_39} ${GLUT_LIBRARIES})
target_link_libraries (${EXECUTABLE_39} ${GLEW_LIBRARIES})
#target_link_libraries (${EXECUTABLE_39} ${GTK_LIBRARIES})

target_link_libraries (${EXECUTABLE_39} ${ACE_LIBRARY})

target_link_libraries (${EXECUTABLE_39} Common)
target_link_libraries (${EXECUTABLE_39} CommonError)
target_link_libraries (${EXECUTABLE_39} CommonLog)
target_link_libraries (${EXECUTABLE_39} CommonOpenGL)
target_link_libraries (${EXECUTABLE_39} CommonTimer)
target_link_libraries (${EXECUTABLE_39} CommonUI)
target_link_libraries (${EXECUTABLE_39} CommonUI_GTK)

if (UNIX)
 target_link_options (${EXECUTABLE_5} PUBLIC "$<$<CXX_COMPILER_ID:GNU>:-Wl,--allow-multiple-definition>")
 target_link_options (${EXECUTABLE_6} PUBLIC "$<$<CXX_COMPILER_ID:GNU>:-Wl,--allow-multiple-definition>")
 target_link_options (${EXECUTABLE_7} PUBLIC "$<$<CXX_COMPILER_ID:GNU>:-Wl,--allow-multiple-definition>")
 target_link_options (${EXECUTABLE_8} PUBLIC "$<$<CXX_COMPILER_ID:GNU>:-Wl,--allow-multiple-definition>")
 target_link_options (${EXECUTABLE_9} PUBLIC "$<$<CXX_COMPILER_ID:GNU>:-Wl,--allow-multiple-definition>")
 target_link_options (${EXECUTABLE_12} PUBLIC "$<$<CXX_COMPILER_ID:GNU>:-Wl,--allow-multiple-definition>")
 target_link_options (${EXECUTABLE_15} PUBLIC "$<$<CXX_COMPILER_ID:GNU>:-Wl,--allow-multiple-definition>")
 target_link_options (${EXECUTABLE_17} PUBLIC "$<$<CXX_COMPILER_ID:GNU>:-Wl,--allow-multiple-definition>")
 target_link_options (${EXECUTABLE_18} PUBLIC "$<$<CXX_COMPILER_ID:GNU>:-Wl,--allow-multiple-definition>")
 target_link_options (${EXECUTABLE_19} PUBLIC "$<$<CXX_COMPILER_ID:GNU>:-Wl,--allow-multiple-definition>")
 target_link_options (${EXECUTABLE_20} PUBLIC "$<$<CXX_COMPILER_ID:GNU>:-Wl,--allow-multiple-definition>")
 target_link_options (${EXECUTABLE_21} PUBLIC "$<$<CXX_COMPILER_ID:GNU>:-Wl,--allow-multiple-definition>")
 target_link_options (${EXECUTABLE_22} PUBLIC "$<$<CXX_COMPILER_ID:GNU>:-Wl,--allow-multiple-definition>")
 target_link_options (${EXECUTABLE_23} PUBLIC "$<$<CXX_COMPILER_ID:GNU>:-Wl,--allow-multiple-definition>")
 target_link_options (${EXECUTABLE_24} PUBLIC "$<$<CXX_COMPILER_ID:GNU>:-Wl,--allow-multiple-definition>")
 target_link_options (${EXECUTABLE_25} PUBLIC "$<$<CXX_COMPILER_ID:GNU>:-Wl,--allow-multiple-definition>")
 target_link_options (${EXECUTABLE_26} PUBLIC "$<$<CXX_COMPILER_ID:GNU>:-Wl,--allow-multiple-definition>")
 target_link_options (${EXECUTABLE_27} PUBLIC "$<$<CXX_COMPILER_ID:GNU>:-Wl,--allow-multiple-definition>")
 target_link_options (${EXECUTABLE_28} PUBLIC "$<$<CXX_COMPILER_ID:GNU>:-Wl,--allow-multiple-definition>")
 target_link_options (${EXECUTABLE_29} PUBLIC "$<$<CXX_COMPILER_ID:GNU>:-Wl,--allow-multiple-definition>")
 target_link_options (${EXECUTABLE_30} PUBLIC "$<$<CXX_COMPILER_ID:GNU>:-Wl,--allow-multiple-definition>")
 target_link_options (${EXECUTABLE_31} PUBLIC "$<$<CXX_COMPILER_ID:GNU>:-Wl,--allow-multiple-definition>")
 target_link_options (${EXECUTABLE_32} PUBLIC "$<$<CXX_COMPILER_ID:GNU>:-Wl,--allow-multiple-definition>")
 target_link_options (${EXECUTABLE_33} PUBLIC "$<$<CXX_COMPILER_ID:GNU>:-Wl,--allow-multiple-definition>")
 target_link_options (${EXECUTABLE_34} PUBLIC "$<$<CXX_COMPILER_ID:GNU>:-Wl,--allow-multiple-definition>")
 target_link_options (${EXECUTABLE_35} PUBLIC "$<$<CXX_COMPILER_ID:GNU>:-Wl,--allow-multiple-definition>")
 target_link_options (${EXECUTABLE_36} PUBLIC "$<$<CXX_COMPILER_ID:GNU>:-Wl,--allow-multiple-definition>")
 target_link_options (${EXECUTABLE_37} PUBLIC "$<$<CXX_COMPILER_ID:GNU>:-Wl,--allow-multiple-definition>")
 target_link_options (${EXECUTABLE_38} PUBLIC "$<$<CXX_COMPILER_ID:GNU>:-Wl,--allow-multiple-definition>")
 target_link_options (${EXECUTABLE_39} PUBLIC "$<$<CXX_COMPILER_ID:GNU>:-Wl,--allow-multiple-definition>")
elseif (WIN32)
 set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /STACK:20000000")
 target_link_options (${EXECUTABLE_5} PUBLIC "$<$<CXX_COMPILER_ID:MSVC>:-FORCE:MULTIPLE>")
 target_link_options (${EXECUTABLE_6} PUBLIC "$<$<CXX_COMPILER_ID:MSVC>:-FORCE:MULTIPLE>")
 target_link_options (${EXECUTABLE_7} PUBLIC "$<$<CXX_COMPILER_ID:MSVC>:-FORCE:MULTIPLE>")
 target_link_options (${EXECUTABLE_8} PUBLIC "$<$<CXX_COMPILER_ID:MSVC>:-FORCE:MULTIPLE>")
 target_link_options (${EXECUTABLE_9} PUBLIC "$<$<CXX_COMPILER_ID:MSVC>:-FORCE:MULTIPLE>")
 target_link_options (${EXECUTABLE_12} PUBLIC "$<$<CXX_COMPILER_ID:MSVC>:-FORCE:MULTIPLE>")
 target_link_options (${EXECUTABLE_15} PUBLIC "$<$<CXX_COMPILER_ID:MSVC>:-FORCE:MULTIPLE>")
 target_link_options (${EXECUTABLE_17} PUBLIC "$<$<CXX_COMPILER_ID:MSVC>:-FORCE:MULTIPLE>")
 target_link_options (${EXECUTABLE_18} PUBLIC "$<$<CXX_COMPILER_ID:MSVC>:-FORCE:MULTIPLE>")
 target_link_options (${EXECUTABLE_19} PUBLIC "$<$<CXX_COMPILER_ID:MSVC>:-FORCE:MULTIPLE>")
 target_link_options (${EXECUTABLE_20} PUBLIC "$<$<CXX_COMPILER_ID:MSVC>:-FORCE:MULTIPLE>")
 target_link_options (${EXECUTABLE_21} PUBLIC "$<$<CXX_COMPILER_ID:MSVC>:-FORCE:MULTIPLE>")
 target_link_options (${EXECUTABLE_22} PUBLIC "$<$<CXX_COMPILER_ID:MSVC>:-FORCE:MULTIPLE>")
 target_link_options (${EXECUTABLE_23} PUBLIC "$<$<CXX_COMPILER_ID:MSVC>:-FORCE:MULTIPLE>")
 target_link_options (${EXECUTABLE_24} PUBLIC "$<$<CXX_COMPILER_ID:MSVC>:-FORCE:MULTIPLE>")
 target_link_options (${EXECUTABLE_25} PUBLIC "$<$<CXX_COMPILER_ID:MSVC>:-FORCE:MULTIPLE>")
 target_link_options (${EXECUTABLE_26} PUBLIC "$<$<CXX_COMPILER_ID:MSVC>:-FORCE:MULTIPLE>")
 target_link_options (${EXECUTABLE_27} PUBLIC "$<$<CXX_COMPILER_ID:MSVC>:-FORCE:MULTIPLE>")
 target_link_options (${EXECUTABLE_28} PUBLIC "$<$<CXX_COMPILER_ID:MSVC>:-FORCE:MULTIPLE>")
 target_link_options (${EXECUTABLE_29} PUBLIC "$<$<CXX_COMPILER_ID:MSVC>:-FORCE:MULTIPLE>")
 target_link_options (${EXECUTABLE_30} PUBLIC "$<$<CXX_COMPILER_ID:MSVC>:-FORCE:MULTIPLE>")
 target_link_options (${EXECUTABLE_31} PUBLIC "$<$<CXX_COMPILER_ID:MSVC>:-FORCE:MULTIPLE>")
 target_link_options (${EXECUTABLE_32} PUBLIC "$<$<CXX_COMPILER_ID:MSVC>:-FORCE:MULTIPLE>")
 target_link_options (${EXECUTABLE_33} PUBLIC "$<$<CXX_COMPILER_ID:MSVC>:-FORCE:MULTIPLE>")
 target_link_options (${EXECUTABLE_34} PUBLIC "$<$<CXX_COMPILER_ID:MSVC>:-FORCE:MULTIPLE>")
 target_link_options (${EXECUTABLE_35} PUBLIC "$<$<CXX_COMPILER_ID:MSVC>:-FORCE:MULTIPLE>")
 target_link_options (${EXECUTABLE_36} PUBLIC "$<$<CXX_COMPILER_ID:MSVC>:-FORCE:MULTIPLE>")
 target_link_options (${EXECUTABLE_37} PUBLIC "$<$<CXX_COMPILER_ID:MSVC>:-FORCE:MULTIPLE>")
 target_link_options (${EXECUTABLE_38} PUBLIC "$<$<CXX_COMPILER_ID:MSVC>:-FORCE:MULTIPLE>")
 target_link_options (${EXECUTABLE_39} PUBLIC "$<$<CXX_COMPILER_ID:MSVC>:-FORCE:MULTIPLE>")
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
target_precompile_headers (${EXECUTABLE_9}
                           PRIVATE stdafx.h) # c++std17
target_precompile_headers (${EXECUTABLE_10}
                           REUSE_FROM ${EXECUTABLE_9})
target_precompile_headers (${EXECUTABLE_11}
                           REUSE_FROM ${EXECUTABLE_9})
target_precompile_headers (${EXECUTABLE_12}
                           REUSE_FROM ${EXECUTABLE})
target_precompile_headers (${EXECUTABLE_13}
                           REUSE_FROM ${EXECUTABLE_9})
target_precompile_headers (${EXECUTABLE_14}
                           REUSE_FROM ${EXECUTABLE})
target_precompile_headers (${EXECUTABLE_15}
                           REUSE_FROM ${EXECUTABLE})
target_precompile_headers (${EXECUTABLE_16}
                           REUSE_FROM ${EXECUTABLE})
target_precompile_headers (${EXECUTABLE_17}
                           REUSE_FROM ${EXECUTABLE})
target_precompile_headers (${EXECUTABLE_18}
                           REUSE_FROM ${EXECUTABLE})
target_precompile_headers (${EXECUTABLE_19}
                           REUSE_FROM ${EXECUTABLE})
target_precompile_headers (${EXECUTABLE_20}
                           REUSE_FROM ${EXECUTABLE})
target_precompile_headers (${EXECUTABLE_21}
                           REUSE_FROM ${EXECUTABLE})
target_precompile_headers (${EXECUTABLE_22}
                           REUSE_FROM ${EXECUTABLE})
target_precompile_headers (${EXECUTABLE_23}
                           REUSE_FROM ${EXECUTABLE})
target_precompile_headers (${EXECUTABLE_24}
                           REUSE_FROM ${EXECUTABLE})
target_precompile_headers (${EXECUTABLE_25}
                           REUSE_FROM ${EXECUTABLE})
target_precompile_headers (${EXECUTABLE_26}
                           REUSE_FROM ${EXECUTABLE})
target_precompile_headers (${EXECUTABLE_27}
                           REUSE_FROM ${EXECUTABLE})
target_precompile_headers (${EXECUTABLE_28}
                           REUSE_FROM ${EXECUTABLE})
target_precompile_headers (${EXECUTABLE_29}
                           REUSE_FROM ${EXECUTABLE})
target_precompile_headers (${EXECUTABLE_30}
                           REUSE_FROM ${EXECUTABLE})
target_precompile_headers (${EXECUTABLE_31}
                           REUSE_FROM ${EXECUTABLE})
target_precompile_headers (${EXECUTABLE_32}
                           REUSE_FROM ${EXECUTABLE})
target_precompile_headers (${EXECUTABLE_33}
                           REUSE_FROM ${EXECUTABLE})
target_precompile_headers (${EXECUTABLE_34}
                           REUSE_FROM ${EXECUTABLE})
target_precompile_headers (${EXECUTABLE_35}
                           REUSE_FROM ${EXECUTABLE})
target_precompile_headers (${EXECUTABLE_36}
                           REUSE_FROM ${EXECUTABLE})
target_precompile_headers (${EXECUTABLE_37}
                           REUSE_FROM ${EXECUTABLE})
target_precompile_headers (${EXECUTABLE_38}
                           REUSE_FROM ${EXECUTABLE})
target_precompile_headers (${EXECUTABLE_39}
                           REUSE_FROM ${EXECUTABLE})

install (TARGETS ${EXECUTABLE} ${EXECUTABLE_2} ${EXECUTABLE_3} ${EXECUTABLE_4} ${EXECUTABLE_5} ${EXECUTABLE_6}
                 ${EXECUTABLE_7} ${EXECUTABLE_8} ${EXECUTABLE_9} ${EXECUTABLE_10} ${EXECUTABLE_11} ${EXECUTABLE_12}
                 ${EXECUTABLE_13} ${EXECUTABLE_14} ${EXECUTABLE_15} ${EXECUTABLE_16} ${EXECUTABLE_17} ${EXECUTABLE_18}
                 ${EXECUTABLE_19} ${EXECUTABLE_20} ${EXECUTABLE_21} ${EXECUTABLE_22} ${EXECUTABLE_23} ${EXECUTABLE_24}
                 ${EXECUTABLE_25} ${EXECUTABLE_26} ${EXECUTABLE_27} ${EXECUTABLE_28} ${EXECUTABLE_29} ${EXECUTABLE_30}
                 ${EXECUTABLE_31} ${EXECUTABLE_32} ${EXECUTABLE_33} ${EXECUTABLE_34} ${EXECUTABLE_35} ${EXECUTABLE_36}
                 ${EXECUTABLE_37} ${EXECUTABLE_38} ${EXECUTABLE_39}
         RUNTIME
         DESTINATION bin
         COMPONENT applications)
install (FILES
         "800x500.png"
         "2017.png"
         "Apple_Computer_Logo_rainbow.png"
         "background_pattern_tenth.png"
         "bladen.png"
         "bluedark.png"
         "cat.jpg"
         "circle.png"
         "gameboy.png"
         "kitty-cat-kitten-pet.jpg"
         "opengl_logo.png"
         "peace.dat"
         "tetra2.png"
         "glut_3.gtk2"
         "glut_3.gtk3"
         "glut_16.gtk2"
         "glut_16.gtk3"
         "glut_169.vert"
         "glut_169.frag"
         "glut_222.vert"
         "glut_222.frag"
         "glut_228.vert"
         "glut_228.frag"
         "glut_239.vert"
         "glut_239.frag"
         "glut_241.vert"
         "glut_241.frag"
         "glut_246.vert"
         "glut_246.frag"
         "glut_248.vert"
         "glut_248.frag"
         "glut_251.vert"
         "glut_251.frag"
         "glut_269.vert"
         "glut_269.frag"
         "glut_274.vert"
         "glut_274.frag"
         "glut_277.vert"
         "glut_277.frag"
         "glut_282.vert"
         "glut_282.frag"
         "glut_293.vert"
         "glut_293.frag"
         "glut_302.vert"
         "glut_302.frag"
         "glut_304.vert"
         "glut_304.frag"
         "glut_311.vert"
         "glut_311.frag"
         "glut_312.vert"
         "glut_312.frag"
         "glut_313.vert"
         "glut_313.frag"
         "glut_321.vert"
         "glut_321.frag"
         "glut_322.vert"
         "glut_322.frag"
         "glut_347.vert"
         "glut_347.frag"
         "glut_357.vert"
         "glut_357.frag"
         "glut_358.vert"
         "glut_358.frag"
         "glut_363.vert"
         "glut_363.frag"
         "glut_366.vert"
         "glut_366.frag"
         "glut_369.vert"
         "glut_369.frag"
         "glut_370.vert"
         "glut_370.frag"
         "glut_371.vert"
         "glut_371.frag"
         "glut_373.vert"
         "glut_373.frag"
         "glut_374.vert"
         "glut_374.frag"
         "glut_375.vert"
         "glut_375.frag"
         "glut_377.vert"
         "glut_377.frag"
         "glut_379.vert"
         "glut_379.frag"
         "glut_382.vert"
         "glut_382.frag"
         "glut_383.vert"
         "glut_383.frag"
         "glut_386.vert"
         "glut_386.frag"
         "glut_388.vert"
         "glut_388.frag"
         "glut_389.vert"
         "glut_389.frag"
         "glut_390.vert"
         "glut_390.frag"
         "glut_391.vert"
         "glut_391.frag"
         "glut_392.vert"
         "glut_392.frag"
         "glut_394.vert"
         "glut_394.frag"
         "glut_398.vert"
         "glut_398.frag"
         "glut_407.vert"
         "glut_407.frag"
         "glut_408.vert"
         "glut_408.frag"
         "glut_410.vert"
         "glut_410.frag"
         "glut_412.vert"
         "glut_412.frag"
         "glut_413.vert"
         "glut_413.frag"
         "glut_415.vert"
         "glut_415.frag"
         "glut_425.vert"
         "glut_425.frag"
         "glut_428.vert"
         "glut_428.frag"
         "glut_431.vert"
         "glut_431.frag"
         "glut_433.vert"
         "glut_433.frag"
         "glut_434.vert"
         "glut_434.frag"
         "glut_436.vert"
         "glut_436.frag"
         "glut_441.vert"
         "glut_441.frag"
         "glut_442.vert"
         "glut_442.frag"
         "glut_444.vert"
         "glut_444.frag"
         "glut_445.vert"
         "glut_445.frag"
         "glut_446.vert"
         "glut_446.frag"
         "glut_448.vert"
         "glut_448.frag"
         "glut_451.vert"
         "glut_451.frag"
         "glut_453.vert"
         "glut_453.frag"
         "glut_456.vert"
         "glut_456.frag"
         "glut_457.vert"
         "glut_457.frag"
         "glut_458.vert"
         "glut_458.frag"
         "glut_459.vert"
         "glut_459.frag"
         "glut_459.gtk3"
         "glut_460.vert"
         "glut_460.frag"
         "glut_465.vert"
         "glut_465.frag"
         "glut_466.vert"
         "glut_466.frag"
         "glut_467.vert"
         "glut_467.frag"
         "glut_467_channel0.png"
         "glut_467_channel1.png"
         "glut_467_channel2.png"
         "glut_468.vert"
         "glut_468.frag"
         "glut_469.vert"
         "glut_469.frag"
         "glut_470.vert"
         "glut_470.frag"
         "glut_471.vert"
         "glut_471.frag"
         "glut_472.vert"
         "glut_472.frag"
         "glut_473.vert"
         "glut_473.frag"
         "glut_474.vert"
         "glut_474.frag"
         "glut_475.vert"
         "glut_475.frag"
         "glut_476.vert"
         "glut_476.frag"
         "glut_476_channel0.png"
         "glut_477.vert"
         "glut_477.frag"
         "glut_477_channel0.png"
         "glut_478.vert"
         "glut_478.frag"
         "glut_478.vert_2"
         "glut_478.frag_2"
         "glut_478_channel0.png"
         "glut_478_channel1.png"
         "glut_479.vert"
         "glut_479.frag"
         "glut_480.vert"
         "glut_480.frag"
         "glut_481.vert"
         "glut_481.frag"
         "glut_481.vert_2"
         "glut_481.frag_2"
         "glut_481_channel0.png"
         "glut_482.vert"
         "glut_482.frag"
         "glut_483.vert"
         "glut_483.frag"
         "glut_484.vert"
         "glut_484.frag"
         "glut_484.vert_2"
         "glut_484.frag_2"
         "glut_484.vert_3"
         "glut_484.frag_3"
         "glut_484.vert_4"
         "glut_484.frag_4"
         "glut_485.vert"
         "glut_485.frag"
         "glut_485.vert_2"
         "glut_485.frag_2"
         "glut_485.vert_3"
         "glut_485.frag_3"
         "glut_485.vert_4"
         "glut_485.frag_4"
         "glut_486.vert"
         "glut_486.frag"
         "glut_486.vert_2"
         "glut_486.frag_2"
         "glut_487_texture0.png"
         "glut_487.vert"
         "glut_487.frag"
         "glut_487.frag_2"
         "glut_487.frag_3"
         "glut_487.frag_4"
         "glut_488.vert"
         "glut_488.frag"
         "glut_488.frag_2"
         "glut_488.frag_3"
         "glut_488.frag_4"
         "glut_488.frag_5"
         "glut_489.vert"
         "glut_489.frag"
         "glut_489.frag_2"
         "glut_490.vert"
         "glut_490.frag"
         "glut_491_channel0.png"
         "glut_491.vert"
         "glut_491.frag"
         "glut_491.frag_2"
         "glut_492.vert"
         "glut_492.frag"
         "glut_493.vert"
         "glut_493.frag"
         "glut_494.vert"
         "glut_494.frag"
         "glut_494_channel0.png"
         "glut_494_channel1.png"
         "glut_495.vert"
         "glut_495.frag"
         "glut_495_channel0.png"
         "glut_496.vert"
         "glut_496.frag"
         "glut_497.vert"
         "glut_497.frag"
         "glut_497.frag_2"
         "glut_497.frag_3"
         "glut_497.frag_4"
         "glut_497.frag_5"
         "glut_498.vert"
         "glut_498.frag"
         "glut_499.vert"
         "glut_499.frag"
         "glut_499_channel0.png"
         "glut_500.vert"
         "glut_500.frag"
         "glut_500_channel0.png"
         "glut_501.vert"
         "glut_501.frag"
         "glut_502.vert"
         "glut_502.frag"
         "glut_503.vert"
         "glut_503.frag"
         "glut_503_channel0.png"
         "glut_503_channel1.png"
         "glut_504.vert"
         "glut_504.frag"
         "glut_504.frag_2"
         "glut_504.frag_3"
         "glut_504.frag_4"
         "glut_504.frag_5"
         "glut_504_channel0.png"
         "glut_505.vert"
         "glut_505.frag"
         "glut_505.frag_2"
         "glut_505.frag_3"
         "glut_505.frag_4"
         "glut_505.frag_5"
         "glut_506.vert"
         "glut_506.frag"
         "glut_506.frag_2"
         "glut_506.frag_3"
         "glut_506.frag_4"
         "glut_506.frag_5"
         "glut_507.vert"
         "glut_507.frag"
         "glut_507.frag_2"
         "glut_508.vert"
         "glut_508.frag"
         "glut_509.vert"
         "glut_509.frag"
         "glut_509_channel0.png"
         "glut_510.vert"
         "glut_510.frag"
         "glut_511.vert"
         "glut_511.frag"
         "glut_511.frag_2"
         "glut_511.frag_3"
         "glut_512.vert"
         "glut_512.frag"
         "glut_512_channel0.png"
         "glut_513.vert"
         "glut_513.frag"
         "glut_514.vert"
         "glut_514.frag"
         "glut_515.vert"
         "glut_515.frag"
         "glut_516.vert"
         "glut_516.frag"
         "glut_516.frag_2"
         "glut_516.frag_3"
         "glut_516.frag_4"
         "glut_516.frag_5"
         "glut_517.vert"
         "glut_517.frag"
         "glut_518.vert"
         "glut_518.frag"
         "glut_519.vert"
         "glut_519.frag"
         "glut_520.vert"
         "glut_520.frag"
         "glut_520.frag_2"
         "glut_521.vert"
         "glut_521.frag"
         "glut_522.vert"
         "glut_522.frag"
         "glut_523.vert"
         "glut_523.frag"
         "glut_523.frag_2"
         "glut_524.vert"
         "glut_524.frag"
         "glut_525.vert"
         "glut_525.frag"
         "glut_525.frag_2"
         "glut_525.frag_3"
         "glut_525.frag_4"
         "glut_525.frag_5"
         "glut_526.vert"
         "glut_526.frag"
         "glut_527.vert"
         "glut_527.frag"
         "glut_528.vert"
         "glut_528.frag"
         "glut_528.frag_2"
         "glut_528.frag_3"
         "glut_528_bufferA_channel1.png"
         "glut_528_image_channel1.png"
         "glut_529.vert"
         "glut_529.frag"
         "glut_530.vert"
         "glut_530.frag"
         "glut_531.vert"
         "glut_531.frag"
         "glut_531.frag_2"
         "glut_531.frag_3"
         "glut_531.frag_4"
         "glut_531.frag_5"
         "glut_532.vert"
         "glut_532.frag"
         "glut_533.vert"
         "glut_533.frag"
         "glut_533_channel0.png"
         "glut_534.vert"
         "glut_534.frag"
         "glut_534_channel0.png"
         "glut_534_channel1.png"
         "glut_534_channel2.png"
         "glut_535.vert"
         "glut_535.frag"
         "glut_535_channel0.png"
         "glut_536.vert"
         "glut_536.frag"
         "glut_537.vert"
         "glut_537.frag"
         "glut_538.vert"
         "glut_538.frag"
         "glut_539.vert"
         "glut_539.frag"
         "glut_540.vert"
         "glut_540.frag"
         "glut_541.vert"
         "glut_541.frag"
         "glut_542.vert"
         "glut_542.frag"
         "glut_543.vert"
         "glut_543.frag"
         "glut_544.vert"
         "glut_544.frag"
         "glut_545.vert"
         "glut_545.frag"
         "glut_545.frag_2"
         "glut_546.vert"
         "glut_546.frag"
         "glut_546_channel0.png"
         "glut_546_channel1.png"
         "glut_546_channel2.png"
         "glut_547.vert"
         "glut_547.frag"
         "glut_548.vert"
         "glut_548.frag"
         "glut_549.vert"
         "glut_549.frag"
         "glut_549_channel0.png"
         "glut_550.vert"
         "glut_550.frag"
         "glut_551.vert"
         "glut_551.frag"
         "glut_551.frag_2"
         "glut_551.frag_3"
         "glut_551.frag_4"
         "glut_551.frag_5"
         "glut_552.vert"
         "glut_552.frag"
         "glut_552.frag_2"
         "glut_552.frag_3"
         "glut_552.frag_4"
         "glut_552.frag_5"
         "glut_553.vert"
         "glut_553.frag"
         "glut_554.vert"
         "glut_554.frag"
         "glut_554.frag_2"
         "glut_554.frag_3"
         "glut_555.vert"
         "glut_555.frag"
         "glut_555.frag_2"
         "glut_555.frag_3"
         "glut_555.frag_4"
         "glut_555.frag_5"
         "glut_556.vert"
         "glut_556.frag"
         "glut_556.frag_2"
         "glut_557.vert"
         "glut_557.frag"
         "glut_557.frag_2"
         "glut_557.frag_3"
         "glut_558.vert"
         "glut_558.frag"
         "glut_559.vert"
         "glut_559.frag"
         "glut_559_channel0.png"
         "glut_560.vert"
         "glut_560.frag"
         "glut_561.vert"
         "glut_561.frag"
         "glut_561_channel0.png"
         "glut_562.vert"
         "glut_562.frag"
         "glut_563.vert"
         "glut_563.frag"
         "glut_564.vert"
         "glut_564.frag"
         "glut_564_channel1.png"
         "glut_564_channel2.png"
         "glut_565.vert"
         "glut_565.frag"
         "glut_565_channel1.png"
         "glut_565_channel2.png"
         "glut_566.vert"
         "glut_566.frag"
         "glut_566_channel0.png"
         "glut_567.vert"
         "glut_567.frag"
         "glut_567_channel0.png"
         "glut_568.vert"
         "glut_568.frag"
         "glut_569.vert"
         "glut_569.frag"
         "glut_570.vert"
         "glut_570.frag"
         "glut_571.vert"
         "glut_571.frag"
         "glut_572.vert"
         "glut_572.frag"
         "glut_572_channel0.png"
         "glut_573.vert"
         "glut_573.frag"
         "glut_573.frag_2"
         "glut_574.vert"
         "glut_574.frag"
         "glut_574.frag_2"
         "glut_575.vert"
         "glut_575.frag"
         "glut_575.frag_2"
         "glut_575_channel1.png"
         "glut_576.vert"
         "glut_576.frag"
         "glut_577.vert"
         "glut_577.frag"
         "glut_577_channel0.png"
         "glut_578.vert"
         "glut_578.frag"
         "glut_579.vert"
         "glut_579.frag"
         "glut_580.vert"
         "glut_580.frag"
         "glut_580.frag_2"
         "glut_580.frag_3"
         "glut_580.frag_4"
         "glut_580.frag_5"
         "glut_580_channel1.png"
         "glut_581.vert"
         "glut_581.frag"
         "glut_581.frag_2"
         "glut_581.frag_3"
         "glut_581.frag_4"
         "glut_581.frag_5"
         "glut_582.vert"
         "glut_582.frag"
         "glut_583.vert"
         "glut_583.frag"
         "glut_584.vert"
         "glut_584.frag"
         "glut_585.vert"
         "glut_585.frag"
         "glut_585_channel0.png"
         "glut_585_channel1.png"
         "glut_586.vert"
         "glut_586.frag"
         "glut_586_channel0.png"
         "glut_587.vert"
         "glut_587.frag"
         "glut_588.vert"
         "glut_588.frag"
         "glut_589.vert"
         "glut_589.frag"
         "glut_590.vert"
         "glut_590.frag"
         "glut_591.vert"
         "glut_591.frag"
         "glut_592.vert"
         "glut_592.frag"
         "glut_593.vert"
         "glut_593.frag"
         "glut_593.frag_2"
         "glut_593.frag_3"
         "glut_593.frag_4"
         "glut_593.frag_5"
         "glut_594.vert"
         "glut_594.frag"
         "glut_595.vert"
         "glut_595.frag"
         "glut_595_channel0.png"
         "glut_595_channel1.png"
         "glut_596.vert"
         "glut_596.frag"
         "glut_596_channel0.png"
         "glut_597.vert"
         "glut_597.frag"
         "glut_598.vert"
         "glut_598.frag"
         "glut_598_channel0.png"
         "glut_599.vert"
         "glut_599.frag"
         "glut_599.frag_2"
         "glut_600.vert"
         "glut_600.frag"
         "glut_600.frag_2"
         "glut_600.frag_3"
         "glut_601.vert"
         "glut_601.frag"
         "glut_601.frag_2"
         "glut_602.vert"
         "glut_602.frag"
         "glut_602_channel0.png"
         "glut_603.vert"
         "glut_603.frag"
         "glut_604.vert"
         "glut_604.frag"
         "glut_605.vert"
         "glut_605.frag"
         "glut_606.vert"
         "glut_606.frag"
         "glut_606.frag_2"
         "glut_607.vert"
         "glut_607.frag"
         "glut_607.frag_2"
         "glut_608.vert"
         "glut_608.frag"
         "glut_609.vert"
         "glut_609.frag"
         "glut_610.vert"
         "glut_610.frag"
         "glut_611.vert"
         "glut_611.frag"
         "glut_612.vert"
         "glut_612.frag"
         "glut_613.vert"
         "glut_613.frag"
         "glut_614.vert"
         "glut_614.frag"
         "glut_615.vert"
         "glut_615.frag"
         "glut_616.vert"
         "glut_616.frag"
         "glut_617.vert"
         "glut_617.frag"
         "glut_617_channel0.png"
         "glut_618.vert"
         "glut_618.frag"
         "glut_618.frag_2"
         "glut_619.vert"
         "glut_619.frag"
         "glut_619.frag_2"
         "glut_620.vert"
         "glut_620.frag"
         "glut_620.frag_2"
         "glut_620.frag_3"
         "glut_621.vert"
         "glut_621.frag"
         "glut_621.frag_2"
         "glut_621.frag_3"
         "glut_621.frag_4"
         "glut_622.vert"
         "glut_622.frag"
         "glut_622_channel0.png"
         "glut_622_channel1.png"
         "glut_623.vert"
         "glut_623.frag"
         "glut_623.frag_2"
         "glut_624.vert"
         "glut_624.frag"
         "glut_624.frag_2"
         "glut_624.frag_3"
         "glut_625.vert"
         "glut_625.frag"
         "glut_625.frag_2"
         "glut_625.frag_3"
         "glut_625.frag_4"
         "glut_626.vert"
         "glut_626.frag"
         "glut_626.frag_2"
         "glut_626.frag_3"
         "glut_626.frag_4"
         "glut_626.frag_5"
         "glut_627.vert"
         "glut_627.frag"
         "glut_627_channel0.png"
         "glut_628.vert"
         "glut_628.frag"
         "glut_629.vert"
         "glut_629.frag"
         "glut_630.vert"
         "glut_630.frag"
         "glut_631.vert"
         "glut_631.frag"
         "glut_632.vert"
         "glut_632.frag"
         "glut_633.vert"
         "glut_633.frag"
         "glut_633_channel0.png"
         "glut_633_channel1.png"
         "glut_634.vert"
         "glut_634.frag"
         "glut_635.vert"
         "glut_635.frag"
         "glut_635_channel0.png"
         "glut_636.vert"
         "glut_636.frag"
         "glut_636.frag_2"
         "glut_636.frag_3"
         "glut_636.frag_4"
         "glut_637.vert"
         "glut_637.frag"
         "glut_637.frag_2"
         "glut_637.frag_3"
         "glut_637.frag_4"
         "glut_637.frag_5"
         "glut_638.vert"
         "glut_638.frag"
         "glut_639.vert"
         "glut_639.frag"
         "glut_640.vert"
         "glut_640.frag"
         "glut_641.vert"
         "glut_641.frag"
         "glut_641_channel0.png"
         "glut_642.vert"
         "glut_642.frag"
         "glut_643.vert"
         "glut_643.frag"
         "glut_643.frag_2"
         "glut_644.vert"
         "glut_644.frag"
         "glut_645.vert"
         "glut_645.frag"
         "glut_646.vert"
         "glut_646.frag"
         "glut_647.vert"
         "glut_647.frag"
         "glut_647_channel0.png"
         "glut_648.vert"
         "glut_648.frag"
         "glut_648.frag_2"
         "glut_648_channel1.png"
         "glut_649.vert"
         "glut_649.frag"
         "glut_649_channel0.png"
         "glut_650.vert"
         "glut_650.frag"
         "glut_651.vert"
         "glut_651.frag"
         "glut_652.vert"
         "glut_652.frag"
         "glut_652.frag_2"
         "glut_652_channel1.png"
         "glut_653.vert"
         "glut_653.frag"
         "glut_654.vert"
         "glut_654.frag"
         "glut_654_channel0.png"
         "glut_654_channel1.png"
         "glut_655.vert"
         "glut_655.frag"
         "glut_656.vert"
         "glut_656.frag"
         "glut_656.frag_2"
         "glut_656.frag_3"
         "glut_656.frag_4"
         "glut_656.frag_5"
         "glut_657.vert"
         "glut_657.frag"
         "glut_658.vert"
         "glut_658.frag"
         "glut_659.vert"
         "glut_659.frag"
         "glut_660.vert"
         "glut_660.frag"
         "glut_661.vert"
         "glut_661.frag"
         "glut_661.frag_2"
         "glut_661_S1_channel1.png"
         "glut_661_S2_channel0.png"
         "glut_662.vert"
         "glut_662.frag"
         "glut_663.vert"
         "glut_663.frag"
         "glut_663.frag_2"
         "glut_663_channel1.png"
         "glut_664.vert"
         "glut_664.frag"
         "glut_665.vert"
         "glut_665.frag"
         "glut_665.frag_2"
         "glut_665.frag_3"
         "glut_665.frag_4"
         "glut_666.vert"
         "glut_666.frag"
         "glut_666.frag_2"
         "glut_666.frag_3"
         "glut_666.frag_4"
         "glut_666.frag_5"
         "glut_667.vert"
         "glut_667.frag"
         "glut_667.frag_2"
         "glut_668.vert"
         "glut_668.frag"
         "glut_668.frag_2"
         "glut_669.vert"
         "glut_669.frag"
         "glut_669_channel0.png"
         "glut_669_channel1.png"
         "glut_670.vert"
         "glut_670.frag"
         "glut_670.frag_2"
         "glut_671.vert"
         "glut_671.frag"
         "glut_671.frag_2"
         "glut_672.vert"
         "glut_672.frag"
         "glut_673.vert"
         "glut_673.frag"
         "glut_674.vert"
         "glut_674.frag"
         "glut_674_channel0.png"
         "glut_675.vert"
         "glut_675.frag"
         "glut_675_channel0.png"
         "glut_676.vert"
         "glut_676.frag"
         "glut_677.vert"
         "glut_677.frag"
         "glut_678.vert"
         "glut_678.frag"
         "glut_678.frag_2"
         "glut_678.frag_3"
         "glut_679.vert"
         "glut_679.frag"
         "glut_680.vert"
         "glut_680.frag"
         "glut_681.vert"
         "glut_681.frag"
         "glut_682.vert"
         "glut_682.frag"
         "glut_682_channel0.png"
         "glut_683.vert"
         "glut_683.frag"
         "glut_683.frag_2"
         "glut_684.vert"
         "glut_684.frag"
         "glut_684.frag_2"
         "glut_684.frag_3"
         "glut_685.vert"
         "glut_685.frag"
         "glut_685_channel0.png"
         "glut_685_channel1.png"
         "glut_686.vert"
         "glut_686.frag"
         "glut_686_channel0.png"
         "glut_687.vert"
         "glut_687.frag"
         "glut_688.vert"
         "glut_688.frag"
         "glut_688_channel0.png"
         "glut_688_channel1.png"
         "glut_688_channel2.png"
         "glut_688_channel3.png"
         "glut_689.vert"
         "glut_689.frag"
         "glut_689.frag_2"
         "glut_689.frag_3"
         "glut_690.vert"
         "glut_690.frag"
         "glut_690.frag_2"
         "glut_690.frag_3"
         "glut_690.frag_4"
         "glut_690.frag_5"
         "glut_691.vert"
         "glut_691.frag"
         "glut_691.frag_2"
         "glut_691.frag_3"
         "glut_691.frag_4"
         "glut_692.vert"
         "glut_692.frag"
         "glut_692_channel0.png"
         "glut_692_channel1.png"
         "glut_693.vert"
         "glut_693.frag"
         "glut_693.frag_2"
         "glut_693.frag_3"
         "glut_694.vert"
         "glut_694.frag"
         "glut_694_channel0.png"
         "glut_694_channel1.png"
         "glut_695.vert"
         "glut_695.frag"
         "glut_696.vert"
         "glut_696.frag"
         "glut_697.vert"
         "glut_697.frag"
         "glut_698.vert"
         "glut_698.frag"
         "glut_699.vert"
         "glut_699.frag"
         "glut_700.vert"
         "glut_700.frag"
         "glut_701.vert"
         "glut_701.frag"
         "glut_702.vert"
         "glut_702.frag"
         "glut_702.frag_2"
         "glut_702.frag_3"
         "glut_703.vert"
         "glut_703.frag"
         "glut_703_channel0.png"
         "glut_704.vert"
         "glut_704.frag"
         "glut_704_channel0.png"
         "glut_704_channel1.png"
         "glut_705.vert"
         "glut_705.frag"
         "glut_706.vert"
         "glut_706.frag"
         "glut_707.vert"
         "glut_707.frag"
         "glut_707_channel0.png"
         "glut_708.vert"
         "glut_708.frag"
         "glut_708.frag_2"
         "glut_708.frag_3"
         "glut_708.frag_4"
         "glut_708.frag_5"
         "glut_709.vert"
         "glut_709.frag"
         "glut_710.vert"
         "glut_710.frag"
         "glut_710_channel0.jpg"
         "glut_710_channel0.png"
         "glut_711.vert"
         "glut_711.frag"
         "glut_712.vert"
         "glut_712.frag"
         "glut_713.vert"
         "glut_713.frag"
         "glut_714.vert"
         "glut_714.frag"
         "glut_714.frag_2"
         "glut_714.frag_3"
         "glut_714.frag_4"
         "glut_715.vert"
         "glut_715.frag"
         "glut_715_channel0.png"
         "glut_715_channel1.png"
         "glut_716.vert"
         "glut_716.frag"
         "glut_717.vert"
         "glut_717.frag"
         "glut_717.frag_2"
         "glut_717.frag_3"
         "glut_717.frag_4"
         "glut_717_S2channel2.png"
         "glut_718.vert"
         "glut_718.frag"
         "glut_718.frag_2"
         "glut_718.frag_3"
         "glut_718.frag_4"
         "glut_718.frag_5"
         "glut_719.vert"
         "glut_719.frag"
         "glut_720.vert"
         "glut_720.frag"
         "glut_721.vert"
         "glut_721.frag"
         "glut_721_channel0.png"
         "glut_722.vert"
         "glut_722.frag"
         "glut_722.frag_2"
         "glut_722_S1channel1.png"
         "glut_722_S1channel2.png"
         "glut_722_S1channel3.png"
         "glut_723.vert"
         "glut_723.frag"
         "glut_724.vert"
         "glut_724.frag"
         "glut_725.vert"
         "glut_725.frag"
         "glut_725_channel0.png"
         "glut_726.vert"
         "glut_726.frag"
         "glut_727.vert"
         "glut_727.frag"
         "glut_727.frag_2"
         "glut_727.frag_3"
         "glut_727.frag_4"
         "glut_728.vert"
         "glut_728.frag"
         "glut_728.frag_2"
         "glut_729.vert"
         "glut_729.frag"
         "glut_730.vert"
         "glut_730.frag"
         "glut_730.frag_2"
         "glut_730.frag_3"
         "glut_730.frag_4"
         "glut_731.vert"
         "glut_731.frag"
         "glut_732.vert"
         "glut_732.frag"
         "glut_733.vert"
         "glut_733.frag"
         "glut_733.frag_2"
         "glut_733.frag_3"
         "glut_734.vert"
         "glut_734.frag"
         "glut_734.frag_2"
         "glut_735.vert"
         "glut_735.frag"
         "glut_735.frag_2"
         "glut_735_S2channel1.png"
         "glut_736.vert"
         "glut_736.frag"
         "glut_736.frag_2"
         "glut_736_S1channel0.png"
         "glut_737.vert"
         "glut_737.frag"
         "glut_737_channel0.png"
         "glut_738.vert"
         "glut_738.frag"
         "glut_739.vert"
         "glut_739.frag"
         "glut_740.vert"
         "glut_740.frag"
         "glut_741.vert"
         "glut_741.frag"
         "glut_742.vert"
         "glut_742.frag"
         "glut_743.vert"
         "glut_743.frag"
         "glut_743.frag_2"
         "glut_743.frag_3"
         "glut_743.frag_4"
         "glut_743.frag_5"
         "glut_744.vert"
         "glut_744.frag"
         "glut_744.frag_2"
         "glut_744.frag_3"
         "glut_745.vert"
         "glut_745.frag"
         "glut_745.frag_2"
         "glut_745.frag_3"
         "glut_745.frag_4"
         "glut_746.vert"
         "glut_746.frag"
         "glut_746.frag_2"
         "glut_746_S1channel0.png"
         "glut_747.vert"
         "glut_747.frag"
         "glut_747_channel0.png"
         "glut_748.vert"
         "glut_748.frag"
         "glut_748.frag_2"
         "glut_748.frag_3"
         "glut_748.frag_4"
         "glut_748.frag_5"
         "glut_749.vert"
         "glut_749.frag"
         "glut_750.vert"
         "glut_750.frag"
         "glut_750.frag_2"
         "glut_750.frag_3"
         "glut_750.frag_4"
         "glut_751.vert"
         "glut_751.frag"
         "glut_751.frag_2"
         "glut_752.vert"
         "glut_752.frag"
         "glut_753.vert"
         "glut_753.frag"
         "glut_754.vert"
         "glut_754.frag"
         "glut_755.vert"
         "glut_755.frag"
         "glut_756.vert"
         "glut_756.frag"
         "glut_757.vert"
         "glut_757.frag"
         "glut_758.vert"
         "glut_758.frag"
         "glut_758.frag_2"
         "glut_759.vert"
         "glut_759.frag"
         "glut_759.frag_2"
         "glut_759_S2channel1.png"
         "glut_760.vert"
         "glut_760.frag"
         "glut_760.frag_2"
         "glut_761.vert"
         "glut_761.frag"
         "glut_762.vert"
         "glut_762.frag"
         "glut_763.vert"
         "glut_763.frag"
         "glut_764.vert"
         "glut_764.frag"
         "glut_764.frag_2"
         "glut_764.frag_3"
         "glut_764.frag_4"
         "glut_765.vert"
         "glut_765.frag"
         "glut_765.frag_2"
         "glut_765.frag_3"
         "glut_765.frag_4"
         "glut_766.vert"
         "glut_766.frag"
         "glut_767.vert"
         "glut_767.frag"
         "glut_767.frag_2"
         "glut_767.frag_3"
         "glut_768.vert"
         "glut_768.frag"
         "glut_768.frag_2"
         "glut_768.frag_3"
         "glut_768.frag_4"
         "glut_769.vert"
         "glut_769.frag"
         "glut_770.vert"
         "glut_770.frag"
         "glut_771.vert"
         "glut_771.frag"
         "glut_772.vert"
         "glut_772.frag"
         "glut_773.vert"
         "glut_773.frag"
         "glut_773_channel0.png"
         "glut_774.vert"
         "glut_774.frag"
         "glut_774.frag_2"
         "glut_775.vert"
         "glut_775.frag"
         "glut_775_channel0.png"
         "glut_776.vert"
         "glut_776.frag"
         "glut_777.vert"
         "glut_777.frag"
         "glut_778.vert"
         "glut_778.frag"
         "glut_779.vert"
         "glut_779.frag"
         "glut_780.vert"
         "glut_780.frag"
         "glut_780_channel0.png"
         "glut_781.vert"
         "glut_781.frag"
         "glut_781.frag_2"
         "glut_781.frag_3"
         "glut_781.frag_4"
         "glut_781.frag_5"
         "glut_782.vert"
         "glut_782.frag"
         "glut_783.vert"
         "glut_783.frag"
         "glut_784.vert"
         "glut_784.frag"
         "glut_785.vert"
         "glut_785.frag"
         "glut_785.frag_2"
         "glut_786.vert"
         "glut_786.frag"
         "glut_787.vert"
         "glut_787.frag"
         "glut_788.vert"
         "glut_788.frag"
         "glut_788.frag_2"
         "glut_789.vert"
         "glut_789.frag"
         "glut_790.vert"
         "glut_790.frag"
         "glut_790.frag_2"
         "glut_791.vert"
         "glut_791.frag"
         "glut_792.vert"
         "glut_792.frag"
         "glut_793.vert"
         "glut_793.frag"
         "glut_794.vert"
         "glut_794.frag"
         "glut_795.vert"
         "glut_795.frag"
         "glut_795.frag_2"
         "glut_795.frag_3"
         "glut_795.frag_4"
         "glut_795.frag_5"
         "glut_795_channel1_3.png"
         "glut_796.vert"
         "glut_796.frag"
         "glut_797.vert"
         "glut_797.frag"
         "glut_797.frag_2"
         "glut_798.vert"
         "glut_798.frag"
         "glut_799.vert"
         "glut_799.frag"
         "glut_800.vert"
         "glut_800.frag"
         "glut_800_channel0.png"
         "glut_801.vert"
         "glut_801.frag"
         "glut_801.frag_2"
         "glut_801.frag_3"
         "glut_802.vert"
         "glut_802.frag"
         "glut_802.frag_2"
         "glut_803.vert"
         "glut_803.frag"
         "glut_804.vert"
         "glut_804.frag"
         "glut_805.vert"
         "glut_805.frag"
         "glut_806.vert"
         "glut_806.frag"
         "glut_806_channel0.png"
         "glut_807.vert"
         "glut_807.frag"
         "glut_808.vert"
         "glut_808.frag"
         "glut_809.vert"
         "glut_809.frag"
         "glut_810.vert"
         "glut_810.frag"
         "glut_811.vert"
         "glut_811.frag"
         "glut_811.frag_2"
         "glut_812.vert"
         "glut_812.frag"
         "glut_813.vert"
         "glut_813.frag"
         "glut_814.vert"
         "glut_814.frag"
         "glut_815.vert"
         "glut_815.frag"
         "glut_816.vert"
         "glut_816.frag"
         "glut_817.vert"
         "glut_817.frag"
         "glut_817.frag_2"
         "glut_818.vert"
         "glut_818.frag"
         "glut_818.frag_2"
         "pge_2.gtk2"
         "pge_2.gtk3"
         "pge_27.gtk2"
         "pge_27.gtk3"
         "pge_440.gtk3"
         DESTINATION bin
         COMPONENT config)
if (WIN32)
 file (TO_NATIVE_PATH $ENV{LIB_ROOT} LIB_ROOT)
 string (REPLACE "\\" "\\\\" LIB_ROOT ${LIB_ROOT})
 install (FILES
          ${ACE_LIB_DIR}/ACE${LIB_FILE_SUFFIX}.dll
          ${LIB_ROOT}/assimp/cmake/bin/${PATH_SUFFIX}/assimp-vc143-mt${LIB_FILE_SUFFIX}.dll
          ${LIB_ROOT}/freeglut/bin/x64/freeglut.dll
          ${LIB_ROOT}/glew/bin/Release/x64/glew32.dll
          ${LIB_ROOT}/libepoxy/bin/epoxy-0.dll
          ${LIB_ROOT}/libnoise/build/msvc/${PATH_SUFFIX}/libnoise.dll
          ${LIB_ROOT}/zlib/build/${PATH_SUFFIX}/zlib${LIB_FILE_SUFFIX}.dll
          ${LIB_ROOT}/libpng/build/${PATH_SUFFIX}/libpng16${LIB_FILE_SUFFIX}.dll
          ${LIB_ROOT}/vcpkg/installed/x64-windows/bin/iconv-2.dll
          ${LIB_ROOT}/vcpkg/installed/x64-windows/bin/liblzma.dll
          ${LIB_ROOT}/vcpkg/installed/x64-windows/bin/bz2.dll
          ${LIB_ROOT}/ffmpeg/libavcodec/avcodec-61.dll
          ${LIB_ROOT}/ffmpeg/libavformat/avformat-61.dll
          ${LIB_ROOT}/ffmpeg/libswresample/swresample-5.dll
          ${LIB_ROOT}/ffmpeg/libswscale/swscale-8.dll
          ${LIB_ROOT}/ffmpeg/libavutil/avutil-59.dll
          ${LIB_ROOT}/ImageMagick/CORE_RL_bzlib_.dll
          ${LIB_ROOT}/ImageMagick/CORE_RL_freetype_.dll
          ${LIB_ROOT}/ImageMagick/CORE_RL_fribidi_.dll
          ${LIB_ROOT}/ImageMagick/CORE_RL_glib_.dll
          ${LIB_ROOT}/ImageMagick/CORE_RL_harfbuzz_.dll
          ${LIB_ROOT}/ImageMagick/CORE_RL_lcms_.dll
          ${LIB_ROOT}/ImageMagick/CORE_RL_libxml_.dll
          ${LIB_ROOT}/ImageMagick/CORE_RL_lqr_.dll
          ${LIB_ROOT}/ImageMagick/CORE_RL_zlib_.dll
          ${LIB_ROOT}/ImageMagick/CORE_RL_MagickCore_.dll
          ${LIB_ROOT}/ImageMagick/CORE_RL_MagickWand_.dll
          ${LIB_ROOT}/gtk3/bin/libatk-1.0-0.dll
          ${LIB_ROOT}/gtk3/bin/libfreetype-6.dll
          ${LIB_ROOT}/gtk3/bin/libintl-8.dll
          ${LIB_ROOT}/gtk3/bin/libcairo-2.dll
          ${LIB_ROOT}/gtk3/bin/libcairo-gobject-2.dll
          ${LIB_ROOT}/gtk3/bin/libfontconfig-1.dll
          ${LIB_ROOT}/gtk3/bin/libgio-2.0-0.dll
          ${LIB_ROOT}/gtk3/bin/libglib-2.0-0.dll
          ${LIB_ROOT}/gtk3/bin/libgmodule-2.0-0.dll
          ${LIB_ROOT}/gtk3/bin/libgobject-2.0-0.dll
          ${LIB_ROOT}/gtk3/bin/libgthread-2.0-0.dll
          ${LIB_ROOT}/gtk3/bin/libgdk_pixbuf-2.0-0.dll
          ${LIB_ROOT}/gtk3/bin/libgdk-3-0.dll
          ${LIB_ROOT}/gtk3/bin/libgtk-3-0.dll
          ${LIB_ROOT}/gtk3/bin/libpango-1.0-0.dll
          ${LIB_ROOT}/gtk3/bin/libpangocairo-1.0-0.dll
          ${LIB_ROOT}/gtk3/bin/libpangoft2-1.0-0.dll
          ${LIB_ROOT}/gtk3/bin/libpangowin32-1.0-0.dll
          ${LIB_ROOT}/gtk3/bin/libiconv-2.dll
          ${LIB_ROOT}/gtk3/bin/libpng15-15.dll
          ${LIB_ROOT}/gtk3/bin/liblzma-5.dll
          ${LIB_ROOT}/gtk3/bin/zlib1.dll
          ${LIB_ROOT}/gtk3/bin/libxml2-2.dll
          ${LIB_ROOT}/gtk3/bin/libffi-6.dll
          ${LIB_ROOT}/gtk3/bin/libpixman-1-0.dll
          ${LIB_ROOT}/gtk3/bin/libwinpthread-1.dll
          DESTINATION bin
          COMPONENT binaries)
 if (CMAKE_BUILD_TYPE STREQUAL Debug)
  install (FILES
           "C:/Windows/System32/msvcp140d.dll"
           "C:/Windows/System32/vcruntime140d.dll"
           DESTINATION bin
           COMPONENT binaries)
 endif (CMAKE_BUILD_TYPE STREQUAL Debug)
endif (WIN32)

file (TO_CMAKE_PATH "$ENV{LIB_ROOT}/cmake-modules" CMAKE_MODUDULES_PATH)
list (APPEND CMAKE_MODULE_PATH ${CMAKE_MODUDULES_PATH})
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
set (ARGS_9 "-l -m200")
set (ARGS_10 "-l -m220")
set (ARGS_11 "-l -m240")
set (ARGS_12 "-l -m260")
set (ARGS_13 "-l -m280")
set (ARGS_14 "-l -m300")
set (ARGS_15 "-l -m320")
set (ARGS_16 "-l -m340")
set (ARGS_17 "-l -m360")
set (ARGS_18 "-l -m390")
set (ARGS_19 "-l -m410")
set (ARGS_20 "-l -m430")
set (ARGS_21 "-l -m450")
set (ARGS_22 "-l -m470")
set (ARGS_23 "-l -m490")
set (ARGS_24 "-l -m510")
set (ARGS_25 "-l -m530")
set (ARGS_26 "-l -m550")
set (ARGS_27 "-l -m570")
set (ARGS_28 "-l -m590")
set (ARGS_29 "-l -m610")
set (ARGS_30 "-l -m630")
set (ARGS_31 "-l -m650")
set (ARGS_32 "-l -m670")
set (ARGS_33 "-l -m690")
set (ARGS_34 "-l -m710")
set (ARGS_35 "-l -m730")
set (ARGS_36 "-l -m750")
set (ARGS_37 "-l -m770")
set (ARGS_38 "-l -m790")
set (ARGS_39 "-l -m810")
if (CMAKE_BUILD_TYPE STREQUAL Debug)
 set (ARGS "${ARGS} -t")
 set (ARGS_2 "${ARGS_2} -t")
 set (ARGS_3 "${ARGS_3} -t")
 set (ARGS_4 "${ARGS_4} -t")
 set (ARGS_5 "${ARGS_5} -t")
 set (ARGS_6 "${ARGS_6} -t")
 set (ARGS_7 "${ARGS_7} -t")
 set (ARGS_8 "${ARGS_8} -t")
 set (ARGS_9 "${ARGS_9} -t")
 set (ARGS_10 "${ARGS_10} -t")
 set (ARGS_11 "${ARGS_11} -t")
 set (ARGS_12 "${ARGS_12} -t")
 set (ARGS_13 "${ARGS_13} -t")
 set (ARGS_14 "${ARGS_14} -t")
 set (ARGS_15 "${ARGS_15} -t")
 set (ARGS_16 "${ARGS_16} -t")
 set (ARGS_17 "${ARGS_17} -t")
 set (ARGS_18 "${ARGS_18} -t")
 set (ARGS_19 "${ARGS_19} -t")
 set (ARGS_20 "${ARGS_20} -t")
 set (ARGS_21 "${ARGS_21} -t")
 set (ARGS_22 "${ARGS_22} -t")
 set (ARGS_23 "${ARGS_23} -t")
 set (ARGS_24 "${ARGS_24} -t")
 set (ARGS_25 "${ARGS_25} -t")
 set (ARGS_26 "${ARGS_26} -t")
 set (ARGS_27 "${ARGS_27} -t")
 set (ARGS_28 "${ARGS_28} -t")
 set (ARGS_29 "${ARGS_29} -t")
 set (ARGS_30 "${ARGS_30} -t")
 set (ARGS_31 "${ARGS_31} -t")
 set (ARGS_32 "${ARGS_32} -t")
 set (ARGS_33 "${ARGS_33} -t")
 set (ARGS_34 "${ARGS_34} -t")
 set (ARGS_35 "${ARGS_35} -t")
 set (ARGS_36 "${ARGS_36} -t")
 set (ARGS_37 "${ARGS_37} -t")
 set (ARGS_38 "${ARGS_38} -t")
 set (ARGS_39 "${ARGS_39} -t")
endif (CMAKE_BUILD_TYPE STREQUAL Debug)
file (TO_CMAKE_PATH ${LIBNOISE_LIB_DIR} LIBNOISE_PATH)
file (TO_CMAKE_PATH "${GLUT_LIB_DIR}" GLUT_PATH)
file (TO_CMAKE_PATH "${GLEW_LIB_DIR}" GLEW_PATH)
file (TO_CMAKE_PATH ${ACE_LIB_DIR} ACE_PATH)
file (TO_CMAKE_PATH "${ASSIMP_LIB_DIR}" ASSIMP_PATH)
file (TO_CMAKE_PATH "${PNG_LIB_DIR}" PNG_PATH)
if (WIN32)
 # *NOTE*: ffmpeg was built with "vcpkg-libpng" support, so now there is a new dependency to zlib1.dll, liblzma.dll, iconv-2.dll :-(
 file (TO_CMAKE_PATH "$ENV{VCPKG_INSTALLATION_ROOT}/installed/x64-windows/bin" VCPKG_PATH)
endif (WIN32)
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
                          ${VCPKG_PATH}
                          ${FFMPEG_PATH}
                          ${IMAGEMAGICK_PATH}
                          ${GTK_PATH})
if (UNIX)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE}" COMMAND)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_2}" COMMAND_2)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_3}" COMMAND_3)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_4}" COMMAND_4)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_5}" COMMAND_5)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_6}" COMMAND_6)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_7}" COMMAND_7)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_8}" COMMAND_8)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_9}" COMMAND_9)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_10}" COMMAND_10)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_11}" COMMAND_11)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_12}" COMMAND_12)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_13}" COMMAND_13)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_14}" COMMAND_14)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_15}" COMMAND_15)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_16}" COMMAND_16)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_17}" COMMAND_17)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_18}" COMMAND_18)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_19}" COMMAND_19)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_20}" COMMAND_20)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_21}" COMMAND_21)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_22}" COMMAND_22)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_23}" COMMAND_23)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_24}" COMMAND_24)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_25}" COMMAND_25)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_26}" COMMAND_26)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_27}" COMMAND_27)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_28}" COMMAND_28)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_29}" COMMAND_29)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_30}" COMMAND_30)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_31}" COMMAND_31)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_32}" COMMAND_32)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_33}" COMMAND_33)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_34}" COMMAND_34)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_35}" COMMAND_35)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_36}" COMMAND_36)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_37}" COMMAND_37)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_38}" COMMAND_38)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_39}" COMMAND_39)
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
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_9}.exe" COMMAND_9)
 string (REPLACE "\\" "\\\\" COMMAND_9 ${COMMAND_9})
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_10}.exe" COMMAND_10)
 string (REPLACE "\\" "\\\\" COMMAND_10 ${COMMAND_10})
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_11}.exe" COMMAND_11)
 string (REPLACE "\\" "\\\\" COMMAND_11 ${COMMAND_11})
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_12}.exe" COMMAND_12)
 string (REPLACE "\\" "\\\\" COMMAND_12 ${COMMAND_12})
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_13}.exe" COMMAND_13)
 string (REPLACE "\\" "\\\\" COMMAND_13 ${COMMAND_13})
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_14}.exe" COMMAND_14)
 string (REPLACE "\\" "\\\\" COMMAND_14 ${COMMAND_14})
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_15}.exe" COMMAND_15)
 string (REPLACE "\\" "\\\\" COMMAND_15 ${COMMAND_15})
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_16}.exe" COMMAND_16)
 string (REPLACE "\\" "\\\\" COMMAND_16 ${COMMAND_16})
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_17}.exe" COMMAND_17)
 string (REPLACE "\\" "\\\\" COMMAND_17 ${COMMAND_17})
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_18}.exe" COMMAND_18)
 string (REPLACE "\\" "\\\\" COMMAND_18 ${COMMAND_18})
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_19}.exe" COMMAND_19)
 string (REPLACE "\\" "\\\\" COMMAND_19 ${COMMAND_19})
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_20}.exe" COMMAND_20)
 string (REPLACE "\\" "\\\\" COMMAND_20 ${COMMAND_20})
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_21}.exe" COMMAND_21)
 string (REPLACE "\\" "\\\\" COMMAND_21 ${COMMAND_21})
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_22}.exe" COMMAND_22)
 string (REPLACE "\\" "\\\\" COMMAND_22 ${COMMAND_22})
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_23}.exe" COMMAND_23)
 string (REPLACE "\\" "\\\\" COMMAND_23 ${COMMAND_23})
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_24}.exe" COMMAND_24)
 string (REPLACE "\\" "\\\\" COMMAND_24 ${COMMAND_24})
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_25}.exe" COMMAND_25)
 string (REPLACE "\\" "\\\\" COMMAND_25 ${COMMAND_25})
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_26}.exe" COMMAND_26)
 string (REPLACE "\\" "\\\\" COMMAND_26 ${COMMAND_26})
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_27}.exe" COMMAND_27)
 string (REPLACE "\\" "\\\\" COMMAND_27 ${COMMAND_27})
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_28}.exe" COMMAND_28)
 string (REPLACE "\\" "\\\\" COMMAND_28 ${COMMAND_28})
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_29}.exe" COMMAND_29)
 string (REPLACE "\\" "\\\\" COMMAND_29 ${COMMAND_29})
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_30}.exe" COMMAND_30)
 string (REPLACE "\\" "\\\\" COMMAND_30 ${COMMAND_30})
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_31}.exe" COMMAND_31)
 string (REPLACE "\\" "\\\\" COMMAND_31 ${COMMAND_31})
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_32}.exe" COMMAND_32)
 string (REPLACE "\\" "\\\\" COMMAND_32 ${COMMAND_32})
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_33}.exe" COMMAND_33)
 string (REPLACE "\\" "\\\\" COMMAND_33 ${COMMAND_33})
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_34}.exe" COMMAND_34)
 string (REPLACE "\\" "\\\\" COMMAND_34 ${COMMAND_34})
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_35}.exe" COMMAND_35)
 string (REPLACE "\\" "\\\\" COMMAND_35 ${COMMAND_35})
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_36}.exe" COMMAND_36)
 string (REPLACE "\\" "\\\\" COMMAND_36 ${COMMAND_36})
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_37}.exe" COMMAND_37)
 string (REPLACE "\\" "\\\\" COMMAND_37 ${COMMAND_37})
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_38}.exe" COMMAND_38)
 string (REPLACE "\\" "\\\\" COMMAND_38 ${COMMAND_38})
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${EXECUTABLE_39}.exe" COMMAND_39)
 string (REPLACE "\\" "\\\\" COMMAND_39 ${COMMAND_39})
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
create_target_launcher (${EXECUTABLE_9}
                        COMMAND ${COMMAND_9}
                        ARGS ${ARGS_9}
#                        FORWARD_ARGS
                        RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        ENVIRONMENT ${ENVIRONMENT_MAGICK})
create_target_launcher (${EXECUTABLE_10}
                        COMMAND ${COMMAND_10}
                        ARGS ${ARGS_10}
#                        FORWARD_ARGS
                        RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        ENVIRONMENT ${ENVIRONMENT_MAGICK})
create_target_launcher (${EXECUTABLE_11}
                        COMMAND ${COMMAND_11}
                        ARGS ${ARGS_11}
#                        FORWARD_ARGS
                        RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        ENVIRONMENT ${ENVIRONMENT_MAGICK})
create_target_launcher (${EXECUTABLE_12}
                        COMMAND ${COMMAND_12}
                        ARGS ${ARGS_12}
#                        FORWARD_ARGS
                        RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        ENVIRONMENT ${ENVIRONMENT_MAGICK})
create_target_launcher (${EXECUTABLE_13}
                        COMMAND ${COMMAND_13}
                        ARGS ${ARGS_13}
#                        FORWARD_ARGS
                        RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        ENVIRONMENT ${ENVIRONMENT_MAGICK})
create_target_launcher (${EXECUTABLE_14}
                        COMMAND ${COMMAND_14}
                        ARGS ${ARGS_14}
#                        FORWARD_ARGS
                        RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        ENVIRONMENT ${ENVIRONMENT_MAGICK})
create_target_launcher (${EXECUTABLE_15}
                        COMMAND ${COMMAND_15}
                        ARGS ${ARGS_15}
#                        FORWARD_ARGS
                        RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        ENVIRONMENT ${ENVIRONMENT_MAGICK})
create_target_launcher (${EXECUTABLE_16}
                        COMMAND ${COMMAND_16}
                        ARGS ${ARGS_16}
#                        FORWARD_ARGS
                        RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        ENVIRONMENT ${ENVIRONMENT_MAGICK})
create_target_launcher (${EXECUTABLE_17}
                        COMMAND ${COMMAND_17}
                        ARGS ${ARGS_17}
#                        FORWARD_ARGS
                        RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        ENVIRONMENT ${ENVIRONMENT_MAGICK})
create_target_launcher (${EXECUTABLE_18}
                        COMMAND ${COMMAND_18}
                        ARGS ${ARGS_18}
#                        FORWARD_ARGS
                        RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        ENVIRONMENT ${ENVIRONMENT_MAGICK})
create_target_launcher (${EXECUTABLE_19}
                        COMMAND ${COMMAND_19}
                        ARGS ${ARGS_19}
#                        FORWARD_ARGS
                        RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        ENVIRONMENT ${ENVIRONMENT_MAGICK})
create_target_launcher (${EXECUTABLE_20}
                        COMMAND ${COMMAND_20}
                        ARGS ${ARGS_20}
#                        FORWARD_ARGS
                        RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        ENVIRONMENT ${ENVIRONMENT_MAGICK})
create_target_launcher (${EXECUTABLE_21}
                        COMMAND ${COMMAND_21}
                        ARGS ${ARGS_21}
#                        FORWARD_ARGS
                        RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        ENVIRONMENT ${ENVIRONMENT_MAGICK})
create_target_launcher (${EXECUTABLE_22}
                        COMMAND ${COMMAND_22}
                        ARGS ${ARGS_22}
#                        FORWARD_ARGS
                        RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        ENVIRONMENT ${ENVIRONMENT_MAGICK})
create_target_launcher (${EXECUTABLE_23}
                        COMMAND ${COMMAND_23}
                        ARGS ${ARGS_23}
#                        FORWARD_ARGS
                        RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        ENVIRONMENT ${ENVIRONMENT_MAGICK})
create_target_launcher (${EXECUTABLE_24}
                        COMMAND ${COMMAND_24}
                        ARGS ${ARGS_24}
#                        FORWARD_ARGS
                        RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        ENVIRONMENT ${ENVIRONMENT_MAGICK})
create_target_launcher (${EXECUTABLE_25}
                        COMMAND ${COMMAND_25}
                        ARGS ${ARGS_25}
#                        FORWARD_ARGS
                        RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        ENVIRONMENT ${ENVIRONMENT_MAGICK})
create_target_launcher (${EXECUTABLE_26}
                        COMMAND ${COMMAND_26}
                        ARGS ${ARGS_26}
#                        FORWARD_ARGS
                        RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        ENVIRONMENT ${ENVIRONMENT_MAGICK})
create_target_launcher (${EXECUTABLE_27}
                        COMMAND ${COMMAND_27}
                        ARGS ${ARGS_27}
#                        FORWARD_ARGS
                        RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        ENVIRONMENT ${ENVIRONMENT_MAGICK})
create_target_launcher (${EXECUTABLE_28}
                        COMMAND ${COMMAND_28}
                        ARGS ${ARGS_28}
#                        FORWARD_ARGS
                        RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        ENVIRONMENT ${ENVIRONMENT_MAGICK})
create_target_launcher (${EXECUTABLE_29}
                        COMMAND ${COMMAND_29}
                        ARGS ${ARGS_29}
#                        FORWARD_ARGS
                        RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        ENVIRONMENT ${ENVIRONMENT_MAGICK})
create_target_launcher (${EXECUTABLE_30}
                        COMMAND ${COMMAND_30}
                        ARGS ${ARGS_30}
#                        FORWARD_ARGS
                        RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        ENVIRONMENT ${ENVIRONMENT_MAGICK})
create_target_launcher (${EXECUTABLE_31}
                        COMMAND ${COMMAND_31}
                        ARGS ${ARGS_31}
#                        FORWARD_ARGS
                        RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        ENVIRONMENT ${ENVIRONMENT_MAGICK})
create_target_launcher (${EXECUTABLE_32}
                        COMMAND ${COMMAND_32}
                        ARGS ${ARGS_32}
#                        FORWARD_ARGS
                        RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        ENVIRONMENT ${ENVIRONMENT_MAGICK})
create_target_launcher (${EXECUTABLE_33}
                        COMMAND ${COMMAND_33}
                        ARGS ${ARGS_33}
#                        FORWARD_ARGS
                        RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        ENVIRONMENT ${ENVIRONMENT_MAGICK})
create_target_launcher (${EXECUTABLE_34}
                        COMMAND ${COMMAND_34}
                        ARGS ${ARGS_34}
#                        FORWARD_ARGS
                        RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        ENVIRONMENT ${ENVIRONMENT_MAGICK})
create_target_launcher (${EXECUTABLE_35}
                        COMMAND ${COMMAND_35}
                        ARGS ${ARGS_35}
#                        FORWARD_ARGS
                        RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        ENVIRONMENT ${ENVIRONMENT_MAGICK})
create_target_launcher (${EXECUTABLE_36}
                        COMMAND ${COMMAND_36}
                        ARGS ${ARGS_36}
#                        FORWARD_ARGS
                        RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        ENVIRONMENT ${ENVIRONMENT_MAGICK})
create_target_launcher (${EXECUTABLE_37}
                        COMMAND ${COMMAND_37}
                        ARGS ${ARGS_37}
#                        FORWARD_ARGS
                        RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        ENVIRONMENT ${ENVIRONMENT_MAGICK})
create_target_launcher (${EXECUTABLE_38}
                        COMMAND ${COMMAND_38}
                        ARGS ${ARGS_38}
#                        FORWARD_ARGS
                        RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        ENVIRONMENT ${ENVIRONMENT_MAGICK})
create_target_launcher (${EXECUTABLE_39}
                        COMMAND ${COMMAND_39}
                        ARGS ${ARGS_39}
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
#  InstallDir 'C:\\\\engine'
#")

include (CPack)
