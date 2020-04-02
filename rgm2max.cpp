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
	std::map<std::string, short> nodeLookupTable;
	unsigned short			nodeCounter = 0;
	unsigned short			bipedalCounter = 0;
	AuModel					currentModel;

	virtual int				DoImport(const TCHAR *name,ImpInterface *i,Interface *gi, BOOL suppressPrompts=FALSE);	// Import file
	virtual char*			fgetstring(FILE* file);
	virtual void			parseNames(FILE* file);
	virtual void			parseFile(FILE* file, Interface* ip);
	virtual void			parseNewNode(FILE* file);
	virtual void			renderMeshIfExists();
	virtual int				parseMesh(FILE* file, Interface* ip);
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
	virtual HINSTANCE HInstance() 					{ return hInstance; }					// returns owning module handle
	

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

int rgm2max::DoImport(const TCHAR* name, ImpInterface* ii, Interface* ip, BOOL suppressPrompts)
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
	parseFile(file, ip);
	fclose(file);

	return TRUE;
}

char* rgm2max::fgetstring(FILE* file)
{
	int mem = 0;
	char* str = (char*)MAX_malloc(1);
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
	const char* root = fgetstring(file);
	int childrenRemaining = 0;
	fread(&childrenRemaining, 1, 1, file);
	while (childrenRemaining > 0)
	{
		const char* nextChild = fgetstring(file);
		int nextAmountChildren = 0;
		fread(&nextAmountChildren, 1, 1, file);
		nodeLookupTable[nextChild] = nodeCounter;
		nodeCounter++;
		childrenRemaining += nextAmountChildren;
		--childrenRemaining;
	}
}

void rgm2max::parseFile(FILE* file, Interface* ip)
{
	int nextByte = fgetc(file);
	for (int i = nextByte; i != -1; i = nextByte)
	{
		switch (i)
		{
			case 0x2A:
				renderMeshIfExists();
				parseNewNode(file);
				if (parseMesh(file, ip) != 0)
					return;
				break;

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

int rgm2max::parseMesh(FILE* file, Interface* ip)
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
		currentModel.verts = (AuVert**)MAX_malloc(sizeof(AuVert) * currentModel.numVerts);
		for (int i = 0; i < currentModel.numVerts; i++)
		{
			AuVert* vert;
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
		currentModel.verts = (AuVert**)MAX_malloc(sizeof(AuFace) * currentModel.numFaces);
		for (int i = 0; i < currentModel.numFaces; i++)
		{
			AuFace* face;
			fread(&face->indexX, 4, 1, file);
			fread(&face->indexZ, 4, 1, file);
			fread(&face->indexY, 4, 1, file);
			currentModel.faces[i] = face;
		}
	}
	return 0;
}
	
void rgm2max::renderMeshIfExists()
{
	return;
}