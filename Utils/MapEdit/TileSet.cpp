/*********************/
/*** TileSet Stuph ***/
/*********************/

#include	"stdafx.h"
#include	<Vector3.h>
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	"GLEnabledView.h"
#include	<Vector>

#include	<GFName.hpp>

#include	"Core.h"
#include	"TileSet.h"
#include	"GinTex.h"
#include	"utils.h"

#include	"MapEdit.h"
#include	"MapEditDoc.h"
#include	"MapEditView.h"
#include	"MainFrm.h"
#include	"LayerTileGui.h"

/*****************************************************************************/
/*** TileBank ****************************************************************/
/*****************************************************************************/

const float	TileBrowserGap=0.2f;
const float	TileBrowserX0=0-TileBrowserGap/2;
const float	TileBrowserX1=1+TileBrowserGap/2;
const float	TileBrowserY0=0-TileBrowserGap/2;
const float	TileBrowserY1=1+TileBrowserGap/2;

/*****************************************************************************/
CTileBank::CTileBank()
{
		LoadFlag=false;
		CurrentSet=0; LastSet=0;
		for (int i=0; i<MaxBrush; i++) Brush[i].Delete();
		LastCursorPos=CursorPos=-1;
		ActiveBrush=0;
		SelStart=-1;
		SelEnd=-1;
		VisibleFlag=true;
}

/*****************************************************************************/
CTileBank::~CTileBank()
{
}

/*****************************************************************************/
void	CTileBank::Load(CFile *File,int Version)
{
		if (Version<FileVersion)
		{
			int		ListSize;
			GFName	RootPath=File->GetFilePath();
			GString	FilePath;
			char	FixPath[1024];

			FilePath=RootPath.Drive();
			FilePath+=RootPath.Dir();
			FilePath.Append('\\');
			FilePath.Upper();
		
			File->Read(&ListSize,sizeof(int));
			File->Read(&CurrentSet,sizeof(int));
			File->Read(&ActiveBrush,sizeof(int));
			Brush[0].Load(File,Version);
			Brush[1].Load(File,Version);
			if (Version<2)
			{
				CurrentSet++;
			}

	// New Style rel storage
			for (int i=0;i<ListSize;i++)
			{
				char	c=1;
				GString	FullName;

				while (c)
				{
					File->Read(&c,1);
					FullName.Append(c);
				}
				FullName.Upper();
				GFName::makeabsolute(FilePath,FullName,FixPath);
				FullName=FixPath;
				_fullpath( FixPath, FullName, 1024);
				for (int z=0; z<strlen(FixPath); z++) 
				{// Invalidate any long name short cackness
					if (FixPath[z]=='~') FixPath[z]='_'; 
				}
				FullName=FixPath;

				CheckFilename(FullName);
				FullName.Upper();
				AddSet(FullName);
			}

		}
		else
		{
		File->Read(&LayerCam,sizeof(Vector3));
		File->Read(&CurrentSet,sizeof(int));
		File->Read(&ActiveBrush,sizeof(int));
		Brush[0].Load(File,Version);
		Brush[1].Load(File,Version);

		CElemBank::Load(File,Version);
		}


}

/*****************************************************************************/
void	CTileBank::Save(CFile *File)
{
int		ListSize=GetSetCount();
GFName	RootPath=File->GetFilePath();
GString	SavePath;

		SavePath=RootPath.Drive();
		SavePath+=RootPath.Dir();
		SavePath.Append('\\');

		SavePath.Upper();

		File->Write(&LayerCam,sizeof(Vector3));
		File->Write(&CurrentSet,sizeof(int));
		File->Write(&ActiveBrush,sizeof(int));
		Brush[0].Save(File);
		Brush[1].Save(File);

		CElemBank::Save(File);
}

/*****************************************************************************/
void	CTileBank::RenderTile(int Set,int Elem,int Flags,bool Is3d)
{
		if (IsValid(Set,Elem))
		{
			SetList[Set].RenderElem(Elem,Flags,Is3d);
		}
		else
		{
			SetList[0].RenderInvalid();
		}
		
}
/*****************************************************************************/
void	CTileBank::DeleteCurrent()
{
int		ListSize=GetSetCount();
// Remap Brushes
		for (int i=0; i<MaxBrush; i++) 
		{
			Brush[i].RemoveSet(CurrentSet);
		}
		for (int Set=CurrentSet; Set<ListSize; Set++)
		{
			for (int i=0; i<MaxBrush; i++) 
			{
				Brush[i].RemapSet(Set,Set);
			}
		}
		SetList.erase(CurrentSet);
		if (CurrentSet) CurrentSet--;
}

/*****************************************************************************/
CPoint	CTileBank::GetTilePos(int ID,int Width)
{
	if (ID==0)
		return(CPoint(-1,-1));
	else
		return(IDToPoint(ID-1,Width));
}

/*****************************************************************************/
void	CTileBank::Render(CCore *Core,Vector3 &CamPos,bool Is3d)
{
		if (!GetSetCount()) return;
CElemSet	&ThisSet=SetList[CurrentSet];
int			ListSize=ThisSet.GetCount();
int			BrowserWidth=ThisSet.GetBrowserWidth();
int			TileID=0;
sMapElem	ThisElem;
int			SelFlag;
float		Scale=CamPos.z/(float)BrowserWidth/2.0;

		ThisElem.Flags=0;
		ThisElem.Set=CurrentSet;
		
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		while(TileID!=ListSize)
		{
			CPoint	Pos=GetTilePos(TileID,BrowserWidth);
			float	XPos=(float)Pos.x*(1+TileBrowserGap);
			float	YPos=(float)Pos.y*(1+TileBrowserGap);

			glLoadIdentity();
			glScalef(Scale,Scale,Scale);
			glTranslatef(-CamPos.x+XPos,CamPos.y-YPos,0);

			glColor3f(1,1,1);
			ThisSet.RenderElem(TileID,0,Is3d);
// Selection
			ThisElem.Tile=TileID;
			SelFlag=0;

			if (Brush[0].DoesContainTile(ThisElem)) SelFlag|=1;
			if (Brush[1].DoesContainTile(ThisElem)) SelFlag|=2;

			if (SelFlag)
			{
				glBegin(GL_QUADS); 
				switch(SelFlag)
				{
					case 1: // L
						glColor4f(1,0,0,0.5);
						BuildGLQuad(TileBrowserX0,TileBrowserX1,TileBrowserY0,TileBrowserY1,0.01f);
						break;
					case 2: // R
						glColor4f(0,0,1,0.5);
						BuildGLQuad(TileBrowserX0,TileBrowserX1,TileBrowserY0,TileBrowserY1,0.01f);
						break;
					case 3: // LR
						glColor4f(1,0,0,0.5);
						BuildGLQuad(TileBrowserX0,0.5,TileBrowserY0,TileBrowserY1,0.01f);
						glColor4f(0,0,1,0.5);
						BuildGLQuad(0.5,TileBrowserX1,TileBrowserY0,TileBrowserY1,0.01f);
						break;
				}
	
				glEnd();
			}
			TileID++;
		}
		glPopMatrix();
	
}

/*****************************************************************************/
void	CTileBank::RenderCursor(CCore *Core,Vector3 &CamPos,bool Is3d)
{
		if (!GetSetCount()) return;
CElemSet	&ThisSet=SetList[CurrentSet];
int			ListSize=ThisSet.GetCount();
int			BrowserWidth=ThisSet.GetBrowserWidth();
CPoint		Start,End;
int			MaxTile=ListSize;
float		Scale=CamPos.z/(float)BrowserWidth/2.0;

		if (CursorPos<-1 || CursorPos>ListSize) return;		
		if (!ListSize) return;


		if (SelStart==-1)
		{
			Start=GetTilePos(CursorPos,BrowserWidth);
			End=Start;
		}
		else
		{

			CPoint	S=IDToPoint(SelStart-1,BrowserWidth);
			CPoint	E=IDToPoint(SelEnd-1,BrowserWidth);
			
			Start=CPoint(	min(S.x,E.x), min(S.y,E.y));
			End=CPoint(		max(S.x,E.x), max(S.y,E.y));
			if (PointToID(End,BrowserWidth)>=MaxTile) return;		// Invalid selection
		}

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		for (int Y=Start.y; Y<=End.y; Y++)
		{
			for (int X=Start.x; X<=End.x; X++)
			{
				float	XPos=(float)X*(1+TileBrowserGap);
				float	YPos=(float)Y*(1+TileBrowserGap);

				glLoadIdentity();
				glScalef(Scale,Scale,Scale);
				glTranslatef(-CamPos.x+XPos,CamPos.y-YPos,0);

	
				glBegin(GL_QUADS); 
				glColor4f(1,1,0,0.5);
				BuildGLQuad(TileBrowserX0,TileBrowserX1,TileBrowserY0,TileBrowserY1,0);
				glEnd();

			}
		}
		glPopMatrix();

}

/*****************************************************************************/
void	CTileBank::RenderGrid(CCore *Core,Vector3 &CamPos,bool Active)
{
		if (!GetSetCount()) return;
CElemSet	&ThisSet=SetList[CurrentSet];
int			ListSize=ThisSet.GetCount();
int			BrowserWidth=ThisSet.GetBrowserWidth();
int			TileID=1;	// Dont bother with blank, its sorted
float		Scale=CamPos.z/(float)BrowserWidth/2.0;
		
		if (!ListSize) return;
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		while(TileID!=ListSize)
		{
			CPoint	Pos=GetTilePos(TileID,BrowserWidth);
			float	XPos=(float)Pos.x*(1+TileBrowserGap);
			float	YPos=(float)Pos.y*(1+TileBrowserGap);

			glLoadIdentity();
			glScalef(Scale,Scale,Scale);
			glTranslatef(-CamPos.x+XPos,CamPos.y-YPos,0);
			
			glBegin(GL_LINES); 
				glColor3f(1,1,1);
			
				glVertex3f( TileBrowserX0,TileBrowserY0,0);
				glVertex3f( TileBrowserX1,TileBrowserY0,0);

				glVertex3f( TileBrowserX0,TileBrowserY1,0);
				glVertex3f( TileBrowserX1,TileBrowserY1,0);

				glVertex3f( TileBrowserX0,TileBrowserY0,0);
				glVertex3f( TileBrowserX0,TileBrowserY1,0);

				glVertex3f( TileBrowserX1,TileBrowserY0,0);
				glVertex3f( TileBrowserX1,TileBrowserY1,0);

			glEnd();

			TileID++;
		}
		glPopMatrix();
}

/*****************************************************************************/
void	CTileBank::FindCursorPos(CCore *Core,Vector3 &CamPos,CPoint &MousePos)
{
		if (!GetSetCount()) return;
CElemSet	&ThisSet=SetList[CurrentSet];
int			ListSize=ThisSet.GetCount();
int			BrowserWidth=ThisSet.GetBrowserWidth();
GLint		Viewport[4];
GLuint		SelectBuffer[SELECT_BUFFER_SIZE];
int			HitCount;
int			TileID=0;
float		Scale=CamPos.z/(float)BrowserWidth/2.0;
		
		if (!ListSize) return;
		glGetIntegerv(GL_VIEWPORT, Viewport);
		glSelectBuffer (SELECT_BUFFER_SIZE, SelectBuffer );
		glRenderMode (GL_SELECT);

	    glInitNames();
		glPushName(-1);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluPickMatrix( MousePos.x ,(Viewport[3]-MousePos.y),5.0,5.0,Viewport);
		Core->GetView()->SetupPersMatrix();

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		while(TileID!=ListSize)
		{
			CPoint	Pos=GetTilePos(TileID,BrowserWidth);
			float	XPos=(float)Pos.x*(1+TileBrowserGap);
			float	YPos=(float)Pos.y*(1+TileBrowserGap);

			glLoadIdentity();
			glScalef(Scale,Scale,Scale);
			glTranslatef(-CamPos.x+XPos,CamPos.y-YPos,0);

			glLoadName (TileID);
			glBegin (GL_QUADS); 
				BuildGLQuad(TileBrowserX0,TileBrowserX1,TileBrowserY0,TileBrowserY1,0);
			glEnd();
			TileID++;
		}

		HitCount= glRenderMode (GL_RENDER);
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

// Process hits

GLuint	*HitPtr=SelectBuffer;

		TileID=-2;
		if (HitCount)	// Just take 1st		
		{
			TileID=HitPtr[3];
		}
		glMatrixMode(GL_MODELVIEW);	// <-- Prevent arse GL assert

		CursorPos=TileID;
		SelEnd=CursorPos;
}

/*****************************************************************************/
bool	CTileBank::LButtonControl(CCore *Core,UINT nFlags, CPoint &CursorPos,bool DownFlag)
{
		if (nFlags & MK_RBUTTON)
		{
			SelectCancel();
			return(false);
		}
		Select(LBrush,DownFlag);

		return(true);	
}

/*****************************************************************************/
bool	CTileBank::RButtonControl(CCore *Core,UINT nFlags, CPoint &CursorPos,bool DownFlag)
{
		if (nFlags & MK_LBUTTON)
		{
			SelectCancel();
			return(false);
		}

		Select(RBrush,DownFlag);

		return(false);	
}

/*****************************************************************************/
bool	CTileBank::MouseMove(CCore *Core,UINT nFlags, CPoint &CursorPos)
{
	return(false);
}

/*****************************************************************************/
bool	CTileBank::Command(int CmdMsg,CCore *Core,int Param0,int Param1)
{
		switch(CmdMsg)
		{
		case CmdMsg_SubViewLoad:
			LoadNewSet(Core);
			GUIUpdate(Core);
			break;
		case CmdMsg_SubViewDelete:
			DeleteSet(Core);
			GUIUpdate(Core);
			break;
		case CmdMsg_SubViewUpdate:
			ReloadAllSets();
			Core->GetTexCache().Purge();
			GUIUpdate(Core);
			break;
		case CmdMsg_SubViewSet:
			CurrentSet=TileBankGUI.m_List.GetCurSel();
			GUIUpdate(Core);
			break;
		case CmdMsg_ActiveBrushLeft:
			ActiveBrush=LBrush;
			break;
		case CmdMsg_ActiveBrushRight:
			ActiveBrush=RBrush;
		default:
			TRACE3("TileBank-Unhandled Command %i (%i,%i)\n",CmdMsg,Param0,Param1);
		}
		return(true);
}

/*****************************************************************************/
/*** Gui *********************************************************************/
/*****************************************************************************/
void	CTileBank::GUIInit(CCore *Core)
{
		Core->GUIAdd(TileBankGUI,IDD_LAYERTILE_GUI);
}

/*****************************************************************************/
void	CTileBank::GUIKill(CCore *Core)
{
		Core->GUIRemove(TileBankGUI,IDD_LAYERTILE_GUI);
}

/*****************************************************************************/
void	CTileBank::GUIUpdate(CCore *Core)
{
int			ListSize=GetSetCount();
bool		IsSubView=Core->IsSubView();

			if (TileBankGUI.m_List)
			{
				TileBankGUI.m_List.ResetContent();
				if (ListSize)
				{
					for (int i=0; i<ListSize; i++)
					{
						TileBankGUI.m_List.AddString(GetSetName(i));
					}
					TileBankGUI.m_List.SetCurSel(CurrentSet);
				}
				else
				{
					IsSubView=FALSE;
				}
				TileBankGUI.m_List.EnableWindow(IsSubView);
			}

}

/*****************************************************************************/
void	CTileBank::GUIChanged(CCore *Core)
{
}

/*****************************************************************************/
/*** Functions ***************************************************************/
/*****************************************************************************/
bool	CTileBank::Select(int BrushID,bool DownFlag)
{
		if (DownFlag && SelStart==-1)
		{
			if (CursorPos<0) return(FALSE);
			SelStart=CursorPos;
			TRACE1("SEL Start %i\n",CursorPos);

			if (CursorPos==0)
			{
				SetBrush(GetBrush(BrushID));
				SelStart=-1;
				TRACE0("Selected Blank\n");

			}
		}
		else
		if (!DownFlag && SelStart!=-1)
		{
			if (CursorPos==-1) return(SelectCancel());
	
			SetBrush(GetBrush(BrushID));

			SelStart=-1;
			TRACE1("END SEL %i\n",CursorPos);
		}

		return(TRUE);
}

/*****************************************************************************/
void	CTileBank::SetBrush(CMap &ThisBrush)
{
int			BW=SetList[CurrentSet].GetBrowserWidth();
CPoint		S=IDToPoint(SelStart-1,BW);
CPoint		E=IDToPoint(SelEnd-1,BW);

int			Width=abs(E.x-S.x)+1;
int			Height=abs(E.y-S.y)+1;

sMapElem	ThisElem;
int			MaxTile=SetList[CurrentSet].GetCount();

//		if (PointToID(End,BW)>=MaxTile) SelectCancel();	// Invalid selection

		ThisElem.Set=CurrentSet;
		ThisElem.Flags=0;
			
		ThisBrush.Delete();
		ThisBrush.SetSize(Width,Height);

		for (int Y=0; Y<Height; Y++)
		{
			for (int X=0; X<Width; X++)
			{
				ThisElem.Tile=SelStart+X+(Y*BW);
				if (!IsValid(CurrentSet,ThisElem.Tile))
				{
					TRACE2("Not valid %i %i\n",CurrentSet,ThisElem.Tile);
					ThisElem.Tile=-1;
				}
				ThisBrush.Set(X,Y,ThisElem,true);
			}
		}
}

/*****************************************************************************/
bool	CTileBank::SelectCancel()
{
		SelStart=-1;
		TRACE0("Select Cancelled\n");
		return(TRUE);
}

/*****************************************************************************/
void	CTileBank::DeleteSet(CCore *Core)
{
		if (Core->Question("Delete Current Tile Bank\n\nAll used tiles in current set will be set to blank\nAre you sure?"))
			{
			int		SetCount=GetSetCount();
			int		i,ListSize=Core->GetLayerCount();

				for (i=0;i<ListSize;i++)
				{
					CLayerTile	*ThisLayer=(CLayerTile*)Core->GetLayer(i);
					if (ThisLayer->GetType()==LAYER_TYPE_TILE)
					{
					ThisLayer->RemoveSet(CurrentSet);
					}
				}
				DeleteCurrent();

				for (int Set=CurrentSet+1; Set<SetCount; Set++)
				{
		
					for (i=0;i<ListSize;i++)
					{
						CLayerTile	*ThisLayer=(CLayerTile*)Core->GetLayer(i);
						if (ThisLayer->GetType()==LAYER_TYPE_TILE)
						{
						ThisLayer->RemapSet(Set,Set-1);
						}
					}
				}
			}
		CurrentSet--;
		GUIUpdate(Core);
}

