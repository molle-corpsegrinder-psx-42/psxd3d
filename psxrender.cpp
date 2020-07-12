/*
 *------------------------------------------------------------------------
 *      Object: psxrender.cpp 
 *   $Revision:  $
 *        Date: 2012                            
 *       Notes: Control of one Renderpass
 *------------------------------------------------------------------------
 * 
 * Copyright (c) 1997-2012 Michael Molkenthin. All Rights Reserved.
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

//
// -- PSXRender() : ins Rendertarget zeichnen
//
bool PSXRender(RENDERPASS l_renderpassCurr)
{
    HRESULT hr = NULL;
    
    // 1st Cube: Rotate around the origin
    //g_World = XMMatrixRotationY( t )*  XMMatrixRotationX(-1.1f * t);;

    // 2nd Cube:  Rotate around origin
    /*
    XMMATRIX mSpin = XMMatrixRotationZ( -t );
    XMMATRIX mOrbit = XMMatrixRotationY( -t * 2.0f );
    XMMATRIX mTranslate = XMMatrixTranslation( -4.0f, 0.0f, 0.0f );
    XMMATRIX mScale = XMMatrixScaling( 0.3f, 0.3f, 0.3f );

    g_World2 = mScale * mSpin * mTranslate * mOrbit;
    */

 
   

    // Set vertex buffer
    //UINT stride = sizeof( SimpleVertex );
    //UINT offset = 0;
    //g_pImmediateContext->IASetVertexBuffers( 0, 1, &g_pVertexBuffer, &stride, &offset );

    // Set index buffer
    //g_pImmediateContext->IASetIndexBuffer( g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0 );


	/*
	if (g_txtassignResStdArray[RESTYPE_LEADER].prepareHeader()==false) return false;
 	XMVECTOR l_vPos = XMVectorZero();
	XMMATRIX l_matLeader = XMMatrixIdentity();
	if (g_txtassignResStdArray[RESTYPE_LEADER].Draw(l_vPos, &g_World, 1.0) == false) return false;
	*/




	/*
    cbChangeEveryObject cb1;
	
    cb1.mWorld = XMMatrixTranspose( g_World );
    cb1.matWorldViewProj= XMMatrixTranspose( g_World * g_View * g_Projection );
 	cb1.d3dMaterial.Ambient = XMFLOAT4(0, 0, 0, 0);
	cb1.d3dMaterial.Diffuse = XMFLOAT4(1, 1, 1, 1);
	cb1.d3dMaterial.Reflect = XMFLOAT4(0, 0, 0, 0);
	cb1.d3dMaterial.Specular = XMFLOAT4(1, 1, 1, 1);
	g_pImmediateContext->UpdateSubresource(g_pcbChangeEveryObject, 0, nullptr, &cb1, 0, 0 );	 
	
    g_pImmediateContext->VSSetShader( g_pVertexShader, nullptr, 0 );
    g_pImmediateContext->VSSetConstantBuffers( 0, 1, &g_pcbChangeEveryObject );
    g_pImmediateContext->VSSetConstantBuffers( 1, 1, &g_pcbChangeOnResize );
    g_pImmediateContext->VSSetConstantBuffers( 2, 1, &g_pcbChangeEveryFrame );
    g_pImmediateContext->PSSetShader( g_pPixelShader, nullptr, 0 );
    g_pImmediateContext->PSSetConstantBuffers( 0, 1, &g_pcbChangeEveryObject );
    g_pImmediateContext->PSSetConstantBuffers( 1, 1, &g_pcbChangeOnResize );
    g_pImmediateContext->PSSetConstantBuffers( 2, 1, &g_pcbChangeEveryFrame );
    g_pImmediateContext->PSSetShaderResources( 0, 1, &g_pTextureRV );
    g_pImmediateContext->PSSetSamplers( 0, 1, &g_pSamplerLinear );

	if (g_meshresManager.setIndices(RESTYPE_LEADER) == false) return false;
	if (g_meshresManager.setStreamSource(0, RESTYPE_LEADER) == false) return false;
	
	if (g_txtArray[TID_LEADER_RED].activate(0) == false) return false;
	
	g_pImmediateContext->DrawIndexed(g_meshresManager.getFaceCount(RESTYPE_LEADER)*3,0, 0);
	*/

	//if (PSXDrawResource(RESTYPE_LEADER, true) == false)
    //{
    //   return false;
    //}
    
    // -- Draw whole object materialindependent
    //
	//char l_cMessageAboutFaceCount[500];
	//PSXPrintf(__LINE__, l_cMessageAboutFaceCount, sizeof(l_cMessageAboutFaceCount), "FaceCount:%i", g_meshresManager.getFaceCount(RESTYPE_LEADER));
	//WriteLogFile(l_cMessageAboutFaceCount);
    //g_pImmediateContext->DrawIndexed( int(3)*g_meshresManager.getFaceCount(RESTYPE_LEADER),0,0);

    //
    // Render each light
    //
	/*
    for( int m = 0; m < 2; m++ )
    {
        XMMATRIX mLight = XMMatrixTranslationFromVector( 5.0f * XMLoadFloat4( &vLightDirs[m] ) );
        XMMATRIX mLightScale = XMMatrixScaling( 0.2f, 0.2f, 0.2f );
        mLight = mLightScale * mLight;

        // Update the world variable to reflect the current light
        cb1.mWorld = XMMatrixTranspose( mLight );
        cb1.vOutputColor = vLightColors[m];

        g_pImmediateContext->UpdateSubresource( g_pcbChangeEveryObject, 0, nullptr, &cb1, 0, 0 );
        g_pImmediateContext->PSSetShader( g_pPixelShaderSolid, nullptr, 0 );

        //
        // Render the second cube
        //
        g_pImmediateContext->DrawIndexed( 36, 0, 0 );
        
    }
     
	 */
 
    switch(l_renderpassCurr)
    {
       case RENDERPASS_BLOOMINPUT:
         if (PSXDoRenderStuffBackground(l_renderpassCurr) == false) return false;
         break;
       case RENDERPASS_BLOOMRESULT:
         if (PSXDoRenderStuffBloom2D(l_renderpassCurr) == false) return false;
         break;
       default:
         if (PSXDoRenderStuffBackground(l_renderpassCurr) == false) return false;
         if (PSXDoRenderStuffScene(l_renderpassCurr) == false) return false;
         if (PSXDoRenderStuff2D(l_renderpassCurr) == false) return false;
    }

 
    return true;

}

//
// -- draw-stuff which hasnt any dedicated function
//
bool PSXDoRenderStuffBackground(RENDERPASS l_renderpassCurr)
{
/*

   // *************************************************************
   //          BACKGROUND: Starfield, Nebula, Planet
   // *************************************************************


   hr = g_pDev->SetVertexShader( NULL);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetVertexShader(PSXVERTEX)");
      return false;
   }
   hr = g_pDev->SetVertexDeclaration(g_pvdeclPSXVertex);
   if (hr != 0) { 
	   hr2message(__LINE__,hr, "g_pDev->SetVertexDeclaration(PSXVERTEX)");
	   return false;
   }



   // --- Paint Skyball
   //
   if (g_bWriteLogFileInLoop) 
   {
      WriteLogFile("drawing Skyball\n");
   }

   memset(&g_D3DMaterial, 0, sizeof(g_D3DMaterial));
   g_D3DMaterial.Diffuse  = g_d3dcolWhite;
   g_D3DMaterial.Ambient  = g_d3dcolWhite; 
   g_D3DMaterial.Specular = g_d3dcolWhite;
   g_D3DMaterial.Emissive = g_d3dcolWhite;
   g_D3DMaterial.Power    = 10.0f;

   hr = g_pDev->SetMaterial(&g_D3DMaterial);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetMaterial(Skyball)");
      return false;
   }

   memcpy (&g_matGeneric1,&g_matIdentity, sizeof(g_matIdentity));

   g_matGeneric1._11 = cos ( g_fPi/2) * 100.0f;
   g_matGeneric1._12 = - sin ( g_fPi/2 ) * 100.0f;
   g_matGeneric1._21 = sin ( g_fPi/2 ) * 100.0f;
   g_matGeneric1._22 = cos ( g_fPi/2 ) * 100.0f;
   g_matGeneric1._33 = 100.0f;
   g_matGeneric1._41 = g_vSkyball.x;
   g_matGeneric1._42 = g_vSkyball.y;
   g_matGeneric1._43 = g_vSkyball.z;

   // -- ZBUFFER aufraeumen, da der Skyball beim Hyperraumsprung
   //    an dem Player vorbei fliegt, und so nicht mehr den ZBUFFER
   //    ueberschreiben kann...
   //
   hr = g_pDev->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->Clear(TARGET)");
      return(FALSE);
   }

   hr = g_pDev->SetTransform(D3DTS_WORLD, &g_matGeneric1);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetTransform(Skyball)");
      return false;
   }

   if (g_meshresManager.setIndices(RESTYPE_SKYBALL) == false) return false;
   if (g_meshresManager.setStreamSource(0, RESTYPE_SKYBALL) == false) return false;

   if (g_bEnableTextures)
   {
      switch (g_Player.m_idxGalaxy)
      {
         case 0: if (g_txtArray[TID_NULL].activate(0) == false) return false; break;
         case 1: if (g_txtArray[TID_NEBULA2].activate(0) == false) return false; break;
         case 2: if (g_txtArray[TID_NEBULA3].activate(0) == false) return false; break;
         case 3: if (g_txtArray[TID_NEBULA4].activate(0) == false) return false; break;
         case 4: if (g_txtArray[TID_NEBULA5].activate(0) == false) return false; break;
         case 5: if (g_txtArray[TID_NEBULA6].activate(0) == false) return false; break;
         case 6: if (g_txtArray[TID_NEBULA7].activate(0) == false) return false; break;
         case 7: if (g_txtArray[TID_NEBULA8].activate(0) == false) return false; break;
      }
   }

   if (PSXDrawResource(RESTYPE_SKYBALL, true) == false)
   {
      return false;
   }

   // -- starfield zeichnen
   //
   if (g_bWriteLogFileInLoop) 
   {
      WriteLogFile("drawing starfield\n");
   }

   hr = g_pDev->SetTexture(0, NULL);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetTexture()");
      return false;
   }

   // -- SFVERTEX format einschalten
   //
   
   hr = g_pDev->SetFVF( D3DFVF_XYZ|
                                 D3DFVF_DIFFUSE|
                                 D3DFVF_SPECULAR );
   if (hr != 0) { 
      hr2message(__LINE__,hr, "g_pDev->SetVertexShader(SFVERTEX)");
      return false;
   }
 

   memcpy (&g_matGeneric1,&g_matIdentity, sizeof(g_matIdentity));
   g_matGeneric1._41 = g_vStarfield.x;  //--g_vStarfield WITCHSPACE releation 
   g_matGeneric1._42 = g_vStarfield.y;
   g_matGeneric1._43 = g_vStarfield.z;

//   g_matGeneric1._41 = g_vCamera.x;
//   g_matGeneric1._42 = g_vCamera.y;
//   g_matGeneric1._43 = g_vCamera.z;

   hr = g_pDev->SetTransform(D3DTS_WORLD, &g_matGeneric1);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetTransform(Starfield)");
      return false;
   }

   hr = g_pDev->SetStreamSource(0, g_lpD3DStarVB, 0, sizeof(SFVERTEX));
   if (hr != 0) { 
      hr2message(__LINE__,hr, "g_pDev->SetStreamSource(Starfield)");
      return false;
   }

   hr = g_pDev->DrawPrimitive( D3DPT_POINTLIST, 0, cnt_sfieldvert -1);
   if (hr != 0) { 
      hr2message(__LINE__,hr, "g_pDev->DrawPrimitive(Starfield)");
      return false;
   }

   // --- Beleuchtungssituation 
   //
   if (g_bWriteLogFileInLoop) 
   {
      WriteLogFile("setting light\n");
   }

   if (g_Player.m_pLevel->m_bDayLight)
   {
      int l_idxExteriorLightsOn;

      for (l_idxExteriorLightsOn=0;
           l_idxExteriorLightsOn<g_cntExteriorLights;
           l_idxExteriorLightsOn++)
      {
         hr = g_pDev->SetLight(l_idxExteriorLightsOn,
                               &g_d3dExteriorLights[l_idxExteriorLightsOn]);
         if (hr != 0) { 
            hr2message(__LINE__,hr,"g_pDev->SetLight(ExteriorLight)");
            return false;
         }
         hr = g_pDev->LightEnable(l_idxExteriorLightsOn,TRUE);
         if (hr != 0) { 
            hr2message(__LINE__,hr,"g_pDev->LightEnable(ExteriorLight)");
            return false;
         }
      }
   }

   // -- Planet, wenn Planet sichtbar
   //
   if (g_bWriteLogFileInLoop) 
   {
      WriteLogFile("drawing planet\n");
   }

   if ((g_Player.m_pLevel->m_d3dcolPlanet.r +
        g_Player.m_pLevel->m_d3dcolPlanet.g +
        g_Player.m_pLevel->m_d3dcolPlanet.b) > 0.0f)
   {
      int l_idxDraw;
      int l_idxAstro;

      // -- Farbe des Planeten 
      //
      memset(&g_D3DMaterial, 0, sizeof(g_D3DMaterial));
      g_D3DMaterial.Diffuse  = g_Player.m_pLevel->m_d3dcolPlanet;
      g_D3DMaterial.Power    = 1.0f;
      hr = g_pDev->SetMaterial(&g_D3DMaterial);
      if (hr != 0) { 
         hr2message(__LINE__,hr,"g_pDev->SetMaterial(PLANET)");
         return false;
      }
   
      hr = g_pDev->SetVertexShader( NULL);
      if (hr != 0) { 
         hr2message(__LINE__,hr,"g_pDev->SetVertexShader(PSXVERTEX)");
         return false;
      }
      hr = g_pDev->SetVertexDeclaration(g_pvdeclPSXVertex);
      if (hr != 0) { 
	      hr2message(__LINE__,hr, "g_pDev->SetVertexDeclaration(PSXVERTEX)");
	      return false;
      }
   
		if (g_bEnableTextures)
		{
			if (g_txtArray[TID_PLANET].activate(0) == false) return false;
		}

		//if (false) // auskommentieren, aber doch mitchecken lassen
		{
			if (g_meshresManager.setIndices(RESTYPE_PLANET) == false) return false;
			if (g_meshresManager.setStreamSource(0, RESTYPE_PLANET) == false) return false;


			memcpy (&g_matGeneric1,&g_matIdentity, sizeof(g_matIdentity));
			g_matGeneric1._11 = cos ( g_fPi/2);
			g_matGeneric1._12 = - sin ( g_fPi/2 );
			g_matGeneric1._21 = sin ( g_fPi/2 );
			g_matGeneric1._22 = cos ( g_fPi/2 );

			//-- Witchspace abhaengige Vectoren
			//
			g_matGeneric1._41 = g_vCam4Astro.x;
			g_matGeneric1._42 = g_vCam4Astro.y;
			g_matGeneric1._43 = g_vCam4Astro.z - g_Player.m_pLevel->m_fPlanetDistance;

			hr = g_pDev->SetTransform(D3DTS_WORLD, &g_matGeneric1);
			if (hr != 0) { 
				hr2message(__LINE__,hr,"g_pDev->SetTransform(planet)");
				return false;
			}

			hr = g_pDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 
				0, 0, g_meshresManager.getVertexCount(RESTYPE_PLANET), 
				0, g_meshresManager.getFaceCount(RESTYPE_PLANET));
			if (hr != 0) 
			{ 
				hr2message(__LINE__,hr, "g_pDev->DrawIndexedPrimitive(PLANET)");
				return true;
			}
		} // if (false) zum logischen auskommentieren


   }
*/

   return true;
}
//
   
// -- draw-stuff for organizated scene , enemy array and others
//
bool PSXDoRenderStuffScene(RENDERPASS l_renderpassCurr)
{
    HRESULT hr = NULL;

   // *************************************************************
   //                      DRAWING ENEMIES 
   // **************************************************************/
   
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_pImmediateContext->IASetInputLayout( g_pVertexLayout );

   // -- count the Enemy-type usage
   //    
   if (g_bWriteLogFileInLoop) 
   {
      WriteLogFile("sort-in enemies into drawerobjects\n");
   }

   // set back counters
   //
   
   {
      int l_idxFirst, l_iIncrement;
      int i;
      ENEMY* l_penemyCurr;

      for (i=0;i<EDRAW_LASTENUM;i++)
      {
         g_arr_EnemyDrawer[i].m_idxFirst = 0;
      }
/*
      for (i=0;i<RESTYPE_LASTENUM;i++)
      {
         g_txtassignResStdArray[i].m_idxFirst = 0;
      }

      for (i=0;i<RESTYPE_LASTENUM;i++)
      {
         g_edrawResEmissiveArray[i].m_idxFirst = 0;
      }

      for (i=0;i<RESTYPE_LASTENUM;i++)
      {
         g_edrawResBumpArray[i].m_idxFirst = 0;
      }
*/
      // get the groupcount of the corresponding restype
      //
      l_penemyCurr = ENEMY::getFirstEnemy();
      while(l_penemyCurr)
      {
         IDXENEMYDRAWER l_idxEnemyDrawer, l_idxEnemyDrawer2;
         if (l_penemyCurr->DecideForEnemyDrawer(&l_idxEnemyDrawer, 
                                                &l_idxEnemyDrawer2)==false)
         { 
            return false;
         }
         g_arr_EnemyDrawer[l_idxEnemyDrawer].m_idxFirst++;
         
         if (l_idxEnemyDrawer2)
         {
            g_arr_EnemyDrawer[l_idxEnemyDrawer2].m_idxFirst++;
         }
/*     
         switch (l_idxEnemyDrawer)
         {
            case EDRAW_RES_STANDARD:
               g_txtassignResStdArray[l_penemyCurr->getRes()].m_idxFirst++;
               break;
            case EDRAW_RES_EMISSIVE:
               g_edrawResEmissiveArray[l_penemyCurr->getRes()].m_idxFirst++;
               break;
            case EDRAW_RES_BUMP:
               g_edrawResBumpArray[l_penemyCurr->getRes()].m_idxFirst++;
               break;
         }
*/
         l_penemyCurr = l_penemyCurr->getNextEnemy();
      }

      // accumulate l_idxFirst thru Array, resetting drawcount
      //
      l_idxFirst = 0;
      for (i=0;i<EDRAW_LASTENUM;i++)
      {
         l_iIncrement = g_arr_EnemyDrawer[i].m_idxFirst;
         g_arr_EnemyDrawer[i].m_idxFirst = l_idxFirst;
         l_idxFirst = l_idxFirst + l_iIncrement;

         g_arr_EnemyDrawer[i].m_cntDraw = 0;
      }
/*
      l_idxFirst = g_arr_EnemyDrawer[EDRAW_RES_STANDARD].m_idxFirst;
      for (i=0;i<RESTYPE_LASTENUM;i++)
      {
         l_iIncrement = g_txtassignResStdArray[i].m_idxFirst;
         g_txtassignResStdArray[i].m_idxFirst = l_idxFirst;
         l_idxFirst = l_idxFirst + l_iIncrement;

         g_txtassignResStdArray[i].m_cntDraw = 0;
      }

      l_idxFirst = g_arr_EnemyDrawer[EDRAW_RES_EMISSIVE].m_idxFirst;
      for (i=0;i<RESTYPE_LASTENUM;i++)
      {
         l_iIncrement = g_edrawResEmissiveArray[i].m_idxFirst;
         g_edrawResEmissiveArray[i].m_idxFirst = l_idxFirst;
         l_idxFirst = l_idxFirst + l_iIncrement;

         g_edrawResEmissiveArray[i].m_cntDraw = 0;
      }

      l_idxFirst = g_arr_EnemyDrawer[EDRAW_RES_BUMP].m_idxFirst;
      for (i=0;i<RESTYPE_LASTENUM;i++)
      {
         l_iIncrement = g_edrawResBumpArray[i].m_idxFirst;
         g_edrawResBumpArray[i].m_idxFirst = l_idxFirst;
         l_idxFirst = l_idxFirst + l_iIncrement;

         g_edrawResBumpArray[i].m_cntDraw = 0;
      }
	  */
   }

   // -- fill restype-sorted pEnemy-Array relying on the groupcounts
   //
   if (g_bWriteLogFileInLoop) 
   {
      WriteLogFile("refill enemy-pointers\n");
   }

   {
      IDXENEMYDRAWER l_idxEnemyDrawer, l_idxEnemyDrawer2;
      ENEMY* l_penemyCurr;
      int l_idxFirst;

      l_penemyCurr = ENEMY::getFirstEnemy();
      while(l_penemyCurr)
      {
         if (l_penemyCurr->DecideForEnemyDrawer(&l_idxEnemyDrawer, &l_idxEnemyDrawer2)==false)
         { 
            return false;
         }

         // -- idxFirst als SchreibIndex ins pEnemyArray feststellen
         //
         l_idxFirst = g_arr_EnemyDrawer[l_idxEnemyDrawer].m_idxFirst+ 
                      g_arr_EnemyDrawer[l_idxEnemyDrawer].m_cntDraw;

         g_arr_EnemyDrawer[l_idxEnemyDrawer].m_cntDraw++;
        
         // -- im Falle von generischen-Drawer, den idxFirst nochmal differenzieren
         //
		 /*
         switch (l_idxEnemyDrawer)
         {
            case EDRAW_RES_STANDARD:
               l_idxFirst = g_txtassignResStdArray[l_penemyCurr->getRes()].m_idxFirst+ 
                            g_txtassignResStdArray[l_penemyCurr->getRes()].m_cntDraw;
      
               g_txtassignResStdArray[l_penemyCurr->getRes()].m_cntDraw++;
               break;
   
            case EDRAW_RES_EMISSIVE:
               l_idxFirst = g_edrawResEmissiveArray[l_penemyCurr->getRes()].m_idxFirst+ 
                            g_edrawResEmissiveArray[l_penemyCurr->getRes()].m_cntDraw;
      
               g_edrawResEmissiveArray[l_penemyCurr->getRes()].m_cntDraw++;
               break;

            case EDRAW_RES_BUMP:
               l_idxFirst = g_edrawResBumpArray[l_penemyCurr->getRes()].m_idxFirst+ 
                            g_edrawResBumpArray[l_penemyCurr->getRes()].m_cntDraw;
      
               g_edrawResBumpArray[l_penemyCurr->getRes()].m_cntDraw++;
               break;
         }
		 */
         // -- in den ausgemachten Index den Enemy-Zeiger wegschreiben
         //
         g_arr_pEnemy[l_idxFirst] = l_penemyCurr;

         // -- zweite mögliche Schublade mit derselben Prozedur
         //
         if (l_idxEnemyDrawer2)
         {
            l_idxFirst = g_arr_EnemyDrawer[l_idxEnemyDrawer2].m_idxFirst+ 
                         g_arr_EnemyDrawer[l_idxEnemyDrawer2].m_cntDraw;
   
            g_arr_EnemyDrawer[l_idxEnemyDrawer2].m_cntDraw++;
   
            g_arr_pEnemy[l_idxFirst] = l_penemyCurr;
         }
      
         l_penemyCurr = l_penemyCurr->getNextEnemy();
      }
   }
   /*
   hr = g_pDev->SetVertexShader( NULL);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetVertexShader(PSXVERTEX)");
      return false;
   }
   hr = g_pDev->SetVertexDeclaration(g_pvdeclPSXVertex);
   if (hr != 0) { 
	   hr2message(__LINE__,hr, "g_pDev->SetVertexDeclaration(PSXVERTEX)");
	   return false;
   }
   */
   // --- Standard-Resourcen Drawer
   //
   if (g_bWriteLogFileInLoop) 
   {
      WriteLogFile("draw Standard-Resources\n");
   }


   if (g_arr_EnemyDrawer[EDRAW_RES_STANDARD].m_cntDraw > 0)
   {
      ENEMYDRAWER* l_pedrawThis = g_arr_EnemyDrawer+EDRAW_RES_STANDARD;
	  for (int l_idxEnemyArray = 0; l_idxEnemyArray < l_pedrawThis->m_cntDraw; l_idxEnemyArray++)
	  {		
		  if (l_pedrawThis->prepareHeader()==false) return false;

		  ENEMY* l_penemyCurr = g_arr_pEnemy[l_pedrawThis->m_idxFirst + l_idxEnemyArray];
		  if (l_penemyCurr)
		  {		  			  
			  
			  //XMVECTOR l_vPos = XMVectorZero();
			  //XMMATRIX l_matLeader = XMMatrixIdentity();

			  RESTYPE l_resType = l_penemyCurr->getRes();
			  if (l_pedrawThis->prepareItem(
                    l_penemyCurr->getPos(), //l_vPos, 
                    l_penemyCurr->getMatrixPointer(), //&g_World, 
                    1.0) == false)
			  {
				  return false;
			  }
   
			  if (l_pedrawThis->setPixelShader() == false) return false;
			  if (l_pedrawThis->Draw(l_resType) == false) return false;

		  }

	  }
	  /*
      for (l_resType = 0;l_resType < RESTYPE_LASTENUM; l_resType++)
      {
         // -- gibt keine enemies dazu => auslassen
         //
         if (g_txtassignResStdArray[l_resType].m_cntDraw > 0)
         {
            if (g_txtassignResStdArray[l_resType].Draw()==false) 
            { 
               return false; 
            }
         }
      }
	  */
   }

   /*
   if (g_bWriteLogFileInLoop) 
   {
      WriteLogFile("draw Emissive Resources\n");
   }

   //if (g_arr_EnemyDrawer[EDRAW_RES_EMISSIVE].prepareHeader() == false) return false;
   if (g_arr_EnemyDrawer[EDRAW_RES_EMISSIVE].m_cntDraw > 0)
   {
      int l_resType;
      for (l_resType = 0;l_resType < RESTYPE_LASTENUM; l_resType++)
      {
         // -- gibt keine enemies dazu => auslassen
         //
         if (g_edrawResEmissiveArray[l_resType].m_cntDraw > 0)
         {
            if (g_edrawResEmissiveArray[l_resType].Draw()==false) 
            { 
               return false; 
            }
         }
      }
   }

   // -- Pixelshader fällt aus, daher die Singletons mit herkömmlichen Mitteln zeichnen
   //
   if (g_hDot3BumpPixelShader == NULL)
   {
      if (g_Player.getGS()->displayNonEnemy(g_edrawResEmissiveArray) == false) return false;
   }
  
   // -- Texturestages auf Normal setzen, aber spezialeinstellung für stage 3
   //
   {
      PREPARE_TSS_DATA l_ptdDefaultArray[] = {
         { 0, D3DTSS_COLOROP, D3DTOP_MODULATE },
         { 0, D3DTSS_COLORARG1, D3DTA_TEXTURE },
         { 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE },
   
         { 0, D3DTSS_TEXCOORDINDEX, 0 },
         { 1, D3DTSS_TEXCOORDINDEX, 1 },
         { 2, D3DTSS_TEXCOORDINDEX, 2 },
         { 3, D3DTSS_TEXCOORDINDEX, 3 },
         { 4, D3DTSS_TEXCOORDINDEX, 4 },
         { 5, D3DTSS_TEXCOORDINDEX, 5 },
         { 6, D3DTSS_TEXCOORDINDEX, 6 },
         { 7, D3DTSS_TEXCOORDINDEX, 7 },
   
         { 1, D3DTSS_COLOROP, D3DTOP_DISABLE },
         { 2, D3DTSS_COLOROP, D3DTOP_DISABLE },
      };
      if ( PSXSetTextureStageState(l_ptdDefaultArray, 
                                   sizeof(l_ptdDefaultArray)/
                                   sizeof(l_ptdDefaultArray[0])) == false)
      { 
         return (false);
      }
   
      PREPARE_TEXTURESAMPLER_DATA l_sampDefaultArray[] = {
         { 2, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP }, 
         { 2, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP },
         { 3, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP }, 
         { 3, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP },
      };
     
      if ( PSXSetTextureSamplerState(l_sampDefaultArray, 
                                   sizeof(l_sampDefaultArray)/
                                   sizeof(l_sampDefaultArray[0])) == false)
      { 
         return (false);
      }
   }

   // --- zeichnen mit dem bumpshader
   //
   if (g_bWriteLogFileInLoop) 
   {
      WriteLogFile("draw Dot3 bumpmapped Resources\n");
   }

   if (g_Player.m_bEntered)
   {
      hr = g_pDev->SetVertexShader( g_hDot3BumpVertexShader );
      if (hr != 0) { 
         hr2message(__LINE__,hr,"g_pDev->SetVertexShader(DOT3Bump Vertexshader)");
         return false;
      }
	  
   }else{
      hr = g_pDev->SetVertexShader( g_hDirectionalLightBumpVertexShader );
      if (hr != 0) { 
         hr2message(__LINE__,hr,"g_pDev->SetVertexShader(directionallight bump Vertexshader)");
         return false;
      }
   }

   hr = g_pDev->SetVertexDeclaration(g_pvdeclPSXVertex);
   if (hr != 0) { 
	   hr2message(__LINE__,hr, "g_pDev->SetVertexDeclaration(PSXVERTEX)");
	   return false;
   }
   */
   // -- bumpshader verwenden für die enemies und den Player
   //
    if (g_arr_EnemyDrawer[EDRAW_RES_BUMP].m_cntDraw > 0)
   {
      ENEMYDRAWER* l_pedrawThis = g_arr_EnemyDrawer+EDRAW_RES_BUMP;
	  for (int l_idxEnemyArray = 0; l_idxEnemyArray < l_pedrawThis->m_cntDraw; l_idxEnemyArray++)
	  {		
		  if (l_pedrawThis->prepareHeader()==false) return false;

		  if (g_bWriteLogFileInLoop) 
		  {
			  char l_cResType[100];

			  PSXPrintf(__LINE__,l_cResType, sizeof(l_cResType), "BumpDrawer für Restype no %i\n", int(l_idxEnemyArray));
			  WriteLogFile(l_cResType);
		  }

		  ENEMY* l_penemyCurr = g_arr_pEnemy[l_pedrawThis->m_idxFirst + l_idxEnemyArray];
		  if (l_penemyCurr)
		  {		  			  
			  
			  //XMVECTOR l_vPos = XMVectorZero();
			  //XMMATRIX l_matLeader = XMMatrixIdentity();

			  RESTYPE l_resType = l_penemyCurr->getRes();
			  if (l_pedrawThis->prepareItem(
                    l_penemyCurr->getPos(), //l_vPos, 
                    l_penemyCurr->getMatrixPointer(), //&g_World, 
                    1.0) == false)
			  {
				  return false;
			  }
   
			  if (l_pedrawThis->setPixelShader() == false) return false;
			  if (l_pedrawThis->Draw(l_resType) == false) return false;

		  }

	  }
      if (g_bWriteLogFileInLoop) 
      {
         WriteLogFile("player cobra and other non-enemy singleton objects\n");
      }

      //if (g_Player.getGS()->displayNonEnemy(g_edrawResBumpArray) == false) return false;
         
   } // ende bumpshader section
   

	/*
   hr = g_pDev->SetPixelShader( NULL );
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetPixelShader(DOT3Bump Pixelshader off)");
      return false;
   }

   // -- reset Adressmode in illumination stage
   //
   {
      PREPARE_TEXTURESAMPLER_DATA l_ptdResetStage3[] = {
         { 2, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP },
         { 2, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP },
         { 3, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP },
         { 3, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP },
      };

      if (PSXSetTextureSamplerState(l_ptdResetStage3, 
                             sizeof(l_ptdResetStage3)/
                             sizeof(l_ptdResetStage3[0])) == false)
      { 
         return (false);
      }
   }
   
   // --- Sternenstaub in Form einer Pointlist zeichnen
   //
   if (g_bWriteLogFileInLoop) 
   {
      WriteLogFile("draw stardust as pointlist\n");
   }

   hr = g_pDev->SetTexture(0, NULL);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetTexture(NULL)");
      return(FALSE);
   }

   // -- Sternenstaub zeichnen
   //
   hr = g_pDev->SetVertexShader( g_hStarDustVertexShader);
   if (hr != 0) { 
      hr2message(__LINE__,hr, "g_pDev->SetVertexShader(StartDustVertShader)");
      return false;
   }

   hr = g_pDev->SetVertexDeclaration(g_pvdeclPSXVector);
   if (hr != 0) { 
      hr2message(__LINE__,hr, "g_pDev->SetVertexDeclaration(PSXVECTOR)");
      return false;
   }

   {
      PSXWVECTOR l_vwConstantArray[10];
      D3DMATRIX l_matWorldViewProj;
 
      memcpy (&l_matWorldViewProj,&g_matIdentity, sizeof(g_matIdentity));
      l_matWorldViewProj._41 = g_vCamera.x;
      l_matWorldViewProj._42 = g_vCamera.y;
      l_matWorldViewProj._43 = g_vCamera.z;
      matmult(&l_matWorldViewProj, &l_matWorldViewProj, &g_matViewProj);
 
      if (PSXCopyTranspose((D3DMATRIX*)l_vwConstantArray, &l_matWorldViewProj) == false)
      {
         return false;
      }

      l_vwConstantArray[4].setFloat( 100.0f, 100.0f, 100.0f, 100.0f); // diffusecolor
      l_vwConstantArray[5].setFloat( 100.0f, 100.0f, 100.0f, 100.0f); // diffusecolor
      l_vwConstantArray[6].setFloat( 1.0f, 1.0f, 1.0f, 1.0f); // diffusecolor

      g_pDev->SetVertexShaderConstantF(0,(const float*)l_vwConstantArray,10);
      if (hr != 0) { 
         hr2message(__LINE__,hr,"g_pDev->SetVertexShaderConstantF(l_vwConstantArray)");
         return false; 
      }
   }


   hr = g_pDev->SetStreamSource(0, g_lpD3DStarDustVB, 0, sizeof(PSXVECTOR));
   if (hr != 0) { 
      hr2message(__LINE__,hr, "g_pDev->SetStreamSource(Starfield)");
      return false;
   }

   hr = g_pDev->DrawPrimitive( D3DPT_POINTLIST, 0, g_cntStarDustDots);
   if (hr != 0) { 
      hr2message(__LINE__,hr, "g_pDev->DrawPrimitive(Starfield)");
      return false;
   }


   // --- Asteroiden: Vertexshader zur Versorgung mit den Environmapkoordinaten
   //
   if (g_bWriteLogFileInLoop) 
   {
      WriteLogFile("draw Asteroids from enemy-pointerarray\n");
   }

   hr = g_pDev->SetVertexShader( g_hAsteroidVertexShader );
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetVertexShader(ASTEROID-SHADER)");
      return false;
   }

   hr = g_pDev->SetVertexDeclaration(g_pvdeclPSXVertex);
   if (hr != 0) { 
	   hr2message(__LINE__,hr, "g_pDev->SetVertexDeclaration(PSXVERTEX)");
	   return false;
   }

   {
      IDXENEMYDRAWER l_idxDrawerArray[] = { EDRAW_ASTEROID,
                                            EDRAW_ASTEROID_EMISSIVE,
                                            EDRAW_ASTEROID_BUMP };
      int k;
  
      PSXWVECTOR l_vwConstantArray[18];

      if (PSXCopyTranspose((D3DMATRIX*)(l_vwConstantArray+4), &g_matViewProj) == false)
      {
         return false;
      }

      l_vwConstantArray[8].setVector(&(g_d3dExteriorLights[0].Direction),1.0f);
      //l_vwConstantArray[9].setVector(&(g_d3dExteriorLights[1].Direction),1.0f);

      l_vwConstantArray[10].setFloat( g_d3dExteriorLights[0].Diffuse.r,
                                      g_d3dExteriorLights[0].Diffuse.g,
                                      g_d3dExteriorLights[0].Diffuse.b,
                                      1.0f); // diffusecolor

      l_vwConstantArray[11].setFloat( 0.002f, 0.002f, 1.0f, 1.0f); // texcoordscale
      l_vwConstantArray[12].setFloat( 0.5f, 0.5f, 1.0f, 1.0f); // middle for texturecoord and color
      l_vwConstantArray[13].setFloat( 1.0f, 1.0f, 1.0f, 1.0f); // max for texturecoord and color
      l_vwConstantArray[14].setFloat( 0.0f, 0.0f, 0.0f, 0.0f); // min for texturecoord and color

      l_vwConstantArray[15].setFloat( 0.0001f, 0.0001f, 0.0001f, 0.0001f); // fogscale

      l_vwConstantArray[16].setFloat( g_Player.m_pLevel->m_d3dcolPlanet.r,
                                      g_Player.m_pLevel->m_d3dcolPlanet.g,
                                      g_Player.m_pLevel->m_d3dcolPlanet.b,
                                      1.0f); // diffusecolor

      g_pDev->SetVertexShaderConstantF(0,(const float*) l_vwConstantArray,17);
      if (hr != 0) { 
         hr2message(__LINE__,hr,"g_pDev->SetVertexShaderConstantF(l_vwConstantArray)");
         return false; 
      }

      // -- fuer alle LOD-Stufen des Asteroiden nachsehen
      //
      for (k = 0;k < 3; k++)
      {
         int l_idxEnemy, l_ResType;
   
         // -- gibt keine enemies dazu => auslassen
         //
         if (g_arr_EnemyDrawer[l_idxDrawerArray[k]].m_cntDraw == 0) continue;
         l_ResType = g_arr_EnemyDrawer[l_idxDrawerArray[k]].m_ResType;
  
         if (g_bWriteLogFileInLoop) 
         {
            char logtext[200];
            PSXPrintf(__LINE__,logtext, sizeof(logtext), "applying enemy-drawer no %i, %s\n",
                    l_idxDrawerArray[k], g_arr_cEnemyDrawerName[l_idxDrawerArray[k]]);
            WriteLogFile(logtext);
         }
   

         // --- prepare Vertex/Indices Input Stream
         //
         if (g_meshresManager.setIndices((RESTYPE)l_ResType) == false) return false;
         if (g_meshresManager.setStreamSource(0, (RESTYPE)l_ResType) == false) return false;

   
         // -- Material setzen
         //
         //l_bMaterialDirty = false;
      
         if (g_bWriteLogFileInLoop) 
         {
            WriteLogFile("turning on appropriate textures\n");
         }

         // -- Texturen einschalten
         //
         if (g_arr_EnemyDrawer[l_idxDrawerArray[k]].PrepareStages(0)==false) 
         { 
            return false; 
         }

         for (l_idxEnemy = 0; 
              l_idxEnemy < g_arr_EnemyDrawer[l_idxDrawerArray[k]].m_cntDraw; 
              l_idxEnemy ++)
         {
            ENEMY* l_penemyCurr;
   
   
            {
               int l_idxDraw;
               l_idxDraw = g_arr_EnemyDrawer[l_idxDrawerArray[k]].m_idxFirst + l_idxEnemy;
   
               l_penemyCurr = g_arr_pEnemy[l_idxDraw];
   
            }
   
            if (g_bWriteLogFileInLoop) 
            {
               WriteLogFile("set transformation ...\n");
            }

            // -- Rotation / Position uebergeben
            //
            if (PSXCopyTranspose(&g_matGeneric2, l_penemyCurr->getMatrixPointer())==false)
            {
               return false;
            }
            g_matGeneric2._14 = l_penemyCurr->getPosX();
            g_matGeneric2._24 = l_penemyCurr->getPosY();
            g_matGeneric2._34 = l_penemyCurr->getPosZ();

            hr = g_pDev->SetVertexShaderConstantF(0, (const float*)&g_matGeneric2, 4);
            if (hr != 0) { 
               hr2message(__LINE__,hr,"g_pDev->SetVertexShaderConstantF(matEnemy)");
               return(FALSE); 
            }

            if (g_bWriteLogFileInLoop) 
            {
               WriteLogFile("set scale ...\n");
            }

            // -- Skalierung einsetzen
            //
            l_vwConstantArray[17].setFloat( l_penemyCurr->m_fScale, 
                                            l_penemyCurr->m_fScale, 
                                            l_penemyCurr->m_fScale, 
                                            1.0f);

            g_pDev->SetVertexShaderConstantF(17,(const float*)(l_vwConstantArray+17),1);
            if (hr != 0) { 
               hr2message(__LINE__,hr,"g_pDev->SetVertexShaderConstantF(ENEMY::m_fScale)");
               return(FALSE); 
            }

            if (g_bWriteLogFileInLoop) 
            {
               WriteLogFile("finally: lets draw...\n");
            }
            hr = g_pDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
                                              g_meshresManager.getVertexCount((RESTYPE)l_ResType), 0, 
                                              g_meshresManager.getFaceCount((RESTYPE)l_ResType));
            if (hr != 0)
            { 
               hr2message(__LINE__,hr, "g_pDev->DrawIndexedPrimitive(ASTEROID-x)");
               return false;
            }
   
         } // end-for idxEnemy l_idxRes
      }
   }

   hr = g_pDev->SetVertexShader( NULL);
   if (hr != 0) { 
      hr2message(__LINE__,hr, "g_pDev->SetVertexShader(NULL)");
      return false;
   }

   hr = g_pDev->SetVertexDeclaration(g_pvdeclPSXVertex);
   if (hr != 0) { 
	   hr2message(__LINE__,hr, "g_pDev->SetVertexDeclaration(PSXVERTEX)");
	   return false;
   }

   // -- Set Texture Stages (emissive) 
   //
   {
      PREPARE_TSS_DATA l_ptdFlagship[] = {
         { 0, D3DTSS_COLOROP, D3DTOP_MODULATE },
         { 0, D3DTSS_COLORARG1, D3DTA_TEXTURE },
         { 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE },
         { 0, D3DTSS_TEXCOORDINDEX, 0 },
         { 1, D3DTSS_TEXCOORDINDEX, 1 },
         { 2, D3DTSS_TEXCOORDINDEX, 2 },
         { 3, D3DTSS_TEXCOORDINDEX, 3 },
         { 4, D3DTSS_TEXCOORDINDEX, 4 },
         { 5, D3DTSS_TEXCOORDINDEX, 5 },
         { 6, D3DTSS_TEXCOORDINDEX, 6 },
         { 7, D3DTSS_TEXCOORDINDEX, 7 },
         { 1, D3DTSS_COLOROP, D3DTOP_ADD}, 
         { 1, D3DTSS_COLORARG1, D3DTA_CURRENT },
         { 1, D3DTSS_COLORARG2, D3DTA_TEXTURE },
         { 2, D3DTSS_COLOROP, D3DTOP_DISABLE },
         { 3, D3DTSS_COLOROP, D3DTOP_DISABLE },
         { 4, D3DTSS_COLOROP, D3DTOP_DISABLE }
      };

      bool l_bSucceed;
      l_bSucceed = PSXSetTextureStageState(l_ptdFlagship, 
                                           sizeof(l_ptdFlagship)/
                                           sizeof(PREPARE_TSS_DATA));
      if (l_bSucceed==FALSE)
      { 
         return (false);
      }

   }

   // --- mipmapping ON
   //
   if ( PSXSetMipMapping(true, 0) == false )
   { 
      return false;
   }

   if ( PSXSetMipMapping(true, 1) == false )
   { 
      return false;
   }


//    schleife die zunaechst delta zwischen vPlayerInFlagshipSpace und Face
//    festellt und danach Winkel zwischen delta und Facenormal :
//    vView = vPlayerInFlagFlyerSpace - vFacepos;
//    fViewLength = sqrt(pow(vView.x,2)+pow(vView.y,2)+pow(vView.z,2));
//    fCosAlpha = (vView.x * vnormal.x + 
//                 vView.y * vnormal.y + 
//                 vView.z * vnormal.z) / 
//              ( fViewLength * (1==FaceNormalLength) );
//
//    hier heisst es also vView*vNormal  (* bedeutet dotproduct)
//    bzw. (vPlayerInFlagFlyerSpace - vFacepos) * vNormal;
//    dies lies sich optimieren: vPlayerInFlagFlyerSpace * vNormal - 
//                               vFacepos * vNormal
//    letzteres product laesst sich precalculaten und ersteres   
//    am Anfang der Schleife fuers ganze Flagschiff
//
//
//   {
//      float fPlayerLength;
//      PSXVECTOR l_vPlayerL1, l_vPlayerToFFlyer;
//      PSXFACE* l_pfaceFiltered, *l_pfaceSrc;
//      int l_cntSrc, l_cntFiltered;
//
//      if (g_bWriteLogFileInLoop) 
//      {
//         WriteLogFile("frontend-culling flagship faces\n");
//      }
//
//      l_vPlayerToFFlyer.x = (g_Player.m_vPos.x-g_levelContent.m_vPos.x);
//      l_vPlayerToFFlyer.y = (g_Player.m_vPos.y-g_levelContent.m_vPos.y);
//      l_vPlayerToFFlyer.z = (g_Player.m_vPos.z-g_levelContent.m_vPos.z);
//
//      fPlayerLength = sqrt(pow(l_vPlayerToFFlyer.x,2)+
//                           pow(l_vPlayerToFFlyer.y,2)+
//                           pow(l_vPlayerToFFlyer.z,2));
//
//      l_vPlayerL1.x = l_vPlayerToFFlyer.x / fPlayerLength;
//      l_vPlayerL1.y = l_vPlayerToFFlyer.y / fPlayerLength;
//      l_vPlayerL1.z = l_vPlayerToFFlyer.z / fPlayerLength;
//
//      l_pfaceSrc = (PSXFACE*)resourceoffsets[RESTYPE_FLAGFLYER].face_offset, 
//      l_cntSrc   = g_meshresManager.getFaceCount(RESTYPE_FLAGFLYER);
//
//      hr = g_lpD3DIB[RESTYPE_FLAGFLYER]->Lock(0,0,
//                                       (unsigned char**) &l_pfaceFiltered,
//                                       D3DLOCK_DISCARD|
//                                       D3DLOCK_NOSYSLOCK);
//      if (hr != 0) { 
//         hr2message(__LINE__,hr,"g_lpD3DIB[FLAGFLYER]->Lock");
//         return false;
//      }
//   
//      l_cntFiltered = 0;
//      for (int i = 0; i < l_cntSrc; i++)
//      {
//         float fDotAll, fCos;
//         float fDotPlayer = (l_vPlayerL1.x * g_faceinfArray[i].vNormal.x + 
//                             l_vPlayerL1.y * g_faceinfArray[i].vNormal.y + 
//                             l_vPlayerL1.z * g_faceinfArray[i].vNormal.z);
//
//         if (fDotPlayer > 0.0f)
//         {
//            l_pfaceFiltered[l_cntFiltered] = l_pfaceSrc[i];
//            l_cntFiltered++;
//         }
//         //fDotAll = g_faceinfArray[i].fDot - fDotPlayer;
//         //fDotAll = fDotPlayer;
//         //if (fDotPlayer < 0.0f) fDotPlayer = (-fDotAll);
//      }
//
//      hr = g_lpD3DIB[RESTYPE_FLAGFLYER]->Unlock();
//      if (hr != 0) { 
//         hr2message(__LINE__,hr,"g_lpD3DIB[FLAGFLYER]->Unlock");
//         return false;
//      }
//
//      if (g_bWriteLogFileInLoop) 
//      {
//         WriteLogFile("drawing flagship\n");
//      }
//   
//      hr = g_pDev->SetIndices( g_lpD3DIB[RESTYPE_FLAGFLYER],0);
//      if (hr != 0) { 
//            hr2message(__LINE__,hr,"g_pDev->SetIndices()");
//         return false;
//      }
//
//      // -- Draw whole object materialindependent
//      //
//      hr = g_pDev->DrawIndexedPrimitive(  
//                D3DPT_TRIANGLELIST, 
//                0, g_meshresManager.getVertexCount(RESTYPE_FLAGFLYER), 
//                0, l_cntFiltered);
//
//      if (hr != 0) { 
//         hr2message(__LINE__,hr,"g_pDev->"
//                                  "DrawIndexedPrimitive(Flagship)");
//         return false;
//      }
//
//   }

   // -- get back to default texture stages (no emissive/bump)
   //
   {
      PREPARE_TSS_DATA l_Restauration[] = {
         { 0, D3DTSS_COLOROP, D3DTOP_MODULATE },
         { 0, D3DTSS_COLORARG1, D3DTA_TEXTURE },
         { 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE },
         { 1, D3DTSS_COLOROP, D3DTOP_DISABLE },
         { 2, D3DTSS_COLOROP, D3DTOP_DISABLE }
      };
      
      bool l_bSucceed;
      l_bSucceed = PSXSetTextureStageState(l_Restauration, 
                                           sizeof(l_Restauration)/
                                           sizeof(PREPARE_TSS_DATA));
      if (l_bSucceed==FALSE)
      { 
         return false;
      }
   }

   // -- Saemtliche Flagshipbeleuchtung aus
   //
   {  
      int l_idxAllLights;
      for (l_idxAllLights=0; l_idxAllLights<8; l_idxAllLights++)
      {
         hr = g_pDev->LightEnable(l_idxAllLights,FALSE);
         if (hr != 0) { 
            hr2message(__LINE__,hr,"g_pDev->LightEnable(ExteriorLights, false)");
            return false;
         }
      }
   }


   if (g_bWriteLogFileInLoop) 
   {
      WriteLogFile("setting tranparency\n");
   }

   // --- Ab hier Transparenz: schuesse, lensflares, explosionen
   //
   hr = g_pDev->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->"
                               "SetRenderState(ALPHAEBLEND)");
      return false;
   }

   hr = g_pDev->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->"
                               "SetRenderState(Z-DISABLE)");
      return false;
   }


   hr = g_pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
   if (hr != 0) { 
      hr2message(__LINE__,hr,
                "g_pDev->SetRenderState(SRCBLEND)"); 
      return(FALSE);
   };

   hr = g_pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
   if (hr != 0) { 
      hr2message(__LINE__,hr,
               "g_pDev->SetRenderState(SRCBLEND)");
      return(FALSE);
   };

   hr = g_pDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD); 
   if (hr != 0) { 
       hr2message(__LINE__,hr,
                 "g_pDev->SetRenderState(BLENDOP)");
       return(FALSE);
   };

   hr = g_pDev->SetRenderState( D3DRS_CULLMODE,D3DCULL_NONE); 
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->"
                               "SetRenderState(CULLMODE)");
      return(FALSE);
   } 

   if (g_bWriteLogFileInLoop) 
   {
      WriteLogFile("set exterior lights\n");
   }

   if (g_Player.m_pLevel->m_bDayLight)
   {
      int l_idxExteriorLightsOn;

      for (l_idxExteriorLightsOn=0;
           l_idxExteriorLightsOn<g_cntExteriorLights;
           l_idxExteriorLightsOn++)
      {
         hr = g_pDev->SetLight(l_idxExteriorLightsOn,
                               &g_d3dExteriorLights[l_idxExteriorLightsOn]);
         if (hr != 0) { 
            hr2message(__LINE__,hr,"g_pDev->SetLight(ExteriorLight)");
            return false;
         }
         hr = g_pDev->LightEnable(l_idxExteriorLightsOn,TRUE);
         if (hr != 0) { 
            hr2message(__LINE__,hr,"g_pDev->LightEnable(ExteriorLight)");
            return false;
         }
      }
   }

   if (g_bWriteLogFileInLoop) 
   {
      WriteLogFile("planet corona\n");
   }

   // -- Atmospehre um den Planeten zeichnen
   //   
    if((g_Player.m_pLevel->m_d3dcolPlanet.r +
        g_Player.m_pLevel->m_d3dcolPlanet.g +
        g_Player.m_pLevel->m_d3dcolPlanet.b) > 0.0f)      
   {
      D3DMATERIAL9 l_D3DMaterial;

      float l_fScale;
 
      if (g_bEnableTextures)
      {
         if (g_txtArray[TID_CORONA].activate(0) == false) return false;
      }

      hr = g_pDev->SetIndices( g_lpD3DBillbIB);
      if (hr != 0) { 
         hr2message(__LINE__,hr, "g_pDev->SetIndices(atmosphere)");
         return false;
      }

      hr = g_pDev->SetVertexShader( NULL);
      if (hr != 0) { 
         hr2message(__LINE__,hr, "g_pDev->SetVertexShader(atmosphere)");
         return false;
      }

	  hr = g_pDev->SetVertexDeclaration(g_pvdeclPSXVertex);
	  if (hr != 0) { 
		  hr2message(__LINE__,hr, "g_pDev->SetVertexDeclaration(PSXVERTEX)");
		  return false;
	  }

      hr = g_pDev->SetStreamSource(0, g_lpD3DBillbVB2, 0, sizeof(PSXVERTEX));
      if (hr != 0) { 
         hr2message(__LINE__,hr, "g_pDev->SetStreamSource(atmosphere)");
         return false;
      }

      memset(&l_D3DMaterial, 0, sizeof(l_D3DMaterial));

      if(g_Player.m_pLevel->m_d3dcolPlanet.r < 0.1f ||
         g_Player.m_pLevel->m_d3dcolPlanet.g < 0.1f ||
         g_Player.m_pLevel->m_d3dcolPlanet.b < 0.1f)
      {
         l_fScale=8.0f;
      }else{
         l_fScale=3.0f;
      }

      l_D3DMaterial.Diffuse.r  = g_Player.m_pLevel->m_d3dcolPlanet.r*l_fScale;
      l_D3DMaterial.Diffuse.g  = g_Player.m_pLevel->m_d3dcolPlanet.g*l_fScale;
      l_D3DMaterial.Diffuse.b  = g_Player.m_pLevel->m_d3dcolPlanet.b*l_fScale;
//      l_D3DMaterial.Ambient  = g_Player.m_pLevel->m_d3dcolPlanet; 
//      l_D3DMaterial.Specular = g_Player.m_pLevel->m_d3dcolPlanet;
//      l_D3DMaterial.Emissive = g_Player.m_pLevel->m_d3dcolPlanet;
 
      l_D3DMaterial.Power    = 1.0f;

      hr = g_pDev->SetMaterial(&l_D3DMaterial);
      if (hr != 0) { 
         hr2message(__LINE__,hr,"g_pDev->SetMaterial(atmosphere)");
         return false;
      }

      memcpy (&g_matGeneric1,&g_matIdentity, sizeof(g_matIdentity));
      g_matGeneric1._11 = cos ( g_fPi/2);
      g_matGeneric1._12 = - sin ( g_fPi/2 );
      g_matGeneric1._21 = sin ( g_fPi/2 );
      g_matGeneric1._22 = cos ( g_fPi/2 );
         
      // --Scaling
//      l_fScale=2500.0f;// /g_Player.m_pLevel->m_fPlanetDistance ;
      l_fScale=1.0f;// /g_Player.m_pLevel->m_fPlanetDistance ;
      g_matGeneric1._11*=l_fScale;
      g_matGeneric1._12*=l_fScale;
      g_matGeneric1._13*=l_fScale;

      g_matGeneric1._21*=l_fScale;
      g_matGeneric1._22*=l_fScale;
      g_matGeneric1._23*=l_fScale;

      g_matGeneric1._31*=l_fScale;
      g_matGeneric1._32*=l_fScale;
      g_matGeneric1._33*=l_fScale;
 
      g_matGeneric1._41 = g_vCam4Astro.x - 500.0f;
      g_matGeneric1._42 = g_vCam4Astro.y - 500.0f;
      g_matGeneric1._43 = g_vCam4Astro.z - g_Player.m_pLevel->m_fPlanetDistance;


         
      hr = g_pDev->SetTransform(D3DTS_WORLD, &g_matGeneric1);
      if (hr != 0) { 
         hr2message(__LINE__,hr,"g_pDev->SetTransform(atmosphere)");
         return false;
      }

      hr = g_pDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,
                                             0,0, 4, 0, 2 ) ;

      if (hr != 0) { 
         hr2message(__LINE__,hr,
                    "g_pDev->DrawIndexedPrimitive(atmosphere)");
         return false;
      }
   }

   {  

      hr = g_pDev->LightEnable(0,FALSE);
      if (hr != 0) { 
         hr2message(__LINE__,hr,"g_pDev->LightEnable(ExteriorLight)");
         return false;
      }
 
   }

   if (g_bWriteLogFileInLoop) 
   {
      WriteLogFile("deflector spheres\n");
   }

   // -- Vertex Shader: Deflector Spheres
   //
   if (g_hFrontShieldVertexShader)
   {
      int l_idxEnemy;
      int l_idxDraw;
      const int l_cntConstants = 13;
      PSXWVECTOR l_vwConstantArray[l_cntConstants];
      int l_idxMatrixRegister;

      if (g_bWriteLogFileInLoop) 
      {
         WriteLogFile("draw mirroring Frontshields\n");
      }

      l_idxMatrixRegister = 4;

      hr = g_pDev->SetVertexShader( g_hFrontShieldVertexShader );
      if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetVertexShader(PSXVERTEX)");
         return false;
      }
	  hr = g_pDev->SetVertexDeclaration(g_pvdeclPSXVertex);
	  if (hr != 0) { 
		  hr2message(__LINE__,hr, "g_pDev->SetVertexDeclaration(PSXVERTEX)");
		  return false;
	  }

      if (g_bEnableTextures)
      {
         if (g_txtArray[TID_ARCNEBULA].activate(0) == false) return false;
      }

      hr = g_pDev->SetIndices( g_lpD3DFrontShieldIB);
      if (hr != 0) { 
         hr2message(__LINE__,hr,"g_pDev->SetIndices(FrontShield)");
         return false;
      }

      // -- Konstanten vorbereiten und blockweise uebertragen
      //
      memset(l_vwConstantArray, 0, sizeof(l_vwConstantArray));

      if (PSXCopyTranspose((D3DMATRIX*)l_vwConstantArray, &g_matViewProj) == false)
      {
         return false;
      }

      l_vwConstantArray[8].setFloat(1.0f, 1.0f, 1.0f, 1.0f);
      l_vwConstantArray[11].setVector(&g_vCamera,0.0f);
      l_vwConstantArray[12].setFloat( 0.5f, 0.5f, 1.0f, -1.0f);

      g_pDev->SetVertexShaderConstantF(0,(const float*)l_vwConstantArray,l_cntConstants);
      if (hr != 0) { 
         hr2message(__LINE__,hr,"g_pDev->SetVertexShaderConstantF(l_vwConstantArray)");
         return false; 
      }

      hr = g_pDev->SetStreamSource(0, g_lpD3DFrontShieldVB, 0, sizeof(PSXVERTEX));
      if (hr != 0) { 
         hr2message(__LINE__,hr,"g_pDev->SetStreamSource(FrontShield)");
         return false;
      }

      for (l_idxEnemy = 0; 
           l_idxEnemy < g_arr_EnemyDrawer[EDRAW_FRONTSHIELD].m_cntDraw; 
           l_idxEnemy ++)
      {
         l_idxDraw = g_arr_EnemyDrawer[EDRAW_FRONTSHIELD].m_idxFirst + l_idxEnemy;

         if (PSXCopyTranspose((D3DMATRIX*) &l_vwConstantArray[l_idxMatrixRegister],
                                           g_arr_pEnemy[l_idxDraw]->getMatrixPointer()) == false)
         {
            return false;
         }


         l_vwConstantArray[9].setFloat(g_arr_pEnemy[l_idxDraw]->getRad(),
                                       g_arr_pEnemy[l_idxDraw]->getRad(),
                                       g_arr_pEnemy[l_idxDraw]->getRad(),
                                       1.0f);
         l_vwConstantArray[10].setVector(&g_arr_pEnemy[l_idxDraw]->getPos(),1.0f);

         hr = g_pDev->SetVertexShaderConstantF(l_idxMatrixRegister, 
                                              (const float*)&l_vwConstantArray[l_idxMatrixRegister], 
                                              11-l_idxMatrixRegister);
         if (hr != 0) { 
            hr2message(__LINE__,hr,"g_pDev->SetVertexShaderConstantF(enemy)");
            return false; 
         }

         hr = g_pDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 
                                            0, 0, g_cntFrontShieldVert, 
                                            0, g_cntFrontShieldFace);
        
         if (hr != 0) { 
            hr2message(__LINE__,hr, "g_pDev->DrawIndexedPrimitive(defsphere)");
            return true;
         }
      } // end-for l_idxEnemy/l_idxDraw (pro EDRAW_FRONTSHIELD)

      // -- der sich bei jedem Enemy veraendernde Teil
      //
      l_idxMatrixRegister = 7;

      // -- volle Sphere einschalten und EDRAW_TRACTORSPHERE behandeln
      //
      hr = g_pDev->SetStreamSource(0, g_lpD3DTractorSphereVB, 0, sizeof(PSXVERTEX));
      if (hr != 0) { 
         hr2message(__LINE__,hr,"g_pDev->SetStreamSource(TractorSphere)");
         return false;
      }

      hr = g_pDev->SetVertexShader( g_hTractorSphereVertexShader );
      if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetVertexShader(PSXVERTEX)");
         return false;
      }
	  hr = g_pDev->SetVertexDeclaration(g_pvdeclPSXVertex);
	  if (hr != 0) { 
		  hr2message(__LINE__,hr, "g_pDev->SetVertexDeclaration(PSXVERTEX)");
		  return false;
	  }

      hr = g_pDev->SetIndices( g_lpD3DTractorSphereIB);
      if (hr != 0) { 
         hr2message(__LINE__,hr,"g_pDev->SetIndices(FrontShield)");
         return false;
      }

      for (l_idxEnemy = 0; 
           l_idxEnemy < g_arr_EnemyDrawer[EDRAW_TRACTORSPHERE].m_cntDraw; 
           l_idxEnemy ++)
      {
         float l_fShieldLuminance;

         l_idxDraw = g_arr_EnemyDrawer[EDRAW_TRACTORSPHERE].m_idxFirst + l_idxEnemy;

         l_fShieldLuminance = (1000.0f - g_arr_pEnemy[l_idxDraw]->getPos2DZ()) / 1000.0f;

         if  ( l_fShieldLuminance > 0.0f)
         {
            l_vwConstantArray[7].setFloat(l_fShieldLuminance,
                                          l_fShieldLuminance,
                                          l_fShieldLuminance,
                                          1.0f);
   
            l_vwConstantArray[9].setFloat(g_arr_pEnemy[l_idxDraw]->getRad() * g_fMineShieldRadScale, 
                                          g_arr_pEnemy[l_idxDraw]->getRad() * g_fMineShieldRadScale, 
                                          g_arr_pEnemy[l_idxDraw]->getRad() * g_fMineShieldRadScale, 
                                          1.0f);
            l_vwConstantArray[10].setVector(&g_arr_pEnemy[l_idxDraw]->getPos(),1.0f);
   
            hr = g_pDev->SetVertexShaderConstantF(l_idxMatrixRegister, 
                                                 (const float*)&l_vwConstantArray[l_idxMatrixRegister], 
                                                 11-l_idxMatrixRegister);
            if (hr != 0) { 
               hr2message(__LINE__,hr,"g_pDev->SetVertexShaderConstantF(enemy)");
               return false; 
            }
   
            hr = g_pDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0,
                                               0, g_cntTractorSphereVert, 
                                               0, g_cntTractorSphereFace);
           
            if (hr != 0) { 
               hr2message(__LINE__,hr, "g_pDev->DrawIndexedPrimitive(defsphere)");
               return true;
            }
         } // end-if luminance > 0
      } // end-for l_idxEnemy/l_idxDraw (pro EDRAW_TRACTORSPHERE)


   } // if hFrontShieldVertexShader


   if (g_bWriteLogFileInLoop) 
   {
      WriteLogFile("plasma\n");
   }

   // -- Zeichnen der StarBase 1 Grenz-Plane oder StarBase 2 Schutzsphere des Flagschiffs
   //
   if (g_cntShowPlasmaTimer && g_bEnableVertexShader && g_bEnablePixelShader)
   {

      float l_fScaleFlash;

      PSXVECTOR l_vPlasmaBarrier;

      // -- Hauptbedingungen
      //

      l_fScaleFlash = 50000.0f;
      l_vPlasmaBarrier.x = 0.0f;
      l_vPlasmaBarrier.y = 0.0f;
      l_vPlasmaBarrier.z = 0.0f;

      // StarBase checken um die richtige Position fuer die Barriere
      // --- Auf Weiss schalten
      //
      memset(&g_D3DMaterial, 0, sizeof(g_D3DMaterial));
      g_D3DMaterial.Diffuse  = g_d3dcolWhite;
      g_D3DMaterial.Ambient  = g_d3dcolWhite; 
      g_D3DMaterial.Specular = g_d3dcolWhite;
      g_D3DMaterial.Emissive = g_d3dcolWhite;
      g_D3DMaterial.Power    = 10.0f;
   
      hr = g_pDev->SetMaterial(&g_D3DMaterial);
      if (hr != 0) { 
         hr2message(__LINE__,hr,"g_pDev->SetMaterial(white)");
         return false;
      }

      // -- nutzt den grad eingestellten billboard zustand
      //
      {
         int k;

         for (k = 0; k < 4; k++)
         {
            if (g_txtArray[TID_SMOKE].activate(k) == false) return false;
         }

      }
 
      memcpy (&g_matGeneric1,&g_matIdentity, sizeof(g_matIdentity));
      g_matGeneric1._11 = l_fScaleFlash;
      g_matGeneric1._22 = l_fScaleFlash;
      g_matGeneric1._33 = l_fScaleFlash;
      g_matGeneric1._41 = l_vPlasmaBarrier.x;
      g_matGeneric1._42 = l_vPlasmaBarrier.y;
      g_matGeneric1._43 = l_vPlasmaBarrier.z;
       
      hr = g_pDev->SetTransform(D3DTS_WORLD, &g_matGeneric1);
      if (hr != 0) { 
         hr2message(__LINE__,hr,"g_pDev->SetTransform(PlasmaBarrier)");
         return(false);
      }

      // --- prepare Vertex/Indices Input Stream
      //

      hr = g_pDev->SetVertexShader( g_hPlasmaVertexShader );
      if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetVertexShader(PSXVERTEX)");
         return false;
      }
	  hr = g_pDev->SetVertexDeclaration(g_pvdeclPSXVertex);
	  if (hr != 0) { 
		  hr2message(__LINE__,hr, "g_pDev->SetVertexDeclaration(PSXVERTEX)");
		  return false;
	  }
      hr = g_pDev->SetPixelShader(g_hPlasmaPixelShader);
      if (hr != 0) { 
         hr2message(__LINE__,hr,"g_pDev->SetPixelShader(PlasmaEnergybarrier)");
         return(false);
      }

      {
         float l_fPlasmaLuminance;
         const int l_cntConstants = 20;
         int l_idxPass, l_cntPass;
         PSXWVECTOR l_vwConstantArray[l_cntConstants];
         PSXWVECTOR l_vwColorMask;

         if (g_Player.m_bEntered)
         {
            l_cntPass = 1;
         }else{
            l_cntPass = 3;
         }

         // -- Konstanten vorbereiten und blockweise uebertragen
         //
         memset(l_vwConstantArray, 0, sizeof(l_vwConstantArray));
         if (PSXCopyTranspose((D3DMATRIX*)l_vwConstantArray, &g_matViewProj) == false)
         {
            return false;
         }

         if (PSXCopyTranspose((D3DMATRIX*)l_vwConstantArray+4, &g_matIdentity) == false)
         {
            return false;
         }
   
         l_fPlasmaLuminance = float(g_cntShowPlasmaTimer) / float(g_maxShowPlasmaTimer);

         l_vwConstantArray[7].setFloat(l_fPlasmaLuminance,
                                       l_fPlasmaLuminance,
                                       l_fPlasmaLuminance,
                                       1.0f);

         l_vwConstantArray[8].setFloat(1.0f, 1.0f, 1.0f, 1.0f);
  
         l_vwConstantArray[9].setFloat(l_fScaleFlash,
                                       l_fScaleFlash,
                                       l_fScaleFlash,
                                       1.0f);

         l_vwConstantArray[10].setVector(&l_vPlasmaBarrier,1.0f);
         l_vwConstantArray[11].setVector(&g_vCamera,0.0f);

         g_pDev->SetVertexShaderConstantF(0,(const float*)l_vwConstantArray,l_cntConstants);
         if (hr != 0) { 
            hr2message(__LINE__,hr,"g_pDev->SetVertexShaderConstantF(l_vwConstantArray)");
            return false; 
         }

         if (g_cntPlasmaShift < (l_cntPass * 16))
         {
            hr2message(__LINE__,hr,"too less Values in PlasmaShiftArray");
            return(false);
         }

         for (l_idxPass = 0; l_idxPass < l_cntPass; l_idxPass++)
         {
            l_vwConstantArray[12].setFloat( sin(g_fPlasmaShift[l_idxPass*16+2]*2.0f*g_fPi)*10.0f,
                                            sin(g_fPlasmaShift[l_idxPass*16+3]*2.0f*g_fPi)*10.0f,
                                            sin(g_fPlasmaShift[l_idxPass*16+6]*2.0f*g_fPi)*10.0f,
                                            sin(g_fPlasmaShift[l_idxPass*16+7]*2.0f*g_fPi)*10.0f);

            l_vwConstantArray[13].setFloat( sin(g_fPlasmaShift[l_idxPass*16+0]*2.0f*g_fPi)/2.0f,
                                            sin(g_fPlasmaShift[l_idxPass*16+1]*2.0f*g_fPi)/2.0f,
                                            sin(g_fPlasmaShift[l_idxPass*16+4]*2.0f*g_fPi)/2.0f,
                                            sin(g_fPlasmaShift[l_idxPass*16+5]*2.0f*g_fPi)/2.0f);
   
            l_vwConstantArray[14].setFloat( sin(g_fPlasmaShift[l_idxPass*16+8]*2.0f*g_fPi)*5.0f,
                                            sin(g_fPlasmaShift[l_idxPass*16+9]*2.0f*g_fPi)*5.0f,
                                            sin(g_fPlasmaShift[l_idxPass*16+12]*2.0f*g_fPi)*5.0f,
                                            sin(g_fPlasmaShift[l_idxPass*16+13]*2.0f*g_fPi)*5.0f);
   
            l_vwConstantArray[15].setFloat( sin(g_fPlasmaShift[l_idxPass*16+10]*2.0f*g_fPi)/2.0f,
                                            sin(g_fPlasmaShift[l_idxPass*16+11]*2.0f*g_fPi)/2.0f,
                                            sin(g_fPlasmaShift[l_idxPass*16+14]*2.0f*g_fPi)/2.0f,
                                            sin(g_fPlasmaShift[l_idxPass*16+15]*2.0f*g_fPi)/2.0f);
   
            g_pDev->SetVertexShaderConstantF(12,(const float*)l_vwConstantArray+12,4);
            if (hr != 0) { 
               hr2message(__LINE__,hr,"g_pDev->SetVertexShaderConstantF(l_vwConstantArray)");
               return false; 
            }
   
            switch (l_idxPass)
            {
                case 0: l_vwColorMask.setFloat(0.0f, l_fPlasmaLuminance, 0.0f, 0.0f); break;
                case 1: l_vwColorMask.setFloat(l_fPlasmaLuminance, 0.0f, 0.0f, 0.0f); break;
                case 2: l_vwColorMask.setFloat(0.0f, 0.0f, l_fPlasmaLuminance, 0.0f); break;
                default: 
                   hr2message(__LINE__,-1,"too many Plasma rendering passes");
                   return false; 
            }

            l_vwColorMask.setFloat(1.0f, 1.0f, 1.0f, 0.0f);
            g_pDev->SetPixelShaderConstantF(0,(const float*)&l_vwColorMask,1);
            if (hr != 0) { 
               hr2message(__LINE__,hr,"g_pDev->SetVertexShaderConstantF(l_vwConstantArray)");
               return false; 
            }

         }
   
         hr = g_pDev->SetPixelShader(NULL);
         if (hr != 0) { 
            hr2message(__LINE__,hr,"g_pDev->SetPixelShader(PlasmaEnergybarrier)");
            return(false);
         }
      }
   }      

   hr = g_pDev->SetVertexShader( NULL);
   if (hr != 0) { 
      hr2message(__LINE__,hr, "g_pDev->SetVertexShader(Billboardloop)");
      return false;
   }

   hr = g_pDev->SetVertexDeclaration(g_pvdeclPSXVertex);
   if (hr != 0) { 
	   hr2message(__LINE__,hr, "g_pDev->SetVertexDeclaration(PSXVERTEX)");
	   return false;
   }

   hr = g_pDev->SetRenderState( D3DRS_CULLMODE,D3DCULL_NONE); 
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->"
                               "SetRenderState(CULLMODE)");
      return(FALSE);
   } 

   hr = g_pDev->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetRenderState(ZWRITEENABLE off)");
      return(FALSE);
   }


   //  -- Powerup pointer
   //
   {

      int   l_idxEnemy;

      
      D3DCOLORVALUE l_d3dcolor_test={ 0.0f, 0.0f, 1.0f, 1.0f };
      D3DCOLORVALUE l_d3dcolor_test2 = { 0.1,0.1, 0.1, 0.1},
                    l_d3dcolor_test3 = { 0.05,0.05, 0.05, 0.1};

      D3DMATRIX l_matPointer;

      // Rotation um die x-Achse
      D3DMATRIX matMoveX = {
         float(1.0), float(0.0),         float(0.0),        float(0.0),
         float(0.0), float( cos( g_fPi/2)),  float(-sin( g_fPi/2)), float(0.0),
         float(0.0), float( sin( g_fPi/2)),  float( cos( g_fPi/2)), float(0.0),
         float(0.0), float(0.0),         float(0.0),        float(1.0)
      };

      // Rotation um die Z-Achse
      D3DMATRIX matMoveZ = {
         float( cos(timeGetTime()/500.0f)), float(sin(timeGetTime()/500.0f)), float(0.0), float(0.0),
         float(-sin(timeGetTime()/500.0f)), float(cos(timeGetTime()/500.0f)), float(0.0), float(0.0),
         float(0.0),                        float(0.0),                       float(1.0), float(0.0),
         float(0.0),                        float(0.0),                       float(0.0), float(1.0)
      };


      // Rotation um die Y-Achse
      D3DMATRIX matMoveY = {
         float(cos(timeGetTime()/10000.0f)), float(0.0), float(-sin(timeGetTime()/10000.0f)), float(0.0),
         float(0.0),                       float(1.0), float(0.0),                        float(0.0),
         float(sin(timeGetTime()/10000.0f)), float(0.0), float( cos(timeGetTime()/10000.0f)),  float(0.0),
         float(0.0),                       float(0.0), float(0.0),                        float(1.0)
      };

      hr = g_pDev->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);
      if (hr != 0) { 
         hr2message(__LINE__,hr,"g_pDev->SetRenderState(ZWRITEENABLE off)");
         return(FALSE);
      }

      for (l_idxEnemy = 0; 
           l_idxEnemy < g_arr_EnemyDrawer[EDRAW_POINTER01].m_cntDraw; 
           l_idxEnemy ++)
      {
         ENEMY* l_penemyCurr;


         {
            int l_idxDraw;
            l_idxDraw = g_arr_EnemyDrawer[EDRAW_POINTER01].m_idxFirst + l_idxEnemy;

            l_penemyCurr = g_arr_pEnemy[l_idxDraw];

         }

//         if (l_penemyCurr->mType != ETYP_EQUIPMENT)
//         {
//            char logtext[80];
//            PSXPrintf (__LINE__,logtext,sizeof(logtext), "alsPointer getarnt: %s",g_cEnemyTypeNameArray[l_penemyCurr->mType] );
//            hr2message(__LINE__,hr,logtext);
//            return false;
//         }

         // --- helligkeit
         //
         {
             memset(&g_D3DMaterial, 0, sizeof(g_D3DMaterial));
             g_D3DMaterial.Diffuse  = g_d3dcolWhite;
             g_D3DMaterial.Ambient  = l_d3dcolor_test2;
             g_D3DMaterial.Specular = g_d3dcolWhite;
             g_D3DMaterial.Emissive = l_d3dcolor_test;
             g_D3DMaterial.Power    = 1.0f;
         }

         hr = g_pDev->SetMaterial(&g_D3DMaterial);
         if (hr != 0) { 
            hr2message(__LINE__,hr,"g_pDev->SetMaterial(explosion)");
            return false;
         }

         hr = g_pDev->SetTexture(0, NULL);
         if (hr != 0) { 
             hr2message(__LINE__,hr,"g_pDev->SetTexture(flagship)");
             return false;
         }

         if (g_meshresManager.setIndices(RESTYPE_POINTER01) == false) return false;
         if (g_meshresManager.setStreamSource(0, RESTYPE_POINTER01) == false) return false;

         // -- reines vorbereiten der matrix
         //

         memcpy (&l_matPointer,&g_matIdentity, sizeof(g_matIdentity));

         //g_fScalePointer = 20.0f;
         l_matPointer._11 = g_fScalePointer * g_matCamera._11;
         l_matPointer._12 = g_fScalePointer * g_matCamera._21;
         l_matPointer._13 = g_fScalePointer * g_matCamera._31;
         l_matPointer._21 = g_fScalePointer * g_matCamera._12;
         l_matPointer._22 = g_fScalePointer * g_matCamera._22;
         l_matPointer._23 = g_fScalePointer * g_matCamera._32;
         l_matPointer._31 = g_fScalePointer * g_matCamera._13;
         l_matPointer._32 = g_fScalePointer * g_matCamera._23;
         l_matPointer._33 = g_fScalePointer * g_matCamera._33;

         l_matPointer._41 = l_penemyCurr->getPosX() + 100.0f * g_matCamera._12;
         l_matPointer._42 = l_penemyCurr->getPosY() + 100.0f * g_matCamera._22;
         l_matPointer._43 = l_penemyCurr->getPosZ() + 100.0f * g_matCamera._32;

         matmult(&l_matPointer, &matMoveX, &l_matPointer);
         matmult(&l_matPointer, &matMoveZ, &l_matPointer);

         hr = g_pDev->SetTransform(D3DTS_WORLD, &l_matPointer);
         if (hr != 0) { 
            hr2message(__LINE__,hr,"g_pDev->SetTransform"
                                     "(Equipment/Pointer)");
            return false;
         }


         hr = g_pDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
                                           g_meshresManager.getVertexCount(RESTYPE_POINTER01), 0, 
                                           g_meshresManager.getFaceCount(RESTYPE_POINTER01));
         if (hr != 0)
         { 
            hr2message(__LINE__,hr, "g_pDev->DrawIndexedPrimitive(POINTER01)");
            return false;
         }

      } // end-for idxEnemy POINTER

      float l_fScaleTmpUp=4.0f;

      for (l_idxEnemy = 0; 
           l_idxEnemy < g_arr_EnemyDrawer[EDRAW_TMPSPEEDUP].m_cntDraw; 
           l_idxEnemy ++)
      {
         ENEMY* l_penemyCurr;


         {
            int l_idxDraw;
            l_idxDraw = g_arr_EnemyDrawer[EDRAW_TMPSPEEDUP].m_idxFirst + l_idxEnemy;

            l_penemyCurr = g_arr_pEnemy[l_idxDraw];

         }

         // --- helligkeit
         //
         {
             memset(&g_D3DMaterial, 0, sizeof(g_D3DMaterial));
             g_D3DMaterial.Diffuse  = g_d3dcolWhite;
             g_D3DMaterial.Ambient  = l_d3dcolor_test2;
             g_D3DMaterial.Specular = g_d3dcolWhite;
             g_D3DMaterial.Emissive = l_d3dcolor_test;
             g_D3DMaterial.Power    = 1.0f;
         }

         hr = g_pDev->SetMaterial(&g_D3DMaterial);
         if (hr != 0) { 
            hr2message(__LINE__,hr,"g_pDev->SetMaterial(explosion)");
            return false;
         }

         hr = g_pDev->SetTexture(0, NULL);
         if (hr != 0) { 
             hr2message(__LINE__,hr,"g_pDev->SetTexture(flagship)");
             return false;
         }

         if (g_meshresManager.setIndices(RESTYPE_TMPSPEEDUP) == false) return false;
         if (g_meshresManager.setStreamSource(0, RESTYPE_TMPSPEEDUP) == false) return false;


         // -- reines vorbereiten der matrix
         //

         memcpy (&l_matPointer,&g_matIdentity, sizeof(g_matIdentity));
         l_matPointer._11 = l_fScaleTmpUp;
         l_matPointer._22 = l_fScaleTmpUp;
         l_matPointer._33 = l_fScaleTmpUp;

         l_matPointer._11 = l_fScaleTmpUp * g_matCamera._11;
         l_matPointer._12 = l_fScaleTmpUp * g_matCamera._21;
         l_matPointer._13 = l_fScaleTmpUp * g_matCamera._31;
         l_matPointer._21 = l_fScaleTmpUp * g_matCamera._12;
         l_matPointer._22 = l_fScaleTmpUp * g_matCamera._22;
         l_matPointer._23 = l_fScaleTmpUp * g_matCamera._32;
         l_matPointer._31 = l_fScaleTmpUp * g_matCamera._13;
         l_matPointer._32 = l_fScaleTmpUp * g_matCamera._23;
         l_matPointer._33 = l_fScaleTmpUp * g_matCamera._33;
         l_matPointer._41 = l_penemyCurr->getPosX();
         l_matPointer._42 = l_penemyCurr->getPosY();
         l_matPointer._43 = l_penemyCurr->getPosZ();

         matmult(&l_matPointer, &matMoveX, &l_matPointer);
//         matmult(&l_matPointer, &matMoveZ, &l_matPointer);

         hr = g_pDev->SetTransform(D3DTS_WORLD, &l_matPointer);
         if (hr != 0) { 
            hr2message(__LINE__,hr,"g_pDev->SetTransform"
                                     "(Equipment/Pointer)");
            return false;
         }


         hr = g_pDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
                                           g_meshresManager.getVertexCount(RESTYPE_TMPSPEEDUP), 0, 
                                           g_meshresManager.getFaceCount(RESTYPE_TMPSPEEDUP));
         if (hr != 0)
         { 
            hr2message(__LINE__,hr, "g_pDev->DrawIndexedPrimitive(POINTER01)");
            return false;
         }

      } // end-for idxEnemy TMPSPEEDUP

      for (l_idxEnemy = 0; 
           l_idxEnemy < g_arr_EnemyDrawer[EDRAW_STARBASEHANGAR].m_cntDraw; 
           l_idxEnemy ++)
      {
         ENEMY* l_penemyCurr;


         {
            int l_idxDraw;
            l_idxDraw = g_arr_EnemyDrawer[EDRAW_STARBASEHANGAR].m_idxFirst + l_idxEnemy;

            l_penemyCurr = g_arr_pEnemy[l_idxDraw];

         }

         // --- helligkeit
         //
         {
             memset(&g_D3DMaterial, 0, sizeof(g_D3DMaterial));
             g_D3DMaterial.Diffuse  = g_d3dcolWhite;
             g_D3DMaterial.Ambient  = l_d3dcolor_test2;
             g_D3DMaterial.Specular = g_d3dcolWhite;
             g_D3DMaterial.Emissive = l_d3dcolor_test;
             g_D3DMaterial.Power    = 1.0f;
         }

         hr = g_pDev->SetMaterial(&g_D3DMaterial);
         if (hr != 0) { 
            hr2message(__LINE__,hr,"g_pDev->SetMaterial(explosion)");
            return false;
         }

         if (g_txtArray[TID_SBHANGAR].activate(0) == false) return false;

         if (g_meshresManager.setIndices(RESTYPE_STARBASEHANGAR) == false) return false;
         if (g_meshresManager.setStreamSource(0, RESTYPE_STARBASEHANGAR) == false) return false;

         // -- reines vorbereiten der matrix
         //
         memcpy (&l_matPointer,l_penemyCurr->getMatrixPointer(), sizeof(g_matIdentity));
         l_matPointer._41 = l_penemyCurr->getPosX();
         l_matPointer._42 = l_penemyCurr->getPosY();
         l_matPointer._43 = l_penemyCurr->getPosZ();

         hr = g_pDev->SetTransform(D3DTS_WORLD, &l_matPointer);
         if (hr != 0) { 
            hr2message(__LINE__,hr,"g_pDev->SetTransform"
                                     "(Equipment/Pointer)");
            return false;
         }

         hr = g_pDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
                                           g_meshresManager.getVertexCount(RESTYPE_STARBASEHANGAR), 0, 
                                           g_meshresManager.getFaceCount(RESTYPE_STARBASEHANGAR));
         if (hr != 0)
         { 
            hr2message(__LINE__,hr, "g_pDev->DrawIndexedPrimitive(POINTER01)");
            return false;
         }

      } // end-for idxEnemy STARBASEHANGAR


      for (l_idxEnemy = 0; 
           l_idxEnemy < g_arr_EnemyDrawer[EDRAW_CHAR].m_cntDraw; 
           l_idxEnemy ++)
      {
         ENEMY* l_penemyCurr;
         RESTYPE l_resType;

         {
            int l_idxDraw;
            l_idxDraw = g_arr_EnemyDrawer[EDRAW_CHAR].m_idxFirst + l_idxEnemy;

            l_penemyCurr = g_arr_pEnemy[l_idxDraw];

         }

         // --- helligkeit
         //
         {
             memset(&g_D3DMaterial, 0, sizeof(g_D3DMaterial));
             g_D3DMaterial.Diffuse  = g_d3dcolGreen;
             g_D3DMaterial.Ambient  = g_d3dcolGreen;
             g_D3DMaterial.Specular = g_d3dcolGreen;
             g_D3DMaterial.Emissive = g_d3dcolGreen;
             g_D3DMaterial.Power    = 1.0f;
         }

         hr = g_pDev->SetMaterial(&g_D3DMaterial);
         if (hr != 0) { 
            hr2message(__LINE__,hr,"g_pDev->SetMaterial(character)");
            return false;
         }

         hr = g_pDev->SetTexture(0, NULL);
         if (hr != 0) { 
             hr2message(__LINE__,hr,"g_pDev->SetTexture(character)");
             return false;
         }

         switch(l_penemyCurr->m_cntGeneric)
         {
            case 'A':
               l_resType = RESTYPE_AG;
               break;

            case 'B':
               l_resType = RESTYPE_BG;
               break;

            case 'C':
               l_resType = RESTYPE_CG;
               break;

            case 'D':
               l_resType = RESTYPE_DG;
               break;

            case 'E':
               l_resType = RESTYPE_EG;
               break;

            case 'F':
               l_resType = RESTYPE_FG;
               break;

            case 'G':
               l_resType = RESTYPE_GG;
               break;

            case 'H':
               l_resType = RESTYPE_HG;
               break;

            case 'I':
               l_resType = RESTYPE_IG;
               break;

            case 'J':
               l_resType = RESTYPE_JG;
               break;

            case 'K':
               l_resType = RESTYPE_KG;
               break;

            case 'L':
               l_resType = RESTYPE_LG;
               break;

            case 'M':
               l_resType = RESTYPE_MG;
               break;

            case 'N':
               l_resType = RESTYPE_NG;
               break;

            case 'O':
               l_resType = RESTYPE_OG;
               break;

            case 'P':
               l_resType = RESTYPE_PG;
               break;

            case 'Q':
               l_resType = RESTYPE_QG;
               break;

            case 'R':
               l_resType = RESTYPE_RG;
               break;

            case 'S':
               l_resType = RESTYPE_SG;
               break;

            case 'T':
               l_resType = RESTYPE_TG;
               break;

            case 'U':
               l_resType = RESTYPE_UG;
               break;

            case 'V':
               l_resType = RESTYPE_VG;
               break;

            case 'W':
               l_resType = RESTYPE_WG;
               break;

            case 'X':
               l_resType = RESTYPE_XG;
               break;

            case 'Y':
               l_resType = RESTYPE_YG;
               break;

            case 'Z':
               l_resType = RESTYPE_ZG;
               break;

            case 32:
               l_resType = RESTYPE_SPACE;
               break;

            case 27:
               l_resType = RESTYPE_BACKSPACE;
               break;

            case 28:
               l_resType = RESTYPE_ENTER;
               break;
         }

         if (g_meshresManager.setIndices(l_resType) == false) return false;
         if (g_meshresManager.setStreamSource(0, l_resType) == false) return false;


         // -- reines vorbereiten der matrix
         //

         memcpy (&l_matPointer,l_penemyCurr->getMatrixPointer(), sizeof(g_matIdentity));

         l_matPointer._41 = l_penemyCurr->getPosX();
         l_matPointer._42 = l_penemyCurr->getPosY();
         l_matPointer._43 = l_penemyCurr->getPosZ();

         hr = g_pDev->SetTransform(D3DTS_WORLD, &l_matPointer);
         if (hr != 0) { 
            hr2message(__LINE__,hr,"g_pDev->SetTransform"
                                     "(Equipment/Pointer)");
            return false;
         }


         hr = g_pDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
                                           g_meshresManager.getVertexCount(l_resType), 0, 
                                           g_meshresManager.getFaceCount(l_resType));
         if (hr != 0)
         { 
            hr2message(__LINE__,hr, "g_pDev->DrawIndexedPrimitive(CHAR)");
            return false;
         }

      } // end-for idxEnemy
 
   } // ende powerup pointer malen

   // -- Debug: Kingsplane zeichnen
   //
   // --- helligkeit
   //
   {
       memset(&g_D3DMaterial, 0, sizeof(g_D3DMaterial));
       g_D3DMaterial.Diffuse  = g_d3dcolGreen;
       g_D3DMaterial.Ambient  = g_d3dcolGreen;
       g_D3DMaterial.Specular = g_d3dcolGreen;
       g_D3DMaterial.Emissive = g_d3dcolGreen;
       g_D3DMaterial.Power    = 1.0f;
   }

   hr = g_pDev->SetMaterial(&g_D3DMaterial);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetMaterial(character)");
      return false;
   }

   hr = g_pDev->SetTexture(0, NULL);
   if (hr != 0) { 
       hr2message(__LINE__,hr,"g_pDev->SetTexture(character)");
       return false;
   }


   hr = g_pDev->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetRenderState(ZWRITEENABLE off)");
      return(FALSE);
   }


   hr = g_pDev->SetIndices( g_lpD3DBillbIB);
   if (hr != 0) { 
      hr2message(__LINE__,hr, "g_pDev->SetIndices(Billboardloop)");
      return false;
   }

   hr = g_pDev->SetStreamSource(0, g_lpD3DBillbVB, 0,sizeof(PSXVERTEX));
   if (hr != 0) { 
      hr2message(__LINE__,hr, "g_pDev->SetStreamSource(Billboardloop)");
      return false;
   }
   */

   // -- Explosionspartikel mit Billboards zeichnen
   //
   if (g_bWriteLogFileInLoop) 
   {
      WriteLogFile("draw EDRAW_EXPLOSION/HOTPLASMA Particle-System\n");
   }
  
   // -- Contant Buffer
   //
   {

      ENEMYDRAWERCONSTANTBUFFER::cbChangeEveryFrame l_cbChangeEveryFrame;
      l_cbChangeEveryFrame.mView = XMMatrixTranspose( g_View );    
      XMFLOAT4 l_float4Camera;
      XMStoreFloat4(&l_float4Camera, g_vCamera);
      l_cbChangeEveryFrame.vCamera = l_float4Camera;
      g_pImmediateContext->UpdateSubresource( ENEMYDRAWER::m_pcbChangeEveryFrame, 0, nullptr, &l_cbChangeEveryFrame, 0, 0 );
   
      ENEMYDRAWERCONSTANTBUFFER::cbChangeOnResize l_cbChangesOnResize;
      l_cbChangesOnResize.mProjection = XMMatrixTranspose( g_Projection );
      g_pImmediateContext->UpdateSubresource( ENEMYDRAWER::m_pcbChangeOnResize, 0, nullptr, &l_cbChangesOnResize, 0, 0 );


   }


   // -- setting to transparent : alpha blending and depth-test
   //
   g_pImmediateContext->OMSetBlendState(g_pd3dblendstateTransparent, NULL, 0xffffffff);
   g_pImmediateContext->OMSetDepthStencilState(g_pd3ddepthstateTransparent, 0);

   // Set the input layout
   g_pImmediateContext->IASetInputLayout(g_pExplosionLayout);

   g_pImmediateContext->VSSetShader( g_pVertexShaderExplosion, nullptr, 0 );
   g_pImmediateContext->VSSetConstantBuffers( 0, 1, &ENEMYDRAWER::m_pcbChangeEveryObject );
   g_pImmediateContext->VSSetConstantBuffers( 1, 1, &ENEMYDRAWER::m_pcbChangeOnResize );
   g_pImmediateContext->VSSetConstantBuffers( 2, 1, &ENEMYDRAWER::m_pcbChangeEveryFrame );
   g_pImmediateContext->PSSetShader( g_pPixelShaderExplosion, nullptr, 0 );
   g_pImmediateContext->PSSetConstantBuffers( 0, 1, &ENEMYDRAWER::m_pcbChangeEveryObject );
   g_pImmediateContext->PSSetConstantBuffers( 1, 1, &ENEMYDRAWER::m_pcbChangeOnResize );
   g_pImmediateContext->PSSetConstantBuffers( 2, 1, &ENEMYDRAWER::m_pcbChangeEveryFrame );
   g_pImmediateContext->GSSetShader(g_pExplosionGeometryShader, nullptr, 0);
   g_pImmediateContext->GSSetConstantBuffers(0, 1, &ENEMYDRAWER::m_pcbChangeEveryObject);
   g_pImmediateContext->GSSetConstantBuffers(1, 1, &ENEMYDRAWER::m_pcbChangeOnResize);
   g_pImmediateContext->GSSetConstantBuffers(2, 1, &ENEMYDRAWER::m_pcbChangeEveryFrame);
   
   g_pImmediateContext->PSSetSamplers( 0, 1, &ENEMYDRAWER::m_pSamplerLinear );
   

   // -- umschaufeln in den Explosion-Positionsbuffer
   //
   if (g_txtArray[TID_SHOOT2].activate(0) == false) return false;

   // -- zuerst die Explosionsglut, dann die Rauchwolke drumrum zeichnen
   //
   //for (int l_idxSmoke = 0; l_idxSmoke <2; l_idxSmoke++)
   {
	   int l_idxSmoke = 0;

      int l_idxEnemy;

      //PSXVECTOR* l_vExplosionArray;
      //D3DMATRIX l_matParticle;
      float l_fScalePos, l_fScaleBillboard;
		float l_fFade;

      /*
      // --- open vertexbuffer for reading
      //
      switch (l_idxSmoke)
      {
         case 1:
            hr = g_pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
            hr = g_pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
            hr = g_pDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD); 
         default:
            hr2message(__LINE__,-1,"invalid loop-iteratorvalue in case 1: l_idxSmoke ");
            return false;
      }
      */

		// -- why is there no explosion
		//
      /*
			char l_cDrawCount[300];

         PSXPrintf(__LINE__,l_cDrawCount, sizeof(l_cDrawCount), "   enemies found for EDRAW_EXPLOSION %i ...\n",
						g_arr_EnemyDrawer[EDRAW_EXPLOSION].m_cntDraw); 
			WriteLogFile(l_cDrawCount);
		*/

      for (l_idxEnemy = 0; 
           l_idxEnemy < g_arr_EnemyDrawer[EDRAW_EXPLOSION].m_cntDraw; 
           l_idxEnemy ++)
      {
         ENEMY* l_penemyCurr;


         {
            int l_idxDraw;
            l_idxDraw = g_arr_EnemyDrawer[EDRAW_EXPLOSION].m_idxFirst + l_idxEnemy;
            l_penemyCurr = g_arr_pEnemy[l_idxDraw];

         }

         // --- Explosions spritehelligkeit
         //
         l_fFade = float(g_maxExplosionSteps-l_penemyCurr->m_cntGeneric)/ float(g_maxExplosionSteps);

         switch (l_idxSmoke)
         {
            case 0:

               l_fScaleBillboard = l_penemyCurr->getRad() * 0.04f; 
               l_fScalePos = l_penemyCurr->getRad() * 0.1f *
                             float(l_penemyCurr->m_cntGeneric)/ float(g_maxExplosionSteps);
   
               break;
            case 1:
               l_fScalePos = 0.0f; 
               l_fScaleBillboard = l_penemyCurr->getRad() * 
                             float(l_penemyCurr->m_cntGeneric)/ float(g_maxExplosionSteps);
               break;
            default:
               hr2message(__LINE__,-1,"invalid loop-iteratorvalue in case 2: l_idxSmoke ");
               return false;
         }
			/*


         // -- reines vorbereiten der Partikel-billboardmatrix
         //
         memcpy (&l_matParticle,&g_matIdentity, sizeof(g_matIdentity));
         l_matParticle._11 = g_matCamera._11 * l_fScaleBillboard;
         for (l_idxExplVert=0; l_idxExplVert < g_cntExplVert; l_idxExplVert++)
         {
             
            l_matParticle._41 = l_penemyCurr->getPosX() + l_vExplosionArray[l_idxExplVert].x * l_fScalePos;
            l_matParticle._42 = l_penemyCurr->getPosY() + l_vExplosionArray[l_idxExplVert].y * l_fScalePos;
            l_matParticle._43 = l_penemyCurr->getPosZ() + l_vExplosionArray[l_idxExplVert].z * l_fScalePos;
            hr = g_pDev->SetTransform(D3DTS_WORLD, &l_matParticle);
            hr = g_pDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2 ) ;
         }
         */

	      XMMATRIX l_matWorld = XMMatrixTranslation(l_penemyCurr->getPosX(),
																	l_penemyCurr->getPosY(),
																	l_penemyCurr->getPosZ());
         //XMMATRIX l_matWorld = XMMatrixTranslation(0,0,10.f);

	      ENEMYDRAWERCONSTANTBUFFER::cbChangeEveryObject cb1;    
	      cb1.mWorld = XMMatrixTranspose( l_matWorld);
	      cb1.matWorldViewProj= XMMatrixTranspose( l_matWorld * g_View * g_Projection );
	      cb1.m_fScalePos = l_fScalePos;
	      cb1.m_fScaleBillboard = l_fScaleBillboard;
	      cb1.m_fFade  = l_fFade;
	      g_pImmediateContext->UpdateSubresource(ENEMYDRAWER::m_pcbChangeEveryObject, 0, nullptr, &cb1, 0, 0 );     
	
			//D3D11_MAPPED_SUBRESOURCE l_mappedsubInitData;
	
		   // Set index buffer
		   //g_pImmediateContext->IASetIndexBuffer( m_indexbufArray[l_resType], DXGI_FORMAT_R32_UINT, 0 );
		   // Set primitive topology
		   g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	
			UINT stride = sizeof(SPRITE);
			UINT offset = 0;
			g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pExplosionVB, &stride, &offset);
			g_pImmediateContext->Draw(g_cntExplVert, 0); 

      } // end-for idxEnemy

      
      /*
      if (l_idxSmoke == 0)
      {
         for (l_idxEnemy = 0; 
              l_idxEnemy < g_arr_EnemyDrawer[EDRAW_HOTPLASMA].m_cntDraw; 
              l_idxEnemy ++)
         {
            ENEMY* l_penemyCurr;
   
   
            {
               int l_idxDraw;
               l_idxDraw = g_arr_EnemyDrawer[EDRAW_HOTPLASMA].m_idxFirst + l_idxEnemy;
               l_penemyCurr = g_arr_pEnemy[l_idxDraw];
   
            }
   
           // --- Explosions spritehelligkeit
           //
           {
               D3DCOLORVALUE d3dcolor_explosion;
               d3dcolor_explosion.r = float(g_maxExplosionSteps-l_penemyCurr->m_cntGeneric)/
                                      float(g_maxExplosionSteps);
   
               d3dcolor_explosion.g = d3dcolor_explosion.r;
               d3dcolor_explosion.b = d3dcolor_explosion.r;
         
               memset(&g_D3DMaterial, 0, sizeof(g_D3DMaterial));
               g_D3DMaterial.Diffuse  = d3dcolor_explosion;
               g_D3DMaterial.Ambient  = d3dcolor_explosion; 
               g_D3DMaterial.Specular = d3dcolor_explosion;
               g_D3DMaterial.Emissive = d3dcolor_explosion;
               g_D3DMaterial.Power    = 1.0f;
           }
   
           hr = g_pDev->SetMaterial(&g_D3DMaterial);
           if (hr != 0) { 
              hr2message(__LINE__,hr,"g_pDev->SetMaterial(explosion)");
              return false;
           }
      
           l_fScaleBillboard = l_penemyCurr->getRad() * 0.08f;  //l_penemyCurr->m_fExplScale; //* 0.3f;
           l_fScalePos = l_penemyCurr->getRad() * //6.0f * 
           float(l_penemyCurr->m_cntGeneric)/ float(g_maxExplosionSteps);
   
   
           // -- reines vorbereiten der Partikel-billboardmatrix
           //
           memcpy (&l_matParticle,&g_matIdentity, sizeof(g_matIdentity));
           l_matParticle._11 = g_matCamera._11 * l_fScaleBillboard;
           l_matParticle._12 = g_matCamera._21 * l_fScaleBillboard;
           l_matParticle._13 = g_matCamera._31 * l_fScaleBillboard;
           l_matParticle._21 = g_matCamera._12 * l_fScaleBillboard;
           l_matParticle._22 = g_matCamera._22 * l_fScaleBillboard;
           l_matParticle._23 = g_matCamera._32 * l_fScaleBillboard;
           l_matParticle._31 = g_matCamera._13 * l_fScaleBillboard;
           l_matParticle._32 = g_matCamera._23 * l_fScaleBillboard;
           l_matParticle._33 = g_matCamera._33 * l_fScaleBillboard;
  
           for (l_idxExplVert=0; l_idxExplVert < g_cntExplVert ; l_idxExplVert++)
           {
               
              l_matParticle._41 = l_penemyCurr->getPosX() + l_vExplosionArray[l_idxExplVert].x * l_fScalePos;
              l_matParticle._42 = l_penemyCurr->getPosY() + l_vExplosionArray[l_idxExplVert].y * l_fScalePos;
              l_matParticle._43 = l_penemyCurr->getPosZ() + l_vExplosionArray[l_idxExplVert].z * l_fScalePos;
              hr = g_pDev->SetTransform(D3DTS_WORLD, &l_matParticle);
              if (hr != 0) { 
                 hr2message(__LINE__,hr,"g_pDev->SetTransform"
                                          "(mine/redflare)");
                 return false;
              }
         
              hr = g_pDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2 ) ;
              if (hr != 0) { 
                 hr2message(__LINE__,hr, "g_pDev->DrawIndexedPrimitive(Expl-Particle)");
                 return false;
              }
           }
        } // end-for idxEnemy
     }
*/

	   // -- fuer debug explosion
	   //
		/*
		
	      XMMATRIX l_matWorld = XMMatrixTranslation(0,0,10.f);
	      ENEMYDRAWERCONSTANTBUFFER::cbChangeEveryObject cb1;    
	      cb1.mWorld = XMMatrixTranspose( l_matWorld);
	      cb1.matWorldViewProj= XMMatrixTranspose( l_matWorld * g_View * g_Projection );
	      cb1.m_fScale = g_fTempScaleExplosion;
	      g_fTempScaleExplosion = g_fTempScaleExplosion + 0.01f;
	
	      if (g_fTempScaleExplosion > 10.0f) g_fTempScaleExplosion = 0.0f;
	      g_pImmediateContext->UpdateSubresource(ENEMYDRAWER::m_pcbChangeEveryObject, 0, nullptr, &cb1, 0, 0 );     
	
			//D3D11_MAPPED_SUBRESOURCE l_mappedsubInitData;
			SPRITE* l_pExplosion = NULL;
	
		   // Set index buffer
		   //g_pImmediateContext->IASetIndexBuffer( m_indexbufArray[l_resType], DXGI_FORMAT_R32_UINT, 0 );
		   // Set primitive topology
		   g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	
			UINT stride = sizeof(SPRITE);
			UINT offset = 0;
			g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pExplosionVB, &stride, &offset);
	
			//g_pImmediateContext->Draw(g_cntShootArray, 0);
			g_pImmediateContext->Draw(g_cntExplVert, 0); //test with 2 vertices
		*/
	}
/*
  hr = g_d3dExplosionVB->Unlock();

  if (hr != 0) { 
     hr2message(__LINE__,hr,"g_lpD3DBillbVB->Unlock");
     return(FALSE);
  }

  hr = g_d3dExplosionVB2->Unlock();

  if (hr != 0) { 
     hr2message(__LINE__,hr,"g_lpD3DBillbVB->Unlock");

     return(FALSE);
  }
  */

   // -- alles wieder transparent
   //
   // alpha blending and depth-test
   //
   g_pImmediateContext->OMSetBlendState(g_pd3dblendstateTransparent, NULL, 0xffffffff);
   g_pImmediateContext->OMSetDepthStencilState(g_pd3ddepthstateTransparent, 0);

   // Set the input layout
   g_pImmediateContext->IASetInputLayout(g_pSpriteLayout);

   g_pImmediateContext->VSSetShader( g_pVertexShaderSpriteDrawer, nullptr, 0 );
   g_pImmediateContext->VSSetConstantBuffers( 0, 1, &ENEMYDRAWER::m_pcbChangeEveryObject );
   g_pImmediateContext->VSSetConstantBuffers( 1, 1, &ENEMYDRAWER::m_pcbChangeOnResize );
   g_pImmediateContext->VSSetConstantBuffers( 2, 1, &ENEMYDRAWER::m_pcbChangeEveryFrame );
   g_pImmediateContext->PSSetShader( g_pPixelShaderSpriteDrawer, nullptr, 0 );
   g_pImmediateContext->PSSetConstantBuffers( 0, 1, &ENEMYDRAWER::m_pcbChangeEveryObject );
   g_pImmediateContext->PSSetConstantBuffers( 1, 1, &ENEMYDRAWER::m_pcbChangeOnResize );
   g_pImmediateContext->PSSetConstantBuffers( 2, 1, &ENEMYDRAWER::m_pcbChangeEveryFrame );
   g_pImmediateContext->GSSetShader(g_pSpriteGeometryShader, nullptr, 0);
   g_pImmediateContext->GSSetConstantBuffers(0, 1, &ENEMYDRAWER::m_pcbChangeEveryObject);
   g_pImmediateContext->GSSetConstantBuffers(1, 1, &ENEMYDRAWER::m_pcbChangeOnResize);
   g_pImmediateContext->GSSetConstantBuffers(2, 1, &ENEMYDRAWER::m_pcbChangeEveryFrame);
   
   g_pImmediateContext->PSSetSamplers( 0, 1, &ENEMYDRAWER::m_pSamplerLinear );
   

	// Set primitive topology
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);


  /*
   hr = g_pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);

   if (hr != 0) { 
      hr2message(__LINE__,hr,
                "g_pDev->SetRenderState(SRCBLEND)");
      return(FALSE);
   };

   // Set the source blend state.
   hr = g_pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
   if (hr != 0) { 
      hr2message(__LINE__,hr,
                "g_pDev->SetRenderState(SRCBLEND)");
      return(FALSE);
   };


   // --- folgende flare textures wurden oben nicht mit mipmaps hinterlegt
   //
   if ( PSXSetMipMapping(false, 0) == false )
   { 
      return false;

   }

   if ( PSXSetMipMapping(false, 1) == false )
   { 
      return false;
   }

   // --- extra-loop through enemy array 
   //     painting billboards: explosions, shieldflashes, bonusdisplays
   //
   cnt_explosions_painted = 0;
	*/

   {
      int l_idxEnemy;

		/*
      if (g_bWriteLogFileInLoop) 
      {
         WriteLogFile("draw Redflares additional to EDRAW_MINE_EMISSIVE\n");
      }

      if (g_bEnableTextures)
      {
         if (g_txtArray[TID_RED_FLARE].activate(0) == false) return false;
      }

      for (l_idxEnemy = 0; 
           l_idxEnemy < g_arr_EnemyDrawer[EDRAW_MINE_EMISSIVE].m_cntDraw; 
           l_idxEnemy ++)
      {
         int l_idxDraw;
         l_idxDraw = g_arr_EnemyDrawer[EDRAW_MINE_EMISSIVE].m_idxFirst + l_idxEnemy;
   
         {
             D3DCOLORVALUE l_d3dcolRedFlare;
             l_d3dcolRedFlare.r = 80000.0f / 
                                  g_arr_pEnemy[l_idxDraw]->getPos2DZ()/
                                  g_arr_pEnemy[l_idxDraw]->getPos2DZ();
             l_d3dcolRedFlare.g = l_d3dcolRedFlare.r;
             l_d3dcolRedFlare.b = l_d3dcolRedFlare.r;
             memset(&g_D3DMaterial, 0, sizeof(g_D3DMaterial));
             g_D3DMaterial.Diffuse  = l_d3dcolRedFlare;
             g_D3DMaterial.Ambient  = l_d3dcolRedFlare; 
             g_D3DMaterial.Specular = l_d3dcolRedFlare;
             g_D3DMaterial.Emissive = l_d3dcolRedFlare;
             g_D3DMaterial.Power    = 1.0f;
         }

         hr = g_pDev->SetMaterial(&g_D3DMaterial);
         if (hr != 0) { 
            hr2message(__LINE__,hr,"g_pDev->"
                                     "SetMaterial(explosion)");
            return false;
         }

         // --- painting redflare at mine
         //
         {
            memcpy (&g_matGeneric1,&g_matIdentity, sizeof(g_matIdentity));
            g_matGeneric1._11 = -g_arr_pEnemy[l_idxDraw]->getMat_11() * 2.0f;
            g_matGeneric1._12 = -g_arr_pEnemy[l_idxDraw]->getMat_12() * 2.0f;
            g_matGeneric1._13 = -g_arr_pEnemy[l_idxDraw]->getMat_13() * 2.0f;
            g_matGeneric1._21 = g_arr_pEnemy[l_idxDraw]->getMat_21() * 2.0f;
            g_matGeneric1._22 = g_arr_pEnemy[l_idxDraw]->getMat_22() * 2.0f;
            g_matGeneric1._23 = g_arr_pEnemy[l_idxDraw]->getMat_23() * 2.0f;
            g_matGeneric1._31 = g_arr_pEnemy[l_idxDraw]->getMat_31() * 2.0f;
            g_matGeneric1._32 = g_arr_pEnemy[l_idxDraw]->getMat_32() * 2.0f;
            g_matGeneric1._33 = g_arr_pEnemy[l_idxDraw]->getMat_33() * 2.0f;
            g_matGeneric1._41 = g_arr_pEnemy[l_idxDraw]->getPosX() + 6.0f * g_arr_pEnemy[l_idxDraw]->getMat_31();
            g_matGeneric1._42 = g_arr_pEnemy[l_idxDraw]->getPosY() + 6.0f * g_arr_pEnemy[l_idxDraw]->getMat_32();
            g_matGeneric1._43 = g_arr_pEnemy[l_idxDraw]->getPosZ() + 6.0f * g_arr_pEnemy[l_idxDraw]->getMat_33();

            hr = g_pDev->SetTransform(D3DTS_WORLD, &g_matGeneric1);
            if (hr != 0) { 
               hr2message(__LINE__,hr,"g_pDev->SetTransform"
                                        "(mine/redflare)");
               return false;
            }
      
            hr = g_pDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2 ) ;
      
            if (hr != 0) { 
               hr2message(__LINE__,hr,
                          "g_pDev->DrawIndexedPrimitive(mine/redflare)");
               return false;
            }
         }
      }

     */

      D3D11_MAPPED_SUBRESOURCE l_mappedsubInitData;
      UINT stride = sizeof(SPRITE);
		UINT offset = 0;

      if (g_bWriteLogFileInLoop) 
      {
         WriteLogFile("draw Flashflare additional to EDRAW_EXPLOSION\n");
      }

			
		hr = g_pImmediateContext->Map(
				g_pSpriteBuffer,             //ID3D11Resource *pResource,
				0,                           //UINT Subresource,
				D3D11_MAP_WRITE_DISCARD,     //D3D11_MAP MapType,
				0,                           //UINT MapFlags,
				&l_mappedsubInitData);       //D3D11_MAPPED_SUBRESOURCE *pMappedResource
		if (FAILED(hr))
		{
			hr2message(__LINE__, hr, "g_pd3dDevice->Map()");
			return false;
		}
			
		SPRITE* l_pSprite = reinterpret_cast<SPRITE*> (l_mappedsubInitData.pData);
		int l_cntFlash = 0;

      if (g_txtArray[TID_LENS1].activate(0) == false) return false;

      for (l_idxEnemy = 0; 
           l_idxEnemy < g_arr_EnemyDrawer[EDRAW_EXPLOSION].m_cntDraw; 
           l_idxEnemy ++)
      {
         int l_idxDraw;
         l_idxDraw = g_arr_EnemyDrawer[EDRAW_EXPLOSION].m_idxFirst + l_idxEnemy;

			
			/*
   
         // --- specifiying material for explosion-billboard 
         //
         {
             D3DCOLORVALUE d3dcolor_explosion;
             d3dcolor_explosion.r = 
                float(g_maxExplosionSteps-
                      g_arr_pEnemy[l_idxDraw]->m_cntGeneric)/
                float(g_maxExplosionSteps);

             d3dcolor_explosion.g = d3dcolor_explosion.r;
             d3dcolor_explosion.b = d3dcolor_explosion.r;
      
             memset(&g_D3DMaterial, 0, sizeof(g_D3DMaterial));
             g_D3DMaterial.Diffuse  = d3dcolor_explosion;
             g_D3DMaterial.Ambient  = d3dcolor_explosion; 
             g_D3DMaterial.Specular = d3dcolor_explosion;
             g_D3DMaterial.Emissive = d3dcolor_explosion;
             g_D3DMaterial.Power    = 1.0f;
         }

         hr = g_pDev->SetMaterial(&g_D3DMaterial);
         if (hr != 0) { 
            hr2message(__LINE__,hr,"g_pDev->SetMaterial(explosionflash)");
            return false;
         }
			*/

         // --- painting explosion-flash
         //
         float l_fScaleFlash = float( g_arr_pEnemy[l_idxDraw]->m_cntGeneric);
			float l_fMaxExplosionSteps = float(g_maxExplosionSteps);
         if ( g_arr_pEnemy[l_idxDraw]->m_cntGeneric < g_maxExplosionSteps) 
			{
				memset(l_pSprite+l_cntFlash, 0, sizeof(l_pSprite[0]));
		   	l_pSprite[l_cntFlash].Pos.x = g_arr_pEnemy[l_idxDraw]->getPosX();
		   	l_pSprite[l_cntFlash].Pos.y = g_arr_pEnemy[l_idxDraw]->getPosY();
		   	l_pSprite[l_cntFlash].Pos.z = g_arr_pEnemy[l_idxDraw]->getPosZ();
		   	l_pSprite[l_cntFlash].Pos.w = 1.0f;
	
		   	l_pSprite[l_cntFlash].dimension.x = l_fScaleFlash * g_arr_pEnemy[l_idxDraw]->getRad() / l_fMaxExplosionSteps; 
		   	l_pSprite[l_cntFlash].dimension.y = l_fScaleFlash * g_arr_pEnemy[l_idxDraw]->getRad() / l_fMaxExplosionSteps; 
		   	l_pSprite[l_cntFlash].opacity = (l_fMaxExplosionSteps-l_fScaleFlash)/l_fMaxExplosionSteps;

				l_cntFlash++;
			}

      } // end-for
		g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pSpriteBuffer, &stride, &offset);
		g_pImmediateContext->Unmap(g_pSpriteBuffer, 0);
		if (l_cntFlash) g_pImmediateContext->Draw(l_cntFlash, 0);

   } // close stack 

/*
   if (g_bWriteLogFileInLoop) 
   {
      WriteLogFile("draw destinationframes\n");
   }

   if (g_bWriteLogFileInLoop) 
   {
      WriteLogFile("draw Shield-Flash\n");
   }

   if (g_bEnableTextures)
   {
      if (g_txtArray[TID_SHOOT2].activate(0) == false) return false;
   }

   {
      int l_idxEnemy;
      int l_idxDraw;
      float ScaleFlash;

      for (l_idxEnemy = 0; 
           l_idxEnemy < g_arr_EnemyDrawer[EDRAW_SHIELDFLASH].m_cntDraw; 
           l_idxEnemy ++)
      {
         l_idxDraw = g_arr_EnemyDrawer[EDRAW_SHIELDFLASH].m_idxFirst + l_idxEnemy;

         if (g_bWriteLogFileInLoop) 
         {
            char logtext[80];
            PSXPrintf(__LINE__,logtext, sizeof(logtext), "   shield flash for enemy %i ...\n",l_idxDraw);
            WriteLogFile(logtext);
         }

         // --- specifiying material Shieldflash
         //
         {
             D3DCOLORVALUE d3dcolor_shieldflash;
             d3dcolor_shieldflash.r = float(g_maxShieldFlashTimer-
                                            g_arr_pEnemy[l_idxDraw]->m_cntShieldFlashTimer)/
                                      float(g_maxShieldFlashTimer);

             d3dcolor_shieldflash.g = d3dcolor_shieldflash.r;
             d3dcolor_shieldflash.b = d3dcolor_shieldflash.r;
      
             memset(&g_D3DMaterial, 0, sizeof(g_D3DMaterial));
             g_D3DMaterial.Diffuse  = d3dcolor_shieldflash;
             g_D3DMaterial.Ambient  = d3dcolor_shieldflash; 
             g_D3DMaterial.Specular = d3dcolor_shieldflash;
             g_D3DMaterial.Emissive = d3dcolor_shieldflash;
             g_D3DMaterial.Power    = 1.0f;
         }
  
         hr = g_pDev->SetMaterial(&g_D3DMaterial);
         if (hr != 0) { 
            hr2message(__LINE__,hr,"g_pDev->SetMaterial(shieldflash)");
            return false;
         }

 
         // --- painting shield-flash
         //
         ScaleFlash = float(g_arr_pEnemy[l_idxDraw]->m_cntShieldFlashTimer);
   
         memcpy (&g_matGeneric1,&g_matIdentity, sizeof(g_matIdentity));
         g_matGeneric1._11 = g_matCamera._11*ScaleFlash;
         g_matGeneric1._12 = g_matCamera._21*ScaleFlash;
         g_matGeneric1._13 = g_matCamera._31*ScaleFlash;
         g_matGeneric1._21 = g_matCamera._12*ScaleFlash;
         g_matGeneric1._22 = g_matCamera._22*ScaleFlash;
         g_matGeneric1._23 = g_matCamera._32*ScaleFlash;
         g_matGeneric1._31 = g_matCamera._13*ScaleFlash;
         g_matGeneric1._32 = g_matCamera._23*ScaleFlash;
         g_matGeneric1._33 = g_matCamera._33*ScaleFlash;
         g_matGeneric1._41 = g_arr_pEnemy[l_idxDraw]->getPosX();
         g_matGeneric1._42 = g_arr_pEnemy[l_idxDraw]->getPosY();
         g_matGeneric1._43 = g_arr_pEnemy[l_idxDraw]->getPosZ();
   
         hr = g_pDev->SetTransform(D3DTS_WORLD, &g_matGeneric1);
         if (hr != 0) { 
            hr2message(__LINE__,hr,"g_pDev->SetTransform"
                                  "(shieldflash)");
            return false;
         }

         hr = g_pDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2 ) ;
         if (hr != 0) { 
            hr2message(__LINE__,hr, "g_pDev->DrawIndexedPrimitive(shieldflash)");
            return false;
         }
      }// end-for l_idxENemy

   } 

   // -- ab hier kein culling mehr
   //
   hr = g_pDev->SetRenderState( D3DRS_CULLMODE,D3DCULL_NONE); 
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->"
                               "SetRenderState(CULLMODE)");
      return false;
   } 


   // *******************************************************
   //           schuss sprites zeichnen und updaten
   // *******************************************************

   */

   if (g_bWriteLogFileInLoop) 
   {
      WriteLogFile("drawing shootfire\n");
   }


   // -- Contant Buffer
   //
   {

       XMMATRIX l_matWorld;
       ENEMYDRAWERCONSTANTBUFFER::cbChangeEveryFrame l_cbChangeEveryFrame;
       l_cbChangeEveryFrame.mView = XMMatrixTranspose( g_View );    
       XMFLOAT4 l_float4Camera;
       XMStoreFloat4(&l_float4Camera, g_vCamera);
       l_cbChangeEveryFrame.vCamera = l_float4Camera;
       g_pImmediateContext->UpdateSubresource( ENEMYDRAWER::m_pcbChangeEveryFrame, 
		                               0, nullptr, &l_cbChangeEveryFrame, 0, 0 );
   
       ENEMYDRAWERCONSTANTBUFFER::cbChangeOnResize l_cbChangesOnResize;
       l_cbChangesOnResize.mProjection = XMMatrixTranspose( g_Projection );
       g_pImmediateContext->UpdateSubresource( ENEMYDRAWER::m_pcbChangeOnResize, 
		                               0, nullptr, &l_cbChangesOnResize, 0, 0 );

       ENEMYDRAWERCONSTANTBUFFER::cbChangeEveryObject cb1;    
       cb1.mWorld = XMMatrixIdentity();
       cb1.matWorldViewProj= XMMatrixTranspose( g_View * g_Projection );
       g_pImmediateContext->UpdateSubresource(ENEMYDRAWER::m_pcbChangeEveryObject, 0, nullptr, &cb1, 0, 0 );     

   }

   // -- setting to transparent : alpha blending and depth-test
   //
   g_pImmediateContext->OMSetBlendState(g_pd3dblendstateTransparent, NULL, 0xffffffff);
   g_pImmediateContext->OMSetDepthStencilState(g_pd3ddepthstateTransparent, 0);

   // Set the input layout
   g_pImmediateContext->IASetInputLayout(g_pShootPosLayout);

   g_pImmediateContext->VSSetShader( g_pVertexShaderShootDrawer, nullptr, 0 );
   g_pImmediateContext->VSSetConstantBuffers( 0, 1, &ENEMYDRAWER::m_pcbChangeEveryObject );
   g_pImmediateContext->VSSetConstantBuffers( 1, 1, &ENEMYDRAWER::m_pcbChangeOnResize );
   g_pImmediateContext->VSSetConstantBuffers( 2, 1, &ENEMYDRAWER::m_pcbChangeEveryFrame );
   g_pImmediateContext->PSSetShader( g_pPixelShaderShootDrawer, nullptr, 0 );
   g_pImmediateContext->PSSetConstantBuffers( 0, 1, &ENEMYDRAWER::m_pcbChangeEveryObject );
   g_pImmediateContext->PSSetConstantBuffers( 1, 1, &ENEMYDRAWER::m_pcbChangeOnResize );
   g_pImmediateContext->PSSetConstantBuffers( 2, 1, &ENEMYDRAWER::m_pcbChangeEveryFrame );
   //g_pImmediateContext->PSSetShaderResources( 0, 1, &g_pTextureRV );
   
   g_pImmediateContext->GSSetShader(g_pShootGeometryShader, nullptr, 0);
   g_pImmediateContext->GSSetConstantBuffers(0, 1, &ENEMYDRAWER::m_pcbChangeEveryObject);
   g_pImmediateContext->GSSetConstantBuffers(1, 1, &ENEMYDRAWER::m_pcbChangeOnResize);
   g_pImmediateContext->GSSetConstantBuffers(2, 1, &ENEMYDRAWER::m_pcbChangeEveryFrame);
   
   g_pImmediateContext->PSSetSamplers( 0, 1, &ENEMYDRAWER::m_pSamplerLinear );

   

   /*
   hr = g_pDev->SetVertexShader( NULL);
   hr = g_pDev->SetVertexDeclaration(g_pvdeclPSXVertex);

   // -- bestimmt in welche Richtung der Schweif geht
   //
   {
      int l_idxShoot;
      for (l_idxShoot=0;l_idxShoot<g_cntShoot; l_idxShoot++)
      {
         if (shoot_array[l_idxShoot].m_cntTimeToLive > 0)
         {
            if (shoot_array[l_idxShoot].determineTrail() == false) return FALSE;
         }
      }
   }

   hr = g_pDev->SetIndices( g_lpD3DBillbIB);
   */

   // -- umschaufeln in den Shoot-Positionsbuffer
	//
   if (g_txtArray[TID_SHOOT2].activate(0) == false) return false;
	{

		D3D11_MAPPED_SUBRESOURCE l_mappedsubInitData;
		SHOOTPOS* l_pShootPos = NULL;
		int l_cntShootPosToDraw = 0;
		hr = g_pImmediateContext->Map(
				g_pShootPosBuffer,             //ID3D11Resource *pResource,
				0,                           //UINT Subresource,
				D3D11_MAP_WRITE_DISCARD,     //D3D11_MAP MapType,
				0,                           //UINT MapFlags,
				&l_mappedsubInitData);       //D3D11_MAPPED_SUBRESOURCE *pMappedResource
		if (FAILED(hr))
		{
			hr2message(__LINE__, hr, "g_pd3dDevice->Map()");
			return false;
		}

		l_pShootPos =	reinterpret_cast<SHOOTPOS*> (l_mappedsubInitData.pData);

      SHOOT* l_pshootCurr = shoot_array;
      for (int l_idxShoot=0;l_idxShoot<g_cntShoot; l_idxShoot++)
      {
				  /*
         if (l_pshootCurr->m_cntTimeToLive > 0)// &&
         {
            float l_fShade;
         
            l_fShade = float(l_pshootCurr->m_cntTimeToLive)/ 
                       float(l_pshootCurr->m_maxTimeToLive);

			}*/

	      XMStoreFloat4(&(l_pShootPos[l_idxShoot].topLeft), l_pshootCurr->m_vPos);
	      XMStoreFloat4(&(l_pShootPos[l_idxShoot].nextPos), l_pshootCurr->m_vPos + l_pshootCurr->m_vStep);
         l_pshootCurr++;
			l_cntShootPosToDraw++;
			
      } // end-for l_idxShoot
		g_pImmediateContext->Unmap(g_pShootPosBuffer, 0);
		//
	   // --- prepare Vertex/Indices Input Stream
	   //

	   // Set index buffer
	   //g_pImmediateContext->IASetIndexBuffer( m_indexbufArray[l_resType], DXGI_FORMAT_R32_UINT, 0 );
	   // Set primitive topology
	   g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

		UINT stride = sizeof(SHOOTPOS);
		UINT offset = 0;
		g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pShootPosBuffer, &stride, &offset);

		//g_pImmediateContext->Draw(g_cntShootArray, 0);
		g_pImmediateContext->Draw(l_cntShootPosToDraw, 0); //test with 2 vertices
	}
	
	/*
   for (int l_idxShootDrawer = 0; l_idxShootDrawer < SHOOTDRAWER_LASTENUM; l_idxShootDrawer++)
   {
      TEXTUREID l_tidShoot;

      if (l_idxShootDrawer == SHOOTDRAWER_NOBILLBOARD) continue;

      if (g_bEnableTextures)
      {
         switch(l_idxShootDrawer)
         {
            case SHOOTDRAWER_REDTORPEDO:
               l_tidShoot = TID_RED_FLARE;
               break;

            case SHOOTDRAWER_REDLASER:
               l_tidShoot = TID_SHOOT2;
               break;

            case SHOOTDRAWER_BLUELASER:
               l_tidShoot = TID_SHOOT1;
               break;
            default: 
               MessageBox(hFrameWind, "IDXSHOOTDRAWER unknown", 
                          "Painting shoot sprites", MB_ICONEXCLAMATION);
         }

         if (g_txtArray[l_tidShoot].activate(0) == false) return false;
      }


      // --- Zeichnen der Schuesse
      //
      memset(&g_D3DMaterial, 0, sizeof(g_D3DMaterial));
      */

      /*if (l_idxShootDrawer == SHOOTDRAWER_REDTORPEDO)*/

      /*
         int l_idxShoot;
         D3DCOLORVALUE l_d3dColor;
         SHOOT* l_pshootCurr;

         hr = g_pDev->SetStreamSource(0, g_lpD3DBillbVB, 0, sizeof(PSXVERTEX));
         if (hr != 0) { 
            hr2message(__LINE__,hr, "g_pDev->SetStreamSource(shoot)");
            return(false);
         }

         memcpy (&g_matGeneric1,&g_matIdentity, sizeof(g_matIdentity));
         memcpy (&g_matGeneric2,&g_matIdentity, sizeof(g_matIdentity));

         l_pshootCurr = shoot_array;
         for (l_idxShoot=0;l_idxShoot<g_cntShoot; l_idxShoot++)
         {
            if (l_pshootCurr->m_cntTimeToLive > 0 &&
                l_pshootCurr->m_idxShootDrawer == l_idxShootDrawer)
            {
               D3DMATRIX l_matShoot;
            
               l_d3dColor.r = float(l_pshootCurr->m_cntTimeToLive)/ 
                              float(l_pshootCurr->m_maxTimeToLive);
               l_d3dColor.g = l_d3dColor.r;
               l_d3dColor.b = l_d3dColor.r;
               g_D3DMaterial.Ambient  = l_d3dColor; 
            
               hr = g_pDev->SetMaterial(&g_D3DMaterial);
               if (hr != 0) { 
                  hr2message(__LINE__,hr,"g_pDev->SetMaterial(shoot)");
                  return(FALSE);
               }
            
               g_matGeneric1._11 = g_matCamera._11 /2.0f * l_pshootCurr->m_fScale;
               g_matGeneric1._12 = g_matCamera._21 /2.0f * l_pshootCurr->m_fScale;
               g_matGeneric1._13 = g_matCamera._31 /2.0f * l_pshootCurr->m_fScale;
               g_matGeneric1._21 = g_matCamera._12 /2.0f * l_pshootCurr->m_fScale;
               g_matGeneric1._22 = g_matCamera._22 /2.0f * l_pshootCurr->m_fScale;
               g_matGeneric1._23 = g_matCamera._32 /2.0f * l_pshootCurr->m_fScale;
               g_matGeneric1._31 = g_matCamera._13 /2.0f * l_pshootCurr->m_fScale;
               g_matGeneric1._32 = g_matCamera._23 /2.0f * l_pshootCurr->m_fScale;
               g_matGeneric1._33 = g_matCamera._33 /2.0f * l_pshootCurr->m_fScale;
            
               g_matGeneric1._41 = l_pshootCurr->m_vPos.x;
               g_matGeneric1._42 = l_pshootCurr->m_vPos.y;
               g_matGeneric1._43 = l_pshootCurr->m_vPos.z;
            
               g_matGeneric2._11 = cos ( l_pshootCurr->m_fViewRotation1);
               g_matGeneric2._12 = - sin ( l_pshootCurr->m_fViewRotation1 );
               g_matGeneric2._21 = sin ( l_pshootCurr->m_fViewRotation1 );
               g_matGeneric2._22 = cos ( l_pshootCurr->m_fViewRotation1 );
            
               matmult(&l_matShoot, &g_matGeneric2, &g_matGeneric1);
                  
               hr = g_pDev->SetTransform(D3DTS_WORLD, &l_matShoot);
               if (hr != 0) { 
                  hr2message(__LINE__,hr,"g_pDev->SetTransform"
                                           "(shoot)");
                  return(false);
               }
               hr = g_pDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2 ) ;
               if (hr != 0) { 
                  hr2message(__LINE__,hr,
                             "g_pDev->DrawIndexedPrimitive(shoot)");
                  return(false);
               }
            }
            l_pshootCurr++;
         }
      }else{
         int l_idxShoot;
         SHOOT* l_pshootCurr;
         D3DCOLORVALUE l_d3dColor;
         
         // -- Vorbereitung der Struktur für die erste Schleife
         //
         memcpy (&g_matGeneric1,&g_matIdentity, sizeof(g_matIdentity));
         g_matGeneric1._21 = g_matCamera._12 /8.0f;
         g_matGeneric1._22 = g_matCamera._22 /8.0f;
         g_matGeneric1._23 = g_matCamera._32 /8.0f;
         g_matGeneric1._31 = g_matCamera._13 /8.0f;
         g_matGeneric1._32 = g_matCamera._23 /8.0f;
         g_matGeneric1._33 = g_matCamera._33 /8.0f;
            
         hr = g_pDev->SetStreamSource(0, g_lpD3DRightPartBillbVB, 0, sizeof(PSXVERTEX));
         if (hr != 0) { 
            hr2message(__LINE__,hr, "g_pDev->SetStreamSource(shoot)");
            return(false);
         }
            
         l_pshootCurr = shoot_array;
         for (l_idxShoot=0;l_idxShoot<g_cntShoot; l_idxShoot++)
         {
            if (l_pshootCurr->m_cntTimeToLive > 0 &&
                l_pshootCurr->m_idxShootDrawer == l_idxShootDrawer)
            {
               float l_fShade;
            
               l_fShade = float(l_pshootCurr->m_cntTimeToLive)/ 
                          float(l_pshootCurr->m_maxTimeToLive);

               if (l_pshootCurr->m_pEquipment)
               {
                  l_d3dColor.r = l_pshootCurr->m_pEquipment->m_d3dcolShoot.r * l_fShade;
                  l_d3dColor.g = l_pshootCurr->m_pEquipment->m_d3dcolShoot.g * l_fShade;
                  l_d3dColor.b = l_pshootCurr->m_pEquipment->m_d3dcolShoot.b * l_fShade;
               }else{
                  l_d3dColor.r = l_fShade;
                  l_d3dColor.g = l_fShade;
                  l_d3dColor.b = l_fShade;
               }
               g_D3DMaterial.Ambient  = l_d3dColor; 
            
               hr = g_pDev->SetMaterial(&g_D3DMaterial);
               if (hr != 0) { 
                  hr2message(__LINE__,hr,"g_pDev->SetMaterial(shoot)");
                  return(FALSE);
               }
            
               g_matGeneric1._11 = l_pshootCurr->m_fTrailFactor * g_matCamera._11 /8.0f;
               g_matGeneric1._12 = l_pshootCurr->m_fTrailFactor * g_matCamera._21 /8.0f;
               g_matGeneric1._13 = l_pshootCurr->m_fTrailFactor * g_matCamera._31 /8.0f;
               g_matGeneric1._41 = l_pshootCurr->m_vPos.x;
               g_matGeneric1._42 = l_pshootCurr->m_vPos.y;
               g_matGeneric1._43 = l_pshootCurr->m_vPos.z;
               hr = g_pDev->SetTransform(D3DTS_WORLD, &g_matGeneric1);
               if (hr != 0) { 
                  hr2message(__LINE__,hr,"g_pDev->SetTransform(shoot)");
                  return(false);
               }
               
               hr = g_pDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2 ) ;
               if (hr != 0) { 
                  hr2message(__LINE__,hr,"g_pDev->DrawIndexedPrimitive(shoot)");
                  return(false);
               }
            }

            l_pshootCurr++;
         } // end-for l_idxShoot
            
         hr = g_pDev->SetStreamSource(0, g_lpD3DBillbVB, 0, sizeof(PSXVERTEX));
         if (hr != 0) { 
            hr2message(__LINE__,hr,
                       "g_pDev->SetStreamSource(shoot)");
            return(false);
         }

         g_matGeneric1._11 = g_matCamera._11 /4.0f;
         g_matGeneric1._12 = g_matCamera._21 /4.0f;
         g_matGeneric1._13 = g_matCamera._31 /4.0f;
         g_matGeneric1._21 = g_matCamera._12 /4.0f;
         g_matGeneric1._22 = g_matCamera._22 /4.0f;
         g_matGeneric1._23 = g_matCamera._32 /4.0f;
         g_matGeneric1._31 = g_matCamera._13 /4.0f;
         g_matGeneric1._32 = g_matCamera._23 /4.0f;
         g_matGeneric1._33 = g_matCamera._33 /4.0f;
            
         l_pshootCurr = shoot_array;
         for (l_idxShoot=0;l_idxShoot<g_cntShoot; l_idxShoot++)
         {
            if (l_pshootCurr->m_cntTimeToLive > 0 &&
                l_pshootCurr->m_idxShootDrawer == l_idxShootDrawer)
            {
               float l_fShade;
            
               l_fShade = float(l_pshootCurr->m_cntTimeToLive)/ 
                          float(l_pshootCurr->m_maxTimeToLive);

               if (l_pshootCurr->m_pEquipment)
               {
                  l_d3dColor.r = l_pshootCurr->m_pEquipment->m_d3dcolShoot.r * l_fShade;
                  l_d3dColor.g = l_pshootCurr->m_pEquipment->m_d3dcolShoot.g * l_fShade;
                  l_d3dColor.b = l_pshootCurr->m_pEquipment->m_d3dcolShoot.b * l_fShade;
               }else{
                  l_d3dColor.r = l_fShade;
                  l_d3dColor.g = l_fShade;
                  l_d3dColor.b = l_fShade;
               }
               g_D3DMaterial.Ambient  = l_d3dColor; 
            
               hr = g_pDev->SetMaterial(&g_D3DMaterial);
               if (hr != 0) { 
                  hr2message(__LINE__,hr,"g_pDev->SetMaterial(shoot)");
                  return(FALSE);
               }
            
               g_matGeneric1._41 = l_pshootCurr->m_vPos.x;
               g_matGeneric1._42 = l_pshootCurr->m_vPos.y;
               g_matGeneric1._43 = l_pshootCurr->m_vPos.z;
                  
               hr = g_pDev->SetTransform(D3DTS_WORLD, &g_matGeneric1);
               if (hr != 0) { 
                  hr2message(__LINE__,hr,"g_pDev->SetTransform"
                                           "(shoot)");
                  return(false);
               }
               hr = g_pDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,
                                                      0, 0, 4, 0, 2 ) ;
               if (hr != 0) { 
                  hr2message(__LINE__,hr,
                             "g_pDev->DrawIndexedPrimitive(shoot)");
                  return(false);
               }
            
            }

            l_pshootCurr++;
         } // end-for l_idxShoot

         hr = g_pDev->SetStreamSource(0, g_lpD3DLeftPartBillbVB, 0, sizeof(PSXVERTEX));
         if (hr != 0) { 
            hr2message(__LINE__,hr, "g_pDev->SetStreamSource(shoot)");
            return(false);
         }

         memcpy (&g_matGeneric1,&g_matIdentity, sizeof(g_matIdentity));
         g_matGeneric1._21 = g_matCamera._12 /8.0f;
         g_matGeneric1._22 = g_matCamera._22 /8.0f;
         g_matGeneric1._23 = g_matCamera._32 /8.0f;
         g_matGeneric1._31 = g_matCamera._13 /8.0f;
         g_matGeneric1._32 = g_matCamera._23 /8.0f;
         g_matGeneric1._33 = g_matCamera._33 /8.0f;
            
         l_pshootCurr = shoot_array;
	 */


				/*
         SHOOT* l_pshootCurr = shoot_array;
         for (int l_idxShoot=0;l_idxShoot<g_cntShoot; l_idxShoot++)
         {
            if (l_pshootCurr->m_cntTimeToLive > 0)// &&
                //TODO l_pshootCurr->m_idxShootDrawer == l_idxShootDrawer)
            {
               float l_fShade;
            
               l_fShade = float(l_pshootCurr->m_cntTimeToLive)/ 
                          float(l_pshootCurr->m_maxTimeToLive);
								  */

//               if (l_pshootCurr->m_pEquipment)
//               {
//                  l_d3dColor.r = l_pshootCurr->m_pEquipment->m_d3dcolShoot.r * l_fShade;
//                  l_d3dColor.g = l_pshootCurr->m_pEquipment->m_d3dcolShoot.g * l_fShade;
//                  l_d3dColor.b = l_pshootCurr->m_pEquipment->m_d3dcolShoot.b * l_fShade;
//               }else{
//                  l_d3dColor.r = l_fShade;
//                  l_d3dColor.g = l_fShade;
//                  l_d3dColor.b = l_fShade;
//               }
           
//               g_D3DMaterial.Ambient  = l_d3dColor; 
            
//               hr = g_pDev->SetMaterial(&g_D3DMaterial);
//               if (hr != 0) { 
//                  hr2message(__LINE__,hr,"g_pDev->SetMaterial(shoot)");
//                  return(FALSE);
//               }
            
//               if (l_pshootCurr->m_pEquipment == g_equipmentArray+EQUIPMENT_MININGLASER)
//               {
//                  g_matGeneric1._11 = l_pshootCurr->m_vStep.x;
//                  g_matGeneric1._12 = l_pshootCurr->m_vStep.y;
//                  g_matGeneric1._13 = l_pshootCurr->m_vStep.z;
//               }else{
//                  g_matGeneric1._11 = l_pshootCurr->m_vStep.x / 4.0f;
//                  g_matGeneric1._12 = l_pshootCurr->m_vStep.y / 4.0f;
//                  g_matGeneric1._13 = l_pshootCurr->m_vStep.z / 4.0f;
//               }
//
//               g_matGeneric1._41 = l_pshootCurr->m_vPos.x;
//               g_matGeneric1._42 = l_pshootCurr->m_vPos.y;
//               g_matGeneric1._43 = l_pshootCurr->m_vPos.z;
//               hr = g_pDev->SetTransform(D3DTS_WORLD, &g_matGeneric1);
//               if (hr != 0) { 
//                  hr2message(__LINE__,hr,"g_pDev->SetTransform"
//                                           "(shoot)");
//                  return(false);
//               }
               
//               hr = g_pDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2 ) ;
//               if (hr != 0) { 
//                  hr2message(__LINE__,hr, "g_pDev->DrawIndexedPrimitive(shoot)");
//                  return(false);
//               }
//            }

				/*
            l_pshootCurr++;
			l_cntShootPosToDraw++;
			
         } // end-for l_idxShoot
		*/
	
	
	
/*
   // - temporär 2D mode
   // 
   hr = g_pDev->SetTransform(D3DTS_VIEW, &g_matIdentity);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetTransform(VIEW)");
      return false;
   }

   hr = g_pDev->SetTransform(D3DTS_PROJECTION, &g_matIdentity);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetTransform(PROJ)");
      return false;
   }

   if (g_bEnableTextures)
   {
      if (g_txtArray[TID_SYMBOL].activate(0) == false) return false;
   }
   
   // --- Auf Weiss schalten
   //
   memset(&g_D3DMaterial, 0, sizeof(g_D3DMaterial));
   g_D3DMaterial.Diffuse  = g_d3dcolWhite;
   g_D3DMaterial.Ambient  = g_d3dcolWhite; 
   g_D3DMaterial.Specular = g_d3dcolWhite;
   g_D3DMaterial.Emissive = g_d3dcolWhite;
   g_D3DMaterial.Power    = 10.0f;

   hr = g_pDev->SetMaterial(&g_D3DMaterial);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetMaterial(white)");
      return false;
   }

   hr = g_pDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD); 
   if (hr != 0) { 
      hr2message(__LINE__,hr,
                 "g_pDev->SetRenderState(BLENDOP)");
      return(FALSE);
   };

   //--- die einzelnen Planeten der Galaxysternenkarte zeichnen
   //
   if (l_renderpassCurr == RENDERPASS_FULLSCREEN)
   {
      if (g_Player.getGS()->displayGalaxyMapStars()==false) return false;
   }

   hr = g_pDev->SetStreamSource(0, g_lpD3DBillbVB, 0, sizeof(PSXVERTEX));
   if (hr != 0) { 
      hr2message(__LINE__,hr,
                 "g_pDev->SetStreamSource(shoot)");
      return(false);
   }

   // --- Auf dunkelgrau schalten, das wird abgezogen
   //
   {
       D3DCOLORVALUE l_d3dcolOverlay = { 0.3f, 0.3f, 0.3f, 0.0f };
       memset(&g_D3DMaterial, 0, sizeof(g_D3DMaterial));
       g_D3DMaterial.Diffuse  = l_d3dcolOverlay;
       g_D3DMaterial.Ambient  = l_d3dcolOverlay; 
       g_D3DMaterial.Specular = l_d3dcolOverlay;
       g_D3DMaterial.Emissive = l_d3dcolOverlay;
       g_D3DMaterial.Power    = 1.0f;
   }
   hr = g_pDev->SetMaterial(&g_D3DMaterial);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetMaterial(white)");
      return false;
   }

   hr = g_pDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT); 
   if (hr != 0) { 
      hr2message(__LINE__,hr,
                 "g_pDev->SetRenderState(BLENDOP)");
      return(FALSE);
   };


   hr = g_pDev->SetTexture(0, NULL);
   if (hr != 0) { 
       hr2message(__LINE__,hr,"g_pDev->SetTexture(Galaxymap)");
       return false;
   }

   // -- Darkenbox für Target-Cargoanzeige
   //
   if (l_renderpassCurr == RENDERPASS_FULLSCREEN && 
       g_Player.getGS()->IsHavingTargetFrame() )
   {
      PSXVECTOR l_vOffset;

      // -- die momentante Selektion einrahmen
      //
      l_vOffset.y = -0.1f;
      if ((ENEMY::getEnemyTarget())->getPos2DX() > 0.0f)
      { 
         l_vOffset.x = 0.50f;
      }else{
         l_vOffset.x = - 0.50f;
      }

      memcpy (&g_matGeneric1,&g_matIdentity, sizeof(g_matIdentity));
      g_matGeneric1._11 = 0.03f;
      g_matGeneric1._22 = 0.06f;
      g_matGeneric1._33 = 1.0f;

      g_matGeneric1._41 = l_vOffset.x;
      g_matGeneric1._42 = l_vOffset.y;
      g_matGeneric1._43 = 1.0;

      hr = g_pDev->SetTransform(D3DTS_WORLD, &g_matGeneric1);
      if (hr != 0) { 
               hr2message(__LINE__,hr,"g_pDev->SetTransform(darkenbox)");
         return false;
      }

      hr = g_pDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2 ) ;

      if (hr != 0) { 
         hr2message(__LINE__,hr, "g_pDev->DrawIndexedPrimitive(darkenbox)");
         return false;
      }
   } //IF ende FullScreen

   // -- Darkenbox für SelectionFrame zeichnen
   //
   if (l_renderpassCurr == RENDERPASS_FULLSCREEN)
   {
      if (g_Player.getGS()->displaySelectionFrameDarkenBox()== false) return false;
   }

   hr = g_pDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD); 
   if (hr != 0) { 
      hr2message(__LINE__,hr,
                 "g_pDev->SetRenderState(BLENDOP)");
      return(FALSE);
   };

   hr = g_pDev->SetTransform(D3DTS_VIEW, &d3dmatrix_view);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetTransform(VIEW2)");
      return(FALSE);
   }
   
   switch(l_renderpassCurr)
   {
      case RENDERPASS_BACKWARD:
         g_projRearView.updateMatrix();
         hr = g_pDev->SetTransform(D3DTS_PROJECTION, &g_projRearView.m_mat);
         break;
      default:
         g_projViewport.updateMatrix();
         hr = g_pDev->SetTransform(D3DTS_PROJECTION, &g_projViewport.m_mat);
   }
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetTransform(PROJ)");
      return(FALSE);
   }

   // ------ Impacts
   //
   if (g_bWriteLogFileInLoop) 
   {
      WriteLogFile("draw EDRAW_IMPACT Particle-System\n");
   }

   if (g_bEnableTextures)
   {
      if (g_txtArray[TID_SMOKE].activate(0) == false) return false;
   }

   hr = g_pDev->SetRenderState(D3DRS_POINTSCALEENABLE,FALSE);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetRenderState(POINTSCALEENABLE)");
      return false;
   };

   hr = g_pDev->SetStreamSource(0, g_d3dExplosionVB, 0, sizeof(PSXVECTOR));
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetStreamSource(Explosion)");
      return false;
   }

   hr = g_pDev->SetVertexShader(NULL);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetVertexShader(VECTOR)");
      return(FALSE);
   }
   hr = g_pDev->SetVertexDeclaration(g_pvdeclPSXVector);
   if (hr != 0) { 
	   hr2message(__LINE__,hr, "g_pDev->SetVertexDeclaration(PSXVECTOR)");
	   return false;
   }

   hr = g_pDev->SetRenderState(D3DRS_POINTSPRITEENABLE,TRUE);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetRenderState(POINTSPRITEENABLE)");
      return(FALSE);
   }

   {
      int l_idxEnemy;
      float l_fPointSize = 10.0f;
      float l_fScale;

      hr = g_pDev->SetRenderState(D3DRS_POINTSIZE, *((DWORD*)&l_fPointSize));
      if (hr != 0) { 
         hr2message(__LINE__,hr,"g_pDev->SetRenderState(POINTSIZE)");
         return(FALSE);
      }

      for (l_idxEnemy = 0; 
           l_idxEnemy < g_arr_EnemyDrawer[EDRAW_IMPACT].m_cntDraw; 
           l_idxEnemy ++)
      {
         ENEMY* l_penemyCurr;

         {
            int l_idxDraw;
            l_idxDraw = g_arr_EnemyDrawer[EDRAW_IMPACT].m_idxFirst + l_idxEnemy;
            l_penemyCurr = g_arr_pEnemy[l_idxDraw];

         }

         // --- Explosions spritehelligkeit
         //
         {
             D3DCOLORVALUE d3dcolor_explosion;

             d3dcolor_explosion.r = 
                float(g_maxExplosionSteps-l_penemyCurr->m_cntGeneric)/
                float(g_maxExplosionSteps);

             d3dcolor_explosion.g = d3dcolor_explosion.r;
             d3dcolor_explosion.b = d3dcolor_explosion.r;
      
             memset(&g_D3DMaterial, 0, sizeof(g_D3DMaterial));
             g_D3DMaterial.Diffuse  = d3dcolor_explosion;
             g_D3DMaterial.Ambient  = d3dcolor_explosion; 
             g_D3DMaterial.Specular = d3dcolor_explosion;
             g_D3DMaterial.Emissive = d3dcolor_explosion;
             g_D3DMaterial.Power    = 1.0f;
         }

         hr = g_pDev->SetMaterial(&g_D3DMaterial);
         if (hr != 0) { 
            hr2message(__LINE__,hr,"g_pDev->SetMaterial(explosion)");
            return false;
         }

         l_fScale = float(l_penemyCurr->m_cntGeneric)/ float(g_maxExplosionSteps);

         memcpy(&g_matGeneric1,&g_matIdentity, sizeof(g_matIdentity));
         g_matGeneric1._11 = l_fScale * 5.0f;
         g_matGeneric1._22 = l_fScale * 5.0f;
         g_matGeneric1._33 = l_fScale * 5.0f;

         g_matGeneric1._41 = l_penemyCurr->getPosX();
         g_matGeneric1._42 = l_penemyCurr->getPosY();
         g_matGeneric1._43 = l_penemyCurr->getPosZ();

         hr = g_pDev->SetTransform(D3DTS_WORLD, &g_matGeneric1);
         if (hr != 0) { 
            hr2message(__LINE__,hr,"g_pDev->SetTransform(explosionshadebob)");
            return(FALSE);
         }

         
         hr = g_pDev->DrawPrimitive( D3DPT_POINTLIST, 0, g_cntExplVert);
         if (hr != 0) { 
            hr2message(__LINE__,hr,"g_pDev->DrawPrimitive(explosion-POINTLIST)");
            return(FALSE);
         }
      }
   }


   // -- SmokeTrays zeichnen...
   //
   if (g_cntSmokeTrays>0) // || g_cnt   >0)
   {  

      if (g_bEnableTextures)
      {
         if (g_txtArray[TID_SHADEBOB].activate(0) == false) return false;
      }

      {
         float l_fPointSize;
   
         l_fPointSize = 7.0f;// g_arr_pEnemy[l_idxDraw]->m_cntGeneric * g_arr_pEnemy[l_idxDraw]->m_fExplScale;
         hr = g_pDev->SetRenderState(D3DRS_POINTSIZE, *((DWORD*)&l_fPointSize));
         if (hr != 0) { 
            hr2message(__LINE__,hr,"g_pDev->SetRenderState(POINTSIZE)");
            return false;
         };
      }

   }

   if (g_cntSmokeTrays>0)
   {

      float l_fScaleBillboard=3.0f;
      float l_fScale=10.0f;
      float l_fScaleExtension=0.0f;

      D3DCOLORVALUE d3dcolor_explosion = {0};

      D3DMATRIX l_matParticle;

      // -- vorbereitung des material records
      //
      memset(&g_D3DMaterial, 0, sizeof(g_D3DMaterial));
      g_D3DMaterial.Power = 1.0f;

      for(int i=0;i<g_maxSmokeFor;i++) 
      {
         if(smoketray_array[i].penemy!=NULL && 
            smoketray_array[i].penemy->mType!=ETYP_EMPTY)
         {

            for(int k=0;k<smoketray_array[i].maxShadebobs;k++)
            {
              if(smoketray_array[i].active[k] &&
                 smoketray_array[i].cntTimeToLive[k]>=0)
              {
                 d3dcolor_explosion.r =
                    float(smoketray_array[i].cntTimeToLive[k])/
                    float(smoketray_array[i].maxTimeToLive*4.0f);
                         
                 d3dcolor_explosion.g = d3dcolor_explosion.r;
                 d3dcolor_explosion.b = d3dcolor_explosion.r;

                 g_D3DMaterial.Ambient  = d3dcolor_explosion; 
         
                 hr = g_pDev->SetMaterial(&g_D3DMaterial);
                 if (hr != 0) { 
                   hr2message(__LINE__,hr,"g_pDev->SetMaterial(explosion)");
                   return false;
                 }

                 //memcpy (&l_matParticle,&smoketray_array[i].mat[k], sizeof(D3DMATRIX));

                 l_fScale = float (smoketray_array[i].maxTimeToLive -
                                   smoketray_array[i].cntTimeToLive[k])/
                            float (smoketray_array[i].maxTimeToLive);
                                   //smoketray_array[i].penemy->getRad()*0.1f;

                 l_fScaleExtension = 32.0f;
          
                 l_matParticle._11 = smoketray_array[i].mat[k]._11 * (l_fScale*l_fScaleExtension);
                 l_matParticle._12 = smoketray_array[i].mat[k]._12 * (l_fScale*l_fScaleExtension);
                 l_matParticle._13 = smoketray_array[i].mat[k]._13 * (l_fScale*l_fScaleExtension);
                 l_matParticle._14 = 0.0f;
                 l_matParticle._21 = smoketray_array[i].mat[k]._21 * (l_fScale*l_fScaleExtension);
                 l_matParticle._22 = smoketray_array[i].mat[k]._22 * (l_fScale*l_fScaleExtension);
                 l_matParticle._23 = smoketray_array[i].mat[k]._23 * (l_fScale*l_fScaleExtension);
                 l_matParticle._24 = 0.0f;
                 l_matParticle._31 = smoketray_array[i].mat[k]._31 * (l_fScale*l_fScaleExtension);
                 l_matParticle._32 = smoketray_array[i].mat[k]._32 * (l_fScale*l_fScaleExtension);
                 l_matParticle._33 = smoketray_array[i].mat[k]._33 * (l_fScale*l_fScaleExtension);
                 l_matParticle._34 = 0.0f;

                 l_matParticle._41 = smoketray_array[i].vPos[k].x ;
                 l_matParticle._42 = smoketray_array[i].vPos[k].y ;
                 l_matParticle._43 = smoketray_array[i].vPos[k].z ;
                 l_matParticle._44 = 1.0f ;
  

                 hr = g_pDev->SetTransform(D3DTS_WORLD, &l_matParticle);
                 if (hr != 0) { 
                    hr2message(__LINE__,hr,"g_pDev->SetTransform"
                                             "(mine/redflare)");
                     return false;
                 }

                 hr = g_pDev->DrawPrimitive( D3DPT_POINTLIST, 0, g_cntExplVert);
                 if (hr != 0) { 
                    hr2message(__LINE__,hr,"g_pDev->DrawPrimitive(explosion-POINTLIST)");
                    return(FALSE);
                 }
              }
            }
         }
      }//ENDFOR
   }


   hr = g_pDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD); 
   if (hr != 0) { 
       hr2message(__LINE__,hr,
                 "g_pDev->SetRenderState(BLENDOP)");
       return(FALSE);
   };

   hr = g_pDev->SetRenderState( D3DRS_FOGENABLE, FALSE); 
   if (hr != 0) { 
   hr2message(__LINE__,hr,"g_pDev->"
                               "SetRenderState(FOGENABLE=FALSE)");
      return false;
   }
 

   // -- Pointlists wieder in Normalzustand versetzen
   //
   {
      float l_fPointSize = 1.0f;// g_arr_pEnemy[l_idxDraw]->m_cntGeneric * g_arr_pEnemy[l_idxDraw]->m_fExplScale;

      hr = g_pDev->SetRenderState(D3DRS_POINTSIZE, *((DWORD*)&l_fPointSize));
      if (hr != 0) { 
         hr2message(__LINE__,hr,"g_pDev->SetRenderState(POINTSIZE)");
         return false;
      };
   }

   hr = g_pDev->SetRenderState(D3DRS_POINTSCALEENABLE,FALSE);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetRenderState(POINTSCALEENABLE=FALSE)");
      return false;
   };

   hr = g_pDev->SetRenderState(D3DRS_POINTSPRITEENABLE,FALSE);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetRenderState(POINTSPRITEENABLE=FALSE)");
      return false;
   };


   // -- Die reine Vektor sektion 
   //    ab hier drahtgittermodelle
   //  
   hr = g_pDev->SetTexture(0, NULL);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->" "SetTexture(shoot)");
      return false;
   }

   {
       D3DCOLORVALUE l_d3dcolOverlay = { 0.1f, 0.2f, 0.1f, 0.1f };
       memset(&g_D3DMaterial, 0, sizeof(g_D3DMaterial));
       g_D3DMaterial.Diffuse  = l_d3dcolOverlay;
       g_D3DMaterial.Ambient  = l_d3dcolOverlay; 
       g_D3DMaterial.Specular = l_d3dcolOverlay;
       g_D3DMaterial.Emissive = l_d3dcolOverlay;
       g_D3DMaterial.Power    = 1.0f;
   }

   hr = g_pDev->SetMaterial(&g_D3DMaterial);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetMaterial(WHITE)");
      return(FALSE);
   }

   hr = g_pDev->SetRenderState(D3DRS_ZENABLE,D3DZB_FALSE);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->"
                               "SetRenderState(Z-DISABLE)");
      return false;
   }


   // -- Markieren wohin der Spieler fliegen soll
   //
   if (g_bWriteLogFileInLoop) 
   {
      WriteLogFile("draw starbase destination marker\n");
   }


   if (g_bTargetLocked)
   {
      g_D3DMaterial.Diffuse  = g_d3dcolRed;
      g_D3DMaterial.Ambient  = g_d3dcolRed; 
      g_D3DMaterial.Specular = g_d3dcolRed;
      g_D3DMaterial.Emissive = g_d3dcolRed;
      g_D3DMaterial.Power    = 1.0f;
   }else{
      g_D3DMaterial.Diffuse  = g_d3dcolGreen;
      g_D3DMaterial.Ambient  = g_d3dcolGreen; 
      g_D3DMaterial.Specular = g_d3dcolGreen;
      g_D3DMaterial.Emissive = g_d3dcolGreen;
      g_D3DMaterial.Power    = 1.0f;
   }
   hr = g_pDev->SetMaterial(&g_D3DMaterial);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->"
                               "SetMaterial(foreach material)");
      return false;
   }

   hr = g_pDev->SetRenderState( D3DRS_CULLMODE,D3DCULL_CW); 
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetRenderState(CULLMODE)");
      return(FALSE);
   } 

   //-- Richtungszeiger StarBase
   //
   if (l_renderpassCurr == RENDERPASS_FULLSCREEN && 
       g_Player.getGS()->IsHavingDirectionPointer() &&
       g_penemyCamera)
   {
      float l_fScale=0.05f;

      D3DMATRIX l_matStaPointTrans;
      memcpy(&l_matStaPointTrans, &g_matIdentity, sizeof(g_matIdentity));
      l_matStaPointTrans._41= 8.50f;
      l_matStaPointTrans._42=-18.35f;
      l_matStaPointTrans._43= 30.00f;

      hr = g_pDev->SetTransform(D3DTS_VIEW, &l_matStaPointTrans);
      if (hr != 0) { 
         hr2message(__LINE__,hr,"g_pDev->SetTransform(VIEW)");
         return false;
      }

      hr = g_pDev->SetIndices( g_d3dStarBasePointerIB);
      if (hr != 0) { 
         hr2message(__LINE__,hr,"g_pDev->SetIndices(STATIONPOINTER)");
         return(false);
      }

      hr = g_pDev->SetStreamSource(0, g_d3dStarBasePointerVB, 0, sizeof(PSXVECTOR));
      if (hr != 0) { 
         hr2message(__LINE__,hr,"g_pDev->SetStreamSource(POINTER01)");
         return(false);
      }

      matmult(&g_matGeneric1, &g_matCompassYZ, &g_matCamera);
      hr = g_pDev->SetTransform(D3DTS_WORLD, &g_matGeneric1);
      if (hr != 0) { 
         hr2message(__LINE__,hr,"g_pDev->SetTransform(Cross)");
         return false;
      }


      hr = g_pDev->DrawIndexedPrimitive(D3DPT_LINELIST, 0, 0,6,0,5);
      if (hr != 0)
      { 
         hr2message(__LINE__,hr, "g_pDev->DrawIndexedPrimitive(StarBasePointer)");
         return false;
      }


      //-- Jetzt Rueckseite zeichen
      //
      hr = g_pDev->SetIndices( g_d3dStarBasePointerBsIB);
      if (hr != 0) { 
         hr2message(__LINE__,hr,"g_pDev->SetIndices(POINTER01)");
         return(false);
      }
  
      hr = g_pDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 6, 0, 2 );
      if (hr != 0) { 
         hr2message(__LINE__,hr, "g_pDev->DrawIndexedPrimitive(Menu)");
         return false;
      }
   }

   // --- Auf Weiss schalten
   //
   memset(&g_D3DMaterial, 0, sizeof(g_D3DMaterial));
   g_D3DMaterial.Diffuse  = g_d3dcolWhite;
   g_D3DMaterial.Ambient  = g_d3dcolWhite; 
   g_D3DMaterial.Specular = g_d3dcolWhite;
   g_D3DMaterial.Emissive = g_d3dcolWhite;
   g_D3DMaterial.Power    = 10.0f;

   hr = g_pDev->SetMaterial(&g_D3DMaterial);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetMaterial(white)");
      return false;
   }

   hr = g_pDev->SetRenderState( D3DRS_CULLMODE,D3DCULL_NONE); 
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->"
                               "SetRenderState(CULLMODE)");
      return(FALSE);
   } 


   //---Radar
   //
   if (l_renderpassCurr == RENDERPASS_FULLSCREEN)
   {
      if (g_Player.getGS()->displayRadar() == false) return false;
   }

   hr = g_pDev->SetVertexShader( NULL);
   if (hr != 0) { 
      hr2message(__LINE__,hr, "g_pDev->SetVertexShader(Billboardloop)");
      return false;
   }
   hr = g_pDev->SetVertexDeclaration(g_pvdeclPSXVertex);
   if (hr != 0) { 
	   hr2message(__LINE__,hr, "g_pDev->SetVertexDeclaration(PSXVERTEX)");
	   return false;
   }

   // --- Auf Weiss schalten
   //
   memset(&g_D3DMaterial, 0, sizeof(g_D3DMaterial));
   g_D3DMaterial.Diffuse  = g_d3dcolWhite;
   g_D3DMaterial.Ambient  = g_d3dcolWhite; 
   g_D3DMaterial.Specular = g_d3dcolWhite;
   g_D3DMaterial.Emissive = g_d3dcolWhite;
   g_D3DMaterial.Power    = 10.0f;

   hr = g_pDev->SetMaterial(&g_D3DMaterial);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetMaterial(white)");
      return false;
   }


   if (l_renderpassCurr == RENDERPASS_FULLSCREEN) 
   {
      if (g_Player.getGS()->displayRadarPointer()==false) return false;
   }


   if (g_Player.getGS()->displayPlasmaShader() == false) return false;
   **/
   return true;
} 

// -- Draw the stuff which needs identity as projection matrix
//
bool PSXDoRenderStuff2D(RENDERPASS l_renderpassCurr)
{
   
    HRESULT hr = NULL;
   /*
   //
   // -- activating 2D mode
   // 
   if (g_bWriteLogFileInLoop) 
   {
      WriteLogFile("2d mode (all matrices to identity)\n");
   }
   hr = g_pDev->SetTransform(D3DTS_VIEW, &g_matIdentity);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetTransform(VIEW)");
      return false;
   }

   hr = g_pDev->SetTransform(D3DTS_PROJECTION, &g_matIdentity);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetTransform(PROJ)");
      return false;
   }

   hr = g_pDev->SetTexture(0, NULL);
   if (hr != 0) { 
       hr2message(__LINE__,hr,"g_pDev->SetTexture(Galaxymap)");
       return false;
   }

   hr = g_pDev->SetVertexShader(NULL);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetVertexShader(VECTOR)");
      return(FALSE);
   }
   hr = g_pDev->SetVertexDeclaration(g_pvdeclPSXVector);
   if (hr != 0) { 
	   hr2message(__LINE__,hr, "g_pDev->SetVertexDeclaration(PSXVECTOR)");
	   return false;
   }

   if (l_renderpassCurr == RENDERPASS_FULLSCREEN)
   {
       //--- die derzeitigen Planeten in der Karte durch Kreuz markieren
       //
       if (g_Player.getGS()->displayGalaxyMapCross() == false) return false;

       // --- permanent DestFrame
       //
       if (g_Player.getGS()->displayDestinationFrame() == false) return false;
   }
   */

/*

   // -- den selektierten Planeten markieren und Info dazu einblenden
   //
   if (l_renderpassCurr == RENDERPASS_FULLSCREEN)
   {
      if (g_Player.getGS()->displaySelectionFrameLines() == false) return false;
   }

   hr = g_pDev->SetIndices( g_lpD3DBillbIB);
   if (hr != 0) { 
      hr2message(__LINE__,hr, "g_pDev->SetIndices(lensflare)");
      return false;
   }

   hr = g_pDev->SetVertexShader( NULL);
   if (hr != 0) { 
      hr2message(__LINE__,hr, "g_pDev->SetVertexShader(lensflare)");
      return false;
   }
   hr = g_pDev->SetVertexDeclaration(g_pvdeclPSXVertex);
   if (hr != 0) { 
	   hr2message(__LINE__,hr, "g_pDev->SetVertexDeclaration(PSXVERTEX)");
	   return false;
   }

   hr = g_pDev->SetStreamSource(0, g_lpD3DBillbVB, 0, sizeof(PSXVERTEX));
   if (hr != 0) { 
      hr2message(__LINE__,hr, "g_pDev->SetStreamSource(lensflare)");
      return false;
   }
	*/

   // -- painting halo line to sun for debug purpose
   //
   if (g_bWriteLogFileInLoop) 
   {
      WriteLogFile("painting lensflares\n");
   }

	//
   // -- Contant Buffer
   //
   {

       XMMATRIX l_matWorld;
       ENEMYDRAWERCONSTANTBUFFER::cbChangeEveryFrame l_cbChangeEveryFrame;
       l_cbChangeEveryFrame.mView = XMMatrixTranspose( g_View );    
       XMFLOAT4 l_float4Camera;
       XMStoreFloat4(&l_float4Camera, g_vCamera);
       l_cbChangeEveryFrame.vCamera = l_float4Camera;
       g_pImmediateContext->UpdateSubresource( ENEMYDRAWER::m_pcbChangeEveryFrame, 
		                               0, nullptr, &l_cbChangeEveryFrame, 0, 0 );
   
       ENEMYDRAWERCONSTANTBUFFER::cbChangeOnResize l_cbChangesOnResize;
       l_cbChangesOnResize.mProjection = XMMatrixTranspose( g_Projection );
       g_pImmediateContext->UpdateSubresource( ENEMYDRAWER::m_pcbChangeOnResize, 
		                               0, nullptr, &l_cbChangesOnResize, 0, 0 );


       ENEMYDRAWERCONSTANTBUFFER::cbChangeEveryObject cb1;    
       cb1.mWorld = XMMatrixIdentity();
       //cb1.matWorldViewProj= XMMatrixTranspose( g_View * g_Projection );
       cb1.matWorldViewProj= XMMatrixIdentity();
       g_pImmediateContext->UpdateSubresource(ENEMYDRAWER::m_pcbChangeEveryObject, 0, nullptr, &cb1, 0, 0 );     

   }

   // -- setting to transparent : alpha blending and depth-test
   //
   g_pImmediateContext->OMSetBlendState(g_pd3dblendstateTransparent, NULL, 0xffffffff);
   g_pImmediateContext->OMSetDepthStencilState(g_pd3ddepthstateTransparent, 0);

   // Set the input layout
   g_pImmediateContext->IASetInputLayout(g_pSpriteLayout);

   g_pImmediateContext->VSSetShader( g_pVertexShaderSpriteDrawer, nullptr, 0 );
   g_pImmediateContext->VSSetConstantBuffers( 0, 1, &ENEMYDRAWER::m_pcbChangeEveryObject );
   g_pImmediateContext->VSSetConstantBuffers( 1, 1, &ENEMYDRAWER::m_pcbChangeOnResize );
   g_pImmediateContext->VSSetConstantBuffers( 2, 1, &ENEMYDRAWER::m_pcbChangeEveryFrame );
   g_pImmediateContext->PSSetShader( g_pPixelShaderSpriteDrawer, nullptr, 0 );
   g_pImmediateContext->PSSetConstantBuffers( 0, 1, &ENEMYDRAWER::m_pcbChangeEveryObject );
   g_pImmediateContext->PSSetConstantBuffers( 1, 1, &ENEMYDRAWER::m_pcbChangeOnResize );
   g_pImmediateContext->PSSetConstantBuffers( 2, 1, &ENEMYDRAWER::m_pcbChangeEveryFrame );
   g_pImmediateContext->GSSetShader(g_pSpriteGeometryShader, nullptr, 0);
   g_pImmediateContext->GSSetConstantBuffers(0, 1, &ENEMYDRAWER::m_pcbChangeEveryObject);
   g_pImmediateContext->GSSetConstantBuffers(1, 1, &ENEMYDRAWER::m_pcbChangeOnResize);
   g_pImmediateContext->GSSetConstantBuffers(2, 1, &ENEMYDRAWER::m_pcbChangeEveryFrame);
   
   g_pImmediateContext->PSSetSamplers( 0, 1, &ENEMYDRAWER::m_pSamplerLinear );
   

	// Set primitive topology
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

   
	
	//   PSXPrintf(__LINE__, debugausgabe, g_maxDebugAusgabe, "%s %i - Dest: %f, %f, %f ",
	//

	//
   // -- sonne
   //
   //if (g_Player.m_bEntered==false && g_Player.m_pLevel->m_bDayLight)
   {
      float halolinevert[6] = { 0,0,0, 0.1,0.1,0};
      float sunx,suny,sunz,sun_div;
		XMVECTOR l_vSunProjected;
		XMVECTOR l_vSunCoord = {0,0,100.0f,1.0f};
      bool l_bShallDraw;
		SPRITE* l_pSprite = NULL;

		struct _lensflare {
   		int textureid;
   		float lengthscale;
   		float imagescale;
		} lensflare[] = {
      	{ TID_LENS1, 1.0f, 1.0f },
      	{ TID_LENS1, 1.0f/3.0f, 0.25f},
      	{ TID_LENS1, -1.0f/2.0f, 0.5f},
      	{ TID_LENS1, -1.0f/5.5f, 0.25f},
      	{ TID_LENS2, 1.0f/2.0f, 0.5f },
      	{ TID_LENS3, 1.0f/8.0f, 1.0f },
      	{ TID_LENS4, -1.0f/4.0f, 0.25f},
      };
		const int max_lensflare = sizeof(lensflare) / sizeof(lensflare[0]);

		l_bShallDraw = false;

		// -- using matrices against the sun coordinates
        //
		l_vSunProjected = XMVector3Transform(l_vSunCoord, g_View * g_Projection);

		// -- perspektive
		//
		sun_div = XMVectorGetW(l_vSunProjected);
		sunx = XMVectorGetX(l_vSunProjected);
		suny = XMVectorGetY(l_vSunProjected);
		sunz = XMVectorGetZ(l_vSunProjected);
	  
	   sunx = sunx/sun_div;
      suny = suny/sun_div;
      sunz = sunz/sun_div;

      if (sunx < 1.1 && sunx > -1.1 &&
          suny < 1.1 && suny > -1.1 &&
          sunz < 1.0 )
      {
	      D3D11_MAPPED_SUBRESOURCE l_mappedsubInitData;
			
			l_bShallDraw = true;
			int l_tidCurrentLensflare = TID_NULL;
			int cnt_lensflare = 0;
			UINT stride = sizeof(SPRITE);
			UINT offset = 0;

			
			hr = g_pImmediateContext->Map(
					g_pSpriteBuffer,             //ID3D11Resource *pResource,
					0,                           //UINT Subresource,
					D3D11_MAP_WRITE_DISCARD,     //D3D11_MAP MapType,
					0,                           //UINT MapFlags,
					&l_mappedsubInitData);       //D3D11_MAPPED_SUBRESOURCE *pMappedResource
			if (FAILED(hr))
			{
				hr2message(__LINE__, hr, "g_pd3dDevice->Map()");
				return false;
			}
				
			SPRITE* l_pSprite = reinterpret_cast<SPRITE*> (l_mappedsubInitData.pData);

			l_tidCurrentLensflare = lensflare[0].textureid;
			if (g_txtArray[l_tidCurrentLensflare].activate(0) == false) return false;

         for (int idx_lensflare=0;
                  idx_lensflare<max_lensflare;
                  idx_lensflare++)
         {
				if (l_tidCurrentLensflare != lensflare[idx_lensflare].textureid) 
				{
					g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pSpriteBuffer, &stride, &offset);
			      g_pImmediateContext->Unmap(g_pSpriteBuffer, 0);
					g_pImmediateContext->Draw(cnt_lensflare, 0);

			   	l_tidCurrentLensflare = lensflare[idx_lensflare].textureid;
					cnt_lensflare = 0;

					if (g_txtArray[l_tidCurrentLensflare].activate(0) == false) return false;

					hr = g_pImmediateContext->Map(
						 g_pSpriteBuffer,             //ID3D11Resource *pResource,
						 0,                           //UINT Subresource,
						 D3D11_MAP_WRITE_DISCARD,     //D3D11_MAP MapType,
						 0,                           //UINT MapFlags,
						 &l_mappedsubInitData);       //D3D11_MAPPED_SUBRESOURCE *pMappedResource
					if (FAILED(hr))
					{
						 hr2message(__LINE__, hr, "g_pd3dDevice->Map()");
						 return false;
					}
					l_pSprite = reinterpret_cast<SPRITE*> (l_mappedsubInitData.pData);

				}
				//
   			// -- umschaufeln in den Sprite-Positionsbuffer
				//
				
				memset(l_pSprite+cnt_lensflare, 0, sizeof(l_pSprite[0]));
			   l_pSprite[cnt_lensflare].Pos.x = sunx*lensflare[idx_lensflare].lengthscale;
			   l_pSprite[cnt_lensflare].Pos.y = suny*lensflare[idx_lensflare].lengthscale;
			   l_pSprite[cnt_lensflare].Pos.z = 0.9f;
			   l_pSprite[cnt_lensflare].Pos.w = 1.0f;

			   l_pSprite[cnt_lensflare].dimension.x = 0.2f * lensflare[idx_lensflare].imagescale;
			   l_pSprite[cnt_lensflare].dimension.y = 0.2f * lensflare[idx_lensflare].imagescale;

			   l_pSprite[cnt_lensflare].opacity = 1.0f;

				cnt_lensflare++;
					
         }
			g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pSpriteBuffer, &stride, &offset);
			g_pImmediateContext->Unmap(g_pSpriteBuffer, 0);
			g_pImmediateContext->Draw(cnt_lensflare, 0);
      }

   }

	/*
   // -- cockpit malen
   //
   if (l_renderpassCurr == RENDERPASS_FULLSCREEN )
   {
     if (g_Player.getGS()->displayCockpitBillboard() == false) return false;
     if (g_Player.getGS()->displayCrossHair() == false) return false;
   }

   hr = g_pDev->SetTransform(D3DTS_WORLD, &g_matIdentity);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetTransform(WORLD)");
      return(FALSE);
   }

*/

	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_pImmediateContext->IASetInputLayout( g_pVertexLayout );
   // -- Info bereich
   //
   if (debugausgabe && strlen(debugausgabe))
   {
	   wchar_t l_wcDebugText[100];
	   size_t l_cntConvertedChars;
	   size_t l_cntDebugTextChars = strlen(debugausgabe) + 1;
	   mbstowcs_s(&l_cntConvertedChars, l_wcDebugText, l_cntDebugTextChars, debugausgabe, _TRUNCATE);

	   g_pd2dRenderTarget->BeginDraw();

	   D2D1_SIZE_F renderTargetSize = g_pd2dRenderTarget->GetSize();

	   //g_pd2dRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
	   g_pd2dRenderTarget->DrawTextW(l_wcDebugText, l_cntConvertedChars, //wcslen(debugausgabe),
		   g_pdwriteTextFormat,
		   D2D1::RectF(0, 0, renderTargetSize.width, renderTargetSize.height),
		   g_pd2dSolidWhiteBrush, D2D1_DRAW_TEXT_OPTIONS_NO_SNAP, DWRITE_MEASURING_MODE_GDI_CLASSIC);

	   ////g_pd2dRenderTarget->DrawRectangle(
		  //// g_d2drectClientRect,
		  //// g_pd2dSolidWhiteBrush);

	   g_pd2dRenderTarget->EndDraw();
   }

/*
   if (l_renderpassCurr == RENDERPASS_FULLSCREEN)
   {

      if (g_bWriteLogFileInLoop) 
      {
         WriteLogFile("preparing for drawing Text\n");
      }
   
      // -- Textausgabe testen
      //
      if (g_bEnableTextures)
      {
         if (g_txtArray[TID_FONT].activate(0) == false) return false;
      }
      if (g_bCheatGodMode)
      {
         //PSXPrintf(__LINE__,debugausgabe, 800, "g_Player.m_bEntered %i", g_Player.m_bEntered);
		  
         PSXTextOut(FONT_TERMINAL, debugausgabe,-1.0,1.0f);
      }

      if (g_bWriteLogFileInLoop) 
      {
         WriteLogFile("printing Terminallines\n");
      }
      PSXUpdateTerminal();
      
      // --- printing and updating Mission Text
      //
      if (g_bWriteLogFileInLoop) 
      {
         WriteLogFile("printing StarBaseintroducing Missiontext\n");
      }

      if (g_bGameHintTextAvailable==true){

         PSXInitializeTerminal(g_cGameHintText[0],
                               NULL,
                               NULL,
                               NULL);

         g_bGameHintTextAvailable = false;
         g_cGameHintText[0]        = NULL;
      }
   
      if (g_Player.getGS()->displayMissionText() == false) return false;
     
      memset(&g_D3DMaterial, 0, sizeof(g_D3DMaterial));
      g_D3DMaterial.Diffuse  = g_d3dcolWhite;
      g_D3DMaterial.Ambient  = g_d3dcolWhite; 
      g_D3DMaterial.Specular = g_d3dcolWhite;
      g_D3DMaterial.Emissive = g_d3dcolWhite;
      g_D3DMaterial.Power    = 1.0f;
   
      hr = g_pDev->SetMaterial(&g_D3DMaterial);
      if (hr != 0) { 
         hr2message(__LINE__,hr,"g_pDev->SetMaterial()");
         return false;
      }

      if (g_bWriteLogFileInLoop) 
      {
         WriteLogFile("printing TargetInfo\n");
      }

      if (g_Player.getGS()->displayTargetInfo() == false) return false;

      if (g_bWriteLogFileInLoop) 
      {
         WriteLogFile("printing textpage");
      }
   
      // -- Mause, Menu, Sternenkarte darstellen
      //
      if(l_renderpassCurr == RENDERPASS_FULLSCREEN)
      {
         if(g_Player.getGS()->displayRearView(g_RearViewBillboard) == false) return false;

	 {
            float x = -0.99f,y = 0.499f;
            float width  = 0.5f;
            float height = 0.5f;
  
            g_RearViewBillboard[0].x = x;
            g_RearViewBillboard[0].y = y;
       
            g_RearViewBillboard[1].x = x + width;
            g_RearViewBillboard[1].y = y;
       
            g_RearViewBillboard[2].x = x;
            g_RearViewBillboard[2].y = y + height;
       
            g_RearViewBillboard[3].x = x + width;
            g_RearViewBillboard[3].y = y + height;
       
            //////////////////////////////////////////////////////////////////////////
            for(int i = 0; i < 4 ; ++i)
               g_RearViewBillboard[i].nx = g_RearViewBillboard[i].ny = g_RearViewBillboard[2].nz;
            //////////////////////////////////////////////////////////////////////////
   
            //texture coordinate
            g_RearViewBillboard[2].tu = 0.0f;
            g_RearViewBillboard[2].tv = 0.0f;
       
            g_RearViewBillboard[3].tu = 1.0f;
            g_RearViewBillboard[3].tv = 0.0f;
       
            g_RearViewBillboard[0].tu = 0.0f;
            g_RearViewBillboard[0].tv = 1.0f;
       
            g_RearViewBillboard[1].tu = 1.0f;
            g_RearViewBillboard[1].tv = 1.0f;
       
            hr = g_pDev->SetTexture(0,g_d3dtextureFxBloomResult);
            if (hr != 0)
            { 
               hr2message(__LINE__,hr, "g_pDev->SetTexture(BACKWARD)");
               return false;
            }

   
            DWORD alpha;
            hr = g_pDev->GetRenderState(D3DRS_ALPHABLENDENABLE,&alpha);
            if (hr != 0)
            { 
               hr2message(__LINE__,hr, "_pDev->GetRenderState(D3DRS_ALPHABLENDENABLE,false)");
               return false;
            }
   
            hr = g_pDev->SetRenderState(D3DRS_ALPHABLENDENABLE,false);
            if (hr != 0)
            { 
               hr2message(__LINE__,hr, "_pDev->SetRenderState(D3DRS_ALPHABLENDENABLE,false)");
               return false;
            }
   
            hr = g_pDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,g_RearViewBillboard,sizeof(PSXVERTEX));
            if (hr != 0) { 
               hr2message(__LINE__,hr,"g_pDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP)");
               return false;
            }
   
            //set alpha state back
            hr = g_pDev->SetRenderState(D3DRS_ALPHABLENDENABLE,alpha);
            if (hr != 0)
            { 
               hr2message(__LINE__,hr, "_pDev->SetRenderState(D3DRS_ALPHABLENDENABLE,false)");
               return false;
            }
         }
      }
      if(g_Player.getGS()->displayMenuItem() == false) return false;

      if (g_Player.getGS()->activateFont() == false) return false;
      if (g_Player.getGS()->isHavingTextOverlay())
      {
         if (g_Player.getGS()->displayTextPage() == false) return false;
      }

      // -- Symbolausgabe vorbereiten
      //
      if (g_bEnableTextures)
      {
         if (g_txtArray[TID_SYMBOL].activate(0) == false) return false;
      }
   
      // --- alles wieder weiss
      //
      memset(&g_D3DMaterial, 0, sizeof(g_D3DMaterial));
      g_D3DMaterial.Diffuse  = g_d3dcolWhite;
      g_D3DMaterial.Ambient  = g_d3dcolWhite; 
      g_D3DMaterial.Specular = g_d3dcolWhite;
      g_D3DMaterial.Emissive = g_d3dcolWhite;
      g_D3DMaterial.Power    = 10.0f;
   
      hr = g_pDev->SetMaterial(&g_D3DMaterial);
      if (hr != 0) { 
         hr2message(__LINE__,hr,"g_pDev->"
                                  "SetMaterial(sun)");
         return false;
      }

      // -- Symbolausgabe vorbereiten
      //
      if (g_bEnableTextures)
      {
         if (g_txtArray[TID_SYMBOL].activate(0) == false) return false;
      }
   
      if (g_bWriteLogFileInLoop) 
      {
         WriteLogFile("printing cockpit\n");
      }
      if (g_Player.getGS()->displayCockpitHUDGfx() == false) return false;

      if (g_bWriteLogFileInLoop) 
      {
         WriteLogFile("printing Mouse\n");
      }
      if (g_Player.getGS()->displayMousePointer() == false) return false;



   } // end-if RENDERPASS_FULLSCREEN
    
   // -- etwaigen eigenen VertexShader deaktivieren
   //
   hr = g_pDev->SetVertexShader( NULL);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetVertexShader(PSXVERTEX)");
      return false;
   }
   hr = g_pDev->SetVertexDeclaration(g_pvdeclPSXVertex);
   if (hr != 0) { 
	   hr2message(__LINE__,hr, "g_pDev->SetVertexDeclaration(PSXVERTEX)");
	   return false;
   }

   hr = g_pDev->SetPixelShader(NULL);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetPixelShader(NULL)");
      return(false);
   }

   if (g_bWriteLogFileInLoop) 
   {
      if (g_pDev)
      {
         WriteLogFile("Device validated\n");
      }else{
         hr2message(__LINE__,hr,"Device invalid!!!!\n");
         return false;
      }
   }
   if (g_bWriteLogFileInLoop) 
   {
      WriteLogFile("unset textures\n");
   }

   hr = g_pDev->SetTexture(0, NULL);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetTexture(NULL)");
      return false;
   }

   if (g_bWriteLogFileInLoop) 
   {
      WriteLogFile("set Highlight-Pane-Color\n");
   }

   // --- Unterstreich-rechtecke zeichnen
   //
   {
      D3DCOLORVALUE l_d3dcolUse = { 0.0f, 0.2f, 0.1f, 0.0f };

      memset(&g_D3DMaterial, 0, sizeof(g_D3DMaterial));
      g_D3DMaterial.Diffuse  = l_d3dcolUse;
      g_D3DMaterial.Ambient  = l_d3dcolUse; 
      g_D3DMaterial.Specular = l_d3dcolUse;
      g_D3DMaterial.Emissive = l_d3dcolUse;
      g_D3DMaterial.Power    = 1.0f;
   }

   hr = g_pDev->SetMaterial(&g_D3DMaterial);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetMaterial()");
      return false;
   }

   // --- Transparenz beenden
   //
   if (g_bWriteLogFileInLoop) 
   {
      WriteLogFile("unset transparency\n");
   }
  
   hr = g_pDev->SetRenderState( D3DRS_ALPHABLENDENABLE,FALSE);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->"
                               "SetRenderState(ALPHAEBLEND)");
      return false;
   }
   

*/
   return true;
}

// -- Draw the stuff which needs identity as projection matrix
//
bool PSXDoRenderStuffBloom2D(RENDERPASS l_renderpassCurr)
{
/*
   //
   // -- activating 2D mode
   // 
   if (g_bWriteLogFileInLoop) 
   {
      WriteLogFile("2d mode for bloom (all matrices to identity)\n");
   }
   hr = g_pDev->SetTransform(D3DTS_WORLD, &g_matIdentity);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetTransform(WORLD, Identity)");
      return false;
   }
   hr = g_pDev->SetTransform(D3DTS_VIEW, &g_matIdentity);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetTransform(VIEW, Identity)");
      return false;
   }
   hr = g_pDev->SetRenderState( D3DRS_CULLMODE,D3DCULL_NONE); 
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->"
                               "SetRenderState(CULLMODE)");
      return(FALSE);
   } 

   hr = g_pDev->SetTransform(D3DTS_PROJECTION, &g_matIdentity);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetTransform(PROJ, Identity)");
      return false;
   }

   hr = g_pDev->SetTexture(0, NULL);
   if (hr != 0) { 
       hr2message(__LINE__,hr,"g_pDev->SetTexture(Bloom)");
       return false;
   }

   hr = g_pDev->SetVertexShader(NULL);
   if (hr != 0) { 
      hr2message(__LINE__,hr,"g_pDev->SetVertexShader(VECTOR)");
      return(FALSE);
   }
   hr = g_pDev->SetVertexDeclaration(g_pvdeclPSXVector);
   if (hr != 0) { 
	   hr2message(__LINE__,hr, "g_pDev->SetVertexDeclaration(PSXVECTOR)");
	   return false;
   }

   if (g_p3dxeffectBloom)
   {

      float x = -1.0f,y = -1.0f;
      float width  = 2.0f;
      float height = 2.0f;

      g_RearViewBillboard[0].x = x;
      g_RearViewBillboard[0].y = y;
	  g_RearViewBillboard[0].z = 0;
    
      g_RearViewBillboard[1].x = x + width;
      g_RearViewBillboard[1].y = y;
	  g_RearViewBillboard[1].z = 0;
 
      g_RearViewBillboard[2].x = x;
      g_RearViewBillboard[2].y = y + height;
	  g_RearViewBillboard[2].z = 0;
 
      g_RearViewBillboard[3].x = x + width;
      g_RearViewBillboard[3].y = y + height;
	  g_RearViewBillboard[3].z = 0;
    
      //////////////////////////////////////////////////////////////////////////
      for(int i = 0; i < 4 ; ++i)
         g_RearViewBillboard[i].nx = g_RearViewBillboard[i].ny = g_RearViewBillboard[i].nz = 0.0f;
      //////////////////////////////////////////////////////////////////////////
 
      //texture coordinate
	  float l_maxTextureWidth = 1.0f;
      g_RearViewBillboard[0].tu = 0.0f;
      g_RearViewBillboard[0].tv = 0.0f;
 
      g_RearViewBillboard[1].tu = l_maxTextureWidth;
      g_RearViewBillboard[1].tv = 0.0f;
 
      g_RearViewBillboard[2].tu = 0.0f;
	  g_RearViewBillboard[2].tv = l_maxTextureWidth;
    
	  g_RearViewBillboard[3].tu = l_maxTextureWidth;
	  g_RearViewBillboard[3].tv = l_maxTextureWidth;
 
     
      memset(&g_D3DMaterial, 0, sizeof(g_D3DMaterial));
      g_D3DMaterial.Diffuse  = g_d3dcolWhite;
      g_D3DMaterial.Ambient  = g_d3dcolWhite; 
      g_D3DMaterial.Specular = g_d3dcolWhite;
      g_D3DMaterial.Emissive = g_d3dcolWhite;
      g_D3DMaterial.Power    = 1.0f;
   
      hr = g_pDev->SetMaterial(&g_D3DMaterial);
      if (hr != 0) { 
         hr2message(__LINE__,hr,"g_pDev->SetMaterial()");
         return false;
      }

      DWORD alpha;
      hr = g_pDev->GetRenderState(D3DRS_ALPHABLENDENABLE,&alpha);
      if (hr != 0)
      { 
         hr2message(__LINE__,hr, "_pDev->GetRenderState(D3DRS_ALPHABLENDENABLE,false)");
         return false;
      }

      hr = g_pDev->SetRenderState(D3DRS_ALPHABLENDENABLE,false);
      if (hr != 0)
      { 
         hr2message(__LINE__,hr, "_pDev->SetRenderState(D3DRS_ALPHABLENDENABLE,false)");
         return false;
      }

	  if (g_bEnableTextures)
      {
		  if (g_txtArray[TID_NEBULA2].activate(0) == false) return false;
      }
   

         hr = g_pDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,g_RearViewBillboard,sizeof(PSXVERTEX));
         if (hr != 0) { 
            hr2message(__LINE__,hr,"g_pDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP)");
            return false;
         }

      hr = g_pDev->SetTexture(0, NULL);
      if (hr != 0) { 
          hr2message(__LINE__,hr,"g_pDev->SetTexture(Bloom)");
          return false;
      }

      //set alpha state back
      hr = g_pDev->SetRenderState(D3DRS_ALPHABLENDENABLE,alpha);
      if (hr != 0)
      { 
         hr2message(__LINE__,hr, "_pDev->SetRenderState(D3DRS_ALPHABLENDENABLE,false)");
         return false;
      }
   }
*/
   return true;
}

