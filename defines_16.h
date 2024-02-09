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

#ifndef DEFINES_16_H
#define DEFINES_16_H

// pge 340
#define ENGINE_PGE_340_DEFAULT_HEIGHT              690
#define ENGINE_PGE_340_DEFAULT_WIDTH               920
#define ENGINE_PGE_340_DEFAULT_W                   ENGINE_PGE_340_DEFAULT_HEIGHT
#define ENGINE_PGE_340_DEFAULT_ALPHA_DECAY         30

// pge 341
#define ENGINE_PGE_341_DEFAULT_HEIGHT              690
#define ENGINE_PGE_341_DEFAULT_WIDTH               960
#define ENGINE_PGE_341_DEFAULT_NUMBER_OF_PARTICLES 64
#define ENGINE_PGE_341_DEFAULT_DISTANCE            100.0f

// pge 342
#define ENGINE_PGE_342_DEFAULT_HEIGHT              690
#define ENGINE_PGE_342_DEFAULT_WIDTH               920
#define ENGINE_PGE_342_DEFAULT_ALPHA_DECAY         50

// pge 343
#define ENGINE_PGE_343_DEFAULT_HEIGHT              834
#define ENGINE_PGE_343_DEFAULT_WIDTH               1112
#define ENGINE_PGE_343_DEFAULT_NUMBER_OF_OBJECTS   250
#define ENGINE_PGE_343_DEFAULT_ALPHA_DECAY         15

// pge 344
#define ENGINE_PGE_344_DEFAULT_HEIGHT              834
#define ENGINE_PGE_344_DEFAULT_WIDTH               1112
#define ENGINE_PGE_344_DEFAULT_ALPHA_DECAY         15

// glut 345
#define ENGINE_GLUT_345_DEFAULT_HEIGHT             690
#define ENGINE_GLUT_345_DEFAULT_WIDTH              920

// pge 346
#define ENGINE_PGE_346_DEFAULT_HEIGHT              834
#define ENGINE_PGE_346_DEFAULT_WIDTH               1112
//#define ENGINE_PGE_346_DEFAULT_ALPHA_DECAY         15

// glut 347
#define ENGINE_GLUT_347_DEFAULT_HEIGHT             834
#define ENGINE_GLUT_347_DEFAULT_WIDTH              1112
#define ENGINE_GLUT_347_DEFAULT_SCALE_FACTOR       30

// glut 348
#define ENGINE_GLUT_348_DEFAULT_HEIGHT             834
#define ENGINE_GLUT_348_DEFAULT_WIDTH              1112

// pge 349
#define ENGINE_PGE_349_DEFAULT_HEIGHT              834
#define ENGINE_PGE_349_DEFAULT_WIDTH               1112
#define ENGINE_PGE_349_DEFAULT_C_SIZE              ENGINE_PGE_349_DEFAULT_HEIGHT
#define ENGINE_PGE_349_DEFAULT_K                   6
#define ENGINE_PGE_349_DEFAULT_K_2                 40
#define ENGINE_PGE_349_DEFAULT_D                   2
//#define ENGINE_PGE_349_DEFAULT_ALPHA_DECAY         15

// pge 350
#define ENGINE_PGE_350_DEFAULT_HEIGHT              834
#define ENGINE_PGE_350_DEFAULT_WIDTH               1112

// glut 351
#define ENGINE_GLUT_351_DEFAULT_HEIGHT             834
#define ENGINE_GLUT_351_DEFAULT_WIDTH              1112
#define ENGINE_GLUT_351_DEFAULT_NUMBER_OF_PARTICLES 1000

// pge 352
#define ENGINE_PGE_352_DEFAULT_HEIGHT              834
#define ENGINE_PGE_352_DEFAULT_WIDTH               1112
#define ENGINE_PGE_352_DEFAULT_GROW_SPEED          0.01f
#define ENGINE_PGE_352_DEFAULT_ROTATE_SPEED        0.01f
#define ENGINE_PGE_352_DEFAULT_NUMBER_OF_NODES     500

// pge 353
#define ENGINE_PGE_353_DEFAULT_HEIGHT              834
#define ENGINE_PGE_353_DEFAULT_WIDTH               1112
#define ENGINE_PGE_353_DEFAULT_RESOLUTION          300
#define ENGINE_PGE_353_DEFAULT_SQUARE_SIZE         4
#define ENGINE_PGE_353_DEFAULT_L1SIZE              2.0f
#define ENGINE_PGE_353_DEFAULT_DT                  static_cast<float> (M_PI) / 30.0f

#endif // DEFINES_16_H
