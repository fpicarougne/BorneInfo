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
// brief data for a unit tetrahedron stored in GL_T2_N3_V3 format
/// @file Tetrahedron.h
/// @brief raw data for the tetrahedron in VBOPrimitives
/// @brief implementation files for Clip class
#ifndef __tetrahedron_H__
#define __tetrahedron_H__
namespace OpenUtility
{
static const unsigned int tetrahedronSIZE=96;
static float tetrahedron[96]={
        //Triangle 0
         0.250000,0.375000,0, -1.000000, 0,1.000000,-1,0,
         0.500000,0,0, -1.000000, 0,-1,-1,1,
         0.750000,0.375000,0, -1.000000, 0,-1,-1,-1,
         //Triangle 1
         0,0,0.471405, 0.333333, 1,1.000000,-1,0,
         0.500000,0,0.471405, 0.333333, 1,0,1,0,
         0.250000,0.375000,0.471405, 0.333333, 1,-1,-1,1,
         //Triangle 2
         0.250000,0.375000,0.471405, 0.333333, -0.816497,0,1,0,
         0.750000,0.375000,0.471405, 0.333333, -0.816497,1.000000,-1,0,
         0.500000,0.750000,0.471405, 0.333333, -0.816497,-1,-1,-1,
         //Triangle 3
         0.750000,0.375000,-0.942809, 0.333333, 0,-1,-1,1,
         0.500000,0,-0.942809, 0.333333, 0,0,1,0,
         1.000000,0,-0.942809, 0.333333, 0,-1,-1,-1,
         //Triangle 4
}; //end of declaration
}
#endif

/*


vn 0.471405, 0.333333, -0.816497
vn 0.471405 0.333333 -0.816497
vn 0.471405 0.333333 -0.816497
vn -0.942809, 0.333333, 0
vn -0.942809 0.333333 0
vn -0.942809 0.333333 0
f 3/1/1 2/2/2 1/3/3
f 3/4/4 4/5/5 2/6/6
f 4/7/7 3/8/8 1/9/9
f 2/10/10 4/11/11 1/12/12


  */




