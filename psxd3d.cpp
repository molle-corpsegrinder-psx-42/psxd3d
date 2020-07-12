/*
 *------------------------------------------------------------------------
 *      Object: psxd3d.cpp 
 *   $Revision: 1.450 $
 *        Date: 2014                            
 *       Notes: LowLevel (DirectX und Resourcen) Part of Engine
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

#include "stdafx.h"
#include "psxd3d.h"
#include "psxglob.h"

//psxinitdirect3d
HRESULT PSXInitDirect3D()
{
    HRESULT hr = S_OK;

    RECT rc;
    GetClientRect( g_hWnd, &rc );
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG ;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = ARRAYSIZE( driverTypes );

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
    UINT numFeatureLevels = ARRAYSIZE( featureLevels );

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd, sizeof( sd ) );
    sd.BufferCount = 1;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    //sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = g_hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
    {
        g_driverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDeviceAndSwapChain( nullptr, g_driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
                                            D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext );

        /*if ( hr == E_INVALIDARG )
        {
            // DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
            hr = D3D11CreateDeviceAndSwapChain( nullptr, g_driverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
                                                D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext );
        }*/

        if( SUCCEEDED( hr ) )
            break;
    }
    if( FAILED( hr ) )
    {
        hr2message(__LINE__,hr, "D3D11CreateDeviceAndSwapChain()");
        return hr;
    }

        // Obtain the Direct3D 11.1 versions if available
    hr = g_pd3dDevice->QueryInterface( __uuidof( ID3D11Device1 ), reinterpret_cast<void**>( &g_pd3dDevice1 ) );
    if ( SUCCEEDED(hr) )
    {
        hr = g_pImmediateContext->QueryInterface( __uuidof( ID3D11DeviceContext1 ), reinterpret_cast<void**>( &g_pImmediateContext1 ) );
    }

    if( FAILED( hr ) )
    {
        hr2message(__LINE__,hr, "g_pd3dDevice->QueryInterface(()");
        return hr;
    }

    // Create a render target view
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = g_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast<void**>( &pBackBuffer ) );
    if( FAILED( hr ) )
    {
        hr2message(__LINE__,hr, "g_pSwapChain->GetBuffer()");
        return hr;
    }

    hr = g_pd3dDevice->CreateRenderTargetView( pBackBuffer, nullptr, &g_pRenderTargetView );
    pBackBuffer->Release();
    if( FAILED( hr ) )
    {
        hr2message(__LINE__,hr, "g_pd3dDevice->CreateRenderTargetView()");
        return hr;
    }

    // Create depth stencil texture
    D3D11_TEXTURE2D_DESC descDepth;
    ZeroMemory( &descDepth, sizeof(descDepth) );
    descDepth.Width = width;
    descDepth.Height = height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    hr = g_pd3dDevice->CreateTexture2D( &descDepth, nullptr, &g_pDepthStencil );
    if( FAILED( hr ) )
    {
        hr2message(__LINE__,hr, "g_pd3dDevice->CreateTexture2D()");
        return hr;
    }

    // Create the depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
    ZeroMemory( &descDSV, sizeof(descDSV) );
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
    hr = g_pd3dDevice->CreateDepthStencilView( g_pDepthStencil, &descDSV, &g_pDepthStencilView );
    if( FAILED( hr ) )
    {
        hr2message(__LINE__,hr, "g_pd3dDevice->CreateDepthStencilView()");
        return hr;
    }

    g_pImmediateContext->OMSetRenderTargets( 1, &g_pRenderTargetView, g_pDepthStencilView );
    if( FAILED( hr ) )
    {
        hr2message(__LINE__,hr, "g_pImmediateContext->OMSetRenderTargets()");
        return hr;
    }
    

    // Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)width;
    vp.Height = (FLOAT)height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    g_pImmediateContext->RSSetViewports( 1, &vp );

	D3D11_RASTERIZER_DESC rastdesc;
	ZeroMemory(&rastdesc, sizeof(rastdesc));
	rastdesc.FillMode = D3D11_FILL_SOLID;


	rastdesc.CullMode = D3D11_CULL_NONE;

	hr = g_pd3dDevice->CreateRasterizerState(&rastdesc, &g_pD3DRasterizerState);
	if( FAILED( hr ) )
    {
       hr2message(__LINE__,hr, "g_pd3dDevice->CreateRasterizerState()");
       return hr;
    }
	g_pImmediateContext->RSSetState(g_pD3DRasterizerState);

    //TCHAR NPath[MAX_PATH];
    //GetCurrentDirectory(MAX_PATH, NPath);
    //MessageBox(g_hWnd, NPath, L"Current Path", MB_OK);

	// -- Additive Blending for transparency
	//
	{
		D3D11_BLEND_DESC l_d3dBlendDesc;
		ZeroMemory(&l_d3dBlendDesc, sizeof(D3D11_BLEND_DESC));
		
		l_d3dBlendDesc.AlphaToCoverageEnable = FALSE;
		l_d3dBlendDesc.IndependentBlendEnable = FALSE;

		l_d3dBlendDesc.RenderTarget[0].BlendEnable = TRUE;
		l_d3dBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		l_d3dBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		l_d3dBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		l_d3dBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		l_d3dBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		l_d3dBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		l_d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = g_pd3dDevice->CreateBlendState((const D3D11_BLEND_DESC*) &l_d3dBlendDesc, &g_pd3dblendstateTransparent);
		if (FAILED(hr))
		{
			hr2message(__LINE__, hr, "g_pd3dDevice->CreateBlendState()");
			return hr;
		}
	}

	// -- depth-test but not depth writing for transparency
	//
	{
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
				
		depthStencilDesc.DepthEnable = TRUE;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthStencilDesc.StencilEnable = FALSE;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;

		// Stencil operations if pixel is front-facing
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Stencil operations if pixel is back-facing
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		hr = g_pd3dDevice->CreateDepthStencilState(&depthStencilDesc, &g_pd3ddepthstateTransparent);
		if (FAILED(hr))
		{
			hr2message(__LINE__, hr, "g_pd3dDevice->CreateDepthStencilState()");
			return hr;
		}

	}
    // -- Vertexshader
    //
	/*{
		bool l_bHasDeclaredVertexSignature = false;
		WIN32_FILE_ATTRIBUTE_DATA l_FileAttrData = {0};

		struct {
			wchar_t* _strShaderFileName;
			ID3D11VertexShader** l_ppVertexShader;
		} l_arr_InitVertShader[] = {
			{ L"vertexshader.cso", &g_pVertexShaderEnemyDrawer},
			{ L"simple_vs.cso", &g_pVertexShaderSimpleDrawer },
			{ L"bump_vs.cso", &g_pVertexShaderBumpDrawer},
			{ L"sprite_vs.cso", &g_pVertexShaderSpriteDrawer},
			{ L"shoot_vs.cso", &g_pVertexShaderShootDrawer},
			{ L"explosion_vs.cso", &g_pVertexShaderExplosion},
		};
		for (auto l_currInitVertShader : l_arr_InitVertShader) {
			
			memset(&l_FileAttrData, 0, sizeof(l_FileAttrData));
			if (GetFileAttributesExW(l_currInitVertShader._strShaderFileName, GetFileExInfoStandard, &l_FileAttrData) == false)
			{
				MessageBox( nullptr,
					L"The Vertexshader file cannot be retrieved", L"Error", MB_OK );
				return ERROR;
			}
			DWORD l_dwVertexShaderSize = l_FileAttrData.nFileSizeLow;
			LPBYTE l_byteVertexShader = new BYTE[l_dwVertexShaderSize];

		
			HANDLE l_hVertexShaderFile = CreateFile(l_currInitVertShader._strShaderFileName, GENERIC_READ,FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL);
			if (l_hVertexShaderFile == INVALID_HANDLE_VALUE)
			{
				MessageBox( nullptr,
					L"The Vertexshader file cannot be opened", L"Error", MB_OK );
				CloseHandle(l_hVertexShaderFile);
				return ERROR;
			}
			DWORD l_dwBytesRead = 0;
			hr = ReadFile(l_hVertexShaderFile, l_byteVertexShader, l_dwVertexShaderSize, &l_dwBytesRead, NULL);
			if FAILED(hr) {
				MessageBox( nullptr,
					L"The Vertexshader file cannot be loaded", L"Error", MB_OK );
				CloseHandle(l_hVertexShaderFile);
				return ERROR;
			}

			// Create the vertex shader
			hr = g_pd3dDevice->CreateVertexShader( l_byteVertexShader, l_dwBytesRead, nullptr, l_currInitVertShader.l_ppVertexShader);
			if( FAILED( hr ) )
			{
				hr2message(__LINE__,hr, "g_pd3dDevice->CreateVertexShader()");
				return hr;
			}

			// Create the input layout
			// Once an input-layout object is created from a shader signature, the input-layout object can be reused 
			// with any other shader that has an identical input signature (semantics included). 
			// This can simplify the creation of input-layout objects when you are working with many shaders with identical inputs.
			if (l_bHasDeclaredVertexSignature == false)
			{
				// Define the input layout
				D3D11_INPUT_ELEMENT_DESC layout[] =
				{
					{ "SV_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },

					//{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },

				};
				UINT numElements = ARRAYSIZE( layout );
				hr = g_pd3dDevice->CreateInputLayout( layout, numElements, l_byteVertexShader,
					l_dwBytesRead, &g_pVertexLayout );
				if( FAILED( hr ) )
				{
					hr2message(__LINE__,hr, "g_pd3dDevice->CreateInputLayout()");
					return hr;
				}
				// Set the input layout
				g_pImmediateContext->IASetInputLayout( g_pVertexLayout );

				l_bHasDeclaredVertexSignature = true;
			}
			delete [] l_byteVertexShader;
			CloseHandle(l_hVertexShaderFile);
		}
	}*/

    
    // -- Pixelshader
    //
	{
		WIN32_FILE_ATTRIBUTE_DATA l_FileAttrData = {0};
		DWORD l_dwBytesRead = 0;

		struct {
			wchar_t* _strShaderFileName;
			ID3D11PixelShader** l_ppPixelShader;
		} l_arr_InitPixelShader[] = {
			{ L"pixelshader.cso", &g_pPixelShaderEnemyDrawer},
			{ L"solid.cso", &g_pPixelShaderSolid},
			{ L"simple_ps.cso", &g_pPixelShaderSimpleDrawer},
			{ L"bump_ps.cso", &g_pPixelShaderBumpDrawer},
			{ L"sprite_ps.cso", &g_pPixelShaderSpriteDrawer},
			{ L"shoot_ps.cso", &g_pPixelShaderShootDrawer},
			{ L"explosion_ps.cso", &g_pPixelShaderExplosion},
		};
		for (auto l_currInitPixelShader : l_arr_InitPixelShader) {
			memset(&l_FileAttrData, 0, sizeof(l_FileAttrData));
	
			if (GetFileAttributesExW(l_currInitPixelShader._strShaderFileName, GetFileExInfoStandard, &l_FileAttrData) == false)
			{
				MessageBox( nullptr,
					L"The Pixelshader file cannot be retrieved", L"Error", MB_OK );
				return ERROR;
			}
			DWORD l_dwShaderSize = l_FileAttrData.nFileSizeLow;    
			LPBYTE l_byteShader = new BYTE[l_dwShaderSize];


			HANDLE l_hShaderFile = CreateFile(l_currInitPixelShader._strShaderFileName, GENERIC_READ,FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL);
			if (l_hShaderFile == INVALID_HANDLE_VALUE) 
			{
				MessageBox( nullptr,
					L"The Pixelshader file cannot be opened", L"Error", MB_OK );
				CloseHandle(l_hShaderFile);
				return ERROR;
			}
			l_dwBytesRead = 0;
			hr = ReadFile(l_hShaderFile, l_byteShader, l_dwShaderSize, &l_dwBytesRead, NULL);
			if FAILED(hr) {
				MessageBox( nullptr,
					L"The Pixelshader file cannot be loaded", L"Error", MB_OK );
				CloseHandle(l_hShaderFile);
				return ERROR;
			}


			// Create the pixel shader    
			hr = g_pd3dDevice->CreatePixelShader( l_byteShader, l_dwBytesRead, nullptr, l_currInitPixelShader.l_ppPixelShader);

			if( FAILED( hr ) )
			{
				hr2message(__LINE__,hr, "g_pd3dDevice->CreatePixelShader()");
				return hr;
			}

			delete [] l_byteShader;
			CloseHandle(l_hShaderFile);
		}
	}


	// -- GeometryShader
	//
	//	
	
	{
		// Define the input layout
		D3D11_INPUT_ELEMENT_DESC layoutPSXVertex[] =
		{
			{ "SV_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },

			//{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		};

		// Define the input layout
		D3D11_INPUT_ELEMENT_DESC layoutSprite[] =
		{
			{ "ANCHOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "DIMENSION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "OPACITY", 0, DXGI_FORMAT_R32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//			{ "FILLBYTE", 0, DXGI_FORMAT_R32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		};

		D3D11_INPUT_ELEMENT_DESC layoutShoot[] =
		{
			{ "ANCHOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NEXTANCHOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			//{ "OPACITY", 0, DXGI_FORMAT_R32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		};

		D3D11_INPUT_ELEMENT_DESC layoutExplosion[] =
		{
			{ "ANCHOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			//{ "NEXTANCHOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			//{ "OPACITY", 0, DXGI_FORMAT_R32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		};


		WIN32_FILE_ATTRIBUTE_DATA l_FileAttrData = { 0 };

		struct {
			wchar_t* _strVertexShaderFileName;
			ID3D11VertexShader** l_ppVertexShader;
			wchar_t* _strGeometryShaderFileName;
			ID3D11GeometryShader** l_ppGeometryShader;
		   D3D11_INPUT_ELEMENT_DESC* l_pLayoutDesc;
		   UINT numElements; 
         ID3D11InputLayout** l_ppd3dInputLayout;
		} l_arr_InitVertShader[] = {
			{ L"vertexshader.cso", &g_pVertexShaderEnemyDrawer, NULL, NULL, layoutPSXVertex, ARRAYSIZE(layoutPSXVertex), &g_pVertexLayout},
			{ L"simple_vs.cso", &g_pVertexShaderSimpleDrawer, NULL, NULL, layoutPSXVertex, ARRAYSIZE(layoutPSXVertex), &g_pVertexLayout },
			{ L"bump_vs.cso", &g_pVertexShaderBumpDrawer, NULL, NULL,  layoutPSXVertex, ARRAYSIZE(layoutPSXVertex), &g_pVertexLayout},
			{ L"sprite_vs.cso", &g_pVertexShaderSpriteDrawer, L"sprite_gs.cso", &g_pSpriteGeometryShader, layoutSprite, ARRAYSIZE(layoutSprite), &g_pSpriteLayout },
			{ L"shoot_vs.cso", &g_pVertexShaderShootDrawer, L"shoot_gs.cso", &g_pShootGeometryShader, layoutShoot, ARRAYSIZE(layoutShoot), &g_pShootPosLayout },
			{ L"explosion_vs.cso", &g_pVertexShaderExplosion, L"explosion_gs.cso", &g_pExplosionGeometryShader, layoutExplosion, ARRAYSIZE(layoutExplosion), &g_pExplosionLayout },

		};
		for (auto l_currInitVertShader : l_arr_InitVertShader) 
		{
			memset(&l_FileAttrData, 0, sizeof(l_FileAttrData));
			if (GetFileAttributesExW(l_currInitVertShader._strVertexShaderFileName, GetFileExInfoStandard, &l_FileAttrData) == false)
			{
				MessageBox( nullptr,
					L"The Vertexshader file cannot be retrieved", L"Error", MB_OK );
				return ERROR;
			}
			DWORD l_dwVertexShaderSize = l_FileAttrData.nFileSizeLow;
			LPBYTE l_byteVertexShader = new BYTE[l_dwVertexShaderSize];

		
			HANDLE l_hVertexShaderFile = CreateFile(l_currInitVertShader._strVertexShaderFileName, 
								                         GENERIC_READ,FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL);
			if (l_hVertexShaderFile == INVALID_HANDLE_VALUE)
			{
				MessageBox( nullptr,
					L"The Vertexshader file cannot be opened", L"Error", MB_OK );
				CloseHandle(l_hVertexShaderFile);
				return ERROR;
			}
			DWORD l_dwBytesRead = 0;
			hr = ReadFile(l_hVertexShaderFile, l_byteVertexShader, l_dwVertexShaderSize, &l_dwBytesRead, NULL);
			if FAILED(hr) {
				MessageBox( nullptr,
					L"The Vertexshader file cannot be loaded", L"Error", MB_OK );
				CloseHandle(l_hVertexShaderFile);
				return ERROR;
			}

			// Create the vertex shader
			hr = g_pd3dDevice->CreateVertexShader( l_byteVertexShader, l_dwBytesRead, nullptr, l_currInitVertShader.l_ppVertexShader);
			if( FAILED( hr ) )
			{
				hr2message(__LINE__,hr, "g_pd3dDevice->CreateVertexShader()");
				return hr;
			}

			// Create the input layout
			// Once an input-layout object is created from a shader signature, the input-layout object can be reused 
			// with any other shader that has an identical input signature (semantics included). 
			// This can simplify the creation of input-layout objects when you are working with many shaders with identical inputs.
			if (l_currInitVertShader.l_pLayoutDesc)
			{
			   hr = g_pd3dDevice->CreateInputLayout(l_currInitVertShader.l_pLayoutDesc, l_currInitVertShader.numElements, l_byteVertexShader,
				   l_dwBytesRead, l_currInitVertShader.l_ppd3dInputLayout);
			   if (FAILED(hr))
			   {
				   hr2message(__LINE__, hr, "g_pd3dDevice->CreateInputLayout()");
				   return hr;
			   }

			}

			delete [] l_byteVertexShader;
			CloseHandle(l_hVertexShaderFile);

			if (l_currInitVertShader.l_ppGeometryShader)
			{
				  
				memset(&l_FileAttrData, 0, sizeof(l_FileAttrData));
				if (GetFileAttributesExW(l_currInitVertShader._strGeometryShaderFileName, GetFileExInfoStandard, &l_FileAttrData) == false)
				{
					MessageBox(nullptr,
						L"The GeometryShader file cannot be retrieved", L"Error", MB_OK);
					return ERROR;
				}
				DWORD l_dwGeometryShaderSize = l_FileAttrData.nFileSizeLow;
				LPBYTE l_byteGeometryShader = new BYTE[l_dwGeometryShaderSize];
	
	
				HANDLE l_hGeometryShaderFile = CreateFile(l_currInitVertShader._strGeometryShaderFileName, 
									                           GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				if (l_hGeometryShaderFile == INVALID_HANDLE_VALUE)
				{
					MessageBox(nullptr,
						L"The GeometryShader file cannot be opened", L"Error", MB_OK);
					CloseHandle(l_hGeometryShaderFile);
					return ERROR;
				}
				DWORD l_dwBytesRead = 0;
				hr = ReadFile(l_hGeometryShaderFile, l_byteGeometryShader, l_dwGeometryShaderSize, &l_dwBytesRead, NULL);
				if FAILED(hr) {
					MessageBox(nullptr,
						L"The GeometryShader file cannot be loaded", L"Error", MB_OK);
					CloseHandle(l_hGeometryShaderFile);
					return ERROR;
				}
	
				// Create the pixel shader
				hr = g_pd3dDevice->CreateGeometryShader(l_byteGeometryShader, l_dwBytesRead, nullptr, l_currInitVertShader.l_ppGeometryShader);
				if (FAILED(hr))
				{
					hr2message(__LINE__, hr, "g_pd3dDevice->CreateGeometryShader()");
					return hr;
				}
				delete[] l_byteGeometryShader;
				CloseHandle(l_hGeometryShaderFile);
			}

		}

	}
    // -- solid pixelshader
    //
	/*
    memset(&l_FileAttrData, 0, sizeof(l_FileAttrData));
    
    wchar_t l_strSolidPixelShaderFileName[] = L"solid.cso";
    if (GetFileAttributesExW(l_strSolidPixelShaderFileName,GetFileExInfoStandard, &l_FileAttrData) == false)
    {
        MessageBox( nullptr,
            L"The Solid Pixelshader file cannot be retrieved", L"Error", MB_OK );
        return ERROR;
    }
    l_dwShaderSize = l_FileAttrData.nFileSizeLow;    
    l_byteShader = new BYTE[l_dwShaderSize];

    l_hShaderFile = CreateFile(l_strSolidPixelShaderFileName, GENERIC_READ,FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL);
    if (l_hShaderFile == INVALID_HANDLE_VALUE) 
    {
        MessageBox( nullptr,
            L"The Solid PIxelshader file cannot be opened", L"Error", MB_OK );
        CloseHandle(l_hShaderFile);
        return ERROR;
    }
    l_dwBytesRead = 0;
    hr = ReadFile(l_hShaderFile, l_byteShader, l_dwShaderSize, &l_dwBytesRead, NULL);
    if FAILED(hr) {
        MessageBox( nullptr,
            L"The SolidPixelshader file cannot be loaded", L"Error", MB_OK );
        CloseHandle(l_hShaderFile);
        return ERROR;
    }


    // Create the pixel shader    
    hr = g_pd3dDevice->CreatePixelShader( l_byteShader, l_dwBytesRead, nullptr, &g_pPixelShaderSolid );    
    if( FAILED( hr ) )
    {
        hr2message(__LINE__,hr, "g_pd3dDevice->CreatePixelShader()");
        return hr;
    }

    delete [] l_byteShader;
    CloseHandle(l_hShaderFile);

	*/
    // Create vertex buffer
	/*
    SimpleVertex vertices[] =
    {
        { XMFLOAT3( -1.0f, 1.0f, -1.0f ), XMFLOAT3( 0.0f, 1.0f, 0.0f ) , XMFLOAT2( 1.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, -1.0f ), XMFLOAT3( 0.0f, 1.0f, 0.0f ) , XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT3( 0.0f, 1.0f, 0.0f ) , XMFLOAT2( 0.0f, 1.0f ) },
        { XMFLOAT3( -1.0f, 1.0f, 1.0f ), XMFLOAT3( 0.0f, 1.0f, 0.0f ) , XMFLOAT2( 1.0f, 1.0f ) },

        { XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT3( 0.0f, -1.0f, 0.0f ) , XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, -1.0f, -1.0f ), XMFLOAT3( 0.0f, -1.0f, 0.0f ) , XMFLOAT2( 1.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT3( 0.0f, -1.0f, 0.0f ) , XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT3( 0.0f, -1.0f, 0.0f ) , XMFLOAT2( 0.0f, 1.0f ) },

        { XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT3( -1.0f, 0.0f, 0.0f ) , XMFLOAT2( 0.0f, 1.0f ) },
        { XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT3( -1.0f, 0.0f, 0.0f ) , XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3( -1.0f, 1.0f, -1.0f ), XMFLOAT3( -1.0f, 0.0f, 0.0f ) , XMFLOAT2( 1.0f, 0.0f ) },
        { XMFLOAT3( -1.0f, 1.0f, 1.0f ), XMFLOAT3( -1.0f, 0.0f, 0.0f ) , XMFLOAT2( 0.0f, 0.0f ) },

        { XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT3( 1.0f, 0.0f, 0.0f ) , XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3( 1.0f, -1.0f, -1.0f ), XMFLOAT3( 1.0f, 0.0f, 0.0f ) , XMFLOAT2( 0.0f, 1.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, -1.0f ), XMFLOAT3( 1.0f, 0.0f, 0.0f ) , XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT3( 1.0f, 0.0f, 0.0f ) , XMFLOAT2( 1.0f, 0.0f ) },

        { XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT3( 0.0f, 0.0f, -1.0f ) , XMFLOAT2( 0.0f, 1.0f ) },
        { XMFLOAT3( 1.0f, -1.0f, -1.0f ), XMFLOAT3( 0.0f, 0.0f, -1.0f ) , XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, -1.0f ), XMFLOAT3( 0.0f, 0.0f, -1.0f ) , XMFLOAT2( 1.0f, 0.0f ) },
        { XMFLOAT3( -1.0f, 1.0f, -1.0f ), XMFLOAT3( 0.0f, 0.0f, -1.0f ) , XMFLOAT2( 0.0f, 0.0f ) },

        { XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT3( 0.0f, 0.0f, 1.0f ) , XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT3( 0.0f, 0.0f, 1.0f ) , XMFLOAT2( 0.0f, 1.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT3( 0.0f, 0.0f, 1.0f ) , XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3( -1.0f, 1.0f, 1.0f ), XMFLOAT3( 0.0f, 0.0f, 1.0f ) , XMFLOAT2( 1.0f, 0.0f ) },
    };

	*/

	

	// -- Billboard positions
	//
	
	{

		//SPRITE l_spriteArray[] = {
		//	{ XMFLOAT4(-0.5f, -0.5f,0.0f,1.0f) },
		//	{ XMFLOAT4(0.5f, 0.5f,0.0f,1.0f) },
		//	{ XMFLOAT4(1.0f, 1.0f,10.0f,1.0f) },
		//};

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.ByteWidth = sizeof(SPRITE) * g_maxSprite;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;		

		//D3D11_SUBRESOURCE_DATA InitData;
		//D3D11_MAPPED_SUBRESOURCE l_mappedsubInitData;
		//ZeroMemory(&InitData, sizeof(InitData));

		//InitData.pSysMem = l_spriteArray;
		//hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pSpriteBuffer);
		hr = g_pd3dDevice->CreateBuffer(&bd, NULL, &g_pSpriteBuffer);
		if (FAILED(hr))
		{
			hr2message(__LINE__, hr, "g_pd3dDevice->CreateBuffer()");
			return hr;
		}

		//hr = g_pImmediateContext->Map(
		//		g_pSpriteBuffer,             //ID3D11Resource *pResource,
		//		0,                           //UINT Subresource,
		//		D3D11_MAP_WRITE_DISCARD,     //D3D11_MAP MapType,
		//		0,                           //UINT MapFlags,
		//		&l_mappedsubInitData);       //D3D11_MAPPED_SUBRESOURCE *pMappedResource
		//if (FAILED(hr))
		//{
		//	hr2message(__LINE__, hr, "g_pd3dDevice->Map()");
		//	return hr;
		//}
		//memcpy(l_mappedsubInitData.pData, l_spriteArray, sizeof(l_spriteArray));
		//g_pImmediateContext->Unmap(g_pSpriteBuffer, 0);

	}
	
	// -- Shootpos Buffer for Geometry Shader
	//
	{
		SHOOTPOS l_shootposArray[] = {
			{ XMFLOAT4(-0.5f, -0.5f,0.0f,1.0f),XMFLOAT4(-0.5f, -0.5f,50.0f,1.0f) },
			{ XMFLOAT4(0.5f, 0.5f,0.0f,1.0f),XMFLOAT4(0.5f, 0.5f,50.0f,1.0f) },
			{ XMFLOAT4(1.0f, 1.0f,10.0f,1.0f),XMFLOAT4(1.0f, 1.0f,50.0f,1.0f) },
			//{ XMFLOAT2(-0.5f, -0.5f), XMFLOAT2(0.1f, 0.1f), 1.0f },
			//{ XMFLOAT2(0.5f, 0.5f), XMFLOAT2(0.1f, 0.1f), 1.0f },
		};

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.ByteWidth = sizeof(SHOOTPOS) * g_maxShoot;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;		

		//D3D11_SUBRESOURCE_DATA InitData;
		D3D11_MAPPED_SUBRESOURCE l_mappedsubInitData;
		//ZeroMemory(&InitData, sizeof(InitData));

		//InitData.pSysMem = l_shootposArray;
		//hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pShootPosBuffer);
		hr = g_pd3dDevice->CreateBuffer(&bd, NULL, &g_pShootPosBuffer);
		if (FAILED(hr))
		{
			hr2message(__LINE__, hr, "g_pd3dDevice->CreateBuffer()");
			return hr;
		}

		hr = g_pImmediateContext->Map(
				g_pShootPosBuffer,             //ID3D11Resource *pResource,
				0,                           //UINT Subresource,
				D3D11_MAP_WRITE_DISCARD,     //D3D11_MAP MapType,
				0,                           //UINT MapFlags,
				&l_mappedsubInitData);       //D3D11_MAPPED_SUBRESOURCE *pMappedResource
		if (FAILED(hr))
		{
			hr2message(__LINE__, hr, "g_pd3dDevice->Map()");
			return hr;
		}
		memcpy(l_mappedsubInitData.pData, l_shootposArray, sizeof(l_shootposArray));
		g_pImmediateContext->Unmap(g_pShootPosBuffer, 0);

	}

	//
	// -- explosion particle system for Geometry Shader
	//
	{
   	EXPLOSIONPOS* l_vExplosionArray;
   	EXPLOSIONPOS* l_vExplosionArray2;
	   float ExplosionVelocity[] = {
	      0.13,0.43,0.26,0.31,0.91,0.36,0.34,0.32,0.18,0.23,
	      0.44,0.22,0.87,0.92,0.87,0.26,0.24,0.83,0.36,0.67,
	      0.54,0.82,0.68,0.72,0.93,0.36,0.85,0.64,0.53,0.32,
	      0.55,0.36,0.44,0.24,0.86,0.74,0.59,0.10,0.37,0.56,
	      0.34,0.13,0.25,0.69,0.89,0.24,0.42,0.35,0.27,0.79,
	      0.28,0.47,0.45,0.34,0.53,0.47,0.29,0.79,0.15,0.63,
	      0.14,0.66,0.58,0.92,0.84,0.16,0.15,0.33,0.83,0.52
	   };
	
	   float SmokeVelocity[] = {
	      0.13,0.43,0.26,0.31,0.11,0.26,0.34,0.32,0.18,0.23,
	      0.44,0.22,0.47,0.12,0.27,0.36,0.24,0.13,0.36,0.27,
	      0.54,0.12,0.28,0.22,0.33,0.36,0.15,0.24,0.53,0.32,
	      0.55,0.36,0.44,0.24,0.46,0.54,0.59,0.10,0.37,0.56,
	      0.34,0.13,0.25,0.49,0.29,0.24,0.42,0.25,0.27,0.19,
	      0.28,0.47,0.45,0.34,0.13,0.47,0.29,0.19,0.15,0.23,
	      0.14,0.26,0.38,0.32,0.24,0.16,0.15,0.33,0.23,0.52
	   };
	
	   int cnt_radpitch = sizeof(ExplosionVelocity)/sizeof(float);

	   const int l_cntExplosionSphereRows    = 8,
	       l_cntExplosionSphereColumns = 8;
	
	   int idx_radpitch = 0;
	
	   int l_idxExplosionRow;
	   float l_fExplosionWidth;
	   float l_fExplosionRingY;
	   float l_fExplosionRowRadStep;
	   float l_fExplosionRowCurrentRad;
	
		WriteLogFile("explosion particle system as vertexbuffer\n");

   	// -- Vertexbuffer dimensionieren
   	//
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.ByteWidth = sizeof(EXPLOSIONPOS) * l_cntExplosionSphereColumns * l_cntExplosionSphereRows;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;		

		//D3D11_SUBRESOURCE_DATA InitData;
		D3D11_MAPPED_SUBRESOURCE l_mappedsubInitData;
		//ZeroMemory(&InitData, sizeof(InitData));

		//InitData.pSysMem = l_explosionposArray;
		hr = g_pd3dDevice->CreateBuffer(&bd, NULL, &g_pExplosionVB);
		if (FAILED(hr))
		{
			hr2message(__LINE__, hr, "g_pd3dDevice->CreateBuffer()");
			return hr;
		}

		hr = g_pImmediateContext->Map(
				g_pExplosionVB,             //ID3D11Resource *pResource,
				0,                           //UINT Subresource,
				D3D11_MAP_WRITE_DISCARD,     //D3D11_MAP MapType,
				0,                           //UINT MapFlags,
				&l_mappedsubInitData);       //D3D11_MAPPED_SUBRESOURCE *pMappedResource
		if (FAILED(hr))
		{
			hr2message(__LINE__, hr, "g_pd3dDevice->Map()");
			return hr;
		}

   	// --- copying to Vertexbuffer
   	//
		l_vExplosionArray = reinterpret_cast<EXPLOSIONPOS*>(l_mappedsubInitData.pData);
		//l_vExplosionArray2
		//
		//memcpy(l_mappedsubInitData.pData, l_explosionposArray, sizeof(l_explosionposArray));
	   g_cntExplVert = 0;
	
	   l_idxExplosionRow   =0;
	   l_fExplosionWidth = 0.0f; 
	   l_fExplosionRingY = 0.0f;
	
	   l_fExplosionRowRadStep = 2 * M_PI / float(l_cntExplosionSphereRows);
	   l_fExplosionRowCurrentRad = l_fExplosionRowRadStep;
	
	   idx_radpitch = 0;
	
	   for (l_idxExplosionRow = 0; 
	        l_idxExplosionRow < (l_cntExplosionSphereRows-1);
	        l_idxExplosionRow++)
	   {
	      l_fExplosionRingY = -cos(l_fExplosionRowCurrentRad);
	      l_fExplosionWidth  = sin(l_fExplosionRowCurrentRad);
	
	      int l_idxExplosionColumn=0;
	      float l_fExplosionColRadStep = 2* M_PI / float(l_cntExplosionSphereColumns);
	      float l_fExplosionColCurrentRad = l_fExplosionColRadStep;
	
	      for(l_idxExplosionColumn=0;
	          l_idxExplosionColumn < (l_cntExplosionSphereColumns-1);
	          l_idxExplosionColumn++)
	      {
	
	         l_vExplosionArray[g_cntExplVert].topLeft.x = -cos(l_fExplosionColCurrentRad) * 
	                                              l_fExplosionWidth * ExplosionVelocity[idx_radpitch];
	         l_vExplosionArray[g_cntExplVert].topLeft.y = l_fExplosionRingY * ExplosionVelocity[idx_radpitch];
	         l_vExplosionArray[g_cntExplVert].topLeft.z = sin(l_fExplosionColCurrentRad) * 
	                                              l_fExplosionWidth * ExplosionVelocity[idx_radpitch];
	
				/*
	         l_vExplosionArray2[g_cntExplVert].topLeft.x = -cos(l_fExplosionColCurrentRad) * 
	                                               l_fExplosionWidth * SmokeVelocity[idx_radpitch];
	         l_vExplosionArray2[g_cntExplVert].topLeft.y = l_fExplosionRingY * SmokeVelocity[idx_radpitch];
	         l_vExplosionArray2[g_cntExplVert].topLeft.z = sin(l_fExplosionColCurrentRad) * 
	                                               l_fExplosionWidth * SmokeVelocity[idx_radpitch];
																  */
	
	         idx_radpitch++;
	         if (idx_radpitch >= cnt_radpitch)
	         {
	            idx_radpitch = 0;
	         }
	     
	         g_cntExplVert++;
	
	         l_fExplosionColCurrentRad = l_fExplosionColCurrentRad + l_fExplosionColRadStep;
	      }
	      l_fExplosionRowCurrentRad = l_fExplosionRowCurrentRad + l_fExplosionRowRadStep;
	   }
	
	

		g_pImmediateContext->Unmap(g_pExplosionVB, 0);

	}
	
	
	// Create index buffer
	// Create vertex buffer
	/*
	WORD indices[] =
	{
        3,1,0,
        2,1,3,

        6,4,5,
        7,4,6,

        11,9,8,
        10,9,11,

        14,12,13,
        15,12,14,

        19,17,16,
        18,17,19,

        22,20,21,
        23,20,22
    };
	*/

    /*bd.Usage = D3D11_USAGE_DEFAULT;
    //bd.ByteWidth = sizeof( WORD ) * 36;        // 36 vertices needed for 12 triangles in a triangle list
	bd.ByteWidth = sizeof(leader_indices);
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
	//InitData.pSysMem = indices;
    InitData.pSysMem = leader_indices;
    hr = g_pd3dDevice->CreateBuffer( &bd, &InitData, &g_pIndexBuffer );
    if( FAILED( hr ) )
    {
        hr2message(__LINE__,hr, "g_pd3dDevice->CreateBuffer(index)");
        return hr;
    }
	*/

    // Set primitive topology
    g_pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

    
    // Load the Texture
    //hr = CreateDDSTextureFromFile( g_pd3dDevice, L"seafloor.dds", nullptr, &g_pTextureRV );
    //if( FAILED( hr ) )
    //{
    //    hr2message(__LINE__,hr, "CreateDDSTextureFromFile()");
    //    return hr;
    //}
        
    // Create the sample state
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory( &sampDesc, sizeof(sampDesc) );
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    hr = g_pd3dDevice->CreateSamplerState( &sampDesc, &ENEMYDRAWER::m_pSamplerLinear );
    if( FAILED( hr ) )
    {
        hr2message(__LINE__,hr, "g_pd3dDevice->CreateSamplerState()");
        return hr;
    }

    // Initialize the world matrix
    g_World = XMMatrixIdentity();

    // Initialize the view matrix
    //XMVECTOR Eye = XMVectorSet( 0.0f, 1.0f, -5.0f, 0.0f );
	
	g_vCamera = XMVectorSet( 0.0f, 0.0f, -10.0f, 0.0f );
	g_matCamera = XMMatrixIdentity();
	g_View = g_matCamera * XMMatrixTranslationFromVector(-g_vCamera);
	
  // Initialize the projection matrix
    g_Projection = XMMatrixPerspectiveFovLH( XM_PIDIV4, width / (FLOAT)height, 0.01f, 1000.0f ); //Far clipping

	WriteLogFile("preparing Direct2D..\n");
	// -- prepare Direct2D
	//
	{
		D2D1_FACTORY_OPTIONS l_options;

		memset(&l_options, 0, sizeof(l_options));
		l_options.debugLevel = D2D1_DEBUG_LEVEL_NONE;//D2D1_DEBUG_LEVEL_WARNING;

		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
			__uuidof(ID2D1Factory1),
			&l_options,
			(void**) &g_pd2dFactory);
			
		if (hr != 0) {
			hr2message(__LINE__, hr, "D2D1CreateFactory()");
			return (FALSE);
		}
	}

	

	hr = g_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)&g_pdxgiDevice);
	if (hr != 0) {
		hr2message(__LINE__, hr, "g_pd3dDevice->QueryInterface(IDXGIDevice)");
		return (FALSE);
	}
	
	hr = g_pd2dFactory->CreateDevice(g_pdxgiDevice, &g_pd2dDevice);
	if (hr != 0) {
		hr2message(__LINE__, hr, "g_pd2dFactory->CreateDevice()");
		return (FALSE);
	}

	
	hr = g_pd2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &g_pd2dDeviceContext);
	if (hr != 0) {
		hr2message(__LINE__, hr, "g_pd2dDevice->CreateDeviceContext()");
		return (FALSE);
	}

	
 //Now we set up the Direct2D render target bitmap linked to the swapchain. 
 //Whenever we render to this bitmap, it will be directly rendered to the 
 //swapchain associated with the window.
	//D2D1_BITMAP_PROPERTIES1 bitmapProperties;
	//bitmapProperties.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
	//	
	//bitmapProperties.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
	//bitmapProperties.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
	//bitmapProperties.dpiX = 96;
	//bitmapProperties.dpiY = 96;

	hr = g_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface), (void**) &g_pdxgiSurface);
	if (hr != 0) {
		hr2message(__LINE__, hr, "g_pSwapChain->GetBuffer(IDXGISurface)");
		return (FALSE);
	}

	
	
	D2D1_RENDER_TARGET_PROPERTIES l_d2dRenderTargetProp = D2D1::RenderTargetProperties();

	//D2D1_HWND_RENDER_TARGET_PROPERTIES l_d2dhwndRenderTargetProp = 
	//	D2D1::HwndRenderTargetProperties(
	//		g_hWnd,
	//		D2D1::SizeU(
	//			rc.right - rc.left,
	//			rc.bottom - rc.top));

	l_d2dRenderTargetProp.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
	l_d2dRenderTargetProp.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
	l_d2dRenderTargetProp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
	l_d2dRenderTargetProp.usage = D2D1_RENDER_TARGET_USAGE_FORCE_BITMAP_REMOTING;
	l_d2dRenderTargetProp.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;
	l_d2dRenderTargetProp.dpiX = 96;
	l_d2dRenderTargetProp.dpiY = 96;

	hr = g_pd2dFactory->CreateDxgiSurfaceRenderTarget(g_pdxgiSurface, l_d2dRenderTargetProp, &g_pd2dRenderTarget);
	if (hr != 0) {
		hr2message(__LINE__, hr, "g_pd2dFactory->CreateDxgiSurfaceRenderTarget()");
		return (FALSE);
	}
	//hr = g_pd2dFactory->CreateHwndRenderTarget(
	//	l_d2dRenderTargetProp,
	//	l_d2dhwndRenderTargetProp,
	//	&g_pd2dhwdRenderTarget
	//);
	//if (hr != 0) {
	//	hr2message(__LINE__, hr, "g_pd2dFactory->CreateHwndRenderTarget()");
	//	return (FALSE);
	//}
	
	//hr = g_pd2dDeviceContext->CreateBitmapFromDxgiSurface(g_pdxgiSurface, &bitmapProperties, &g_pd2dBitmap);
	//if (hr != 0) {
	//	hr2message(__LINE__, hr, "g_pd2dDeviceContext->CreateBitmapFromDxgiSurface()");
	//	return (FALSE);
	//}
	//
	//g_pd2dDeviceContext->SetTarget(g_pd2dBitmap);

	hr = g_pd2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &g_pd2dSolidWhiteBrush);
	if (hr != 0) {
		hr2message(__LINE__, hr, "g_pd2dhwdRenderTarget->CreateSolidColorBrush(Black)");
		return (FALSE);
	}

	// -- initialize DirectWrite
	//
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&g_pdwriteFactory));
	if (hr != 0) {
		hr2message(__LINE__, hr, "DWriteCreateFactory()");
		return (FALSE);
	}
	hr = g_pdwriteFactory->CreateTextFormat(L"Gabriola", NULL,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		30.0f,
		L"en-us",
		&g_pdwriteTextFormat);
	if (hr != 0) {
		hr2message(__LINE__, hr, "g_pd2dDeviceContext->CreateTextFormat()");
		return (FALSE);
	}
	
	//hr = g_pdwriteTextFormat->SetFlowDirection(DWRITE_FLOW_DIRECTION_LEFT_TO_RIGHT);
	//hr = g_pdwriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	hr = g_pdwriteTextFormat->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);
	if (hr != 0) {
		hr2message(__LINE__, hr, "g_pdwriteTextFormat->SetReadingDirection(DWRITE_FLOW_DIRECTION_LEFT_TO_RIGHT)");
		return (FALSE);
	}

	hr = g_pdwriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
	if (hr != 0) {
		hr2message(__LINE__, hr, "g_pdwriteTextFormat->SetTextAlignment()");
		return (FALSE);
	}
	
	hr = g_pdwriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	if (hr != 0) {
		hr2message(__LINE__, hr, "g_pdwriteTextFormat->SetParagraphAlignment()");
		return (FALSE);
	}

	
	g_d2dsizeClientRect = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
	D2D1_RECT_F layoutRect = D2D1::RectF(
		static_cast<FLOAT>(rc.left),// / l_d2dRenderTargetProp.dpiX,
	
		static_cast<FLOAT>(rc.top),// / l_d2dRenderTargetProp.dpiY,
		static_cast<FLOAT>(rc.right - rc.left),// / l_d2dRenderTargetProp.dpiX,
		static_cast<FLOAT>(rc.bottom - rc.top)// / l_d2dRenderTargetProp.dpiY
	);

    // --- preparing DirectInput for mouse , joystick
    //
    WriteLogFile("preparing DirectInput for key, mouse , joystick\n");
    hr = DirectInput8Create(g_hInst, DIRECTINPUT_VERSION, IID_IDirectInput8,
                          (void**)&lpDirectInput, NULL);
    if (hr != 0) { 
       hr2message(__LINE__,hr,"DirectInputCreate()");
       return (FALSE) ;
    }
       
    hr = lpDirectInput->CreateDevice(GUID_SysMouse, &g_didevMouse, NULL);
    if (hr != 0) { 
       hr2message(__LINE__,hr,"lpDirectInput->CreateDevice(SysMouse)");
       return (FALSE) ;
    }
    
    
    hr = g_didevMouse->SetDataFormat(&c_dfDIMouse);
    if (hr != 0) { 
       hr2message(__LINE__,hr,"g_didevMouse->SetDataFormat()");
       return (FALSE) ;
    }
    
    hr = g_didevMouse->SetCooperativeLevel(g_hWnd, 
                                           DISCL_FOREGROUND|
                                           DISCL_EXCLUSIVE);
    if (hr != 0) { 
       hr2message(__LINE__,hr,"g_didevMouse->SetCooperativeLevel()");
       return (FALSE) ;
    }
    

    HANDLE l_hMouseEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    
    if (l_hMouseEvent == NULL) {
       hr2message(__LINE__,hr,"g_didevMouse->CreateEvent()");
       return FALSE;
    }
     
    hr = g_didevMouse->SetEventNotification(l_hMouseEvent);
    if (FAILED(hr)) {
       hr2message(__LINE__,hr,"g_didevMouse->SetEventNotification()");
       return FALSE;
    }
       
    
    hr = g_didevMouse->Acquire();
    
    // -- falls Access Denied: dann liest windows die mausbewegungen eben mit und 
    //    knarzt mit dem systemlautsprecher
    //
    if (hr == E_ACCESSDENIED)
    {
        hr = g_didevMouse->SetCooperativeLevel(g_hWnd, 
                                               DISCL_BACKGROUND|
                                               DISCL_NONEXCLUSIVE);
        if (hr != 0) { 
           hr2message(__LINE__,hr,"g_didevMouse->SetCooperativeLevel()");
           return (FALSE) ;
        }
        hr = g_didevMouse->Acquire();
    }
    
    if (hr != 0) { 
       hr2message(__LINE__,hr,"g_didevMouse->Acquire()");
       return (FALSE) ;
    }
    
    ShowCursor(FALSE);
    
    // -- get keyboard
    //
    hr = lpDirectInput->CreateDevice(GUID_SysKeyboard, &g_didevKeyboard, NULL);
    if (hr != 0) { 
       hr2message(__LINE__,hr,"lpDirectInput->CreateDevice(SysKeyboard)");
       return (FALSE) ;
    }
    
    hr = g_didevKeyboard->SetCooperativeLevel(g_hWnd, 
                                              DISCL_FOREGROUND|
                                              DISCL_EXCLUSIVE);
    if (hr != 0) { 
       hr2message(__LINE__,hr,"g_didevKeyboard->SetCooperativeLevel()");
       return (FALSE) ;
    }
    
    hr = g_didevKeyboard->SetDataFormat(&c_dfDIKeyboard);
    if (hr != 0) { 
       hr2message(__LINE__,hr,"g_didevKeyboard->SetDataFormat()");
       return (FALSE) ;
    }
    
    hr = g_didevKeyboard->Acquire();
    
    // -- falls Access Denied: dann liest windows die tastatur eben mit und 
    //    knarzt mit dem systemlautsprecher
    //
    if (hr == E_ACCESSDENIED)
    {
        hr = g_didevKeyboard->SetCooperativeLevel(g_hWnd, 
                                                  DISCL_BACKGROUND|
                                                  DISCL_NONEXCLUSIVE);
        if (hr != 0) { 
           hr2message(__LINE__,hr,"g_didevKeyboard->SetCooperativeLevel()");
           return (FALSE) ;
        }
        hr = g_didevKeyboard->Acquire();
    }
    
    if (hr != 0) { 
       hr2message(__LINE__,hr,"g_didevKeyboard->Acquire()");
       return (FALSE) ;
    }
    
    
    return S_OK;
    

}

// -- den Bildschirm und EingabeFocus in Beschlag nehmen und GamePlay walten lassen
//
bool PSXRunFullScreen()
{
	HRESULT hr;

	/***************************************************************
	*							EINGABE
	***************************************************************/
	hr = g_didevMouse->GetDeviceState(sizeof(DIMOUSESTATE), &diMouseState);
	if (hr != 0) {
		hr2message(__LINE__,hr,"g_didevMouse->GetDeviceState()");
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
			hr2message(__LINE__,hr,"g_didevKeyboard->Acquire()");
			return(FALSE);
		}

	}else{
		if (hr != 0)
		{
			hr2message(__LINE__,hr,"g_didevKeyboard->GetDeviceState()");
			return(FALSE);
		}
	}

	/***************************************************************
	*						VERARBEITUNG
	***************************************************************/

	// -- gamebezogene Eingabe+Verarbeitung
	//
	if (PSXUpdateGameManagement() == false) return false;

	/*if (g_Player.getGS()->ShallUpdateEnemies())*/
	{
		if (g_bWriteLogFileInLoop) 
		{
			WriteLogFile("invoking dWorldQuickStep()\n");
		}
	
		if (g_fElapsedTime > 0.0f)
		{
			if (LEVEL::m_iWorldID) dWorldQuickStep(LEVEL::m_iWorldID, g_fElapsedTime * 0.05f); //0.4f);
		}

	}

	/***************************************************************
	*                        AUSGABE 
	***************************************************************/
	//
    // Clear the back buffer
    //
    g_pImmediateContext->ClearRenderTargetView( g_pRenderTargetView, Colors::MidnightBlue );
    
    //
    // Clear the depth buffer to 1.0 (max depth)
    //
    g_pImmediateContext->ClearDepthStencilView( g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );

	// -- reset: default Blending state and Depth-Stencil State, restore primitiv and input layout
	//
	g_pImmediateContext->OMSetBlendState(NULL, NULL, 0xffffffff);
	g_pImmediateContext->OMSetDepthStencilState(NULL, 0);
	g_pImmediateContext->IASetInputLayout(g_pVertexLayout);
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	if (PSXRender(RENDERPASS_FULLSCREEN) == false)
	{
		return(FALSE);
	}



	//
	// Present our back buffer to our front buffer
	//
	hr = g_pSwapChain->Present( 0, 0 );
	if (FAILED(hr))
	{
		hr2message(__LINE__,hr, "g_pSwapChain->Present( 0, 0 )");
		return false;
	}



	// -- CPU unabhängiger Timer
	//
	{
		DWORD l_dwNewTickCount = 0, l_dwTickDiff = 0;
		int l_iJumpWitchSpace = 10;
      

		// -- Neuen Differenz-Wert errechnen
		//
		l_dwNewTickCount = GetTickCount();
		if (l_dwNewTickCount > g_dwLastTickCount) //nach 47 Tagen auftretener überlauf ?
		{
   		l_dwTickDiff = l_dwNewTickCount - g_dwLastTickCount;
         if (l_dwTickDiff < 100)
         {
             PSXTIMER::m_dwSample[PSXTIMER::m_idxSample] = l_dwTickDiff;

             if (PSXTIMER::m_cntSample < PSXTIMER::m_maxSample)
             {
                 PSXTIMER::m_cntSample++;
             }

             if (PSXTIMER::m_idxSample < (PSXTIMER::m_maxSample - 1))
             {
                 PSXTIMER::m_idxSample++;
             }
             else {
                 PSXTIMER::m_idxSample = 0;
             }
             /*
             // -- Platz schaffen und Differenz ans Ende der Samplekette hängen
             //
             if (g_cntFrame < 100)
             {
                for (l_idxSample = 0; l_idxSample < (PSXTIMER::m_cntSample - 1); l_idxSample++)
                {
                   PSXTIMER::m_dwSample[l_idxSample] = PSXTIMER::m_dwSample[l_idxSample + 1];
                }
                PSXTIMER::m_dwSample[PSXTIMER::m_cntSample - 1] = l_dwTickDiff;

                g_fElapsedTime = 1.0f;
             }
             else {
                if (float(l_dwTickDiff) <= float(PSXTIMER::m_dwSample[PSXTIMER::m_cntSample - 1]) * 2.0f &&
                   float(l_dwTickDiff) >= float(PSXTIMER::m_dwSample[PSXTIMER::m_cntSample - 1]) * 0.5f)
                {
                   for (l_idxSample = 0; l_idxSample < (PSXTIMER::m_cntSample - 1); l_idxSample++)
                   {
                      PSXTIMER::m_dwSample[l_idxSample] = PSXTIMER::m_dwSample[l_idxSample + 1];
                   }
                   PSXTIMER::m_dwSample[PSXTIMER::m_cntSample - 1] = l_dwTickDiff;
                }
                */
         }
		}
   	g_dwLastTickCount = l_dwNewTickCount;

      // -- Durchschnitt der Samplekette ermitteln
      //     
      g_fElapsedTime = 0.0f;

      
      float l_fSampleSum = 0.0f;

      // -- überhaupt erstmal Werte sammeln
      //
      if (PSXTIMER::m_cntSample > 0)
      {

      
          for (int l_idxSample = 0; l_idxSample < PSXTIMER::m_cntSample; l_idxSample++)
          {
          
              l_fSampleSum = l_fSampleSum + float(PSXTIMER::m_dwSample[l_idxSample]);
              
          }
      
          g_fElapsedTime = float(int(l_fSampleSum / float(PSXTIMER::m_cntSample) + 0.5f));
      }
      

		//PSXPrintf(__LINE__, debugausgabe, g_maxDebugAusgabe, "g_fElapsedTime = %f",g_fElapsedTime);

     
	}

	// -- fps: Frames per Second ausrechnen
	//
	/*
	if (g_fElapsedTime / 1000.0f >= 1.0f)
	{
		g_iFramesPerLastSec = g_iFramesPerSec;

		g_iFramesPerSec = 0;
	}
	else {
		g_iFramesPerSec++;
	}
	*/
	if (g_fElapsedTime <= 0.0f)
	{
		g_fElapsedTime = 1.0f;
	}

	g_cntFrame = g_cntFrame + 1;

	return true;
}


//---------------------------------------------------------------------------
// Clean up the objects we've created
//---------------------------------------------------------------------------
void CleanupDevice()
{
	if (g_pdwriteTextFormat) g_pdwriteTextFormat->Release();
	if (g_pdwriteFactory) g_pdwriteFactory->Release();

	if (g_pd2dSolidWhiteBrush) g_pd2dSolidWhiteBrush->Release();
	if (g_pd2dRenderTarget) g_pd2dRenderTarget->Release();
	if (g_pd2dDeviceContext) g_pd2dDeviceContext->Release();
	if (g_pd2dDevice) g_pd2dDevice->Release();
	if (g_pdxgiDevice) g_pdxgiDevice->Release();
	if (g_pd2dFactory) g_pd2dFactory->Release();
    //if( g_pImmediateContext ) g_pImmediateContext->ClearState();

	if( ENEMYDRAWER::m_pSamplerLinear ) ENEMYDRAWER::m_pSamplerLinear->Release();
    
	PSXReleaseAllTextures();

	ID3D11Debug* l_pd3dDebug = NULL;
	HRESULT hr = g_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&l_pd3dDebug);
	if (hr != 0) {
		hr2message(__LINE__, hr, "g_pd3dDevice->QueryInterface(IDXGIDevice)");
	}

	if (l_pd3dDebug)
	{
		l_pd3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
		l_pd3dDebug->Release();
	}


	// -- clean buffers
	//
	if (g_pShootPosBuffer) g_pShootPosBuffer->Release();
	if (g_pSpriteBuffer) g_pSpriteBuffer->Release();
   if (g_pExplosionVB ) g_pExplosionVB->Release();

   //if( g_pTextureRV ) g_pTextureRV->Release();
   if( ENEMYDRAWER::m_pcbChangeEveryFrame ) ENEMYDRAWER::m_pcbChangeEveryFrame->Release();
   if (ENEMYDRAWER::m_pcbChangeOnResize ) ENEMYDRAWER::m_pcbChangeOnResize->Release();
   if (ENEMYDRAWER::m_pcbChangeEveryObject) ENEMYDRAWER::m_pcbChangeEveryObject->Release();
   //if( g_pVertexBuffer ) g_pVertexBuffer->Release();
   //if( g_pIndexBuffer ) g_pIndexBuffer->Release();

	//
	// -- clean pixelshaders
	//
   if( g_pVertexLayout ) g_pVertexLayout->Release();
   if( g_pVertexShaderEnemyDrawer ) g_pVertexShaderEnemyDrawer->Release();
   if( g_pPixelShaderEnemyDrawer ) g_pPixelShaderEnemyDrawer->Release();
	if( g_pVertexShaderSimpleDrawer ) g_pVertexShaderSimpleDrawer->Release();
   if( g_pPixelShaderSimpleDrawer ) g_pPixelShaderSimpleDrawer->Release();
   if( g_pPixelShaderSolid ) g_pPixelShaderSolid->Release();

   if (g_pSpriteLayout ) g_pSpriteLayout->Release();

   if (g_pShootPosLayout ) g_pShootPosLayout->Release();
   if (g_pVertexShaderBumpDrawer) g_pVertexShaderBumpDrawer->Release();
   if (g_pPixelShaderBumpDrawer) g_pPixelShaderBumpDrawer->Release();

   if (g_pVertexShaderSpriteDrawer) g_pVertexShaderSpriteDrawer->Release();
   if (g_pPixelShaderSpriteDrawer) g_pPixelShaderSpriteDrawer->Release();
   if (g_pSpriteGeometryShader) g_pSpriteGeometryShader->Release();

   if (g_pVertexShaderShootDrawer) g_pVertexShaderShootDrawer->Release();
   if (g_pPixelShaderShootDrawer) g_pPixelShaderShootDrawer->Release();
   if (g_pShootGeometryShader) g_pShootGeometryShader->Release();

   if (g_pVertexShaderExplosion) g_pVertexShaderExplosion->Release();
   if (g_pPixelShaderExplosion) g_pPixelShaderExplosion->Release();
   if (g_pExplosionGeometryShader ) g_pExplosionGeometryShader->Release();
   if (g_pExplosionLayout ) g_pExplosionLayout->Release();

   if( g_pDepthStencil ) g_pDepthStencil->Release();
   if( g_pDepthStencilView ) g_pDepthStencilView->Release();
	
   if( g_pRenderTargetView ) g_pRenderTargetView->Release();
   if( g_pSwapChain ) g_pSwapChain->Release();
   if( g_pImmediateContext ) g_pImmediateContext->ClearState();
   if( g_pImmediateContext1 ) g_pImmediateContext1->Release();
   if( g_pImmediateContext ) g_pImmediateContext->Release();


   if( g_pd3dDevice1 ) g_pd3dDevice1->Release();
   if( g_pd3dDevice ) g_pd3dDevice->Release();
	
}




DWORD WriteLogFile(char* pLogText)
{
   DWORD NumberOfBytesWritten;
   WriteFile(g_hLogFile, pLogText, strlen(pLogText),
             &NumberOfBytesWritten,NULL);
   return(NumberOfBytesWritten);
}

DWORD WriteLogFile2(wchar_t* pLogText)
{
   DWORD NumberOfBytesWritten;
   WriteFile(g_hLogFile, reinterpret_cast<const void*>(pLogText), lstrlen(pLogText) * sizeof(wchar_t),
             &NumberOfBytesWritten,NULL);
   return(NumberOfBytesWritten);
}


// -- Ersatz fürs strcat
//
HRESULT PSXStrCat(int l_idxLine, char* pszDest, size_t cbDest, const char* pszSrc )
{
   unsigned int l_cntDest, l_cntSrc;

   l_cntDest = strlen((const char*)pszDest) + 1; 
   l_cntSrc  = strlen((const char*)pszSrc) + 1;

   if (l_cntDest + l_cntSrc <= cbDest)
   {
      strcat_s(pszDest, cbDest, (const char*) pszSrc);
      return 0;
   }else{
      char l_cLineNo[100];
      sprintf_s(l_cLineNo, "Strcat-Buffer Overflow in Line %i\n", l_idxLine);
      WriteLogFile(l_cLineNo);
      return -1;
   }
}
// -- Ersatz fürs strcat
//
HRESULT PSXStrCat2(int l_idxLine, wchar_t* pszDest, size_t cbDest, const wchar_t* pszSrc )
{
   unsigned int l_cntDest, l_cntSrc;

   l_cntDest = wcslen((const wchar_t*)pszDest) + 1; 
   l_cntSrc  = wcslen((const wchar_t*)pszSrc) + 1;

   if (l_cntDest + l_cntSrc <= cbDest)
   {
	   wcscat_s(pszDest, cbDest, (const wchar_t*) pszSrc);
      return 0;
   }else{
      char l_cLineNo[100];
      sprintf_s(l_cLineNo, "Strcat-Buffer Overflow in Line %i\n", l_idxLine);
      WriteLogFile(l_cLineNo);
      return -1;
   }
}

// -- Ersatz fürs strcpy
//
HRESULT PSXStrCpy(int l_idxLine, char* pszDest, size_t cchDest, const char* pszSrc )
{
   if (strlen(pszSrc) + 1 <= cchDest)
   {
      strcpy_s(pszDest, cchDest, pszSrc);
      return 0;
   }else{
      char l_cLineNo[100];
      sprintf_s(l_cLineNo, "Strcpy-Buffer Overflow in Line %i\n", l_idxLine);
      WriteLogFile(l_cLineNo);
      return -1;
   }
}
// -- Ersatz fürs strcpy
//
HRESULT PSXStrCpy2(int l_idxLine, wchar_t* pszDest, size_t cchDest, const wchar_t* pszSrc )
{
	if (wcslen(pszSrc) + 1 <= cchDest)
   {
	  wcscpy_s(pszDest, cchDest, pszSrc);
      return 0;
   }else{
      char l_cLineNo[100];
      sprintf_s(l_cLineNo, "Strcpy-Buffer Overflow in Line %i\n", l_idxLine);
      WriteLogFile(l_cLineNo);
      return -1;
   }
}
// -- Ersatz fürs sprintf
//
HRESULT PSXPrintf(int l_idxLine, char* pszDest, size_t cchDest, const char* pszFormat, ...  )
{
   int l_iResult;
 
   va_list p;

   va_start(p, pszFormat);

   if (static_cast<size_t>(_vscprintf(pszFormat, p) +1) < cchDest)
   {
      vsprintf_s(pszDest, cchDest, pszFormat, p);
      l_iResult = 0;
   }else{
      char l_cLineNo[100];
      sprintf_s(l_cLineNo, "printf-Buffer Overflow in Line %i\n", l_idxLine);
      WriteLogFile(l_cLineNo);
      l_iResult = -1;
   }
   va_end(p);
   return l_iResult;
}

//
// -- Ersatz fürs sprintf
//
HRESULT PSXPrintf2(int l_idxLine, wchar_t* pszDest, size_t cchDest, const wchar_t* pszFormat, ...  )
{
   int l_iResult;

   va_list p;

   cchDest = cchDest / sizeof(wchar_t);
   va_start(p, pszFormat);
   if (static_cast<size_t>(_vcwprintf(pszFormat, p) +1) < cchDest)  
   {
       
       _vsnwprintf_s(pszDest,cchDest, cchDest-1, pszFormat, p);
           
       l_iResult = 0;
   }else{
      char l_cLineNo[100];
      sprintf_s(l_cLineNo, "printf-Buffer Overflow in Line %i\n", l_idxLine);
      WriteLogFile(l_cLineNo);
      l_iResult = -1;
   }
   va_end(p);
   return l_iResult;
}


/*
 *  This is a useful win32 error handler
 */
void hr2message(int line_no,HRESULT hresult, char* actioninfotext){
   char errortext[800];

   sprintf_s(errortext, "Call:\t%s\n\n"
                      "LineNo:      \t%i\n"
                      "hResult: \t %i \n"
                      "hResult hex: \t%lx \n",
                     actioninfotext, 
                     line_no,
                     hresult,
                     hresult
                     );

   switch (hresult)
   {
   case DXGI_ERROR_INVALID_CALL: PSXStrCat(__LINE__,errortext, sizeof(errortext), "DXGI_ERROR_INVALID_CALL");
       break;
   case E_OUTOFMEMORY: PSXStrCat(__LINE__,errortext, sizeof(errortext), "D3DERR_OUTOFMEMORY");
       break;
   }


   {
      DWORD NumberOfBytesWritten;
      WriteFile(g_hLogFile, errortext, strlen(errortext),
                &NumberOfBytesWritten,NULL);
   }
                
   CleanupDevice();
   MessageBoxA(g_hWnd, errortext, actioninfotext, MB_ICONEXCLAMATION);
   
   //DestroyWindow(msgboxwnd);
   /*
      printf("%s \n", actioninfotext);
      printf("%s \n", errortext);
   */

};

// --- VertexbufferCreation
//
bool psx_create_vertex_buffer(char* caller_content, ID3D11Buffer** lplpVertBuf, PSXVERTEX* vertices, int l_cntVert)
{
   HRESULT hr;
   char lokal_error_message[100];

   D3D11_BUFFER_DESC bd;
   ZeroMemory( &bd, sizeof(bd) );
   bd.Usage = D3D11_USAGE_DEFAULT;
   bd.ByteWidth = sizeof( vertices[0]) * l_cntVert;
   bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
   bd.CPUAccessFlags = 0;

   D3D11_SUBRESOURCE_DATA InitData;
   ZeroMemory( &InitData, sizeof(InitData) );
   InitData.pSysMem = vertices;

   hr = g_pd3dDevice->CreateBuffer( &bd, &InitData, lplpVertBuf );

   if( FAILED( hr ) ) 
   {
      // --- cat Errormessage
      // 
      PSXPrintf(__LINE__,lokal_error_message, sizeof(lokal_error_message), 
              "g_pd3dDevice->CreateBuffer(%s)", caller_content);

      hr2message(__LINE__,hr,lokal_error_message);
      return false;
   }
   return true;
}

bool PSXDrawResource(RESTYPE idx_resourceoffset, bool l_bTextured)
{

	/*D3DCOLORVALUE l_d3dcolDiffuse,
		l_d3dcolAmbient,
		l_d3dcolSpecular,
		l_d3dcolEmissive;*/

	g_pImmediateContext->DrawIndexed(g_meshresManager.getFaceCount(RESTYPE_LEADER)*3,0, 0);
	/*
	if (l_bTextured)
	{
		g_pImmediateContext->DrawIndexed(g_meshresManager.getFaceCount(RESTYPE_LEADER)*3,0, 0);
	}else{
		int l_idxStartFace;
		PSXMATERIAL* l_materArray;
		int l_cntMat;

		// -- setting color foreach material
		//
		l_materArray = g_meshresManager.getMaterialArray(idx_resourceoffset);
		l_cntMat = g_meshresManager.getMaterialCount(idx_resourceoffset);
		l_idxStartFace = 0;

		for (int idxMat = 0; idxMat < l_cntMat; idxMat ++)
		{
			cbChangeEveryObject cb1;
				
			// --- specifiying the material
			//
			cb1.mWorld = XMMatrixTranspose( g_World );
			cb1.matWorldViewProj= XMMatrixTranspose( g_World * g_View * g_Projection );
			cb1.d3dMaterial.Ambient = XMFLOAT4( l_materArray[idxMat].ambient_r, l_materArray[idxMat].ambient_g, l_materArray[idxMat].ambient_b,1.0f);
			cb1.d3dMaterial.Diffuse = XMFLOAT4(l_materArray[idxMat].diffuse_r, l_materArray[idxMat].diffuse_g, l_materArray[idxMat].diffuse_b,1.0f);
			cb1.d3dMaterial.Reflect = XMFLOAT4(0, 0, 0, 0);
			cb1.d3dMaterial.Specular = XMFLOAT4(l_materArray[idxMat].specular_r, l_materArray[idxMat].specular_g, l_materArray[idxMat].specular_b, 1.0f);
			g_pImmediateContext->UpdateSubresource(g_pcbChangeEveryObject, 0, nullptr, &cb1, 0, 0 );	 

			g_pImmediateContext->DrawIndexed(l_materArray[idxMat].cnt_faces*3,l_idxStartFace*3, 0);
		}

	}
	*/
	return true;
}


void PSXErrorHandlerForODE (int num, const char *msg, va_list ap)
{
  char l_cTitle[100], l_cMessage[500];

/*
  PSXLeaveFullScreen();
*/

  va_start (ap,msg);
    _snprintf_s (l_cTitle,sizeof(l_cTitle),"ODE Error %d",num);
    _vsnprintf_s (l_cMessage,sizeof(l_cMessage),msg,ap);
    l_cMessage[sizeof(l_cMessage)-1] = 0;
    WriteLogFile(l_cTitle);
    WriteLogFile(":");
    WriteLogFile(l_cMessage);
    WriteLogFile("\n");
    MessageBoxA(g_hWnd, l_cMessage,l_cTitle,MB_OK | MB_ICONWARNING);
  va_end(ap);

  //g_bKeepInLoop = false;
}

void PSXMessageHandlerForODE (int num, const char *msg, va_list ap)
{
  char l_cMessage[500];

  va_start (ap,msg);
    //_snprintf (l_cTitle,sizeof(l_cTitle),"ODE Message %d",num);
    _vsnprintf_s (l_cMessage,sizeof(l_cMessage),msg,ap);
    l_cMessage[sizeof(l_cMessage)-1] = 0;
    //WriteLogFile(l_cTitle);
    WriteLogFile("ODE:");
    WriteLogFile(l_cMessage);

    if (num)
    {
       ENEMY* l_pEnemy;
	   dGeomID l_dgeomidErrorGeom = dSpaceGetGeom(LEVEL::m_dSpaceID, num);
       l_pEnemy = reinterpret_cast<ENEMY*>(dGeomGetData(l_dgeomidErrorGeom));
	   /*
       WriteLogFile(" in ");
	   
       WriteLogFile(g_cEnemyTypeNameArray[l_pEnemy->getTypeID()]);
	   */
    }
    WriteLogFile("\n");
  va_end(ap);

}

void PSXDebugHandlerForODE (int num, const char *msg, va_list ap)
{
  char l_cTitle[100], l_cMessage[500];

  va_start (ap,msg);
    _snprintf_s (l_cTitle,sizeof(l_cTitle),"ODE Debug Message %d",num);
    _vsnprintf_s (l_cMessage,sizeof(l_cMessage),msg,ap);
    l_cMessage[sizeof(l_cMessage)-1] = 0;
    WriteLogFile(l_cTitle);
    WriteLogFile(":");
    WriteLogFile(l_cMessage);
    WriteLogFile("\n");
  va_end(ap);

}
