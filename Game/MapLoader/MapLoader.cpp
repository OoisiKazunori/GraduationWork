#include "MapLoader.h"
#include <filesystem>


std::list<std::list<MapObject>> MapManager::m_maps;

void MapManager::Init()
{
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
			//名前
			l_obj.m_objetName = l_mapData.doc["Objects"].GetArray()[o_counter]["name"].GetString();


			//エコー範囲のあるものはここで登録していく
			if (l_obj.m_objetName.starts_with("echo") == true)
			{
				
			}

			m_objects.push_back(l_obj);
		}

		l_fileNameItr++;
		//マップデータへの登録
		m_maps.push_back(m_objects);
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
