#pragma once
#include "Resource.h"
#include "Entity.h"

class Menu {
public:
	Menu();
	~Menu();
	Menu(std::vector<std::string> _OptVect, glm::vec2 Pos);
	void IncrementMenu();
	void DecrementMenu();
	int GetCurrentOpt() { return CurrentOption; };
	void ReplaceOption(int _OptIndex, std::string _OptionText);
	void Process();
private:
	void Render();
	std::vector<std::shared_ptr<Text>> OptionVect;

	GLuint TextShader;
	int NumMenuOptions;
	int CurrentOption;
};