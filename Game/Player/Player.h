#pragma once
#include"../KazLibrary/Render/BasicDraw.h"
#include"../UI/UI.h"
#include"../KazLibrary/Sound/SoundManager.h"
#include"../Game/UI/UI.h"

class MeshCollision;
class BulletMgr;
class Camera;
class ThrowableObjectController;

class Player {

private:

	BasicDraw::BasicModelRender m_model;	//�g�p���郂�f��
	KazMath::Transform3D m_transform;		//���f���̕`��Ɏg�p����g�����X�t�H�[�����
	KazMath::Vec3<float> m_prevPos;

	BasicDraw::BasicModelRender m_mk23Model;	//�g�p���郂�f��
	BasicDraw::BasicModelRender m_mk23MagModel;	//�g�p���郂�f��
	KazMath::Transform3D m_weaponTransform;
	KazMath::Transform3D m_magTransform;
	KazMath::Vec3<float> m_weaponPosOffset;		//�e�̃��f����z�u����I�t�Z�b�g�BADS���Ă���ʒu����Ƃ���ADS���Ă��Ȃ��ʒu�ɂ��炵���肷��Ƃ��Ɏg�p����B
	KazMath::Vec3<float> m_gunReaction;
	const float GUN_REACTION = 0.25f;

	bool m_onGround;
	bool m_isADS;		//�e���\���Ă����Ԃ��H

	//�����蔻��p�̃��f��
	BasicDraw::BasicModelRender m_collisionModel;	//�g�p���郂�f��
	std::shared_ptr<MeshCollision> m_meshCollision;

	float m_gravity;
	const float GRAVITY = 0.05f;

	//�ړ����x
	const float MOVE_SPEED_STAND = 0.5f;
	const float MOVE_SPEED_SQUAT = 0.3f;
	const float MOVE_SPEED_CREEPING = 0.15f;

	//���˂̒x��
	float m_shotDelay;
	const float SHOT_DELAY = 20;

	//�S���̃^�C�}�[
	float m_heatbeatTimer;
	const float HEARTBEAT_TIMER = 60;
	const float HEARTBEAT_TIMER_FOUND = 32;
	SoundData m_heartbeatSE;

	bool m_isFoundToEnemy;

	SoundData m_playerShotSE;
	SoundData m_sonarSE;
	SoundData m_adsSE;
	SoundData m_changeWeaponSE;
	SoundData m_noBullet;

	//�p���̃X�e�[�^�X
	enum class PlayerAttitude {
		STAND,
		SQUAT,
	}m_playerAttitude;

	//�����[�h�Ɋւ���ϐ�
	enum class RELOAD_MOTION {
		PHASE_1,	//����������[�h����Ƃ��p�̊p�x�ɌX����B(������ƍ����ɌX���ďe������Ɍ�����B)
		PHASE_2,	//�}�K�W���������������B
		PHASE_3,	//�}�K�W����}������B
		PHASE_4,	//�e�̕����𐳂��������ɒ����B
	}m_reloadMotionPhase;
	bool m_isReloadMotionNow;
	bool m_isPhase3ShowMag;
	KazMath::Transform3D m_reloadMotionTransform;	//�����[�h���[�V�����ŕ�����f�t�H���g�̈ʒu����ς������Ƃ��Ɏg�p����B
	KazMath::Transform3D m_reloadMotionMagTransform;	//�����[�h���[�V�����Ń}�K�W�����f�t�H���g�̈ʒu����ς������Ƃ��Ɏg�p����B
	float m_reloadMotionTimer;	//�����[�h���[�V�����ł����Ȏg����������֗��^�C�}�[
	const float PHASE1_UPPER_TIMER = 15;
	const float RELOAD_MOTION_PHASE1_TIMER = 20;
	const float PHASE2_DOWN_TIMER = 10;
	const float RELOAD_MOTION_PHASE2_TIMER = 15;
	const float RELOAD_MOTION_PHASE3_SHOWMAG_TIMER = 5;
	const float RELOAD_MOTION_PHASE3_INSERT_TIMER = 15;
	const float RELOAD_MOTION_PHASE4_TIMER = 20;

	const float RELOAD_MOTION_POSITION_Y_IN_MAG = 0.6f;	//�����[�h���[�V�������ɂ�����Ƃ�����ɂ����邽�߂̕ϐ��B
	const float RELOAD_MOTION_POSITION_Y_OUT_MAG = 0.1f;	//�����[�h���[�V�������ɂ�����Ƃ�����ɂ����邽�߂̕ϐ��B
	const float RELOAD_MOTION_POSITION_Y_INSERT_MAG = 0.2f;	//�����[�h���[�V�������ɂ�����Ƃ�����ɂ����邽�߂̕ϐ��B
	const float RELOAD_MOTION_POSITION_Y_INSERT_END_MAG = 0.25f;	//�����[�h���[�V�������ɂ�����Ƃ�����ɂ����邽�߂̕ϐ��B

	//���ő��Ղ�`�悷��p�B
	float m_footprintSpan;
	const float FOOTPRINT_SPAN = 1;
	bool m_footprintSide;

	//�O�ƒ���BGM��؂�ւ���n�_
	const KazMath::Vec3<float> BGM_OUTDOOR_POS = KazMath::Vec3<float>(171.0f, -43.0f, 168.0f);
	const KazMath::Vec3<float> BGM_ROOM_POS = KazMath::Vec3<float>(171.0f, -43.0f, 141.0f);
	const float BGM_CHANGE_SPHERE_RADIUS = 5.0f;

	bool m_isDebug = false;	//�Ǔ˂����������ړ������B




public:
	//BGM�������p�Ǝ��O�p�Ő؂�ւ��邽�߂̕ϐ�
	bool m_inRoom;
	Player(DrawingByRasterize& arg_rasterize, KazMath::Transform3D f_startPos, bool arg_toStartPos);

	void Init();

	void TitleUpdate(std::weak_ptr<Camera> arg_camera, DrawingByRasterize& arg_rasterize, std::list<std::shared_ptr<MeshCollision>> f_stageColliders);

	void Update(std::weak_ptr<Camera> arg_camera, WeponUIManager::WeponNumber arg_weaponNumber, std::weak_ptr<BulletMgr> arg_bulletMgr, std::weak_ptr<ThrowableObjectController> arg_throwableObjectController, std::list<std::shared_ptr<MeshCollision>> f_stageColliders, HPUI& arg_hpUI, bool arg_isTitle, bool arg_is1F);

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	KazMath::Transform3D GetTransform() { return m_transform; }
	void SetPosition(KazMath::Vec3<float> f_pos) { m_transform.pos = f_pos; }
	bool GetIsADS() { return m_isADS; }


private:

	void Input(std::weak_ptr<Camera> arg_camera, std::weak_ptr<BulletMgr> arg_bulletMgr, WeponUIManager::WeponNumber arg_weaponNumber, std::weak_ptr<ThrowableObjectController> arg_throwableObjectController);
	void UpdateReload();
	void Rotate(std::weak_ptr<Camera> arg_camera);
	void Collision(std::list<std::shared_ptr<MeshCollision>> f_stageColliders);
	float GetMoveSpeed();

};