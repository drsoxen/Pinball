class ModelLoader 
{
public:
	
	ModelLoader();
	~ModelLoader();

	Model* Load(char* filename);
	LineModel* LoadLines(char* filename);
	SpawnPoints* LoadSpawnPoints(char* filename);
	template<typename T> static void GenerateVector(std::vector<T> &vec, const std::vector<int> &indices);
	
};