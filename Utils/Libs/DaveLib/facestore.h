/****************************************/
/*** Generic Face Compilation Storage ***/
/****************************************/

#ifndef __FACESTORE_HEADER__
#define __FACESTORE_HEADER__

#include <ginio.h>
#include <gintex.h>
#include <Vector>
#include <TexGrab.h>

#include "..\..\..\tools\data\include\dStructs.h"

using namespace std;

//***************************************************************************
/*
struct	sFaceTexList
{
	GString		Name;
	int			TexGrabId;
};
*/
//***************************************************************************
class	CFace
{
public:
//	int				PntCount;
	int				Mat;
	Vector3			vtx[4];
	int				pts[4];
	int				idx[4];
	sUV				uvs[4];
	int				vis[4];
	Vector3			Normal;
	bool			Avail;
	int				ID;
	GString			TexName;
};

/*
class	CFace
{
public:
	CFace()
	{
	vtx.resize(3);
	pts.resize(3);
	idx.resize(3);
	uvs.resize(3);
	vis.resize(3);
	}

//	int				PntCount;
	int				Mat;
	vector<Vector3>	vtx;
	vector<int>		pts;
	vector<int>		idx;
	vector<sUV>		uvs;
	vector<int>		vis;
	Vector3			Normal;
	bool			Avail;
	int				ID;
	GString			TexName;
};

*/
//***************************************************************************
#ifndef	sTriFace
struct  sTriFace
{
	int			Mat;
	Vector3		vtx[3];
	int			pts[3];
	sUV			uvs[3];
};
#endif

//***************************************************************************
class CFaceStore
{

public:
		CFaceStore()			{MaxStrip = 3;TexGrab=&FaceStoreTexGrab;}
		CFaceStore(int Max)		{MaxStrip=Max;TexGrab=&FaceStoreTexGrab;}
		~CFaceStore(){};

		CFace					&AddFace(vector<Vector3> const &P, const sGinTri &T, const sUVTri &uv,GString const &Tex,int ID=0,bool ProcessTexFlag=false);
		CFace					&AddFace(CFace &F,bool TexFlag=true);
		void					AddFaces(vector<CFace>&Faces,bool TexFlag=true);
		void					AddFaces(CFaceStore &Faces,bool TexFlag=true);

		CFace					&AddFace(sTriFace &Face,int ID=0);

		void					SetTexGrab(CTexGrab &NewGrab)						{TexGrab=&NewGrab;}
		int						AddTex(GString const &TexName);

		void					SetTexOut(GString &Name,int TPBase,int TPW,int TPH)	{TexGrab->SetOutFile(Name); TexGrab->SetTPage(TPBase,TPW,TPH);}
		void					SetTexInclude(GString &Name)						{TexGrab->SetIncFile(Name);}
		void					SetTexDebug(bool f)									{TexGrab->SetDebug(f);}
		void					SetTexDebugOut(GString &Name)						{TexGrab->SetDebugOut(Name);}
		void					SetTexShrinkToFit(bool f)							{TexGrab->ShrinkToFit(f);}
		void					SetTexNoSort()										{TexGrab->NoSort();}
		void					SetTexAnimatedHeadersOnly(bool f)					{TexGrab->AnimatedHeadersOnly(f);}
		void					SetTexDontOutputBoxes(bool f)						{TexGrab->DontOutputBoxes(f);}
		void					SetTexAllowRotate(bool f)							{TexGrab->AllowRotate(f);}

		CTexGrab				&GetTexGrab()						{return(FaceStoreTexGrab);}
		vector<sTexOutInfo>		&GetTexInfo()						{return(TexGrab->GetTexInfo());}

		void					ProcessTextures();
		void					Process();

		int						WriteTriList(FILE *File)						{return(WriteTriList(File,OutTriList));}
		int						WriteQuadList(FILE *File)						{return(WriteQuadList(File,OutQuadList));}
		int						WriteVtxList(FILE *File)						{return(WriteVtxList(File,OutVtxList));}

		int						WriteTriList(FILE *File,vector<sTri> &List);
		int						WriteQuadList(FILE *File,vector<sQuad> &List);
		int						WriteVtxList(FILE *File,vector<sVtx> &List);
		
			
		vector<CFace> const		&GetFaceList()					{return(FaceList);}
		int						GetFaceCount()					{return(FaceList.size());}

		vector<CFace> const		&GetTriFaceList()				{return(TriFaceList);}
		int						GetTriFaceCount()				{return(TriFaceList.size());}

		vector<CFace> const		&GetQuadFaceList()				{return(QuadFaceList);}
		int						GetQuadFaceCount()				{return(QuadFaceList.size());}

		vector<sVtx> const		&GetVtxList()					{return(OutVtxList);}
		int						GetVtxCount()					{return(OutVtxList.size());}
		int						AddVtx(Vector3 &Vtx);

		void					setMaxStripLength(int v)		{MaxStrip = v;}

		CFace& operator []( int nIndex )	{return(FaceList[nIndex]);}

private:
		void					Quad();
		void					SetupUV(CFace const &In, sTri &Out);
		void					SetupUV(CFace const &In, sQuad &Out);

		void					BuildOutTriLists();
		void					BuildOutQuadList();

//		int						QuadGetAttached(int FaceNo);
//		void					QuadGetPnts(CFace &F,int *Join0,int *Join1,int *Pnt);
//		void					QuadGetUVs(CFace &F,int *Join0,int *Join1,int *Pnt);
//		void					OrderPnts( CFace &F ,int unc);

//		bool					CanConnect( int f0, int f1 );
//		int						CountFacesAttached ( int f );
//		void					FollowFace( int id, CFace &F );
//		int						GetUnconnectedPoint( int f0, int f1, int &f0p0, int &f0p1 );
//		bool					GetFace( CFace & F );

		int						MaxStrip;

		vector<CFace>			FaceList;
//		vector<sFaceTexList>	TexList;
		CTexGrab				FaceStoreTexGrab,*TexGrab;

		vector<CFace>			TriFaceList;
		vector<CFace>			QuadFaceList;

		vector<sTri>			OutTriList;
		vector<sQuad>			OutQuadList;
		vector<sVtx>			OutVtxList;

};

#endif