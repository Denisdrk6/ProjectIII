#include "PrefabManager.h"
#include "Application.h"
#include "Globals.h"

#include "ModuleScene.h"
#include "FileSystem.h"

PrefabManager* PrefabManager::instance = nullptr;

PrefabManager* PrefabManager::GetInstance()
{
	if (!instance) instance = new PrefabManager();

	return instance;
}

void PrefabManager::ReleaseInstance()
{
	RELEASE(instance);
}

PrefabManager::PrefabManager()
{
}

PrefabManager::~PrefabManager()
{
}

void PrefabManager::SavePrefab(GameObject* gameObject, int option)
{
	std::string fileName = gameObject->name.c_str();
	fileName += ".rgprefab";

	if (option == 1)
	{
		JsonParsing prefabFile;

		prefabFile = prefabFile.SetChild(prefabFile.GetRootValue(), "Prefab");
		JSON_Array* array = prefabFile.SetNewJsonArray(prefabFile.GetRootValue(), "Game Objects");

		std::string savePath = PREFABS_FOLDER;
		savePath += fileName;

		if (app->fs->Exists(savePath.c_str()) != 0)
		{
			fileName = RenameFile(gameObject, fileName);
			savePath = PREFABS_FOLDER;
			savePath += fileName;
		}

		gameObject->prefabPath = savePath;

		gameObject->OnSavePrefab(prefabFile, array, option);

		char* buf;
		uint size = prefabFile.Save(&buf);

		if (app->fs->Save(savePath.c_str(), buf, size) > 0)
			DEBUG_LOG("Prefab saved succesfully");
		else
			DEBUG_LOG("Prefab couldn't be saved");

		RELEASE_ARRAY(buf);
	}
	else if (option == 2)
	{
		JsonParsing prefabFile;

		prefabFile = prefabFile.SetChild(prefabFile.GetRootValue(), "Prefab");
		JSON_Array* array = prefabFile.SetNewJsonArray(prefabFile.GetRootValue(), "Game Objects");

		gameObject->OnSavePrefab(prefabFile, array, option);

		char* buf;
		uint size = prefabFile.Save(&buf);

		if (app->fs->Save(gameObject->prefabPath.c_str(), buf, size) > 0)
			DEBUG_LOG("Prefab saved succesfully");
		else
			DEBUG_LOG("Prefab couldn't be saved");

		RELEASE_ARRAY(buf);
	}
	else if (option == 3)
	{
		std::string namePath = gameObject->prefabPath;
		app->fs->GetFilenameWithExtension(namePath);

		if (namePath == fileName)
		{
			fileName = RenameFile(gameObject, fileName);

			JsonParsing prefabFile;

			prefabFile = prefabFile.SetChild(prefabFile.GetRootValue(), "Prefab");
			JSON_Array* array = prefabFile.SetNewJsonArray(prefabFile.GetRootValue(), "Game Objects");

			std::string savePath = PREFABS_FOLDER;
			savePath += fileName;
			gameObject->prefabPath = savePath;

			std::string saveName = fileName;
			app->fs->GetFilenameWithoutExtension(saveName);
			gameObject->SetName(saveName.c_str());

			gameObject->OnSavePrefab(prefabFile, array, option);

			char* buf;
			uint size = prefabFile.Save(&buf);

			if (app->fs->Save(savePath.c_str(), buf, size) > 0)
				DEBUG_LOG("Prefab saved succesfully");
			else
				DEBUG_LOG("Prefab couldn't be saved");

			RELEASE_ARRAY(buf);

			option = 1;
		}
		else
		{
			JsonParsing prefabFile;

			prefabFile = prefabFile.SetChild(prefabFile.GetRootValue(), "Prefab");
			JSON_Array* array = prefabFile.SetNewJsonArray(prefabFile.GetRootValue(), "Game Objects");

			std::string savePath = PREFABS_FOLDER;
			savePath += fileName;
			gameObject->prefabPath = savePath;

			gameObject->OnSavePrefab(prefabFile, array, option);

			char* buf;
			uint size = prefabFile.Save(&buf);

			if (app->fs->Save(savePath.c_str(), buf, size) > 0)
				DEBUG_LOG("Prefab saved succesfully");
			else
				DEBUG_LOG("Prefab couldn't be saved");

			RELEASE_ARRAY(buf);
		}
	}

	if (option != 1)
		UpdatePrefabs(gameObject);
}

std::string PrefabManager::RenameFile(GameObject* gameObject, std::string& fileName)
{
	bool nameDone = false;
	int count = 0;

	std::vector<std::string> files;
	app->fs->DiscoverFiles("Assets/Prefabs/", files);

	for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
	{
		if ((*it).find(gameObject->name) != std::string::npos)
			count++;
	}

	fileName = gameObject->name.substr(0, gameObject->name.find_last_of("(")) + "(" + std::to_string(count) + ")" + ".rgprefab";
	gameObject->prefabPath = "Assets/Prefabs/" + fileName;

	while (!nameDone)
	{
		if (app->fs->Exists(gameObject->prefabPath.c_str()) != 0)
		{
			count++;
			fileName = gameObject->name.substr(0, gameObject->name.find_last_of("(")) + "(" + std::to_string(count) + ")" + ".rgprefab";
			gameObject->prefabPath = "Assets/Prefabs/" + fileName;
		}
		else
			nameDone = true;
	}

	return fileName;
}

void PrefabManager::LoadPrefab(const char* path)
{
	JsonParsing prefabFile = JsonParsing();

	if (prefabFile.ParseFile(path) > 0)
	{
		JSON_Array* jsonArray = prefabFile.GetJsonArray(prefabFile.ValueToObject(prefabFile.GetRootValue()), "Game Objects");

		size_t size = prefabFile.GetJsonArrayCount(jsonArray);
		for (int i = 0; i < size; ++i)
		{
			if (i == 0)
			{
				JsonParsing go = prefabFile.GetJsonArrayValue(jsonArray, i);
				GameObject* parent = app->scene->GetGoByUuid(0);
				GameObject* child = app->scene->CreateGameObject(parent, false);
				child->OnLoad(go);
			}
			else
			{
				JsonParsing go = prefabFile.GetJsonArrayValue(jsonArray, i);
				GameObject* parent = app->scene->GetGoByUuid(go.GetJsonNumber("Parent UUID"));
				GameObject* child = app->scene->CreateGameObject(parent, false);
				child->OnLoad(go);
			}
		}
	}
	else
	{
		DEBUG_LOG("Prefab couldn't be loaded");
	}
}

void PrefabManager::UpdatePrefabs(GameObject* gameObject)
{
	std::queue<GameObject*> que;
	que.push(app->scene->GetRoot());

	std::vector<GameObject*> listGo;
	std::vector<GameObject*> listParents;
	std::vector<uint> idToDeleteScene;
	std::vector<uint> idToDeletePrefab;
	JsonParsing prefabFile = JsonParsing();

	FillListGoParents(que, listGo, idToDeleteScene, listParents);
	AddObjectsFromPrefabs(prefabFile, gameObject, idToDeletePrefab, listGo, listParents);
	RemoveObjectFromPrefabs(idToDeletePrefab, idToDeleteScene, listGo);

	idToDeleteScene.clear();
	idToDeletePrefab.clear();
	listGo.clear();
}

void PrefabManager::FillListGoParents(std::queue<GameObject*>& que, std::vector<GameObject*>& listGo, std::vector<uint>& idToDeleteScene, std::vector<GameObject*>& listParents)
{
	while (!que.empty())
	{
		GameObject* itGo = que.front();
		que.pop();

		if (itGo->GetPrefabID() != 0)
		{
			listGo.push_back(itGo);

			bool exists = false;
			for (std::vector<uint>::iterator it = idToDeleteScene.begin(); it != idToDeleteScene.end(); ++it)
			{
				if ((*it) == itGo->GetPrefabID())
				{
					exists = true;
					break;
				}
			}

			if (!exists)
				idToDeleteScene.push_back(itGo->GetPrefabID());

			if (itGo->prefabPath != "None")
			{
				listParents.push_back(itGo);
			}
		}

		if (itGo->GetChilds().size() > 0)
		{
			for (std::vector<GameObject*>::iterator it = itGo->GetChilds().begin(); it != itGo->GetChilds().end(); ++it)
			{
				que.push((*it));
			}
		}
	}
}
// Add objects to prefabs if in the reference have been added 
void PrefabManager::AddObjectsFromPrefabs(JsonParsing& prefabFile, GameObject* gameObject, std::vector<uint>& idToDeletePrefab, std::vector<GameObject*>& listGo, std::vector<GameObject*>& listParents)
{
	if (prefabFile.ParseFile(gameObject->prefabPath.c_str()) > 0)
	{
		JSON_Array* jsonArray = prefabFile.GetJsonArray(prefabFile.ValueToObject(prefabFile.GetRootValue()), "Game Objects");

		size_t size = prefabFile.GetJsonArrayCount(jsonArray);
		for (int i = 0; i < size; ++i)
		{
			JsonParsing go = prefabFile.GetJsonArrayValue(jsonArray, i);
			uint prefabID = go.GetJsonNumber("PrefabID");

			idToDeletePrefab.push_back(prefabID);

			for (std::vector<GameObject*>::iterator it = listGo.begin(); it != listGo.end(); ++it)
			{
				if (prefabID == (*it)->GetPrefabID())
				{
					if (i == 0)
					{
						(*it)->UpdateFromPrefab(go, true);
					}
					else
					{
						(*it)->UpdateFromPrefab(go, false);
					}
				}
			}

			for (std::vector<GameObject*>::iterator it = listParents.begin(); it != listParents.end(); ++it)
			{
				bool exist = false;

				if (prefabID == (*it)->GetPrefabID())
				{
					exist = true;
				}

				for (std::vector<GameObject*>::iterator it2 = (*it)->GetChilds().begin(); it2 != (*it)->GetChilds().end(); ++it2)
				{
					if (prefabID == (*it2)->GetPrefabID())
					{
						exist = true;
						break;
					}
				}


				if (!exist)
				{
					GameObject* newGO = app->scene->CreateGameObject((*it), false);
					newGO->OnLoad(go);
				}
			}
		}
	}
	else
	{
		DEBUG_LOG("Prefab couldn't be updated");
	}
}
// Remove objects to prefabs if in the reference have been removed 
void PrefabManager::RemoveObjectFromPrefabs(std::vector<uint>& idToDeletePrefab, std::vector<uint>& idToDeleteScene, std::vector<GameObject*>& listGo)
{
	if (idToDeletePrefab.size() != idToDeleteScene.size())
	{
		std::vector<uint> toDelete;

		for (std::vector<uint>::iterator it = idToDeleteScene.begin(); it != idToDeleteScene.end(); ++it)
		{
			bool exists = false;
			for (std::vector<uint>::iterator it2 = idToDeletePrefab.begin(); it2 != idToDeletePrefab.end(); ++it2)
			{
				if ((*it) == (*it2))
				{
					exists = true;
					break;
				}
			}

			if (!exists)
				toDelete.push_back((*it));
		}

		for (std::vector<uint>::iterator it = toDelete.begin(); it != toDelete.end(); ++it)
		{
			for (std::vector<GameObject*>::iterator it2 = listGo.begin(); it2 != listGo.end(); ++it2)
			{
				if ((*it) == (*it2)->GetPrefabID())
				{
					GameObject* selectedParent = (*it2)->GetParent();

					for (std::vector<GameObject*>::iterator i = selectedParent->GetChilds().begin(); i != selectedParent->GetChilds().end(); ++i)
					{
						if ((*it2) == (*i))
						{
							selectedParent->GetChilds().erase(i);
							RELEASE((*it2));
							app->scene->ResetQuadtree();
							break;
						}
					}
				}
			}
		}

		toDelete.clear();
	}
}