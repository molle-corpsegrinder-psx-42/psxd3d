/*
 *------------------------------------------------------------------------
 *      Object: psxmain.cpp 
 *        Date: Mar 2014
 *       Notes: Definiert den Einstiegspunkt für die Anwendung.
 *------------------------------------------------------------------------
 * 
 * Copyright (c) 1997-2014 Michael Molkenthin. All Rights Reserved.
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

// -- covering whole architecture of a win32-application-architecture here
//

#include "stdafx.h"
#include "resource.h"

#include "psxd3d.h"
#include "psxglob.h"  
// Vorwärtsdeklarationen der in diesem Codemodul enthaltenen Funktionen:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

HRESULT CompileShaderFromFile( WCHAR* , LPCSTR , LPCSTR , ID3DBlob** );

DWORD WINAPI UpdaterThreadFunction(void *pParam);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    GAME l_gameInstance;
	memcpy(&g_game, &l_gameInstance,   sizeof(g_game));

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Hier Code einfügen.
	MSG msg  = {0};
	HACCEL hAccelTable;

	HANDLE l_hDefaultHeap = GetProcessHeap();

	if (l_hDefaultHeap == NULL)
	{
		WriteLogFile("GetProcessHeap() failed\n");
		return false;
	}

	debugausgabe = (char*)HeapAlloc(l_hDefaultHeap,
		HEAP_ZERO_MEMORY,
		g_maxDebugAusgabe * sizeof(debugausgabe[0]));


	// Globale Zeichenfolgen initialisieren
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_D11INIT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Anwendungsinitialisierung ausführen:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	if (FAILED(PSXInitDirect3D()) )
	{
		CleanupDevice();
		return false;
	}
	
	if (g_game.initGame() == false) return false;
	//return (int)msg.wParam; //scaler

	if (g_game.resetGame() == false) return false;

	g_cntFrame = 0;
	g_dwLastTickCount = GetTickCount();

	g_bKeepUpdaterThread = true;
	
	g_hSemaphore = CreateSemaphore(NULL, 1, 1, NULL);
	if (g_hSemaphore == NULL)
	{
		hr2message(__LINE__, -1,  "CreateSemaphore()");
		return false;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_D11INIT));
	if (PSXUpdateGameManagement() == false) return false;


	DWORD uiThreadId;
	HANDLE l_hThread = CreateThread(0, 0, UpdaterThreadFunction, NULL, 0, &uiThreadId);

	// Main message loop
	while( WM_QUIT != msg.message )
    {
        if( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
		}
		else
		{			
			if (PSXRunFullScreen() == false)
			{
				//SendMessage(g_hStatusWindow, SB_SETTEXT,
				//	0, (LPARAM) g_cLang[txxErrorOccurredPleaseSave]);

			}
        }
    }		
	g_bKeepUpdaterThread = false;

	WaitForSingleObjectEx(l_hThread, INFINITE, TRUE);
	CloseHandle(l_hThread);
	CloseHandle(g_hSemaphore);

	CleanupDevice();

	if (g_game.destroyGame()==false) return 0;

	HeapFree(l_hDefaultHeap, 0, debugausgabe);

	return (int) msg.wParam;
}

DWORD WINAPI UpdaterThreadFunction(void *pParam)
{
	DWORD dwWaitResult;

	while (g_bKeepUpdaterThread)
	{
		dwWaitResult = WaitForSingleObject(
			g_hSemaphore,   // handle to semaphore
			0L);           // zero-second time-out interval

		if (PSXUpdateGameManagement() == false) return false;

		/*if (g_Player.getGS()->ShallUpdateEnemies())*/
			
		SleepEx(10, TRUE);
	}
	return 0;
}



char* GAME::getWindowTitle()
{
   return "Elite Starfighter - Cobra MK4";
}
   
   
// --- MAIN
// 
bool GAME::initGame()
{
   
   // --- Debug logfile
   //
   g_hLogFile = CreateFile( L"psxd3d.log", 
                          GENERIC_WRITE,
                          FILE_SHARE_WRITE|FILE_SHARE_READ,
                          NULL,
                          CREATE_ALWAYS,
                          FILE_ATTRIBUTE_NORMAL,	
                          NULL 	
                        );
   {
      char BuildText[100];
      PSXPrintf(__LINE__,BuildText,sizeof(BuildText),
              "hello, this is psxd3d.exe $Revision: 1.16 $ speaking. I was built on %s, %s !\n",
              __DATE__, __TIME__);
      WriteLogFile(BuildText);
   }
   
   /*
   memset(g_cSaveGameFileName, 0, sizeof(g_cSaveGameFileName));
   */
   if (associateTextureRsc() == false) return false;

   //if (PSXInitDirect3D() == false) return false;
   
      
   dSetErrorHandler(PSXErrorHandlerForODE);
   dSetMessageHandler(PSXMessageHandlerForODE);
   dSetDebugHandler(PSXDebugHandlerForODE);

   // --- open the material file
   //
   if (g_meshresManager.init() == false) return false;
   //if (g_meshresManager.scaler() == false) return false;
   //return true;
   // -- initialize texture management array
   //
   if (PSXInitTextureArray()==false) {
	   return false;
   }


   // -- GAME MANAGEMENT INITIALIZATION
   //       
   if (allocate() == false) return false;
   
   
   
   // -- zu allen texturierenden Enemydrawers die Ids zuordnen
   //
   WriteLogFile("initializing Bump and Enemydrawer, assigning Texture-ids\n");
   if (PSXInitDrawers() == false) return false;

   // -- mapped Objects 
   //
   WriteLogFile("creating MAPOBJECTAGGREGATE  opening and mapping\n");
   {
      MAPOBJECTAGGREGATE* l_pmo_aggrNew;
      l_pmo_aggrNew = new MAPOBJECTAGGREGATE(L"objects", L"mapobj_psx");
      if (l_pmo_aggrNew == NULL)
      {
         hr2message(__LINE__,GetLastError(), "cant instanciate MAPOBJECTAGGREGATE");
         return false;
      }
   
      if (MAPOBJECT::setAggregate(LIFECYCLE_GAME, l_pmo_aggrNew) == false) return false;
   
      l_pmo_aggrNew = new MAPOBJECTAGGREGATE(L"levelobj", L"levobj_psx");
      if (l_pmo_aggrNew == NULL)
      {
         hr2message(__LINE__,GetLastError(), "cant instanciate MAPOBJECTAGGREGATE 2");
         return false;
      }
   
      if (MAPOBJECT::setAggregate(LIFECYCLE_LEVEL, l_pmo_aggrNew) == false) return false;
   }
   
   // -- dauernd offene Hilfsdatei
   //
   /*
   WriteLogFile("open permanent characterbufferfile\n");
   
   {
      char l_cTempPath[MAX_PATH];
   
      if (GetTempPath(sizeof(l_cTempPath), l_cTempPath))
      {
         if (GetTempFileName(l_cTempPath, "character", 0, CHARACTER::m_cCharacterFileName))
         {
            CHARACTER::m_hCharacterFile =  CreateFile(
               CHARACTER::m_cCharacterFileName,    // pointer to name of the file 
               GENERIC_WRITE|GENERIC_READ ,   // access (read-write) mode 
               0 ,	                     // share mode 
               NULL ,	             // pointer to security attributes 
               CREATE_ALWAYS ,	     // how to create 
               FILE_ATTRIBUTE_NORMAL ,  // file attributes 
               NULL );	
            
            if (CHARACTER::m_hCharacterFile==INVALID_HANDLE_VALUE) 
            {
               hr2message(__LINE__,GetLastError(), "CreateFile(WriteCharacterArray())");
               return false;
            }
         }else{
            hr2message(__LINE__,GetLastError(), "GetTempFileName(character.psx)");
            return false;
         }
      }else{
         hr2message(__LINE__,GetLastError(), "GetTempPath(character.psx)");
         return false;
      }
   }
*/
   
   
   // -- Sprachlokalisierung
   //
   WriteLogFile("user language and localisation: ");
   
   WriteLogFile("setting to german\n");
   g_cLang       = g_cLangGerman;

/*
   g_langCurrent = LANGGERMAN;
   
   for(int i=0;i<g_maxPlayerName;i++)
   {
      g_cPlayerName[i]=0;
   }
   PSXStrCpy(__LINE__, g_cPlayerName, g_maxPlayerName, "Jameson");
   PSXStrCpy(__LINE__, g_cSaveGameFileName,256, "jameson.es1");
   

   //-- Optionen initialisieren
   //
   g_options.init();
   
   g_idxLoadMp3=MP3RES_ANFLUGPHASE;
   
   // -- Optionen aus Registry lesen
   //
   if (g_options.UpdateRegistry(false)== false ) return false;
   
   if(resetGame()==false)
   {
      return false;
   }
   
   
   if(PSXInitSound()==false)
   {
      WriteLogFile("PSXInitSound() failed!\n");
      return (FALSE);
   }
   
   // -- Mp3 Thread starten
   //
   if (g_options.IsEnableMusic())
   {
      if (PSXStartMusic() == false) return false;
   }
   */
	   return true;
}

bool GAME::destroyGame()
{
   WriteLogFile("behind Messageloop\n");
/*   
   if (g_options.UpdateRegistry(true)== false ) return false;
   
   // -- Freds Selbstbeendung signalisieren und dann warten
   //    killen wenn er dann noch rumdoedelt
   //
   if (PSXStopMusic() == false) return false;
   
   WriteLogFile("close Character File\n");
   CloseHandle(CHARACTER::m_hCharacterFile);
   
   WriteLogFile("deleting CharacterFile ");
   WriteLogFile(CHARACTER::m_cCharacterFileName);
   WriteLogFile("\n ");
   DeleteFile(CHARACTER::m_cCharacterFileName);
   
   
   WriteLogFile("Unallocate AviStream(Screenshot)\n");
   if (g_strmPlain) AVIStreamClose(g_strmPlain);
   WriteLogFile("Release Avi-file\n");
   if (g_avfileCurrent) AVIFileRelease(g_avfileCurrent);
   WriteLogFile("Release Avi-library\n");
   if (g_bCaptureFlag) AVIFileExit();
      
   WriteLogFile("Release Direct3D\n");
   if (lpDirect3D)          lpDirect3D->Release();
   
   WriteLogFile("Release DirectSound\n");
   if (lpDirectSound) lpDirectSound->Release();
   
   UnregisterClass("PSXWNDCLASS",g_hInstance);
   
   WriteLogFile("Unallocate g_pvFrameWriteBuffer(Screenshot)\n");
   if (g_pvFrameWriteBuffer) VirtualFree(g_pvFrameWriteBuffer,0, MEM_RELEASE);
   */
   
   g_meshresManager.release();

   WriteLogFile("Unallocate GameManagement\n");
   if (g_arr_EnemyDrawer) VirtualFree(g_arr_EnemyDrawer,0, MEM_RELEASE);
   
   WriteLogFile("MAPOBJECTAGGREGATE: do closing\n");
   delete (MAPOBJECT::getAggregate(LIFECYCLE_LEVEL));
   delete (MAPOBJECT::getAggregate(LIFECYCLE_GAME));
      
   WriteLogFile("Close Logfile \n");
   if (g_hLogFile) CloseHandle(g_hLogFile);
   
   /*
   if (g_hbmpSponsor) DeleteObject(g_hbmpSponsor);
   
   dCloseODE();
   */
   
   return true;
   
}

   
   

//
//  FUNKTION: MyRegisterClass()
//
//  ZWECK: Registriert die Fensterklasse.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_D11INIT));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_D11INIT);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNKTION: InitInstance(HINSTANCE, int)
//
//   ZWECK: Speichert das Instanzenhandle und erstellt das Hauptfenster.
//
//   KOMMENTARE:
//
//        In dieser Funktion wird das Instanzenhandle in einer globalen Variablen gespeichert, und das
//        Hauptprogrammfenster wird erstellt und angezeigt.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   g_hInst = hInstance; // Instanzenhandle in der globalen Variablen speichern
   g_hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!g_hWnd)
   {
      return FALSE;
   }

   ShowWindow(g_hWnd, nCmdShow);
   UpdateWindow(g_hWnd);

   return TRUE;
}

//
//  FUNKTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ZWECK:  Verarbeitet Meldungen vom Hauptfenster.
//
//  WM_COMMAND	- Verarbeiten des Anwendungsmenüs
//  WM_PAINT	- Zeichnen des Hauptfensters
//  WM_DESTROY	- Beenden-Meldung anzeigen und zurückgeben
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Menüauswahl bearbeiten:
		switch (wmId)
		{
		case IDM_INITD3D:
			
			break;
		case IDM_ABOUT:
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Hier den Zeichnungscode hinzufügen.
		EndPaint(hWnd, &ps);
		break;
	//case WM_KEYDOWN:
//		g_game.handleKey(wParam);
	//	break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Meldungshandler für Infofeld.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

