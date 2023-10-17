#pragma once
#include <vector>
#include <memory>
#include "Echo.h"

/// <summary>
/// 使用するEchoのデータが入っているvectorクラス。そのフレームで使用するEchoを追加していく。
/// </summary>
class EchoVector {

private:

	std::vector<Echo::EchoData> m_echoVector;
	const int MAX_ELEMENT_COUNT = 64;


public:

	EchoVector();

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


};