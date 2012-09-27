enum TYPEOFCOLLIDABLES
{
	DEFAULT,
	FLIPPERLEFT,
	FLIPPERRIGHT,
	TABLE,
	COLLISION,
	SPAWN,
	PATH,
	TYPEOFCOLLIDABLESSIZE
};

class LineModel
{
public:
	//LineModel(D3DShader* Shader);
	LineModel(ID3D10Device* device);
	~LineModel();

	LineModel* m_Child;

	TYPEOFCOLLIDABLES m_Type;

	vector<Vector3>Verticies;
	vector<Vector3> Normals;

	vector<float> FinalVerts;

	vector<Vector3> FinalList;

	int m_VertexCount;

	ID3D10Buffer* m_VertexBuffer;

	Shader* m_Shader;

	bool ChangedOrigin;

	void Init();
	void Update();
	void Draw(D3DXMATRIX WorldMatrix,D3DXMATRIX ViewMatrix,D3DXMATRIX ProjectionMatrix,float DeltaTime);

	void SetPosition(Vector3 Position);
	void SetRotation(Vector3 Rotation);

	Vector3 GetPosition();
	Vector3 GetRotation();

	void MoveRelativeToRotation(Vector2 Rotation);

	void ChangeOrigin( Vector3 NewOrigin );
	void RotateVertsAroundOrigin(Vector3 Rotation);

	D3DXMATRIX rotationMatrix;

	Vector3 newOrigin;
	Vector3 OriginalOrigin;

	vector<Vector3>* GetVerts();

private:

	ID3D10Device* m_Device;


	bool m_Visible;
	Vector3 m_Position;
	Vector3 m_Rotation;	
	Vector3 tempOrigin;
	bool beenInitted;
};