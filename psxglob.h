using namespace DirectX;
//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
extern HWND                    g_hWnd;
extern D3D_DRIVER_TYPE         g_driverType;
extern D3D_FEATURE_LEVEL       g_featureLevel ;
extern ID3D11Device*           g_pd3dDevice ;
extern ID3D11Device1*          g_pd3dDevice1 ;
extern ID3D11DeviceContext*    g_pImmediateContext ;
extern ID3D11DeviceContext1*   g_pImmediateContext1 ;
extern IDXGISwapChain*         g_pSwapChain ;
extern ID3D11RenderTargetView* g_pRenderTargetView ;
extern ID3D11Texture2D*        g_pDepthStencil ;
extern ID3D11DepthStencilView* g_pDepthStencilView ;

extern ID3D11PixelShader*      g_pPixelShaderSolid ;
extern ID3D11InputLayout*      g_pVertexLayout ;

extern ID3D11InputLayout*      g_pSpriteLayout ;
extern ID3D11Buffer*           g_pSpriteBuffer;

extern ID3D11InputLayout*      g_pShootPosLayout ;
extern ID3D11Buffer*           g_pShootPosBuffer;
//extern ID3D11Buffer*           g_pVertexBuffer ;
//extern ID3D11Buffer*           g_pIndexBuffer ;
//extern ID3D11ShaderResourceView*           g_pTextureRV ;
extern ID3D11VertexShader*     g_pVertexShaderEnemyDrawer;
extern ID3D11PixelShader*      g_pPixelShaderEnemyDrawer;
extern ID3D11VertexShader*     g_pVertexShaderSimpleDrawer;
extern ID3D11PixelShader*      g_pPixelShaderSimpleDrawer;
extern ID3D11VertexShader*     g_pVertexShaderBumpDrawer;
extern ID3D11PixelShader*      g_pPixelShaderBumpDrawer;

extern ID3D11VertexShader*     g_pVertexShaderSpriteDrawer;
extern ID3D11PixelShader*      g_pPixelShaderSpriteDrawer;
extern ID3D11GeometryShader*   g_pSpriteGeometryShader;

extern ID3D11VertexShader*     g_pVertexShaderShootDrawer;
extern ID3D11PixelShader*      g_pPixelShaderShootDrawer;
extern ID3D11GeometryShader*   g_pShootGeometryShader;

extern ID3D11VertexShader*     g_pVertexShaderExplosion;
extern ID3D11PixelShader*      g_pPixelShaderExplosion;
extern ID3D11GeometryShader*   g_pExplosionGeometryShader ;
extern ID3D11InputLayout*      g_pExplosionLayout ;
extern ID3D11Buffer*           g_pExplosionVB ;

extern int g_cntExplVert;
extern float g_fTempScaleExplosion;
const int g_maxExplosionSteps = 100;
const int g_maxShieldFlashTimer = 10;

//extern ID3D11SamplerState*                 g_pSamplerLinear ;
extern ID3D11BlendState* g_pd3dblendstateTransparent;
extern ID3D11DepthStencilState *g_pd3ddepthstateTransparent;
extern XMMATRIX                g_World;
extern XMMATRIX                g_View;
extern XMMATRIX                g_Projection;
extern XMMATRIX g_matCamera;
extern XMMATRIX g_matPlayer;
extern XMMATRIX g_matSingleMovement;
extern XMVECTOR g_vCamera;
#define MAX_LOADSTRING 100
extern ENEMYHANDLINGBUFFER g_ehdlBuf;

// Globale Variablen:
extern HINSTANCE g_hInst ;
extern TCHAR szTitle[MAX_LOADSTRING];					// Titelleistentext
extern TCHAR szWindowClass[MAX_LOADSTRING];			// Klassenname des Hauptfensters


extern HANDLE g_hLogFile;

extern GAME g_game;

extern DWORD g_dwLastTickCount;
extern int g_cntFrame;
extern float g_fElapsedTime;

extern BIGRES g_bigresArray[];
extern RESASSIGN g_resassignArray[];

extern BIGRESITEM g_bigresitemArray[];
extern int g_cntBigResItem;

extern TEXTUREASSIGN g_txtassigncompressedArray[];
extern TEXTUREASSIGN* g_txtassignResStdArray;

extern int g_cntTxtAssignCompressed;

extern TEXTURE* g_txtArray;



extern MESHRESOURCEMANAGER g_meshresManager;
extern ID3D11RasterizerState* g_pD3DRasterizerState;

extern LPDIRECTINPUT8                   lpDirectInput;
extern LPDIRECTINPUTDEVICE8             g_didevMouse;
extern LPDIRECTINPUTDEVICE8             g_didevKeyboard;
// -- direct input records
//
extern DIMOUSESTATE                     diMouseState;
extern DIJOYSTATE2                      diJoystickState2;
extern char g_arr_cKeystate[];
extern int g_cntKeystateCheckTimer[];
extern int g_maxKeystateCheckTimer;

extern int g_cntMousestateCheckTimer[];
extern int g_maxMousestateCheckTimer;

extern ENEMY**    g_arr_pEnemy;
extern PSXFACEINFO** g_pfaceinfSelectedArray;
extern dContactGeom* g_contactgeomArray;

extern ENEMYDRAWER* g_arr_EnemyDrawer;


extern ENEMY* g_penemyCamera;
extern ENEMY* g_penemyInput;
extern ENEMY* g_penemyDebug;

extern bool g_bWriteLogFileInLoop;

extern PLAYER g_Player;

extern SHOOT* shoot_array;
extern int g_cntShoot;
/*extern int g_cntShootInserted;
extern int g_cntShootDeleted;*/
const int g_maxShoot = 1000;
const int g_maxSprite = 10;
//const int g_maxExplosion = 20;

extern char** g_cLang;

extern char* g_cLangEnglish[];
extern char* g_cLangGerman[];
extern char* g_cLangItalian[];
extern char* g_cLangPolish[];

extern char* g_cActionNameArray[];
extern char* g_cEnemyTypeNameArray[];

// -- Laser Spawn Punkte für ausgesuchte Restypes
//
extern PSXLASERSPAWN g_laserHunterXArray[];
const int g_cntLaserHunterX = 6;

extern PSXLASERSPAWN g_laserPoliceLeaderArray[];
const int g_cntLaserPoliceLeader = 3;

extern PSXLASERSPAWN g_laserPoliceFlyerArray[];
const int g_cntLaserPoliceFlyer = 2;

extern PSXLASERSPAWN g_laserGunArray[];
extern PSXLASERSPAWN g_laserGunArrayS[];
const int g_cntLaserGun = 3;

extern PSXLASERSPAWN g_laserWingmanArray[];
const int g_cntLaserWingman = 2;

// --- gameplay (score und shield )
//
extern EQUIPMENT g_equipmentArray[];

//-- 0 = Laser
//
extern EQUIPMENT* g_pequipmentSelectWeapon;
// -- Direct2D
//
extern ID2D1Factory1* g_pd2dFactory;
extern IDXGIDevice* g_pdxgiDevice;
extern ID2D1Device* g_pd2dDevice;
extern ID2D1DeviceContext* g_pd2dDeviceContext;
extern IDXGISurface* g_pdxgiSurface;
//extern ID2D1Bitmap1* g_pd2dBitmap;
extern ID2D1SolidColorBrush* g_pd2dSolidWhiteBrush;
extern IDWriteFactory* g_pdwriteFactory;
extern IDWriteTextFormat* g_pdwriteTextFormat;
extern D2D1_SIZE_U g_d2dsizeClientRect;
extern D2D1_RECT_F g_d2drectClientRect;
extern ID2D1RenderTarget* g_pd2dRenderTarget;
//extern ID2D1HwndRenderTarget* g_pd2dhwdRenderTarget;

extern char* debugausgabe;
const size_t g_maxDebugAusgabe = 800;

