#pragma once
#include"Scene/SceneBase.h"
#include"../KazLibrary/Buffer/DrawFuncData.h"
#include"../KazLibrary/Render/DrawFunc.h"
#include"../KazLibrary/Sound/SoundManager.h"
#include"../KazLibrary/Loader/ModelLoader.h"
#include"../KazLibrary/Render/DrawFuncHelper.h"
#include"../Game/Debug/DebugCamera.h"
#include"../KazLibrary/Render/DrawFuncHelper.h"
#include"../KazLibrary/Render/BasicDraw.h"

class RenderScene :public SceneBase
{
public:
	RenderScene(DrawingByRasterize& arg_rasterize);
	~RenderScene();

	void Init();
	void PreInit();
	void Finalize();
	void Input();
	void Update(DrawingByRasterize& arg_rasterize);
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	int SceneChange();

private:
	//カメラ--------------------------
	DebugCamera m_camera;

	//モデルの描画--------------------------
	BasicDraw::BasicModelRender m_sponzaModelRender;//スポンザ描画
	KazMath::Transform3D m_sponzaModelTransform;

	/// <summary>
	/// モデルをXY上に並べた配置するクラス
	/// </summary>
	class ParallelModels
	{
		static const int X_ARRAY = 10;
		static const int Y_ARRAY = 4;
	public:
		void Load(DrawingByRasterize& arg_rasterize, std::string arg_filePass, std::string arg_fileName, const KazMath::Transform3D& arg_baseTransform);
		void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

		std::array<KazMath::Vec3<float>, X_ARRAY* Y_ARRAY> GetPosArray();
	private:
		//モデルの配置
		std::array<std::array<BasicDraw::BasicModelRender, Y_ARRAY>, X_ARRAY>m_modelDrawArray;
		//モデルのtransform
		std::array<std::array<KazMath::Transform3D, Y_ARRAY>, X_ARRAY>m_modelTransformArray;
		//座標のみの抽出
		std::array<KazMath::Vec3<float>, X_ARRAY* Y_ARRAY>m_posArray;
	};
	std::array<ParallelModels, 7> m_models;

	//ライトの位置
	KazBufferHelper::BufferData m_uploadLightBuffer, m_defaultLightBuffer;
	std::array<ParallelModels, 12> m_lights;
	bool m_drawLightFlag;

	//G-Bufferの描画--------------------------
	enum GBufferTexEnum
	{
		GBUFFER_ALBEDO,
		GBUFFER_NORMAL,
		GBUFFER_FINAL,
		GBUFFER_MAX
	};
	int m_gBufferType;
	KazMath::Transform2D m_renderTransform;
	std::array<BasicDraw::BasicTextureRender, GBUFFER_MAX>m_gBufferRender;//GBufferに書き込まれたテクスチャの描画

	int m_sceneNum;

	//削除予定
	BasicDraw::BasicModelRender m_axisRender;
};

