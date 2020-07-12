/*
 *------------------------------------------------------------------------
 *      Object: player.h
 *         $Id: player.h,v 1.13 2009/12/07 20:55:58 molle Exp $
 *        Date: Okt 2005
 *       Notes: Deklarationen fuer Player-klasse
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

#ifndef PLAYER_H
#define PLAYER_H
/*

const int g_maxPlayerName = 23;

typedef enum _condition{
   COID_DOCKED=0,
   COID_GREEN,
   COID_YELLOW,
   COID_RED,
   COID_LASTENUM
}PSXCONDITION;


typedef enum _crimerec{
   CRID_CLEAN=0,  //Sauber
   CRID_OFFENDER, //Verbrecher
   CRID_FUGITIVE, //Flüchtig
   CRID_LASTENUM
} PSXCRIME;

typedef enum _classifi{
   CLID_HARMLESS=0,
   CLID_MOSTLYHARMLESS,
   CLID_POOR,
   CLID_AVERAGE,
   CLID_ABOVEAVERAGE,
   CLID_COMPETENT,
   CLID_DANGEROUS,
   CLID_DEADLY,
   CLID_ELITE,
   CLID_LASTENUM
} PSXCLASSIFICATION;

class PLAYERDATACONTAINER {
   public:
     int    m_idxEnemy;

     bool   m_bNearStarBase;
     bool   m_bCollisionOnStep;
     float  m_fShieldAmount;

     int    m_cntShieldBlink;
     int    m_cntTemperatureBlink;
     int    m_iFightExperience;
     bool   m_bPiratesInserted;
     bool   m_bPoliceScanPassed;

     D3DMATRIX m_matCamera;

     char   m_cPlayerName[g_maxPlayerName];
     int   m_idxPlayerName;
     int   m_idxLevel;
     
     float m_fWitchSpaceDistance;
     bool  m_bAutopilotDestValid;

     PSXCRIME m_crimePoliceFile;
     float m_maxFuel;
     int   m_idxLevelDest;
     int   m_iBankAccount;
     int   m_idxCondition;
     int   m_idxClassifications;

     int   m_iFluct;
     bool  m_bProximityToHunter;
     bool  m_bEntered;
     bool  m_bRescue;
     bool  m_bGameOver;
     IDXGAMESCREEN m_idxGameScreen;
     bool  m_bDummy;
     PSXVECTOR m_vDummy;
     PSXVECTOR m_vDummyStep;
     float  m_fWingmanSpeed;
     bool   m_bEnableWitchSpace;
     bool   m_bEnableBooster;

     int    m_idxGalaxy;
     int    m_idxGalaxyDest;
};
*/

class PLAYER {
   protected:
     int   m_idxLevel;
     int   m_idxLevelDest;
     //GAMESCREEN m_gamescreen;
     bool  m_bRescue;
     int   m_idxCondition;

   public:
     //PSXVECTOR m_vBounceNormal;
     ENEMY* m_pEnemy;
/*
     bool   m_bNearStarBase;
     */
     float  m_fShieldAmount;
     /*

     int    m_cntShieldBlink;
     int    m_cntTemperatureBlink;
     */
     int    m_iFightExperience;
     /*
     bool   m_bPiratesInserted;
     bool   m_bPoliceScanPassed;

     char   m_cPlayerName[g_maxPlayerName];
     int   m_idxPlayerName;
*/
     LEVEL* m_pLevel;
     LEVEL* m_pLevelDest;
/*

     float m_fWitchSpaceDistance;
     bool  m_bAutopilotDestValid;

     PSXCRIME m_crimePoliceFile;
     float m_maxFuel;
     int   m_iBankAccount;
     int   m_idxClassifications;

     int   m_iFluct;
     bool  m_bProximityToHunter;
     bool  m_bEntered;
     bool  m_bGameOver;
     float  m_fWingmanSpeed;
     bool   m_bEnableWitchSpace;
     bool   m_bEnableBooster;

     int    m_idxGalaxy;
     int    m_idxGalaxyDest;

*/
     void Init();
/*
     bool setBouncing();
     bool Update();
     bool updateMatrices();
     bool doAutopilot();
     bool setPoliceFile(PSXCRIME);
     bool updateCondition(int);
     bool payCrimeTicket();
     bool switchToWingman();
     bool setGameOver();

     bool loadFromDC(PLAYERDATACONTAINER*);
     bool saveToDC(PLAYERDATACONTAINER*);
*/

     bool setLevelIndex(int);
     int getLevelIndex();

     bool setLevelDestIndex(int);
     int getLevelDestIndex();
   
/*
     bool validateInternalPointers();
     bool Diagnostics();
*/
     bool jumpDestinationLevel();

     bool insertEnemy();
/*
     GAMESCREEN* getGS();
     bool setRescue(bool);
     bool getRescue();
	  void setCondition(PSXCONDITION);
	  int getCondition();
	  bool isCondition(PSXCONDITION);
*/
};


#endif
