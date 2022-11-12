/*
 *------------------------------------------------------------------------
 *      Object: TEXTURE
 *         $Id: psxtxt.cpp,v 1.8 2009/12/07 20:56:00 molle Exp $
 *        Date: Jun 2005
 *       Notes: Verwalten und Laden der Texturen
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

#include "stdafx.h"
#include "psxd3d.h"
#include "psxglob.h"

struct DDSURFACEFORMAT
{
    uint32_t    size;
    uint32_t    flags;
    uint32_t    fourCC;
    uint32_t    RGBBitCount;
    uint32_t    RBitMask;
    uint32_t    GBitMask;
    uint32_t    BBitMask;
    uint32_t    ABitMask;
};

struct DDSURFACEDESC2
{
    uint32_t        size;
    uint32_t        flags;
    uint32_t        height;
    uint32_t        width;
    uint32_t        pitchOrLinearSize;
    uint32_t        depth; // only if DDS_HEADER_FLAGS_VOLUME is set in flags
    uint32_t        mipMapCount;
    uint32_t        reserved1[11];
    DDSURFACEFORMAT ddspf;
    uint32_t        caps;
    uint32_t        caps2;
    uint32_t        caps3;
    uint32_t        caps4;
    uint32_t        reserved2;
};


// -- richtet das globale TexturArray ein und prüft auf FileExists
//
bool PSXInitTextureArray()
{
   int l_idxTexture, l_idxFile;
   
   WriteLogFile("check Texture-Bitmap integrity\n"); 
   memset (g_txtArray, 0, sizeof(TEXTURE) * TID_LASTENUM);

   for (l_idxFile = 0; l_idxFile < g_game.getTextureRscCount(); l_idxFile++)
   {
      DDSURFACEDESC2 l_ddsdProxy   = {0};

      HANDLE l_hBmpFile;
      unsigned long l_ulFileLength, l_ulFileRest;

	  char l_cCompletePath[200];
      char l_cCreateTexture[1000];

      CHAR l_tcTxtFile[MAX_PATH];
      CHAR* l_tcOnlyFilename;

      l_idxTexture = g_game.m_txtrscArray[l_idxFile].idxTexture;

      // -- erhoehen, sonst wird die texture als miplevel der haupttexture geladen
      //
      if (g_game.m_txtrscArray[l_idxFile].MipLevel == 0)
      {
         if (g_txtArray[l_idxTexture].m_bFileExists)
         {  
             char l_cFuncText[100];
             PSXPrintf(__LINE__,l_cFuncText, sizeof(l_cFuncText), 
                       "file %s.dds want to occupy an already associated Texture-ID\n", 
                       g_game.m_txtrscArray[l_idxFile].name);
             hr2message(__LINE__,-1,l_cFuncText);
             return false;
         }
         g_txtArray[l_idxTexture].m_bMipMapped = false; 
         g_txtArray[l_idxTexture].m_idxTxtRsc = l_idxFile; 
         g_txtArray[l_idxTexture].m_cntTxtRsc++;
      }else{
		  g_txtArray[l_idxTexture].m_bMipMapped = true; // ab zweiten level mipmapping freischalten
		  g_txtArray[l_idxTexture].m_cntTxtRsc++;
      }
      g_txtArray[l_idxTexture].m_bFileExists = true; // zunaechst initialisieren
      
	  PSXStrCpy(__LINE__,l_cCompletePath, sizeof(l_cCompletePath), "textures\\");
      PSXStrCat(__LINE__,l_cCompletePath, sizeof(l_cCompletePath), g_game.m_txtrscArray[l_idxFile].name);
      
      if (SearchPathA( NULL, l_cCompletePath, ".dds", 
                      sizeof(l_tcTxtFile), l_tcTxtFile, &l_tcOnlyFilename ) == false)
      {
         char l_cFuncText[100];
		 PSXPrintf(__LINE__,l_cFuncText, sizeof(l_cFuncText), "can't find Texturefile %s.dds \n", l_cCompletePath);
         hr2message(__LINE__,GetLastError(),l_cFuncText);
         
         return false;
      }
      //PSXStrCat(__LINE__,l_cCompletePath,sizeof(l_cCompletePath), ".dds");

      l_hBmpFile =  CreateFileA(l_tcTxtFile,
                               GENERIC_READ ,
                               0 ,
                               NULL ,
                               OPEN_EXISTING ,
                               FILE_ATTRIBUTE_NORMAL ,
                               NULL );	

      if (l_hBmpFile==INVALID_HANDLE_VALUE) 
      {
         char l_cFuncText[100];
         PSXPrintf(__LINE__,l_cFuncText, sizeof(l_cFuncText), "CreateFile(%s)\n", l_cCompletePath);
         hr2message(__LINE__,GetLastError(),l_cFuncText);
         
         return false;
      }
      l_ulFileLength = GetFileSize(l_hBmpFile,NULL);
      l_ulFileRest   = l_ulFileLength;

      // -- Read magic values from dds-file
      //
      {
         ULONG l_dwBytesRead;
         DWORD l_dwMagic;

         ReadFile(l_hBmpFile, (void*)&l_dwMagic, sizeof(l_dwMagic), &l_dwBytesRead, NULL);
     
         if (l_dwMagic != ' SDD') // DDS_
         {
            char l_cIsNotDDSFile[100];
            PSXPrintf(__LINE__,l_cIsNotDDSFile, sizeof(l_cIsNotDDSFile), 
                    "This is not a dds file: %s - aborting", 
                    g_game.m_txtrscArray[l_idxFile].name);

            hr2message(__LINE__,-1,l_cIsNotDDSFile);
            return false;
         }
         l_ulFileRest = l_ulFileRest - l_dwBytesRead;
      }
      
      // -- Reading DDSURFACEDESC
      //
      {
         ULONG l_dwBytesRead;
         ReadFile(l_hBmpFile, (void*)&l_ddsdProxy, sizeof(l_ddsdProxy), &l_dwBytesRead, NULL);

         if (l_ddsdProxy.size != sizeof(DDSURFACEDESC2))
         {
            char l_cHeaderWrong[100];
            PSXPrintf(__LINE__,l_cHeaderWrong,sizeof(l_cHeaderWrong),
                    "Header is not DDSURFACEDESC2: %s - aborting\n",
                    g_game.m_txtrscArray[l_idxFile].name);

            hr2message(__LINE__,-1,l_cHeaderWrong);
            return false;
         }
         l_ulFileRest = l_ulFileRest - l_dwBytesRead;

      }

      PSXPrintf(__LINE__,l_cCreateTexture, sizeof(l_cCreateTexture), 
              "%s: g_pDev->CreateTexture(width=%i, height=%i,", 
               g_game.m_txtrscArray[l_idxFile].name,
               l_ddsdProxy.width,
               l_ddsdProxy.height);

      switch (l_ddsdProxy.ddspf.fourCC)
      {   
         case '1TXD': 
            g_txtArray[l_idxTexture].m_d3dformatDest = DXGI_FORMAT_BC1_UNORM; 
            PSXStrCat(__LINE__,l_cCreateTexture, sizeof(l_cCreateTexture), "format=DXT1");
            break; // DXT1 ..
         case '2TXD': 
			// While pre-mulitplied alpha isn't directly supported by the DXGI formats,
            // they are basically the same as these BC formats so they can be mapped
            g_txtArray[l_idxTexture].m_d3dformatDest = DXGI_FORMAT_BC2_UNORM; 
            PSXStrCat(__LINE__,l_cCreateTexture, sizeof(l_cCreateTexture), "format=DXT2");
            break;
         case '3TXD': 
            g_txtArray[l_idxTexture].m_d3dformatDest = DXGI_FORMAT_BC2_UNORM; 
            PSXStrCat(__LINE__,l_cCreateTexture, sizeof(l_cCreateTexture), "format=DXT3");
            break;
         case '4TXD': 
 			// While pre-mulitplied alpha isn't directly supported by the DXGI formats,
            // they are basically the same as these BC formats so they can be mapped

            g_txtArray[l_idxTexture].m_d3dformatDest = DXGI_FORMAT_BC3_UNORM; 
            PSXStrCat(__LINE__,l_cCreateTexture, sizeof(l_cCreateTexture), "format=DXT4");
            break;
         case '5TXD': 
            g_txtArray[l_idxTexture].m_d3dformatDest = DXGI_FORMAT_BC3_UNORM; 
            PSXStrCat(__LINE__,l_cCreateTexture, sizeof(l_cCreateTexture), "format=DXT5");
            break;

		case '1ITA':
            g_txtArray[l_idxTexture].m_d3dformatDest = DXGI_FORMAT_BC4_UNORM;
            PSXStrCat(__LINE__,l_cCreateTexture, sizeof(l_cCreateTexture), "format=ATI1");
            break;
        case 'U4CB':
            g_txtArray[l_idxTexture].m_d3dformatDest = DXGI_FORMAT_BC4_UNORM;
            PSXStrCat(__LINE__,l_cCreateTexture, sizeof(l_cCreateTexture), "format=BC4U");
            break;
        case 'S4CB':
            g_txtArray[l_idxTexture].m_d3dformatDest = DXGI_FORMAT_BC4_SNORM;
            PSXStrCat(__LINE__,l_cCreateTexture, sizeof(l_cCreateTexture), "format=BC4S");
            break;

        case '2ITA':
            g_txtArray[l_idxTexture].m_d3dformatDest = DXGI_FORMAT_BC5_UNORM;
            PSXStrCat(__LINE__,l_cCreateTexture, sizeof(l_cCreateTexture), "format=ATI2");
            break;
        case 'U5CB':
            g_txtArray[l_idxTexture].m_d3dformatDest = DXGI_FORMAT_BC5_UNORM;
            PSXStrCat(__LINE__,l_cCreateTexture, sizeof(l_cCreateTexture), "format=BC5U");
            break;
        case 'S5CB':
            g_txtArray[l_idxTexture].m_d3dformatDest = DXGI_FORMAT_BC5_SNORM;
            PSXStrCat(__LINE__,l_cCreateTexture, sizeof(l_cCreateTexture), "format=BC5S");
            break;

        // BC6H and BC7 are written using the "DX10" extended header

		case 'GBGR':
            g_txtArray[l_idxTexture].m_d3dformatDest = DXGI_FORMAT_R8G8_B8G8_UNORM;
            PSXStrCat(__LINE__,l_cCreateTexture, sizeof(l_cCreateTexture), "format=RGBG");
            break;
		case 'BGRG':
            g_txtArray[l_idxTexture].m_d3dformatDest = DXGI_FORMAT_G8R8_G8B8_UNORM;
            PSXStrCat(__LINE__,l_cCreateTexture, sizeof(l_cCreateTexture), "format=GRGB");
            break;

         //case '8V8U': 
         //   g_txtArray[l_idxTexture].m_d3dformatDest = D3DFMT_V8U8; 
         //   if (g_bEnableBumpMapping == false)
         //   {
         //      g_txtArray[l_idxTexture].m_d3dformatDest = D3DFMT_R5G6B5; // liebloser ersatz
         //   }
         //   PSXStrCat(__LINE__,l_cCreateTexture, sizeof(l_cCreateTexture), "format=V8U8");
         //   break; // bumpmap
         //default: 
         //   g_txtArray[l_idxTexture].m_d3dformatDest = D3DFMT_A8R8G8B8;//D3DFMT_R5G6B5;
         //   PSXStrCat(__LINE__,l_cCreateTexture, sizeof(l_cCreateTexture), "format=A8R8G8B8");
			
      }

      PSXStrCat(__LINE__, l_cCreateTexture, sizeof(l_cCreateTexture), ");\n");
      WriteLogFile(l_cCreateTexture);

      if (g_game.m_txtrscArray[l_idxFile].MipLevel == 0)
      {
         g_txtArray[l_idxTexture].m_pd3dTexture = NULL;
         g_txtArray[l_idxTexture].m_iWidth  = l_ddsdProxy.width;
         g_txtArray[l_idxTexture].m_iHeight = l_ddsdProxy.height;
      }

      CloseHandle(l_hBmpFile);
                         
   } //end for

   return true;
}
// -- saemtliche Textures rausschmeissen
//
bool PSXReleaseAllTextures()
{
   int l_idxTexture;
   HRESULT hr;

   if (g_txtArray)
   {
	   for (l_idxTexture = 0; l_idxTexture < TID_LASTENUM; l_idxTexture++)
	   {
		   if ( g_txtArray[l_idxTexture].m_pd3dTextureRV )
		   {
			   hr = g_txtArray[l_idxTexture].m_pd3dTextureRV->Release();
			   if (hr != 0) { 
				   hr2message(__LINE__,hr,"m_pd3dTextureRV->Release()");
				   return false;
			   }
			   g_txtArray[l_idxTexture].m_pd3dTextureRV = NULL;
		   }
		   if ( g_txtArray[l_idxTexture].m_pd3dTexture )
		   {
			   hr = g_txtArray[l_idxTexture].m_pd3dTexture->Release();
			   if (hr != 0) { 
				   hr2message(__LINE__,hr,"m_pd3dTexture->Release()");
				   return false;
			   }
			   g_txtArray[l_idxTexture].m_pd3dTexture = NULL;
		   }
	   }
   }
   return true;
}


bool TEXTURE::activate(int l_iStage)
{
  

   if (m_pd3dTexture == NULL)
   {
      if (m_bFileExists)
      {
         if (readFromHDD()==false) return false;
		  
      }else{
         hr2message(__LINE__,-1, "activating Texture that is not defined in this game\n");
         return false;
      }
   }
   g_pImmediateContext->PSSetShaderResources( l_iStage	, 1, &m_pd3dTextureRV);
   
   return true;
}

bool TEXTURE::readFromHDD()
{
   HRESULT hr;
   DDSURFACEDESC2     l_ddsdProxy   = {0};

   HANDLE l_hBmpFile = 0;

   unsigned long l_ulFileLength, l_ulFileRest;
   char l_cCompletePath[200];

   CHAR l_tcTxtFile[MAX_PATH];
   CHAR* l_tcOnlyFilename;
   
   uint8_t** ddsDataArray = new uint8_t*[m_cntTxtRsc];

   D3D11_SUBRESOURCE_DATA* InitDataArray = new D3D11_SUBRESOURCE_DATA[m_cntTxtRsc];
   ZeroMemory( InitDataArray, sizeof(InitDataArray) * m_cntTxtRsc);
   D3D11_TEXTURE2D_DESC desc = {0};

   int l_idxTxtRsc = 0;

   ZeroMemory( &desc, sizeof(desc) );

   desc.Width = static_cast<UINT>( m_iWidth);
   desc.Height = static_cast<UINT>( m_iHeight);
   desc.MipLevels = static_cast<UINT>( m_cntTxtRsc);
   desc.ArraySize = static_cast<UINT>( 1 );

   desc.Format = m_d3dformatDest;
   desc.SampleDesc.Count = 1;
   desc.SampleDesc.Quality = 0;
   desc.Usage = D3D11_USAGE_DEFAULT;
   desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
   desc.CPUAccessFlags = 0;//D3D11_CPU_ACCESS_WRITE;

   for (l_idxTxtRsc = 0; l_idxTxtRsc < m_cntTxtRsc; l_idxTxtRsc++)
   {
      int l_idxFile;
   
      l_idxFile = m_idxTxtRsc + l_idxTxtRsc;
	
	  // -- Datei öffnen
	  //
	  //PSXStrCat(__LINE__,l_cCompletePath,sizeof(l_cCompletePath), ".dds");
	  PSXStrCpy(__LINE__,l_cCompletePath, sizeof(l_cCompletePath), "textures\\");
	  PSXStrCat(__LINE__,l_cCompletePath, sizeof(l_cCompletePath), g_game.m_txtrscArray[l_idxFile].name);

	  if (SearchPathA( NULL, l_cCompletePath, ".dds", 
		  sizeof(l_tcTxtFile), l_tcTxtFile, &l_tcOnlyFilename ) == false)
	  {
		  char l_cFuncText[100];
		  PSXPrintf(__LINE__,l_cFuncText, sizeof(l_cFuncText), "can't find Texturefile %s.dds \n", l_cCompletePath);
		  hr2message(__LINE__,GetLastError(),l_cFuncText);

		  return false;
	  }

	  // Load the Texture
	  l_hBmpFile =  CreateFileA(l_tcTxtFile,
		  GENERIC_READ ,
		  0 ,
		  NULL ,
		  OPEN_EXISTING ,
		  FILE_ATTRIBUTE_NORMAL ,
		  NULL );	

	  if (l_hBmpFile==INVALID_HANDLE_VALUE) 
	  {
		  hr2message(__LINE__,GetLastError(),"CreateFile in TEXTURE::readFromHDD");
		  return false;
	  }

	  l_ulFileLength = GetFileSize(l_hBmpFile,NULL);
	  l_ulFileRest   = l_ulFileLength;

	  // -- Read magic values from dds-file
	  //
	  {
		  ULONG l_dwBytesRead;
		  DWORD l_dwMagic;
		  hr = ReadFile( l_hBmpFile, (void*)&l_dwMagic, sizeof(l_dwMagic), &l_dwBytesRead, NULL);
		  if (l_dwMagic != ' SDD') // DDS_
		  {
			  hr2message(__LINE__,-1, "This is not a dds file - aborting");
			  return false;
		  }
		  l_ulFileRest = l_ulFileRest - l_dwBytesRead;
	  }

	  // -- Reading DDSURFACEDESC
	  //
	  {
		  ULONG l_dwBytesRead;          
		  ReadFile( l_hBmpFile, (void*)&l_ddsdProxy, sizeof(l_ddsdProxy), &l_dwBytesRead, NULL);

		  if (l_ddsdProxy.size != sizeof(DDSURFACEDESC2))
		  {
			  hr2message(__LINE__,-1, "Header is not DDSURFACEDESC2 - aborting");
			  return false;
		  }
		  l_ulFileRest = l_ulFileRest - l_dwBytesRead;

	  }

	  // create enough space for the file data

	  ddsDataArray[l_idxTxtRsc] = new uint8_t[ l_ulFileRest ];

	  if (!ddsDataArray[l_idxTxtRsc])
	  {
		  hr2message(__LINE__,E_OUTOFMEMORY, "new[texturefilelength]");
		  return false;
	  }
	  ZeroMemory( ddsDataArray[l_idxTxtRsc], sizeof(l_ulFileRest) );

	  // -- load rest of dds directly into locked memory

	  ULONG l_dwBytesRead;
	  ReadFile(l_hBmpFile, ddsDataArray[l_idxTxtRsc], l_ulFileRest, &l_dwBytesRead, NULL);
 
	  size_t NumBytes = 0;
	  size_t RowBytes = 0;
	  size_t NumRows = 0;
	  
	  PSXGetSurfaceInfo( l_ddsdProxy.width, l_ddsdProxy.height, m_d3dformatDest,&NumBytes,&RowBytes,&NumRows);

	  InitDataArray[l_idxTxtRsc].pSysMem = static_cast <const void *> (ddsDataArray[l_idxTxtRsc]);
	  InitDataArray[l_idxTxtRsc].SysMemPitch = RowBytes;
	  InitDataArray[l_idxTxtRsc].SysMemSlicePitch = l_ulFileRest;//NumBytes;
   }

   // -- Behaelterobjekt für Haupttextur und untergeordnete Mipmaps
   //
   hr = g_pd3dDevice->CreateTexture2D( &desc,
	   InitDataArray,
	   &m_pd3dTexture
	   );

   if FAILED(hr) { 
	   hr2message(__LINE__,hr,"g_pDev->CreateTexture() in TEXTURE::readFromHDD");
	   return false;
   }

   // -- free allocated dds-image-space
   //
   for (l_idxTxtRsc = 0; l_idxTxtRsc < m_cntTxtRsc; l_idxTxtRsc++)
   {
	   delete [] ddsDataArray[l_idxTxtRsc];
   }

   delete [] ddsDataArray;
   CloseHandle(l_hBmpFile);

   delete [] InitDataArray;

   D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
   ZeroMemory( &SRVDesc, sizeof(SRVDesc));
   SRVDesc.Format = m_d3dformatDest;
   SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
   SRVDesc.Texture2D.MipLevels = desc.MipLevels;
   hr = g_pd3dDevice->CreateShaderResourceView( m_pd3dTexture,
	   &SRVDesc,
	   &m_pd3dTextureRV
	   );
   if ( FAILED(hr) )
   {
	   m_pd3dTexture->Release();
	   hr2message(__LINE__,hr,"g_pDev->CreateShaderResourceView() in TEXTURE::readFromHDD");
	   return false;
   }

   //} // end-for idx_miplevel

   return true;

}

// -- Default Texturen
//
bool GAME::associateTextureRsc()
{

   static TEXTURERSC l_txtrscMain[] = { 
                              {TID_NULL, 0,"nebula1"},
							  {TID_NEBULA2, 0,"nebula2"},
                              {TID_NEBULA3, 0,"nebula3"},
                              {TID_NEBULA4, 0,"nebula4"},
                              {TID_NEBULA5, 0,"nebula5"},
                              {TID_NEBULA6, 0,"nebula6"},
                              {TID_NEBULA7, 0,"nebula7"},
                              {TID_NEBULA8, 0,"nebula8"},
                              {TID_CROSSHAIR, 0,"crosshair"},
                              {TID_CROSSHAIR2, 0,"crosshair2"},
                              {TID_LENS1, 0,"lens1"},
                              {TID_LENS2, 0,"lens2"},
                              {TID_LENS3, 0,"lens3"},
                              {TID_LENS4, 0,"lens4"},
                              {TID_SHOOT1, 0,"schuss1"},
                              {TID_SHOOT2, 0,"schuss2"},

                              {TID_ASTEROID, 0,"asteroid"},
                              {TID_ASTEROID, 1,"asteroid2"},
                              {TID_ASTEROID, 2,"asteroid3"},
                              {TID_ASTEROID, 3,"asteroid4"},
                              {TID_ASTEROID, 4,"asteroid5"},
                              {TID_ASTEROID, 5,"asteroid6"},
                              {TID_ASTEROID, 6,"asteroid7"},
                              {TID_ASTEROID, 7,"asteroid8"},
                              {TID_ASTEROID_EMISSIVE, 0,"asteroid_em"},
                              {TID_ASTEROID_EMISSIVE, 1,"asteroid_em2"},
                              {TID_ASTEROID_EMISSIVE, 2,"asteroid_em3"},
                              {TID_ASTEROID_EMISSIVE, 3,"asteroid_em4"},
                              {TID_ASTEROID_EMISSIVE, 4,"asteroid_em5"},
                              {TID_ASTEROID_EMISSIVE, 5,"asteroid_em6"},
                              {TID_ASTEROID_EMISSIVE, 6,"asteroid_em7"},
                              {TID_ASTEROID_EMISSIVE, 7,"asteroid_em8"},
                              {TID_ASTEROID_BUMP, 0,"asteroid_bump"},
                              {TID_SHADEBOB, 0,"shadebob"},
                              {TID_SHADEBOB, 1,"shadebob2"},
                              {TID_SHADEBOB, 2,"shadebob3"},
                              {TID_SHADEBOB, 3,"shadebob4"},
                              {TID_SHADEBOB, 4,"shadebob5"},
                              {TID_SHADEBOB, 5,"shadebob6"},
                              {TID_SHADEBOB, 6,"shadebob7"},
                              {TID_SHADEBOB, 7,"shadebob8"},
                              {TID_FLYER_RED, 0,"jaeger_rot"},
                              {TID_FLYER_RED, 1,"jaeger_rot2"},
                              {TID_FLYER_RED, 2,"jaeger_rot3"},
                              {TID_FLYER_RED, 3,"jaeger_rot4"},
                              {TID_FLYER_RED, 4,"jaeger_rot5"},
                              {TID_FLYER_RED, 5,"jaeger_rot6"},
                              {TID_FLYER_RED, 6,"jaeger_rot7"},
                              {TID_FLYER_RED, 7,"jaeger_rot8"},
                              {TID_FLYER_RED_EMISSIVE, 0,"jaeger_rot_em"},
                              {TID_FLYER_RED_EMISSIVE, 1,"jaeger_rot_em2"},
                              {TID_FLYER_RED_EMISSIVE, 2,"jaeger_rot_em3"},
                              {TID_FLYER_RED_EMISSIVE, 3,"jaeger_rot_em4"},
                              {TID_FLYER_RED_EMISSIVE, 4,"jaeger_rot_em5"},
                              {TID_FLYER_RED_EMISSIVE, 5,"jaeger_rot_em6"},
                              {TID_FLYER_RED_EMISSIVE, 6,"jaeger_rot_em7"},
                              {TID_FLYER_RED_EMISSIVE, 7,"jaeger_rot_em8"},
                              {TID_LEADER_RED, 0,"leader_rot"},
                              {TID_LEADER_RED, 1,"leader_rot2"},
                              {TID_LEADER_RED, 2,"leader_rot3"},
                              {TID_LEADER_RED, 3,"leader_rot4"},
                              {TID_LEADER_RED, 4,"leader_rot5"},
                              {TID_LEADER_RED, 5,"leader_rot6"},
                              {TID_LEADER_RED, 6,"leader_rot7"},
                              {TID_LEADER_RED, 7,"leader_rot8"},
                              {TID_LEADER_RED_EMISSIVE, 0,"leader_rot_em"},
                              {TID_LEADER_RED_EMISSIVE, 1,"leader_rot_em2"},
                              {TID_LEADER_RED_EMISSIVE, 2,"leader_rot_em3"},
                              {TID_LEADER_RED_EMISSIVE, 3,"leader_rot_em4"},
                              {TID_LEADER_RED_EMISSIVE, 4,"leader_rot_em5"},
                              {TID_LEADER_RED_EMISSIVE, 5,"leader_rot_em6"},
                              {TID_LEADER_RED_EMISSIVE, 6,"leader_rot_em7"},
                              {TID_LEADER_RED_EMISSIVE, 7,"leader_rot_em8"},
                              {TID_FLYER_GREEN, 0,"jaeger_gruen"},
                              {TID_FLYER_GREEN, 1,"jaeger_gruen2"},
                              {TID_FLYER_GREEN, 2,"jaeger_gruen3"},
                              {TID_FLYER_GREEN, 3,"jaeger_gruen4"},
                              {TID_FLYER_GREEN, 4,"jaeger_gruen5"},
                              {TID_FLYER_GREEN, 5,"jaeger_gruen6"},
                              {TID_FLYER_GREEN, 6,"jaeger_gruen7"},
                              {TID_FLYER_GREEN, 7,"jaeger_gruen8"},
                              {TID_FLYER_GREEN_EMISSIVE, 0,"jaeger_gruen_em"},
                              {TID_FLYER_GREEN_EMISSIVE, 1,"jaeger_gruen_em2"},
                              {TID_FLYER_GREEN_EMISSIVE, 2,"jaeger_gruen_em3"},
                              {TID_FLYER_GREEN_EMISSIVE, 3,"jaeger_gruen_em4"},
                              {TID_FLYER_GREEN_EMISSIVE, 4,"jaeger_gruen_em5"},
                              {TID_FLYER_GREEN_EMISSIVE, 5,"jaeger_gruen_em6"},
                              {TID_FLYER_GREEN_EMISSIVE, 6,"jaeger_gruen_em7"},
                              {TID_FLYER_GREEN_EMISSIVE, 7,"jaeger_gruen_em8"},
                              {TID_LEADER_GREEN, 0,"leader_gruen"},
                              {TID_LEADER_GREEN, 1,"leader_gruen2"},
                              {TID_LEADER_GREEN, 2,"leader_gruen3"},
                              {TID_LEADER_GREEN, 3,"leader_gruen4"},
                              {TID_LEADER_GREEN, 4,"leader_gruen5"},
                              {TID_LEADER_GREEN, 5,"leader_gruen6"},
                              {TID_LEADER_GREEN, 6,"leader_gruen7"},
                              {TID_LEADER_GREEN, 7,"leader_gruen8"},
                              {TID_LEADER_GREEN_EMISSIVE, 0,"leader_gruen_em"},
                              {TID_LEADER_GREEN_EMISSIVE, 1,"leader_gruen_em2"},
                              {TID_LEADER_GREEN_EMISSIVE, 2,"leader_gruen_em3"},
                              {TID_LEADER_GREEN_EMISSIVE, 3,"leader_gruen_em4"},
                              {TID_LEADER_GREEN_EMISSIVE, 4,"leader_gruen_em5"},
                              {TID_LEADER_GREEN_EMISSIVE, 5,"leader_gruen_em6"},
                              {TID_LEADER_GREEN_EMISSIVE, 6,"leader_gruen_em7"},
                              {TID_LEADER_GREEN_EMISSIVE, 7,"leader_gruen_em8"},
                              {TID_MINE, 0,"mine"},
                              {TID_MINE, 1,"mine2"},
                              {TID_MINE, 2,"mine3"},
                              {TID_MINE, 3,"mine4"},
                              {TID_MINE, 4,"mine5"},
                              {TID_MINE, 5,"mine6"},
                              {TID_MINE, 6,"mine7"},
                              {TID_MINE, 7,"mine8"},
                              {TID_MINE_EMISSIVE, 0,"mine_em"},
                              {TID_MINE_EMISSIVE, 1,"mine_em2"},
                              {TID_MINE_EMISSIVE, 2,"mine_em3"},
                              {TID_MINE_EMISSIVE, 3,"mine_em4"},
                              {TID_MINE_EMISSIVE, 4,"mine_em5"},
                              {TID_MINE_EMISSIVE, 5,"mine_em6"},
                              {TID_MINE_EMISSIVE, 6,"mine_em7"},
                              {TID_MINE_EMISSIVE, 7,"mine_em8"},
                              {TID_RED_FLARE, 0,"red_flare"},
                              {TID_PLANET, 0,"heimatpl"},
                              {TID_PLANET, 1,"heimatpl2"},
                              {TID_PLANET, 2,"heimatpl3"},
                              {TID_PLANET, 3,"heimatpl4"},
                              {TID_PLANET, 4,"heimatpl5"},
                              {TID_PLANET, 5,"heimatpl6"},
                              {TID_PLANET, 6,"heimatpl7"},
                              {TID_PLANET, 7,"heimatpl8"},

//                              {TID_ELEKTRON, 0,"elektron"},
//                              {TID_ELEKTRON, 1,"elektron2"},
//                              {TID_ELEKTRON, 2,"elektron3"},
//                              {TID_ELEKTRON, 3,"elektron4"},
//                              {TID_ELEKTRON, 4,"elektron5"},
//                              {TID_ELEKTRON, 5,"elektron6"},
//                              {TID_ELEKTRON, 6,"elektron7"},
//                              {TID_ELEKTRON, 7,"elektron8"},
//                              {TID_FENSTERF, 0,"FensterF"},
//                              {TID_FENSTERF, 1,"FensterF2"},
//                              {TID_FENSTERF, 2,"FensterF3"},
//                              {TID_FENSTERF, 3,"FensterF4"},
//                              {TID_FENSTERF, 4,"FensterF5"},
//                              {TID_FENSTERF, 5,"FensterF6"},
//                              {TID_FENSTERF_EM, 0,"Fenste_em"},
//                              {TID_FENSTERF_EM, 1,"fenste_em2"},
//                              {TID_FENSTERF_EM, 2,"fenste_em3"},
//                              {TID_FENSTERF_EM, 3,"fenste_em4"},
//                              {TID_FENSTERF_EM, 4,"fenste_em5"},
//                              {TID_FENSTERF_EM, 5,"fenste_em6"},
//                              {TID_REAKTORB, 0,"Reaktorb"},
//                              {TID_REAKTORB, 1,"reaktorb2"},
//                              {TID_REAKTORB, 2,"reaktorb3"},
//                              {TID_REAKTORB, 3,"reaktorb4"},
//                              {TID_REAKTORB, 4,"reaktorb5"},
//                              {TID_REAKTORB, 5,"reaktorb6"},
//                              {TID_REAKTORB_EM, 0,"Reakto_em"},
//                              {TID_REAKTORB_EM, 1,"reakto_em2"},
//                              {TID_REAKTORB_EM, 2,"reakto_em3"},
//                              {TID_REAKTORB_EM, 3,"reakto_em4"},
//                              {TID_REAKTORB_EM, 4,"reakto_em5"},
//                              {TID_REAKTORB_EM, 5,"reakto_em6"},
//                              {TID_ALIENPO, 0,"Alien_Po"},
//                              {TID_ALIENPO, 1,"alien_po2"},
//                              {TID_ALIENPO, 2,"alien_po3"},
//                              {TID_ALIENPO, 3,"alien_po4"},
//                              {TID_ALIENPO, 4,"alien_po5"},
//                              {TID_ALIENPO, 5,"alien_po6"},
//                              {TID_ALIENPO_EM, 0,"AlienPo_em"},
//                              {TID_ALIENPO_EM, 1,"alienpo_em2"},
//                              {TID_ALIENPO_EM, 2,"alienpo_em3"},
//                              {TID_ALIENPO_EM, 3,"alienpo_em4"},
//                              {TID_ALIENPO_EM, 4,"alienpo_em5"},
//                              {TID_ALIENPO_EM, 5,"alienpo_em6"},
//                              {TID_ALIENPO_EM, 6,"alienpo_em7"},
//                              {TID_ALIENPO_EM, 7,"alienpo_em8"},
//                              {TID_ALIENPO_EM, 8,"alienpo_em9"},
//                              {TID_ALIENPO_EM, 9,"alienpo_em10"},
//                              {TID_ALIENPO_EM, 10,"alienpo_em11"},
//                              {TID_A_HULL_M, 0,"A_Hull_M"},
//                              {TID_A_HULL_M, 1,"A_Hull_M2"},
//                              {TID_A_HULL_M, 2,"A_Hull_M3"},
//                              {TID_A_HULL_M, 3,"A_Hull_M4"},
//                              {TID_A_HULL_M, 4,"A_Hull_M5"},
//                              {TID_A_HULL_M, 5,"A_Hull_M6"},
//                              {TID_A_HULL_M, 6,"A_Hull_M7"},
//                              {TID_A_HULL_M, 7,"A_Hull_M8"},
//                              {TID_B_HULL_M, 0,"B_Hull_M"},
//                              {TID_B_HULL_M, 1,"B_Hull_M2"},
//                              {TID_B_HULL_M, 2,"B_Hull_M3"},
//                              {TID_B_HULL_M, 3,"B_Hull_M4"},
//                              {TID_B_HULL_M, 4,"B_Hull_M5"},
//                              {TID_B_HULL_M, 5,"B_Hull_M6"},
//                              {TID_B_HULL_M, 6,"B_Hull_M7"},
//                              {TID_B_HULL_M, 7,"B_Hull_M8"},
//                              {TID_B_HULL_M_EM, 0,"BHullM_em"},
//                              {TID_B_HULL_M_EM, 1,"bhullm_em2"},
//                              {TID_B_HULL_M_EM, 2,"bhullm_em3"},
//                              {TID_B_HULL_M_EM, 3,"bhullm_em4"},
//                              {TID_B_HULL_M_EM, 4,"bhullm_em5"},
//                              {TID_B_HULL_M_EM, 5,"bhullm_em6"},
//                              {TID_B_HULL_M_EM, 6,"bhullm_em7"},
//                              {TID_B_HULL_M_EM, 7,"bhullm_em8"},
//                              {TID_C_HULL_M, 0,"C_Hull_M"},
//                              {TID_C_HULL_M, 1,"C_Hull_M2"},
//                              {TID_C_HULL_M, 2,"C_Hull_M3"},
//                              {TID_C_HULL_M, 3,"C_Hull_M4"},
//                              {TID_C_HULL_M, 4,"C_Hull_M5"},
//                              {TID_C_HULL_M, 5,"C_Hull_M6"},
//                              {TID_C_HULL_M, 6,"C_Hull_M7"},
//                              {TID_C_HULL_M, 7,"C_Hull_M8"},
//                              {TID_D_HULL_M, 0,"D_Hull_M"},
//                              {TID_D_HULL_M, 1,"D_Hull_M2"},
//                              {TID_D_HULL_M, 2,"D_Hull_M3"},
//                              {TID_D_HULL_M, 3,"D_Hull_M4"},
//                              {TID_D_HULL_M, 4,"D_Hull_M5"},
//                              {TID_D_HULL_M, 5,"D_Hull_M6"},
//                              {TID_D_HULL_M, 6,"D_Hull_M7"},
//                              {TID_D_HULL_M, 7,"D_Hull_M8"},
//                              {TID_D_HULL_M_EM, 0,"DHullM_em"},
//                              {TID_D_HULL_M_EM, 1,"dhullm_em2"},
//                              {TID_D_HULL_M_EM, 2,"dhullm_em3"},
//                              {TID_D_HULL_M_EM, 3,"dhullm_em4"},
//                              {TID_D_HULL_M_EM, 4,"dhullm_em5"},
//                              {TID_D_HULL_M_EM, 5,"dhullm_em6"},
//                              {TID_D_HULL_M_EM, 6,"dhullm_em7"},
//                              {TID_D_HULL_M_EM, 7,"dhullm_em8"},
//
//                              {TID_EXTRAS, 0,"extras"},
//                              {TID_EXTRAS, 1,"extras2"},
//                              {TID_EXTRAS, 2,"extras3"},
//                              {TID_EXTRAS, 3,"extras4"},
//                              {TID_EXTRAS, 4,"extras5"},
//                              {TID_EXTRAS, 5,"extras6"},
//                              {TID_EXTRAS, 6,"extras7"},
//                              {TID_EXTRAS, 7,"extras8"},
//                              {TID_EXTRAS_EM, 0,"extras_em"},
//                              {TID_EXTRAS_EM, 1,"extras_em2"},
//                              {TID_EXTRAS_EM, 2,"extras_em3"},
//                              {TID_EXTRAS_EM, 3,"extras_em4"},
//                              {TID_EXTRAS_EM, 4,"extras_em5"},
//                              {TID_EXTRAS_EM, 5,"extras_em6"},
//                              {TID_EXTRAS_EM, 6,"extras_em7"},
//                              {TID_EXTRAS_EM, 7,"extras_em8"},

                              {TID_SYMBOL, 0,"chars"},
                              {TID_FONT, 0,"font"},
                              {TID_CORONA, 0,"corona"},
                              {TID_SPACEWARP02, 0,"spacewarp02"},
                              {TID_RADAR01, 0,"radar01"},
                              {TID_SMOKE, 0,"smoke"},
                              {TID_SMOKE, 1,"smoke2"},
                              {TID_SMOKE, 2,"smoke3"},
                              {TID_SMOKE, 3,"smoke4"},
                              {TID_SMOKE, 4,"smoke5"},
                              {TID_SMOKE, 5,"smoke6"},
                              {TID_SMOKE, 6,"smoke7"},
                              {TID_SMOKE, 7,"smoke8"},
                              {TID_ARCNEBULA, 0,"arcneb"},
                              {TID_ARCNEBULA, 1,"arcneb2"},
                              {TID_ARCNEBULA, 2,"arcneb3"},
                              {TID_ARCNEBULA, 3,"arcneb4"},
                              {TID_ARCNEBULA, 4,"arcneb5"},
                              {TID_ARCNEBULA, 5,"arcneb6"},
                              {TID_ARCNEBULA, 6,"arcneb7"},
                              {TID_ARCNEBULA, 7,"arcneb8"},

//                              {TID_SCHLAUCH, 0,"schlauch"},
//                              {TID_SCHLAUCH, 1,"schlauch2"},
//                              {TID_SCHLAUCH, 2,"schlauch3"},
//                              {TID_SCHLAUCH, 3,"schlauch4"},
//                              {TID_SCHLAUCH, 4,"schlauch5"},
//                              {TID_SCHLAUCH, 5,"schlauch6"},
//                              {TID_SCHLAUCH_EM, 0,"schlau_em"},
//                              {TID_SCHLAUCH_EM, 1,"schlau_em2"},
//                              {TID_SCHLAUCH_EM, 2,"schlau_em3"},
//                              {TID_SCHLAUCH_EM, 3,"schlau_em4"},
//                              {TID_SCHLAUCH_EM, 4,"schlau_em5"},
//                              {TID_SCHLAUCH_EM, 5,"schlau_em6"},

                              {TID_HUNTER, 0,"hunt_col"},
                              {TID_HUNTER, 1,"hunt_col2"},
                              {TID_HUNTER, 2,"hunt_col3"},
                              {TID_HUNTER, 3,"hunt_col4"},
                              {TID_HUNTER, 4,"hunt_col5"},
                              {TID_HUNTER, 5,"hunt_col6"},
                              {TID_HUNTER, 6,"hunt_col7"},
                              {TID_HUNTER, 7,"hunt_col8"},
                              {TID_HUNTER_NORMALS, 0,"hunt_nor"},
                              {TID_HUNTER_NORMALS, 1,"hunt_nor2"},
                              {TID_HUNTER_NORMALS, 2,"hunt_nor3"},
                              {TID_HUNTER_NORMALS, 3,"hunt_nor4"},
                              {TID_HUNTER_NORMALS, 4,"hunt_nor5"},
                              {TID_HUNTER_NORMALS, 5,"hunt_nor6"},
                              {TID_HUNTER_NORMALS, 6,"hunt_nor7"},
                              {TID_HUNTER_NORMALS, 7,"hunt_nor8"},

                              {TID_HUNTER01, 0,"hunter01_col"},
                              {TID_HUNTER01, 1,"hunter01_col2" },
                              {TID_HUNTER01, 2,"hunter01_col3"},
                              {TID_HUNTER01, 3,"hunter01_col4"},
                              {TID_HUNTER01, 4,"hunter01_col5"},
                              {TID_HUNTER01, 5,"hunter01_col6"},
                              {TID_HUNTER01, 6,"hunter01_col7"},
                              {TID_HUNTER01, 7,"hunter01_col8"},
                              {TID_HUNTER01_NORMALS, 0,"hunter01_nor"},
                              {TID_HUNTER01_NORMALS, 1,"hunter01_nor2"},
                              {TID_HUNTER01_NORMALS, 2,"hunter01_nor3"},
                              {TID_HUNTER01_NORMALS, 3,"hunter01_nor4"},
                              {TID_HUNTER01_NORMALS, 4,"hunter01_nor5"},
                              {TID_HUNTER01_NORMALS, 5,"hunter01_nor6"},
                              {TID_HUNTER01_NORMALS, 6,"hunter01_nor7"},
                              {TID_HUNTER01_NORMALS, 7,"hunter01_nor8"},
                              {TID_HUNTER01_EM, 0,"hunter01_em"},
                              {TID_HUNTER01_EM, 1,"hunter01_em2"},
                              {TID_HUNTER01_EM, 2,"hunter01_em3"},
                              {TID_HUNTER01_EM, 3,"hunter01_em4"},
                              {TID_HUNTER01_EM, 4,"hunter01_em5"},
                              {TID_HUNTER01_EM, 5,"hunter01_em6"},
                              {TID_HUNTER01_EM, 6,"hunter01_em7"},
                              {TID_HUNTER01_EM, 7,"hunter01_em8"},

                              {TID_HUNTER02, 0,"hunter02_col"},
                              {TID_HUNTER02, 1,"hunter02_col2" },
                              {TID_HUNTER02, 2,"hunter02_col3"},
                              {TID_HUNTER02, 3,"hunter02_col4"},
                              {TID_HUNTER02, 4,"hunter02_col5"},
                              {TID_HUNTER02, 5,"hunter02_col6"},
                              {TID_HUNTER02, 6,"hunter02_col7"},
                              {TID_HUNTER02, 7,"hunter02_col8"},
                              {TID_HUNTER02_NORMALS, 0,"hunter02_nor"},
                              {TID_HUNTER02_NORMALS, 1,"hunter02_nor2"},
                              {TID_HUNTER02_NORMALS, 2,"hunter02_nor3"},
                              {TID_HUNTER02_NORMALS, 3,"hunter02_nor4"},
                              {TID_HUNTER02_NORMALS, 4,"hunter02_nor5"},
                              {TID_HUNTER02_NORMALS, 5,"hunter02_nor6"},
                              {TID_HUNTER02_NORMALS, 6,"hunter02_nor7"},
                              {TID_HUNTER02_NORMALS, 7,"hunter02_nor8"},
                              {TID_HUNTER02_EM, 0,"hunter02_em"},
                              {TID_HUNTER02_EM, 1,"hunter02_em2"},
                              {TID_HUNTER02_EM, 2,"hunter02_em3"},
                              {TID_HUNTER02_EM, 3,"hunter02_em4"},
                              {TID_HUNTER02_EM, 4,"hunter02_em5"},
                              {TID_HUNTER02_EM, 5,"hunter02_em6"},
                              {TID_HUNTER02_EM, 6,"hunter02_em7"},
                              {TID_HUNTER02_EM, 7,"hunter02_em8"},

                              {TID_HUNTER03, 0,"hunter03_col"},
                              {TID_HUNTER03, 1,"hunter03_col2" },
                              {TID_HUNTER03, 2,"hunter03_col3"},
                              {TID_HUNTER03, 3,"hunter03_col4"},
                              {TID_HUNTER03, 4,"hunter03_col5"},
                              {TID_HUNTER03, 5,"hunter03_col6"},
                              {TID_HUNTER03, 6,"hunter03_col7"},
                              {TID_HUNTER03, 7,"hunter03_col8"},
                              {TID_HUNTER03_NORMALS, 0,"hunter03_nor"},
                              {TID_HUNTER03_NORMALS, 1,"hunter03_nor2"},
                              {TID_HUNTER03_NORMALS, 2,"hunter03_nor3"},
                              {TID_HUNTER03_NORMALS, 3,"hunter03_nor4"},
                              {TID_HUNTER03_NORMALS, 4,"hunter03_nor5"},
                              {TID_HUNTER03_NORMALS, 5,"hunter03_nor6"},
                              {TID_HUNTER03_NORMALS, 6,"hunter03_nor7"},
                              {TID_HUNTER03_NORMALS, 7,"hunter03_nor8"},
                              {TID_HUNTER03_EM, 0,"hunter03_em"},
                              {TID_HUNTER03_EM, 1,"hunter03_em2"},
                              {TID_HUNTER03_EM, 2,"hunter03_em3"},
                              {TID_HUNTER03_EM, 3,"hunter03_em4"},
                              {TID_HUNTER03_EM, 4,"hunter03_em5"},
                              {TID_HUNTER03_EM, 5,"hunter03_em6"},
                              {TID_HUNTER03_EM, 6,"hunter03_em7"},
                              {TID_HUNTER03_EM, 7,"hunter03_em8"},

                              {TID_HUNTER09, 0,"hunter09_col"},
                              {TID_HUNTER09, 1,"hunter09_col2" },
                              {TID_HUNTER09, 2,"hunter09_col3"},
                              {TID_HUNTER09, 3,"hunter09_col4"},
                              {TID_HUNTER09, 4,"hunter09_col5"},
                              {TID_HUNTER09, 5,"hunter09_col6"},
                              {TID_HUNTER09, 6,"hunter09_col7"},
                              {TID_HUNTER09, 7,"hunter09_col8"},
                              {TID_HUNTER09_NORMALS, 0,"hunter09_nor"},
                              {TID_HUNTER09_NORMALS, 1,"hunter09_nor2"},
                              {TID_HUNTER09_NORMALS, 2,"hunter09_nor3"},
                              {TID_HUNTER09_NORMALS, 3,"hunter09_nor4"},
                              {TID_HUNTER09_NORMALS, 4,"hunter09_nor5"},
                              {TID_HUNTER09_NORMALS, 5,"hunter09_nor6"},
                              {TID_HUNTER09_NORMALS, 6,"hunter09_nor7"},
                              {TID_HUNTER09_NORMALS, 7,"hunter09_nor8"},
                              {TID_HUNTER09_EM, 0,"hunter09_em"},
                              {TID_HUNTER09_EM, 1,"hunter09_em2"},
                              {TID_HUNTER09_EM, 2,"hunter09_em3"},
                              {TID_HUNTER09_EM, 3,"hunter09_em4"},
                              {TID_HUNTER09_EM, 4,"hunter09_em5"},
                              {TID_HUNTER09_EM, 5,"hunter09_em6"},
                              {TID_HUNTER09_EM, 6,"hunter09_em7"},
                              {TID_HUNTER09_EM, 7,"hunter09_em8"},

                              {TID_HUNTER10, 0,"hunter10_col"},
                              {TID_HUNTER10, 1,"hunter10_col2" },
                              {TID_HUNTER10, 2,"hunter10_col3"},
                              {TID_HUNTER10, 3,"hunter10_col4"},
                              {TID_HUNTER10, 4,"hunter10_col5"},
                              {TID_HUNTER10, 5,"hunter10_col6"},
                              {TID_HUNTER10, 6,"hunter10_col7"},
                              {TID_HUNTER10, 7,"hunter10_col8"},
                              {TID_HUNTER10_NORMALS, 0,"hunter10_nor"},
                              {TID_HUNTER10_NORMALS, 1,"hunter10_nor2"},
                              {TID_HUNTER10_NORMALS, 2,"hunter10_nor3"},
                              {TID_HUNTER10_NORMALS, 3,"hunter10_nor4"},
                              {TID_HUNTER10_NORMALS, 4,"hunter10_nor5"},
                              {TID_HUNTER10_NORMALS, 5,"hunter10_nor6"},
                              {TID_HUNTER10_NORMALS, 6,"hunter10_nor7"},
                              {TID_HUNTER10_NORMALS, 7,"hunter10_nor8"},
                              {TID_HUNTER10_EM, 0,"hunter10_em"},
                              {TID_HUNTER10_EM, 1,"hunter10_em2"},
                              {TID_HUNTER10_EM, 2,"hunter10_em3"},
                              {TID_HUNTER10_EM, 3,"hunter10_em4"},
                              {TID_HUNTER10_EM, 4,"hunter10_em5"},
                              {TID_HUNTER10_EM, 5,"hunter10_em6"},
                              {TID_HUNTER10_EM, 6,"hunter10_em7"},
                              {TID_HUNTER10_EM, 7,"hunter10_em8"},

                              {TID_CORIOLIS_EXTERIOR, 0,"coriolis_ext_col"},
                              {TID_CORIOLIS_EXTERIOR, 1,"coriolis_ext_col2"},
                              {TID_CORIOLIS_EXTERIOR, 2,"coriolis_ext_col3"},
                              {TID_CORIOLIS_EXTERIOR, 3,"coriolis_ext_col4"},
                              {TID_CORIOLIS_EXTERIOR, 4,"coriolis_ext_col5"},
                              {TID_CORIOLIS_EXTERIOR, 5,"coriolis_ext_col6"},
                              {TID_CORIOLIS_EXTERIOR, 6,"coriolis_ext_col7"},
                              {TID_CORIOLIS_EXTERIOR, 7,"coriolis_ext_col8"},
                              {TID_CORIOLIS_EXTERIOR_NORMALS, 0,"coriolis_ext_nor"},
                              {TID_CORIOLIS_EXTERIOR_NORMALS, 1,"coriolis_ext_nor2"},
                              {TID_CORIOLIS_EXTERIOR_NORMALS, 2,"coriolis_ext_nor3"},
                              {TID_CORIOLIS_EXTERIOR_NORMALS, 3,"coriolis_ext_nor4"},
                              {TID_CORIOLIS_EXTERIOR_NORMALS, 4,"coriolis_ext_nor5"},
                              {TID_CORIOLIS_EXTERIOR_NORMALS, 5,"coriolis_ext_nor6"},
                              {TID_CORIOLIS_EXTERIOR_NORMALS, 6,"coriolis_ext_nor7"},
                              {TID_CORIOLIS_EXTERIOR_NORMALS, 7,"coriolis_ext_nor8"},
                              {TID_CORIOLIS_EXTERIOR_EM, 0,"coriolis_ext_em"},
                              {TID_CORIOLIS_EXTERIOR_EM, 1,"coriolis_ext_em2"},
                              {TID_CORIOLIS_EXTERIOR_EM, 2,"coriolis_ext_em3"},
                              {TID_CORIOLIS_EXTERIOR_EM, 3,"coriolis_ext_em4"},
                              {TID_CORIOLIS_EXTERIOR_EM, 4,"coriolis_ext_em5"},
                              {TID_CORIOLIS_EXTERIOR_EM, 5,"coriolis_ext_em6"},
                              {TID_CORIOLIS_EXTERIOR_EM, 6,"coriolis_ext_em7"},
                              {TID_CORIOLIS_EXTERIOR_EM, 7,"coriolis_ext_em8"},

                              {TID_CORIOLIS_CORE, 0,"coriolis_core_col"},
                              {TID_CORIOLIS_CORE, 1,"coriolis_core_col2"},
                              {TID_CORIOLIS_CORE, 2,"coriolis_core_col3"},
                              {TID_CORIOLIS_CORE, 3,"coriolis_core_col4"},
                              {TID_CORIOLIS_CORE, 4,"coriolis_core_col5"},
                              {TID_CORIOLIS_CORE, 5,"coriolis_core_col6"},
                              {TID_CORIOLIS_CORE, 6,"coriolis_core_col7"},
                              {TID_CORIOLIS_CORE, 7,"coriolis_core_col8"},
                              {TID_CORIOLIS_CORE_NORMALS, 0,"coriolis_core_nor"},
                              {TID_CORIOLIS_CORE_NORMALS, 1,"coriolis_core_nor2"},
                              {TID_CORIOLIS_CORE_NORMALS, 2,"coriolis_core_nor3"},
                              {TID_CORIOLIS_CORE_NORMALS, 3,"coriolis_core_nor4"},
                              {TID_CORIOLIS_CORE_NORMALS, 4,"coriolis_core_nor5"},
                              {TID_CORIOLIS_CORE_NORMALS, 5,"coriolis_core_nor6"},
                              {TID_CORIOLIS_CORE_NORMALS, 6,"coriolis_core_nor7"},
                              {TID_CORIOLIS_CORE_NORMALS, 7,"coriolis_core_nor8"},
                              {TID_CORIOLIS_CORE_EM, 0,"coriolis_core_em"},
                              {TID_CORIOLIS_CORE_EM, 1,"coriolis_core_em2"},
                              {TID_CORIOLIS_CORE_EM, 2,"coriolis_core_em3"},
                              {TID_CORIOLIS_CORE_EM, 3,"coriolis_core_em4"},
                              {TID_CORIOLIS_CORE_EM, 4,"coriolis_core_em5"},
                              {TID_CORIOLIS_CORE_EM, 5,"coriolis_core_em6"},
                              {TID_CORIOLIS_CORE_EM, 6,"coriolis_core_em7"},
                              {TID_CORIOLIS_CORE_EM, 7,"coriolis_core_em8"},

                              {TID_BOA, 0,"boa_col"},
                              {TID_BOA, 1,"boa_col2" },
                              {TID_BOA, 2,"boa_col3"},
                              {TID_BOA, 3,"boa_col4"},
                              {TID_BOA, 4,"boa_col5"},
                              {TID_BOA, 5,"boa_col6"},
                              {TID_BOA, 6,"boa_col7"},
                              {TID_BOA, 7,"boa_col8"},
                              {TID_BOA_NORMALS, 0,"boa_nor"},
                              {TID_BOA_NORMALS, 1,"boa_nor2"},
                              {TID_BOA_NORMALS, 2,"boa_nor3"},
                              {TID_BOA_NORMALS, 3,"boa_nor4"},
                              {TID_BOA_NORMALS, 4,"boa_nor5"},
                              {TID_BOA_NORMALS, 5,"boa_nor6"},
                              {TID_BOA_NORMALS, 6,"boa_nor7"},
                              {TID_BOA_NORMALS, 7,"boa_nor8"},
                              {TID_BOA_EM, 0,"boa_em"},
                              {TID_BOA_EM, 1,"boa_em2"},
                              {TID_BOA_EM, 2,"boa_em3"},
                              {TID_BOA_EM, 3,"boa_em4"},
                              {TID_BOA_EM, 4,"boa_em5"},
                              {TID_BOA_EM, 5,"boa_em6"},
                              {TID_BOA_EM, 6,"boa_em7"},
                              {TID_BOA_EM, 7,"boa_em8"},

                              {TID_CARGO01, 0,"cargo01_col"},
                              {TID_CARGO01, 1,"cargo01_col2" },
                              {TID_CARGO01, 2,"cargo01_col3"},
                              {TID_CARGO01, 3,"cargo01_col4"},
                              {TID_CARGO01, 4,"cargo01_col5"},
                              {TID_CARGO01, 5,"cargo01_col6"},
                              {TID_CARGO01, 6,"cargo01_col7"},
                              {TID_CARGO01, 7,"cargo01_col8"},
                              {TID_CARGO01_NORMALS, 0,"cargo01_nor"},
                              {TID_CARGO01_NORMALS, 1,"cargo01_nor2"},
                              {TID_CARGO01_NORMALS, 2,"cargo01_nor3"},
                              {TID_CARGO01_NORMALS, 3,"cargo01_nor4"},
                              {TID_CARGO01_NORMALS, 4,"cargo01_nor5"},
                              {TID_CARGO01_NORMALS, 5,"cargo01_nor6"},
                              {TID_CARGO01_NORMALS, 6,"cargo01_nor7"},
                              {TID_CARGO01_NORMALS, 7,"cargo01_nor8"},
                              {TID_CARGO01_EM, 0,"cargo01_em"},
                              {TID_CARGO01_EM, 1,"cargo01_em2"},
                              {TID_CARGO01_EM, 2,"cargo01_em3"},
                              {TID_CARGO01_EM, 3,"cargo01_em4"},
                              {TID_CARGO01_EM, 4,"cargo01_em5"},
                              {TID_CARGO01_EM, 5,"cargo01_em6"},
                              {TID_CARGO01_EM, 6,"cargo01_em7"},
                              {TID_CARGO01_EM, 7,"cargo01_em8"},

                              {TID_CARGO03, 0,"cargo03_col"},
                              {TID_CARGO03, 1,"cargo03_col2" },
                              {TID_CARGO03, 2,"cargo03_col3"},
                              {TID_CARGO03, 3,"cargo03_col4"},
                              {TID_CARGO03, 4,"cargo03_col5"},
                              {TID_CARGO03, 5,"cargo03_col6"},
                              {TID_CARGO03, 6,"cargo03_col7"},
                              {TID_CARGO03, 7,"cargo03_col8"},
                              {TID_CARGO03_NORMALS, 0,"cargo03_nor"},
                              {TID_CARGO03_NORMALS, 1,"cargo03_nor2"},
                              {TID_CARGO03_NORMALS, 2,"cargo03_nor3"},
                              {TID_CARGO03_NORMALS, 3,"cargo03_nor4"},
                              {TID_CARGO03_NORMALS, 4,"cargo03_nor5"},
                              {TID_CARGO03_NORMALS, 5,"cargo03_nor6"},
                              {TID_CARGO03_NORMALS, 6,"cargo03_nor7"},
                              {TID_CARGO03_NORMALS, 7,"cargo03_nor8"},
                              {TID_CARGO03_EM, 0,"cargo03_em"},
                              {TID_CARGO03_EM, 1,"cargo03_em2"},
                              {TID_CARGO03_EM, 2,"cargo03_em3"},
                              {TID_CARGO03_EM, 3,"cargo03_em4"},
                              {TID_CARGO03_EM, 4,"cargo03_em5"},
                              {TID_CARGO03_EM, 5,"cargo03_em6"},
                              {TID_CARGO03_EM, 6,"cargo03_em7"},
                              {TID_CARGO03_EM, 7,"cargo03_em8"},

                              {TID_CARGO04, 0,"cargo04_col"},
                              {TID_CARGO04, 1,"cargo04_col2" },
                              {TID_CARGO04, 2,"cargo04_col3"},
                              {TID_CARGO04, 3,"cargo04_col4"},
                              {TID_CARGO04, 4,"cargo04_col5"},
                              {TID_CARGO04, 5,"cargo04_col6"},
                              {TID_CARGO04, 6,"cargo04_col7"},
                              {TID_CARGO04, 7,"cargo04_col8"},
                              {TID_CARGO04_NORMALS, 0,"cargo04_nor"},
                              {TID_CARGO04_NORMALS, 1,"cargo04_nor2"},
                              {TID_CARGO04_NORMALS, 2,"cargo04_nor3"},
                              {TID_CARGO04_NORMALS, 3,"cargo04_nor4"},
                              {TID_CARGO04_NORMALS, 4,"cargo04_nor5"},
                              {TID_CARGO04_NORMALS, 5,"cargo04_nor6"},
                              {TID_CARGO04_NORMALS, 6,"cargo04_nor7"},
                              {TID_CARGO04_NORMALS, 7,"cargo04_nor8"},
                              {TID_CARGO04_EM, 0,"cargo04_em"},
                              {TID_CARGO04_EM, 1,"cargo04_em2"},
                              {TID_CARGO04_EM, 2,"cargo04_em3"},
                              {TID_CARGO04_EM, 3,"cargo04_em4"},
                              {TID_CARGO04_EM, 4,"cargo04_em5"},
                              {TID_CARGO04_EM, 5,"cargo04_em6"},
                              {TID_CARGO04_EM, 6,"cargo04_em7"},
                              {TID_CARGO04_EM, 7,"cargo04_em8"},

                              {TID_CARGO05, 0,"cargo05_col"},
                              {TID_CARGO05, 1,"cargo05_col2" },
                              {TID_CARGO05, 2,"cargo05_col3"},
                              {TID_CARGO05, 3,"cargo05_col4"},
                              {TID_CARGO05, 4,"cargo05_col5"},
                              {TID_CARGO05, 5,"cargo05_col6"},
                              {TID_CARGO05, 6,"cargo05_col7"},
                              {TID_CARGO05, 7,"cargo05_col8"},
                              {TID_CARGO05_NORMALS, 0,"cargo05_nor"},
                              {TID_CARGO05_NORMALS, 1,"cargo05_nor2"},
                              {TID_CARGO05_NORMALS, 2,"cargo05_nor3"},
                              {TID_CARGO05_NORMALS, 3,"cargo05_nor4"},
                              {TID_CARGO05_NORMALS, 4,"cargo05_nor5"},
                              {TID_CARGO05_NORMALS, 5,"cargo05_nor6"},
                              {TID_CARGO05_NORMALS, 6,"cargo05_nor7"},
                              {TID_CARGO05_NORMALS, 7,"cargo05_nor8"},
                              {TID_CARGO05_EM, 0,"cargo05_em"},
                              {TID_CARGO05_EM, 1,"cargo05_em2"},
                              {TID_CARGO05_EM, 2,"cargo05_em3"},
                              {TID_CARGO05_EM, 3,"cargo05_em4"},
                              {TID_CARGO05_EM, 4,"cargo05_em5"},
                              {TID_CARGO05_EM, 5,"cargo05_em6"},
                              {TID_CARGO05_EM, 6,"cargo05_em7"},
                              {TID_CARGO05_EM, 7,"cargo05_em8"},

                              {TID_CARGO06, 0,"cargo06_col"},
                              {TID_CARGO06, 1,"cargo06_col2" },
                              {TID_CARGO06, 2,"cargo06_col3"},
                              {TID_CARGO06, 3,"cargo06_col4"},
                              {TID_CARGO06, 4,"cargo06_col5"},
                              {TID_CARGO06, 5,"cargo06_col6"},
                              {TID_CARGO06, 6,"cargo06_col7"},
                              {TID_CARGO06, 7,"cargo06_col8"},
                              {TID_CARGO06_NORMALS, 0,"cargo06_nor"},
                              {TID_CARGO06_NORMALS, 1,"cargo06_nor2"},
                              {TID_CARGO06_NORMALS, 2,"cargo06_nor3"},
                              {TID_CARGO06_NORMALS, 3,"cargo06_nor4"},
                              {TID_CARGO06_NORMALS, 4,"cargo06_nor5"},
                              {TID_CARGO06_NORMALS, 5,"cargo06_nor6"},
                              {TID_CARGO06_NORMALS, 6,"cargo06_nor7"},
                              {TID_CARGO06_NORMALS, 7,"cargo06_nor8"},
                              {TID_CARGO06_EM, 0,"cargo06_em"},
                              {TID_CARGO06_EM, 1,"cargo06_em2"},
                              {TID_CARGO06_EM, 2,"cargo06_em3"},
                              {TID_CARGO06_EM, 3,"cargo06_em4"},
                              {TID_CARGO06_EM, 4,"cargo06_em5"},
                              {TID_CARGO06_EM, 5,"cargo06_em6"},
                              {TID_CARGO06_EM, 6,"cargo06_em7"},
                              {TID_CARGO06_EM, 7,"cargo06_em8"},

                              {TID_CARGO07, 0,"cargo07_col"},
                              {TID_CARGO07, 1,"cargo07_col2" },
                              {TID_CARGO07, 2,"cargo07_col3"},
                              {TID_CARGO07, 3,"cargo07_col4"},
                              {TID_CARGO07, 4,"cargo07_col5"},
                              {TID_CARGO07, 5,"cargo07_col6"},
                              {TID_CARGO07, 6,"cargo07_col7"},
                              {TID_CARGO07, 7,"cargo07_col8"},
                              {TID_CARGO07_NORMALS, 0,"cargo07_nor"},
                              {TID_CARGO07_NORMALS, 1,"cargo07_nor2"},
                              {TID_CARGO07_NORMALS, 2,"cargo07_nor3"},
                              {TID_CARGO07_NORMALS, 3,"cargo07_nor4"},
                              {TID_CARGO07_NORMALS, 4,"cargo07_nor5"},
                              {TID_CARGO07_NORMALS, 5,"cargo07_nor6"},
                              {TID_CARGO07_NORMALS, 6,"cargo07_nor7"},
                              {TID_CARGO07_NORMALS, 7,"cargo07_nor8"},

                              {TID_CARGO08, 0,"cargo08_col"},
                              {TID_CARGO08, 1,"cargo08_col2" },
                              {TID_CARGO08, 2,"cargo08_col3"},
                              {TID_CARGO08, 3,"cargo08_col4"},
                              {TID_CARGO08, 4,"cargo08_col5"},
                              {TID_CARGO08, 5,"cargo08_col6"},
                              {TID_CARGO08, 6,"cargo08_col7"},
                              {TID_CARGO08, 7,"cargo08_col8"},
                              {TID_CARGO08_NORMALS, 0,"cargo08_nor"},
                              {TID_CARGO08_NORMALS, 1,"cargo08_nor2"},
                              {TID_CARGO08_NORMALS, 2,"cargo08_nor3"},
                              {TID_CARGO08_NORMALS, 3,"cargo08_nor4"},
                              {TID_CARGO08_NORMALS, 4,"cargo08_nor5"},
                              {TID_CARGO08_NORMALS, 5,"cargo08_nor6"},
                              {TID_CARGO08_NORMALS, 6,"cargo08_nor7"},
                              {TID_CARGO08_NORMALS, 7,"cargo08_nor8"},
                              {TID_CARGO08_EM, 0,"cargo08_em"},
                              {TID_CARGO08_EM, 1,"cargo08_em2"},
                              {TID_CARGO08_EM, 2,"cargo08_em3"},
                              {TID_CARGO08_EM, 3,"cargo08_em4"},
                              {TID_CARGO08_EM, 4,"cargo08_em5"},
                              {TID_CARGO08_EM, 5,"cargo08_em6"},
                              {TID_CARGO08_EM, 6,"cargo08_em7"},
                              {TID_CARGO08_EM, 7,"cargo08_em8"},

                              {TID_TRANSPORT1, 0,"transport1_col"},
                              {TID_TRANSPORT1, 1,"transport1_col2" },
                              {TID_TRANSPORT1, 2,"transport1_col3"},
                              {TID_TRANSPORT1, 3,"transport1_col4"},
                              {TID_TRANSPORT1, 4,"transport1_col5"},
                              {TID_TRANSPORT1, 5,"transport1_col6"},
                              {TID_TRANSPORT1, 6,"transport1_col7"},
                              {TID_TRANSPORT1, 7,"transport1_col8"},
                              {TID_TRANSPORT1_NORMALS, 0,"transport1_nor"},
                              {TID_TRANSPORT1_NORMALS, 1,"transport1_nor2"},
                              {TID_TRANSPORT1_NORMALS, 2,"transport1_nor3"},
                              {TID_TRANSPORT1_NORMALS, 3,"transport1_nor4"},
                              {TID_TRANSPORT1_NORMALS, 4,"transport1_nor5"},
                              {TID_TRANSPORT1_NORMALS, 5,"transport1_nor6"},
                              {TID_TRANSPORT1_NORMALS, 6,"transport1_nor7"},
                              {TID_TRANSPORT1_NORMALS, 7,"transport1_nor8"},
                              {TID_TRANSPORT1_EM, 0,"transport1_em"},
                              {TID_TRANSPORT1_EM, 1,"transport1_em2"},
                              {TID_TRANSPORT1_EM, 2,"transport1_em3"},
                              {TID_TRANSPORT1_EM, 3,"transport1_em4"},
                              {TID_TRANSPORT1_EM, 4,"transport1_em5"},
                              {TID_TRANSPORT1_EM, 5,"transport1_em6"},
                              {TID_TRANSPORT1_EM, 6,"transport1_em7"},
                              {TID_TRANSPORT1_EM, 7,"transport1_em8"},

                              {TID_CARGOBOX, 0,"crate_col"},
                              {TID_CARGOBOX, 1,"crate_col2" },
                              {TID_CARGOBOX, 2,"crate_col3"},
                              {TID_CARGOBOX, 3,"crate_col4"},
                              {TID_CARGOBOX, 4,"crate_col5"},
                              {TID_CARGOBOX, 5,"crate_col6"},
                              {TID_CARGOBOX, 6,"crate_col7"},
                              {TID_CARGOBOX, 7,"crate_col8"},
                              {TID_CARGOBOX_NORMALS, 0,"crate_nor"},
                              {TID_CARGOBOX_NORMALS, 1,"crate_nor2"},
                              {TID_CARGOBOX_NORMALS, 2,"crate_nor3"},
                              {TID_CARGOBOX_NORMALS, 3,"crate_nor4"},
                              {TID_CARGOBOX_NORMALS, 4,"crate_nor5"},
                              {TID_CARGOBOX_NORMALS, 5,"crate_nor6"},
                              {TID_CARGOBOX_NORMALS, 6,"crate_nor7"},
                              {TID_CARGOBOX_NORMALS, 7,"crate_nor8"},
                              {TID_LASER, 0,"gun_col"},
                              {TID_LASER, 1,"gun_col2" },
                              {TID_LASER, 2,"gun_col3"},
                              {TID_LASER, 3,"gun_col4"},
                              {TID_LASER, 4,"gun_col5"},
                              {TID_LASER, 5,"gun_col6"},
                              {TID_LASER, 6,"gun_col7"},
                              {TID_LASER, 7,"gun_col8"},
                              {TID_LASER_NORMALS, 0,"gun_nor"},
                              {TID_LASER_NORMALS, 1,"gun_nor2"},
                              {TID_LASER_NORMALS, 2,"gun_nor3"},
                              {TID_LASER_NORMALS, 3,"gun_nor4"},
                              {TID_LASER_NORMALS, 4,"gun_nor5"},
                              {TID_LASER_NORMALS, 5,"gun_nor6"},
                              {TID_LASER_NORMALS, 6,"gun_nor7"},
                              {TID_LASER_NORMALS, 7,"gun_nor8"},
                              {TID_LASER_EM, 0,"gun_em"},
                              {TID_LASER_EM, 1,"gun_em2"},
                              {TID_LASER_EM, 2,"gun_em3"},
                              {TID_LASER_EM, 3,"gun_em4"},
                              {TID_LASER_EM, 4,"gun_em5"},
                              {TID_LASER_EM, 5,"gun_em6"},
                              {TID_LASER_EM, 6,"gun_em7"},
                              {TID_LASER_EM, 7,"gun_em8"},
                              {TID_SBHANGAR, 0,"hangar_col"},
                              {TID_SBHANGAR, 1,"hangar_col2" },
                              {TID_SBHANGAR, 2,"hangar_col3"},
                              {TID_SBHANGAR, 3,"hangar_col4"},
                              {TID_SBHANGAR, 4,"hangar_col5"},
                              {TID_SBHANGAR, 5,"hangar_col6"},
                              {TID_SBHANGAR, 6,"hangar_col7"},
                              {TID_SBHANGAR, 7,"hangar_col8"},
                              {TID_SBHANGAR_NORMALS, 0,"hangar_nor"},
                              {TID_SBHANGAR_NORMALS, 1,"hangar_nor2"},
                              {TID_SBHANGAR_NORMALS, 2,"hangar_nor3"},
                              {TID_SBHANGAR_NORMALS, 3,"hangar_nor4"},
                              {TID_SBHANGAR_NORMALS, 4,"hangar_nor5"},
                              {TID_SBHANGAR_NORMALS, 5,"hangar_nor6"},
                              {TID_SBHANGAR_NORMALS, 6,"hangar_nor7"},
                              {TID_SBHANGAR_NORMALS, 7,"hangar_nor8"},
                              {TID_SBHANGAR_EM, 0,"hangar_em"},
                              {TID_SBHANGAR_EM, 1,"hangar_em2"},
                              {TID_SBHANGAR_EM, 2,"hangar_em3"},
                              {TID_SBHANGAR_EM, 3,"hangar_em4"},
                              {TID_SBHANGAR_EM, 4,"hangar_em5"},
                              {TID_SBHANGAR_EM, 5,"hangar_em6"},
                              {TID_SBHANGAR_EM, 6,"hangar_em7"},
                              {TID_SBHANGAR_EM, 7,"hangar_em8"},
                              {TID_ILLUMINATION, 0,"bumpshade"},

                              {TID_RESCUE, 0,"rescue_col"},
                              {TID_RESCUE, 1,"rescue_col2" },
                              {TID_RESCUE, 2,"rescue_col3"},
                              {TID_RESCUE, 3,"rescue_col4"},
                              {TID_RESCUE, 4,"rescue_col5"},
                              {TID_RESCUE, 5,"rescue_col6"},
                              {TID_RESCUE, 6,"rescue_col7"},
                              {TID_RESCUE, 7,"rescue_col8"},
                              {TID_RESCUE_NORMALS, 0,"rescue_nor"},
                              {TID_RESCUE_NORMALS, 1,"rescue_nor2"},
                              {TID_RESCUE_NORMALS, 2,"rescue_nor3"},
                              {TID_RESCUE_NORMALS, 3,"rescue_nor4"},
                              {TID_RESCUE_NORMALS, 4,"rescue_nor5"},
                              {TID_RESCUE_NORMALS, 5,"rescue_nor6"},
                              {TID_RESCUE_NORMALS, 6,"rescue_nor7"},
                              {TID_RESCUE_NORMALS, 7,"rescue_nor8"},
                              {TID_RESCUE_EM, 0,"rescue_em"},
                              {TID_RESCUE_EM, 1,"rescue_em2"},
                              {TID_RESCUE_EM, 2,"rescue_em3"},
                              {TID_RESCUE_EM, 3,"rescue_em4"},
                              {TID_RESCUE_EM, 4,"rescue_em5"},
                              {TID_RESCUE_EM, 5,"rescue_em6"},
                              {TID_RESCUE_EM, 6,"rescue_em7"},
                              {TID_RESCUE_EM, 7,"rescue_em8"},

                              {TID_MISSILE, 0,"rocket_col"},
                              {TID_MISSILE, 1,"rocket_col2" },
                              {TID_MISSILE, 2,"rocket_col3"},
                              {TID_MISSILE, 3,"rocket_col4"},
                              {TID_MISSILE, 4,"rocket_col5"},
                              {TID_MISSILE, 5,"rocket_col6"},
                              {TID_MISSILE, 6,"rocket_col7"},
                              {TID_MISSILE, 7,"rocket_col8"},
                              {TID_MISSILE_NORMALS, 0,"rocket_nor"},
                              {TID_MISSILE_NORMALS, 1,"rocket_nor2"},
                              {TID_MISSILE_NORMALS, 2,"rocket_nor3"},
                              {TID_MISSILE_NORMALS, 3,"rocket_nor4"},
                              {TID_MISSILE_NORMALS, 4,"rocket_nor5"},
                              {TID_MISSILE_NORMALS, 5,"rocket_nor6"},
                              {TID_MISSILE_NORMALS, 6,"rocket_nor7"},
                              {TID_MISSILE_NORMALS, 7,"rocket_nor8"},
                              {TID_MISSILE_EM, 0,"rocket_em"},
                              {TID_MISSILE_EM, 1,"rocket_em2"},
                              {TID_MISSILE_EM, 2,"rocket_em3"},
                              {TID_MISSILE_EM, 3,"rocket_em4"},
                              {TID_MISSILE_EM, 4,"rocket_em5"},
                              {TID_MISSILE_EM, 5,"rocket_em6"},
                              {TID_MISSILE_EM, 6,"rocket_em7"},
                              {TID_MISSILE_EM, 7,"rocket_em8"},

                              {TID_MINE2, 0,"mine2_col"},
                              {TID_MINE2, 1,"mine2_col2" },
                              {TID_MINE2, 2,"mine2_col3"},
                              {TID_MINE2, 3,"mine2_col4"},
                              {TID_MINE2, 4,"mine2_col5"},
                              {TID_MINE2, 5,"mine2_col6"},
                              {TID_MINE2, 6,"mine2_col7"},
                              {TID_MINE2, 7,"mine2_col8"},
                              {TID_MINE2_NORMALS, 0,"mine2_nor"},
                              {TID_MINE2_NORMALS, 1,"mine2_nor2"},
                              {TID_MINE2_NORMALS, 2,"mine2_nor3"},
                              {TID_MINE2_NORMALS, 3,"mine2_nor4"},
                              {TID_MINE2_NORMALS, 4,"mine2_nor5"},
                              {TID_MINE2_NORMALS, 5,"mine2_nor6"},
                              {TID_MINE2_NORMALS, 6,"mine2_nor7"},
                              {TID_MINE2_NORMALS, 7,"mine2_nor8"},

                              {TID_COBRA, 0,"cobra_col"},
                              {TID_COBRA, 1,"cobra_col2" },
                              {TID_COBRA, 2,"cobra_col3"},
                              {TID_COBRA, 3,"cobra_col4"},
                              {TID_COBRA, 4,"cobra_col5"},
                              {TID_COBRA, 5,"cobra_col6"},
                              {TID_COBRA, 6,"cobra_col7"},
                              {TID_COBRA, 7,"cobra_col8"},
                              {TID_COBRA_NORMALS, 0,"cobra_nor"},
                              {TID_COBRA_NORMALS, 1,"cobra_nor2"},
                              {TID_COBRA_NORMALS, 2,"cobra_nor3"},
                              {TID_COBRA_NORMALS, 3,"cobra_nor4"},
                              {TID_COBRA_NORMALS, 4,"cobra_nor5"},
                              {TID_COBRA_NORMALS, 5,"cobra_nor6"},
                              {TID_COBRA_NORMALS, 6,"cobra_nor7"},
                              {TID_COBRA_NORMALS, 7,"cobra_nor8"},
                              {TID_COBRA_EM, 0,"cobra_em"},
                              {TID_COBRA_EM, 1,"cobra_em2"},
                              {TID_COBRA_EM, 2,"cobra_em3"},
                              {TID_COBRA_EM, 3,"cobra_em4"},
                              {TID_COBRA_EM, 4,"cobra_em5"},
                              {TID_COBRA_EM, 5,"cobra_em6"},
                              {TID_COBRA_EM, 6,"cobra_em7"},
                              {TID_COBRA_EM, 7,"cobra_em8"},
                              {TID_COBRA_EXTRA, 0,"cobra_extra_col"},
                              {TID_COBRA_EXTRA, 1,"cobra_extra_col2" },
                              {TID_COBRA_EXTRA, 2,"cobra_extra_col3"},
                              {TID_COBRA_EXTRA, 3,"cobra_extra_col4"},
                              {TID_COBRA_EXTRA, 4,"cobra_extra_col5"},
                              {TID_COBRA_EXTRA, 5,"cobra_extra_col6"},
                              {TID_COBRA_EXTRA, 6,"cobra_extra_col7"},
                              {TID_COBRA_EXTRA, 7,"cobra_extra_col8"},
                              {TID_COBRA_EXTRA_NORMALS, 0,"cobra_extra_nor"},
                              {TID_COBRA_EXTRA_NORMALS, 1,"cobra_extra_nor2"},
                              {TID_COBRA_EXTRA_NORMALS, 2,"cobra_extra_nor3"},
                              {TID_COBRA_EXTRA_NORMALS, 3,"cobra_extra_nor4"},
                              {TID_COBRA_EXTRA_NORMALS, 4,"cobra_extra_nor5"},
                              {TID_COBRA_EXTRA_NORMALS, 5,"cobra_extra_nor6"},
                              {TID_COBRA_EXTRA_NORMALS, 6,"cobra_extra_nor7"},
                              {TID_COBRA_EXTRA_NORMALS, 7,"cobra_extra_nor8"},
                              {TID_COBRA_EXTRA_EM, 0,"cobra_extra_em"},
                              {TID_COBRA_EXTRA_EM, 1,"cobra_extra_em2"},
                              {TID_COBRA_EXTRA_EM, 2,"cobra_extra_em3"},
                              {TID_COBRA_EXTRA_EM, 3,"cobra_extra_em4"},
                              {TID_COBRA_EXTRA_EM, 4,"cobra_extra_em5"},
                              {TID_COBRA_EXTRA_EM, 5,"cobra_extra_em6"},
                              {TID_COBRA_EXTRA_EM, 6,"cobra_extra_em7"},
                              {TID_COBRA_EXTRA_EM, 7,"cobra_extra_em8"},
                              {TID_THARGOID, 0,"thargoid_col"},
                              {TID_THARGOID, 1,"thargoid_col2" },
                              {TID_THARGOID, 2,"thargoid_col3"},
                              {TID_THARGOID, 3,"thargoid_col4"},
                              {TID_THARGOID, 4,"thargoid_col5"},
                              {TID_THARGOID, 5,"thargoid_col6"},
                              {TID_THARGOID, 6,"thargoid_col7"},
                              {TID_THARGOID, 7,"thargoid_col8"},
                              {TID_THARGOID_EM, 0,"thargoid_em"},
                              {TID_THARGOID_EM, 1,"thargoid_em2" },
                              {TID_THARGOID_EM, 2,"thargoid_em3"},
                              {TID_THARGOID_EM, 3,"thargoid_em4"},
                              {TID_THARGOID_EM, 4,"thargoid_em5"},
                              {TID_THARGOID_EM, 5,"thargoid_em6"},
                              {TID_THARGOID_EM, 6,"thargoid_em7"},
                              {TID_THARGOID_EM, 7,"thargoid_em8"},
                              {TID_THARGOID_NORMALS, 0,"thargoid_nor"},
                              {TID_THARGOID_NORMALS, 1,"thargoid_nor2"},
                              {TID_THARGOID_NORMALS, 2,"thargoid_nor3"},
                              {TID_THARGOID_NORMALS, 3,"thargoid_nor4"},
                              {TID_THARGOID_NORMALS, 4,"thargoid_nor5"},
                              {TID_THARGOID_NORMALS, 5,"thargoid_nor6"},
                              {TID_THARGOID_NORMALS, 6,"thargoid_nor7"},
                              {TID_THARGOID_NORMALS, 7,"thargoid_nor8"},
                              {TID_COCKPIT, 0, "cockpit"},
                              {TID_MENUITEM,0,"menuitem"},
                              {TID_MENUITEM_HOVER,0,"menuitem_hover"}

                           };


   m_txtrscArray = l_txtrscMain;

   m_cntTextureRsc = sizeof(l_txtrscMain) / sizeof(l_txtrscMain[0]);
   return true;
}

// -- getter auf protected members
//
int GAME::getTextureRscCount()
{
   return m_cntTextureRsc;
}


//--------------------------------------------------------------------------------------
// Return the BPP for a particular format
//--------------------------------------------------------------------------------------

size_t PSXBitsPerPixel( _In_ DXGI_FORMAT fmt )
{
    switch( fmt )
    {
    case DXGI_FORMAT_R32G32B32A32_TYPELESS:
    case DXGI_FORMAT_R32G32B32A32_FLOAT:
    case DXGI_FORMAT_R32G32B32A32_UINT:
    case DXGI_FORMAT_R32G32B32A32_SINT:
        return 128;

    case DXGI_FORMAT_R32G32B32_TYPELESS:
    case DXGI_FORMAT_R32G32B32_FLOAT:
    case DXGI_FORMAT_R32G32B32_UINT:
    case DXGI_FORMAT_R32G32B32_SINT:
        return 96;

    case DXGI_FORMAT_R16G16B16A16_TYPELESS:
    case DXGI_FORMAT_R16G16B16A16_FLOAT:
    case DXGI_FORMAT_R16G16B16A16_UNORM:
    case DXGI_FORMAT_R16G16B16A16_UINT:
    case DXGI_FORMAT_R16G16B16A16_SNORM:
    case DXGI_FORMAT_R16G16B16A16_SINT:
    case DXGI_FORMAT_R32G32_TYPELESS:
    case DXGI_FORMAT_R32G32_FLOAT:
    case DXGI_FORMAT_R32G32_UINT:
    case DXGI_FORMAT_R32G32_SINT:
    case DXGI_FORMAT_R32G8X24_TYPELESS:
    case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
    case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
    case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
        return 64;

    case DXGI_FORMAT_R10G10B10A2_TYPELESS:
    case DXGI_FORMAT_R10G10B10A2_UNORM:
    case DXGI_FORMAT_R10G10B10A2_UINT:
    case DXGI_FORMAT_R11G11B10_FLOAT:
    case DXGI_FORMAT_R8G8B8A8_TYPELESS:
    case DXGI_FORMAT_R8G8B8A8_UNORM:
    case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
    case DXGI_FORMAT_R8G8B8A8_UINT:
    case DXGI_FORMAT_R8G8B8A8_SNORM:
    case DXGI_FORMAT_R8G8B8A8_SINT:
    case DXGI_FORMAT_R16G16_TYPELESS:
    case DXGI_FORMAT_R16G16_FLOAT:
    case DXGI_FORMAT_R16G16_UNORM:
    case DXGI_FORMAT_R16G16_UINT:
    case DXGI_FORMAT_R16G16_SNORM:
    case DXGI_FORMAT_R16G16_SINT:
    case DXGI_FORMAT_R32_TYPELESS:
    case DXGI_FORMAT_D32_FLOAT:
    case DXGI_FORMAT_R32_FLOAT:
    case DXGI_FORMAT_R32_UINT:
    case DXGI_FORMAT_R32_SINT:
    case DXGI_FORMAT_R24G8_TYPELESS:
    case DXGI_FORMAT_D24_UNORM_S8_UINT:
    case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
    case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
    case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
    case DXGI_FORMAT_R8G8_B8G8_UNORM:
    case DXGI_FORMAT_G8R8_G8B8_UNORM:
    case DXGI_FORMAT_B8G8R8A8_UNORM:
    case DXGI_FORMAT_B8G8R8X8_UNORM:
    case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
    case DXGI_FORMAT_B8G8R8A8_TYPELESS:
    case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
    case DXGI_FORMAT_B8G8R8X8_TYPELESS:
    case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
        return 32;

    case DXGI_FORMAT_R8G8_TYPELESS:
    case DXGI_FORMAT_R8G8_UNORM:
    case DXGI_FORMAT_R8G8_UINT:
    case DXGI_FORMAT_R8G8_SNORM:
    case DXGI_FORMAT_R8G8_SINT:
    case DXGI_FORMAT_R16_TYPELESS:
    case DXGI_FORMAT_R16_FLOAT:
    case DXGI_FORMAT_D16_UNORM:
    case DXGI_FORMAT_R16_UNORM:
    case DXGI_FORMAT_R16_UINT:
    case DXGI_FORMAT_R16_SNORM:
    case DXGI_FORMAT_R16_SINT:
    case DXGI_FORMAT_B5G6R5_UNORM:
    case DXGI_FORMAT_B5G5R5A1_UNORM:

#ifdef DXGI_1_2_FORMATS
    case DXGI_FORMAT_B4G4R4A4_UNORM:
#endif
        return 16;

    case DXGI_FORMAT_R8_TYPELESS:
    case DXGI_FORMAT_R8_UNORM:
    case DXGI_FORMAT_R8_UINT:
    case DXGI_FORMAT_R8_SNORM:
    case DXGI_FORMAT_R8_SINT:
    case DXGI_FORMAT_A8_UNORM:
        return 8;

    case DXGI_FORMAT_R1_UNORM:
        return 1;

    case DXGI_FORMAT_BC1_TYPELESS:
    case DXGI_FORMAT_BC1_UNORM:
    case DXGI_FORMAT_BC1_UNORM_SRGB:
    case DXGI_FORMAT_BC4_TYPELESS:
    case DXGI_FORMAT_BC4_UNORM:
    case DXGI_FORMAT_BC4_SNORM:
        return 4;

    case DXGI_FORMAT_BC2_TYPELESS:
    case DXGI_FORMAT_BC2_UNORM:
    case DXGI_FORMAT_BC2_UNORM_SRGB:
    case DXGI_FORMAT_BC3_TYPELESS:
    case DXGI_FORMAT_BC3_UNORM:
    case DXGI_FORMAT_BC3_UNORM_SRGB:
    case DXGI_FORMAT_BC5_TYPELESS:
    case DXGI_FORMAT_BC5_UNORM:
    case DXGI_FORMAT_BC5_SNORM:
    case DXGI_FORMAT_BC6H_TYPELESS:
    case DXGI_FORMAT_BC6H_UF16:
    case DXGI_FORMAT_BC6H_SF16:
    case DXGI_FORMAT_BC7_TYPELESS:
    case DXGI_FORMAT_BC7_UNORM:
    case DXGI_FORMAT_BC7_UNORM_SRGB:
        return 8;

    default:
        return 0;
    }
}

//--------------------------------------------------------------------------------------
// Get surface information for a particular format
//--------------------------------------------------------------------------------------
void PSXGetSurfaceInfo( _In_ size_t width,
                            _In_ size_t height,
                            _In_ DXGI_FORMAT fmt,
                            _Out_opt_ size_t* outNumBytes,
                            _Out_opt_ size_t* outRowBytes,
                            _Out_opt_ size_t* outNumRows )
{
    size_t numBytes = 0;
    size_t rowBytes = 0;
    size_t numRows = 0;

    bool bc = false;
    bool packed  = false;
    size_t bcnumBytesPerBlock = 0;
    switch (fmt)
    {
    case DXGI_FORMAT_BC1_TYPELESS:
    case DXGI_FORMAT_BC1_UNORM:
    case DXGI_FORMAT_BC1_UNORM_SRGB:
    case DXGI_FORMAT_BC4_TYPELESS:
    case DXGI_FORMAT_BC4_UNORM:
    case DXGI_FORMAT_BC4_SNORM:
        bc=true;
        bcnumBytesPerBlock = 8;
        break;

    case DXGI_FORMAT_BC2_TYPELESS:
    case DXGI_FORMAT_BC2_UNORM:
    case DXGI_FORMAT_BC2_UNORM_SRGB:
    case DXGI_FORMAT_BC3_TYPELESS:
    case DXGI_FORMAT_BC3_UNORM:
    case DXGI_FORMAT_BC3_UNORM_SRGB:
    case DXGI_FORMAT_BC5_TYPELESS:
    case DXGI_FORMAT_BC5_UNORM:
    case DXGI_FORMAT_BC5_SNORM:
    case DXGI_FORMAT_BC6H_TYPELESS:
    case DXGI_FORMAT_BC6H_UF16:
    case DXGI_FORMAT_BC6H_SF16:
    case DXGI_FORMAT_BC7_TYPELESS:
    case DXGI_FORMAT_BC7_UNORM:
    case DXGI_FORMAT_BC7_UNORM_SRGB:
        bc = true;
        bcnumBytesPerBlock = 16;
        break;

    case DXGI_FORMAT_R8G8_B8G8_UNORM:
    case DXGI_FORMAT_G8R8_G8B8_UNORM:
        packed = true;
        break;
    }

    if (bc)
    {
        size_t numBlocksWide = 0;
        if (width > 0)
        {
            numBlocksWide = std::max<size_t>( 1, (width + 3) / 4 );
        }
        size_t numBlocksHigh = 0;
        if (height > 0)
        {
            numBlocksHigh = std::max<size_t>( 1, (height + 3) / 4 );
        }
        rowBytes = numBlocksWide * bcnumBytesPerBlock;
        numRows = numBlocksHigh;
    }
    else if (packed)
    {
        rowBytes = ( ( width + 1 ) >> 1 ) * 4;
        numRows = height;
    }
    else
    {
        size_t bpp = PSXBitsPerPixel( fmt );
        rowBytes = ( width * bpp + 7 ) / 8; // round up to nearest byte
        numRows = height;
    }

    numBytes = rowBytes * numRows;
    if (outNumBytes)
    {
        *outNumBytes = numBytes;
    }
    if (outRowBytes)
    {
        *outRowBytes = rowBytes;
    }
    if (outNumRows)
    {
        *outNumRows = numRows;
    }
}

