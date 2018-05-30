#pragma once
#ifndef __RESOURCE_FILE_H__
#define __RESOURCE_FILE_H__

#pragma once

//Include files
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\soil\SOIL.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"
#include "Dependencies\FMOD\fmod.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H

//VLD
//#include "Dependencies\Visual Leak Detector\include\vld.h"

//Main Includes
#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <vector>
#include <iterator>
#include <memory>

namespace UTILS {
	extern float WindowWidth;
	extern float WindowHeight;
}

//Shader Macros
#define VERT_SHADER				"Assets/Shaders/Vertex Shader.vs"
#define FRAG_SHADER				"Assets/Shaders/Fragment Shader.fs"
#define TEXT_VERT_SHADER		"Assets/Shaders/Text.vs"
#define TEXT_FRAG_SHADER		"Assets/Shaders/Text.fs"

//Sprite Macros
#define PLAYER_SPRITE			"Assets/Sprite/Rayman.jpg"
#define BOSS_SPRITE				"Assets/Sprite/AwesomeFace.png"

//Background Macros
#define MAIN_MENU_SPRITE		""
#define SEA_BG					"Assets/Background/Sea BG.png"

//Sound Macros
#define THUMP_SOUND				"Assets/Sound/Thump.wav"
#define BG_MUSIC_LOOP			"Assets/Sound/Background.mp3"

//Text Macros
#define ARIAL					"Assets/Fonts/arial.ttf"
#define SNACKERCOMIC			"Assets/Fonts/SnackerComic.ttf"
#define WALLTOGRAPHUI			"Assets/Fonts/waltographUI.ttf"

//Keyboard Macros
#define INPUT_FIRST_RELEASED	0
#define INPUT_RELEASED			1
#define INPUT_FIRST_PRESSED		2
#define INPUT_HOLD				3


#endif //__RESOURCE_FILE_H__