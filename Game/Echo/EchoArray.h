#pragma once
#include <vector>
#include <memory>
#include "Echo.h"
#include "../KazLibrary/Helper/KazBufferHelper.h"
#include "../KazLibrary/Helper/ISinglton.h"

/// <summary>
/// 使用するEchoのデータが入っているArrayクラス。そのフレームで使用するEchoを追加していく。
/// </summary>
class EchoArray : public ISingleton<EchoArray>{

private:

	//GPUに転送する構造化バッファ本体。
	static const int MAX_ELEMENT_COUNT = 64;
	std::array<Echo::EchoData, MAX_ELEMENT_COUNT> m_echoArray;

	//構造化バッファ
	KazBufferHelper::BufferData m_echoStructuredBuffer;


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
	/// 配列に追加する処理。「Echoのフラグが有効化されているか」 と 「配列の要素数の上限を超えないか」 を見て追加する。
	/// </summary>
	/// <param name="arg_refEcho"></param>
	void Add(std::weak_ptr<Echo> arg_refEcho);

	KazBufferHelper::BufferData GetEchoStructuredBuffer() { return m_echoStructuredBuffer; }


};