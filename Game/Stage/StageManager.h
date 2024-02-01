#pragma once
#include"StageModel.h"
#include "../Game/Collision/MeshCollision.h"

class StageNumCounter
{
public:
	static int stageNumCount;
};
/// <summary>
/// �X�e�[�W�S�̂̊Ǘ�����
/// MapLoader�̃f�[�^�Ȃǂ������ŕ`�悵�Ă���
/// </summary>
class StageManager
{
private:
	int m_nowStageNumber, m_nextStageNumber;
	bool m_changeSceneTriggerFlag;
	//�n�`�̃��f��

	std::list<std::unique_ptr<StageModel>> m_phone;

	std::list<std::unique_ptr<StageModel>> m_cylinder;

	std::list<std::unique_ptr<StageModel>> m_block01;
	std::list<std::unique_ptr<StageModel>> m_enemys;
	std::list<std::unique_ptr<StageModel>> m_turets;

	std::unique_ptr<StageModel> m_Bird1;
	std::unique_ptr<StageModel> m_Bird2;
	std::unique_ptr<StageModel> m_Bird3;

	std::list<std::unique_ptr<StageModel>> m_plane;



	std::list<std::shared_ptr<MeshCollision>> m_collisions[3];
	//���̑��ؓ��̊O���I�u�W�F�N�g���f��--------------------------------

	//�X�e�[�W�؂�ւ��O�ɕK���Ăяo���Ă�������
	void ChangeScene(DrawingByRasterize& arg_rasterize);
	std::unique_ptr<StageModel> m_goal;
public:
	StageManager();

	void Init(DrawingByRasterize& arg_rasterize, int f_stageNum);
	void Update(DrawingByRasterize& arg_rasterize);
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);
	//�X�e�[�W���؂�ւ�����g���K�[
	bool ChangeSceneTrigger();
	void AddMapDatas(DrawingByRasterize& arg_rasterize, int f_stageNum);

	void CheckInEcho(std::weak_ptr<MeshCollision> arg_stageMeshCollision);

	std::list<std::unique_ptr<StageModel>> m_stone;
	std::list<std::unique_ptr<StageModel>> m_magazin;

	std::list<std::shared_ptr<MeshCollision>> GetColliders() { return m_collisions[m_nowStageNumber]; };
	std::unique_ptr<StageModel> m_stage[3];
	std::unique_ptr<StageModel> m_colStage[3];
	std::unique_ptr<StageModel> m_player;
	KazMath::Transform3D GetGoalTransform();
	//���̂��邩1����J�E���g�����
	int GetEnemyCount();
	//GetEnemyCount - 1�������ɂ���ƒ��x��������
	std::list<KazMath::Transform3D> GetEnemyPositions(int f_enemyNum);
	//�^���b�g�p
	int GetTuretCount();
	KazMath::Transform3D GetTuretPosition(int f_turetNum);
};

