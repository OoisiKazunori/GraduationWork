#pragma once
#include "../KazLibrary/Math/KazMath.h"

/// <summary>
/// エコークラス。エコーを出す各クラスがこれを持ち、EchoArrayに登録することでGPUに値が送られる。
/// </summary>
class Echo {

public:

	/// <summary>
	/// GPUに送るエコーの情報。EchoArrayにEchoクラスを渡すことでこの構造体の値が追加される。
	/// </summary>
	struct EchoData {
		KazMath::Vec3<float> m_pos;
		float m_radius;
		KazMath::Vec3<float> m_color;
		float m_alpha;
	};

private:

	EchoData m_echoData;	//エコーに関するデータ
	float m_maxEchoRadius;	//エコーの半径の最大値
	bool m_isActive;		//このエコーが有効化されているかのフラグ

	enum class STATUS {
		APPEAR,
		EXIT,
	}m_status;
	float m_easingTimer;
	const float APPEAR_EASING_TIMER = 12.0f;
	const float EXIT_EASING_TIMER = 16.0f;
	const float ALPHA = 0.12f;


public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Echo();

	/// <summary>
	/// 初期化処理 強制的にエコーを切る。
	/// </summary>
	void Init();

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="arg_maxEchoRadius"> エコーの到達半径 </param>
	/// <param name="arg_echoColor"> エコーの色 </param>
	void Generate(KazMath::Vec3<float> arg_pos, float arg_maxEchoRadius, KazMath::Vec3<float> arg_echoColor);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// エコーの色を変更したいとき。
	/// </summary>
	/// <param name="arg_echoColor"> 適用したいエコーの色 </param>
	void ChangeColor(KazMath::Vec3<float> arg_echoColor) { m_echoData.m_color = arg_echoColor; }

	//各種ゲッタ
	EchoData GetEchoData() { return m_echoData; }
	KazMath::Vec3<float> GetPos() { return m_echoData.m_pos; }
	float GetNowRadius() { return m_echoData.m_radius; }
	bool GetIsActive() { return m_isActive; }

};