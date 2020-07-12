/*
 *------------------------------------------------------------------------
 *      Object: MESHRESOURCE
 *         $Id: psxres.h,v 1.17 2010/02/04 15:40:00 molle Exp $
 *        Date: Mar 2005
 *       Notes: Verwalten und Laden der Meshresourcen
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

typedef enum _restype { 
   RESTYPE_FLYER = 0, 
   RESTYPE_LEADER, 
   RESTYPE_ASTEROID,
   RESTYPE_ASTEROIDSCALE2,
   RESTYPE_HANGAR,
   RESTYPE_GUN,
   RESTYPE_MINE,
   RESTYPE_MINE_LOWPOLY,
   RESTYPE_MINE_LOWPOLY2,
   RESTYPE_PLANET,
   RESTYPE_SKYBALL,
   RESTYPE_REACTOR,
   RESTYPE_POINTER01,
   RESTYPE_SPACEWARP,
   RESTYPE_BRENNSTAB,
   RESTYPE_FELDGENERATOR,
   RESTYPE_RADAR,
   RESTYPE_TMPSPEEDUP,
   RESTYPE_AG,
   RESTYPE_BG,
   RESTYPE_CG,
   RESTYPE_DG,
   RESTYPE_EG,
   RESTYPE_FG,
   RESTYPE_GG,
   RESTYPE_HG,
   RESTYPE_IG,
   RESTYPE_JG,
   RESTYPE_KG,
   RESTYPE_LG,
   RESTYPE_MG,
   RESTYPE_NG,
   RESTYPE_OG,
   RESTYPE_PG,
   RESTYPE_QG,
   RESTYPE_RG,
   RESTYPE_SG,
   RESTYPE_TG,
   RESTYPE_UG,
   RESTYPE_VG,
   RESTYPE_WG,
   RESTYPE_XG,
   RESTYPE_YG,
   RESTYPE_ZG,
   RESTYPE_SPACE,
   RESTYPE_BACKSPACE,
   RESTYPE_ENTER,
   RESTYPE_HUNTER_X,
   RESTYPE_BOA01,
   RESTYPE_CARGO03,
   RESTYPE_CARGO04,
   RESTYPE_CARGO05,
   RESTYPE_CARGO06,
   RESTYPE_CARGO07,
   RESTYPE_CARGO08,
   RESTYPE_TRANSPORT1,
   RESTYPE_CARGOFLYER,
   RESTYPE_HUNTER01,
   RESTYPE_HUNTER02,
   RESTYPE_HUNTER03,
   RESTYPE_HUNTER09,
   RESTYPE_HUNTER10,
   RESTYPE_WINGMAN,
   RESTYPE_COBRA_PART1,
   RESTYPE_COBRA_MOUNTS,
   RESTYPE_COBRA_REAR,
   RESTYPE_COBRA_WEAPONFRONT,
   RESTYPE_COBRA_WEAPONLEFT,
   RESTYPE_COBRA_WEAPONREAR,
   RESTYPE_COBRA_WEAPONRIGHT,
   RESTYPE_MINE2,
	RESTYPE_DAZ3D_CUBE,
   RESTYPE_MISSILE,
   RESTYPE_RESCUE,
   RESTYPE_CARGOBOX,
   RESTYPE_STARBASEHANGAR,
   RESTYPE_CORIOLISKINGSPLANE,
   RESTYPE_FLAGSHIPKINGSPLANE,
   RESTYPE_THARGOID,
   RESTYPE_THARGOIDDRONE,
   RESTYPE_DOOR01A,
   RESTYPE_DOOR01B,
   RESTYPE_DOOR01C,
   RESTYPE_DOOR02A,
   RESTYPE_DOOR02B,
   RESTYPE_CORIOLIS_CORE,
   RESTYPE_CORIOLIS_EXTERIOR,
   RESTYPE_LASERGUN,
   RESTYPE_LASERHALTER,
   RESTYPE_LASERHALTERAUSTRITT,
   RESTYPE_CARGO01,

/*
   RESTYPE_FS_KOLLEKTORSTANGEN,        
   RESTYPE_FS_KOLLEKTOR1,              
   RESTYPE_FS_RUMPF_PLATTEN_ST,        
   RESTYPE_FS_RUMPF_PLA1,              
   RESTYPE_FS_BAUCH_ELEKTRONIK,        
   RESTYPE_FS_HAUPTSTEGE_1,            
   RESTYPE_FS_REAKTOR_01_A,            
   RESTYPE_FS_RUMPF_PLA2,              
   RESTYPE_FS_RUMPF_PLA3,              
   RESTYPE_FS_ANTRIEB_1_B,             
   RESTYPE_FS_RUMPF_PLA4,              
   RESTYPE_FS_ANTRIEB_1_A,             
   RESTYPE_FS_ANTRIEB_1_C,             
   RESTYPE_FS_ZENTRALE_1_BASE_,        
   RESTYPE_FS_RUMPF_02,                
   RESTYPE_FS_RUMPF_04,                
   RESTYPE_FS_RUMPF_PLA5,              
   RESTYPE_FS_RUMPF_PLA6,              
   RESTYPE_FS_RUMPF_06,                
   RESTYPE_FS_RUMPF_PLA7,              
   RESTYPE_FS_RUMPF_PLA8,              
   RESTYPE_FS_RUMPF_01,                
   RESTYPE_FS_RUMPF_03,                
   RESTYPE_FS_RUMPF_PLA9,              
   RESTYPE_FS_RUMPF_PL10,              
   RESTYPE_FS_RUMPF_05,                
   RESTYPE_FS_RUMPF_PL11,              
   RESTYPE_FS_RUMPF_PL12,              
   RESTYPE_FS_RUMPF_PL13,              
   RESTYPE_FS_RUMPF_PL14,              
   RESTYPE_FS_RUMPF_PL15,              
   RESTYPE_FS_TUNNEL_FRONT,            
   RESTYPE_FS_KOLLEKTORRING_1_,        
   RESTYPE_FS_KOLLEKTOR2,              
   RESTYPE_FS_KOLLEKTORRING_2_,        
   RESTYPE_FS_KOLLEKTOR3,              
   RESTYPE_FS_KOLLEKTORRING_3_,        
   RESTYPE_FS_KOLLEKTOR4,              
   RESTYPE_FS_ZENTRALE_1_D,            
   RESTYPE_FS_ZENTRALE_1_C,            
   RESTYPE_FS_ZENTRALE_1_B,            
   RESTYPE_FS_ZENTRALE_1_A,            
   RESTYPE_FS_TUNNEL_BACK,             
   RESTYPE_FS_KOLLEKTORRING_4_,        
   RESTYPE_FS_KOLLEKTOR5,              
   RESTYPE_FS_SCHLAUCH,                
   RESTYPE_FS_ZENTRALE_2_C,            
   RESTYPE_FS_ZENTRALE_2,              
   RESTYPE_FS_STUTZEN,                 
   RESTYPE_FS_ZENTRALE_1_E,            
   RESTYPE_FS_AUFBAUTEN,               
   RESTYPE_FS_ZENTRALE_2_A,            
   RESTYPE_FS_ZENTRALE_2_B,            
   RESTYPE_FS_WOHNINSEL_1_TURM,        
   RESTYPE_FS_KOLLEKTOR6,              
   RESTYPE_FS_WOHNINSEL_1_UNTE,        
   RESTYPE_FS_WOHNINSEL1,              
   RESTYPE_FS_WOHNINSEL_1_DECK,        
   RESTYPE_FS_WOHNINSEL_1_DACH,        
   RESTYPE_FS_TUNNEL,                  
   RESTYPE_FS_STADT,                   
   RESTYPE_FS_RUMPF_ZEICHEN,           
   RESTYPE_FS_METALL,                  

   // -- innenteil
   //
   RESTYPE_FS_TUNNEL_INNENSEIT_2,      
   RESTYPE_FS_WOHNINSEL_1_DECK_2,      
   RESTYPE_FS_ZENTRALE_2_A_2,          
   RESTYPE_FS_KOLLEKTORSTANGEN_2,      
   RESTYPE_FS_KOLLEKTOR1_2,            
   RESTYPE_FS_KOLLEKTOR2_2,            
   RESTYPE_FS_SCHLAUCH_2,              
   RESTYPE_FS_REAKTOR_INNEN_01_2,      
   RESTYPE_FS_REAKTOR_I1_2,            
   RESTYPE_FS_REAKTOR_01_A_2,          
   RESTYPE_FS_REAKTOR_I2_2,            
   RESTYPE_FS_REAKTOR_I3_2,            
   RESTYPE_FS_ZENTRALE_2_2,
*/

   RESTYPE_LASTENUM
} RESTYPE;

class MESHRESOURCEMANAGER
{
   protected:
      void* m_pMeshResourceBlock;

      int m_iVertices, 
          m_iFaces, 
          m_iMaterials, 
          m_iBigRes, 
          m_iBigResItem,
          m_iFaceInf,
          m_iPFaceInf;

      PSXRESOURCEOFFSET* m_resArray;
      PSXFACEINFO*       m_faceinfArray;
      PSXFACEINFO**      m_pfaceinfArray;
//      PSXDOT3EXTENSION*  m_dt3extArray;
      
      int m_cntRes, m_cntFace;

      unsigned long m_cntMaterialBytes,
                    m_cntVertexBytes  ,
                    m_cntResourceBytes ,
                    m_cntFaceBytes,
                    m_cntBigResBytes, 
                    m_cntBigResItemBytes,
                    m_cntFaceInfBytes,
                    m_cntPFaceInfBytes;

      HANDLE m_hVerticesFile, m_hMapVertices,
             m_hFacesFile,    m_hMapFaces,
             m_hFaceInfFile,  m_hMapFaceInf;

      ID3D11Buffer**  m_vertbufArray;
      ID3D11Buffer**   m_indexbufArray;

      dTriMeshDataID* m_trimeshdataidArray;

   public:
      MESHRESOURCEMANAGER();
      PSXFACEINFO*       getFaceInfArray(RESTYPE);
      
      bool init();
      bool loadPureMesh();
      bool initBigRes();
      bool recalcMinMax();
      bool generateBigRes();
      bool modifyMeshes();
      bool scaleFlagshipDoors();
      bool initAllFaceInfoArrays();
      bool locatePFaceInfArray();
      bool updateBigRes();
      bool createVertexBuffers();
      bool releaseVertexBuffers();
      bool initFaceInfoArray(PSXFACEINFO**, RESTYPE );
      bool release();
      bool scaler();
      bool save();
      bool allocate();
      bool setIndices(RESTYPE);
      bool setStreamSource(int, RESTYPE);
      bool createTriMeshData(RESTYPE l_idxRes);
      bool createIndexBuffer(RESTYPE l_idxRes);
      bool createVertexBuffer(RESTYPE l_idxRes);
 
      int getMaxFaces();

      // -- getter
      //
      int            getVertexCount(RESTYPE);
      PSXVERTEX*     getVertexArray(RESTYPE);
      int            getFaceCount(RESTYPE);
      PSXFACE*       getFaceArray(RESTYPE);
      float          getRad(RESTYPE);
      int            getMaterialCount(RESTYPE);
      PSXMATERIAL*   getMaterialArray(RESTYPE);
      dTriMeshDataID getTriMeshDataID(RESTYPE l_idxRes);
      float          getSideLengthX(RESTYPE);
      float          getSideLengthY(RESTYPE);
      float          getSideLengthZ(RESTYPE);
      float          getMiddleX(RESTYPE);
      float          getMiddleY(RESTYPE);
      float          getMiddleZ(RESTYPE);

};

