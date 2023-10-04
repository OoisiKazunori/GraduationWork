#pragma once
#include"../KazLibrary/Scene/SceneBase.h"
#include"../KazLibrary/Buffer/DrawFuncData.h"
#include"../KazLibrary/Render/DrawFunc.h"
#include"../KazLibrary/Sound/SoundManager.h"
#include"../KazLibrary/Loader/ModelLoader.h"
#include"../KazLibrary/Render/DrawFuncHelper.h"
#include"../Game/Debug/DebugCamera.h"

class GameScene :public SceneBase
{
private:
public:
	GameScene(DrawingByRasterize& arg_rasterize);
	~GameScene();

	void Init();
	void PreInit();
	void Finalize();
	void Input();
	void Update();
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	int SceneChange();

private:
	int m_sceneNum;

	int GetDigits(int arg_value, int arg_m, int arg_n) {
		int mod_value;
		int result;

		/* n���ڈȉ��̌����擾 */
		mod_value = arg_value % (int)pow(10, arg_n + 1);

		/* m���ڈȏ�̌����擾 */
		result = mod_value / static_cast<int>(pow(10, arg_m));

		return result;

	}
};