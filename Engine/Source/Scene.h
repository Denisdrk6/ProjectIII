#pragma once
#include "GameObject.h"
#include "Quadtree.h"

#include "Resource.h"

#include <vector>

struct SerializedField;

enum class Object3D
{
	CUBE = 0,
	PYRAMIDE,
	SPHERE,
	CYLINDER
};

class CameraComponent;

class Scene : public Resource
{
public:
	Scene(uint uid, std::string& assets, std::string& library);
	~Scene();

	bool Start();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate();
	bool Draw();
	bool CleanUp();

	void NewScene();

	GameObject* CreateGameObject(GameObject* parent, bool createTransform = true);
	GameObject* CreateGameObjectChild(const char* name, GameObject* parent);
	GameObject* CreateGameObjectParent(const char* name, GameObject* child);
	inline std::vector<GameObject*> GetGameObjectsList() const 
	{ 
		return root->GetChilds(); 
	}

	inline GameObject* GetRoot() const { return root; }
	GameObject* GetGoByUuid(double uuid) const;

	void SetMainCamera(CameraComponent* camComponent) { mainCamera = camComponent; }

	GameObject* Create3DObject(Object3D type, GameObject* parent);

	void MoveGameObjectUp(GameObject* object);
	void MoveGameObjectDown(GameObject* object);
	void ReparentGameObjects(uint uuid, GameObject* go);

	void Load() override;
	void UnLoad() override;
	
	bool LoadScene(const char* name);
	bool SaveScene(const char* name);

	void DuplicateGO(GameObject* go, GameObject* parent);

	inline void ResetQuadtree() { resetQuadtree = true; }
	inline bool* GetDrawQuad() { return &drawQuad; }

	Quadtree& GetQuadtree() { return qTree; }

	inline GameObject* GetPlayer() { return player; };

	CameraComponent* mainCamera;
	GameObject* camera;

private:
	GameObject* root;
	GameObject* player;

	Quadtree qTree;

	bool frameSkip;
	bool resetQuadtree;
	bool drawQuad = false;
};