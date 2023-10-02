#pragma once
#include"Render/DrawFunc.h"
#include"../KazLibrary/Render/DrawFuncHelper.h"
#include"Render/DrawingByRasterize.h"
#include"Loader/ModelLoader.h"
#include"Buffer/GBufferMgr.h"

namespace BasicDraw
{
	DrawFuncData::DrawCallData SetModel(const std::shared_ptr<ModelInfomation>& arg_modelInfomation);
	DrawFuncData::DrawCallData SetTex();

	struct BasicModelRender
	{
		BasicModelRender(DrawingByRasterize& arg_rasterize, const std::string& arg_fileDir, const std::string& arg_fileName);
		BasicModelRender(DrawingByRasterize& arg_rasterize);
		BasicModelRender();
		void Load(DrawingByRasterize& arg_rasterize, const std::string& arg_fileDir, const std::string& arg_fileName);
		DrawFuncHelper::ModelRender m_model;
	};
	struct BasicTextureRender
	{
		BasicTextureRender(DrawingByRasterize& arg_rasterize,const std::string& arg_filePass, bool arg_isUIFlag = false);
		BasicTextureRender(DrawingByRasterize& arg_rasterize, bool arg_isUIFlag = false);
		DrawFuncHelper::TextureRender m_tex;
	};

};