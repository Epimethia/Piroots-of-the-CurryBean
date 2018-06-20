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
#include "Dependencies\Visual Leak Detector\include\vld.h"

//Main Includes
#include <iostream>
#include <map>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>
#include <iterator>
#include <memory>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <strstream>

//Network Programming
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

namespace UTILS {
	const int WindowWidth = 750;
	const int WindowHeight = 750;
}

#define VALIDATE(a) if (!a) return (false)

namespace {
	std::string ToString(sockaddr_in _sockAddress) {
		//INET_ADDRSTRLEN - maximum length for IPv4 addresses
		char _pcAddress[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &_sockAddress.sin_addr, _pcAddress, INET_ADDRSTRLEN);

		std::string _strAddress = _pcAddress;
		std::string _strPort = std::to_string(ntohs(_sockAddress.sin_port));
		std::string _strAddressPort = _strAddress + ':' + _strPort;

		return _strAddressPort;
	}
}

template<typename T>
std::string ToString(const T& _value) {
	std::strstream theStream;
	theStream << _value << std::ends;
	return (theStream.str());
}


#pragma region SHADER MACROS
//Entity Shader
#define VERT_SHADER				"Assets/Shaders/Entity.vs"
#define FRAG_SHADER				"Assets/Shaders/Entity.fs"

//Text Shader
#define TEXT_VERT_SHADER		"Assets/Shaders/Text.vs"
#define TEXT_FRAG_SHADER		"Assets/Shaders/Text.fs"

//Wave Shader
#define WAVE_VERT_SHADER		"Assets/Shaders/Wave.vs"
#define WAVE_FRAG_SHADER		"Assets/Shaders/Wave.fs"

//Model Shader
#define MODEL_VERT_SHADER		"Assets/Shaders/Model.vs"
#define MODEL_FRAG_SHADER		"Assets/Shaders/Model.fs"

//CubeMap Shader
#define CUBEMAP_VERT_SHADER		"Assets/Shaders/CubeMap.vs"
#define CUBEMAP_FRAG_SHADER		"Assets/Shaders/CubeMap.fs"

#pragma endregion	SHADER MACROS END

//Sprite Macros
#define PLAYER_SPRITE			"Assets/Sprite/Rayman.jpg"
#define BOSS_SPRITE				"Assets/Sprite/AwesomeFace.png"
#define POWER_UP_1				"Assets/Sprite/Power_Up_1.png"
#define POWER_UP_2				"Assets/Sprite/Power_Up_2.png"
#define BULLET_SPRITE			"Assets/Sprite/Bullet.png"

//CubeMap Macro
#define CUBEMAP_FILE_PATH		"Assets/Sprite/"

//Sound Macros
#define SHOOT_FX				"Assets/Sound/CannonShot.mp3"
#define BG_MUSIC_LOOP			"Assets/Sound/BGsound.mp3"

//Text Macros
#define ARIAL					"Assets/Fonts/arial.ttf"
#define SNACKERCOMIC			"Assets/Fonts/SnackerComic.ttf"
#define WALLTOGRAPHUI			"Assets/Fonts/waltographUI.ttf"
#define PIRATEFONT				"Assets/Fonts/PretendPirates-Regular.ttf"

//Model Macros					
#define PLAYER_MODEL			"Assets/Models/PlayerBoat/Boat1.obj"
#define WAVE_MODEL				"Assets/Models/wave/Wave2.obj"
#define POWERUP_MODEL			"Assets/Models/PowerUp3/Power_Up_3.obj"

//Keyboard Macros
#define INPUT_FIRST_RELEASED	0
#define INPUT_RELEASED			1
#define INPUT_FIRST_PRESSED		2
#define INPUT_HOLD				3


#endif //__RESOURCE_FILE_H__