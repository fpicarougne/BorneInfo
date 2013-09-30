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
// \brief data for a unit Octahedron stored in GL_T2_N3_V3 format
/// @file Octahedron.h
/// @brief raw data for VBO Octahedron
#ifndef __Octahedron_H__
#define __Octahedron_H__
namespace OpenUtility
{
static const int octahedronSIZE=192;
static float octahedron[192]={
        //vn 0.577350 0.577350 -0.577350

        0.285714,0.625,0.577350 ,0.577350, -0.577350,1,0,0,
        0.142857,0.375,0.577350 ,0.577350, -0.577350,0,0,-1,
        0.428571,0.375,0.577350 ,0.577350, -0.577350,0,1,0,
        //vn -0.577350, 0.577350, -0.577350

        0.571429,0.125,-0.577350, 0.577350, -0.577350,0,0,-1,
        0.714286,0.375,-0.577350, 0.577350, -0.577350,-1,0,0,
        0.428571,0.375,-0.577350, 0.577350, -0.577350,0,1,0,
         //vn -0.577350 0.577350 0.577350

        0.714286,0.375,-0.577350, 0.577350, 0.577350,-1,0,0,
        0.571429,0.625,-0.577350, 0.577350, 0.577350,0,-0,1,
        0.428571,0.375,-0.577350, 0.577350, 0.577350,0,1,0,
        //vn 0.577350 0.577350 0.577350

        0.428571,0.375,0.577350, 0.577350, 0.577350,0,1,0,
        0.571429,0.625,0.577350, 0.577350, 0.577350,0,-0,1,
        0.285714,0.625,0.577350, 0.577350, 0.577350,1,0,0,
        //vn 0.577350 -0.577350 -0.577350

        0,0.625,0.577350, -0.577350, -0.577350,0,-1,-0,
        0.142857,0.577350, -0.577350, -0.577350,-1,0,0,-1,
        0.285714,0.577350, -0.577350, -0.577350,0,1,0,0,

        //vn -0.577350 -0.577350 -0.577350
        0.857143,0.625,-0.577350, -0.577350, -0.577350,0,-1,-0,
        0.714286,0.375,-0.577350, -0.577350, -0.577350,-1,0,0,
        1,0.375,-0.577350, -0.577350, -0.577350,0,0,-1,
        //vn -0.577350, -0.577350, 0.577350

        0.857143,0.625,-0.577350, -0.577350, 0.577350,0,-1,-0,
        0.571429,0.625,-0.577350, -0.577350, 0.577350,0,-0,1,
        0.714286,0.375,-0.577350, -0.577350, 0.577350,-1,0,0,
        //vn 0.577350, -0.577350, 0.577350

        0.428571,0.875,0.577350, -0.577350, 0.577350,0,-1,-0,
        0.285714,0.625,0.577350, -0.577350, 0.577350,1,0,0,
        0.571429,0.625,0.577350, -0.577350, 0.577350,0,-0,1,


}; //end of declaration
} // end ngl namespace
#endif









