#pragma once
#include <vector>
#include <memory>
#include "Echo.h"
#include "../KazLibrary/Helper/KazBufferHelper.h"
#include "../KazLibrary/Helper/ISinglton.h"
#include "../Game/Echo/Echo.h"

/// <summary>
/// 使用するEchoのデータが入っているArrayクラス。そのフレームで使用するEchoを追加していく。
/// </summary>
class EchoArray : public ISingleton<EchoArray>{

private:

	//GPUに転送する構造化バッファ本体。
	static const int MAX_ELEMENT_COUNT = 64;
	std::array<Echo::EchoData, MAX_ELEMENT_COUNT> m_echoArray;

	//ラスタライズに送るEchoの記録情報
	static const int MAX_MEMORY_ELEMENT_COUNT = 256;
	std::array<Echo::EchoMemoryData, MAX_MEMORY_ELEMENT_COUNT> m_echoMemoryArray;

	//Echoクラス
	std::array<Echo, MAX_ELEMENT_COUNT> m_echo;
	std::array<Echo, MAX_MEMORY_ELEMENT_COUNT> m_echoMemory;

	//構造化バッファ
	KazBufferHelper::BufferData m_echoStructuredBuffer;
	KazBufferHelper::BufferData m_echoMemoryStructuredBuffer;


public:

	EchoArray();

	/// <summary>
	/// 構造化バッファなどを生成する。
	/// </summary>
	void Setting();

	/// <summary>
	/// 初期化処理 主に配列をクリアする処理を書く予定。シーンの更新処理の一番先頭に持ってきたい。
	/// </summary>
	void Init();

	/// <summary>
	/// 更新処理 主にGPUにデータを転送する処理を書く予定。シーンの更新処理の下の方に持ってきたい。
	/// </summary>
	void Update();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="arg_pos"></param>
	/// <param name="arg_maxRadius"></param>
	/// <param name="arg_color"></param>
	void Generate(KazMath::Vec3<float> arg_pos, float arg_maxRadius, Echo::COLOR arg_colorID);

	KazBufferHelper::BufferData* GetEchoStructuredBuffer() { return &m_echoStructuredBuffer; }
	const KazBufferHelper::BufferData &GetEchoMemoryStructuredBuffer() { return m_echoMemoryStructuredBuffer; }
	std::array<Echo, MAX_ELEMENT_COUNT>& GetEcho() { return m_echo; }


};