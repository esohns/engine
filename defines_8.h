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

#ifndef DEFINES_8_H
#define DEFINES_8_H

// pge 180
#define ENGINE_PGE_180_DEFAULT_HEIGHT           720
#define ENGINE_PGE_180_DEFAULT_WIDTH            720
#define ENGINE_PGE_180_DEFAULT_W                720
#define ENGINE_PGE_180_DEFAULT_D                64

// pge 181
#define ENGINE_PGE_181_DEFAULT_HEIGHT           800
#define ENGINE_PGE_181_DEFAULT_WIDTH            800
#define ENGINE_PGE_181_DEFAULT_NUMBER_OF_STARS  3000
#define ENGINE_PGE_181_DEFAULT_SPEED            5.0f

// pge 182
#define ENGINE_PGE_182_DEFAULT_HEIGHT           800
#define ENGINE_PGE_182_DEFAULT_WIDTH            800
#define ENGINE_PGE_182_DEFAULT_SHIFT_INTERVAL   60
#define ENGINE_PGE_182_DEFAULT_NUMBER_OF_HOLES  24
#define ENGINE_PGE_182_DEFAULT_NUMBER_OF_STARS  5000
#define ENGINE_PGE_182_DEFAULT_SPEED            3.0f

// pge 183
#define ENGINE_PGE_183_DEFAULT_HEIGHT           720
#define ENGINE_PGE_183_DEFAULT_WIDTH            720
#define ENGINE_PGE_183_DEFAULT_W                720

// pge 184
#define ENGINE_PGE_184_DEFAULT_HEIGHT           360
#define ENGINE_PGE_184_DEFAULT_WIDTH            360
#define ENGINE_PGE_184_DEFAULT_W                360

// pge 185
#define ENGINE_PGE_185_DEFAULT_HEIGHT           640
#define ENGINE_PGE_185_DEFAULT_WIDTH            640
#define ENGINE_PGE_185_DEFAULT_W                640

// pge 186
#define ENGINE_PGE_186_DEFAULT_HEIGHT           720
#define ENGINE_PGE_186_DEFAULT_WIDTH            720
#define ENGINE_PGE_186_DEFAULT_W                720
#define ENGINE_PGE_186_DEFAULT_ALPHA_DECAY      10

// pge 187
#define ENGINE_PGE_187_DEFAULT_HEIGHT           720
#define ENGINE_PGE_187_DEFAULT_WIDTH            720
#define ENGINE_PGE_187_DEFAULT_W                720
#define ENGINE_PGE_187_DEFAULT_R                99

// pge 188
#define ENGINE_PGE_188_DEFAULT_HEIGHT           720
#define ENGINE_PGE_188_DEFAULT_WIDTH            720
#define ENGINE_PGE_188_DEFAULT_W                720
#define ENGINE_PGE_188_DEFAULT_R                25

// pge 189
#define ENGINE_PGE_189_DEFAULT_HEIGHT           600
#define ENGINE_PGE_189_DEFAULT_WIDTH            800
#define ENGINE_PGE_188_DEFAULT_STEP             3.0f
#define ENGINE_PGE_188_DEFAULT_MIN_DIST         ENGINE_PGE_188_DEFAULT_STEP / 2.0f
#define ENGINE_PGE_188_DEFAULT_MAX_DIST         25.0f

// pge 190
#define ENGINE_PGE_190_DEFAULT_HEIGHT           600
#define ENGINE_PGE_190_DEFAULT_WIDTH            800
#define ENGINE_PGE_190_DEFAULT_NUMBER_OF_CIRCLES 6
#define ENGINE_PGE_190_DEFAULT_MIN_SPEED        0.2f
#define ENGINE_PGE_190_DEFAULT_MAX_SPEED        0.8f

// pge 191
#define ENGINE_PGE_191_DEFAULT_HEIGHT           240
#define ENGINE_PGE_191_DEFAULT_WIDTH            320
#define ENGINE_PGE_191_DEFAULT_NUMBER_OF_POINTS 11

// pge 192
#define ENGINE_PGE_192_DEFAULT_HEIGHT           256
#define ENGINE_PGE_192_DEFAULT_WIDTH            256
#define ENGINE_PGE_192_DEFAULT_W                ENGINE_PGE_192_DEFAULT_WIDTH

// pge 193
#define ENGINE_PGE_193_DEFAULT_HEIGHT           720
#define ENGINE_PGE_193_DEFAULT_WIDTH            800
#define ENGINE_PGE_193_DEFAULT_NUMBER_OF_CREATURES 5

// pge 194
#define ENGINE_PGE_194_DEFAULT_HEIGHT           512
#define ENGINE_PGE_194_DEFAULT_WIDTH            512
#define ENGINE_PGE_194_DEFAULT_W                512
#define ENGINE_PGE_194_SENSOR_DISTANCE          3.0f
#define ENGINE_PGE_194_SENSOR_ANGLE             40 / 180.0f * static_cast<float> (M_PI)
#define ENGINE_PGE_194_TURNING_SPEED            0.2f
#define ENGINE_PGE_194_SPEED                    2.0f
#define ENGINE_PGE_194_DECAY_FACTOR             0.9f
#define ENGINE_PGE_194_DEPOSIT_AMOUNT           0.2f
#define ENGINE_PGE_194_NUMBER_OF_AGENTS         1000
#define ENGINE_PGE_194_START_IN_CIRCLE          true
#define ENGINE_PGE_194_HIGHLIGHT_AGENTS         false
#define ENGINE_PGE_194_RAMDOM_TURNING           false
#define ENGINE_PGE_194_WRAP_AROUND              true

// pge 195
#define ENGINE_PGE_195_DEFAULT_HEIGHT           1024
#define ENGINE_PGE_195_DEFAULT_WIDTH            1024
#define ENGINE_PGE_195_DEFAULT_CO               92 * 2
#define ENGINE_PGE_195_DEFAULT_MW               1.0f

// pge 196
#define ENGINE_PGE_196_DEFAULT_HEIGHT           256
#define ENGINE_PGE_196_DEFAULT_WIDTH            256
#define ENGINE_PGE_196_DEFAULT_SIZE             256

// pge 197
#define ENGINE_PGE_197_DEFAULT_HEIGHT           512
#define ENGINE_PGE_197_DEFAULT_WIDTH            512

// pge 198
#define ENGINE_PGE_198_DEFAULT_HEIGHT           600
#define ENGINE_PGE_198_DEFAULT_WIDTH            800
#define ENGINE_PGE_198_DEFAULT_SPACING          16
#define ENGINE_PGE_198_DEFAULT_MEAN_DIAMETER    8

#endif
