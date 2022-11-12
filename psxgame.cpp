/*
 *------------------------------------------------------------------------
 *      Object: psxgame.cpp 
 *   $Revision: 1.229 $
 *        Date: 2004                            
 *       Notes: Game Logik
 *------------------------------------------------------------------------
 * 
 * Copyright (c) 1997-2004 Michael Molkenthin. All Rights Reserved.
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

bool BIGRES::initBase()
{
   m_fScale = 1.0f;
   m_bHaveGuts = false;
   return true;
}

   
bool GAME::resetGame()
{

   WriteLogFile("flush all with Lifecycle game\n");

   // -- ab hier gilt die Datei: alle LIFECYCLE_GAME Levels wegschmeissen
   //
   if (MAPOBJECT::flushAll(LIFECYCLE_GAME) == false)
   {
      return false;
   } 


/*
   g_cntCargoBox=0;

   g_bDebugTriangleValid = false;

   g_iChkChrGrp=0;
   g_cntChrUpdateTimer=500;
   g_maxChrUpdateTimer=5000;

   g_iThreadExitTimer   = 100;
   g_maxThreadExitTimer = 100;
   g_bThreadExit=false;

   g_cntMousestateCheckTimer[0]=0;
   g_cntMousestateCheckTimer[1]=0;

   g_fSpaceWarpShade=0.1f;
   g_fSpaceWarpAngle=0.0f;

   g_idxTarget = 0;
   g_cntTarget = 0;
*/

   m_fVelocity = 0.0f;

   if (initSeeds() == false) return false;

   // -- LEVEL initialisieren
   //
   if (generateAllLevels() == false) return false;

   if (LEVEL::setLevelSelected(NULL) == false) return false;
   LEVEL::m_iWorldID = 0;
   LEVEL::m_dSpaceID = 0;
   LEVEL::m_dJointGroupID = 0;

   LEVEL::createWorldAndSpace();
   dHashSpaceSetLevels(LEVEL::m_dSpaceID, 10, 24);

/*
   // -- FunksprÃ¼che zurÃ¼cksetzen
   //
   if (PSXInitRadioMessages() == false) return false;
*/

   // -- Die Charaktere anlegen
   //
   /*
   if (populateGalaxy() == false) return false;
   */

   // -- Radar
/*
   g_fRadarZoomFactor = 2.0f;

   g_cntSmokeTrays =0;
   g_idxSmokeTrays =0;
   g_maxSmokeFor   =0;

   g_cntAfterBurner =0;
   g_idxAfterBurner =0;
   g_maxBurnerFor   =0;

 
   g_idxBrennstabWeapon=SHOOTDRAWER_REDLASER;
   
   g_fcntJumpWitchSpace  = 0.0f;
  
   g_fScalePointer    =24.0f;
   g_fScalePointerMax =30.0f;
   g_fScalePointerMin =20.0f;
   
   g_bScalePointer=true;
   
   g_bRearViewEnabled = 0;
   
   g_arrFadeStat[0] = FID_AWAY;
   g_arrFadeStat[1] = FID_AWAY;
   g_arrFadeStat[2] = FID_AWAY;
   g_arrFadeStat[3] = FID_CENTER;
   g_arrFadeStat[4] = FID_AWAY;
   
   g_arrBillBoardMovement[0].fBoardFadingStep    = 0.0f;
   g_arrBillBoardMovement[0].fBoardFadingStepAdd = 0.0f;
   g_arrBillBoardMovement[0].maxBoardFading      = 15;
   g_arrBillBoardMovement[0].cntBoardFading      = 0;
   
   g_arrBillBoardMovement[1].fBoardFadingStep    = 0.0f;
   g_arrBillBoardMovement[1].fBoardFadingStepAdd = 0.0f;
   g_arrBillBoardMovement[1].maxBoardFading      = 65;
   g_arrBillBoardMovement[1].cntBoardFading      = 0;
   
   g_arrBillBoardMovement[2].fBoardFadingStep    = 0.1f;
   g_arrBillBoardMovement[2].fBoardFadingStepAdd = 0.1f;
   g_arrBillBoardMovement[2].maxBoardFading      = 5;
   g_arrBillBoardMovement[2].cntBoardFading      = 0;

   g_arrBillBoardMovement[3].fBoardFadingStep    = 0.02f;
   g_arrBillBoardMovement[3].fBoardFadingStepAdd = 0.02f;
   g_arrBillBoardMovement[3].maxBoardFading      = 20;
   g_arrBillBoardMovement[3].cntBoardFading      = 0;
   
   g_bGameHintTextAvailable = false;
   
   g_bTorpedoUsageHintPrinted            = false;
   g_bDestroyLeaderFromBehindHintPrinted = false;
   g_bDestroyMinesHintPrinted            = false;
   g_bGoNextStarBasePrinted              = false;
   g_bDamageHintPrinted                  = false;
   g_bMineDetonatedHintPrinted           = false;
   
*/
/*
   g_vRumbleWaveRad.x  = 1.0;
   g_vRumbleWaveRad.y  = 1.5;
   g_vRumbleWaveRad.z  = 0;
   g_vRumbleWaveStep.x = 0.5f;
   g_vRumbleWaveStep.y = 0.8f;
   g_vRumbleWaveStep.z = 0.9f;
   
   //g_bGeneratorView = false;
*/
   
/*
   g_arrFadeStat[BBID_HIGHSCORE]=FID_AWAY;
   
   g_bAsteroidGenerators=true;

   g_fSpeedUpTime     =0.0f;
   g_fTmpSpeedUpFactor=0.0f;
   
   g_vPlayer1.x = 0;
   g_vPlayer1.y = -20000;
   g_vPlayer1.z = 0;
   
   
   g_vPlayer2.x = 0;
   g_vPlayer2.y = 6000;
   g_vPlayer2.z = 0;
   
   g_vAsteroidOrigin.x = 0;
   //g_vAsteroidOrigin.y =-10000;
   g_vAsteroidOrigin.y = 0;
   g_vAsteroidOrigin.z = 0;
   
   g_vBrennstab.x      =-62.498135;
   g_vBrennstab.y      = 88.913094;
   g_vBrennstab.z      =-65.544899;
   
   g_fBalanceFactor = 1.0f;
   
   g_iTimeCompressionState=0;
*/
   


   // --- shoot array zuruecksetzen
   //
   g_cntShoot = 0;
   memset(shoot_array, 0, sizeof(SHOOT)* g_maxShoot);

	{
		
		XMFLOAT4 l_spriteArray[] = {
			{ XMFLOAT4(-0.5f, -0.5f,0.0f,1.0f)},
			{ XMFLOAT4(0.5f, 0.5f,0.0f,1.0f)},
			{ XMFLOAT4(1.0f, 1.0f,10.0f,1.0f)},
	   };
		XMFLOAT4 l_float4Step = XMFLOAT4(0.0f, 0.0f, 5.0f, 0.0f);
		XMVECTOR l_vStep = XMLoadFloat4(&l_float4Step);

		int l_cntInitShoot = sizeof(l_spriteArray) / sizeof(l_spriteArray[0]);
		for (int l_idxInitShoot = 0;l_idxInitShoot < l_cntInitShoot;l_idxInitShoot++)
		{
			shoot_array[l_idxInitShoot].m_vPos = XMLoadFloat4(l_spriteArray + l_idxInitShoot);
			shoot_array[l_idxInitShoot].m_vStep = l_vStep;
			
			shoot_array[l_idxInitShoot].m_cntTimeToLive = 100; //g_maxTimeToLiveMenaceSelf;

			shoot_array[l_idxInitShoot].menacetype = MENACE_SELF;
			shoot_array[l_idxInitShoot].m_penemyHitableArray = NULL;
			shoot_array[l_idxInitShoot].m_idxShootDrawer = SHOOTDRAWER_BLUELASER;
			
		}
		g_cntShoot = l_cntInitShoot;
		
	}
/*
   g_cntGunShootDelay = 0;
   g_maxGunShootDelay = 100; // wird zur Laufzeit spezifiziert

   g_idxNewestShoot = 6;
*/
   
   // -- Leadhist zuruecksetzen
   //
/*
   cnt_leadhist = 0;
   memset(leadhist, 0, sizeof(LEADHIST)* max_leadhist);
*/
   
/*
   {
      float l_fPlasmaShift[] = { 0.03f, 0.018f, 0.04f, 0.034f,
                                 0.093f, 0.024f, 0.072f, 0.021f,
                                 0.07f, 0.098f, 0.01f, 0.034f,
                                 0.023f, 0.084f, 0.012f, 0.061f,
       
                                 0.02f, 0.05f, 0.075f, 0.011f,
                                 0.04f, 0.087f, 0.5f, 0.032f,
                                 0.032f, 0.05f, 0.08f, 0.011f,
                                 0.042f, 0.042f, 0.032f, 0.03f ,
      
                                 0.01f, 0.02f, 0.025f, 0.025f,
                                 0.03f, 0.052, 0.33f, 0.021f,
                                 0.021f, 0.023f, 0.058f, 0.012f,
                                 0.05f, 0.055f, 0.022f, 0.05f 
                               };
      int l_cntPlasmaShift = sizeof(l_fPlasmaShift) / sizeof(l_fPlasmaShift[0]);
   
      if(l_cntPlasmaShift!=g_cntPlasmaShift)
      {
         hr2message(__LINE__,-1,"plasmashift with criminal number!\n");
         return(FALSE);
      }
   
      memcpy(&g_fPlasmaShift,&l_fPlasmaShift, sizeof(l_fPlasmaShift)); 
   }
   g_cntShowPlasmaTimer = 0;
*/

   // --- CheatFlags
/*
   g_bCheatGodMode=false;
*/

   // -- prefill the rotationmatrix for the mines
   //
/*
   WriteLogFile("prefill mine-rotationmatrix\n");
   {
      float l_fViewRotation1, l_fViewRotation2;
   
      l_fViewRotation1 = 0.02f;
      l_fViewRotation2 = 0.005f;
   
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
   
      matmult(&g_matMineRotation, &g_matGeneric1, &g_matGeneric2);
   }
*/
   
   g_Player.Init();
/*

   // -- Markt initialisieren und 
   //
   memset(g_iShipsHoldArray, 0, sizeof(unsigned int) * GOOID_LASTENUM);

   WriteLogFile("initial jump to lave\n");
   if (g_levelContent.initBase(LEVELCONTENT_CORIOLIS)==false) return false;

*/
   WriteLogFile("init equipment\n");
   if (initEquipment() == false) return false;

   //if (PSXReadCharacterArray(g_Player.getLevelDestIndex()) == false) return false;

   g_Player.jumpDestinationLevel(); 


/*
   g_iShipsHoldArray[GOOID_PULSELASER] = 1;
   g_iShipsHoldArray[GOOID_FUEL]       = g_Player.m_maxFuel; // Treibstoffverbrauch rÃ¼ckgaengig machen
   g_cntEquipmentTicker = 0;

   // -- Begruessungs-Info ueber erstes StarBase an Player
   // 
   g_cntTerminalTimer = 0;
*/
   
/*
   g_cMissionTextArr[0] = g_cLang[txxMissionBriefing1];
   g_cMissionTextArr[1] = g_cLang[txxMissionBriefing2];
   g_cMissionTextArr[2] = NULL;

   g_fcntMissionTextTimer = g_fmaxMissionTextTimer;
   g_fMissionTimerDiff=0.0f;
*/
   
   // -- bullettime standard maessig aus
   //
   g_equipmentArray[EQUIPMENT_TORPEDO].m_bSingleton = false;
   g_equipmentArray[EQUIPMENT_MISSILE].m_bSingleton = false;
   g_equipmentArray[EQUIPMENT_WINGMAN].m_bSingleton = false;
   g_equipmentArray[EQUIPMENT_MINE].m_bSingleton = false;
   
/*
   g_cntHitableEnemies = 0;
   
   g_dwLastTickCount = GetTickCount();
   g_fElapsedTime   = 0.0f;
   g_fProfilingMilliSec = 0.0f;
*/

/*
   // -- Die Buttons des Start-Menu positionieren und eingrenzen
   //
   if (g_Player.getGS()->setIndex(GAMESCREEN_COCKPIT) == false) return false; //GAMESCREEN_MENU;

   g_vMouse.x = 0.0f;
   g_vMouse.y = 0.0f;
   GAMESCREEN::m_bChoosedMenuItemValid = false;
   
   if (g_Player.getGS()->initMenuItems()== false)
   {
      return false;
   }
*/

   return(true);

}
// -- Speicher an die Verwaltungsarrays verteilen
//
int GAME::distributeMem(int l_iStartAdress)
{
   int l_NeededWavMem=0,l_NeededMp3Mem=0;

/*
   for(int idxSndRes=0;idxSndRes<g_cntSndRes;idxSndRes++)
   { 
      l_NeededWavMem+= PSXWavFileCntByte(idxSndRes);
   }

   if(PSXMp3GetCntByte()==false)
   {
      hr2message(__LINE__,GetLastError(),"PSXMp3CntGetCntByte()");
      return false;
   }

   for(int idxMp3Res=0;idxMp3Res<MP3RES_LASTENUM;idxMp3Res++)
   {
      l_NeededMp3Mem+=(int) mp3_filelength[idxMp3Res];
   }

   // -- auf 4er begradigen
   //
   l_NeededMp3Mem = (l_NeededMp3Mem + 3) & 0xfffffffc;
*/
   // -- liegt auf dem selben Speicher:
   //

   g_arr_pEnemy    = (ENEMY**) l_iStartAdress;   
   /*
   g_pfaceinfSelectedArray = (PSXFACEINFO**) l_iStartAdress;
   */
   g_contactgeomArray = (dContactGeom*) l_iStartAdress;
   
   // -- Buffer overflow ausschliessen: Speicher wird als Arbeitspuffer verwendet
   //    fuer die Assembler SelectHitObjects routine, die wiederum
   //    zur Selektion Enemies als auch Flagship faces verwendet wird
   //    Beide Maxwerte muessen daher beruecksichtigt werden
   //
   {

      int l_iEnemyResultArrayLength;
	  /*
      int l_iSelectHitFSFaceBufferLength;
	  */
      int l_iContactGeomLength;
	  
      l_iEnemyResultArrayLength = g_maxEnemy * sizeof(float) + g_maxEnemy * sizeof(ENEMY*);
	  
      // -- hier werden spÃ¤ter durch dCollide() die BerÃ¼hrungspunkte abgelegt
      //
      l_iContactGeomLength = sizeof(dContactGeom) * g_meshresManager.getMaxFaces();
   
      // -- damit spaeter die Assembler Hit-Selectroutine 
      //    genug Puffer zum Arbeiten hat (zum Zwischenspeichern der Vektorlaengen)
      //
	  /*
      l_iSelectHitFSFaceBufferLength = sizeof(float) * g_meshresManager.getMaxFaces();
   
      if (l_iSelectHitFSFaceBufferLength > l_iEnemyResultArrayLength)
      {
         l_iEnemyResultArrayLength = l_iSelectHitFSFaceBufferLength;
      }
	  */
      if (l_iContactGeomLength > l_iEnemyResultArrayLength)
      {
         l_iEnemyResultArrayLength = l_iContactGeomLength;
      }

	  l_iStartAdress = l_iStartAdress + l_iEnemyResultArrayLength;	  
   }

   g_arr_EnemyDrawer = (ENEMYDRAWER*) (l_iStartAdress);
   l_iStartAdress = int(g_arr_EnemyDrawer + EDRAW_LASTENUM); 

   g_txtassignResStdArray = (TEXTUREASSIGN*) l_iStartAdress;
   l_iStartAdress = int(g_txtassignResStdArray + RESTYPE_LASTENUM);

   //g_edrawResEmissiveArray = (ENEMYDRAWER*) int(g_txtassignResStdArray + RESTYPE_LASTENUM); eliminitated by cut shader from enemydrawer (no need Std vs Emissive vs BUmp)
   //g_edrawResBumpArray = (ENEMYDRAWER*) int(g_edrawResEmissiveArray + RESTYPE_LASTENUM);

   shoot_array  = (SHOOT*) l_iStartAdress;
   l_iStartAdress = int(shoot_array + g_maxShoot); 

   /*
   g_penemyHitableArray = (ENEMY**) int(shoot_array + g_maxShoot); 
   
   g_sound_array    = (SOUND*) int(g_penemyHitableArray + g_maxHitableEnemies);//!!!
   g_radiomsg_array = (SOUND*) int(g_sound_array + g_cntSndRes);

   g_Mp3Src_Array      = (char*) int(g_radiomsg_array +   g_maxRadioMsg);
   g_penemyTargetArray = (ENEMY**) int(g_Mp3Src_Array) + l_NeededMp3Mem;

   g_pShipsLostArray   = (unsigned int*) int(g_penemyTargetArray + g_maxTarget);
   
   return int(g_pShipsLostArray + (GOOID_LASTENUM * g_maxCargoBox)); // hier immer noch das letzte Array eintragen
   */

//   return int(g_penemyTargetArray + g_maxTarget); // hier immer noch das letzte Array eintragen
   return l_iStartAdress;
}

// -- Speicher fuer alle Verwaltungsarrays anfordern
//
bool GAME::allocate()
{

   char l_cInfoText[1000]; 
   int l_iStartAdress;
   int l_NeededMem=0; 

   l_NeededMem = distributeMem(0);

   PSXPrintf(__LINE__, l_cInfoText,sizeof(l_cInfoText), "allocate GameManagement-Memory: %i Chars \n",l_NeededMem);
   WriteLogFile(l_cInfoText);

   l_iStartAdress = int(VirtualAlloc(NULL, l_NeededMem, MEM_COMMIT, PAGE_READWRITE));

   if (l_iStartAdress==0)
   {
      hr2message(__LINE__,GetLastError(),"VirtualAlloc(GAME MANAGEMENT)");
      return false;
   }

   // -- Speicher verteilen anhand der wirklichen Startadresse
   //
   distributeMem(l_iStartAdress);

   WriteLogFile("zeroing EnemyDrawer\n");
   memset(g_arr_EnemyDrawer, 0, sizeof(ENEMYDRAWER) * EDRAW_LASTENUM);
   memset(g_txtassignResStdArray, 0, sizeof(ENEMYDRAWER) * RESTYPE_LASTENUM);
   /*
   WriteLogFile("zeroing sound_array\n");
   memset(g_sound_array, 0, g_cntSndRes * sizeof(SOUND));

   WriteLogFile("zeroing radiomsg_array\n");
   memset(g_radiomsg_array, 0, g_maxRadioMsg * sizeof(SOUND));

   WriteLogFile("zeroing targetarray\n");
   memset(g_penemyTargetArray, 0, g_maxTarget * sizeof(ENEMY*));

   WriteLogFile("zeroing g_pShipsLostArray\n");
   memset(g_pShipsLostArray, 0, g_maxCargoBox * GOOID_LASTENUM * sizeof(unsigned int*));
   

   // -- Adresse jedes potentiellen Enemy in Zeigerarray zum sortieren laden 
   //
   WriteLogFile("allocate leadhist array\n");
   leadhist = new LEADHIST[max_leadhist];
   if (leadhist == NULL)
   {
      MessageBox(hFrameWind, "can't allocate leaderhistory array", 
                             "initialization fail", MB_ICONEXCLAMATION);
      return false;
   }
   */
   return true;
}   

bool GAME::initSeeds()
{
   // -- Starfield Marsennetwister starten
   //
   {
      unsigned long l_dwInitMT[]={0x123, 0x234, 0x345, 0x456};
      g_mtwistStarfield.init_by_array(l_dwInitMT, sizeof(l_dwInitMT)/ sizeof(l_dwInitMT[0]));
   }

   // -- start Marsennetwister used to generate fibonacci-seed for lazy planet offset finding
   //
   {
      unsigned long l_dwInitMT[]={0x654, 0x543, 0x432, 0x321};
      g_mtwistPlanetOffsetSeed.init_by_array(l_dwInitMT, sizeof(l_dwInitMT)/ sizeof(l_dwInitMT[0]));
   }
/*
   // -- Initialise seed for galaxy 1
   //
   g_seedGalaxy.w0=g_ushortBase0; 
   g_seedGalaxy.w1=g_ushortBase1; 
   g_seedGalaxy.w2=g_ushortBase2; 

   g_seedPlanet.w0=0x4389; 
   g_seedPlanet.w1=0xf462; 
   g_seedPlanet.w2=0x1a64; 

   g_seedAsteroid.w0=0x3212; 
   g_seedAsteroid.w1=0xed85; 
   g_seedAsteroid.w2=0x81af; 

   g_seedPatrol.w0=0x162a; 
   g_seedPatrol.w1=0x7385; 
   g_seedPatrol.w2=0xf132; 

   g_seedStar.w0=0x3212; 
   g_seedStar.w1=0xed85; 
   g_seedStar.w2=0x81af; 

   g_seedTrayManipulation.w0=0x3212; 
   g_seedTrayManipulation.w1=0xed85; 
   g_seedTrayManipulation.w2=0x81af; 

   g_seedCalibrateEnemy.w0=0xae2a; 
   g_seedCalibrateEnemy.w1=0xea31; 
   g_seedCalibrateEnemy.w2=0xff82; 

//   g_seedChrAction.w0=0x3212; 
//   g_seedChrAction.w1=0xed85; 
//   g_seedChrAction.w2=0x81af; 

   g_seedChrAction.w0= GetTickCount() & 0xffff;
   g_seedChrAction.w1= GetTickCount() & 0xffff;
   g_seedChrAction.w2= GetTickCount() & 0xffff;

   g_seedThargoidAction.w0=0x3212 + GetTickCount() & 0xffff; 
   g_seedThargoidAction.w1=0xed85 + GetTickCount() & 0xffff; 
   g_seedThargoidAction.w2=0x81af + GetTickCount() & 0xffff; 
*/

   return true;
}

	
// -- fÃ¼llt das Planetenarray
//
bool GAME::generateAllLevels()
{
	
   //int i, l_cntPlanet,l_idxGalaxy;

   LEVEL::setFirstAndLastToNull();

   WriteLogFile("GAME::generateAllLevels()\n");
   /*
   // -- Das Universum
   //
   //g_idxGoatSoup = 0;

   l_cntPlanet = 0;
   
   for(l_idxGalaxy=0;l_idxGalaxy<g_maxPlanetsPerGalaxy;l_idxGalaxy++) 
   {  

      // -- Put galaxy data into array of structures 
      //
      for(i=0;i<g_maxPlanetsPerGalaxy;i++) 
      {
	  */
	LEVEL* l_pLevel;

	l_pLevel = new(LIFECYCLE_GAME) LEVEL(LIFECYCLE_GAME);

	if (l_pLevel == NULL)
	{
		hr2message(__LINE__, -1, "generateAllLevels():instancing of LEVEL failed\n");
		return false;
	}
		 /*
         if (l_pLevel->make() == false)
         {
            return false;
         }

         //setLevelCatalog(l_cntPlanet, l_pLevel);
         
         l_cntPlanet++;  
      }
      g_seedGalaxy.twist();
      g_seedPlanet.twist();
   }
   
   // -- Das Thargoiden-Sonderlevel
   //
   THARGOIDLEVEL::m_pInstance = NULL;
   (new(LIFECYCLE_GAME) THARGOIDLEVEL(LIFECYCLE_GAME))->make();
   */
   return true;
}

// -- regelt alles rund um die Player-eigenen Waffen/PowerUps
//
bool GAME::initEquipment()
{
	/*
   // -- Zuordnung der BigResItems zu den Equipmentrecords
   //
   WriteLogFile("assigning Equipment to BigResItems\n");
   {
      int l_idxItem, l_idxAssign;
      EQUIPMENT* l_pEquipment;
      BIGRESITEM* l_pBigResItem;

      struct _assignitem {
        RESTYPE      resItem;
        EQUIPMENTID  idxEquipment;
      } *l_ptempCurr, l_tempItemAssignArray[] = {
         //{ RESTYPE_HAESSLICHCONTAINER, 
         { RESTYPE_WINGMAN,            EQUIPMENT_WINGMAN }, 
         { RESTYPE_COBRA_MOUNTS,       EQUIPMENT_LARGECARGOBAY },
         { RESTYPE_COBRA_REAR,         EQUIPMENT_GALACTICHYPERDRIVE },
         { RESTYPE_COBRA_WEAPONFRONT,  EQUIPMENT_GATLINGLASER1 },
         { RESTYPE_COBRA_WEAPONLEFT,   EQUIPMENT_GATLINGLASER2 },
         { RESTYPE_COBRA_WEAPONREAR,   EQUIPMENT_GALACTICHYPERDRIVE },
         { RESTYPE_COBRA_WEAPONRIGHT,  EQUIPMENT_MILITARYLASER }, // geruest notwendig
      };
      int l_cntAssign = sizeof(l_tempItemAssignArray) / sizeof(l_tempItemAssignArray[0]);

      for (l_idxItem = g_bigresArray[BIGRES_COBRA].m_idxBigResItem;
           l_idxItem < g_bigresArray[BIGRES_COBRA].m_cntBigResItem;
           l_idxItem ++)
      {
         l_pBigResItem = g_bigresitemArray+ l_idxItem;

         l_ptempCurr = l_tempItemAssignArray;
         for (l_idxAssign = 0; l_idxAssign < l_cntAssign; l_idxAssign++)
         {
            l_pEquipment = g_equipmentArray + l_ptempCurr->idxEquipment;

            if (l_pBigResItem->m_resPart == l_ptempCurr->resItem)
            {
               l_pBigResItem->m_pEquipment = l_pEquipment;
               
               // -- falls Zuordnungsziel-Equipment = Waffe, dann dort gleich 
               //    die Abfeuerposition festlegen
               //
               if (l_pEquipment->m_bIsAWeapon)
               {
                  int l_idxSpawn;
                  for (l_idxSpawn = 0; l_idxSpawn < l_pEquipment->m_cntSpawn; l_idxSpawn++)
                  {
                     l_pEquipment->m_vSpawn[l_idxSpawn].x = l_pEquipment->m_vSpawn[l_idxSpawn].x + 
                                                            l_pBigResItem->m_vPos.x;
                     l_pEquipment->m_vSpawn[l_idxSpawn].y = l_pEquipment->m_vSpawn[l_idxSpawn].y + 
                                                            l_pBigResItem->m_vPos.y;
                     l_pEquipment->m_vSpawn[l_idxSpawn].z = l_pEquipment->m_vSpawn[l_idxSpawn].z + 
                                                            l_pBigResItem->m_vPos.z;
                  }
               }
            }
            l_ptempCurr++;
         }
      }
   }
   */

   // --- alle Equipments disablen
   //
	/*
   for (int l_idxEquipment=0; l_idxEquipment < EQUIPMENT_LASTENUM; l_idxEquipment++)
   {
      g_arr_cntEquipmentBlinking[l_idxEquipment] = 0;
   }
   g_arr_cntEquipmentBlinking[EQUIPMENT_LASER] = 50;
   */
   // -- Das EquipmentArray initialisieren
   //
   memset(g_equipmentArray, 0, sizeof(EQUIPMENT)*EQUIPMENT_LASTENUM);
   {
      int l_idxEquipment;
      for (l_idxEquipment=0; l_idxEquipment < EQUIPMENT_LASTENUM; l_idxEquipment++)
      {
         if (g_equipmentArray[l_idxEquipment].init() == false) return false;
		 /*
         g_arr_cntEquipmentBlinking[l_idxEquipment] = 0;
		 */
      }
   }

   
   g_pequipmentSelectWeapon=g_equipmentArray;
   

   // -- Grundausstattung des Players
   //
   WriteLogFile("initializing Equipment\n");
   {
      int l_idxInitEquipment, l_idxEquipment;

      struct _equiptemplate {
      int         iIsAWeapon;
      float       fShieldDamage;
      EQUIPMENTID equipmentidKey;
	  /*
      COMMGOODID  commgoodidRef;
	  */
      ENUM_TXX_ID txxName;
      int         maxTimeToLive;
      int         maxShootDelay;
      float       fSpeed;
      IDXSHOOTDRAWER idxShootDrawer;
      /*SNDRES      sndresShoot;*/
      } *l_ptempCurr, l_tempInitEquipmentArray[] = {
         { 1, .05, EQUIPMENT_LASER,         /*GOOID_PULSELASER,*/     txxPulseLaser,     60, 300, 12.0f, SHOOTDRAWER_REDLASER}, /*, SNDRES_OWNSHOOT},*/
         { 1, .0, EQUIPMENT_MININGLASER,   /*GOOID_MININGLASER,*/    txxMiningLaser,    60, 800, 12.0f, SHOOTDRAWER_REDLASER}, /*, SNDRES_OWNSHOOT},*/
         { 1, .05, EQUIPMENT_GATLINGLASER1, /*GOOID_GATLINGLASERI,*/  txxGatlingLaserI,  60, 200,  18.0f, SHOOTDRAWER_REDLASER}, /*, SNDRES_GATLINGLASERI},*/
         { 1, .05, EQUIPMENT_GATLINGLASER2, /*GOOID_GATLINGLASERII,*/ txxGatlingLaserII, 60, 100,  24.0f, SHOOTDRAWER_REDLASER}, /*, SNDRES_GATLINGLASERII},*/
         { 1, .1, EQUIPMENT_BATTLELASER,   /*GOOID_BATTLELASER,*/    txxBattleLaser,    120, 200,  18.0f, SHOOTDRAWER_REDLASER}, /*, SNDRES_BATTLELASER},*/
         { 1, .15, EQUIPMENT_MILITARYLASER, /*GOOID_MILITARYLASER,*/  txxMilitaryLaser,  120, 200,  24.0f, SHOOTDRAWER_REDLASER}, /*, SNDRES_MILITARYLASER},*/
         { 1, .2, EQUIPMENT_SUPERLASER,    /*GOOID_SUPERLASER,*/     txxSuperLaser,     120, 200,  30.0f, SHOOTDRAWER_REDLASER}, /*, SNDRES_SUPERLASER},*/
         { 1, .4, EQUIPMENT_TORPEDO,    /*GOOID_TORPEDO,*/     txxTorpedo,    800, 800,  1.0f, SHOOTDRAWER_REDTORPEDO}, /*, SNDRES_MISSILESTART},*/
         { 1, .4, EQUIPMENT_MISSILE,    /*GOOID_MISSILE,*/     txxMissile,    800, 800,  2.0f, SHOOTDRAWER_NOBILLBOARD}, /*, SNDRES_MISSILESTART},*/
         { 1, .5, EQUIPMENT_MINE, /*GOOID_MINE,*/ txxMine, 200, 800,  2.0f, SHOOTDRAWER_NOBILLBOARD}, /*, SNDRES_MINENAUSWURF},*/
   
         { 0, 0, EQUIPMENT_LARGECARGOBAY,      /*GOOID_LARGECARGOBAY,*/     txxLargeCargoBay,  0, 0, 0.0f, SHOOTDRAWER_REDLASER}, /*, SNDRES_OWNSHOOT},*/
         { 0, 0, EQUIPMENT_SHIELDGENERATOR,          /*GOOID_SHIELDGENERATOR,*/         txxShieldGenerator,0, 0, 0.0f, SHOOTDRAWER_REDLASER}, /*, SNDRES_OWNSHOOT},*/
         { 0, 0, EQUIPMENT_FUELSCOOPS,         /*GOOID_FUELSCOOPS,*/        txxFuelScoops,0, 0, 0.0f, SHOOTDRAWER_REDLASER}, /*, SNDRES_OWNSHOOT},*/
         { 0, 0, EQUIPMENT_ESCAPECAPSULE,      /*GOOID_ESCAPECAPSULE,*/     txxEscapeCapsule,      0, 0, 0.0f, SHOOTDRAWER_REDLASER}, /*, SNDRES_OWNSHOOT},*/
         { 0, 0, EQUIPMENT_EXTRAENERGYUNIT,    /*GOOID_EXTRAENERGYUNIT,*/   txxExtraEnergyUnit,    0, 0, 0.0f, SHOOTDRAWER_REDLASER}, /*, SNDRES_OWNSHOOT},*/
         { 0, 0, EQUIPMENT_GALACTICHYPERDRIVE, /*GOOID_GALACTICHYPERDRIVE,*/txxGalacticHyperdrive, 0, 0, 0.0f, SHOOTDRAWER_REDLASER}, /*, SNDRES_OWNSHOOT},*/
         { 0, 0, EQUIPMENT_WINGMAN,            /*GOOID_WINGMAN,*/           txxWingman, 800, 800, 1.0f, SHOOTDRAWER_NOBILLBOARD}, /*, SNDRES_ANDOCK_KLEIN},*/
      };
      int l_cntInitEquipment = sizeof(l_tempInitEquipmentArray) /  
                               sizeof(l_tempInitEquipmentArray[0]);   

      l_ptempCurr = l_tempInitEquipmentArray;
      EQUIPMENT* l_pequipmentCurr;
  
      struct _ownspawns {
         EQUIPMENTID equipmentidKey;
         PSXVECTOR   vSpawn;
      } l_tempSpawn[] = {
// original
         { EQUIPMENT_LASER,         { -16.400026, -0.299995, 35.500031}},
         { EQUIPMENT_LASER,         { +16.400026, -0.299995, 35.500031}},
         { EQUIPMENT_GATLINGLASER1, { -2.0f, 8.0f, 0.0f } },
         { EQUIPMENT_GATLINGLASER1, { +2.0f, 8.0f, 0.0f } },
         { EQUIPMENT_GATLINGLASER2, { 15.300022, -0.799995, -25.700062}},
         { EQUIPMENT_GATLINGLASER2, { 16.900028, -0.799995, -25.700062}},
         { EQUIPMENT_BATTLELASER,   { -27.800070, -3.999994, 8.499994}},
         { EQUIPMENT_BATTLELASER,   { +27.800070, -3.999994, 8.499994}},
         { EQUIPMENT_MILITARYLASER, { -15.600023, 0.000000, -55.699722}},
         { EQUIPMENT_SUPERLASER,    { -15.600023, 0.000000, -55.699722}},
         { EQUIPMENT_TORPEDO,       { -15.600023, 0.000000, 80.699722}},
         { EQUIPMENT_MISSILE,       { -15.600023, 0.000000, -55.699722}},
         { EQUIPMENT_MINE,          { -15.600023, 0.000000, -55.699722}},
         { EQUIPMENT_MININGLASER,   { -16.400026, -0.299995, 35.500031}},
         { EQUIPMENT_WINGMAN,       { -0.000123, 12.705368, -39.219814}},
      };
      int l_idxSpawn, l_cntSpawn = sizeof(l_tempSpawn) / sizeof(l_tempSpawn[0]);

      struct _shootcolors {
         EQUIPMENTID   equipmentidKey;
         D3DCOLORVALUE d3dCol;
      } l_tempColor[] = {
         { EQUIPMENT_LASER,         { 1.0f, 0.0f, 0.0f, 1.0f }},
         { EQUIPMENT_MININGLASER,   { 1.0f, 1.0f, 1.0f, 1.0f }},
         { EQUIPMENT_GATLINGLASER1, { 0.0f, 1.0f, 0.0f, 1.0f }},
         { EQUIPMENT_GATLINGLASER2, { 0.0f, 0.0f, 1.0f, 1.0f }},
         { EQUIPMENT_BATTLELASER,   { 1.0f, 1.0f, 0.0f, 1.0f }},
         { EQUIPMENT_MILITARYLASER, { 1.0f, 0.0f, 1.0f, 1.0f }},
         { EQUIPMENT_SUPERLASER,    { 0.0f, 1.0f, 1.0f, 1.0f }},
         { EQUIPMENT_TORPEDO,       { 1.0f, 0.0f, 0.0f, 1.0f }},
      };
      int l_idxColor, l_cntColor = sizeof(l_tempColor) / sizeof(l_tempColor[0]);

      for (l_idxInitEquipment = 0; l_idxInitEquipment < l_cntInitEquipment; l_idxInitEquipment++)
      {
         l_pequipmentCurr = g_equipmentArray + l_ptempCurr->equipmentidKey;
		 
         /*l_pequipmentCurr->m_commgoodid      = l_ptempCurr->commgoodidRef;*/
         l_pequipmentCurr->m_cName           = g_cLang[l_ptempCurr->txxName];
         l_pequipmentCurr->m_maxTimeToLive   = l_ptempCurr->maxTimeToLive;
         l_pequipmentCurr->m_fSpeed          = l_ptempCurr->fSpeed;
         l_pequipmentCurr->m_idxShootDrawer  = l_ptempCurr->idxShootDrawer;
         l_pequipmentCurr->m_fShieldDamage   = l_ptempCurr->fShieldDamage;
         l_pequipmentCurr->m_maxShootDelay   = l_ptempCurr->maxShootDelay;
         /*l_pequipmentCurr->m_sndresShoot     = l_ptempCurr->sndresShoot;*/
        
         if (l_ptempCurr->iIsAWeapon >0)
         { 
            l_pequipmentCurr->m_bIsAWeapon = true;
         }else{
            l_pequipmentCurr->m_bIsAWeapon = false;
            //g_iShipsHoldArray[l_ptempCurr->commgoodidRef] = 0; // debug: alle nichtwaffen weg
         }
         l_ptempCurr++;
      }
      
      for (l_idxEquipment = 0; l_idxEquipment < EQUIPMENT_LASTENUM; l_idxEquipment++)
      {
         g_equipmentArray[l_idxEquipment].m_cntSpawn = 0;
      }

      // -- Austrittspunkte der Laser definieren
      //
      for (l_idxSpawn = 0; l_idxSpawn < l_cntSpawn; l_idxSpawn++)
      {
         int k;
         l_pequipmentCurr = g_equipmentArray + l_tempSpawn[l_idxSpawn].equipmentidKey;

         k = l_pequipmentCurr->m_cntSpawn;
         l_pequipmentCurr->m_cntSpawn++;
		 
		 XMFLOAT3 l_vLoad;
		 l_vLoad.x = l_tempSpawn[l_idxSpawn].vSpawn.x;
		 l_vLoad.y = l_tempSpawn[l_idxSpawn].vSpawn.y;
		 l_vLoad.z = l_tempSpawn[l_idxSpawn].vSpawn.z;

		 l_pequipmentCurr->m_vSpawn[k] = XMLoadFloat3(&l_vLoad);
      }

      // -- Farben der Laser definieren
      //
      for (l_idxColor = 0; l_idxColor < l_cntColor; l_idxColor++)
      {
         l_pequipmentCurr = g_equipmentArray + l_tempColor[l_idxColor].equipmentidKey;
         /* l_pequipmentCurr->m_d3dcolShoot = l_tempColor[l_idxColor].d3dCol;*/
      }

   }


   // -- Abhängikeiten bestimmter Waren von Ausrüstungsgegenständen
   //
   /*
   WriteLogFile("initializing Good/Equipment Dependencies\n");
   {
      int l_idxDependency;

      TRADEGOOD* l_ptradegoodCurr;

      struct _dependency {
        COMMGOODID  commgoodidRef;
        EQUIPMENTID equipmentidKey;
      } l_tempDependencyArray[] = {
         { GOOID_MILITARYLASER, EQUIPMENT_LARGECARGOBAY},
         { GOOID_SUPERLASER,    EQUIPMENT_LARGECARGOBAY},
      };
      int l_cntDependency = sizeof(l_tempDependencyArray) /  
                               sizeof(l_tempDependencyArray[0]);   

      for (l_idxDependency = 0; l_idxDependency < l_cntDependency; l_idxDependency++)
      {
         l_ptradegoodCurr = g_tradegoodCommoditiesArray + l_tempDependencyArray[l_idxDependency].commgoodidRef;
         l_ptradegoodCurr->m_idxNeccessaryEquipment = l_tempDependencyArray[l_idxDependency].equipmentidKey;
      }
   }
   */
   return true;
}


//bool PSXSaveGame() {spÃ¤ter}

//bool PSXLoadGame()  {spÃ¤ter}

// -- Eingabe+Verarbeitung fÃ¼r die gamebezogene Logik
//
bool PSXUpdateGameManagement()
{
	HRESULT hr;

	/***************************************************************
	*							EINGABE
	***************************************************************/
	hr = g_didevMouse->GetDeviceState(sizeof(DIMOUSESTATE), &diMouseState);
	if (hr != 0) {
		hr2message(__LINE__, hr, "g_didevMouse->GetDeviceState()");
		return(FALSE);
	}

	//--- Bei alt+tab INPUTLOST
	//
	hr = g_didevKeyboard->GetDeviceState(256, g_arr_cKeystate);
	if (hr == DIERR_INPUTLOST)
	{
		hr = g_didevKeyboard->Acquire();
		if (hr != 0)
		{
			hr2message(__LINE__, hr, "g_didevKeyboard->Acquire()");
			return(FALSE);
		}

	}
	else {
		if (hr != 0)
		{
			hr2message(__LINE__, hr, "g_didevKeyboard->GetDeviceState()");
			return(FALSE);
		}
	}

	/***************************************************************
	*						VERARBEITUNG
	***************************************************************/
	// -- Toggle Debugenemy control
	//
	if (g_arr_cKeystate[DIK_D])
	{
		if (g_cntKeystateCheckTimer[DIK_D] > 0)
		{
			g_cntKeystateCheckTimer[DIK_D] -= 1;
		}
		else {
			g_cntKeystateCheckTimer[DIK_D] = g_maxKeystateCheckTimer;

			if (g_penemyDebug)
			{
				if (g_penemyCamera == g_penemyDebug)
				{
					//g_penemyInput  = g_Player.m_pEnemy;
					g_penemyCamera = g_Player.m_pEnemy;
				}
				else {
					//g_penemyInput  = g_penemyDebug;
					g_penemyCamera = g_penemyDebug;
				}
			}
/*			else {
				PSXInitializeTerminal("g_penemyDebug NULL", NULL, NULL, NULL);
			}
*/
		}
	}
	else {
		g_cntKeystateCheckTimer[DIK_D] = 0;
	}

   /* migrated back to main loop 
   if (g_arr_cKeystate[DIK_X] & 0x80)
   {
	   PostQuitMessage(0);
   }
   */

   
   // -- g_matCameraRotation manipulierbar machen
   //

   //float l_fViewRotation1, l_fViewRotation2, l_fViewRotation3;
   //XMMATRIX l_matViewRotation1,l_matViewRotation2;

   //l_fViewRotation3 = 0.0f;
   //l_fViewRotation1 = 0.0f;
   //l_fViewRotation2 = 0.0f;

   //l_fViewRotation1 = float(diMouseState.lX) * 0.001f;

   //l_fViewRotation2 =  float(diMouseState.lY) * 0.001f;

   //l_matViewRotation1 = XMMatrixRotationY(l_fViewRotation1);
   //l_matViewRotation2 = XMMatrixRotationX(l_fViewRotation2);

   //if (g_penemyInput)
   //{

	  // XMMATRIX l_matCameraTransposed = l_matViewRotation2 * l_matViewRotation1 * *(g_penemyInput->getMatrixPointer());
	  // g_penemyInput->setMat(l_matCameraTransposed);

	  // XMVECTOR l_vPos = g_penemyInput->getPos();
	  // l_vPos = l_vPos + l_matCameraTransposed.r[2] * g_game.m_fVelocity;
	  // g_penemyInput->setPos(l_vPos);

   //}


   //if (g_arr_cKeystate[DIK_UP] & 0x80)
   //{
	  // g_game.m_fVelocity = g_game.m_fVelocity + .0001f;
   //}

   //if (g_arr_cKeystate[DIK_DOWN] & 0x80)
   //{
	  // g_game.m_fVelocity = g_game.m_fVelocity - .0001f;
   //}

   //if (g_arr_cKeystate[DIK_SPACE] & 0x80)
   //{
	  // g_game.m_fVelocity = g_game.m_fVelocity * 0.9f;
   //}




   // -- Kameramatrix vorbereiten
   //
   if (g_bWriteLogFileInLoop) 
   {
      WriteLogFile("deriving current camera position and matrix from Camera-Enemy\n");
   }

   
   // -- Entscheidung, woher die Cameramatrix herkommen soll
   //
/*
   if (g_equipmentArray[EQUIPMENT_TORPEDO].m_bSingleton)
   {
      g_matCamera = g_equipmentArray[EQUIPMENT_TORPEDO].m_matSingleton;
      g_vCamera   = shoot_array[g_equipmentArray[EQUIPMENT_TORPEDO].m_idxShootSingleton].m_vPos;
   }else{
*/

      if (g_penemyCamera) 
      {
		  
         g_vCamera = g_penemyCamera->getPos();
		 
         // -- momentane kamera matrix aus dem enemy transponieren
         //
         //if (g_penemyCamera->m_Action != ENACT_REFLECTION)
         {
            g_matCamera = XMMatrixTranspose(*(g_penemyCamera->getRawMatrixPointer()));

            // -- temporaere Neigung/Schwenkungen um die Bewegung mit den Fluegeln anzuzeigen
            //

			/*if (g_Player.getGS()->IsHavingOwnShip()) */
            
			
			/*
			const dReal* l_pdAngular;
			l_pdAngular = dBodyGetAngularVel(g_penemyCamera->getBodyID());
			if (l_pdAngular)
			{				
								
				
				FXMMATRIX l_matGeneric1 = XMMatrixRotationZ(g_penemyCamera->m_fViewRotation1*20);
				CXMMATRIX l_matGeneric2 = XMMatrixRotationX(g_penemyCamera->m_fViewRotation2*20);
				CXMMATRIX l_matWings = XMMatrixMultiply(l_matGeneric1, l_matGeneric2);
				g_matCamera = XMMatrixMultiply(g_matCamera, l_matWings);

				g_penemyCamera->m_fViewRotation1 = g_penemyCamera->m_fViewRotation1 * 0.99;
				g_penemyCamera->m_fViewRotation2 = g_penemyCamera->m_fViewRotation2 * 0.99;

				if (l_pdAngular[1] > 0)
				{		
					if (l_pdAngular[1] > g_penemyCamera->m_fViewRotation1)
					{
						g_penemyCamera->m_fViewRotation1 = l_pdAngular[1];
					}
				}
				else {
					if (l_pdAngular[1] < g_penemyCamera->m_fViewRotation1)
					{
						g_penemyCamera->m_fViewRotation1 = l_pdAngular[1];
					}

				}

				if (l_pdAngular[2]>0)
				{
					if (l_pdAngular[2] > g_penemyCamera->m_fViewRotation2)
					{
						g_penemyCamera->m_fViewRotation2 = l_pdAngular[2];
					}
				}
				else {
					if (l_pdAngular[2] < g_penemyCamera->m_fViewRotation2)
					{
						g_penemyCamera->m_fViewRotation2 = l_pdAngular[2];
					}
				}
			
				
				
			}
            */
            // -- oben vorbereitete g_matCameraRotate matrix draufschlagen
            //
/*
            if (g_Player.getGS()->IsCameraBehindPlayer())
            {
               matmult(&g_matCamera, &g_matCamera, &g_matCameraRotate );
            }
*/

         }
      }
/* 
   }
*/

   // frÃ¼her war hier das versetzen des betrachters und Anwendung der g_matCameraRotate matrix
   // ist dadurch aktueller. Passiert aber nun _vor_ der aktualisierung von matCameraRotate
   // damits nur dann auf matCamera angewendet wird, wenn es aus dem penemyCamera frisch geholt wurde

   // -- versetzen des betrachters und Anwendung der g_matCameraRotate matrix
   //
/*
   if (g_Player.getGS()->IsCameraBehindPlayer())
   {
*/

	  if (g_penemyCamera)
	  {
		  g_vCamera = g_vCamera
			  - g_penemyCamera->getMatrixPointer()->r[2] * 10.0f
			  + g_penemyCamera->getMatrixPointer()->r[1] * 2.0f;
	  }
      
/*
   }
*/

	// -- Constant buffers je frame
	//
    
	//XMMATRIX l_matCameraTransposed = XMMatrixTranspose(g_matCamera);
	//g_vCamera = g_vCamera + l_matCameraTransposed.r[2] * g_game.m_fVelocity;

	g_View = XMMatrixTranslationFromVector(-g_vCamera) * g_matCamera;
	
	// -- Verzögerungstimer fürs Schiessen pflegen
	//
	{
      int l_idxEquipment;
      for (l_idxEquipment = 0; l_idxEquipment < EQUIPMENT_LASTENUM; l_idxEquipment++)
      {
         if (g_equipmentArray[l_idxEquipment].m_cntShootDelay > 0)
         {
			 g_equipmentArray[l_idxEquipment].m_cntShootDelay =
				 g_equipmentArray[l_idxEquipment].m_cntShootDelay - 1;
                                          //int(g_fElapsedTime);

            if (g_equipmentArray[l_idxEquipment].m_cntShootDelay < 0)
            {
               g_equipmentArray[l_idxEquipment].m_cntShootDelay = 0;
            }
         }
      }

   }


	// -- Einfügen der Schuesse, die nach vorne gehen
	//
	if (g_bWriteLogFileInLoop)
	{
		WriteLogFile("player's shoot/fire\n");
	}


	  

   // -- Abfrage des Mausknopfes: Schuss auslösen oder Menüpunkt wählen
   //
   if (diMouseState.rgbButtons[0] ||  //linke Maustaste
	   diJoystickState2.rgbButtons[0])
   {
	   if (g_cntMousestateCheckTimer[0] > 0)
	  {
		   g_cntMousestateCheckTimer[0] -= 1; // g_fElapsedTime;
	   }
	   else {
		   g_cntMousestateCheckTimer[0] = g_maxMousestateCheckTimer;

		   if (g_bWriteLogFileInLoop)
		   {
			   WriteLogFile("insert new shoot from player\n");
		   }

		   if (g_penemyInput)
		   {
			   if (g_penemyInput->Shoot() == false) return false;
		   }

	   }
   }
   else {
	   g_cntMousestateCheckTimer[0] = 0;
   }
 
	if (g_bWriteLogFileInLoop)
   {
      WriteLogFile("doing shoot/enemy collision \n");
   }

   
   
   //strcpy_s(debugausgabe, g_maxDebugAusgabe, "keine Abfrage insgesamt");
   
   // --- Durch Shoots getroffene Enemies ins pEnemy-Array selektieren
   //
   if (g_cntShoot > 0 && ENEMY::IsAnyEnemy() ) // && g_bSupportingSSE==true)
   {
      int l_cntEnSel, l_idxEnSel, l_idxShoot;
   
      for (l_idxShoot=0; l_idxShoot < g_cntShoot; l_idxShoot++)
      {
         if (shoot_array[l_idxShoot].m_cntTimeToLive > 0)// && shoot_array[l_idxShoot].m_cntHitableEnemies > 0)
         {
            float l_fLenPow2;
            XMVECTOR l_vLenPow2;

            // -- sozusagen als ersatzradius des schussobjektes nehmen
            //
            //l_fLenPow2 = pow2(shoot_array[l_idxShoot].m_vStep.x)+ 
            //             pow2(shoot_array[l_idxShoot].m_vStep.y)+ 
            //             pow2(shoot_array[l_idxShoot].m_vStep.z); 
            l_vLenPow2 = XMVector3LengthSq(shoot_array[l_idxShoot].m_vStep);
            l_fLenPow2 = XMVectorGetX(l_vLenPow2);

			//PSXPrintf(__LINE__, debugausgabe, g_maxDebugAusgabe, "l_fLenPow2 = %f", l_fLenPow2);
			
            l_cntEnSel = shoot_array[l_idxShoot].m_cntHitableEnemies;

            if (shoot_array[l_idxShoot].m_penemyHitableArray)
            {
               l_cntEnSel = shoot_array[l_idxShoot].m_cntHitableEnemies;

               for (l_idxEnSel = 0; l_idxEnSel < l_cntEnSel ; l_idxEnSel++)
               {
                  ENEMY* l_penemyCurr;
                  l_penemyCurr = shoot_array[l_idxShoot].m_penemyHitableArray[l_idxEnSel];
   
                  if (l_penemyCurr->checkIfHitByShoot(l_idxShoot, l_fLenPow2) == false) return false;
               }
            }else{
               ENEMY* l_penemyCurr;

               l_penemyCurr = ENEMY::getFirstEnemy();
               while(l_penemyCurr)
               {
				   //strcpy_s(debugausgabe, g_maxDebugAusgabe, "Abfrage checkIfHitByShoot()");
                  if (l_penemyCurr->checkIfHitByShoot(l_idxShoot, l_fLenPow2) == false) return false;
                  l_penemyCurr = l_penemyCurr->getNextEnemy();
               }
            }
         }
      }
   }

   if (g_bWriteLogFileInLoop)
   {
		WriteLogFile("emptying Collision-JointGroup\n");
   }

  if (LEVEL::m_dJointGroupID)
  {
	  dJointGroupEmpty(LEVEL::m_dJointGroupID);
  }


  if (g_bWriteLogFileInLoop)
  {
	  WriteLogFile("calling enemy-ai/movement-update \n");
  }
  /*
  g_cntGunShootDelay = 0;
  g_fProfilingMilliSec = GetTickCount();

  if (g_Player.getGS()->ShallUpdateEnemies())
  */
  {
	  ENEMY* l_penemyCurr;
	  int l_cntCurrentlyAttackingEnemies;

	  l_cntCurrentlyAttackingEnemies = 0;

	  // -- initialisierungen fÃ¼r ::updateDestFrame()
	  //
	  /*
	  if (g_bTargetLocked == false)
	  {
		  if (ENEMY::setEnemyTarget(NULL) == false) return false;
	  }
	  g_cntTarget = 0;

	  g_bSelectionFrame = false;
	  */
	  l_penemyCurr = ENEMY::getFirstEnemy();
	  while (l_penemyCurr)
	  {

		  if (l_penemyCurr->mType != ETYP_EMPTY)
		  {
			  if (l_penemyCurr->Update() == false)
			  {
				  char l_cFailText[100];
				  PSXPrintf(__LINE__, l_cFailText, sizeof(l_cFailText), "enemy-a.i-Update failed in No %i \n",
					  l_penemyCurr->getEnemyIndex());

				  WriteLogFile(l_cFailText);
				  return (FALSE);
			  }

			  // -- Faktor zur Alarmstufebestimmung pflegen
			  //
			  /*
			  if (l_penemyCurr->isTriggered() &&
			  l_penemyCurr->m_fDestLength < pow2(3000.0f))
			  {
				  l_cntCurrentlyAttackingEnemies++;
			  }
			  */

		  }
		  l_penemyCurr = l_penemyCurr->getNextEnemy();
	  } // end-for alle enemy objekte
	  /*
	  if (g_options.IsWriteLogFileInLoop())
	  {
		  WriteLogFile("check LockTarget\n");
	  }

		  // -- falls durch die permanente Bestimmung ein Index rausgerutscht ist
		  //
		  if (g_cntTarget > 0)
		  {
			  if (g_idxTarget >= g_cntTarget)
			  {
				  g_idxTarget = g_cntTarget - 1;
			  }
			  // -- Page Up
			  //
			  if (g_arr_cKeystate[DIK_PRIOR] & 0x80)
			  {
				  if (g_cntKeystateCheckTimer[DIK_PRIOR]> 0)
				  {
					  g_cntKeystateCheckTimer[DIK_PRIOR] -= g_fElapsedTime;
				  }
				  else {
					  g_cntKeystateCheckTimer[DIK_PRIOR] = g_maxKeystateCheckTimer;

					  g_idxTarget--;
					  if (g_idxTarget < 0)
					  {
						  g_idxTarget = g_cntTarget - 1;
					  }

					  g_bTargetInfo = true;
					  if (ENEMY::setEnemyTarget(g_penemyTargetArray[g_idxTarget]) == false) return false;
					  g_bTargetLocked = true;

				  }
			  }
			  else {
				  g_cntKeystateCheckTimer[DIK_PRIOR] = 0;
			  }

			  // -- Page Down
			  //
			  if (g_arr_cKeystate[DIK_NEXT] & 0x80 || diJoystickState2.rgbButtons[1])
			  {
				  if (g_cntKeystateCheckTimer[DIK_NEXT]> 0)
				  {
					  g_cntKeystateCheckTimer[DIK_NEXT] -= g_fElapsedTime;
				  }
				  else {
					  g_cntKeystateCheckTimer[DIK_NEXT] = g_maxKeystateCheckTimer;

					  g_idxTarget++;
					  if (g_idxTarget >= g_cntTarget)
					  {
						  g_idxTarget = 0;
					  }

					  g_bTargetInfo = true;
					  if (ENEMY::setEnemyTarget(g_penemyTargetArray[g_idxTarget]) == false) return false;
					  g_bTargetLocked = true;
				  }
			  }
			  else {
				  g_cntKeystateCheckTimer[DIK_NEXT] = 0;
			  }
		  }

		  if (g_options.IsWriteLogFileInLoop())
		  {
			  WriteLogFile("update condition\n");
		  }
		  if (g_Player.updateCondition(l_cntCurrentlyAttackingEnemies) == false) return false;

		  // -- Return: Target Lock
		  //
		  if (g_arr_cKeystate[DIK_RETURN] & 0x80)
		  {
			  if (g_cntKeystateCheckTimer[DIK_RETURN]> 0)
			  {
				  g_cntKeystateCheckTimer[DIK_RETURN] -= g_fElapsedTime;
			  }
			  else {
				  g_cntKeystateCheckTimer[DIK_RETURN] = g_maxKeystateCheckTimer;

				  g_bTargetInfo = false;
				  g_bTargetLocked = true;
			  }
		  }
		  else {
			  g_cntKeystateCheckTimer[DIK_RETURN] = 0;
		  }
		  // -- N: Next Enemy auf Target Lock
		  //
		  if (g_arr_cKeystate[DIK_N] & 0x80)
		  {
			  if (g_penemyNextTriggered)
			  {
				  g_bTargetInfo = true;
				  if (ENEMY::setEnemyTarget(g_penemyNextTriggered) == false) return false;
				  g_bTargetLocked = true;
			  }
		  }

		  // -- F: Next Untriggered auf Target Lock
		  //
		  if (g_arr_cKeystate[DIK_F] & 0x80)
		  {
			  if (g_penemyNextUntriggered)
			  {
				  g_bTargetInfo = true;
				  if (ENEMY::setEnemyTarget(g_penemyNextUntriggered) == false) return false;
				  g_bTargetLocked = true;
			  }
		  }

		  // -- space: Target Lock aufheben
		  //
		  if (g_arr_cKeystate[DIK_SPACE] & 0x80)
		  {
			  if (g_cntKeystateCheckTimer[DIK_SPACE]> 0)
			  {
				  g_cntKeystateCheckTimer[DIK_SPACE] -= g_fElapsedTime;
			  }
			  else {
				  g_cntKeystateCheckTimer[DIK_SPACE] = g_maxKeystateCheckTimer;

				  g_bTargetLocked = false;
				  g_bTargetInfo = false;

				  if (g_equipmentArray[EQUIPMENT_MISSILE].m_bSingleton)
				  {
					  int l_idxShoot;

					  l_idxShoot = g_equipmentArray[EQUIPMENT_MISSILE].m_idxShootSingleton;
					  shoot_array[l_idxShoot].insertImpact();
					  shoot_array[l_idxShoot].Die();
				  }

				  if (g_equipmentArray[EQUIPMENT_MINE].m_bSingleton)
				  {
					  int l_idxShoot;

					  l_idxShoot = g_equipmentArray[EQUIPMENT_MINE].m_idxShootSingleton;
					  shoot_array[l_idxShoot].insertImpact();
					  shoot_array[l_idxShoot].Die();
				  }

				  if (g_equipmentArray[EQUIPMENT_TORPEDO].m_bSingleton)
				  {
					  int l_idxShoot;

					  l_idxShoot = g_equipmentArray[EQUIPMENT_TORPEDO].m_idxShootSingleton;
					  shoot_array[l_idxShoot].insertImpact();
					  shoot_array[l_idxShoot].Die();
				  }
			  }
		  }
		  else {
			  g_cntKeystateCheckTimer[DIK_SPACE] = 0;
		  }
		  if (g_options.IsWriteLogFileInLoop())
		  {
			  WriteLogFile("set Selection Frame\n");
		  }

		  // -- Targetinfo mit SelectionFrame umrahmen
		  //
		  if (g_bTargetInfo && ENEMY::IsAnyTarget() && (ENEMY::getEnemyTarget())->getPos2DZ() > 0)
		  {
			  g_vSelectionFrame = (ENEMY::getEnemyTarget())->getPos2D();
			  g_bSelectionFrame = true;
			  g_vSelectionFrameScale.x = 0.03f;
			  g_vSelectionFrameScale.y = 0.02f;
		  }
		  else {
			  g_bSelectionFrame = false;
		  }
		  */
   }
   /*
   g_maxGunShootDelay = g_cntGunShootDelay;

   g_fProfileStateArray[PROFILE_ENEMYUPDATE] = GetTickCount() - g_fProfilingMilliSec;
*/

   if (LEVEL::m_dSpaceID)/* && g_Player.getGS()->ShallUpdateEnemies())*/
   {
	   // -- kollision der enemies untereinander
	   //
	   if (g_bWriteLogFileInLoop)
	   {
		   WriteLogFile("doing enemy/enemy collision \n");
	   }
	   dSpaceCollide(LEVEL::m_dSpaceID, NULL, ENEMY::dNearCallback);
   }
   
   // --- Verwalten der Schuesse
   //
   //if (g_Player.getGS()->ShallUpdateShoots())
   {
      int l_idxShoot;

      if (g_bWriteLogFileInLoop)
      {
         WriteLogFile("moving shoot/fire\n");
      }

      // -- moving shoots
      //
      for (l_idxShoot=0;l_idxShoot<g_cntShoot; l_idxShoot++)
      {
         if (shoot_array[l_idxShoot].Update()==false) return false;
      }

	  // -- compressing shootarray: delete zero-timetolive items
	  //    and recalculate cntShoot
	  //
	  g_cntShoot = 0;
	  for (l_idxShoot = 0; l_idxShoot < g_maxShoot; l_idxShoot++)
	  {
		  if (shoot_array[l_idxShoot].m_cntTimeToLive > 0)
		  {
			  if (g_cntShoot != l_idxShoot)
			  {
				  shoot_array[g_cntShoot] = shoot_array[l_idxShoot];

				  // -- falls sich der Eintrag fuer die Smartbomb mitverschoben hat
				  //
					   /*
				  if (l_idxShoot == g_equipmentArray[EQUIPMENT_TORPEDO].m_idxShootSingleton)
				  {
					 g_equipmentArray[EQUIPMENT_TORPEDO].m_idxShootSingleton = g_cntShoot;
				  }

				  if (l_idxShoot == g_equipmentArray[EQUIPMENT_MISSILE].m_idxShootSingleton)
				  {
					 g_equipmentArray[EQUIPMENT_MISSILE].m_idxShootSingleton = g_cntShoot;
				  }

				  if (l_idxShoot == g_equipmentArray[EQUIPMENT_MINE].m_idxShootSingleton)
				  {
					 g_equipmentArray[EQUIPMENT_MINE].m_idxShootSingleton = g_cntShoot;
				  }
					   */

			  }
			  g_cntShoot++;
		  }
	  }

	  // -- zero out the rest
	  //
	  if (g_cntShoot < g_maxShoot)
	  {
		  memset(shoot_array + g_cntShoot, 0, sizeof(SHOOT)*(g_maxShoot - g_cntShoot));
	  }
   }

   if (g_bWriteLogFileInLoop)
   {
	   WriteLogFile("invoking dWorldQuickStep()\n");
   }

   if (LEVEL::m_iWorldID) dWorldQuickStep(LEVEL::m_iWorldID, 0.5f); //0.4f);

   return true;
}

bool GAME::handleKey(WORD wordKey)
{
	XMMATRIX l_matViewRotation1;

	switch (wordKey)
	{
	case 0x53:
		l_matViewRotation1 = XMMatrixRotationX(-0.1f);
		break;

	case 0x57: // w
		
		l_matViewRotation1 = XMMatrixRotationX(0.1f);
		break;

	
	}
	g_matCamera = g_matCamera * l_matViewRotation1;
	g_View = XMMatrixTranslationFromVector(-g_vCamera) * g_matCamera;
	
	return true;
}


// --- PSXAllocateShoot()
//     sucht freien Eintrag in Schusstabelle und fuegt neuen ein
//
//     in  l_cntNeeded, pShoot[]
//     out l_cntFound,  pShoot[]
//
int PSXAllocateShoot(int l_cntNeeded,SHOOT** l_arr_pShoot)
{
   int l_idxShoot; 
   int l_cntFound;

   // -- init Rueckgabe array 
   //
   for (int i=0; i< l_cntNeeded; i++)
   {
      l_arr_pShoot[i] = NULL;
   }

   // -- finde benoetigte freie positionen
   //
   for (l_idxShoot = 0, l_cntFound = 0; 
        (l_idxShoot<g_maxShoot) && (l_cntFound<l_cntNeeded); 
        l_idxShoot ++)
   {
      if (shoot_array[l_idxShoot].m_cntTimeToLive == 0)
      {
         //g_idxNewestShoot = l_idxShoot;
         l_arr_pShoot[l_cntFound] = shoot_array+l_idxShoot;
         l_cntFound++;
      }
   }

   if (l_cntFound < l_cntNeeded)
   {
      char l_cNeedFailText[100];
      PSXPrintf(__LINE__, l_cNeedFailText, sizeof(l_cNeedFailText), "need more shootentries allocated "
                               " - needed %i, got %i \n",
              l_cntNeeded, l_cntFound);
      WriteLogFile(l_cNeedFailText);
   }

   return l_cntFound;
}


/*
// -- bestimmt in welche Richtung der LaserSchweif geht
//
bool SHOOT::determineTrail()
{
   PSXVECTOR l_vPos1, l_vPos2;
   float xDest, xydiv, xDest2;
   bool l_bValid;
   l_vPos1.x = m_vPos.x;
   l_vPos1.y = m_vPos.y;
   l_vPos1.z = m_vPos.z;

   l_vPos2.x = m_vPos.x + m_vStep.x/4.0f;
   l_vPos2.y = m_vPos.y + m_vStep.y/4.0f;
   l_vPos2.z = m_vPos.z + m_vStep.z/4.0f;

   xDest = g_matViewProj._11*l_vPos1.x+
           g_matViewProj._21*l_vPos1.y+
           g_matViewProj._31*l_vPos1.z+
           g_matViewProj._41;
   xydiv = g_matViewProj._14*l_vPos1.x+
           g_matViewProj._24*l_vPos1.y+
           g_matViewProj._34*l_vPos1.z+
           g_matViewProj._44;
   if (xydiv==0.0f) 
   {
      xydiv = 1.0f;
      l_bValid = false;
      WriteLogFile("xydiv == 0.0f while shoot-drawing\n");

      if (l_vPos1.x == 0.0f &&
          l_vPos1.y == 0.0f &&
          l_vPos1.z == 0.0f )
      {
         WriteLogFile("weil alle Positionsangaben 0 sind\n");
      }
      if (g_matViewProj._14 == 0.0f &&
          g_matViewProj._24 == 0.0f &&
          g_matViewProj._34 == 0.0f &&
          g_matViewProj._44 == 0.0f)
      {
         WriteLogFile("last row of View/Projmatrix is completely zeroed\n");
      }
   }
   xDest = xDest / xydiv;

   xDest2 = g_matViewProj._11*l_vPos2.x+
            g_matViewProj._21*l_vPos2.y+
            g_matViewProj._31*l_vPos2.z+
            g_matViewProj._41;
   xydiv = g_matViewProj._14*l_vPos2.x+
           g_matViewProj._24*l_vPos2.y+
           g_matViewProj._34*l_vPos2.z+
           g_matViewProj._44;
   if (xydiv==0.0f) 
   {
      xydiv = 1.0f;

      if (l_vPos2.x == 0.0f &&
          l_vPos2.y == 0.0f &&
          l_vPos2.z == 0.0f )
      {
         WriteLogFile("weil alle Positionsangaben 0 sind\n");
      }
      l_bValid = false;
   }
   xDest2 = xDest2 / xydiv;

   if (xDest2 < xDest)
   {
      m_fTrailFactor = -1.0f;
   } else {
      m_fTrailFactor = +1.0f;
   }

   return true;
}
*/

// -- sofortige Neutralisierung
//
bool SHOOT::Die()
{
   int l_idxSndRes;

   m_cntTimeToLive = 0; // der wichtigste Teil

   //g_cntShootDeleted++;

   /*
   if (g_equipmentArray[EQUIPMENT_TORPEDO].m_bSingleton)
   {
      if ((this - shoot_array) == g_equipmentArray[EQUIPMENT_TORPEDO].m_idxShootSingleton)
      {
         if (g_Player.getGS()->setIndex(GAMESCREEN_COCKPIT) == false) return false;
         g_equipmentArray[EQUIPMENT_TORPEDO].m_bSingleton = false;
      }
   }

   if (g_equipmentArray[EQUIPMENT_MISSILE].m_bSingleton)
   {
      if ((this - shoot_array) == g_equipmentArray[EQUIPMENT_MISSILE].m_idxShootSingleton)
      {
         g_equipmentArray[EQUIPMENT_MISSILE].m_bSingleton = false;
      }
   }

   if (g_equipmentArray[EQUIPMENT_MINE].m_bSingleton)
   {
      if ((this - shoot_array) == g_equipmentArray[EQUIPMENT_MINE].m_idxShootSingleton)
      {
         g_equipmentArray[EQUIPMENT_MINE].m_bSingleton = false;
      }
   }

   if (g_options.IsEnableSound())
   {
      for (l_idxSndRes = 0; l_idxSndRes < SNDRES_LASTENUM; l_idxSndRes++)
      {
         if (g_sound_array[l_idxSndRes].m_pShoot == this) 
         {
            if (g_sound_array[l_idxSndRes].stopLooping() == false) return false;
            g_sound_array[l_idxSndRes].m_pShoot = NULL;
         }
      }
   }
   */
   return true;
}

// -- Damit sich der Schuss fortbewegt
//
bool SHOOT::Update()
{
   if (m_cntTimeToLive > 0)
   {
      m_cntTimeToLive = m_cntTimeToLive - 1;


      /*
      if (m_cntTimeToLive == 0)
      {
         if (Die()==false) return false;
      }
      */

      m_vPos = m_vPos + m_vStep * 0.06f;

      m_fViewRotation1 = m_fViewRotation1 + 0.2f;
      m_fScale = m_fScale * 0.9f;
      if (m_fScale < 0.7f)
      {
         m_fScale = 1.0f;
      } 
   }
   return true;
}

// --- Selektiert weiterhin auf Kollision zu pruefende Enemies
//
/*
bool SHOOT::selectHitableEnemies()
{
   int l_cntHitPoints;
   ENEMY* l_penemyCurr;
   float l_fLengthPow2;
   PSXVECTOR l_vDelta;

   // -- wieder an Anfang wenn die Enemies nicht mehr ganz reinpassen wuerden 
   //
   if (g_cntHitableEnemies + g_maxHitablePerShoot > g_maxHitableEnemies)
   {
      g_cntHitableEnemies = 0;
   }

   m_cntHitableEnemies = 0;
   m_penemyHitableArray = g_penemyHitableArray + g_cntHitableEnemies;

   l_fLengthPow2 = pow2(m_vStep.x) + 
                   pow2(m_vStep.y) + 
                   pow2(m_vStep.z);

   l_penemyCurr = ENEMY::getFirstEnemy();
   while (l_penemyCurr && (m_cntHitableEnemies < g_maxHitablePerShoot))
   {
      if ((l_penemyCurr->mType != ETYP_EMPTY) &&
          (l_penemyCurr->mType != ETYP_EXPLOSION) &&
          (l_penemyCurr->mType != ETYP_IMPACT))
      {
         //if (l_penemyCurr->isVisible() == true && l_penemyCurr->getPos2DZ() > 0.0f)
         {

            float b,c;
    
            l_vDelta.x = m_vPos.x - l_penemyCurr->getPosX();
            l_vDelta.y = m_vPos.y - l_penemyCurr->getPosY();
            l_vDelta.z = m_vPos.z - l_penemyCurr->getPosZ();

            b = 2 * ( m_vStep.x * ( l_vDelta.x) +
                      m_vStep.y * ( l_vDelta.y) +
                      m_vStep.z * ( l_vDelta.z));

            c = pow2(l_vDelta.x) +
                pow2(l_vDelta.y) +
                pow2(l_vDelta.z) -
                l_penemyCurr->m_fShootHitRadPow2 - 
                l_fLengthPow2;

            l_cntHitPoints = int(pow2(b) - 4.0f * l_fLengthPow2 * c);
                                      
            if (l_cntHitPoints>=0)
            {
               m_penemyHitableArray[m_cntHitableEnemies] = l_penemyCurr;
               m_cntHitableEnemies++;
               g_cntHitableEnemies++;
            }
         }
      }
      l_penemyCurr = l_penemyCurr->getNextEnemy();
   }

   if (m_cntHitableEnemies >= g_maxHitablePerShoot)
   {
      hr2message(__LINE__,-1, "too many hitable Enemies to save them into penemyHitableArray!\n");
      return false;
   }
   
   return true;
}
*/

// --- neuen Enemy eintrag für pseudo explosion
//     "aufschlagene Laser Bolzen"
//
bool SHOOT::insertImpact()
{
   ENEMY* l_pNewEnemy;

   l_pNewEnemy = new (LIFECYCLE_LEVEL) EXPLOSION(LIFECYCLE_LEVEL);
   if (l_pNewEnemy != NULL)
   { 
      XMVECTOR l_vEnemyStep;

      //l_fShootLength = pow2(m_vStep.x)+
      //                 pow2(m_vStep.y)+
      //                 pow2(m_vStep.z);

      //if (l_fShootLength > 0.0f)
      //{
      //   l_fShootLength = sqrt(l_fShootLength);
      //}
      //l_pNewEnemy->Init();
      
      l_pNewEnemy->SetType(ETYP_IMPACT);
      l_pNewEnemy->setPos(m_vPos);

      l_vEnemyStep = XMVector3NormalizeEst(m_vStep);
      //l_vEnemyStep.x = m_vStep.x / l_fShootLength;
      //l_vEnemyStep.y = m_vStep.y / l_fShootLength;
      //l_vEnemyStep.z = m_vStep.z / l_fShootLength;
      if (l_pNewEnemy->setStep(l_vEnemyStep)== false) return false;

      l_pNewEnemy->setVisible(true);
      l_pNewEnemy->setTriggered(true);
      l_pNewEnemy->m_fExplScale = 1.0f;
      //l_pNewEnemy->m_iFightExperience     = 0; 
   }
   return true;
}

// -- erzaubert eine ganze Matrix aus nur einem Vektor
//
bool PSXMatrixFromVector( XMMATRIX* l_pmatDest, XMVECTOR l_vSrc)
{
   XMVECTOR l_vCrossProduct;
   float l_fLength;

   XMVECTORF32 l_vSrc90Degree = { XMVectorGetY(l_vSrc), -XMVectorGetX(l_vSrc), 0.0f}; 

   //l_fLength = sqrt(pow2(l_vSrc90Degree.x)+
   //                 pow2(l_vSrc90Degree.y)+
   //                 pow2(l_vSrc90Degree.z));
   //l_vSrc90Degree.x = l_vSrc90Degree.x / l_fLength;
   //l_vSrc90Degree.y = l_vSrc90Degree.y / l_fLength;
   //l_vSrc90Degree.z = l_vSrc90Degree.z / l_fLength;

   l_vCrossProduct = XMVector3Cross(l_vSrc, XMVector3Normalize(l_vSrc90Degree));
   //l_vCrossProduct.x = (l_vSrc.y * l_vSrc90Degree.z - l_vSrc.z * l_vSrc90Degree.y);
   //l_vCrossProduct.y = (l_vSrc.z * l_vSrc90Degree.x - l_vSrc.x * l_vSrc90Degree.z);
   //l_vCrossProduct.z = (l_vSrc.x * l_vSrc90Degree.y - l_vSrc.y * l_vSrc90Degree.x);

   //l_fLength = sqrt(pow2(l_vCrossProduct.x)+
   //                 pow2(l_vCrossProduct.y)+
   //                 pow2(l_vCrossProduct.z));

   //l_vCrossProduct.x = l_vCrossProduct.x / l_fLength;
   //l_vCrossProduct.y = l_vCrossProduct.y / l_fLength;
   //l_vCrossProduct.z = l_vCrossProduct.z / l_fLength;
   l_vCrossProduct = XMVector3Normalize(l_vCrossProduct);

   //memcpy(l_pmatDest, &g_matIdentity, sizeof(g_matIdentity));
   if (l_pmatDest) {
      *l_pmatDest = XMMatrixIdentity();
      l_pmatDest->r[0] = l_vSrc90Degree;
      l_pmatDest->r[1] = l_vCrossProduct;
      l_pmatDest->r[2] = l_vSrc;
   }else{
      hr2message(__LINE__,-1,"PSXMatrixFromVector(l_pmatDest=NULL)");
      return false;
   }
   //l_pmatDest->_11 = l_vSrc90Degree.x;
   //l_pmatDest->_12 = l_vSrc90Degree.y;
   //l_pmatDest->_13 = l_vSrc90Degree.z;

   //l_pmatDest->_21 = l_vCrossProduct.x;
   //l_pmatDest->_22 = l_vCrossProduct.y;
   //l_pmatDest->_23 = l_vCrossProduct.z;

   //l_pmatDest->_31 = l_vSrc.x;
   //l_pmatDest->_32 = l_vSrc.y;
   //l_pmatDest->_33 = l_vSrc.z;

   return true;
}


// -- prueft ob mit Ausrüstung korrespondierender Bestandsartikel verfuegbar
//

bool EQUIPMENT::isEnabled()
{
	/*
   if (g_iShipsHoldArray[m_commgoodid] > 0)
   {
      return true;
   }else{
      return false;
   }
   */
	return true;
}
/*
float EQUIPMENT::getShootRangePow2()
{
   if (g_Player.m_pEnemy)
   {
      return pow2((g_Player.m_pEnemy->m_fSpeed + m_fSpeed) * m_maxTimeToLive);
   }else{
      return pow2((1.0f + m_fSpeed) * m_maxTimeToLive);
   }
}
*/


bool EQUIPMENT::insertShoot()
{

   /*
   if (m_bSingleton) return true; // wenn dieser bereits in abgeworfen ist
   if ((this - g_equipmentArray) == EQUIPMENT_MISSILE)
   {
      if (g_bTargetLocked == NULL)
      {
         if (PSXInitializeTerminal(g_cLang[txxNoTargetSet], NULL, NULL, NULL) == false)
         {
            return false;
         }
         return true;
      }

   }
   */

   if (isEnabled() && g_Player.m_pEnemy)
   {
      if(m_fTemperature<115.0f) 
      {
         m_fTemperature = m_fTemperature + 0.3f;
      }
      if (m_fTemperature < 100.0f ) //|| g_bCheatGodMode)
      {
         if (m_cntShootDelay == 0) 
         {
            SHOOT* l_arr_pShoot[4];
            int    l_cntFound;
				char   l_cDebugString[400];
   
				//XMFLOAT3 l_float3LoadDummy = { 0.5f, 0.0f, 1.0f };

            m_cntShootDelay = 50; //m_maxShootDelay; //float(m_maxShootDelay)/g_fBalanceFactor;

				XMMATRIX l_matPlayerTransposed = XMMatrixTranspose(*(g_Player.m_pEnemy->getMatrixPointer()));


            l_cntFound = PSXAllocateShoot(m_cntSpawn, l_arr_pShoot);
      
   //       PSXPrintf(__LINE__, l_cDebugString,sizeof(l_cDebugString), "%i shoots allocated\n",l_cntFound);
	//			WriteLogFile(l_cDebugString);

            for (int i=0; i < l_cntFound; i++)
            {


				   l_arr_pShoot[i]->m_vPos = g_Player.m_pEnemy->getPos() + XMVector3Transform(m_vSpawn[i], *(g_Player.m_pEnemy->getMatrixPointer()));

				   XMVECTOR l_vStep = g_Player.m_pEnemy->getMatrixPointer()->r[2];
               //l_arr_pShoot[i]->m_vPos.x = g_Player.m_pEnemy->getPosX()+ 
               //                            m_vSpawn[i].x * l_pMat->_11 +
               //                            m_vSpawn[i].y * l_pMat->_21 +
               //                            m_vSpawn[i].z * l_pMat->_31;
   
               //l_arr_pShoot[i]->m_vPos.y = g_Player.m_pEnemy->getPosY()+ 
               //                            m_vSpawn[i].x * l_pMat->_12 +
               //                            m_vSpawn[i].y * l_pMat->_22 +
               //                            m_vSpawn[i].z * l_pMat->_32;
   
               //l_arr_pShoot[i]->m_vPos.z = g_Player.m_pEnemy->getPosZ()+ 
               //                            m_vSpawn[i].x * l_pMat->_13 +
               //                            m_vSpawn[i].y * l_pMat->_23 +
               //                            m_vSpawn[i].z * l_pMat->_33;
   

				   l_arr_pShoot[i]->m_vStep = l_vStep * m_fSpeed;// *(g_Player.m_pEnemy->m_fSpeed + m_fSpeed);
				   //l_arr_pShoot[i]->m_vStep = XMLoadFloat3(&l_float3LoadDummy);

					
               //l_arr_pShoot[i]->m_vStep.x = l_pMat->_31 * (g_Player.m_pEnemy->m_fSpeed + m_fSpeed);
               //l_arr_pShoot[i]->m_vStep.y = l_pMat->_32 * (g_Player.m_pEnemy->m_fSpeed + m_fSpeed);
               //l_arr_pShoot[i]->m_vStep.z = l_pMat->_33 * (g_Player.m_pEnemy->m_fSpeed + m_fSpeed);
   
               l_arr_pShoot[i]->menacetype       = MENACE_ENEMY;
               l_arr_pShoot[i]->m_idxShootDrawer = m_idxShootDrawer;
               l_arr_pShoot[i]->m_pEquipment = this;
   
               l_arr_pShoot[i]->m_cntTimeToLive = 50; //m_maxTimeToLive;
               l_arr_pShoot[i]->m_maxTimeToLive = 50; //m_maxTimeToLive;
   
               // -- dies ist ein Fernlenktorpedos
               //
			   /*
               switch (this - g_equipmentArray) 
               {

                  case EQUIPMENT_TORPEDO:
                     if (g_Player.getGS()->setIndex(GAMESCREEN_SMARTBOMB) == false) return false;

                     m_idxShootSingleton = l_arr_pShoot[i] - shoot_array;
                     memcpy(&m_matSingleton, g_Player.m_pEnemy->getMatrixPointer(),sizeof(m_matSingleton));
                     m_bSingleton = true;
         
                     l_arr_pShoot[i]->menacetype           = MENACE_BOTH;
                     l_arr_pShoot[i]->m_penemyHitableArray = NULL; 
                     l_arr_pShoot[i]->m_cntHitableEnemies  = 1; 

                     if (g_iShipsHoldArray[GOOID_TORPEDO] > 0)
                     { 
                        g_iShipsHoldArray[GOOID_TORPEDO]--;
                     }
                     break;

                  case EQUIPMENT_MISSILE:
                     if (g_options.IsEnableSound())
                     {
                        if (g_sound_array[SNDRES_MISSILEENGINE].playLooped() == false) return false;
                        g_sound_array[SNDRES_MISSILEENGINE].m_pShoot = l_arr_pShoot[0];
                     }

                     m_idxShootSingleton = l_arr_pShoot[i] - shoot_array;
                     memcpy(&m_matSingleton, l_pMat, sizeof(m_matSingleton));
                     m_bSingleton = true;
         
                     l_arr_pShoot[i]->m_penemyHitableArray = NULL; 
                     l_arr_pShoot[i]->m_cntHitableEnemies = 1; 

                     if (g_iShipsHoldArray[GOOID_MISSILE] > 0)
                     { 
                        g_iShipsHoldArray[GOOID_MISSILE]--;
                     }
                     break;

                  case EQUIPMENT_MINE:

                     m_idxShootSingleton = l_arr_pShoot[i] - shoot_array;
                     memcpy(&m_matSingleton, l_pMat, sizeof(m_matSingleton));
                     m_bSingleton = true;
         
                     l_arr_pShoot[i]->m_vStep.x = 0.0f; 
                     l_arr_pShoot[i]->m_vStep.y = 0.0f; 
                     l_arr_pShoot[i]->m_vStep.z = 0.0f; 
                     l_arr_pShoot[i]->m_penemyHitableArray = NULL; 
                     l_arr_pShoot[i]->m_cntHitableEnemies = 1; 

                     if (g_iShipsHoldArray[GOOID_MINE] > 0)
                     { 
                        g_iShipsHoldArray[GOOID_MINE]--;
                     }
                     break;

                  default:
                     l_arr_pShoot[i]->selectHitableEnemies();
               }
			   */
   
               //g_cntShootInserted++;
      
            } // end-for i=0 to l_cntFound
           
			/*if (g_Player.getGS()->isHavingTextOverlay())
			{
				if (g_Player.getGS()->applyUpdateText(g_Player.getGS()->getIndex()) == false)
				{
					return false;
				}
			}*/

            // --- play shoot sound 
            //
			/*
            if (g_options.IsEnableSound())
            {
               HRESULT hr;
               hr = g_sound_array[m_sndresShoot].m_SoundBuf->SetCurrentPosition(0);
               hr = g_sound_array[m_sndresShoot].m_SoundBuf->Play(0, 0, 0);
               if (hr != 0) { 
                  hr2message(__LINE__,hr,"lpDirectSoundBuffer->Play()");
                  return false;
               }
 
            }
			*/
         }
      }
   } 

   return true;
}
         
bool EQUIPMENT::update()
{
   if(m_fTemperature>0.0f)
   {
      //m_fTemperature= m_fTemperature - (g_fBalanceFactor*0.02f)-0.1f;
	  m_fTemperature = m_fTemperature - (0.02f) - 0.1f;
	  
   }else{
      m_fTemperature=0.0f;
   }
   return true;
}

bool EQUIPMENT::init()
{
//   m_commgoodid = GOOID_PULSELASER;
   m_bIsAWeapon = false;
   m_cName      = "unnamed Equipment";
   m_maxTimeToLive = 30;
   m_fTemperature  = 0.0f;
   m_cntShootDelay = 0;
   m_maxShootDelay = 8;
   m_fSpeed        = 4.0f;
   /*m_sndresShoot   = SNDRES_OWNSHOOT;*/

   m_bSingleton = false;
   m_idxShootDrawer = SHOOTDRAWER_REDLASER;
   m_cntSpawn = 0;
   return true;
   
}


// -- Kollisionsabfragekern: stellt Faceintersection mit Position/Flugvektor fest
//
bool PSXFaceIntersection(XMVECTOR     l_vPos,
                         XMVECTOR     l_vStep,
                         float        l_fRadPow2,
                         CXMMATRIX     l_matView,
                         RESTYPE       l_resPart,
                         PSXFACEINFO*  l_faceinfCurr,
                         PSXFACEINFO** l_pfaceinfSelectedArray,

                         bool*         l_pbIntersectAtValid, 
                         float*        l_pfIntersectAt,
                         PSXFACEINFO** l_ppfaceinfIntersect)
{
   XMVECTOR l_vFace;
   int l_cntHits, l_cntSuspects, l_idxBigResItem;
   
   *l_pbIntersectAtValid = false;

   l_cntHits     = 0;
   l_cntSuspects = 0;
   
   {
      int l_cntFaces;
      //int l_iAlignedPosArray[4];

      PSXFACE*   l_faceArray;
      PSXVERTEX* l_vertArray;

      int l_cntSuspect, l_idxSuspect, l_idxFace;
 
      l_cntFaces  = g_meshresManager.getFaceCount((RESTYPE)l_resPart);
      l_faceArray = g_meshresManager.getFaceArray((RESTYPE)l_resPart);
      l_vertArray = g_meshresManager.getVertexArray((RESTYPE)l_resPart);

      //memset((void*)l_iAlignedPosArray, 0, sizeof(l_iAlignedPosArray));
      //memcpy((void*)l_iAlignedPosArray, (void*)&l_vPos, sizeof(l_vPos));

      // -- fuer jeden Hinweis tatsaechliche Faceintersection pruefen
      //
      for (l_idxFace=0; l_idxFace < l_cntFaces; l_idxFace++)
      {
         XMVECTOR l_vLineArray[3];
         int l_idxVert[3];
         float l_fLengthPow2;

         XMVECTOR l_vDeltaArray2D[3], l_vLineArray2D[4];
         XMFLOAT3 l_float3LineArray2D[4];
         float l_fIncrement[3];
         HRESULT hr;
         bool l_bCrossed;
         bool l_bInclusion;

	 XMVECTORF32 l_vCentroid = {l_faceinfCurr[l_idxFace].vCentroid.x,
		                    l_faceinfCurr[l_idxFace].vCentroid.y,
		                    l_faceinfCurr[l_idxFace].vCentroid.z};
         l_vFace = l_vCentroid - l_vPos;
         //l_vFace.x = l_faceinfCurr[l_idxFace].vCentroid.x - l_vPos.x;
         //l_vFace.y = l_faceinfCurr[l_idxFace].vCentroid.y - l_vPos.y;
         //l_vFace.z = l_faceinfCurr[l_idxFace].vCentroid.z - l_vPos.z;
         //l_fLengthPow2 = pow2( l_vFace.x)+ pow2( l_vFace.y)+ pow2( l_vFace.z) - l_fRadPow2;
	 XMVECTOR l_vLengthPow2 = XMVector3LengthSq(l_vFace);
         l_fLengthPow2 = XMVectorGetX(l_vLengthPow2) - l_fRadPow2;

         if (l_fLengthPow2 < l_faceinfCurr[l_idxFace].fRadPow2 * 4.0f)
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

	       XMVECTORF32 l_vFaceVert = { l_vertArray[l_idxVert[j]].x ,
                                           l_vertArray[l_idxVert[j]].y ,
                                           l_vertArray[l_idxVert[j]].z };

               //x = l_vertArray[l_idxVert[j]].x - l_vPos.x;
               //y = l_vertArray[l_idxVert[j]].y - l_vPos.y;
               //z = l_vertArray[l_idxVert[j]].z - l_vPos.z;
	       XMVECTOR l_vFaceVertDist = l_vFaceVert - l_vPos;
	       XMMATRIX l_matTransposedView = XMMatrixTranspose( l_matView );

	       l_vLineArray2D[j] = XMVector3Transform(l_vFaceVertDist, l_matTransposedView);
               //l_vLineArray2D[j].x = l_matView._11*x + l_matView._12*y + l_matView._13*z;
               //l_vLineArray2D[j].y = l_matView._21*x + l_matView._22*y + l_matView._23*z;
               //l_vLineArray2D[j].z = l_matView._31*x + l_matView._32*y + l_matView._33*z;

            }

            // -- The following algorithm doesn't know that the first vert is also
            //    the last
            //
            l_vLineArray2D[3] = l_vLineArray2D[0];
         
            // -- getting delta values
            //
            for (int m=0; m<3; m++)
            {
               l_vDeltaArray2D[m] = l_vLineArray2D[m+1] - l_vLineArray2D[m];
               //l_vDeltaArray2D[m].x = l_vLineArray2D[m+1].x - l_vLineArray2D[m].x;
               //l_vDeltaArray2D[m].y = l_vLineArray2D[m+1].y - l_vLineArray2D[m].y;
               //l_vDeltaArray2D[m].z = l_vLineArray2D[m+1].z - l_vLineArray2D[m].z;
            }
            
	    // -- convert for easier access
	    //
            for (int m=0; m<4; m++) 
	    {
               XMStoreFloat3(l_float3LineArray2D+m, l_vLineArray2D[m]);
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
               if ( (l_float3LineArray2D[i].y   <=0) && (l_float3LineArray2D[i+1].y >0)||
                    (l_float3LineArray2D[i+1].y <=0) && (l_float3LineArray2D[i].y   >0) )
               {
                  register float l_fAtZero, l_fIncrement;
          
                  l_bCrossed = true;
         
                  l_fIncrement = XMVectorGetX(l_vDeltaArray2D[i]) / XMVectorGetY(l_vDeltaArray2D[i]);
         
                  // -- x-value at zero on y axis 
                  //
                  l_fAtZero = l_float3LineArray2D[i].x -
                              l_float3LineArray2D[i].y * l_fIncrement;
            
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

               XMVECTOR l_vLineList[2];
               PSXVECTOR* l_pvNormal = &(l_faceinfCurr[l_idxFace].vNormal);

	       XMFLOAT3 l_f3Face;
	       XMFLOAT3 l_f3Step;
               XMStoreFloat3(&l_f3Face, l_vFace);
               XMStoreFloat3(&l_f3Step, l_vStep);
	       
               l_fCandidate = 
                 (l_f3Face.x * l_pvNormal->x + 
                  l_f3Face.y * l_pvNormal->y + 
                  l_f3Face.z * l_pvNormal->z) /
                 ( l_f3Step.x * l_pvNormal->x +
                   l_f3Step.y * l_pvNormal->y +
                   l_f3Step.z * l_pvNormal->z);
         
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

			               /* später DebugTriangleArray wiederbeleben
                        if (l_fCandidate <=1.0f)
                        {
                           for (int j=0; j<3; j++)
                           {
                              g_vDebugTriangleArray[j].x = l_vertArray[l_idxVert[j]].x;
                              g_vDebugTriangleArray[j].y = l_vertArray[l_idxVert[j]].y;
                              g_vDebugTriangleArray[j].z = l_vertArray[l_idxVert[j]].z;
         
                              g_bDebugTriangleValid = true;
                           }
                        }
			               */
                     }
                  }else{
                     *l_pbIntersectAtValid = true;
                     *l_pfIntersectAt = l_fCandidate;
                     *l_ppfaceinfIntersect = l_faceinfCurr + l_idxFace;

                     /* später DebugTriangleArray wiederbeleben
                     if (l_fCandidate <=1.0f)
                     {
                        for (int j=0; j<3; j++)
                        {
                           g_vDebugTriangleArray[j].x = l_vertArray[l_idxVert[j]].x;
                           g_vDebugTriangleArray[j].y = l_vertArray[l_idxVert[j]].y;
                           g_vDebugTriangleArray[j].z = l_vertArray[l_idxVert[j]].z;
      
                           g_bDebugTriangleValid = true;
                        }
                     }
                     */
                  }


               }
            }
         }
      } // end-foreach suspect faces

   } // end-if in diesem part

   return true;
}

 
/* 
   A C-program for MT19937, with initialization improved 2002/1/26.
   Coded by Takuji Nishimura and Makoto Matsumoto.

   Before using, initialize the state by using init_genrand(seed)  
   or init_by_array(init_key, key_length).

   Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.                          

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote 
        products derived from this software without specific prior written 
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


   Any feedback is very welcome.
   http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove space)
*/

MARSENNETWISTER::MARSENNETWISTER()
{
   mti=g_maxMTStateVector+1; /* mti==N+1 means mt[N] is not initialized */
   m_iVecOffset = 397;
   m_MatrixA = 0x9908b0dfUL;   /* constant vector a */
   m_dwUpperMask = 0x80000000UL; /* most significant w-r bits */
   m_dwLowerMask = 0x7fffffffUL; /* least significant r bits */
}

/* initializes mt[N] with a seed */
void MARSENNETWISTER::init_genrand(unsigned long s)
{
    mt[0]= s & 0xffffffffUL;
    for (mti=1; mti<g_maxMTStateVector; mti++) {
        mt[mti] = 
	    (1812433253UL * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti); 
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        /* In the previous versions, MSBs of the seed affect   */
        /* only MSBs of the array mt[].                        */
        /* 2002/01/09 modified by Makoto Matsumoto             */
        mt[mti] &= 0xffffffffUL;
        /* for >32 bit machines */
    }
}

/* initialize by an array with array-length */
/* init_key is the array for initializing keys */
/* key_length is its length */
/* slight change for C++, 2004/2/26 */
void MARSENNETWISTER::init_by_array(unsigned long init_key[], int key_length)
{
    int i, j, k;
    init_genrand(19650218UL);
    i=1; j=0;
    k = (g_maxMTStateVector>key_length ? g_maxMTStateVector : key_length);
    for (; k; k--) {
        mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1664525UL))
          + init_key[j] + j; /* non linear */
        mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
        i++; j++;
        if (i>=g_maxMTStateVector) { mt[0] = mt[g_maxMTStateVector-1]; i=1; }
        if (j>=key_length) j=0;
    }
    for (k=g_maxMTStateVector-1; k; k--) {
        mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1566083941UL))
          - i; /* non linear */
        mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
        i++;
        if (i>=g_maxMTStateVector) { mt[0] = mt[g_maxMTStateVector-1]; i=1; }
    }

    mt[0] = 0x80000000UL; /* MSB is 1; assuring non-zero initial array */ 
}

/* generates a random number on [0,0xffffffff]-interval */
unsigned long MARSENNETWISTER::getInt32(void)
{
    unsigned long y;
    static unsigned long mag01[2]={0x0UL, m_MatrixA};
    /* mag01[x] = x * m_MatrixA  for x=0,1 */

    if (mti >= g_maxMTStateVector) { /* generate N words at one time */
        int kk;

        if (mti == g_maxMTStateVector+1)   /* if init_genrand() has not been called, */
            init_genrand(5489UL); /* a default initial seed is used */

        for (kk=0;kk<g_maxMTStateVector-m_iVecOffset;kk++) {
            y = (mt[kk]&m_dwUpperMask)|(mt[kk+1]&m_dwLowerMask);
            mt[kk] = mt[kk+m_iVecOffset] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        for (;kk<g_maxMTStateVector-1;kk++) {
            y = (mt[kk]&m_dwUpperMask)|(mt[kk+1]&m_dwLowerMask);
            mt[kk] = mt[kk+(m_iVecOffset-g_maxMTStateVector)] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        y = (mt[g_maxMTStateVector-1]&m_dwUpperMask)|(mt[0]&m_dwLowerMask);
        mt[g_maxMTStateVector-1] = mt[m_iVecOffset-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

        mti = 0;
    }
  
    y = mt[mti++];

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return y;
}

/* generates a random number on [0,0x7fffffff]-interval */
long MARSENNETWISTER::getInt31(void)
{
    return (long)(getInt32()>>1);
}

/* generates a random number on [0,1]-real-interval */
float MARSENNETWISTER::getReal1(void)
{
    return getInt32()*(1.0/4294967295.0); 
    /* divided by 2^32-1 */ 
}

/* generates a random number on [0,1)-real-interval */
float MARSENNETWISTER::getReal2(void)
{
    return getInt32()*(1.0/4294967296.0); 
    /* divided by 2^32 */
}

/* generates a random number on (0,1)-real-interval */
float MARSENNETWISTER::getReal3(void)
{
    return (((double)getInt32()) + 0.5)*(1.0/4294967296.0); 
    /* divided by 2^32 */
}

/* generates a random number on [0,1) with 53-bit resolution*/
float MARSENNETWISTER::getRes53(void) 
{ 
    unsigned long a=getInt32()>>5, b=getInt32()>>6; 
    return(a*67108864.0+b)*(1.0/9007199254740992.0); 
} 
/* These real versions are due to Isaku Wada, 2002/01/09 added */

/* generates a random number on [0,0x7fffffff]-interval */

bool MARSENNETWISTER::getBool(void)
{
   if ((static_cast <unsigned long> (getInt32()))>>31)
   {
      return true;
   }else{
      return false;
   }
}

