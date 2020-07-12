/*
 *------------------------------------------------------------------------
 *      Object: mapobj.cpp 
 *         $Id: mapobj.cpp,v 1.7 2007/03/15 19:30:59 molle Exp $
 *        Date: 2005                            
 *       Notes: Erzeugung und Behandlung von filemapped Objects
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


// -- Zuständig für die Ermittlung einer freien Speicheradresse, 
//    jedoch keine Pflege der Verkettung
//
void* MAPOBJECT::operator new(size_t l_cntBytes, LIFECYCLE l_lifecycle)
{
   MAPOBJECT* l_pmoNew;

   if (m_pmo_aggrArray[l_lifecycle] == NULL) 
   {
      hr2message(__LINE__,-1, "Aggregator Array for this lifecyclescope nul");
      return NULL;
   }

   // -- beim ersten Mal ist das Ergebnis sofort klar
   //
   if ( m_pmo_aggrArray[l_lifecycle]->IsInUse() )
   {
      int l_iAddress;

      // -- neue Adresse berechnen
      //
      {
         MAPOBJECT* l_mapobjectLast;
         l_mapobjectLast = static_cast <MAPOBJECT*> (m_pmo_aggrArray[l_lifecycle]->getLast());
         l_iAddress = int(l_mapobjectLast) + l_mapobjectLast->m_iSizeOf;
         l_pmoNew = reinterpret_cast <MAPOBJECT*>(l_iAddress);
      }

      if (l_iAddress > int(m_pmo_aggrArray[l_lifecycle]->getFirst())+m_pmo_aggrArray[l_lifecycle]->getMaxMappedSize())
      {
         hr2message(__LINE__,-1, "out of virtual MappedObject-Memoryspace");
         return NULL;
      }
   }else{
      l_pmoNew = static_cast <MAPOBJECT*> (m_pmo_aggrArray[l_lifecycle]->getFirst());

      if (l_pmoNew==NULL)
      {
         hr2message(__LINE__,-1, "Aggregator Array for this lifecyclescope has no first Object set");
         return NULL;
      }
   }

   return l_pmoNew;
   
}

// -- nach new und damit der internen Initialisierung und Anlage von vtable&co: 
//    Pflegen der Next/Previous Verkettungen
//
MAPOBJECT::MAPOBJECT(LIFECYCLE l_lifecycle)
{
   if (m_pmo_aggrArray[l_lifecycle]->IsInUse()) 
   {
      // -- das momentane mapobjectLast als Previous von diesem hier verketten, 
      //
      m_pmapobjectPrevious = m_pmo_aggrArray[l_lifecycle]->getLast();

      // -- beim momentanen mapobjectLast dies hier als Next verketten
      //
      {
         MAPOBJECT* l_mapobjectLast;
         l_mapobjectLast = static_cast <MAPOBJECT*> (m_pmo_aggrArray[l_lifecycle]->getLast());
         l_mapobjectLast->m_pmapobjectNext = this;
      }
   }else{
      m_pmapobjectPrevious = NULL;
      m_pmo_aggrArray[l_lifecycle]->setInUse(true);
   }

   // -- letztendlich ist nun dies das neue mapobjectLast
   //
   m_pmo_aggrArray[l_lifecycle]->setLast(this);

   m_pmapobjectNext = NULL;
   m_iSizeOf = sizeof(this); // deswegen sollte später immer auch der Konstruktor überschrieben werden
}


// -- in Files gemappte Objecte: Notification über Pointeränderung
//
bool MAPOBJECT::PointerChanged(void*, void*)
{
   return true;
}

MAPOBJECT* MAPOBJECT::getNext()
{
   return static_cast <MAPOBJECT*> (m_pmapobjectNext);
}

MAPOBJECT* MAPOBJECT::getPrevious()
{
   return static_cast <MAPOBJECT*> (m_pmapobjectPrevious);
}

/*

unsigned int MAPOBJECT::getTypeID()
{
   return 'OPAM';
}
*/
/*
bool MAPOBJECT::setNext(MAPOBJECT* l_pmapobjectValue)
{
   m_pmapobjectNext = l_pmapobjectValue;
   return true;
}

bool MAPOBJECT::setPrevious(MAPOBJECT* l_pmapobjectValue)
{
   m_pmapobjectPrevious = l_pmapobjectValue;
   return true;
}
*/

// -- c++ style zerstörung eines Map-Objektes
//
void MAPOBJECT::operator delete( void* l_pEnemy, LIFECYCLE l_lifecycle)
{
	MAPOBJECT* l_penemyToDestroy;
   l_penemyToDestroy = reinterpret_cast<MAPOBJECT*>(l_pEnemy);
   l_penemyToDestroy->Destroy(l_lifecycle);
}

// -- object aus der verkettung entfernen
//
bool MAPOBJECT::Destroy(LIFECYCLE l_lifecycle)
{
   // -- ist dies schon das letzte Objekt
   //
   if (m_pmo_aggrArray[l_lifecycle]->getLast() == this)
   {
      // -- wenn vorheriges Objekt vorhanden, dann den dortigen Nextzeiger löschen
      //
      if (m_pmapobjectPrevious)
      {
         m_pmo_aggrArray[l_lifecycle]->setLast(m_pmapobjectPrevious);
         //WriteLogFile("previous nun lastobject\n");
      }

      // -- war dies auch gleichzeitig das erste objekt, dann
      //    ist der gesamte mapobject space als unbenutzt zu kennz.
      //
      if (m_pmo_aggrArray[l_lifecycle]->getFirst() == this)
      {
         m_pmo_aggrArray[l_lifecycle]->setInUse(false);
         //WriteLogFile("alles alle - InUse = false\n");
      }
   }

   // -- dem vorherigen Objekt das eigene Next aufdrücken 
   //
   if (m_pmapobjectPrevious)
   {
      (static_cast <MAPOBJECT*> (m_pmapobjectPrevious))->m_pmapobjectNext = m_pmapobjectNext;
   }

   // -- dem nächsten Objekt das eigene Previous aufdrücken 
   //
   if (m_pmapobjectNext)
   {
      (static_cast <MAPOBJECT*> (m_pmapobjectNext))->m_pmapobjectPrevious = m_pmapobjectPrevious;
   }

   return true;
}

// -- MAPOBJECTAGGREGATE 
//    definieren wie gross die auslagerungsdatei ist 
//    und dann dort die mapobjects installieren
//
MAPOBJECTAGGREGATE::MAPOBJECTAGGREGATE(wchar_t* l_wcFileName, wchar_t* l_wcMappingName) 
{
   wchar_t l_wcTempPath[MAX_PATH];

   m_maxMappedSize = 10 * 1024 * 1024; // GB

   m_wcFullFileName = new wchar_t[MAX_PATH];

   if (GetTempPath(sizeof(l_wcTempPath)/sizeof(wchar_t), l_wcTempPath))
   {
      if (GetTempFileName(l_wcTempPath, l_wcFileName , 0, m_wcFullFileName))
      {
         //char l_cSummary[500];
  
         // -- auslagerungsdatei erzeugen und öffnen
         //
         m_hObjectSpaceFile = CreateFile( m_wcFullFileName, GENERIC_WRITE|GENERIC_READ , 0 ,
                                          NULL , CREATE_ALWAYS , FILE_ATTRIBUTE_NORMAL , NULL );	
         
         if (m_hObjectSpaceFile==NULL) 
         {
            hr2message(__LINE__,GetLastError(),"CreateFile(in MAPOBJECTAGGREGATE::mapObjectSpace)");
            //return false;
         }

/*
         PSXPrintf(__LINE__,l_cSummary, sizeof(l_cSummary), 
                  "opening %s for mapping with lifecycle %i\n",
                  m_cFullFileName, l_cMappingName, m_hObjectSpaceMap);
         WriteLogFile(l_cSummary);
*/

         // -- File mapping erzeugen
         //
         m_hObjectSpaceMap = CreateFileMapping(m_hObjectSpaceFile, 
                                               NULL, PAGE_READWRITE, 0,m_maxMappedSize,l_wcMappingName);
         if (m_hObjectSpaceMap==NULL) 
         {
            hr2message(__LINE__,GetLastError(),"CreateFileMapping(in MAPOBJECTAGGREGATE::mapObjectSpace)");
            //return false;
         }
         
         // -- Zeiger aufs Filemapping holen
         //
         m_pmapobjectFirst = static_cast <MAPOBJECT*> 
                             (MapViewOfFile(m_hObjectSpaceMap, 
                                            FILE_MAP_READ|FILE_MAP_WRITE, 
                                            0, 0, 0));

         if (m_pmapobjectFirst==NULL) 
         {
            hr2message(__LINE__,GetLastError(),"MapViewOfFile(in MAPOBJECTAGGREGATE::mapObjectSpace)");
            //return false;
         }

      }else{
         hr2message(__LINE__,GetLastError(), "GetTempFileName(character.psx)");
         //return false;
      }
   }else{
      hr2message(__LINE__,GetLastError(), "GetTempPath(character.psx)");
      //return false;
   }

   m_pmapobjectLast = m_pmapobjectFirst;
 
   m_bInUse = false;

   //return true;
}

MAPOBJECTAGGREGATE::~MAPOBJECTAGGREGATE()
{
   UnmapViewOfFile(m_pmapobjectFirst);
   CloseHandle(m_hObjectSpaceMap);
   CloseHandle(m_hObjectSpaceFile);
   DeleteFile(m_wcFullFileName);
   delete [] m_wcFullFileName;
   //return true;
}

// -- beispielhafte Erzeugung eines MapObjects, bei Erben von 
//    MapObject jedoch nicht mehr verwenden
//
MAPOBJECTDATACONTAINER* MAPOBJECTAGGREGATE::Insert()
{
   return new(LIFECYCLE_GAME) MAPOBJECT(LIFECYCLE_GAME);
}

bool MAPOBJECTAGGREGATE::setFirst(MAPOBJECTDATACONTAINER* l_mapobjectValue)
{
   m_pmapobjectFirst = l_mapobjectValue;
   return true;
}

bool MAPOBJECTAGGREGATE::setLast(MAPOBJECTDATACONTAINER* l_mapobjectValue)
{
   m_pmapobjectLast = l_mapobjectValue;
   return true;
}

MAPOBJECTDATACONTAINER* MAPOBJECTAGGREGATE::getFirst()
{
   return m_pmapobjectFirst;
}

MAPOBJECTDATACONTAINER* MAPOBJECTAGGREGATE::getLast()
{
   return m_pmapobjectLast;
}

bool MAPOBJECTAGGREGATE::setMaxMappedSize(int l_iValue)
{
   m_maxMappedSize = l_iValue;
   return true;
}

int MAPOBJECTAGGREGATE::getMaxMappedSize()
{
   return m_maxMappedSize;
}

bool MAPOBJECTAGGREGATE::setInUse(bool l_bValue)
{ 
   m_bInUse = l_bValue;
   return true;
}

bool MAPOBJECTAGGREGATE::IsInUse()
{ 
   return m_bInUse;
}

