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

#endif
