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

#ifndef DEFINES_20_H
#define DEFINES_20_H

// glut 430
#define ENGINE_GLUT_430_DEFAULT_HEIGHT             834
#define ENGINE_GLUT_430_DEFAULT_WIDTH              1112
#define ENGINE_GLUT_430_DEFAULT_DETAIL             1200
#define ENGINE_GLUT_430_DEFAULT_ELLIPSE_SEGMENTS   10

// glut 431
#define ENGINE_GLUT_431_DEFAULT_HEIGHT             834
#define ENGINE_GLUT_431_DEFAULT_WIDTH              1112
#define ENGINE_GLUT_431_DEFAULT_SCALE_FACTOR       30

// pge 432
#define ENGINE_PGE_432_DEFAULT_HEIGHT              768
#define ENGINE_PGE_432_DEFAULT_WIDTH               1024
#define ENGINE_PGE_432_DEFAULT_NUMBER_OF_MOVERS    100
#define ENGINE_PGE_432_DEFAULT_MAX_LINE_LENGTH     120
#define ENGINE_PGE_432_DEFAULT_ALPHA_DECAY         30

// glut 433
#define ENGINE_GLUT_433_DEFAULT_HEIGHT             834
#define ENGINE_GLUT_433_DEFAULT_WIDTH              1112
#define ENGINE_GLUT_433_DEFAULT_SCALE_FACTOR       30

// glut 434
#define ENGINE_GLUT_434_DEFAULT_HEIGHT             834
#define ENGINE_GLUT_434_DEFAULT_WIDTH              1112
#define ENGINE_GLUT_434_DEFAULT_SCALE_FACTOR       30

// pge 435
#define ENGINE_PGE_435_DEFAULT_HEIGHT              720
#define ENGINE_PGE_435_DEFAULT_WIDTH               1080
#define ENGINE_PGE_435_DEFAULT_RIPPLE_RADIUS       3
#define ENGINE_PGE_435_DEFAULT_RIPPLE_RATE_MS      700

// glut 436
#define ENGINE_GLUT_436_DEFAULT_HEIGHT             834
#define ENGINE_GLUT_436_DEFAULT_WIDTH              1112
#define ENGINE_GLUT_436_DEFAULT_SCALE_FACTOR       30
#define ENGINE_GLUT_436_DEFAULT_SPEED              1.0f
#define ENGINE_GLUT_436_DEFAULT_TILING             10.0f
#define ENGINE_GLUT_436_DEFAULT_STRENGTH           0.0f

// pge 437
#define ENGINE_PGE_437_DEFAULT_HEIGHT              550
#define ENGINE_PGE_437_DEFAULT_WIDTH               550
#define ENGINE_PGE_437_DEFAULT_NSTEPS              25
#define ENGINE_PGE_437_DEFAULT_RSTEP               5
#define ENGINE_PGE_437_DEFAULT_RMIN                ENGINE_PGE_437_DEFAULT_RSTEP * 15
#define ENGINE_PGE_437_DEFAULT_NDIVS               120
#define ENGINE_PGE_437_DEFAULT_N_ANIM              200
#define ENGINE_PGE_437_DEFAULT_N_TRANSITION        ENGINE_PGE_437_DEFAULT_N_ANIM / 2.0f
#define ENGINE_PGE_437_DEFAULT_N_WAIT              100
#define ENGINE_PGE_437_DEFAULT_N_FRAMES            2 * (ENGINE_PGE_437_DEFAULT_N_ANIM + ENGINE_PGE_437_DEFAULT_N_WAIT)
#define ENGINE_PGE_437_DEFAULT_N_FRAMES_PATTERN    ENGINE_PGE_437_DEFAULT_N_FRAMES / 3.0f
#define ENGINE_PGE_437_DEFAULT_ALPHA_DECAY         5

// pge 438
#define ENGINE_PGE_438_DEFAULT_HEIGHT              768
#define ENGINE_PGE_438_DEFAULT_WIDTH               1024
#define ENGINE_PGE_438_DEFAULT_NUMBER_OF_PARTICLES 1000
#define ENGINE_PGE_438_DEFAULT_MAX_SIZE            2
#define ENGINE_PGE_438_DEFAULT_MAX_SPEED           0.75f
#define ENGINE_PGE_438_DEFAULT_MAX_FORCE           1.0f
#define ENGINE_PGE_438_DEFAULT_ALPHA_DECAY         30

// glut 439
#define ENGINE_GLUT_439_DEFAULT_HEIGHT             834
#define ENGINE_GLUT_439_DEFAULT_WIDTH              1112
#define ENGINE_GLUT_439_DEFAULT_SCALE_FACTOR       30
#define ENGINE_GLUT_439_DEFAULT_M                  6000
#define ENGINE_GLUT_439_DEFAULT_BB                 3.0f

// pge 440
#define ENGINE_PGE_440_DEFAULT_HEIGHT              768
#define ENGINE_PGE_440_DEFAULT_WIDTH               1024
#define ENGINE_PGE_440_DEFAULT_SIZE                4
#define ENGINE_PGE_440_DEFAULT_ALPHA               1.2f
#define ENGINE_PGE_440_DEFAULT_BETA                1.0f
#define ENGINE_PGE_440_DEFAULT_GAMMA               1.0f
#if defined (GTK2_USE)
#define ENGINE_PGE_440_GTK_UI_DEFINITION_FILE      "pge_440.gtk2"
#else
#define ENGINE_PGE_440_GTK_UI_DEFINITION_FILE      "pge_440.gtk3"
#endif // GTK2_USE
#define ENGINE_PGE_440_GTK_DIALOG_MAIN_NAME        "dialog_main"
#define ENGINE_PGE_440_GTK_SCALE_ALPHA_NAME        "scale_alpha"
#define ENGINE_PGE_440_GTK_SCALE_BETA_NAME         "scale_beta"
#define ENGINE_PGE_440_GTK_SCALE_GAMMA_NAME        "scale_gamma"

// glut 441
#define ENGINE_GLUT_441_DEFAULT_HEIGHT             834
#define ENGINE_GLUT_441_DEFAULT_WIDTH              1112
#define ENGINE_GLUT_441_DEFAULT_SCALE_FACTOR       30

#endif // DEFINES_20_H
