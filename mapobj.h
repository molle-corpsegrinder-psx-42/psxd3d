/*
 *------------------------------------------------------------------------
 *      Object: mapobj.h
 *         $Id: mapobj.h,v 1.4 2007/03/15 19:30:59 molle Exp $
 *        Date: Okt 2005
 *       Notes: Deklaration der dynamischen Map-Objekte, die auf Platte liegen
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

#ifndef MAPOBJ_H
#define MAPOBJ_H

typedef enum _enlifecycle { LIFECYCLE_GAME, LIFECYCLE_LEVEL , LIFECYCLE_LASTENUM} LIFECYCLE;

// -- dynamischer, auf Festplatte gemappter Objectpool
//
class MAPOBJECTDATACONTAINER 
{
   protected:
      MAPOBJECTDATACONTAINER* m_pmapobjectNext;
      MAPOBJECTDATACONTAINER* m_pmapobjectPrevious;
      int m_iSizeOf;

/*
   public:
      virtual bool PointerChanged(void*, void*) = 0;
      virtual MAPOBJECTDATACONTAINER* getNext() = 0;
      virtual MAPOBJECTDATACONTAINER* getPrevious() = 0;
*/
};

// -- erzeugt und instanziert Objekte
//
class MAPOBJECTAGGREGATE {
   protected:
      wchar_t* m_wcFullFileName;
      MAPOBJECTDATACONTAINER* m_pmapobjectFirst;
      MAPOBJECTDATACONTAINER* m_pmapobjectLast;
 
      HANDLE m_hObjectSpaceFile;
      HANDLE m_hObjectSpaceMap;
      int m_maxMappedSize;
 
      bool m_bInUse;

   public:
      MAPOBJECTAGGREGATE(wchar_t*, wchar_t*);
      ~MAPOBJECTAGGREGATE();

      MAPOBJECTDATACONTAINER* Insert();
      bool unmapObjectSpace();

      bool setFirst(MAPOBJECTDATACONTAINER*);
      bool setLast(MAPOBJECTDATACONTAINER*);
     
      MAPOBJECTDATACONTAINER* getFirst();
      MAPOBJECTDATACONTAINER* getLast();
      
      bool setMaxMappedSize(int);
      int getMaxMappedSize();

      bool setInUse(bool);
      bool IsInUse();
};

// -- dynamischer, auf Festplatte gemappter Objectpool
//
class MAPOBJECT: public MAPOBJECTDATACONTAINER
{
   protected:
      static MAPOBJECTAGGREGATE* m_pmo_aggrArray[LIFECYCLE_LASTENUM];
      int m_iTypeID;


   public:

      static MAPOBJECTAGGREGATE* getAggregate(LIFECYCLE l_lifecycle)
      {
         return m_pmo_aggrArray[l_lifecycle];
      };

      static bool setAggregate(LIFECYCLE l_lifecycle, 
                               MAPOBJECTAGGREGATE* l_pmo_aggrValue)
      {
         if (l_pmo_aggrValue == NULL) return false;
         m_pmo_aggrArray[l_lifecycle] = l_pmo_aggrValue;
         return true;
      };

      static bool flushAll(LIFECYCLE l_lifecycle)
      { 
         while (m_pmo_aggrArray[l_lifecycle]->IsInUse())
         {
            (static_cast <MAPOBJECT*> (m_pmo_aggrArray[l_lifecycle]->getLast()))->Destroy(l_lifecycle);
         }
         return true;
      };

      MAPOBJECT(LIFECYCLE);
      void* operator new( size_t, LIFECYCLE);
      void operator delete( void*, LIFECYCLE);

      bool PointerChanged(void*, void*);
      MAPOBJECT* getNext();
      MAPOBJECT* getPrevious();
/*
      bool setNext(MAPOBJECT*);
      bool setPrevious(MAPOBJECT*);
*/
      virtual bool Destroy(LIFECYCLE);
	  /*
      virtual unsigned int getTypeID();      
	  */

};

#endif
