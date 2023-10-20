#include "EchoVector.h"

EchoVector::EchoVector()
{
}

void EchoVector::Init()
{

	//配列をクリア。
	m_echoVector.clear();

}

void EchoVector::Update()
{

	//GPUにデータを転送する。

}

void EchoVector::Add(std::weak_ptr<Echo> arg_refEcho)
{

	//フラグが有効化されているか？
	if (!arg_refEcho.lock()->GetIsActive()) return;

	//要素数をオーバーしないか？
	const int NOW_ELEMENT_COUNT = static_cast<int>(m_echoVector.size());
	if (MAX_ELEMENT_COUNT <= NOW_ELEMENT_COUNT - 1) return;

	//追加。
	m_echoVector.emplace_back(arg_refEcho.lock()->GetEchoData());

}