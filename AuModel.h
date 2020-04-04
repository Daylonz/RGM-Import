#include <matrix3.h>
#include "AuVert.h"
#include "AuFace.h"

class AuModel
{
public:
	const wchar_t* name;
	short index;
	short fileType;
	int lightObjectFlag;
	Matrix3 nodePos;
	int numVerts;
	int numFaces;
	AuVert** verts;
	AuFace** faces;
};