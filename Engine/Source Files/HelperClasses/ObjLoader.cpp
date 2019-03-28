#include "ObjLoader.h"

using namespace std;
using namespace DirectX;

pair<vector<unsigned>, vector<Vertex>> ObjLoader::LoadObject(const wstring & pFilename)
{
	vector<unsigned> indices;
	vector<Vertex> vertices;

	vector<XMFLOAT3> positions;
	vector<XMFLOAT2> texCoords;
	vector<XMFLOAT3> normals;

	vector<tuple<unsigned, unsigned, unsigned>> addedInd;
	int indexCount = 0;

	ifstream fin(pFilename);

	while (!fin.eof())
	{
		string type;
		fin >> type;

		if (type == "v")
		{
			//vertex position
			XMFLOAT3 pos{};
			fin >> pos.x >> pos.y >> pos.z;
			positions.push_back(pos);
		}
		else if (type == "vt")
		{
			//vertex texture coordinates
			XMFLOAT2 texCoord{};
			fin >> texCoord.x >> texCoord.y;
			texCoords.push_back(texCoord);
		}
		else if (type == "vn")
		{
			//vertex normal
			XMFLOAT3 norm{};
			fin >> norm.x >> norm.y >> norm.z;
			normals.push_back(norm);
		}
		else if (type == "f")
		{
			for (auto i = 0; i < 3; i++)
			{
				unsigned int posIndex, texIndex, normIndex;
				char c;
				fin >> posIndex >> c >> texIndex >> c >> normIndex;

				//try to find if the vertex has already been created
				auto index = find(addedInd.begin(), addedInd.end(), make_tuple(posIndex, texIndex, normIndex));
				if (index == addedInd.end())
				{
					//if it hasnt been created, add the vertex, and add a new index
					addedInd.emplace_back(make_tuple(posIndex, texIndex, normIndex));
					Vertex v{};
					v.mPosition = positions[posIndex - 1];
					v.mNormal = normals[normIndex - 1];
					v.mTexCoords = texCoords[texIndex - 1];
					vertices.push_back(v);
					indices.push_back(indexCount);
					++indexCount;
				}
				else
				{
					//if it has been created then add the same index again
					indices.push_back(static_cast<int>(index - addedInd.begin()));
				}
			}
		}
	}

	return make_pair(indices, vertices);
}
