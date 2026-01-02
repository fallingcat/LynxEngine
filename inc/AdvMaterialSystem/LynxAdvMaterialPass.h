//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/ 
// 
//  LynxEngine Ver 1.00 
//  Copyright (C) 2003 fallingCAT studios.  All Rights Reserved.
//
//
//  Created by Owen Wu : 2005/11/02
//  Last Update : 
//--------------------------------------------------------------------------
//  說明:
//
//###########################################################################

#ifndef __LYNXADVMATERIALPASS_H__
#define __LYNXADVMATERIALPASS_H__

#include <LynxSDK.h>
#include <LynxBaseObj.h>
#include <LynxArray.h>
#include <LynxSystem.h>
#include <LynxParam.h>

namespace LynxEngine 
{	
	namespace AdvMaterialSystem 
	{		
		class CAdvMaterialSystem;		
		class CAdvMaterial;		
		class LYNXENGCLASS CAdvMaterialPass : public CBaseObj, public LYNXMATERIALPASS
		{	
			LYNX_DECLARE_CLASS(CAdvMaterialPass);
		public:
			#define SET_VS_PARAMETER(s, v, n)							lynxSetVertexShaderConstantTable(s, (#v), &v, n)
			#define SET_PS_PARAMETER(s, v, n)							lynxSetPixelShaderConstantTable(s, (#v), &v, n)							
		protected:			
			CAdvMaterial*							m_lpMaterial;	
			CArray<CParam>							m_ParamArray;
		public:			
			CAdvMaterialPass();
			CAdvMaterialPass(CAdvMaterial* lpm);
			CAdvMaterialPass(LYNXMATERIALPASS&);
			virtual ~CAdvMaterialPass();				

			CAdvMaterialPass&						operator =(const CAdvMaterialPass& m);
			CAdvMaterialPass&						operator =(const LYNXMATERIALPASS& mp);
			
			virtual	void							vInit();	
			virtual void							vLoad(LPLYNXFILE lpf) {};
			virtual void							vPlayAll(LYNXREAL skip, LYNXPLAYMODE mode);
			virtual LYNXBOOL						vPlay(int channel, LYNXREAL skip, LYNXPLAYMODE mode);
			virtual void							vRelease(void);
			void									CreateTextureFrame(int channel, int num);
			void									LoadTexture(int channel, int frame, const char *filename);
			LYNXINLINE void							SetlpMaterial(CAdvMaterial *lpmat) {m_lpMaterial = lpmat; };
			LYNXINLINE CAdvMaterial* const			GetlpMaterial() {return m_lpMaterial; };
			virtual void							vSetParameter(void);
			void									Set(void);			
			LYNXINLINE void							SetBlendFunc(LYNXBLENDFUNC src, LYNXBLENDFUNC dest) {SrcBlend = src; DestBlend = dest; };
			LYNXINLINE void							SetBlendMode(LYNXBLENDMODE m) {lynxSetMaterialPassBlendMode((LPLYNXMATERIALPASS)this, m); };
			LYNXINLINE void							SetCombineFunc(int channel, LYNXCOMBINEFUNC func) {CombineFunc[channel] = func;};
			LYNXINLINE void							SetCombineArg(int channel, int argno, LYNXCOMBINEARG arg) {CombineArg[channel][argno] = arg;};
			LYNXINLINE LPLYNXTEXTUREFRAME			GetlpTextureFrame(int i) {return &TextureFrame[i]; };
			LYNXINLINE void							SetFixeVertexShader(LYNXFIXEDVERTEXSHADER fs) {FixedVS = fs;};
			LYNXINLINE LYNXVERTEXLAYOUT&			GetVertexLayout() {return VertexLayout; };
			LYNXINLINE void							SetVertexLayout(LYNXVERTEXLAYOUT& lo) {VertexLayout = lo; };
			LYNXINLINE LYNXVERTEXSHADERINDEX		GetVS() {return VS; };
			LYNXINLINE void							SetVS(LYNXVERTEXSHADERINDEX vs) {VS = vs; };
			LYNXINLINE LYNXPIXELSHADERINDEX			GetPS() {return PS; };
			LYNXINLINE void							SetPS(LYNXPIXELSHADERINDEX ps) {PS = ps; };
			LYNXINLINE void							LoadVS(const char *name, LPLYNXFILE lpfs, long offset) {lynxLoadCompiledVertexShader(&VS, name, lpfs, offset);};						
			LYNXINLINE void							LoadPS(const char *name, LPLYNXFILE lpfs, long offset) {lynxLoadCompiledPixelShader(&PS, name, lpfs, offset);};						
			LYNXINLINE void							SetVSParamMatrix(const char *name, LPLYNXMATRIX lpm) {lynxSetVertexShaderConstantTableMatrix(VS, name, lpm); };
			LYNXINLINE void							SetVSParamFloat(const char *name, float *lpf, int num) {lynxSetVertexShaderConstantTable(VS, name, lpf, num); };
			LYNXINLINE void							SetVSParamBool(const char *name, BOOL *lpb, int num) {lynxSetVertexShaderConstantTableB(VS, name, lpb, num); };	

			friend class CAdvMaterial;
		};		
	}
	namespace AMS = AdvMaterialSystem;
}

#endif