/***************************************************************************
*   Copyright (C) 2009 by Erik Sohns   *
*   erik.sohns@web.de   *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/

#ifndef DEFINES_H
#define DEFINES_H

#define MAKE_VERSION_STRING_VARIABLE(program,version,variable) std::string variable; do {                                \
  variable = program; variable += ACE_TEXT_ALWAYS_CHAR (" ");                                                            \
  variable += version; variable += ACE_TEXT_ALWAYS_CHAR (" compiled on ");                                               \
  variable += ACE_TEXT_ALWAYS_CHAR (COMPILATION_DATE_TIME);                                                              \
  variable += ACE_TEXT_ALWAYS_CHAR (" host platform "); variable += Common_Tools::compilerPlatformName ();               \
  variable += ACE_TEXT_ALWAYS_CHAR (" with "); variable += Common_Tools::compilerName ();                                \
  variable += ACE_TEXT_ALWAYS_CHAR (" "); variable += Common_Tools::compilerVersion ();                                  \
  variable += ACE_TEXT_ALWAYS_CHAR (" against ACE "); variable += Common_Tools::compiledVersion_ACE ();                  \
  variable += ACE_TEXT_ALWAYS_CHAR (" , Common "); variable += ACE_TEXT_ALWAYS_CHAR (Common_PACKAGE_VERSION_FULL);       \
} while (0)

// pge 2
//#define IX(i, j) i + j * resolution_
//#define SWAP(x0, x) {float* tmp=x0;x0=x;x=tmp;}
#define ENGINE_PGE_2_DEFAULT_SIZE               320

#define ENGINE_PGE_2_DEFAULT_NUM_ITERATIONS    16
#define ENGINE_PGE_2_DEFAULT_DT                1.0f
#define ENGINE_PGE_2_DEFAULT_DIFFUSION         0.00000001f
#define ENGINE_PGE_2_DEFAULT_VISCOSITY         0.00000001f

#if defined (GTK2_USE)
#define ENGINE_PGE_2_UI_DEFINITION_FILE        "pge_2.gtk2"
#else
#define ENGINE_PGE_2_UI_DEFINITION_FILE        "pge_2.gtk3"
#endif // GTK2_USE

#define ENGINE_UI_GTK_SCALE_ITERATIONS_NAME     "scale_iterations"
#define ENGINE_UI_GTK_SCALE_DT_NAME             "scale_dt"
#define ENGINE_UI_GTK_SCALE_DIFFUSION_NAME      "scale_diffusion"
#define ENGINE_UI_GTK_SCALE_VISCOSITY_NAME      "scale_viscosity"

// glut 3
#define ENGINE_GLUT_3_DEFAULT_NOISE_X           1.25 // coordinate
#define ENGINE_GLUT_3_DEFAULT_NOISE_Y           0.75 // coordinate
#define ENGINE_GLUT_3_DEFAULT_NOISE_Z           0.5  // coordinate
#define ENGINE_GLUT_3_DEFAULT_NOISE_STEP        0.05
#define ENGINE_GLUT_3_DEFAULT_NOISE_FREQUENCY   noise::module::DEFAULT_PERLIN_FREQUENCY // frequency of first octave
#define ENGINE_GLUT_3_DEFAULT_NOISE_OCTAVES     noise::module::DEFAULT_PERLIN_OCTAVE_COUNT // detail: 1-30
#define ENGINE_GLUT_3_DEFAULT_NOISE_PERSISTENCE noise::module::DEFAULT_PERLIN_PERSISTENCE // roughness: 0.0-1.0
#define ENGINE_GLUT_3_DEFAULT_NOISE_LACUNARITY  noise::module::DEFAULT_PERLIN_LACUNARITY // frequency multiplier between successive octaves: 1.5-3.5
#define ENGINE_GLUT_3_DEFAULT_NOISE_LEVEL       50.0

#if defined (GTK2_USE)
#define ENGINE_GLUT_3_UI_DEFINITION_FILE        "glut_3.gtk2"
#else
#define ENGINE_GLUT_3_UI_DEFINITION_FILE        "glut_3.gtk3"
#endif // GTK2_USE

#define ENGINE_UI_GTK_DIALOG_MAIN_NAME          "dialog_main"
#define ENGINE_UI_GTK_SCALE_FREQUENCY_NAME      "scale_frequency"
#define ENGINE_UI_GTK_SCALE_OCTAVES_NAME        "scale_octaves"
#define ENGINE_UI_GTK_SCALE_PERSISTENCE_NAME    "scale_persistence"
#define ENGINE_UI_GTK_SCALE_LACUNARITY_NAME     "scale_lacunarity"
#define ENGINE_UI_GTK_SCALE_STEP_NAME           "scale_step"
#define ENGINE_UI_GTK_SCALE_LEVEL_NAME          "scale_level"
#define ENGINE_UI_GTK_SCALE_SPEED_NAME          "scale_speed"

// pge 5
#define ENGINE_PGE_5_DEFAULT_HEIGHT             240
#define ENGINE_PGE_5_DEFAULT_WIDTH              320

// pge 6
#define ENGINE_PGE_6_DEFAULT_HEIGHT             400
#define ENGINE_PGE_6_DEFAULT_WIDTH              600
#define ENGINE_PGE_6_DEFAULT_NUMBER_OF_FIRES    75
#define ENGINE_PGE_6_2_DEFAULT_HEIGHT           240
#define ENGINE_PGE_6_2_DEFAULT_WIDTH            320
#define ENGINE_PGE_6_2_DEFAULT_NUMBER_OF_FIRE_LINES 3 // #bottom lines

// pge 7
#define ENGINE_PGE_7_DEFAULT_HEIGHT             240
#define ENGINE_PGE_7_DEFAULT_WIDTH              320

// pge 8
#define ENGINE_PGE_8_DEFAULT_HEIGHT             240
#define ENGINE_PGE_8_DEFAULT_WIDTH              320

// pge 9
#define ENGINE_PGE_9_DEFAULT_HEIGHT             500
#define ENGINE_PGE_9_DEFAULT_WIDTH              800

// pge 10
#define ENGINE_PGE_10_DEFAULT_HEIGHT            480
#define ENGINE_PGE_10_DEFAULT_WIDTH             640

// pge 11
#define ENGINE_PGE_11_DEFAULT_HEIGHT            300
#define ENGINE_PGE_11_DEFAULT_WIDTH             400

// pge 12
#define ENGINE_PGE_12_DEFAULT_HEIGHT            480
#define ENGINE_PGE_12_DEFAULT_WIDTH             640

// pge 13
#define ENGINE_PGE_13_DEFAULT_HEIGHT            300
#define ENGINE_PGE_13_DEFAULT_WIDTH             600

// pge 14
#define ENGINE_PGE_14_DEFAULT_HEIGHT            512
#define ENGINE_PGE_14_DEFAULT_WIDTH             512

// pge 15
#define ENGINE_PGE_15_DEFAULT_HEIGHT            600
#define ENGINE_PGE_15_DEFAULT_WIDTH             800
#define ENGINE_PGE_15_DEFAULT_SCALE             10
#define ENGINE_PGE_15_DEFAULT_PARTICLES         700

// glut 16
#define ENGINE_GLUT_16_DEFAULT_M                7.0F
#define ENGINE_GLUT_16_DEFAULT_N1               0.2F
#define ENGINE_GLUT_16_DEFAULT_N2               1.7F
#define ENGINE_GLUT_16_DEFAULT_N3               1.7F

#if defined (GTK2_USE)
#define ENGINE_GLUT_16_UI_DEFINITION_FILE       "glut_16.gtk2"
#else
#define ENGINE_GLUT_16_UI_DEFINITION_FILE       "glut_16.gtk3"
#endif // GTK2_USE

#define ENGINE_UI_GTK_SCALE_M_NAME              "scale_m"
#define ENGINE_UI_GTK_SCALE_N1_NAME             "scale_n1"
#define ENGINE_UI_GTK_SCALE_N2_NAME             "scale_n2"
#define ENGINE_UI_GTK_SCALE_N3_NAME             "scale_n3"

// pge 17
#define ENGINE_PGE_17_DEFAULT_HEIGHT            480
#define ENGINE_PGE_17_DEFAULT_WIDTH             640
#define ENGINE_PGE_17_DEFAULT_BOIDS             100

// glut 18
#define ENGINE_GLUT_18_DEFAULT_HEIGHT           480
#define ENGINE_GLUT_18_DEFAULT_WIDTH            640

// pge 19
#define ENGINE_PGE_19_DEFAULT_HEIGHT            480
#define ENGINE_PGE_19_DEFAULT_WIDTH             640
#define ENGINE_PGE_19_DEFAULT_SEGMENT_LENGTH    20
#define ENGINE_PGE_19_DEFAULT_SEGMENTS          5

// pge 20
#define ENGINE_PGE_20_DEFAULT_HEIGHT            480
#define ENGINE_PGE_20_DEFAULT_WIDTH             640

// pge 21
#define ENGINE_PGE_21_DEFAULT_HEIGHT            800
#define ENGINE_PGE_21_DEFAULT_WIDTH             800

// pge 22
#define ENGINE_PGE_22_DEFAULT_HEIGHT            400
#define ENGINE_PGE_22_DEFAULT_WIDTH             900
#define ENGINE_PGE_22_DEFAULT_SPOTS_FILE        "2017.png"

// pge 23
#define ENGINE_PGE_23_DEFAULT_HEIGHT            200
#define ENGINE_PGE_23_DEFAULT_WIDTH             200

// pge 24
#define ENGINE_PGE_24_DEFAULT_HEIGHT            400
#define ENGINE_PGE_24_DEFAULT_WIDTH             400
#define ENGINE_PGE_24_DEFAULT_CELL_SIZE         10

// glut 25
#define ENGINE_GLUT_25_DEFAULT_HEIGHT           600
#define ENGINE_GLUT_25_DEFAULT_WIDTH            800
#define ENGINE_GLUT_25_DEFAULT_DIMENSION        128

// pge 26
#define ENGINE_PGE_26_DEFAULT_HEIGHT            600
#define ENGINE_PGE_26_DEFAULT_WIDTH             600

// pge 27
#define ENGINE_PGE_27_DEFAULT_HEIGHT            600
#define ENGINE_PGE_27_DEFAULT_WIDTH             600
#define ENGINE_PGE_27_DEFAULT_DELTA             0
#define ENGINE_PGE_27_DEFAULT_ANGLE             75 // degrees

#if defined (GTK2_USE)
#define ENGINE_PGE_27_UI_DEFINITION_FILE        "pge_27.gtk2"
#else
#define ENGINE_PGE_27_UI_DEFINITION_FILE        "pge_27.gtk3"
#endif // GTK2_USE

#define ENGINE_UI_GTK_SCALE_DELTA_NAME          "scale_delta"
#define ENGINE_UI_GTK_SCALE_ANGLE_NAME          "scale_angle"

// pge 28
#define ENGINE_PGE_28_DEFAULT_HEIGHT            600
#define ENGINE_PGE_28_DEFAULT_WIDTH             800
#define ENGINE_PGE_28_DEFAULT_VEL_LIMIT         5.0f
#define ENGINE_PGE_28_DEFAULT_DIST_LIMIT_MIN    1.0f
#define ENGINE_PGE_28_DEFAULT_DIST_LIMIT_MAX    25.0f
#define ENGINE_PGE_28_DEFAULT_G                 50.0f
#define ENGINE_PGE_28_DEFAULT_DIST_LIMIT        20.0f
#define ENGINE_PGE_28_DEFAULT_REPEL_FORCE_MAG   -10.0f

// pge 29
#define ENGINE_PGE_29_DEFAULT_HEIGHT            600
#define ENGINE_PGE_29_DEFAULT_WIDTH             800

// pge 30
#define ENGINE_PGE_30_DEFAULT_HEIGHT            600
#define ENGINE_PGE_30_DEFAULT_WIDTH             600
#define ENGINE_PGE_30_DEFAULT_MAX_ITERATIONS    100

#endif
