#pragma once
#include"Render/DrawFunc.h"
#include"Render/DrawingByRasterize.h"
#include"Loader/ModelLoader.h"
#include"Raytracing/BlasVector.h"

namespace DrawFuncHelper
{
	//スプライト描画のラッパー
	struct TextureRender
	{
		DrawFuncData::DrawCallData m_drawCommand;
		const DrawFuncData::DrawData* m_drawCommandData;
		KazBufferHelper::BufferData m_textureBuffer;
		KazMath::Vec2<float> m_textureSize;

		/// <summary>
		/// テクスチャ読み込み
		/// </summary>
		/// <param name="arg_textureFilePass">テクスチャのファイルパス</param>
		/// <param name="arg_alphaFlag">透過有り無し</param>
		TextureRender(DrawingByRasterize& arg_rasterize, const std::string& arg_textureFilePass, bool arg_isUIFlag, bool arg_deletePipelineInScene = false);
		/// <summary>
		/// テクスチャ読み込みと描画情報の設定
		/// </summary>
		/// <param name="arg_textureFilePass">テクスチャのファイルパス</param>
		/// <param name="arg_drawCall">描画情報の生成</param>
		TextureRender(DrawingByRasterize& arg_rasterize, const std::string& arg_textureFilePass, const DrawFuncData::DrawCallData& arg_drawCall, bool arg_isUIFlag, bool arg_deletePipelineInScene = false);
		TextureRender(DrawingByRasterize& arg_rasterize, const DrawFuncData::DrawCallData& arg_drawCall, bool arg_isUIFlag, bool arg_deletePipelineInScene = false);
		TextureRender(DrawingByRasterize& arg_rasterize, bool arg_isUIFlag, bool arg_deletePipelineInScene = false);
		TextureRender();

		void Load(DrawingByRasterize& arg_rasterize, const std::string& arg_textureFilePass, bool arg_isUIFlag, bool arg_deletePipelineInScene = false);

		void operator=(const KazBufferHelper::BufferData& rhs);

		/// <summary>
		/// スプライト2D描画
		/// </summary>
		void Draw2D(DrawingByRasterize& arg_rasterize, const KazMath::Transform2D& arg_trasform2D, const KazMath::Color& arg_addColor = KazMath::Color(255, 255, 255, 255));
		void Draw2D(DrawingByRasterize& arg_rasterize, const KazMath::Transform2D& arg_trasform2D, const KazBufferHelper::BufferData& arg_textureBuffer, const KazMath::Color& arg_addColor = KazMath::Color(255, 255, 255, 255));
		/// <summary>
		/// スプライト3D描画
		/// </summary>
		void Draw3D(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec, const KazMath::Transform3D& arg_trasform3D, const KazMath::Color& arg_addColor = KazMath::Color(255, 255, 255, 255));
		void Draw3D(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec, const KazMath::Transform3D& arg_trasform3D, const KazBufferHelper::BufferData& arg_textureBuffer, const KazMath::Color& arg_addColor = KazMath::Color(255, 255, 255, 255));
		void Draw3DBillBoard(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec, const KazMath::Transform3D& arg_trasform3D, const KazMath::Color& arg_addColor = KazMath::Color(255, 255, 255, 255));
		void Draw3DBillBoard(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec, const KazMath::Transform3D& arg_trasform3D, const KazBufferHelper::BufferData& arg_textureBuffer, const KazMath::Color& arg_addColor = KazMath::Color(255, 255, 255, 255));

		void DrawGaussian(DrawingByRasterize& arg_rasterize, const KazMath::Transform2D& arg_trasform2D);

		void Error();
		void StackOnBlas(Raytracing::BlasVector& arg_blasVec, const DirectX::XMMATRIX& arg_worldMat);
	};

	struct TextureRectRender
	{
		TextureRectRender(DrawingByRasterize& arg_rasterize, const std::string& arg_textureFilePass, bool arg_isUIFlag, bool arg_deletePipelineInScene = false);
		TextureRectRender();

		void Load(DrawingByRasterize& arg_rasterize, bool arg_isUIFlag, bool arg_deletePipelineInScene = false);
		void Load(DrawingByRasterize& arg_rasterize, const DrawFuncData::DrawCallData& arg_drawCallData, bool arg_isUIFlag, bool arg_deletePipelineInScene = false);

		void Draw(DrawingByRasterize& arg_rasterize,
			Raytracing::BlasVector& arg_blasVec,
			const KazMath::Vec3<float>& arg_pos,
			const KazMath::Vec2<float>& arg_upScale,
			const KazMath::Vec2<float>& arg_downScale,
			const KazBufferHelper::BufferData& arg_texBuffer,
			float arg_angle,
			const KazMath::Color& arg_color = { 255,255,255,255 }
		);

		DrawFuncData::DrawCallData m_drawCommand;
		const DrawFuncData::DrawData* m_drawCommandData;
		KazBufferHelper::BufferData m_textureBuffer;
		KazMath::Vec2<float> m_textureSize;

		RESOURCE_HANDLE m_vertexHandle;

		void Error();
		void StackOnBlas(Raytracing::BlasVector& arg_blasVec, const DirectX::XMMATRIX& arg_worldMat);

	};


	struct ModelRender
	{
		DrawFuncData::DrawCallData m_drawCommand;
		const DrawFuncData::DrawData* m_drawCommandData;
		std::shared_ptr<ModelInfomation> m_modelInfo;
		std::shared_ptr<ModelAnimator> m_animator;

		ModelRender(const std::string& arg_fileDir, const std::string& arg_filePass);
		ModelRender(const std::shared_ptr<ModelInfomation>& arg_modelInfomation, const DrawFuncData::DrawCallData& arg_drawCall);
		ModelRender();

		void Load(const std::string& arg_fileDir, const std::string& arg_filePass);
		void Load(const std::shared_ptr<ModelInfomation>& arg_modelInfomation, const DrawFuncData::DrawCallData& arg_drawCall);

		bool LoadAnimation();

		void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec, KazMath::Transform3D& arg_trasform3D, const KazMath::Color& arg_addColor = KazMath::Color(255, 255, 255, 255), float arg_timeScale = 1.0f);
		void Error();

	};

	struct LineRender
	{
		DrawFuncData::DrawCallData m_drawCommand;
		const DrawFuncData::DrawData* m_drawCommandData;


		LineRender(DrawingByRasterize& arg_rasterize, const DrawFuncData::DrawCallData& arg_drawCall);
		LineRender();
		void Generate(DrawingByRasterize& arg_rasterize);
		void Generate(DrawingByRasterize& arg_rasterize, const DrawFuncData::DrawCallData& arg_drawCall);
		void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec, const KazMath::Vec3<float>& arg_startPos, const KazMath::Vec3<float>& arg_endPos, const KazMath::Color& arg_color = KazMath::Color(255, 255, 255, 255));

	private:
		std::array<KazMath::Vec3<float>, 2>posArray;
		RESOURCE_HANDLE m_vertexHandle;
	};

}