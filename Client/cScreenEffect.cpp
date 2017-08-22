#include "stdafx.h"
#include <io.h>
#include <stdio.h>
#include "Dragon.h"
#include "Hong_Sub.h"	
#include "Hong_Sprite.h"
#include "GameProc.h"
#include "cScreenEffect.h"


cEffectSpriteManager		g_EffectSpriteManager;
cParticleManager			g_ParticleManager;

// Extern Define ____________________________________________
extern int					Mapx, Mapy;

extern int  convert565to555_LoadSprite(Spr *sp);
extern int GetSysInfo(int type);
extern BOOL FileReadData(char *buf, FILE *fp);

// Extern Define End_________________________________________


void DrawParticleEffect()	// LTS DRAGON
{
	g_ParticleManager.Draw();
}

void CheckParticleEffect()
{
	g_ParticleManager.CheckContinueParticle();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
cFog::cFog(t_CommonArg* Arg)
{
	this->m_cSpriteIndex = Arg->FogArg.SpriteIndex;
	this->m_cFrameCount = 0;
	this->m_cFogType = 0;
	this->m_Location = Arg->FogArg.Location;
	this->StartTime = Arg->FogArg.StartTime;
	this->DelayTime = Arg->FogArg.DelayTime;
}

void cFog::GetData(t_CommonArg* Arg)
{
	return;
}

cParticle* cFog::Insert(cParticle* Arg)
{
	//this->m_cSpriteIndex	= Arg->FogArg.SpriteIndex;
	//this->m_cFrameCount		= 0;
	//this->m_cFogType		= 0;
	//this->m_Location		= Arg->FogArg.Location;
	//this->StartTime			= Arg->FogArg.StartTime;
	//	this->DelayTime			= Arg->FogArg.DelayTime;
	return NULL;
}

int cFog::Draw()
{
	if (this->StartTime <= g_curr_time)
	{
		Spr* Sprite = g_EffectSpriteManager.GetSpriteData(this->m_cSpriteIndex, this->m_cFrameCount);
		PutCompressedImageFX(this->m_Location.x - Mapx, this->m_Location.y - Mapy, Sprite, 15, 2);
	}
	return TRUE;
}

int cFog::CheckAndRemove()
{
	if (this->DelayTime <= g_curr_time) return TRUE;
	return FALSE;
}

cBreath::cBreath(t_CommonArg* Arg)
{
	this->m_cSpriteIndex = Arg->BreathArg.SpriteIndex;
	this->m_cFrameCount = 0;
	this->m_Start = Arg->BreathArg.StartLocation;
	this->m_Dest = Arg->BreathArg.DestLocation;
	this->m_cDirection = Arg->BreathArg.Direction;
	this->m_ucPower = Arg->BreathArg.Power;
	this->m_uiStartTime = Arg->BreathArg.StartTime;
}

void cBreath::GetData(t_CommonArg* Arg)
{
	Arg->BreathArg.SpriteIndex = this->m_cSpriteIndex;
	Arg->BreathArg.StartLocation = this->m_Start;
	Arg->BreathArg.DestLocation = this->m_Dest;
	Arg->BreathArg.Direction = this->m_cDirection;
	Arg->BreathArg.Power = this->m_ucPower;
	Arg->BreathArg.StartTime = this->m_uiStartTime;
}

void cBreath::GetArgData(cParticle* Arg, t_CommonArg* ReturnArg)
{
	Arg->GetData(ReturnArg);
}

extern LPCHARACTER FindHimBySprNo(int SprNo);		// LTS DRAGON MODIFY

cParticle* cBreath::Insert(cParticle* Arg)
{
	// 13,13,14,14,15,15,15,16,17,17,17,18,18,18,19,19,19,20,20,20,21,21,21,22,22,22    // 에니메이션 프레임
	int NpcDirection[26] = { 3, 3, 3, 3, 3, 3, 2, 2, 2, 2,
		2, 2, 3, 3, 3, 3, 3, 3, 2, 2,
		2, 2, 2, 2, 2, 2 };

	t_CommonArg tempArg;
	LPCHARACTER ch = FindHimBySprNo(50);	// 드래곤 찾음.
	if (!ch) return NULL;

	if (ch->nCurrentAction != 4) return NULL;

	GetArgData(Arg, &tempArg);
	tempArg.BreathArg.Direction = NpcDirection[ch->nCurrentFrame];	// 현재 용의 데이터에 따라 Direction을 보정한다.
	cBreath* tempBreath = new cBreath(&tempArg);

	return tempBreath;
}

int cBreath::Draw()
{
	POINTS		Progress[16] = {
								{0,2},{-1,2},{-2,2},{-2,1},
								{-2,0},{-2,-1},{-2,-2},{-1,-2},
								{0,-2},{1,-2},{2,-2},{2,-1},
								{2,0},{2,1},{2,2},{1,2} };

	if (this->m_ucPower > 0)
	{
		//if (this->m_uiStartTime>g_ClientTime)
		//{
		Spr* Sprite = g_EffectSpriteManager.GetSpriteData(this->m_cSpriteIndex, this->m_cFrameCount);
		if (!Sprite)
		{
			int i = 0;
			i++;
		}
		PutCompressedImageFX(this->m_Start.x - Mapx, this->m_Start.y - Mapy, Sprite, 12, 2);
		this->m_Start.x += Progress[this->m_cDirection].x;
		//this->m_Start.y+=Progress[this->m_cDirection].y;

		this->m_ucPower -= 1;
		//}
	}
	//AddCurrentStatusMessage(255,255,0,"현재시각 : %d",g_ClientTime);
	return TRUE;

}

int cBreath::CheckAndRemove()
{
	if (this->m_ucPower == 0) return TRUE;
	return FALSE;
}

cParticleManager::cParticleManager()
{	//< CSD-030306
	m_bDisplay = true;
}	//> CSD-030306

cParticleManager::~cParticleManager()
{
}

void cParticleManager::DeleteParticle()		// LTS DRAGON MODIFY
{
	for (tParticleListItor itor = ParticleList.begin(); itor != ParticleList.end();)
	{
		delete (*itor);
		itor = ParticleList.erase(itor);
	}
}

void cParticleManager::Insert(cParticle* Particle)
{
	ParticleList.push_back(Particle);
}

void cParticleManager::Draw()			// LTS DRAGON MODIFY
{
	if (!m_bDisplay)
	{	//< CSD-030306 : 포그 효과가 나오지 않음
		return;
	}	//> CSD-030306

	for (tParticleListItor itor = ParticleList.begin(); itor != ParticleList.end();)
	{
		(*itor)->Draw();
		if ((*itor)->CheckAndRemove())	// Remove Particle
		{
			delete (*itor);
			itor = ParticleList.erase(itor);
		}
		else itor++;
	}
}

void cParticleManager::SetContinueParticle(cParticle* Particle, int Count)
{
	this->m_pContinueParticle = Particle;
	this->m_iContinueparticleCount = Count;
}

void cParticleManager::CheckContinueParticle()	// LTS DRAGON MODIFY
{
	if (this->m_iContinueparticleCount > 0)
	{
		if (this->m_pContinueParticle != NULL)
		{
			cParticle* tempParticle = this->m_pContinueParticle->Insert(this->m_pContinueParticle);
			if (tempParticle) this->Insert(tempParticle);
		}
		this->m_iContinueparticleCount--;
	}
}

// Class cEffectSpriteManager 
//
// 이펙트 데이터를 관리 한다.
//

cEffectSpriteManager::cEffectSpriteManager()
{
	for (int i = 0; i < MAX_EFFECT_SPRITE; i++)	this->m_pSpriteData[i] = NULL;
}

cEffectSpriteManager::~cEffectSpriteManager()
{

}

int cEffectSpriteManager::LoadEFFile(int Index)
{
	FILE*	file;
	char	text_filename[FILENAME_MAX];
	char	bin_filename[FILENAME_MAX];
	char    text_Temp_file[FILENAME_MAX];
	char	animate_Name[FILENAME_MAX];
	int		tempInteger;

	sprintf(animate_Name, "./data/es%03d", Index);

	strcpy(text_filename, animate_Name);
	strcat(text_filename, ".ef");
	strcpy(bin_filename, animate_Name);
	strcat(bin_filename, ".eb");
	strcpy(text_Temp_file, animate_Name);
	strcat(text_Temp_file, ".txt");

	//	char  DataPath[FILENAME_MAX];

	if (GetSysInfo(0)) file = Fopen(text_filename, "rt");
	else file = NULL;

	if (file)
	{
		fscanf(file, "%d", &m_pEffectList[Index].CheckRepeat);	// 프레임 반복 0: 반복없이 마지막 프레임지속, 1:반복
		fscanf(file, "%d", &m_pEffectList[Index].MaxFrame);	// ef파일의 그림수
		fscanf(file, "%d\n", &m_pEffectList[Index].TotalFrame);	// 에니메이션 프레임 수

		if (m_pEffectList[Index].Frame != NULL)
		{
			MemFree(m_pEffectList[Index].Frame);
			m_pEffectList[Index].Frame = NULL;
		}
		MemAlloc(m_pEffectList[Index].Frame, sizeof(unsigned char)*m_pEffectList[Index].TotalFrame);

		if (m_pEffectList[Index].Frame == NULL)
		{
			MemFree(m_pEffectList[Index].Frame);
			return FALSE;
		}

		for (int i = 0; i < m_pEffectList[Index].TotalFrame; i++)
		{
			if (fscanf(file, "%d", &tempInteger) == EOF)
			{
				MemFree(m_pEffectList[Index].Frame);
				return FALSE;
			}
			m_pEffectList[Index].Frame[i] = tempInteger;
		}
		fclose(file);

		file = Fopen(bin_filename, "wb");

		if (file)
		{
			fwrite(&m_pEffectList[Index].CheckRepeat, 1, sizeof(unsigned char), file);
			fwrite(&m_pEffectList[Index].MaxFrame, 1, sizeof(unsigned char), file);
			fwrite(&m_pEffectList[Index].TotalFrame, 1, sizeof(unsigned char), file);
			fwrite(m_pEffectList[Index].Frame, m_pEffectList[Index].TotalFrame, sizeof(unsigned char), file);
			fclose(file);
		}
	}
	else
	{
		file = Fopen(bin_filename, "rb");

		if (file == NULL)
		{
			return FALSE;
		}

		fread(&m_pEffectList[Index].CheckRepeat, sizeof(unsigned char), 1, file);
		fread(&m_pEffectList[Index].MaxFrame, sizeof(unsigned char), 1, file);
		fread(&m_pEffectList[Index].TotalFrame, sizeof(unsigned char), 1, file);

		if (m_pEffectList[Index].Frame != NULL)
		{
			MemFree(m_pEffectList[Index].Frame);
			m_pEffectList[Index].Frame = NULL;
		}

		MemAlloc(m_pEffectList[Index].Frame, sizeof(unsigned char)*m_pEffectList[Index].TotalFrame);
		if (m_pEffectList[Index].Frame == NULL)
		{
			MemFree(m_pEffectList[Index].Frame);
			fclose(file);
			return FALSE;
		}

		fread(m_pEffectList[Index].Frame, m_pEffectList[Index].TotalFrame, sizeof(unsigned char), file);
		//if (fread(m_pEffectList[Index].Frame,m_pEffectList[Index].MaxFrame,sizeof(unsigned char),file))
		//{
		//	MemFree(m_pEffectList[Index].Frame);
		//	return FALSE;
		//}

		fclose(file);
	}
	return TRUE;
}

int cEffectSpriteManager::LoadSprite(int Index, int FrameCount)
{
	FILE *fp;
	short no;
	DWORD sprfilelength, size;
	DWORD nOff = 0;
	int   j;
	Spr*  s;
	int length;
	char strFileName[MAX_PATH];

	sprintf(strFileName, "./data/Es%03d.dat", Index);

	fp = Fopen(strFileName, "rb");
	if (fp == NULL)
	{
		return FALSE;
	}

	fread(&no, 2, 1, fp);
	fseek(fp, no * 4, SEEK_CUR);
	length = sprfilelength = _filelength(_fileno(fp)) - no * 4 * 2 - 2;

	MemAlloc(m_pSpriteData[Index], sprfilelength);
	for (j = 0; j < FrameCount; j++)
	{
		s = &m_Sprite[Index][j];
		fread(&size, 4, 1, fp);
		if (size == 0)
		{
			s->xl = 0;
			s->yl = 0;
			s->size = 0;
			s->img = NULL;
			continue;
		}
		fread(&(s->xl), sizeof(short), 1, fp);
		fread(&(s->yl), sizeof(short), 1, fp);
		fread(&(s->ox), sizeof(short), 1, fp);
		fread(&(s->oy), sizeof(short), 1, fp);
		fread(&(s->size), sizeof(unsigned int), 1, fp);

		if (nOff + s->size >= sprfilelength)
		{
			JustMsg("%s File Error !!", strFileName);
			ExitApplication(EA_LOAD_SPRITE_INFO);
		}
		fread(m_pSpriteData[Index] + nOff, s->size, 1, fp);
		s->img = m_pSpriteData[Index] + nOff;

		convert565to555_LoadSprite(s);

		nOff += size;
	}
	fclose(fp);
	return TRUE;
}

void cEffectSpriteManager::CheckAndLoadSpriteData(int Index)
{
	if (Index < 0 || Index >= MAX_SPRITE_TYPE) return;

	if (m_pSpriteData[Index] == NULL)
	{
		if (!this->LoadEFFile(Index)) return;
		this->LoadSprite(Index, m_pEffectList[Index].MaxFrame);
	}
}

Spr* cEffectSpriteManager::GetSpriteData(int Index, unsigned char& Frame)		// LTS DRAGON MODIFY
{
	if (Index < 0 || Index >= MAX_SPRITE_TYPE) return NULL;
	Spr* ReturnSpr;

	if (m_pEffectList[Index].CheckRepeat)	// 반복함.
	{
		if ((m_pEffectList[Index].TotalFrame - 1) <= Frame) Frame = 0;
		else Frame++;
		ReturnSpr = &m_Sprite[Index][m_pEffectList[Index].Frame[Frame]];
	}
	else									//반복하지 않음. 마지막 프레임 찍음
	{
		if ((m_pEffectList[Index].TotalFrame - 1) != Frame) Frame++;
		ReturnSpr = &m_Sprite[Index][m_pEffectList[Index].Frame[Frame]];
	}

	return ReturnSpr;
}
