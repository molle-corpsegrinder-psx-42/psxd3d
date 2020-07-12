/*
 *------------------------------------------------------------------------
 *      Object: GAME
 *         $Id: psxgame.h,v 1.11 2006/06/10 12:57:05 molle Exp $
 *        Date: 2014
 *       Notes: Hier werden die versch. Games äusserlich definiert
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

// -- Marsenne-Twister: hoffentlich besserer Zufallsgenerator als Fibonacci
//
const int g_maxMTStateVector = 624;

class MARSENNETWISTER {
   int m_iVecOffset;
   unsigned long m_MatrixA; /* constant vector a */
   unsigned long m_dwUpperMask; /* most significant w-r bits */
   unsigned long m_dwLowerMask; /* least significant r bits */

   unsigned long mt[g_maxMTStateVector]; /* the array for the state vector  */
   int mti;

   public:
      MARSENNETWISTER();
      void init_genrand(unsigned long);
      void init_by_array(unsigned long init_key[], int);

      unsigned long getInt32(void);
      long getInt31(void);
      bool getBool(void);
      float getReal1(void);
      float getReal2(void);
      float getReal3(void);
      float getRes53(void);
};

class GAME {
   protected:
      int m_cntTextureRsc;
	  
      //MAPOBJECTAGGREGATE m_mo_aggrGame;

      //LEVEL** m_plevelArray;

      virtual char* getWindowTitle();
      //virtual bool createSplashImageFileHandle(HANDLE*);
      virtual bool allocate();
      virtual int  distributeMem(int);
      virtual bool generateAllLevels();
      virtual bool initSeeds();
      //virtual bool populateGalaxy();
      virtual bool initEquipment();
      
   public:
	   float m_fVelocity;
      //virtual MAPOBJECTAGGREGATE* getMapObjectAggregateGame();
/*
      virtual bool   setLevelCatalog(int, LEVEL*);
      virtual LEVEL* getLevelCatalog(int);
*/
	   
      virtual bool initGame();
	  virtual bool destroyGame();
      virtual bool resetGame();
      //virtual bool jumpDestinationLevel();
      //virtual bool isPlanetMapPossible();
      //virtual int  getLevelCount();

      virtual bool associateTextureRsc();
      virtual int  getTextureRscCount();
      //virtual bool updateAllCharacters();
	  //later, when psxres arrives...
      //virtual bool updateCharactersOfGroup(int*, CHARUPDPASS , bool, CGROUP);
      TEXTURERSC* m_txtrscArray; 

      MARSENNETWISTER g_mtwistStarfield;
      MARSENNETWISTER g_mtwistPlanetOffsetSeed;
	  bool handleKey(WORD);

};

bool PSXUpdateGameManagement();

bool PSXMatrixFromVector( );
//later, when psxgame arrives fully...
//bool PSXCalibrateEnemyArray(ELOC);




