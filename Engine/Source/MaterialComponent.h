#pragma once
#include "Component.h"
#include "Shader.h"
#include "TextEditor.h"

#include <string>

class Texture;
class Resource;
class CameraComponent;

class MaterialComponent : public Component
{
public:
	MaterialComponent(GameObject* own, bool defaultMat = true);
	MaterialComponent(MaterialComponent* mat);
	~MaterialComponent();

	bool Update(float dt) override;
	void OnEditor() override;

	void ShaderEditor();
	void MenuShaderList();
	void MenuTextureList();

	std::vector<Uniform> GetShaderUniforms();
	void ShowUniforms();

	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;

	void Bind(CameraComponent* gameCam);
	void ShaderSetUniforms();
	void Unbind();
	bool IsDefaultMat() const { return defaultMat; }

	std::shared_ptr<Texture> GetTexture() { return diff; }
	void SetTexture(std::shared_ptr<Resource> tex);
	void EditorShader();

private:
	bool checker = false;
	bool showTexMenu = false;
	bool showShaderEditor = false;
	bool showShaderMenu = false;

	TextEditor editor;
	std::string fileToEdit;

	std::shared_ptr<Shader> shadertoRecompily;
	std::shared_ptr<Texture> diff = nullptr;
	std::shared_ptr<Shader> shader;

	// TODO: Temporary, should go in Material.h
	float3 ambientColor;
	float3 diffuseColor;
	float3 specularColor;

	float shininess;
	float refreshShaderTimer;
	bool defaultMat;
};