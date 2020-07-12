/*
 *------------------------------------------------------------------------
 *      Object: player.cpp 
 *         $Id: player.cpp,v 1.36 2010/06/01 17:25:53 molle Exp $
 *        Date: Oct 2005
 *       Notes: Player methoden
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

void PLAYER::Init()
{
/*
   // -- Aussenansichtsmatrix wieder auf vernünftigen Wert setzen
   //
   {
      float l_fViewRotation1, l_fViewRotation2;
   
      l_fViewRotation2 = - g_fPi * 0.10f;
      l_fViewRotation1 = 0.0f; //g_fPi / 2.0f;
   
      memcpy (&g_matGeneric1,&g_matIdentity, sizeof(g_matIdentity));
      g_matGeneric1._11 = cos ( l_fViewRotation1);
      g_matGeneric1._13 = - sin ( l_fViewRotation1 );
      g_matGeneric1._31 = sin ( l_fViewRotation1 );
      g_matGeneric1._33 = cos ( l_fViewRotation1 );
   
      memcpy (&g_matGeneric2,&g_matIdentity, sizeof(g_matIdentity));
      g_matGeneric2._22 = cos ( l_fViewRotation2 );
      g_matGeneric2._23 = sin ( l_fViewRotation2 );
      g_matGeneric2._32 = - sin ( l_fViewRotation2 );
      g_matGeneric2._33 = cos ( l_fViewRotation2 );
   
      matmult(&g_matCameraRotate, &g_matGeneric1, &g_matGeneric2);
   }

   m_idxGalaxy = 0; 

   setLevelIndex(7); //LAVE
   setLevelDestIndex(7); //LAVE
   */
   setLevelIndex(0); //LAVE
   setLevelDestIndex(0); //LAVE
/*

   m_bRescue = false;
   m_bGameOver = false;
   m_bEntered = false;
   m_bEnableBooster = false;
   //m_bWingman = false;

   m_idxGalaxy = 0;

   m_iFluct = int(GetTickCount()) & 0xFF;
   
   m_maxFuel            = 7.0f;
   m_iBankAccount       = 1000; // entspricht 100 Credits
   m_idxCondition       = COID_GREEN;
   m_crimePoliceFile    = CRID_CLEAN;
   m_idxClassifications = CLID_HARMLESS;
   */

   // -- Punkte und Schild
   //
   m_iFightExperience    = 0;
   m_fShieldAmount       = 100.0f;
   /*
   m_cntShieldBlink      = 0;

   m_bNearStarBase = false;
   //m_bDetonationValid=false;
   m_pEnemy=NULL;

   m_bAutopilotDestValid = false;
   m_bPiratesInserted = false;
*/
}

// -- alle Updates, welche nicht den Player bewegen
//
/*
bool PLAYER::Update()
{
   if (g_options.IsWriteLogFileInLoop()) 
   {
      WriteLogFile("PLAYER::Update()\n");
   }

   if (g_equipmentArray[EQUIPMENT_SHIELDGENERATOR].isEnabled())
   {
      m_fShieldAmount = m_fShieldAmount + 0.002f * g_fElapsedTime;
   }else{
      m_fShieldAmount = m_fShieldAmount + 0.001f * g_fElapsedTime;
   }

   if (m_fShieldAmount > 100.0f)
   {
      m_fShieldAmount = 100.0f;
   }

   if (g_options.IsWriteLogFileInLoop()) 
   {
      WriteLogFile("update player values (shield, bonus, score)\n");
   }

   if (g_options.IsWriteLogFileInLoop()) 
   {
      WriteLogFile("compass\n");
   }

   // -- Kompass Richtungspfeil bestimmen und starbase-entfernungabhängige flags setzen
   //
   if (m_pEnemy)
   {
      float l_fLength;
      PSXVECTOR l_vPlayerStarBaseRel;
      D3DMATRIX l_matCompass;

      l_vPlayerStarBaseRel.x = m_pEnemy->getPosX() + m_pEnemy->getStepX() - g_levelContent.m_vPos.x;
      l_vPlayerStarBaseRel.y = m_pEnemy->getPosY() + m_pEnemy->getStepY() - g_levelContent.m_vPos.y;
      l_vPlayerStarBaseRel.z = m_pEnemy->getPosZ() + m_pEnemy->getStepZ() - g_levelContent.m_vPos.z;

      l_fLength =sqrt(pow2(l_vPlayerStarBaseRel.x) + 
                      pow2(l_vPlayerStarBaseRel.y) + 
                      pow2(l_vPlayerStarBaseRel.z));

      // -- Failsafe: Falls man von der Raumstation weg ist , aber die Kingsplane
      //    nicht gegriffen hatte wird hier knallhart der Zustand bereinigt
      //
      if (g_levelContent.m_penemyStarBase)
      {
         if (m_bEntered && l_fLength > g_levelContent.m_penemyStarBase->getRad())
         {
            m_bEntered = false;
            WriteLogFile("far from starbase but m_bEntered was set\n");
         }
      }

      if (g_options.IsWriteLogFileInLoop()) 
      {
         WriteLogFile("check if Thargoid content\n");
      }


      // --- ThargoidenStatus checken
      //
      if (g_levelContent.m_idxContent == LEVELCONTENT_THARGOID)
      {
         if(ENEMY::m_cntThargoidDrone==0 && ENEMY::m_cntThargoid==0)
         {
            m_bEnableWitchSpace=true;
            m_bEnableBooster   =true;
            m_bNearStarBase    =true;
         }else{
            m_bEnableWitchSpace=false;
            m_bEnableBooster   =false;
            m_bNearStarBase    =false;
         }

			if (g_Player.getGS()->isHavingTextOverlay())
			{
				if (g_Player.getGS()->applyUpdateText(g_Player.getGS()->getIndex())==false)
				{
					return false;
				}
			}


      }else{
  
         if (g_options.IsWriteLogFileInLoop()) 
         {
            WriteLogFile("update dependencies\n");
         }

         // -- Player kann Hyperraumsprung erst ab einer bestimmten Distanz ausführen
         //
         if(l_fLength<g_levelContent.m_pBigRes->m_fRad * 10.0f)
         {
            m_bEnableWitchSpace = false;
         }else{
            m_bEnableWitchSpace = true;
         }
   
         // -- Player kann Booster erst ab einer bestimmten Distanz ausführen
         //
         if(l_fLength<g_levelContent.m_pBigRes->m_fRad * 10.0f)
         {
            if (g_bCheatGodMode)
            {
               m_bEnableBooster = true;
            }else{
               m_bEnableBooster = false;
            }
         }else{
            m_bEnableBooster = true;
         }

         if (g_options.IsWriteLogFileInLoop()) 
         {
            WriteLogFile("check security zone \n");
         }

         // -- Player im Schutzbereich der Sternenbasis
         //
         if(l_fLength<g_levelContent.m_pBigRes->m_fRad * 10.0f)
         {
            if (g_options.IsWriteLogFileInLoop()) 
            {
               WriteLogFile("in zone \n");
            }
   
            if(m_bNearStarBase == false)
            {
               m_bNearStarBase = true;
               //g_levelContent.m_bCheckPlayerCollision = true;
      
               g_cMissionTextArr[0] = g_cLang[txxWelcomeToPlanetSystem];
               g_cMissionTextArr[1] = m_pLevelDest->m_cName;
               g_fcntMissionTextTimer = 5.0f;
   
               //g_levelArray[m_idxLevel].insertDefensePos();

					if (g_Player.getGS()->isHavingTextOverlay())
					{
						if (g_Player.getGS()->applyUpdateText(g_Player.getGS()->getIndex())==false)
						{
							return false;
						}
					}
            }
   
            g_arrFadeStat[BBID_STA_DOCKING_MODE]=FID_AWAY;
            g_arrFadeStat[BBID_STA_DEFENSE_MODE]=FID_AWAY;
       
         }else{
   
            if (g_options.IsWriteLogFileInLoop()) 
            {
               WriteLogFile("out of zone \n");
            }

            g_arrFadeStat[BBID_STA_DOCKING_MODE]=FID_PRESENT;
            g_arrFadeStat[BBID_STA_DEFENSE_MODE]=FID_PRESENT;
   
            if(m_bNearStarBase)
            {
               m_bNearStarBase = false;
   
               m_pLevel->deleteDefensePos();

					if (g_Player.getGS()->isHavingTextOverlay())
					{
						if (g_Player.getGS()->applyUpdateText(g_Player.getGS()->getIndex())==false)
						{
							return false;
						}
					}
            }
   
            // -- falls der Spieler grad gejagt wurde, wird er an die Kopfgeldjäger 
            //    gemeldet
            //
            if (m_crimePoliceFile == CRID_OFFENDER)
            {
               setPoliceFile(CRID_FUGITIVE);
            }
   
         } // end-if im flagschiffradius
      }


      if (g_options.IsWriteLogFileInLoop()) 
      {
         WriteLogFile("check target\n");
      }

      // -- Pfeil für die Starbase
      //
      if (g_bTargetLocked == false)
      {
         l_vPlayerStarBaseRel.x = l_vPlayerStarBaseRel.x / -l_fLength;
         l_vPlayerStarBaseRel.y = l_vPlayerStarBaseRel.y / -l_fLength;
         l_vPlayerStarBaseRel.z = l_vPlayerStarBaseRel.z / -l_fLength;
   
         if (PSXMatrixFromVector( &l_matCompass, l_vPlayerStarBaseRel) == false)
         {
            return false;
         }
   
         memcpy (&g_matCompassYZ,&g_matIdentity, sizeof(g_matIdentity));
         g_matCompassYZ._11 = l_matCompass._11;
         g_matCompassYZ._12 = l_matCompass._12;
         g_matCompassYZ._13 = l_matCompass._13;
   
         g_matCompassYZ._21 = l_vPlayerStarBaseRel.x;
         g_matCompassYZ._22 = l_vPlayerStarBaseRel.y;
         g_matCompassYZ._23 = l_vPlayerStarBaseRel.z;
   
         g_matCompassYZ._31 = l_matCompass._21;
         g_matCompassYZ._32 = l_matCompass._22;
         g_matCompassYZ._33 = l_matCompass._23;
      }

   }

   if (g_options.IsWriteLogFileInLoop()) 
   {
      WriteLogFile("check Temperature Blink\n");
   }

   // -- blinken der Lasertemperatur
   //
   if (m_cntTemperatureBlink>0)
   {
      m_cntTemperatureBlink = m_cntTemperatureBlink - 1;

   }else {
      if (g_pequipmentSelectWeapon->m_fTemperature > 50.0f)
      {
         m_cntTemperatureBlink = 20;
      }

      if (g_pequipmentSelectWeapon->m_fTemperature > 75.0f)
      {
         m_cntTemperatureBlink = 10;
      }
   }

   if (g_options.IsWriteLogFileInLoop()) 
   {
      WriteLogFile("check Fight Experience\n");
   }

   if (m_iFightExperience > 900000)
   {
      m_idxClassifications = CLID_ELITE;
   }else{
      if (m_iFightExperience > 800000)
      {
         m_idxClassifications = CLID_DEADLY;
      }else{
         if (m_iFightExperience > 700000)
         {
            m_idxClassifications = CLID_DANGEROUS;
         }else{
            if (m_iFightExperience > 600000)
            {
               m_idxClassifications = CLID_COMPETENT;
            }else{
               if (m_iFightExperience > 500000)
               {
                  m_iFightExperience > 400000)
                  {
                     m_idxClassifications = CLID_AVERAGE;
                  }else{
                     if (m_iFightExperience > 300000)
                     {
                        m_idxClassifications = CLID_POOR;
                     }else{
                        if (m_iFightExperience > 200000)
                        {
                           m_idxClassifications = CLID_MOSTLYHARMLESS;
                        }else{
                           m_idxClassifications = CLID_HARMLESS;
                        }
                     }
                  }
               }
            }
         }
      }
   }
  
   // -- übertrage ggf. Enemykoordinaten
   //
   if(getGS()->ShallUpdateEnemies() && m_pEnemy)
   {
      g_vStarfield.x = m_pEnemy->getPosX();
      g_vStarfield.y = m_pEnemy->getPosY();
      g_vStarfield.z = m_pEnemy->getPosZ();
   }


   if (g_options.IsWriteLogFileInLoop()) 
   {
      WriteLogFile("update shield\n");
   }

   // -- Update Gameplay globals: shield, bonus, score
   //
   {
      bool l_bShallPlayUpdateSound = false;

      if (m_fShieldAmount > 0.0f && g_arr_cKeystate[DIK_Q] == 0)
      { 
         if (m_cntShieldBlink>0)
         {
            m_cntShieldBlink = m_cntShieldBlink - 1;
      
         }else {
            if (m_fShieldAmount < 50.0f)
            {
               m_cntShieldBlink = 10;
            }
   
            if (m_fShieldAmount < 30.0f)
            {
               m_cntShieldBlink = 5;
            }
         }
      }else{

         if (g_bCheatGodMode == false &&
             m_bGameOver == false)  // wenn noch *kein* gameover ist => dann explosion
         {
            //--- Spieler explodieren lassen
            //

            ENEMY* l_pNewEnemy;
            l_pNewEnemy = new (LIFECYCLE_LEVEL) EXPLOSION(LIFECYCLE_LEVEL);
            if (l_pNewEnemy != NULL)
            { 
               
               l_pNewEnemy->SetType(ETYP_EXPLOSION);
               if (l_pNewEnemy->setPos(m_pEnemy->getPos())==false) return false;
               if (l_pNewEnemy->setStep(m_pEnemy->getStep())==false) return false;

               l_pNewEnemy->setVisible(true);
               l_pNewEnemy->setTriggered(true);
               l_pNewEnemy->m_fExplScale = 1.0f;
            }

            //--- GameOver einleiten...
            //
            if (setGameOver() == false) return false;

         }
      }
   }

   if (g_options.IsWriteLogFileInLoop()) 
   {
      WriteLogFile("player updated \n");
   }
   return true;
}
*/

// -- getter for condition  "Alarmstufe"
//
/*
int PLAYER::getCondition()
{
	return m_idxCondition;
}

void PLAYER::setCondition(PSXCONDITION l_psxcondNew)
{
	m_idxCondition = l_psxcondNew;
}
bool PLAYER::isCondition(PSXCONDITION l_psxcondCheck)
{
	return (m_idxCondition == l_psxcondCheck);
}

// -- bestimmt die Alarmstufe des Players
//
bool PLAYER::updateCondition(int l_cntEnemies)
{
   int l_idxBackupCond;
   ENUM_TXX_ID l_txxGreenYellowRed;


   if (m_idxCondition > COID_DOCKED)
   {
      l_idxBackupCond = m_idxCondition;

      m_idxCondition = COID_GREEN;
      l_txxGreenYellowRed = txxGreen;
      if (l_cntEnemies > 0)
      {
         m_idxCondition = COID_YELLOW;
         l_txxGreenYellowRed = txxYellow;
         if (l_cntEnemies > 4)
         {
            m_idxCondition = COID_RED;
            l_txxGreenYellowRed = txxRed;
         }
      }

      if (l_idxBackupCond != m_idxCondition)
      {
         if (g_options.IsEnableSound())
         {
            if (g_sound_array[SNDRES_ALARM].play() == false) return false;
         }
      }
   }
   return true;
}

// --- Matrix bauen mit einzelnen aktuellen Drehschritt 
//     
//
bool PLAYER::updateMatrices()
{
   return true;

}

bool PLAYER::payCrimeTicket()
{
   // -- Wenn Player zuwenig Kohlen hat um sich 'frei zukaufen'
   //    -> gameover einleiten....
   //
   if((g_Player.m_iBankAccount/10)<g_iCrimeTicket)
   {
      g_cMissionTextArr[0] = g_cLang[txxGameOver];
      g_cMissionTextArr[1] = NULL;

      g_fcntMissionTextTimer = g_fmaxMissionTextTimer;
      if (g_Player.getGS()->setIndex(GAMESCREEN_GAMEOVER) == false) return false;
      g_Player.m_bGameOver = true;

      if (PSXInitializeTerminal(g_cLang[txxCannotPayCrimeTicket],
                                NULL,
                                NULL,
                                NULL) == false)
      {
         return false;
      }

   }else{
      // -- Strafe von Player-Konto abziehen...
      //
      g_Player.m_iBankAccount-= (g_iCrimeTicket * 10); 

      if (PSXInitializeTerminal(g_cLang[txxCrimeTicketPaid],
                                NULL,
                                NULL,
                                NULL) == false)
      {
         return false;
      }
   }
   return true;
}

bool PLAYER::setGameOver()
{
   ENEMY* l_penemyChild;

   WriteLogFile("PLAYER::setGameOver()\n");
   // -- bei evtl. untergebenen Objekten vorher abmelden oder diese mitzerstören
   //
   l_penemyChild = m_pEnemy->getNextEnemy(); // also erst ab nächsten Enemy suchen
   while(l_penemyChild)
   {

      if (l_penemyChild->m_pLeader == m_pEnemy)
      {
         PSXVECTOR l_vDist;
         float l_fLength;


         l_vDist.x = l_penemyChild->getPosX() - m_pEnemy->getPosX();
         l_vDist.y = l_penemyChild->getPosY() - m_pEnemy->getPosY();
         l_vDist.z = l_penemyChild->getPosZ() - m_pEnemy->getPosZ();
        
         l_fLength = pow2(l_vDist.x) + pow2(l_vDist.y) + pow2(l_vDist.z);
         l_fLength = sqrt(l_fLength);

         if (l_penemyChild->isPartOfLeader())
         {
            l_penemyChild->becomeIndependent();
         }

         dBodyAddForce(l_penemyChild->getBodyID(), 
                       l_vDist.x / l_fLength, 
                       l_vDist.y / l_fLength, 
                       l_vDist.z / l_fLength);

         l_penemyChild->m_pLeader = NULL;
      }
      l_penemyChild = l_penemyChild->getNextEnemy();
   }  

   // -- Explosion verraucht: entweder man war schon in der Rettungskapsel => gameover
   //                         oder steigt erst noch in sie ein => notausgestattet
   if (m_bRescue)
   {
      if (m_pEnemy->Destroy(LIFECYCLE_LEVEL) == false) return false;

      g_cMissionTextArr[0] = g_cLang[txxGameOver];
      g_cMissionTextArr[1] = NULL;

      g_fcntMissionTextTimer = g_fmaxMissionTextTimer;

      if (getGS()->setIndex(GAMESCREEN_GAMEOVER) == false) return false;
      m_bGameOver = true;
   }else{
      ENEMY* l_penemyCurr;

      if (g_equipmentArray[EQUIPMENT_ESCAPECAPSULE].isEnabled())
      {
         // -- Spieler behält keine Waren oder Waffen über
         //
         memset(g_iShipsHoldArray, 0, sizeof(unsigned int) * GOOID_LASTENUM);

         if (setRescue(true) == false) return false;

         if (getGS()->getIndex() == GAMESCREEN_COCKPIT)
         {
            if (getGS()->setIndex( GAMESCREEN_3RDPARTYVIEW) == false) return false;
         }

         // -- neuen m_fCollisionRadPow2 errechnen
         //
         l_penemyCurr = ENEMY::getFirstEnemy();
         while(l_penemyCurr)
         {
            l_penemyCurr->CalculateRad();
            l_penemyCurr = l_penemyCurr->getNextEnemy();
         }
   
   
         m_fShieldAmount = 100.0f; // wenigstens hat er aber wieder vollen Schild
      }else{
         g_cMissionTextArr[0] = g_cLang[txxGameOver];
         g_cMissionTextArr[1] = NULL;
   
         g_fcntMissionTextTimer = g_fmaxMissionTextTimer;
   
         WriteLogFile("set Gameover-Gamescreen\n");
         if (getGS()->setIndex(GAMESCREEN_GAMEOVER) == false) return false;
         m_bGameOver = true;
         g_cMissionTextArr[0] = g_cLang[txxGameOver];

         WriteLogFile("Destroying ETYP_PLAYER Enemy\n");
         if (m_pEnemy->Destroy(LIFECYCLE_LEVEL) == false) return false;

      }
   }
   return true;
} // 

// -- aendert die Polizeiakte des Spielers
//
bool PLAYER::setPoliceFile( PSXCRIME l_crimeNew)
{
   m_crimePoliceFile = l_crimeNew;

   return true;
}
*/

bool PLAYER::setLevelIndex(int l_idxValue)
{
   // -- Validation
   //
	/*
   if (l_idxValue >=0)
   {
      if (l_idxValue >= g_game.getLevelCount())
      {
         l_idxValue = 7;
         WriteLogFile("too high Planet Index, back to lave\n"); 
         return false;
      }
   }else{
      WriteLogFile("negative Planet Index, back to lave\n"); 
      l_idxValue = 7;
   }
   */
   m_idxLevel = l_idxValue;
/*
   m_idxGalaxy= int(m_idxLevel / g_maxPlanetsPerGalaxy);
*/

   m_pLevel = LEVEL::getByIndex(l_idxValue);
   return true;
}

int PLAYER::getLevelIndex()
{
   return m_idxLevel;
}

bool PLAYER::setLevelDestIndex(int l_idxValue)
{
   // -- Validation
   //
/*
   if (l_idxValue >=0)
   {
      if (l_idxValue >= g_game.getLevelCount())
      {
         l_idxValue = 7;
         WriteLogFile("too high Planet Index, back to lave\n"); 
         return false;
      }
   }else{
      WriteLogFile("negative Planet Index, back to lave\n"); 
      l_idxValue = 7;
   }
*/
   m_idxLevelDest = l_idxValue;
   
/*
   m_idxGalaxyDest= int(m_idxLevelDest / g_maxPlanetsPerGalaxy);
   { 
      char l_cPlanetNo[100];
      PSXPrintf(__LINE__, l_cPlanetNo, sizeof(l_cPlanetNo), 
                "PLAYER::setLevelDestIndex(%i Galaxy %i)\n", m_idxLevelDest, m_idxGalaxy);
      WriteLogFile(l_cPlanetNo);
   }
*/

   m_pLevelDest = LEVEL::getByIndex(l_idxValue);
   return true;
}

int PLAYER::getLevelDestIndex()
{
   return m_idxLevelDest;
}


/*
bool PLAYER::loadFromDC(PLAYERDATACONTAINER* l_dcPlayer)
{
   if (l_dcPlayer->m_idxEnemy >= 0)
   {
      m_pEnemy = ENEMY::getByIndex(l_dcPlayer->m_idxEnemy);
   }else{
      m_pEnemy = NULL;
   }

   m_bNearStarBase = l_dcPlayer->m_bNearStarBase;
   m_fShieldAmount = l_dcPlayer->m_fShieldAmount;
   m_cntShieldBlink = l_dcPlayer->m_cntShieldBlink;
   m_cntTemperatureBlink = l_dcPlayer->m_cntTemperatureBlink;
   m_iFightExperience = l_dcPlayer->m_iFightExperience;
   m_bPiratesInserted = l_dcPlayer->m_bPiratesInserted;
   m_bPoliceScanPassed = l_dcPlayer->m_bPoliceScanPassed;

   if (PSXStrCpy( __LINE__, m_cPlayerName, sizeof(m_cPlayerName), 
                 l_dcPlayer->m_cPlayerName) == false) 
   {
      WriteLogFile("Playername-Buffer overflow to long PLAYER::loadFromDC()\n");
   }

   if (setLevelIndex(l_dcPlayer->m_idxLevel) == false) 
   {
      WriteLogFile("PLAYER::setLevelIndex failed\n");
      return false;
   }

   m_idxPlayerName = l_dcPlayer->m_idxPlayerName;
   m_fWitchSpaceDistance = l_dcPlayer->m_fWitchSpaceDistance;
   m_bAutopilotDestValid = l_dcPlayer->m_bAutopilotDestValid;
   m_crimePoliceFile = l_dcPlayer->m_crimePoliceFile;
   m_maxFuel = l_dcPlayer->m_maxFuel;

   if (setLevelDestIndex(l_dcPlayer->m_idxLevelDest) == false)
   {
      WriteLogFile("PLAYER::setLevelDestIndex failed\n");
      return false;
   }

   m_iBankAccount = l_dcPlayer->m_iBankAccount;
   m_idxCondition = l_dcPlayer->m_idxCondition;
   m_idxClassifications = l_dcPlayer->m_idxClassifications;
   m_iFluct = l_dcPlayer->m_iFluct;
   m_bProximityToHunter = l_dcPlayer->m_bProximityToHunter;
   m_bEntered = l_dcPlayer->m_bEntered;
   if (m_bEntered)
   {
      WriteLogFile("Entered was set!\n");
   }else{
      WriteLogFile("Entered was NOT set!\n");
   }
   if (setRescue(l_dcPlayer->m_bRescue)==false) return false;
   m_bGameOver = l_dcPlayer->m_bGameOver;
   if (getGS()->setIndex(l_dcPlayer->m_idxGameScreen) == false) return false;
   m_fWingmanSpeed = l_dcPlayer->m_fWingmanSpeed;
   m_bEnableWitchSpace = l_dcPlayer->m_bEnableWitchSpace;
   m_bEnableBooster = l_dcPlayer->m_bEnableBooster;
   m_idxGalaxy = l_dcPlayer->m_idxGalaxy;
   m_idxGalaxyDest = l_dcPlayer->m_idxGalaxyDest;

   return true;
}

bool PLAYER::saveToDC(PLAYERDATACONTAINER* l_dcPlayer)
{

   if (m_pEnemy)
   {
      l_dcPlayer->m_idxEnemy = m_pEnemy->getEnemyIndex();
   }else{
      l_dcPlayer->m_idxEnemy = -1;
   }

   l_dcPlayer->m_bNearStarBase = m_bNearStarBase;
   l_dcPlayer->m_fShieldAmount = m_fShieldAmount;
   l_dcPlayer->m_cntShieldBlink = m_cntShieldBlink;
   l_dcPlayer->m_cntTemperatureBlink = m_cntTemperatureBlink;
   l_dcPlayer->m_iFightExperience = m_iFightExperience;
   l_dcPlayer->m_bPiratesInserted = m_bPiratesInserted;
   l_dcPlayer->m_bPoliceScanPassed = m_bPoliceScanPassed;

   l_dcPlayer->m_cPlayerName[g_maxPlayerName] = m_cPlayerName[g_maxPlayerName];
   if (PSXStrCpy( __LINE__, l_dcPlayer->m_cPlayerName, sizeof(l_dcPlayer->m_cPlayerName), 
                 m_cPlayerName) == false) 
   {
      WriteLogFile("Playername-Buffer overflow to long PLAYER::saveToDC()\n");
   }


   l_dcPlayer->m_idxLevel = getLevelIndex();

   l_dcPlayer->m_idxPlayerName = m_idxPlayerName;
   l_dcPlayer->m_fWitchSpaceDistance = m_fWitchSpaceDistance;
   l_dcPlayer->m_bAutopilotDestValid = m_bAutopilotDestValid;
   l_dcPlayer->m_crimePoliceFile = m_crimePoliceFile;
   l_dcPlayer->m_maxFuel = m_maxFuel;
   l_dcPlayer->m_idxLevelDest = getLevelDestIndex();
   l_dcPlayer->m_iBankAccount = m_iBankAccount;
   l_dcPlayer->m_idxCondition = m_idxCondition;
   l_dcPlayer->m_idxClassifications = m_idxClassifications;
   l_dcPlayer->m_iFluct = m_iFluct;
   l_dcPlayer->m_bProximityToHunter = m_bProximityToHunter;
   l_dcPlayer->m_bEntered = m_bEntered;
   l_dcPlayer->m_bRescue = m_bRescue;
   l_dcPlayer->m_bGameOver = m_bGameOver;
   l_dcPlayer->m_idxGameScreen = getGS()->getIndex();
   l_dcPlayer->m_fWingmanSpeed = m_fWingmanSpeed;
   l_dcPlayer->m_bEnableWitchSpace = m_bEnableWitchSpace;
   l_dcPlayer->m_bEnableBooster = m_bEnableBooster;
   l_dcPlayer->m_idxGalaxy = m_idxGalaxy;
   l_dcPlayer->m_idxGalaxyDest = m_idxGalaxyDest;

   return true;
}

bool PLAYER::validateInternalPointers()
{
   m_pLevel     = LEVEL::getByIndex(m_idxLevel);
   if (m_pLevel == NULL)
   {

      char l_cLevelNotFound[100];
      PSXPrintf(__LINE__, l_cLevelNotFound, sizeof(l_cLevelNotFound), 
                "Pointer to Level %i not found in PLAYER::validateInternalPointers()", m_idxLevel);
      hr2message(__LINE__,-1,l_cLevelNotFound);
      return false;
   }

   m_pLevelDest = LEVEL::getByIndex(m_idxLevelDest);
   if (m_pLevelDest == NULL)
   {
      char l_cLevelNotFound[100];
      PSXPrintf(__LINE__, l_cLevelNotFound, sizeof(l_cLevelNotFound), 
                "Pointer to DestLevel %i not found in PLAYER::validateInternalPointers()", m_idxLevelDest);
      hr2message(__LINE__,-1,l_cLevelNotFound);
   }

   return true;
}

bool PLAYER::Diagnostics()
{
   PSXPrintf(__LINE__, debugausgabe, 800,
             "bEntered: %i - gamescr:%i", int(m_bEntered), int(getGS()->getIndex()));
   
   return true;
}
*/

// -- betritt das Hyperraumziel-Planetensystem: 
//    Setzen der Marktpreise, bevoelkern des Enemyarrays..
//  
bool PLAYER::jumpDestinationLevel()
{
   float l_fThargoidAttVal=0.0f;
   WriteLogFile("PLAYER::jumpDestinationLevel\n");
/*
   l_fThargoidAttVal = g_seedThargoidAction.getFloat();
*/
   if (LEVEL::m_dJointGroupID)
   {
      dJointGroupEmpty(LEVEL::m_dJointGroupID);
   }

   WriteLogFile("dropping all enemies\n");

   // -- alle alten enemies ordnungsgemäss plattmachen
   //
   if (getLevelDestIndex() != getLevelIndex())
   {
      ENEMY::DropAllEnemies(LIFECYCLE_LEVEL);
   }

   // -- wieder Platz machen für evtl. andere Herausforderungen an das VideoRAM
   //
   WriteLogFile("release all Textures..\n");
   if (PSXReleaseAllTextures() == false) return false;

   // -- rauswerfen aller mapobjects, wenn durch diesen Aufruf 
   //    tatsächlich anderes system betreten wird als momentan
   //    sowieso vorhanden
   //

/*
   if (getLevelDestIndex() != getLevelIndex())
   {
      WriteLogFile("change to another Planet-Sytem:\n");
      if (PSXWriteCharacterArray() == false) return false;
 
      if (g_bCheatGodMode == false)
      {
         g_iShipsHoldArray[GOOID_FUEL] = (int)
            float(g_iShipsHoldArray[GOOID_FUEL])-m_fWitchSpaceDistance;
      }
   }
*/
   WriteLogFile("flush LIFECYCLE_LEVEL-Nodes\n");
   MAPOBJECT::flushAll(LIFECYCLE_LEVEL);

/*
   if (g_game.updateAllCharacters() == false) return false;

   if (getLevelDestIndex() != getLevelIndex())
   {
      // --- Hyperraumsprung sound
      //
      if (g_options.IsEnableSound())
      {
         if (g_sound_array[SNDRES_TRANSFER].play() == false) return false;
      } 
   }

   //if (getLevelDestIndex() != getLevelIndex())
   {
      if (ASTRO::populateAstroArray()==false) return false;
   }

   //
   // --- Pruefen ob Thargoiden den Hyperraumsprung stoeren
   
   if (g_arr_cKeystate[DIK_MULTIPLY] ||
       (g_Player.m_idxClassifications>=CLID_AVERAGE && l_fThargoidAttVal >= 0.5f))
   {
      g_cMissionTextArr[0] = g_cLang[txxWitchSpaceDriveMalfunction];
      g_cMissionTextArr[1] = NULL;
      g_fcntMissionTextTimer = g_fmaxMissionTextTimer;
         
      WriteLogFile("trapping into ThargoidLevel..\n");
	  if (THARGOIDLEVEL::m_pInstance == NULL) WriteLogFile("no instance of Thargoidlevel there\n");
      m_pLevel = THARGOIDLEVEL::m_pInstance;

	  g_levelContent.initBase(LEVELCONTENT_THARGOID);
   }else{
      g_levelContent.initBase(LEVELCONTENT_CORIOLIS);
*/

   // -- den Wechsel vollziehen
   //
   setLevelIndex(getLevelDestIndex());
   /*
   if (PSXReadCharacterArray(getLevelIndex()) == false) return false;

   m_pLevel->genMarket(m_iFluct);

   GAMESCREEN::m_iMarketMode = 0;
   if (g_Player.getGS()->initCommodityMenuItems() == false)
   {
      return false;
   }

   GAMESCREEN::m_idxChoosedCommodity = 0;
   GAMESCREEN::m_bChoosedCommodityValid = false;
   GAMESCREEN::m_iSellBuyMode = 0;

   m_bPoliceScanPassed = false; // solange ist betreten der Raumstation unmöglich

   // -- falls der Spieler per Hyperraumsprung flüchtet wird er an die Kopfgeldjäger 
   //    gemeldet
   //
   if (m_crimePoliceFile == CRID_OFFENDER)
   {
      setPoliceFile(CRID_FUGITIVE);
   }

   g_levelContent.m_vPos = m_pLevel->m_vPos;
*/

   if (m_pLevel->calibrateEnemyArray(LID_OUTDOOR) == false)
   {
      return false;
   }

   if (insertEnemy() == false) return false;

   // -- Defaultmässig ist der Player zuständig
   //
   g_penemyCamera = m_pEnemy;
   g_penemyInput = m_pEnemy;

   // -- update textfields in cockpit
   //
/*
   if (m_gamescreen.isHavingTextOverlay())
   {
      if (m_gamescreen.applyUpdateText(m_gamescreen.getIndex()) == false)
      {
         return false;
      }
   }
   // -- je nach Anzahl der Enemies bestimmte Selektionsergebnis arrays vorallokieren
   //
   RADARLIST::setInstanceToNull();
   RADARLIST::Instance(LIFECYCLE_LEVEL, ENEMY::getEstimatedCount());
   WriteLogFile("jumpDestinationLevel() completed.\n");

   // -- damit nicht sofort ein gigantischer Simulationsstep errechnet wird
   //
   g_dwLastTickCount = GetTickCount();
*/
   return true;
}


// -- greift auf den protected Gamescreen zu
//
/*
GAMESCREEN* PLAYER::getGS()
{
   return &m_gamescreen;
}
*/

// -- setzt den Enemy ein
//
bool PLAYER::insertEnemy()
{
   WriteLogFile("PLAYER::insertEnemy()\n");

   m_pEnemy = new (LIFECYCLE_LEVEL) ENEMY(LIFECYCLE_LEVEL);
   if (m_pEnemy == NULL) return false;
   m_pEnemy->SetType(ETYP_PLAYER);

/*
   if (g_levelContent.m_pBigRes)
   {
      if(m_pEnemy->setPos(cos(m_pLevel->m_fPlayerAngle1) * g_levelContent.m_pBigRes->m_fRad * 10.0f * 1.5f + g_levelContent.m_vPos.x,
                          sin(m_pLevel->m_fPlayerAngle1) * g_levelContent.m_pBigRes->m_fRad * 10.0f * 1.5f + g_levelContent.m_vPos.y,
                          g_levelContent.m_vPos.z ) == false) return false;

   }

*/
   m_pEnemy->setPos(0.0f, 0.0f, -10.0f);
   if(m_pEnemy->setMat(XMMatrixIdentity())==false) return false; //temp for if(m_pEnemy->setMat(m_pLevel->m_matPlayer)==false) return false;
   m_pEnemy->m_fSpeed = 0.0f;
/*

   if (m_pEnemy->checkAttachedEnemies() == false) return false;
*/
   WriteLogFile("done insertEnemy().\n");
   return true;
} 

// -- abhängig davon, die resource des enemies setzen
//
/*
bool PLAYER::setRescue(bool l_bValue)
{
   WriteLogFile("PLAYER::setRescue");
   if (l_bValue != m_bRescue)
   {
      m_bRescue = l_bValue;

      if (m_pEnemy)
      {
         if (m_bRescue)
         {
            WriteLogFile("(true)\n");
            if (m_pEnemy->getRes() != RESTYPE_RESCUE) 
            {
               if (m_pEnemy->setRes(RESTYPE_RESCUE)==false) return false;
            }
         }else{
            WriteLogFile("(false)\n");
            if (m_pEnemy->getRes() != RESTYPE_COBRA_PART1) 
            {
               if (m_pEnemy->setRes(RESTYPE_COBRA_PART1)==false) return false;
               if (m_pEnemy->checkAttachedEnemies() == false) return false;
            }
         }
      }
   }

   return true;
}

bool PLAYER::getRescue()
{
   return m_bRescue;
}
*/
