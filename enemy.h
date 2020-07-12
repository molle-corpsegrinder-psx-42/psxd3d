/*
 *------------------------------------------------------------------------
 *      Object: enemy.h
 *         $Id: enemy.h,v 1.35 2010/02/04 15:39:59 molle Exp $
 *        Date: Mar 2005
 *       Notes: Deklarationen fuer die ENEMY klasse + verbundene Objekte
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

#ifndef ENEMY_H
#define ENEMY_H

typedef enum c_en_type { ETYP_EMPTY       = 0,
                         ETYP_FLYER,
                         ETYP_EXPLOSION,
                         ETYP_IMPACT,
                         ETYP_MINE,
                         ETYP_ASTEROID,
                         ETYP_HANGAR,
                         ETYP_GUN,
                         ETYP_REACTORGUN,
                         ETYP_LEADER,
                         ETYP_DOOR,
                         ETYP_TUNNELFLYER,
                         ETYP_EQUIPMENT,
                         ETYP_STATIC,
                         ETYP_FELDGENERATOR,
                         ETYP_TMPSPEEDUP,
                         ETYP_GHOST,
                         ETYP_CHAR,
                         ETYP_TRADER,
                         ETYP_HUNTER,
                         ETYP_PIRATE,
                         ETYP_P_FLYER,
                         ETYP_LASERHALTER,
                         ETYP_LASERGUN,
                         ETYP_CARGOBOX,
                         ETYP_BIGRESITEM,
                         ETYP_STARBASE,
                         ETYP_KINGSPLANE,
                         ETYP_THARGOID,
                         ETYP_THARGOIDDRONE,
                         ETYP_PLAYER,
                         ETYP_WINGMAN,
                         ETYP_STARBASEHANGAR,
                         ETYP_PLANET,
                         ETYP_DAZ3D_CUBE,
                         ETYP_LASTENUM,
                       } ETYPE;

const int g_maxEnemy   = 10000;

// --- hold decision of the a.i-routines
//
typedef enum _gep { ENACT_DONOTHING = 0,  
                    ENACT_CURVEAROUND,
                    ENACT_CHECKPTALIGNY, 
                    ENACT_BECOMELEADERSPART, 
                    ENACT_ALIGNYSMOOTH, 
                    ENACT_ALIGNX,
                    ENACT_SHOOT, 
                    ENACT_ESCAPE,
                    ENACT_BANKROTATION,
                    ENACT_REFLECTION,
                    ENACT_FOLLOWLEADER,
                    ENACT_COUNTDOWN,
                    ENACT_AVOIDPLAYER,
                    ENACT_AVOIDSTARBASE,
                    ENACT_AVOIDFACEUP,
                    ENACT_AVOIDFACEDOWN,
                    ENACT_AVOIDFACEUPHARD,
                    ENACT_AVOIDFACEDOWNHARD,
                    ENACT_BREAKOUT_UP,
                    ENACT_BREAKOUT_DOWN,
                    ENACT_PULLUP,
                    ENACT_PULLDOWN,
                    ENACT_ACCELERATE,
                    ENACT_LEAVEDOCK,
                    ENACT_CHASEPLAYER,
                    ENACT_KAMIKAZE,
                    ENACT_AVOIDDESTSMOOTH,
                    ENACT_DROPCARGO,
                    ENACT_GOTOSTARBASE,
                    ENACT_DOCKING,
                    ENACT_DETONATE} ENEMYACTION;


// -- Datacontainer, der die Vereinigungsmenge aller Variablen darstellt
//
/*
class ENEMYDATACONTAINER {
  public:
    PSXVECTOR m_vPos;
    int       filler;  
    PSXVECTOR m_vPos2D;
    float     m_fCameraW;
    PSXVECTOR m_vStep;
    float     m_fMinRad2DForVisibility;
    ETYPE     mType;
    int       m_cntGeneric;
    float     m_fViewRotation1;
    float     m_fViewRotation2;
    float     m_fExplScale;

    int          m_iFlags;
    ENEMYACTION  m_Action;
    int          m_iActionTimer;
    PSXVECTOR    dead_element6; //m_vDest;
    float        m_fDestLength;
    int          m_idxEnemy; //m_iFightExperience;

    D3DMATRIX    m_Mat;
    int          dead_element4;  //m_idxLeadhist;
    int          m_idxLeader;
    int          m_cntShieldFlashTimer;
    int          m_cntRanking;
    float        m_fSpeed;
    float        m_fScale;
    float        m_fRad;
    float        m_fRad2D;
    float        m_fCollisionRadPow2;
    float        m_fShootHitRadPow2;
    RESTYPE      m_ResType;
    DISPLAYSTYLE dead_element3;
    ENUM_TXX_ID  m_txxDestFrameInfo;
    float        m_fShieldAmount;
    int          m_idxCharacter;
    int          dead_element;  // ex m_idxFaceInfArray;
    int          m_idxBigResItem;
    SEED         m_seedShipsHold;
    int          dead_element5;  //ex m_pShipsHoldArray;
    dMass        m_dMass;
};
*/


// -- Basisklasse
//
class ENEMY : public MAPOBJECT {
protected:
/*     int   m_idxCharacter;
     CHARACTER* m_pCharacter;*/
	int m_idxEnemy;
	static int m_idxHighest;

	ENEMY* m_penemyNext;
	ENEMY* m_penemyPrevious;
	static ENEMY* m_penemyFirst;
	static ENEMY* m_penemyLast;
	static ENEMY* m_penemyTarget;
	static ENEMY* m_pCurrLeader;

     dBodyID m_dBodyID;
     dGeomID m_dGeomID;

     // -- müssen alle hintereinanderliegen wegen PSXCheckVisibleEnemies-Sichtbarkeitsprüfung
     //

     XMVECTOR m_vPos;
	 /*
     int filler;  // damit bildet der m_vPos-vektor ein SSE-kompatibles DoubleQuadword 
	 
     PSXVECTOR m_vPos2D;
     float m_fCameraW;
	 */
     float m_fRad;   
	 /*
     float m_fRad2D; 

	 */
     XMVECTOR m_vStep;
     float m_fMinRad2DForVisibility;
	 
     XMMATRIX m_Mat;

     RESTYPE m_ResType;

     dMass m_dMass;

public:
/*
     static int m_cntLeadersToDestroy;
     static int m_cntLeadersDestroyed;
     static int m_cntCurrentlyAttackingLeaders;
     static int m_cntCurrentlyAttackingPirates;

     static int m_cntThargoid;
     static int m_cntThargoidDrone;
*/
	ENEMY(LIFECYCLE);
/*	
    ENEMY(LIFECYCLE , CHARACTER* ) ;

*/
    virtual bool Update(void);
	
    bool SetType(ETYPE);
	/*
    void checkVisibility();
    void CalculateCameraSpace();
*/
    bool TreatAsExplosion();
    bool TreatAsFlyer();
    bool TreatAsLeader();
/*
    bool TreatAsChar();
    bool TreatAsHangar();
    bool TreatAsGun();
    bool TreatAsTunnelFlyer();
    bool TreatAsMine();
    bool TreatAsGenerator();
    bool TreatAsStatic();
    bool TreatAsGhost();
    bool TreatAsHunter();
    bool TreatAsTrader();
    bool TreatAsPirate();
    bool TreatAsPirateFlyer();
	*/
    bool TreatAsBigResItem();
	/*
    bool TreatAsStarBase();
*/
    bool TreatAsPlayer();
/*
    bool TreatAsWingman();
    bool TreatAsLaserGun();
    bool TreatAsLaserHalter();
    bool TreatAsKingsplane();
    bool TreatAsThargoid();
    bool TreatAsThargoidDrone();
    bool TreatAsStarBaseHangar();
	 bool TreatAsPlanet();
	 */
    bool handleDefaultSmallShip(float, PSXLASERSPAWN*, int);
    bool handleDefaultBigCargo(float);
    bool handleCurveAroundCircles();
    bool handleGoToStarBase();
    bool handleAvoidStarBase();
    bool handleFollowLeader(int);
	
    bool getFormationPlace(int, XMVECTOR*);
	
    void RotationToMatrix();
	
    bool Shoot();
    bool Shoot(PSXLASERSPAWN*, int);
    bool ShootStraightly(PSXLASERSPAWN*, int);
    bool ShotAgainstShield(int, XMVECTOR*, XMVECTOR* );
    bool DecideForEnemyDrawer(IDXENEMYDRAWER*, IDXENEMYDRAWER*);
	
    bool Collision(bool*, ENEMY*,int);
    bool checkIfHitByShoot(int , float);

    bool HitByShoot(int, XMVECTOR*);
    bool HitByShoot(int, XMVECTOR*, XMVECTOR*);

    void CalculateRad();
    void setRadius(float);
	/*
    bool TickEquipmentAppearance();
    bool CollectEquipment();
    bool CollectTmpUp();
    void WriteDebug(char* );
	*/
    void TransformDest();
	/*
    void TransformStarBase();
	*/
    bool setMatrixAndStepSmallShip();	
    bool setMatrixAndStepBigCargo();
	/*
    bool updateDestFrame();
	*/
    bool initTypeDependend();
    bool setRes(RESTYPE);
	/*
    bool setScale(float);
	*/
    bool detonate();
	/*
    bool loadFromDC(ENEMYDATACONTAINER*);
    bool saveToDC(ENEMYDATACONTAINER*);
	*/
    bool updateAction();
	/*
    bool setCharacterIndex(int);
    int  getCharacterIndex();
    CHARACTER* getCharacter();
    bool setCharacterToPolice();
    
    static int getEstimatedCount() {
       return m_idxHighest+1;
    };
	*/
	bool init();
    bool setStep(XMVECTOR l_vVal);
    bool setStep(float, float, float);
    bool setPos(XMVECTOR l_vVal);
    bool setPos(float, float, float);

    bool setMat(CXMMATRIX l_matVal);
    bool copyMat(ENEMY*);
    bool copyPos(ENEMY*);


    bool setBodyMat();
	XMMATRIX* getMatrixPointer();
/*
    inline bool isAnyCharacter()
    {
       if (m_idxCharacter >= 0)
       {
          return true;
       }else{
          return false;
       }
    }
	*/
    inline void setTriggered(bool l_bVal){
      if (l_bVal) {
         m_iFlags = m_iFlags | 1L;
      }else{
         m_iFlags = m_iFlags & (-1L -1);
      }
    };

    inline bool isTriggered(){
      if (m_iFlags & 1) {
         return true;
      }else{
         return false;
      }
    };
	
    inline void setAffectableByShoot(bool l_bVal){
      if (l_bVal) {
         m_iFlags = m_iFlags | 2L;
      }else{
         m_iFlags = m_iFlags & (-2L - 1);
      }
    };

    inline bool isAffectableByShoot(){
      if (m_iFlags & 2) {
         return true;
      }else{
         return false;
      }
    };
	
    inline void setPartOfLeader(bool l_bVal)
    {
      if (l_bVal) {
         m_iFlags = m_iFlags | 4L;
      }else{
         m_iFlags = m_iFlags & (-4L - 1);
      }
    };
    inline bool isPartOfLeader()
    {
      if (m_iFlags & 4) {
         return true;
      }else{
         return false;
      }
    };


    inline void setBigResRepresented(bool l_bVal)
    {
      if (l_bVal) {
         m_iFlags = m_iFlags | 8L;
      }else{
         m_iFlags = m_iFlags & (-8L -1);
      }
    };
    inline bool isBigResRepresented()
    {
      if (m_iFlags & 8) {
         return true;
      }else{
         return false;
      }
    };

    inline void setVisible(bool l_bVal){
      if (l_bVal) {
         m_iFlags = m_iFlags | 16L;
      }else{
         m_iFlags = m_iFlags & (-16L -1);
      }
    };

    inline bool isVisible(){
      if (m_iFlags & 16) {
         return true;
      }else{
         return false;
      }
    };

    inline void setTargetable(bool l_bVal){
      if (l_bVal) {
         m_iFlags = m_iFlags | 32L;
      }else{
         m_iFlags = m_iFlags & (-32L -1);
      }
    };

    inline bool isTargetable(){
      if (m_iFlags & 32) {
         return true;
      }else{
         return false;
      }
    };

    inline void setCollisionOnMove(bool l_bVal){
      if (l_bVal) {
         m_iFlags = m_iFlags | 64L;
      }else{
         m_iFlags = m_iFlags & (-64L -1);
      }
    };

    inline bool isCollisionOnMove(){
      if (m_iFlags & 64L) {
         return true;
      }else{
         return false;
      }
    };

    inline void setCheckFaceIntersection(bool l_bVal){
      if (l_bVal) {
         m_iFlags = m_iFlags | 128L;
      }else{
         m_iFlags = m_iFlags & (-128L -1);
      }
    };

    inline bool isCheckFaceIntersection(){
      if (m_iFlags & 128L) {
         return true;
      }else{
         return false;
      }
    };

    inline void setShootMagnetical(bool l_bVal){
      if (l_bVal) {
         m_iFlags = m_iFlags | 256L;
      }else{
         m_iFlags = m_iFlags & (-256L -1);
      }
    };

    inline bool isShootMagnetical(){
      if (m_iFlags & 256L) {
         return true;
      }else{
         return false;
      }
    };

    inline void setCargoBoxOnBoard(bool l_bVal){
      if (l_bVal) {
         m_iFlags = m_iFlags | 512L;
      }else{
         m_iFlags = m_iFlags & (-512L -1);
      }
    };

    inline bool isCargoBoxOnBoard(){
      if (m_iFlags & 512L) {
         return true;
      }else{
         return false;
      }
    };

    inline void setRadarable(bool l_bVal){
      if (l_bVal) {
         m_iFlags = m_iFlags | 1024L;
      }else{
         m_iFlags = m_iFlags & (-1024L -1);
      }
    };

    inline bool isRadarable(){
      if (m_iFlags & 1024L) {
         return true;
      }else{
         return false;
      }
    };

    inline void setBreakingOut(bool l_bVal){
      if (l_bVal) {
         m_iFlags = m_iFlags | 2048L;
      }else{
         m_iFlags = m_iFlags & (-2048L -1);
      }
    };

    inline bool isBreakingOut(){
      if (m_iFlags & 2048L) {
         return true;
      }else{
         return false;
      }
    };

    inline void setDestFrame(bool l_bVal){
      if (l_bVal) {
         m_iFlags = m_iFlags | 4096L;
      }else{
         m_iFlags = m_iFlags & (-4096L -1);
      }
    };

    inline bool isDestFrame(){
      if (m_iFlags & 4096L) {
         return true;
      }else{
         return false;
      }
    };

    inline void setShipsLostArray(bool l_bVal){
      if (l_bVal) {
         m_iFlags = m_iFlags | 8192L;
      }else{
         m_iFlags = m_iFlags & (-8192L -1);
      }
    };

    inline bool isShipsLostArray(){
      if (m_iFlags & 8192L) {
         return true;
      }else{
         return false;
      }
    };
	
    inline XMVECTOR getPos() {
      return m_vPos;
    };
	

    inline float getPosX() {
		return XMVectorGetX(m_vPos);      
    };
    inline float getPosY() {
		return XMVectorGetY(m_vPos);
    };
    inline float getPosZ() {
		return XMVectorGetZ(m_vPos);
    };
    inline float getRad() {
      return m_fRad;
    };
	/*

    inline PSXVECTOR getPos2D() {
      return m_vPos2D;
    };
    inline float getPos2DX() {
      return m_vPos2D.x;
    };
    inline float getPos2DY() {
      return m_vPos2D.y;
    };
    inline float getPos2DZ() {
      return m_vPos2D.z;
    };

    */

    inline XMVECTOR getStep() {
      return m_vStep;
    };

    /*
    inline float getStepX() {
      return m_vStep.x;
    };
    inline float getStepY() {
      return m_vStep.y;
    };
    inline float getStepZ() {
      return m_vStep.z;
    };
	*/
    inline XMMATRIX* getRawMatrixPointer() {
      return &m_Mat;
    };
	/*

    inline float getMat_11() {
      return m_Mat._11; 
    };
    inline float getMat_12() {
      return m_Mat._12; 
    };
    inline float getMat_13() {
      return m_Mat._13; 
    };
    inline float getMat_21() {
      return m_Mat._21; 
    };
    inline float getMat_22() {
      return m_Mat._22; 
    };
    inline float getMat_23() {
      return m_Mat._23; 
    };
    inline float getMat_31() {
      return m_Mat._31; 
    };
    inline float getMat_32() {
      return m_Mat._32; 
    };
    inline float getMat_33() {
      return m_Mat._33; 
    };
	*/

    inline RESTYPE getRes() {
      return m_ResType;
    };
	

    inline dBodyID getBodyID() { 
       return m_dBodyID;
    };

    inline dMass* getMassPointer(){
       return &m_dMass;
    };

    inline dGeomID getGeomID() { 
       return m_dGeomID;
    };
	
    ETYPE mType;
    int m_cntGeneric;
    float m_fViewRotation1;
    float m_fViewRotation2;
	
    float m_fExplScale;
	
    int  m_iFlags;

    ENEMYACTION m_Action;
    int m_iActionTimer;
    /* 
	float m_fDestLength; 
	*/

	ENEMY* m_pLeader;
    int m_cntShieldFlashTimer;
    int m_cntRanking;	
    float m_fSpeed;
	/*
    float m_fScale;
    float m_fCollisionRadPow2;
	*/
    float m_fShootHitRadPow2;
    char* m_cDestFrameInfo;	
    float m_fShieldAmount;
	
    BIGRESITEM* m_pBigResItem;
	/*
    SEED        m_seedShipsHold;
*/

    bool setEnemyIndex(int);
    int  getEnemyIndex();

    // -- initialisiert die static-globals 
    //
    static void setFirstAndLastToNull() 
    {
       m_idxHighest = 0;
       m_penemyFirst = NULL;
       m_penemyLast  = NULL;
    }

    // -- löscht alle Enemies und löscht andere globale Pointer , Counter und Flags 
    //
    static void DropAllEnemies(LIFECYCLE l_lifecycle) 
    {
       ENEMY* l_penemyCurr;
 
       l_penemyCurr = getFirstEnemy();
       while (l_penemyCurr)
       {
          l_penemyCurr->Destroy(l_lifecycle);
          l_penemyCurr = l_penemyCurr->getNextEnemy();
       }

       setFirstAndLastToNull();
/* 
       m_cntLeadersDestroyed = 0;
       m_cntLeadersToDestroy=0; 
       m_cntCurrentlyAttackingLeaders = 0;
       m_cntCurrentlyAttackingPirates = 0;
    
       m_cntThargoid = 0;
       m_cntThargoidDrone = 0;
*/
    }

	ENEMY* getNextEnemy();
	ENEMY* getPreviousEnemy();
	static inline ENEMY* getFirstEnemy(){
		return m_penemyFirst;
	};
/*
    static inline bool setEnemyTarget(ENEMY* l_pEnemy) {
       if (l_pEnemy)
       {
          if ((l_pEnemy >= m_penemyFirst) &&
              (l_pEnemy <= m_penemyLast))
          {
             m_penemyTarget = l_pEnemy;
             return true;
          }else{
             hr2message(__LINE__,-1,"ENEMY::setEnemyTarget:Enemypointer out of First/Last Zone");
             return false;
          }
       }else{
          m_penemyTarget = NULL;
          return true;
       }
    }

    static inline bool IsAnyTarget() {
       if (m_penemyTarget)
       {
          return true;
       }else{
          return false;
       }
    }

    static inline ENEMY* getEnemyTarget()
    {
       return m_penemyTarget;
    }
*/

    static inline bool IsAnyEnemy() {
       if (m_penemyFirst)
       {
          return true;
       }else{
          return false;
       }
    }
/*

    static inline ENEMY* getByIndexDebug(int l_idxEnemy)
    {
       ENEMY* l_penemyFound;
       char l_cDebugText[100];
       l_penemyFound = m_penemyFirst;
     
       HRESULT hr = PSXPrintf(__LINE__, 
                              l_cDebugText, sizeof(l_cDebugText),
                              "Searching for %i\n", l_idxEnemy);

       WriteLogFile(l_cDebugText);

       do {
         HRESULT hr = PSXPrintf(__LINE__, 
                                l_cDebugText, sizeof(l_cDebugText),
                                "Meeting %i\n", l_penemyFound->m_idxEnemy);

         WriteLogFile(l_cDebugText);

         if (l_penemyFound->m_idxEnemy == l_idxEnemy) 
         {
            return l_penemyFound;
         }
         l_penemyFound = l_penemyFound->m_penemyNext;
       } while (l_penemyFound);
       return NULL;
    };

    static inline ENEMY* getByIndex(int l_idxEnemy)
    {
       ENEMY* l_penemyFound;
       l_penemyFound = m_penemyFirst;
     
       do {
         if (l_penemyFound->m_idxEnemy == l_idxEnemy) 
         {
            return l_penemyFound;
         }
         l_penemyFound = l_penemyFound->m_penemyNext;
       } while (l_penemyFound);
       return NULL;
    };
*/
	bool Destroy(LIFECYCLE l_lifecycle);

   static void dNearCallback(void *data, dGeomID o1, dGeomID o2);
   
   bool setTorque(dReal*);
   /*
   unsigned int getTypeID();
   */
   bool createBody();
   bool releaseBody();
   /*
   bool becomeIndependent();
   bool getBackIntoLeadersBody();
   bool checkAttachedEnemies();
   */
   bool createCollisionGeoms(RESTYPE);
   /*
   bool armWithLaserGuns();
   bool setOffset(float , float , float );
   bool setOffsetMat(D3DMATRIX* );
   */
   bool controlledByUser();
   
   bool isPlayer();

};

// -- besondere Enemytypes
//
class EXPLOSION : public ENEMY {
   public:
      virtual bool Update();
      EXPLOSION(LIFECYCLE);
      //EXPLOSION(LIFECYCLE , CHARACTER* );
      bool setPos(float , float , float );
      bool setPos(XMVECTOR );

};

// -- Singleton Objekt dessen Werte Gültigkeit für einen Update durchlauf eines Enemy haben
//    quasi Zwischenspeicher, um zwischen den Methoden zu kommunizieren, der darüber hinaus 
//    keinen Bestand haben soll
//
class ENEMYHANDLINGBUFFER {
   friend ENEMY;
 
   protected:
   bool m_bSingleMovementInPipe;
   XMFLOAT3 m_vDest, m_vStarBase;   
   float m_fStarBaseLength;
   float m_fDestLength;
};

#endif
