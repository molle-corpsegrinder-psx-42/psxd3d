/*
 *------------------------------------------------------------------------
 *      Object: psxedraw.cpp 
 *   $Revision: 1.13 $
 *        Date: 2005                            
 *       Notes: ENEMYDRAWER Basis-Klasse
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


#include "stdafx.h"
#include "psxd3d.h"
#include "psxglob.h"

// -- kein Pixelshader aktivieren, solange noch die texturestages verwendet werden
//
bool ENEMYDRAWER::setPixelShader()
{

   if (g_bWriteLogFileInLoop)
   {
      WriteLogFile("ENEMYDRAWER::setPixelShader()\n");
   }
/*
   if (m_bEmissive)
   {
      hr = g_pDev->SetPixelShader( g_hDot3BumpPixelShader );
      if (hr != 0) { 
         hr2message(__LINE__,hr,"g_pDev->SetPixelShader(DOT3Bump Pixelshader)");
         return false;
      }
   }else{
      hr = g_pDev->SetPixelShader( g_hBumpNoEmPixelShader );
      if (hr != 0) { 
         hr2message(__LINE__,hr,"g_pDev->SetPixelShader(DOT3Bump Pixelshader)");
         return false;
      }
   }
*/
   
  
   g_pImmediateContext->VSSetShader( g_pVertexShaderEnemyDrawer, nullptr, 0 );
   g_pImmediateContext->VSSetConstantBuffers( 0, 1, &m_pcbChangeEveryObject );
   g_pImmediateContext->VSSetConstantBuffers( 1, 1, &m_pcbChangeOnResize );
   g_pImmediateContext->VSSetConstantBuffers( 2, 1, &m_pcbChangeEveryFrame );
   g_pImmediateContext->PSSetShader( g_pPixelShaderEnemyDrawer, nullptr, 0 );
   g_pImmediateContext->PSSetConstantBuffers( 0, 1, &m_pcbChangeEveryObject );
   g_pImmediateContext->PSSetConstantBuffers( 1, 1, &m_pcbChangeOnResize );
   g_pImmediateContext->PSSetConstantBuffers( 2, 1, &m_pcbChangeEveryFrame );
   //g_pImmediateContext->PSSetShaderResources( 0, 1, &g_pTextureRV );
   g_pImmediateContext->PSSetSamplers( 0, 1, &ENEMYDRAWER::m_pSamplerLinear );
   g_pImmediateContext->GSSetShader(nullptr, nullptr, 0);

   return true;
}

// -- bedient die Shaderkonstanten passend zum eigenen bumpshade Vertex/Pixelshader
//
bool ENEMYDRAWER::prepareHeader()
{
   

   if (g_bWriteLogFileInLoop)
   {
      WriteLogFile("ENEMYDRAWER::prepareHeader()\n");
   }

/*
   g_vwVSConstantArray[10].setFloat( g_d3dExteriorLights[0].Diffuse.r,
                                   g_d3dExteriorLights[0].Diffuse.g,
                                   g_d3dExteriorLights[0].Diffuse.b,
                                   1.0f); // diffusecolor
   g_vwVSConstantArray[12].setFloat( 0.5f, 0.5f, 0.5f, 1.0f); // bias

   hr = g_pDev->SetVertexShaderConstantF(0,(const float*) g_vwVSConstantArray,13);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetVertexShaderConstantF(dot3bumpshader 1)");
      return false; 
   }
   
   // -- pixelshader konstanten
   //
   g_vwPSConstArray[0].setFloat( 4.0f, 4.0f, 4.0f, 1.0f); // bias
   g_vwPSConstArray[1].setFloat( g_Player.m_pLevel->m_d3dcolPlanet.r * 0.5f,
                                 g_Player.m_pLevel->m_d3dcolPlanet.g * 0.5f,
                                 g_Player.m_pLevel->m_d3dcolPlanet.b * 0.5f,
                                 1.0f); // diffusecolor
   g_vwPSConstArray[2].setFloat( 0.5f,
                                 0.5f,
                                 0.5f,
                                 1.0f); // ambient
   hr = g_pDev->SetPixelShaderConstantF(0,(const float*) g_vwPSConstArray,3);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetPixelShaderConstantF(dot3bumpshader 1)");
      return false; 
   }
*/
   // Update our time
    static float t = 0.0f;
    if( g_driverType == D3D_DRIVER_TYPE_REFERENCE )
    {
        t += ( float )XM_PI * 0.0125f;
    }
    else
    {
        static ULONGLONG timeStart = 0;
        ULONGLONG timeCur = GetTickCount64();
        if( timeStart == 0 )
            timeStart = timeCur;
        t = ( timeCur - timeStart ) / 1000.0f;
    }


   // Setup our lighting parameters
    XMFLOAT3 vLightDirs[2] =
    {
        XMFLOAT3( -0.577f, 0.577f, -0.577f),
        XMFLOAT3( 0.0f, 0.0f, -1.0f),
    };
    
    // Rotate the second light around the origin
    XMMATRIX mRotate = XMMatrixRotationY( -2.0f * t );
    XMVECTOR l_vLightDir = XMLoadFloat3( &vLightDirs[1] );
    l_vLightDir = XMVector3Transform( l_vLightDir, mRotate );
    XMStoreFloat3( &vLightDirs[1], l_vLightDir );
    
   XMFLOAT4 l_float4Camera;
   XMStoreFloat4(&l_float4Camera, g_vCamera);

   //if (setPixelShader()== false) return false;

   // -- Transfer to Contant Buffer
   //
   ENEMYDRAWERCONSTANTBUFFER::cbChangeEveryFrame l_cbChangeEveryFrame;
   l_cbChangeEveryFrame.mView = XMMatrixTranspose( g_View );    
   l_cbChangeEveryFrame.vCamera = l_float4Camera;

   // Directional light.
   l_cbChangeEveryFrame.DirLight[0].Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);        
   l_cbChangeEveryFrame.DirLight[0].Diffuse  = XMFLOAT4( 0.1f, 0.1f, 0.1f, 1.0f );
   l_cbChangeEveryFrame.DirLight[0].Specular = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
   l_cbChangeEveryFrame.DirLight[0].Direction = vLightDirs[0];

   l_cbChangeEveryFrame.DirLight[1].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);        
   l_cbChangeEveryFrame.DirLight[1].Diffuse  = XMFLOAT4( 0.5f, 0.5f, 0.5f, 1.0f );
   l_cbChangeEveryFrame.DirLight[1].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
   l_cbChangeEveryFrame.DirLight[1].Direction = vLightDirs[1];    

   // Point light--position is changed every frame to animate in UpdateScene function.
   l_cbChangeEveryFrame._PointLight.Ambient  = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
   l_cbChangeEveryFrame._PointLight.Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
   l_cbChangeEveryFrame._PointLight.Specular = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
   l_cbChangeEveryFrame._PointLight.Att      = XMFLOAT3(1.0f, 1.1f, 1.0f);
   l_cbChangeEveryFrame._PointLight.Range    = 1000.0f;
   l_cbChangeEveryFrame._PointLight.Position = XMFLOAT3(0,0,-1.0f);

   // Spot light--position and direction changed every frame to animate in UpdateScene function.
   l_cbChangeEveryFrame._SpotLight.Ambient  = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
   l_cbChangeEveryFrame._SpotLight.Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
   l_cbChangeEveryFrame._SpotLight.Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
   l_cbChangeEveryFrame._SpotLight.Att      = XMFLOAT3(1.0f, 0.0f, 0.0f);
   l_cbChangeEveryFrame._SpotLight.Spot     = 96.0f;
   l_cbChangeEveryFrame._SpotLight.Range    = 10000.0f; 

   XMStoreFloat3(&l_cbChangeEveryFrame._SpotLight.Position,g_vCamera);   
   XMMATRIX l_matCameraTranspose = XMMatrixTranspose(g_matCamera);
   XMStoreFloat3(&l_cbChangeEveryFrame._SpotLight.Direction, l_matCameraTranspose.r[2]);

   g_pImmediateContext->UpdateSubresource( m_pcbChangeEveryFrame, 0, nullptr, &l_cbChangeEveryFrame, 0, 0 );
   
   ENEMYDRAWERCONSTANTBUFFER::cbChangeOnResize l_cbChangesOnResize;
   l_cbChangesOnResize.mProjection = XMMatrixTranspose( g_Projection );
   g_pImmediateContext->UpdateSubresource( m_pcbChangeOnResize, 0, nullptr, &l_cbChangesOnResize, 0, 0 );

   return true;
}

bool ENEMYDRAWER::prepareItem(FXMVECTOR l_vPos, XMMATRIX* l_pMat, float l_fScale)
{
    
    XMMATRIX l_matWorld;

   if (g_bWriteLogFileInLoop)
   {
       WriteLogFile("ENEMYDRAWER::prepareItem()\n");
   }

   ENEMYDRAWERCONSTANTBUFFER::cbChangeEveryObject cb1;    
   l_matWorld = (*l_pMat) * XMMatrixTranslationFromVector(l_vPos);
   cb1.mWorld = XMMatrixTranspose(l_matWorld);
   cb1.matWorldViewProj= XMMatrixTranspose( l_matWorld * g_View * g_Projection );
   cb1.d3dMaterial.Ambient = XMFLOAT4(0.1, 0.1, 0.1, 0);
   cb1.d3dMaterial.Diffuse = XMFLOAT4(1, 1, 1, 1);
   cb1.d3dMaterial.Reflect = XMFLOAT4(0, 0, 0, 0);
   cb1.d3dMaterial.Specular = XMFLOAT4(1, 1, 1, 1);
   g_pImmediateContext->UpdateSubresource(m_pcbChangeEveryObject, 0, nullptr, &cb1, 0, 0 );     

    
/*
   memcpy(&g_matGeneric1, l_pMat, sizeof(g_matGeneric1));
   if (l_fScale != 1.0f)
   {
      g_matGeneric1._11 = g_matGeneric1._11 * l_fScale;
      g_matGeneric1._12 = g_matGeneric1._12 * l_fScale;
      g_matGeneric1._13 = g_matGeneric1._13 * l_fScale;
      g_matGeneric1._21 = g_matGeneric1._21 * l_fScale;
      g_matGeneric1._22 = g_matGeneric1._22 * l_fScale;
      g_matGeneric1._23 = g_matGeneric1._23 * l_fScale;
      g_matGeneric1._31 = g_matGeneric1._31 * l_fScale;
      g_matGeneric1._32 = g_matGeneric1._32 * l_fScale;
      g_matGeneric1._33 = g_matGeneric1._33 * l_fScale;
   }
   g_matGeneric1._41 = l_vPos.x;
   g_matGeneric1._42 = l_vPos.y;
   g_matGeneric1._43 = l_vPos.z;
   hr = g_pDev->SetTransform(D3DTS_WORLD, &g_matGeneric1);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetTransform(enemy)");
      return false;
   }
*/
   return true;

}

/*
bool SHADEDRAWER::prepareItem(PSXVECTOR l_vPos, D3DMATRIX* l_pMat, float l_fScale)
{
   PSXVECTOR l_vLight1, l_vLight2, l_vCamera;

   HRESULT hr;

   if (g_bWriteLogFileInLoop)
   {
      WriteLogFile("SHADEDRAWER::prepareItem()\n");
   }

   // -- Lichtquellen und Betrachter aus Objektsicht 
   //
   {
      float x,y,z;
             
         
      if (g_Player.m_bEntered)
      {
         x = g_Player.m_pEnemy->getPosX() - l_vPos.x;
         y = g_Player.m_pEnemy->getPosY() - l_vPos.y;
         z = g_Player.m_pEnemy->getPosZ() - l_vPos.z;
      }else{
         x = g_d3dExteriorLights[0].Direction.x;
         y = g_d3dExteriorLights[0].Direction.y;
         z = g_d3dExteriorLights[0].Direction.z;
      }
   
      l_vLight1.x = l_pMat->_11*x + l_pMat->_12*y + l_pMat->_13*z;
      l_vLight1.y = l_pMat->_21*x + l_pMat->_22*y + l_pMat->_23*z;
      l_vLight1.z = l_pMat->_31*x + l_pMat->_32*y + l_pMat->_33*z;
         
      x = g_d3dExteriorLights[1].Direction.x;
      y = g_d3dExteriorLights[1].Direction.y;
      z = g_d3dExteriorLights[1].Direction.z;
      l_vLight2.x = l_pMat->_11*x + l_pMat->_12*y + l_pMat->_13*z;
      l_vLight2.y = l_pMat->_21*x + l_pMat->_22*y + l_pMat->_23*z;
      l_vLight2.z = l_pMat->_31*x + l_pMat->_32*y + l_pMat->_33*z;
         
      x = g_vCamera.x - l_vPos.x;
      y = g_vCamera.y - l_vPos.y;
      z = g_vCamera.z - l_vPos.z;
      l_vCamera.x = l_pMat->_11*x + l_pMat->_12*y + l_pMat->_13*z;
      l_vCamera.y = l_pMat->_21*x + l_pMat->_22*y + l_pMat->_23*z;
      l_vCamera.z = l_pMat->_31*x + l_pMat->_32*y + l_pMat->_33*z;
         
   }
         
   // -- Diese Vektoren normalisieren
   //
   {
      float l_fLength;
   
      if (g_Player.m_bEntered == false)
      {
         l_fLength = sqrt(pow2(l_vLight1.x) + pow2(l_vLight1.y) + pow2(l_vLight1.z));
         l_vLight1.x = l_vLight1.x  / l_fLength;
         l_vLight1.y = l_vLight1.y  / l_fLength;
         l_vLight1.z = l_vLight1.z  / l_fLength;
      }
         
      l_fLength = sqrt(pow2(l_vLight2.x) + pow2(l_vLight2.y) + pow2(l_vLight2.z));
      l_vLight2.x = l_vLight2.x  / l_fLength;
      l_vLight2.y = l_vLight2.y  / l_fLength;
      l_vLight2.z = l_vLight2.z  / l_fLength;
         
      l_fLength = sqrt(pow2(l_vCamera.x) + pow2(l_vCamera.y) + pow2(l_vCamera.z));
      l_vCamera.x = l_vCamera.x  / l_fLength;
      l_vCamera.y = l_vCamera.y  / l_fLength;
      l_vCamera.z = l_vCamera.z  / l_fLength;
   }
         
   // -- dem KonstantenRegisterpuffer zuweisen
   //
   g_vwVSConstantArray[4].setVector(&l_vCamera,1.0f);
   g_vwVSConstantArray[8].setVector(&l_vLight1,1.0f);
   g_vwVSConstantArray[9].setVector(&l_vLight2,1.0f);
         
   // -- World matrix zum Enemy erzeugen = matEnemy * matViewProj
   //
   memcpy(&g_matGeneric2, l_pMat, sizeof(g_matGeneric2));
   g_matGeneric2._41 = l_vPos.x;
   g_matGeneric2._42 = l_vPos.y;
   g_matGeneric2._43 = l_vPos.z;
         
   matmult(&g_matGeneric1, &g_matGeneric2, &g_matViewProj);
         
   if (PSXCopyTranspose((D3DMATRIX*)g_vwVSConstantArray, &g_matGeneric1) == false)
   {
      return false;
   }
         
   // -- Skalierung einsetzen
   //
   g_vwVSConstantArray[5].setFloat( l_fScale, 
                                  l_fScale, 
                                  l_fScale, 
                                  1.0f);
            
   hr = g_pDev->SetVertexShaderConstantF(0, (const float*)g_vwVSConstantArray, 10);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetVertexShaderConstantF(matEnemy)");
      return(FALSE); 
   }
   return true;
}
*/
         

/*
BUMPDRAWER::BUMPDRAWER()
{
   if (g_bWriteLogFileInLoop)
   {
      WriteLogFile("BUMPDRAWER::BUMPDRAWER()\n");
   }
}

ENEMYDRAWER::ENEMYDRAWER()
{
   if (g_bWriteLogFileInLoop)
   {
      WriteLogFile("ENEMYDRAWER::ENEMYDRAWER()\n");
   }
}
*/
// --- Texturen einschalten
//
bool ENEMYDRAWER::setTextures(RESTYPE l_resType)
{
   if (g_bWriteLogFileInLoop)
   {
      WriteLogFile("ENEMYDRAWER::setTextures()\n");
   }

   // --- Textur handles zuordnen
   //
   
   if (g_txtassignResStdArray[l_resType].tidColor > TID_NULL)
   {
       if (g_txtArray[g_txtassignResStdArray[l_resType].tidColor].activate(0) == false)
       { 
          return false; 
       }
   }

   if (g_txtassignResStdArray[l_resType].tidEmissive > TID_NULL)
   {
       if (g_txtArray[g_txtassignResStdArray[l_resType].tidEmissive].activate(1) == false)
       { 
          return false; 
       }
   }

   if (g_txtassignResStdArray[l_resType].tidBump > TID_NULL)
   {
       if (g_txtArray[g_txtassignResStdArray[l_resType].tidBump].activate(2) == false)
       { 
          return false; 
       }
   }

   //if (g_bEnableTextures)
   //{
   //for (int i = 0;i < m_cntStages; i++)
   //{

      // if (m_arrStages[i] > TID_NULL)
      // {
         //  if (g_txtArray[m_arrStages[i]].activate(i) == false) return false;
         
         //  if ( PSXSetMipMapping(g_txtArray[m_arrStages[i]].m_bMipMapped,i) == false ) 
         //  { 
            //   return false;
         //  }
         
      // }else{
         //  HRESULT hr;
         //  g_pImmediateContext->PSSetShaderResources( i    , 1, NULL);
      // }

   //}
   //}

   return true;
}

// --- den ResType im Ausgabe-Stream setzen
//
bool ENEMYDRAWER::setOutputStream(RESTYPE l_resType)
{

   if (g_bWriteLogFileInLoop)
   {
      WriteLogFile("ENEMYDRAWER::setOutputStream()\n");
   }

   // --- prepare Vertex/Indices Input Stream
   //
   if (g_meshresManager.setIndices(l_resType) == false) return false;
   if (g_meshresManager.setStreamSource(0, l_resType) == false) return false;
   
   return true;
}


// --- Alle durch pointer referenzierten Arrayeintraege zeichnen
//
bool ENEMYDRAWER::Draw(RESTYPE l_resType)                        
{

   if (g_bWriteLogFileInLoop)
   {
      WriteLogFile("ENEMYDRAWER::Draw()\n");
   }

   
   if (setOutputStream(l_resType) == false) return false;
   
   // -- Texturen einschalten
   //
   if (setTextures(l_resType) == false) return false; 
   
   if (g_meshresManager.getVertexCount(l_resType) > 0 &&
       g_meshresManager.getFaceCount(l_resType) > 0)
   {
    
       //if (g_meshresManager.setIndices(m_ResType) == false) return false;
       //if (g_meshresManager.setStreamSource(0, m_ResType) == false) return false;

       g_pImmediateContext->DrawIndexed(g_meshresManager.getFaceCount(l_resType)*3,0, 0);      

      //hr = g_pDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
      //                                  g_meshresManager.getVertexCount(m_ResType), 0, 
      //                                  g_meshresManager.getFaceCount(m_ResType));

      //if (hr != 0)
      //{ 
      //   hr2message(__LINE__,hr, "g_pDev->DrawIndexedPrimitive(dot3bumpshader)");
      //   return false;
      //}
   }
   
   return true;
}


// -- initialisiert die Bumpdrawer und Enemydrawer arrays, die ResType indiziert funktionieren,
//    u.a. mit den Texture-Ids 
//
bool PSXInitDrawers()
{
   
   unsigned int l_resType, l_idxDrawer;
   int l_idxAssign;
   TEXTUREASSIGN l_txtassignLocal;
   ENEMYDRAWER l_edrawLocal;
   SIMPLEDRAWER l_simpledrawLocal;
   BUMPDRAWER l_bumpdrawerLocal;   
   
   // -- initialisieren des Kopf-Drawer Arrays
   //
   for (l_idxDrawer = 0;l_idxDrawer < EDRAW_LASTENUM; l_idxDrawer++)
   {     
      memcpy(g_arr_EnemyDrawer+l_idxDrawer, &l_edrawLocal , sizeof(l_edrawLocal));
      switch (l_idxDrawer)
         {
            case EDRAW_RES_STANDARD:               
               memcpy(g_arr_EnemyDrawer+l_idxDrawer, &l_edrawLocal, sizeof(g_arr_EnemyDrawer[0]));
               break;
            case EDRAW_RES_EMISSIVE:
               memcpy(g_arr_EnemyDrawer+l_idxDrawer, &l_simpledrawLocal, sizeof(g_arr_EnemyDrawer[0]));
               break;
            case EDRAW_RES_BUMP:
                memcpy(g_arr_EnemyDrawer+l_idxDrawer, &l_bumpdrawerLocal, sizeof(g_arr_EnemyDrawer[0]));
               break;

            default:
               memcpy(g_arr_EnemyDrawer+l_idxDrawer, &l_edrawLocal, sizeof(g_arr_EnemyDrawer[0]));              
         }      
   }

   l_txtassignLocal.tidBump = TID_NULL;
   l_txtassignLocal.tidColor = TID_NULL;
   l_txtassignLocal.tidEmissive = TID_NULL;
   l_txtassignLocal.tidEnv = TID_NULL;
   l_txtassignLocal.idxEnemyDrawer = EDRAW_RES_STANDARD;

   // -- initialisieren des gesamten ResStd-Arrays
   //
   for (l_resType = 0; l_resType < RESTYPE_LASTENUM; l_resType++)
   {           
       memcpy(g_txtassignResStdArray+l_resType, &l_txtassignLocal, sizeof(g_txtassignResStdArray[0]));            
       g_txtassignResStdArray[l_resType].resPart = static_cast<RESTYPE>(l_resType);       
   }

   // -- Texturzuordnung für ausgesuchte Standard-Res-Drawer
   //
   for (l_idxAssign = 0; l_idxAssign < g_cntTxtAssignCompressed; l_idxAssign++)
   {
      l_resType = g_txtassigncompressedArray[l_idxAssign].resPart;

      g_txtassignResStdArray[l_resType].tidColor    = g_txtassigncompressedArray[l_idxAssign].tidColor;
      g_txtassignResStdArray[l_resType].tidEmissive = g_txtassigncompressedArray[l_idxAssign].tidEmissive;
      g_txtassignResStdArray[l_resType].tidBump     = g_txtassigncompressedArray[l_idxAssign].tidBump;
      g_txtassignResStdArray[l_resType].tidEnv      = g_txtassigncompressedArray[l_idxAssign].tidEnv; 
      g_txtassignResStdArray[l_resType].idxEnemyDrawer = g_txtassigncompressedArray[l_idxAssign].idxEnemyDrawer;
      {
         char l_cStage[500];
         PSXPrintf(__LINE__,l_cStage, sizeof(l_cStage), 
                   "assign directive %i: restype %i with textures %i,%i,%i\n", 
                   l_idxAssign,
                   l_resType,
                   g_txtassigncompressedArray[l_idxAssign].tidColor,
                   g_txtassigncompressedArray[l_idxAssign].tidEmissive,
                   g_txtassigncompressedArray[l_idxAssign].tidBump);
         WriteLogFile(l_cStage);
      }

   }

    D3D11_BUFFER_DESC bd;
    ZeroMemory( &bd, sizeof(bd) );
    /*
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( vertices );
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    */
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory( &InitData, sizeof(InitData) );

    HRESULT hr = 0;
   // Create the constant buffer
   bd.Usage = D3D11_USAGE_DEFAULT;
   bd.ByteWidth = sizeof(ENEMYDRAWERCONSTANTBUFFER::cbChangeEveryFrame);
   bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
   bd.CPUAccessFlags = 0;
   hr = g_pd3dDevice->CreateBuffer( &bd, nullptr, &ENEMYDRAWER::m_pcbChangeEveryFrame );
   if( FAILED( hr ) )
   {
       hr2message(__LINE__,hr, "g_pd3dDevice->CreateBuffer(pcbChangeEveryFrame)");
       return false;
   }

   bd.ByteWidth = sizeof(ENEMYDRAWERCONSTANTBUFFER::cbChangeOnResize);
   hr = g_pd3dDevice->CreateBuffer( &bd, nullptr, &ENEMYDRAWER::m_pcbChangeOnResize );
   if( FAILED( hr ) )
   {
       hr2message(__LINE__,hr, "g_pd3dDevice->CreateBuffer(pcbChangeOnResiz)");
       return false;
   }

   bd.ByteWidth = sizeof(ENEMYDRAWERCONSTANTBUFFER::cbChangeEveryObject);
   hr = g_pd3dDevice->CreateBuffer( &bd, nullptr, &ENEMYDRAWER::m_pcbChangeEveryObject );
   if( FAILED( hr ) )
   {
       hr2message(__LINE__,hr, "g_pd3dDevice->CreateBuffer(pcbChangeEveryObject)");
       return false;
   }

   // -- Zuordnung die nicht im TxtAssign Array drinstehen kann
   //
/*
   g_txtassignResStdArray[RESTYPE_HANGAR].m_cntMatTexture = 4;
   g_txtassignResStdArray[RESTYPE_HANGAR].m_TextureID[0]    = TID_SCHLAUCH;   
   g_txtassignResStdArray[RESTYPE_HANGAR].m_EmissiveID[0]   = TID_SCHLAUCH_EM;
   g_txtassignResStdArray[RESTYPE_HANGAR].m_TextureID[1]    = TID_D_HULL_M;
   g_txtassignResStdArray[RESTYPE_HANGAR].m_EmissiveID[1]   = TID_D_HULL_M_EM;
   g_txtassignResStdArray[RESTYPE_HANGAR].m_TextureID[2]    = TID_FENSTERF;
   g_txtassignResStdArray[RESTYPE_HANGAR].m_EmissiveID[2]   = TID_FENSTERF_EM;
   g_txtassignResStdArray[RESTYPE_HANGAR].m_TextureID[3]    = TID_ELEKTRON;
   g_txtassignResStdArray[RESTYPE_HANGAR].m_EmissiveID[3]   = TID_NULL;
   g_edrawResEmissiveArray[RESTYPE_HANGAR] = g_txtassignResStdArray[RESTYPE_HANGAR];
   g_edrawResEmissiveArray[RESTYPE_HANGAR].m_bEmissive = true;

*/
 

   return true;
}
// -- kein Pixelshader aktivieren, solange noch die texturestages verwendet werden
//
bool SIMPLEDRAWER::setPixelShader()
{
   if (g_bWriteLogFileInLoop)
   {
      WriteLogFile("SIMPLEDRAWER::setPixelShader()\n");
   }
/*
   if (m_bEmissive)
   {
      hr = g_pDev->SetPixelShader( g_hDot3BumpPixelShader );
      if (hr != 0) { 
         hr2message(__LINE__,hr,"g_pDev->SetPixelShader(DOT3Bump Pixelshader)");
         return false;
      }
   }else{
      hr = g_pDev->SetPixelShader( g_hBumpNoEmPixelShader );
      if (hr != 0) { 
         hr2message(__LINE__,hr,"g_pDev->SetPixelShader(DOT3Bump Pixelshader)");
         return false;
      }
   }
*/
   g_pImmediateContext->IASetInputLayout( g_pVertexLayout );
   g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
   g_pImmediateContext->VSSetShader( g_pVertexShaderSimpleDrawer, nullptr, 0 );
   g_pImmediateContext->VSSetConstantBuffers( 0, 1, &m_pcbChangeEveryObject );
   g_pImmediateContext->VSSetConstantBuffers( 1, 1, &m_pcbChangeOnResize );
   g_pImmediateContext->VSSetConstantBuffers( 2, 1, &m_pcbChangeEveryFrame );
   g_pImmediateContext->PSSetShader( g_pPixelShaderSimpleDrawer, nullptr, 0 );
   g_pImmediateContext->PSSetConstantBuffers( 0, 1, &m_pcbChangeEveryObject );
   g_pImmediateContext->PSSetConstantBuffers( 1, 1, &m_pcbChangeOnResize );
   g_pImmediateContext->PSSetConstantBuffers( 2, 1, &m_pcbChangeEveryFrame );
   //g_pImmediateContext->PSSetShaderResources( 0, 1, &g_pTextureRV );
   g_pImmediateContext->PSSetSamplers( 0, 1, &ENEMYDRAWER::m_pSamplerLinear );
   g_pImmediateContext->GSSetShader(nullptr, nullptr, 0);

   return true;
}


// -- kein Pixelshader aktivieren, solange noch die texturestages verwendet werden
//
bool BUMPDRAWER::setPixelShader()
{
   if (g_bWriteLogFileInLoop)
   {
      WriteLogFile("BUMPDRAWER::setPixelShader()\n");
   }
   g_pImmediateContext->IASetInputLayout( g_pVertexLayout );
   g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
   g_pImmediateContext->VSSetShader( g_pVertexShaderBumpDrawer, nullptr, 0 );
   g_pImmediateContext->VSSetConstantBuffers( 0, 1, &m_pcbChangeEveryObject );
   g_pImmediateContext->VSSetConstantBuffers( 1, 1, &m_pcbChangeOnResize );
   g_pImmediateContext->VSSetConstantBuffers( 2, 1, &m_pcbChangeEveryFrame );
   g_pImmediateContext->PSSetShader( g_pPixelShaderBumpDrawer, nullptr, 0 );
   g_pImmediateContext->PSSetConstantBuffers( 0, 1, &m_pcbChangeEveryObject );
   g_pImmediateContext->PSSetConstantBuffers( 1, 1, &m_pcbChangeOnResize );
   g_pImmediateContext->PSSetConstantBuffers( 2, 1, &m_pcbChangeEveryFrame );   
   g_pImmediateContext->PSSetSamplers( 0, 1, &ENEMYDRAWER::m_pSamplerLinear );
   g_pImmediateContext->GSSetShader(nullptr, nullptr, 0);
   return true;
}

bool BUMPDRAWER::prepareItem(FXMVECTOR l_vPos, XMMATRIX* l_pMat, float l_fScale)
{
	if (g_bWriteLogFileInLoop)
	{
		WriteLogFile("BUMPDRAWER::prepareItem()\n");
	}

	XMMATRIX l_matWorld = (*l_pMat)* XMMatrixTranslationFromVector(l_vPos);
	XMMATRIX l_matWorldTransposed = XMMatrixTranspose(*l_pMat);
	
	const XMFLOAT3 l_f3PseudoDirection = XMFLOAT3(1.0, 0.0, 0.0);
	const XMFLOAT3 l_f3PseudoPosition = XMFLOAT3(100.0, 0.0, 0.0);
	XMVECTOR l_vPseudoDirection = XMLoadFloat3(&l_f3PseudoDirection);
	XMVECTOR l_vPseudoPosition = XMLoadFloat3(&l_f3PseudoPosition);
	    
	//XMVECTOR l_vCameraObjRelative = XMVector3Transform(g_vCamera - l_vPos, l_matWorldTransposed);
	XMVECTOR l_vCameraObjRelative = XMVector3Transform(l_vPseudoPosition, (*l_pMat));
	//XMVECTOR l_vCameraDirection = XMVector3Transform(g_matCamera.r[2], l_matWorldTransposed);
	XMVECTOR l_vCameraDirection = XMVector3Transform(l_vPseudoDirection, (*l_pMat));
	
	ENEMYDRAWERCONSTANTBUFFER::cbChangeEveryObject cb1;
	memset(&cb1, 0, sizeof(cb1));

	cb1.mWorld = l_matWorldTransposed; //XMMatrixTranspose(l_matWorld);
	cb1.matWorldViewProj = XMMatrixTranspose(l_matWorld * g_View * g_Projection);
	cb1.d3dMaterial.Ambient = XMFLOAT4(0.1, 0.1, 0.1, 0);
	cb1.d3dMaterial.Diffuse = XMFLOAT4(1, 1, 1, 1);
	cb1.d3dMaterial.Reflect = XMFLOAT4(0, 0, 0, 0);
	cb1.d3dMaterial.Specular = XMFLOAT4(1, 1, 1, 1);

	XMStoreFloat3(&cb1.SpotLightPositionObjRelative, l_vCameraObjRelative);
	/*cb1.SpotLightDirectionObjRelative.x = 1.0;
	cb1.SpotLightDirectionObjRelative.y = 0.0;
	cb1.SpotLightDirectionObjRelative.z = 0.0;*/
	XMStoreFloat3(&cb1.SpotLightDirectionObjRelative, l_vCameraDirection);


	//if (g_Player.m_pEnemy)
	//{
	   // //l_vCameraObjRelative = XMVector3Transform(g_Player.m_pEnemy->getPos() - l_vPos, l_matWorldTransposed);
	   // l_vCameraObjRelative = g_Player.m_pEnemy->getPos();// -l_vPos;
	   // 
	//}
	//else {
	   // //l_vCameraObjRelative = XMVector3Transform(g_vCamera - l_vPos, l_matWorldTransposed);
	   // l_vCameraObjRelative = g_vCamera;// -l_vPos;
	//}

	g_pImmediateContext->UpdateSubresource(m_pcbChangeEveryObject, 0, nullptr, &cb1, 0, 0);


	/*
	*/
	return true;

}
