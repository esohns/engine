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

#ifndef DEFINES_2_H
#define DEFINES_2_H

// pge 30
#define ENGINE_PGE_30_DEFAULT_HEIGHT            600
#define ENGINE_PGE_30_DEFAULT_WIDTH             600
#define ENGINE_PGE_30_DEFAULT_MAX_ITERATIONS    100

// pge 31
#define ENGINE_PGE_31_DEFAULT_HEIGHT            400
#define ENGINE_PGE_31_DEFAULT_WIDTH             400
#define ENGINE_PGE_31_DEFAULT_NOISE_X           1.25 // coordinate
#define ENGINE_PGE_31_DEFAULT_NOISE_Y           0.75 // coordinate
#define ENGINE_PGE_31_DEFAULT_NOISE_Z           0.5  // coordinate
#define ENGINE_PGE_31_DEFAULT_RADIUS            150

// pge 32
#define ENGINE_PGE_32_DEFAULT_HEIGHT            600
#define ENGINE_PGE_32_DEFAULT_WIDTH             800
#define ENGINE_PGE_32_DEFAULT_HZ                60.0f
#define ENGINE_PGE_32_DEFAULT_VEL_ITERATIONS    8
#define ENGINE_PGE_32_DEFAULT_POS_ITERATIONS    3
#define ENGINE_PGE_32_DEFAULT_DIM_WAVEMACHINE   4.0f
#define ENGINE_PGE_32_DEFAULT_DIM_WEB           40.0f

// pge 33
#define ENGINE_PGE_33_DEFAULT_HEIGHT            400
#define ENGINE_PGE_33_DEFAULT_WIDTH             600

// pge 34
#define ENGINE_PGE_34_DEFAULT_HEIGHT            600
#define ENGINE_PGE_34_DEFAULT_WIDTH             800
#define ENGINE_PGE_34_DEFAULT_FACTOR_INCREMENT  0.015f
#define ENGINE_PGE_34_DEFAULT_TOTAL             200

// pge 35
#define ENGINE_PGE_35_DEFAULT_HEIGHT            600
#define ENGINE_PGE_35_DEFAULT_WIDTH             800
#define ENGINE_PGE_35_DEFAULT_RESOLUTION        2

// pge 36
#define ENGINE_PGE_36_DEFAULT_HEIGHT            600
#define ENGINE_PGE_36_DEFAULT_WIDTH             800
#define ENGINE_PGE_36_DEFAULT_RADIUS            7
#define ENGINE_PGE_36_DEFAULT_MAXSPEED          5
#define ENGINE_PGE_36_DEFAULT_MAXFORCE          0.5f
#define ENGINE_PGE_36_DEFAULT_MUTATION_RATE     0.01f
#define ENGINE_PGE_36_DEFAULT_FOOD_PROBABILITY  0.1f
#define ENGINE_PGE_36_DEFAULT_POISON_PROBABILITY 0.01f
#define ENGINE_PGE_36_DEFAULT_FOOD_RADIUS       2
#define ENGINE_PGE_36_DEFAULT_NUM_VEHICLES      50
#define ENGINE_PGE_36_DEFAULT_NUM_FOOD          40
#define ENGINE_PGE_36_DEFAULT_NUM_POISON        20
#define ENGINE_PGE_36_DEFAULT_FOOD_NUTRITION    0.2f
#define ENGINE_PGE_36_DEFAULT_POISON_NUTRITION  -0.5f
#define ENGINE_PGE_36_DEFAULT_CLONE_PROBABILITY 0.002f
#define ENGINE_PGE_36_DEFAULT_DIGESTION_PER_TURN 0.005f
#define ENGINE_PGE_36_DEFAULT_HEALTH            1.0f
#define ENGINE_PGE_36_DEFAULT_FOOD_WEIGHT       2.0f
#define ENGINE_PGE_36_DEFAULT_FOOD_PERCEPTION   100
#define ENGINE_PGE_36_DEFAULT_WEIGHT_MUTATION   0.1f
#define ENGINE_PGE_36_DEFAULT_PERCEPTION_MUTATION 10

// pge 37
#define ENGINE_PGE_37_DEFAULT_HEIGHT            600
#define ENGINE_PGE_37_DEFAULT_WIDTH             600
#define ENGINE_PGE_37_DEFAULT_SPACING           2
#define ENGINE_PGE_37_DEFAULT_RADIUS            200

// pge 38
#define ENGINE_PGE_38_DEFAULT_HEIGHT            600
#define ENGINE_PGE_38_DEFAULT_WIDTH             800
#define ENGINE_PGE_38_DEFAULT_LIFESPAN          1000
#define ENGINE_PGE_38_DEFAULT_MAXFORCE          0.2f
#define ENGINE_PGE_38_DEFAULT_MUTATION_RATE     0.01f
#define ENGINE_PGE_38_DEFAULT_ROCKET_RADIUS     5
#define ENGINE_PGE_38_DEFAULT_TARGET_RADIUS     8
#define ENGINE_PGE_38_DEFAULT_MAXSPEED          4.0f
#define ENGINE_PGE_38_DEFAULT_ROCKETS           50

// pge 39
#define ENGINE_PGE_39_DEFAULT_HEIGHT            600
#define ENGINE_PGE_39_DEFAULT_WIDTH             800
#define ENGINE_PGE_39_DEFAULT_RESOLUTION        5
#define ENGINE_PGE_39_DEFAULT_XY_INCREMENT      0.03
#define ENGINE_PGE_39_DEFAULT_Z_INCREMENT       0.01

// pge 40
#define ENGINE_PGE_40_DEFAULT_HEIGHT            600
#define ENGINE_PGE_40_DEFAULT_WIDTH             800
#define ENGINE_PGE_40_DEFAULT_CURSOR_SIZE       20
#define ENGINE_PGE_40_DEFAULT_CLOTH_WIDTH       600
#define ENGINE_PGE_40_DEFAULT_CLOTH_HEIGHT      400
#define ENGINE_PGE_40_DEFAULT_CLOTH_SPACING     7
#define ENGINE_PGE_40_DEFAULT_ELASTICITY        150.0f
#define ENGINE_PGE_40_DEFAULT_DRAG              0.01f
#define ENGINE_PGE_40_DEFAULT_GRAVITY           9.81f
#define ENGINE_PGE_40_DEFAULT_DT_FACTOR         10.0f
#define ENGINE_PGE_40_DEFAULT_MOUSE_FACTOR      150.0f
#define ENGINE_PGE_40_DEFAULT_Y0_X_MOD_PIN      2

// pge 41
#define ENGINE_PGE_41_DEFAULT_HEIGHT            600
#define ENGINE_PGE_41_DEFAULT_WIDTH             800
#define ENGINE_PGE_41_DEFAULT_NUMBER_OF_MOVERS  10
#define ENGINE_PGE_41_DEFAULT_ALPHA_DECAY       20

// pge 42
#define ENGINE_PGE_42_DEFAULT_HEIGHT            600
#define ENGINE_PGE_42_DEFAULT_WIDTH             800
#define ENGINE_PGE_42_DEFAULT_ITERATIONS_PER_CYCLE 100

// pge 43
#define ENGINE_PGE_43_DEFAULT_HEIGHT            600
#define ENGINE_PGE_43_DEFAULT_WIDTH             800
#define ENGINE_PGE_43_DEFAULT_PHASE_INCREMENT   0.03f
#define ENGINE_PGE_43_DEFAULT_NUMBER_OF_WAVES   5
#define ENGINE_PGE_43_DEFAULT_RADIUS            12

// pge 44
#define ENGINE_PGE_44_DEFAULT_HEIGHT            1024
#define ENGINE_PGE_44_DEFAULT_WIDTH             1280
#define ENGINE_PGE_44_DEFAULT_RADIUS            7
#define ENGINE_PGE_44_DEFAULT_MAXSPEED          4.0f
#define ENGINE_PGE_44_DEFAULT_MAXFORCE          0.2f
#define ENGINE_PGE_44_DEFAULT_NUMBER_OF_VEHICLES 8

// pge 45
#define ENGINE_PGE_45_DEFAULT_HEIGHT            1024
#define ENGINE_PGE_45_DEFAULT_WIDTH             1024
#define ENGINE_PGE_45_DEFAULT_ORDER             8

// pge 46
#define ENGINE_PGE_46_DEFAULT_HEIGHT            600
#define ENGINE_PGE_46_DEFAULT_WIDTH             800
#define ENGINE_PGE_46_DEFAULT_RADIUS            150.0f

// pge 47
#define ENGINE_PGE_47_DEFAULT_HEIGHT            800
#define ENGINE_PGE_47_DEFAULT_WIDTH             600

// pge 48
#define ENGINE_PGE_48_DEFAULT_HEIGHT            600
#define ENGINE_PGE_48_DEFAULT_WIDTH             800
#define ENGINE_PGE_48_DEFAULT_K                 -4.0f
#define ENGINE_PGE_48_DEFAULT_RESOLUTION        50
#define ENGINE_PGE_48_DEFAULT_NUMBER_OF_ORBITS  7

#endif
