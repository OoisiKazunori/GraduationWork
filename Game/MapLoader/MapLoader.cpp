#include "MapLoader.h"

void MapData::Init()
{
	ParameterMgr l_mapData;
	l_mapData.LoadFile("Resource/MapDatas/map1.json");

	for (int o_counter = 0; o_counter < static_cast<int>(l_mapData.doc["Objects"].GetArray().Size()); o_counter++)
	{
		MapObject l_obj;

		l_obj.m_position.x = l_mapData.doc["Objects"].GetArray()[o_counter]["transform"]["translation"].GetArray()[0].GetFloat();
		l_obj.m_position.y = l_mapData.doc["Objects"].GetArray()[o_counter]["transform"]["translation"].GetArray()[1].GetFloat();
		l_obj.m_position.z = l_mapData.doc["Objects"].GetArray()[o_counter]["transform"]["translation"].GetArray()[2].GetFloat();

		l_obj.m_rotition.x = l_mapData.doc["Objects"].GetArray()[o_counter]["transform"]["rotation"].GetArray()[0].GetFloat();
		l_obj.m_rotition.y = l_mapData.doc["Objects"].GetArray()[o_counter]["transform"]["rotation"].GetArray()[1].GetFloat();
		l_obj.m_rotition.z = l_mapData.doc["Objects"].GetArray()[o_counter]["transform"]["rotation"].GetArray()[2].GetFloat();

		l_obj.m_scale.x = l_mapData.doc["Objects"].GetArray()[o_counter]["transform"]["scaling"].GetArray()[0].GetFloat();
		l_obj.m_scale.y = l_mapData.doc["Objects"].GetArray()[o_counter]["transform"]["scaling"].GetArray()[1].GetFloat();
		l_obj.m_scale.z = l_mapData.doc["Objects"].GetArray()[o_counter]["transform"]["scaling"].GetArray()[2].GetFloat();

		l_obj.m_objetName = l_mapData.doc["Objects"].GetArray()[o_counter]["name"].GetString();

		m_objects.push_back(l_obj);
	}
	int a = static_cast<int>(m_objects.size());
}
