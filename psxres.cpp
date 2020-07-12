/*
 *------------------------------------------------------------------------
 *      Object: MESHRESOURCE
 *         $Id: psxres.cpp,v 1.17 2010/02/04 15:40:00 molle Exp $
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

#include "stdafx.h"
#include "psxd3d.h"
#include "psxglob.h"

MESHRESOURCEMANAGER::MESHRESOURCEMANAGER()
{
   m_iVertices        = NULL;
   m_iFaces           = NULL;
   m_iMaterials       = NULL;
   m_resArray      = NULL;
   m_faceinfArray  = NULL;
   m_pfaceinfArray = NULL;
//   m_dt3extArray   = NULL;
   m_trimeshdataidArray = NULL;
}

// -- bei Programmstart aufrufen
//
bool MESHRESOURCEMANAGER::init()
{
   if ( allocate() == false) return false;
   if ( locatePFaceInfArray() == false) return false;
   if ( updateBigRes() == false) return false;

   return true;
}

// -- von scaler aus aufrufen
//
bool MESHRESOURCEMANAGER::scaler()
{
   if ( loadPureMesh() == false) return false;
   if ( initBigRes() == false) return false;
   //if ( modifyMeshes() == false) return false; doch genau nicht scalen
   if ( generateBigRes() == false) return false;
   //if ( scaleFlagshipDoors() == false) return false;
   if ( recalcMinMax() == false) return false;
   if ( initAllFaceInfoArrays() == false) return false;
   if ( save() == false) return false;
   if ( release() == false) return false;

   return true;
}


// -- *.psx Mesh dateien laden
//
bool MESHRESOURCEMANAGER::loadPureMesh()
{
   HANDLE hMaterialFile, hVertexFile, hFaceFile,hResourceOffsetFile;
   int l_iNeededMemory;
   int cnt_material, cnt_vert;

   struct PSXFILEOPENLIST {
     HANDLE* m_pHandle;
     wchar_t*   m_wcFileName;
     DWORD*  m_pdwShallRead;
     void**  m_ppDestination;
   } l_arr_FileList[] = 
   {
      {  &hMaterialFile,       
         L"materials.psx", 
         &m_cntMaterialBytes,
         (void**) &m_iMaterials },
      {  &hVertexFile,         
         L"vertices.psx",  
         &m_cntVertexBytes,
         (void**) &m_iVertices},
      {  &hFaceFile,           
         L"faces.psx",     
         &m_cntFaceBytes,
         (void**) &m_iFaces},
      {  &hResourceOffsetFile, 
         L"offsets.psx",   
         &m_cntResourceBytes,
         (void**) &m_resArray}
   };
   int l_cntFileList = sizeof(l_arr_FileList)/sizeof(PSXFILEOPENLIST);

   int l_iResourceOffsets, 
       l_iTexture;

   int i;

   WriteLogFile("open the files.\n");

   for (i=0; i < l_cntFileList; i++)
   {
      HANDLE l_hToOpen;
      l_hToOpen = CreateFile(
         l_arr_FileList[i].m_wcFileName,
         GENERIC_READ ,        // access (read-write) mode 
         0 ,	                  // share mode 
         NULL ,                // pointer to security attributes 
         OPEN_EXISTING ,       // how to create 
         FILE_ATTRIBUTE_NORMAL,// file attributes 
         NULL );	

      if (l_hToOpen==INVALID_HANDLE_VALUE) 
      {
         wchar_t l_wcFileNotFound[100];
		 PSXPrintf2(__LINE__,l_wcFileNotFound, sizeof(l_wcFileNotFound),
                 L"Mesh-Resourcefile %s not found! Aborting...\n",
                 l_arr_FileList[i].m_wcFileName);
         MessageBox(g_hWnd,l_wcFileNotFound,
                    L"Loading Mesh-resources",
                    MB_ICONSTOP);
         WriteLogFile2(l_wcFileNotFound);
         return false;
      }
      *(l_arr_FileList[i].m_pHandle) = l_hToOpen;
   }

   // -- getting info about files
   //
   m_cntMaterialBytes       = GetFileSize(hMaterialFile,NULL);
   m_cntVertexBytes         = GetFileSize(hVertexFile,NULL);
   m_cntResourceBytes       = GetFileSize(hResourceOffsetFile,NULL);
   m_cntFaceBytes           = GetFileSize(hFaceFile,NULL);

   // --- calculating recordcount from bytecount 
   //
   cnt_material  = m_cntMaterialBytes /sizeof(PSXMATERIAL); 
   m_cntFace     = m_cntFaceBytes     /sizeof(PSXFACE);
   m_cntRes      = m_cntResourceBytes /sizeof(PSXRESOURCEOFFSET);
   cnt_vert      = m_cntVertexBytes   /sizeof(PSXVERTEX);

   // -- check if bytecount is a multiple of the recordlength
   // 
   {
      bool l_bErrorHappened;
      char l_cNotMultipleOf[300];
  
      l_bErrorHappened = false;

      if (m_cntMaterialBytes != (cnt_material * sizeof(PSXMATERIAL)))
      {
         PSXStrCpy(__LINE__, l_cNotMultipleOf, sizeof(l_cNotMultipleOf), "Materials:");
         l_bErrorHappened = true;
      }

      if (m_cntFaceBytes     != (m_cntFace     * sizeof(PSXFACE)))
      {
         PSXStrCpy(__LINE__, l_cNotMultipleOf, sizeof(l_cNotMultipleOf), "Faces:");
         l_bErrorHappened = true;
      }

      if (m_cntVertexBytes  != (cnt_vert     * sizeof(PSXVERTEX)))
      {
         PSXStrCpy(__LINE__, l_cNotMultipleOf, sizeof(l_cNotMultipleOf), "Vertices:");
         l_bErrorHappened = true;
      }

      if (m_cntResourceBytes != (m_cntRes *sizeof(PSXRESOURCEOFFSET)))
      {
         PSXStrCpy(__LINE__, l_cNotMultipleOf, sizeof(l_cNotMultipleOf), "Resourceoffsets:");
         l_bErrorHappened = true;
      }

      if (l_bErrorHappened)
      {
         PSXStrCat(__LINE__,l_cNotMultipleOf, sizeof(l_cNotMultipleOf), 
                "bytecount is not a multiple of the record length, quit\n");
         WriteLogFile(l_cNotMultipleOf);
         MessageBoxA(g_hWnd, l_cNotMultipleOf, 
                    "Consistency Checkcalculations",
                    MB_ICONSTOP);
         return (false);
      }
   }

   // -- Calculating Memory Usage
   //
   m_cntFaceInfBytes  = m_cntFace * sizeof(PSXFACEINFO);
   m_cntPFaceInfBytes = m_cntRes * sizeof(PSXFACEINFO**);

   l_iNeededMemory = m_cntMaterialBytes +
                     m_cntVertexBytes +
                     m_cntResourceBytes +
                     m_cntFaceBytes +
                     m_cntFaceInfBytes +
                     m_cntPFaceInfBytes +
                     TID_LASTENUM * sizeof(TEXTURE);

   m_pMeshResourceBlock = VirtualAlloc(NULL, l_iNeededMemory,
                                   MEM_COMMIT,
                                   PAGE_READWRITE);
   if (m_pMeshResourceBlock==NULL)
   {
      hr2message(__LINE__,GetLastError(),"VirtualAlloc(RESOURCES)");
      return(false);
   }

   m_iVertices             = int(m_pMeshResourceBlock);
   m_iFaces                = m_iVertices + m_cntVertexBytes;
   m_iMaterials            = m_iFaces    + m_cntFaceBytes;

   l_iResourceOffsets      = m_iMaterials + m_cntMaterialBytes;
   m_iFaceInf      = l_iResourceOffsets + m_cntResourceBytes;
   m_iPFaceInf = m_iFaceInf + m_cntFace * sizeof(PSXFACEINFO); 
   l_iTexture              = m_iPFaceInf + m_cntRes * sizeof(PSXFACEINFO**);

/*
   vertices           = (PSXVERTEX*)        l_iVertices;
   faces              = (PSXFACE*)          l_iFaces;
   materials          = (PSXMATERIAL*)      l_iMaterials;
*/

   m_resArray         = (PSXRESOURCEOFFSET*)l_iResourceOffsets;
   m_faceinfArray     = (PSXFACEINFO*)  m_iFaceInf;
   m_pfaceinfArray    = (PSXFACEINFO**) m_iPFaceInf;
   g_txtArray         = (TEXTURE*)      l_iTexture;

   // -- Daten endlich einlesen
   //
   for (i=0; i<l_cntFileList; i++)
   {
      DWORD l_dwBytesRead;
      ReadFile(
         *l_arr_FileList[i].m_pHandle,        // handle of file to read 
         *l_arr_FileList[i].m_ppDestination,  // address of buffer 
         *l_arr_FileList[i].m_pdwShallRead,   // number of bytes to read 
         &l_dwBytesRead,              // address of number of bytes read 
         NULL                         // address of overlap structure 
      );
      CloseHandle(*(l_arr_FileList[i].m_pHandle));

      if (l_dwBytesRead != (*l_arr_FileList[i].m_pdwShallRead))
      {
         wchar_t l_wcNotComplete[100];
         PSXPrintf2(__LINE__,l_wcNotComplete, sizeof(l_wcNotComplete),
                 L"%s was not loaded completely, aborting."
                 L" Should have read %i, got only %i \n",
                 l_arr_FileList[i].m_wcFileName,
                 *l_arr_FileList[i].m_pdwShallRead,
                 l_dwBytesRead );

         WriteLogFile2(l_wcNotComplete);
         MessageBox(g_hWnd, l_wcNotComplete, L"Loading Resourcefile",
                    MB_ICONSTOP);
         return(false);
      }
   }
   
   // --- complete the count fields in resourceoffset array and 
   //
   for (i=0; i<m_cntRes;i++)
   {
      int taken_vertex_count, taken_face_count, taken_material_count;
      if ((i+1)==m_cntRes)
      {
        taken_vertex_count   = m_cntVertexBytes;
        taken_face_count     = m_cntFaceBytes;
        taken_material_count = m_cntMaterialBytes;
      } else {
        taken_vertex_count   = m_resArray[i+1].vertex_offset;
        taken_face_count     = m_resArray[i+1].face_offset;
        taken_material_count = m_resArray[i+1].material_offset;
      }
      m_resArray[i].cnt_vert     = 
             (taken_vertex_count - m_resArray[i].vertex_offset)/ 
              sizeof(PSXVERTEX);

      m_resArray[i].cnt_faces    = 
             (taken_face_count     - m_resArray[i].face_offset)/ 
              sizeof(PSXFACE);

      m_resArray[i].cnt_material = 
             (taken_material_count - m_resArray[i].material_offset)/ 
              sizeof(PSXMATERIAL);
   
/*
      m_resArray[i].vertex_offset   = m_resArray[i].vertex_offset   + m_iVertices;
      m_resArray[i].face_offset     = m_resArray[i].face_offset     + m_iFaces;
      m_resArray[i].material_offset = m_resArray[i].material_offset + m_iMaterials;
*/
   }

   return true;
}

// -- alle *.psx dateien laden, von denen ausgegangen wird, dass sie durch scaler.exe bearbeitet wurden
//
bool MESHRESOURCEMANAGER::allocate()
{
   HANDLE hMaterialFile, 
          hResourceOffsetFile, 
          hBigResFile, 
          hBigResItemFile;
   int l_iNeededMemory;
   int cnt_material, cnt_vert, l_cntBigResItem, l_cntBigRes, l_cntFaceInf; 

   struct PSXFILEOPENLIST {
     HANDLE* m_pHandle;
     HANDLE* m_pMapHandle;
     wchar_t* m_wcFileName;
     DWORD*  m_pdwShallRead;
     void**  m_ppDestination;
     bool    m_bReadIn;
   } l_arr_FileList[] = 
   {
      {  &hMaterialFile, NULL,
         L"materials.psx", 
         &m_cntMaterialBytes,
         (void**) &m_iMaterials, true },
      {  &m_hVerticesFile, &m_hMapVertices,
         L"vertices.psx",  
         &m_cntVertexBytes,
         (void**) &m_iVertices, false},
      {  &m_hFacesFile, &m_hMapFaces,
         L"faces.psx",     
         &m_cntFaceBytes,
         (void**) &m_iFaces, false},

      {  &hResourceOffsetFile,NULL,
         L"offsets.psx",   
         &m_cntResourceBytes,
         (void**) &m_resArray, true},

      {  &hBigResFile,NULL,
         L"bigres.psx",   
         &m_cntBigResBytes,
         (void**) &m_iBigRes, true},

      {  &hBigResItemFile,NULL,
         L"bresitem.psx",   
         &m_cntBigResItemBytes,
         (void**) &m_iBigResItem, true},

      {  &m_hFaceInfFile, &m_hMapFaceInf,
         L"faceinf.psx",   
         &m_cntFaceInfBytes,
         (void**) &m_iFaceInf, false},
   };
   int l_cntFileList = sizeof(l_arr_FileList)/sizeof(PSXFILEOPENLIST);

   int l_iResourceOffsets, 
       l_iTexture;

   int i;

   WriteLogFile("open the files.\n");

   for (i=0; i < l_cntFileList; i++)
   {
      HANDLE l_hToOpen;
      l_hToOpen = CreateFile(
         l_arr_FileList[i].m_wcFileName,
         GENERIC_READ ,        // access (read-write) mode 
         0 ,	                  // share mode 
         NULL ,                // pointer to security attributes 
         OPEN_EXISTING ,       // how to create 
         FILE_ATTRIBUTE_NORMAL,// file attributes 
         NULL );	

      if (l_hToOpen==INVALID_HANDLE_VALUE) 
      {
         wchar_t l_wcFileNotFound[100];
         PSXPrintf2(__LINE__,l_wcFileNotFound, sizeof(l_wcFileNotFound),
                 L"Mesh-Resourcefile %s not found! Aborting...\n",
                  l_arr_FileList[i].m_wcFileName);
         MessageBox(g_hWnd,l_wcFileNotFound,
                    L"Loading Mesh-resources",
                    MB_ICONSTOP);
         WriteLogFile2(l_wcFileNotFound);
         return(false);
      }
      *(l_arr_FileList[i].m_pHandle) = l_hToOpen;

      *l_arr_FileList[i].m_pdwShallRead = GetFileSize(l_hToOpen,NULL);
   }

   // -- getting info about files
   //
/*
   m_cntVertexBytes         = GetFileSize(hVertexFile,NULL);
   m_cntResourceBytes       = GetFileSize(hResourceOffsetFile,NULL);
   m_cntFaceBytes           = GetFileSize(hFaceFile,NULL);
   m_cntBigResBytes         = GetFileSize(hBigResFile,NULL);
   m_cntBigResItemBytes     = GetFileSize(hBigResItemFile,NULL);
*/

   // --- calculating recordcount from bytecount 
   //
   cnt_material       = m_cntMaterialBytes   /sizeof(PSXMATERIAL); 
   m_cntFace          = m_cntFaceBytes       /sizeof(PSXFACE);
   m_cntRes           = m_cntResourceBytes   /sizeof(PSXRESOURCEOFFSET);
   cnt_vert           = m_cntVertexBytes     /sizeof(PSXVERTEX);
   l_cntBigRes        = m_cntBigResBytes     /sizeof(BIGRES);
   l_cntBigResItem    = m_cntBigResItemBytes /sizeof(BIGRESITEM);
   l_cntFaceInf       = m_cntFaceInfBytes    /sizeof(PSXFACEINFO);
   
   
   // -- check if bytecount is a multiple of the recordlength
   // 
   {
      bool l_bErrorHappened;
      char l_cNotMultipleOf[300];
  
      l_bErrorHappened = false;
      if (m_cntMaterialBytes != (cnt_material * sizeof(PSXMATERIAL)))
      {
         PSXStrCpy(__LINE__, l_cNotMultipleOf, sizeof(l_cNotMultipleOf), "Materials:");
         l_bErrorHappened = true;
      }
      if (m_cntFaceBytes     != (m_cntFace     * sizeof(PSXFACE)))
      {
         PSXStrCpy(__LINE__, l_cNotMultipleOf, sizeof(l_cNotMultipleOf), "Faces:");
         l_bErrorHappened = true;
      }

      if (m_cntFaceInfBytes != (m_cntFace     * sizeof(PSXFACEINFO)))
      {
         PSXStrCpy(__LINE__, l_cNotMultipleOf, sizeof(l_cNotMultipleOf), "FaceInfo:");
         l_bErrorHappened = true;
      }

      if (m_cntVertexBytes  != (cnt_vert     * sizeof(PSXVERTEX)))
      {
         PSXStrCpy(__LINE__, l_cNotMultipleOf, sizeof(l_cNotMultipleOf), "Vertices:");
         l_bErrorHappened = true;
      }
      if (m_cntResourceBytes != (m_cntRes *sizeof(PSXRESOURCEOFFSET)))
      {
         PSXStrCpy(__LINE__, l_cNotMultipleOf, sizeof(l_cNotMultipleOf), "Resourceoffsets:");
         l_bErrorHappened = true;
      }

      if (m_cntBigResBytes != (l_cntBigRes *sizeof(BIGRES)))
      {
         PSXStrCpy(__LINE__, l_cNotMultipleOf, sizeof(l_cNotMultipleOf), "BigRes:");
         l_bErrorHappened = true;
      }

      if (m_cntBigResItemBytes != (l_cntBigResItem *sizeof(BIGRESITEM)))
      {
         PSXStrCpy(__LINE__, l_cNotMultipleOf, sizeof(l_cNotMultipleOf), "BigResItem:");
         l_bErrorHappened = true;
      }

      if (l_bErrorHappened)
      {
         PSXStrCat(__LINE__,l_cNotMultipleOf, sizeof(l_cNotMultipleOf), 
                "bytecount is not a multiple of the record length, quit\n");
         WriteLogFile(l_cNotMultipleOf);
         MessageBoxA(g_hWnd, l_cNotMultipleOf, 
                    "Consistency Checkcalculations",
                    MB_ICONSTOP);
         return (false);
      }
   }

   // -- Calculating Memory Usage
   //
   m_cntPFaceInfBytes = m_cntRes * sizeof(PSXFACEINFO**);

   l_iNeededMemory = m_cntMaterialBytes +
                     m_cntResourceBytes +
                     m_cntPFaceInfBytes +
                     TID_LASTENUM * sizeof(TEXTURE);

   m_pMeshResourceBlock = VirtualAlloc(NULL, l_iNeededMemory,
                                   MEM_COMMIT,
                                   PAGE_READWRITE);
   if (m_pMeshResourceBlock==NULL)
   {
      hr2message(__LINE__,GetLastError(),"VirtualAlloc(RESOURCES)");
      return(false);
   }

   m_iMaterials            = int(m_pMeshResourceBlock);

   l_iResourceOffsets      = m_iMaterials + m_cntMaterialBytes;
   m_iPFaceInf             = l_iResourceOffsets + m_cntResourceBytes;
   l_iTexture              = m_iPFaceInf + m_cntRes * sizeof(PSXFACEINFO**);

   m_resArray         = (PSXRESOURCEOFFSET*)l_iResourceOffsets;
   m_pfaceinfArray    = (PSXFACEINFO**) m_iPFaceInf;
   g_txtArray         = (TEXTURE*)      l_iTexture;
   WriteLogFile("g_txtArray assigned\n");
   m_iBigRes               = int(g_bigresArray);
   m_iBigResItem           = int(g_bigresitemArray);

   // -- Daten entweder endlich einlesen oder filemappen
   //
   for (i=0; i<l_cntFileList; i++)
   {
      DWORD l_dwBytesRead;

/*
      WriteLogFile("reading in ");
      WriteLogFile (l_arr_FileList[i].m_cFileName);
      if (g_txtArray) 
      {
         WriteLogFile(": g_txtArray still valid\n");
      }else{
         WriteLogFile(": g_txtArray NOT valid\n");
      }
*/

      if (l_arr_FileList[i].m_bReadIn)
      {
         ReadFile(
            *l_arr_FileList[i].m_pHandle,        // handle of file to read 
            *l_arr_FileList[i].m_ppDestination,  // address of buffer 
            *l_arr_FileList[i].m_pdwShallRead,   // number of bytes to read 
            &l_dwBytesRead,              // address of number of bytes read 
            NULL                         // address of overlap structure 
         );
         CloseHandle(*(l_arr_FileList[i].m_pHandle));
   
         if (l_dwBytesRead != (*l_arr_FileList[i].m_pdwShallRead))
         {
            wchar_t l_wcNotComplete[100];
            PSXPrintf2(__LINE__,l_wcNotComplete, sizeof(l_wcNotComplete),
                    L"%s was not loaded completely, aborting."
                    L" Should have read %i, got only %i \n",
                    l_arr_FileList[i].m_wcFileName,
                    *l_arr_FileList[i].m_pdwShallRead,
                    l_dwBytesRead );
   
            WriteLogFile2(l_wcNotComplete);
            MessageBox(g_hWnd, l_wcNotComplete, L"Loading Resourcefile",
                       MB_ICONSTOP);
            return(false);
         }

      }else{

         // -- faceinf memory mappen
         //
         *l_arr_FileList[i].m_pMapHandle= CreateFileMapping(*l_arr_FileList[i].m_pHandle, 
                                           NULL, PAGE_READONLY, 
                                           0,*l_arr_FileList[i].m_pdwShallRead,
										   l_arr_FileList[i].m_wcFileName);
         if (*l_arr_FileList[i].m_pMapHandle==INVALID_HANDLE_VALUE) 
         {
            hr2message(__LINE__,GetLastError(),"CreateFileMapping()");
            return false;
         }
         
         *l_arr_FileList[i].m_ppDestination = 
                                  MapViewOfFile(*l_arr_FileList[i].m_pMapHandle, 
                                                FILE_MAP_READ, 
                                                0, 0, 0);

         if (*l_arr_FileList[i].m_ppDestination==NULL) 
         {
            hr2message(__LINE__,GetLastError(),"MapViewOfFile(g_hMapEnemyFile) #1");
            return false;
         }
      }
   }

   if (g_txtArray == NULL) 
   {
      hr2message(__LINE__,-1,"during allocate() g_txtArray gone NULL");
      return(FALSE);
      
   }


   m_faceinfArray = (PSXFACEINFO*)  m_iFaceInf;
   
   // --- complete the count fields in resourceoffset array and 
   //
   for (i=0; i<m_cntRes;i++)
   {
      int taken_vertex_count, taken_face_count, taken_material_count;
      if ((i+1)==m_cntRes)
      {
        taken_vertex_count   = m_cntVertexBytes;
        taken_face_count     = m_cntFaceBytes;
        taken_material_count = m_cntMaterialBytes;
      } else {
        taken_vertex_count   = m_resArray[i+1].vertex_offset;
        taken_face_count     = m_resArray[i+1].face_offset;
        taken_material_count = m_resArray[i+1].material_offset;
      }
      m_resArray[i].cnt_vert     = 
             (taken_vertex_count - m_resArray[i].vertex_offset)/ 
              sizeof(PSXVERTEX);

      m_resArray[i].cnt_faces    = 
             (taken_face_count     - m_resArray[i].face_offset)/ 
              sizeof(PSXFACE);

      m_resArray[i].cnt_material = 
             (taken_material_count - m_resArray[i].material_offset)/ 
              sizeof(PSXMATERIAL);
   
/*
      m_resArray[i].vertex_offset   = m_resArray[i].vertex_offset   + m_iVertices;
      m_resArray[i].face_offset     = m_resArray[i].face_offset     + m_iFaces;
      m_resArray[i].material_offset = m_resArray[i].material_offset + m_iMaterials;
*/
   }

   m_vertbufArray  = new ID3D11Buffer*[m_cntRes];
   m_indexbufArray = new ID3D11Buffer*[m_cntRes];
   memset(m_vertbufArray, 0 , sizeof(m_vertbufArray[0])*m_cntRes);
   memset(m_indexbufArray, 0 , sizeof(m_indexbufArray[0])*m_cntRes);

   m_trimeshdataidArray = new dTriMeshDataID[m_cntRes];
   memset(m_trimeshdataidArray, 0, sizeof(m_trimeshdataidArray[0]) * m_cntRes);



   return true;
}


// -- Vor-VertexBuffer-Änderungen an den Daten vornehmen
//
bool MESHRESOURCEMANAGER::modifyMeshes()
{
   
   // --- scaling objects
   //
   WriteLogFile("meshresource scaling\n");
   {
      PSXVERTEX* l_pvCurr;

      struct _scalelist {
         RESTYPE resToScale;
         float   fScale;
      } l_arr_Scale[] =  {
         { RESTYPE_ASTEROID,           10.0f}, 
         { RESTYPE_ASTEROIDSCALE2,     20.0f}, 
         { RESTYPE_COBRA_PART1,        10.0f}, 
         { RESTYPE_FLAGSHIPKINGSPLANE,  g_bigresArray[BIGRES_FLAGSHIP].m_fScale },
         { RESTYPE_GUN,         g_bigresArray[BIGRES_FLAGSHIP].m_fScale },
         { RESTYPE_HANGAR,      g_bigresArray[BIGRES_FLAGSHIP].m_fScale },
         { RESTYPE_REACTOR,     0.1f },
         { RESTYPE_POINTER01,   0.1f },
         //{ RESTYPE_RADAR,       100.0f },
         { RESTYPE_BRENNSTAB,   g_bigresArray[BIGRES_FLAGSHIP].m_fScale },
         { RESTYPE_FELDGENERATOR,  g_bigresArray[BIGRES_FLAGSHIP].m_fScale },

         { RESTYPE_LEADER,             8.0f},
         { RESTYPE_FLYER,              8.0f},
         { RESTYPE_MISSILE,            20.0f},
         { RESTYPE_MINE2,              10.0f},
         { RESTYPE_RESCUE,             10.0f},
         { RESTYPE_BOA01,              100.0f},
         { RESTYPE_CARGO01,            10.0f},
         { RESTYPE_CARGO03,            10.0f},
         { RESTYPE_CARGO04,            10.0f},
         { RESTYPE_CARGO05,            10.0f},
         { RESTYPE_CARGO06,            10.0f},
         { RESTYPE_CARGO07,            30.0f},
         { RESTYPE_CARGO08,            20.0f},
         { RESTYPE_TRANSPORT1,         20.0f},

         { RESTYPE_CARGOFLYER,         10.0f},
         { RESTYPE_HUNTER01,           30.0f},
         { RESTYPE_HUNTER02,           10.0f},
         { RESTYPE_HUNTER03,           10.0f},
         { RESTYPE_HUNTER09,           10.0f},
         { RESTYPE_HUNTER10,           20.0f},
         { RESTYPE_HUNTER_X,           10.0f},
         { RESTYPE_LASERHALTER,        100.0f},
         { RESTYPE_LASERGUN,           100.0f},
         //{ RESTYPE_LASERHALTER1,       10.0f},
         //{ RESTYPE_LASERGUN1,          10.0f},
         { RESTYPE_CARGOBOX,           10.0f},
         { RESTYPE_THARGOID,          100.0f},
         { RESTYPE_THARGOIDDRONE,      10.0f},
         { RESTYPE_STARBASEHANGAR,     100.0f},
         { RESTYPE_CORIOLISKINGSPLANE, g_bigresArray[BIGRES_CORIOLIS].m_fScale },
         { RESTYPE_TMPSPEEDUP,  0.1f  }
      };
      int l_cntScale = sizeof(l_arr_Scale)/ sizeof(l_arr_Scale[0]);
      RESTYPE l_resObj;

      for (int i = 0; i < l_cntScale; i ++)
      {
         l_resObj = l_arr_Scale[i].resToScale;

         l_pvCurr = (PSXVERTEX*) (m_resArray[l_resObj].vertex_offset + m_iVertices);

         m_resArray[l_resObj].rad = m_resArray[l_resObj].rad * l_arr_Scale[i].fScale;

         m_resArray[l_resObj].min.x = m_resArray[l_resObj].min.x * l_arr_Scale[i].fScale;
         m_resArray[l_resObj].min.y = m_resArray[l_resObj].min.y * l_arr_Scale[i].fScale;
         m_resArray[l_resObj].min.z = m_resArray[l_resObj].min.z * l_arr_Scale[i].fScale;
         m_resArray[l_resObj].max.x = m_resArray[l_resObj].max.x * l_arr_Scale[i].fScale;
         m_resArray[l_resObj].max.y = m_resArray[l_resObj].max.y * l_arr_Scale[i].fScale;
         m_resArray[l_resObj].max.z = m_resArray[l_resObj].max.z * l_arr_Scale[i].fScale;

         for (int k=0;
                  k < m_resArray[l_resObj].cnt_vert; 
                  k++)
         {
            l_pvCurr[k].x = l_pvCurr[k].x * l_arr_Scale[i].fScale;
            l_pvCurr[k].y = l_pvCurr[k].y * l_arr_Scale[i].fScale;
            l_pvCurr[k].z = l_pvCurr[k].z * l_arr_Scale[i].fScale;
         }
      }
   }

   // --- Objekte vor-rotieren, weil z.B. die KI in y-Richtung fliegen will
   //     aber der Flieger in z-Richtung schaut
   //
   /*
   WriteLogFile("meshresource xy-rotating\n");
      PSXVERTEX* l_pvCurr;

      struct _rotatelist {
         RESTYPE resToRotate;
         float   fAngle1;
         float   fAngle2;
      } l_tempRotateArray[] =  {
         { RESTYPE_HUNTER02,    g_fPi / 2.0f, 0.0f},
         { RESTYPE_HUNTER03,    g_fPi * 1.5f, g_fPi / 2.0f},

         //{ RESTYPE_CARGO03,            g_fPi * 1.5f, g_fPi * 1.5f},
         //{ RESTYPE_CARGO04,            g_fPi * 1.5f, g_fPi * 1.5f},
         //{ RESTYPE_CARGO05,            g_fPi * 1.5f, g_fPi * 1.5f},
         //{ RESTYPE_HUNTER_X,           - g_fPi * 0.5f, 0.0f}, //g_fPi * 1.5f},
         //{ RESTYPE_STARBASE,           - g_fPi * 0.5f, 0.0f}, //g_fPi * 1.5f},
//         { RESTYPE_CARGOFLYER,         g_fPi * 1.5f, g_fPi * 1.5f},
      };
      int l_cntRotate = sizeof(l_tempRotateArray)/ sizeof(l_tempRotateArray[0]);
      RESTYPE l_resObj;
      float l_fAngle1, l_fAngle2;

      for (int i = 0; i < l_cntRotate; i ++)
      {
         l_resObj  = l_tempRotateArray[i].resToRotate;
         l_fAngle1 = l_tempRotateArray[i].fAngle1;
         l_fAngle2 = l_tempRotateArray[i].fAngle2;

         l_pvCurr = (PSXVERTEX*) (m_resArray[l_resObj].vertex_offset + m_iVertices);

         for (int k=0; k < m_resArray[l_resObj].cnt_vert; k++)
         {
            float x,y, z;
            x = l_pvCurr[k].x;
            y = l_pvCurr[k].y;
            l_pvCurr[k].x = cos(l_fAngle1) * x - sin(l_fAngle1) * y;
            l_pvCurr[k].y = sin(l_fAngle1) * x + cos(l_fAngle1) * y;

            x = l_pvCurr[k].x;
            z = l_pvCurr[k].z;
            l_pvCurr[k].x = cos(l_fAngle2) * x - sin(l_fAngle2) * z;
            l_pvCurr[k].z = sin(l_fAngle2) * x + cos(l_fAngle2) * z;

            // -- dasselbe mit den normals
            //
            x = l_pvCurr[k].nx;
            y = l_pvCurr[k].ny;
            l_pvCurr[k].nx = cos(l_fAngle1) * x - sin(l_fAngle1) * y;
            l_pvCurr[k].ny = sin(l_fAngle1) * x + cos(l_fAngle1) * y;

            x = l_pvCurr[k].nx;
            z = l_pvCurr[k].nz;
            l_pvCurr[k].nx = cos(l_fAngle2) * x - sin(l_fAngle2) * z;
            l_pvCurr[k].nz = sin(l_fAngle2) * x + cos(l_fAngle2) * z;
         }
      }
   */

//-----------

   WriteLogFile("SpaceWarp texture anpassen\n");
   {
      PSXVERTEX* l_pvCurr;

      struct _rotatelist {
         RESTYPE resToRotate;
         float   fValue1;
      } l_tempWarpArray[] =  {
         { RESTYPE_SPACEWARP,  0.5f}

      };
      int l_cntWarp = sizeof(l_tempWarpArray)/ sizeof(l_tempWarpArray[0]);
      RESTYPE l_resObj;
      float l_fValue1;

      for (int i = 0; i < l_cntWarp; i ++)
      {
         l_resObj  = l_tempWarpArray[i].resToRotate;
         l_fValue1 = l_tempWarpArray[i].fValue1;

         l_pvCurr = (PSXVERTEX*) (m_resArray[l_resObj].vertex_offset + m_iVertices);

         for (int k=0; k < m_resArray[l_resObj].cnt_vert; k++)
         {
            if(l_pvCurr[k].tu <= l_fValue1)
            {
               l_pvCurr[k].tu = (1.0f + l_pvCurr[k].tu); 
            }

            if(l_pvCurr[k].tu >= 1.0f)
            {
               l_pvCurr[k].tu -= 1.0f;
               l_pvCurr[k].tu  = 1.0f - l_pvCurr[k].tu;
            }
         }
      }
   }

   return true;
}

// -- fuellen des BigRes Array
//
bool MESHRESOURCEMANAGER::initBigRes()
{

   // -- die Verwaltung n-teiliger Resourcen schonmal initialisieren
   //
   WriteLogFile("BigResource initializing\n");
   {
      int k, l_idxBigRes;
   
      struct _bigrestemplate {
        IDXBIGRES idxBigRes;
        float   fScale;
        bool    bTextured;
        bool    bTranslatePivot;
      } l_tempBigResArray[] = {
         { BIGRES_FLAGSHIP,  6.0f, true,  true},
         { BIGRES_CORIOLIS, 30.0f, false, false},
         { BIGRES_DODEK,    10.0f, false, false},
         { BIGRES_COBRA, 10.0f, true, true},
         //{ BIGRES_CARGO01,  10.0f, false, false},
      };
      int l_cntBigResTemplate = sizeof(l_tempBigResArray) / sizeof(l_tempBigResArray[0]);
   
      for (l_idxBigRes = 0; l_idxBigRes < BIGRES_LASTENUM; l_idxBigRes++)
      {
         if (g_bigresArray[l_idxBigRes].initBase() == false) return false;
      }
   
      for (k = 0; k < l_cntBigResTemplate; k++)
      {
         l_idxBigRes = l_tempBigResArray[k].idxBigRes;
         g_bigresArray[l_idxBigRes].m_fScale        = l_tempBigResArray[k].fScale;
         g_bigresArray[l_idxBigRes].m_bTextured     = l_tempBigResArray[k].bTextured;
         g_bigresArray[l_idxBigRes].m_bTranslatePivot = l_tempBigResArray[k].bTranslatePivot;
      }
   }

   m_cntBigResBytes     = BIGRES_LASTENUM * sizeof(BIGRES);
   m_cntBigResItemBytes = g_cntBigResItem * sizeof(BIGRESITEM);
   m_iBigRes      = int(g_bigresArray);
   m_iBigResItem  = int(g_bigresitemArray);
  
   return true;
}

// -- ermittelt die höchste Facezahl, die in einem der Objekte auftritt
//
int MESHRESOURCEMANAGER::getMaxFaces()
{
   int l_iMaxFacesCount;
   int l_resObj;

   l_iMaxFacesCount = 0;
   for (l_resObj=0; l_resObj<m_cntRes;l_resObj++)
   {
      if (m_resArray[l_resObj].cnt_faces > l_iMaxFacesCount)
      {
         l_iMaxFacesCount = m_resArray[l_resObj].cnt_faces;
      }
   }
   return l_iMaxFacesCount;
}

bool MESHRESOURCEMANAGER::recalcMinMax()
{
   int l_idxRes, l_idxVert;

   // --- complete the count fields in resourceoffset array and 
   //
   for (l_idxRes=0; l_idxRes<m_cntRes;l_idxRes++)
   {
      PSXVECTOR l_max = {0}, l_min = {0};
      PSXVERTEX* l_pvCurr;
      float l_fMaxLength, l_fLength;

      l_pvCurr = (PSXVERTEX*) (m_resArray[l_idxRes].vertex_offset + m_iVertices);

      // -- radius und min/max werte anhand
      //    des neuen Ursprungs
      //
      l_fMaxLength = 0;
      l_max.x = .0f;
      l_max.y = .0f;
      l_max.z = .0f;

      l_min.x = .0f;
      l_min.y = .0f;
      l_min.z = .0f;

      for (l_idxVert=0; l_idxVert < m_resArray[l_idxRes].cnt_vert; l_idxVert++)
      {
         l_fLength = pow2(l_pvCurr[l_idxVert].x)+
                     pow2(l_pvCurr[l_idxVert].y)+
                     pow2(l_pvCurr[l_idxVert].z);

         if (l_fLength > l_fMaxLength) l_fMaxLength = l_fLength;

         // -- Minimal/Maximalwerte ermitteln 
         //
         if ( l_pvCurr[l_idxVert].x > l_max.x )
         {
            l_max.x = l_pvCurr[l_idxVert].x;
         }
         if ( l_pvCurr[l_idxVert].y > l_max.y ) 
         {
            l_max.y = l_pvCurr[l_idxVert].y;
         }
         if ( l_pvCurr[l_idxVert].z > l_max.z ) 
         {
            l_max.z = l_pvCurr[l_idxVert].z;
         }

         if ( l_pvCurr[l_idxVert].x < l_min.x )
         {
            l_min.x = l_pvCurr[l_idxVert].x;
         }
         if ( l_pvCurr[l_idxVert].y < l_min.y ) 
         {
            l_min.y = l_pvCurr[l_idxVert].y;
         }
         if ( l_pvCurr[l_idxVert].z < l_min.z ) 
         {
            l_min.z = l_pvCurr[l_idxVert].z;
         }
      } //end-foreach vertex

      m_resArray[l_idxRes].rad = sqrt(l_fMaxLength);
      m_resArray[l_idxRes].min = l_min;
      m_resArray[l_idxRes].max = l_max;

      char l_cRadius[1000];
      PSXPrintf(__LINE__,l_cRadius, sizeof(l_cRadius), 
                "l_idxRes %i , Radius = %f, Side length: %f, %f, %f\n", 
                l_idxRes, 
                getSideLengthX((RESTYPE)l_idxRes),
                getSideLengthY((RESTYPE)l_idxRes),
                getSideLengthZ((RESTYPE)l_idxRes));
                
      WriteLogFile(l_cRadius);
   }

   return true;
}
// -- fuellen des BigRes Array
//
bool MESHRESOURCEMANAGER::generateBigRes()
{
   // --- scaling and translating bigresources
   //
   WriteLogFile("BigResource scaling and translating\n");
   {
      PSXVERTEX* l_pvCurr;
      PSXFACE*   l_faceArray;
      RESTYPE l_resObj;
      PSXVECTOR l_vMiddle;
      float l_fLength, l_fMaxLength;
      int l_iMaxFacesCount;
      bool l_bGutsOnly;
      IDXBIGRES l_idxBigResLast;
      int l_idxBigRes;

      // -- Gesamtradius der BigRes wird hier noch ermittelt
      //
      for (l_idxBigRes = 0; l_idxBigRes < BIGRES_LASTENUM; l_idxBigRes++)
      {
         g_bigresArray[l_idxBigRes].m_fRad = 0.0f;
         g_bigresArray[l_idxBigRes].m_vMax.x = 0.0f;
         g_bigresArray[l_idxBigRes].m_vMax.y = 0.0f;
         g_bigresArray[l_idxBigRes].m_vMax.z = 0.0f;
      }


      l_iMaxFacesCount = 0;

      l_idxBigResLast = (IDXBIGRES) -1;

      for (int l_idxItem = 0; l_idxItem < g_cntBigResItem; l_idxItem ++)
      {
         int i,k,m, n;
         PSXVECTOR l_max = {0}, l_min = {0};

         l_resObj = g_resassignArray[l_idxItem].resPart;

         l_pvCurr = (PSXVERTEX*) (m_resArray[l_resObj].vertex_offset + m_iVertices);
         l_faceArray = (PSXFACE*)(m_resArray[l_resObj].face_offset + m_iFaces);

         l_idxBigRes = g_resassignArray[l_idxItem].idxBigRes;

         g_bigresitemArray[l_idxItem].m_resPart        = g_resassignArray[l_idxItem].resPart;
         g_bigresitemArray[l_idxItem].m_fViewRotation2 = g_resassignArray[l_idxItem].fViewRotation2;
         g_bigresitemArray[l_idxItem].m_fScale         = g_resassignArray[l_idxItem].fScale;
		 /*
         g_bigresitemArray[l_idxItem].m_pEquipment     = NULL;
		 */

         // -- Bei Gruppenwechsel den jetzigen Part als Startindex eintragen
         //
         if (l_idxBigRes != l_idxBigResLast)
         {
            g_bigresArray[l_idxBigRes].m_idxBigResItem     = l_idxItem;
            g_bigresArray[l_idxBigRes].m_idxBigResItemGuts = l_idxItem;
            l_bGutsOnly = false;
         }

         g_bigresArray[l_idxBigRes].m_cntBigResItem = l_idxItem + 1;

         if (m_resArray[l_resObj].cnt_faces > l_iMaxFacesCount)
         {
            l_iMaxFacesCount = m_resArray[l_resObj].cnt_faces;
         }

         // -- first scaling
         //
         for (i=0; i < m_resArray[l_resObj].cnt_vert; i++)
         {
            l_pvCurr[i].x = l_pvCurr[i].x * g_bigresArray[l_idxBigRes].m_fScale;
            l_pvCurr[i].y = l_pvCurr[i].y * g_bigresArray[l_idxBigRes].m_fScale;
            l_pvCurr[i].z = l_pvCurr[i].z * g_bigresArray[l_idxBigRes].m_fScale;
         }

         // -- max rad of all FS-Parts , nicht zu verwechseln mit dem einzelteil radius
         //
         for (i=0; i < m_resArray[l_resObj].cnt_vert; i++)
         {
            l_fLength = pow2(l_pvCurr[i].x)+
                        pow2(l_pvCurr[i].y)+
                        pow2(l_pvCurr[i].z);

            if (l_fLength > g_bigresArray[l_idxBigRes].m_fRad) 
            {
               g_bigresArray[l_idxBigRes].m_fRad = l_fLength;
            }
         }

         // -- min/max werte des gesamten BigRes
         //
         for (n=0; n < m_resArray[l_resObj].cnt_vert; n++)
         {
            // -- Minimal/Maximalwerte ermitteln 
            //
            if ( l_pvCurr[n].x > g_bigresArray[l_idxBigRes].m_vMax.x )
            {
               g_bigresArray[l_idxBigRes].m_vMax.x = l_pvCurr[n].x;
            }
            if ( l_pvCurr[n].y > g_bigresArray[l_idxBigRes].m_vMax.y ) 
            {
               g_bigresArray[l_idxBigRes].m_vMax.y = l_pvCurr[n].y;
            }
            if ( l_pvCurr[n].z > g_bigresArray[l_idxBigRes].m_vMax.z ) 
            {
               g_bigresArray[l_idxBigRes].m_vMax.z = l_pvCurr[n].z;
            }
   
            if ( l_pvCurr[n].x < g_bigresArray[l_idxBigRes].m_vMin.x )
            {
               g_bigresArray[l_idxBigRes].m_vMin.x = l_pvCurr[n].x;
            }
            if ( l_pvCurr[n].y < g_bigresArray[l_idxBigRes].m_vMin.y ) 
            {
               g_bigresArray[l_idxBigRes].m_vMin.y = l_pvCurr[n].y;
            }
            if ( l_pvCurr[n].z < g_bigresArray[l_idxBigRes].m_vMin.z ) 
            {
               g_bigresArray[l_idxBigRes].m_vMin.z = l_pvCurr[n].z;
            }
         }

         // -- arithmetical Vertex Average is Middle
         //
         if (g_bigresArray[l_idxBigRes].m_bTranslatePivot)
         {
            memset(&l_vMiddle, 0, sizeof(l_vMiddle));
            for (k = 0; k < m_resArray[l_resObj].cnt_vert; k++)
            {
               l_vMiddle.x = l_vMiddle.x + l_pvCurr[k].x;
               l_vMiddle.y = l_vMiddle.y + l_pvCurr[k].y;
               l_vMiddle.z = l_vMiddle.z + l_pvCurr[k].z;
            }
            l_vMiddle.x = l_vMiddle.x / float(m_resArray[l_resObj].cnt_vert);
            l_vMiddle.y = l_vMiddle.y / float(m_resArray[l_resObj].cnt_vert);
            l_vMiddle.z = l_vMiddle.z / float(m_resArray[l_resObj].cnt_vert);

            for (m=0; m < m_resArray[l_resObj].cnt_vert; m++)
            {
               l_pvCurr[m].x = l_pvCurr[m].x - l_vMiddle.x;
               l_pvCurr[m].y = l_pvCurr[m].y - l_vMiddle.y;
               l_pvCurr[m].z = l_pvCurr[m].z - l_vMiddle.z;
            }
   
            g_bigresitemArray[l_idxItem].m_vPos.x = l_vMiddle.x;
            g_bigresitemArray[l_idxItem].m_vPos.y = l_vMiddle.y;
            g_bigresitemArray[l_idxItem].m_vPos.z = l_vMiddle.z;
         }else{
            g_bigresitemArray[l_idxItem].m_vPos = g_resassignArray[l_idxItem].vPos;
         }

         // -- radius und min/max werte anhand
         //    des neuen Ursprungs
         //
         l_fMaxLength = 0;
         for (n=0; n < m_resArray[l_resObj].cnt_vert; n++)
         {
            l_fLength = pow2(l_pvCurr[n].x)+
                        pow2(l_pvCurr[n].y)+
                        pow2(l_pvCurr[n].z);

            if (l_fLength > l_fMaxLength) l_fMaxLength = l_fLength;

            // -- Minimal/Maximalwerte ermitteln 
            //
            if ( l_pvCurr[n].x > l_max.x )
            {
               l_max.x = l_pvCurr[n].x;
            }
            if ( l_pvCurr[n].y > l_max.y ) 
            {
               l_max.y = l_pvCurr[n].y;
            }
            if ( l_pvCurr[n].z > l_max.z ) 
            {
               l_max.z = l_pvCurr[n].z;
            }
   
            if ( l_pvCurr[n].x < l_min.x )
            {
               l_min.x = l_pvCurr[n].x;
            }
            if ( l_pvCurr[n].y < l_min.y ) 
            {
               l_min.y = l_pvCurr[n].y;
            }
            if ( l_pvCurr[n].z < l_min.z ) 
            {
               l_min.z = l_pvCurr[n].z;
            }
         }
         m_resArray[l_resObj].rad = sqrt(l_fMaxLength);
         m_resArray[l_resObj].min = l_min;
         m_resArray[l_resObj].max = l_max;

         //m_resArray[l_resObj].rad = sqrt(pow(l_vBoundingBox.x/2,2)+
         //                                     pow(l_vBoundingBox.y/2,2)+
         //                                     pow(l_vBoundingBox.z/2,2));
 
         if (g_resassignArray[l_idxItem].bGutsOnly == true && l_bGutsOnly == false)
         {
            l_bGutsOnly = true;
            g_bigresArray[l_idxBigRes].m_idxBigResItemGuts = l_idxItem;
            g_bigresArray[l_idxBigRes].m_bHaveGuts           = true;
         }

         l_idxBigResLast = (IDXBIGRES) l_idxBigRes;

      } // end-foreach Part

      // -- Gesamtradius der Starbases wird hier noch ermittelt
      //
      for (l_idxBigRes = 0; l_idxBigRes < BIGRES_LASTENUM; l_idxBigRes++)
      {
         char l_cRadius[100];
         g_bigresArray[l_idxBigRes].m_fRad = sqrt(g_bigresArray[l_idxBigRes].m_fRad); 
         PSXPrintf(__LINE__,l_cRadius, sizeof(l_cRadius), "l_idxBigRes %i , Radius = %f\n",
                 l_idxBigRes, g_bigresArray[l_idxBigRes].m_fRad);
         WriteLogFile(l_cRadius);
          
      }


   }

   return true;
}

// -- Special 
//
/*
bool MESHRESOURCEMANAGER::scaleFlagshipDoors()
{
   // -- Schotts Resourcen wie Flagschiffparts re-translated aber 
   //    als Enemies positionieren
   //
   WriteLogFile("scaling Flagship-Tube-Doors\n");
   {
      PSXVERTEX*   l_vertArray;
      RESTYPE l_resObj;
      int l_idxDoor;
      PSXVECTOR l_vMiddle;
      float l_fLength, l_fMaxLength;
   
      g_pCurrLeader = NULL; // erste Door ist unabhaengig von jeder anderen zerstoerbar
   
      for (l_idxDoor = 0; l_idxDoor < g_cntDoor; l_idxDoor ++)
      {
         int k, m,n; // classical iterators
   
         l_resObj    = g_resDoorArray[l_idxDoor];
         l_vertArray = (PSXVERTEX*) (m_resArray[l_resObj].vertex_offset + m_iVertices);
         //l_faceArray = (PSXFACE*)   (m_resArray[l_resObj].face_offset + m_iFaces);
   
         // -- first scaling
         for (int i=0; i < m_resArray[l_resObj].cnt_vert; i++)
         {
            l_vertArray[i].x = l_vertArray[i].x * g_bigresArray[BIGRES_FLAGSHIP].m_fScale;
            l_vertArray[i].y = l_vertArray[i].y * g_bigresArray[BIGRES_FLAGSHIP].m_fScale;
            l_vertArray[i].z = l_vertArray[i].z * g_bigresArray[BIGRES_FLAGSHIP].m_fScale;
         }
   
         // -- arithmetical Vertex Average is Middle
         //
         memset(&l_vMiddle, 0, sizeof(l_vMiddle));
         for (k = 0; k < m_resArray[l_resObj].cnt_vert; k++)
         {
            l_vMiddle.x = l_vMiddle.x + l_vertArray[k].x;
            l_vMiddle.y = l_vMiddle.y + l_vertArray[k].y;
            l_vMiddle.z = l_vMiddle.z + l_vertArray[k].z;
         }
         l_vMiddle.x = l_vMiddle.x / float(m_resArray[l_resObj].cnt_vert);
         l_vMiddle.y = l_vMiddle.y / float(m_resArray[l_resObj].cnt_vert);
         l_vMiddle.z = l_vMiddle.z / float(m_resArray[l_resObj].cnt_vert);
   
         for (m=0; m < m_resArray[l_resObj].cnt_vert; m++)
         {
            l_vertArray[m].x = l_vertArray[m].x - l_vMiddle.x;
            l_vertArray[m].y = l_vertArray[m].y - l_vMiddle.y;
            l_vertArray[m].z = l_vertArray[m].z - l_vMiddle.z;
         }
   
         // -- radius = maximale Vektorlaenge eines Vertex anhand
         //             des neuen Ursprungs
         //
         l_fMaxLength = 0;
         for (n=0; n < m_resArray[l_resObj].cnt_vert; n++)
         {
            l_fLength = pow2(l_vertArray[n].x)+
                        pow2(l_vertArray[n].y)+
                        pow2(l_vertArray[n].z);
   
            if (l_fLength > l_fMaxLength) l_fMaxLength = l_fLength;
         }
         m_resArray[l_resObj].rad = sqrt(l_fMaxLength);
   
         // -- Die Position der Schotts relativ zur FS-Mitte zwischenspeichern
         //
         g_vDoorArray[l_idxDoor].x = l_vMiddle.x;
         g_vDoorArray[l_idxDoor].y = l_vMiddle.y;
         g_vDoorArray[l_idxDoor].z = l_vMiddle.z;
   
      } // end-foreach Door
   } 

   return true;
}
*/
// -- nachtragen der faceinfo zeiger in die BigResstruktur
//
bool MESHRESOURCEMANAGER::updateBigRes()
{
   // -- nach pflegen der BigRes faceinfArray Angaben
   //
   {
      RESTYPE l_resObj;
      int l_idxItem;

      for (l_idxItem = 0; l_idxItem < g_cntBigResItem; l_idxItem ++)
      {
   
         l_resObj = g_bigresitemArray[l_idxItem].m_resPart;
         g_bigresitemArray[l_idxItem].m_faceinfArray = m_pfaceinfArray[l_resObj];

      } 

/*
      m_faceinfAsteroidArray = m_pfaceinfArray[RESTYPE_ASTEROID];
      m_faceinfGeneratorArray = m_pfaceinfArray[RESTYPE_FELDGENERATOR]; 
*/

/*
      for (l_idxDoor = 0; l_idxDoor < g_cntDoor; l_idxDoor ++)
      {
         l_resObj    = g_resDoorArray[l_idxDoor];
         m_pfaceinfDoorArray[l_idxDoor] = m_pfaceinfArray[l_resObj];
      } 
*/
   }
   return true;
}

// --- just in time vertexbuffer
//
bool MESHRESOURCEMANAGER::createVertexBuffer(RESTYPE l_idxRes)
{
   ID3D11Buffer* lpDirect3DVertexBuffer;
   char lObjName[100];
   bool lCreateFlag;
   char l_cPrintOutLine[250];

   PSXVERTEX* l_vertPrintOutArray;
   

   PSXPrintf(__LINE__,lObjName, sizeof(lObjName),
              "creating vertbuffer for ResourceObject No. %i with %i vertices\n", 
              l_idxRes,
              m_resArray[l_idxRes].cnt_vert );
   WriteLogFile(lObjName);

   //   -- print out contents of vertexbuffer
   //
   l_vertPrintOutArray = reinterpret_cast<PSXVERTEX*>(m_resArray[l_idxRes].vertex_offset + m_iVertices);
   
   for (int l_idxVert = 0; l_idxVert < m_resArray[l_idxRes].cnt_vert; l_idxVert++ )
   {
      PSXPrintf(__LINE__,l_cPrintOutLine, sizeof(l_cPrintOutLine),
                 "Vertex: %f,%f,%f Normal : %f,%f,%f  TU,TV: %f,%f\n",
		 l_vertPrintOutArray[l_idxVert].x,
		 l_vertPrintOutArray[l_idxVert].y,
		 l_vertPrintOutArray[l_idxVert].z,
		 l_vertPrintOutArray[l_idxVert].nx,
		 l_vertPrintOutArray[l_idxVert].ny,
		 l_vertPrintOutArray[l_idxVert].nz,
		 l_vertPrintOutArray[l_idxVert].tu,
		 l_vertPrintOutArray[l_idxVert].tv);
      WriteLogFile(l_cPrintOutLine);
   }
   //   DWORD* temporary_index_buffer;  
   
   lCreateFlag = psx_create_vertex_buffer(                   
                    lObjName,
                    &lpDirect3DVertexBuffer,
		    		reinterpret_cast<PSXVERTEX*>(m_resArray[l_idxRes].vertex_offset + m_iVertices),
                    m_resArray[l_idxRes].cnt_vert);
   
   if (lCreateFlag == false) return (FALSE);
   
   m_vertbufArray[l_idxRes] = lpDirect3DVertexBuffer;
   
   return true;
}   

// -- just in time Indexbuffer erzeugen
//
bool MESHRESOURCEMANAGER::createIndexBuffer(RESTYPE l_idxRes)
{
   HRESULT hr;
   void* l_pFaceOffsets;

   ID3D11Buffer* lpDirect3DIndexBuffer = NULL;

   l_pFaceOffsets = (void*) (m_resArray[l_idxRes].face_offset + m_iFaces);
   /*
   LPDIRECT3DINDEXBUFFER9  lpDirect3DIndexBuffer;
   short* temporary_index_buffer;  
   */
   D3D11_BUFFER_DESC l_d3dBufDesc;
   ZeroMemory( &l_d3dBufDesc, sizeof(l_d3dBufDesc) );
   l_d3dBufDesc.Usage = D3D11_USAGE_IMMUTABLE;
   l_d3dBufDesc.ByteWidth = sizeof(PSXFACE) * m_resArray[l_idxRes].cnt_faces;
   l_d3dBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
   l_d3dBufDesc.CPUAccessFlags = 0;
   l_d3dBufDesc.MiscFlags = 0;
   l_d3dBufDesc.StructureByteStride = 0;
   
   D3D11_SUBRESOURCE_DATA l_subresData;
   ZeroMemory( &l_subresData, sizeof(l_subresData) );
   l_subresData.pSysMem = l_pFaceOffsets;

   hr = g_pd3dDevice->CreateBuffer( &l_d3dBufDesc, 
		                    &l_subresData, 
				    &lpDirect3DIndexBuffer );

   /*
   
   hr = g_pDev->CreateIndexBuffer(m_resArray[l_idxRes].cnt_faces*sizeof(short)*3,
                                  D3DUSAGE_DYNAMIC| D3DUSAGE_WRITEONLY,
                                  D3DFMT_INDEX16, D3DPOOL_DEFAULT, 
                                  &lpDirect3DIndexBuffer, NULL);
   if (hr != 0) { 
				  */
   if( FAILED( hr ) ) 
   {
      hr2message(__LINE__,hr, "g_pd3dDevice->CreateBuffer(Index)");
      return(FALSE);
   }

   /*
   hr = lpDirect3DIndexBuffer->Lock(0,0,
                                    (void**)&temporary_index_buffer,
                                    D3DLOCK_DISCARD|
                                    D3DLOCK_NOSYSLOCK);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"lpDirect3DIndexBuffer->Lock");
      return(FALSE);
   }

   l_pFaceOffsets = (int*) (m_resArray[l_idxRes].face_offset + m_iFaces);

   // -- int nach short konvertierung 
   //
   for (int i=0;i<m_resArray[l_idxRes].cnt_faces*3;i++)
   {
      temporary_index_buffer[i] = (short)l_pFaceOffsets[i];
   }

   hr = lpDirect3DIndexBuffer->Unlock();
   if (hr != 0) { 
      hr2message(__LINE__,hr,"lpDirect3DIndexBuffer->Unlock");
      return(FALSE);
   }
   
   */
   m_indexbufArray[l_idxRes] = lpDirect3DIndexBuffer;

   return true;
}   

// -- drop all Index and Vertexbuffers
//
bool MESHRESOURCEMANAGER::releaseVertexBuffers()
{
   HRESULT hr;
   WriteLogFile("MESHRESOURCEMANAGER::releaseVertexBuffers\n");
   if (m_vertbufArray)
   {
      for (int l_idxResourceOffsetReleasing=0;
               l_idxResourceOffsetReleasing<m_cntRes;
               l_idxResourceOffsetReleasing++)
      {
         if (m_vertbufArray[l_idxResourceOffsetReleasing])
         {
            hr = m_vertbufArray[l_idxResourceOffsetReleasing]->Release();
            if (hr != 0) { 
               WriteLogFile("Error g_pD3DVertexBuffer->Release()\n");
            }
            m_vertbufArray[l_idxResourceOffsetReleasing] = NULL;
         }
         if (m_indexbufArray[l_idxResourceOffsetReleasing])
         {
            m_indexbufArray[l_idxResourceOffsetReleasing]->Release();
            if (hr != 0) { 
               WriteLogFile("Error in g_pD3DIndexBuffer->Release()\n");
            }
            m_indexbufArray[l_idxResourceOffsetReleasing] = NULL;
         }
      }
   }
   return true;
}
   
// -- Aufruf bei Programmbeendigung
//
bool MESHRESOURCEMANAGER::release()
{
   int i;
   struct _releasemaphandles {
     HANDLE* m_pHandle;
     HANDLE* m_pMapHandle;
     void**  m_ppDestination;
   } l_tempRelease[] = 
   {
      { &m_hVerticesFile, &m_hMapVertices, (void**) &m_iVertices},
      { &m_hFacesFile,    &m_hMapFaces,    (void**) &m_iFaces},
      { &m_hFaceInfFile,  &m_hMapFaceInf,  (void**) &m_iFaceInf},
   };
   int l_cntRelease = sizeof(l_tempRelease)/sizeof(l_tempRelease[0]);

   WriteLogFile("MESHRESOURCEMANAGER::release()\n");
   if (m_trimeshdataidArray) 
   {
      WriteLogFile("release ode-trimeshdata\n");
      for (int l_idxRes=0; l_idxRes < m_cntRes; l_idxRes++)
      {
         if (m_trimeshdataidArray[l_idxRes]) dGeomTriMeshDataDestroy(m_trimeshdataidArray[l_idxRes]);
      }
      delete [] m_trimeshdataidArray;
   }

   WriteLogFile("Release VB-Array itself\n");
   if (m_vertbufArray) delete [] m_vertbufArray;
   if (m_indexbufArray) delete [] m_indexbufArray;

   WriteLogFile("unmap, close Mappings and its Files\n");
   for (i = 0; i< l_cntRelease; i++)
   {
      UnmapViewOfFile((void*)*l_tempRelease[i].m_ppDestination);
      CloseHandle(*l_tempRelease[i].m_pMapHandle);
      CloseHandle(*l_tempRelease[i].m_pHandle);
   }

   WriteLogFile("Unallocate Resourceblock\n");
   if (m_pMeshResourceBlock) VirtualFree(m_pMeshResourceBlock,0, MEM_RELEASE);
   return true;
}

bool MESHRESOURCEMANAGER::createTriMeshData(RESTYPE l_resType)
{
   if (m_trimeshdataidArray[l_resType] == NULL)
   {
      m_trimeshdataidArray[l_resType] = dGeomTriMeshDataCreate();

      dGeomTriMeshDataBuildSingle (m_trimeshdataidArray[l_resType],                            // dTriMeshDataID g 
                                   (void*)(m_resArray[l_resType].vertex_offset + m_iVertices), // const void* Vertices
                                   sizeof(PSXVERTEX),                                          // int VertexStride 
                                   m_resArray[l_resType].cnt_vert,                             // int VertexCount
                                   (void*) (m_resArray[l_resType].face_offset + m_iFaces),     // const void* Indices
                                   m_resArray[l_resType].cnt_faces * 3,                            // int IndexCount
                                   sizeof(PSXFACE));                                           // int TriStride 
                                   //const void* Normals

   }


   // -- immer noch NULL
   //
   if (m_trimeshdataidArray[l_resType] == NULL)
   {
      hr2message(__LINE__,-1, "dGeomTriMeshDataBuildSingle() failed to return a valid TriMeshDataID");
      return false;
   }
   return true;
}

dTriMeshDataID MESHRESOURCEMANAGER::getTriMeshDataID(RESTYPE l_resType)
{
   return m_trimeshdataidArray[l_resType];
}

// -- fuellen der kollisionsrelevanten Faceinfos
//
bool MESHRESOURCEMANAGER::initAllFaceInfoArrays()
{
   int l_resType;
   PSXFACEINFO* l_faceinfCurrent;

   WriteLogFile("init faceinfo arrays\n");

   l_faceinfCurrent = m_faceinfArray;

   // -- Faceinfo array für jedes Resourceobjekt erzeugen
   //
   for (l_resType=0; l_resType < m_cntRes; l_resType++)
   {
      m_pfaceinfArray[l_resType] = l_faceinfCurrent;
      if (initFaceInfoArray(&l_faceinfCurrent, (RESTYPE)l_resType) == false)
      {
         return false;
      }
   }
   
   return true;
}

// -- m_pfaceinf Array generieren anhand cnt_faces
//
bool MESHRESOURCEMANAGER::locatePFaceInfArray()
{
   int l_resType;
   PSXFACEINFO* l_faceinfCurrent;

   WriteLogFile("locate pfaceinfo array\n");

   l_faceinfCurrent = m_faceinfArray;

   // -- Faceinfo array für jedes Resourceobjekt erzeugen
   //
   for (l_resType=0; l_resType < m_cntRes; l_resType++)
   {
      m_pfaceinfArray[l_resType] = l_faceinfCurrent;
      l_faceinfCurrent = l_faceinfCurrent + m_resArray[l_resType].cnt_faces; 
   }

   
   return true;
}


// -- initialisiert ein Faceinfoarray
//    in/out Zeiger auf die mitlaufende l_faceinfCurrent Variable
//    in     Nummer des Resourceobjekts
//
bool MESHRESOURCEMANAGER::initFaceInfoArray(PSXFACEINFO** l_pfaceinfArray,
                                            RESTYPE       l_resObj)
{
   int l_idxVert[3];
   int l_idxFace, l_cntFace;
   float l_fLen[3];
   PSXFACE*     l_faceArray;
   PSXVERTEX*   l_vertArray;
   PSXFACEINFO* l_faceinfCurrent;

   PSXVECTOR l_vDelta[3], l_vCentroid, l_vNormal;

   l_cntFace   = m_resArray[l_resObj].cnt_faces; 
   l_faceArray = (PSXFACE*)   (m_resArray[l_resObj].face_offset + m_iFaces);
   l_vertArray = (PSXVERTEX*) (m_resArray[l_resObj].vertex_offset + m_iVertices);

   l_faceinfCurrent = *l_pfaceinfArray;

   for (l_idxFace=0; l_idxFace< l_cntFace; l_idxFace++)
   {
      float l_fFaceposLength, l_fNormalLength;

      l_idxVert[0] = l_faceArray[l_idxFace].a;
      l_idxVert[1] = l_faceArray[l_idxFace].b;
      l_idxVert[2] = l_faceArray[l_idxFace].c;

      // -- 2 Deltavektoren fuer Normalen berechnung
      //
      l_vDelta[0].x = l_vertArray[l_idxVert[1]].x - 
                      l_vertArray[l_idxVert[0]].x;
      l_vDelta[0].y = l_vertArray[l_idxVert[1]].y - 
                      l_vertArray[l_idxVert[0]].y;
      l_vDelta[0].z = l_vertArray[l_idxVert[1]].z - 
                      l_vertArray[l_idxVert[0]].z;

      l_vDelta[1].x = l_vertArray[l_idxVert[2]].x - 
                      l_vertArray[l_idxVert[1]].x;
      l_vDelta[1].y = l_vertArray[l_idxVert[2]].y - 
                      l_vertArray[l_idxVert[1]].y;
      l_vDelta[1].z = l_vertArray[l_idxVert[2]].z - 
                      l_vertArray[l_idxVert[1]].z;


      // --- crossproduct
      //
      l_vNormal.x = (l_vDelta[0].y * l_vDelta[1].z - 
                     l_vDelta[0].z * l_vDelta[1].y); 
      l_vNormal.y = (l_vDelta[0].z * l_vDelta[1].x - 
                     l_vDelta[0].x * l_vDelta[1].z); 
      l_vNormal.z = (l_vDelta[0].x * l_vDelta[1].y - 
                     l_vDelta[0].y * l_vDelta[1].x);

      l_fNormalLength  = sqrt(pow2(l_vNormal.x)+
                              pow2(l_vNormal.y)+
                              pow2(l_vNormal.z));

      if (l_fNormalLength > 0.0f)
      {
         l_vNormal.x = l_vNormal.x / l_fNormalLength;
         l_vNormal.y = l_vNormal.y / l_fNormalLength;
         l_vNormal.z = l_vNormal.z / l_fNormalLength;
         l_faceinfCurrent->vNormal = l_vNormal;
      }else{
         l_faceinfCurrent->vNormal.x = 0.0f;
         l_faceinfCurrent->vNormal.y = 0.0f;
         l_faceinfCurrent->vNormal.z = 0.0f;
      }


      l_vCentroid.x = (l_vertArray[l_idxVert[0]].x + 
                       l_vertArray[l_idxVert[1]].x + 
                       l_vertArray[l_idxVert[2]].x) / 3.0f;

      l_vCentroid.y = (l_vertArray[l_idxVert[0]].y + 
                       l_vertArray[l_idxVert[1]].y + 
                       l_vertArray[l_idxVert[2]].y) / 3.0f;

      l_vCentroid.z = (l_vertArray[l_idxVert[0]].z + 
                       l_vertArray[l_idxVert[1]].z + 
                       l_vertArray[l_idxVert[2]].z) / 3.0f;

      l_faceinfCurrent->vCentroid = l_vCentroid;

      // -- Deltavektor vom Centroid bis zur Triangle-Ecke
      //
      for (int a=0; a<3; a++)
      {
         l_vDelta[a].x = l_vertArray[l_idxVert[a]].x-l_vCentroid.x;
         l_vDelta[a].y = l_vertArray[l_idxVert[a]].y-l_vCentroid.y;
         l_vDelta[a].z = l_vertArray[l_idxVert[a]].z-l_vCentroid.z;
      }

      // -- laenge hoch 2 der Deltavektoren
      //
      for (int q=0;q<3;q++)
      {
         l_fLen[q] = pow2( l_vDelta[q].x) + 
                     pow2( l_vDelta[q].y) + 
                     pow2( l_vDelta[q].z);
      }

      // -- Maxwert der Vektorlaengen ermitteln
      //
      l_faceinfCurrent->fRadPow2 = 0;
      for (int n=0;n<3;n++)
      {
         if (l_fLen[n] > l_faceinfCurrent->fRadPow2)
         {
            l_faceinfCurrent->fRadPow2 = l_fLen[n];
         }
      }

      // -- Dotproduct zwischen Normale und Face-Centroid
      //    CAUTION: Ursprung des FaceCentroid ist natuerlich
      //             der Mittelpunkt des einzelnen Flagship-Parts
      //
      l_fFaceposLength = sqrt(pow2(l_vCentroid.x)+
                              pow2(l_vCentroid.y)+
                              pow2(l_vCentroid.z));

      if (l_fFaceposLength)
      {
         l_faceinfCurrent->fDot = (l_vCentroid.x * l_vNormal.x + 
                                   l_vCentroid.y * l_vNormal.y + 
                                   l_vCentroid.z * l_vNormal.z) 
                                 / (l_fFaceposLength);
      }else{
         l_faceinfCurrent->fDot = 0.0f;
      }
     
      if ((l_faceinfCurrent->fDot > 1.0f) &&
          (l_faceinfCurrent->fDot < -1.0f))
      {
         WriteLogFile("calculating normals in faceinfo: dot > 1.0f!\n");
         return false;
      }

      l_faceinfCurrent++;
   }

   *l_pfaceinfArray = l_faceinfCurrent;
   return true;
}  

// -- diese Methoden lassen Zugriff auf geschütze Elemente zu
//
PSXFACEINFO* MESHRESOURCEMANAGER::getFaceInfArray(RESTYPE l_resType)
{
   return m_pfaceinfArray[int(l_resType)];
}

int MESHRESOURCEMANAGER::getVertexCount(RESTYPE l_resType)
{
   return m_resArray[int(l_resType)].cnt_vert;
}

int MESHRESOURCEMANAGER::getFaceCount(RESTYPE l_resType)
{
   return m_resArray[int(l_resType)].cnt_faces;
}

int MESHRESOURCEMANAGER::getMaterialCount(RESTYPE l_resType)
{
   return m_resArray[int(l_resType)].cnt_material;
}

PSXMATERIAL* MESHRESOURCEMANAGER::getMaterialArray(RESTYPE l_resType)
{
   return (PSXMATERIAL*) (m_resArray[int(l_resType)].material_offset + m_iMaterials);
}

float MESHRESOURCEMANAGER::getRad(RESTYPE l_resType)
{
   return m_resArray[int(l_resType)].rad;
}

PSXFACE* MESHRESOURCEMANAGER::getFaceArray(RESTYPE l_resType)
{
   return (PSXFACE*) (m_resArray[int(l_resType)].face_offset + m_iFaces);
}

PSXVERTEX* MESHRESOURCEMANAGER::getVertexArray(RESTYPE l_resType)
{
   return (PSXVERTEX*) (m_resArray[int(l_resType)].vertex_offset + m_iVertices);
}

float MESHRESOURCEMANAGER::getSideLengthX(RESTYPE l_resType)
{
   return (m_resArray[int(l_resType)].max.x - m_resArray[int(l_resType)].min.x);
}
float MESHRESOURCEMANAGER::getSideLengthY(RESTYPE l_resType)
{
   return (m_resArray[int(l_resType)].max.y - m_resArray[int(l_resType)].min.y);
}
float MESHRESOURCEMANAGER::getSideLengthZ(RESTYPE l_resType)
{
   return (m_resArray[int(l_resType)].max.z - m_resArray[int(l_resType)].min.z);
}

float MESHRESOURCEMANAGER::getMiddleX(RESTYPE l_resType)
{
   return (m_resArray[int(l_resType)].max.x + m_resArray[int(l_resType)].min.x) / 2.0f;
}
float MESHRESOURCEMANAGER::getMiddleY(RESTYPE l_resType)
{
   return (m_resArray[int(l_resType)].max.y + m_resArray[int(l_resType)].min.y) / 2.0f;
}
float MESHRESOURCEMANAGER::getMiddleZ(RESTYPE l_resType)
{
   return (m_resArray[int(l_resType)].max.z + m_resArray[int(l_resType)].min.z) / 2.0f;
}

// -- *.psx Mesh dateien wieder abspeichern
//
bool MESHRESOURCEMANAGER::save()
{
  
   struct _filewritelist {
     wchar_t*   m_wcFileName;
     DWORD*  m_pdwLength;
     void**  m_ppSource;
   } l_tempFileWriteList[] = 
   {
      {  L"materials.psx", &m_cntMaterialBytes,   (void**) &m_iMaterials },
      {  L"vertices.psx",  &m_cntVertexBytes,     (void**) &m_iVertices},
      {  L"faces.psx",     &m_cntFaceBytes,       (void**) &m_iFaces},
      {  L"offsets.psx",   &m_cntResourceBytes,   (void**) &m_resArray},
      {  L"bigres.psx",    &m_cntBigResBytes,     (void**) &m_iBigRes},
      {  L"bresitem.psx",  &m_cntBigResItemBytes, (void**) &m_iBigResItem},
      {  L"faceinf.psx",   &m_cntFaceInfBytes,    (void**) &m_iFaceInf},
   };
   int l_cntFileList = sizeof(l_tempFileWriteList)/sizeof(l_tempFileWriteList[0]);

   int i;

   WriteLogFile("MESHRESOURCEMANAGER::save()\n");

   if (m_iBigRes == 0)
   {
      WriteLogFile("resmanager::save(): bigresArray Pointer is NULL\n");
      return false;
   }

   if (m_iBigResItem == 0)
   {
      WriteLogFile("resmanager::save(): bigresitemArray Pointer is NULL\n");
      return false;
   }

   WriteLogFile("write the files.\n");

   for (i=0; i < l_cntFileList; i++)
   {
      WriteLogFile("create file\n");
      DWORD l_dwBytesWritten;
      HANDLE l_hToOpen;
      l_hToOpen = CreateFile(
         l_tempFileWriteList[i].m_wcFileName,
         GENERIC_WRITE |GENERIC_READ,        // access (read-write) mode 
         0 ,	                // share mode 
         NULL ,                 // pointer to security attributes 
         CREATE_ALWAYS,         // how to create 
         FILE_ATTRIBUTE_NORMAL, // file attributes 
         NULL );	


      if (l_hToOpen==INVALID_HANDLE_VALUE) 
      {
         wchar_t l_wcFileNotFound[100];
         PSXPrintf2(__LINE__,l_wcFileNotFound, sizeof(l_wcFileNotFound),
                 L"Mesh-Resourcefile %s not creatable! Aborting...\n",
                  l_tempFileWriteList[i].m_wcFileName);
         MessageBox(g_hWnd,l_wcFileNotFound,
                    L"Loading Mesh-resources",
                    MB_ICONSTOP);
         WriteLogFile2(l_wcFileNotFound);
         return(false);
      }

      WriteFile( l_hToOpen,
                 *l_tempFileWriteList[i].m_ppSource, 
                 *l_tempFileWriteList[i].m_pdwLength,   
                 &l_dwBytesWritten,              
                 NULL );

      if (l_dwBytesWritten != (*l_tempFileWriteList[i].m_pdwLength))
      {
         wchar_t l_wcNotComplete[200];
         PSXPrintf2(__LINE__, l_wcNotComplete, sizeof(l_wcNotComplete),
                 L"%s was not saved completely, aborting."
                 L" Should have written %i, done only %i \n"
                 L"Last Win Error: %x", 
                 l_tempFileWriteList[i].m_wcFileName,
                 *l_tempFileWriteList[i].m_pdwLength,
                 l_dwBytesWritten,
                 GetLastError());

         WriteLogFile2( l_wcNotComplete);
         return false;
      }
   }


   return true;
}


// -- setzt indexbuffer
//
bool MESHRESOURCEMANAGER::setIndices(RESTYPE l_resType)
{
   
   // -- muss nachgeladen werden
   //
   if (m_indexbufArray[l_resType] == NULL)
   {
      if (createIndexBuffer(l_resType) == false) return false;
   }

   // Set index buffer
   g_pImmediateContext->IASetIndexBuffer( m_indexbufArray[l_resType], 
		                          DXGI_FORMAT_R32_UINT, 0 );

   // Set primitive topology
   g_pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

   return true;
}

// -- setzt vertexbuffer
//
bool MESHRESOURCEMANAGER::setStreamSource(int l_idxStream, RESTYPE l_resType)
{

   
   if (m_vertbufArray[l_resType] == NULL)
   {
      if (createVertexBuffer(l_resType) == false) return false;
   }

      
   UINT stride = sizeof( PSXVERTEX);
   UINT offset = 0;
   g_pImmediateContext->IASetVertexBuffers( l_idxStream, 1, m_vertbufArray+l_resType, &stride, &offset );

   return true;
}

