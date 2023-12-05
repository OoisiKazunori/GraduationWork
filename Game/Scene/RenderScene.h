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
	public:
		void Load(DrawingByRasterize& arg_rasterize, std::string arg_filePass, std::string arg_fileName,float arg_scale);
		void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec, const KazMath::Transform3D &arg_baseTransform);

	private:
		float m_scale;
		//モデルの配置
		std::array<std::array<BasicDraw::BasicModelRender, 4>, 10>m_modelDrawArray;
	};
	std::array<ParallelModels, 7> m_models;

	//ライトの位置
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

