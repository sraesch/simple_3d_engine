#ifndef _ALCTYPES_H_
#define _ALCTYPES_H_

/**
 * OpenAL cross platform audio library
 * Copyright (C) 1999-2000 by authors.
 * This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the
 *  Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 *  Boston, MA  02111-1307, USA.
 * Or go to http://www.gnu.org/copyleft/lgpl.html
 */


#ifdef __cplusplus
extern "C" {
#endif

//
// This file contained nothing but bogus definitions and is here only to
// warn those people that used it.  All of the typedefs are not defined in the
// original specification and are just duplicates of the AL types.  Why would
// you want to have ALCboolean be a different type than ALboolean?  All of the
// #defines have been removed becuase an application should be using
// alcGetEnumValue and alGetEnumValue and not using the values out of this file
// as those values are dependent upon the OpenAL implementation.
//
int bogusHeader[-1];

#ifdef __cplusplus
}
#endif

#endif

