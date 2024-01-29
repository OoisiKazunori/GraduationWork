#pragma once
#include"Render/DrawFunc.h"
#include"../KazLibrary/Render/DrawFuncHelper.h"
#include"Render/DrawingByRasterize.h"
#include"Loader/ModelLoader.h"
#include"Buffer/GBufferMgr.h"
#include"Helper/Compute.h"

namespace BasicDraw
{
	DrawFuncData::DrawCallData SetModel(const std::shared_ptr<ModelInfomation>& arg_modelInfomation);
	DrawFuncData::DrawCallData SetInstanceModel(const std::shared_ptr<ModelInfomation>& arg_modelInfomation);
	DrawFuncData::DrawCallData SetTex();

	struct BasicModelRender
	{
<<<<<<< HEAD
		BasicModelRender(DrawingByRasterize &arg_rasterize, const std::string &arg_fileDir, const std::string &arg_fileName, bool arg_deletePipelineInScene = false);
		BasicModelRender(DrawingByRasterize &arg_rasterize, const std::shared_ptr<ModelInfomation>& arg_modelInfomation, const DrawFuncData::DrawCallData& arg_drawCall, bool arg_deletePipelineInScene = false);
		BasicModelRender(DrawingByRasterize &arg_rasterize, bool arg_deletePipelineInScene = false);
=======
		BasicModelRender(DrawingByRasterize& arg_rasterize, const std::string& arg_fileDir, const std::string& arg_fileName, bool arg_deletePipelineInScene = false);
		BasicModelRender(DrawingByRasterize& arg_rasterize, bool arg_deletePipelineInScene = false);
>>>>>>> f0b3d7a285d53d36e82bd80fb5a13da810360779
		BasicModelRender();
		void Load(DrawingByRasterize& arg_rasterize, const std::string& arg_fileDir, const std::string& arg_fileName);
		void Load(DrawingByRasterize& arg_rasterize, const std::shared_ptr<ModelInfomation>& arg_modelInfomation, const DrawFuncData::DrawCallData& arg_drawCall);
		DrawFuncHelper::ModelRender m_model;
	};

	struct BasicModelInstanceRender
	{
		BasicModelInstanceRender(DrawingByRasterize& arg_rasterize, const std::string& arg_fileDir, const std::string& arg_fileName, bool arg_deletePipelineInScene = false);
		BasicModelInstanceRender(DrawingByRasterize& arg_rasterize, bool arg_deletePipelineInScene = false);
		BasicModelInstanceRender();
		void Load(DrawingByRasterize& arg_rasterize, const std::string& arg_fileDir, const std::string& arg_fileName);
		void Load(DrawingByRasterize& arg_rasterize, const std::shared_ptr<ModelInfomation>& arg_modelInfomation, const DrawFuncData::DrawCallData& arg_drawCall);

		void UploadTransformMatrix(std::vector<KazMath::Transform3D>arg_transform);
		void UploadColor(std::vector<KazMath::Color>arg_color);
		void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blas);
		DrawFuncHelper::ModelRender m_model;

		KazBufferHelper::BufferData m_uploadTransformBuffer, m_vramTransformBuffer;
		KazBufferHelper::BufferData m_uploadColorBuffer, m_vramColorBuffer;
		void GenerateTransformBuffer(int arg_elementNum);

		ComputeShader m_compute;
		UINT m_dispatchNum;
	};

	struct BasicTextureRender
	{
		BasicTextureRender(DrawingByRasterize& arg_rasterize, const std::string& arg_filePass, bool arg_isUIFlag = false, bool arg_deletePipelineInScene = false);
		BasicTextureRender(DrawingByRasterize& arg_rasterize, const char* arg_filePass, bool arg_isUIFlag = false, bool arg_deletePipelineInScene = false);
		BasicTextureRender(DrawingByRasterize& arg_rasterize, bool arg_isUIFlag = false, bool arg_deletePipelineInScene = false);
		BasicTextureRender(DrawingByRasterize& arg_rasterize, const DrawFuncData::DrawCallData& arg_drawCall, bool arg_isUIFlag = false, bool arg_deletePipelineInScene = false);
		BasicTextureRender();
		DrawFuncHelper::TextureRender m_tex;
	};

	struct BasicLineRender
	{
		BasicLineRender(DrawingByRasterize& arg_rasterize);
		BasicLineRender();
		void Generate(DrawingByRasterize& arg_rasterize);
		DrawFuncHelper::LineRender m_render;
	};

	class SilhouetteModelRender
	{
	public:
		SilhouetteModelRender(DrawingByRasterize& arg_rasterize, const std::string& arg_fileDir, const std::string& arg_fileName, bool arg_deletePipelineInScene = false);
		SilhouetteModelRender();
		void Load(DrawingByRasterize& arg_rasterize, const std::string& arg_fileDir, const std::string& arg_fileName);

		void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec, KazMath::Transform3D& arg_trasform3D, const KazMath::Color& arg_addColor = KazMath::Color(255, 255, 255, 255), float arg_timeScale = 1.0f);
	private:
		DrawFuncHelper::ModelRender m_modelSilhouette;
	};

};