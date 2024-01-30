#include "DebugKey.h"
#include<assert.h>

void DebugKey::CountReset()
{
	for (int i = 0; i < m_coundKyeUsedNum.size(); ++i)
	{
		m_coundKyeUsedNum[i] = 0;
	}


	for (int i = 0; i < m_errorName.size(); ++i)
	{
		std::string name = "DIK_" + std::to_string(i);
		m_errorName[i] = name;
	}
}

bool DebugKey::DebugKeyTrigger(int KEY, const std::string& KEY_NAME, const std::string& KEY_NUM)
{
//#ifdef  _DEBUG
	CheckKey(KEY, KEY_NAME, KEY_NUM);
	return KeyBoradInputManager::Instance()->InputTrigger(KEY);
//#endif //  DEBUG
	return false;
}

bool DebugKey::DebugKeyState(int KEY, const std::string& KEY_NAME, const std::string& KEY_NUM)
{
#ifdef  _DEBUG
	CheckKey(KEY, KEY_NAME, KEY_NUM);
	return KeyBoradInputManager::Instance()->InputState(KEY);
#endif //  DEBUG
	return false;
}

void DebugKey::DrawImGui()
{
#ifdef  _DEBUG

	ImGui::Begin("DebugKey");
	for (int i = 0; i < m_debugStringData.size(); ++i)
	{
		std::string drawSting = m_debugStringData[i]->m_keyName + ":" + m_debugStringData[i]->m_keyNumber;
		ImGui::Text(drawSting.c_str());
	}
	ImGui::End();

#endif //  DEBUG
}

void DebugKey::CheckKey(int KEY, const std::string& KEY_NAME, const std::string& KEY_NUM)
{
	if (m_debugKeyStorage.size() == 0)
	{
		//CheckErrorNumber(KEY_NUM);
		m_debugKeyStorage.emplace_back(KEY);
		m_debugStringData.emplace_back(std::make_shared<DebugData>(KEY_NUM, KEY_NAME));
		m_coundKyeUsedNum.emplace_back(0);
	}

	//見つからなかったら追加する
	bool findFlag = false;
	for (int i = 0; i < m_debugKeyStorage.size(); ++i)
	{
		if (m_debugKeyStorage[i] == KEY)
		{
			findFlag = true;
			break;
		}
	}
	if (!findFlag)
	{
		//CheckErrorNumber(KEY_NUM);
		m_debugKeyStorage.emplace_back(KEY);
		m_debugStringData.emplace_back(std::make_shared<DebugData>(KEY_NUM, KEY_NAME));
		m_coundKyeUsedNum.emplace_back(0);
	}


	for (int i = 0; i < m_debugKeyStorage.size(); ++i)
	{
		if (m_debugKeyStorage[i] == KEY)
		{
			++m_coundKyeUsedNum[i];
		}
	}

	//同じキーが二回以上使われていたらエラーを吐く
	for (int i = 0; i < m_coundKyeUsedNum.size(); ++i)
	{
		if (2 <= m_coundKyeUsedNum[i])
		{
			std::string name = m_debugStringData[i]->m_keyNumber + "は既に使われています。片方のみ別のキーを使用してください";
			MessageBox(NULL, KazHelper::GetWideStrFromStr(name).c_str(), TEXT("デバックキー被り"), MB_OK);
			assert(0);
		}
	}
}
