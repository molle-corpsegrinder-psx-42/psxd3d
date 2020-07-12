/*
 *------------------------------------------------------------------------
 *      Object: enemy.cpp 
 *   $Revision: 1.159 $
 *        Date: 2004                            
 *       Notes: ENEMY Methoden
 *------------------------------------------------------------------------
 * 
 * Copyright (c) 1997-2006 Michael Molkenthin. All Rights Reserved.
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

ENEMY::ENEMY(LIFECYCLE l_lifecycle) : MAPOBJECT(l_lifecycle)
{
   init();
}
/*
ENEMY::ENEMY(LIFECYCLE l_lifecycle, CHARACTER* l_pcharSource) : MAPOBJECT(l_lifecycle)
{
   init();

   if (l_pcharSource)
   {
      m_pCharacter   = l_pcharSource;
      m_idxCharacter = l_pcharSource->getCharacterIndex();
   }
}
*/

// -- default werte setzen + 
//    die Verkettung zwischen den ENEMY objekten innerhalb des grossen Pools
//
bool ENEMY::init()
{
   // -- MapObj verkettung pflegen
   //
   m_iSizeOf = sizeof(ENEMY); 

   if (m_penemyFirst == NULL) 
   {
      m_penemyFirst = this;
   }

   if (m_penemyLast)
   {
      m_penemyPrevious = m_penemyLast;
      m_penemyPrevious->m_penemyNext = this;
   }else{
      m_penemyPrevious = NULL;
   }
 
   m_penemyNext = NULL;
   m_penemyLast = this;

   setEnemyIndex(m_idxHighest);

   m_dGeomID = NULL;
   m_dBodyID = NULL;

   // -- weitere Flags
   //
   m_iFlags                  = 0;
   m_cntGeneric              = 0;
   m_fViewRotation1          = 0.0f;
   m_fViewRotation2          = 0.0f;
   m_cntShieldFlashTimer     = 0;
   m_fExplScale              = 1.0f;
   setVisible(true);
   /*
   */
   m_Mat                     = XMMatrixIdentity();

   
   m_fRad                    = 100.0f;
   m_fShootHitRadPow2        = 100.0f;
   /*
   m_fCollisionRadPow2       = 100.0f;
   m_fRad2D                  = 100.0f;
*/
   m_ResType                 = (RESTYPE)0;
   m_vPos = XMVectorZero();
/*   
   if (setStep(0.0f, 0.0f, 0.0f)==false) return false;
   */
   m_Action                  = ENACT_DONOTHING;
   m_iActionTimer            = 0;
   m_pLeader                 = NULL;
   //m_idxLeadhist             = -1;
   /*
   m_cntRanking              = 0;
   setTriggered(false);
   filler                    = 0;
   m_fMinRad2DForVisibility  = 0.01f;
   */
   m_fSpeed                  = 0.0f;
   /*
   setDestFrame(false);
   m_cDestFrameInfo          = g_cLang[txxNull];
   m_fShieldAmount           = 1.0f;
   m_fScale                  = 1.0f;
   */
   m_pBigResItem             = NULL;
   /*
   m_idxCharacter            = -1;
   m_pCharacter              = NULL;
*/
   return true;
}


ENEMY* ENEMY::getNextEnemy()
{
   return m_penemyNext;
}

ENEMY* ENEMY::getPreviousEnemy()
{
   return m_penemyPrevious;
}
/*
unsigned int ENEMY::getTypeID()
{
   return mType;
}
*/
bool ENEMY::setEnemyIndex(int l_idxNew)
{
   m_idxEnemy = l_idxNew;

   if (m_idxEnemy >= m_idxHighest ) 
   {
      m_idxHighest = m_idxEnemy + 1;
   }

   return true;
}

int ENEMY::getEnemyIndex()
{
   return m_idxEnemy;
}

//   Update():
//   Einen Denk und Handlungsschritt fuer einen Gegner herbeifuehren
//

bool ENEMY::Update()
{
	/*
   PSXVECTOR l_vPlayer;
   */
   if (g_bWriteLogFileInLoop) 
   {
      WriteLogFile(g_cEnemyTypeNameArray[mType]);
      WriteLogFile("::Update() method beginning\n");
   }

   // --- Ausnahme: explosionen sind reine animationen ohne weiteres
   //               handling
   //
   if (mType == ETYP_EXPLOSION) 
   {
      hr2message(__LINE__,-1, "ETYP_EXPLOSION not implemented in ENEMY::Update()");
      return false;
   }

   if (mType == ETYP_IMPACT) 
   {
      hr2message(__LINE__,-1, "ETYP_IMPACT not implemented in ENEMY::Update()");
      return false;
   }

   if (m_dGeomID)
   {
	    const dReal* l_pdPos;
	   // -- für KI die momentane Position und Orientierung holen
	   //
	   l_pdPos = dGeomGetPosition(m_dGeomID);	   
	   m_vPos = XMVectorSet(l_pdPos[0],l_pdPos[1],l_pdPos[2],0.0f);
   }else{
	   hr2message(__LINE__,-1, "m_dGeomID not set in ENEMY::Update()");
	   return false;
   }

   // -- ODE auslesen, denn Geschwindigkeit und Matrix sind 
   //    Entscheidungsgrundlage der KI
   //
   if (m_dGeomID && m_dBodyID)
   {
      const dReal* l_pdPos;
      const dReal* l_pdAngular;
      const dReal* l_pdMatrix3;
// Vorbild
      //const dReal* Pos = dGeomGetPosition(geom);
      //const dReal* Rot = dGeomGetRotation(geom);
      //const double Transform[16] = 
      //{
      //   Rot[0], Rot[4], Rot[8],  0,
      //   Rot[1], Rot[5], Rot[9],  0,
      //   Rot[2], Rot[6], Rot[10], 0,
      //   Pos[0], Pos[1], Pos[2],  1
      //};
	  XMFLOAT3 l_vEnemy, l_float3Step;
	  XMStoreFloat3(&l_vEnemy, m_vPos);	  

      // -- Retten der Werte für Kollisionsabfrage
      //
      if (dGeomGetClass(m_dGeomID) == dTriMeshClass)
      {
		  XMFLOAT4X4 l_matEnemy;
		  XMStoreFloat4x4(&l_matEnemy, m_Mat);

         double l_dLastTransformArray[16];
		 l_dLastTransformArray[0]  = l_matEnemy._11;			 
         l_dLastTransformArray[1]  = l_matEnemy._21;
         l_dLastTransformArray[2]  = l_matEnemy._31;
         l_dLastTransformArray[3]  = 0.0f;
         l_dLastTransformArray[4]  = l_matEnemy._12;
         l_dLastTransformArray[5]  = l_matEnemy._22;
         l_dLastTransformArray[6]  = l_matEnemy._32;
         l_dLastTransformArray[7]  = 0.0f;
         l_dLastTransformArray[8]  = l_matEnemy._13;
         l_dLastTransformArray[9]  = l_matEnemy._23;
         l_dLastTransformArray[10] = l_matEnemy._33;
         l_dLastTransformArray[11] = 0.0f;
         l_dLastTransformArray[12] = l_vEnemy.x;
         l_dLastTransformArray[13] = l_vEnemy.y;
         l_dLastTransformArray[14] = l_vEnemy.z;
         l_dLastTransformArray[15] = 1.0f;
         dGeomTriMeshSetLastTransform(reinterpret_cast<dTriMeshID>(m_dGeomID), l_dLastTransformArray);
      }
      // -- für KI die momentane Position und Orientierung holen
      //
      l_pdPos = dGeomGetPosition(m_dGeomID);
      l_vEnemy.x = l_pdPos[0];
      l_vEnemy.y = l_pdPos[1];
      l_vEnemy.z = l_pdPos[2];
      m_vPos = XMLoadFloat3(&l_vEnemy);

      l_pdPos = dBodyGetLinearVel(m_dBodyID);
      l_float3Step.x = l_pdPos[0];
      l_float3Step.y = l_pdPos[1];
      l_float3Step.z = l_pdPos[2];

      if (_isnan(l_float3Step.x) ||
          _isnan(l_float3Step.y) ||
          _isnan(l_float3Step.z) )
      {
         WriteLogFile("m_vStep is NAN\n");
         setStep(0.0f, 0.0f, 0.0f);
      }

      l_pdAngular = dBodyGetAngularVel(m_dBodyID);
      if(l_pdAngular)
      {
         if (_isnan(l_pdAngular[0]) ||
             _isnan(l_pdAngular[1]) ||
             _isnan(l_pdAngular[2]) )
         {
            WriteLogFile("Angular velocity is NAN\n");
            dBodySetAngularVel( m_dBodyID,0.0f,0.0f,0.0f);
         }
      }

      // -- Matrix abhängige Werte holen
      //
      {
		  XMFLOAT4X4 l_matEnemy;
         l_pdMatrix3 = dGeomGetRotation(m_dGeomID);
		 
   
         // -- Offset draufrechnen, sofern das Geom noch ein anderes mit einschliesst
         //
         l_matEnemy._11 = l_pdMatrix3[0];
         l_matEnemy._21 = l_pdMatrix3[1];
         l_matEnemy._31 = l_pdMatrix3[2];
         l_matEnemy._41 = l_pdMatrix3[3];
         l_matEnemy._12 = l_pdMatrix3[4];
         l_matEnemy._22 = l_pdMatrix3[5];
         l_matEnemy._32 = l_pdMatrix3[6];
         l_matEnemy._42 = l_pdMatrix3[7];
         l_matEnemy._13 = l_pdMatrix3[8];
         l_matEnemy._23 = l_pdMatrix3[9];
         l_matEnemy._33 = l_pdMatrix3[10];
         l_matEnemy._43 = l_pdMatrix3[11];
         l_matEnemy._14 = 0.0f;
         l_matEnemy._24 = 0.0f;
         l_matEnemy._34 = 0.0f;
         l_matEnemy._44 = 1.0f;
		 m_Mat = XMLoadFloat4x4(&l_matEnemy);

         // -- evtl. Middle wieder abziehen um drawrelevanten Pivot herzustellen
         //		
         if (dGeomGetClass(m_dGeomID) != dTriMeshClass)
         {
            dVector3 l_dTransformed, l_dOffset;
			XMVECTOR l_vTransformed;			

            l_dOffset[0] = g_meshresManager.getMiddleX(m_ResType);/* * m_fScale;*/
            l_dOffset[1] = g_meshresManager.getMiddleY(m_ResType);/* * m_fScale;*/
            l_dOffset[2] = g_meshresManager.getMiddleZ(m_ResType);/* * m_fScale;*/
   
            dMULTIPLY0_331 (l_dTransformed,l_pdMatrix3,l_dOffset);			
			l_vTransformed = XMVectorSet(l_dTransformed[0], l_dTransformed[1], l_dTransformed[2], 1.0f);
            m_vPos = m_vPos - l_vTransformed;
         }
	  
      }
   } 
   /*
   g_ehdlBuf.m_bSingleMovementInPipe = false;
   
   // --- Trigger naehe zwischen player und enemy feststellen
   //
   if (g_Player.m_pEnemy)
   {
      l_vPlayer.x = g_Player.m_pEnemy->getPosX() - m_vPos.x;
      l_vPlayer.y = g_Player.m_pEnemy->getPosY() - m_vPos.y;
      l_vPlayer.z = g_Player.m_pEnemy->getPosZ() - m_vPos.z;
                      
      m_fDestLength = pow2(l_vPlayer.x)+ pow2(l_vPlayer.y)+ pow2(l_vPlayer.z);
   }

   // -- Wenn enemy in gewisser reichweite,
   //    kann das radar ihn erfassen
   //
   if(m_fDestLength<=g_maxfRadarRange*pow2(g_fRadarZoomFactor))
   {
      if (isRadarable())
      {
         if (RADARLIST::Instance()->addEntry(this) == false) return false;
      }
   }

   // -- sofortausstieg - keine weitere KI notwendig
   //
   if (mType == ETYP_ASTEROID)
   {
      return true;
   }

   if (g_Player.m_bGameOver)
   {
      if (setStep(0.0f, 0.0f, 0.0f) == false) return false;
      return true;
   }

   // -- Welches Objekt bekommt Fadenkreuz , wenn es denn sichtbar ist
   //    zugehoeriger Text wird bestimmt
   //
   if(updateDestFrame() == false) return false;

   */

   if (g_penemyInput == this)
   {
      if (g_bWriteLogFileInLoop) 
      {
         WriteLogFile("controlledByUser()...\n");
      }
      if (controlledByUser() == false) return false;
   }else{
      if (g_bWriteLogFileInLoop) 
      {
         WriteLogFile("TreatAs");
      }
      if (updateAction() == false) return false;
   }
   // --- aktuellen Bewegungsschritt mit derzeitiger drehmatrix verrechnen
   //

   // -- shield-aufblitzen updaten
   //
   if (m_cntShieldFlashTimer > 0)
   {
      m_cntShieldFlashTimer++;
      if (m_cntShieldFlashTimer >= g_maxShieldFlashTimer)
      {
         m_cntShieldFlashTimer = 0;
      }
   }
   // -- Abfragen ob dieser Enemy mit dem Player kollidiert
   //
      // -- Abfragen ob dieser Enemy mit dem Raumstation kollidiert
      //
   if (g_bWriteLogFileInLoop) 
   {
      WriteLogFile("enemy-update done.\n");
   }

   return true;
}

// -- aktualisiert am Zielkreuz angedruckte Zusatz info
//
/*
bool ENEMY::updateDestFrame()
{
   float l_fShootRangePow2,
         l_fDestLength;

   PSXVECTOR l_vDest; 
   static float     s_fTargetLength;
   
   if (g_Player.m_pEnemy == NULL) return true;
   l_fDestLength = m_fDestLength;

   // -- Dest ist hierbei der Enemy (für Mine und Missile)
   //
   l_vDest.x = m_vPos.x - g_penemyCamera->getPosX();
   l_vDest.y = m_vPos.y - g_penemyCamera->getPosY();
   l_vDest.z = m_vPos.z - g_penemyCamera->getPosZ();

   // -- Next Triggered/Untriggered Enemy werte pflegen == nächster Feind/Freund
   //
   if (isTriggered())
   {
      if (isTargetable() && ((g_penemyNextTriggered && l_fDestLength < g_fNextTriggeredPow2) || g_penemyNextTriggered == NULL))
      {
         g_penemyNextTriggered = this;
         g_fNextTriggeredPow2 = l_fDestLength;
      }

      if (g_penemyNextUntriggered == this)
      {
         g_penemyNextUntriggered = NULL;
      }
   }else{
      if (g_penemyNextTriggered == this)
      {
         g_penemyNextTriggered = NULL;
      }

      if (isTargetable() && ((g_penemyNextUntriggered && l_fDestLength < g_fNextUntriggeredPow2) || g_penemyNextUntriggered == NULL))
      {
         g_penemyNextUntriggered = this;
         g_fNextUntriggeredPow2 = l_fDestLength;
      }
   }
 
   // -- Vorschlag des allernächsten Enemies in der Mitte des Fadenkreuzes
   //
   if (isTargetable() &&
       m_vPos2D.x < m_fRad2D && m_vPos2D.y < m_fRad2D &&
       m_vPos2D.x > -m_fRad2D && m_vPos2D.y > -m_fRad2D &&
       m_vPos2D.z > 0.0f)
   {
      // -- entweder ist EnemyCenter leer oder genügt jeglichen Kriterien
      //
      if ( g_bEnemyCenterFrameValid == false ||
          (g_bEnemyCenterFrameValid && 
           g_penemyCenterFrame &&
           l_fDestLength<g_fEnemyCenterFramePow2) )
      {
         g_fEnemyCenterFramePow2 = l_fDestLength;
         g_penemyCenterFrame = this;
         g_bEnemyCenterFrameValid = true;
      }

   }else{
      // -- ungültig gewordenen Vorschlag löschen
      //
      if (g_bEnemyCenterFrameValid && 
          g_penemyCenterFrame &&
          g_penemyCenterFrame == this) 
      {
         {
            g_penemyCenterFrame = NULL;
            g_bEnemyCenterFrameValid = false;
         }
      }
   }

   // -- reguläre Next/Previous durchsteppbare Auswahl pflegen
   //
   if (m_vPos2D.z > 0.0f && isVisible() && isTargetable())// && isPlayer()==false)
   {
      if (l_fDestLength < pow2(2000.0f))
      {
         if (g_cntTarget < g_maxTarget)
         {
            g_penemyTargetArray[g_cntTarget] = this;
            g_cntTarget++;
         }
      }

      l_fShootRangePow2 = g_pequipmentSelectWeapon->getShootRangePow2();
      if (l_fDestLength < l_fShootRangePow2)
      {
         // -- Vorauswahl des Target Enemies nur treffen, wenn die Auswahl nicht
         //    in der Schwebe und wenn das Target nicht gelockt wurde
         //
         if (g_bTargetLocked == false && g_bTargetInfo == false)
         {
            //m_dsDestFrame = DISPLAYSTYLE_FLASHING;
            if ((IsAnyTarget() && l_fDestLength < s_fTargetLength) || (IsAnyTarget() == false))
            {
               if (setEnemyTarget(this) == false) return false;
               s_fTargetLength = l_fDestLength;

               setDestFrame(true);
            }
         }
      }
   }

   // -- Richtungs Pfeil für das gelockte Target
   //
   if (g_bTargetLocked && getEnemyTarget() == this)
   {
      D3DMATRIX l_matCompass;

      l_fDestLength = sqrt(l_fDestLength);

      l_vDest.x = l_vDest.x / l_fDestLength;
      l_vDest.y = l_vDest.y / l_fDestLength;
      l_vDest.z = l_vDest.z / l_fDestLength;

      if (PSXMatrixFromVector( &l_matCompass, l_vDest) == false)
      {
         return false;
      }

      memcpy (&g_matCompassYZ,&g_matIdentity, sizeof(g_matIdentity));
      g_matCompassYZ._11 = l_matCompass._11;
      g_matCompassYZ._12 = l_matCompass._12;
      g_matCompassYZ._13 = l_matCompass._13;

      g_matCompassYZ._21 = l_vDest.x;
      g_matCompassYZ._22 = l_vDest.y;
      g_matCompassYZ._23 = l_vDest.z;

      g_matCompassYZ._31 = l_matCompass._21;
      g_matCompassYZ._32 = l_matCompass._22;
      g_matCompassYZ._33 = l_matCompass._23;

      // -- Player Rakete auf diesen Enemy zusteuern
      //
      if (g_equipmentArray[EQUIPMENT_MISSILE].m_bSingleton && isPlayer() == false)
      {
         float l_fLength, l_fSpeed;
         int l_idxShoot;
         PSXVECTOR l_vDelta, l_vStep;

         l_idxShoot = g_equipmentArray[EQUIPMENT_MISSILE].m_idxShootSingleton;

         // -- Richtungsvektor von Missile zu diesem Enemy beeinflusst momentanen Missilestep
         //
         l_vDelta.x = (m_vPos.x - shoot_array[l_idxShoot].m_vPos.x) / 2.0f;
         l_vDelta.y = (m_vPos.y - shoot_array[l_idxShoot].m_vPos.y) / 2.0f;
         l_vDelta.z = (m_vPos.z - shoot_array[l_idxShoot].m_vPos.z) / 2.0f;

         l_vStep.x = shoot_array[l_idxShoot].m_vStep.x + l_vDelta.x;
         l_vStep.y = shoot_array[l_idxShoot].m_vStep.y + l_vDelta.y;
         l_vStep.z = shoot_array[l_idxShoot].m_vStep.z + l_vDelta.z;

         l_fLength = sqrt(pow2(l_vStep.x)+ pow2(l_vStep.y)+ pow2(l_vStep.z));

         l_vStep.x = l_vStep.x / l_fLength;
         l_vStep.y = l_vStep.y / l_fLength;
         l_vStep.z = l_vStep.z / l_fLength;

         if (PSXMatrixFromVector( &g_equipmentArray[EQUIPMENT_MISSILE].m_matSingleton, 
                                  l_vStep) == false)
         {
            return false;
         }

         l_fSpeed = g_equipmentArray[EQUIPMENT_MISSILE].m_fSpeed + m_fSpeed;

         shoot_array[l_idxShoot].m_vStep.x = l_vStep.x * l_fSpeed;
         shoot_array[l_idxShoot].m_vStep.y = l_vStep.y * l_fSpeed;
         shoot_array[l_idxShoot].m_vStep.z = l_vStep.z * l_fSpeed;
  
      }
   }

   // -- Mine auf diesen Enemy zusteuern
   //
   if (g_equipmentArray[EQUIPMENT_MINE].m_bSingleton && isPlayer()==false)
   {
      float l_fLength;
      int l_idxShoot;
      PSXVECTOR l_vDelta;
      l_idxShoot = g_equipmentArray[EQUIPMENT_MINE].m_idxShootSingleton;

      // -- Richtungsvektor von Mine zu diesem Enemy beeinflusst momentanen Minenstep
      //
      l_vDelta.x = (m_vPos.x - shoot_array[l_idxShoot].m_vPos.x);
      l_vDelta.y = (m_vPos.y - shoot_array[l_idxShoot].m_vPos.y);
      l_vDelta.z = (m_vPos.z - shoot_array[l_idxShoot].m_vPos.z);
      l_fLength = pow2(l_vDelta.x)+ pow2(l_vDelta.y)+ pow2(l_vDelta.z);

      if (l_fLength < pow2(200.0f))
      {
         //--- Minenexplosion
         //
         ENEMY* l_pEnemy;
         l_pEnemy = new (LIFECYCLE_LEVEL) EXPLOSION(LIFECYCLE_LEVEL);
         if (l_pEnemy != NULL)
         { 
            //l_pNewEnemy->Init();
               
            l_pEnemy->SetType(ETYP_EXPLOSION);
            l_pEnemy->m_vPos = shoot_array[l_idxShoot].m_vPos;
            if (l_pEnemy->setStep(0.0f,0.0f, 0.0f)==false) return false;
            l_pEnemy->setVisible(true);
            l_pEnemy->setTriggered(false);
            l_pEnemy->m_fExplScale = 0.1f;
         }

         shoot_array[l_idxShoot].Die();
         
         // -- andere Enemies auch informieren
         //
         l_pEnemy = ENEMY::getFirstEnemy();
         while(l_pEnemy)
         {

            if ((l_pEnemy->mType != ETYP_EMPTY) &&
                (l_pEnemy->mType != ETYP_EXPLOSION) &&
                (l_pEnemy->mType != ETYP_IMPACT))
            {
               // -- Richtungsvektor von Mine zu diesem Enemy beeinflusst momentanen Minenstep
               //
               l_vDelta.x = (l_pEnemy->m_vPos.x - shoot_array[l_idxShoot].m_vPos.x);
               l_vDelta.y = (l_pEnemy->m_vPos.y - shoot_array[l_idxShoot].m_vPos.y);
               l_vDelta.z = (l_pEnemy->m_vPos.z - shoot_array[l_idxShoot].m_vPos.z);
               l_fLength = pow2(l_vDelta.x)+ pow2(l_vDelta.y)+ pow2(l_vDelta.z);

               if (l_fLength < pow2(200.0f))
               {
                  shoot_array[l_idxShoot].m_vPos = l_pEnemy->m_vPos;
                  shoot_array[l_idxShoot].m_cntTimeToLive = 100;
                  shoot_array[l_idxShoot].menacetype = MENACE_ENEMY;

                  l_pEnemy->HitByShoot(l_idxShoot, &l_vDelta);

                  //l_pEnemy->m_Action = ENACT_BANKROTATION;
                  //l_pEnemy->m_iActionTimer = 20000.0f;
               }
            }
            l_pEnemy = l_pEnemy->getNextEnemy();
         }
      }
   }

   return true;
}
*/
// -- ob dieser Enemy den Player repräsentiert oder zu ihm gehört
//

bool ENEMY::isPlayer()
{
   bool l_bReturn;
   if (g_Player.m_pEnemy == this)
   {
      l_bReturn = true;
   }else{
      if (m_pLeader) 
      {
         if (g_Player.m_pEnemy == m_pLeader)
         {
            l_bReturn = true;
         }else{
            l_bReturn = false;
         }
      }else{
         l_bReturn = false;
      }
   }
   return l_bReturn;
}

//   SetType():
//   Setzt je nach Gegnertyp weitere Werte
//

bool ENEMY::detonate()
{
   m_Action = ENACT_DONOTHING;

   //--- Minenexplosion
   //
   ENEMY* l_pNewEnemy;
   l_pNewEnemy = new (LIFECYCLE_LEVEL) EXPLOSION(LIFECYCLE_LEVEL);

   if (l_pNewEnemy != NULL)
   { 
      l_pNewEnemy->SetType(ETYP_EXPLOSION);
      if (l_pNewEnemy->setPos(m_vPos)==false) return false;
      if (l_pNewEnemy->setStep(m_vStep)==false) return false;
      l_pNewEnemy->m_fExplScale               = m_fExplScale;

      l_pNewEnemy->setVisible(true);
      l_pNewEnemy->setTriggered(false);
   }

   // -- falls explosionen veranlasst wurden, auch den sound dazu abspielen
   // 
   
   /*
   if (g_options.IsEnableSound())
   {
      if (g_options.IsWriteLogFileInLoop())
      {
            WriteLogFile("play Sound\n");
      }
      if (g_sound_array[SNDRES_EXPLOSION1].play(l_pNewEnemy) == false) return false;
   }
   */
   
   m_cntGeneric = 0;
   if (Destroy(LIFECYCLE_LEVEL) == false) return false;
   mType = ETYP_EMPTY;

   return true;
}

bool ENEMY::SetType(ETYPE NeuerType)
{
   mType = NeuerType;

   WriteLogFile("ENEMY::SetType(");
   WriteLogFile(g_cEnemyTypeNameArray[mType]);
   WriteLogFile(")\n");

   // -- ausnahmen zur erstinitialisierung zuweisen
   //
   if (mType > ETYP_EMPTY)
   {
	   
      if (initTypeDependend() == false) return false;
	  
      // -- zwar nicht wichtig fuer eigentliche Zeichnen, aber sehr wohl relevant
      //    fuer die Visibility pruefung, die den Radius und damit das 
      //    repraesentative Resource Objekt wissen muss
      //    und nun auch für die ODE Engine, welche die Masse kennen muss
      //
      switch(mType)
      {
         case ETYP_FLYER:            setRes(RESTYPE_FLYER);   break;
         case ETYP_TUNNELFLYER:      setRes(RESTYPE_FLYER);   break;
         case ETYP_LEADER:           setRes(RESTYPE_LEADER);  break;
         case ETYP_ASTEROID:         setRes(RESTYPE_ASTEROID);break;
      
         case ETYP_HANGAR:           setRes( RESTYPE_HANGAR); break;
		 case ETYP_GUN:              setRes( RESTYPE_GUN);    break;
         case ETYP_REACTORGUN:       setRes( RESTYPE_GUN);    break;
         case ETYP_MINE:             setRes( RESTYPE_MINE);   break;
         case ETYP_DOOR:             setRes( RESTYPE_DOOR01A);break;
         case ETYP_FELDGENERATOR:    setRes( RESTYPE_FELDGENERATOR);break;
         case ETYP_TMPSPEEDUP:       setRes( RESTYPE_TMPSPEEDUP);break;
         case ETYP_CHAR:             setRes( RESTYPE_GUN);    break;
         case ETYP_GHOST:            setRes( RESTYPE_FLYER);break;
         case ETYP_HUNTER:           setRes( RESTYPE_HUNTER_X);break;
         case ETYP_PIRATE:           setRes( RESTYPE_CARGO01);break;
//         case ETYP_PIRATE:           setRes( RESTYPE_BOA01);break;
         case ETYP_P_FLYER:          setRes( RESTYPE_HUNTER_X);break;
         case ETYP_TRADER:           setRes( RESTYPE_CARGO05);break;
         case ETYP_EQUIPMENT:        setRes( RESTYPE_REACTOR);break;
         //case ETYP_LASERHALTER:      setRes( RESTYPE_LASERHALTER);break;
         case ETYP_LASERHALTER:      setRes( RESTYPE_LASERHALTER);break;
         case ETYP_LASERGUN:         setRes( RESTYPE_LASERGUN);break;
         case ETYP_STARBASE:         setRes( RESTYPE_CORIOLIS_EXTERIOR);break;
         case ETYP_KINGSPLANE:       setRes( RESTYPE_CORIOLISKINGSPLANE);break;
         case ETYP_STARBASEHANGAR:   setRes( RESTYPE_STARBASEHANGAR); break;

         case ETYP_THARGOID:         setRes( RESTYPE_THARGOID);break;
         case ETYP_THARGOIDDRONE:    setRes( RESTYPE_THARGOIDDRONE);break;
         case ETYP_PLAYER:           setRes( RESTYPE_COBRA_PART1);break;
		 case ETYP_DAZ3D_CUBE:       setRes(RESTYPE_DAZ3D_CUBE); break;
         //default:                    setRes( RESTYPE_LEADER); break;
      }
   }

   return true;
}

// -- diese wird zurückgerufen mit kollisionsresultaten
//
/*
void PSXTriArrayCallback(dGeomID TriMesh, dGeomID RefObject, const int* TriIndices, int TriCount)
{
}

int PSXTriCallback (dGeomID TriMesh, dGeomID RefObject, int TriangleIndex)
{
   return 1;
}
*/

bool ENEMY::setRes(RESTYPE l_resNew)
{
	/*
   WriteLogFile(g_cEnemyTypeNameArray[mType]);
   WriteLogFile("::setRes()\n");
   */
   m_ResType = l_resNew;
   /*
   CalculateRad();
   */
   // -- entweder Body-ID des Leaders übernehmen oder aber die Inertiamatrix des eigenen Bodies setzen
   //
   if (isPartOfLeader())
   {
      if (m_pLeader && m_pLeader->m_dBodyID)
      {
         m_dBodyID = m_pLeader->m_dBodyID;
      }
   }

   if (m_dBodyID)
   {
      if (createCollisionGeoms(l_resNew) == false) return false;

      // -- nur setzen, wenn dieser enemy eigenen Body hat
      //
      if (isPartOfLeader() == false)
      {
         dBodySetMass(m_dBodyID, &m_dMass);
      }
   }else{
	   hr2message(__LINE__, -1, "m_dBodyID not set in ENEMY::setRes()");
	   return false;
   }

   
   return true;
}

// -- eigenen rigid body für den enemy anlegen und neutral vorinitialisieren
//

bool ENEMY::createBody()
{
   dMatrix3 l_matODEIdentity;

   WriteLogFile(g_cEnemyTypeNameArray[mType]);
   WriteLogFile("::createBody()\n");

   m_dBodyID = dBodyCreate(LEVEL::m_iWorldID);
   dBodySetData(m_dBodyID, this);
   dRSetIdentity(l_matODEIdentity);
   dBodySetRotation(m_dBodyID, l_matODEIdentity);
   dBodySetLinearVel( m_dBodyID,0.0f,0.0f,0.0f);
   dBodySetAngularVel( m_dBodyID,0.0f,0.0f,0.0f);

   return true;
}

// -- freigabe der eigenen BodyID
//
bool ENEMY::releaseBody()
{
   WriteLogFile(g_cEnemyTypeNameArray[mType]);
   WriteLogFile("::releaseBody()\n");

   if (m_dBodyID) 
   { 
      int l_idxJoint, l_cntJoint;
   
      // -- alle verbundenen Joints freigeben
      //
      l_cntJoint = dBodyGetNumJoints(m_dBodyID);
      for (l_idxJoint = 0; l_idxJoint < l_cntJoint; l_idxJoint++)
      {
          dJointID l_dCurrJointID;
          l_dCurrJointID = dBodyGetJoint(m_dBodyID, l_cntJoint - l_idxJoint - 1);
          dJointDestroy(l_dCurrJointID);
      }
   
      dBodyDestroy(m_dBodyID);
      m_dBodyID = NULL;
        
      return true;
   }else{
      hr2message(__LINE__,-1, "ENEMY::releaseBody() m_dBody is null");
      return false;
   }
}

// -- dem Body Kollisions-Geoms zuordnen
//

bool ENEMY::createCollisionGeoms(RESTYPE l_resNew)
{
   bool l_bSpecialGeom;
   int l_idxManualGeom;
   struct _tempMassManualGeom {
      RESTYPE   res;
      int       l_iPrimitive;
   } l_tempMassManualGeom[] = {{RESTYPE_LEADER, -1},
                               {RESTYPE_FLYER,  -1},
							   { RESTYPE_HUNTER_X,  -1 },
							   { RESTYPE_DAZ3D_CUBE,1},
							   
							   { RESTYPE_COBRA_PART1, -1},

                               {RESTYPE_WINGMAN,  -1},
                               {RESTYPE_LASERGUN,    0},
                               {RESTYPE_LASERHALTER, 1},
                              };
   const int l_cntManualGeom = sizeof(l_tempMassManualGeom) / sizeof(l_tempMassManualGeom[0]);

   WriteLogFile(g_cEnemyTypeNameArray[mType]);
   WriteLogFile("::createCollisionGeoms()\n");

   // -- falls ein anderer GeomID schon gesetzt war, diesen droppen
   //
   if (m_dGeomID)
   {
      //dGeomSetBody(m_dGeomID, NULL);
      dGeomDestroy(m_dGeomID);
      m_dGeomID = NULL;
   }

   // -- prüfen, ob nicht manuell kollisions-geoms angelegt werden sollen
   //
   l_bSpecialGeom = false;
   for (l_idxManualGeom = 0; l_idxManualGeom < l_cntManualGeom; l_idxManualGeom++)
   {
      if (l_tempMassManualGeom[l_idxManualGeom].res == l_resNew)
      {
         l_bSpecialGeom = true;
         switch (l_tempMassManualGeom[l_idxManualGeom].l_iPrimitive)
         {
            case 0:
               m_dGeomID = dCreateCapsule(LEVEL::m_dSpaceID,
                                            g_meshresManager.getSideLengthX(l_resNew)/2.0f, //radius des cylinders
                                            g_meshresManager.getSideLengthZ(l_resNew));
			   dGeomSetData(m_dGeomID, (void*) this);
               dMassSetCapsule(&m_dMass, 1.0f, 3, // Direction 3 ist z
                           g_meshresManager.getSideLengthX(l_resNew) / 2.0f,//radius des cylinders
                           g_meshresManager.getSideLengthZ(l_resNew) );
               break;
            case 1:
               m_dGeomID = dCreateBox(LEVEL::m_dSpaceID,
                                      g_meshresManager.getSideLengthX(l_resNew),
                                      g_meshresManager.getSideLengthY(l_resNew),
                                      g_meshresManager.getSideLengthZ(l_resNew) );
			   dGeomSetData(m_dGeomID, (void*) this);
               dMassSetBox(&m_dMass, 1.0f,
                           g_meshresManager.getSideLengthX(l_resNew),
                           g_meshresManager.getSideLengthY(l_resNew),
                           g_meshresManager.getSideLengthZ(l_resNew) );
               break;

            case 2:
               m_dGeomID = dCreateSphere(LEVEL::m_dSpaceID,
                                      g_meshresManager.getRad(l_resNew));
			   dGeomSetData(m_dGeomID, (void*) this);
               dMassSetSphere(&m_dMass, 1.0f, g_meshresManager.getRad(l_resNew));
               break;
            default: l_bSpecialGeom = false; //doch nicht
         }
      }
   }

   if (l_bSpecialGeom == false)
   {

      // -- sicherstellen dass ein TriMeshData zur Verfügung steht und
      //    dann ein TriMesh-Geom daraus instanzieren
      //
      if (g_meshresManager.createTriMeshData(l_resNew) == false) return false;
   
      m_dGeomID = dCreateTriMesh (LEVEL::m_dSpaceID,
                                  g_meshresManager.getTriMeshDataID(l_resNew), // dTriMeshDataID Data,
                                  NULL,                 //dTriCallback *Callback,
                                  NULL,            //dTriArrayCallback * ArrayCallback,
                                  NULL);                          //dTriRayCallback* RayCallback); 
	  dGeomSetData(m_dGeomID, (void*) this);
      dMassSetBox(&m_dMass, 1.0f,
                  g_meshresManager.getSideLengthX(l_resNew),
                  g_meshresManager.getSideLengthY(l_resNew),
                  g_meshresManager.getSideLengthZ(l_resNew) );
   }



   // -- diesen neuen Geom nun zum Space und dem Body hinzufügen
   //
   if (m_dGeomID)
   {
      // -- Transformation Geom vorher zwischenschieben 
      //
      dGeomSetData(m_dGeomID, (void*) this);
      dGeomSetBody(m_dGeomID, m_dBodyID);
   }else{
      hr2message(__LINE__,-1, "dCreateTriMesh() failed to return a valid GeomID");
      return false;
   }
   return true;
}

/*
bool ENEMY::setOffset(float x, float y, float z)
{
   PSXVECTOR l_vOffset; 

   //l_dGeomIDEncapsulated = dGeomTransformGetGeom(m_dGeomID);

   // -- bei reinen Collision-Primitives, den Pivot etwas versetzen,
   //    da dieser insbesondere bei der Lasergun nicht dem Rotationspivot entspricht
   //
   if (dGeomGetClass(m_dGeomID) != dTriMeshClass)
   {
      l_vOffset.x = x + g_meshresManager.getMiddleX(m_ResType) * m_fScale;
      l_vOffset.y = y + g_meshresManager.getMiddleY(m_ResType) * m_fScale;
      l_vOffset.z = z + g_meshresManager.getMiddleZ(m_ResType) * m_fScale;
   }else{
      l_vOffset.x = x;
      l_vOffset.y = y;
      l_vOffset.z = z;
   }
   //dGeomSetOffset(l_dGeomIDEncapsulated, l_vOffset.x, l_vOffset.y, l_vOffset.z);

   if (m_dGeomID) dGeomSetOffsetPosition(m_dGeomID, l_vOffset.x, l_vOffset.y, l_vOffset.z);
   return true;
}
*/
/*
bool ENEMY::setOffsetMat(D3DMATRIX* l_pmatValue)
{
   dMatrix3 l_dMatrix3;
   l_dMatrix3[0]  = l_pmatValue->_11;
   l_dMatrix3[1]  = l_pmatValue->_21;
   l_dMatrix3[2]  = l_pmatValue->_31;
   l_dMatrix3[3]  = l_pmatValue->_41;
   l_dMatrix3[4]  = l_pmatValue->_12;
   l_dMatrix3[5]  = l_pmatValue->_22;
   l_dMatrix3[6]  = l_pmatValue->_32;
   l_dMatrix3[7]  = l_pmatValue->_42;
   l_dMatrix3[8]  = l_pmatValue->_13;
   l_dMatrix3[9]  = l_pmatValue->_23;
   l_dMatrix3[10] = l_pmatValue->_33;
   l_dMatrix3[11] = l_pmatValue->_43;
   if (m_dGeomID) dGeomSetOffsetRotation(m_dGeomID, l_dMatrix3);
   return true;
}
// -- gliedert das geom wieder in den gesamtbody des leaders ein
//
/*
bool ENEMY::getBackIntoLeadersBody()
{
   if (m_pLeader && m_pLeader->m_dBodyID)
   {
      setPartOfLeader(true);
   
      // -- Überweisung des Geoms in den Body des Leaders
      //
      if (m_dGeomID)
      {
         dGeomSetBody(m_dGeomID, m_pLeader->m_dBodyID);
      }
   
      // -- freigabe der eigenen BodyID
      //
      if (releaseBody() == false) return false;
 
      m_dBodyID = m_pLeader->m_dBodyID;
   }

  

   return true;
}
*/
// -- ehemalige Bigres-Items sollen sich als Trümmerteile verselbstständigen
//
/*
bool ENEMY::becomeIndependent()
{
   WriteLogFile(g_cEnemyTypeNameArray[mType]);
   WriteLogFile("::becomeIndependent()\n");

   if (isPartOfLeader())
   {
      if (mType==ETYP_BIGRESITEM)
      {
         //if (SetType(ETYP_STATIC) == false) return false;
         if (m_ResType == RESTYPE_WINGMAN)
         {
            mType = ETYP_WINGMAN;
         }else{
            mType = ETYP_STATIC;
         }
      }
   
      setPartOfLeader(false);
       
      WriteLogFile("greife gleich auf Leader zu\n");
      m_fSpeed = m_pLeader->m_fSpeed;

      //m_pLeader = NULL;
   
      if (m_dGeomID)
      {
         WriteLogFile("dGeomClearOffset\n");
         dGeomClearOffset(m_dGeomID);
   
         // -- eigenen Body instanzieren und aktuellen Werten aufpeppeln
         //
         WriteLogFile("call createBody()\n");
         if (createBody()== false) return false;
         WriteLogFile("call setBodyMat()\n");
         if (setBodyMat()== false) return false;
         WriteLogFile("call dBodySetPosition()\n");
         dBodySetPosition(m_dBodyID, m_vPos.x, m_vPos.y, m_vPos.z);
   
         if (m_dBodyID)
         {
            WriteLogFile("call dGeomSetBody()\n");
            dGeomSetBody(m_dGeomID, m_dBodyID);
         }

         WriteLogFile("call dBodySetMass()\n");
         dBodySetMass(m_dBodyID, &m_dMass);
      }
   }
   return true;
}
*/
/*
bool ENEMY::setScale(float l_fNewScale)
{
   // -- etwaige Non-Triangle Collisionprimitives neu skalieren
   //
   if (l_fNewScale != 1.0f &&
       l_fNewScale != m_fScale)
   {
      if (m_dGeomID)
      { 
         dVector3 l_dLengthArray;
         dReal l_dLength, l_dRad;

         switch (dGeomGetClass(m_dGeomID))
         {
            case dBoxClass:
               dGeomBoxGetLengths( m_dGeomID, l_dLengthArray);
               dGeomBoxSetLengths( m_dGeomID, 
                                   l_dLengthArray[0] * l_fNewScale,
                                   l_dLengthArray[1] * l_fNewScale,
                                   l_dLengthArray[2] * l_fNewScale);
               break;
            case dSphereClass:
               dGeomSphereSetRadius (m_dGeomID, dGeomSphereGetRadius(m_dGeomID) * l_fNewScale);
               break;
            case dCapsuleClass:
               dGeomCapsuleGetParams(m_dGeomID, &l_dRad, &l_dLength);
               dGeomCapsuleSetParams(m_dGeomID, l_dRad * l_fNewScale, l_dLength * l_fNewScale);
               break;
         }
      }
   }

   m_fScale = l_fNewScale;
   CalculateRad();
   return true;
}
// -- Holt den Radius aus der Resouce und rechnet den neuen Radius 
//    dabei kommt noch ein Scale faktor drauf, der vom Asteroidenfeld genutzt wird
//
*/
void ENEMY::CalculateRad()
{
   setRadius(g_meshresManager.getRad(m_ResType));
}
// -- just set rad when no standard resource type is used
//
void ENEMY::setRadius(float l_fNewRad)
{
	m_fRad = l_fNewRad;
/*
   m_fRad = l_fNewRad * m_fScale;
   if (g_Player.getRescue())
   {
      m_fCollisionRadPow2 = pow2(m_fRad+g_meshresManager.getRad(RESTYPE_RESCUE));
   }else{
      m_fCollisionRadPow2 = pow2(m_fRad+g_bigresArray[BIGRES_COBRA].m_fRad);
                                        //g_bigresArray[BIGRES_COBRA].m_fScale);
   }
   */
   m_fShootHitRadPow2  = m_fRad * m_fRad;
}
/*
// --- CalculateCamerarpace()
//     Ermittelt die lfd. 2D Koordinaten und den Cameraspace Radius
//
/*
void ENEMY::CalculateCameraSpace()
{
   float xydiv;
   float xDest, yDest, zDest;
   PSXVECTOR l_vPos;

   l_vPos.x = m_vPos.x + g_matCamera._13 * m_fRad;
   l_vPos.y = m_vPos.y + g_matCamera._23 * m_fRad;
   l_vPos.z = m_vPos.z + g_matCamera._33 * m_fRad;

   xDest = g_matViewProj._11*l_vPos.x + 
           g_matViewProj._21*l_vPos.y + 
           g_matViewProj._31*l_vPos.z + 
           g_matViewProj._41;
   yDest = g_matViewProj._12*l_vPos.x + 
           g_matViewProj._22*l_vPos.y + 
           g_matViewProj._32*l_vPos.z + 
           g_matViewProj._42;
   zDest = g_matViewProj._13*l_vPos.x + 
           g_matViewProj._23*l_vPos.y + 
           g_matViewProj._33*l_vPos.z + 
           g_matViewProj._43;
   xydiv = g_matViewProj._14*l_vPos.x + 
           g_matViewProj._24*l_vPos.y + 
           g_matViewProj._34*l_vPos.z + 
           g_matViewProj._44;

   if (xydiv == 0.0f) 
   {
      char l_cEnemyNo[100];
      PSXPrintf(__LINE__, l_cEnemyNo, sizeof(l_cEnemyNo),
              "Enemyarray Object %i :" 
              "translating to 2d would be division by zero!\n", 
              (m_idxEnemy));
      WriteLogFile(l_cEnemyNo);
      m_vPos2D.x = m_vPos.x;
      m_vPos2D.y = m_vPos.y;
      m_vPos2D.z = m_vPos.z;
      m_fRad2D   = m_fRad;

      return; // failsafe abfrage: weitermachen waere in diesem falle fatal
   }

   // -- zur spaeteren verwendung abspeichern
   //

   m_vPos2D.x = xDest/xydiv;
   m_vPos2D.y = yDest/xydiv;
   m_vPos2D.z = zDest; //xydiv;
   m_fRad2D = m_fRad / xydiv;
}
*/
// --- checkVisibility():
//     Pruefen ob Zeichnen des Gegners notwendig wird
//
/*
void ENEMY::checkVisibility()
{
   setVisible(false);

   // --- liegt es vor dem Gegner
   //
   if (m_vPos2D.z > 0)
   {

      // -- Pruefen ob Objektmittelpunkt in den um Radius verbreiterten
      ///   Viewport passt
      //
      if ((m_vPos2D.x <  (1.0f + m_fRad2D * m_fScale)) &&
          (m_vPos2D.x > (-1.0f - m_fRad2D * m_fScale)) &&
          (m_vPos2D.y <  (1.0f + m_fRad2D * m_fScale)) &&
          (m_vPos2D.y > (-1.0f - m_fRad2D * m_fScale)) )
      {
         if (m_fRad2D * m_fScale  > m_fMinRad2DForVisibility) // Mindestgroesse
         {  
            setVisible(true);
         }
      } // end-if mitte innerhalb screen
      
   } // end-if zdest > 0
}
*/
// --- TreatAsExplosion()
//     Behandlung einer Explosion
//
bool ENEMY::TreatAsExplosion()
{
   m_cntGeneric ++;


   if (m_cntGeneric>g_maxExplosionSteps)
   {
      m_cntGeneric = 0;
      SetType(ETYP_EMPTY);
   }

   return true;
}
// -- behandlungsroutine fuer Feldgeneratoren
//
/*
bool ENEMY::TreatAsGenerator()
{

   // -- Pruefen ob Aufprall mit Flagschiffhuelle bevorsteht
   //    
   if (isCheckFSCollision()) // aufmerksamkeitsflag
   {
      float  l_fFSIntersectAt;
      bool l_bFSIntersectValid;
   
      PSXFACEINFO* l_pfaceinfFSIntersect;

      // -- kollisionen mit Flagschiff-Faces selektieren
      //
      if (CheckFlagshipCollision( &l_bFSIntersectValid, 
                                  &l_fFSIntersectAt,
                                  &l_pfaceinfFSIntersect ) == false)
      {
         return false;
      }

      // -- bei kollision nachsehen ob ganz aufgeschlagen
      //    sonst abmelden aus der weiteren kollisionspruefung
      //
      if (l_bFSIntersectValid)
      {
         if (l_fFSIntersectAt >= 0.0f && l_fFSIntersectAt < m_fSpeed )
         {
            memset(&m_vStep, 0, sizeof(m_vStep));
            detonate();
            return true;
         }
      }
   }

   switch(m_Action)
   {
      case ENACT_COUNTDOWN:
 
         m_cntGeneric = m_cntGeneric + 1;

         if (m_cntGeneric > 10)
         {
            detonate();
         }

         break;
      case ENACT_DONOTHING:
         break;
      default:
         hr2message(__LINE__,-1, "not implemented EnemyAction in TreatAsMine()");
         return false;
   } // end-switch
   return true;

}
*/
// -- behandlungsroutine fuer aktivierte Minen
//
/*
bool ENEMY::TreatAsMine()
{
   // -- Mine schiesst um sich, wenn diese nicht im Detonationscountdown ist (=triggered)
   //    und keinem Leader unterstellt ist
   //
   if (m_fDestLength < 400000.0f && m_pLeader == NULL)
   {
      if (isTriggered() == false && 
          m_Action == ENACT_DONOTHING &&
          m_cntCurrentlyAttackingMines < g_maxCurrentlyAttackingMines)
      { 

         setTriggered(true);
         m_Action = ENACT_CHASEPLAYER;
         m_cntCurrentlyAttackingMines++;
      }

   }else{
      if (isTriggered() == true)  
      {
         setTriggered(false);
         m_cntCurrentlyAttackingMines--;

      }
   }
   // -- pruefen, ob mine nicht wieder abgeschaltet werden soll, 
   //    weil asteroid im wanderweg steht
   //
         if (isTriggered())
         {
            int l_idxEnemy, l_cntHitPoints,l_cntSuspect, l_idxSuspect;
            float l_fEnemyEnemyLength;
            PSXVECTOR l_vEnemyToEnemyDelta;
            int l_iAlignedPosArray[4];

            memset((void*)l_iAlignedPosArray, 0, sizeof(l_iAlignedPosArray));
            memcpy((void*)l_iAlignedPosArray, (void*)&g_Player.m_vPos, sizeof(g_Player.m_vPos));
         
            l_cntSuspect = 
               PSXSelectHitObjects(
                   &(enemyxarray[0].m_vPos), 
                   g_cntEnemy,
                   (PSXVECTOR*)l_iAlignedPosArray,
                   sizeof(ENEMY),
                   (void**)g_arr_pEnemy,
                   int(&(enemyxarray[0].m_fShootHitRadPow2))-int(enemyxarray),
                   int(&(enemyxarray[0].m_vPos))-int(enemyxarray),
                   int(g_bSupportingSSE),
                   400000.0f // als Playerradius
                   );

            // -- alle asteroiden in trigger reichweite pruefen 
            //
            for (l_idxSuspect=0; l_idxSuspect < l_cntSuspect; l_idxSuspect++)
            {
               if (g_arr_pEnemy[l_idxSuspect]->mType == ETYP_ASTEROID) 
               {
                  float b,l_fSphereSphereDistance;
          
                  l_vEnemyToEnemyDelta.x = m_vPos.x - g_arr_pEnemy[l_idxSuspect]->m_vPos.x;
                  l_vEnemyToEnemyDelta.y = m_vPos.y - g_arr_pEnemy[l_idxSuspect]->m_vPos.y;
                  l_vEnemyToEnemyDelta.z = m_vPos.z - g_arr_pEnemy[l_idxSuspect]->m_vPos.z;

                  l_fEnemyEnemyLength = pow2(l_vEnemyToEnemyDelta.x ) +
                                        pow2(l_vEnemyToEnemyDelta.y ) +
                                        pow2(l_vEnemyToEnemyDelta.z );

                  l_fSphereSphereDistance = l_fEnemyEnemyLength -
                                            g_arr_pEnemy[l_idxSuspect]->m_fShootHitRadPow2 - 
                                            m_fShootHitRadPow2;
      
                  if (l_fSphereSphereDistance <= m_fDestLength)
                  {
                     b = 2 * ( m_vCurrentNode.x * ( l_vEnemyToEnemyDelta.x) +
                               m_vCurrentNode.y * ( l_vEnemyToEnemyDelta.y) +
                               m_vCurrentNode.z * ( l_vEnemyToEnemyDelta.z));

                     l_cntHitPoints = int(pow2(b) - 4.0f * m_fDestLength * l_fSphereSphereDistance);

                     if (l_cntHitPoints>=0)
                     {
                        m_Action = ENACT_DONOTHING;
                        setTriggered(false);
                        m_cntCurrentlyAttackingMines--;
                        memset(&m_vStep, 0, sizeof(m_vStep));

                        l_idxSuspect = l_cntSuspect; //genug gesehen, raus
                     
                     }
                  }
               }
            }
         }
   // -- Pruefen ob Aufprall mit Flagschiffhuelle bevorsteht
   //    
   if (isCheckFSCollision()) // aufmerksamkeitsflag
   {
      float  l_fFSIntersectAt;
      bool l_bFSIntersectValid;
   
      PSXFACEINFO* l_pfaceinfFSIntersect;
      
      // -- kollisionen mit Flagschiff-Faces selektieren
      //
      if (CheckFlagshipCollision( &l_bFSIntersectValid, 
                                  &l_fFSIntersectAt,
                                  &l_pfaceinfFSIntersect ) == false)
      {
         return false;
      }

      // -- bei kollision nachsehen ob ganz aufgeschlagen
      //    sonst abmelden aus der weiteren kollisionspruefung
      //
      if (l_bFSIntersectValid)
      {
         if (l_fFSIntersectAt >= 0.0f && l_fFSIntersectAt < m_fSpeed )
         {
            memset(&m_vStep, 0, sizeof(m_vStep));
            detonate();
            setTriggered(false);
         }
      }
   }
   if (isTriggered() == false && m_Action != ENACT_ESCAPE)
   {
      return true;
   }

   switch(m_Action)
   {
      case ENACT_COUNTDOWN:
 
         m_cntGeneric = m_cntGeneric + 1;

         if (m_cntGeneric > 50)
         {
            m_fExplScale = 8.0;
            //g_Player.m_fRumbleScale = 0.1f;
            g_Player.m_fShieldAmount = g_Player.m_fShieldAmount - 10.0f;
            detonate();
         }
         break;

      case ENACT_CHASEPLAYER:

         PSXVECTOR l_vPredictedPos;
         PSXVECTOR l_vDistance;
         float l_fDistance, l_fDistPow2;
         l_vPredictedPos.x = g_Player.m_pEnemy->getPosX() + g_Player.m_pEnemy->getStepX() * 5.0f;
         l_vPredictedPos.y = g_Player.m_pEnemy->getPosY() + g_Player.m_pEnemy->getStepY() * 5.0f;
         l_vPredictedPos.z = g_Player.m_pEnemy->getPosZ() + g_Player.m_pEnemy->getStepZ() * 5.0f;

         l_vDistance.x = l_vPredictedPos.x - m_vPos.x;
         l_vDistance.y = l_vPredictedPos.y - m_vPos.y;
         l_vDistance.z = l_vPredictedPos.z - m_vPos.z;
        
         l_fDistPow2 = pow2(l_vDistance.x)+ 
                       pow2(l_vDistance.y)+ 
                       pow2(l_vDistance.z);

         l_fDistance = sqrt(l_fDistPow2);
         if (g_iCurrStarBase == 1) // nur im 1. StarBase, damit diese im Tunnel nicht wandern
         {
            m_fSpeed      = 2.0f;
            m_vStep.x     = l_vDistance.x/l_fDistance * m_fSpeed;
            m_vStep.y     = l_vDistance.y/l_fDistance * m_fSpeed;
            m_vStep.z     = l_vDistance.z/l_fDistance * m_fSpeed;
         }
         if (m_cntGeneric)
         {
            m_cntGeneric--;
         }else{
            int l_cntFound;
            SHOOT* l_pShoot;
      
            m_cntGeneric = 50;

            l_cntFound = PSXAllocateShoot(1, &l_pShoot);
      
            if (l_cntFound > 0)
            {
      
               l_pShoot->m_vPos.x = m_vPos.x;
               l_pShoot->m_vPos.y = m_vPos.y;
               l_pShoot->m_vPos.z = m_vPos.z;
       
               l_pShoot->m_vStep.x     = l_vDistance.x/l_fDistance * 8.0f;
               l_pShoot->m_vStep.y     = l_vDistance.y/l_fDistance * 8.0f;
               l_pShoot->m_vStep.z     = l_vDistance.z/l_fDistance * 8.0f;
      
               l_pShoot->m_cntTimeToLive = g_maxTimeToLiveMenaceSelf;
               l_pShoot->menacetype      = MENACE_SELF;
               l_pShoot->m_penemyHitableArray = NULL;
               l_pShoot->m_idxShootDrawer     = SHOOTDRAWER_BLUELASER;
      
               if (g_options.IsEnableSound())
               {
                  if (g_sound_array[SNDRES_BATTLELASER_FAR].play(this) == false) return false;
               }
      
               g_idxNewestShoot = l_pShoot - shoot_array;

            }
         }
         break;

      case ENACT_ESCAPE:
         m_fViewRotation1 = 0.25f;
         m_fViewRotation2 = 0.1f;
         RotationToMatrix(); m_fViewRotation1 = 0.0f; m_fViewRotation2 = 0.0f;
         break;



      case ENACT_DONOTHING:
         if (setStep(0.0f, 0.0f, 0.0f) == false) return false;
         break;
      default:
         hr2message(__LINE__,-1, "not implemented EnemyAction in TreatAsMine()");
         return false;
   } // end-switch

   return true;
}
*/
/*
bool ENEMY::TreatAsGun()
{
   D3DMATRIX l_matShoot;
   PSXVECTOR l_vPlayerGunRel;
   PSXWVECTOR l_vwQuat;

   if (m_fDestLength < 20000000.0f && 
       (g_Player.m_bEntered == false && mType == ETYP_GUN ||
        g_Player.m_bEntered == true &&  mType  == ETYP_REACTORGUN) &&
       g_Player.m_crimePoliceFile != CRID_CLEAN)
   { 
      float l_fPlayer;
      l_fPlayer = m_Mat._31*(g_Player.m_pEnemy->getPosX()-m_vPos.x) + 
                  m_Mat._32*(g_Player.m_pEnemy->getPosY()-m_vPos.y) + 
                  m_Mat._33*(g_Player.m_pEnemy->getPosZ()-m_vPos.z); 

      if (l_fPlayer > 0)
      {
         if (isTriggered() == false)
         {
            setTriggered(true);
            m_cntGeneric = g_cntGunShootDelay;
         }
         g_cntGunShootDelay = g_cntGunShootDelay + 4;

      }else{
         setTriggered(false);
      }
   }else{
      setTriggered(false);
   }

   if (isTriggered() == false) return true; 

   if (m_cntGeneric == 0)
   {
      int l_cntFacesInWay;
      PSXVECTOR l_vPredictedPos, l_vLaunchPos;

      l_vLaunchPos.x = m_vPos.x + m_Mat._31 * m_fRad;
      l_vLaunchPos.y = m_vPos.y + m_Mat._32 * m_fRad;
      l_vLaunchPos.z = m_vPos.z + m_Mat._33 * m_fRad;

      m_cntGeneric = float(g_maxGunShootDelay);

      l_vPredictedPos.x = g_Player.m_pEnemy->getPosX() + g_Player.m_pEnemy->getStepx() * 20.0f;
      l_vPredictedPos.y = g_Player.m_pEnemy->getPosY() + g_Player.m_pEnemy->getStepY() * 20.0f;
      l_vPredictedPos.z = g_Player.m_pEnemy->getPosZ() + g_Player.m_pEnemy->getStepZ() * 20.0f;

      // -- Feuern wenn das Flagschiff nicht selbst im Wege steht
      //
      //if (l_cntFacesInWay == 0)

      {
         int l_cntFound;
         SHOOT* l_pShoot;
         PSXVECTOR l_vDistance;
         float l_fDistance, l_fDistPow2;

         l_vDistance.x = l_vPredictedPos.x - l_vLaunchPos.x;
         l_vDistance.y = l_vPredictedPos.y - l_vLaunchPos.y;
         l_vDistance.z = l_vPredictedPos.z - l_vLaunchPos.z;
        
         l_fDistPow2 = pow2(l_vDistance.x)+ 
                       pow2(l_vDistance.y)+ 
                       pow2(l_vDistance.z);
   
         l_fDistance = sqrt(l_fDistPow2);
   
         l_cntFound = PSXAllocateShoot(1, &l_pShoot);
         if (l_cntFound > 0)
         {
            l_pShoot->m_vPos.x = l_vLaunchPos.x;
            l_pShoot->m_vPos.y = l_vLaunchPos.y;
            l_pShoot->m_vPos.z = l_vLaunchPos.z;
            
            l_pShoot->m_vStep.x     = l_vDistance.x/l_fDistance * 40.0f;
            l_pShoot->m_vStep.y     = l_vDistance.y/l_fDistance * 40.0f;
            l_pShoot->m_vStep.z     = l_vDistance.z/l_fDistance * 40.0f;
   
            l_pShoot->m_cntTimeToLive = g_maxTimeToLiveMenaceSelf;
            l_pShoot->menacetype      = MENACE_SELF;
            l_pShoot->m_idxShootDrawer     = SHOOTDRAWER_BLUELASER;
            l_pShoot->m_penemyHitableArray = NULL;
   
            if (g_options.IsEnableSound())
            {
               if (g_sound_array[SNDRES_BATTLELASER_FAR].play(this) == false) return false;
            }
   
            g_idxNewestShoot = l_pShoot - shoot_array;
   
         }
      }
   }else{
      m_cntGeneric = m_cntGeneric - int(g_fElapsedTime);
   }  
   return true;
}
*/
// --- RotationToMatrix()
//     Die Rotationsvorgaben zur Matrix verrrechnen
//

void ENEMY::RotationToMatrix()
{
	
	g_matSingleMovement = XMMatrixRotationY(m_fViewRotation1) * XMMatrixRotationX(m_fViewRotation2);

   
   /*
   g_ehdlBuf.m_bSingleMovementInPipe = true;
   */
}

/*
bool ENEMY::TreatAsGhost()
{
   D3DMATRIX l_matShoot;
   PSXVECTOR l_vPlayerGhostRel;
   PSXVECTOR l_vPredictedPos;
   int l_cntFound;
   SHOOT* l_pShoot;
   float l_fDistance, l_fDistPow2;

   l_vPredictedPos.x = g_Player.m_pEnemy->getPosX() + g_Player.m_pEnemy->getStepx() * 05.0f;
   l_vPredictedPos.y = g_Player.m_pEnemy->getPosY() + g_Player.m_pEnemy->getStepY() * 05.0f;
   l_vPredictedPos.z = g_Player.m_pEnemy->getPosZ() + g_Player.m_pEnemy->getStepZ() * 05.0f;

   l_vPlayerGhostRel.x = l_vPredictedPos.x - m_vPos.x;
   l_vPlayerGhostRel.y = l_vPredictedPos.y - m_vPos.y;
   l_vPlayerGhostRel.z = l_vPredictedPos.z - m_vPos.z;
  
   l_fDistPow2 = pow2(l_vPlayerGhostRel.x)+ 
                 pow2(l_vPlayerGhostRel.y)+ 
                 pow2(l_vPlayerGhostRel.z);

   l_fDistance = sqrt(l_fDistPow2);

   //--- Ghost sollen auf den player zufliegen
   //
   if (l_fDistance >= 40000.0f)
   {
      m_fSpeed  = 4.0f;
   }else{
      m_fSpeed  = 1.0f;
   }

   m_vStep.x = (l_vPlayerGhostRel.x/l_fDistance * m_fSpeed);
   m_vStep.y = (l_vPlayerGhostRel.y/l_fDistance * m_fSpeed);
   m_vStep.z = (l_vPlayerGhostRel.z/l_fDistance * m_fSpeed);

   if(m_Action==ENACT_COUNTDOWN)
   {
      g_Player.m_fShieldAmount = 0.0f;
   }

   if (m_cntGeneric == 0)
   {
      int l_cntFacesInWay;
      // -- pruefen ob ein fs-face zwischen gun und player steht
      //
      l_cntFacesInWay = 0;
      if (g_Player.m_bNearStarBase)
      {
         int l_idxBigResItem;

         m_cntGeneric = int(25.0f / g_fBalanceFactor);

         // -- face und player aus 2D-sicht der gun
         //    die 2d position des players wird dann zum ursprung gemacht
         //

         
         if (PSXMatrixFromVector( &l_matShoot, l_vPlayerGhostRel) == false)
         {
            return false;
         }

         for (l_idxBigResItem = g_levelContent.m_pBigRes->m_idxBigResItem;
              l_idxBigResItem < g_levelContent.m_pBigRes->m_cntBigResItem;
              l_idxBigResItem ++)
         {
            int l_cntFaces;
            int l_iAlignedPosArray[4];
 
            float l_fSearchRadPow2;
            float l_fGhostLengthPow2;

            RESTYPE l_resPart;

            PSXFACE*   l_faceArray;
            PSXVERTEX* l_vertArray;
            PSXFACEINFO* l_faceinfCurr;
      
            PSXVECTOR l_vGhostFSPartRel, l_vPlayerFSPartRel;

            int l_cntSuspect, l_idxSuspect, l_idxFace;

            l_vGhostFSPartRel.x = m_vPos.x - 
                                  g_bigresitemArray[l_idxBigResItem].m_vPos.x - 
                                  g_levelContent.m_vPos.x;
            l_vGhostFSPartRel.y = m_vPos.y - 
                                  g_bigresitemArray[l_idxBigResItem].m_vPos.y - 
                                  g_levelContent.m_vPos.y;
            l_vGhostFSPartRel.z = m_vPos.z - 
                                  g_bigresitemArray[l_idxBigResItem].m_vPos.z - 
                                  g_levelContent.m_vPos.z;

            l_fGhostLengthPow2 = pow2( l_vGhostFSPartRel.x)+
                                 pow2( l_vGhostFSPartRel.y)+
                                 pow2( l_vGhostFSPartRel.z);

            l_vPlayerFSPartRel.x = l_vPredictedPos.x - 
                                   g_bigresitemArray[l_idxBigResItem].m_vPos.x - 
                                   g_levelContent.m_vPos.x;
            l_vPlayerFSPartRel.y = l_vPredictedPos.y - 
                                   g_bigresitemArray[l_idxBigResItem].m_vPos.y - 
                                   g_levelContent.m_vPos.y;
            l_vPlayerFSPartRel.z = l_vPredictedPos.z - 
                                   g_bigresitemArray[l_idxBigResItem].m_vPos.z - 
                                   g_levelContent.m_vPos.z;
         
            l_resPart = g_bigresitemArray[l_idxBigResItem].m_resPart;
         
            l_fSearchRadPow2 = pow2( l_vPlayerFSPartRel.x)+
                               pow2( l_vPlayerFSPartRel.y)+
                               pow2( l_vPlayerFSPartRel.z);
         
            l_cntFaces  = resourceoffsets[l_resPart].cnt_faces;
            l_faceArray = (PSXFACE*)(resourceoffsets[l_resPart].face_offset);
            l_vertArray = (PSXVERTEX*)
                          (resourceoffsets[l_resPart].vertex_offset);
            l_faceinfCurr = g_bigresitemArray[l_idxBigResItem].m_faceinfArray;
      
            // -- evtl Faces im Radius der PlayerEntfernung selektieren
            //    
            l_cntSuspect = 0;
            memset((void*)l_iAlignedPosArray, 0, sizeof(l_iAlignedPosArray));
            memcpy((void*)l_iAlignedPosArray, (void*)&l_vGhostFSPartRel, sizeof(l_vGhostFSPartRel));
            if (l_cntFaces > 0)
            {
               l_cntSuspect = 
               PSXSelectHitObjects(
                   &(l_faceinfCurr[0].vCentroid), 
                   l_cntFaces,
                   (PSXVECTOR*)l_iAlignedPosArray,
                   sizeof(PSXFACEINFO),
                   (void**)g_pfaceinfSelectedArray,
                   int(&(l_faceinfCurr[0].fRadPow2))-int(l_faceinfCurr),
                   int(&(l_faceinfCurr[0].vCentroid))-int(l_faceinfCurr),
                   int(g_bSupportingSSE), l_fSearchRadPow2 ); 
            }

            // -- fuer jeden Hinweis tatsaechliche Faceintersection pruefen
            //
            for (l_idxSuspect=0; l_idxSuspect < l_cntSuspect; l_idxSuspect++)
            {
               PSXVECTOR l_vFaceGhostRel[3];
               float l_fFaceGhostViewed_z[3];

               int l_idxVert[3];
               bool l_bVertInFront;
               float l_fMinZ;

               //l_idxFace = (g_pfaceinfSelectedArray[l_idxSuspect]) - l_faceinfCurr;

               l_idxVert[0]  = l_faceArray[l_idxFace].a;
               l_idxVert[1]  = l_faceArray[l_idxFace].b;
               l_idxVert[2]  = l_faceArray[l_idxFace].c;

               l_bVertInFront = false;
               for (int j=0; j<3; j++)
               {
                  l_vFaceGhostRel[j].x = l_vertArray[l_idxVert[j]].x - l_vGhostFSPartRel.x;
                  l_vFaceGhostRel[j].y = l_vertArray[l_idxVert[j]].y - l_vGhostFSPartRel.y;
                  l_vFaceGhostRel[j].z = l_vertArray[l_idxVert[j]].z - l_vGhostFSPartRel.z;

                  l_fFaceGhostViewed_z[j] = m_Mat._31*l_vFaceGhostRel[j].x + 
                                          m_Mat._32*l_vFaceGhostRel[j].y + 
                                          m_Mat._33*l_vFaceGhostRel[j].z; 
                  if (l_fFaceGhostViewed_z[j] > 0)
                  {
                     l_bVertInFront = true;
                  }
               }

               // -- pruefen ob face im mittelpunkt des gun beobachtungsfeldes 
               //
               if (l_bVertInFront)
               {
                  PSXVECTOR l_vLineArray[3];
                  PSXVECTOR l_vDeltaArray2D[3], l_vLineArray2D[4];


                  float l_fIncrement[3];
                  HRESULT hr;
                  bool l_bCheckInclusion, l_bInclusion;

                  l_bCheckInclusion = false;


                  for (int j=0; j<3; j++)
                  {
                     float x,y,z;
   
                     x = l_vFaceGhostRel[j].x;
                     y = l_vFaceGhostRel[j].y;
                     z = l_vFaceGhostRel[j].z;
   
                     l_vLineArray2D[j].x = l_matShoot._11*x + l_matShoot._12*y + l_matShoot._13*z;
                     l_vLineArray2D[j].y = l_matShoot._21*x + l_matShoot._22*y + l_matShoot._23*z;
                  }
           
                  l_bInclusion = false;

                  // -- getting delta values
                  //
                  l_vLineArray2D[3] = l_vLineArray2D[0];
                  for (int m=0; m<3; m++)
                  {
                     l_vDeltaArray2D[m].x = l_vLineArray2D[m+1].x - l_vLineArray2D[m].x;
                     l_vDeltaArray2D[m].y = l_vLineArray2D[m+1].y - l_vLineArray2D[m].y;
                  }
                  
                  // -- ermitteln, ob das face die mitte des gun-2d-bildschirms einschliesst
                  //
                  for (int i=0; i<3; i++)
                  {
                     // -- if line is crossing x axis boundary, first condition for
                     //    inclusion of the screencenter is given: screen center is at
                     //    the same height of the polygon
                     //
                     if ( (l_vLineArray2D[i].y   <=0) && (l_vLineArray2D[i+1].y >0)||
                          (l_vLineArray2D[i+1].y <=0) && (l_vLineArray2D[i].y   >0))
                     {
                        register float l_fAtZero, l_fIncrement;
                
               
                        l_fIncrement = l_vDeltaArray2D[i].x / l_vDeltaArray2D[i].y;
               
                        // -- x-value at zero on y axis 
                        //
                        l_fAtZero = l_vLineArray2D[i].x -
                                    l_vLineArray2D[i].y * l_fIncrement;
                  
                        // -- negate Inclusion flag if jumped over the fence walking to the
                        //    right: 
                        //
                        //    if screen center is at same height of polygon but
                        //    the fence have to be jumped twice, the flag will also
                        //    be negated twice => false again
                        //
                        if ((l_fAtZero > 0))
                        {
                           l_bInclusion = !l_bInclusion;
                        }
                     }
                  }
   
                  // -- pruefen, ob die player entfernung kleiner als die entfernung
                  //    zum schnittpunkt mit dem face
                  //
                  if (l_bInclusion)
                  {
                     float l_fIntersectAt;
                     
                     l_fIntersectAt = 
                       (l_vFaceGhostRel[0].x * l_faceinfCurr[l_idxFace].vNormal.x + 
                        l_vFaceGhostRel[0].y * l_faceinfCurr[l_idxFace].vNormal.y + 
                        l_vFaceGhostRel[0].z * l_faceinfCurr[l_idxFace].vNormal.z) /
                       ( (l_vPredictedPos.x - m_vPos.x)  * l_faceinfCurr[l_idxFace].vNormal.x + 
                         (l_vPredictedPos.y - m_vPos.y)  * l_faceinfCurr[l_idxFace].vNormal.y + 
                         (l_vPredictedPos.z - m_vPos.z)  * l_faceinfCurr[l_idxFace].vNormal.z);
               
                     if (l_fIntersectAt <= 1.0f && l_fIntersectAt >= 0.0f)
                     {
                        l_cntFacesInWay++;
                
                     }
                  }
               }
   
            } // end-foreach selected face
         }
      }

      // -- Feuern wenn das Flagschiff nicht selbst im Wege steht
      //
      if (l_cntFacesInWay == 0)
      {
         l_cntFound = PSXAllocateShoot(1, &l_pShoot);
         if (l_cntFound > 0)
         {
            l_pShoot->m_vPos.x = m_vPos.x;
            l_pShoot->m_vPos.y = m_vPos.y;
            l_pShoot->m_vPos.z = m_vPos.z;
            
            l_pShoot->m_vStep.x     = l_vPlayerGhostRel.x/l_fDistance * 40.0f;
            l_pShoot->m_vStep.y     = l_vPlayerGhostRel.y/l_fDistance * 40.0f;
            l_pShoot->m_vStep.z     = l_vPlayerGhostRel.z/l_fDistance * 40.0f;
   
            l_pShoot->m_cntTimeToLive = g_maxTimeToLiveMenaceSelf;
            l_pShoot->menacetype      = MENACE_SELF;

            l_pShoot->m_idxShootDrawer     = SHOOTDRAWER_REDLASER;
            l_pShoot->m_penemyHitableArray = NULL;
   
            if (g_options.IsEnableSound())
            {
               if (g_sound_array[SNDRES_BATTLELASER_FAR].play(this) == false) return false;
            }
   
            g_idxNewestShoot = l_pShoot - shoot_array;
         }
      }
   }else{
      m_cntGeneric = m_cntGeneric - 1;
   }  
   return true;
}
*/
// --- TreatAsChar()
//     Behandlung eines Char
//
/*
bool ENEMY::TreatAsChar()
{
   if(m_Action==ENACT_BANKROTATION)
   {
      if(m_iActionTimer>0)
      {
         m_iActionTimer = m_iActionTimer - int(g_fElapsedTime);
         m_fViewRotation1 = 0.25f;
         m_fViewRotation2 = 0.1f;
         RotationToMatrix(); m_fViewRotation1 = 0.0f; m_fViewRotation2 = 0.0f;
         if (g_Player.m_bNearStarBase)
         {
            setCheckFSCollision(true);
         }
      }else{
         m_Action = ENACT_DONOTHING;
         memcpy (&m_Mat,&g_matIdentity, sizeof(g_matIdentity));

         //-- Wenn der letzte Buchstabe der 'ThumbUp' war,
         //   gillt die letzte Mission, dass HighScore-eintragen,
         //   als beendet, an anderer stelle wird darauf hin,
         //   das menu activiert!
         //
         if(m_cntGeneric==28)
         {
            g_bHighScoreDone=true;
            DWORD NumberOfBytesWritten;

            HANDLE l_hHighScoreFileW;
            HRESULT hr;

            SCORELIST* l_SortData;

            char  l_cFuncText[100];

            l_SortData = new SCORELIST[g_maxHighScore];

            //-- Speicher initialisieren 
            //
            for(int s=0;s<g_maxHighScore;s++)
            {
               l_SortData[s] = g_HighScore[s];
            }  

            for(int i=0;i<g_maxHighScore;i++)
            {
               if(g_Player.m_iFightExperience>g_HighScore[i].iScore)
               {
                  g_HighScore[i].iScore = g_Player.m_iFightExperience;

                  memcpy(g_HighScore[i].cName,
                         g_cPlayerName, 
                         g_maxPlayerName);


                  for(int k=i;k<g_maxHighScore-1;k++)
                  {
                     g_HighScore[k+1] = l_SortData[k];
                  }


                  i=g_maxHighScore;
               }
            }

            delete [] l_SortData;

            //-- HighScore wieder zurueckschreiben!
            //
            l_hHighScoreFileW = CreateFile("stats.psx",
                                           GENERIC_WRITE, 
                                           0, 
                                           NULL,
                                           OPEN_ALWAYS,
                                           FILE_ATTRIBUTE_NORMAL,
                                           NULL);

            if (l_hHighScoreFileW==INVALID_HANDLE_VALUE) 
            {
               PSXPrintf(__LINE__, l_cFuncText, sizeof(l_cFuncText), "CreateFile(stats.psx)\n");
                       hr2message(__LINE__,GetLastError(),l_cFuncText);
               return(FALSE);
            }

            WriteFile(l_hHighScoreFileW, g_HighScore, sizeof(SCORELIST) * g_maxHighScore,
                      &NumberOfBytesWritten,NULL);

            CloseHandle(l_hHighScoreFileW);
         }
      }
      return true;
   }
   return true;
}
*/
// -- Transformiert aktuellen Anflug knoten in den relativen Enemy space
//

void ENEMY::TransformDest()
{
   //float x,y,z;
   XMVECTOR l_vDest;

   l_vDest = g_Player.m_pEnemy->getPos() - m_vPos;
	  
   //x = g_Player.m_pEnemy->getPosX() - m_vPos.x;
   //y = g_Player.m_pEnemy->getPosY() - m_vPos.y;
   //z = g_Player.m_pEnemy->getPosZ() - m_vPos.z;

   XMMATRIX l_matEnemy = XMMatrixTranspose(m_Mat);
   XMVECTOR l_vDestLocalSpace = XMVector3Transform(l_vDest, l_matEnemy);
   XMStoreFloat3(&g_ehdlBuf.m_vDest, l_vDestLocalSpace);
   //g_ehdlBuf.m_vDest.x = m_Mat._11*x + m_Mat._12*y + m_Mat._13*z; 
   //g_ehdlBuf.m_vDest.y = m_Mat._21*x + m_Mat._22*y + m_Mat._23*z; 
   //g_ehdlBuf.m_vDest.z = m_Mat._31*x + m_Mat._32*y + m_Mat._33*z; 
   

}

/*
void ENEMY::TransformStarBase()
{
   float x,y,z;

   if (g_levelContent.m_penemyStarBase)
   {
      x = g_levelContent.m_penemyStarBase->m_vPos.x - m_vPos.x;
      y = g_levelContent.m_penemyStarBase->m_vPos.y - m_vPos.y;
      z = g_levelContent.m_penemyStarBase->m_vPos.z - m_vPos.z;
   
      g_ehdlBuf.m_vStarBase.x = m_Mat._11*x + m_Mat._12*y + m_Mat._13*z; 
      g_ehdlBuf.m_vStarBase.y = m_Mat._21*x + m_Mat._22*y + m_Mat._23*z; 
      g_ehdlBuf.m_vStarBase.z = m_Mat._31*x + m_Mat._32*y + m_Mat._33*z; 
   }
}
*/
// -- schaltet index und pointer auf den globalen Polizeicharacter
//
/*
bool ENEMY::setCharacterToPolice()
{
   m_pCharacter   = CHARACTER::getCharacterPolice();
   if (m_pCharacter) 
   {
      m_idxCharacter = m_pCharacter->getCharacterIndex();
      return true;
   }else{
      return false;
   }
}
*/
// --- TreatAsLeader()
//     Behandlung eines Leaders
//
bool ENEMY::TreatAsLeader()
{
	

	if (g_penemyDebug == NULL)
		g_penemyDebug = this;

	/*
   if (!CHARACTER::IsAnyPolice()) 
   {
      hr2message(__LINE__,-1,"Police-Character is not defined, but needed in ENEMY::TreatAsLeader()\n");
      return false;
   }
   */
   // -- checkTrigger legt weitere aktionen 
   //    des characters fest..
   //
	/*
   if (setCharacterToPolice() == false) return false;
   if((getCharacter())->checkTrigger(this)==false) return false;

   if(isTriggered()==false)
   {
      if((getCharacter())->m_iAction == CHRACT_ATTACKPLAYER)
      {
         setTriggered(true);
         m_cntCurrentlyAttackingLeaders = m_cntCurrentlyAttackingLeaders + 1;
      }
   }else{
      if((getCharacter())->m_iAction != CHRACT_ATTACKPLAYER)
      {
         setTriggered(false);
         m_cntCurrentlyAttackingLeaders = m_cntCurrentlyAttackingLeaders - 1;
      }
   }

   */
   switch(m_Action)
   {
      case ENACT_DONOTHING:
		 m_Action = ENACT_CHASEPLAYER;
		 /*
         if (isTriggered())
         {
            m_Action = ENACT_CHASEPLAYER;
         }else{
            m_Action = ENACT_CURVEAROUND;
         }
         break;
		 
      case ENACT_CURVEAROUND:
         if (handleCurveAroundCircles() == false) return false;
		 */
         break;
		 
      default:
         // --- Verhalten eines wendigen Schiffes
         //
         if (handleDefaultSmallShip(10.0f,
                                   g_laserPoliceLeaderArray, 
                                   g_cntLaserPoliceLeader) == false) return false;
   }
   /*
   l_bFSIntersectValid = false;

   if (isCheckFSCollision())
   {
      // -- kollisionen mit Flagschiff-Faces selektieren
      //
      SetFlyerStep(); // da der Step in der Intersectionberechnung benutzt wird

      if (CheckFlagshipCollision( &l_bFSIntersectValid, 
                                  &l_fFSIntersectAt,
                                  &l_pfaceinfFSIntersect ) == false)
      {
         return false;
      }

      // -- bei kollision nachsehen ob ganz aufgeschlagen
      //    sonst abmelden aus der weiteren kollisionspruefung
      //
      if (l_bFSIntersectValid)
      {
         if (l_fFSIntersectAt < 0.0f)
         { 
            l_bFSIntersectValid = false;
            setCheckFSCollision(false); 
         }else{
            if (l_fFSIntersectAt < m_fSpeed )
            {
               memset(&m_vStep, 0, sizeof(m_vStep));
               detonate();
               return true;
            }
         }
      }
   }
   TransformDest();
   */
   
   
   // -- aktion entscheiden fuer den fall das Leader Triggerd ist
   //
   /*
   if ( isTriggered() ) 
   {
	   /*
      // -- Bei Hindernis auf Weg zum Player ganz aus dem Angriffsmodus rausgehen 
      //    
      if (l_bFSIntersectValid)
      {
         // -- entweder sofortabprall fuer kurzfristig absehbare kollision oder 
         //    searchwayout 
         //
         if (l_fFSIntersectAt/m_fSpeed < 20.0f)
         {
            PSXVECTOR l_vDestWind2D;
            m_vCurrNode.x = l_pfaceinfFSIntersect->vNormal.x;
            m_vCurrNode.y = l_pfaceinfFSIntersect->vNormal.y;
            m_vCurrNode.z = l_pfaceinfFSIntersect->vNormal.z;
   
            TransformDest();
   
            m_iActionTimer = 10;

            l_vDestWind2D.x = 0.3f;
            l_vDestWind2D.z = 0.3f;
            if (l_fFSIntersectAt/m_fSpeed < 10.0f)
            {
               m_Action = ENACT_ALIGNYHARD;  
               if ((g_ehdlBuf.m_vDest.x < l_vDestWind2D.x) && (g_ehdlBuf.m_vDest.x > -l_vDestWind2D.x))
               { 
   
                  if (g_ehdlBuf.m_vDest.z < 0.0f)
                  {
                     m_Action = ENACT_AVOIDFACEDOWNHARD;
                  }else{
                     m_Action = ENACT_AVOIDFACEUPHARD;
                  }
                  setCheckFSCollision(false); 
               }
            }else{
               m_Action = ENACT_ALIGNYSMOOTH;  
               if ((g_ehdlBuf.m_vDest.x < l_vDestWind2D.x) && (g_ehdlBuf.m_vDest.x > -l_vDestWind2D.x))
               { 
   
                  if (g_ehdlBuf.m_vDest.z < 0.0f)
                  {
                     m_Action = ENACT_AVOIDFACEDOWN;
                  }else{
                     m_Action = ENACT_AVOIDFACEUP;
                  }
                  setCheckFSCollision(false); 
               }
            }
         }else{

            if (m_Action == ENACT_CHASEPLAYER || 
                m_Action == ENACT_SHOOT ||
                m_Action == ENACT_ACCELERATE)
            {
               if (pow2(l_fFSIntersectAt*g_fBalanceFactor) < m_fDestLength)
               {
                  m_Action       = ENACT_SEARCHWAYOUT;
                  m_iActionTimer = sqrt(m_fDestLength) / m_fSpeed / g_fBalanceFactor;
               }
            }
 
           
         }
      }
      
      // -- wenn kein hinweis auf unmittelbare kollision vorhanden ist
      //    wird eventueller AVOIDFACE status zum PULL weitergeschaltet
      //    damit noch etwas abstand zum drohenden face gewonnen wird
      //
      if ((l_bFSIntersectValid && l_fFSIntersectAt < 100.0f) == false)
      {
         // -- kollision mit fs-huelle grad abgewendet 
         //    in dem falle gleich weiterschalten zum abstand gewinnen
         //
         if (m_Action == ENACT_AVOIDFACEUPHARD)   m_Action = ENACT_AVOIDFACEUP;
         if (m_Action == ENACT_AVOIDFACEDOWNHARD) m_Action = ENACT_AVOIDFACEDOWN;
         if (m_Action == ENACT_AVOIDFACEUP || m_Action == ENACT_AVOIDFACEDOWN)
         {
  
            switch (m_Action)
            {
               case ENACT_AVOIDFACEUP:
                  m_Action = ENACT_PULLUP;
                  break;
               case ENACT_AVOIDFACEDOWN:
                  m_Action = ENACT_PULLDOWN;
                  break;
               default:
                  hr2message(__LINE__,-1,"unknown Enemyaction in ENEMY::TreatAsLeader()\n");
                  return false;
            }
 
            // -- feste einheiten zum hoch/runterziehen veranschlagen
            //
            m_iActionTimer = 10; 
         }
      }
	  
      // --- sichergehen dass der grund fuer deaktivierung eine potenzielle Kollision war
      //
      if (m_Action == ENACT_SEARCHWAYOUT) 
      {
         if (l_bFSIntersectValid)
         {

            if (float(m_iActionTimer) < l_fFSIntersectAt)
            {
               m_Action    = ENACT_ESCAPE;
               //setCheckFSCollision(false); 
            }
         }else{
            m_Action       = ENACT_ESCAPE;
            //setCheckFSCollision(false); 
         }

      }else{
         // -- abdrehen weil enemy sonst mit player kollidiert
         //
         if ((g_ehdlBuf.m_vDest.y < 200.0f) && (g_ehdlBuf.m_vDest.y > 0.0f) &&
             (g_ehdlBuf.m_vDest.z < 10.0f) && (g_ehdlBuf.m_vDest.z > (-10.0f)) &&
             (g_ehdlBuf.m_vDest.x < 10.0f) && (g_ehdlBuf.m_vDest.x > (-10.0f)) )
         {
            m_Action = ENACT_AVOIDPLAYER;
      
            // -- rote, fuehrungslose jaeger fliehen laenger
            //
            if ((mType == ETYP_FLYER) && (m_pLeader == NULL) && (m_bGreenGroup==false))
            {
               m_iActionTimer = 600.0f / g_fBalanceFactor;
            }else{
               m_iActionTimer = 300.0f / g_fBalanceFactor;
            }
         }else{
            // -- falls Player ausgewichen, kann ohne anfassen des timers
            //    wieder auf flucht weitergeschaltet werden
            //
            if (m_Action == ENACT_AVOIDPLAYER)
            {
               m_Action = ENACT_ESCAPE;
            }
         }
         
         if (m_iActionTimer>0)
         { 
            // --- laufende phase abwarten
            //
            m_iActionTimer = m_iActionTimer - 1;
      
         }else{
      
            PSXVECTOR l_vDestWind2D;
      
            // -- Warp-Verfolgungsgeschwindigkeit notwendig: 
            //    wird bei Tunneljaegern schon bei geringer Entfernung verwendet 
            //    ( Tunnel so kurz)
            //    Normale Staffeln machen dies bei groesserer Entfernung
            //   
      
            // -- Zielfenster realistisch setzen: Bei hoher Geschw.
            //    reicht eine ungefaehre Richtung
            //
            if (g_ehdlBuf.m_vDest.y > 1000.0f || g_ehdlBuf.m_vDest.y < -1000.0f)
            {
               l_vDestWind2D.x = 50.0f;
               l_vDestWind2D.z = 50.0f;
            }else{
               l_vDestWind2D.x = 4.0f;
               l_vDestWind2D.z = 4.0f;
            }

            // -- pullup/pulldown phase grad zuende 
            //
            if (m_Action == ENACT_PULLUP || 
                m_Action == ENACT_PULLDOWN ||
                m_Action == ENACT_AVOIDFACEUP || 
                m_Action == ENACT_AVOIDFACEDOWN)
            {  
               // -- player sitzt nicht dem flyer direkt im nacken
               //    sonst pull phase lassen zwecks abstandgewinnung gegenueber face
               //
               m_Action       = ENACT_SEARCHWAYOUT;
               m_iActionTimer = 300;
            }else{

               // --- Vorraussetzungen fuer die naechste Phasen pruefen
               //
               m_iActionTimer=10;
               m_Action = ENACT_ALIGNYSMOOTH;  
               {
                  if ((g_ehdlBuf.m_vDest.x < l_vDestWind2D.x) && (g_ehdlBuf.m_vDest.x > -l_vDestWind2D.x))
                  { 
                     //m_fSpeed = l_fCatchSpeed;
         
                     m_Action = ENACT_ALIGNX;
         
                     if ((g_ehdlBuf.m_vDest.z <  l_vDestWind2D.z) && 
                         (g_ehdlBuf.m_vDest.z > -l_vDestWind2D.z) && 
                         (g_ehdlBuf.m_vDest.y > 0))
                     { 
                        if (g_ehdlBuf.m_vDest.y < 1000.0f)
                        {
                           if (g_ehdlBuf.m_vDest.y < 500.0f)
                           {
                              m_Action = ENACT_SHOOT;
                              m_iActionTimer = 20; //kein Faktor
                           }else{
                              m_Action = ENACT_CHASEPLAYER;
                              m_iActionTimer = 120; //kein Faktor
                           }
                        }else{
                           m_Action = ENACT_ACCELERATE;
                           m_iActionTimer = 20.0f / g_fBalanceFactor;
                        }
                     }
                  }
               }
            }
         }
      }
	  
      if (setMinSpeed() == false) return false;
      if (ExecuteAction()== false) 
      {
         return false;
      }
   } else{     //nicht getriggert
     
      m_Action = ENACT_CURVEAROUND;
   }
   
   // -- Starbase umrunden
   //   
   if (m_Action == ENACT_CURVEAROUND)
   {
      float l_fDestLength2D;
      double l_dDegree;

      if (setMinSpeed() == false) return false;

      // -- StarBase als Bezugspunkt
      //
      m_vCurrNode.x = g_levelContent.m_vPos.x - m_vPos.x,
      m_vCurrNode.y = g_levelContent.m_vPos.y - m_vPos.y,
      m_vCurrNode.z = g_levelContent.m_vPos.z - m_vPos.z;

      TransformDest();
      m_fDestLength = pow2(m_vCurrNode.x)+ pow2(m_vCurrNode.y)+ pow2(m_vCurrNode.z);
  
      // --- Vorraussetzungen fuer die naechste Phasen pruefen
      //
      if ((g_ehdlBuf.m_vDest.x < 2.0f) && (g_ehdlBuf.m_vDest.x > (-2.0f)))
      { 
         l_fDestLength2D = sqrt(g_ehdlBuf.m_vDest.y*g_ehdlBuf.m_vDest.y + g_ehdlBuf.m_vDest.z*g_ehdlBuf.m_vDest.z);
         l_dDegree = asin(g_ehdlBuf.m_vDest.z/l_fDestLength2D);
   
         if (g_ehdlBuf.m_vDest.y > 0.0f && m_fDestLength < pow2(g_levelContent.m_pBigRes->m_fRad *1.5f))
         {
            m_fViewRotation2 = (- g_fPi * 0.5f + l_dDegree) * 0.1f;
         }else{
            if (l_dDegree > 0)
            {
               m_fViewRotation2 = 0.01f;
            }else{
               m_fViewRotation2 = -0.01f;
            }
         }
      }else{

         // -- Fluegel richten (ex ENACT_ALIGNYHARD)
         //
         l_fDestLength2D = sqrt(g_ehdlBuf.m_vDest.x*g_ehdlBuf.m_vDest.x + g_ehdlBuf.m_vDest.z*g_ehdlBuf.m_vDest.z);

         l_dDegree = asin(g_ehdlBuf.m_vDest.x/l_fDestLength2D);
         if (g_ehdlBuf.m_vDest.z < 0.0f) { l_dDegree = - l_dDegree; }
         m_fViewRotation1 = l_dDegree;
      }
   }
   */

   // -- Aktion in der Historie speichern
   //
   /*
   if (mType == ETYP_LEADER)
   {
      if (m_idxLeadhist >= 0)
      {
         int k = leadhist[m_idxLeadhist].LastWrittenEntry + 1;
         if (k >= ENTRIESPERLEADHIST) k = 0;

         leadhist[m_idxLeadhist].EnemyAction[k]   = m_Action;
         leadhist[m_idxLeadhist].ViewRotation1[k] = m_fViewRotation1;
         leadhist[m_idxLeadhist].ViewRotation2[k] = m_fViewRotation2;
         leadhist[m_idxLeadhist].fSpeed[k]        = m_fSpeed;
         leadhist[m_idxLeadhist].LastWrittenEntry = k;

      }
   }

   RotationToMatrix(); m_fViewRotation1 = 0.0f; m_fViewRotation2 = 0.0f;
   SetFlyerStep();
   */
   return true;
}

bool ENEMY::TreatAsFlyer() 
{
   bool l_bRetVal;
   float l_fLeadSpeed;

   l_bRetVal = true;

   if (g_penemyDebug == NULL)
	   g_penemyDebug = this;

   if (m_pLeader)
   {
      setTriggered(m_pLeader->isTriggered());
      l_fLeadSpeed = m_pLeader->m_fSpeed;
   }else{
      setTriggered(true);
      l_fLeadSpeed = 10.0f;
   }

   switch(m_Action)
   {
      case ENACT_DONOTHING:
         if (m_pLeader)
         {
            // -- leaderlose schiff bleiben passiv (kurven rum) wenn spieler zu weit weg
            //
            if (isTriggered() == true)
            {
               m_Action = ENACT_CHASEPLAYER;
            }else{
               m_Action = ENACT_FOLLOWLEADER;
            }
         }else{
            m_Action = ENACT_CHASEPLAYER;
         }
         break;
		 /*
      case ENACT_CURVEAROUND:
         if (handleCurveAroundCircles() == false) return false;
         break;
		 */
      case ENACT_FOLLOWLEADER:

         if (handleFollowLeader(0) == false) return false;
         break;
      default:
         // --- Verhalten eines wendigen Jägers
         //
         if (handleDefaultSmallShip(l_fLeadSpeed,g_laserPoliceFlyerArray,g_cntLaserPoliceFlyer) == false) return false;
   
   }

   return true;
}

// --- TreatAsTunnelFlyer()
//     Behandlung eines Tunneljaegers
//
/*
bool ENEMY::TreatAsTunnelFlyer()
{
   float  l_fFSIntersectAt;
   bool l_bFSIntersectValid;

   PSXVECTOR l_vDestMovement;
   PSXFACEINFO* l_pfaceinfFSIntersect;

   // -- Kriterium ob angegriffen werden soll: Spieler muss sich im 
   //    Tunnel und vor dem Jaeger befinden
   //    Ausserdem darf das Schott nicht mehr existieren
   //    welches am Eingang zum Tunnel steht
   //
   if (g_Player.m_bEntered == true && m_pLeader == NULL)
   { 
      // -- transfering player world coord in coordinates 
      //    relative to enemy 
      //
      TransformDest();

      if (g_ehdlBuf.m_vDest.y > 0 && isTriggered() == false)
      {
         setTriggered(true);
      }
   }else{
      if (isTriggered())
      {
         detonate();
      }

      setTriggered(false);
   }
      
   // -- aktion entscheiden fuer den fall das Leader Triggerd ist
   //
   if(isTriggered()) 
   {
      l_bFSIntersectValid = false;
   
      // -- Pruefen ob Aufprall mit Flagschiffhuelle bevorsteht, wenn Kurs auf Player besteht
      //    

      // -- Pruefen ob Aufprall mit Flagschiffhuelle bevorsteht
      //    
      if (isCheckFSCollision()) // aufmerksamkeitsflag
      {
         // -- kollisionen mit Flagschiff-Faces selektieren
         //
         SetFlyerStep(); // da der Step in der Intersectionberechnung benutzt wird

         if (CheckFlagshipCollision( &l_bFSIntersectValid, 
                                     &l_fFSIntersectAt,
                                     &l_pfaceinfFSIntersect ) == false)
         {
            return false;
         }

         // -- bei kollision nachsehen ob ganz aufgeschlagen
         //    sonst abmelden aus der weiteren kollisionspruefung
         //
         if (l_bFSIntersectValid)
         {
            if (l_fFSIntersectAt >= 0.0f && l_fFSIntersectAt < m_fSpeed )
            {
               memset(&m_vStep, 0, sizeof(m_vStep));
               detonate();
               return true;
            }
         }else{ 
            setCheckFSCollision(false); 
         }
      }

      // -- transfering player world coord in coordinates 
      //    relative to enemy 
      //
      if (l_bFSIntersectValid && l_fFSIntersectAt/m_fSpeed < 20.0f && l_fFSIntersectAt >= 0.0f)
      {
         float x,y,z;
         PSXVECTOR l_vDestWind2D;
         
         m_vCurrNode.x = l_pfaceinfFSIntersect->vNormal.x;
         m_vCurrNode.y = l_pfaceinfFSIntersect->vNormal.y;
         m_vCurrNode.z = l_pfaceinfFSIntersect->vNormal.z;
         TransformDest();
         l_vDestWind2D.x = 0.3f;
         l_vDestWind2D.z = 0.3f;

         // --- Vorraussetzungen fuer die naechste Phasen pruefen
         //
         //m_iActionTimer=0;

         m_Action = ENACT_ALIGNYSMOOTH;  
         {
            if ((g_ehdlBuf.m_vDest.x < l_vDestWind2D.x) && (g_ehdlBuf.m_vDest.x > -l_vDestWind2D.x))
            { 

                  if (g_ehdlBuf.m_vDest.z < 0.0f)
                  {
                     m_Action = ENACT_AVOIDFACEDOWN;
                  }else{
                     m_Action = ENACT_AVOIDFACEUP;
                  }
               //}
            }
         }

      }else{

         // -- abdrehen weil enemy sonst mit player kollidiert
         //
         if ((g_ehdlBuf.m_vDest.y < 100.0f) && (g_ehdlBuf.m_vDest.y > 0.0f) &&
             (g_ehdlBuf.m_vDest.z < 10.0f) && (g_ehdlBuf.m_vDest.z > (-10.0f)) &&
             (g_ehdlBuf.m_vDest.x < 10.0f) && (g_ehdlBuf.m_vDest.x > (-10.0f)) )
         {
            m_Action = ENACT_AVOIDPLAYER;
            m_iActionTimer = 100.0f / g_fBalanceFactor;
         }else{
            if (m_Action == ENACT_AVOIDPLAYER)
            {
               m_Action = ENACT_ESCAPE;
            }
         }

         // -- hier im elsezweig anzukommen kann bedeuten:
         //    kollision mit fs-huelle gaenzlich abgewendet 
         //    in dem falle gleich weiterschalten zum abstand gewinnen
         //
         if (m_Action == ENACT_AVOIDFACEUPHARD)   m_Action = ENACT_AVOIDFACEUP;
         if (m_Action == ENACT_AVOIDFACEDOWNHARD) m_Action = ENACT_AVOIDFACEDOWN;
         if (m_Action == ENACT_AVOIDFACEUP || m_Action == ENACT_AVOIDFACEDOWN)
         {
            setCheckFSCollision(false); // pruefung erstmal nicht mehr notwendig

            switch (m_Action)
            {
               case ENACT_AVOIDFACEUP:
                  m_Action = ENACT_PULLUP;
                  break;
               case ENACT_AVOIDFACEDOWN:
                  m_Action = ENACT_PULLDOWN;
                  break;
               default:
                  hr2message(__LINE__,-1,"unknown Enemyaction in ENEMY::TreatAsTunnelFlyer()\n");
                  return false;
            }

            // -- feste einheiten zum hoch/runterziehen veranschlagen
            //
            m_iActionTimer = 5; 
         }

         if (m_iActionTimer>0)
         { 
            // --- laufende phase abwarten
            //
            m_iActionTimer = m_iActionTimer - 1;
      
         }else{
            PSXVECTOR l_vDestWind2D;

      
            // -- pullup/pulldown phase grad zuende 
            //
            if (m_Action == ENACT_PULLUP || m_Action == ENACT_PULLDOWN ||
                m_Action == ENACT_AVOIDFACEUP || m_Action == ENACT_AVOIDFACEDOWN)
            {  
               // -- player sitzt nicht dem flyer direkt im nacken
               //    sonst pull phase lassen zwecks kehrtwende
               //

               if (g_ehdlBuf.m_vDest.y < 0)
               {
                  if (g_ehdlBuf.m_vDest.y > -500.0f)
                  {
                     m_Action = ENACT_ACCELERATE;
                     m_iActionTimer = 100.0f / g_fBalanceFactor;
                  }else{ 
                     m_iActionTimer = 50;   // pulldown/pullup 
                  }
               }else{
                  m_Action = ENACT_DONOTHING;
                  m_iActionTimer = 20; // zeit um in die mitte der röhre zu kommen
               }
            }else{


               // -- Grobere Zielsetzung bei weiter Distanz
               //
               if (g_ehdlBuf.m_vDest.y > 500.0f)
               {
                  l_vDestWind2D.x = 50.0f;
                  l_vDestWind2D.z = 50.0f;
               }else{
                  l_vDestWind2D.x = 4.0f;
                  l_vDestWind2D.z = 4.0f;
               }
   
               // --- Vorraussetzungen fuer die naechste Phasen pruefen
               //
               m_Action = ENACT_ALIGNYSMOOTH;  
               m_iActionTimer = 10;
               {
                  if ((g_ehdlBuf.m_vDest.x < l_vDestWind2D.x) && (g_ehdlBuf.m_vDest.x > -l_vDestWind2D.x))
                  { 
                     m_Action = ENACT_ALIGNX;
         
                     if ((g_ehdlBuf.m_vDest.z <  l_vDestWind2D.z) && 
                         (g_ehdlBuf.m_vDest.z > -l_vDestWind2D.z) && 
                         (g_ehdlBuf.m_vDest.y > 0))
                     { 
                        if (g_ehdlBuf.m_vDest.y < 500.0f)
                        {
                           m_Action = ENACT_SHOOT;
                           m_iActionTimer = 20.0f / g_fBalanceFactor;
                        }else{
                           m_Action = ENACT_ACCELERATE;
                           m_iActionTimer = 20;
                        }
                     }
                  }
               }
            }
         }
      } // end-if kollision

      if (setMinSpeed() == false) return false;
      if (ExecuteAction()== false) 
      {
         return false;
      }
      RotationToMatrix(); m_fViewRotation1 = 0.0f; m_fViewRotation2 = 0.0f;
      SetFlyerStep();
     
   }// end-if triggered
   return true;
}
*/
// -- Autopilot, falls g_penemyInput grad nicht auf Player zeigt
//
bool ENEMY::TreatAsPlayer()
{
   switch (m_Action)
   {
      case ENACT_LEAVEDOCK:
         if(m_iActionTimer>0)
         {
            m_iActionTimer = m_iActionTimer - int(g_fElapsedTime);
         }else{
            m_Action = ENACT_DONOTHING;
         }
         break; 

      case ENACT_DOCKING:
		  /*
         if (g_Player.getGS()->setIndex(GAMESCREEN_MARKET) == false) return false;
		 */
         m_Action = ENACT_DONOTHING;
         break;

      case ENACT_REFLECTION:

         // -- abwarten, dann wieder user kontrolle übergeben
         //
         if(m_iActionTimer>0)
         {
            m_iActionTimer = m_iActionTimer - int(g_fElapsedTime);
         }else{
            m_Action = ENACT_DONOTHING;
         }
         break; 


   } // end-case
   /*
   if (m_Action != ENACT_REFLECTION)
   {
      dReal l_dNewTorque[3];
      l_dNewTorque[0]= m_fViewRotation2;
      l_dNewTorque[1]= m_fViewRotation1;
      l_dNewTorque[2]= 0.0f; //l_fViewRotation3;
      if (setTorque(l_dNewTorque) == false) return false;

      PSXVECTOR l_vForce;
      l_vForce.x = (0.5f * m_Mat._31 * (g_fTmpSpeedUpFactor + m_fSpeed) - m_vStep.x) * m_dMass.mass;
      l_vForce.y = (0.5f * m_Mat._32 * (g_fTmpSpeedUpFactor + m_fSpeed) - m_vStep.y) * m_dMass.mass;
      l_vForce.z = (0.5f * m_Mat._33 * (g_fTmpSpeedUpFactor + m_fSpeed) - m_vStep.z) * m_dMass.mass;
      dBodyAddForce(m_dBodyID, l_vForce.x, l_vForce.y, l_vForce.z);
   }
   */
   return true;
}

/*
bool ENEMY::TreatAsWingman()
{
   switch (m_Action)
   {
      case ENACT_REFLECTION:

         // -- abwarten, dann wieder user kontrolle übergeben
         //
         if(m_iActionTimer>0)
         {
            m_iActionTimer = m_iActionTimer - int(g_fElapsedTime);
         }else{
            if (g_penemyInput == this)
            {
               m_Action = ENACT_DONOTHING;
            }else{
               m_Action = ENACT_DOCKING;
            }
         }
         break; 

      case ENACT_DOCKING:
         if (handleFollowLeader(2) == false) return false;
         break;
      case ENACT_BECOMELEADERSPART:
         if (getBackIntoLeadersBody() == false) return false;
 
         if (m_pBigResItem)
         { 
            mType = ETYP_BIGRESITEM;
            if (setOffset(m_pBigResItem->m_vPos.x, 
                          m_pBigResItem->m_vPos.y, 
                          m_pBigResItem->m_vPos.z)==false) return false;
         }
         break;

   } // end-case

   return true;
}
*/
// -- statt TreatAsXXX, wenn der Enemy direkt durch den Benutzer gesteuert werden soll
//

bool ENEMY::controlledByUser()
{
   float l_fTempRotation1, l_fTempRotation2, l_fTempRotation3;

   dReal l_dNewTorque[3];

   l_fTempRotation3 = 0.0f;
   l_fTempRotation1 = 0.0f;
   l_fTempRotation2 = 0.0f;

   if (m_dBodyID == NULL) return false;

   // -- Beschleunigung
   //
   /*
   if (g_Player.getGS()->getIndex() == GAMESCREEN_WITCHSPACE && m_fSpeed <3000.0f )
   {
      if (m_fSpeed > 1.0f)
      {
         m_fSpeed = m_fSpeed * 1.2f;
      }else{
         m_fSpeed = 5.0f;
      }
   }
   */

   // --- Steuerung der Geschwindigkeit
   // 
   //if (g_Player.getGS()->IsCockpitInput())

   float l_fMaxSpeed, l_fMinSpeed;

   l_fMaxSpeed = 10.0f;
   l_fMinSpeed = -5.0f;

   if (g_arr_cKeystate[DIK_UP])
   {
       m_fSpeed = m_fSpeed + 0.00002f;/* * g_fElapsedTime;*/
       if (m_fSpeed > l_fMaxSpeed)
       {
           m_fSpeed = l_fMaxSpeed;
       }
       /*
           if (g_Player.getGS()->isHavingTextOverlay())
           {
           if (g_Player.getGS()->applyUpdateText(g_Player.getGS()->getIndex())==false)
           {
               return false;
           }
           }
       */
   }

   if (g_arr_cKeystate[DIK_DOWN])
   {
       m_fSpeed = m_fSpeed - 0.00002f;/* * g_fElapsedTime;*/
       if (m_fSpeed < l_fMinSpeed)
       {
           m_fSpeed = l_fMinSpeed;
       }
       /*
           if (g_Player.getGS()->isHavingTextOverlay())
           {
           if (g_Player.getGS()->applyUpdateText(g_Player.getGS()->getIndex())==false)
           {
               return false;
           }
           }
       */
   }

   if (g_arr_cKeystate[DIK_END])
   {
       m_fSpeed = m_fSpeed * 0.7f;
       l_fTempRotation1 = l_fTempRotation1 * 0.7f;
       /*
           if (g_Player.getGS()->isHavingTextOverlay())
           {
           if (g_Player.getGS()->applyUpdateText(g_Player.getGS()->getIndex())==false)
           {
               return false;
           }
           }
       */
   }
   /*
   if (g_options.IsEnableJoystick() && diJoystickState2.lZ && g_options.IsEnableJoystickZAxis())
   {
       m_fSpeed = 10.0f - float(diJoystickState2.lZ)/float(joystickrange_z) * 10.0f;

       if (g_Player.getGS()->isHavingTextOverlay())
       {
       if (g_Player.getGS()->applyUpdateText(g_Player.getGS()->getIndex())==false)
       {
           return false;
       }
       }
   }
   */

   
   /*
   switch (m_Action)
   {
      case ENACT_LEAVEDOCK:
         if(m_iActionTimer>0)
         {
            m_iActionTimer = m_iActionTimer - int(g_fElapsedTime);
         }else{
            m_Action = ENACT_DONOTHING;
         }
         break; 

      case ENACT_DOCKING:
         if (g_Player.getGS()->setIndex(GAMESCREEN_MARKET) == false) return false;
         m_Action = ENACT_DONOTHING;
         break;

      case ENACT_REFLECTION:

         // -- abwarten, dann wieder user kontrolle übergeben
         //
         if(m_iActionTimer>0)
         {
            m_iActionTimer = m_iActionTimer - int(g_fElapsedTime);
         }else{
            m_Action = ENACT_DONOTHING;

            // -- Aussenansicht wieder zurück ins Cockpit
            //
            if (g_Player.getGS()->getIndex()== GAMESCREEN_3RDPARTYVIEW)
            {
               if (g_Player.getGS()->setIndex(GAMESCREEN_COCKPIT) == false) return false;
            }
         }
         break; 


   } // end-case
   */
   


   // -- Behandlung der Maus als Schiffssteuerung oder GUI-Zeiger bewegung
   //
   //if (g_Player.getGS()->IsCockpitInput())

   // -- viewrotation ist die ableitung der rotation
   //    also der schritt um die sich die rotier geschwindigkeit erhoeht.
   //    dieser soll von der akt mausbewegung erhoeht werden 
   //    aber im anschluss sofort degressiv abnehmen
   //
   /*
   if(g_options.IsEnableJoystick())
   {
       m_fViewRotation1 = (float(diJoystickState2.lX)/float(joystickrange_x)
                       - joystick_abs_rel)/100.0f;

       if (g_options.IsSwapYControl())
       {
       m_fViewRotation2 = (float(diJoystickState2.lY)/float(joystickrange_y)
                           - joystick_abs_rel)/100.0f;
       }else{
       m_fViewRotation2 = (float(diJoystickState2.lY)/float(joystickrange_y)
                           - joystick_abs_rel)/100.0f;
       }
   }else{
   */

   // -- die Mausbewegung beinhaltet kummuliert alle noch nicht verarbeiteten Mausbewegungen
   //    Daher muss diese auf eine Zeiteinheit normalisiert werden
   //
   float l_fScale;
   //l_fScale = 0.00001f;// 0.001f;
   l_fScale = 0.0007f / g_fElapsedTime;


   l_fTempRotation1 = float(diMouseState.lX) * l_fScale;
   /*
   if (g_options.IsSwapYControl())
   {
       m_fViewRotation2 = - float(diMouseState.lY) * l_fScale;
   }else{
   */
   l_fTempRotation2 = +float(diMouseState.lY) * l_fScale;

   // -- alte Link/rechts Horizont-Rotation auf die Strafe Tasten
   //
   if (g_arr_cKeystate[DIK_LEFT])
   {
       l_fTempRotation3 = -0.1f;
   }

   if (g_arr_cKeystate[DIK_RIGHT])
   {
       l_fTempRotation3 = 0.1f;
   }

 
   // -- stabilisiert auch gleichzeitig, dass eine Kollision nicht zu ewiger Rotation führt
   //
   l_dNewTorque[0] = l_fTempRotation2;
   l_dNewTorque[1] = l_fTempRotation1;
   l_dNewTorque[2] = 0.0f; //l_fViewRotation3;
   if (setTorque(l_dNewTorque) == false) return false;
   
   
   PSXVECTOR l_vForce;
   
   XMFLOAT3 l_float3Direction;
   const dReal* l_pdStep;
   XMStoreFloat3(&l_float3Direction, m_Mat.r[2]);
   l_pdStep = dBodyGetLinearVel(m_dBodyID);
   

   l_vForce.x = (0.5f * l_float3Direction.x * m_fSpeed * m_dMass.mass - l_pdStep[0]);
   l_vForce.y = (0.5f * l_float3Direction.y * m_fSpeed * m_dMass.mass - l_pdStep[1]);
   l_vForce.z = (0.5f * l_float3Direction.z * m_fSpeed * m_dMass.mass - l_pdStep[2]);
   dBodyAddForce(m_dBodyID, l_vForce.x, l_vForce.y, l_vForce.z);
   /*
   const dReal* l_pdAngular;
   l_pdAngular = dBodyGetAngularVel(g_penemyCamera->getBodyID());
   if (l_pdAngular)
   {
		l_fTempRotation1 = l_fTempRotation1 * 20.0f;
		l_fTempRotation2 = l_fTempRotation2 * 2.0f;;

	   float l_fMaxWingRotation;

	   l_fMaxWingRotation = 0.025f;
	   if (l_fTempRotation1 > l_fMaxWingRotation) l_fTempRotation1 = l_fMaxWingRotation;
	   if (l_fTempRotation1 < -l_fMaxWingRotation) l_fTempRotation1 = -l_fMaxWingRotation;

	   l_fMaxWingRotation = 0.0025f;
	   if (l_fTempRotation2 < -l_fMaxWingRotation) l_fTempRotation2 = -l_fMaxWingRotation;
	   if (l_fTempRotation2 > l_fMaxWingRotation) l_fTempRotation2 = l_fMaxWingRotation;

	   m_fViewRotation1 = m_fViewRotation1 - l_fTempRotation1;
	   m_fViewRotation2 = m_fViewRotation2 - l_fTempRotation2;

	   m_fViewRotation1 = m_fViewRotation1 * 0.99;
	   m_fViewRotation2 = m_fViewRotation2 * 0.99;
   }

	// -- the displaymatrix of player's ship with wing rotation due to torque strength
	//
	FXMMATRIX l_matGeneric1 = XMMatrixRotationZ(m_fViewRotation1);
	CXMMATRIX l_matGeneric2 = XMMatrixRotationX(m_fViewRotation2);
	CXMMATRIX l_matWings = XMMatrixMultiply(l_matGeneric1, l_matGeneric2);
		
	//g_matPlayer = XMMatrixMultiply(l_matWings, m_Mat);
   */
   g_matPlayer = m_Mat;
   return true;
}

// --
//
XMMATRIX* ENEMY::getMatrixPointer()
{
	if (mType == ETYP_PLAYER)
	{
		
//		FXMMATRIX l_matGeneric1 = XMMatrixRotationZ(m_fViewRotation1 * 20);
//		CXMMATRIX l_matGeneric2 = XMMatrixRotationX(m_fViewRotation2 * 5);
//		CXMMATRIX l_matWings = XMMatrixMultiply(l_matGeneric1, l_matGeneric2);
		
//		g_matPlayer = XMMatrixMultiply(l_matWings, m_Mat);
		return &g_matPlayer;

	}else{
		return &m_Mat;
	}
}
// -- die Inertia-Tensor Matrix in der Steuerung-Torque berücksichtigen
//    jedoch nur die pure Masse des BigRes-Leaders
//    so dass bei angehängten bigresitems das ganze absichtlich schwerfällig wird
//

bool ENEMY::setTorque(dReal* l_dNewTorque)
{
   const dReal* l_pdAngular;
   dReal l_dStabTorque[3];
   const dReal* l_pdMatrix3;
   dMatrix3 l_odematTemp;
   dMatrix3 l_odematI;

   dMULTIPLY0_331 (l_dNewTorque,m_dMass.I, l_dNewTorque);

   dBodyAddRelTorque( m_dBodyID,
                       l_dNewTorque[0],
                       l_dNewTorque[1],
                       l_dNewTorque[2]);

   l_pdAngular = dBodyGetAngularVel(m_dBodyID);
   if(l_pdAngular)
   {
      if (_isnan(l_pdAngular[0]) ||
          _isnan(l_pdAngular[1]) ||
          _isnan(l_pdAngular[2]) )
      {
         dBodySetAngularVel( m_dBodyID,0.0f,0.0f,0.0f);
      }else{

         // -- die Inertia-Tensor Matrix in der Stabilisator Torque berücksichtigen
         //
         l_pdMatrix3 = dBodyGetRotation(m_dBodyID);
         dMULTIPLY2_333 (l_odematTemp,m_dMass.I,l_pdMatrix3);
         dMULTIPLY0_333 (l_odematI,l_pdMatrix3,l_odematTemp);

         l_dStabTorque[0] = - l_pdAngular[0]* .1f;
         l_dStabTorque[1] = - l_pdAngular[1]* .1f;
         l_dStabTorque[2] = - l_pdAngular[2]* .1f;

         dMULTIPLY0_331 (l_dNewTorque,l_odematI, l_dStabTorque);

         dBodyAddTorque( m_dBodyID,
                         l_dNewTorque[0],
                         l_dNewTorque[1],
                         l_dNewTorque[2]);
      }

   
   }else{
      hr2message(__LINE__,-1,"no angular velocity available");
      return false;
   }

   return true;
}

      
// -- die sich lediglich drehende Starbase
//
/*
bool ENEMY::TreatAsStarBase()
{

   //dBodySetAngularVel( m_dBodyID,0.0f,0.0001f,0.0f);

   const dReal* l_pdAngular;
   {

      l_pdAngular = dBodyGetAngularVel(m_dBodyID);
      if(l_pdAngular)
      {
         if (_isnan(l_pdAngular[0]) ||
             _isnan(l_pdAngular[1]) ||
             _isnan(l_pdAngular[2]) )
         {
            dBodySetAngularVel( m_dBodyID,0.0f,0.0f,0.0f);
         }else{
        
            if (g_Player.m_bEntered == false)
            {
               if (l_pdAngular[1] < 0.0001f) dBodyAddRelTorque( m_dBodyID, 0.0f, 50000.0f, 0.0f);

   
            }else{
               dBodySetAngularVel( m_dBodyID,0.0f,0.0f,0.0f);
   
            }
         }
      }
   }

   return true;
}
*/
// -- Kopfgeldjaeger
//
/*
bool ENEMY::TreatAsHunter()
{

   if (g_bEnemyDebugValid == false)
   {
      g_penemyDebug = this;
      g_bEnemyDebugValid = true;
   }

   if (g_penemyDebug == this)
   {
      PSXPrintf(__LINE__, debugausgabe,800, "%s %i %f", g_cActionNameArray[m_Action], m_iActionTimer, m_fDestLength);
   }

   if (isAnyCharacter())
   {
      if((getCharacter())->checkTrigger(this)== false) return false;
      if(isTriggered()==false)
      {
         if((getCharacter())->m_iAction == CHRACT_ATTACKPLAYER)
         {
            setTriggered(true);
         }
      }else{
         if((getCharacter())->m_iAction != CHRACT_ATTACKPLAYER)
         {
            setTriggered(false);
         }
      }
   }else{
      setTriggered(false);
   }
   switch(m_Action)
   {
      case ENACT_DONOTHING:

         // -- abwarten, dann angreifen
         //
         if(m_iActionTimer>0)
         {
            m_iActionTimer = m_iActionTimer - int(g_fElapsedTime);
         }else{
            if (m_fDestLength >= pow2(2000.0f) ||isTriggered())
            {
               m_Action = ENACT_CHASEPLAYER;
            }else{
               m_fSpeed = m_fSpeed * 0.80f;
               m_fViewRotation1 = 0.0f;
               m_fViewRotation2 = 0.0f;
            }
         }
         if (setMatrixAndStepSmallShip()==false) return false;
         break;

      default:
         if (handleDefaultSmallShip(10.0f ,
                                   g_laserHunterXArray,
                                   g_cntLaserHunterX) == false) return false;
         break;

   
   } // end-case m_Action
   
   return true;
}
*/
/*
bool ENEMY::TreatAsTrader()
{
   if (isAnyCharacter())
   {
      if((getCharacter())->checkTrigger(this)== false) return false;

      if(isTriggered()==false)
      {
         if((getCharacter())->m_iAction == CHRACT_ATTACKPLAYER)
         {
            setTriggered(true);
         }
      }else{
         if((getCharacter())->m_iAction != CHRACT_ATTACKPLAYER)
         {
            setTriggered(false);
         }
      }
   }else{
      setTriggered(false);
   }
  
   

   switch(m_Action)
   {
      case ENACT_DONOTHING:
         if (isTriggered())
         {
            m_Action = ENACT_CHASEPLAYER;
         }else{
            m_Action = ENACT_GOTOSTARBASE;
         }
         break;

      case ENACT_GOTOSTARBASE:
         if (handleGoToStarBase()==false) return false;
         break;

      case ENACT_DOCKING:
         m_fViewRotation1 = m_fViewRotation1 * 0.99f;
         m_fSpeed = m_fSpeed * 0.99f;
         if(m_iActionTimer>0)
         {
            m_iActionTimer = m_iActionTimer - int(g_fElapsedTime);
         }else{
            m_Action = ENACT_AVOIDSTARBASE;
         }
         break;

      case ENACT_AVOIDSTARBASE:
         if (handleAvoidStarBase()==false) return false;

         // -- irgendwann aber wieder zurück in Richtung Raumstation
         //
         if( g_levelContent.m_penemyStarBase &&
             g_ehdlBuf.m_fStarBaseLength > 
             g_levelContent.m_penemyStarBase->m_fCollisionRadPow2 * 100.0f)
         {
            m_Action = ENACT_GOTOSTARBASE;
         }

         break;

      default:
         if (m_ResType == RESTYPE_CARGO03 ||
             m_ResType == RESTYPE_CARGO08)
         {
            if (handleDefaultSmallShip(10.0f,
                                      g_laserPoliceLeaderArray, 
                                      g_cntLaserPoliceLeader) == false) return false;
         }else{
            if (handleDefaultBigCargo(5.0f)==false) return false;
         }
      
         break;
   }
   return true;
}
*/

// --- TreatAsPirate()
//     Behandlung eines Pirate
//
/*
bool ENEMY::TreatAsPirate()
{
   if (isAnyCharacter())
   {
      if((getCharacter())->checkTrigger(this)== false) return false;
   
      if(isTriggered()==false)
      {
         if((getCharacter())->m_iAction == CHRACT_ATTACKPLAYER)
         {
            setTriggered(true);
            m_cntCurrentlyAttackingPirates = m_cntCurrentlyAttackingPirates + 1;
         }
      }else{
         if((getCharacter())->m_iAction != CHRACT_ATTACKPLAYER)
         {
            setTriggered(false);
            m_cntCurrentlyAttackingPirates = m_cntCurrentlyAttackingPirates - 1;
         }
      }
   }else{
      setTriggered(false);
   }

   switch(m_Action)
   {
      case ENACT_DONOTHING:

         // -- hinfliegen, sobald er aus schutzzone raus ist
         //
         if (g_Player.m_bNearStarBase == false)
         {
            m_Action = ENACT_CHASEPLAYER;
         }else{
            m_fSpeed = m_fSpeed * 0.80f;
            m_fViewRotation1 = 0.0f;
            m_fViewRotation2 = 0.0f;
         }
         if (setMatrixAndStepBigCargo()==false) return false;
         break;

      default:
         // --- Verhalten eines schweren Schiffes
         //
         if (handleDefaultBigCargo(3.0f)==false) return false;
         
   }

   return true;

}
*/
// -- piratenflyer nach neuer Logik
//
/*
bool ENEMY::TreatAsPirateFlyer()
{
   // -- Triggerstatus in Abhängigkeit Leader/Character feststellen
   //
   if (m_pLeader)
   {
      setTriggered(m_pLeader->isTriggered());
   }else{

      // -- Handlungsstatus ausführen und pflegen
      //
      if (isAnyCharacter())
      {
         if((getCharacter())->m_iAction != CHRACT_ATTACKPLAYER && isTriggered())
         { 
            setTriggered(false);
         }
      }else{
         setTriggered(true);
      }
   }

   // -- Enemytyp spezifische Action behandlungen
   //
   switch(m_Action)
   {
      case ENACT_DONOTHING:
         // -- abwarten, dann angreifen
         //
         if(m_iActionTimer>0)
         {
            m_iActionTimer = m_iActionTimer - int(g_fElapsedTime);
         }else{
            if (m_pLeader)
            {
               if (isTriggered())
               {
                  if (m_fDestLength > pow2(1000.0f) )
                  {
                     m_Action = ENACT_CHASEPLAYER;
                  }else{
                     m_Action = ENACT_ESCAPE; //sonst abstand gewinnen
                     m_iActionTimer = 5000;
                  }
               }else{
                  m_Action = ENACT_FOLLOWLEADER;

               }
            }else{
               setTriggered(false);
               m_Action = ENACT_AVOIDSTARBASE; 
            }
         }
         break;

      case ENACT_FOLLOWLEADER:

         if (isTriggered())
         {
            m_Action = ENACT_DONOTHING; // erneute Entscheidung herbeiführen
            m_iActionTimer = 1000;
         }

         if (handleFollowLeader(1)==false) return false;
         break;

      default:
         if (handleDefaultSmallShip(10.0f,
                                   g_laserHunterXArray,
                                   g_cntLaserHunterX) == false) return false;
   }


   return true;
}
*/
// -- Action Umsetzen/Weiterschalten für weniger wendige Schiffe
//
/*
bool ENEMY::handleDefaultBigCargo(float l_fFightSpeed)
{
   float l_fTolerance;
   float l_fTooFarPow2;

   if (g_options.IsWriteLogFileInLoop())
   {
      WriteLogFile(g_cEnemyTypeNameArray[mType]);
      WriteLogFile("::handleDefaultBigCargo()\n");
   }

   TransformDest();
   //m_fDestLength = pow2(m_vCurrNode.x)+ pow2(m_vCurrNode.y)+ pow2(m_vCurrNode.z);

   // -- Zielfenster realistisch setzen: Bei weiter Distanz
   //    reicht eine ungefaehre Anpeilung
   //
   l_fTooFarPow2 = pow2(2000.0f);

   if (m_fDestLength > l_fTooFarPow2)
   {
      l_fTolerance = 50.0f;
   }else{
      l_fTolerance = 10.0f;
   }
   //l_fTolerance = g_bigresArray[BIGRES_COBRA].m_fRad;
   
   {
      float l_fDestLength2D;
      double l_dDegree = 0;
   
      // -- Handlungsstatus ausführen und pflegen
      //
      switch(m_Action)
      {
         case ENACT_DETONATE:
            if (detonate()==false) return false;
            break;

         case ENACT_CHASEPLAYER: // dieser Action fasst ALIGNY/ALIGNX, samt pruefung zusammen

            if (isTriggered() == false)
            {
               m_Action = ENACT_DONOTHING;
            }

            // -- allmähliche Beschleunigung des Frachters
            //
            if (m_fSpeed < l_fFightSpeed)
            {
               if (m_fSpeed)
               {
                  m_fSpeed = m_fSpeed * 1.111f;
             
               }else{
                  m_fSpeed = 0.1f;
               }
            }else{

               if (m_fSpeed > l_fFightSpeed)
               {
                  m_fSpeed = m_fSpeed * 0.80f;
               }
            }

            // -- wenn enemy mit player zu kollidieren droht => goto AVOIDPLAYER
            //
            if (g_ehdlBuf.m_vDest.z > 0.0f &&
                g_ehdlBuf.m_vDest.z < g_bigresArray[BIGRES_COBRA].m_fRad * m_fSpeed * 0.5f &&
                g_ehdlBuf.m_vDest.x < g_bigresArray[BIGRES_COBRA].m_fRad * 4.0f && 
                g_ehdlBuf.m_vDest.x > (-g_bigresArray[BIGRES_COBRA].m_fRad) * 4.0f &&
                g_ehdlBuf.m_vDest.y < g_bigresArray[BIGRES_COBRA].m_fRad * 4.0f && 
                g_ehdlBuf.m_vDest.y > (-g_bigresArray[BIGRES_COBRA].m_fRad* 4.0f ) )
            {
               m_Action = ENACT_AVOIDPLAYER;
            }else{


               if (m_fSpeed >= l_fFightSpeed)
               {
                  // -- sonst erstmal Fluegel/Horizont drehen, damit der Spieler auf 
                  //    einer Seite liegt
                  //
                  if ((g_ehdlBuf.m_vDest.y < l_fTolerance) && (g_ehdlBuf.m_vDest.y > -l_fTolerance))
                  { 
                     // -- nach links oder rechts steuern
                     //
                     l_fDestLength2D = sqrt(pow2(g_ehdlBuf.m_vDest.x) + pow2(g_ehdlBuf.m_vDest.z));
                     l_dDegree = asin(g_ehdlBuf.m_vDest.x/l_fDestLength2D);
                     m_fViewRotation2 = l_dDegree;//* g_fElapsedTime;

                     if ((g_ehdlBuf.m_vDest.x < l_fTolerance) && (g_ehdlBuf.m_vDest.x > -l_fTolerance))
                     { 
                        if (m_fDestLength > l_fTooFarPow2)
                        {
                           m_Action = ENACT_ACCELERATE;
                           m_iActionTimer = 1000;
                        }
                     }


                  }else{
                     // -- Die Fluegelausrichtung/Horizon neigen bis Dest auf Vertikalachse
                     //
                     l_fDestLength2D = sqrt(pow2(g_ehdlBuf.m_vDest.x) + pow2(g_ehdlBuf.m_vDest.y));
         
                     l_dDegree = asin(g_ehdlBuf.m_vDest.y/l_fDestLength2D);
                     if (g_ehdlBuf.m_vDest.y > 0.0f) { l_dDegree = - l_dDegree; }
                     m_fViewRotation1 = - l_dDegree ;//* g_fElapsedTime;

                  }
      
               }
            }

            break;
         case ENACT_AVOIDPLAYER:
   
            if (g_ehdlBuf.m_vDest.z < 0.0f)
            {
               // -- falls Player hinter dem Enemy liegt, kann 
               //    wieder auf flucht weitergeschaltet werden
               //
               m_Action = ENACT_ESCAPE;
               m_iActionTimer = 1000;
            }else{

               l_fDestLength2D = sqrt(pow2(g_ehdlBuf.m_vDest.z) + pow2(g_ehdlBuf.m_vDest.x));
               l_dDegree = asin(g_ehdlBuf.m_vDest.x/l_fDestLength2D);
      
               m_fViewRotation2 = (g_fPi * 0.5f - l_dDegree);//* g_fElapsedTime ; 

            }
            break;

         case ENACT_ESCAPE:

            if(m_iActionTimer>0)
            {
               m_iActionTimer = m_iActionTimer - int(g_fElapsedTime);
            }else{
               if (m_fDestLength > pow2(1000.0f) && m_fShieldAmount >0.1f) 
               {
                  m_Action = ENACT_DONOTHING; // entscheidung forcieren
               }else{
                  m_fViewRotation1 = m_fViewRotation1 * 0.99f; 
               }
            }

            break;

         case ENACT_ACCELERATE:
            if (g_ehdlBuf.m_vDest.z > 0.0f && m_fDestLength > l_fTooFarPow2)
            {
               // -- Verfolgungsgeschwindigkeit 
               //    Geschwindigkeit des Players, mindestens aber 2
               //   
               m_fSpeed = m_fSpeed * 1.02f;
               if (m_fSpeed > 50.0f)
               {
                  m_fSpeed = 50.0f;
               }
               m_fViewRotation1 = 0;
               m_fViewRotation2 = 0;
            }else{
               m_Action = ENACT_CHASEPLAYER;
            }
            break;

        case ENACT_BANKROTATION: // soll hier direkt zu drop cargo führen
            if(m_iActionTimer>0)
            {
               m_iActionTimer = m_iActionTimer - int(g_fElapsedTime);
            }else{
               m_Action = ENACT_DETONATE;
            }
            break;
         case ENACT_DONOTHING:

            // -- abwarten, dann angreifen
            //
            if(m_iActionTimer>0)
            {
               m_iActionTimer = m_iActionTimer - int(g_fElapsedTime);
            }else{
               if (isTriggered())
               {
                  m_Action = ENACT_CHASEPLAYER;
               }
            }
            break;

         case ENACT_REFLECTION:

            // -- Abprall von StarBase
            //
            if(m_iActionTimer>0)
            {
               m_iActionTimer = m_iActionTimer - int(g_fElapsedTime);
            }else{
   
               // -- entscheidung forcieren, wenn ausser reichweite, sonst ausrotieren
               //
               m_Action = ENACT_DONOTHING; 
            }
            break;
  
      } // end-case m_Action
   
   }
   if (setMatrixAndStepBigCargo() == false) return false;
   return true;
}
*/
// -- Action Umsetzen/Weiterschalten für wendige Schiffe
//

bool ENEMY::handleDefaultSmallShip(float l_fFightSpeed, 
                                  PSXLASERSPAWN* l_laserspawnArray, int l_cntSpawn)
{
   float l_fTolerance;
   float l_fTooFarPow2, l_fMinShootDistPow2, l_fDot;

   if (g_bWriteLogFileInLoop)
   {
      WriteLogFile(g_cEnemyTypeNameArray[mType]);
      WriteLogFile("::handleDefaultSmallShip(");
	  WriteLogFile(g_cActionNameArray[m_Action]);
	  WriteLogFile(")\n");

   }
   
   TransformDest();
   g_ehdlBuf.m_fDestLength = pow2(g_ehdlBuf.m_vDest.x) + pow2(g_ehdlBuf.m_vDest.y)+ pow2(g_ehdlBuf.m_vDest.z);
   
   //if (g_penemyDebug == this)
   //{
	//   PSXPrintf(__LINE__, debugausgabe, g_maxDebugAusgabe, "%s %i - Dest: %f, %f, %f ",
	//	   g_cActionNameArray[m_Action], m_iActionTimer,
	//	   g_ehdlBuf.m_vDest.x, g_ehdlBuf.m_vDest.y, g_ehdlBuf.m_vDest.z);
	//	   
   //}
   
   // -- 3D-sound: Maschinengeräusche 
   //
   /*
   if (g_options.IsEnableSound())
   {
      if (m_fDestLength < pow2(m_fRad * 10.0f))
      {
         // -- prüfen ob schon einer auf dieser resource sitzt
         //
         if (g_sound_array[SNDRES_ENEMYENGINE].m_pEnemy)
         {
            // -- vielleicht ist dieser ja doch näher am Player als der "Geräusch-Besetzer"
            //
            if (m_fDestLength < g_sound_array[SNDRES_ENEMYENGINE].m_pEnemy->m_fDestLength)
            {
               // -- ist noch frei, einfach nehmen
               //
               if ( g_sound_array[SNDRES_ENEMYENGINE].playLooped() == false) return false;
               g_sound_array[SNDRES_ENEMYENGINE].m_pEnemy = this;
            }
         }else{
            // -- ist noch frei, einfach nehmen
            //
            if ( g_sound_array[SNDRES_ENEMYENGINE].playLooped() == false) return false;
            g_sound_array[SNDRES_ENEMYENGINE].m_pEnemy = this;
         }
      }else{
         if (g_sound_array[SNDRES_ENEMYENGINE].m_pEnemy==this)
         {
            if ( g_sound_array[SNDRES_ENEMYENGINE].stopLooping() == false) return false;
            g_sound_array[SNDRES_ENEMYENGINE].m_pEnemy = NULL;
         }
         
      }
   }
   */
   // -- Zielfenster realistisch setzen: Bei weiter Distanz
   //    reicht eine ungefaehre Anpeilung
   //
   l_fTooFarPow2 = pow2(1500.0f);
   l_fMinShootDistPow2 = pow2(1300.0f);

   if (g_ehdlBuf.m_fDestLength > l_fTooFarPow2)
   {
      l_fTolerance = 50.0f;
   }else{
      l_fTolerance = 10.0f;
   }
   //l_fTolerance = g_bigresArray[BIGRES_COBRA].m_fRad;

   //m_fViewRotation1 = 0.0f;
   //m_fViewRotation2 = 0.0f;

   // -- momentanen m_Action pflegen
   //
   {
	   float l_fDestLength2D;
      double l_dDegree = 0;

      switch(m_Action)
      {
         case ENACT_DETONATE:
            if (detonate()==false) return false;
            break;

         case ENACT_SHOOT:
            // -- Pruefen ob Spieler "vor Flinte"
            //
            if (m_iActionTimer>0)
            { 
               // --- laufende phase abwarten
               //
               m_iActionTimer = m_iActionTimer - int(g_fElapsedTime);
         
            }else{
         
               // -- sobald Spieler im Zielkorridor: schiessen
               //
               if ((g_ehdlBuf.m_vDest.x < g_bigresArray[BIGRES_COBRA].m_fRad * 2.0f) && 
                   (g_ehdlBuf.m_vDest.x > -g_bigresArray[BIGRES_COBRA].m_fRad * 2.0f) && 
                   (g_ehdlBuf.m_vDest.y < g_bigresArray[BIGRES_COBRA].m_fRad * 2.0f) && 
                   (g_ehdlBuf.m_vDest.y > -g_bigresArray[BIGRES_COBRA].m_fRad * 2.0f))
               { 
                  if (Shoot() == false) return false;

                  m_iActionTimer=1000; // naechsten schuss timen
               }else{
                  m_Action = ENACT_CHASEPLAYER;
               }
            } // end-if timer abgelaufen

            // *kein* break, damit er nach chaseplayer durchrutscht

         case ENACT_CHASEPLAYER: // dieser Action fasst ALIGNY/ALIGNX, samt pruefung zusammen

            m_fViewRotation1 = 0.0f;
            m_fViewRotation2 = 0.0f;

            // -- Dotproduct zwischen Player- und SmallFlyer-Richtungsvektor
            //
            {
                //float x,y,z;
				XMFLOAT4 p,e;
				XMStoreFloat4(&p, g_Player.m_pEnemy->getMatrixPointer()->r[2]);
				XMStoreFloat4(&e, m_Mat.r[2]);				                                
				l_fDot = e.x*p.x + e.y*p.y + e.z*p.z; 
                 
                //da diese Zeile aussieht, wie die letzte Berechnung von TransformDest()
                //kann l_fDot auch angesehen werden als in den enemyraum versetzten Playerstep.z
            }

			 //if (g_penemyDebug == this)
			 //{
			 // char l_cDebugText[100];
			 // size_t l_cntConvertedChars;
			 // PSXPrintf(__LINE__, l_cDebugText, sizeof(l_cDebugText), "l_fDot = %f", l_fDot);
			 // size_t l_cntDebugTextChars = strlen(l_cDebugText) + 1;
			 // mbstowcs_s(&l_cntConvertedChars, debugausgabe, l_cntDebugTextChars, l_cDebugText, _TRUNCATE);
			 //}

            // -- angsthase-spiel: wenn enemy mit player zu kollidieren droht => goto AVOIDPLAYER
            //
            if (g_ehdlBuf.m_vDest.z > 0.0f &&
                g_ehdlBuf.m_vDest.x < g_bigresArray[BIGRES_COBRA].m_fRad * 4.0f && 
                g_ehdlBuf.m_vDest.x > (-g_bigresArray[BIGRES_COBRA].m_fRad) * 4.0f &&
                g_ehdlBuf.m_vDest.y < g_bigresArray[BIGRES_COBRA].m_fRad * 4.0f && 
                g_ehdlBuf.m_vDest.y > (-g_bigresArray[BIGRES_COBRA].m_fRad* 4.0f ) )
            {
                if (g_ehdlBuf.m_fDestLength < l_fMinShootDistPow2 && 
                    m_Action != ENACT_SHOOT)
                {
                   if (isTriggered())
                   {
                      m_Action = ENACT_SHOOT;
                   }else{
                      m_Action = ENACT_DONOTHING;
                   }                   
                }

                // -- Spieler direkt vor Enemy
                //
                if (g_ehdlBuf.m_vDest.z < g_bigresArray[BIGRES_COBRA].m_fRad * m_fSpeed &&
                    g_ehdlBuf.m_vDest.x < g_bigresArray[BIGRES_COBRA].m_fRad && 
                    g_ehdlBuf.m_vDest.x > (-g_bigresArray[BIGRES_COBRA].m_fRad) &&
                    g_ehdlBuf.m_vDest.y < g_bigresArray[BIGRES_COBRA].m_fRad && 
                    g_ehdlBuf.m_vDest.y > (-g_bigresArray[BIGRES_COBRA].m_fRad ) )

                {
                   // --- direkt vor einem mit selber flugrichtung
                   //     diese Situation lieber ausnutzen statt abzudrehen
                   //
                   if ( l_fDot > 0.9f)  
                   { 
                      l_fFightSpeed = g_Player.m_pEnemy->m_fSpeed;
					  
                   }else{

                      // -- Abdrehen wenn Player direkt auf enemy draufhält
                      //    oder er sich sehr kurz vor dem enemy befindet
                      //
                      if (g_ehdlBuf.m_vDest.z < g_Player.m_pEnemy->m_fRad * m_fSpeed * 0.3f )
                      {
                         m_Action = ENACT_AVOIDPLAYER; //abdrehen
                         m_iActionTimer = 1000;
                      }
					  
                   }
                }
            }

            // -- schnelle Beschleunigung 
            //
            if (m_fSpeed < l_fFightSpeed)
            {
               if (m_fSpeed)
               {
                  m_fSpeed = m_fSpeed * 1.1111f;
               }else{
                  m_fSpeed = 0.1f;
               }
            }else{

               if (m_fSpeed > l_fFightSpeed)
               {
                  m_fSpeed = m_fSpeed * 0.80f;
               }
            }

            if (m_Action != ENACT_AVOIDPLAYER) // wenn sich der verdacht nicht erhärtet hat
            {

               // -- Die Fluegelausrichtung/Horizon neigen bis Dest auf Vertikalachse
               //
               l_fDestLength2D = sqrt(pow2(g_ehdlBuf.m_vDest.x) + pow2(g_ehdlBuf.m_vDest.y));
   
               l_dDegree = asin(g_ehdlBuf.m_vDest.x/l_fDestLength2D);
               //if (g_ehdlBuf.m_vDest.y > 0.0f) { l_dDegree = - l_dDegree; }

               m_fViewRotation1 = l_dDegree;//* g_fElapsedTime;

               // -- Schnauze hoch/runterziehen bis Spieler in Mitte 
               //    sonst erstmal Fluegel/Horizont drehen, damit der Spieler auf 
               //    Vertikale liegt
               //
               if ((g_ehdlBuf.m_vDest.x < l_fTolerance) && (g_ehdlBuf.m_vDest.x > -l_fTolerance))
               { 
                  // -- Wenn Player im Zielfenster, aber zu weit weg, dann 
                  //    goto ACCELERATE
                  //
                  if (g_ehdlBuf.m_vDest.z > 0.0f && 
                      g_ehdlBuf.m_vDest.y < l_fTolerance && g_ehdlBuf.m_vDest.y > -l_fTolerance)
                  {
                     if (g_ehdlBuf.m_fDestLength > l_fTooFarPow2)
                     {
                        m_Action = ENACT_ACCELERATE;
                        m_iActionTimer = 1000;
                     }
                  }else{
   
                     // -- sont wie ursprünglich beabsichtigt Nase hochziehen/runterziehen 
                     //
                     l_fDestLength2D = sqrt(pow2(g_ehdlBuf.m_vDest.z) + pow2(g_ehdlBuf.m_vDest.y));
                     l_dDegree = asin(g_ehdlBuf.m_vDest.y/l_fDestLength2D);

                     m_fViewRotation2 = - l_dDegree;//* g_fElapsedTime;

                  }
               }
            }
   

			 

            break;
      
         case ENACT_AVOIDPLAYER:

            m_fViewRotation1 = 0.0f;
            m_fViewRotation2 = 0.0f;

            if(m_iActionTimer>0)
            {
               //PSXVECTOR l_vPlayerStep;

               m_iActionTimer = m_iActionTimer - int(g_fElapsedTime);

               m_fViewRotation1 = 0.0f;
               if (g_ehdlBuf.m_vDest.y > 0)
               {
                  m_fViewRotation2 = 0.1f;
               }else{
                  m_fViewRotation2 = -0.1f;
               }

            }else{

               // -- falls Player hinter dem Enemy liegt, kann 
               //    wieder auf flucht weitergeschaltet werden
               //
               m_Action = ENACT_ESCAPE;
               m_iActionTimer = 5000.0f;

               //m_fViewRotation1 = 0.00004f;
            }
            break;

         case ENACT_ESCAPE:

            // -- schnelle Beschleunigung 
            //

            if (m_fSpeed < l_fFightSpeed)
            {
               if (m_fSpeed)
               {
                  m_fSpeed = m_fSpeed * 1.1111f;
               }else{
                  m_fSpeed = 0.1f;
               }
            }else{

               if (m_fSpeed > l_fFightSpeed)
               {
                  m_fSpeed = m_fSpeed * 0.80f;
               }
            }

            if(m_iActionTimer>0)
            {
               m_iActionTimer = m_iActionTimer - int(g_fElapsedTime);
            }else{
               // -- entscheidung forcieren, wenn ausser reichweite, sonst ausrotieren
               //
               if (g_ehdlBuf.m_fDestLength > pow2(m_fRad * 5.0f))
               {
                  m_Action = ENACT_DONOTHING; 
                  m_fViewRotation1 = 0.0f;
               }
            }
            break;
         case ENACT_ACCELERATE:
   
            if (g_ehdlBuf.m_vDest.z > 0.0f && g_ehdlBuf.m_fDestLength > l_fTooFarPow2)
            {
               // -- Verfolgungsgeschwindigkeit 
               //    Geschwindigkeit des Players, mindestens aber 2
               //   
               m_fSpeed = m_fSpeed * 1.02f;
               if (m_fSpeed > 50.0f)
               {
                  m_fSpeed = 50.0f;
               }
               m_fViewRotation1 = 0;
               m_fViewRotation2 = 0;
            }else{
               m_Action = ENACT_CHASEPLAYER;
            }
            break;

        case ENACT_BANKROTATION:
            if(m_iActionTimer>0)
            {
               m_iActionTimer = m_iActionTimer - int(g_fElapsedTime);
               //m_fViewRotation1 = 0.1f;
            }else{
               detonate();
            }
            break;


         case ENACT_KAMIKAZE: 

            if ((g_ehdlBuf.m_vDest.z > 300.0f) || (g_ehdlBuf.m_vDest.z < 0.0f) ||
                (g_ehdlBuf.m_vDest.x > 20.0f) || (g_ehdlBuf.m_vDest.x < (-20.0f)) ||
                (g_ehdlBuf.m_vDest.y > 20.0f) || (g_ehdlBuf.m_vDest.y < (-20.0f)) )
            {
            //   m_Action = ENACT_AVOIDPLAYER;
//            }else{

               m_fViewRotation1 = 0.0f;
               m_fViewRotation2 = 0.0f;

               // -- Schnauze hoch/runterziehen bis Spieler in Mitte 
               //    sonst erstmal Fluegel/Horizont drehen, damit der Spieler auf 
               //    Vertikale liegt
               //
               if ((g_ehdlBuf.m_vDest.x < l_fTolerance) && (g_ehdlBuf.m_vDest.x > -l_fTolerance))
               { 
                  // -- Wenn Player im Zielfenster, aber zu weit weg, dann 
                  //    goto ACCELERATE
                  //
                  if (g_ehdlBuf.m_vDest.z > 0.0f && g_ehdlBuf.m_fDestLength > l_fTooFarPow2 &&
                      g_ehdlBuf.m_vDest.y < l_fTolerance && g_ehdlBuf.m_vDest.y > -l_fTolerance)
                  {
                   //  m_Action = ENACT_ACCELERATE;
                   //  m_iActionTimer = 20.0f / g_fBalanceFactor;
                  }else{
   
                     // -- sont wie ursprünglich beabsichtigt Nase hochziehen/runterziehen 
                     //
                     l_fDestLength2D = sqrt(pow2(g_ehdlBuf.m_vDest.z) + pow2(g_ehdlBuf.m_vDest.y));
                     l_dDegree = asin(g_ehdlBuf.m_vDest.y/l_fDestLength2D);
                     m_fViewRotation2 = - l_dDegree;

                  }
               }else{
   
                  // -- Die Fluegelausrichtung/Horizon neigen bis Dest auf Vertikalachse
                  //
                  l_fDestLength2D = sqrt(pow2(g_ehdlBuf.m_vDest.x) + pow2(g_ehdlBuf.m_vDest.y));
      
                  l_dDegree = asin(g_ehdlBuf.m_vDest.x/l_fDestLength2D);
                  //if (g_ehdlBuf.m_vDest.y > 0.0f) { l_dDegree = - l_dDegree; }
                  m_fViewRotation1 = l_dDegree;

               }
            }
            break;


         case ENACT_BREAKOUT_UP:
         case ENACT_BREAKOUT_DOWN:
			 // -- Dotproduct zwischen Player- und SmallFlyer-Richtungsvektor
			 //
		 {
			 float x, y, z;
			 XMFLOAT4 p, e;
			 XMStoreFloat4(&p, g_Player.m_pEnemy->getMatrixPointer()->r[2]);
			 XMStoreFloat4(&e, m_Mat.r[2]);
			 l_fDot = e.x*p.x + e.y*p.y + e.z*p.z;

			 //da diese Zeile aussieht, wie die letzte Berechnung von TransformDest()
			 //kann l_fDot auch angesehen werden als in den enemyraum versetzten Playerstep.z
		 }


            if ( l_fDot > 0.9f)  
            {
               switch (m_Action)
               {
                  case ENACT_BREAKOUT_UP:
                     m_fViewRotation2 = (M_PI * 0.5f - l_dDegree);//* g_fElapsedTime ;
                     break;
                  case ENACT_BREAKOUT_DOWN:
                     m_fViewRotation2 = (M_PI * 0.5f - l_dDegree);// * g_fElapsedTime;
                     break;
               }
            }else{
               m_Action = ENACT_ESCAPE;
               m_fViewRotation2 = 0.0f;
            }

            break;

         case ENACT_PULLUP:
            m_fViewRotation2 = 0.05f;
            break;
         case ENACT_PULLDOWN:
            m_fViewRotation2 = -0.05f;
            break;

         case ENACT_REFLECTION:

            // -- Abprall von StarBase
            //
            if(m_iActionTimer>0)
            {
               m_iActionTimer = m_iActionTimer - int(g_fElapsedTime);
            }else{
   
               // -- entscheidung forcieren, wenn ausser reichweite, sonst ausrotieren
               //
               m_Action = ENACT_DONOTHING; 
            }
            break;
   
      } // end-case m_Action

   }

   if (setMatrixAndStepSmallShip()==false) return false;

   return true;
}

// -- wandelt die Winkel in eine Drehmatrix und ermittelt einen neuen Step
//

bool ENEMY::setMatrixAndStepSmallShip()
{
   // -- Torques und Forces aus den beschlossenen Werten generieren
   //
   if (m_Action != ENACT_REFLECTION && 
       m_Action != ENACT_BANKROTATION &&
       m_dBodyID)
   {
      dReal l_dNewTorque[3];

	  
	  //if (g_penemyDebug == this)
	  //{
		 // char l_cDebugText[100];
		 // size_t l_cntConvertedChars;
		 // PSXPrintf(__LINE__, l_cDebugText, sizeof(l_cDebugText),
			//  "g_fElapsedTime= %f, m_fViewRotation1 =%f, m_fViewRotation2= %f",
			//  g_fElapsedTime,
			//  m_fViewRotation1,
			//  m_fViewRotation2);
		 // size_t l_cntDebugTextChars = strlen(l_cDebugText) + 1;
		 // mbstowcs_s(&l_cntConvertedChars, debugausgabe, l_cntDebugTextChars, l_cDebugText, _TRUNCATE);
	  //}
      // -- zunächst einmal relative Drehmomentsanforderung konvertieren in gesamt
      //    und dann differenz errechnen 
      //
      l_dNewTorque[0]= m_fViewRotation2 * 0.0005f;
      l_dNewTorque[1]= 0.0f; 
      l_dNewTorque[2]= m_fViewRotation1 * 0.0005f;
      if (setTorque(l_dNewTorque) == false) return false;

	  // -- get current Step from ODE and calc contained speed 
	  //
	  const dReal* l_pdStep;
	  
	  l_pdStep = dBodyGetLinearVel(m_dBodyID);
	  float l_fStepLengthPow2 = 
		  l_pdStep[0] * l_pdStep[0] + 
		  l_pdStep[1] * l_pdStep[1] +
		  l_pdStep[2] * l_pdStep[2];
	  float l_fStepLength = sqrtf(l_fStepLengthPow2);
	  XMFLOAT3 l_float3Velocity = XMFLOAT3(l_pdStep[0], l_pdStep[1], l_pdStep[2]);
	  XMVECTOR l_vVelocity = XMLoadFloat3( &l_float3Velocity);	  
	  XMVECTOR l_vDirection = XMVector3Normalize(m_Mat.r[2]);
	  XMVECTOR l_vForce = l_fStepLength * l_vDirection - l_vVelocity;
	  l_vForce = l_vForce + l_vDirection * m_fSpeed * 0.001f;

	  XMFLOAT3 l_float3Force;
	  XMStoreFloat3(&l_float3Force, l_vForce);

      dBodyAddForce(m_dBodyID, l_float3Force.x, l_float3Force.y, l_float3Force.z);
   }
   return true;
}

// -- interpretiert die Rotationswinkel anders, denn ein grosses Schiff dreht zur Seite ab
//    und zieht seine Schnauze nicht hoch oder runter
//
bool ENEMY::setMatrixAndStepBigCargo()
{
   // -- statt RotationToMatrix(); 
   //
   //if (isCollisionOnMove()==false)

   // -- Torques und Forces aus den beschlossenen Werten generieren
   //
   if (m_Action != ENACT_REFLECTION && 
       m_Action != ENACT_BANKROTATION &&
       m_dBodyID)
   {
      dReal l_dNewTorque[3];
      PSXVECTOR l_vForce;

      l_dNewTorque[0]= 0.0f;
      l_dNewTorque[1]= m_fViewRotation2 * 0.001f; 
      l_dNewTorque[2]= m_fViewRotation1 * 0.001f;
      if (setTorque(l_dNewTorque) == false) return false;

	  XMFLOAT3 l_float3Direction;
	  const dReal* l_pdStep;

	  XMStoreFloat3(&l_float3Direction, m_Mat.r[2]);
	  l_pdStep = dBodyGetLinearVel(m_dBodyID);
	  l_vForce.x = (l_float3Direction.x * m_fSpeed - l_pdStep[0]) * m_dMass.mass;
	  l_vForce.y = (l_float3Direction.y * m_fSpeed - l_pdStep[1]) * m_dMass.mass;
	  l_vForce.z = (l_float3Direction.z * m_fSpeed - l_pdStep[2]) * m_dMass.mass;
	  
      dBodyAddForce(m_dBodyID, l_vForce.x, l_vForce.y, l_vForce.z);

   }
 
   return true;
}

// -- Laserhalter ist Leader der Lasergun, hat aber selbst das Schiff als Leader
//
/*
bool ENEMY::TreatAsLaserHalter()
{
   float l_fDestLength2D;
   double l_dDegree;

   float l_fTolerance;
   float l_fTooFarPow2;

   // -- vom Leader den Angriffsstatus übernehmen und 
   //    die Ausrichtung des Transformgeoms aktualisieren
   //
   if (m_pLeader)
   {
      setTriggered(m_pLeader->isTriggered());
      
      if (isPartOfLeader())
      {
         D3DMATRIX l_matSingleMovement, 
                   l_matRotation1,
                   l_matRotation2;

         memcpy (&l_matRotation1,&g_matIdentity, sizeof(g_matIdentity));
         l_matRotation1._11 =   cos (m_fViewRotation1);
         l_matRotation1._12 = - sin (m_fViewRotation1);
         l_matRotation1._21 =   sin (m_fViewRotation1);
         l_matRotation1._22 =   cos (m_fViewRotation1);
      
         memcpy (&l_matRotation2,&g_matIdentity, sizeof(g_matIdentity));
         l_matRotation2._11 =   cos (m_fViewRotation2);
         l_matRotation2._13 = - sin (m_fViewRotation2);
         l_matRotation2._31 =   sin (m_fViewRotation2);
         l_matRotation2._33 =   cos (m_fViewRotation2);

         matmult(&l_matSingleMovement, &l_matRotation1, &l_matRotation2);
         if (setOffsetMat(&l_matSingleMovement)==false) return false;
      }
   }else{
      setTriggered(false);
   }
   //m_fDestLength = pow2(m_vCurrNode.x)+ pow2(m_vCurrNode.y)+ pow2(m_vCurrNode.z);
   TransformDest();

   switch(m_Action)
   {
      case ENACT_SHOOT:
         // extra kein break, damit er nachfolgenden Teil mitausführt
      case ENACT_CHASEPLAYER: 

         l_fTooFarPow2 = pow2(2000.0f);

         // -- Zielfenster realistisch setzen: Bei weiter Distanz
         //    reicht eine ungefaehre Anpeilung
         //
         if (m_fDestLength > l_fTooFarPow2)
         {
            l_fTolerance = 500.0f;
         }else{
            l_fTolerance = 40.0f;
         }

         // -- dieser if muss als erstes kommen, damit m_Action noch überschrieben werden kann
         //
//         if(m_fDestLength < l_fTooFarPow2 &&
//            g_ehdlBuf.m_vDest.x < l_fTolerance && g_ehdlBuf.m_vDest.x > -l_fTolerance && 
//            g_ehdlBuf.m_vDest.y > 0.0f)
         if(m_fDestLength < l_fTooFarPow2 &&
            g_ehdlBuf.m_vDest.x < l_fTolerance && g_ehdlBuf.m_vDest.x > -l_fTolerance && 
            g_ehdlBuf.m_vDest.y < 0.0f)
         { 
            m_Action = ENACT_SHOOT;
         }else{
            m_Action = ENACT_CHASEPLAYER;
         }

         if (isTriggered() == false || m_pLeader == NULL)
         {
            m_Action = ENACT_DONOTHING;
         }

         l_fDestLength2D = sqrt(pow2(g_ehdlBuf.m_vDest.x) + pow2(g_ehdlBuf.m_vDest.z));

         l_dDegree = asin(g_ehdlBuf.m_vDest.x/l_fDestLength2D);
         m_fViewRotation2 = m_fViewRotation2 + l_dDegree * 0.004f * g_fElapsedTime;

         break;

      case ENACT_FOLLOWLEADER:
         m_fViewRotation2 = m_fViewRotation2 * 0.9f;

         if (m_pLeader)
         {
            if (isTriggered())
            {
               m_Action = ENACT_DONOTHING;
            }
         }else{
            m_Action = ENACT_DONOTHING;
         }
         break;
      // -- Entscheidung über nächste Aktion
      //
      case ENACT_DONOTHING:
         if(m_pLeader)
         { 
            if (isTriggered())
            {
               m_Action = ENACT_CHASEPLAYER;
            }else{
               m_Action = ENACT_FOLLOWLEADER;
            }
         }else{
            setTriggered(false);
            m_Action = ENACT_BANKROTATION;
            m_iActionTimer = 10000;
         }
         break;
      
   }
   return true;
}
*/
// -- Lauf senken/heben - hat als Leader den Halter
//
/*
bool ENEMY::TreatAsLaserGun()
{
   ENEMY* l_penemyHalter;
   float l_fDestLength2D;
   double l_dDegree;

   float l_fTolerance;
   float l_fTooFarPow2;

   // -- die Transformationsmatrix aus dem Laserhalter holen
   //    und eigene Rotation draufrechnen
   //
   l_penemyHalter = m_pLeader->getNextEnemy();
   while(l_penemyHalter)
   {
      if (l_penemyHalter->mType == ETYP_LASERHALTER)
      {
         //dGeomID l_dGeomIDEncapsulated;
         const dReal* l_pdMatrix3Halter;
         dMatrix3 l_dMatrix3SingleMovement, l_dMatrix3Gun;

         //l_dGeomIDHalter = dGeomTransformGetGeom(l_penemyHalter->getGeomID());
         l_pdMatrix3Halter = dGeomGetOffsetRotation(l_penemyHalter->getGeomID());

         // -- ODE-mässig die anzuhängende Rotationsmatrix aufbauen
         //
         dRSetIdentity(l_dMatrix3SingleMovement);
         l_dMatrix3SingleMovement[5]  =   cos ( m_fViewRotation2);
         l_dMatrix3SingleMovement[6]  =   sin ( m_fViewRotation2);
         l_dMatrix3SingleMovement[9]  = - sin ( m_fViewRotation2);
         l_dMatrix3SingleMovement[10] =   cos ( m_fViewRotation2);
         dMULTIPLY0_333 (l_dMatrix3Gun,l_dMatrix3SingleMovement,l_pdMatrix3Halter);

         // -- eigenes gekapseltes Geom holen und auf die neue Matrix setzen
         //
         //l_dGeomIDEncapsulated = dGeomTransformGetGeom(getGeomID());
         dGeomSetOffsetRotation(m_dGeomID, l_dMatrix3Gun);
         
         // -- weitere Daten übernehmen
         //
         setTriggered(l_penemyHalter->isTriggered());

         m_Action         = l_penemyHalter->m_Action;

         l_penemyHalter = NULL; // aussteigen
      }else{
         l_penemyHalter = l_penemyHalter->getNextEnemy();
      }
   }

   // -- Zielfenster realistisch setzen: Bei weiter Distanz
   //    reicht eine ungefaehre Anpeilung
   //
   //m_fDestLength = pow2(m_vCurrNode.x)+ pow2(m_vCurrNode.y)+ pow2(m_vCurrNode.z);
   TransformDest();

   // -- m_Action wird selber nur durch den Leader angepasst
   //
   switch(m_Action)
   {
      case ENACT_SHOOT:

         if(m_iActionTimer > 0)
         {
            m_iActionTimer = m_iActionTimer - int(g_fElapsedTime);
         }else{
            if(m_pLeader->mType==ETYP_TRADER)
            { 
               if (ShootStraightly(g_laserGunArrayS, g_cntLaserGun) == false) return false;
            }else{
               if (ShootStraightly(g_laserGunArray, g_cntLaserGun) == false) return false;
            }
            m_iActionTimer=300; // naechsten schuss timen
         }

         l_fTooFarPow2 = pow2(2000.0f);
         if (m_fDestLength > l_fTooFarPow2)
         {
            l_fTolerance = 500.0f;
         }else{
            l_fTolerance = 40.0f;
         }

         l_fDestLength2D = sqrt(pow2(g_ehdlBuf.m_vDest.y) + pow2(g_ehdlBuf.m_vDest.z));
         l_dDegree = asin(g_ehdlBuf.m_vDest.y/l_fDestLength2D);

//         if (m_fViewRotation2 <= g_fPi && m_fViewRotation2 <= 0.0f)
         {
            m_fViewRotation2 = m_fViewRotation2 - l_dDegree * 0.004f * g_fElapsedTime;

         }

         break;

      case ENACT_BANKROTATION:
      case ENACT_FOLLOWLEADER:
         m_fViewRotation2 = m_fViewRotation2 * 0.9f;
         break;
   }

   return true;
}
*/
// -- hängt evtl. eine Laserkanone dran
//
/*
bool ENEMY::armWithLaserGuns()
{
   ENEMY* l_penemyHalter, *l_penemyGun;

   struct _tempLaserpos {
      RESTYPE   resLeaderCargo;
      PSXVECTOR vRelative;
      float     fScale;
      float     fRotation;
   } static l_tempLaser[] = {
      { RESTYPE_CARGO01,    { -0.043000, -0.368999, -0.300000}, 0.800000, 0.266000},
      { RESTYPE_CARGO04,    { 0.0f, -0.352999f, 0.0f}, 1.0f,0.0f},
      { RESTYPE_CARGO05,    { 0.000000, -0.263000, 0.615005}, 0.195010, 0.000000},
      { RESTYPE_CARGO06,    { 0.000000, -0.185000, 0.761994}, 0.438007, 0.000000},
      { RESTYPE_CARGO07,    { -0.034000, -0.411998, -0.032000}, 0.241010, 0.000000},
      { RESTYPE_TRANSPORT1, { 0.257000, -0.364999, 0.726994}, 0.161009, 0.000000},
      { RESTYPE_BOA01,      { -0.304000, -0.288000, 0.145000}, 1.066003, 0.000000}
   };
   int k, l_cntLaserPos = sizeof(l_tempLaser) / sizeof(l_tempLaser[0]);
   D3DMATRIX l_matSingleMovement;

   WriteLogFile(g_cEnemyTypeNameArray[mType]);
   WriteLogFile("::armWithLaserGuns()\n");

   for (k = 0; k < l_cntLaserPos; k++)
   {
      if (m_ResType == l_tempLaser[k].resLeaderCargo)
      {

         g_pCurrLeader = this;

         // -- den Laserhalter montieren 
         //
         l_penemyHalter = new (LIFECYCLE_LEVEL) ENEMY(LIFECYCLE_LEVEL, m_pCharacter);
         if (l_penemyHalter == NULL) return false;

         l_penemyHalter->SetType(ETYP_LASERHALTER);
         l_penemyHalter->m_cntRanking = m_cntRanking;

         if (l_penemyHalter->setOffset( l_tempLaser[k].vRelative.x * m_fRad,
                                        l_tempLaser[k].vRelative.y * m_fRad,
                                        l_tempLaser[k].vRelative.z * m_fRad ) == false) return false;

         memcpy (&l_matSingleMovement,&g_matIdentity, sizeof(g_matIdentity));
         l_matSingleMovement._11 =   cos ( l_tempLaser[k].fRotation);
         l_matSingleMovement._12 = - sin ( l_tempLaser[k].fRotation);
         l_matSingleMovement._21 =   sin ( l_tempLaser[k].fRotation);
         l_matSingleMovement._22 =   cos ( l_tempLaser[k].fRotation);
         if (l_penemyHalter->setOffsetMat(&l_matSingleMovement)==false) return false;

         l_penemyHalter->m_fViewRotation1 = l_tempLaser[k].fRotation;

         if (l_penemyHalter->setScale(l_tempLaser[k].fScale) == false) return false;

         // -- die Kanone in den Laserhalter einbringen
         //
         l_penemyGun = new (LIFECYCLE_LEVEL) ENEMY(LIFECYCLE_LEVEL, m_pCharacter);
         if (l_penemyGun == NULL) return false;
         l_penemyGun->SetType(ETYP_LASERGUN);
         l_penemyGun->m_cntRanking = m_cntRanking;
      
         //-- anpassen des Mittelpunktes der Gun
         //
         if (l_penemyGun->setOffsetMat(&l_matSingleMovement)==false) return false;
         l_penemyGun->m_fViewRotation1 = l_tempLaser[k].fRotation;

         if (l_penemyGun->setOffset( l_tempLaser[k].vRelative.x * m_fRad,
                                     l_tempLaser[k].vRelative.y * m_fRad +
                                     (-l_penemyHalter->m_fRad * l_penemyHalter->m_fScale * 0.643314), 
                                     l_tempLaser[k].vRelative.z * m_fRad ) == false) return false;

                                     
         if (l_penemyGun->setScale(l_tempLaser[k].fScale) == false) return false;
      }
   }
   

   return true;
}
*/
// -- Fuer den Debug-enemy eine Mitteilung rausschreiben
//
/*
void ENEMY::WriteDebug(char* l_cDebugMessage)
{
   if (g_bEnemyDebugValid)
   {
      if (g_penemyDebug == this)
      {
         WriteLogFile(l_cDebugMessage);
      }
   }
}
*/
// --- Shoot()
//     sucht freien Eintrag in Schusstabelle und fuegt neuen ein
//
bool ENEMY::Shoot()
{
   PSXLASERSPAWN* l_laserspawnArray;
   int l_cntLaserSpawn;
   MENACETYPE l_menaceType;

   int l_cntFound;
   bool l_bStraight;
   
   const int l_maxSpawn = 20;
   SHOOT* l_arr_pShoot[l_maxSpawn];

   if (mType == ETYP_PLAYER)
   {
      if (g_pequipmentSelectWeapon->insertShoot()==false) return false;

   }else{

      switch (m_ResType)
      {
         case RESTYPE_LEADER:  l_laserspawnArray = g_laserPoliceLeaderArray;
                               l_cntLaserSpawn   = g_cntLaserPoliceLeader;
                               l_bStraight       = false;
                               l_menaceType      = MENACE_SELF;
                               break;
         case RESTYPE_FLYER:   l_laserspawnArray = g_laserPoliceFlyerArray;
                               l_cntLaserSpawn   = g_cntLaserPoliceFlyer;
                               l_bStraight       = false;
                               l_menaceType      = MENACE_SELF;
                               break;
         case RESTYPE_HUNTER_X:l_laserspawnArray = g_laserHunterXArray;
                               l_cntLaserSpawn   = g_cntLaserHunterX;
                               l_bStraight       = false;
                               l_menaceType      = MENACE_SELF;
                               break;
         case RESTYPE_LASERGUN:l_laserspawnArray = g_laserGunArray;
                               l_cntLaserSpawn   = g_cntLaserGun;
                               l_bStraight       = true;
                               l_menaceType      = MENACE_SELF;
                               break;
         case RESTYPE_WINGMAN: l_laserspawnArray = g_laserWingmanArray;
                               l_cntLaserSpawn   = g_cntLaserWingman;
                               l_bStraight       = true;
                               l_menaceType      = MENACE_ENEMY;
                               g_cntMousestateCheckTimer[0] = 200; //Maustastenverzögerung 
                               break;
         default:              l_laserspawnArray = g_laserHunterXArray;
                               l_cntLaserSpawn   = g_cntLaserHunterX;
                               l_bStraight       = false;
                               l_menaceType      = MENACE_SELF;
                               break;
      }
   
      if (l_cntLaserSpawn >= l_maxSpawn)
      {
         l_cntLaserSpawn = l_maxSpawn;
      }
      l_cntFound = PSXAllocateShoot(l_cntLaserSpawn, l_arr_pShoot);
   
      for (int i=0; i < l_cntFound; i++)
      {
		  XMFLOAT3 l_float3Load;
		  l_float3Load.x = l_laserspawnArray[i].vPos.x;
		  l_float3Load.y = l_laserspawnArray[i].vPos.y;
		  l_float3Load.z = l_laserspawnArray[i].vPos.z;
		  XMVECTOR l_vSpawn = XMLoadFloat3(&l_float3Load);

		  l_arr_pShoot[i]->m_vPos = m_vPos + XMVector3Transform(l_vSpawn, m_Mat);

         //l_arr_pShoot[i]->m_vPos.x = m_vPos.x + 
         //                            l_laserspawnArray[i].vPos.x * m_fScale * m_Mat._11 +
         //                            l_laserspawnArray[i].vPos.y * m_fScale * m_Mat._21 +
         //                            l_laserspawnArray[i].vPos.z * m_fScale * m_Mat._31;
         //l_arr_pShoot[i]->m_vPos.y = m_vPos.y + 
         //                            l_laserspawnArray[i].vPos.x * m_fScale * m_Mat._12 +
         //                            l_laserspawnArray[i].vPos.y * m_fScale * m_Mat._22 +
         //                            l_laserspawnArray[i].vPos.z * m_fScale * m_Mat._32;
         //l_arr_pShoot[i]->m_vPos.z = m_vPos.z + 
         //                            l_laserspawnArray[i].vPos.x * m_fScale * m_Mat._13 +
         //                            l_laserspawnArray[i].vPos.y * m_fScale * m_Mat._23 +
         //                            l_laserspawnArray[i].vPos.z * m_fScale * m_Mat._33;
   /*
         if (l_bStraight)
         {
   */
		  XMVECTOR l_vStep = m_Mat.r[3];
		  l_arr_pShoot[i]->m_vStep = l_vStep * l_laserspawnArray[i].fSpeed;

		  //l_arr_pShoot[i]->m_vStep.x = m_Mat._31 * l_laserspawnArray[i].fSpeed;
		  //l_arr_pShoot[i]->m_vStep.y = m_Mat._32 * l_laserspawnArray[i].fSpeed;
		  //l_arr_pShoot[i]->m_vStep.z = m_Mat._33 * l_laserspawnArray[i].fSpeed;
/*
         }else{
            PSXVECTOR l_vDirection;
            float l_fLength;

            l_fLength = sqrt(m_fDestLength);
            l_vDirection.x = (g_Player.m_pEnemy->getPosX() - l_arr_pShoot[i]->m_vPos.x) / l_fLength;
            l_vDirection.y = (g_Player.m_pEnemy->getPosY() - l_arr_pShoot[i]->m_vPos.y) / l_fLength;
            l_vDirection.z = (g_Player.m_pEnemy->getPosZ() - l_arr_pShoot[i]->m_vPos.z) / l_fLength;
      
            l_arr_pShoot[i]->m_vStep.x = l_vDirection.x * (l_laserspawnArray[i].fSpeed + m_fSpeed);
            l_arr_pShoot[i]->m_vStep.y = l_vDirection.y * (l_laserspawnArray[i].fSpeed + m_fSpeed);
            l_arr_pShoot[i]->m_vStep.z = l_vDirection.z * (l_laserspawnArray[i].fSpeed + m_fSpeed);

         }
*/

	  l_arr_pShoot[i]->m_cntTimeToLive = 90;  //g_maxTimeToLiveMenaceSelf;
	  l_arr_pShoot[i]->m_maxTimeToLive = 90;  //g_maxTimeToLiveMenaceSelf;
	  l_arr_pShoot[i]->menacetype = l_menaceType;
	  l_arr_pShoot[i]->m_penemyHitableArray = NULL;
	  l_arr_pShoot[i]->m_pEquipment = NULL;
	  l_arr_pShoot[i]->m_idxShootDrawer = l_laserspawnArray[i].idxShootDrawer;
	  //l_arr_pShoot[i]->selectHitableEnemies();
                  
	} // end-for i=0 to l_cntFound
                  
      /*if (g_options.IsEnableSound())
      {
         if (g_sound_array[SNDRES_SUPERLASER_FAR].play(this) == false) return false;
      }*/
   }
   return true;
}

// -- schiesst stur gerade aus 
//

bool ENEMY::ShootStraightly(PSXLASERSPAWN* l_laserspawnArray, int l_cntSpawn)
{
	/*
   int l_cntFound;
   
   float l_fLength;
 
   const int l_maxSpawn = 20;
   SHOOT* l_arr_pShoot[l_maxSpawn];

   l_fLength = sqrt(m_fDestLength);

   if (l_cntSpawn >= l_maxSpawn)
   {
      l_cntSpawn = l_maxSpawn;
   }
   l_cntFound = PSXAllocateShoot(l_cntSpawn, l_arr_pShoot);

   for (int i=0; i < l_cntFound; i++)
   {
      l_arr_pShoot[i]->m_vPos.x = m_vPos.x + 
                                  l_laserspawnArray[i].vPos.x * m_fScale * m_Mat._11 +
                                  l_laserspawnArray[i].vPos.y * m_fScale * m_Mat._21 +
                                  l_laserspawnArray[i].vPos.z * m_fScale * m_Mat._31;
      l_arr_pShoot[i]->m_vPos.y = m_vPos.y + 
                                  l_laserspawnArray[i].vPos.x * m_fScale * m_Mat._12 +
                                  l_laserspawnArray[i].vPos.y * m_fScale * m_Mat._22 +
                                  l_laserspawnArray[i].vPos.z * m_fScale * m_Mat._32;
      l_arr_pShoot[i]->m_vPos.z = m_vPos.z + 
                                  l_laserspawnArray[i].vPos.x * m_fScale * m_Mat._13 +
                                  l_laserspawnArray[i].vPos.y * m_fScale * m_Mat._23 +
                                  l_laserspawnArray[i].vPos.z * m_fScale * m_Mat._33;

      l_arr_pShoot[i]->m_vStep.x     = m_Mat._31 * l_laserspawnArray[i].fSpeed;
      l_arr_pShoot[i]->m_vStep.y     = m_Mat._32 * l_laserspawnArray[i].fSpeed;
      l_arr_pShoot[i]->m_vStep.z     = m_Mat._33 * l_laserspawnArray[i].fSpeed;

      l_arr_pShoot[i]->m_cntTimeToLive = g_maxTimeToLiveMenaceSelf;
      l_arr_pShoot[i]->m_maxTimeToLive = g_maxTimeToLiveMenaceSelf;
      l_arr_pShoot[i]->menacetype      = MENACE_SELF;
      l_arr_pShoot[i]->m_penemyHitableArray = NULL;
      l_arr_pShoot[i]->m_pEquipment = NULL;
      l_arr_pShoot[i]->m_idxShootDrawer = l_laserspawnArray[i].idxShootDrawer;
      //l_arr_pShoot[i]->selectHitableEnemies();
               
   } // end-for i=0 to l_cntFound
               
   if (g_options.IsEnableSound())
   {
      if (g_sound_array[SNDRES_SUPERLASER_FAR].play(this) == false) return false;
   }
   */
   return true;
}

bool ENEMY::ShotAgainstShield(int l_idxShoot, XMVECTOR* l_pvNormal, XMVECTOR* l_pvPos)
{

   float l_fStepLength, l_fNormalLength, l_fDegree; 
   XMVECTOR l_vReflectNormal, l_vStepLength, l_vDegree;

   if (g_bWriteLogFileInLoop)
   {
      WriteLogFile("::ShotAgainstShield");
   }

   // --- play sound for Deflector Hit
   //
   /*
   if (g_options.IsEnableSound())
   {
      if (g_options.IsWriteLogFileInLoop())
      {
         WriteLogFile("play sound");
      }

      if (g_sound_array[SNDRES_HITENEMYDEFLECTOR].play(this) == false) return false;
   }
   */


   // -- falls bereits eine normal aus facegenauer pruefung vorliegt
   //
   if (l_pvNormal)
   {
      l_vReflectNormal = *l_pvNormal;
      //l_vReflectNormal.x = l_pvNormal->x;
      //l_vReflectNormal.y = l_pvNormal->y;
      //l_vReflectNormal.z = l_pvNormal->z;
      
   }else{

      // --- Reflektionsvektor ist neuer SHOOT::Step (-I dot N)*N*2+ I = R
      //
      l_vReflectNormal = XMVector3NormalizeEst(shoot_array[l_idxShoot].m_vPos - 
                                               shoot_array[l_idxShoot].m_vStep - 
                                               m_vPos);
      //l_vReflectNormal.x = shoot_array[l_idxShoot].m_vPos.x - 
      //                     shoot_array[l_idxShoot].m_vStep.x - 
      //                     m_vPos.x;
      //l_vReflectNormal.y = shoot_array[l_idxShoot].m_vPos.y - 
      //                     shoot_array[l_idxShoot].m_vStep.y - 
      //                     m_vPos.y;
      //l_vReflectNormal.z = shoot_array[l_idxShoot].m_vPos.z - 
      //                     shoot_array[l_idxShoot].m_vStep.z - 
      //                     m_vPos.z;
      //l_fNormalLength = sqrt(pow2(l_vReflectNormal.x)+
      //                       pow2(l_vReflectNormal.y)+
      //                       pow2(l_vReflectNormal.z));

      //l_vReflectNormal.x = l_vReflectNormal.x / l_fNormalLength;
      //l_vReflectNormal.y = l_vReflectNormal.y / l_fNormalLength;
      //l_vReflectNormal.z = l_vReflectNormal.z / l_fNormalLength;
   }

   //l_fStepLength = sqrt(pow2(shoot_array[l_idxShoot].m_vStep.x)+
   //                     pow2(shoot_array[l_idxShoot].m_vStep.y)+
   //                     pow2(shoot_array[l_idxShoot].m_vStep.z));

   l_vStepLength = XMVector3ReciprocalLengthEst(shoot_array[l_idxShoot].m_vStep);
	
   l_vDegree =  XMVector3Dot( l_vReflectNormal, (shoot_array[l_idxShoot].m_vStep * l_vStepLength)) ;
   //l_vDegree =  XMVector3Dot( l_vReflectNormal, (shoot_array[l_idxShoot].m_vStep / l_vStepLength)) ;

   //l_fDegree = ( l_vReflectNormal.x * shoot_array[l_idxShoot].m_vStep.x +
   //              l_vReflectNormal.y * shoot_array[l_idxShoot].m_vStep.y +
   //              l_vReflectNormal.z * shoot_array[l_idxShoot].m_vStep.z )/ l_fStepLength;
   
   m_cntShieldFlashTimer = 1;
   //shoot_array[l_idxShoot].m_cntTimeToLive = 100;
   shoot_array[l_idxShoot].menacetype = MENACE_NONE;

   //shoot_array[l_idxShoot].m_vStep.x = - 2.0f * l_fDegree * l_vReflectNormal.x * l_fStepLength +
   //                                    shoot_array[l_idxShoot].m_vStep.x ;
   //shoot_array[l_idxShoot].m_vStep.y = - 2.0f * l_fDegree * l_vReflectNormal.y * l_fStepLength +
   //                                    shoot_array[l_idxShoot].m_vStep.y ;
   //shoot_array[l_idxShoot].m_vStep.z = - 2.0f * l_fDegree * l_vReflectNormal.z * l_fStepLength +
   //                                    shoot_array[l_idxShoot].m_vStep.z ;

   shoot_array[l_idxShoot].m_vStep = - 2.0f * l_vDegree * l_vReflectNormal * l_vStepLength + shoot_array[l_idxShoot].m_vStep ;

	/*
   if (g_equipmentArray[EQUIPMENT_TORPEDO].m_bSingleton)
   {
      if (l_idxShoot == g_equipmentArray[EQUIPMENT_TORPEDO].m_idxShootSingleton)
      {
         if (g_Player.getGS()->setIndex(GAMESCREEN_COCKPIT) == false) return false;
         g_equipmentArray[EQUIPMENT_TORPEDO].m_bSingleton = false;
      }
   }
	*/

   // -- Schuss zum Neustart genau auf den Kollisionspunkt setzen
   //
   if (l_pvPos)
   {
      shoot_array[l_idxShoot].m_vPos = *l_pvPos;
   }
   
   return true;
 
}

// -- definiert typ-abhaengige Ausnahmen zu Init
//

bool ENEMY::initTypeDependend()
{
	
   struct _typeinitlist {
      ETYPE etypeMatch;
      int   iIsLeader;
      int   iHasLeader;
      float fShieldAmount;
      int   iTargetable;
      float fExplScale;
      float fSpeed;
      float fMinRad2DForVisibility;
      int   iAffectableByShoot;
      int   iCheckFaceIntersection;
      int   iShootMagnetical;
      int   iCargoBoxOnBoard;
      int   iRadarable;
      int   iIsBreakingOut;
      int   iIsBodyInstantly; 
      int   iIsPartOfLeader; 
      ENUM_TXX_ID txxDestFrameInfo;
   } l_tempTypeInitArray[] = {
      //  type             is has shld  tar Expl  Speed Min2DRad aff fi mg cb rd bo bi pl m_cDestFrameInfo
      {ETYP_FLYER,         0, 1,  0.7f, 1,  5.0f, 2.0f, 0.01f,   1,  0, 1, 0, 1, 0, 1, 0, txxPoliceFlyer},
      {ETYP_LEADER,        1, 0,  1.0f, 1,  5.0f, 2.0f, 0.01f,   1,  0, 1, 0, 1, 0, 1, 0, txxPoliceLeader},
      {ETYP_HUNTER,        0, 0,  1.0f, 1,  5.0f, 2.0f, 0.01f,   1,  0, 1, 0, 1, 1, 1, 0, txxBountyHunter},
	  { ETYP_DAZ3D_CUBE,   0, 0,  1.0f, 1,  5.0f, 2.0f, 0.01f,   1,  0, 1, 0, 1, 1, 1, 0, txxBountyHunter },
      {ETYP_PIRATE,        1, 0,  5.0f, 1,  5.0f, 2.0f, 0.01f,   1,  1, 0, 1, 1, 0, 1, 0, txxPirate},
      {ETYP_P_FLYER,       0, 1,  2.8f, 1,  8.0f, 2.0f, 0.01f,   1,  0, 1, 0, 1, 1, 1, 0, txxPirate},
      {ETYP_TRADER,        0, 0,  3.0f, 1,  5.0f, 2.0f, 0.01f,   1,  0, 0, 1, 1, 0, 1, 0, txxTrader},
      {ETYP_HANGAR,        0, 0,  0.6f, 1,  2.0f, 0.0f, 0.005f,  1,  1, 0, 0, 0, 0, 1, 0, txxNull},

      {ETYP_GUN,           0, 0,  0.5f, 1,  1.0f, 0.0f, 0.005f,  1,  0, 0, 0, 0, 0, 1, 0, txxGun},
      {ETYP_STATIC,        0, 0,  0.5f, 0,  1.0f, 0.0f, 0.005f,  0,  0, 0, 0, 0, 0, 0, 0, txxNull},
      {ETYP_CARGOBOX,      0, 0,  0.5f, 0,  1.0f, 0.0f, 0.005f,  0,  0, 0, 0, 0, 0, 0, 0, txxCommodity},
      {ETYP_BIGRESITEM,    0, 0,  0.5f, 0,  1.0f, 0.0f, 0.005f,  0,  0, 0, 0, 0, 0, 0, 1, txxCommodity},
      {ETYP_LASERGUN,      0, 1,  1.0f, 0,  1.0f, 0.0f, 0.005f,  0,  0, 0, 0, 0, 0, 0, 1, txxGun},
      {ETYP_LASERHALTER,   0, 1,  1.0f, 0,  1.0f, 0.0f, 0.005f,  0,  0, 0, 0, 0, 0, 0, 1, txxGun},
      {ETYP_REACTORGUN,    0, 0,  0.0f, 1,  1.0f, 0.0f, 0.005f,  1,  0, 0, 0, 0, 0, 1, 0, txxNull},
      {ETYP_ASTEROID,      0, 0,  0.0f, 0,  0.0f, 0.0f, 0.01f,   0,  1, 0, 0, 1, 0, 1, 0, txxAsteroid},
      {ETYP_STARBASE,      0, 0,  0.0f, 0,  0.0f, 0.0f, 0.01f,   0,  1, 0, 0, 1, 0, 1, 0, txxStarBase},
      {ETYP_STARBASEHANGAR,0, 0,  0.0f, 0,  0.0f, 0.0f, 0.01f,   0,  1, 0, 0, 1, 0, 1, 0, txxStarBase},
      //{ETYP_DOOR,         1, 1,  0.0f, 1,  5.0f, 0.0f, 0.005f,  1,  1, 0, 0, 0, 0, txxDoorToInnerFlagship},
      {ETYP_KINGSPLANE,    0, 1,  0.0f, 0,  0.0f, 0.0f, 0.005f,  0,  1, 0, 0, 0, 0, 1, 0, txxNull},
      {ETYP_PLANET,        0, 0,  0.0f, 0,  0.0f, 0.0f, 0.005f,  0,  0, 0, 0, 0, 0, 0, 0, txxNull},

      {ETYP_THARGOID,      1, 0,  5.0f, 1,  5.0f, 2.0f, 0.01f,   1,  1, 0, 0, 1, 0, 1, 0, txxThargoid},
      {ETYP_THARGOIDDRONE, 0, 1,  0.7f, 1,  5.0f, 2.0f, 0.01f,   1,  0, 1, 0, 1, 0, 1, 0, txxThargoiddrone},
      {ETYP_PLAYER,        0, 0,  5.0f, 0,  5.0f, 2.5f, 0.01f,   1,  0, 0, 0, 0, 0, 1, 0, txxNull},
   };
   int l_idxInitList;
   int l_cntInitList = sizeof(l_tempTypeInitArray) / sizeof(l_tempTypeInitArray[0]);
   
   for (l_idxInitList = 0; l_idxInitList < l_cntInitList; l_idxInitList++)
   {
      if (l_tempTypeInitArray[l_idxInitList].etypeMatch == mType)
      {
         m_fShieldAmount          = l_tempTypeInitArray[l_idxInitList].fShieldAmount;
         m_fExplScale             = l_tempTypeInitArray[l_idxInitList].fExplScale;

         m_fSpeed                 = l_tempTypeInitArray[l_idxInitList].fSpeed;
         m_fMinRad2DForVisibility = l_tempTypeInitArray[l_idxInitList].fMinRad2DForVisibility;
		 
         if (l_tempTypeInitArray[l_idxInitList].txxDestFrameInfo > txxNull)
         {
            m_cDestFrameInfo = g_cLang[l_tempTypeInitArray[l_idxInitList].txxDestFrameInfo];
         }else{
            m_cDestFrameInfo = NULL;
         }
		 
         if (l_tempTypeInitArray[l_idxInitList].iAffectableByShoot > 0)
         {
            setAffectableByShoot(true);
         }else{
            setAffectableByShoot(false);
         }

         if (l_tempTypeInitArray[l_idxInitList].iCheckFaceIntersection > 0)
         {
            setCheckFaceIntersection(true);
         }else{
            setCheckFaceIntersection(false);
         }

         if (l_tempTypeInitArray[l_idxInitList].iShootMagnetical > 0)
         {
            setShootMagnetical(true);
         }else{
            setShootMagnetical(false);
         }

         if (l_tempTypeInitArray[l_idxInitList].iCargoBoxOnBoard > 0)
         {
            setCargoBoxOnBoard(true);
         }else{
            setCargoBoxOnBoard(false);
         }


         if (l_tempTypeInitArray[l_idxInitList].iTargetable > 0)
         {
            setTargetable(true);
         }else{
            setTargetable(false);
         }

         if (l_tempTypeInitArray[l_idxInitList].iRadarable > 0)
         {
            setRadarable(true);
         }else{
            setRadarable(false);
         }
		 /*
         if (l_tempTypeInitArray[l_idxInitList].iHasLeader > 0)
         {
            m_pLeader        = m_pCurrLeader;
         }else{
            m_pLeader        = NULL;
         }
		 */
		 /*
         if (l_tempTypeInitArray[l_idxInitList].iIsBreakingOut > 0)
         {
            setBreakingOut(true);
         }else{
            setBreakingOut(false);
         }

         if (l_tempTypeInitArray[l_idxInitList].iIsLeader > 0)
         {
            m_pCurrLeader    = this;
		 }
		 */
         // -- ODE Rigid Body anlegen
         //
		 
         if (l_tempTypeInitArray[l_idxInitList].iIsBodyInstantly > 0 && LEVEL::m_iWorldID)
         {
            if (createBody() == false) return false;
         }
		 
         if (l_tempTypeInitArray[l_idxInitList].iIsPartOfLeader > 0)
         {
            setPartOfLeader(true);
         }else{
            setPartOfLeader(false);
         }
		 
         l_idxInitList = l_cntInitList;
      } // end-if matched
   }
   
   return true;
}

// -- object aus der verkettung entfernen
//
bool ENEMY::Destroy(LIFECYCLE l_lifecycle) 
{

   WriteLogFile("::Destroy()\n");

   WriteLogFile("taking enemy out of chain\n");

   // -- ist dies schon der letzte Enemy
   //
   if (m_penemyLast == this)
   {
      WriteLogFile("This was the last enemy in list, set lastpointer to previous enemy");
      // -- wenn vorheriges Objekt vorhanden, dann den dortigen Nextzeiger löschen
      //
      if (m_penemyPrevious)
      {
         m_penemyLast = m_penemyPrevious;
      }

      // -- war dies auch gleichzeitig das erste Level, dann
      //   den Indexvergabe zähler zurücksetzen 
      //
      if (m_penemyFirst == this)
      {
         m_idxHighest = 0;
      }
   }
   // -- dem vorherigen Level das eigene Next aufdrücken 
   //
   if (m_penemyPrevious)
   {
      m_penemyPrevious->m_penemyNext = m_penemyNext;
   }

   // -- dem nächsten Level das eigene Previous aufdrücken 
   //
   if (m_penemyNext)
   {
      m_penemyNext->m_penemyPrevious = m_penemyPrevious;
   }

   if (g_penemyCamera == this)
   {
      g_penemyCamera = NULL;
   }

   if (g_penemyInput == this)
   {
      g_penemyInput = NULL;
   }

/*

   if (g_levelContent.m_penemyStarBase == this)
   {
      g_levelContent.m_penemyStarBase  = NULL;
   }

   if (g_levelContent.m_penemyKingsplane == this)
   {
      g_levelContent.m_penemyKingsplane  = NULL;
   }

   if (g_levelContent.m_penemyHangar == this)
   {
      g_levelContent.m_penemyHangar  = NULL;
   }

   if (getEnemyTarget() == this)
   {
      setEnemyTarget(NULL);
      g_bTargetLocked = false;
   }

   if (g_penemyNextTriggered == this)
   {
      g_penemyNextTriggered = NULL;
   }

   if (g_penemyNextUntriggered == this)
   {
      g_penemyNextUntriggered = NULL;
   }

   if (g_penemyCenterFrame == this)
   {
      g_penemyCenterFrame = NULL;
      g_bEnemyCenterFrameValid = false;
   }

   if (g_Player.m_pEnemy == this)
   {
      g_Player.m_pEnemy = NULL;
      //g_Player.m_bDetonationValid = false;
   }

   if (g_options.IsEnableSound())
   {
      for (l_idxSndRes = 0; l_idxSndRes < SNDRES_LASTENUM; l_idxSndRes++)
      {
         if (g_sound_array[l_idxSndRes].m_pEnemy == this) 
         {
            if (g_sound_array[l_idxSndRes].stopLooping() == false) return false;
            g_sound_array[l_idxSndRes].m_pEnemy = NULL;
         }
      }
   }
   switch(mType)
   {
      case ETYP_GHOST:
         g_cntGhost--;
         break;

      case ETYP_TUNNELFLYER:
         g_cntTunnelFlyersDestroyed++;
         break;
   //         if (g_cntTunnelFlyersDestroyed == g_cntTunnelFlyersToDestroy)
   //            g_cntShowPlasmaTimer     = g_maxShowPlasmaTimer;
   //            g_cntJumpWitchSpace = g_maxTransferToNextStarBase;
   //            g_bGoalReached[MID_MISSION03] = true;
   //            g_iLastStarBase=3;
   //            g_iCurrStarBase=4;
   //            g_bMissionGoalThree=true;        
      case ETYP_FLYER:
         m_cntLeadersDestroyed++;

         break;

      case ETYP_LEADER:
         if (isTriggered())
         {
            m_cntCurrentlyAttackingLeaders = m_cntCurrentlyAttackingLeaders - 1;
         }
         m_cntLeadersDestroyed++;

         break;
      case ETYP_DOOR:
         g_cntDoorsDestroyed++;

         g_vAutopilotDest.x = g_vDoorArray[g_cntDoorsDestroyed].x + g_levelContent.m_vPos.x;
         g_vAutopilotDest.y = g_vDoorArray[g_cntDoorsDestroyed].y + g_levelContent.m_vPos.y;
         g_vAutopilotDest.z = g_vDoorArray[g_cntDoorsDestroyed].z + g_levelContent.m_vPos.z;
         switch(g_cntDoorsDestroyed)
         {
            case 1: // erster Tunnel wird betreten
               // --- Tunnel mit Enemys fuellen
               //
               g_pCurrLeader = this;
               if (PSXCalibrateEnemyArray(LID_TUNNEL01)==false)
               {
                  return;
               };
               break;

            case 3: // BattleField ueberlebt!

               // --- Tunnel mit Enemys fuellen
               //
               g_pCurrLeader = this;
               if (PSXCalibrateEnemyArray(LID_TUNNEL02)==false)
               {
                  return;
               };
               break;

            case 5: // BattleField erneut ueberlebt! Nun Reaktor

               // --- Tunnel mit Enemys fuellen
               //
               g_pCurrLeader = this;
               if (PSXCalibrateEnemyArray(LID_TUNNEL03)==false)
               {
                  return;
               };

               if (PSXCalibrateEnemyArray(LID_INDOOR)==false)
               {
                  return;
               };
 
               break;
         }
         break;
      case ETYP_MINE:
      
         // the seccond starbase depends on it.

         if (isTriggered())
         {
            m_cntCurrentlyAttackingMines = m_cntCurrentlyAttackingMines - 1;
         }
         break;  
      case ETYP_PIRATE:
         if (isTriggered())
         {
            m_cntCurrentlyAttackingPirates = m_cntCurrentlyAttackingPirates - 1;
         }
         break;  

      case ETYP_HUNTER:
         if (isAnyCharacter())
         {
            (getCharacter())->m_bHatePlayer = false;
         }
         break;  

      case ETYP_THARGOID:
         m_cntThargoid--;
         break;  

      case ETYP_THARGOIDDRONE:
         m_cntThargoidDrone--;
         break;  
      case ETYP_BRENNSTAB:
      case ETYP_FELDGENERATOR:
                 
         // -- Explosion verschieben
         //
         m_vPos.x  = m_vPos.x + m_Mat._21 * m_fRad/2.0f; 
         m_vPos.y  = m_vPos.y + m_Mat._22 * m_fRad/2.0f;
         m_vPos.z  = m_vPos.z + m_Mat._23 * m_fRad/2.0f;

         g_cntDestroyedGenerators++;
         break;  

   } // end-switch
   */
   m_pBigResItem = NULL;
   mType = ETYP_EMPTY;
   
   WriteLogFile("Done:");
   WriteLogFile(g_cEnemyTypeNameArray[mType]);
   
   WriteLogFile("::Destroy()\n");

   // -- nachdem nun dieses Level aus der Verkettung raus ist,
   //    auch die Mapobjekt verkettung auflösen
   //
   return MAPOBJECT::Destroy(l_lifecycle);

}
// -- hier entscheidet sich der Enemy fuer eine bestimmte EDRAW Schublade
//    ausgabe: idxEnemyDrawer
//

bool ENEMY::DecideForEnemyDrawer(IDXENEMYDRAWER* pidxEnemyDrawer, IDXENEMYDRAWER* pidxEnemyDrawer2)
{
   *pidxEnemyDrawer2 = EDRAW_EMPTY; 
/*
   if (isVisible() == false)
   {
      *pidxEnemyDrawer = EDRAW_EMPTY; 
      return true;
   }
*/
   switch(mType)
   {
	   /*
      case ETYP_ASTEROID:         

         if (g_bEnableBumpMapping && g_bEnableVertexShader && m_vPos2D.z < 2000.0f)
         {
            *pidxEnemyDrawer = EDRAW_ASTEROID_BUMP; 
         }else{
            *pidxEnemyDrawer = EDRAW_ASTEROID_EMISSIVE; 
         }

         break;

      case ETYP_MINE:             
         if (m_fRad2D > 0.03f)
         {
            if (g_CntMineDetonatorBlink < 25)
            {
               *pidxEnemyDrawer = EDRAW_MINE_EMISSIVE; 

            }else{
               *pidxEnemyDrawer = EDRAW_MINE; 
            }
         }else{
            if (m_fRad2D > 0.02f * m_fExplScale )
            {
               *pidxEnemyDrawer = EDRAW_MINE_LOWPOLY;
            }else{
               *pidxEnemyDrawer = EDRAW_MINE_LOWPOLY2;
            }
         }

         if (m_fRad2D * g_fMineShieldRadScale > m_fMinRad2DForVisibility)
         {
            if (m_cntShieldFlashTimer)
            {
               *pidxEnemyDrawer2 = EDRAW_SHIELDFLASH;
            }else{
               if (m_Action != ENACT_ESCAPE)
               {
                  *pidxEnemyDrawer2 = EDRAW_TRACTORSPHERE;
               }
            }
         }

         break;

      case ETYP_EQUIPMENT:
         *pidxEnemyDrawer = EDRAW_EQUIPMENT;
         if (m_cntShieldFlashTimer)
         {
            *pidxEnemyDrawer2 = EDRAW_SHIELDFLASH;
         }else{
            *pidxEnemyDrawer2 = EDRAW_POINTER01;
         }
         break;

	 */
      case ETYP_EMPTY:           
         *pidxEnemyDrawer = EDRAW_EMPTY; 
         break;

      case ETYP_EXPLOSION:           
         //if (m_fRad2D > 0.5f)
         //{
         //   *pidxEnemyDrawer  = EDRAW_HOTPLASMA; 
         //}else{
         *pidxEnemyDrawer  = EDRAW_EXPLOSION; 
         WriteLogFile("ENEMY::DecideForEnemyDrawer(ETYP_EXPLOSION)\n");
         //}
         break;

	 /*

      case ETYP_TMPSPEEDUP:
         *pidxEnemyDrawer  = EDRAW_TMPSPEEDUP; 
         break;
      
      case ETYP_CHAR:
         *pidxEnemyDrawer  = EDRAW_CHAR; 
         if (m_cntShieldFlashTimer)
         {
            *pidxEnemyDrawer2 = EDRAW_SHIELDFLASH;
         }
         break;
      case ETYP_GHOST:           
         *pidxEnemyDrawer  = EDRAW_GHOST; 
         break;

      case ETYP_IMPACT:           
         *pidxEnemyDrawer = EDRAW_IMPACT; 
         break;

      case ETYP_KINGSPLANE:
         *pidxEnemyDrawer = EDRAW_EMPTY; 
         break;
      case ETYP_CARGOBOX:
      case ETYP_BIGRESITEM:
         if (g_Player.getGS()->IsHavingOwnShip() || m_pLeader->mType != ETYP_PLAYER)
         {
            if (g_edrawResEmissiveArray[m_ResType].m_bEmissive)
            {
               *pidxEnemyDrawer = EDRAW_RES_EMISSIVE; 
            }

            if (g_edrawResBumpArray[m_ResType].m_bBump && 
                g_hDot3BumpPixelShader && g_hBumpNoEmPixelShader )
            {
               *pidxEnemyDrawer = EDRAW_RES_BUMP; 
            }

            if (m_cntShieldFlashTimer)
            {
               *pidxEnemyDrawer2 = EDRAW_SHIELDFLASH;
            }

         }else{
            *pidxEnemyDrawer = EDRAW_EMPTY; 
         }
         break;

      case ETYP_PLANET:
         
         *pidxEnemyDrawer = EDRAW_PLANET; 
         break;

      case ETYP_PLAYER:
         if (g_Player.getGS()->IsHavingOwnShip())
         {
            if (g_edrawResEmissiveArray[m_ResType].m_bEmissive)
            {
               *pidxEnemyDrawer = EDRAW_RES_EMISSIVE; 
            }

            if (g_edrawResBumpArray[m_ResType].m_bBump && 
                g_hDot3BumpPixelShader && g_hBumpNoEmPixelShader )
            {
               *pidxEnemyDrawer = EDRAW_RES_BUMP; 
            }

            if (m_cntShieldFlashTimer)
            {
               *pidxEnemyDrawer2 = EDRAW_SHIELDFLASH;
            }

         }else{
            *pidxEnemyDrawer = EDRAW_EMPTY; 
         }
         break;
	 */
         
      default: 
		 /*
         if (isBigResRepresented() == false)// && g_Player.m_idxCondition != COID_DOCKED)
         {
            *pidxEnemyDrawer = EDRAW_RES_STANDARD; 
   
            //if (m_fRad2D > 0.05f)
            //{
            if (g_edrawResEmissiveArray[m_ResType].m_bEmissive)
            {
               *pidxEnemyDrawer = EDRAW_RES_EMISSIVE; 
            }

            if (g_edrawResBumpArray[m_ResType].m_bBump && 
                g_hDot3BumpPixelShader && g_hBumpNoEmPixelShader )
            {
               *pidxEnemyDrawer = EDRAW_RES_BUMP; 
            }
  
            if (g_bEnemyDebugValid && g_penemyDebug == this)
            {
               *pidxEnemyDrawer2 = EDRAW_POINTER01;
            }
         }else{
            *pidxEnemyDrawer = EDRAW_EMPTY; 
         }

*/
         if (m_cntShieldFlashTimer)
         {
            *pidxEnemyDrawer2 = EDRAW_SHIELDFLASH;
         }
	
	if ((m_ResType < RESTYPE_LASTENUM) && (m_ResType >= 0))	
	{
		*pidxEnemyDrawer = g_txtassignResStdArray[m_ResType].idxEnemyDrawer;		
	}else{
		*pidxEnemyDrawer = EDRAW_RES_STANDARD; 
		
	}
   }
   return true;
}



/*
      case ETYP_FELDGENERATOR:           
         if(g_bGeneratorView)
         {
            *pidxEnemyDrawer = EDRAW_FELDGENERATOR_IR; 
         }else{
            *pidxEnemyDrawer = EDRAW_FELDGENERATOR; 
         }
         break;
*/
/*
      case ETYP_FLYER:           
         if (m_bGreenGroup)
         {
            *pidxEnemyDrawer = EDRAW_FLYER_GREEN; 
            if (m_fRad2D > 0.15f)
            {
               *pidxEnemyDrawer = EDRAW_FLYER_GREEN_EMISSIVE; 
            }
         }else{
            *pidxEnemyDrawer = EDRAW_FLYER_RED; 
            if (m_fRad2D > 0.15f)
            {
               *pidxEnemyDrawer = EDRAW_FLYER_RED_EMISSIVE; 
            }
         }
   
         if (m_cntShieldFlashTimer)
         {
            *pidxEnemyDrawer2 = EDRAW_SHIELDFLASH;
         }else{
            if (m_cntShieldAmount)
            {
               *pidxEnemyDrawer2 = EDRAW_FRONTSHIELD;
            }
         }

         break;
      case ETYP_LEADER:           
         if (m_bGreenGroup)
         {
            *pidxEnemyDrawer = EDRAW_LEADER_GREEN; 
            if (m_fRad2D > 0.15f)
            {
               *pidxEnemyDrawer = EDRAW_LEADER_GREEN_EMISSIVE; 
            }
         }else{
            *pidxEnemyDrawer = EDRAW_LEADER_RED; 
            if (m_fRad2D > 0.15f)
            {
               *pidxEnemyDrawer = EDRAW_LEADER_RED_EMISSIVE; 
            }
         }

         if (m_cntShieldFlashTimer)
         {
            *pidxEnemyDrawer2 = EDRAW_SHIELDFLASH;
         }else{
            if (m_cntShieldAmount==7)
            {
               *pidxEnemyDrawer2 = EDRAW_FRONTSHIELD;
            }
         }

         break;
      case ETYP_TUNNELFLYER:           
         *pidxEnemyDrawer = EDRAW_FLYER_RED; 
         if (m_fRad2D > 0.15f)
         {
            *pidxEnemyDrawer = EDRAW_FLYER_RED_EMISSIVE; 
         }
         break;

      case ETYP_HANGAR:           
         *pidxEnemyDrawer = EDRAW_HANGAR_EMISSIVE; 

         if ((g_CntMineDetonatorBlink > 25) &&
             (isTriggered()))
         {
            *pidxEnemyDrawer = EDRAW_HANGAR;
         }

         break;
      case ETYP_GUN:           
         *pidxEnemyDrawer = EDRAW_GUN; 
         break;

      case ETYP_REACTORGUN:           
         *pidxEnemyDrawer = EDRAW_GUN; 
         break;

      case ETYP_DOOR:           
         switch( m_cntRanking)
         {
            case 0: *pidxEnemyDrawer = EDRAW_DOOR01B; 
                    break;
            case 1: *pidxEnemyDrawer = EDRAW_DOOR02B; 
                    break;
            case 2: *pidxEnemyDrawer = EDRAW_DOOR01C; 
                    break;
            case 3: *pidxEnemyDrawer = EDRAW_DOOR02A; 
                    break;
            case 4: *pidxEnemyDrawer = EDRAW_DOOR01A; 
                    break;
            default: *pidxEnemyDrawer = EDRAW_EMPTY; 
         }
         break;
*/
// --- stoesst im ausgeloesten Zustand ein Geschwader aus
//
/*
bool ENEMY::TreatAsHangar()
{
   bool l_bNoAttack;

   // --- Angriffsstatus fuer Sicherheitskraefte und Starbase Verteidung
   //
   if (g_Player.m_bEntered || 
       g_Player.m_crimePoliceFile == CRID_CLEAN) // artige bürger nicht angreifen
   {
      l_bNoAttack = true;
   }else{
      l_bNoAttack = false;
   }

   // -- hangare bleiben passiv, bis eine gewisse naehe erreicht ist
   //
   if (isTriggered())
   {
      if (m_cntCurrentlyAttackingLeaders < g_maxCurrentlyAttackingLeaders)
      { 
         if ( l_bNoAttack || (m_cntRanking >= 10) )
         { 
            setTriggered(false);
         }
      }else{
         if ( l_bNoAttack || (m_cntRanking >= 5) )
         { 
            setTriggered(false);
         }
      }
   }else{
      if (m_cntCurrentlyAttackingLeaders < g_maxCurrentlyAttackingLeaders)
      { 
         if ((m_fDestLength < (m_fCollisionRadPow2*100.0f)) &&
             (l_bNoAttack == false) &&
             (m_cntRanking <= 10))
         { 
            float l_fPlayer;
            l_fPlayer = m_Mat._31*(g_Player.m_pEnemy->getPosX()-m_vPos.x) + 
                        m_Mat._32*(g_Player.m_pEnemy->getPosY()-m_vPos.y) + 
                        m_Mat._33*(g_Player.m_pEnemy->getPosZ()-m_vPos.z); 

            if (l_fPlayer > 0)
            {
               setTriggered(true);
               m_iActionTimer=150.0f;
               // Verzoegerung zum Abballern vorm Ausschwaermen
            }
         }
      }
   }
   
   if (isTriggered() == true) 
   {

      if (m_Action == ENACT_ESCAPE)
      { 
         //m_fViewRotation1 = 0.25f;
         m_fViewRotation1 = 0.008f;
         m_fViewRotation2 = 0.01f;
         RotationToMatrix(); m_fViewRotation1 = 0.0f; m_fViewRotation2 = 0.0f;
      }

      if (m_iActionTimer>0)
      { 
         // --- laufende phase abwarten
         //
         m_iActionTimer = m_iActionTimer - 1;

      }else{
         ENEMY* l_pNewEnemy;

         l_pNewEnemy = new (LIFECYCLE_LEVEL) ENEMY(LIFECYCLE_LEVEL);
         if (l_pNewEnemy != NULL)
         {
            //l_pNewEnemy->Init();
            if (m_cntRanking == 0 || m_cntRanking == 5)
            {
               l_pNewEnemy->SetType(ETYP_LEADER);
               m_pLeader = l_pNewEnemy;
               m_cntCurrentlyAttackingLeaders = m_cntCurrentlyAttackingLeaders + 1;
            }else{
               l_pNewEnemy->SetType(ETYP_FLYER);
               l_pNewEnemy->m_pLeader = m_pLeader;
            }
 
            if (m_cntRanking>4) 
            {
               l_pNewEnemy->m_cntRanking = m_cntRanking-5;
            }else{
               l_pNewEnemy->m_cntRanking = m_cntRanking;
            }

            //l_pNewEnemy->m_Mat = m_Mat;

            memcpy (&g_matGeneric1,&g_matIdentity, sizeof(g_matIdentity));
            g_matGeneric1._22 =   cos (-g_fPi*0.5f);
            g_matGeneric1._23 = - sin (-g_fPi*0.5f);
            g_matGeneric1._32 =   sin (-g_fPi*0.5f);
            g_matGeneric1._33 =   cos (-g_fPi*0.5f);
           
            matmult(&(l_pNewEnemy->m_Mat), &g_matGeneric1, &m_Mat);

            l_pNewEnemy->setTriggered(true);

            m_cntRanking++;

            if (m_cntRanking==5)
            {
               //m_cntRanking   = 0;
               m_iActionTimer=50.0f;
            }else{
               m_iActionTimer=10.0f;
            }

            //l_pNewEnemy->SetFlyerStep();
            if (l_pNewEnemy->setPos(m_vPos)==false) return false; //+ l_pNewEnemy->m_vStep.x * 10.0f;
         } 
      }
   }
   return true;
}
*/
// -- Pflegt die Koordinaten des reinen Praesentations Unterobjektes nach
//

bool ENEMY::TreatAsBigResItem()
{

   if (m_pBigResItem && m_pLeader && m_dBodyID)
   {
   }else{
      hr2message(__LINE__,-1,"TreatAsBigResItem: m_pBigResItem or m_pLeader is NULL");
      return false;
   }
   return true;
}

// -- prüft ob der Schuss diesen Enemy getroffen hat
//
bool ENEMY::checkIfHitByShoot(int l_idxShoot, float l_fLenPow2)
{
   XMVECTOR l_vEnemy, l_vLengthPow2;
   XMVECTOR l_vShootStepLength;
   float l_fLength;
   float x,y,z, l_fLengthPow2;
   float l_fShootStepLength;
   bool l_bHit;

   l_vEnemy = m_vPos - shoot_array[l_idxShoot].m_vPos;

   l_vLengthPow2 = XMVector3LengthSq(l_vEnemy);
   l_fLengthPow2 = XMVectorGetX(l_vLengthPow2);

   l_vShootStepLength = XMVector3LengthEst(shoot_array[l_idxShoot].m_vStep);
   l_fShootStepLength = XMVectorGetX(l_vShootStepLength);

   l_bHit = false;

   if ( isBigResRepresented() == false &&
        mType != ETYP_EXPLOSION &&
        mType != ETYP_EMPTY &&
        mType != ETYP_IMPACT)
   {
      if (l_fLengthPow2 < pow2(m_fRad + l_fShootStepLength))
      {
         //PSXFACEINFO** l_pfaceinfSelected;
   
         // -- Der Ergebnispuffer des zweiten Select in FaceIntersection() 
         //    darf erst hinter der Liste des ersten Selects anfangen
         //
         //l_pfaceinfSelected = (PSXFACEINFO**) (g_arr_pEnemy + l_cntEnSel);
   
         // --- kollision muss nach faces differenziert werden
         //
         if (false) //(g_meshresManager.getFaceInfArray(m_ResType)) // && isCheckFaceIntersection())
         // >> if (g_meshresManager.getFaceInfArray(m_ResType)) // && isCheckFaceIntersection())
         {
            XMVECTOR l_vPos, l_vStep;
            XMMATRIX l_matView;
            //XMMATRIX* l_pMat;
   
            float  l_fIntersectAt;
            bool l_bIntersectValid;
            PSXFACEINFO* l_pfaceinfIntersect;
   
            //l_pMat = &(m_Mat);
   
            // -- Schussposition/Richtungsvektor errechnen, die sich im unskalierten 
            //    Verhaeltnis relativ zum Enemy befinden 
            //
            { 
               XMVECTOR l_vDirection;
            
               float x,y,z;
	       XMVECTOR l_vShoot = shoot_array[l_idxShoot].m_vPos - m_vPos;
      
               XMMATRIX l_matEnemyTrp = XMMatrixTranspose(m_Mat);
	       l_vPos = XMVector3Transform(l_vShoot, l_matEnemyTrp);
               //l_vPos.x = (l_pMat->_11*x + l_pMat->_12*y + l_pMat->_13*z);
               //l_vPos.y = (l_pMat->_21*x + l_pMat->_22*y + l_pMat->_23*z);
               //l_vPos.z = (l_pMat->_31*x + l_pMat->_32*y + l_pMat->_33*z);
   
               //x = shoot_array[l_idxShoot].m_vStep.x;// / m_fScale;
   
               //l_fLenPow2 = pow2(x)+ pow2(y)+ pow2(z); =>
               //l_fLength = sqrt(l_fLenPow2);           => l_fShootStepLength 
   
	       l_vStep =  XMVector3Transform( shoot_array[l_idxShoot].m_vStep, l_matEnemyTrp);
               //l_vStep.x = (l_pMat->_11*x + l_pMat->_12*y + l_pMat->_13*z);
               //l_vStep.y = (l_pMat->_21*x + l_pMat->_22*y + l_pMat->_23*z);
               //l_vStep.z = (l_pMat->_31*x + l_pMat->_32*y + l_pMat->_33*z);
   
               l_vDirection = l_vStep / l_fShootStepLength;
      
               if (PSXMatrixFromVector( &l_matView, l_vDirection) == false) 
               {
                  return false;
               }
              
            }
   
            if (PSXFaceIntersection(l_vPos,
                                    l_vStep,
                                    l_fLenPow2,
                                    l_matView,
                                    m_ResType,
                                    g_meshresManager.getFaceInfArray(m_ResType),
                                    g_pfaceinfSelectedArray, //Arbeitsbereich
                                    &l_bIntersectValid, 
                                    &l_fIntersectAt,
                                    &l_pfaceinfIntersect ) == false) return false;
      
            if (l_fIntersectAt<= (g_fElapsedTime) && l_fIntersectAt>=0.0f && l_bIntersectValid)
            {
               XMVECTORF32 l_vFaceNormal = { l_pfaceinfIntersect->vNormal.x,
                                            l_pfaceinfIntersect->vNormal.y,
                                            l_pfaceinfIntersect->vNormal.z };
		  
               XMVECTOR l_vNormal, l_vPos;
             
               //x = l_pfaceinfIntersect->vNormal.x;
               //y = l_pfaceinfIntersect->vNormal.y;
               //z = l_pfaceinfIntersect->vNormal.z;
      
               l_vNormal = XMVector3Transform(l_vFaceNormal, m_Mat);
               //l_vNormal.x = (l_pMat->_11*x + l_pMat->_21*y + l_pMat->_31*z);
               //l_vNormal.y = (l_pMat->_12*x + l_pMat->_22*y + l_pMat->_32*z);
               //l_vNormal.z = (l_pMat->_13*x + l_pMat->_23*y + l_pMat->_33*z);

               l_vPos = shoot_array[l_idxShoot].m_vPos + l_fIntersectAt * shoot_array[l_idxShoot].m_vStep;
               //l_vPos.x = shoot_array[l_idxShoot].m_vPos.x + l_fIntersectAt * shoot_array[l_idxShoot].m_vStep.x;
               //l_vPos.y = shoot_array[l_idxShoot].m_vPos.y + l_fIntersectAt * shoot_array[l_idxShoot].m_vStep.y;
               //l_vPos.z = shoot_array[l_idxShoot].m_vPos.z + l_fIntersectAt * shoot_array[l_idxShoot].m_vStep.z;
               HitByShoot(l_idxShoot, &l_vNormal, &l_vPos);
               l_bHit = true;
            }
         }else{
      
            // -- default: kollisionsbehandlung wird durchgefuehrt ohne die 
            //    faces im einelnen zu pruefen
            //
            HitByShoot(l_idxShoot, NULL);
            l_bHit = true;
         } // end-if m_faceinfArray gegeben
      } // end-if innerhalb kollisionsradius
    
   }// end-if überhaupt für kollision infrage kommend
    
   // -- nicht getroffen -> flugrichtung anpassen
   //
   if (l_bHit == false && 
       (shoot_array[l_idxShoot].menacetype & MENACE_ENEMY) &&
       isShootMagnetical() &&
       l_fLengthPow2 < (m_fShootHitRadPow2 * 4.0f))
   {
      //l_fSpeed = pow2(shoot_array[l_idxShoot].m_vStep.x )+
      //           pow2(shoot_array[l_idxShoot].m_vStep.y )+
      //           pow2(shoot_array[l_idxShoot].m_vStep.z );
      //l_fSpeed = sqrt(l_fSpeed);

      //l_fLength = sqrt(l_fLengthPow2);
      XMVECTOR l_vLength = XMVector3LengthEst(l_vEnemy);
      float l_fLength = XMVectorGetX(l_vLength);
      shoot_array[l_idxShoot].m_vStep = l_vEnemy * l_fShootStepLength / l_fLength;
      //shoot_array[l_idxShoot].m_vStep.x = l_vEnemy.x * l_fShootStepLength / l_fLength;
      //shoot_array[l_idxShoot].m_vStep.y = l_vEnemy.y * l_fShootStepLength / l_fLength;
      //shoot_array[l_idxShoot].m_vStep.z = l_vEnemy.z * l_fShootStepLength / l_fLength;
   }

   return true;
}

// -- Wrapper, für Aufruf ohne dedizierte Position
// 
bool ENEMY::HitByShoot(int l_idxShoot, XMVECTOR* l_pvNormal)
{
   return HitByShoot(l_idxShoot, l_pvNormal, &shoot_array[l_idxShoot].m_vPos);
}
// -- Kollision Schuesse/Gegner => Punktegewinn/Gameplay
//    prueft ob Enemy von Schuessen getroffen worden ist und behandelt die Kollision
// 
bool ENEMY::HitByShoot(int l_idxShoot, XMVECTOR* l_pvNormal, XMVECTOR* l_pvPos)
{
   float dx,dy,dz, l_fDistPow2;
   ENEMY* l_pNewEnemy;
   SHOOT* l_pshootCurr;
   bool l_bRelevant;

   if (g_bWriteLogFileInLoop)
   {
      WriteLogFile("ENEMY::HitByShoot(");
      WriteLogFile(g_cEnemyTypeNameArray[mType]);
      WriteLogFile("\n");
   }

   // -- Eine Explosion kann man nicht erschiessen und ein leeres Objekt genauso wenig
   //
   if (mType == ETYP_EXPLOSION ||
       mType == ETYP_EMPTY || 
       mType == ETYP_IMPACT)
   {
      return true;
   }
   
   if (m_pLeader && isPartOfLeader())
   {
      return m_pLeader->HitByShoot(l_idxShoot,l_pvNormal);
   }

   if (isPlayer())
   {
      l_bRelevant = bool(shoot_array[l_idxShoot].menacetype & MENACE_SELF);
   }else{
      l_bRelevant = bool(shoot_array[l_idxShoot].menacetype & MENACE_ENEMY);
   }

   l_pshootCurr = shoot_array+l_idxShoot;

   if (l_pshootCurr->m_cntTimeToLive > 0 && l_bRelevant)
   {

      // -- nochmal gegenchecken, ob die kollision wirklich infrage kommt
      //
		XMVECTOR l_vShoot = l_pshootCurr->m_vPos - m_vPos;
      //dx = l_pshootCurr->m_vPos.x - m_vPos.x;
      //dy = l_pshootCurr->m_vPos.y - m_vPos.y;
      //dz = l_pshootCurr->m_vPos.z - m_vPos.z;
   
      //l_fDistPow2 = dx*dx + dy*dy + dz*dz;
      XMVECTOR l_vDistPow2 = XMVector3LengthSq(l_vShoot);
   	l_fDistPow2 = XMVectorGetX(l_vDistPow2);
   	//XMVector3LengthEst(


      if (l_fDistPow2 < m_fShootHitRadPow2)
      {
         float xDest=0.0f;

         // -- Shoot als Drehmoment auf den Körper anwenden
         //
         if (m_dBodyID && l_pvPos)
         {
            float l_fForce;
	    XMFLOAT3 l_f3ShootPos, l_f3ShootStep;

            if (l_pshootCurr->m_pEquipment)
            {
               l_fForce = l_pshootCurr->m_pEquipment->m_fShieldDamage * 1000.0f;
            }else{
               l_fForce = 100.0f;
            }
            //l_fDistance = sqrt(l_fDistPow2);

	    XMStoreFloat3(&l_f3ShootStep, l_pshootCurr->m_vStep * l_fForce);
	    XMStoreFloat3(&l_f3ShootPos, l_pshootCurr->m_vPos);

            dBodyAddForceAtPos(m_dBodyID, 
                           //l_pshootCurr->m_vStep.x * l_fForce,
                           //l_pshootCurr->m_vStep.y * l_fForce,
                           //l_pshootCurr->m_vStep.z * l_fForce,
			   l_f3ShootStep.x,
			   l_f3ShootStep.y,
			   l_f3ShootStep.z,
                           //l_pvPos->x,
                           //l_pvPos->y,
                           //l_pvPos->z);
			   l_f3ShootPos.x,
			   l_f3ShootPos.y,
			   l_f3ShootPos.z);
         }

 
         if (isPlayer())
         {
             g_Player.m_fShieldAmount = g_Player.m_fShieldAmount - 1.0f;

             //g_Player.m_fRumbleScale = 0.05f;
             //g_Player.m_bShieldVisible = true;
             m_cntShieldFlashTimer = 1;

             // --- players damage sound 
             //
	     /*
             if (g_options.IsEnableSound())
             {
                if (g_sound_array[SNDRES_PLAYERHIT].play() == false) return false;
             }

             g_bRearViewEnabled = true;
	     */
         }

         // -- Polizistenmord bzw. Zerstörung von Regierungseigentum,
         //    Tötung unschuldiger ruft die Sicherheitskraefte auf den Plan
         //
	 /*
         if (mType == ETYP_LEADER || 
             mType == ETYP_FLYER || 
             mType == ETYP_TRADER || 
             mType == ETYP_STARBASE || 
             mType == ETYP_HANGAR)
         { 
                    
            if ( g_Player.m_crimePoliceFile == CRID_CLEAN )
            {
               g_Player.setPoliceFile(CRID_OFFENDER);
            }

            // -- Der Enemy wird nun dem Policecharacter untergeordnet
            //    und dann wird dieser gerufen, um zur neuen Situation
            //    zu entscheiden
            //
            if (CHARACTER::IsAnyPolice())
            {
               if (setCharacterToPolice() == false) return false;
               (getCharacter())->m_bHatePlayer   = true;
               (getCharacter())->m_bAmILame      = false;
               (getCharacter())->m_iAction = CHRACT_ATTACKPLAYER;
               (getCharacter())->m_bUnspokenAction = true;
            }else{
               hr2message(__LINE__,-1,"Police-Character is not defined, but needed in ENEMY::HitByShot()\n");
               return false;
            }
         }
	 */

         // -- Abbau von Erz
         //
	 /*
         if (mType == ETYP_ASTEROID && 
             l_pshootCurr->m_pEquipment == g_equipmentArray+EQUIPMENT_MININGLASER)
         {
            int l_cntSpace;

            if (PSXCheckSpaceInCargoBay(GOOID_ORE, &l_cntSpace) == false)
            {
               return false;
            }
             
            if ( l_cntSpace > 0)
            {
               g_iShipsHoldArray[GOOID_ORE]++;
               PSXInitializeTerminal(g_cLang[txxOreLoadedInCargoBay], 
                                     NULL, NULL, NULL);
            }else{
               PSXInitializeTerminal(g_cLang[txxMaximumCapacityReached], 
                                     NULL, NULL, NULL);
            }
         }
	 */


         if (isAffectableByShoot())
         {
            // -- Angeschossen werden allgemeingueltig behandelt
            //

            if ((l_pshootCurr->menacetype & MENACE_ENEMY))
            {
		         /*
               if (isAnyCharacter())
               {
                  if((getCharacter())->m_iAction!=CHRACT_ATTACKPLAYER)
                  {
                     (getCharacter())->m_iAction = CHRACT_ATTACKPLAYER;
                     (getCharacter())->m_bUnspokenAction = true;
                     (getCharacter())->m_bHatePlayer   = true;
                  }
               }
               */
               // -- Auswirkung der Waffenstärke
               //
               
               if (l_pshootCurr->m_pEquipment)
               {
                  m_fShieldAmount = m_fShieldAmount - l_pshootCurr->m_pEquipment->m_fShieldDamage;
               }

               // -- ist der Schutzschild noch wirksam
               //
               if (m_fShieldAmount>0.2f)
               {
                  
                  // -- direkte Verhaltens-Reaktion auf Beschuss
                  //
                  if (isBreakingOut())
                  {
                     switch (m_Action)
                     {
                        case ENACT_ESCAPE:
                           m_Action = ENACT_BREAKOUT_UP;
                           break;
                        case ENACT_BREAKOUT_UP:
                           m_Action = ENACT_BREAKOUT_DOWN;
                           break;
                        case ENACT_BREAKOUT_DOWN:
                           m_Action = ENACT_BREAKOUT_UP;
                           break;
                     }
                  }

                  // -- was passiert mit nun unnuetzen Shoot?
                  //
                  if (g_equipmentArray[EQUIPMENT_MISSILE].m_bSingleton &&
                      g_equipmentArray[EQUIPMENT_MISSILE].m_idxShootSingleton == l_idxShoot)
                  {
                     l_pshootCurr->insertImpact();
                     l_pshootCurr->Die();
                  }else{

                     if (g_equipmentArray[EQUIPMENT_MINE].m_bSingleton &&
                         g_equipmentArray[EQUIPMENT_MINE].m_idxShootSingleton == l_idxShoot)
                     {
                        l_pshootCurr->insertImpact();
                        l_pshootCurr->Die();
                     }else{
                        ShotAgainstShield(l_idxShoot, l_pvNormal, l_pvPos);
                     }
                  }

                  
               }else{ //if (m_fShieldAmount>0.2f)

                  // -- noch ist unwirksamer restschild da, keine Zerstörung
                  //
                  if(m_fShieldAmount > 0.0f)
                  {
                     /*
                     if (l_pvPos)
                     {
                        SmokeTrayActivate(this, *l_pvPos);
                     }else{
                        SmokeTrayActivate(this, l_pshootCurr->m_vPos);
                     }

                     // -- Nebst Rauchspur auch CargoBox aussetzen, wenn vorhanden
                     //
                     if(isCargoBoxOnBoard())
                     { 
                        ENEMY* l_pEnemy;
                        l_pEnemy = new (LIFECYCLE_LEVEL) ENEMY(LIFECYCLE_LEVEL);
                        if (l_pEnemy == NULL) return false;
                    
                        setCargoBoxOnBoard(false);
         
                        //l_pEnemy->Init();
                        l_pEnemy->SetType(ETYP_CARGOBOX);
                        l_pEnemy->createBody();
                        l_pEnemy->setRes(RESTYPE_CARGOBOX);
                        l_pEnemy->m_vPos       = m_vPos;
                        l_pEnemy->m_Mat        = m_Mat;
               
                        l_pEnemy->m_vPos.x = m_vPos.x - m_fRad * m_Mat._31;
                        l_pEnemy->m_vPos.y = m_vPos.y - m_fRad * m_Mat._32;
                        l_pEnemy->m_vPos.z = m_vPos.z - m_fRad * m_Mat._33;
            
                        l_pEnemy->m_seedShipsHold = m_seedShipsHold;
                        l_pEnemy->setRadarable(true);
            
                        //m_Action = ENACT_ESCAPE; // entscheidung forcieren
                     }

							*/
                     // -- Unterhalb einer Grenze schaltet er auf Bankrotation
                     //
                     if (m_fShieldAmount < 0.1f)
                     {
                        ENEMY* l_penemyChild;

                        //if (mType == ETYP_HANGAR)
                        //{
                        //   // --- verpuffung des ersten schusses
                        //   //
                        //   if (m_Action != ENACT_ESCAPE)
                        //   {
                        //      l_pNewEnemy = new (LIFECYCLE_LEVEL) EXPLOSION(LIFECYCLE_LEVEL);
                        //      if (l_pNewEnemy != NULL)
                        //      { 
                        //         //l_pNewEnemy->Init();
                        //         
                        //         l_pNewEnemy->SetType(ETYP_EXPLOSION);
                        //         if (l_pNewEnemy->setPos(m_vPos)==false) return false;
                        //         if (l_pNewEnemy->setStep(0.0f, 0.0f, 0.0f)==false) return false;
                        //         l_pNewEnemy->setVisible(true);
                        //         l_pNewEnemy->setTriggered(true);
                        //         l_pNewEnemy->m_fExplScale = m_fExplScale;
                        //         l_pNewEnemy->m_fRad = m_fRad;
                        //      }
                        //      m_Action = ENACT_ESCAPE;
         
                        //      // -- Bestimmung der Escape-Flugrichtung und Geschwindigkeit
                        //      //
                        //      m_fSpeed  = 4.0f;
                        //      if (setStep(m_Mat._31 * m_fSpeed,
                        //                  m_Mat._32 * m_fSpeed,
                        //                  m_Mat._33 * m_fSpeed)==false) return false;
                        //   }
                        //}else{
   
                        // -- Handlungsunfähigkeit demonstrieren
                        //
                        m_Action=ENACT_BANKROTATION; 
                        m_iActionTimer = 20000;
                        
                        // -- Handlungsunfähig : bei Untergebenen schonmal abmelden
                        //
                        l_penemyChild = ENEMY::getFirstEnemy();
                        while(l_penemyChild)
                        {
                           if (l_penemyChild->m_pLeader == this)
                           {
                         
                              if (l_penemyChild->isPartOfLeader() == false)
                              {
                                 l_penemyChild->m_pLeader = NULL;
                              }
   
                              // --- neue Entscheidung forcieren
                              //
                              l_penemyChild->m_Action = ENACT_DONOTHING;
                           }
                           l_penemyChild = l_penemyChild->getNextEnemy();
                        }
                     } // end-if Injured

                     // -- was passiert mit nun unnuetzen Shoot?
                     //
							/*
                     if (g_equipmentArray[EQUIPMENT_MISSILE].m_bSingleton &&
                         g_equipmentArray[EQUIPMENT_MISSILE].m_idxShootSingleton == l_idxShoot)
                     {
                        l_pshootCurr->insertImpact();
                        l_pshootCurr->Die();
                     }else{
   
                        if (g_equipmentArray[EQUIPMENT_MINE].m_bSingleton &&
                            g_equipmentArray[EQUIPMENT_MINE].m_idxShootSingleton == l_idxShoot)
                        {
                           l_pshootCurr->insertImpact();
                           l_pshootCurr->Die();
                        }else{
                           ShotAgainstShield(l_idxShoot, l_pvNormal, l_pvPos);
                        }
                     }
							*/
                     ShotAgainstShield(l_idxShoot, l_pvNormal, l_pvPos);
                  }else{ //if(m_fShieldAmount > 0.0f)

                     // -- enemy ist ERLEDIGT
                     //
                     int l_iFightExperience;

                     l_iFightExperience = 0;     

                     switch (mType)
                     {
                        case ETYP_PIRATE:       l_iFightExperience = 1000;  break;
                        case ETYP_P_FLYER:      l_iFightExperience = 1000;  break;

                        case ETYP_FLYER:        l_iFightExperience = 500;   break;
                        case ETYP_LEADER:       l_iFightExperience = 1000;  break;
                        case ETYP_HUNTER:       l_iFightExperience = 1000;  break;
                        case ETYP_TRADER:       l_iFightExperience = 1000;  break;
                        case ETYP_HANGAR:       l_iFightExperience = 100;   break;
 
                        case ETYP_GUN:          l_iFightExperience = 50;    break;
                        case ETYP_LASERGUN:     l_iFightExperience = 50;    break;
                        case ETYP_LASERHALTER:  l_iFightExperience = 50;    break;
                        case ETYP_REACTORGUN:   l_iFightExperience = 50;    break;
                        case ETYP_DOOR:         l_iFightExperience = 500;   break;

                        case ETYP_THARGOID:     l_iFightExperience = 1000;  break;
                        case ETYP_THARGOIDDRONE:l_iFightExperience = 500;   break;
                     }

                     // -- Zerstörung des Hauptpirat verdient eine Belohnung
                     //
							/*
                     if (mType == ETYP_PIRATE && m_pCharacter)
                     {
                        static char l_cReward[50]; 

                        PSXPrintf(__LINE__, l_cReward, sizeof(l_cReward), 
                                   "%.1f CR", float(m_pCharacter->getReward()) / 10.0f);

                        g_Player.m_iBankAccount = g_Player.m_iBankAccount + 
                                                  m_pCharacter->getReward();

                        if (PSXInitializeTerminal( 
                                g_cPhantasyNameArray[m_pCharacter->getPhantasyNameIndex()],
                                g_cLang[txxRewardEarned],
                                l_cReward,
                                NULL ) == false)
                        {
                        }

                        m_pCharacter->m_bAmILame = true;
                     }
							*/

                     g_Player.m_iFightExperience = g_Player.m_iFightExperience + 
                                                            l_iFightExperience;

                     l_pshootCurr->Die();
                     detonate();

                  }//if(m_fShieldAmount > 0.0f)
               }//end-if (m_fShieldAmount>0.2f)
            } // end-if menacetype enemy

         }else{ //if bAffectable  
            l_pshootCurr->Die();

            // --- wirkungslose verpuffung: 
            //     neuen Enemy eintrag für pseudo explosion
            //     "aufschlagene Laser Bolzen"
            //
            l_pNewEnemy = new (LIFECYCLE_LEVEL) EXPLOSION(LIFECYCLE_LEVEL);
            if (l_pNewEnemy != NULL)
            { 
               //l_pNewEnemy->Init();
               
               l_pNewEnemy->SetType(ETYP_IMPACT);
               if (l_pNewEnemy->setPos(l_pshootCurr->m_vPos)==false) return false;
               if (l_pNewEnemy->setStep(0.0f, 0.0f, 0.0f)==false) return false;
               l_pNewEnemy->setVisible(true);
               l_pNewEnemy->setTriggered(true);
               l_pNewEnemy->m_fExplScale = 1.0f;
            }

         } // end-if bAffectable
      } // end-if l_iDistance < m_fShootHitRadPow2
   }

   return true;
}


              

/*
            case ETYP_DOOR:

               if (m_pLeader == NULL &&
                   g_cntDestroyedGenerators==g_cntGeneratorsToDestroy)
               {
                  g_Player.m_iFightExperience = g_Player.m_iFightExperience + m_iFightExperience;
                  detonate();
                  shoot_array[l_idxShoot].Die();
                  //if (TickEquipmentAppearance() == false) return false;

               }else{
                  ShotAgainstShield(l_idxShoot, l_pvNormal);
               }

               break;
               
            case ETYP_CHAR:

               // -- transfering shoot step in coordinates 
               //    relative to Enemy sight
               //
               yDest= m_Mat._21 * (shoot_array[l_idxShoot].m_vPos.x - m_vPos.x)+ 
                      m_Mat._22 * (shoot_array[l_idxShoot].m_vPos.y - m_vPos.y)+
                      m_Mat._23 * (shoot_array[l_idxShoot].m_vPos.z - m_vPos.z);

               m_cntShieldFlashTimer=1;

               m_Action=ENACT_BANKROTATION;
               m_iActionTimer=100.0f/ g_fBalanceFactor; 

               ShotAgainstShield(l_idxShoot, l_pvNormal);

               if(m_cntGeneric==27)               
               {
                  g_idxPlayerName--;
                  g_cPlayerName[g_idxPlayerName]=NULL;
               }else{
                  if(m_cntGeneric!=28 &&  
                     g_idxPlayerName<g_maxPlayerName)
                  {
                     g_cPlayerName[g_idxPlayerName]=(char)m_cntGeneric;
                        g_idxPlayerName++;
                  }else{
                     g_bHighScoreDone=false;
                  }
               }
               break;

            case ETYP_LEADER:
               // -- transfering shoot step in coordinates 
               //    relative to Enemy sight
               //
               x = shoot_array[l_idxShoot].m_vStep.x;
               y = shoot_array[l_idxShoot].m_vStep.y;
               z = shoot_array[l_idxShoot].m_vStep.z;

               yDest = m_Mat._21*x + m_Mat._22*y + m_Mat._23*z; 
               if ((l_fDistPow2 < 25) && 
                   (shoot_array[l_idxShoot].menacetype & MENACE_ENEMY))
               {
                  triggerIfPossible();

                  if(yDest > 0 || 
                     (shoot_array[l_idxShoot].m_idxShootDrawer == SHOOTDRAWER_REDTORPEDO &&
                      shoot_array[l_idxShoot].menacetype & MENACE_ENEMY)|| 
                     m_cntShieldAmount<7)
                  {
                     if (m_cntShieldAmount>=4)
                     {
                        shoot_array[l_idxShoot].Die();
                        
                        if(shoot_array[l_idxShoot].m_idxShootDrawer == SHOOTDRAWER_REDTORPEDO && 
                           shoot_array[l_idxShoot].menacetype & MENACE_ENEMY)
                        {
                           m_cntShieldAmount = m_cntShieldAmount - 2;
                        }else{
                           m_cntShieldAmount = m_cntShieldAmount - 1;
                        }

                        xDest= m_Mat._11 * (shoot_array[l_idxShoot].m_vPos.x - m_vPos.x)+ 
                               m_Mat._12 * (shoot_array[l_idxShoot].m_vPos.y - m_vPos.y)+
                               m_Mat._13 * (shoot_array[l_idxShoot].m_vPos.z - m_vPos.z);

                        SmokeTrayActivate(this, xDest);

                        if (leadhist[m_idxLeadhist].pLeader == this)
                        {
                           memset(leadhist+m_idxLeadhist, 0, sizeof(LEADHIST));
                        }
               
                     }else{

                        g_Player.m_iFightExperience = g_Player.m_iFightExperience + m_iFightExperience;
                        //mType  = ETYP_EXPLOSION;
                        detonate();
                        shoot_array[l_idxShoot].Die();
                      //  if (TickEquipmentAppearance() == false) return false;
                     }

                     if(m_cntShieldAmount<=3)
                     {
                        float l_fShootLength;

                        xDest= m_Mat._11 * (shoot_array[l_idxShoot].m_vPos.x - m_vPos.x)+ 
                               m_Mat._12 * (shoot_array[l_idxShoot].m_vPos.y - m_vPos.y)+
                               m_Mat._13 * (shoot_array[l_idxShoot].m_vPos.z - m_vPos.z);

                        SmokeTrayActivate(this, xDest);

                        m_Action=ENACT_BANKROTATION;
                        m_iActionTimer = 500.0f/ g_fBalanceFactor; // solange keinen Angriff unternehmen

                        l_fShootLength = sqrt( pow2(shoot_array[l_idxShoot].m_vStep.x)+
                                               pow2(shoot_array[l_idxShoot].m_vStep.y)+
                                               pow2(shoot_array[l_idxShoot].m_vStep.z));

                        m_fSpeed = 2.0f;
                        m_vStep.x = shoot_array[l_idxShoot].m_vStep.x * m_fSpeed / l_fShootLength;
                        m_vStep.y = shoot_array[l_idxShoot].m_vStep.y * m_fSpeed / l_fShootLength;
                        m_vStep.z = shoot_array[l_idxShoot].m_vStep.z * m_fSpeed / l_fShootLength;

                     }

                  }else{
                     ShotAgainstShield(l_idxShoot, l_pvNormal);
                     if (g_bDestroyLeaderFromBehindHintPrinted == false)
                     {
                        g_bDestroyLeaderFromBehindHintPrinted = true;
                        if (PSXInitializeTerminal( g_cLang[txxDestLeaderFromBehind1],
                                                   g_cLang[txxDestLeaderFromBehind2],
                                                   NULL,
                                                   NULL ) == false)
                        {
                        }
                     }
                  }
               }
               break;

*/
/*
            case ETYP_FLYER:
               // -- transfering shoot step in coordinates 
               //    relative to Enemy sight
               //
               x = shoot_array[l_idxShoot].m_vStep.x;
               y = shoot_array[l_idxShoot].m_vStep.y;
               z = shoot_array[l_idxShoot].m_vStep.z;

               yDest = m_Mat._21*x + m_Mat._22*y + m_Mat._23*z; 
               if ((l_fDistPow2 < 25) && 
                   (shoot_array[l_idxShoot].menacetype & MENACE_ENEMY))
               {
                  if (m_pLeader)
                  {
                     if (m_pLeader->isTriggered()==false)
                     { 
                        if (m_cntCurrentlyAttackingLeaders < g_maxCurrentlyAttackingLeaders)
                        {
                           m_pLeader->setTriggered(true);
                           m_cntCurrentlyAttackingLeaders = m_cntCurrentlyAttackingLeaders + 1;
                        }
                     }
                  }

                  if(yDest > 0 ||
                     (shoot_array[l_idxShoot].m_idxShootDrawer == SHOOTDRAWER_REDTORPEDO &&
                      shoot_array[l_idxShoot].menacetype & MENACE_ENEMY) || 
                     (m_pLeader == NULL))
                  {
                     if(m_cntShieldAmount>=1)
                     {
                        if(shoot_array[l_idxShoot].m_idxShootDrawer == SHOOTDRAWER_REDTORPEDO && 
                           shoot_array[l_idxShoot].menacetype & MENACE_ENEMY)
                        {
                           m_cntShieldAmount = m_cntShieldAmount - 2;
                        }else{
                           m_cntShieldAmount = m_cntShieldAmount - 1;
                        }
                        shoot_array[l_idxShoot].Die();

                        xDest= m_Mat._11 * (shoot_array[l_idxShoot].m_vPos.x - m_vPos.x)+ 
                               m_Mat._12 * (shoot_array[l_idxShoot].m_vPos.y - m_vPos.y)+
                               m_Mat._13 * (shoot_array[l_idxShoot].m_vPos.z - m_vPos.z);

                        SmokeTrayActivate(this,xDest);
                     }

                     if(m_cntShieldAmount<=0)
                     {
                        if ( g_Player.m_crimePoliceFile == CRID_CLEAN )
                        {
                           g_Player.setPoliceFile(CRID_OFFENDER);
                        }

                        g_Player.m_iFightExperience = g_Player.m_iFightExperience + m_iFightExperience;
                        detonate();
                        shoot_array[l_idxShoot].Die();
                        //if (TickEquipmentAppearance() == false) return false;
                     }

                  }else{
                     ShotAgainstShield(l_idxShoot, l_pvNormal);

                     if (g_bDestroyLeaderFromBehindHintPrinted == false)
                     {
                        g_bDestroyLeaderFromBehindHintPrinted = true;
                        if (PSXInitializeTerminal( g_cLang[txxDestLeaderFromBehind1],
                                                   g_cLang[txxDestLeaderFromBehind2],
                                                   NULL,
                                                   NULL ) == false)
                        {
                        }
                     }
                  }
               }
               break;
*/
/*

            case ETYP_TUNNELFLYER:
               if(m_cntShieldAmount>=1)
               {
                  shoot_array[l_idxShoot].Die();

                  if(shoot_array[l_idxShoot].m_idxShootDrawer == SHOOTDRAWER_REDTORPEDO && 
                     shoot_array[l_idxShoot].menacetype & MENACE_ENEMY)
                  {
                     m_cntShieldAmount = m_cntShieldAmount - 2;
                  }else{
                     m_cntShieldAmount = m_cntShieldAmount - 1;
                  }

                  xDest= m_Mat._11 * (shoot_array[l_idxShoot].m_vPos.x - m_vPos.x)+ 
                         m_Mat._12 * (shoot_array[l_idxShoot].m_vPos.y - m_vPos.y)+
                         m_Mat._13 * (shoot_array[l_idxShoot].m_vPos.z - m_vPos.z);

                  SmokeTrayActivate(this,xDest);
               }

               if(m_cntShieldAmount<=0)
               {
                  g_Player.m_iFightExperience = g_Player.m_iFightExperience + m_iFightExperience;
                  detonate();
                  shoot_array[l_idxShoot].Die();
                  //if (TickEquipmentAppearance() == false) return false;
               }
               break;
*/

/*
            case ETYP_BRENNSTAB:

               // -- Der Brennstab kann erst zerstoert werden, wenn alle Feldgeneratoren
               //    vernichtet sind.
               if (g_cntDestroyedGenerators<g_cntGeneratorsToDestroy)
               {
                  ShotAgainstShield(l_idxShoot, l_pvNormal);
               }else{
                  if(m_cntShieldAmount>0)
                  {
                     
                     m_cntShieldAmount--;
                     //g_cntBrennstabHit++;
                     g_cntBrennstabHit=g_maxBrennstabHit-m_cntShieldAmount;

                     if(m_cntShieldAmount==4 ||
                        m_cntShieldAmount==1);
                     {
                        m_cntRanking=int(100.0f/g_fBalanceFactor);
                     }
                  }else{
                     detonate();
                     g_Player.m_iFightExperience = g_Player.m_iFightExperience + m_iFightExperience;
                  }
               }
               break;

            case ETYP_EQUIPMENT:

               if (shoot_array[l_idxShoot].menacetype & MENACE_ENEMY)
               {
                  ShotAgainstShield(l_idxShoot, l_pvNormal);
               }
               break;

            case ETYP_GHOST:
               m_cntShieldAmount--;

               if(m_cntShieldAmount>0)
               {
                  PSXVECTOR l_vPos ={0.0f,0.0f,0.0f}, 
                            l_vStep={0.0f,0.0f,0.0f}; 
                  
                  l_vPos.x =m_vPos.x;
                  l_vPos.y =m_vPos.y;
                  l_vPos.z =m_vPos.z;

                  l_vStep.x =m_vStep.x;
                  l_vStep.y =m_vStep.y;
                  l_vStep.z =m_vStep.z;

                  ENEMY* l_pNewEnemy;
                  l_pNewEnemy = new (LIFECYCLE_LEVEL) ENEMY(LIFECYCLE_LEVEL);
                  if(l_pNewEnemy != NULL)
                  { 
                     
                     //l_pNewEnemy->Init();
                     
                     l_pNewEnemy->detonate();
                     l_pNewEnemy->m_vPos.x  = l_vPos.x;
                     l_pNewEnemy->m_vPos.y  = l_vPos.y;
                     l_pNewEnemy->m_vPos.z  = l_vPos.z;
                                
                     l_pNewEnemy->m_vStep.x = l_vStep.x;
                     l_pNewEnemy->m_vStep.y = l_vStep.y;
                     l_pNewEnemy->m_vStep.z = l_vStep.z;

                     l_pNewEnemy->setVisible(true);
                     l_pNewEnemy->setTriggered(true);
                     l_pNewEnemy->m_fExplScale = 3.0f;
                     l_pNewEnemy->m_iFightExperience     = 0; 
                  }

               }else{
                  detonate();
               }
               break;


            case ETYP_TMPSPEEDUP:
               ShotAgainstShield(l_idxShoot, l_pvNormal);
               break;

            case ETYP_MINE:

               if (m_cntShieldAmount>=0)
               {
                  if(shoot_array[l_idxShoot].m_idxShootDrawer == SHOOTDRAWER_REDTORPEDO && 
                     shoot_array[l_idxShoot].menacetype & MENACE_ENEMY)
                  {
                     m_cntShieldAmount = m_cntShieldAmount - 2;
                  }else{
                     m_cntShieldAmount = m_cntShieldAmount - 1;
                  }
               }

               if (m_cntShieldAmount>=0)
               {
                  // --- verpuffung des ersten schusses
                  //
                  l_pNewEnemy = new (LIFECYCLE_LEVEL) EXPLOSION(LIFECYCLE_LEVEL);
                  if (l_pNewEnemy != NULL)
                  { 
                     //l_pNewEnemy->Init();
                     
                     l_pNewEnemy->SetType(ETYP_IMPACT);
                     l_pNewEnemy->m_vPos.x  = shoot_array[l_idxShoot].m_vPos.x;
                     l_pNewEnemy->m_vPos.y  = shoot_array[l_idxShoot].m_vPos.y;
                     l_pNewEnemy->m_vPos.z  = shoot_array[l_idxShoot].m_vPos.z;
                                
                     l_pNewEnemy->m_vStep.x    = 0.0f;
                     l_pNewEnemy->m_vStep.y    = 0.0f;
                     l_pNewEnemy->m_vStep.z    = 0.0f;
                     l_pNewEnemy->setVisible(true);
                     l_pNewEnemy->setTriggered(true);
                     l_pNewEnemy->m_fExplScale = 1.0f;
                     l_pNewEnemy->m_iFightExperience  = 0; 
                  }

                  // -- Rauchspur aktivieren
                  //
                  xDest= m_Mat._11 * (shoot_array[l_idxShoot].m_vPos.x - m_vPos.x)+ 
                         m_Mat._12 * (shoot_array[l_idxShoot].m_vPos.y - m_vPos.y)+
                         m_Mat._13 * (shoot_array[l_idxShoot].m_vPos.z - m_vPos.z);

                  SmokeTrayActivate(this,xDest);

                  m_Action = ENACT_ESCAPE;

                  // -- Bestimmung der Escape-Flugrichtung und Geschwindigkeit
                  //
                  {
                     float l_fShootSpeed = sqrt( pow2(shoot_array[l_idxShoot].m_vStep.x)+
                                                 pow2(shoot_array[l_idxShoot].m_vStep.y)+
                                                 pow2(shoot_array[l_idxShoot].m_vStep.z));

                     m_fSpeed = 4.0f;
                     m_vStep.x = shoot_array[l_idxShoot].m_vStep.x * m_fSpeed / l_fShootSpeed;
                     m_vStep.y = shoot_array[l_idxShoot].m_vStep.y * m_fSpeed / l_fShootSpeed;
                     m_vStep.z = shoot_array[l_idxShoot].m_vStep.z * m_fSpeed / l_fShootSpeed;
                  }

                  ShotAgainstShield(l_idxShoot, l_pvNormal);

               }else{
                  g_Player.m_iFightExperience = g_Player.m_iFightExperience + m_iFightExperience;

                  //if (TickEquipmentAppearance() == false) return false;
                  
                  detonate();
                  shoot_array[l_idxShoot].Die();
               }
               break;

            case ETYP_IMPACT: // ist wie luft, der schuss geht ohne behelligung hindurch
               break;
*/
/*

            case ETYP_HANGAR:

               if (m_cntShieldAmount)
               {
                  m_cntShieldAmount = m_cntShieldAmount - 1;

                  // --- verpuffung des ersten schusses
                  //
                  if (m_Action != ENACT_ESCAPE)
                  {
                     l_pNewEnemy = new (LIFECYCLE_LEVEL) ENEMY(LIFECYCLE_LEVEL);
                     if (l_pNewEnemy != NULL)
                     { 
                        //l_pNewEnemy->Init();
                        
                        l_pNewEnemy->detonate();
                        l_pNewEnemy->m_vPos.x  = m_vPos.x;
                        l_pNewEnemy->m_vPos.y  = m_vPos.y;
                        l_pNewEnemy->m_vPos.z  = m_vPos.z;
                                   
                        l_pNewEnemy->m_vStep.x = 0.0f;
                        l_pNewEnemy->m_vStep.y = 0.0f;
                        l_pNewEnemy->m_vStep.z = 0.0f;
                        l_pNewEnemy->setVisible(true);
                        l_pNewEnemy->setTriggered(true);
                        l_pNewEnemy->m_fExplScale = m_fExplScale;
                        l_pNewEnemy->m_fRad = m_fRad;
                        l_pNewEnemy->m_iFightExperience     = 0; 
                     }
                     m_Action = ENACT_ESCAPE;

                     // -- Bestimmung der Escape-Flugrichtung und Geschwindigkeit
                     //
                     m_fSpeed  = 4.0f;
                     m_vStep.x = m_Mat._31 * m_fSpeed;
                     m_vStep.y = m_Mat._32 * m_fSpeed;
                     m_vStep.z = m_Mat._33 * m_fSpeed;
                  }

                  shoot_array[l_idxShoot].Die();

               }else{
                  if ( g_Player.m_crimePoliceFile == CRID_CLEAN )
                  {
                     g_Player.setPoliceFile(CRID_OFFENDER);
                  }
                  g_Player.m_iFightExperience = g_Player.m_iFightExperience + m_iFightExperience;

                  detonate();
                  shoot_array[l_idxShoot].Die();
               }
               break;

            case ETYP_FELDGENERATOR:

               if (m_cntShieldAmount)
               {
                  m_cntShieldAmount = m_cntShieldAmount - 1;

                  // --- verpuffung des ersten schusses
                  //
                  if (m_Action != ENACT_COUNTDOWN)
                  {
                     l_pNewEnemy = new (LIFECYCLE_LEVEL) EXPLOSION(LIFECYCLE_LEVEL);
                     if (l_pNewEnemy != NULL)
                     { 
                        //l_pNewEnemy->Init();
                        
                        l_pNewEnemy->SetType(ETYP_IMPACT);
                        l_pNewEnemy->m_vPos.x  = m_vPos.x;
                        l_pNewEnemy->m_vPos.y  = m_vPos.y;
                        l_pNewEnemy->m_vPos.z  = m_vPos.z;
                                   
                        l_pNewEnemy->m_vStep.x = 0.0f;
                        l_pNewEnemy->m_vStep.y = 0.0f;
                        l_pNewEnemy->m_vStep.z = 0.0f;
                        l_pNewEnemy->setVisible(true);
                        l_pNewEnemy->setTriggered(true);
                        l_pNewEnemy->m_fExplScale = g_levelContent.m_fScale;
                        l_pNewEnemy->m_fRad = m_fRad;
                        l_pNewEnemy->m_iFightExperience     = 0; 
                     }
                     m_Action = ENACT_COUNTDOWN;

                     // -- Bestimmung der Escape-Flugrichtung und Geschwindigkeit
                     //
                     m_fSpeed  = 1.0f;
                     m_vStep.x = m_Mat._21 * 1.0f;
                     m_vStep.y = m_Mat._22 * 1.0f;
                     m_vStep.z = m_Mat._23 * 1.0f;
                  }

                  shoot_array[l_idxShoot].Die();

               }else{
                  g_Player.m_iFightExperience = g_Player.m_iFightExperience + m_iFightExperience;

                  detonate();
                  shoot_array[l_idxShoot].Die();
               }
               break;
*/
/*
bool ENEMY::TickEquipmentAppearance()
{
   ENEMY* l_pNewEnemy;
   int l_idxEquipment;

   // -- waehrend der Transferphase darf die Equipment logik nicht
   //    beruehrt werden, sonst erscheint ein powerup kurz bevor der
   //    Spieler versetzt wird
   //
   if (g_fcntJumpWitchSpace >0.0f) return true;
   
   // --- Pruefen welche Equipments verfuegbar sind
   //
   for (l_idxEquipment=0; l_idxEquipment < EQUIPMENT_LASTENUM; l_idxEquipment++)
   {
      if (g_cntEquipmentTicker == l_idxEquipment)
      {
         // --- neuen Enemy eintrag zur Symbolisierung des Equipments
         //
         l_pNewEnemy = new (LIFECYCLE_LEVEL) ENEMY(LIFECYCLE_LEVEL);
         if (l_pNewEnemy != NULL)
         { 
            //l_pNewEnemy->Init();
            
            l_pNewEnemy->SetType(ETYP_EQUIPMENT);
            if (l_pNewEnemy->setPos(m_vPos)==false) return false;
            if (l_pNewEnemy->setStep(0.0f,0.0f,0.0f)==false) return false;
            l_pNewEnemy->setVisible(true);
            l_pNewEnemy->m_cntRanking     = l_idxEquipment;
            l_pNewEnemy->m_cDestFrameInfo = g_equipmentArray[l_idxEquipment].m_cName;
            l_pNewEnemy->setDestFrame(true);
         }
      }
   }

   g_cntEquipmentTicker ++;

   return true;
}
*/
/*
// -- Behandlung wenn Spieler mit Equipment in Beruehrung kommt
//
bool ENEMY::CollectEquipment()
{
   //g_arr_bEquipmentEnabled[m_cntRanking] = true;
   if(mType==ETYP_CARGOBOX)
   {
      for(int i=0; i<GOOID_LASTENUM;i++)
      {

         //-- Spieler sammelt die Fracht in seinen eigenen
         //   Frachtraum
         //
         if(isShipsLostArray())
         {
            if(g_pShipsLostArray[i]>0)
            {
               g_iShipsHoldArray[i]+= g_pShipsLostArray[i];
               g_pShipsLostArray[i]=0; // -> wieder initialisieren....
            }
         }else{


            if(g_iShipsFoundArray[i]>0)
            {
               int l_cntSpace;

               if (PSXCheckSpaceInCargoBay(i, &l_cntSpace) == false)
               {
                  return false;
               }
 
               if (l_cntSpace >= g_iShipsFoundArray[i])  
               {
                  g_iShipsHoldArray[i]+=g_iShipsFoundArray[i];
                  g_iShipsFoundArray[i]=0; // -> wieder initialisieren....
               }else{
                  if (l_cntSpace)
                  {
                     g_iShipsHoldArray[i]+=l_cntSpace;
                     g_iShipsFoundArray[i]-=l_cntSpace;
                  }
               }
            }
         }
      }

      if(isShipsLostArray())
      {
         g_cntCargoBox--; 
      }
   }
   // --- play Gameplay-Update sound 
   //
   if (g_options.IsEnableSound())
   {
      if (g_sound_array[SNDRES_SHIELDRELOAD].play(this) == false) return false;
   }
   
   if (Destroy(LIFECYCLE_LEVEL)==false) return false;
   SetType(ETYP_EMPTY);

   if (g_Player.getGS()->isHavingTextOverlay())
   {
      if (g_Player.getGS()->applyUpdateText(g_Player.getGS()->getIndex()) == false)
      {
         return false;
      }
   }
   return true;
}
*/
/*
bool ENEMY::CollectTmpUp()
{
   g_fSpeedUpTime+=1.0f;
   g_fTmpSpeedUpFactor=15.0f;

   if (PSXInitializeTerminal(g_cLang[txxTurboActivated],
                             NULL,
                             NULL,
                             NULL) == false)
   {
      return false;
   }

   // --- play Gameplay-Update sound 
   //
   if (g_options.IsEnableSound())
   {
      if (g_sound_array[SNDRES_SHIELDRELOAD].play(this) == false) return false;
   }

   
//   TmpSpeedUp sollen bestehen bleiben....
//   SetType(ETYP_EMPTY);
   return true;
}

*/
// -- selektiert die Faces bei drohenden Zusammenstoessen mit dem Flagschiff
//
/*
bool ENEMY::CheckFlagshipCollision(bool* l_pbIntersectAtValid, 
                                   float* l_pfIntersectAt,
                                   PSXFACEINFO** l_ppfaceinfIntersect)
{
   PSXVECTOR l_vFaceEnemyRel,
             l_vEnemyFSRel;
   float l_fRadPow2, l_fLengthPow2;
   int l_cntHits, l_cntSuspects, l_idxBigResItem;
   RESTYPE l_resPart;
   
   if (g_options.IsWriteLogFileInLoop())
   {
      WriteLogFile("::CheckFlagshipCollision\n");
   }

   *l_pbIntersectAtValid = false;

   l_cntHits = 0;
   l_cntSuspects = 0;
   
   for (l_idxBigResItem = g_levelContent.m_pBigRes->m_idxBigResItem;
        l_idxBigResItem < g_levelContent.m_pBigRes->m_cntBigResItem;
        l_idxBigResItem ++)
   {
      l_vEnemyFSRel.x = m_vPos.x - 
                        g_bigresitemArray[l_idxBigResItem].m_vPos.x - 
                        g_levelContent.m_vPos.x;
      l_vEnemyFSRel.y = m_vPos.y - 
                        g_bigresitemArray[l_idxBigResItem].m_vPos.y - 
                        g_levelContent.m_vPos.y;
      l_vEnemyFSRel.z = m_vPos.z - 
                        g_bigresitemArray[l_idxBigResItem].m_vPos.z - 
                        g_levelContent.m_vPos.z;
   
      l_resPart = g_bigresitemArray[l_idxBigResItem].m_resPart;
   
      l_fLengthPow2 = pow2( l_vEnemyFSRel.x)+
                      pow2( l_vEnemyFSRel.y)+
                      pow2( l_vEnemyFSRel.z);
      l_fRadPow2 = pow2(resourceoffsets[l_resPart].rad);
   
      if (l_fLengthPow2 < l_fRadPow2)
      {
         int l_cntFaces;
         int l_iAlignedPosArray[4];
   
         PSXFACE*   l_faceArray;
         PSXVERTEX* l_vertArray;
         PSXFACEINFO* l_faceinfCurr;
   
         int l_cntSuspect, l_idxSuspect, l_idxFace;
    
         l_cntFaces  = resourceoffsets[l_resPart].cnt_faces;
         l_faceArray = (PSXFACE*)(resourceoffsets[l_resPart].face_offset);
         l_vertArray = (PSXVERTEX*)
                       (resourceoffsets[l_resPart].vertex_offset);
         l_faceinfCurr = g_bigresitemArray[l_idxBigResItem].m_faceinfArray;
   
         memset((void*)l_iAlignedPosArray, 0, sizeof(l_iAlignedPosArray));
         memcpy((void*)l_iAlignedPosArray, 
                (void*)&l_vEnemyFSRel, sizeof(l_vEnemyFSRel));
   
         // -- drohende Faces im zehnfachen Radius des Enemies selektieren
         //    
         l_cntSuspect = 0;
         if (l_cntFaces > 0)
         {
            l_cntSuspect = 
               PSXSelectHitObjects(
                   &(l_faceinfCurr[0].vCentroid), 
                   l_cntFaces,
                   (PSXVECTOR*)l_iAlignedPosArray,
                   sizeof(PSXFACEINFO),
                   (void**)g_pfaceinfSelectedArray,
                   int(&(l_faceinfCurr[0].fRadPow2))-int(l_faceinfCurr),
                   int(&(l_faceinfCurr[0].vCentroid))-int(l_faceinfCurr),
                   int(g_bSupportingSSE), pow2(m_fRad*100.0f) ); 
         }
   
         // -- fuer jeden Hinweis tatsaechliche Faceintersection pruefen
         //
         for (l_idxSuspect=0; l_idxSuspect < l_cntSuspect; l_idxSuspect++)
         {
            PSXVECTOR l_vLineArray[3];
            int l_idxVert[3];

            PSXVECTOR l_vDeltaArray2D[3], l_vLineArray2D[4];
            float l_fIncrement[3];
            HRESULT hr;
            bool l_bCrossed;
            bool l_bInclusion;
   
            l_idxFace = (g_pfaceinfSelectedArray[l_idxSuspect]) - 
                        l_faceinfCurr;
             
            l_vFaceEnemyRel.x = l_faceinfCurr[l_idxFace].vCentroid.x - l_vEnemyFSRel.x;
            l_vFaceEnemyRel.y = l_faceinfCurr[l_idxFace].vCentroid.y - l_vEnemyFSRel.y;
            l_vFaceEnemyRel.z = l_faceinfCurr[l_idxFace].vCentroid.z - l_vEnemyFSRel.z;
   
            l_fLengthPow2 = pow2( l_vFaceEnemyRel.x)+
                            pow2( l_vFaceEnemyRel.y)+
                            pow2( l_vFaceEnemyRel.z);
   
            if (l_fLengthPow2 < l_faceinfCurr[l_idxFace].fRadPow2)
            {
   
               l_cntSuspects++;
   
               // zuweisung des arrays kommt nach debug phase wieder unten in den if
               //
               l_idxVert[0]  = l_faceArray[l_idxFace].a;
               l_idxVert[1]  = l_faceArray[l_idxFace].b;
               l_idxVert[2]  = l_faceArray[l_idxFace].c;

               for (int j=0; j<3; j++)
               {
                  float l_f2D_z;
                  float x,y,z, l_fDiv;

                  x = l_vertArray[l_idxVert[j]].x +
                      g_levelContent.m_vPos.x +
                      g_bigresitemArray[l_idxBigResItem].m_vPos.x -
                      m_vPos.x;

                  y = l_vertArray[l_idxVert[j]].y +
                      g_levelContent.m_vPos.y +
                      g_bigresitemArray[l_idxBigResItem].m_vPos.y - 
                      m_vPos.y;

                  z = l_vertArray[l_idxVert[j]].z + 
                      g_levelContent.m_vPos.z +
                      g_bigresitemArray[l_idxBigResItem].m_vPos.z - 
                      m_vPos.z;

                  l_vLineArray2D[j].x = m_Mat._11*x + m_Mat._12*y + m_Mat._13*z;
                  l_vLineArray2D[j].y = m_Mat._21*x + m_Mat._22*y + m_Mat._23*z;
                  l_vLineArray2D[j].z = m_Mat._31*x + m_Mat._32*y + m_Mat._33*z;

               }

               // -- The following algorithm doesn't know that the first vert is also
               //    the last
               //
               l_vLineArray2D[3] = l_vLineArray2D[0];
            
               // -- getting delta values
               //
               for (int m=0; m<3; m++)
               {
                  l_vDeltaArray2D[m].x = l_vLineArray2D[m+1].x - l_vLineArray2D[m].x;
                  l_vDeltaArray2D[m].y = l_vLineArray2D[m+1].y - l_vLineArray2D[m].y;
                  l_vDeltaArray2D[m].z = l_vLineArray2D[m+1].z - l_vLineArray2D[m].z;
               }
               
               // -- check if center is included
               //
               l_bCrossed   = false;
               l_bInclusion = false;
               for (int i=0; i<3; i++)
               {
            
                  // -- if line is crossing x axis boundary, first condition for
                  //    inclusion of the screencenter is given: screen center is at
                  //    the same height of the polygon
                  //
                  if ( (l_vLineArray2D[i].y   <=0) && (l_vLineArray2D[i+1].y >0)||
                       (l_vLineArray2D[i+1].y <=0) && (l_vLineArray2D[i].y   >0) )
                  {
                     register float l_fAtZero, l_fIncrement;
             
                     l_bCrossed = true;
            
                     l_fIncrement = l_vDeltaArray2D[i].x / l_vDeltaArray2D[i].y;
            
                     // -- x-value at zero on y axis 
                     //
                     l_fAtZero = l_vLineArray2D[i].x -
                                 l_vLineArray2D[i].y * l_fIncrement;
               
                     // -- negate Inclusion flag if jumped over the fence walking to the
                     //    right: 
                     //    if screen center is at same height of polygon but
                     //    the fence have to be jumped twice, the flag will also
                     //    be negated twice => false again
                     //
                     if ((l_fAtZero > 0))
                     {
                        l_bInclusion = !l_bInclusion;
                     }
                  }
               }

               if (l_bInclusion)
               {
                  float l_fSpeedDiv,
                        l_fCandidate;
                  
                  //t =(nx*centroid.x + ny*centroid.y + nz*centroid.z)/
                  //   (nx*vStep.x + ny*vStep.y + nz*vStep.z)

                  PSXVECTOR *l_pvNormal;
                  PSXVECTOR l_vLineList[2];
                  l_pvNormal = &(l_faceinfCurr[l_idxFace].vNormal);
                 
                  l_fCandidate = 
                    (l_vFaceEnemyRel.x * l_pvNormal->x + 
                     l_vFaceEnemyRel.y * l_pvNormal->y + 
                     l_vFaceEnemyRel.z * l_pvNormal->z) /
                    ( m_vStep.x/m_fSpeed  * l_pvNormal->x +
                      m_vStep.y/m_fSpeed  * l_pvNormal->y +
                      m_vStep.z/m_fSpeed  * l_pvNormal->z);
            
                  // -- Zusammenstoesse die bereits hinter dem Enemy liegen
                  //    interessieren nicht
                  //
                  if (l_fCandidate >= 0.0f)
                  {
                     // -- falls eben schon ein Kandidat gefunden wurde
                     //    geht der allerdichteste Zusammenstoss vor
                     //    sonst Kandidat nun als gefunden gelten lassen
                     //
                     if (*l_pbIntersectAtValid)
                     {
                        if (l_fCandidate < *l_pfIntersectAt)
                        {
                           *l_pfIntersectAt = l_fCandidate;
                           *l_ppfaceinfIntersect = l_faceinfCurr + l_idxFace;
                        }
                     }else{
                        *l_pbIntersectAtValid = true;
                        *l_pfIntersectAt = l_fCandidate;
                        *l_ppfaceinfIntersect = l_faceinfCurr + l_idxFace;
                     }
                  }
               }
            }
         } // end-foreach suspect faces

      } // end-if in diesem part
   }

   return true;
}
*/
// -- das thargoiden mutterschiff
//
/*
bool ENEMY::TreatAsThargoid()
{

   if(m_cntThargoidDrone<g_maxThargoidDrone)
   {
      ENEMY* l_pEnemy, *l_pLeader;

      l_pLeader = this;

      l_pEnemy = new (LIFECYCLE_LEVEL) ENEMY(LIFECYCLE_LEVEL);
      if (l_pEnemy == NULL) return false;

      //l_pEnemy->Init();
      l_pEnemy->SetType(ETYP_THARGOIDDRONE);
      l_pEnemy->m_vPos.x = l_pLeader->m_vPos.x + g_mtwistStarfield.getReal1() * 1000.0f;
      l_pEnemy->m_vPos.y = l_pLeader->m_vPos.y + g_mtwistStarfield.getReal1() * 1000.0f;
      l_pEnemy->m_vPos.z = l_pLeader->m_vPos.z + g_mtwistStarfield.getReal1() * 1000.0f;
//      l_pEnemy->m_cntRanking = l_cntRanking;
      l_pEnemy->m_pLeader = l_pLeader;
      m_cntThargoidDrone++;
   }

   // --- Verhalten eines schweren Schiffes
   //
   if (handleDefaultBigCargo(5.0f)==false) return false;
   
   switch(m_Action)
   {
      case ENACT_DONOTHING:
         if (isTriggered())
         {
            m_Action = ENACT_CHASEPLAYER;
         }
         break;
   }
   return true;
}
// -- die Thargoid-Drohnen
//
/*
bool ENEMY::TreatAsThargoidDrone()
{

   float l_fDistance, l_fDistPow2;

   PSXVECTOR l_vPlayerGhostRel;
   PSXVECTOR l_vPredictedPos;

   l_vPredictedPos.x = g_Player.m_pEnemy->getPosX() + g_Player.m_pEnemy->getStepX() * 05.0f;
   l_vPredictedPos.y = g_Player.m_pEnemy->getPosY() + g_Player.m_pEnemy->getStepY() * 05.0f;
   l_vPredictedPos.z = g_Player.m_pEnemy->getPosZ() + g_Player.m_pEnemy->getStepZ() * 05.0f;

   l_vPlayerGhostRel.x = l_vPredictedPos.x - m_vPos.x;
   l_vPlayerGhostRel.y = l_vPredictedPos.y - m_vPos.y;
   l_vPlayerGhostRel.z = l_vPredictedPos.z - m_vPos.z;
  
   l_fDistPow2 = pow2(l_vPlayerGhostRel.x)+ 
                 pow2(l_vPlayerGhostRel.y)+ 
                 pow2(l_vPlayerGhostRel.z);

   l_fDistance = sqrt(l_fDistPow2);


   setTriggered(true);
   m_Action = ENACT_CHASEPLAYER;


   if (l_fDistance <= 300.0f)
   {
      m_Action = ENACT_AVOIDPLAYER;
   }else{
      m_Action = ENACT_CHASEPLAYER;
      m_Action = ENACT_SHOOT;
   }

   // --- Verhalten eines wendigen Schiffes
   //
   if (handleDefaultSmallShip(10.0f,
                             g_laserPoliceFlyerArray,
                             g_cntLaserPoliceFlyer) == false) return false;
 

   return true;
}
/*
bool ENEMY::TreatAsStarBaseHangar()
{
   return true;
}
*/
/*
bool ENEMY::TreatAsPlanet()
{
   int l_idxTrigger, l_cntTrigger;
   SEED l_seedPlanet;
   float l_fDegree, l_fDegree2;
   float l_fCircleRad;

   // -- throw dices m_cntRanking times
   //
   l_seedPlanet = g_Player.m_pLevel->m_seedPlanetOffsets;
   l_cntTrigger = m_cntRanking * 10;
   for (l_idxTrigger = 0; l_idxTrigger<l_cntTrigger; l_idxTrigger++)
   {
      l_seedPlanet.tickFibonacci();
   }

   // -- get orbital range and angles to planets from dice
   //
   l_fCircleRad = 1000.0f + float(m_cntRanking) * 10000.0f;

   l_fDegree  = l_seedPlanet.getFloat() * 2.0f * g_fPi;
   l_fDegree2 = l_seedPlanet.getFloat() * 2.0f * g_fPi;

   m_vPos.x = g_vCam4Astro.x + sin(l_fDegree) * cos(l_fDegree2) * l_fCircleRad;
   m_vPos.y = g_vCam4Astro.y +                  sin(l_fDegree2) * l_fCircleRad;
   m_vPos.z = g_vCam4Astro.z + cos(l_fDegree)                   * l_fCircleRad;

   setScale(l_seedPlanet.getFloat()*500.0f + 500.0f) ;
   setRadius(1.0f);
   return true;
}
*/
// -- aus einem Datacontainer alle Membervariablen zuweisen
//
/*
bool ENEMY::loadFromDC(ENEMYDATACONTAINER* l_pDC)
{
   //Init();

   // -- Zeiger aus den Indizes generieren
   //
   if (setCharacterIndex(l_pDC->m_idxCharacter) == false) return false;
   if (setEnemyIndex(l_pDC->m_idxEnemy) == false) return false;

   // -- das wichtigste zuerst: den typ festlegen
   //
   if (SetType(l_pDC->mType) == false) return false;

   if ( l_pDC->m_idxLeader >= 0)
   {
      m_pLeader = ENEMY::getByIndex(l_pDC->m_idxLeader);
      if (m_pLeader == NULL) 
      {
         hr2message(__LINE__,-1, "This Datacontainer has a valid LeaderIndex, but the Leader wasn't loaded");
         return false;
      }
   }else{
      m_pLeader = NULL;
   }

   if ( l_pDC->m_idxBigResItem >= 0)
   {
      m_pBigResItem = g_bigresitemArray + l_pDC->m_idxBigResItem;
   }else{
      m_pBigResItem = NULL;
   }
   
   if (m_ResType != l_pDC->m_ResType)
   {
      if (setRes(l_pDC->m_ResType) == false) return false;
   }

   // -- sonderfall offset
   //
   if (getGeomID() && m_pBigResItem)
   {
      if (setOffset(m_pBigResItem->m_vPos.x, m_pBigResItem->m_vPos.y, m_pBigResItem->m_vPos.z)==false) return false;
   }

   m_cntRanking                 = l_pDC->m_cntRanking;

   if (setPos(l_pDC->m_vPos)==false)     return false;
   if (setStep(l_pDC->m_vStep) == false) return false;
   if (setMat(l_pDC->m_Mat) == false)    return false;

   m_dMass                      = l_pDC->m_dMass;
   m_vPos2D                     = l_pDC->m_vPos2D;
   m_fCameraW                   = l_pDC->m_fCameraW;

   m_fMinRad2DForVisibility     = l_pDC->m_fMinRad2DForVisibility;
   m_cntGeneric                 = l_pDC->m_cntGeneric;
   m_fViewRotation1             = l_pDC->m_fViewRotation1;
   m_fViewRotation2             = l_pDC->m_fViewRotation2;
   m_fExplScale                 = l_pDC->m_fExplScale;
   m_iFlags                     = l_pDC->m_iFlags;
   m_Action                     = l_pDC->m_Action;
   m_iActionTimer               = l_pDC->m_iActionTimer;
   //g_ehdlBuf.m_vDest                      = l_pDC->g_ehdlBuf.m_vDest;
   m_fDestLength                = l_pDC->m_fDestLength;
   //m_idxLeadhist                = l_pDC->m_idxLeadhist;
   m_cntShieldFlashTimer        = l_pDC->m_cntShieldFlashTimer;
   m_fSpeed                     = l_pDC->m_fSpeed;
   m_fScale                     = l_pDC->m_fScale;
   m_fRad                       = l_pDC->m_fRad;
   m_fRad2D                     = l_pDC->m_fRad2D;
   m_fCollisionRadPow2          = l_pDC->m_fCollisionRadPow2;
   m_fShootHitRadPow2           = l_pDC->m_fShootHitRadPow2;
   //m_dsDestFrame                = l_pDC->m_dsDestFrame;

   m_fShieldAmount              = l_pDC->m_fShieldAmount;
   m_seedShipsHold              = l_pDC->m_seedShipsHold;

   if (l_pDC->m_txxDestFrameInfo >= 0 && l_pDC->m_txxDestFrameInfo < TXX_LASTENUM)
   {
      m_cDestFrameInfo = g_cLang[l_pDC->m_txxDestFrameInfo];
   }else{
      m_cDestFrameInfo = NULL;
   }

   return true;
}
*/
// -- sich selbst in den Datacontainer persistieren
//
/*
bool ENEMY::saveToDC(ENEMYDATACONTAINER* l_pDC)
{
   int l_idxLang;

   l_pDC->m_idxEnemy                = getEnemyIndex();
   l_pDC->m_vPos                    = m_vPos;
   l_pDC->m_vPos2D                  = m_vPos2D;
   l_pDC->m_fCameraW                = m_fCameraW;
   l_pDC->m_vStep                   = m_vStep;
   l_pDC->m_fMinRad2DForVisibility  = m_fMinRad2DForVisibility;
   l_pDC->mType                     = mType;
   l_pDC->m_cntGeneric              = m_cntGeneric;
   l_pDC->m_fViewRotation1          = m_fViewRotation1;
   l_pDC->m_fViewRotation2          = m_fViewRotation2;
   l_pDC->m_fExplScale              = m_fExplScale;
   l_pDC->m_iFlags                  = m_iFlags;
   l_pDC->m_Action                  = m_Action;
   l_pDC->m_iActionTimer            = m_iActionTimer;
   //l_pDC->g_ehdlBuf.m_vDest                   = g_ehdlBuf.m_vDest;
   l_pDC->m_fDestLength             = m_fDestLength;
   l_pDC->m_Mat                     = m_Mat;
   //l_pDC->m_idxLeadhist             = m_idxLeadhist;
   l_pDC->m_cntShieldFlashTimer     = m_cntShieldFlashTimer;
   l_pDC->m_cntRanking              = m_cntRanking;
   l_pDC->m_fSpeed                  = m_fSpeed;
   l_pDC->m_fScale                  = m_fScale;
   l_pDC->m_fRad                    = m_fRad;
   l_pDC->m_fRad2D                  = m_fRad2D;
   l_pDC->m_fCollisionRadPow2       = m_fCollisionRadPow2;
   l_pDC->m_fShootHitRadPow2        = m_fShootHitRadPow2;
   l_pDC->m_ResType                 = m_ResType;
   //l_pDC->m_dsDestFrame             = m_dsDestFrame;
   l_pDC->m_fShieldAmount           = m_fShieldAmount;
   l_pDC->m_seedShipsHold           = m_seedShipsHold;
   l_pDC->m_dMass                   = m_dMass;

   // -- den char Zeiger umwandeln in txx-Enum
   //
   for (l_idxLang=0; l_idxLang < TXX_LASTENUM; l_idxLang++)
   {
      if (m_cDestFrameInfo == g_cLang[l_idxLang])
      {
         l_pDC->m_txxDestFrameInfo = (ENUM_TXX_ID)l_idxLang;
      }else{
         l_pDC->m_txxDestFrameInfo = (ENUM_TXX_ID)0;
      }
   }

   // -- Zeiger in speicheradressunabhängige Indizes verwandeln
   //
   l_pDC->m_idxCharacter = getCharacterIndex();
 
   if (m_pBigResItem)
   {
      l_pDC->m_idxBigResItem = (m_pBigResItem - g_bigresitemArray);
   }else{
      l_pDC->m_idxBigResItem = -1;
   }
  
   if (m_pLeader)
   {
      l_pDC->m_idxLeader = m_pLeader->getEnemyIndex();
   }else{
      l_pDC->m_idxLeader = -1;
   }
   return true;
}
*/
// -- ruft die TreatAsX funktionen auf
//

bool ENEMY::updateAction()
{
	
   // --- Behandlungsroutinen aufrufen
   //
   switch(mType)
   {
	   
      case ETYP_LEADER:
         if (g_bWriteLogFileInLoop) 
         {
            WriteLogFile("...Leader\n");
         }

         if (TreatAsLeader() == false)
         {
            return false;
         }
         break;
		 /*
      case ETYP_STARBASE:
         if (g_bWriteLogFileInLoop) 
         {
            WriteLogFile("...StarBase\n");
         }
         if (TreatAsStarBase() == false)
         {
            return false;
         }
         break;

      case ETYP_HUNTER:
         if (g_bWriteLogFileInLoop) 
         {
            WriteLogFile("...Hunter\n");
         }
         if (TreatAsHunter() == false)
         {
            return false;
         }
         break;

      case ETYP_TRADER:
         if (g_bWriteLogFileInLoop) 
         {
            WriteLogFile("...Trader\n");
         }
         if (TreatAsTrader() == false)
         {
            return false;
         }
         break;


      case ETYP_PIRATE:
         if (g_bWriteLogFileInLoop) 
         {
            WriteLogFile("...Pirate\n");
         }
         if (TreatAsPirate() == false)
         {
            return false;
         }
         break;


      case ETYP_P_FLYER:
         if (g_bWriteLogFileInLoop) 
         {
            WriteLogFile("...Pirate-Flyer\n");
         }
         if (TreatAsPirateFlyer() == false)
         {
            return false;
         }
         break;

		 */
      case ETYP_FLYER:

         if (g_bWriteLogFileInLoop) 
         {
            WriteLogFile("...Flyer\n");
         }
         if (TreatAsFlyer() == false)
         {
            return false;
         }
         break;
		 /*
      case ETYP_TUNNELFLYER:
         if (g_bWriteLogFileInLoop) 
         {
            WriteLogFile("...TunnelFlyer\n");
         }
         if (TreatAsTunnelFlyer() == false)
         {
            return false;
         }
         break;
      case ETYP_GUN:
         if (g_bWriteLogFileInLoop) 
         {
            WriteLogFile("...Gun\n");
         }
         if (TreatAsGun() == false)
         {
            return false;
         }
         break;
      case ETYP_LASERGUN:
         if (g_bWriteLogFileInLoop) 
         {
            WriteLogFile("...LaserGun\n");
         }
         if (TreatAsLaserGun() == false)
         {
            return false;
         }
         break;

      case ETYP_LASERHALTER:
         if (g_bWriteLogFileInLoop) 
         {
            WriteLogFile("...LaserHalter\n");
         }
         if (TreatAsLaserHalter() == false)
         {
            return false;
         }
         break;
      case ETYP_REACTORGUN:
         if (g_bWriteLogFileInLoop) 
         {
            WriteLogFile("...ReactorGun\n");
         }
         if (TreatAsGun() == false)
         {
            return false;
         }
         break;

      case ETYP_MINE:
         if (g_bWriteLogFileInLoop) 
         {
            WriteLogFile("...Mine\n");
         }

         if (TreatAsMine() == false)
         {
            return false;
         }
         break;

      case ETYP_HANGAR:
         if (g_bWriteLogFileInLoop) 
         {
            WriteLogFile("...Hangar\n");
         }
         if (TreatAsHangar()==false)
         { 
            return(false);
         }
         break;
     
      case ETYP_STATIC:
         if (g_bWriteLogFileInLoop) 
         {
            WriteLogFile("...Static\n");
         }
         break;

      case ETYP_FELDGENERATOR:
         if (g_bWriteLogFileInLoop) 
         {
            WriteLogFile("...Feldgenerator\n");
         }
         if (TreatAsGenerator() == false)
         {
            return false;
         }
         break;

      case ETYP_CHAR:
         if (g_bWriteLogFileInLoop) 
         {
            WriteLogFile("...Char\n");
         }
         if (TreatAsChar() == false)
         {
            return false;
         }
         break;

       case ETYP_GHOST:
         if (g_bWriteLogFileInLoop) 
         {
            WriteLogFile("...Ghost\n");
         }
         if (TreatAsGhost() == false)
         {
            return false;
         }
         break;
		 */
       case ETYP_BIGRESITEM:
         if (g_bWriteLogFileInLoop) 
         {
            WriteLogFile("...BigResItem\n");
         }
         if (TreatAsBigResItem() == false)
         {
            return false;
         }
         break;
		 /*
       case ETYP_KINGSPLANE:
         if (g_bWriteLogFileInLoop) 
         {
            WriteLogFile("...Kingsplane\n");
         }
         if (TreatAsKingsplane() == false)
         {
            return false;
         }
         break;

       case ETYP_THARGOID:
         if (g_bWriteLogFileInLoop) 
         {
            WriteLogFile("...Thargoid\n");
         }
         if (TreatAsThargoid() == false)
         {
            return false;
         }
         break;
       case ETYP_THARGOIDDRONE:
         if (g_bWriteLogFileInLoop) 
         {
            WriteLogFile("...Thargoiddrone\n");
         }
         if (TreatAsThargoidDrone() == false)
         {
            return false;
         }
         break;
		 */
       case ETYP_PLAYER:
         if (g_bWriteLogFileInLoop) 
         {
            WriteLogFile("...Player\n");
         }
         if (TreatAsPlayer() == false)
         {
            return false;
         }
         break;
		 /*
       case ETYP_WINGMAN:
         if (g_bWriteLogFileInLoop) 
         {
            WriteLogFile("...Wingman\n");
         }
         if (TreatAsWingman() == false)
         {
            return false;
         }
         break;

       case ETYP_STARBASEHANGAR:
         if (g_bWriteLogFileInLoop) 
         {
            WriteLogFile("...Starbase-Hangar\n");
         }
         if (TreatAsStarBaseHangar() == false)
         {
            return false;
         }
         break;

       case ETYP_PLANET:
         if (g_bWriteLogFileInLoop) 
         {
            WriteLogFile("...Planet\n");
         }
         if (TreatAsPlanet() == false)
         {
            return false;
         }
         break;
		 */
   } // end-switch mType
   
   return true;
}

// -- keinerlei weitere Info
//
bool EXPLOSION::Update()
{
   if (g_bWriteLogFileInLoop) 
   {
      WriteLogFile("EXPLOSION::Update() method beginning\n");
   }
   return TreatAsExplosion();
}

EXPLOSION::EXPLOSION(LIFECYCLE l_lifecycle) : ENEMY(l_lifecycle)
{
}

bool EXPLOSION::setPos(float x, float y, float z)
{
   m_vPos = XMVectorSet(x,y,z,0.0f);
   return true;

}

bool EXPLOSION::setPos(XMVECTOR l_vVal) {
   m_vPos = l_vVal;
   return true;
};

/*

EXPLOSION::EXPLOSION(LIFECYCLE l_lifecycle, CHARACTER* l_pcharSource)  : 
   ENEMY(l_lifecycle, l_pcharSource)
{
}
*/
// -- case ENACT_CURVEAROUND:
//    Umkreisung der Raumstation
//
/*
bool ENEMY::handleCurveAroundCircles()
{
   float l_fDestLength2D;
   double l_dDegree;

   m_fViewRotation1 = 0.0f;
   m_fViewRotation2 = 0.0f;

   if (isTriggered())
   {
      m_Action = ENACT_DONOTHING;
   }

   TransformStarBase();
   // -- Starbase umrunden
   //
   if (m_Action != ENACT_ACCELERATE)
   {
      if (m_fSpeed > g_Player.m_pEnemy->m_fSpeed && m_fSpeed > 5.0f)
      {
         m_fSpeed = m_fSpeed * 0.80f;
      }
   }

   if (m_fSpeed < 5.0f)
   {
      m_fSpeed = 5.0f;
   }
   // --- Vorraussetzungen fuer die naechste Phasen pruefen
   //
   if ((g_ehdlBuf.m_vStarBase.x < 2.0f) && (g_ehdlBuf.m_vStarBase.x > (-2.0f)))
   { 
      l_fDestLength2D = sqrt(pow2(g_ehdlBuf.m_vStarBase.y) + pow2(g_ehdlBuf.m_vStarBase.z));
      l_dDegree = asin(g_ehdlBuf.m_vStarBase.y/l_fDestLength2D);

      m_fViewRotation2 = (- g_fPi * 0.5f + l_dDegree) * 0.004f;

   }else{

      // -- Fluegel richten (ex ENACT_ALIGNYHARD)
      //
      l_fDestLength2D = sqrt(pow2(g_ehdlBuf.m_vStarBase.x) + pow2(g_ehdlBuf.m_vStarBase.y));

      l_dDegree = asin(g_ehdlBuf.m_vStarBase.x/l_fDestLength2D);
      m_fViewRotation1 = l_dDegree * 0.004f;
   }

   if (setMatrixAndStepSmallShip()==false) return false;

   return true;
}
*/   
// -- Geradeaus zur Raumstation
//
/*
bool ENEMY::handleGoToStarBase()
{
   float l_fDestLength2D, l_fTolerance;
   double l_dDegree;

   m_fViewRotation1 = 0.0f;
   m_fViewRotation2 = 0.0f;

   if (isTriggered())
   {
      m_Action = ENACT_DONOTHING;
   }

   TransformStarBase();

   // -- momentane Entfernung zur StarBase
   //
   if (g_levelContent.m_penemyStarBase)
   {
      g_ehdlBuf.m_fStarBaseLength = pow2( g_levelContent.m_penemyStarBase->m_vPos.x - m_vPos.x)+
                                    pow2( g_levelContent.m_penemyStarBase->m_vPos.y - m_vPos.y)+
                                    pow2( g_levelContent.m_penemyStarBase->m_vPos.z - m_vPos.z);
   }

   // -- vor der Raumstation warten und Stehenbleiben
   //
   if( g_levelContent.m_penemyStarBase &&
       g_ehdlBuf.m_fStarBaseLength < g_levelContent.m_penemyStarBase->m_fCollisionRadPow2 * 4.0f)
   {
      m_iActionTimer = 5000;
      m_Action = ENACT_DOCKING;
   }

   l_fTolerance = 1.0f;

   // -- Schnauze hoch/runterziehen bis Spieler in Mitte 
   //    sonst erstmal Fluegel/Horizont drehen, damit der Spieler auf 
   //    Vertikale liegt
   //
   if ((g_ehdlBuf.m_vStarBase.x < l_fTolerance) && (g_ehdlBuf.m_vStarBase.x > -l_fTolerance))
   { 
      // -- sont wie ursprünglich beabsichtigt Nase hochziehen/runterziehen 
      //
      l_fDestLength2D = sqrt(pow2(g_ehdlBuf.m_vStarBase.z) + pow2(g_ehdlBuf.m_vStarBase.y));
      l_dDegree = asin(g_ehdlBuf.m_vStarBase.y/l_fDestLength2D);
      m_fViewRotation2 = - l_dDegree;
   }else{

      // -- Die Fluegelausrichtung/Horizon neigen bis Dest auf Vertikalachse
      //
      l_fDestLength2D = sqrt(pow2(g_ehdlBuf.m_vStarBase.x) + pow2(g_ehdlBuf.m_vStarBase.y));
      l_dDegree = asin(g_ehdlBuf.m_vStarBase.x/l_fDestLength2D);
      m_fViewRotation1 = l_dDegree;
   }

   if (m_fSpeed < 5.0f)
   {
      if (m_fSpeed)
      {
         m_fSpeed = m_fSpeed * 1.111f;
      }else{
         m_fSpeed = 0.1f;
      }
   }

   if (setMatrixAndStepSmallShip()==false) return false;

   return true;
}
*/
// -- Starbase meiden, geradewegs wegfliegen
//
/*
bool ENEMY::handleAvoidStarBase()
{
   float l_fDestLength2D, l_fTolerance;
   double l_dDegree;

   m_fViewRotation1 = 0.0f;
   m_fViewRotation2 = 0.0f;

   if (isTriggered())
   {
      m_Action = ENACT_DONOTHING;
   }

   TransformStarBase();

   // -- momentane Entfernung zur StarBase
   //
   if (g_levelContent.m_penemyStarBase)
   {
      g_ehdlBuf.m_fStarBaseLength = pow2( g_levelContent.m_penemyStarBase->m_vPos.x - m_vPos.x)+
                                    pow2( g_levelContent.m_penemyStarBase->m_vPos.y - m_vPos.y)+
                                    pow2( g_levelContent.m_penemyStarBase->m_vPos.z - m_vPos.z);
   }

   l_fTolerance = 1.0f;

   // -- Schnauze hoch/runterziehen bis Spieler in Mitte 
   //    sonst erstmal Fluegel/Horizont drehen, damit der Spieler auf 
   //    Vertikale liegt
   //
   if ((g_ehdlBuf.m_vStarBase.x < l_fTolerance) && (g_ehdlBuf.m_vStarBase.x > -l_fTolerance))
   { 
      // -- sont wie ursprünglich beabsichtigt Nase hochziehen/runterziehen 
      //
      l_fDestLength2D = sqrt(pow2(g_ehdlBuf.m_vStarBase.z) + pow2(g_ehdlBuf.m_vStarBase.y));
      if (g_ehdlBuf.m_vStarBase.z > 0.0f)
      {
         l_dDegree = g_fPi / 2.0f;
      }else{
         l_dDegree = asin(g_ehdlBuf.m_vStarBase.y/l_fDestLength2D);
      }
      m_fViewRotation2 = l_dDegree;
   }else{

      // -- Die Fluegelausrichtung/Horizon neigen bis Dest auf Vertikalachse
      //
      l_fDestLength2D = sqrt(pow2(g_ehdlBuf.m_vStarBase.x) + pow2(g_ehdlBuf.m_vStarBase.y));
      l_dDegree = asin(g_ehdlBuf.m_vStarBase.x/l_fDestLength2D);
      m_fViewRotation1 = l_dDegree;
   }

   if (m_fSpeed < 5.0f)
   {
      if (m_fSpeed)
      {
         m_fSpeed = m_fSpeed * 1.111f;
      }else{
         m_fSpeed = 0.1f;
      }
   }

   if (setMatrixAndStepSmallShip()==false) return false;

   return true;
}
*/
// -- ermittelt die Position innerhalb der Formation
//

bool ENEMY::getFormationPlace(int l_idxFormation, XMVECTOR* l_pvPlace)
{
   float l_fx, l_fy, l_fz;
  
   // -- Formation um den leader einnehmen
   //
   switch (l_idxFormation) 
   {
      case 0:
         // --- Zugvoegel Formation
         //
         switch (m_cntRanking) 
         {
            case 1: l_fx = -2; l_fy = -2; l_fz = 0; break;
            case 2: l_fx = -1; l_fy = -1; l_fz = 0; break;
            case 3: l_fx = +1; l_fy = -1; l_fz = 0; break;
            case 4: l_fx = +2; l_fy = -2; l_fz = 0; break;
            default:l_fx = 0; 
         }
         l_fx = l_fx * m_pLeader->m_fRad;
         l_fy = l_fy * m_pLeader->m_fRad;
         l_fz = l_fz * m_pLeader->m_fRad;
         break;

      case 1:

         // --- Kreuz Formation
         //
         switch (m_cntRanking) 
         {
            case 1: l_fx =  0; l_fy = -1; l_fz = 0; break;
            case 2: l_fx =  0; l_fy = +1; l_fz = 0; break;
            case 3: l_fx = -1; l_fy =  0; l_fz = 0; break;
            case 4: l_fx = +1; l_fy =  0; l_fz = 0; break;
            default:l_fx = 0; 
         }
         l_fx = l_fx * m_pLeader->m_fRad;
         l_fy = l_fy * m_pLeader->m_fRad;
         l_fz = l_fz * m_pLeader->m_fRad;
         break;
      case 2:

         // --- BigResItem Platz einnehmen
         //
         if (m_pBigResItem)
         {
            l_fx = m_pBigResItem->m_vPos.x;
            l_fy = m_pBigResItem->m_vPos.y;
            l_fz = m_pBigResItem->m_vPos.z;
         }else{
            l_fx = 0.0f;
            l_fy = 0.0f;
            l_fz = 0.0f;
         }
         break;
         
   }
      
   const XMFLOAT3 l_f3FormationPos = XMFLOAT3(l_fx, l_fy, l_fz);
   
   
   XMVECTOR l_vFormationPos = XMLoadFloat3(&l_f3FormationPos);
   
   XMVECTOR l_vFormationWorldPos = XMVector3Transform(l_vFormationPos, m_pLeader->m_Mat) + m_pLeader->m_vPos;
   *l_pvPlace = l_vFormationWorldPos;

   /*
   l_pvPlace->x = m_pLeader->m_vPos.x +
                  m_pLeader->m_Mat._11 * l_fx +
                  m_pLeader->m_Mat._21 * l_fy +
                  m_pLeader->m_Mat._31 * l_fz;
   l_pvPlace->y = m_pLeader->m_vPos.y +
                  m_pLeader->m_Mat._12 * l_fx +
                  m_pLeader->m_Mat._22 * l_fy +
                  m_pLeader->m_Mat._32 * l_fz;
   l_pvPlace->z = m_pLeader->m_vPos.z +
                  m_pLeader->m_Mat._13 * l_fx +
                  m_pLeader->m_Mat._23 * l_fy +
                  m_pLeader->m_Mat._33 * l_fz;
   */
   return true;
}


bool ENEMY::handleFollowLeader(int l_idxFormation)
{
   if(m_pLeader)
   {
      XMVECTOR l_vPlace, l_vDirection;
	  XMFLOAT3 l_float3Direction;

      float l_fDestLength2D,
            l_fTolerance_x, l_fTolerance_y;

      double l_dDegree;

      if (m_pLeader->m_Action == ENACT_SHOOT)
      {
         m_Action = ENACT_SHOOT;
      }

      if (getFormationPlace(l_idxFormation, &l_vPlace)==false) return false;

	  l_vDirection = 

      // -- l_vDirection in Enemyraum versetzen
      //      
	  l_vDirection = XMVector3Transform(l_vPlace - m_vPos, m_Mat);
      m_fSpeed = m_pLeader->m_fSpeed * 0.9f;

      // -- Geschwindigkeit nach Entfernung justieren
      //
      m_fViewRotation1 = 0.0f;
      m_fViewRotation2 = 0.0f;

	  XMStoreFloat3(&l_float3Direction, l_vDirection);

      // -- Schnauze hoch/runterziehen bis Spieler in Mitte 
      //    sonst erstmal Fluegel/Horizont drehen, damit der Spieler auf 
      //    Vertikale liegt
      //
      l_fTolerance_x = 50.0f;
      l_fTolerance_y = 20.0f;
      if ((l_float3Direction.x < l_fTolerance_x) && (l_float3Direction.x > -l_fTolerance_x))
      {  
         // -- kein Handlungsbedarf, wenn Zielposition im Fenster
         //
         if (l_float3Direction.y < l_fTolerance_y && l_float3Direction.y > -l_fTolerance_y)
         {
            // -- aufschliessen oder zurückfallen lassen
            //
            if (l_float3Direction.z > 50.0f)
            {
               m_fSpeed = m_pLeader->m_fSpeed * 1.0f;
            }


         }else{
            // -- sonst wie ursprünglich beabsichtigt Nase hochziehen/runterziehen 
            //
            l_fDestLength2D = sqrt(pow2(l_float3Direction.z) + pow2(l_float3Direction.y));
            l_dDegree = asin(l_float3Direction.y/l_fDestLength2D);

            if (l_float3Direction.z > 0.0f)
            {
               m_fViewRotation2 = - l_dDegree;//* g_fElapsedTime;
            }else{
   
               if (l_dDegree > 0.0f)
               {
                  m_fViewRotation2 = (- M_PI + l_dDegree);// * 0.004f;
               }else{ 
                  m_fViewRotation2 = (+M_PI + l_dDegree);// * 0.004f;
               }
            }
         }
      }else{

         l_fDestLength2D = sqrt(pow2(l_float3Direction.x) + pow2(l_float3Direction.y));

         // -- Die Fluegelausrichtung/Horizon neigen bis Dest auf Vertikalachse
         //
         l_dDegree = asin(l_float3Direction.x/l_fDestLength2D);
         m_fViewRotation1 = l_dDegree;//* g_fElapsedTime;
         m_fViewRotation2 = 0.0f;
      }

   }else{
      m_Action = ENACT_DONOTHING; //leader verloren => Neuentscheidung
   }

   if (setMatrixAndStepSmallShip()==false) return false;

   return true;
}

// -- setter für idxCharacter, mit versuch den pointer mitzusetzen
//
/*
bool ENEMY::setCharacterIndex(int l_idxNew)
{
   m_idxCharacter = l_idxNew;

   // -- schonmal nachsehen, ob der Character Record da ist - muss aber nicht
   //
   if ( m_idxCharacter >= 0 )
   {
      m_pCharacter = CHARACTER::getByIndex(m_idxCharacter);
   }else{
      m_pCharacter = NULL;
   }

   return true;
}

// -- einfacher getter
//
int  ENEMY::getCharacterIndex()
{
   return m_idxCharacter;
}

// -- den Pointer auf den Character zurückliefern, evtl. erst ermitteln
//
CHARACTER* ENEMY::getCharacter()
{
   if (m_pCharacter == NULL && m_idxCharacter >= 0)
   {
      m_pCharacter = CHARACTER::getByIndexDebug(m_idxCharacter);
      if (m_pCharacter == NULL) 
      {
         hr2message(__LINE__,-1, "try to get Characterpointer by Index, but the Character wasn't loaded");
         m_pCharacter = NULL;
      }
   }
   return m_pCharacter;
}
*/
bool ENEMY::setStep(float x, float y, float z)
{
   //PSXVECTOR l_vValue;
   //l_vValue.x = x;
   //l_vValue.y = y;
   //l_vValue.z = z;
   //return setStep(l_vValue);
   if (m_dBodyID) dBodySetLinearVel(m_dBodyID, x, y, z);
   return true;
}

bool ENEMY::setStep(XMVECTOR l_vVal) {
   XMFLOAT3 l_float3Pos;
   XMStoreFloat3(&l_float3Pos, l_vVal);
   return setPos(l_float3Pos.x, l_float3Pos.y, l_float3Pos.z);

   //m_vStep = l_vVal;
   //if (m_dBodyID) dBodySetLinearVel(m_dBodyID, l_vVal.x, l_vVal.y, l_vVal.z);
   //return true;
};

bool ENEMY::setPos(float x, float y, float z)
{
   //m_vPos = XMVectorSet(x,y,z,0.0f);
	if (m_dGeomID)
	{
		dGeomSetPosition(m_dGeomID, x, y, z);
	}else{
		hr2message(__LINE__, -1, "m_dGeomID not set in ENEMY::setPos()");
		return false;
	}
	if (m_dBodyID) {
		dBodySetPosition(m_dBodyID, x, y, z) ;
	} else {
		hr2message(__LINE__, -1, "m_dBodyID not set in ENEMY::setPos()");
		return false;
	}
	return true;

}

bool ENEMY::setPos(XMVECTOR l_vVal) {
	XMFLOAT3 l_float3Pos;
   //m_vPos = l_vVal;
   XMStoreFloat3(&l_float3Pos, m_vPos);
   return setPos(l_float3Pos.x, l_float3Pos.y, l_float3Pos.z);
   
};

bool ENEMY::setMat(CXMMATRIX l_matVal) {
   m_Mat = l_matVal;
   return setBodyMat();

};

bool ENEMY::copyPos(ENEMY* l_pEnemy) {
   return setPos(l_pEnemy->m_vPos);
};

bool ENEMY::copyMat(ENEMY* l_pEnemy) {
   m_Mat = l_pEnemy->m_Mat;
   return setBodyMat();


};


bool ENEMY::setBodyMat()
{
	XMFLOAT4X4 l_float4x4Enemy ;
   dMatrix3 l_dMatrix3;
   XMStoreFloat4x4(&l_float4x4Enemy, m_Mat);

   l_dMatrix3[0] = l_float4x4Enemy._11;
   l_dMatrix3[1] = l_float4x4Enemy._21;
   l_dMatrix3[2] = l_float4x4Enemy._31;
   l_dMatrix3[3] = l_float4x4Enemy._41;
   l_dMatrix3[4] = l_float4x4Enemy._12;
   l_dMatrix3[5] = l_float4x4Enemy._22;
   l_dMatrix3[6] = l_float4x4Enemy._32;
   l_dMatrix3[7] = l_float4x4Enemy._42;
   l_dMatrix3[8] = l_float4x4Enemy._13;
   l_dMatrix3[9] = l_float4x4Enemy._23;
   l_dMatrix3[10] = l_float4x4Enemy._33;
   l_dMatrix3[11] = l_float4x4Enemy._43;
   if (m_dBodyID) dBodySetRotation(m_dBodyID, l_dMatrix3);
   return true;
}

// --- Collisionhandling, wird aus dNearCallback aufgerufen =>keine bodyrelevanten Aufrufe
//

bool ENEMY::Collision(bool *l_pbBounce, ENEMY* l_penemyOther, int l_cntContact)
{

	float l_fShieldLoss;
   
   bool l_bExplosive, l_bAnnoying;

   if (g_bWriteLogFileInLoop)
   {
      WriteLogFile("::Collision\n");
   }

   
   // -- bei BigResItems gleich hochreichen
   //
   if (m_pLeader && isPartOfLeader())
   {
      return m_pLeader->Collision(l_pbBounce, l_penemyOther, l_cntContact);
   }

   if (l_penemyOther->isPlayer())
   {
      // -- kollision während des Hyperraumsprungs lässt diesen abbrechen
      //
	   /*
      if (g_Player.getGS()->getIndex() == GAMESCREEN_WITCHSPACE)
      {
         if (g_Player.getGS()->setIndex(GAMESCREEN_OWNSHIP) == false) return false;
         if (PSXInitializeTerminal(g_cLang[txxWitchSpaceJumpAborted],
                                   NULL,
                                   NULL,
                                   NULL) == false)
         {
            return false;
         }
      }
	  */
      // -- flags setzen
      //
      switch (mType)
      {
         case ETYP_ASTEROID:
            l_fShieldLoss = 00.0f;
            l_bExplosive  = false;
            *l_pbBounce   = true;
            l_bAnnoying   = false;
   
            break;

         case ETYP_WINGMAN:
            l_fShieldLoss = 00.0f;
            l_bExplosive  = false;
            l_bAnnoying   = false;

            // -- nur wenn nicht camera-enemy, dann wird auch TreatAsWingman ausgeführt
            //
            if (g_penemyInput == this)
            {
               *l_pbBounce   = true;
            }else{
               *l_pbBounce   = false;
			   /*
               m_Action = ENACT_BECOMELEADERSPART;
			   */
            }
            break;
   
         case ETYP_CARGOBOX:
   
            // -- Einsammeln der CargoBox geht nur mit dem Greifarm
            //
			 /*
            if (g_equipmentArray[EQUIPMENT_FUELSCOOPS].isEnabled() || g_bCheatGodMode)
            { 
               l_fShieldLoss     = 0.0f;
               l_bExplosive      = false;
               *l_pbBounce       = false;
               l_bAnnoying       = false;
             
               if(PSXgenCargo(m_seedShipsHold))
               {
                  if (CollectEquipment() == false) return false;

               }
            }else{
			*/
			 l_fShieldLoss = 23.5f;
			 l_bExplosive = false;
			 *l_pbBounce = true;
			 l_bAnnoying = false;

           
            break;
   
         case ETYP_DOOR:
            l_fShieldLoss = 00.0f;
         
            l_bAnnoying       = false;
   
            // -- rueckseitige Schotts explodieren bei Beruehrung von selber
            //    wenn Eingangsschott bereits weggeschossen
            //
			/*
            if ((m_cntRanking == 1 || m_cntRanking == 3) && (m_pLeader == NULL))
            {
               l_bExplosive      = false;
               *l_pbBounce       = true;
   
            }else{
				*/
			l_bExplosive = false;
			*l_pbBounce = false;

            break;
   
         case ETYP_MINE:
            l_fShieldLoss     = 10.0f;
            l_bExplosive      = true;
            *l_pbBounce       = false;
            l_bAnnoying       = false;
   
            break;
   
         case ETYP_EQUIPMENT:
            l_fShieldLoss     = 0.0f;
            l_bExplosive      = false;
            *l_pbBounce       = false;
            l_bAnnoying       = false;
/*
            if (CollectEquipment() == false) return false;
*/
            break;
   
         case ETYP_KINGSPLANE:
            l_fShieldLoss     = 0.0f;
            l_bExplosive      = false;
            l_bAnnoying       = false;
            *l_pbBounce       = false;
/*
            if (l_penemyOther->m_Action != ENACT_LEAVEDOCK) 
            {
               if (g_Player.m_bEntered)
               {
                  g_Player.m_bEntered = false;
                  *l_pbBounce       = false;
                  l_penemyOther->m_Action = ENACT_LEAVEDOCK;
                  l_penemyOther->m_iActionTimer = 5000;
                  // LEAVEDOCK Klippo, um Zeit zu haben die Kingsplane mit dem ganzen Schiff zu durchlaufen
      
               }else{
      
                  // -- falls der Spieler mit der Rettungskapsel andockt => gerettet,neues Schiff
                  //
                  if (g_Player.getRescue())
                  {
                     *l_pbBounce       = false;
      
                     g_Player.setRescue(false); // gerettet!
                     g_Player.m_fShieldAmount = 100.0f;
      
                     if (g_Player.m_crimePoliceFile > CRID_CLEAN)
                     {
                        g_Player.m_crimePoliceFile = CRID_CLEAN;
                        if (CHARACTER::IsAnyPolice())
                        {
                           (CHARACTER::getCharacterPolice())->m_iAction = CHRACT_DONOTHING;
                        }
      
                        if (g_Player.payCrimeTicket() == false) return false;
                     }
      
                     // -- solange nicht durch payCrimeTicket gameover eingeleitet wurde
                     //
                     if (g_Player.m_bGameOver == false) // && g_Player.m_bDetonationValid == false)
                     {
                        g_Player.m_bEntered = true;
                        
                        //if (g_Player.getGS()->setIndex(GAMESCREEN_MARKET) == false) return false;
                        l_penemyOther->m_Action = ENACT_DOCKING;
                     }
      
                  }else{
      
                     // -- Andocken, wenn Polizeiakte sauber 
                     //    Scan durchgeführt und überhaupt noch Polizeischiffe da
                     //
                     if (g_Player.m_crimePoliceFile >= CRID_OFFENDER &&
                         g_Player.m_bPoliceScanPassed == false &&
                         m_cntLeadersDestroyed < m_cntLeadersToDestroy)
                     {
                        *l_pbBounce       = true;
      
                        if (PSXInitializeTerminal(g_cLang[txxPoliceScanMustBePassed],
                                                  NULL,
                                                  NULL,
                                                  NULL) == false)
                        {
                           return false;
                        }
                     }else{
                        *l_pbBounce       = false;
                        g_Player.m_bEntered = true;
                        //if (g_Player.getGS()->setIndex(GAMESCREEN_MARKET) == false) return false;
                        l_penemyOther->m_Action = ENACT_DOCKING;
                     }
                  } // end-if Rescue
               } //end-if m_bEntered
            }else{
               *l_pbBounce       = false;
            } // end-if not leaving dock
			*/
            break;
   
         default:
			 l_fShieldLoss = 0.01f; /* g_fElapsedTime * 0.01f; */
            l_bExplosive  = false;
            *l_pbBounce   = true;
            l_bAnnoying   = false;


            break;
      }

      if (l_bAnnoying)
      {
         //g_Player.m_fRumbleScale = 0.02f;
         g_Player.m_pEnemy->m_fSpeed = 0.0f;
		 /*
         m_cntShieldFlashTimer = 1;
         */
      }
    
      if (*l_pbBounce) 
      {
         // -- in diesem Falle doch nicht mehr bouncen, sondern lieber explodieren
         //    bevor nämlich dWorldQuickStep abstürzt
         //
         if (l_cntContact > 2000)
         {
            *l_pbBounce    = false;
            l_bExplosive = true;
         }

         // -- Aussenansicht 
         //
		 /*
         if (g_Player.getGS()->getIndex()== GAMESCREEN_COCKPIT)
         {
            if (g_Player.getGS()->setIndex(GAMESCREEN_3RDPARTYVIEW) == false) return false;
         }
		 */
      }

      if (l_bExplosive)  
      {
         // -- Polizistenmord bzw. Zerstörung von Regierungseigentum,
         //    Tötung unschuldiger ruft die Sicherheitskraefte auf den Plan
         //
         if (mType == ETYP_LEADER || 
             mType == ETYP_FLYER || 
             mType == ETYP_TRADER || 
             mType == ETYP_STARBASE || 
             mType == ETYP_HANGAR)
         { 
           /*
            if ( g_Player.m_crimePoliceFile == CRID_CLEAN )
            {
               g_Player.setPoliceFile(CRID_OFFENDER);
            }
			*/
            // -- Der Enemy wird nun dem Policecharacter untergeordnet
            //    und dann wird dieser gerufen, um zur neuen Situation
            //    zu entscheiden
            //
			/*
            if (CHARACTER::IsAnyPolice())
            {
               if (setCharacterToPolice() == false) return false;
               (getCharacter())->m_bHatePlayer   = true;
               (getCharacter())->m_bAmILame      = false;
               (getCharacter())->m_iAction = CHRACT_ATTACKPLAYER;
               (getCharacter())->m_bUnspokenAction = true;
            }else{
               hr2message(__LINE__,-1,"Police-Character is not defined, but needed in ENEMY::Collision()\n");
               return false;
            }
			*/
         }
		 /*
         if(mType==ETYP_GHOST)
         {
             g_Player.m_fShieldAmount = 0.0f;
         }

         m_Action = ENACT_DETONATE;
		 */
         
      }
	  /*
      g_Player.m_fShieldAmount = g_Player.m_fShieldAmount - l_fShieldLoss;
	  */
   }else{ // wenn der Other-Enemy nämlich doch kein Player ist, sondern neuerdings einfach ein anderer Enemy

      *l_pbBounce = true;
   }
   /*
   if (*l_pbBounce)
   {
      m_iActionTimer = 1000;
      m_Action = ENACT_REFLECTION;

      l_penemyOther->m_iActionTimer = 1000;
      l_penemyOther->m_Action = ENACT_REFLECTION;

   }
   */
   return true;
}

// -- statische Funktion, die einem potenziellen Zusammenstoss nachgeht
//    und zurückgerufen wird von dSpaceCollide() benutzt in 
//    GAME::Update() für die enemy/enemy collision
//
void ENEMY::dNearCallback(void *data, dGeomID o1, dGeomID o2)
{
   int l_idxContact, l_cntContact;
   dContact l_dContact;
   dJointID l_dJointID;
   bool l_bBounce= false;

   ENEMY* l_pEnemy1; 
   ENEMY* l_pEnemy2; 

   l_pEnemy1 = reinterpret_cast<ENEMY*>(dGeomGetData(o1));
   l_pEnemy2 = reinterpret_cast<ENEMY*>(dGeomGetData(o2));

   if (l_pEnemy1 && l_pEnemy2 && dAreConnectedExcluding (l_pEnemy1->m_dBodyID,l_pEnemy2->m_dBodyID,dJointTypeContact)) return;  
   // -- falls die enemies sowieso zum selben rigid-body gehören, Collision irrelevant
   //
   if (l_pEnemy1->m_dBodyID == l_pEnemy2->m_dBodyID)
   {
      return;
   }

   // -- konstante Werte in dContact vorbereiten
   //
   memset(&l_dContact, 0, sizeof(l_dContact));
   l_dContact.surface.mode = dContactBounce;//| dContactSoftCFM;
   l_dContact.surface.mu = 0.5f;
   l_dContact.surface.mu2 = 0;
   l_dContact.surface.bounce = 1.0f;
   l_dContact.surface.bounce_vel = 0.0f;
   //l_dContact.surface.soft_cfm = 1e-9;

   // -- für jede Berührung den Contact-Joint anlegen
   //
   l_cntContact = dCollide(o1,o2, g_meshresManager.getMaxFaces(), g_contactgeomArray, sizeof(g_contactgeomArray[0]));

   // -- betroffene enemies über die kollision informieren
   //
   if (l_cntContact)
   {

      // -- vor nächster Prüfung evtl. BigResItems auflösen
      //
      if (l_pEnemy1->m_pLeader && l_pEnemy1->isPartOfLeader())
      {
         l_pEnemy1 = l_pEnemy1->m_pLeader;
      }

      if (l_pEnemy2->m_pLeader && l_pEnemy2->isPartOfLeader())
      {
         l_pEnemy2 = l_pEnemy2->m_pLeader;
      }

      // -- Player muss immer als pOther Parameter übergeben werden
      //
      if (l_pEnemy1->isPlayer())
      {
         if (l_pEnemy2->Collision(&l_bBounce, l_pEnemy1, l_cntContact) == false) return;
      }else{
         if (l_pEnemy1->Collision(&l_bBounce, l_pEnemy2, l_cntContact) == false) return;
      }
   }

   // -- wenn diese beide auf bounce votieren, dann die kontaktjoints in ode erzeugen
   //
   if (l_bBounce)
   {
      // -- falls diese bedrohlichen Zahlen bis hierher vordringen
      //
      if (l_cntContact > 2000) 
      {
         //l_cntContact = 1000;
         char l_cSuspicious[300];
         PSXPrintf(__LINE__, l_cSuspicious, sizeof(l_cSuspicious),
                   "suspicious much contacts (%i), this may crash in ode\n",
                   l_cntContact);
         
         WriteLogFile(l_cSuspicious);
		 g_bWriteLogFileInLoop = true;
		 /*
         g_options.set_bWriteLogFileInLoop(true);
		 */
      }
     
      // -- falls es noch Sinn macht, hier durch Rum-gebounce die Geoms wieder
      //    auseinander zu bekommen, oder ob nicht kurzer Prozess gemacht werden soll
      //
      if (l_cntContact < 1000)
      {
         for (l_idxContact = 0; l_idxContact < l_cntContact; l_idxContact++)
         {
            l_dContact.geom           = g_contactgeomArray[l_idxContact];
   
            l_dJointID = dJointCreateContact(LEVEL::m_iWorldID, LEVEL::m_dJointGroupID, &l_dContact);
            //dJointAttach(l_dJointID, l_pEnemy1->m_dBodyID, l_pEnemy2->m_dBodyID);
            dJointAttach(l_dJointID, dGeomGetBody(o1), dGeomGetBody(o2));
         }
   
      }
   }

}

// -- baut unter enemies dran
//
/*
bool ENEMY::checkAttachedEnemies()
{
   WriteLogFile(g_cEnemyTypeNameArray[mType]);
   WriteLogFile("::checkAttachedEnemies() method beginning\n");
   
   switch (m_ResType)
   {
      // -- Zusatzteile der Händlerschiffe anhängen
      //
      case RESTYPE_CARGO03:
         WriteLogFile("calling BIGRES_CARGO03::attachEnemies()\n");
         if (g_bigresArray[BIGRES_CARGO03].attachEnemies(this)) return false;
         break;
      case RESTYPE_CARGO08:
         WriteLogFile("calling BIGRES_CARGO08::attachEnemies()\n");
         if (g_bigresArray[BIGRES_CARGO08].attachEnemies(this)) return false;
         break;

      // -- Anlegen von dem Laser/Laserhalter paar
      //
      case RESTYPE_CARGO01:
      case RESTYPE_CARGO04:
      case RESTYPE_CARGO05:
      case RESTYPE_CARGO06:
      case RESTYPE_CARGO07:
      case RESTYPE_TRANSPORT1:
      case RESTYPE_BOA01:

         WriteLogFile("attaching Laserguns to RESTYPE_BOA01\n");
         if (armWithLaserGuns() == false) return false;
         break;
         

      case RESTYPE_COBRA_PART1:
         WriteLogFile("calling BIGRES_COBRA_PART1::attachEnemies()\n");
         if (g_bigresArray[BIGRES_COBRA].attachEnemies(this) == false) return false;
      
         if (g_iShipsHoldArray[GOOID_LARGECARGOBAY] > 0)
         {
            // -- Container an die Cobra klemmen
            //
            int l_idxBox, l_cntBox;
            float l_fScale = 1.0f;
            PSXVECTOR l_vBoxArray[] = {
               {  0.0, -8.199995, -50.699799},
               {  0.0, -8.199995, -28.900072}, 
               { +5.799997, 4.499998, -50.699799},
               { -5.799997, 4.499998, -50.699799},
               { +5.799997, 4.499998, -28.900072}, 
               { -5.799997, 4.499998, -28.900072}, 
            };
            
            WriteLogFile("checking Cargoboxes\n");
      
            l_cntBox = g_iShipsHoldArray[GOOID_LARGECARGOBAY];
            if (l_cntBox > 6) l_cntBox = 6;
            for (l_idxBox = 0; l_idxBox < l_cntBox; l_idxBox++)
            {
               ENEMY* l_penemyCargo;
      
               bool l_bAlreadyThere;
      
      
               // -- Prüfen, ob korrespondierender Enemy schon angelegt
               //
               l_bAlreadyThere = false;
               l_penemyCargo = getNextEnemy();
               while (l_penemyCargo)
               {
                  if (l_penemyCargo->m_pLeader == this &&
                      l_penemyCargo->mType == ETYP_CARGOBOX && 
                      l_penemyCargo->m_cntRanking == l_idxBox)
                  {
                     l_bAlreadyThere = true;
                     l_penemyCargo = NULL; // schleifenabbruch
                  }else{
                     l_penemyCargo = l_penemyCargo->getNextEnemy();
                  }
               }
      
               if (l_bAlreadyThere == false)
               {
                  l_penemyCargo = new (LIFECYCLE_LEVEL) ENEMY(LIFECYCLE_LEVEL);
                  if (l_penemyCargo == NULL) return false;
                 
                  l_penemyCargo->SetType(ETYP_CARGOBOX);
                  l_penemyCargo->m_pLeader = this;
                  l_penemyCargo->setPartOfLeader(true);
                  l_penemyCargo->setRes(RESTYPE_CARGOBOX); // nochnmal extra, damit Geom angelegt wird
                  l_penemyCargo->m_cntRanking = l_idxBox;
                  l_penemyCargo->m_fScale = 1.0f;
           
                  if (l_penemyCargo->getGeomID())
                  {
                     l_penemyCargo->setOffset(l_vBoxArray[l_idxBox].x,
                                              l_vBoxArray[l_idxBox].y,
                                              l_vBoxArray[l_idxBox].z);
                  }
               }
            }// end-for l_idxBox
         }
         break;
      
   }
   WriteLogFile("done checkAttachedEnemies()\n");

   return true;
}

*/
