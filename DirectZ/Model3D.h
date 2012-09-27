class Model3D 
{
public:
	//Model3D(D3DShader* Shader);
	Model3D(ID3D10Device* device);
	~Model3D();

	Model3D* m_Child;

	vector<Vertex>Verticies;
	vector<Vector3>NormalCoords;
	vector<Vector3>NormalVertexCoords;

	vector<float> FinalVerts;
	vector<float> FinalNormals;
	vector<float> FinalTexCoord;
	vector<Vertex> FinalList;

	int m_VertexCount;

	ID3D10Buffer* m_VertexBuffer;
	
	ID3D10ShaderResourceView* m_Texture;
	ID3D10EffectShaderResourceVariable* m_ShaderTexture;

	Shader* m_Shader;

	bool ChangedOrigin;

	void Init();
	void Update();
	void Draw(D3DXMATRIX WorldMatrix,D3DXMATRIX ViewMatrix,D3DXMATRIX ProjectionMatrix,float DeltaTime,Light lights[],Vector3 CurrentCam);
	void DrawNormals(D3DXMATRIX WorldMatrix,D3DXMATRIX ViewMatrix,D3DXMATRIX ProjectionMatrix,float DeltaTime);

	void SetPosition(Vector3 Position);
	void SetRotation(Vector3 Rotation);
	//void SetBallPositionDependent(Ball* m_ball);

	Vector3 GetPosition();
	Vector3 GetRotation();

	void MoveRelativeToRotation(Vector2 Rotation);

	void ChangeOrigin(Vector3 newOrigin);

private:
	vector<LineModel3D*> debugNormals;
	ID3D10Device* m_Device;
	Vector3 newOrigin;

	bool m_Visible;
	Vector3 m_Position;
	Vector3 m_Rotation;	
	
	Vector3 tempOrigin;
	bool beenInitted;
};