#pragma once
#include <string>

namespace text {
	class TextObject
	{
	public:
		TextObject();
		virtual ~TextObject();

		void Draw(std::string DrawTexLine);
	};
}