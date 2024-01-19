#include "MapLoader.h"
#include <filesystem>
#include <fstream>
#include <sstream>

std::list<std::list<MapObject>> MapManager::m_maps;
std::list<std::list<EnemyData>> MapManager::m_enemys;
std::list<DirectX::XMINT2> MapManager::m_mapSize;
std::list<std::list<std::list<int>>> MapManager::m_mapChips;

void MapManager::Init()
{
	m_maps.clear();
	m_enemys.clear();
	m_mapSize.clear();
	m_mapChips.clear();

	std::list<std::string> l_fileNames;

	//フォルダ内のファイル名全てを取得する
	if (!GetFileNames("Resource/MapDatas", l_fileNames)) return;

	std::list<std::string>::iterator l_fileNameItr = l_fileNames.begin();
	for (int mapNum = 0; mapNum < static_cast<int>(l_fileNames.size()); mapNum++)
	{
		std::list<MapObject> m_objects;
		//jsonデータ
		ParameterMgr l_mapData;
		//ファイル名でjsonを取得
		l_mapData.LoadFile(*l_fileNameItr);

		for (int o_counter = 0; o_counter < static_cast<int>(l_mapData.doc["Objects"].GetArray().Size()); o_counter++)
		{
			MapObject l_obj;
			//ポジション
			l_obj.m_position.x = l_mapData.doc["Objects"].GetArray()[o_counter]["transform"]["translation"].GetArray()[0].GetFloat();
			l_obj.m_position.y = l_mapData.doc["Objects"].GetArray()[o_counter]["transform"]["translation"].GetArray()[1].GetFloat();
			l_obj.m_position.z = l_mapData.doc["Objects"].GetArray()[o_counter]["transform"]["translation"].GetArray()[2].GetFloat();
			//ローテーション
			l_obj.m_rotition.x = l_mapData.doc["Objects"].GetArray()[o_counter]["transform"]["rotation"].GetArray()[0].GetFloat();
			l_obj.m_rotition.y = l_mapData.doc["Objects"].GetArray()[o_counter]["transform"]["rotation"].GetArray()[1].GetFloat();
			l_obj.m_rotition.z = l_mapData.doc["Objects"].GetArray()[o_counter]["transform"]["rotation"].GetArray()[2].GetFloat();
			//スケール
			l_obj.m_scale.x = l_mapData.doc["Objects"].GetArray()[o_counter]["transform"]["scaling"].GetArray()[0].GetFloat();
			l_obj.m_scale.y = l_mapData.doc["Objects"].GetArray()[o_counter]["transform"]["scaling"].GetArray()[1].GetFloat();
			l_obj.m_scale.z = l_mapData.doc["Objects"].GetArray()[o_counter]["transform"]["scaling"].GetArray()[2].GetFloat();
			//エコー範囲
			l_obj.echoScale = l_mapData.doc["Objects"].GetArray()[o_counter]["echo"].GetFloat();
			//名前
			l_obj.m_objetName = l_mapData.doc["Objects"].GetArray()[o_counter]["name"].GetString();


			//エコー範囲のあるものはここで登録していく
			/*if (l_obj.m_objetName.starts_with("echo") == true)
			{

			}*/

			m_objects.push_back(l_obj);
		}

		l_fileNameItr++;
		//マップデータへの登録
		m_maps.push_back(m_objects);
	}

	std::list<std::string> l_enemyFileNames;

	//フォルダ内のファイル名全てを取得する
	if (!GetFileNames("Resource/EnemyDatas", l_enemyFileNames)) return;

	std::list<std::string>::iterator l_enemyFileNameItr = l_enemyFileNames.begin();
	//int x,y = 0;
	for (; l_enemyFileNameItr != l_enemyFileNames.end(); ++l_enemyFileNameItr)
	{
		const int enemyMax = 9;
		std::list<EnemyData> l_enemys;
		l_enemys.resize(enemyMax);
		for (auto itr = l_enemys.begin(); itr != l_enemys.end(); ++itr)
		{
			(*itr).m_position.resize(enemyMax);
			for (auto hogeItr = (*itr).m_position.begin(); hogeItr != (*itr).m_position.end(); ++hogeItr)
			{
				hogeItr->x = -1;
				hogeItr->y = -1;
			}
		}


		ifstream file((*l_enemyFileNameItr));
		std::list<std::list<int>> l_mapChips;
		DirectX::XMINT2 maxSize = { 0, 0 };
		if (file.is_open())
		{
			string line;
			DirectX::XMINT2 pos = { 0, -0 };

			while (getline(file, line))
			{
				istringstream line_stream(line);

				string key;
				l_mapChips.push_back(std::list<int>());
				while (getline(line_stream, key, ','))
				{
					int num = atoi(key.c_str());
					if (num > 10 && num < 100)
					{
						int enemy10Number = -1;
						int enemy1WalkPosint = -1;
						enemy10Number = num / 10;
						enemy10Number -= 1;
						enemy1WalkPosint = num % 10;
						enemy1WalkPosint -= 1;
						auto enemyNumItr = l_enemys.begin();
						for (int i = 0; i < enemy10Number; i++)
						{
							enemyNumItr++;

						}
						auto enemyWalkItr = (*enemyNumItr).m_position.begin();
						for (int k = 0; k < enemy1WalkPosint; k++)
						{
							enemyWalkItr++;
						}
						(*enemyWalkItr) = pos;
					}
					pos.x++;
					(*--l_mapChips.end()).push_back(num);
				}
				pos.y++;
				if (maxSize.x < pos.x)
				{
					maxSize.x = pos.x;
				}
				pos.x = 0;
			}
			maxSize.y = pos.y;
		}
		m_enemys.push_back(l_enemys);
		m_mapSize.push_back(maxSize);
		m_mapChips.push_back(l_mapChips);
	}
}

bool MapManager::GetFileNames(std::string f_folderPath, std::list<std::string>& f_fileNames)
{
	std::filesystem::directory_iterator iter(f_folderPath), end;
	std::error_code err;

	for (; iter != end && !err; iter.increment(err)) {
		const std::filesystem::directory_entry entry = *iter;

		f_fileNames.push_back(entry.path().string());
		printf("%s\n", f_fileNames.back().c_str());
	}

	//エラー処理
	if (err) {
		std::cout << err.value() << std::endl;
		std::cout << err.message() << std::endl;
		return false;
	}
	return true;
}

KazMath::Transform3D MapManager::GetPlayerStartPosition(int f_stageNum)
{
	KazMath::Transform3D l_result;
	auto l_map = MapManager::GetStageData(f_stageNum);

	for (auto l_mapItr = l_map.begin(); l_mapItr != l_map.end(); ++l_mapItr)
	{
		if (l_mapItr->m_objetName.starts_with("player") == true)
		{
			l_result = KazMath::Transform3D({ l_mapItr->m_position.x, l_mapItr->m_position.y, l_mapItr->m_position.z },
				{ l_mapItr->m_scale.x, l_mapItr->m_scale.y, l_mapItr->m_scale.z });
			//ローテーションも後で適応する
			l_mapItr->m_rotition;
		}
	}

	return l_result;
}
