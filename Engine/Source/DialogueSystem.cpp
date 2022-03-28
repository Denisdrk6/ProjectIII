#include "DialogueSystem.h"
#include "Globals.h"
#include "JsonParsing.h"
#include "Imgui/imgui.h"
#include "IconsFontAwesome5.h"

DialogueSystem* DialogueSystem::instance = nullptr;

DialogueSystem* DialogueSystem::GetInstance()
{
	if (!instance) instance = new DialogueSystem();

	return instance;
}

void DialogueSystem::ReleaseInstance()
{
	RELEASE(instance);
}

DialogueSystem::DialogueSystem()
{
	createDialogue = false;
	numOfLines = 0;
}

DialogueSystem::~DialogueSystem()
{
}

void DialogueSystem::OnEditor()
{
	ImGui::Begin("Create Dialogue", &createDialogue);

	/*char text[256];

	int countId = 0;
	for (std::vector<std::string>::iterator it = dialogueLines.begin(); it != dialogueLines.end(); ++it)
	{
		ImGui::PushID(countId);

		strcpy_s(text, (*it).c_str());
		ImGui::InputText("Text", text, IM_ARRAYSIZE(text));
		(*it) = text;

		ImGui::PopID();
		countId++;
	}

	if (ImGui::Button(ICON_FA_PLUS" Add Line"))
	{
		std::string aux = "";
		dialogues.push_back(aux);
	}

	if (ImGui::Button(ICON_FA_FILE" Create Dialogue Archive"))
	{
		
	}*/

	ImGui::End();
}

void DialogueSystem::LoadDialogue(std::vector<std::string>& dialogs, std::string path)
{
	JsonParsing prefabFile = JsonParsing();

	if (prefabFile.ParseFile(path.c_str()) > 0)
	{
		JSON_Array* jsonArray = prefabFile.GetJsonArray(prefabFile.ValueToObject(prefabFile.GetRootValue()), "Dialogs");


	}
}

void DialogueSystem::SaveDialogue(Dialogue& dialogue)
{
	JsonParsing dialogueFile;
	JSON_Array* array = dialogueFile.SetNewJsonArray(dialogueFile.GetRootValue(), "Dialogue");
}
