#pragma once
#include"../Helper/ISinglton.h"
#include"../Helper/KazHelper.h"
#include"../Input/KeyBoradInputManager.h"
#include<vector>
#include<string>
#include<tuple>
#include<memory>
#include<array>
#include"../KazLibrary/Imgui/MyImgui.h"

class DebugKey :public ISingleton<DebugKey>
{
public:
	DebugKey()
	{};
	void CountReset();
	bool DebugKeyTrigger(int KEY, const std::string& KEY_NAME, const std::string& KEY_NUM);
	bool DebugKeyState(int KEY, const std::string& KEY_NAME, const std::string& KEY_NUM);

	void DrawImGui();

private:
	struct DebugData
	{
		std::string m_keyNumber;
		std::string m_keyName;

		DebugData(const std::string& KEY_NUM, const std::string& KEY_NAME) :m_keyNumber(KEY_NUM), m_keyName(KEY_NAME)
		{
		}
	};

	std::vector<std::shared_ptr<DebugData>> m_debugStringData;
	std::vector<int> m_debugKeyStorage;
	std::vector<int> m_coundKyeUsedNum;
	std::array<std::string, 10> m_errorName;

	void CheckKey(int KEY, const std::string& KEY_NAME, const std::string& KEY_NUM);

	void CheckErrorNumber(const std::string& KEY_NUM)
	{
		for (int i = 0; i < m_errorName.size(); ++i)
		{
			if (m_errorName[i] == KEY_NUM)
			{
				std::string name = KEY_NUM + "は数字キーです。デバック用に数字キーを使わないでください";
				MessageBox(NULL, KazHelper::GetWideStrFromStr(name).c_str(), TEXT("数字キー禁止"), MB_OK);
				assert(0);
			}
		}
	}

};
