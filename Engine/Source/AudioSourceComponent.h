#pragma once
#include "Component.h"

class TransformComponent;

struct AudioClip
{
	std::string clipName;
	bool playOnAwake;
};

class AudioSourceComponent : public Component
{
public:
	AudioSourceComponent(GameObject* own, TransformComponent* trans);
	~AudioSourceComponent();

	void OnEditor() override;
	bool Update(float dt) override;

	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;

	void SwapAudioClip(const char* clipName);

	void PlayClip(const char* clipMap);
	void PlayClipsOnAwake();
	void StopClip();
	void PauseClip();
	void ResumeClip();

	inline void ChangePosition() { changePosition = true; }
private:
	bool changePosition;
	TransformComponent* transform;

	// Audio settings
	std::vector<AudioClip> audioClip;
	unsigned int playingID;
	bool mute;
	float volume;
	float pitch;
};