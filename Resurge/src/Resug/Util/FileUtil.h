#pragma once
#include"rgpch.h"

#include<string>

namespace Resug
{

	class Files
	{
	public:
		static std::string  OpenFile(const char* filter);
		static std::string  SaveFile(const char* filter);
	};
}