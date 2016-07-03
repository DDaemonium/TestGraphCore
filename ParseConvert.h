#pragma once
#pragma comment (lib, "libfbxsdk-md.lib")
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <fbxsdk.h>
#include <iostream>//delete

	struct PCFLOAT3;
	struct PCFLOAT2;

class ParseConvert
{
private:
	std::string filename, mtlName;

	struct Vertex;
	struct Materials;
	std::vector<std::string> useMtl;
	std::map<std::string, std::unique_ptr<Materials>> materials;
	std::vector<PCFLOAT3> verteñis, normals;
	std::vector<PCFLOAT2> textÑoords;
	std::vector<Vertex> vertex;
	std::vector<int> meshStartPosition, indecis;
	void clearAll();
public:
	ParseConvert();
	bool LoadOBJ(const std::string &filename);
	bool LoadMTL();
	bool LoadFBX(const std::string &filename);
	bool SaveDPM(const std::string &filename);
	bool LoadDPM(const std::string &filename);
	void Out();//delete
	~ParseConvert();
};

