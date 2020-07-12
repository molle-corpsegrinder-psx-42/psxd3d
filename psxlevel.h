/*
 *------------------------------------------------------------------------
 *      Object: LEVEL
 *         $Id: psxlevel.h,v 1.31 2010/06/01 17:25:54 molle Exp $
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
enum ELOC {LID_INDOOR  = 0,
                   LID_OUTDOOR,
                   LID_NOOUTDOOR,
				   LID_DOCK,   // starbase weg, dock/hangar einfügen
				   LID_UNDOCK, // starbase wieder hin
                   LID_TUNNEL01,
                   LID_TUNNEL02,
                   LID_TUNNEL03,
                   LID_MAINTUBE,
                   LID_BRUECKE,
                   LID_UTASSEN,
                   LID_CHARBOARD,
                   LID_ASTEROIDAREA,
                   LID_SOMEWHERE,
                   LID_PATROLANDDOORS,
                   LID_LASTENUM};

// -- planeten/sonnensystem
//
/*
typedef enum _politsys { ANARCHY = 0,
                         FEUDAL,
                         MULTI_GOV,
                         DICTATORSHIP,
                         COMMUNIST,
                         CONFEDERACY,
                         DEMOCRACY,
                         CORPORATE_STATE,
                         POLITSYS_LASTENUM} POLITSYS;


typedef enum _richness {RICH_IND = 0,
                        AVERAGE_IND,
                        POOR_IND,
                        MAINLY_IND,
                        MAINLY_AGRI,
                        RICH_AGRI,
                        AVERAGE_AGRI,
                        POOR_AGRI} RICHNESS;

class LEVELDATACONTAINER 
{
   public:
      PSXVECTOR m_vPos;
      PSXVECTOR m_vPos2D;
      PSXVECTOR m_vSunToPlayerOffset;

      float m_fPlanetDistance;
      float m_fSunDistance;
      float m_fSunAngle;

      bool m_bDayLight;
      bool m_bAsteroids;

      float m_fPlayerAngle1;
      D3DMATRIX m_matPlayer;
   
      RICHNESS m_richnessEconomy; 
      POLITSYS m_politsysGov;   

      UINT m_iTechLev;            
      UINT m_iPopulation;         
      UINT m_iProductivity;       
      UINT m_iRadius;             

      D3DCOLORVALUE m_d3dcolPlanet;
      FASTSEED m_fastseedGoatSoup;
      bool m_bHavePirates;
      char m_cName[12];
      RESTYPE m_resCargo;

      int m_idxFirstCharacter;

      unsigned int m_iTypeID;
      int m_idxLevel;

      int m_idxMostWantedPirate;
      int m_iMostWantedReward;
      SEED m_seedPlanetOffsets;
      //int m_idxLevelMostWantedFrom;
};


DWORD WriteLogFile(char*);
HRESULT PSXPrintf(int, LPTSTR , size_t , LPCTSTR , ...  );
*/

class LEVEL : public MAPOBJECT
{
   protected:
      int m_idxLevel;
      static int m_idxHighest;

      LEVEL* m_plevelNext;
      LEVEL* m_plevelPrevious;
      static LEVEL* m_plevelFirst;
      static LEVEL* m_plevelLast;
      static LEVEL* m_plevelSelected;

	  /*
      virtual bool insertFlagShipDoors();
      virtual bool insertMainEnemy();
      
      virtual bool insertPatrols();
      virtual bool insertAsteroids();
	   virtual bool insertThargoids();
      virtual bool insertCharacterEnemies();
      virtual bool setExteriorLights();

		//virtual bool insertPlanets();
      int m_idxMostWantedPirate;
      int m_iMostWantedReward;
*/
      //LEVEL* m_plevelMostWantedFrom;

   public:
	   /*
	   bool insertStarBaseHangar();
	   */
      static dWorldID m_iWorldID;
      static dSpaceID m_dSpaceID;
      static dJointGroupID m_dJointGroupID;

      // -- initialisiert die static-globals 
      //
      static void setFirstAndLastToNull() {
         m_idxHighest = 0;
         m_plevelFirst = NULL;
         m_plevelLast  = NULL;
      }

      LEVEL(LIFECYCLE);
      LEVEL* getNextLevel();
      LEVEL* getPreviousLevel();
      static inline LEVEL* getFirstLevel(){
         return m_plevelFirst;
      };
 
      static inline bool setLevelSelected(LEVEL* l_pLevel) {
         if (l_pLevel)
         {
            if ((l_pLevel >= m_plevelFirst) &&
                (l_pLevel <= m_plevelLast))
            {
               m_plevelSelected = l_pLevel;
               return true;
            }else{
               hr2message(__LINE__,-1,"Levelpointer out of First/Last Zone");
               return false;
            }
         }else{
            m_plevelSelected = NULL;
            return true;
         }
      }

      static inline bool IsAnySelected() {
         if (m_plevelSelected)
         {
            return true;
         }else{
            return false;
         }
      }

      static inline LEVEL* getLevelSelected()
      {
         return m_plevelSelected;
      }

      static inline LEVEL* getByIndexDebug(int l_idxLevel)
      {
         LEVEL* l_plevelFound;
         char l_cDebugText[100];
         l_plevelFound = m_plevelFirst;
       
         HRESULT hr = PSXPrintf(__LINE__, 
                                l_cDebugText, sizeof(l_cDebugText),
                                "Searching for %i", l_idxLevel);

         WriteLogFile(l_cDebugText);

         do {
           HRESULT hr = PSXPrintf(__LINE__, 
                                  l_cDebugText, sizeof(l_cDebugText),
                                  "Meeting %i", l_plevelFound->m_idxLevel);
  
           WriteLogFile(l_cDebugText);

           if (l_plevelFound->m_idxLevel == l_idxLevel) 
           {
              return l_plevelFound;
           }
           l_plevelFound = l_plevelFound->m_plevelNext;
         } while (l_plevelFound);
         return NULL;
      };

      static inline LEVEL* getByIndex(int l_idxLevel)
      {
         LEVEL* l_plevelFound;
         l_plevelFound = m_plevelFirst;
       
         do {
           if (l_plevelFound->m_idxLevel == l_idxLevel) 
           {
              return l_plevelFound;
           }
           l_plevelFound = l_plevelFound->m_plevelNext;
         } while (l_plevelFound);
         return NULL;
      };

/*
      static inline bool resetAllStatistics()
      {
         LEVEL* l_plevelCurr;

         l_plevelCurr = m_plevelFirst;
         while(l_plevelCurr)
         {
            l_plevelCurr->resetStatistics();
            l_plevelCurr = l_plevelCurr->getNextLevel();
         }
         return true;
      };
      bool resetStatistics();

*/
      static void dropWorldAndSpace()
      {
         if (m_dSpaceID)
         {
            dSpaceDestroy(m_dSpaceID);
            m_dSpaceID = 0;
         }

         if (m_dJointGroupID)
         {
            dJointGroupEmpty(m_dJointGroupID);
            dJointGroupDestroy(m_dJointGroupID);
            m_dJointGroupID = 0;
         }

         if (m_iWorldID) 
         {
            dWorldDestroy(m_iWorldID);
            m_iWorldID = 0;
         }
      
      }

      static void createWorldAndSpace()
      {
         m_iWorldID = dWorldCreate();
         dWorldSetAutoDisableFlag(m_iWorldID, 0);
         m_dJointGroupID = dJointGroupCreate(0);
         m_dSpaceID = dHashSpaceCreate(NULL);
      }

/*
      PSXVECTOR m_vPos;
      PSXVECTOR m_vSunToPlayerOffset;
      float m_fSunDistance;

      D3DCOLORVALUE m_d3dcolPlanet;
      bool m_bAsteroids;

      bool      m_bHavePirates;
      RESTYPE   m_resCargo;
      float     m_fPlanetDistance;
      float     m_fSunAngle;
      bool      m_bDayLight;
      float     m_fPlayerAngle1;
      D3DMATRIX m_matPlayer;

      PSXVECTOR m_vPos2D;
      RICHNESS m_richnessEconomy; 
      POLITSYS m_politsysGov;   

      UINT m_iTechLev;            
      UINT m_iPopulation;         
      UINT m_iProductivity;       
      UINT m_iRadius;             

      FASTSEED m_fastseedGoatSoup;
      char m_cName[12];

      int m_idxFirstCharacter;
      SEED m_seedPlanetOffsets;
*/
      virtual bool calibrateEnemyArray(ELOC l_elocID);
/*
      virtual bool deleteDefensePos();
      virtual bool insertDefensePos();

      virtual bool make();
      virtual bool getGoatSoup(const char *, char*);
      virtual bool genMarket(unsigned int );
      virtual bool setSunAngle(float);
      virtual bool validate();

      virtual bool loadFromDC(LEVELDATACONTAINER*);
      virtual bool saveToDC(LEVELDATACONTAINER*);
      virtual char* getPolitSysName();
	  */
      int getLevelIndex();
      bool setLevelIndex(int);
     
      virtual bool Destroy(LIFECYCLE l_lifecycle);
/*
      unsigned int getTypeID();
      int getMostWantedPirateIndex();
      bool setMostWantedPirateIndex(int);

      int getMostWantedReward();
      bool setMostWantedReward(int);
*/

};
/*

class SCLEVEL : public LEVEL
{  
   protected:
      bool insertFlagShipDoors();
      bool deleteDefensePos();
      bool insertDefensePos();
      bool insertMainEnemy();
      bool insertPatrols();
      bool insertAsteroids();

   public:
      unsigned int getTypeID();

      
};

class THRUSTLEVEL : public LEVEL
{  
   protected:
      bool insertFlagShipDoors();
      bool deleteDefensePos();
      bool insertDefensePos();
      bool insertMainEnemy();
      bool insertPatrols();
      bool insertCharacterEnemies();
      bool insertAsteroids();
   public:
      unsigned int getTypeID();
};

// -- Absicherung durch Singleton-Pattern
//
class THARGOIDLEVEL : public LEVEL 
{
   public:
      static THARGOIDLEVEL* m_pInstance;
      THARGOIDLEVEL(LIFECYCLE);
      bool Destroy(LIFECYCLE l_lifecycle);

      unsigned int getTypeID();
};
*/
