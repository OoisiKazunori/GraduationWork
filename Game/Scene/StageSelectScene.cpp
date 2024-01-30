#include "StageSelectScene.h"
#include "../KazLibrary/Input/KeyBoradInputManager.h"
#include "../Menu/Menu.h"
#include "../Camera.h"
#include "../UI/UI.h"

float StageSelectScene::volume = 1.0f;
int StageSelectScene::startStageNum = 0;
const int StageSelectScene::C_StageMaxNum = 2;

StageSelectScene::StageSelectScene(DrawingByRasterize& arg_rasterize, float cameraSensitivity, float f_volume, bool f_isFlip) :
	m_backSp(arg_rasterize, "Resource/MenuTex/SelectSceneBack.png"),
	m_backBarSp(arg_rasterize, "Resource/MenuTex/SelectSceneBar.png"),
	m_loadGameSp(arg_rasterize, "Resource/MenuTex/LoadGame.png"),
	m_exitGameSp(arg_rasterize, "Resource/MenuTex/exitGame.png"),
	m_selectBackSp(arg_rasterize, "Resource/MenuTex/nowSelectBack.png"),
	m_OprionsSp(arg_rasterize, "Resource/MenuTex/Options.png"),

	m_MouseReveralSp(arg_rasterize, "Resource/MenuTex/mouse_flip.png"),
	m_MouseSensSp(arg_rasterize, "Resource/MenuTex/Sensitivity.png"),
	m_VolumeSp(arg_rasterize, "Resource/MenuTex/Volume.png"),

	m_SensitivityBarSp(arg_rasterize, "Resource/MenuTex/volumeBar.png"),
	m_SensitivityIconSp(arg_rasterize, "Resource/MenuTex/volumeIcon.png"),

	m_VolumeBarSp(arg_rasterize, "Resource/MenuTex/volumeBar.png"),
	m_VolumeIconSp(arg_rasterize, "Resource/MenuTex/volumeIcon.png"),

	m_FlipCheckBoxSp(arg_rasterize, "Resource/MenuTex/checkBox.png"),
	m_MouseFlipCheckSp(arg_rasterize, "Resource/MenuTex/check.png"),
	m_escSp(arg_rasterize, "Resource/UITexture/ESC.png")
{
	m_sceneNum = -1;
	m_nowSelectNum = 0;

	mouseSensitivity = cameraSensitivity;
	volume = f_volume;

	isMouseReversal = f_isFlip;

	sensitivityIconOffset = ((int)cameraSensitivity * 100) - 100;


	m_backSp.SetPosition({ 1280 / 2.0f, 720.0f / 2.0f });
	m_backBarSp.SetPosition({ 1280 / 2.0f, 720.0f / 2.0f });

	m_loadGameSp.SetPosition({ UIHidePos, (float)UIBaseY + ((float)UIDistance * (float)ToGame) });
	m_loadGameSp.EasePosInit({ (float)UIBaseX, (float)UIBaseY + ((float)UIDistance * (float)ToGame) });

	m_OprionsSp.SetPosition({ UIHidePos, (float)UIBaseY + ((float)UIDistance * (float)Opsions) });
	m_OprionsSp.EasePosInit({ (float)UIBaseX, (float)UIBaseY + ((float)UIDistance * (float)Opsions) });

	m_selectBackSp.SetPosition({ UIHidePos, (float)UIBaseY + ((float)UIDistance * m_nowSelectNum) });
	m_selectBackSp.EasePosInit({ (float)UIBaseX, (float)UIBaseY + ((float)UIDistance * m_nowSelectNum) });

	m_exitGameSp.SetPosition({ UIHidePos, (float)UIBaseY + ((float)UIDistance * (float)ExitGame) });
	m_exitGameSp.EasePosInit({ (float)UIBaseX, (float)UIBaseY + ((float)UIDistance * (float)ExitGame) });

	m_MouseReveralSp.SetPosition({ UIHidePos, (float)UIBaseY + ((float)UIDistance * 0.0f) });
	m_MouseSensSp.SetPosition({ UIHidePos, (float)UIBaseY + ((float)UIDistance * 1.0f) });
	m_VolumeSp.SetPosition({ UIHidePos, (float)UIBaseY + ((float)UIDistance * 2.0f) });

	m_SensitivityBarSp.SetPosition({ SensitivityBarX + 0.5f, (float)UIBaseY + ((float)UIDistance * 1.0f) });
	m_SensitivityIconSp.SetPosition({ SensitivityBarX, (float)UIBaseY + ((float)UIDistance * 1.0f) });
	m_SensitivityBarSp.SetScale({ 0.7f, 1.0f });

	m_VolumeBarSp.SetPosition({ SensitivityBarX + 0.5f, (float)UIBaseY + ((float)UIDistance * 2.0f) });
	m_VolumeIconSp.SetPosition({ SensitivityBarX, (float)UIBaseY + ((float)UIDistance * 2.0f) });
	m_VolumeBarSp.SetScale({ 0.7f, 1.0f });


	m_FlipCheckBoxSp.SetPosition({ SensitivityBarX, (float)UIBaseY + ((float)UIDistance * 0.0f) });
	m_MouseFlipCheckSp.SetPosition({ SensitivityBarX, (float)UIBaseY + ((float)UIDistance * 0.0f) });

	m_escSp.m_color = { 255, 255, 255 ,255};
	m_escSp.SetScale({0.5f, 0.5f});

	m_selectSE = SoundManager::Instance()->SoundLoadWave("Resource/sound/UI_Click.wav");
	m_selectSE.volume = 0.15f;

	m_upDownSE = SoundManager::Instance()->SoundLoadWave("Resource/sound/ChangeWeapon.wav");
	m_upDownSE.volume = 0.08f;
}

StageSelectScene::~StageSelectScene()
{
}

void StageSelectScene::Init()
{
}

void StageSelectScene::PreInit()
{
}

void StageSelectScene::Finalize()
{
}

void StageSelectScene::Input()
{
	if (inputDeray >= C_InputDeray)
	{
		if (!m_isOptionsOpen)
		{
			m_escSp.SetPosition({ -300.0f, 600.0f });
			if (KeyBoradInputManager::Instance()->InputTrigger(DIK_SPACE) || KeyBoradInputManager::Instance()->InputTrigger(DIK_F))
			{
				SoundManager::Instance()->SoundPlayerWave(m_selectSE, 0);
				if (m_nowSelectNum == ToGame)
				{
					//1�Ԃ̃Q�[���ɍs��
					m_sceneNum = 1;
					//HP�̏�����
					HPUI::InitHP();
				}
				else if (m_nowSelectNum == SelectOpsions::Opsions)
				{
					m_isOptionsOpen = true;
					OpenOptionsInit();
				}
				//��U�Q�[���I���Ƃ����Ŕԍ���ς���
				else if (m_nowSelectNum == ExitGame)
				{
					Menu::SetIsGameEnd(true);
				}
			}
			if (KeyBoradInputManager::Instance()->InputTrigger(DIK_S) || KeyBoradInputManager::Instance()->InputTrigger(DIK_DOWN))
			{
				SoundManager::Instance()->SoundPlayerWave(m_upDownSE, 0);
				m_nowSelectNum++;
				if (m_nowSelectNum > ExitGame)
				{
					m_nowSelectNum = 0;
				}
			}
			if (KeyBoradInputManager::Instance()->InputTrigger(DIK_W) || KeyBoradInputManager::Instance()->InputTrigger(DIK_UP))
			{
				SoundManager::Instance()->SoundPlayerWave(m_upDownSE, 0);
				m_nowSelectNum--;
				if (m_nowSelectNum < 0)
				{
					m_nowSelectNum = ExitGame;
				}
			}
		}
		else
		{
			m_escSp.SetPosition({ 60.0f, 585.0f });
			if (m_OptionsOpenSelect == -1)
			{
				if (KeyBoradInputManager::Instance()->InputTrigger(DIK_SPACE) || KeyBoradInputManager::Instance()->InputTrigger(DIK_F))
				{
					SoundManager::Instance()->SoundPlayerWave(m_selectSE, 0);
					m_OptionsOpenSelect = m_opsionsSelectNum;
				}
			}
			else if (m_OptionsOpenSelect != -1)
			{
				switch (m_OptionsOpenSelect)
				{
				case OptionsOpstions::MouseReversal:
					if (KeyBoradInputManager::Instance()->InputTrigger(DIK_SPACE) ||
						KeyBoradInputManager::Instance()->InputTrigger(DIK_F))
					{
						SoundManager::Instance()->SoundPlayerWave(m_selectSE, 0);
						if (isMouseReversal)isMouseReversal = false;
						else isMouseReversal = true;
					}
					Camera::isFlip = isMouseReversal;
					break;
				case OptionsOpstions::MouseSens:
					if (KeyBoradInputManager::Instance()->InputTrigger(DIK_D))
					{
						if (mouseSensitivity < 2.0f)
						{
							mouseSensitivity += 0.01f;
							sensitivityIconOffset++;
						}
					}
					else if (KeyBoradInputManager::Instance()->InputState(DIK_D))
					{
						triggerTime++;
						if (triggerTime % 2 == 0)
						{
							if (mouseSensitivity < 2.0f)
							{
								mouseSensitivity += 0.01f;
								sensitivityIconOffset++;
							}
						}
					}
					if (KeyBoradInputManager::Instance()->InputTrigger(DIK_A))
					{
						if (mouseSensitivity > 0.02f)
						{
							mouseSensitivity -= 0.01f;
							sensitivityIconOffset--;
						}
					}
					else if (KeyBoradInputManager::Instance()->InputState(DIK_A))
					{
						triggerTime++;
						if (triggerTime % 2 == 0)
						{
							if (mouseSensitivity > 0.02f)
							{
								mouseSensitivity -= 0.01f;
								sensitivityIconOffset--;
							}
						}
					}

					if (!KeyBoradInputManager::Instance()->InputState(DIK_D) && !KeyBoradInputManager::Instance()->InputState(DIK_A))
					{
						triggerTime = 0;
					}
					Camera::CameraSensitivity = mouseSensitivity;
					break;
				/*case OptionsOpstions::Volume:
					if (KeyBoradInputManager::Instance()->InputTrigger(DIK_D))
					{
						if (volume < 2.0f)
						{
							volume += 0.01f;
							volumeIconOffset++;
						}
					}
					else if (KeyBoradInputManager::Instance()->InputState(DIK_D))
					{
						triggerTime++;
						if (triggerTime % 2 == 0)
						{
							if (volume < 2.0f)
							{
								volume += 0.01f;
								volumeIconOffset++;
							}
						}
					}
					if (KeyBoradInputManager::Instance()->InputTrigger(DIK_A))
					{
						if (volume > 0.02f)
						{
							volume -= 0.01f;
							volumeIconOffset--;
						}
					}
					else if (KeyBoradInputManager::Instance()->InputState(DIK_A))
					{
						triggerTime++;
						if (triggerTime % 2 == 0)
						{
							if (volume > 0.02f)
							{
								volume -= 0.01f;
								volumeIconOffset--;
							}
						}
					}

					break;*/
				default:
					break;
				}
			}

			if (KeyBoradInputManager::Instance()->InputTrigger(DIK_W) || KeyBoradInputManager::Instance()->InputTrigger(DIK_UP))
			{
				if (m_OptionsOpenSelect == -1)
				{
					SoundManager::Instance()->SoundPlayerWave(m_upDownSE, 0);
					m_opsionsSelectNum--;
					if (m_opsionsSelectNum < 0)
					{
						//m_opsionsSelectNum = OptionsOpstions::Volume;
						m_opsionsSelectNum = OptionsOpstions::MouseSens;
					}
				}
				else
				{

				}
			}
			if (KeyBoradInputManager::Instance()->InputTrigger(DIK_S) || KeyBoradInputManager::Instance()->InputTrigger(DIK_DOWN))
			{
				if (m_OptionsOpenSelect == -1)
				{
					SoundManager::Instance()->SoundPlayerWave(m_upDownSE, 0);
					m_opsionsSelectNum++;
					//if (m_opsionsSelectNum > OptionsOpstions::Volume)
					if (m_opsionsSelectNum > OptionsOpstions::MouseSens)
					{
						m_opsionsSelectNum = OptionsOpstions::MouseReversal;
					}
				}
				else
				{
					
				}
			}
			if (KeyBoradInputManager::Instance()->InputTrigger(DIK_ESCAPE))
			{
				if (m_OptionsOpenSelect == -1)
				{
					SoundManager::Instance()->SoundPlayerWave(m_selectSE, 0);
					m_isOptionsOpen = false;
					CloseOptionsInit();
				}
				else
				{
					m_OptionsOpenSelect = -1;
				}
			}
		}
	}
}

void StageSelectScene::Update(DrawingByRasterize& arg_rasterize)
{
	m_loadGameSp.Update();
	m_OprionsSp.Update();

	if (!m_isOptionsOpen)
	{
		m_selectBackSp.SetPosition({ (float)UIBaseX, (float)UIBaseY + ((float)UIDistance * m_nowSelectNum) });
	}
	else
	{
		if (m_OptionsOpenSelect == -1)
		{
			m_selectBackSp.SetPosition({ (float)UIBaseX, (float)UIBaseY + ((float)UIDistance * m_opsionsSelectNum) });
		}
	}
	m_exitGameSp.Update();

	m_MouseReveralSp.Update();
	m_MouseSensSp.Update();
	m_VolumeSp.Update();
	if (inputDeray < C_InputDeray)
	{
		inputDeray++;
	}
}

void StageSelectScene::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{
	m_backBarSp.Draw(arg_rasterize);
	m_exitGameSp.Draw(arg_rasterize);
	m_loadGameSp.Draw(arg_rasterize);
	m_OprionsSp.Draw(arg_rasterize);

	m_MouseReveralSp.Draw(arg_rasterize);
	m_MouseSensSp.Draw(arg_rasterize);
	//m_VolumeSp.Draw(arg_rasterize);

	m_escSp.Draw(arg_rasterize);

	if (m_OptionsOpenSelect == OptionsOpstions::MouseSens)
	{
		m_SensitivityIconSp.SetPosition({ SensitivityBarX + (float)sensitivityIconOffset, (float)UIBaseY + ((float)UIDistance * 1.0f) });
		m_SensitivityIconSp.Draw(arg_rasterize);
		m_SensitivityBarSp.Draw(arg_rasterize);

	}
	/*if (m_OptionsOpenSelect == OptionsOpstions::Volume)
	{
		m_VolumeIconSp.SetPosition({ SensitivityBarX + (float)volumeIconOffset, (float)UIBaseY + ((float)UIDistance * 2.0f) });
		m_VolumeIconSp.Draw(arg_rasterize);
		m_VolumeBarSp.Draw(arg_rasterize);
	}*/
	if (m_OptionsOpenSelect == OptionsOpstions::MouseReversal)
	{
		if (isMouseReversal)
		{
			m_MouseFlipCheckSp.Draw(arg_rasterize);
		}
		m_FlipCheckBoxSp.Draw(arg_rasterize);
	}
	m_selectBackSp.Draw(arg_rasterize);

	m_backSp.Draw(arg_rasterize);
}

int StageSelectScene::SceneChange()
{
	if (m_sceneNum != SCENE_NONE)
	{
		int tmp = m_sceneNum;
		m_sceneNum = SCENE_NONE;
		return tmp;
	}
	return SCENE_NONE;
}

void StageSelectScene::OpenOptionsInit()
{

	m_loadGameSp.EasePosInit({ UIHidePos, (float)UIBaseY + ((float)UIDistance * (float)ToGame) });
	m_selectBackSp.EasePosInit({ UIHidePos, (float)UIBaseY + ((float)UIDistance * m_nowSelectNum) });
	m_exitGameSp.EasePosInit({ UIHidePos, (float)UIBaseY + ((float)UIDistance * (float)ExitGame) });
	m_OprionsSp.EasePosInit({ UIHidePos, (float)UIBaseY + ((float)UIDistance * (float)Opsions) });


	m_MouseReveralSp.SetPosition({ UIHidePos, (float)UIBaseY + ((float)UIDistance * 0.0f) });
	m_MouseSensSp.SetPosition({ UIHidePos, (float)UIBaseY + ((float)UIDistance * 1.0f) });
	m_VolumeSp.SetPosition({ UIHidePos, (float)UIBaseY + ((float)UIDistance * 2.0f) });

	m_MouseReveralSp.EasePosInit({ (float)UIBaseX, (float)UIBaseY + ((float)UIDistance * 0.0f) });
	m_MouseSensSp.EasePosInit({ (float)UIBaseX, (float)UIBaseY + ((float)UIDistance * 1.0f) });
	m_VolumeSp.EasePosInit({ (float)UIBaseX, (float)UIBaseY + ((float)UIDistance * 2.0f) });
}

void StageSelectScene::CloseOptionsInit()
{

	m_loadGameSp.EasePosInit({ (float)UIBaseX, (float)UIBaseY + ((float)UIDistance * (float)ToGame) });
	m_selectBackSp.EasePosInit({ (float)UIBaseX, (float)UIBaseY + ((float)UIDistance * m_nowSelectNum) });
	m_exitGameSp.EasePosInit({ (float)UIBaseX, (float)UIBaseY + ((float)UIDistance * (float)ExitGame) });
	m_OprionsSp.EasePosInit({ (float)UIBaseX, (float)UIBaseY + ((float)UIDistance * (float)Opsions) });

	m_MouseReveralSp.EasePosInit({ UIHidePos, (float)UIBaseY + ((float)UIDistance * 0.0f) });
	m_MouseSensSp.EasePosInit({ UIHidePos, (float)UIBaseY + ((float)UIDistance * 1.0f) });
	m_VolumeSp.EasePosInit({ UIHidePos, (float)UIBaseY + ((float)UIDistance * 2.0f) });
}
