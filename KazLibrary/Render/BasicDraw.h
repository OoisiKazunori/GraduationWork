#pragma once
#include"Render/DrawFunc.h"
#include"../KazLibrary/Render/DrawFuncHelper.h"
#include"Render/DrawingByRasterize.h"
#include"Loader/ModelLoader.h"
#include"Buffer/GBufferMgr.h"

namespace BasicDraw
{
	DrawFuncData::DrawCallData SetModel(const std::shared_ptr<ModelInfomation> &arg_modelInfomation);
	DrawFuncData::DrawCallData SetTex();

	struct BasicModelRender
	{
		BasicModelRender(DrawingByRasterize &arg_rasterize, const std::string &arg_fileDir, const std::string &arg_fileName, bool arg_deletePipelineInScene = false);
		BasicModelRender(DrawingByRasterize &arg_rasterize, const std::shared_ptr<ModelInfomation>& arg_modelInfomation, const DrawFuncData::DrawCallData& arg_drawCall, bool arg_deletePipelineInScene = false);
		BasicModelRender(DrawingByRasterize &arg_rasterize, bool arg_deletePipelineInScene = false);
		BasicModelRender();
		void Load(DrawingByRasterize &arg_rasterize, const std::string &arg_fileDir, const std::string &arg_fileName);
		void Load(DrawingByRasterize &arg_rasterize, const std::shared_ptr<ModelInfomation> &arg_modelInfomation, const DrawFuncData::DrawCallData &arg_drawCall);
		DrawFuncHelper::ModelRender m_model;
	};
	struct BasicTextureRender
	{
		BasicTextureRender(DrawingByRasterize &arg_rasterize, const std::string &arg_filePass, bool arg_isUIFlag = false, bool arg_deletePipelineInScene = false);
		BasicTextureRender(DrawingByRasterize &arg_rasterize, const char *arg_filePass, bool arg_isUIFlag = false, bool arg_deletePipelineInScene = false);
		BasicTextureRender(DrawingByRasterize &arg_rasterize, bool arg_isUIFlag = false, bool arg_deletePipelineInScene = false);
		BasicTextureRender(DrawingByRasterize &arg_rasterize, const DrawFuncData::DrawCallData &arg_drawCall, bool arg_isUIFlag = false, bool arg_deletePipelineInScene = false);
		BasicTextureRender();
		DrawFuncHelper::TextureRender m_tex;
	};

	struct BasicLineRender
	{
		BasicLineRender(DrawingByRasterize &arg_rasterize);
		BasicLineRender();
		void Generate(DrawingByRasterize& arg_rasterize);
		DrawFuncHelper::LineRender m_render;
	};

	class SilhouetteModelRender
	{
	public:
		SilhouetteModelRender(DrawingByRasterize &arg_rasterize, const std::string &arg_fileDir, const std::string &arg_fileName, bool arg_deletePipelineInScene = false);
		SilhouetteModelRender();
		void Load(DrawingByRasterize &arg_rasterize, const std::string &arg_fileDir, const std::string &arg_fileName);

		void Draw(DrawingByRasterize &arg_rasterize, Raytracing::BlasVector &arg_blasVec, KazMath::Transform3D &arg_trasform3D, const KazMath::Color &arg_addColor = KazMath::Color(255, 255, 255, 255), float arg_timeScale = 1.0f);
	private:
		DrawFuncHelper::ModelRender m_modelSilhouette;
	};

};