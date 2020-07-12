/*
 *------------------------------------------------------------------------
 *      Object: ENEMYDRAWER
 *         $Id: psxedraw.h,v 1.6 2010/02/04 15:40:00 molle Exp $
 *       Notes: ENEMYDRAWER Basis-Klasse und deren Derivate
 *------------------------------------------------------------------------
 * 
 * Copyright (c) 1997-2005 Michael Molkenthin. All Rights Reserved.
 *
 * Proprietary & Confidential
 *
 * The entire sourcecode is copyrighted under the German copyright laws. 
 * The owner of the copyright is Michael Molkenthin
 * 
 * Alle einfachen oder ausschliesslichen Nutzungs- und Verwertungsrechte 
 * verbleiben allein bei Michael Molkenthin, Lerchenstrasse 98, 22767 Hamburg
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

// -- schubladen fuer jede unterschiedliche permutation
//    texturing/mesh-resource
//
typedef enum _enemydrawindex 
{
   EDRAW_EMPTY = 0,
   EDRAW_EQUIPMENT,
   EDRAW_BRENNSTAB,
   EDRAW_GHOST,
/*
   EDRAW_FELDGENERATOR,
   EDRAW_FELDGENERATOR_IR,
   EDRAW_FLYER_RED,
   EDRAW_FLYER_RED_EMISSIVE,
   EDRAW_LEADER_RED,
   EDRAW_LEADER_RED_EMISSIVE,
   EDRAW_FLYER_GREEN,
   EDRAW_FLYER_GREEN_EMISSIVE,
   EDRAW_LEADER_GREEN,
   EDRAW_LEADER_GREEN_EMISSIVE,
   EDRAW_HUNTER02,
   EDRAW_HUNTER03,
   EDRAW_COBRAMK3,
   EDRAW_HANGAR,
   EDRAW_HANGAR_EMISSIVE,
   EDRAW_GUN,
*/
   EDRAW_MINE,
   EDRAW_MINE_EMISSIVE,
   EDRAW_MINE_LOWPOLY,
   EDRAW_MINE_LOWPOLY2,
   EDRAW_ASTEROID,
   EDRAW_ASTEROID_EMISSIVE,
   EDRAW_ASTEROID_BUMP,
/*
   EDRAW_DOOR01A,
   EDRAW_DOOR01B,
   EDRAW_DOOR01C,
   EDRAW_DOOR02A,               // ERGAENZUNGEN AUCH IM EnemyDrawerName-Array
   EDRAW_DOOR02B,
*/
   EDRAW_EXPLOSION, //<--- g_cntEnemyDrawerSolids soll genau hier hin zeigen
   EDRAW_HOTPLASMA,
   EDRAW_IMPACT,
   EDRAW_SHIELDFLASH,
   EDRAW_FRONTSHIELD,
   EDRAW_TRACTORSPHERE,
   EDRAW_POINTER01,
   EDRAW_TMPSPEEDUP,
   EDRAW_CHAR,
   EDRAW_RES_STANDARD,
   EDRAW_RES_EMISSIVE,
   EDRAW_RES_BUMP,
   EDRAW_STARBASEHANGAR,
   EDRAW_PLANET,
   EDRAW_LASTENUM
} IDXENEMYDRAWER;

namespace ENEMYDRAWERCONSTANTBUFFER
{
	//***************************************************************************************
	// LightHelper.h by Frank Luna (C) 2011 All Rights Reserved.
	//
	// Helper classes for lighting.
	//***************************************************************************************

	// Note: Make sure structure alignment agrees with HLSL structure padding rules. 
	//   Elements are packed into 4D vectors with the restriction that an element
	//   cannot straddle a 4D vector boundary.

	struct DirectionalLight
	{
		DirectionalLight() { ZeroMemory(this, sizeof(this)); }

		XMFLOAT4 Ambient;
		XMFLOAT4 Diffuse;
		XMFLOAT4 Specular;
		XMFLOAT3 Direction;
		float Pad; // Pad the last float so we can set an array of lights if we wanted.
	};

	struct PointLight
	{
		PointLight() { ZeroMemory(this, sizeof(this)); }

		XMFLOAT4 Ambient;
		XMFLOAT4 Diffuse;
		XMFLOAT4 Specular;

		// Packed into 4D vector: (Position, Range)
		XMFLOAT3 Position;
		float Range;

		// Packed into 4D vector: (A0, A1, A2, Pad)
		XMFLOAT3 Att;
		float Pad; // Pad the last float so we can set an array of lights if we wanted.
	};

	struct SpotLight
	{
		SpotLight() { ZeroMemory(this, sizeof(this)); }

		XMFLOAT4 Ambient;
		XMFLOAT4 Diffuse;
		XMFLOAT4 Specular;

		// Packed into 4D vector: (Position, Range)
		XMFLOAT3 Position;
		float Range;

		// Packed into 4D vector: (Direction, Spot)
		XMFLOAT3 Direction;
		float Spot;

		// Packed into 4D vector: (Att, Pad)
		XMFLOAT3 Att;
		float Pad; // Pad the last float so we can set an array of lights if we wanted.
	};

	struct Material
	{
		Material() { ZeroMemory(this, sizeof(this)); }

		XMFLOAT4 Ambient;
		XMFLOAT4 Diffuse;
		XMFLOAT4 Specular; // w = SpecPower
		XMFLOAT4 Reflect;
	};
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
		float    m_fScalePos;
		float    m_fScaleBillboard;
		float    m_fFade;
		//XMFLOAT4 vOutputColor;
		Material d3dMaterial;
			
		XMFLOAT3 SpotLightPositionObjRelative;			
		XMFLOAT3 SpotLightDirectionObjRelative;

	};
}
// -- Basis Klasse zum Zeichnen des Enemyarray um Funktionen der alten herkömmlichen
//    fixed-pipeline abzubilden
//
class ENEMYDRAWER {
protected:
	virtual bool setOutputStream(RESTYPE);
public:    

	static ID3D11Buffer*           m_pcbChangeEveryFrame ;
	static ID3D11Buffer*           m_pcbChangeOnResize ;
	static ID3D11Buffer*           m_pcbChangeEveryObject ;
	static ID3D11SamplerState*     m_pSamplerLinear;

	virtual bool prepareItem(FXMVECTOR , XMMATRIX* , float );

	virtual bool prepareHeader();
	virtual bool setPixelShader();

	virtual bool setTextures(RESTYPE);	
	virtual bool Draw(RESTYPE);

	//virtual bool AssignTextures();

	//TEXTUREID m_TextureID[5], m_EmissiveID[5], m_BumpID[5], m_EnvID[5]; 
	//TEXTUREID m_arrStages[5];
	//int m_cntStages, m_cntMatTexture;
	//bool m_bTextured, m_bEmissive, m_bBump;

	RESTYPE m_ResType;
	int m_idxFirst, m_cntDraw;
};

class SIMPLEDRAWER : public ENEMYDRAWER {
public:
	virtual bool setPixelShader();
};

class BUMPDRAWER : public ENEMYDRAWER {
public:
	virtual bool setPixelShader();
	virtual bool prepareItem(FXMVECTOR , XMMATRIX* , float );
};



