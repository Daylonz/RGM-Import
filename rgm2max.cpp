//**************************************************************************/
// Copyright (c) 1998-2007 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// DESCRIPTION: Appwizard generated plugin
// AUTHOR: 
//***************************************************************************/

#include "rgm2max.h"

#define rgm2max_CLASS_ID	Class_ID(0x6f5cb792, 0xa02697b6)


class rgm2max : public SceneImport
{
public:
	//Constructor/Destructor
	rgm2max();
	virtual ~rgm2max();

	virtual int				ExtCount();					// Number of extensions supported
	virtual const TCHAR *	Ext(int n);					// Extension #n (i.e. "3DS")
	virtual const TCHAR *	LongDesc();					// Long ASCII description (i.e. "Autodesk 3D Studio File")
	virtual const TCHAR *	ShortDesc();				// Short ASCII description (i.e. "3D Studio")
	virtual const TCHAR *	AuthorName();				// ASCII Author name
	virtual const TCHAR *	CopyrightMessage();			// ASCII Copyright message
	virtual const TCHAR *	OtherMessage1();			// Other message #1
	virtual const TCHAR *	OtherMessage2();			// Other message #2
	virtual unsigned int	Version();					// Version number * 100 (i.e. v3.01 = 301)
	virtual void			ShowAbout(HWND hWnd);		// Show DLL's "About..." box
	std::map<short, std::wstring> nodeLookupTable;
	unsigned short			nodeCounter = 0;
	unsigned short			bipedalCounter = 0;
	AuModel					currentModel;

	virtual int				DoImport(const TCHAR *name,ImpInterface *i,Interface *gi, BOOL suppressPrompts=FALSE);	// Import file
	virtual char*			fgetstring(FILE* file);
	virtual void			parseNames(FILE* file);
	virtual void			parseFile(FILE* file, ImpInterface* ii, Interface* ip);
	virtual void			parseNewNode(FILE* file);
	virtual int				renderMesh(ImpInterface* ii, Interface* ip);
	virtual int				parseMesh(FILE* file, ImpInterface* ii, Interface* ip);
	virtual int				parseRig(FILE* file, Interface* ip);
	StdMat*					GetMatForFlag(int flag, std::string name);
};



class rgm2maxClassDesc : public ClassDesc2 
{
public:
	virtual int IsPublic() 							{ return TRUE; }
	virtual void* Create(BOOL /*loading = FALSE*/) 		{ return new rgm2max(); }
	virtual const TCHAR *	ClassName() 			{ return GetString(IDS_CLASS_NAME); }
	virtual SClass_ID SuperClassID() 				{ return SCENE_IMPORT_CLASS_ID; }
	virtual Class_ID ClassID() 						{ return rgm2max_CLASS_ID; }
	virtual const TCHAR* Category() 				{ return GetString(IDS_CATEGORY); }

	virtual const TCHAR* InternalName() 			{ return _T("rgm2max"); }	// returns fixed parsable name (scripter-visible name)
	virtual HINSTANCE HInstance() 					{ return hInstance; }		// returns owning module handle
	

};


ClassDesc2* Getrgm2maxDesc() { 
	static rgm2maxClassDesc rgm2maxDesc;
	return &rgm2maxDesc; 
}




INT_PTR CALLBACK rgm2maxOptionsDlgProc(HWND hWnd,UINT message,WPARAM ,LPARAM lParam) {
	static rgm2max* imp = nullptr;

	switch(message) {
		case WM_INITDIALOG:
			imp = (rgm2max *)lParam;
			CenterWindow(hWnd,GetParent(hWnd));
			return TRUE;

		case WM_CLOSE:
			EndDialog(hWnd, 0);
			return 1;
	}
	return 0;
}


//--- rgm2max -------------------------------------------------------
rgm2max::rgm2max()
{

}

rgm2max::~rgm2max() 
{

}

int rgm2max::ExtCount()
{
	return 1;
}

const TCHAR *rgm2max::Ext(int /*n*/)
{		
	return _T("rgm");
}

const TCHAR *rgm2max::LongDesc()
{
	return _T("Audition Game Model");
}
	
const TCHAR *rgm2max::ShortDesc() 
{			
	return _T("Audition Game Model");
}

const TCHAR *rgm2max::AuthorName()
{			
	return _T("Daylon");
}

const TCHAR *rgm2max::CopyrightMessage() 
{	
	#pragma message(TODO("Return ASCII Copyright message"))
	return _T("");
}

const TCHAR *rgm2max::OtherMessage1() 
{		
	//TODO: Return Other message #1 if any
	return _T("");
}

const TCHAR *rgm2max::OtherMessage2() 
{		
	//TODO: Return other message #2 in any
	return _T("");
}

unsigned int rgm2max::Version()
{				
	return 100;
}

void rgm2max::ShowAbout(HWND /*hWnd*/)
{			
	// Optional
}

int rgm2max::DoImport(const TCHAR* name, ImpInterface* ii, Interface* ip, BOOL /*suppressPrompts*/)
{
	#pragma message(TODO("Implement the actual file import here and"))	
	//if(!suppressPrompts)
	//	DialogBoxParam(hInstance, 
	//			MAKEINTRESOURCE(IDD_PANEL), 
	//			GetActiveWindow(), 
	//			rgm2maxOptionsDlgProc, (LPARAM)this);
	//
	#pragma message(TODO("return TRUE If the file is imported properly"))

	FILE* file;
	bstr_t b(name);
	const char* file_name = b;
	file = fopen(file_name, "rb");

	std::string magic(30, '\0');
	fread(&magic[0], sizeof(char), 30, file);
	if (magic.compare("Delight3D 3D DynamicObjectFile") != 0)
	{
		MessageBoxA(ip->GetMAXHWnd(), "File is not an RGM file.", "RGM Import - By Daylon", MB_ICONINFORMATION);
		fclose(file);
		return TRUE;
	}
	fseek(file, 11, SEEK_CUR);
	parseNames(file);
	parseFile(file, ii, ip);
	fclose(file);
	return TRUE;
}

char* rgm2max::fgetstring(FILE* file)
{
	int mem = 0;
	char* str = (char*)malloc(1);
	char next_read = fgetc(file);
	str[mem] = next_read;
	while (true)
	{
		next_read = fgetc(file);
		mem += 1;
		if (next_read == '\0')
		{
			str = (char*)MAX_realloc(str, mem + sizeof(char));
			str[mem] = next_read;
			break;
		}
		str = (char*)MAX_realloc(str, mem + sizeof(char));
		str[mem] = next_read;
	}
	return str;
}

void rgm2max::parseNames(FILE* file)
{
	fgetstring(file);
	int childrenRemaining = 0;
	fread(&childrenRemaining, 1, 1, file);
	while (childrenRemaining > 0)
	{
		std::string nextChild = fgetstring(file);
		int nextAmountChildren = 0;
		fread(&nextAmountChildren, 1, 1, file);
		nodeLookupTable[nodeCounter] = std::wstring(nextChild.begin(), nextChild.end());
		nodeCounter++;
		childrenRemaining += nextAmountChildren;
		--childrenRemaining;
	}
}

void rgm2max::parseFile(FILE* file, ImpInterface* ii, Interface* ip)
{
	int fps;
	int framesInScene;
	fread(&fps, 4, 1, file);
	fread(&framesInScene, 4, 1, file);

	int nextByte = fgetc(file);
	for (int i = nextByte; i != -1; i = nextByte)
	{
		switch (i)
		{
			case 0x2A: //new node
				parseNewNode(file);
				if (parseMesh(file, ii, ip) != 0)
					return;
				break;
			case 0x6D: //anim data
				MessageBoxA(ip->GetMAXHWnd(), "Error 6D: RGM contains type not yet supported.", "RGM Import - By Daylon", MB_ICONINFORMATION);
				return;
			case 0x70: //??? Not yet supported
				MessageBoxA(ip->GetMAXHWnd(), "Error 70: RGM contains type not yet supported.", "RGM Import - By Daylon", MB_ICONINFORMATION);
				return;
			case 0x71: //rigging data
				MessageBoxA(ip->GetMAXHWnd(), "Error 71: RGM contains type not yet supported.", "RGM Import - By Daylon", MB_ICONINFORMATION);
				return;
			case 0x72: //??? Not yet supported
				MessageBoxA(ip->GetMAXHWnd(), "Error 72: RGM contains type not yet supported.", "RGM Import - By Daylon", MB_ICONINFORMATION);
				return;
			case 0x73: //scale factor. Maybe we can support this if we find an example of it?
				MessageBoxA(ip->GetMAXHWnd(), "Error 73: RGM contains type not yet supported.", "RGM Import - By Daylon", MB_ICONINFORMATION);
				return;
			default:
				MessageBoxA(ip->GetMAXHWnd(), "Error D: Unknown byte at offset...", "RGM Import - By Daylon", MB_ICONINFORMATION);
				MessageBoxA(ip->GetMAXHWnd(), std::to_string(ftell(file)).c_str(), "RGM Import - By Daylon", MB_ICONINFORMATION);
				return;

		}
		nextByte = fgetc(file);
	}
}

void rgm2max::parseNewNode(FILE* file)
{
	AuModel model;
	fread(&model.index, 2, 1, file);
	fread(&model.fileType, 2, 1, file);
	currentModel = model;
}

int rgm2max::parseMesh(FILE* file, ImpInterface* ii, Interface* ip)
{
	Matrix3 matrix;
	Point3 row1;
	Point3 row2;
	Point3 row3;
	Point3 row4;
	fread(&row1.x, 4, 1, file);
	fread(&row1.z, 4, 1, file);
	fread(&row1.y, 4, 1, file);
	matrix.SetRow(0, row1);
	fread(&row3.x, 4, 1, file);
	fread(&row3.z, 4, 1, file);
	fread(&row3.y, 4, 1, file);
	matrix.SetRow(1, row2);
	fread(&row2.x, 4, 1, file);
	fread(&row2.z, 4, 1, file);
	fread(&row2.y, 4, 1, file);
	matrix.SetRow(2, row3);
	fread(&row4.x, 4, 1, file);
	fread(&row4.z, 4, 1, file);
	fread(&row4.y, 4, 1, file);
	matrix.SetRow(3, row4);
	currentModel.nodePos = matrix;

	fread(&currentModel.numVerts, 4, 1, file);
	if (currentModel.numVerts > 0)
	{
		fread(&currentModel.lightObjectFlag, 1, 1, file);
		if (currentModel.lightObjectFlag > 0)
		{
			//do something
			MessageBoxA(ip->GetMAXHWnd(), "RGM contains type not yet supported.", "RGM Import - By Daylon", MB_ICONINFORMATION);
			return 1;
		}
		currentModel.verts = new AuVert*[currentModel.numVerts];
		for (int i = 0; i < currentModel.numVerts; i++)
		{
			AuVert* vert = new AuVert;
			fread(&vert->x, 4, 1, file);
			fread(&vert->z, 4, 1, file);
			fread(&vert->y, 4, 1, file);
			fread(&vert->nx, 4, 1, file);
			fread(&vert->nz, 4, 1, file);
			fread(&vert->ny, 4, 1, file);
			fread(&vert->u, 4, 1, file);
			float v;
			fread(&v, 4, 1, file);
			vert->v = 1 - v;
			currentModel.verts[i] = vert;
		}
		fread(&currentModel.numFaces, 4, 1, file);
		currentModel.faces = new AuFace*[currentModel.numFaces];
		for (int i = 0; i < currentModel.numFaces; i++)
		{
			AuFace* face = new AuFace;
			fread(&face->indexX, 2, 1, file);
			fread(&face->indexZ, 2, 1, file);
			fread(&face->indexY, 2, 1, file);
			currentModel.faces[i] = face;
		}

		fread(&currentModel.numMaterialIDs, 4, 1, file); // read the number of material IDs
		currentModel.materialIDCounts = new int*[currentModel.numMaterialIDs];
		for (int i = 0; i < currentModel.numMaterialIDs; i++)
		{
			int* next = new int;
			fread(next, 4, 1, file);
			currentModel.materialIDCounts[i] = next;
		}

		fread(&currentModel.numMaterials, 4, 1, file); // read the number of materials
		currentModel.materials = new AuMaterial*[currentModel.numMaterials];
		for (int i = 0; i < currentModel.numMaterials; i++)
		{
			AuMaterial* newMat = new AuMaterial;
			newMat->name = fgetstring(file);
			int charsToSkip = 39 - newMat->name.length();
			fseek(file, charsToSkip, SEEK_CUR);
			newMat->materialFlag = fgetc(file);
			currentModel.materials[i] = newMat;
		}
		if (renderMesh(ii, ip) == 1)
		{
			MessageBoxA(ip->GetMAXHWnd(), "Unable to create node. Please submit RGM for review.", "RGM Import - By Daylon", MB_ICONINFORMATION);
			return 1;
		}
	}
	return 0;
}
	
int rgm2max::renderMesh(ImpInterface* ii, Interface* ip)
{
	Matrix3 transform;
	Point3 p;
	Point3 n;
	Point3 r;
	TriObject* object = CreateNewTriObject();
	Mesh* m = &object->GetMesh();

	m->setNumVerts(currentModel.numVerts); // set num verts
	m->setNumTVerts(currentModel.numVerts); // set num tverts
	m->setNumFaces(currentModel.numFaces); // set num faces

	for (int i = 0; i < m->getNumVerts(); i++) // set verts, tverts
	{
		p.x = currentModel.verts[i]->x;
		p.y = currentModel.verts[i]->y;
		p.z = currentModel.verts[i]->z;

		m->setVert(i, p);
	}

	for (int i = 0; i < m->getNumFaces(); i++) // set faces
	{
		m->faces[i].setVerts(currentModel.faces[i]->indexX,
			currentModel.faces[i]->indexY,
			currentModel.faces[i]->indexZ);
		m->faces[i].setSmGroup(1);
	}

	m->SpecifyNormals();
	MeshNormalSpec* nspec = m->GetSpecifiedNormals();
	nspec->ClearNormals();
	nspec->SetNumNormals(currentModel.numVerts);
	nspec->SetNumFaces(currentModel.numFaces);
	for (int i = 0; i < m->getNumVerts(); i++) // set normals
	{
		n.x = currentModel.verts[i]->nx;
		n.y = currentModel.verts[i]->ny;
		n.z = currentModel.verts[i]->nz;

		nspec->Normal(i) = n;
		nspec->SetNormalExplicit(i, TRUE);
	}
	MeshNormalFace* faces = nspec->GetFaceArray();
	for (int i = 0; i < m->getNumFaces(); i++)
	{
		faces[i].SpecifyAll();
		faces[i].SetNormalID(0, currentModel.faces[i]->indexX);
		faces[i].SetNormalID(1, currentModel.faces[i]->indexY);
		faces[i].SetNormalID(2, currentModel.faces[i]->indexZ);
	}
	/*
	for (int i = 0; i < m->getNumVerts(); i++) // set normals
	{
		n.x = currentModel.verts[i]->nx;
		n.y = currentModel.verts[i]->ny;
		n.z = currentModel.verts[i]->nz;

		m->setNormal(i, n);
	}

	MeshNormalSpec* nspec = m->GetSpecifiedNormals();

	if (nspec && !nspec->GetFlag(MESH_NORMAL_NORMALS_BUILT))
	{
		m->SpecifyNormals();
		nspec = m->GetSpecifiedNormals();
	}
	nspec->ClearFlag(MESH_NORMAL_NORMALS_BUILT);
	nspec->CheckNormals();
	Point3* normals = nspec->GetNormalArray();
	for (int i = 0; i < nspec->GetNumNormals(); i++)
	{
		n.x = currentModel.verts[i]->nx;
		n.y = currentModel.verts[i]->ny;
		n.z = currentModel.verts[i]->nz;
		normals[i] = n;
		delete currentModel.verts[i];
	}
	nspec->SetAllExplicit(true);
	*/

	
	// Setup UVW maps
	m->setNumMaps(2); // 2 is minimum, 0 being color and 1 texture
	m->setMapSupport(0, FALSE); // no color
	m->setMapSupport(1, TRUE); // use texture

	m->setNumMapVerts(1, currentModel.numVerts);
	for (int i = 0; i < currentModel.numVerts; ++i) // Create texture verts
	{
		m->mapVerts(1)[i].Set(currentModel.verts[i]->u, currentModel.verts[i]->v, 0);
		delete currentModel.verts[i];
	}

	for (int i = 0; i < currentModel.numFaces; ++i) // Create texture faces
	{
		m->mapFaces(1)[i].setTVerts(currentModel.faces[i]->indexX,
			currentModel.faces[i]->indexY,
			currentModel.faces[i]->indexZ);
		delete currentModel.faces[i];
	}

	// Assign material IDs

	int faceIndex = 0;
	for (int i = 0; i < currentModel.numMaterialIDs; i++)
	{
		for (int j = 0; j < *currentModel.materialIDCounts[i]; j++)
		{
			m->faces[faceIndex].setMatID(i);
			faceIndex++;
		}
	}

	// Create multi/sub material
	MultiMtl* mat = NewDefaultMultiMtl();

	mat->SetNumSubMtls(currentModel.numMaterials);
	for (int i = 0; i < currentModel.numMaterials; i++)
	{
		StdMat* nextMat = GetMatForFlag(currentModel.materials[i]->materialFlag, currentModel.materials[i]->name); // Get base material from flag
		mat->SetSubMtl(i, nextMat);
		GetCOREInterface()->ActivateTexture(nextMat, mat);
	}

	// Fixup mesh
	m->buildNormals();
	m->buildBoundingBox();
	m->InvalidateEdgeList();
	m->InvalidateGeomCache();

	ImpNode* node = ii->CreateNode();
	if (!node)
	{
		return 1;
	}
	node->Reference(object);
	node->GetINode()->SetMtl(mat);

	node->SetName(nodeLookupTable.find(currentModel.index - 1)->second.c_str());
	//node->GetINode()->SetNodeTM(0, currentModel.nodePos);
	ii->AddNodeToScene(node);
	//node->SetName(_T("Test"));
	ii->RedrawViews();
	return 0;
}

int rgm2max::parseRig(FILE* file, Interface* ip)
{
	return 0;
}

StdMat* rgm2max::GetMatForFlag(int flag, std::string name)
{
	StdMat2* result = NewDefaultStdMat();

	BitmapTex* bm = NewDefaultBitmapTex(); // Create new Bitmap
	bm->SetAlphaAsMono(TRUE);

	std::string mapNameS = name + ".dds";
	std::wstring mapNameW(mapNameS.begin(), mapNameS.end());
	bm->SetMapName(mapNameW.c_str(), FALSE); // Assign name to Bitmap

	switch (flag)
	{
		case MATERIAL_DIFFUSE_FACETED_TWOSIDED:
			result->EnableMap(ID_DI, TRUE);
			result->SetSubTexmap(ID_DI, bm);
			result->SetFaceted(TRUE);
			result->SetTwoSided(TRUE);
			break;
		case MATERIAL_DIFFUSE_FACETED:
			result->EnableMap(ID_DI, TRUE);
			result->SetSubTexmap(ID_DI, bm);
			result->SetFaceted(TRUE);
			break;
		case MATERIAL_DIFFUSE_TWOSIDED:
			result->EnableMap(ID_DI, TRUE);
			result->SetSubTexmap(ID_DI, bm);
			result->SetTwoSided(TRUE);
			break;
		case MATERIAL_DIFFUSE:
			result->EnableMap(ID_DI, TRUE);
			result->SetSubTexmap(ID_DI, bm);
			break;
		case MATERIAL_OPAQUE_FACETED_TWOSIDED:
			result->EnableMap(ID_DI, TRUE);
			result->EnableMap(ID_OP, TRUE);
			result->SetSubTexmap(ID_DI, bm);
			result->SetSubTexmap(ID_OP, bm);
			result->SetFaceted(TRUE);
			result->SetTwoSided(TRUE);
			break;
		case MATERIAL_OPAQUE_FACETED:
			result->EnableMap(ID_DI, TRUE);
			result->EnableMap(ID_OP, TRUE);
			result->SetSubTexmap(ID_DI, bm);
			result->SetSubTexmap(ID_OP, bm);
			result->SetFaceted(TRUE);
			break;
		case MATERIAL_OPAQUE_TWOSIDED:
			result->EnableMap(ID_DI, TRUE);
			result->EnableMap(ID_OP, TRUE);
			result->SetSubTexmap(ID_DI, bm);
			result->SetSubTexmap(ID_OP, bm);
			result->SetTwoSided(TRUE);
			break;
		case MATERIAL_OPAQUE:
			result->EnableMap(ID_DI, TRUE);
			result->EnableMap(ID_OP, TRUE);
			result->SetSubTexmap(ID_DI, bm);
			result->SetSubTexmap(ID_OP, bm);
			break;
	}
	return result;
}