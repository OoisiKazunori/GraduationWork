#include "Player.h"
#include "Input/KeyBoradInputManager.h"
#include "Input/ControllerInputManager.h"
#include "../Game/Collision/MeshCollision.h"
#include "../Bullet/BulletMgr.h"
#include "../Camera.h"
#include "../KazLibrary/PostEffect/Outline.h"
#include "../Echo/EchoArray.h"
#include "../ThrowableObject/ThrowableObjectController.h"
#include "Imgui/MyImgui.h"
#include "../Game/Menu/Menu.h"
#include "PlayerStatus.h"
#include "../Footprint/FootprintMgr.h"
#include "../Effect/StopMgr.h"
#include "../KazLibrary/Easing/easing.h"
#include "../KazLibrary/Debug/DebugKey.h"
#include "../BGM/BGMController.h"

Player::Player(DrawingByRasterize& arg_rasterize, KazMath::Transform3D f_startPos, bool arg_toStartPos) :
	m_model(arg_rasterize, "Resource/Test/Virus/", "virus_cur.gltf"),
	m_collisionModel(arg_rasterize, "Resource/Player/Collision/", "collision.gltf"),
	m_mk23Model(arg_rasterize, ModelLoader::Instance()->Load("Resource/Weapon/Mk23/", "Mk23.gltf"), DrawFuncData::SetDefferdRenderingModelAnimationAppearByEchoDepthAlways(ModelLoader::Instance()->Load("Resource/Weapon/Mk23/", "Mk23.gltf"))),
	m_mk23MagModel(arg_rasterize, ModelLoader::Instance()->Load("Resource/Weapon/Mk23/", "Mag.gltf"), DrawFuncData::SetDefferdRenderingModelAnimationAppearByEchoDepthAlways(ModelLoader::Instance()->Load("Resource/Weapon/Mk23/", "Mag.gltf")))
{


	m_playerShotSE = SoundManager::Instance()->SoundLoadWave("Resource/Sound/Shot_Player.wav");
	m_playerShotSE.volume = 0.05f;

	m_sonarSE = SoundManager::Instance()->SoundLoadWave("Resource/Sound/Sonar.wav");
	m_sonarSE.volume = 0.05f;

	m_adsSE = SoundManager::Instance()->SoundLoadWave("Resource/Sound/ADS.wav");
	m_adsSE.volume = 0.05f;

	m_heartbeatSE = SoundManager::Instance()->SoundLoadWave("Resource/Sound/Heartbeat.wav");
	m_heartbeatSE.volume = 0.2f;

	m_changeWeaponSE = SoundManager::Instance()->SoundLoadWave("Resource/Sound/ChangeWeapon.wav");
	m_changeWeaponSE.volume = 0.05f;

	m_noBullet = SoundManager::Instance()->SoundLoadWave("Resource/Sound/Player/Shot_SE_1_.wav");
	m_noBullet.volume = 0.05f;

	m_meshCollision = std::make_shared<MeshCollision>();
	m_meshCollision->Setting(m_collisionModel.m_model.m_modelInfo->modelData[0].vertexData, m_transform);

	m_transform = f_startPos;
	Init();


	m_inRoom = arg_toStartPos;

	m_transform.pos.y += 10.0f;
}

void Player::Init()
{

	m_playerAttitude = PlayerAttitude::STAND;
	m_onGround = false;
	m_isFoundToEnemy = false;
	m_isADS = false;
	m_gravity = 0.0f;
	m_heatbeatTimer = 0;
	m_gunReaction = KazMath::Vec3<float>();
	m_shotDelay = SHOT_DELAY;
	m_footprintSpan = 0;
	m_footprintSide = false;
	m_isReloadMotionNow = false;
	m_inRoom = false;

}

void Player::TitleUpdate(std::weak_ptr<Camera> arg_camera, DrawingByRasterize& arg_rasterize, std::list<std::shared_ptr<MeshCollision>> f_stageColliders)
{
	//アウトラインを出す中心点代入
	GBufferMgr::Instance()->m_outline->SetOutlineCenterPos(m_transform.pos);

	//当たり判定
	Collision(f_stageColliders);

	//重力をかける。
	if (!m_onGround) {
		m_gravity -= GRAVITY;
	}
	else {
		m_gravity = 0.0f;
	}
	m_transform.pos.y += m_gravity;

	m_transform.quaternion = arg_camera.lock()->GetShotQuaternion().quaternion;

	//心音のタイマー
	m_heatbeatTimer += 1.0f * StopMgr::Instance()->GetGameSpeed();
	float heartBeatTimer = HEARTBEAT_TIMER;
	float heartBeatRange = 20.0f;
	if (heartBeatTimer <= m_heatbeatTimer) {

		SoundManager::Instance()->SoundPlayerWave(m_heartbeatSE, 0);
		EchoArray::Instance()->Generate(m_transform.pos, heartBeatRange, Echo::COLOR::WHITE);
		m_heatbeatTimer = 0;

	}

	//銃の連射の遅延を更新。
	m_shotDelay = std::clamp(m_shotDelay + 1.0f * StopMgr::Instance()->GetGameSpeed(), 0.0f, SHOT_DELAY);

	//メッシュコライダーにトランスフォームを適用
	m_meshCollision->Setting(m_collisionModel.m_model.m_modelInfo->modelData[0].vertexData, m_transform);
}

void Player::Update(std::weak_ptr<Camera> arg_camera, WeponUIManager::WeponNumber arg_weaponNumber, std::weak_ptr<BulletMgr> arg_bulletMgr, std::weak_ptr<ThrowableObjectController> arg_throwableObjectController, std::list<std::shared_ptr<MeshCollision>> f_stageColliders, HPUI& arg_hpUI, bool arg_isTitle, bool arg_is1F)
{

	//動かす前の座標。
	m_prevPos = m_transform.pos;

	//入力処理
	if (!arg_isTitle) {
		Input(arg_camera, arg_bulletMgr, arg_weaponNumber, arg_throwableObjectController);
	}

	/*for (auto itr = f_stageColliders.begin(); itr != f_stageColliders.end(); ++itr)
	{
		Collision(*itr);
	}*/
	//当たり判定
	if (m_isDebug) {

		m_onGround = true;

	}
	else {
		Collision(f_stageColliders);

	}

	//重力をかける。
	if (!m_onGround) {
		m_gravity -= GRAVITY;
	}
	else {
		m_gravity = 0.0f;
	}
	m_transform.pos.y += m_gravity;
	//m_transform.pos.y = 50.0f;


	//動いた方向に回転させる。
	//Rotate(arg_camera);

	//現在の姿勢のステータスによってモデルのスケール量をいじる。アニメーションとかモデルを置き変える処理の代替処理。
	switch (m_playerAttitude)
	{
	case Player::PlayerAttitude::STAND:
		m_transform.scale.x += (1.0f - m_transform.scale.x) / (5.0f * StopMgr::Instance()->GetGameSpeed());
		m_transform.scale.y += (1.0f - m_transform.scale.y) / (5.0f * StopMgr::Instance()->GetGameSpeed());
		m_transform.scale.z += (1.0f - m_transform.scale.z) / (5.0f * StopMgr::Instance()->GetGameSpeed());
		break;
	case Player::PlayerAttitude::SQUAT:
		m_transform.scale.x += (1.0f - m_transform.scale.x) / (5.0f * StopMgr::Instance()->GetGameSpeed());
		m_transform.scale.y += (0.5f - m_transform.scale.y) / (5.0f * StopMgr::Instance()->GetGameSpeed());
		m_transform.scale.z += (1.0f - m_transform.scale.z) / (5.0f * StopMgr::Instance()->GetGameSpeed());
		break;
	default:
		break;
	}

	//アウトラインを出す中心点代入
	GBufferMgr::Instance()->m_outline->SetOutlineCenterPos(m_transform.pos);

	m_weaponTransform.pos = m_transform.pos;
	m_weaponTransform.quaternion = DirectX::XMQuaternionSlerp(m_weaponTransform.quaternion, m_transform.quaternion, 0.9f * StopMgr::Instance()->GetGameSpeed());
	//武器を持っていなかったら
	if (arg_weaponNumber == WeponUIManager::e_NonWepon) {

		m_weaponPosOffset.x += (3.0f - m_weaponPosOffset.x) * (0.2f * StopMgr::Instance()->GetGameSpeed());
		m_weaponPosOffset.y += (3.0f - m_weaponPosOffset.y) * (0.2f * StopMgr::Instance()->GetGameSpeed());
		m_weaponPosOffset.z += (3.0f - m_weaponPosOffset.z) * (0.2f * StopMgr::Instance()->GetGameSpeed());

	}
	else if (m_isADS) {

		m_weaponPosOffset.x += (1.5f - m_weaponPosOffset.x) * (0.5f * StopMgr::Instance()->GetGameSpeed());
		m_weaponPosOffset.y += (0.3f - m_weaponPosOffset.y) * (0.5f * StopMgr::Instance()->GetGameSpeed());
		m_weaponPosOffset.z += (0.0f - m_weaponPosOffset.z) * (0.5f * StopMgr::Instance()->GetGameSpeed());

	}
	else {

		m_weaponPosOffset.x += (1.5f - m_weaponPosOffset.x) * (0.5f * StopMgr::Instance()->GetGameSpeed());
		m_weaponPosOffset.y += (0.5f - m_weaponPosOffset.y) * (0.5f * StopMgr::Instance()->GetGameSpeed());
		m_weaponPosOffset.z += (0.5f - m_weaponPosOffset.z) * (0.5f * StopMgr::Instance()->GetGameSpeed());

	}

	KazMath::Vec3<float> baseWeaponOffsetPos = KazMath::Vec3<float>();
	baseWeaponOffsetPos += m_weaponTransform.GetFront() * m_weaponPosOffset.x;
	baseWeaponOffsetPos -= m_weaponTransform.GetUp() * m_weaponPosOffset.y;
	baseWeaponOffsetPos += m_weaponTransform.GetRight() * m_weaponPosOffset.z;
	m_weaponTransform.pos += baseWeaponOffsetPos;
	m_weaponTransform.pos += m_gunReaction;
	m_weaponTransform.pos += m_reloadMotionTransform.pos;
	m_weaponTransform.quaternion = DirectX::XMQuaternionMultiply(m_weaponTransform.quaternion, m_reloadMotionTransform.quaternion);

	//マガジンの位置も決める。
	m_magTransform = m_weaponTransform;
	m_magTransform.pos += m_reloadMotionMagTransform.pos;

	//銃の反動を更新。
	if (0.01f < m_gunReaction.Length()) {

		m_gunReaction -= m_gunReaction * (0.5f * StopMgr::Instance()->GetGameSpeed());

	}

	//心音のタイマー
	m_heatbeatTimer += 1.0f * StopMgr::Instance()->GetGameSpeed();
	float heartBeatTimer = HEARTBEAT_TIMER;
	float heartBeatRange = 40.0f;
	if (m_isFoundToEnemy) {
		heartBeatTimer = HEARTBEAT_TIMER_FOUND;
		heartBeatRange = 90.0f;
	}
	if (m_isDebug) {
		heartBeatRange = 1500;
	}
	if (heartBeatTimer <= m_heatbeatTimer) {

		SoundManager::Instance()->SoundPlayerWave(m_heartbeatSE, 0);
		EchoArray::Instance()->Generate(m_transform.pos, heartBeatRange, Echo::COLOR::WHITE);
		m_heatbeatTimer = 0;

	}

	//銃の連射の遅延を更新。
	m_shotDelay = std::clamp(m_shotDelay + 1.0f * StopMgr::Instance()->GetGameSpeed(), 0.0f, SHOT_DELAY);

	//メッシュコライダーにトランスフォームを適用
	m_meshCollision->Setting(m_collisionModel.m_model.m_modelInfo->modelData[0].vertexData, m_transform);

	//弾とプレイヤーの当たり判定
	int hitCount = arg_bulletMgr.lock()->CheckMeshCollision(m_meshCollision);
	if (0 < hitCount) {

		//ダメージを受けています。
		arg_hpUI.HitDamage(10, 10);

	}

	//リロードの更新処理
	UpdateReload();

	//室内か外かを判断する。
	if (arg_is1F && KazMath::Vec3<float>(BGM_OUTDOOR_POS - m_transform.pos).Length() <= BGM_CHANGE_SPHERE_RADIUS) {
		m_inRoom = false;
	}
	if (arg_is1F && KazMath::Vec3<float>(BGM_ROOM_POS - m_transform.pos).Length() <= BGM_CHANGE_SPHERE_RADIUS) {
		m_inRoom = true;
	}

	//BGMを更新。
	if (PlayerStatus::Instance()->m_isFound) {

		BGMController::Instance()->ChangeBGM(BGMController::BGM::EMERGENCY);


	}
	else {


		if (m_inRoom || !arg_is1F) {
			BGMController::Instance()->ChangeBGM(BGMController::BGM::INDOOR);
		}
		else {
			BGMController::Instance()->ChangeBGM(BGMController::BGM::OUTSIDE);
		}

	}


	if (DebugKey::Instance()->DebugKeyTrigger(DIK_UP, "PlayerDebugMode", "UPKey")) {
		m_isDebug = !m_isDebug;
	}

}

void Player::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{
	m_mk23Model.m_model.Draw(arg_rasterize, arg_blasVec, m_weaponTransform);
	m_mk23MagModel.m_model.Draw(arg_rasterize, arg_blasVec, m_magTransform);
}

void Player::Input(std::weak_ptr<Camera> arg_camera, std::weak_ptr<BulletMgr> arg_bulletMgr, WeponUIManager::WeponNumber arg_weaponNumber, std::weak_ptr<ThrowableObjectController> arg_throwableObjectController)
{

	m_transform.quaternion = arg_camera.lock()->GetShotQuaternion().quaternion;

	//前方向と右方向のベクトルを取得
	KazMath::Vec3<float> frontVec = m_transform.GetFront();
	frontVec.y = 0;
	KazMath::Vec3<float> rightVec = m_transform.GetRight();
	rightVec.y = 0;

	//前後左右に移動する。
	KazMath::Vec3<float> inputMoveVec = {};
	if (KeyBoradInputManager::Instance()->InputState(DIK_W)) {
		inputMoveVec += frontVec;
	}
	if (KeyBoradInputManager::Instance()->InputState(DIK_S)) {
		inputMoveVec -= frontVec;
	}
	if (KeyBoradInputManager::Instance()->InputState(DIK_A)) {
		inputMoveVec -= rightVec;
	}
	if (KeyBoradInputManager::Instance()->InputState(DIK_D)) {
		inputMoveVec += rightVec;
	}
	m_transform.pos += inputMoveVec.GetNormal() * GetMoveSpeed() * StopMgr::Instance()->GetGameSpeed();

	if (m_isDebug) {


		m_transform.pos += inputMoveVec.GetNormal() * GetMoveSpeed() * StopMgr::Instance()->GetGameSpeed();
		m_transform.pos += inputMoveVec.GetNormal() * GetMoveSpeed() * StopMgr::Instance()->GetGameSpeed();
		m_transform.pos += inputMoveVec.GetNormal() * GetMoveSpeed() * StopMgr::Instance()->GetGameSpeed();

	}

	////CTRLが押されたらステータスを切り返る。
	//if (KeyBoradInputManager::Instance()->InputTrigger(DIK_LCONTROL)) {
	//	switch (m_playerAttitude)
	//	{
	//	case Player::PlayerAttitude::STAND:
	//		m_playerAttitude = PlayerAttitude::SQUAT;
	//		break;
	//	case Player::PlayerAttitude::SQUAT:
	//		m_playerAttitude = PlayerAttitude::STAND;
	//		break;
	//	default:
	//		break;
	//	}
	//}

	//右クリックされている間はADS状態にする。
	bool isOldADS = m_isADS;
	m_isADS = !m_isReloadMotionNow && KeyBoradInputManager::Instance()->MouseInputState(MOUSE_INPUT_RIGHT);
	if (!isOldADS && m_isADS) {
		SoundManager::Instance()->SoundPlayerWave(m_adsSE, 0);
	}

	switch (arg_weaponNumber)
	{
	case WeponUIManager::e_NonWepon:

		arg_throwableObjectController.lock()->InputHold(KeyBoradInputManager::Instance()->MouseInputState(MOUSE_INPUT_LEFT));

		break;
		//case WeponUIManager::e_Echo:
	case WeponUIManager::e_Hundgun:

		//弾をうつ入力も受け付ける。
		if (!m_isReloadMotionNow && KeyBoradInputManager::Instance()->MouseInputTrigger(MOUSE_INPUT_LEFT) && SHOT_DELAY <= m_shotDelay) {

			if (!WeponUIManager::GetCanShot()) {
				SoundManager::Instance()->SoundPlayerWave(m_noBullet, 0);
				return;
			}

			WeponUIManager::Shot();
			//bool isEchoBullet = arg_weaponNumber == WeponUIManager::e_Echo;

			//arg_bulletMgr.lock()->Genrate(m_weaponTransform.pos, arg_camera.lock()->GetShotQuaternion().GetFront(), isEchoBullet);
			arg_bulletMgr.lock()->Genrate(m_weaponTransform.pos, arg_camera.lock()->GetShotQuaternion().GetFront(), true);

			//銃の反動を追加。
			/*if (isEchoBullet) {*/

			m_gunReaction = -arg_camera.lock()->GetShotQuaternion().GetFront() * GUN_REACTION * 3.0f;
			SoundManager::Instance()->SoundPlayerWave(m_playerShotSE, 0);

			/*}
			else {

				m_gunReaction = -arg_camera.lock()->GetShotQuaternion().GetFront() * GUN_REACTION;
				SoundManager::Instance()->SoundPlayerWave(m_playerShotSE, 0);

			}*/

			m_shotDelay = 0;

		}
		if (!m_isReloadMotionNow && KeyBoradInputManager::Instance()->InputTrigger(DIK_R) && WeponUIManager::CanReload())
		{
			m_isReloadMotionNow = true;
			m_reloadMotionPhase = RELOAD_MOTION::PHASE_1;
			m_reloadMotionTransform = KazMath::Transform3D();
			m_reloadMotionMagTransform = KazMath::Transform3D();
			m_reloadMotionTimer = 0;

			//WeponUIManager::Reload();
		}


		break;
	case WeponUIManager::e_WeponMax:
		break;
	default:
		break;
	}


	//リロード中なのに違う武器に変わっていたら。
	if (m_isReloadMotionNow && arg_weaponNumber != WeponUIManager::e_Hundgun) {

		//リロードモーションがフェーズ4以外だったら(まだ弾込めが終了していなかったら)
		if (m_reloadMotionPhase != RELOAD_MOTION::PHASE_4) {

			//リロードをキャンセルする。
			m_reloadMotionPhase = RELOAD_MOTION::PHASE_1;
			m_reloadMotionTransform = KazMath::Transform3D();
			m_reloadMotionMagTransform = KazMath::Transform3D();
			m_reloadMotionTimer = 0;
			m_isReloadMotionNow = false;

		}

	}



}

void Player::UpdateReload()
{

	if (!m_isReloadMotionNow) return;

	switch (m_reloadMotionPhase)
	{
	case Player::RELOAD_MOTION::PHASE_1:
	{

		//銃を回転させる。
		DirectX::XMVECTOR quatenion = DirectX::XMQuaternionIdentity();
		quatenion = DirectX::XMQuaternionRotationAxis(m_weaponTransform.GetUp().ConvertXMVECTOR(), -DirectX::XMConvertToRadians(30.0f));
		quatenion = DirectX::XMQuaternionMultiply(quatenion, DirectX::XMQuaternionRotationAxis(m_weaponTransform.GetRight().ConvertXMVECTOR(), -DirectX::XMConvertToRadians(20.0f)));
		quatenion = DirectX::XMQuaternionMultiply(quatenion, DirectX::XMQuaternionRotationAxis(m_weaponTransform.GetFront().ConvertXMVECTOR(), -DirectX::XMConvertToRadians(10.0f)));

		//補完する。
		m_reloadMotionTransform.quaternion = DirectX::XMQuaternionSlerp(m_reloadMotionTransform.quaternion, quatenion, 0.09f);

		//タイマーの値に応じてY軸を移動させる。
		++m_reloadMotionTimer;
		float easingAmount = EasingMaker(Out, Sine, std::clamp(m_reloadMotionTimer, 0.0f, PHASE1_UPPER_TIMER) / PHASE1_UPPER_TIMER);
		m_reloadMotionTransform.pos = m_weaponTransform.GetUp() * (easingAmount * RELOAD_MOTION_POSITION_Y_IN_MAG);

		if (RELOAD_MOTION_PHASE1_TIMER < m_reloadMotionTimer) {
			m_reloadMotionTimer = 0;
			m_reloadMotionPhase = RELOAD_MOTION::PHASE_2;

		}

	}
	break;
	case Player::RELOAD_MOTION::PHASE_2:
	{

		//マガジンを引っこ抜く。
		m_reloadMotionMagTransform.pos -= m_weaponTransform.GetUp() * 0.1f;

		++m_reloadMotionTimer;
		//タイマーの値に応じて一気に下に落とす。
		float easingAmount = EasingMaker(Out, Cubic, std::clamp(m_reloadMotionTimer, 0.0f, PHASE2_DOWN_TIMER) / PHASE2_DOWN_TIMER);
		m_reloadMotionTransform.pos = m_weaponTransform.GetUp() * (RELOAD_MOTION_POSITION_Y_IN_MAG - (easingAmount * (RELOAD_MOTION_POSITION_Y_IN_MAG - RELOAD_MOTION_POSITION_Y_OUT_MAG)));
		//タイマーが下に落ちる規定時間を超えていたら、慣性を表現する。
		if (PHASE2_DOWN_TIMER < m_reloadMotionTimer) {
			m_reloadMotionTransform.pos -= m_weaponTransform.GetUp() * 0.01f;
		}
		if (RELOAD_MOTION_PHASE2_TIMER < m_reloadMotionTimer) {
			m_reloadMotionTimer = 0;
			m_reloadMotionPhase = RELOAD_MOTION::PHASE_3;
			m_isPhase3ShowMag = true;
		}

		//全体の時間のちょうど半分の時に音を鳴らす。
		if (static_cast<int>(RELOAD_MOTION_PHASE2_TIMER / 4.0f) == m_reloadMotionTimer) {

			SoundManager::Instance()->SoundPlayerWave(m_changeWeaponSE, 0);

		}

	}
	break;
	case Player::RELOAD_MOTION::PHASE_3:
	{

		//最初は銃を既定の位置に戻して、マガジンを見せる。
		if (m_isPhase3ShowMag) {

			m_reloadMotionTransform.pos += (m_weaponTransform.GetUp() * RELOAD_MOTION_POSITION_Y_INSERT_MAG - m_reloadMotionTransform.pos) * 0.1f;
			m_reloadMotionMagTransform.pos += (m_weaponTransform.GetUp() * -0.15f - m_reloadMotionMagTransform.pos) * 0.5f;

			++m_reloadMotionTimer;
			if (RELOAD_MOTION_PHASE3_SHOWMAG_TIMER < m_reloadMotionTimer) {

				m_reloadMotionTimer = 0;
				m_isPhase3ShowMag = false;

			}

		}
		//マガジンを一気に挿入する。
		else {

			++m_reloadMotionTimer;
			const float INSERT_MAG_TIMER = 10.0f;
			float easingAmountMag = EasingMaker(Out, Exp, std::clamp(m_reloadMotionTimer, 0.0f, INSERT_MAG_TIMER) / INSERT_MAG_TIMER);
			float easingAmountGun = EasingMaker(InOut, Sine, m_reloadMotionTimer / RELOAD_MOTION_PHASE3_INSERT_TIMER);
			m_reloadMotionMagTransform.pos = m_weaponTransform.GetUp() * (-0.15f - (-0.15f * easingAmountMag));
			m_reloadMotionTransform.pos = m_weaponTransform.GetUp() * (RELOAD_MOTION_POSITION_Y_INSERT_MAG + (RELOAD_MOTION_POSITION_Y_INSERT_END_MAG - RELOAD_MOTION_POSITION_Y_INSERT_MAG) * easingAmountMag);
			if (RELOAD_MOTION_PHASE3_INSERT_TIMER < m_reloadMotionTimer) {

				m_reloadMotionTimer = 0;
				m_reloadMotionPhase = RELOAD_MOTION::PHASE_4;

				WeponUIManager::Reload();

			}

			//全体の時間のちょうど半分の時に音を鳴らす。
			if (static_cast<int>(RELOAD_MOTION_PHASE3_INSERT_TIMER / 4.0f) == m_reloadMotionTimer) {

				SoundManager::Instance()->SoundPlayerWave(m_changeWeaponSE, 0);

			}

		}
	}

	break;
	case Player::RELOAD_MOTION::PHASE_4:


		m_reloadMotionTransform.quaternion = DirectX::XMQuaternionSlerp(m_reloadMotionTransform.quaternion, DirectX::XMQuaternionIdentity(), 0.3f);


		m_reloadMotionTransform.pos.x += (0.0f - m_reloadMotionTransform.pos.x) * 0.05f;
		m_reloadMotionTransform.pos.y += (0.0f - m_reloadMotionTransform.pos.y) * 0.05f;
		m_reloadMotionTransform.pos.z += (0.0f - m_reloadMotionTransform.pos.z) * 0.05f;

		++m_reloadMotionTimer;
		if (RELOAD_MOTION_PHASE4_TIMER < m_reloadMotionTimer) {

			m_isReloadMotionNow = false;

		}
		break;
	default:
		break;
	}

}

void Player::Rotate(std::weak_ptr<Camera> arg_camera)
{

	KazMath::Vec3<float> movedVec = m_transform.pos - m_prevPos;
	movedVec.y = 0.0f;
	if (0 < movedVec.Length()) {

		//動いた方向。
		movedVec.Normalize();

		//デフォルトの正面ベクトルからの回転量を求める。
		float angle = std::acosf(KazMath::Vec3<float>(0, 0, 1).Dot(movedVec));

		//クォータニオンを求める。
		KazMath::Vec3<float> cross = KazMath::Vec3<float>(0, 0, 1).Cross(movedVec);
		if (cross.Length() <= 0) {
			cross = { 0,1,0 };
		}
		DirectX::XMVECTOR rotateQ = DirectX::XMQuaternionRotationAxis({ cross.x, cross.y, cross.z }, angle);

		//回転を適応
		m_transform.quaternion = DirectX::XMQuaternionSlerp(m_transform.quaternion, rotateQ, 0.15f * StopMgr::Instance()->GetGameSpeed());

	}

	////ADSしていたらカメラと同じ方向を向く。
	//if (m_isADS) {

	//	m_transform.quaternion = arg_camera.lock()->GetCameraQuaternion().quaternion;

	//}

}

void Player::Collision(std::list<std::shared_ptr<MeshCollision>> f_stageColliders)
{

	float GROUND_RAY = 12.0f;

	switch (m_playerAttitude)
	{
	case Player::PlayerAttitude::STAND:
		GROUND_RAY = 7.0f;
		break;
	case Player::PlayerAttitude::SQUAT:
		GROUND_RAY = 3.0f;
		break;
	default:
		break;
	}

	const float RAY_LENGTH = 1.0f;

	//地面と当たり判定を行う。
	m_onGround = false;

	int counter = 0;
	for (auto itr = f_stageColliders.begin(); itr != f_stageColliders.end(); ++itr) {

		//下方向の当たり判定
		MeshCollision::CheckHitResult rayResult = (*itr)->CheckHitRay(m_transform.pos, -KazMath::Vec3<float>(0, 1, 0));
		if (rayResult.m_isHit && 0.0f < rayResult.m_distance && rayResult.m_distance <= GROUND_RAY) {

			//押し戻し。
			m_transform.pos += rayResult.m_normal * (GROUND_RAY - rayResult.m_distance - 0.2f);
			m_onGround = true;

		}

		//当たり判定を計算。
		rayResult = (*itr)->CheckHitRay(m_transform.pos, m_transform.GetFront());
		if (rayResult.m_isHit && 0.0f < rayResult.m_distance && rayResult.m_distance <= RAY_LENGTH) {

			//押し戻し。
			m_transform.pos += rayResult.m_normal * (RAY_LENGTH - rayResult.m_distance);

		}
		//後ろ方向
		rayResult = (*itr)->CheckHitRay(m_transform.pos, -m_transform.GetFront());
		if (rayResult.m_isHit && 0.0f < rayResult.m_distance && rayResult.m_distance <= RAY_LENGTH) {

			//押し戻し。
			m_transform.pos += rayResult.m_normal * (RAY_LENGTH - rayResult.m_distance);

		}
		//右方向
		rayResult = (*itr)->CheckHitRay(m_transform.pos, m_transform.GetRight());
		if (rayResult.m_isHit && 0.0f < rayResult.m_distance && rayResult.m_distance <= RAY_LENGTH) {

			//押し戻し。
			m_transform.pos += rayResult.m_normal * (RAY_LENGTH - rayResult.m_distance);

		}
		//左方向
		rayResult = (*itr)->CheckHitRay(m_transform.pos, -m_transform.GetRight());
		if (rayResult.m_isHit && 0.0f < rayResult.m_distance && rayResult.m_distance <= RAY_LENGTH) {

			//押し戻し。
			m_transform.pos += rayResult.m_normal * (RAY_LENGTH - rayResult.m_distance);

		}

		++counter;
	}
	int a = 0;

}

float Player::GetMoveSpeed()
{
	switch (m_playerAttitude)
	{
	case Player::PlayerAttitude::STAND:
		return MOVE_SPEED_STAND;
		break;
	case Player::PlayerAttitude::SQUAT:
		return MOVE_SPEED_SQUAT;
		break;
	default:
		break;
	}
	return MOVE_SPEED_STAND;
}
