#pragma once
#include <unordered_map>
#include <memory>
#include <glew.h>
#include <glm/glm.hpp>

class Texture;

namespace text {
	class TTFFactory
	{
		std::unordered_map<unsigned char, std::shared_ptr<Texture>> m_TTFCharacterTexMap;
	private:
	public:
		TTFFactory();
		virtual ~TTFFactory();

		bool Load();

		//const SFTCharacter GetFTChar(char character)const;
	};
}