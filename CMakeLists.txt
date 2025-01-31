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
     stdafx.h)

set (EXECUTABLE_21 engine_21)
add_executable (${EXECUTABLE_21}
                ${engine_21_EXE_SRCS})

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

install (TARGETS ${EXECUTABLE} ${EXECUTABLE_2} ${EXECUTABLE_3} ${EXECUTABLE_4} ${EXECUTABLE_5} ${EXECUTABLE_6}
                 ${EXECUTABLE_7} ${EXECUTABLE_8} ${EXECUTABLE_9} ${EXECUTABLE_10} ${EXECUTABLE_11} ${EXECUTABLE_12}
                 ${EXECUTABLE_13} ${EXECUTABLE_14} ${EXECUTABLE_15} ${EXECUTABLE_16} ${EXECUTABLE_17} ${EXECUTABLE_18}
                 ${EXECUTABLE_19} ${EXECUTABLE_20} ${EXECUTABLE_21}
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
          ${LIB_ROOT}/glew/bin/${PATH_SUFFIX}/x64/glew32.dll
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
