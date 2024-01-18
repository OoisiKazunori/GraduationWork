#pragma once
#include"../KazLibrary/Imgui/MyImgui.h"
#include"../EnemyAIData.h"
#include"../KazLibrary/Helper/ISinglton.h"
#include"../KazLibrary/Render/BasicDraw.h"
#include"../Game/AI/Evaluation/FieldAI.h"

class EnemyDebugManager :public ISingleton<EnemyDebugManager>
{
public:

	struct EnemyDebugData
	{
		std::string m_enemyName;
		const KazMath::Transform3D* m_transform;
		const State* m_status;
		const FindGaugeData* m_gaugeData;
		std::array<KazMath::Vec2<float>*, 12>m_coneSightPointArray;
		std::array<KazMath::Vec2<float>*, 12>m_boxSightPointArray;
		bool *m_isFindFlag;

		~EnemyDebugData()
		{
			bool debug = false;
		}
	};

	EnemyDebugManager();

	void Init(DrawingByRasterize& arg_rasterize);

	void Generate(
		const EnemyDebugData *arg_enemyData
	);
	void Update();
	void Draw(DrawingByRasterize& arg_rasterize,
		Raytracing::BlasVector& arg_blasVec);
	void DrawImGui();

	bool m_debugAIFlag;
private:
	std::array<EnemyDebugData,10>m_enemyAIDataArray;
	EnemyDebugData m_debugData;

	BasicDraw::BasicModelRender m_selectEnemyModelRender;//‘I‘ð’†‚Ì•\Ž¦

	std::array<BasicDraw::BasicLineRender, 6>m_coneSightArray;
	//Ž‹–ìŠp‚Ì•\Ž¦
};
