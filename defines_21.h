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

#ifndef DEFINES_21_H
#define DEFINES_21_H

// glut 450
#define ENGINE_GLUT_450_DEFAULT_HEIGHT             400
#define ENGINE_GLUT_450_DEFAULT_WIDTH              400
#define ENGINE_GLUT_450_DEFAULT_H                  200

// glut 451
#define ENGINE_GLUT_451_DEFAULT_HEIGHT             834
#define ENGINE_GLUT_451_DEFAULT_WIDTH              1112
#define ENGINE_GLUT_451_DEFAULT_SCALE_FACTOR       30

// pge 452
#define ENGINE_PGE_452_DEFAULT_HEIGHT              1024
#define ENGINE_PGE_452_DEFAULT_WIDTH               1024
#define ENGINE_PGE_452_DEFAULT_OBJECTS_PER_COL_ROW 15
#define ENGINE_PGE_452_DEFAULT_ALPHA_DECAY         15

// glut 453
#define ENGINE_GLUT_453_DEFAULT_HEIGHT             834
#define ENGINE_GLUT_453_DEFAULT_WIDTH              1112
#define ENGINE_GLUT_453_DEFAULT_SCALE_FACTOR       30

// glut 454
#define ENGINE_GLUT_454_DEFAULT_HEIGHT             834
#define ENGINE_GLUT_454_DEFAULT_WIDTH              1112
#define ENGINE_GLUT_454_DEFAULT_NUMBER_OF_AGENTS   150
#define ENGINE_GLUT_454_DEFAULT_CUBE_SIZE          400
#define ENGINE_GLUT_454_DEFAULT_NUMBER_OF_SEGMENTS 20
#define ENGINE_GLUT_454_DEFAULT_MIN_WIDTH          2
#define ENGINE_GLUT_454_DEFAULT_MAX_WIDTH          6
#define ENGINE_GLUT_454_DEFAULT_MIN_SPEED          5
#define ENGINE_GLUT_454_DEFAULT_MAX_SPEED          20
#define ENGINE_GLUT_454_DEFAULT_W_CHANCE           0.01f

// pge 455
#define ENGINE_PGE_455_DEFAULT_HEIGHT              640
#define ENGINE_PGE_455_DEFAULT_WIDTH               640
#define ENGINE_PGE_455_DEFAULT_W                   640
#define ENGINE_PGE_455_DEFAULT_BRUSH_RADIUS        20

// glut 456
#define ENGINE_GLUT_456_DEFAULT_HEIGHT             834
#define ENGINE_GLUT_456_DEFAULT_WIDTH              1112
#define ENGINE_GLUT_456_DEFAULT_SCALE_FACTOR       30

// glut 457
#define ENGINE_GLUT_457_DEFAULT_HEIGHT             834
#define ENGINE_GLUT_457_DEFAULT_WIDTH              1112
#define ENGINE_GLUT_457_DEFAULT_SCALE_FACTOR       30

// glut 458
#define ENGINE_GLUT_458_DEFAULT_HEIGHT             834
#define ENGINE_GLUT_458_DEFAULT_WIDTH              1112
#define ENGINE_GLUT_458_DEFAULT_SCALE_FACTOR       30

// glut 459
#define ENGINE_GLUT_459_DEFAULT_HEIGHT             834
#define ENGINE_GLUT_459_DEFAULT_WIDTH              1112
#define ENGINE_GLUT_459_DEFAULT_SCALE_FACTOR       30

#define ENGINE_GLUT_459_DEFAULT_NOISE_SCALE        1.0f
#define ENGINE_GLUT_459_DEFAULT_NOISE_OCTAVES      4
#define ENGINE_GLUT_459_DEFAULT_NOISE_FACTOR       10.0f
#define ENGINE_GLUT_459_DEFAULT_FREQUENCY          10.0f
#define ENGINE_GLUT_459_DEFAULT_SHARPNESS          -2.0f
#define ENGINE_GLUT_459_DEFAULT_PATTERN            0

#if defined (GTK2_USE)
#define ENGINE_GLUT_459_UI_GTK_DEFINITION_FILE          "glut_459.gtk2"
#else
#define ENGINE_GLUT_459_UI_GTK_DEFINITION_FILE          "glut_459.gtk3"
#endif // GTK2_USE

#define ENGINE_GLUT_459_UI_GTK_SCALE_NOISE_SCALE_NAME   "scale_noise_scale"
#define ENGINE_GLUT_459_UI_GTK_SCALE_NOISE_OCTAVES_NAME "scale_noise_octaves"
#define ENGINE_GLUT_459_UI_GTK_SCALE_NOISE_FACTOR_NAME  "scale_noise_factor"
#define ENGINE_GLUT_459_UI_GTK_SCALE_FREQUENCY_NAME     "scale_frequency"
#define ENGINE_GLUT_459_UI_GTK_SCALE_SHARPNESS_NAME     "scale_sharpness"
#define ENGINE_GLUT_459_UI_GTK_SCALE_PATTERN_NAME       "scale_pattern"

// glut 460
#define ENGINE_GLUT_460_DEFAULT_HEIGHT             834
#define ENGINE_GLUT_460_DEFAULT_WIDTH              1112
#define ENGINE_GLUT_460_DEFAULT_SCALE_FACTOR       30
#define ENGINE_GLUT_460_DEFAULT_FOV                45.0 // degrees !
#define ENGINE_GLUT_460_DEFAULT_ZNEAR              0.1
#define ENGINE_GLUT_460_DEFAULT_ZFAR               -1000.0
#define ENGINE_GLUT_460_DEFAULT_SCALE              2000.0f
#define ENGINE_GLUT_460_DEFAULT_X_ADD              0.0f
#define ENGINE_GLUT_460_DEFAULT_Y_ADD              -500.0f
#define ENGINE_GLUT_460_DEFAULT_Z_ADD              0.0f

// pge 461
#define ENGINE_PGE_461_DEFAULT_HEIGHT              1024
#define ENGINE_PGE_461_DEFAULT_WIDTH               1024
#define ENGINE_PGE_461_DEFAULT_NUMBER_OF_FRAMES    120
#define ENGINE_PGE_461_DEFAULT_NUMBER_OF_CORNERS   4
#define ENGINE_PGE_461_DEFAULT_NUMBER_OF_RADIUS    250
#define ENGINE_PGE_461_DEFAULT_NUMBER_OF_POINTS    160

// pge 462
#define ENGINE_PGE_462_DEFAULT_HEIGHT              768
#define ENGINE_PGE_462_DEFAULT_WIDTH               1024
#define ENGINE_PGE_462_DEFAULT_LINE_SPACING        1
#define ENGINE_PGE_462_DEFAULT_LINE_THICKNESS      1

// pge 463
#define ENGINE_PGE_463_DEFAULT_HEIGHT              768
#define ENGINE_PGE_463_DEFAULT_WIDTH               1024

// pge 464
#define ENGINE_PGE_464_DEFAULT_HEIGHT              768
#define ENGINE_PGE_464_DEFAULT_WIDTH               1024
#define ENGINE_PGE_464_DEFAULT_NUMBER_OF_HAIRS     180
#define ENGINE_PGE_464_DEFAULT_NUMBER_OF_SEGMENTS  30
#define ENGINE_PGE_464_DEFAULT_G                   1.0f

// glut 465
#define ENGINE_GLUT_465_DEFAULT_HEIGHT             834
#define ENGINE_GLUT_465_DEFAULT_WIDTH              1112
#define ENGINE_GLUT_465_DEFAULT_SCALE_FACTOR       30

// glut 466
#define ENGINE_GLUT_466_DEFAULT_HEIGHT             834
#define ENGINE_GLUT_466_DEFAULT_WIDTH              1112
#define ENGINE_GLUT_466_DEFAULT_SCALE_FACTOR       30

// glut 467
#define ENGINE_GLUT_467_DEFAULT_HEIGHT             834
#define ENGINE_GLUT_467_DEFAULT_WIDTH              1112
#define ENGINE_GLUT_467_DEFAULT_SCALE_FACTOR       30

// glut 468
#define ENGINE_GLUT_468_DEFAULT_HEIGHT             834
#define ENGINE_GLUT_468_DEFAULT_WIDTH              1112
#define ENGINE_GLUT_468_DEFAULT_SCALE_FACTOR       30

// glut 469
#define ENGINE_GLUT_469_DEFAULT_HEIGHT             834
#define ENGINE_GLUT_469_DEFAULT_WIDTH              1112
#define ENGINE_GLUT_469_DEFAULT_SCALE_FACTOR       30

#endif // DEFINES_21_H
