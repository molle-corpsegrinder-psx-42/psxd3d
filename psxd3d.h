/*
 *------------------------------------------------------------------------
 *      Object: psxd3d.h
 *   $Revision: 1.183 $
 *        Date: 2014
 *       Notes: Deklarationen fuer die psx-engine
 *------------------------------------------------------------------------
 *
 * Copyright (c) 1997-2014 Michael Molkenthin. All Rights Reserved.
 *
 * Proprietary & Confidential
 *
 * The entire sourcecode is copyrighted under the German copyright laws. 
 * The owner of the copyright is Michael Molkenthin
 * 
 * Alle einfachen oder ausschliesslichen Nutzungs- und Verwertungsrechte 
 * verbleiben allein bei Michael Molkenthin, Lerchenstr. 98, 22767 Hamburg
 *
 * Eine Modifikation oder Bearbeitung des Quellcodes durch Dritte begruendet 
 * ausdruecklich keinerlei Miturheberschaft.
 *
 * Redistribution of the sourcecode without prior permission of Michael Molkenthin
 * are strictly prohibited.

 * This source code and the algorithms implemented therein constitute
 * confidential information and may comprise trade secrets of M. Molkenthin
 * or its associates, and any use thereof is subject to the terms and
 * conditions of the Non-Disclosure Agreement pursuant to which this
 * source code was originally received.
 * 
 * Modified copies of the source code must retain the above copyright
 * notice and the following disclaimer.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */
//
//----------------------------------------------------------------------------
// Structures
//----------------------------------------------------------------------------

#pragma once 
//pragma warning(disable: 4305) 
//// dass double nach float abgekürzt wird, lässt sich nicht vermeiden
#ifndef PSXD3D_H
#define PSXD3D_H

using namespace DirectX;

#include <ode/ode.h>
#include "mapobj.h"
#include "psxtypes.h"

typedef struct _matrix3x3 {
   float _11,_12,_13, _21,_22,_23, _31,_32,_33;
}MATRIX3X3;

struct SHOOTPOS
{
	XMFLOAT4 topLeft;
	XMFLOAT4 nextPos;
	//XMFLOAT2 dimensions;
	//float opacity;
};

struct SPRITE
{
	XMFLOAT4 Pos;
	XMFLOAT2 dimension;
//	XMFLOAT2 Tex;
	float opacity;
//	float fill;
};

struct EXPLOSIONPOS
{
	XMFLOAT4 topLeft;
	//XMFLOAT2 dimensions;
	//float opacity;
};


/*
struct cbChangeEveryFrame
{
	XMMATRIX mView;
	XMFLOAT4 vCamera;
	
	DirectionalLight DirLight[2];
	PointLight _PointLight;
    SpotLight _SpotLight;
	//XMFLOAT4 vLightDir[2];
	//XMFLOAT4 vLightColor[2];	
};

struct cbChangeOnResize
{
	XMMATRIX mProjection;
};
struct cbChangeEveryObject
{
	XMMATRIX mWorld;
	XMMATRIX matWorldViewProj;
	//XMFLOAT4 vOutputColor;
	Material d3dMaterial;

};
*/

HRESULT PSXStrCat(int , char* , size_t , const char* );
HRESULT PSXStrCpy(int , char* , size_t , const char* );
HRESULT PSXStrCat2(int , wchar_t* , size_t , const wchar_t* );
HRESULT PSXStrCpy2(int , wchar_t* , size_t , const wchar_t* );


// -- Schneller als pow(..,2)
//
inline float pow2(float a)
{
   return a * a;
} 


typedef enum c_shootdrawer {
	SHOOTDRAWER_REDLASER = 0,
	SHOOTDRAWER_BLUELASER,
	SHOOTDRAWER_REDTORPEDO,
	SHOOTDRAWER_NOBILLBOARD,
	SHOOTDRAWER_LASTENUM
} IDXSHOOTDRAWER;

typedef struct _spawnarray {
	PSXVECTOR vPos;
	IDXSHOOTDRAWER idxShootDrawer;
	float fSpeed;
} PSXLASERSPAWN;

// --- bonus /Equipment verwaltung
//
typedef enum _powupid {
     EQUIPMENT_LASER=0,
     EQUIPMENT_MININGLASER,
     EQUIPMENT_GATLINGLASER1,
     EQUIPMENT_GATLINGLASER2,
     EQUIPMENT_BATTLELASER,
     EQUIPMENT_MILITARYLASER,
     EQUIPMENT_SUPERLASER,
     EQUIPMENT_TORPEDO,
     EQUIPMENT_MISSILE,
     EQUIPMENT_MINE,

     EQUIPMENT_LARGECARGOBAY,
     EQUIPMENT_SHIELDGENERATOR,
     EQUIPMENT_FUELSCOOPS,
     EQUIPMENT_ESCAPECAPSULE,
     EQUIPMENT_EXTRAENERGYUNIT,
     EQUIPMENT_GALACTICHYPERDRIVE,
     EQUIPMENT_WINGMAN,
     EQUIPMENT_LASTENUM
} EQUIPMENTID ;

#include "psxtxt.h"
#include "psxres.h"
#include "psxedraw.h"

class EQUIPMENT {
   public:
	   /*
      COMMGOODID m_commgoodid;
      */
      bool    m_bIsAWeapon;
      char*   m_cName;
      int     m_maxTimeToLive;
      float   m_fTemperature;
      int     m_maxShootDelay;
      int     m_cntShootDelay;
      float   m_fSpeed;
      int     m_cntSpawn;
      IDXSHOOTDRAWER m_idxShootDrawer;
      XMVECTOR m_vSpawn[4];
      float   m_fShieldDamage;
      /*
      SNDRES  m_sndresShoot; */

      bool          m_bSingleton;
      int           m_idxShootSingleton;
      /*
      D3DMATRIX     m_matSingleton;
      D3DCOLORVALUE m_d3dcolShoot;
      */

   bool  isEnabled();
   bool  isOperable();
   float getShootRangePow2();
   bool insertShoot();
   bool init();
   bool update();
};




// -- Verwaltung von grossen 3D-Resourcen die sich aus mehreren zusammensetzen
//
class BIGRES {
   public:
      float m_fScale;
      float m_fRad;

      int   m_idxBigResItem;
      int   m_idxBigResItemGuts;
      bool  m_bHaveGuts;
      bool  m_bTextured;
      bool  m_bTranslatePivot;
      int   m_cntBigResItem;
      PSXVECTOR m_vMin;
      PSXVECTOR m_vMax;

   bool initBase();
//   bool attachEnemies(ENEMY*);
/*
   inline int getStartItemIndex()
   {
      int l_idxBigResItem;
      if (m_bEntered && m_bHaveGuts)
      {
         l_idxBigResItem = m_idxBigResItemGuts;
      }else{
         l_idxBigResItem = m_idxBigResItem;
      }
      return l_idxBigResItem;
   }
*/
};
typedef enum _enbigres {
   BIGRES_NONE = 0,
   BIGRES_FLAGSHIP,
   BIGRES_CORIOLIS,
   BIGRES_DODEK,
   BIGRES_COBRA,
   BIGRES_CARGO03,
   BIGRES_CARGO08,
   BIGRES_LASTENUM
} IDXBIGRES;

class BIGRESITEM {
   public:
      RESTYPE      m_resPart;
      TEXTUREID    m_tidColor;
      TEXTUREID    m_tidEmissive;
      PSXVECTOR    m_vPos;
      PSXFACEINFO* m_faceinfArray;
      EQUIPMENT*   m_pEquipment; 
      float        m_fViewRotation2;
      float        m_fScale;
   bool isEnabled();
   bool updateEnemy(MAPOBJECT*, int, dMass*);

};
#include "enemy.h"

typedef enum c_menace_type { MENACE_NONE  = 0,
                             MENACE_SELF,
                             MENACE_ENEMY,
                             MENACE_BOTH } MENACETYPE;

class SHOOT {
  public:
    XMVECTOR m_vPos;
    XMVECTOR m_vStep;
    int m_cntTimeToLive;
    int m_maxTimeToLive;
    MENACETYPE menacetype; // fuer wen ist der schuss bedrohlich
    EQUIPMENT* m_pEquipment;
    IDXSHOOTDRAWER m_idxShootDrawer;   // aussehen
    float     m_fViewRotation1;
    float     m_fScale;
    float     m_fTrailFactor;

    ENEMY** m_penemyHitableArray;
    int  m_cntHitableEnemies;

    bool Update();
    bool Die();
    bool determineTrail();
    bool selectHitableEnemies();
    bool insertImpact();
};

#include "psxtxv.h"

typedef struct _resourceassignment {
   IDXBIGRES  idxBigRes;
   bool         bGutsOnly;
   float        fViewRotation2;
   float        fScale;
   RESTYPE      resPart;
   PSXVECTOR    vPos;
} RESASSIGN;

typedef struct _resourceassignment2 {
   RESTYPE      resPart;
   IDXENEMYDRAWER idxEnemyDrawer;
   TEXTUREID    tidColor;
   TEXTUREID    tidEmissive;
   TEXTUREID    tidBump;
   TEXTUREID    tidEnv;
} TEXTUREASSIGN;


HRESULT PSXPrintf(int l_idxLine, char* pszDest, size_t cchDest, const char* pszFormat, ...  );

HRESULT PSXPrintf2(int l_idxLine, 
                   wchar_t* pszDest, 
                   size_t cchDest, 
                   const wchar_t* pszFormat, ...  );

DWORD WriteLogFile(char* pLogText);
DWORD WriteLogFile2(wchar_t* pLogText);

int PSXAllocateShoot(int, SHOOT**);

typedef enum _psxrenderpass { RENDERPASS_FULLSCREEN,
                              RENDERPASS_BACKWARD,
                              RENDERPASS_BLOOMINPUT,
                              RENDERPASS_BLOOMRESULT} RENDERPASS;

bool PSXRunFullScreen();
HRESULT PSXInitDirect3D();
void CleanupDevice();
bool PSXRender(RENDERPASS);
bool PSXDoRenderStuffBackground(RENDERPASS);
bool PSXDoRenderStuffScene(RENDERPASS);
bool PSXDoRenderStuff2D(RENDERPASS);
bool PSXDoRenderStuffBloom2D(RENDERPASS);

void hr2message(int ,HRESULT , char* );


bool psx_create_vertex_buffer(char* , ID3D11Buffer** , PSXVERTEX* , int );

bool PSXReleaseAllTextures();
bool PSXInitTextureArray();
size_t PSXBitsPerPixel( _In_ DXGI_FORMAT fmt );
void PSXGetSurfaceInfo( _In_ size_t width,
                            _In_ size_t height,
                            _In_ DXGI_FORMAT fmt,
                            _Out_opt_ size_t* outNumBytes,
                            _Out_opt_ size_t* outRowBytes,
                            _Out_opt_ size_t* outNumRows );

bool PSXInitDrawers();

void PSXErrorHandlerForODE (int , const char *, va_list );
void PSXMessageHandlerForODE (int , const char *, va_list );
void PSXDebugHandlerForODE (int , const char *, va_list );

bool PSXMatrixFromVector(XMMATRIX* , XMVECTOR);
bool PSXFaceIntersection(XMVECTOR, XMVECTOR, float, CXMMATRIX, RESTYPE, PSXFACEINFO*, PSXFACEINFO**, bool*, float*, PSXFACEINFO** );


#include "psxgame.h"
#include "psxlevel.h"

class PSXTIMER {
public:
	const static int m_maxSample = 1000;
   static int m_idxSample;
   static int m_cntSample;
	static DWORD m_dwSample[m_maxSample];
};


#include "player.h"
#endif
