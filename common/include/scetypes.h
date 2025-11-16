#ifndef _TYPES_H
#define _TYPES_H

typedef signed char SceChar8;
typedef unsigned char SceUChar8;
typedef signed char SceInt8;
typedef unsigned char SceUInt8;
typedef signed short SceShort16;
typedef unsigned short SceUShort16;
typedef short SceInt16;
typedef unsigned short SceUInt16;
typedef signed int SceInt32;
typedef unsigned int SceUInt32;
typedef int SceInt;
typedef unsigned int SceUInt;
typedef signed long long SceInt64;
typedef unsigned long long SceUInt64;
typedef long SceLong;
typedef unsigned long SceULong;
typedef signed long long SceLong64;
typedef unsigned long long SceULong64;
typedef float SceFloat;
typedef float SceFloat32;
typedef double SceDouble;
typedef double SceDouble64;
typedef signed char SceSByte;
typedef signed char SceSByte8;
typedef unsigned char SceByte;
typedef unsigned char SceByte8;
typedef unsigned short SceWChar16;
typedef unsigned int SceWChar32;

#ifdef __cplusplus
#define SCE_NULL (0)
#else
#define SCE_NULL ((void *)0)
#endif

typedef int SceBool;

#define SCE_FALSE (0)
#define SCE_TRUE (1)

typedef int SceIntPtr;
typedef unsigned int SceUIntPtr;
typedef void SceVoid;
typedef void *ScePVoid;

typedef struct SceIVector2 {
  int x, y;
} SceIVector2;

typedef struct SceFVector2 {
  float x, y;
} SceFVector2;

typedef union SceUVector2 {
  SceIVector2 iv;
  SceFVector2 fv;
} SceUVector2;

typedef struct SceIVector3 {
  int x, y, z;
} SceIVector3;

typedef struct SceFVector3 {
  float x, y, z;
} SceFVector3;

typedef union SceUVector3 {
  SceIVector3 iv;
  SceFVector3 fv;
} SceUVector3;

typedef struct SceIVector4 {
  int x, y, z, w;
} SceIVector4;

typedef struct SceFVector4 {
  float x, y, z, w;
} SceFVector4;

typedef union SceUVector4 {
  SceIVector4 iv;
  SceFVector4 fv;
} SceUVector4;

typedef struct SceIMatrix2 {
  SceIVector2 x, y;
} SceIMatrix2;

typedef struct SceFMatrix2 {
  SceFVector2 x, y;
} SceFMatrix2;

typedef union SceUMatrix2 {
  SceFMatrix2 fm;
  SceIMatrix2 im;
  SceFVector2 fv[2];
  SceIVector2 iv[2];
  SceUVector2 uv[2];
  float f[2][2];
  int i[2][2];
} SceUMatrix2;

typedef struct SceIMatrix3 {
  SceIVector3 x, y, z;
} SceIMatrix3;

typedef struct SceFMatrix3 {
  SceFVector3 x, y, z;
} SceFMatrix3;

typedef union SceUMatrix3 {
  SceFMatrix3 fm;
  SceIMatrix3 im;
  SceFVector3 fv[3];
  SceIVector3 iv[3];
  SceUVector3 uv[3];
  float f[3][3];
  int i[3][3];
} SceUMatrix3;

typedef struct SceIMatrix4 {
  SceIVector4 x, y, z, w;
} SceIMatrix4;

typedef struct SceFMatrix4 {
  SceFVector4 x, y, z, w;
} SceFMatrix4;

typedef union SceUMatrix4 {
  SceFMatrix4 fm;
  SceIMatrix4 im;
  SceFVector4 fv[4];
  SceIVector4 iv[4];
  SceUVector4 uv[4];
  float f[4][4];
  int i[4][4];
} SceUMatrix4;

typedef struct SceFQuaternion {
  float x, y, z, w;
} SceFQuaternion;

typedef struct SceFColor {
  float r, g, b, a;
} SceFColor;

typedef struct SceFPlane {
  float a, b, c, d;
} SceFPlane;

typedef union SceUnion32 {
  unsigned int ui;
  int i;
  unsigned short us[2];
  short s[2];
  unsigned char uc[4];
  char c[4];
  float f;
  void *p;
} SceUnion32;

typedef union SceUnion64 {
  SceULong64 ull;
  SceLong64 ll;
  unsigned int ui[2];
  int i[2];
  unsigned short us[4];
  short s[4];
  unsigned char uc[8];
  char c[8];
  float f[2];

  SceFVector2 fv;
  SceIVector2 iv;
} SceUnion64;

typedef union SceUnion128 {

  SceULong64 ull[2];
  SceLong64 ll[2];
  unsigned int ui[4];
  int i[4];
  unsigned short us[8];
  short s[8];
  unsigned char uc[16];
  char c[16];
  float f[4];

  SceFVector4 fv;
  SceFQuaternion fq;
  SceFPlane fp;
  SceFColor fc;
  SceIVector4 iv;
} SceUnion128;

typedef union SceUnion512 {
  SceUnion32 un32[16];
  SceUnion64 un64[8];

  SceULong64 ull[8];
  SceLong64 ll[8];
  unsigned int ui[16];
  int i[16];
  unsigned short us[32];
  short s[32];
  unsigned char uc[64];
  char c[64];
  float f[16];

  SceFMatrix4 fm;
  SceIMatrix4 im;
  SceUMatrix4 um;
} SceUnion512;

typedef struct SceDateTime {
  unsigned short year;
  unsigned short month;
  unsigned short day;
  unsigned short hour;
  unsigned short minute;
  unsigned short second;
  unsigned int microsecond;
} SceDateTime;

typedef unsigned int SceSize;
typedef SceSize ScePSize;
typedef SceSize SceVSize;
typedef int SceSSize;
typedef SceSSize ScePSSize;
typedef SceSSize SceVSSize;
typedef unsigned int SceUIntVAddr;
typedef unsigned int SceUIntPAddr;

typedef int SceUID;

#ifdef __cplusplus
#define SCE_UID_INVALID_UID (static_cast<SceUID>(0xFFFFFFFF))
#else
#define SCE_UID_INVALID_UID ((SceUID)0xFFFFFFFF)
#endif

/** Object name */
typedef char *SceName;

typedef SceInt64 SceOff;
typedef SceUID ScePID;

#endif /* _TYPES_H */
