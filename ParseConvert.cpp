#include "ParseConvert.h"

struct PCFLOAT3 {
	float x;
	float y;
	float z;
	PCFLOAT3(const float &x = 0.f, const float &y = 0.f, const float &z = 0.f) :x(x), y(y), z(z) {}
	bool operator==(const PCFLOAT3 &xmlfloat3) const {
		return (this->x == xmlfloat3.x) && (this->y == xmlfloat3.y) && (this->z == xmlfloat3.z);
	}
};

struct PCFLOAT2 {
	float x;
	float y;
	PCFLOAT2(const float &x = 0.f, const float &y = 0.f) :x(x), y(y) {}
	bool operator==(const PCFLOAT2 &xmlfloat3) const {
		return (this->x == xmlfloat3.x) && (this->y == xmlfloat3.y);
	}
};

struct ParseConvert::Vertex {
	PCFLOAT3 pos; 
	PCFLOAT2 textCoord;
	PCFLOAT3 normals;
	Vertex(const PCFLOAT3 &pos = PCFLOAT3(), const PCFLOAT3 &normals = PCFLOAT3(), const PCFLOAT2 &textCoord = PCFLOAT2())
		:pos(pos), normals(normals), textCoord(textCoord) {}
	bool operator==(const Vertex &vertex)const {
		return (vertex.normals == this->normals) && (vertex.pos == this->pos) && (vertex.textCoord == this->textCoord);
	}
};

struct ParseConvert::Materials {
	bool hasTexture;
	bool hasNormMap;
	float specularFactor;
	float reflectionFactor;
	std::string diffuseMap;
	std::string normalMap;
	PCFLOAT3 ambient;
	PCFLOAT3 diffuse;
	PCFLOAT3 specular;
	PCFLOAT3 transparency;
};

void ParseConvert::clearAll() {
	verteñis.clear();
	verteñis.shrink_to_fit();
	textÑoords.clear();
	textÑoords.shrink_to_fit();
	indecis.clear();
	indecis.shrink_to_fit();
	normals.clear();
	normals.shrink_to_fit();
	useMtl.clear();
	useMtl.shrink_to_fit();
	vertex.clear();
	vertex.shrink_to_fit();
	materials.clear();
	meshStartPosition.clear();
	meshStartPosition.shrink_to_fit();
}

ParseConvert::ParseConvert()
{
}


bool ParseConvert::LoadOBJ(const std::string &filename) {
	this->indecis.clear();
	this->materials.clear();
	this->meshStartPosition.clear();
	this->normals.clear();
	this->textÑoords.clear();
	this->verteñis.clear();
	this->vertex.clear();
	std::ifstream input(filename);
	std::string buffer;
	int firstPosition, secondPosition;
	float x, y, z;
	int x_buf, y_buf, z_buf;
	if (input) {
		while (input) {
			input >> buffer;
			if (buffer == "#") {
				getline(input, buffer);
			}
			else if (buffer == "g") {
				this->indecis.size() == 0 ? this->meshStartPosition.push_back(0): this->meshStartPosition.push_back(indecis.size());
				std::cout << indecis.size() << '\n';
			}
			else if (buffer == "mtllib") { //check mtllib
				input >> buffer;
				this->mtlName = buffer;
			}
			else if (buffer == "v") { //search vertecis
				input >> x >> y >> z;
				this->verteñis.push_back(PCFLOAT3(x, y, z));
			}
			else if (buffer == "vn") { // search normals
				input >> x >> y >> z;
				this->normals.push_back(PCFLOAT3(x, y, z));
			}
			else if (buffer == "vt") { // search text coordinates
				input >> x >> y >> z;
				this->textÑoords.push_back(PCFLOAT2(x, y));
			}
			else if (buffer == "usemtl") {
				input >> buffer;
				this->useMtl.push_back(buffer);
			}
			else if (buffer == "f") { // calculate normals
				for (size_t i(0); i < 3; i++) { //read each Vertex for triangle
					input >> buffer;
					firstPosition = buffer.find_first_of('/');
					secondPosition = buffer.find_last_of('/');
					if (firstPosition == std::string::npos || firstPosition == secondPosition || firstPosition + 1 == secondPosition)
					{
						throw std::invalid_argument("Cannot convert faces format");
					}
					x_buf = std::stoi(buffer.substr(0, firstPosition));//position
					if (x_buf < 0) {
						x_buf *= (-1);
					}
					y_buf = std::stoi(buffer.substr(firstPosition + 1, secondPosition - firstPosition - 1)); // texture coordinates
					if (y_buf < 0) {
						y_buf *= (-1);
					}
					z_buf = std::stoi(buffer.substr(secondPosition + 1, std::wstring::npos));// normals 
					if (z_buf < 0) {
						z_buf *= (-1);
					}
					auto findResult = find(vertex.begin(), vertex.end(), Vertex (verteñis[x_buf - 1], normals[z_buf - 1], textÑoords[y_buf - 1]));
					if (findResult == vertex.end()) { //have not such vertex
						this->vertex.push_back(Vertex(verteñis[x_buf - 1], normals[z_buf - 1], textÑoords[y_buf - 1]));// add new triangle
						this->indecis.push_back(vertex.size() - 1);
					}
					else {//already have vertex
						this->indecis.push_back(findResult - vertex.begin());
					}
				}
			}
		}
		if (!this->LoadMTL()) {
			std::cerr << "MTL wasn't loaded";
		}
		return true;
	}
	else {
		return false;
	}
}

bool ParseConvert::LoadMTL() {
	this->clearAll();
	std::ifstream input(this->mtlName);
	std::string buffer;
	std::map <std::string, std::unique_ptr<Materials>>::iterator matIter(materials.end());
	if (input) {
		while (input) {
			input >> buffer;
			if (buffer == "#") {
				getline(input, buffer);
			}
			else if (buffer == "newmtl") {
				input >> buffer;
				this->materials.insert(std::pair<std::string, std::unique_ptr<Materials>>(buffer, std::make_unique<Materials>()));
				matIter = this->materials.find(buffer);
			}
			else if (buffer == "Ns") {
				if (matIter != materials.end()) {
					input >> matIter->second->specularFactor;
				}
			}
			else if (buffer == "Tf") {
				if (matIter != materials.end()) {
					input >> matIter->second->transparency.x >> matIter->second->transparency.y >> matIter->second->transparency.z;
				}
			}
			else if (buffer == "Ka") {
				if (matIter != materials.end()) {
					input >> matIter->second->ambient.x >> matIter->second->ambient.y >> matIter->second->ambient.z;
				}
			}
			else if (buffer == "Kd") {
				if (matIter != materials.end()) {
					input >> matIter->second->diffuse.x >> matIter->second->diffuse.y >> matIter->second->diffuse.z;
				}
			}
			else if (buffer == "Ks") {
				if (matIter != materials.end()) {
					input >> matIter->second->specular.x >> matIter->second->specular.y >> matIter->second->specular.z;
				}
			}
			else if (buffer == "map_Kd") {
				if (matIter != materials.end()) {
					getline(input, buffer);
					matIter->second->diffuseMap = buffer.substr(buffer.find_last_of(' ') + 1, std::string::npos);
					matIter->second->hasTexture = true;
				}
				else if (buffer == "map_bump" || buffer == "bump") {
					if (matIter != materials.end()) {
						getline(input, buffer);
						matIter->second->normalMap = buffer.substr(buffer.find_last_of(' ') + 1, std::string::npos);
						matIter->second->hasNormMap = true;
					}

				}
			}
		}
		return true;
	}
	else {
		return false;
	}
}

bool ParseConvert::LoadFBX(const std::string &filename) {//finish mtl load
	this->clearAll();
	std::map <std::string, std::unique_ptr<Materials>>::iterator matIter(materials.end());
	std::vector<int> indecisBuffer;
	// Initialize the SDK manager. This object handles all our memory management.
	FbxManager* lSdkManager = FbxManager::Create();

	// Create the IO settings object.
	FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	lSdkManager->SetIOSettings(ios);

	// Create an importer using the SDK manager.
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

	// Use the first argument as the filename for the importer.
	if (!lImporter->Initialize(filename.c_str(), -1, lSdkManager->GetIOSettings())) {
		return false;
	}

	// Create a new scene so that it can be populated by the imported file.
	FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");

	// Import the contents of the file into the scene.
	lImporter->Import(lScene);

	// The file is imported; so get rid of the importer.
	lImporter->Destroy();

	FbxNode* lRootNode = lScene->GetRootNode();
	FbxNode *childNode(nullptr);
	FbxMesh *mesh(nullptr);
	int numIndices;
	FbxGeometryElementNormal* normalEl;
	if (lRootNode) {
		for (int i = 0; i < lRootNode->GetChildCount(); i++) {
			childNode = lRootNode->GetChild(i);
			mesh = childNode->GetMesh();
			if (mesh) {
				this->indecis.size() == 0 ? this->meshStartPosition.push_back(0) : this->meshStartPosition.push_back(indecis.size());
				std::cout << indecis.size() << '\n';
				int numVerts = mesh->GetControlPointsCount();
				for (int j(0); j < numVerts; j++)
				{
					FbxVector4 vert = mesh->GetControlPointAt(j);
					this->verteñis.push_back(PCFLOAT3(static_cast<float>(vert.mData[0]), static_cast<float>(vert.mData[1]), static_cast<float>(vert.mData[2])));
				}
				numIndices = mesh->GetPolygonVertexCount();
				indecisBuffer.assign( mesh->GetPolygonVertices(), mesh->GetPolygonVertices() + numIndices);// get indecis

				normalEl = mesh->GetElementNormal();
				bool unMaped;

				if (normalEl) {
					/*Get normals and texture coords and calculate vertex*/
					int vertexCounter(0);
					for (int polyCounter = 0; polyCounter < mesh->GetPolygonCount(); polyCounter++) {
						for (int j(0); j < 3; j++)
						{
							FbxVector2 fbxTexCoord;
							FbxStringList UVSetNameList;
							FbxVector4 normal = normalEl->GetDirectArray().GetAt(vertexCounter);
							// Get the name of each set of UV coords
							mesh->GetUVSetNames(UVSetNameList);

							// Get the UV coords for this vertex in this poly which belong to the first UV set
							// Note: Using 0 as index into UV set list as this example supports only one UV set
							mesh->GetPolygonVertexUV(polyCounter, j, UVSetNameList.GetStringAt(0), fbxTexCoord, unMaped);// for texture UV

							auto findResult = find(vertex.begin(), vertex.end(), Vertex(this->verteñis[indecisBuffer[vertexCounter]], PCFLOAT3(static_cast<float>(normal[0]), static_cast<float>(normal[1]), static_cast<float>(normal[2])), PCFLOAT2(static_cast<float>(fbxTexCoord[0]), static_cast<float>(fbxTexCoord[1]))));
							if (findResult == vertex.end()) { //have not such vertex
								this->vertex.push_back(Vertex(this->verteñis[indecisBuffer[vertexCounter]], PCFLOAT3(static_cast<float>(normal[0]), static_cast<float>(normal[1]), static_cast<float>(normal[2])), PCFLOAT2(static_cast<float>(fbxTexCoord[0]), static_cast<float>(fbxTexCoord[1]))));// add new triangle
								this->indecis.push_back(vertex.size() - 1 + this->meshStartPosition[meshStartPosition.size() - 1]);
							}
							else {//already have vertex
								this->indecis.push_back(findResult - vertex.begin() + this->meshStartPosition[meshStartPosition.size() - 1]);
							}
							vertexCounter++;
						}
					}
				}
				indecisBuffer.clear();
				indecisBuffer.shrink_to_fit();
				FbxSurfaceMaterial *material = mesh->GetNode()->GetMaterial(0);
				if (material) {
					if (this->materials.find(material->GetName()) != this->materials.end())
					{
						continue;
					}
					this->materials.insert(std::pair<std::string, std::unique_ptr<Materials>>(material->GetName(), std::make_unique<Materials>()));
					matIter = this->materials.find(material->GetName());
					FbxProperty prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
					int layeredTextureCount = prop.GetSrcObjectCount<FbxLayeredTexture>();

					// Directly get textures
					FbxFileTexture* texture = FbxCast<FbxFileTexture>(prop.GetSrcObject<FbxFileTexture>(0));
					if (texture) {
						matIter->second->hasTexture = true;
						matIter->second->diffuseMap = texture->GetFileName();
					}

					if (material->GetClassId().Is(FbxSurfacePhong::ClassId))
					{
						// We found a Phong material.  Display its properties.
						// Display the Ambient Color
						matIter->second->ambient = PCFLOAT3(static_cast<float>((((FbxSurfacePhong *)material)->Ambient).Get()[0]),
															static_cast<float>((((FbxSurfacePhong *)material)->Ambient).Get()[1]),
															static_cast<float>((((FbxSurfacePhong *)material)->Ambient).Get()[2]));
						// Display the Diffuse Color
						matIter->second->diffuse = PCFLOAT3(static_cast<float>((((FbxSurfacePhong *)material)->Diffuse).Get()[0]),
															static_cast<float>((((FbxSurfacePhong *)material)->Diffuse).Get()[1]),
															static_cast<float>((((FbxSurfacePhong *)material)->Diffuse).Get()[2]));
						// Display the Specular Color (unique to Phong materials)
						matIter->second->specular = PCFLOAT3(static_cast<float>((((FbxSurfacePhong *)material)->Specular).Get()[0]),
															 static_cast<float>((((FbxSurfacePhong *)material)->Specular).Get()[1]),
															 static_cast<float>((((FbxSurfacePhong *)material)->Specular).Get()[2]));
						//Opacity is Transparency factor now
						matIter->second->transparency = PCFLOAT3(static_cast<float>((((FbxSurfacePhong *)material)->TransparentColor).Get()[0]),
																 static_cast<float>((((FbxSurfacePhong *)material)->TransparentColor).Get()[1]), 
																 static_cast<float>((((FbxSurfacePhong *)material)->TransparentColor).Get()[2]));
						// Display the Shininess
						matIter->second->specularFactor = static_cast<float>((((FbxSurfacePhong *)material)->Shininess).Get());
						// Display the Reflectivity
						matIter->second->reflectionFactor = static_cast<float>((((FbxSurfacePhong *)material)->ReflectionFactor).Get());
					}
				    if (material->GetClassId().Is(FbxSurfaceLambert::ClassId))
					{
						// We found a Lambert material. Display its properties.
						// Display the Ambient Color
						matIter->second->ambient = PCFLOAT3(static_cast<float>((((FbxSurfaceLambert *)material)->Ambient).Get()[0]),
							static_cast<float>((((FbxSurfaceLambert *)material)->Ambient).Get()[1]),
							static_cast<float>((((FbxSurfaceLambert *)material)->Ambient).Get()[2]));
						// Display the Diffuse Color
						matIter->second->diffuse = PCFLOAT3(static_cast<float>((((FbxSurfaceLambert *)material)->Diffuse).Get()[0]),
							static_cast<float>((((FbxSurfaceLambert *)material)->Diffuse).Get()[1]),
							static_cast<float>((((FbxSurfaceLambert *)material)->Diffuse).Get()[2]));
						//Opacity is Transparency factor now
						matIter->second->transparency = PCFLOAT3(static_cast<float>((((FbxSurfaceLambert *)material)->TransparentColor).Get()[0]),
							static_cast<float>((((FbxSurfaceLambert *)material)->TransparentColor).Get()[1]),
							static_cast<float>((((FbxSurfaceLambert *)material)->TransparentColor).Get()[2]));
					}
				}
			}
		}
	}
	// Destroy the SDK manager and all the other objects it was handling.
	lSdkManager->Destroy();
	return true;
}

bool ParseConvert::SaveDPM(const std::string &filename) {
	if (meshStartPosition.size() == 0) {
		return false;
	}
	std::ofstream output(filename, std::ios::out | std::ios::binary);
	int startPositionsSize(this->meshStartPosition.size());
	output.write((char*)&startPositionsSize, sizeof(int));
	for (auto &el : this->meshStartPosition) {
		output.write((char*)&el, sizeof(int));
	}
	int vertexSize(this->vertex.size());
	output.write((char*)&vertexSize, sizeof(int));
	for (auto &el : this->vertex) {
		output.write((char*)&el, sizeof(Vertex));
	}
	int indecisSize(this->indecis.size());
	output.write((char*)&indecisSize, sizeof(int));
	for (auto &el : this->indecis) {
		output.write((char*)&el, sizeof(int));
	}
	output.close();
	return true;
}

bool ParseConvert::LoadDPM(const std::string &filename) {
	this->clearAll();
	int size, data;
	Vertex buffer;
	std::ifstream input(filename, std::ios::in | std::ios::binary);
	if (!input) {
		input.close();
		return false;
	}
	input.read((char*)&size, sizeof(int));
	this->meshStartPosition.reserve(size);
	for (int i(0); i < size; i++) {
		input.read((char*)&data, sizeof(int));
		this->meshStartPosition.push_back(data);
	}
	input.read((char*)&size, sizeof(int));
	this->vertex.reserve(size);
	for (int i(0); i < size; i++) {
		input.read((char*)&buffer, sizeof(Vertex));
		this->vertex.push_back(buffer);
	}
	input.read((char*)&size, sizeof(int));
	this->indecis.reserve(size);
	for (int i(0); i < size; i++) {
		input.read((char*)&data, sizeof(int));
		this->indecis.push_back(data);
	}
	input.close();
	return true;
}


void ParseConvert::Out() {
	std::ofstream out("check.txt");
	for (const auto &element : this->meshStartPosition) {
		out << element << '\n';
	}
	for (const auto &element : this->vertex) {
		out << element.pos.x << ',' << element.pos.y << ',' << element.pos.z << ',';
		out << element.normals.x << ',' << element.normals.y << ',' << element.normals.z << ',';
		out << element.textCoord.x << ',' << element.textCoord.y << '\n';
	}
	for (const auto &element : this->materials) {
		out << element.first << '\n';
		out << element.second->diffuseMap << '\n';
		out << element.second->transparency.x << ',' << element.second->transparency.y << ',' << element.second->transparency.z << '\n';
		out << element.second->ambient.x << ',' << element.second->ambient.y << ',' << element.second->ambient.z << '\n';
		out << element.second->diffuse.x << ',' << element.second->diffuse.y << ',' << element.second->diffuse.z << '\n';
		out << element.second->specular.x << ',' << element.second->specular.y << ',' << element.second->specular.z << '\n';

	}
	size_t count(0);
	for (const auto &element : this->indecis) {
		if (count == 3) {
			out << '\n';
			count = 0;
		}
		out << element << ' ';
		count++;
	}
	out << '\n';
	out.close();
}
ParseConvert::~ParseConvert()
{
	this->clearAll();
}
