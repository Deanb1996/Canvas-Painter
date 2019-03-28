#pragma once
#include <string>
#include <vector>
#include "Vertex.h"
#include <fstream>
#include <tuple>

class ObjLoader
{
public:

	static std::pair<std::vector<unsigned>, std::vector<Vertex>> LoadObject(const std::wstring& pFilename);
};

