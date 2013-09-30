/*
  Copyright (C) 2009 Jon Macey

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
// VBO File in the format tx,ty,nx,ny,nz,vx,vy,vz for use with LoadBinaryVBO function in
// VBOPrimitives.cpp from the NCCA GraphicsLib.
//Automatically generated using WriteVBO function
// \brief data for a unit cube stored in GL_T2_N3_V3 format
/// @file Cube.h
/// @brief raw data for the cube vbo primitive
#ifndef __cube_H__
#define __cube_H__
namespace OpenUtility
{
static const int cubeSIZE=288;

static const float cube[288]={
        // Face #1
        0,0,0,0,1,-0.5,-0.5,0.5,
        1,0,0,0,1,0.5,-0.5,0.5,
        0,1,0,0,1,-0.5,0.5,0.5,
        0,1,0,0,1,-0.5,0.5,0.5,
        1,0,0,0,1,0.5,-0.5,0.5,
        1,1,0,0,1,0.5,0.5,0.5,
        // Face #2
        1,0,0,0,-1,-0.5,0.5,-0.5,
        0,0,0,0,-1,0.5,0.5,-0.5,
        1,1,0,0,-1,-0.5,-0.5,-0.5,
        1,1,0,0,-1,-0.5,-0.5,-0.5,
        0,0,0,0,-1,0.5,0.5,-0.5,
        0,1,0,0,-1,0.5,-0.5,-0.5,
        // Face #3
        0,1,0,1,0,-0.5,0.5,0.5,
        1,1,0,1,0,0.5,0.5,0.5,
        0,0,0,1,0,-0.5,0.5,-0.5,
        0,0,0,1,0,-0.5,0.5,-0.5,
        1,1,0,1,0,0.5,0.5,0.5,
        1,0,0,1,0,0.5,0.5,-0.5,
        // Face #4
        1,1,0,-1,0,-0.5,-0.5,-0.5,
        0,1,0,-1,0,0.5,-0.5,-0.5,
        1,0,0,-1,0,-0.5,-0.5,0.5,
        1,0,0,-1,0,-0.5,-0.5,0.5,
        0,1,0,-1,0,0.5,-0.5,-0.5,
        0,0,0,-1,0,0.5,-0.5,0.5,
        // Face #5
        0,1,1,0,0,0.5,-0.5,0.5,
        0,0,1,0,0,0.5,-0.5,-0.5,
        1,1,1,0,0,0.5,0.5,0.5,
        1,1,1,0,0,0.5,0.5,0.5,
        0,0,1,0,0,0.5,-0.5,-0.5,
        1,0,1,0,0,0.5,0.5,-0.5,
        // Face #6
        1,1,-1,0,0,-0.5,-0.5,-0.5,
        1,0,-1,0,0,-0.5,-0.5,0.5,
        0,1,-1,0,0,-0.5,0.5,-0.5,
        0,1,-1,0,0,-0.5,0.5,-0.5,
        1,0,-1,0,0,-0.5,-0.5,0.5,
        0,0,-1,0,0,-0.5,0.5,0.5,
}; //end of declaration
} // end namespace OpenUtility
#endif


