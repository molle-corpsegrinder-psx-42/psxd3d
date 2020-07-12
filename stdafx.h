// stdafx.h : Includedatei f�r Standardsystem-Includedateien
// oder h�ufig verwendete projektspezifische Includedateien,
// die nur in unregelm��igen Abst�nden ge�ndert werden.
//

#pragma once

//define _WIN32_WINNT _WIN32_WINNT_WIN7
//include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Selten verwendete Teile der Windows-Header nicht einbinden.
// Windows-Headerdateien:
#include <windows.h>

// C RunTime-Headerdateien
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: Hier auf zus�tzliche Header, die das Programm erfordert, verweisen.
//
#include <dxgiformat.h>
#include <assert.h>
#include <algorithm>
#include <memory>

#include <d3d11_1.h>
#include <directxcolors.h>
#include "DDSTextureLoader.h"

#include <dinput.h>
#include <d2d1_1.h>
#include <dwrite.h>
