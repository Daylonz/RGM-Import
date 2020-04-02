#include <matrix3.h>
#include "AuVert.h"

class AuModel
{
public:
	short index;
	short fileType;
	int lightObjectFlag;
	Matrix3 nodePos;
	int numVerts;
	int numFaces;
	AuVert** verts;
	AuFace** faces;
};
