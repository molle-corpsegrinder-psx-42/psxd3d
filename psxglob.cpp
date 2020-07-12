/*
 *------------------------------------------------------------------------
 *      Object: psxglob.cpp
 *   $Revision: 1.203 $
 *        Date: 2004
 *       Notes: Globals, Constants, Arrays
 *------------------------------------------------------------------------
 *
 * Copyright (c) 1997-2004 Michael Molkenthin. All Rights Reserved.
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

#include "stdafx.h"
#include "d11init.h"

using namespace DirectX;

#include "psxd3d.h"

//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
HWND                    g_hWnd = nullptr;
D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11Device*           g_pd3dDevice = nullptr;
ID3D11Device1*          g_pd3dDevice1 = nullptr;
ID3D11DeviceContext*    g_pImmediateContext = nullptr;
ID3D11DeviceContext1*   g_pImmediateContext1 = nullptr;
IDXGISwapChain*         g_pSwapChain = nullptr;
ID3D11RenderTargetView* g_pRenderTargetView = nullptr;
ID3D11Texture2D*        g_pDepthStencil = nullptr;
ID3D11DepthStencilView* g_pDepthStencilView = nullptr;

ID3D11VertexShader*     g_pVertexShaderEnemyDrawer = nullptr;
ID3D11PixelShader*      g_pPixelShaderEnemyDrawer = nullptr;
ID3D11VertexShader*     g_pVertexShaderSimpleDrawer = nullptr;
ID3D11PixelShader*      g_pPixelShaderSimpleDrawer = nullptr;
ID3D11VertexShader*     g_pVertexShaderBumpDrawer = nullptr;
ID3D11PixelShader*      g_pPixelShaderBumpDrawer = nullptr;

ID3D11PixelShader*      g_pPixelShaderSolid = nullptr;
ID3D11InputLayout*      g_pVertexLayout = nullptr;

ID3D11VertexShader*     g_pVertexShaderSpriteDrawer = nullptr;
ID3D11PixelShader*      g_pPixelShaderSpriteDrawer = nullptr;
ID3D11GeometryShader*   g_pSpriteGeometryShader = nullptr;

ID3D11InputLayout*      g_pSpriteLayout = nullptr;
ID3D11Buffer*           g_pSpriteBuffer = nullptr;

ID3D11VertexShader*     g_pVertexShaderShootDrawer = nullptr;
ID3D11PixelShader*      g_pPixelShaderShootDrawer = nullptr;
ID3D11GeometryShader*   g_pShootGeometryShader = nullptr;
ID3D11InputLayout*      g_pShootPosLayout = nullptr;
ID3D11Buffer*           g_pShootPosBuffer = nullptr;

ID3D11VertexShader*     g_pVertexShaderExplosion = nullptr;
ID3D11PixelShader*      g_pPixelShaderExplosion = nullptr;
ID3D11GeometryShader*   g_pExplosionGeometryShader = nullptr;
ID3D11InputLayout*      g_pExplosionLayout = nullptr;
ID3D11Buffer*           g_pExplosionVB = nullptr;
int g_cntExplVert;
float g_fTempScaleExplosion = 0.0f;

ID3D11Buffer*           ENEMYDRAWER::m_pcbChangeEveryFrame = nullptr;
ID3D11Buffer*           ENEMYDRAWER::m_pcbChangeOnResize = nullptr;
ID3D11Buffer*           ENEMYDRAWER::m_pcbChangeEveryObject = nullptr;
ID3D11SamplerState*                 ENEMYDRAWER::m_pSamplerLinear = nullptr;

ID3D11BlendState* g_pd3dblendstateTransparent = nullptr;
ID3D11DepthStencilState *g_pd3ddepthstateTransparent = nullptr;

//ID3D11ShaderResourceView*           g_pTextureRV = nullptr;
//ID3D11SamplerState*                 g_pSamplerLinear = nullptr;

XMMATRIX                g_World;
XMMATRIX                g_View;
XMMATRIX                g_Projection;

XMMATRIX g_matCamera;
XMVECTOR g_vCamera;


XMMATRIX g_matPlayer;

XMMATRIX g_matSingleMovement;

#define MAX_LOADSTRING 100

// Globale Variablen:
HINSTANCE g_hInst =nullptr;								// Aktuelle Instanz
TCHAR szTitle[MAX_LOADSTRING];					// Titelleistentext
TCHAR szWindowClass[MAX_LOADSTRING];			// Klassenname des Hauptfensters

HANDLE g_hLogFile     = NULL;

GAME g_game;

DWORD g_dwLastTickCount;
float g_fElapsedTime;
int g_cntFrame;
DWORD PSXTIMER::m_dwSample[PSXTIMER::m_maxSample] = { 0 };
int PSXTIMER::m_cntSample = 0;
int PSXTIMER::m_idxSample = 0;


BIGRES g_bigresArray[BIGRES_LASTENUM];

// --- Teile des Flagschiffs, die jeweils eine eigene Meshresource sind
//
RESASSIGN g_resassignArray[] = {

/*
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_KOLLEKTORSTANGEN,        { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_KOLLEKTOR1,              { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_RUMPF_PLATTEN_ST,        { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_RUMPF_PLA1,              { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_BAUCH_ELEKTRONIK,        { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_HAUPTSTEGE_1,            { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_REAKTOR_01_A,            { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_RUMPF_PLA2,              { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_RUMPF_PLA3,              { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_ANTRIEB_1_B,             { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_RUMPF_PLA4,              { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_ANTRIEB_1_A,             { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_ANTRIEB_1_C,             { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_ZENTRALE_1_BASE_,        { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_RUMPF_02,                { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_RUMPF_04,                { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_RUMPF_PLA5,              { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_RUMPF_PLA6,              { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_RUMPF_06,                { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_RUMPF_PLA7,              { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_RUMPF_PLA8,              { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_RUMPF_01,                { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_RUMPF_03,                { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_RUMPF_PLA9,              { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_RUMPF_PL10,              { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_RUMPF_05,                { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_RUMPF_PL11,              { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_RUMPF_PL12,              { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_RUMPF_PL13,              { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_RUMPF_PL14,              { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_RUMPF_PL15,              { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_TUNNEL_FRONT,            { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_KOLLEKTORRING_1_,        { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_KOLLEKTOR2,              { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_KOLLEKTORRING_2_,        { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_KOLLEKTOR3,              { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_KOLLEKTORRING_3_,        { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_KOLLEKTOR4,              { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_ZENTRALE_1_D,            { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_ZENTRALE_1_C,            { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_ZENTRALE_1_B,            { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_ZENTRALE_1_A,            { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_TUNNEL_BACK,             { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_KOLLEKTORRING_4_,        { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_KOLLEKTOR5,              { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_SCHLAUCH,                { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_ZENTRALE_2_C,            { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_ZENTRALE_2,              { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_STUTZEN,                 { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_ZENTRALE_1_E,            { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_AUFBAUTEN,               { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_ZENTRALE_2_A,            { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_ZENTRALE_2_B,            { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_WOHNINSEL_1_TURM,        { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_KOLLEKTOR6,              { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_WOHNINSEL_1_UNTE,        { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_WOHNINSEL1,              { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_WOHNINSEL_1_DECK,        { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_WOHNINSEL_1_DACH,        { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_TUNNEL,                  { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_STADT,                   { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_RUMPF_ZEICHEN,           { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 0, 0.0f, 1.0f, RESTYPE_FS_METALL,                  { 0.0f, 0.0f, 0.0f}},


   // -- flagschiff innenteil
   //
   { BIGRES_FLAGSHIP, 1, 0.0f, 1.0f, RESTYPE_FS_TUNNEL_INNENSEIT_2,      { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 1, 0.0f, 1.0f, RESTYPE_FS_WOHNINSEL_1_DECK_2,      { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 1, 0.0f, 1.0f, RESTYPE_FS_ZENTRALE_2_A_2,          { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 1, 0.0f, 1.0f, RESTYPE_FS_KOLLEKTORSTANGEN_2,      { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 1, 0.0f, 1.0f, RESTYPE_FS_KOLLEKTOR1_2,            { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 1, 0.0f, 1.0f, RESTYPE_FS_KOLLEKTOR2_2,            { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 1, 0.0f, 1.0f, RESTYPE_FS_SCHLAUCH_2,              { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 1, 0.0f, 1.0f, RESTYPE_FS_REAKTOR_INNEN_01_2,      { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 1, 0.0f, 1.0f, RESTYPE_FS_REAKTOR_I1_2,            { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 1, 0.0f, 1.0f, RESTYPE_FS_REAKTOR_01_A_2,          { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 1, 0.0f, 1.0f, RESTYPE_FS_REAKTOR_I2_2,            { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 1, 0.0f, 1.0f, RESTYPE_FS_REAKTOR_I3_2,            { 0.0f, 0.0f, 0.0f}},
   { BIGRES_FLAGSHIP, 1, 0.0f, 1.0f, RESTYPE_FS_ZENTRALE_2_2,            { 0.0f, 0.0f, 0.0f}},
*/

   // -- Dukes Starbase
   //
   { BIGRES_CORIOLIS, 0, 0.0f, 1.0f, RESTYPE_CORIOLIS_EXTERIOR,          { 0.0f, 0.0f, 0.0f}},
   { BIGRES_CORIOLIS, 0, 0.0f, 1.0f, RESTYPE_CORIOLIS_CORE,              { 0.0f, 0.0f, 0.0f}},

   // -- die cobra des Players
   //
//   { BIGRES_COBRA, 0, 0.0f, 1.0f, RESTYPE_COBRA_PART1,                { 0.0f, 0.0f, 0.0f}},
//   { BIGRES_COBRA, 0, 0.0f, 1.0f, RESTYPE_COBRA_PART2,                { 0.0f, 0.0f, 0.0f}},
//   { BIGRES_COBRA, 0, 0.0f, 1.0f, RESTYPE_COBRA_PART3,                { 0.0f, 0.0f, 0.0f}},
   { BIGRES_COBRA, 0, 0.0f, 1.0f, RESTYPE_COBRA_MOUNTS,               { 0.0f, 0.0f, 0.0f}},
   { BIGRES_COBRA, 0, 0.0f, 1.0f, RESTYPE_COBRA_REAR,                 { 0.0f, 0.0f, 0.0f}},
// "grund" raumschiff
   { BIGRES_COBRA, 0, 0.0f, 1.0f, RESTYPE_COBRA_WEAPONFRONT,          { 0.0f, 0.0f, 0.0f}},
   { BIGRES_COBRA, 0, 0.0f, 1.0f, RESTYPE_COBRA_WEAPONLEFT,           { 0.0f, 0.0f, 0.0f}},
   //{ BIGRES_COBRA, 0, 0.0f, 1.0f, RESTYPE_COBRA_WEAPONREAR,           { 0.0f, 0.0f, 0.0f}},
   { BIGRES_COBRA, 0, 0.0f, 1.0f, RESTYPE_COBRA_WEAPONRIGHT,          { 0.0f, 0.0f, 0.0f}},
   { BIGRES_COBRA, 0, 0.0f, 1.0f, RESTYPE_WINGMAN,                    { 0.0f, 0.0f, 0.0f}},

   { BIGRES_CARGO03,  0, 0.0f, 2.889998, RESTYPE_CARGOBOX, { -0.700000f, -4.899999f, 20.300039f}},
   { BIGRES_CARGO03,  0, 0.0f, 2.889998, RESTYPE_CARGOBOX, { -0.700000f, -4.899999f, -32.200085f}},

   { BIGRES_CARGO08,  0, 0.0f, 2.539999, RESTYPE_CARGOBOX, { 0.000000f, 4.899998f, -20.300039f}},
   { BIGRES_CARGO08,  0, 0.0f, 2.539999, RESTYPE_CARGOBOX, { 0.000000f, 4.899998f, -62.999611f}},

/*
   { BIGRES_CARGO01,  0, 0.0f, 1.0f, RESTYPE_CARGO01,                 { 0.0f, 0.0f, 0.0f}},
   { BIGRES_CARGO01,  0, -0.01f, 1.0f, RESTYPE_CARGO01_HULL1,         { 0.0f, 0.0f, 0.0f}},
   { BIGRES_CARGO01,  0, +0.01f, 1.0f, RESTYPE_CARGO01_HULL2,         { 0.0f, 0.0f, 0.0f}},
   { BIGRES_CARGO01,  0, 0.0f, 1.0f, RESTYPE_CARGO01_SHIP,            { 0.0f, 0.0f, 0.0f}},
*/
}; 
const int l_cntBigResItem = sizeof(g_resassignArray)/sizeof(g_resassignArray[0]);
BIGRESITEM g_bigresitemArray[l_cntBigResItem];

int g_cntBigResItem = l_cntBigResItem;

TEXTUREASSIGN g_txtassigncompressedArray[] = {
	   { RESTYPE_FLYER,             EDRAW_RES_STANDARD,  TID_FLYER_RED,  TID_FLYER_RED_EMISSIVE,  TID_NULL, TID_NULL},
   { RESTYPE_LEADER,            EDRAW_RES_STANDARD,  TID_LEADER_RED, TID_LEADER_RED_EMISSIVE, TID_NULL, TID_NULL},
   { RESTYPE_REACTOR,           EDRAW_RES_STANDARD,  TID_MINE,       TID_NULL,                TID_NULL, TID_NULL},
   //{ RESTYPE_FELDGENERATOR,     EDRAW_RES_STANDARD,  TID_ALIEN_PO,   TID_NULL,                TID_NULL, TID_NULL},
   { RESTYPE_MINE,              EDRAW_RES_STANDARD,  TID_MINE,       TID_MINE_EMISSIVE,       TID_NULL, TID_NULL},
   { RESTYPE_MINE_LOWPOLY,      EDRAW_RES_STANDARD,  TID_MINE,       TID_NULL,                TID_NULL, TID_NULL},
   { RESTYPE_MINE_LOWPOLY2,     EDRAW_RES_STANDARD,  TID_MINE,       TID_NULL,                TID_NULL, TID_NULL},
/*
   { RESTYPE_DOOR01A,           EDRAW_RES_STANDARD,  TID_EXTRAS,     TID_EXTRAS_EM,           TID_NULL, TID_NULL},
   { RESTYPE_DOOR01B,           EDRAW_RES_STANDARD,  TID_EXTRAS,     TID_EXTRAS_EM,           TID_NULL, TID_NULL},
   { RESTYPE_DOOR01C,           EDRAW_RES_STANDARD,  TID_EXTRAS,     TID_EXTRAS_EM,           TID_NULL, TID_NULL},
   { RESTYPE_DOOR02A,           EDRAW_RES_STANDARD,  TID_REAKTORB,   TID_REAKTORB_EM,         TID_NULL, TID_NULL},
   { RESTYPE_DOOR02B,           EDRAW_RES_STANDARD,  TID_REAKTORB,   TID_REAKTORB_EM,         TID_NULL, TID_NULL},
   { RESTYPE_GUN,               EDRAW_RES_STANDARD,  TID_SCHLAUCH,   TID_SCHLAUCH_EM,         TID_NULL, TID_NULL},
   { RESTYPE_HANGAR,            EDRAW_RES_STANDARD,  TID_SCHLAUCH,   TID_SCHLAUCH_EM,         TID_NULL, TID_NULL},
*/
   { RESTYPE_ASTEROID,          EDRAW_RES_STANDARD,  TID_ASTEROID,   TID_ASTEROID_EMISSIVE,   TID_ASTEROID_BUMP,
                                                                                 TID_SMOKE },

   { RESTYPE_HUNTER_X,          EDRAW_RES_BUMP,  TID_HUNTER, TID_NULL, TID_HUNTER_NORMALS, TID_ILLUMINATION},
   { RESTYPE_BOA01,             EDRAW_RES_BUMP,  TID_BOA,    TID_BOA_EM,  TID_BOA_NORMALS, TID_ILLUMINATION},

   { RESTYPE_CARGO01,           EDRAW_RES_BUMP,  TID_CARGO01, TID_CARGO01_EM, TID_CARGO01_NORMALS, TID_NULL},
   { RESTYPE_CARGO03,           EDRAW_RES_BUMP,  TID_CARGO03, TID_CARGO03_EM, TID_CARGO03_NORMALS, TID_NULL},
   { RESTYPE_CARGO04,           EDRAW_RES_BUMP,  TID_CARGO04, TID_CARGO04_EM, TID_CARGO04_NORMALS, TID_NULL},
   { RESTYPE_CARGO05,           EDRAW_RES_BUMP,  TID_CARGO05, TID_CARGO05_EM, TID_CARGO05_NORMALS, TID_NULL},
   { RESTYPE_CARGO06,           EDRAW_RES_BUMP,  TID_CARGO06, TID_CARGO06_EM, TID_CARGO06_NORMALS, TID_NULL},
   { RESTYPE_CARGO07,           EDRAW_RES_BUMP,  TID_CARGO07, TID_NULL,       TID_CARGO07_NORMALS, TID_NULL},
   { RESTYPE_CARGO08,           EDRAW_RES_BUMP,  TID_CARGO08, TID_CARGO08_EM, TID_CARGO08_NORMALS, TID_NULL},

   { RESTYPE_CARGOBOX,          EDRAW_RES_BUMP,  TID_CARGOBOX,   TID_NULL,    TID_CARGOBOX_NORMALS, TID_NULL},

   { RESTYPE_HUNTER01,          EDRAW_RES_BUMP,  TID_HUNTER01, TID_HUNTER01_EM, TID_HUNTER01_NORMALS, TID_NULL},
   { RESTYPE_HUNTER02,          EDRAW_RES_BUMP,  TID_HUNTER02, TID_HUNTER02_EM, TID_HUNTER02_NORMALS, TID_NULL},
   { RESTYPE_HUNTER03,          EDRAW_RES_BUMP,  TID_HUNTER03, TID_HUNTER03_EM, TID_HUNTER03_NORMALS, TID_NULL},
   { RESTYPE_HUNTER09,          EDRAW_RES_BUMP,  TID_HUNTER09, TID_HUNTER09_EM, TID_HUNTER09_NORMALS, TID_NULL},
   { RESTYPE_HUNTER10,          EDRAW_RES_BUMP,  TID_HUNTER10, TID_HUNTER10_EM, TID_HUNTER10_NORMALS, TID_NULL},

   { RESTYPE_MINE2,             EDRAW_RES_BUMP,  TID_MINE2,      TID_NULL,          TID_MINE2_NORMALS, TID_NULL},
   { RESTYPE_MISSILE,            EDRAW_RES_BUMP,  TID_MISSILE,     TID_MISSILE_EM,     TID_MISSILE_NORMALS, TID_NULL},
   { RESTYPE_RESCUE,            EDRAW_RES_BUMP,  TID_RESCUE,     TID_RESCUE_EM,     TID_RESCUE_NORMALS, TID_NULL},
   { RESTYPE_TRANSPORT1,        EDRAW_RES_BUMP,  TID_TRANSPORT1, TID_TRANSPORT1_EM, TID_TRANSPORT1_NORMALS, TID_NULL},
/*
   { RESTYPE_FS_KOLLEKTORSTANGEN, EDRAW_RES_STANDARD,  TID_ALIEN_PO, TID_ALIEN_PO_EM, TID_NULL, TID_NULL},
   { RESTYPE_FS_KOLLEKTOR1,       EDRAW_RES_STANDARD,  TID_ELEKTRON, TID_NULL,        TID_NULL, TID_NULL},
   { RESTYPE_FS_RUMPF_PLATTEN_ST, EDRAW_RES_STANDARD,  TID_C_HULL_M, TID_NULL,        TID_NULL, TID_NULL},
   { RESTYPE_FS_RUMPF_PLA1,       EDRAW_RES_STANDARD,  TID_C_HULL_M, TID_NULL,        TID_NULL, TID_NULL},
   { RESTYPE_FS_BAUCH_ELEKTRONIK, EDRAW_RES_STANDARD,  TID_ELEKTRON, TID_NULL,        TID_NULL, TID_NULL},
   { RESTYPE_FS_HAUPTSTEGE_1,     EDRAW_RES_STANDARD,  TID_FENSTERF, TID_FENSTERF_EM, TID_NULL, TID_NULL},
   { RESTYPE_FS_REAKTOR_01_A,     EDRAW_RES_STANDARD,  TID_SCHLAUCH, TID_SCHLAUCH_EM, TID_NULL, TID_NULL},
   { RESTYPE_FS_RUMPF_PLA2,       EDRAW_RES_STANDARD,  TID_C_HULL_M, TID_NULL,        TID_NULL, TID_NULL},
   { RESTYPE_FS_RUMPF_PLA3,       EDRAW_RES_STANDARD,  TID_C_HULL_M, TID_NULL,        TID_NULL, TID_NULL},
   { RESTYPE_FS_ANTRIEB_1_B,      EDRAW_RES_STANDARD,  TID_ALIEN_PO, TID_ALIEN_PO_EM, TID_NULL, TID_NULL},
   { RESTYPE_FS_RUMPF_PLA4,       EDRAW_RES_STANDARD,  TID_A_HULL_M, TID_NULL,        TID_NULL, TID_NULL},
   { RESTYPE_FS_ANTRIEB_1_A,      EDRAW_RES_STANDARD,  TID_D_HULL_M, TID_D_HULL_M_EM, TID_NULL, TID_NULL},
   { RESTYPE_FS_ANTRIEB_1_C,      EDRAW_RES_STANDARD,  TID_ALIEN_PO, TID_ALIEN_PO_EM, TID_NULL, TID_NULL},
   { RESTYPE_FS_ZENTRALE_1_BASE_, EDRAW_RES_STANDARD,  TID_A_HULL_M, TID_NULL,        TID_NULL, TID_NULL},
   { RESTYPE_FS_RUMPF_02,         EDRAW_RES_STANDARD,  TID_A_HULL_M, TID_NULL,        TID_NULL, TID_NULL},
   { RESTYPE_FS_RUMPF_04,         EDRAW_RES_STANDARD,  TID_A_HULL_M, TID_NULL,        TID_NULL, TID_NULL},
   { RESTYPE_FS_RUMPF_PLA5,       EDRAW_RES_STANDARD,  TID_C_HULL_M, TID_NULL,        TID_NULL, TID_NULL},
   { RESTYPE_FS_RUMPF_PLA6,       EDRAW_RES_STANDARD,  TID_C_HULL_M, TID_NULL,        TID_NULL, TID_NULL},
   { RESTYPE_FS_RUMPF_06,         EDRAW_RES_STANDARD,  TID_A_HULL_M, TID_NULL,        TID_NULL, TID_NULL},
   { RESTYPE_FS_RUMPF_PLA7,       EDRAW_RES_STANDARD,  TID_C_HULL_M, TID_NULL,        TID_NULL, TID_NULL},
   { RESTYPE_FS_RUMPF_PLA8,       EDRAW_RES_STANDARD,  TID_C_HULL_M, TID_NULL,        TID_NULL, TID_NULL},
   { RESTYPE_FS_RUMPF_01,         EDRAW_RES_STANDARD,  TID_A_HULL_M, TID_NULL,        TID_NULL, TID_NULL},
   { RESTYPE_FS_RUMPF_03,         EDRAW_RES_STANDARD,  TID_A_HULL_M, TID_NULL,        TID_NULL, TID_NULL},
   { RESTYPE_FS_RUMPF_PLA9,       EDRAW_RES_STANDARD,  TID_C_HULL_M, TID_NULL,        TID_NULL, TID_NULL},
   { RESTYPE_FS_RUMPF_PL10,       EDRAW_RES_STANDARD,  TID_C_HULL_M, TID_NULL,        TID_NULL, TID_NULL},
   { RESTYPE_FS_RUMPF_05,         EDRAW_RES_STANDARD,  TID_A_HULL_M, TID_NULL,        TID_NULL, TID_NULL},
   { RESTYPE_FS_RUMPF_PL11,       EDRAW_RES_STANDARD,  TID_C_HULL_M, TID_NULL,        TID_NULL, TID_NULL},
   { RESTYPE_FS_RUMPF_PL12,       EDRAW_RES_STANDARD,  TID_C_HULL_M, TID_NULL,        TID_NULL, TID_NULL},
   { RESTYPE_FS_RUMPF_PL13,       EDRAW_RES_STANDARD,  TID_C_HULL_M, TID_NULL,        TID_NULL, TID_NULL},
   { RESTYPE_FS_RUMPF_PL14,       EDRAW_RES_STANDARD,  TID_C_HULL_M, TID_NULL,        TID_NULL, TID_NULL},
   { RESTYPE_FS_RUMPF_PL15,       EDRAW_RES_STANDARD,  TID_C_HULL_M, TID_NULL,        TID_NULL, TID_NULL},
   { RESTYPE_FS_TUNNEL_FRONT,     EDRAW_RES_STANDARD,  TID_ALIEN_PO, TID_ALIEN_PO_EM, TID_NULL, TID_NULL},
   { RESTYPE_FS_KOLLEKTORRING_1_, EDRAW_RES_STANDARD,  TID_D_HULL_M, TID_D_HULL_M_EM, TID_NULL, TID_NULL},
   { RESTYPE_FS_KOLLEKTOR2,       EDRAW_RES_STANDARD,  TID_FENSTERF, TID_FENSTERF_EM, TID_NULL, TID_NULL},
   { RESTYPE_FS_KOLLEKTORRING_2_, EDRAW_RES_STANDARD,  TID_FENSTERF, TID_FENSTERF_EM, TID_NULL, TID_NULL},
   { RESTYPE_FS_KOLLEKTOR3,       EDRAW_RES_STANDARD,  TID_D_HULL_M, TID_D_HULL_M_EM, TID_NULL, TID_NULL},
   { RESTYPE_FS_KOLLEKTORRING_3_, EDRAW_RES_STANDARD,  TID_FENSTERF, TID_FENSTERF_EM, TID_NULL, TID_NULL},
   { RESTYPE_FS_KOLLEKTOR4,       EDRAW_RES_STANDARD,  TID_D_HULL_M, TID_D_HULL_M_EM, TID_NULL, TID_NULL},
   { RESTYPE_FS_ZENTRALE_1_D,     EDRAW_RES_STANDARD,  TID_D_HULL_M, TID_D_HULL_M_EM, TID_NULL, TID_NULL},
   { RESTYPE_FS_ZENTRALE_1_C,     EDRAW_RES_STANDARD,  TID_ELEKTRON, TID_NULL,        TID_NULL, TID_NULL},
   { RESTYPE_FS_ZENTRALE_1_B,     EDRAW_RES_STANDARD,  TID_FENSTERF, TID_FENSTERF_EM, TID_NULL, TID_NULL},
   { RESTYPE_FS_ZENTRALE_1_A,     EDRAW_RES_STANDARD,  TID_D_HULL_M, TID_D_HULL_M_EM, TID_NULL, TID_NULL},
   { RESTYPE_FS_TUNNEL_BACK,      EDRAW_RES_STANDARD,  TID_ALIEN_PO, TID_ALIEN_PO_EM, TID_NULL, TID_NULL},
   { RESTYPE_FS_KOLLEKTORRING_4_, EDRAW_RES_STANDARD,  TID_D_HULL_M, TID_D_HULL_M_EM, TID_NULL, TID_NULL},
   { RESTYPE_FS_KOLLEKTOR5,       EDRAW_RES_STANDARD,  TID_FENSTERF, TID_FENSTERF_EM, TID_NULL, TID_NULL},
   { RESTYPE_FS_SCHLAUCH,         EDRAW_RES_STANDARD,  TID_SCHLAUCH, TID_SCHLAUCH_EM, TID_NULL, TID_NULL},
   { RESTYPE_FS_ZENTRALE_2_C,     EDRAW_RES_STANDARD,  TID_D_HULL_M, TID_D_HULL_M_EM, TID_NULL, TID_NULL},
   { RESTYPE_FS_ZENTRALE_2,       EDRAW_RES_STANDARD,  TID_REAKTORB, TID_REAKTORB_EM, TID_NULL, TID_NULL},
   { RESTYPE_FS_STUTZEN,          EDRAW_RES_STANDARD,  TID_B_HULL_M, TID_B_HULL_M_EM, TID_NULL, TID_NULL},
   { RESTYPE_FS_ZENTRALE_1_E,     EDRAW_RES_STANDARD,  TID_FENSTERF, TID_FENSTERF_EM, TID_NULL, TID_NULL},
   { RESTYPE_FS_AUFBAUTEN,        EDRAW_RES_STANDARD,  TID_FENSTERF, TID_FENSTERF_EM, TID_NULL, TID_NULL},
   { RESTYPE_FS_ZENTRALE_2_A,     EDRAW_RES_STANDARD,  TID_FENSTERF, TID_FENSTERF_EM, TID_NULL, TID_NULL},
   { RESTYPE_FS_ZENTRALE_2_B,     EDRAW_RES_STANDARD,  TID_ELEKTRON, TID_NULL,        TID_NULL, TID_NULL},
   { RESTYPE_FS_WOHNINSEL_1_TURM, EDRAW_RES_STANDARD,  TID_FENSTERF, TID_FENSTERF_EM, TID_NULL, TID_NULL},
   { RESTYPE_FS_KOLLEKTOR6,       EDRAW_RES_STANDARD,  TID_C_HULL_M, TID_NULL,        TID_NULL, TID_NULL},
   { RESTYPE_FS_WOHNINSEL_1_UNTE, EDRAW_RES_STANDARD,  TID_C_HULL_M, TID_NULL,        TID_NULL, TID_NULL},
   { RESTYPE_FS_WOHNINSEL1,       EDRAW_RES_STANDARD,  TID_FENSTERF, TID_FENSTERF_EM, TID_NULL, TID_NULL},
   { RESTYPE_FS_WOHNINSEL_1_DECK, EDRAW_RES_STANDARD,  TID_D_HULL_M, TID_D_HULL_M_EM, TID_NULL, TID_NULL},
   { RESTYPE_FS_WOHNINSEL_1_DACH, EDRAW_RES_STANDARD,  TID_ELEKTRON, TID_NULL,        TID_NULL, TID_NULL},
   { RESTYPE_FS_TUNNEL,           EDRAW_RES_STANDARD,  TID_ALIEN_PO, TID_ALIEN_PO_EM, TID_NULL, TID_NULL},
   { RESTYPE_FS_STADT,            EDRAW_RES_STANDARD,  TID_B_HULL_M, TID_B_HULL_M_EM, TID_NULL, TID_NULL},
   { RESTYPE_FS_RUMPF_ZEICHEN,    EDRAW_RES_STANDARD,  TID_B_HULL_M, TID_B_HULL_M_EM, TID_NULL, TID_NULL},
   { RESTYPE_FS_METALL,           EDRAW_RES_STANDARD,  TID_EXTRAS,   TID_EXTRAS_EM,   TID_NULL, TID_NULL},


   // -- flagschiff innenteil
   //
   { RESTYPE_FS_TUNNEL_INNENSEIT_2, EDRAW_RES_STANDARD,  TID_ALIEN_PO, TID_ALIEN_PO_EM, TID_NULL, TID_NULL},
   { RESTYPE_FS_WOHNINSEL_1_DECK_2, EDRAW_RES_STANDARD,  TID_D_HULL_M, TID_D_HULL_M_EM, TID_NULL, TID_NULL},
   { RESTYPE_FS_ZENTRALE_2_A_2,     EDRAW_RES_STANDARD,  TID_FENSTERF, TID_FENSTERF_EM, TID_NULL, TID_NULL},
   { RESTYPE_FS_KOLLEKTORSTANGEN_2, EDRAW_RES_STANDARD,  TID_ELEKTRON, TID_NULL,        TID_NULL, TID_NULL},
   { RESTYPE_FS_KOLLEKTOR1_2,       EDRAW_RES_STANDARD,  TID_ALIEN_PO, TID_ALIEN_PO_EM, TID_NULL, TID_NULL},
   { RESTYPE_FS_KOLLEKTOR2_2,       EDRAW_RES_STANDARD,  TID_C_HULL_M, TID_NULL,        TID_NULL, TID_NULL},
   { RESTYPE_FS_SCHLAUCH_2,         EDRAW_RES_STANDARD,  TID_SCHLAUCH, TID_SCHLAUCH_EM, TID_NULL, TID_NULL},
   { RESTYPE_FS_REAKTOR_INNEN_01_2, EDRAW_RES_STANDARD,  TID_D_HULL_M, TID_D_HULL_M_EM, TID_NULL, TID_NULL},
   { RESTYPE_FS_REAKTOR_I1_2,       EDRAW_RES_STANDARD,  TID_D_HULL_M, TID_D_HULL_M_EM, TID_NULL, TID_NULL},
   { RESTYPE_FS_REAKTOR_01_A_2,     EDRAW_RES_STANDARD,  TID_SCHLAUCH, TID_SCHLAUCH_EM, TID_NULL, TID_NULL},
   { RESTYPE_FS_REAKTOR_I2_2,       EDRAW_RES_STANDARD,  TID_ALIEN_PO, TID_ALIEN_PO_EM, TID_NULL, TID_NULL},
   { RESTYPE_FS_REAKTOR_I3_2,       EDRAW_RES_STANDARD,  TID_REAKTORB, TID_REAKTORB_EM, TID_NULL, TID_NULL},
   { RESTYPE_FS_ZENTRALE_2_2,       EDRAW_RES_STANDARD,  TID_REAKTORB, TID_REAKTORB_EM, TID_NULL, TID_NULL},
*/

   // -- Dukes Starbase
   //
   { RESTYPE_CORIOLIS_EXTERIOR,     EDRAW_RES_BUMP,  TID_CORIOLIS_EXTERIOR, TID_CORIOLIS_EXTERIOR_EM, TID_CORIOLIS_EXTERIOR_NORMALS, TID_NULL},
   { RESTYPE_CORIOLIS_CORE,         EDRAW_RES_BUMP,  TID_CORIOLIS_CORE, TID_CORIOLIS_CORE_EM, TID_CORIOLIS_CORE_NORMALS, TID_NULL},

   // -- Waffen Systeme
   { RESTYPE_LASERHALTER,       EDRAW_RES_BUMP,  TID_LASER,      TID_LASER_EM,      TID_LASER_NORMALS, TID_NULL},
   { RESTYPE_LASERGUN,          EDRAW_RES_BUMP,  TID_LASER,      TID_LASER_EM,      TID_LASER_NORMALS, TID_NULL},

/*
   { RESTYPE_LASERHALTER1,       EDRAW_RES_STANDARD,  TID_LASER,      TID_LASER_EM,      TID_LASER_NORMALS, TID_NULL},
   { RESTYPE_LASERGUN1,          EDRAW_RES_STANDARD,  TID_LASER,      TID_LASER_EM,      TID_LASER_NORMALS, TID_NULL},
*/

   { RESTYPE_STARBASEHANGAR,     EDRAW_RES_STANDARD,  TID_SBHANGAR,   TID_SBHANGAR_EM,   TID_SBHANGAR_NORMALS, TID_NULL},
   // -- die cobra des Players
   //
   { RESTYPE_COBRA_PART1,       EDRAW_RES_BUMP,  TID_COBRA, TID_COBRA_EM, TID_COBRA_NORMALS, TID_NULL},
   { RESTYPE_COBRA_MOUNTS,      EDRAW_RES_BUMP,  TID_COBRA_EXTRA, TID_COBRA_EXTRA_EM, TID_COBRA_EXTRA_NORMALS, TID_NULL},
   { RESTYPE_COBRA_REAR,        EDRAW_RES_BUMP,  TID_COBRA_EXTRA, TID_COBRA_EXTRA_EM, TID_COBRA_EXTRA_NORMALS, TID_NULL},
   { RESTYPE_COBRA_WEAPONFRONT, EDRAW_RES_BUMP,  TID_COBRA_EXTRA, TID_COBRA_EXTRA_EM, TID_COBRA_EXTRA_NORMALS, TID_NULL},
   { RESTYPE_COBRA_WEAPONLEFT,  EDRAW_RES_BUMP,  TID_COBRA_EXTRA, TID_COBRA_EXTRA_EM, TID_COBRA_EXTRA_NORMALS, TID_NULL},
   { RESTYPE_COBRA_WEAPONREAR,  EDRAW_RES_BUMP,  TID_COBRA_EXTRA, TID_COBRA_EXTRA_EM, TID_COBRA_EXTRA_NORMALS, TID_NULL},
   { RESTYPE_COBRA_WEAPONRIGHT, EDRAW_RES_BUMP,  TID_COBRA_EXTRA, TID_COBRA_EXTRA_EM, TID_COBRA_EXTRA_NORMALS, TID_NULL},
   { RESTYPE_WINGMAN,           EDRAW_RES_BUMP,  TID_COBRA_EXTRA, TID_COBRA_EXTRA_EM, TID_COBRA_EXTRA_NORMALS, TID_NULL},

   { RESTYPE_THARGOID,          EDRAW_RES_BUMP,  TID_THARGOID, TID_THARGOID_EM, TID_THARGOID_NORMALS, TID_NULL},
   { RESTYPE_THARGOIDDRONE,     EDRAW_RES_BUMP,  TID_THARGOID, TID_THARGOID_EM, TID_THARGOID_NORMALS, TID_NULL},
//   { RESTYPE_THRUSTLEVEL, EDRAW_RES_STANDARD,  TID_THARGOID, TID_NULL, TID_NULL, TID_NULL},
/*
   { RESTYPE_CARGO01_HULL1,         EDRAW_RES_STANDARD,  TID_CARGO01,  TID_CARGO01_EM,  TID_CARGO01_NORMALS, TID_NULL},
   { RESTYPE_CARGO01_HULL2,         EDRAW_RES_STANDARD,  TID_CARGO01,  TID_CARGO01_EM,  TID_CARGO01_NORMALS, TID_NULL},
   { RESTYPE_CARGO01_SHIP,          EDRAW_RES_STANDARD,  TID_CARGO01,  TID_CARGO01_EM,  TID_CARGO01_NORMALS, TID_NULL},
*/

};
const int l_cntTxtAssignCompressed = sizeof(g_txtassigncompressedArray)/sizeof(g_txtassigncompressedArray[0]);

int g_cntTxtAssignCompressed = l_cntTxtAssignCompressed;

TEXTUREASSIGN* g_txtassignResStdArray;

TEXTURE* g_txtArray;

MAPOBJECTAGGREGATE* MAPOBJECT::m_pmo_aggrArray[LIFECYCLE_LASTENUM];

MESHRESOURCEMANAGER g_meshresManager;

ID3D11RasterizerState* g_pD3DRasterizerState;


LPDIRECTINPUT8                   lpDirectInput                  = NULL;
LPDIRECTINPUTDEVICE8             g_didevMouse                   = NULL;
LPDIRECTINPUTDEVICE8             g_didevKeyboard                = NULL;

// -- direct input records
//
DIMOUSESTATE                     diMouseState             = {0};
DIJOYSTATE2                      diJoystickState2         = {0};
char g_arr_cKeystate[256];
int g_cntKeystateCheckTimer[256];
int g_maxKeystateCheckTimer = 1000;

int g_cntMousestateCheckTimer[2];
int g_maxMousestateCheckTimer = 1000;


ENEMY**       g_arr_pEnemy;
PSXFACEINFO** g_pfaceinfSelectedArray;
dContactGeom* g_contactgeomArray;

PLAYER g_Player;
ENEMYDRAWER* g_arr_EnemyDrawer;

//ENEMYDRAWER* g_edrawResEmissiveArray;
//ENEMYDRAWER* g_edrawResBumpArray;


// -- statische Member in ENEMY
//
int ENEMY::m_idxHighest;
ENEMY* ENEMY::m_penemyFirst;
ENEMY* ENEMY::m_penemyLast;
ENEMY* ENEMY::m_penemyTarget;

ENEMY* g_penemyCamera;
ENEMY* g_penemyInput;
ENEMY* g_penemyDebug;

ENEMYHANDLINGBUFFER g_ehdlBuf;

SHOOT* shoot_array = NULL;
int g_cntShoot;
/*int g_cntShootInserted;
int g_cntShootDeleted;
int g_maxShoot = 1000;
*/
bool g_bWriteLogFileInLoop= true;

char** g_cLang;

char* g_cLangEnglish[] = {
   "Null",
   "Police Combatflyer",
   "Police Combatflyer frontshielded",
   "Police Leader",
   "Bounty Hunter",
   "language changed to english",
   "Functions on your keyboard: ",
   "Cursor Up:      accelerate",
   "Cursor Down:    slowdown ",
   "Cursor Left:    rotate horizon Left",
   "Cursor Right:   rotate horizon Right ",
   "Cursor End:     Immediate Halt",
   "R:              toggle Rearview",
   "V:              toggle FirstPersonView",
   "H:              jump to Witchspace Destination",
   "J:              temporary SpeedUp-Boost",
   "S:              surrender to bountyhunter",
   "W:              switch control to Wingman",
   //"Shift Right:    step to next Weapon",
   "NumKey 0-9:     Select Weapon directly",
   "PgDown/Up:      Select Target and show additional Info ",
   "Return:         Lock (selected) Target",
   "Space Bar:      Unlock Target",
   "Cursor Del:     Immediate Rotation Halt",
   "+/- in Cockpit: Zoom/Unzoom Radarrange ",
   "+/- in Map:     toggle Galaxy (galactic Hyperdrive required)",
   "N:              Next Enemy",
   "F:              Next Friend",
   "I:              Toggle Inventary",
   "L:              Switch Language",
   "Press F1 for Keymap, ESC for Menu",
   "Press ESC to continue",
   "Mission goal",
   "Mission completely done",
   "Journey through Witchspace in %i seconds",
   "Game over",
   "Start the Game",
   "Optionmenu",
   "ExteriorView",
   "CargoBox",
   "Temporary acceleration acitvated!",
   "Producer",
   "GameDesign",
   "Coding",
   "Grafix",
   "Sound",
   "Distance",
   "Economy",
   "Goverment",
   "Technical Level",
   "Population",
   "Turnover",
   "Aver. Rad",
   "Billions",
   "Temperature of Lasergun",
   "LY",
   "Anarchy",
   "Feudal",
   "Multi-gov",
   "Dictatorship",
   "Communist",
   "Confederacy",
   "Democracy",
   "Corporate State",
   "Rich Ind",
   "Average Ind",
   "Poor Ind",
   "Mainly Ind",
   "Mainly Agri",
   "Rich Agri",
   "Average Agri",
   "Poor Agri",
   "Fuel",
   "Deflector",
   "State",
   "Docked",
   "Green",
   "Yellow",
   "Red",
   "Clean",
   "Offender",
   "Fugitive",
   "Harmless",
   "MostlyHarmless",
   "Poor",
   "Average",
   "AboveAverage",
   "Competent",
   "Dangerous",
   "Deadly",
   "Elite",
   "Present System",
   "Hyperspace System",
   "Condition",
   "Chash",
   "Legal Status",
   "Rating ",
   "Equipment",
   "Missile",
   "Large Cargo Bay",
   "Shield Generator",
   "Pulse Laser",
   "Battle Laser",
   "Gatling Laser I",
   "Gatling Laser II",
   "Super Laser",
   "Fuel Scoops",
   "Escape Capsule",
   "Energy Bomb",
   "Torpedo",
   "Extra Energy Unit",
   "Docking Computers",
   "Galactic Hyperdrive",
   "Mining Laser",
   "Military Laser",
   "Food",
   "Textiles",
   "Radioactives",
   "Cyborgs",
   "Beverages",
   "Cyborgs",
   "Liquor/Wines",
   "Luxuries",
   "Rare Species",
   "Narcotics",
   "Computers",
   "Machinery",
   "Alloys",
   "Firearms",
   "Furs",
   "Minerals",
   "Gold",
   "Platinum",
   "Gem-Stones",
   "Alien Items",
   "Commodity",
   "Offer",
   "Price",
   "Ships Hold",
   "Sell",
   "Buy",
   "Bankaccount",
   "Cockpit",
   "Galaxymap",
   "Help",
   "Leave Dock",
   "Artifacts",
   "Ore",
   "Spare Parts",
   "Pepper-Games Shares",
   "was chosen as Witchspace-Destination",
   "is to far for current fuel",
   "Credits",
   "Welcome to the planetsystem",
   "Already in Witchspace-Destinationsystem",
   "Trade",
   "Ship is not docked",
   "Maximum capacity reached",
   "Caution: Asteroids",
   "No Asteroids",
   "Trader",
   "Hunter",
   "Pirate",
   "Wingman",
   "Combatexperience",
   "Worlds Tech-Level too low",
   "Out of Stock",
   "too expensive",
   "not offered",
   "Speed",
   "Next Page",
   "Hunter chases you throughout the Hyperspace!",
   "Drop",
   "Eject CargoBox",
   "Gun",
   "Asteroid",
   "StarBase",
   "Enemy",
   "Neutral",
   "Shield",
   "Choose Save-Game File",
   "Game successfully saved",
   "Game successfully loaded",
   "Ready.",
   "An error occurred - Please save your game and restart program.",
   "Good Morning, Commander",
   "Surrender to Bounties!",
   "Radiation too high",
   "No Target set",
   "Mine",
   "Too near to Starbase",
   "Witchspace-Jump aborted",
   "Crime-Ticked paid",
   "can't pay Crime-Ticket",
   "Police-Scan must be passed",
   "Galaxy",
   "+/- Keys to Change",
   "Illegal",
   "Load",
   "Save",
   "Can't create file",
   "Can't open file",
   "No valid file or unknown version",
   "1234 : unknown chunk",
   "read error occured",
   "Ore loaded in Cargobay",
   "Total",
   "Thargoid",
   "Thargoiddrone",
   "Malfunction of Witchspacedrive",
   "Reward earned",
   "Language",
   "Seen Pir.",
   "Last Seen at",
   "X Jumps ago",
   "Reward",
   "Traders report pirates they've seen before last jump:",
   "needed to mount this",
   "Other goods or equipment",
   "are dependent to this",
};

char* g_cLangGerman[] = {
   "Null",
   "Polizei Kampfgleiter",
   "Polizei Kampfgleiter (Frontschild)",
   "Polizei Groupleader",
   "Kopfgeld-Jäger",
   "Sprache nach -Deutsch- gewechselt",
   "Funktionen auf der Tastatur:",
   "Cursor Hoch:   Beschleunigen ",
   "Cursor Runter: Bremsen ",
   "Cursor Links:  Horizont nach links kippen",
   "Cursor Rechts: Horizont nach rechts kippen",
   "Cursor Ende:   Sofort Halt",
   "R:             Rueckansicht an/auschalten",
   "V:             Cockpit-Sicht an/ausschalten",
   "H:             zum Hyperraumziel springen",
   "J:             zeitweilige Beschleunigung",
   "S:             Kapitulieren vor dem Kopfgeldjäger",
   "W:             Steuerungswechsel zum Wingman",
   //"Shift Rechts:  Zur nächsten Waffe wechseln",
   "NumKey 0-9:    Waffen direkt auswählen",
   "Bild Auf/Ab:   Ziel wählen und Zusatzinfo anzeigen ",
   "Return:        (gewähltes) Ziel einrasten und verfolgen",
   "Leertaste:     eingerastetes Ziel wieder freigeben",
   "Cursor Entf:   Sofortiger Halt der Rotation",
   "+/- (Cockpit): Radarreichweite zoomen ",
   "+/- (Karte):   Galaxie umschalten wenn galaktischer Hyperraumantrieb vorhanden",
   "N:             Nächster Feind",
   "F:             Nächster Freund",
   "I:             Inventaranzeige zu-/wegschalten",
   "L:             Sprache umschalten",
   "F1-Taste zeigt Tastenbelegung, ESC-Taste das Menu",
   "ESC-Taste zum Weitermachen",
   "Missionsziel",
   "Mission erledigt",
   "Sprung durch Hyperraum in %i Sekunden",
   "Game Over",
   "Spiel starten",
   "Hauptmenü",
   "Außenansicht",
   "FrachtRaum",
   "Kurzzeitige Beschleunigung aktiviert!",
   "Produktion",
   "Spielkonzept",
   "Coder",
   "Grafik",
   "Sound",
   "Entfernung",
   "Wirtschaft",
   "Regierung",
   "Technische Stufe",
   "Bevölkerung",
   "Bruttosozialprodukt",
   "Mittl. Radius",
   "Milliarden",
   "Laser Temperatur",
   "LJ",
   "Anarchie",
   "Feudal",
   "Gemischt",
   "Diktatur",
   "Kommunismus",
   "Staatenbund",
   "Demokratie",
   "Sozialstaat",
   "Reiche Industrie",
   "Durchschn. Ind.",
   "Arme Industrie",
   "Hauptsächlich Industrie",
   "Hauptsächlich Agrarwirtsch.",
   "Reiche Agrarwirtsch.",
   "Durchschn. Agrarw.",
   "Arme Agrarwirtsch.",
   "Treibstoff",
   "Schutzschild",
   "Status",
   "Angedockt",
   "Grün",
   "Gelb",
   "Rot",
   "Sauber",
   "Verbrecher",
   "Flüchtig",
   "Harmlos",
   "Relativ Harmlos",
   "Schwach",
   "Durchschnitt",
   "Über Durchschnitt",
   "Kompetent",
   "Gefährlich",
   "Tödlich",
   "Elite",
   "Derzeitiges System",
   "Hyperraum-Ziel",
   "Zustand",
   "Konto",
   "Strafregister",
   "Einstufung ",
   "Ausrüstung",
   "Rakete",
   "Zusatz-Laderaum",
   "Schildgenerator",
   "Impuls-Laser",
   "Kampf-Laser",
   "Gatling Laser I",
   "Gatling Laser II",
   "Super-Laser",
   "Raumgreifer",
   "Rettungskapsel",
   "Energiebombe",
   "Torpedo",
   "Zusatz-Energieakku",
   "Andock-Computer",
   "Gal.Hyperraum-Antrieb",
   "Asteroiden Laser",
   "Militär Laser",
   "Nahrung",
   "Textilien",
   "Spaltstoffe",
   "Robotersklaven",
   "Spirituosen",
   "Cyborgs",
   "Liköre/Weine",
   "Luxusgüter",
   "Begehrte Substanzen",
   "Narkotika",
   "Computer",
   "Maschinen",
   "Legierungen",
   "Waffen",
   "Felle",
   "Mineralien",
   "Gold",
   "Platin",
   "Edelsteine",
   "Fremde Güter",
   "Waren",
   "Angebot",
   "Preis",
   "Schiffsbestand",
   "Verkaufen",
   "Kaufen",
   "Bankkonto",
   "Cockpit",
   "Galaxiekarte",
   "Hilfe",
   "Dock verlassen",
   "Artefakte",
   "Erz",
   "Ersatzteile",
   "Pepper-Games Aktien",
   "wurde gewählt als Hyperraumziel",
   "zu weit entfernt für den derzeitigen Treibstoffstand",
   "Mitwirkende",
   "Willkommen im Planetensystem",
   "Bereits im Hyperraum-Zielsystem",
   "Handel",
   "Schiff ist nicht angedockt",
   "Höchstfassungsvermögen erreicht",
   "Vorsicht Asteroiden",
   "Keine Asteroiden",
   "Händler",
   "Kopfgeldjäger",
   "Pirat",
   "Wingman",
   "Kampferfahrung",
   "Planet zu primitiv für Technologie",
   "Kein Bestand",
   "zu teuer",
   "nicht angeboten",
   "Geschw.",
   "nächste Seite",
   "Kopfgeldjäger verfolgt Dich durch den Hyperraum!",
   "Abwerfen",
   "Cargobox aussetzen",
   "Kanone",
   "Asteroid",
   "Sternenbasis",
   "Feind",
   "Neutral",
   "Schild",
   "Save-Game Datei wählen",
   "Spiel wurde erfolgreich gespeichert",
   "Spiel wurde erfolgreich geladen",
   "Bereit.",
   "Ein Fehler ist aufgetreten. Bitte den Spielstand speichern und Programm neu starten.",
   "Guten Morgen, Commander",
   "Den Kopfgeldjägern ergeben!",
   "Strahlung zu hoch.",
   "Kein Ziel eingestellt",
   "Mine",
   "Zu nahe an Raumstation",
   "Hyperraumsprung abgebrochen",
   "Geldstrafe bezahlt",
   "Geldstrafe konnte nicht bezahlt werden",
   "Polizei-Scan muss abgewartet werden",
   "Galaxie",
   "Tasten +/- zum Wechseln",
   "Illegal",
   "Laden",
   "Speichern",
   "Datei kann nicht erzeugt werden",
   "Datei kann nicht geöffnet werden",
   "keine gültige Datei oder falsche Version davon",
   "1234 : unbekannter Block in Datei",
   "Lesefehler systemseitig aufgetreten",
   "Erz im Frachtraum geladen",
   "Gesamt",
   "Thargoide",
   "Thargoidendrohne",
   "Fehlfunktion des Hyperraumantriebs",
   "Abschussprämie verdient",
   "Sprache",
   "Kopfgeldtipps",
   "Zuletzt gesehen in",
   "vor X Sprüngen",
   "Kopfgeld",
   "Händler melden die Piraten, die sie vor dem letzten Sprung gesehen haben:",
   "wird zur Anbringung benötigt",
   "Andere Waren oder Ausrüstungsgegenstände",
   "sind abhängig hiervon",
};

char* g_cLangItalian[] = {
   "Null",
   "Astronave da combattimento della Polizia",
   "Astronave da combattimento della Polizia con scudi frontali",
   "Comandante della Polizia",
   "Cacciatore di taglie",
   "cambiamento della lingua in Italiano",
   "Funzioni della tastiera",
   "Freccia in su: accelera",
   "Freccia in giu': rallenta",
   "Freccia a sinistra: ruota a sx",
   "Freccia a destra: ruota a dx",
   "Cancella: arresto immediato",
   "R:            vista posteriore",
   "V:            vista in soggettiva",
   "H:            iperspazio alla destinazione selezionata",
   "J:            impulso turbo ai motori",
   "S:            arrenditi!",
   "W:            passa al controllo WingMan",
   //"Shift destro: seleziona l'arma",
   "NumKey 0-9:   seleziona l'arma",
   "Pag su/giu':  seleziona l'obbiettivo e mostra le informazioni addizionali",
   "Invio:        aggancia l'obbiettivo selzionato",
   "Barra:        spaziatrice: sgancia l'obbiettivo",
   "Canc:         ferma subito la rotazione",
   "+/-           nell'abitacolo: fattori d'ingrandimento radar",
   "+/-           in mappa: seleziona la galassia (serve l'hyperdrive galattico)",
   "N:            nemico successivo",
   "F:            amico successivo",
   "I:            vai all'inventario",
   "L:            switch language",
   "F1 per la mappadella tastiera, ESC per uscire",
   "Pigia ESC per continuare",
   "Scopo della missione",
   "Missione interamente completata",
   "Salto spaziale in %i secondi",
   "Fine della partita..",
   "Inizia il gioco",
   "Menu delle opzioni",
   "Vista esterna",
   "Stiva",
   "Accelerazione temporale abilitata!",
   "Produttore",
   "Disegnatore del giuoco",
   "Codice",
   "Grafica",
   "Suono",
   "Distanza",
   "Economia",
   "Governo",
   "Livello tecnologico",
   "Popolazione",
   "Turnover",
   "Radiazione media",
   "Billioni",
   "Temperatura dei laser",
   "LY",
   "Anarchico",
   "Feudale",
   "Multigovernativo",
   "Berlusconinista dittatoriale",
   "Comunista in stile PCI",
   "Confederazione",
   "Democartico",
   "Corporazione di stati",
   "Ricco industriale",
   "Mediamente industrializzato",
   "Industria povera",
   "Prettamente industriale",
   "Prettamente agricolo",
   "Agricoltura ricca",
   "Mediamente agricolo",
   "Agricoltura povera",
   "Carburante (colza)",
   "Deflettori",
   "Stato",
   "Attraccato",
   "Verde",
   "Giallo",
   "Rosso",
   "Libero",
   "Offensivo",
   "Fuggitivo",
   "Inoffensivo",
   "Tendenzialmente inoffensivo",
   "Povero",
   "Medio",
   "Sopra la media",
   "Competente",
   "Pericoloso",
   "Mortale",
   "Elite",
   "Sistema attuale",
   "Sistema ipersaziale",
   "Condizione",
   "Soldi",
   "Stato legale",
   "Classifica",
   "Equipaggiamento",
   "Missile",
   "Stiva ampia",
   "Generatore di scudi",
   "Laser ad impulsi",
   "Laser da battaglia",
   "Laser Gantling I",
   "Laser Gantling II ",
   "Laser potenziato",
   "Rifornimenti di carburante",
   "Capsula di salvataggio",
   "Bomba d'energia",
   "Torpedine",
   "Unita' supplementare d'energia",
   "Computer d'attracco",
   "Hyperdrive galattico",
   "Laser da estrazione mineraria",
   "Laser militare",
   "Cibo",
   "Tessile",
   "Radioattivo",
   "Umanoidi",
   "Abbeveraggi",
   "Cyborgs",
   "Vini e liquori",
   "Beni di lusso",
   "Speci rare",
   "Droga",
   "Computers",
   "Cianfrusaglia e macchinari",
   "Leghe metalliche",
   "Armi da fuoco",
   "Pellicceria",
   "Minerali",
   "Oro",
   "Platino",
   "Diamanti",
   "Beni alieni",
   "Facilitazioni",
   "Offerta",
   "Prezzo",
   "Proprieta' di astronavi",
   "Vendita",
   "Acquisto",
   "Conto in banca",
   "Abitacolo",
   "Mappa della galassia",
   "AIUTO!",
   "Abbandona l'attracco",
   "Artefatti",
   "Minerale",
   "Pezzi separati",
   "Scambio di giochi Pepper",
   "era stato scelto come destinazione iperspaziale",
   "e' troppo lontano: non ti basta il carburante",
   "Crediti",
   "Benvenuto nel sistema planetario",
   "Sei gia' nell'iperspazio-sistema di destinazione",
   "Commercia",
   "L'astronave non e' attraccata",
   "Hai raggiunto la capacita' massima",
   "Attenzione: asteroidi",
   "Non vi sono asteroidi",
   "Commerciante",
   "Cacciatore",
   "Pirata",
   "WingMan",
   "Esperienza di combattimento",
   "Livello tecnologico dei modni troppo basso",
   "Non disponibile",
   "Troppo caro",
   "Non e' in vendita",
   "Velocita'",
   "Pagina successiva",
   "Un cacciatore t'insegue nell'iperspazio!",
   "Abbandona",
   "Espelli il cargo",
   "Fucile",
   "Asteroide",
   "Base spaziale",
   "Nemico",
   "Neutrale",
   "Scudi",
   "Seleziona il file di salvataggio",
   "Gioco salvato correttamente",
   "Gioca caricato correttamente",
   "Pronto.",
   "ERORE! Salva la partita e riavvia il giuoco!",
   "Buongiorno comandante",
   "Arrenditi ai bucanieri!",
   "Radiazione troppo alta",
   "Nessun obbiettivo selezionato",
   "Mina",
   "Troppo vicino alla base spaziale",
   "Salto spaziale annullato",
   "Penale di cimine pagata",
   "Non puoi pagare la penale di crimine",
   "Devi passare un controllo da parte della Polizia",
   "Galassia",
   "+/- per cambiare",
   "ILLEGALE",
   "Carica",
   "Salva",
   "Non posso creare il file",
   "Non posso aprire il file",
   "File non valido o versione errata",
   "1234 : parte sconosciuta",
   "Errore di lettura",
   "Minerale caricato in stiva",
   "Totale",
   "Thargoid",
   "Drone Thargoid",
   "Malfunzionamento del sistema iperspaziale",
   "Ricompensa guadagnata",
   "Lingua",
   "Seen Pir.",
   "last seen at",
   "before x jumps",
   "Reward",
   "Traders report pirates they've seen before last jump:",
   "needed to mount this",
   "Other goods or equipment",
   "are dependent to this",
};

char* g_cLangPolish[] = {
   "Zero",
   "Bojowy Slizgacz Policyjny",
   "Bojowy Slizgacz Policyjny (tarcza przednia)",
   "Dowodca Grupy Policyjnej",
   "Lowca Nagrod",
   "Jezyk zostal zmieniony na -polski-",
   "Funkcje dostepne z klawiatury:",
   "Klawisz kursora w gore:  przyspieszanie ",
   "Klawisz kursora w dol:   hamowanie ",
   "Klawisz kursora w lewo:  obrot w lewo",
   "Klawisz kursora w prawo: obrot w prawo",
   "Klawisz End:             natychmiastowe zatrzymanie",
   "R:                       Wlaczanie/wylaczanie widoku do tylu",
   "V:                       Wlaczanie/wylaczanie widoku kokpitu",
   "H:                       skok w nadprzestrzen",
   "J:                       krotki skok czasowy",
   "S:                       kapitulacja przed lowca nagrod",
   "W:                       przekazanie sterowania skrzydlowemu",
   //"Prawy kl. Shift:         wybor/zmiana broni",
   "NumKey 0-9:              wybor/zmiana broni",
   "Klawisz PgUp/PgDn:       wybor celu i wyswietlenie informacji ",
   "Return:                  ledzenie i poscig za (wybranym) celem",
   "Spacja:                  zwolnienie celu",
   "Backspace:               natychmiastowe zatrzymanie obrotu",
   "+/- (w kokpicie):        zwiêkszanie/zmniejszanie zasiegu radaru ",
   "+/- (na mapie):          przelaczanie miedzy galaktykami, gdy zamontowany jest naped miedzygalaktyczny",
   "N:                       nastepny wrog",
   "F:                       nastepny sojusznik",
   "I:                       wlaczanie/wylaczanie ekranu posiadania",
   "L:                       przelaczanie wersji jezykowej",
   "klawisz F1 wyswietla oblozenie klawiatury, klawisz ESC  - menu",
   "nacisnij klawisz ESC aby kontynuowac",
   "cel misji",
   "misja wykonana",
   "Skok w nadprzestrzen za %i sekundy",
   "Koniec gry",
   "Uruchomienie gry",
   "Menu glowne",
   "Widok zewnetrzny",
   "Ladownia",
   "Aktywacja krotkiego skoku czasowego!",
   "Produkcja",
   "Koncepcja gry",
   "Kod gry",
   "Grafika",
   "Dzwiek",
   "Odleglosc",
   "Typ gospodarki",
   "Typ rzadow",
   "Poziom techniczny",
   "Ludnosc",
   "Produkt Globalny Brutto",
   "Promien",
   "miliardow",
   "Temperatura lasera",
   "LJ",
   "Anarchia",
   "Feudalizm",
   "Mieszany",
   "Dyktatura",
   "Komunizm",
   "Federacja",
   "Demokracja",
   "Panstweo opiekuncze",
   "Silnie uprzemyslowiona",
   "Przecietnie uprzemyslowiona.",
   "Slabo uprzemyslowiona",
   "Glownie przemyslowa",
   "Glownie rolnicza",
   "Silnie rolnicza",
   "Przecietnie rolnicza",
   "Slabo rolnicza",
   "Paliwo",
   "Tarcza ochronna",
   "Stan",
   "Zadokowany",
   "Zielony",
   "Zolty",
   "czerwony",
   "Czysty",
   "Przestepca",
   "Scigany",
   "Nieszkodliwy",
   "Wzglednie nieszkodliwy",
   "Slaby",
   "Przecietny",
   "Ponadprzecietny",
   "Fachowiec",
   "Niebezpieczny",
   "Zabojczy",
   "Elita",
   "Obecny system",
   "Cel skoku",
   "Stan",
   "Konto",
   "Kartoteka policyjna",
   "Stopien ",
   "Wyposazenie",
   "Rakieta",
   "Dodatkowa ladownia",
   "Generator tarcz",
   "Laser impulsowy",
   "Laser bojowy",
   "Laser Gatling I",
   "Laser Gatling II",
   "Superlaser",
   "Chwytak",
   "Kapsula ratunkowa",
   "Bomba energetyczna",
   "Torpeda",
   "Dodatkowy generator energii",
   "Komputer dokujacy",
   "Naped miedzygalaktyczny",
   "Laser gorniczy",
   "Laser wojskowy",
   "Zywnosc",
   "Tekstylia",
   "Materialy radioaktywne",
   "Roboty-niewolnicy",
   "Alkohole",
   "Cyborgi",
   "Likiery/wina",
   "Towary luksusowe",
   "Towary rzadkie",
   "Narkotyki",
   "Komputery",
   "Maszyny",
   "Stopy metali",
   "Bron",
   "Futra",
   "Mineraly",
   "Zloto",
   "Platyna",
   "Kamienie szlachetne",
   "Artefakty Obcych",
   "Towary",
   "Oferta",
   "Cena",
   "Zapas na statku",
   "Sprzedaz",
   "Kupno",
   "Konto bankowe",
   "Kokpit",
   "Mapa galaktyki",
   "Pomoc",
   "Opuszczenie doku",
   "Artefakty",
   "Ruda",
   "Czesci zamienne",
   "Akcje firmy Pepper-Games",
   "wybrano na cel skoku",
   "za malo paliwa na skok",
   "Wspoltworcy",
   "witamy w systemie",
   "juz w systemie docelowym",
   "Handel",
   "Statek nie jest zadokowany",
   "Osiagnieto najwyzsze przyspieszenie czasu",
   "Uwaga: asteroidy",
   "Brak asteroid",
   "Handlarz",
   "Lowca nagrod",
   "Pirat",
   "Skrzydlowy",
   "Doswiadczenie bojowe",
   "Planeta jest zbyt prymitywna na te technologie",
   "Brak na stanie",
   "za drogo",
   "nie oferowane",
   "Predk.",
   "nastepna strona",
   "Lowca nagrod sciga cie w nadprzestrzeni!",
   "Odrzucic",
   "Odlaczenie ladowni",
   "Dzialo",
   "Asteroida",
   "Baza gwiezdna",
   "Wrog",
   "Neutralny",
   "Tarcza",
   "Wybierz zapis gry",
   "Gra zostala pomyslnie zapisana",
   "Gra zostala pomyslnie zaladowana",
   "Gotowe.",
   "Wystapil blad. Prosze zapisac stan gry i uruchomic gre od nowa",
   "Dzien dobry KOmandorze",
   "Poddac sie lowcy nagrod!",
   "Promieniowanie za wysokie.",
   "Brak wybranego celu",
   "Mina",
   "Zbyt blisko stacji",
   "Skok w nadprzestrzen przerwany",
   "Grzywna zaplacona",
   "Grzywna nie moze byc zaplacona",
   "Trzeba odczekac na kontrole Policji",
   "Galaktyka",
   "Zmiana klawiszami +/-",
   "Nielegalne",
   "Ladowanie",
   "Zapis",
   "Nie mozna utworzyc pliku",
   "Nie mozna otworzyc pliku",
   "zly plik lub jego wersja",
   "1234 : nieznany blok w pliku",
   "Systemowy blad odczytu ",
   "Ruda zaladowana do ladowni",
   "Razem",
   "Thargoidy",
   "Mysliwce bezzalogowe Thargoidow",
   "Awaria napedu nadprzestrzennego",
   "Nagrody",
   "Wersje jezykowe",
   "Informacje o poszukiwanych",
   "Ostatnio widziany w:",
   "przed X skokami",
   "Nagroda",
   "Kupcy donosza o piratach, ktorych widzieli ostanio przed skokiem:",
   "Jest potrzebny do zamontowania",
   "Inne towary lub wyposazenie",
   "sa od tego zalezne",
};


ENEMY* ENEMY::m_pCurrLeader;

char* g_cActionNameArray[] = {
	"ENACT_DONOTHING",
	"ENACT_CURVEAROUND",
	"ENACT_CHECKPTALIGNY, ",
	"ENACT_BECOMELEADERSPART, ",
	"ENACT_ALIGNYSMOOTH, ",
	"ENACT_ALIGNX",
	"ENACT_SHOOT",
	"ENACT_ESCAPE",
	"ENACT_BANKROTATION",
	"ENACT_REFLECTION",
	"ENACT_FOLLOWLEADER",
	"ENACT_COUNTDOWN",
	"ENACT_AVOIDPLAYER",
	"ENACT_AVOIDSTARBASE",
	"ENACT_AVOIDFACEUP",
	"ENACT_AVOIDFACEDOWN",
	"ENACT_AVOIDFACEUPHARD",
	"ENACT_AVOIDFACEDOWNHARD",
	"ENACT_BREAKOUT_UP",
	"ENACT_BREAKOUT_DOWN",
	"ENACT_PULLUP",
	"ENACT_PULLDOWN",
	"ENACT_ACCELERATE",
	"ENACT_LEAVEDOCK",
	"ENACT_CHASEPLAYER",
	"ENACT_KAMIKAZE",
	"ENACT_AVOIDDESTSMOOTH",
	"ENACT_DROPCARGO",
	"ENACT_GOTOSTARBASE",
	"ENACT_DOCKING",
	"ENACT_DETONATE" };

char* g_cEnemyTypeNameArray[] = { 
                                  "ETYP_EMPTY",
                                  "ETYP_FLYER",
                                  "ETYP_EXPLOSION",
                                  "ETYP_IMPACT",
                                  "ETYP_MINE",
                                  "ETYP_ASTEROID",
                                  "ETYP_HANGAR",
                                  "ETYP_GUN",
                                  "ETYP_REACTORGUN",
                                  "ETYP_LEADER",
                                  "ETYP_DOOR",
                                  "ETYP_TUNNELFLYER",
                                  "ETYP_EQUIPMENT",
                                  "ETYP_STATIC",
                                  "ETYP_FELDGENERATOR",
                                  "ETYP_TMPSPEEDUP",
                                  "ETYP_GHOST",
                                  "ETYP_CHAR",
                                  "ETYP_TRADER",
                                  "ETYP_HUNTER",
                                  "ETYP_PIRATE",
                                  "ETYP_P_FLYER",
                                  "ETYP_LASERHALTER",
                                  "ETYP_LASERGUN",
                                  "ETYP_CARGOBOX",
                                  "ETYP_BIGRESITEM",
                                  "ETYP_STARBASE",
                                  "ETYP_KINGSPLANE",
                                  "ETYP_THARGOID",
                                  "ETYP_THARGOIDDRONE",
                                  "ETYP_PLAYER",
                                  "ETYP_WINGMAN",
                                  "ETYP_STARBASEHANGAR",
                                  "ETYP_PLANET",
                                  "ETYP_LASTENUM"};

// -- die Spawnpunkt arrays pro Restype
//
PSXLASERSPAWN g_laserHunterXArray[] = {
	{ { -03.63918f, -00.32218f,  22.66527f },SHOOTDRAWER_BLUELASER,  12.0f },
	{ { -01.98234f, -04.17529f,  17.33864f },SHOOTDRAWER_REDTORPEDO, 8.0f },
	{ { -01.98234f,  04.17529f,  17.33864f },SHOOTDRAWER_REDLASER,   16.0f },

	{ { -03.63918f, -00.32218f, -22.66527f },SHOOTDRAWER_BLUELASER,  12.0f },
	{ { -01.98234f, -04.17529f, -17.33864f },SHOOTDRAWER_REDTORPEDO, 8.0f },
	{ { -01.98234f,  04.17529f, -17.33864f },SHOOTDRAWER_REDLASER,   16.0f },
};

PSXLASERSPAWN g_laserPoliceLeaderArray[] = {
	{ { +02., 0,  2.0f },SHOOTDRAWER_BLUELASER,  8.0f },
	{ { -02., 0,  2.0f },SHOOTDRAWER_BLUELASER,  8.0f },
	{ { -01.98234f, -04.17529f, -17.33864f },SHOOTDRAWER_REDTORPEDO, 4.0f },
};

PSXLASERSPAWN g_laserPoliceFlyerArray[] = {
	{ { +02., 0,  2.0f },SHOOTDRAWER_BLUELASER,  8.0f },
	{ { -02., 0,  2.0f },SHOOTDRAWER_BLUELASER,  8.0f },
};

PSXLASERSPAWN g_laserGunArray[] = {
	{ { 0.0f, -8.3236f, 113.48f },SHOOTDRAWER_BLUELASER,  12.0f },
	{ { 10.0f,  6.6764f, 113.48f },SHOOTDRAWER_BLUELASER,  12.0f },
	{ { -10.0f, 6.6764f, 113.48f },SHOOTDRAWER_BLUELASER,  12.0f },
};

PSXLASERSPAWN g_laserGunArrayS[] = {
	{ { 0.0f, -0.83236f, 11.348f },SHOOTDRAWER_BLUELASER,  12.0f },
	{ { 1.0f,  0.66764f, 11.348f },SHOOTDRAWER_BLUELASER,  12.0f },
	{ { -1.0f, 0.66764f, 11.348f },SHOOTDRAWER_BLUELASER,  12.0f },
};

PSXLASERSPAWN g_laserWingmanArray[] = { { { -8.799997f, -0.299995f, 35.500031f },SHOOTDRAWER_REDLASER,  12.0f },
{ { +8.799997f, -0.299995f, 35.500031f },SHOOTDRAWER_REDLASER,  12.0f },
};


// -- static member von LEVEL
//
LEVEL* LEVEL::m_plevelFirst;
LEVEL* LEVEL::m_plevelLast;
LEVEL* LEVEL::m_plevelSelected;
int LEVEL::m_idxHighest;

dWorldID LEVEL::m_iWorldID;
dSpaceID LEVEL::m_dSpaceID;
dJointGroupID LEVEL::m_dJointGroupID;

// --- gameplay (score und shield )
//
EQUIPMENT g_equipmentArray[EQUIPMENT_LASTENUM];

//-- 0 = Laser
//
EQUIPMENT* g_pequipmentSelectWeapon;

// -- Direct2D
//
ID2D1Factory1* g_pd2dFactory;
IDXGIDevice* g_pdxgiDevice;
ID2D1Device* g_pd2dDevice;
ID2D1DeviceContext* g_pd2dDeviceContext;
IDXGISurface* g_pdxgiSurface;
//ID2D1Bitmap1* g_pd2dBitmap;
ID2D1SolidColorBrush* g_pd2dSolidWhiteBrush;
IDWriteFactory* g_pdwriteFactory;
IDWriteTextFormat* g_pdwriteTextFormat;
D2D1_SIZE_U g_d2dsizeClientRect;
D2D1_RECT_F g_d2drectClientRect;
ID2D1RenderTarget* g_pd2dRenderTarget;
//ID2D1HwndRenderTarget* g_pd2dhwdRenderTarget;

char* debugausgabe;
