/*
 *------------------------------------------------------------------------
 *      Object: psxtxt.h
 *         $Id: psxtxt.h,v 1.4 2009/12/07 20:56:00 molle Exp $
 *        Date: Oct 2005
 *       Notes: Textures
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

#ifndef TEXTURE_H
#define TEXTURE_H

typedef enum en_textureid { TID_NULL = 0,
                            TID_NEBULA2,
                            TID_NEBULA3,
                            TID_NEBULA4,
                            TID_NEBULA5,
                            TID_NEBULA6,
                            TID_NEBULA7,
                            TID_NEBULA8,
                            TID_CROSSHAIR,
                            TID_CROSSHAIR2,
                            TID_LENS1,
                            TID_LENS2,
                            TID_LENS3,
                            TID_LENS4,
                            TID_SHOOT1,
                            TID_SHOOT2,
                            TID_ASTEROID,
                            TID_ASTEROID_EMISSIVE,
                            TID_ASTEROID_BUMP,
                            TID_SHADEBOB,
                            TID_FLYER_RED, 
                            TID_FLYER_RED_EMISSIVE, 
                            TID_LEADER_RED,
                            TID_LEADER_RED_EMISSIVE,
                            TID_FLYER_GREEN, 
                            TID_FLYER_GREEN_EMISSIVE, 
                            TID_LEADER_GREEN,
                            TID_LEADER_GREEN_EMISSIVE,
                            TID_MINE,
                            TID_MINE_EMISSIVE,
                            TID_RED_FLARE,
                            TID_PLANET,

                            TID_ELEKTRON,
                            TID_FENSTERF,
                            TID_FENSTERF_EM,
                            TID_REAKTORB,
                            TID_REAKTORB_EM,
                            TID_ALIEN_PO,
                            TID_ALIEN_PO_EM,
                            TID_A_HULL_M,
                            TID_B_HULL_M,
                            TID_B_HULL_M_EM,
                            TID_C_HULL_M,
                            TID_D_HULL_M,
                            TID_D_HULL_M_EM,
                            TID_EXTRAS,
                            TID_EXTRAS_EM,

                            TID_SYMBOL,
                            TID_FONT,
                            TID_CORONA,
                            TID_SPACEWARP02,
                            TID_RADAR01,
                            TID_SMOKE,
                            TID_ARCNEBULA,

                            TID_SCHLAUCH,
                            TID_SCHLAUCH_EM,

                            TID_HUNTER,
                            TID_HUNTER_NORMALS,

                            TID_HUNTER01,
                            TID_HUNTER01_NORMALS,
                            TID_HUNTER01_EM,
                            TID_HUNTER02,
                            TID_HUNTER02_NORMALS,
                            TID_HUNTER02_EM,
                            TID_HUNTER03,
                            TID_HUNTER03_NORMALS,
                            TID_HUNTER03_EM,
                            TID_HUNTER09,
                            TID_HUNTER09_NORMALS,
                            TID_HUNTER09_EM,
                            TID_HUNTER10,
                            TID_HUNTER10_NORMALS,
                            TID_HUNTER10_EM,

                            TID_CORIOLIS_EXTERIOR,
                            TID_CORIOLIS_EXTERIOR_NORMALS,
                            TID_CORIOLIS_EXTERIOR_EM,
                            TID_CORIOLIS_CORE,
                            TID_CORIOLIS_CORE_NORMALS,
                            TID_CORIOLIS_CORE_EM,
                            TID_BOA,
                            TID_BOA_NORMALS,
                            TID_BOA_EM,

                            TID_CARGO01,
                            TID_CARGO01_NORMALS,
                            TID_CARGO01_EM,
                            TID_CARGO03,
                            TID_CARGO03_NORMALS,
                            TID_CARGO03_EM,
                            TID_CARGO04,
                            TID_CARGO04_NORMALS,
                            TID_CARGO04_EM,
                            TID_CARGO05,
                            TID_CARGO05_NORMALS,
                            TID_CARGO05_EM,
                            TID_CARGO06,
                            TID_CARGO06_NORMALS,
                            TID_CARGO06_EM,
                            TID_CARGO07,
                            TID_CARGO07_NORMALS,
                            TID_CARGO08,
                            TID_CARGO08_NORMALS,
                            TID_CARGO08_EM,
                            TID_TRANSPORT1,
                            TID_TRANSPORT1_NORMALS,
                            TID_TRANSPORT1_EM,

                            TID_CARGOBOX,
                            TID_CARGOBOX_NORMALS,

                            TID_LASER,
                            TID_LASER_NORMALS,
                            TID_LASER_EM,

                            TID_SBHANGAR,
                            TID_SBHANGAR_NORMALS,
                            TID_SBHANGAR_EM,

                            TID_ILLUMINATION,

                            TID_RESCUE,
                            TID_RESCUE_NORMALS,
                            TID_RESCUE_EM,

                            TID_MISSILE,
                            TID_MISSILE_NORMALS,
                            TID_MISSILE_EM,

                            TID_MINE2,
                            TID_MINE2_NORMALS,

                            TID_COBRA,
                            TID_COBRA_NORMALS,
                            TID_COBRA_EM,

                            TID_COBRA_EXTRA,
                            TID_COBRA_EXTRA_NORMALS,
                            TID_COBRA_EXTRA_EM,

                            TID_THARGOID,
                            TID_THARGOID_EM,
                            TID_THARGOID_NORMALS,

                            TID_COCKPIT,
							TID_MENUITEM,
							TID_MENUITEM_HOVER,
                            TID_LASTENUM
                          } TEXTUREID;

typedef enum en_logoid { TID_OWNLOGO = 0,
                         TID_COVER
                       } LOGOID;


typedef struct _texturersc { TEXTUREID idxTexture;
                             int MipLevel;
                             char* name; } TEXTURERSC;

typedef struct _txtsize { int width; int height;} TEXTURESIZE;

class TEXTURE {
    public:
       int m_iWidth, m_iHeight;
       int m_idxTxtRsc;
       int m_cntTxtRsc;
       bool m_bMipMapped;
       bool m_bFileExists;
	   ID3D11Texture2D* m_pd3dTexture;
       ID3D11ShaderResourceView*  m_pd3dTextureRV;
       DXGI_FORMAT m_d3dformatDest;
       bool activate(int);
  
	   
	   bool readFromHDD();
};



#endif
