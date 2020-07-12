#ifndef PSXTYPES_H
#define PSXTYPES_H


typedef struct _psxvertex {
   float x;
   float y;
   float z;
   float nx;
   float ny;
   float nz;
   float tu;
   float tv;
} PSXVERTEX;

typedef struct _SFVERTEX { 
        float x; 
        float y; 
        //float z; 
		float w;
		float h;

		float opacity;
        //DWORD diffuse; 
        //DWORD specular; 
} SFVERTEX;


typedef struct _psxvector {
    float x,y,z;
} PSXVECTOR; 

typedef struct _psxface {
    __int32 a;
    __int32 b;
    __int32 c;
} PSXFACE;

typedef struct _faceinfo {
   float     fDot; 
   PSXVECTOR vNormal;
   PSXVECTOR vCentroid;
   __int32       filler;
   float     fRadPow2;
} PSXFACEINFO;


struct PSXSHORTFACE
{ 
   short a,b,c;
};


typedef struct _psxmaterial {
    float emissive_r;
    float emissive_g;
    float emissive_b;

    float ambient_r;
    float ambient_g;
    float ambient_b;

    float diffuse_r;
    float diffuse_g;
    float diffuse_b;

    float specular_r;
    float specular_g;
    float specular_b;
    
    int cnt_faces;

} PSXMATERIAL;

typedef struct _psxresourceoffset {
    __int32 vertex_offset;
    __int32 cnt_vert;
    __int32 face_offset;
    __int32 cnt_faces;
    __int32 material_offset;
    __int32 cnt_material;
    PSXVECTOR min;
    PSXVECTOR max;
    float rad;
} PSXRESOURCEOFFSET;

typedef struct _cullinfo {
   float     fDot; 
   PSXVECTOR vNormal;
} PSXCULLINFO;

#endif