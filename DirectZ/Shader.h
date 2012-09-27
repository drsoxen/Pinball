class Shader
{
public:
	Shader(wchar_t* path, ID3D10Device* graphicsDevice);
		~Shader();

	void SetProjection(float* Data);
	void SetProjection(D3DXMATRIX* Data);

	void SetWorld(float* Data);
	void SetWorld(D3DXMATRIX* Data);

	void SetView(float* Data);
	void SetView(D3DXMATRIX* Data);

	void SetWVP(float* Data);
	void SetWVP(D3DXMATRIX* Data);

	void SetDeltaTime(float Data);

	void SetTotalTime(float Data);

	void SetLight(Light* Data);

	void SetBlendingState(int Data);

	void SetCurrentCamera(Vector3 Data);

	ID3D10EffectShaderResourceVariable* m_Texture;

	ID3D10InputLayout* GetVertexLayout();
	ID3D10EffectTechnique* GetTech();
	bool m_isValid;

	ID3D10Effect* m_Effect;
	ID3D10EffectTechnique* m_Tech;
	ID3D10InputLayout* m_Layout;
	ID3D10EffectMatrixVariable* m_ProjectionMatrix;
	ID3D10EffectMatrixVariable* m_WorldMatrix;
	ID3D10EffectMatrixVariable* m_ViewMatrix;
	ID3D10EffectMatrixVariable* m_WVP;
	ID3D10EffectVariable* m_DeltaTime;
	ID3D10EffectVariable* m_TotalTime;
	ID3D10EffectVariable* m_Lighting;
	ID3D10EffectVariable* m_Camera;
	ID3D10EffectScalarVariable* m_LightType;
	ID3D10EffectVariable* m_BlendingEffect;

};