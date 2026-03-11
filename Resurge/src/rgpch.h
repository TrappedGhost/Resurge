#pragma once

#include<stdio.h>
#include<iostream>
#include<sstream>
#include<functional>
#include<memory>
#include <utility>
#include <iterator>

#include<string>
#include<vector>
#include <initializer_list>
#include <unordered_map>

#include"Resug/Core/Core.h"
#include"Resug/Core/Log.h"
#include"Resug/Debug/Instrumentor.h"
#include"Resug/Debug/Operator.h"

#ifdef RG_PLATFORM_WINDOWS
	#include<Windows.h>
#endif