/*
 *------------------------------------------------------------------------
 *      Object: LEVEL
 *         $Id: psxlevel.cpp,v 1.42 2010/06/01 17:25:54 molle Exp $
 *        Date: Mar 2005
 *       Notes: Anlegen der Enemies levelabhängig
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

// -- durchreich operator, um nur die virtual functionpointer füllen zu lassen
//
/*
void* LEVEL::operator new( size_t dummy, void* proxy)
{
   return proxy;
}
*/

// -- pflegt die Verkettung zwischen den LEVEL objekten innerhalb des grossen Pools
//
LEVEL::LEVEL(LIFECYCLE l_lifecycle) : MAPOBJECT(l_lifecycle)
{
   m_iSizeOf = sizeof(LEVEL); 

   if (m_plevelSelected == this)
   {
      m_plevelSelected = NULL;
   }

   if (m_plevelFirst == NULL) 
   {
      m_plevelFirst = this;
   }

   if (m_plevelLast)
   {
      m_plevelPrevious = m_plevelLast;
      m_plevelPrevious->m_plevelNext = this;
   }else{
      m_plevelPrevious = NULL;
   }
 
   m_plevelNext = NULL;
   m_plevelLast = this;

   setLevelIndex(m_idxHighest);

   // -- die normalen Felder initialisieren
   //
   /*
   m_idxMostWantedPirate = 0;
   m_iMostWantedReward = 0;
   */
}

LEVEL* LEVEL::getNextLevel()
{
   return m_plevelNext;
}

LEVEL* LEVEL::getPreviousLevel()
{
   return m_plevelPrevious;
}
/*
unsigned int LEVEL::getTypeID()
{
   return 'EVEL';
}
*/
// -- füllt das EnemyArray oder konfiguriert es geringfügig um
//
bool LEVEL::calibrateEnemyArray(ELOC l_elocID )
{
   WriteLogFile("LEVEL::calibrateEnemyArray()\n");

   // -- test Enemy-Array-Management-Functionality
  
   ENEMY* l_pEnemy = new (LIFECYCLE_LEVEL) ENEMY(LIFECYCLE_LEVEL);
   l_pEnemy->SetType(ETYP_FLYER);
   l_pEnemy->setPos(1.0f, 1.0f, 10.0f);
/*
   ENEMY* l_pEnemy2 = new (LIFECYCLE_LEVEL) ENEMY(LIFECYCLE_LEVEL);   
   l_pEnemy2->SetType(ETYP_HUNTER);
   l_pEnemy2->setPos(10.0f, 1.0f, 10.0f);
   
   ENEMY* l_pEnemy3 = new (LIFECYCLE_LEVEL) ENEMY(LIFECYCLE_LEVEL);
   l_pEnemy3->SetType(ETYP_DAZ3D_CUBE);
   l_pEnemy3->setPos(100.0f, 0.0f, 50.0f);

   //--- Minenexplosion
   //
   EXPLOSION* l_pEnemy4 = new (LIFECYCLE_LEVEL) EXPLOSION(LIFECYCLE_LEVEL);
   if (l_pEnemy4 != NULL)
   { 
      XMVECTOR l_vPos = XMVectorSet(0.0f,0.0f,10.0f,1.0f);
      l_pEnemy4->SetType(ETYP_EXPLOSION);
      l_pEnemy4->setPos(l_vPos);
      l_pEnemy4->setVisible(true);
      l_pEnemy4->setTriggered(false);
      l_pEnemy4->m_fExplScale = 0.1f;
   }else{
      hr2message(__LINE__,-1, "LEVEL::calibrateEnemyArray() - cannot insert EXPLOSION into enemy_array ");
      return false;
   }
*/

   // -- anhängige Targetlocks löschen
   //
   /*
   ENEMY::setEnemyTarget(NULL);
   g_bTargetLocked = false;
   switch(l_elocID) 
   {
      case LID_DOCK:
         WriteLogFile("Levelpopulation for LID_DOCK\n");
         if (g_levelContent.m_penemyStarBase)
         {
            g_levelContent.m_penemyStarBase->Destroy(LIFECYCLE_LEVEL);
         }

         if (g_levelContent.m_penemyKingsplane)
         { 
             g_levelContent.m_penemyKingsplane->Destroy(LIFECYCLE_LEVEL);
         }
         if (insertStarBaseHangar() == false) return false;
         break;

      case LID_UNDOCK:
         WriteLogFile("Levelpopulation for LID_UNDOCK\n");
         if (g_levelContent.m_penemyHangar)
         {
            g_levelContent.m_penemyHangar->Destroy(LIFECYCLE_LEVEL);
         }
         if (insertMainEnemy() == false) return false;
         break;

      default:   
         WriteLogFile("Levelpopulation for LID_[ANY]\n");

         ENEMY::DropAllEnemies(LIFECYCLE_LEVEL);
    

         if (g_levelContent.m_idxContent == LEVELCONTENT_THARGOID)
         {
            if (insertThargoids() == false)
            {
               return false;
            }
         }else{
            // -- Die Starbase-Stellvertreter Enemies anlegen
            //
            if (insertMainEnemy()==false)
            {
               return false;
            }

            // -- Specialelemente des Flagschiffes ggf. beruecksichtigen
            //
             if (insertFlagShipDoors() == false) return false;

            if (insertPatrols()==false)
            {
               return false;
            }
         }

         if (m_bAsteroids)
         {
            if (insertAsteroids()==false)
            {
               return false;
            }
         }

         if (insertCharacterEnemies()==false)
         {
            return false;
         }

   }


   if (setExteriorLights()==false)
   {
      return false;
   }

   */

   return true;
}

int LEVEL::getLevelIndex()
{
   return m_idxLevel;
}

bool LEVEL::setLevelIndex(int l_idxValue)
{
   m_idxLevel = l_idxValue;

   if (m_idxLevel >= m_idxHighest ) 
   {
      m_idxHighest = m_idxLevel + 1;
   }

   return true;
}


// -- object aus der verkettung entfernen
//
bool LEVEL::Destroy(LIFECYCLE l_lifecycle) 
{
   // -- ist dies schon das letzte Level
   //
   if (m_plevelLast == this)
   {
      // -- wenn vorheriges Objekt vorhanden, dann den dortigen Nextzeiger löschen
      //
      if (m_plevelPrevious)
      {
         m_plevelLast = m_plevelPrevious;
      }

      // -- war dies auch gleichzeitig das erste Level, dann
      //   den Indexvergabe zähler zurücksetzen 
      //
      if (m_plevelFirst == this)
      {
         m_idxHighest = 0;
      }
   }

   // -- dem vorherigen Level das eigene Next aufdrücken 
   //
   if (m_plevelPrevious)
   {
      m_plevelPrevious->m_plevelNext = m_plevelNext;
   }

   // -- dem nächsten Level das eigene Previous aufdrücken 
   //
   if (m_plevelNext)
   {
      m_plevelNext->m_plevelPrevious = m_plevelPrevious;
   }

   // -- nachdem nun dieses Level aus der Verkettung raus ist,
   //    auch die Mapobjekt verkettung auflösen
   //
   return MAPOBJECT::Destroy(l_lifecycle);
}

