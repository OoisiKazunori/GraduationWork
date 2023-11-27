#pragma once
#include "../KazLibrary/Scene/SceneBase.h"
#include "../KazLibrary/Buffer/DrawFuncData.h"
#include "../KazLibrary/Render/DrawFunc.h"
#include "../KazLibrary/Sound/SoundManager.h"
#include "../KazLibrary/Loader/ModelLoader.h"
#include "../KazLibrary/Render/DrawFuncHelper.h"
#include "../Game/Debug/DebugCamera.h"
#include "../KazLibrary/Render/BasicDraw.h"
#include "../UI/UI.h"

class Player;
class Camera;

class SelectSceneSprite : public UI2DElement
{
public:
	using UI2DElement::UI2DElement;

};

class StageSelectScene :public SceneBase
{
public:
	StageSelectScene(DrawingByRasterize& arg_rasterize, float cameraSensitivity, float f_volume, bool f_isFlip);
	~StageSelectScene();

	void Init();
	void PreInit();
	void Finalize();
	void Input();
	void Update(DrawingByRasterize& arg_rasterize);
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);
	//指定のステージに移動する
	int SceneChange();

	void OpenOptionsInit();
	void CloseOptionsInit();

	enum SelectOpsions
	{
		ToGame,
		Opsions,
		ExitGame,
	};

	enum OptionsOpstions
	{
		//マウス反転
		MouseReversal,
		//マウス感度調整
		MouseSens,
		//ボリューム調整
		//Volume,
	};
	
	
private:
	//描画--------------------------

	int m_sceneNum;

	int m_nowSelectNum;
	const int C_MaxSelectNum = 1;

	int m_opsionsSelectNum = MouseReversal;
	bool m_isOptionsOpen = false;
	int m_OptionsOpenSelect = -1; 


	const int UIBaseX = 300;
	const int UIBaseY = 200;

	const int UIDistance = 50;

	const float UIHidePos = -200.0f; 

	float mouseSensitivity = 1.0;

	const float SensitivityBarX = 700;

	int sensitivityIconOffset = 0;
	int volumeIconOffset = 0;

	static float volume;

	int triggerTime = 0;

	bool isMouseReversal = false;

	int inputDeray = 0;
	const int C_InputDeray = 45;

	SelectSceneSprite m_backSp;
	SelectSceneSprite m_backBarSp;
	SelectSceneSprite m_loadGameSp;
	SelectSceneSprite m_exitGameSp;
	SelectSceneSprite m_selectBackSp;
	SelectSceneSprite m_OprionsSp;

	SelectSceneSprite m_MouseReveralSp;
	SelectSceneSprite m_MouseSensSp;
	SelectSceneSprite m_VolumeSp;

	SelectSceneSprite m_SensitivityBarSp;
	SelectSceneSprite m_SensitivityIconSp;

	SelectSceneSprite m_VolumeBarSp;
	SelectSceneSprite m_VolumeIconSp;

	SelectSceneSprite m_FlipCheckBoxSp;
	SelectSceneSprite m_MouseFlipCheckSp;

	SelectSceneSprite m_escSp;

	public :
	static int startStageNum;
	static const int C_StageMaxNum;
	static float GetVolune(){return volume;};
	static int GetStartStageNum(){return startStageNum;};
};