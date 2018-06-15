#include "Menu.h"

Menu::Menu() {}

Menu::~Menu() {};

Menu::Menu(std::vector<std::string> _OptVect, glm::vec2 _Pos) {
	ShaderLoader sl;
	TextShader = sl.CreateProgram(TEXT_VERT_SHADER, TEXT_FRAG_SHADER);
	NumMenuOptions = 0;
	CurrentOption = 0;
	glm::vec2 Position = _Pos;

	//Iterating through the provided option vect
	for (unsigned int i = 0; i < _OptVect.size(); ++i) {
		OptionVect.push_back(std::make_shared<Text>(_OptVect[i], PIRATEFONT, Position, TextShader, 40));
		Position.y -= 60.0f;
	}
	NumMenuOptions = _OptVect.size() - 1;
}

void Menu::IncrementMenu() {
	if (CurrentOption == NumMenuOptions) CurrentOption = 0;
	else ++CurrentOption;
}

void Menu::DecrementMenu() {
	if (CurrentOption == 0) CurrentOption = NumMenuOptions;
	else --CurrentOption;
}

void Menu::Process() {
	Render();
}

void Menu::ReplaceOption(int _OptIndex, std::string _OptionText) {
	if (_OptIndex > NumMenuOptions || _OptIndex < 0) return;
	OptionVect[_OptIndex] = std::make_shared<Text>(_OptionText, PIRATEFONT, OptionVect[_OptIndex]->GetPos(), TextShader, 60);
}

void Menu::Render() {

	for (unsigned int i = 0; i < OptionVect.size(); ++i) {
		if (CurrentOption == i) {
			OptionVect[i]->SetColor(glm::vec3(0.10, 0.2, 0.37));
			OptionVect[i]->SetScale(1.1f);
		}
		else {
			OptionVect[i]->SetColor(glm::vec3(1.0, 1.0, 1.0));
			OptionVect[i]->SetScale(1.0f);
		}
		OptionVect[i]->Render();
	}
}
