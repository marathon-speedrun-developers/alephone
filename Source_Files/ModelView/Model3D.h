/*

	Copyright (C) 1991-2001 and beyond by Bungie Studios, Inc.
	and the "Aleph One" developers.
 
	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	This license is contained in the file "COPYING",
	which is included with this source code; it is available online at
	http://www.gnu.org/licenses/gpl.html

	3D-Model Object Storage
	It is intended to be as OpenGL-friendly as is reasonably possible
	
	By Loren Petrich, June 16, 2001
*/
#ifndef MODEL_3D
#define MODEL_3D

using namespace std;

#ifdef HAVE_OPENGL

#if defined (__APPLE__) && defined (__MACH__)
# include <OpenGL/gl.h>
#else
# ifdef WIN32
#  include <wtypes.h>	// for POINT, so we can include wingdi.h
#  include <wingdi.h>	// for WINGDIAPI, appears in GL/gl.h
# endif
# include <GL/gl.h>
#endif

#include <vector>


// For boned models; calculate vertices from these
struct Model3D_VertexSource
{
	GLfloat Position[3];
	GLushort Bone0, Bone1;	// Indices of the two bones (NONE is no bone)
	GLfloat Blend;			// Blend factor: limits are 0 = first bone, 1 = second bone.
};


// Bone definition: neutral position is assumed to be zero rotation.
// 
struct Model3D_Bone
{
	enum
	{
		Pop		= 0x0001,
		Push	= 0x0002
	};

	GLfloat Position[3];
	
	// Tomb-Raider-like system for traversing a tree with reference to a stack of bones;
	// "pop" removes a bone from that stack and makes it the current one, while
	// "push" adds the current bone to that stack.
	// Popping an empty stack means reaching the assumed root bone.
	GLushort Flags;
};


struct Model3D
{
	// Assumed dimensions:
	// Positions: 3
	// Texture coordinates: 2
	// Normals: 3
	// Colors: 3 [should an alpha channel also be included?]
	
	// Positions assumed to be 3-dimensional
	vector<GLfloat> Positions;
	GLfloat *PosBase() {return &Positions[0];}
	
	// Texture coordinates assumed to be 2-dimensional
	// Parallel to the vertex-position array
	vector<GLfloat> TxtrCoords;
	GLfloat *TCBase() {return &TxtrCoords[0];}
	
	// Normals assumed to be 3-dimensional
	// Parallel to the vertex-position array
	vector<GLfloat> Normals;
	GLfloat *NormBase() {return &Normals[0];}
	
	// Vertex colors (useful for by-hand vertex lighting)
	// Parallel to the vertex-position array
	vector<GLfloat> Colors;
	GLfloat *ColBase() {return &Colors[0];}
	
	// Vertex-source indices; into vertex-source array:
	vector<GLushort> VtxSrcIndices;
	GLushort *VtxSIBase() {return &VtxSrcIndices[0];}
	
	// Vertex-source array: useful with boned models
	vector<Model3D_VertexSource> VtxSources;
	Model3D_VertexSource *VtxSrcBase() {return &VtxSources[0];}
	
	// Bone array:
	vector<Model3D_Bone> Bones;
	Model3D_Bone *BoneBase() {return &Bones[0];}
	
	// List of indices into the aforementioned vertices;
	// the list is a list of triangles.
	vector<GLushort> VertIndices;
	GLushort *VIBase() {return &VertIndices[0];}
	int NumVI() {return VertIndices.size();}
	
	// Bounding box (0 = min, 1 = max)
	GLfloat BoundingBox[2][3];
	
	// From the position data
	void FindBoundingBox();
	
	// For debugging bounding-box-handling code
	// NULL means don't render one set of edges
	void RenderBoundingBox(const GLfloat *EdgeColor, const GLfloat *DiagonalColor);
	
	// Process the normals in various ways
	enum
	{
		None,					// Gets rid of them
		Original,				// Uses the model's original normals
		Reversed,				// Reverses the direction of the original normals
		ClockwiseSide,			// Normals point in the sides' clockwise direction
		CounterclockwiseSide,	// Normals point in the sides' counterclockwise direction
		NUMBER_OF_NORMAL_TYPES
	};
	// The second is for deciding whether a vertex is to have
	// the average of its neighboring polygons' normals
	// or whether a vertex is to be split into separate vertices,
	// each with a polygon's normal
	void AdjustNormals(int NormalType, float SmoothThreshold = 0.5);
	
	// So they all have length 1
	void NormalizeNormals() {AdjustNormals(Original);}
	
	// Erase everything
	void Clear();
	
	// Constructor
	Model3D() {FindBoundingBox();}
};

#endif
class Model3D;
#endif
