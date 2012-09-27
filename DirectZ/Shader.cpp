#include "Common.h"

Shader::Shader(wchar_t* path, ID3D10Device* Device)
{
	memset(this, 0, sizeof(Shader));


	ID3D10Blob* compilationErrors = 0;
	HRESULT hr = D3DX10CreateEffectFromFile(path, 0, 0, "fx_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, Device, 0, 0, &m_Effect, &compilationErrors, 0);

	if (FAILED(hr))
	{
		DevConsole::GetInstance()->logError("Failed to compile shader");
		m_isValid = false;

		if( compilationErrors )
		{
			DevConsole::GetInstance()->logError((char*)compilationErrors->GetBufferPointer());
			compilationErrors->Release();
		}

		return;
	}
	else
	{
		m_isValid = true;
	}


	m_Tech = m_Effect->GetTechniqueByIndex(0);
	m_ProjectionMatrix = m_Effect->GetVariableByName("Projection")->AsMatrix();
	m_WorldMatrix = m_Effect->GetVariableByName("World")->AsMatrix();
	m_ViewMatrix = m_Effect->GetVariableByName("View")->AsMatrix();
	m_WVP = m_Effect->GetVariableByName("WVP")->AsMatrix();
	m_DeltaTime = m_Effect->GetVariableByName("DeltaTime");
	m_Texture = m_Effect->GetVariableByName("Texture")->AsShaderResource();

	m_Lighting  = m_Effect->GetVariableByName("Lights");
	m_BlendingEffect = m_Effect->GetVariableByName("BlendEffect");
	m_TotalTime = m_Effect->GetVariableByName("TotalTime");
	m_Camera = m_Effect->GetVariableByName("EyePos");


	D3D10_INPUT_ELEMENT_DESC vertexDesc[] =
	{ 
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(float)*0, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float)*3, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(float)*5, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"DIFFUSE",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(float)*8, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"SPECULAR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(float)*12, D3D10_INPUT_PER_VERTEX_DATA, 0},
	};

	D3D10_PASS_DESC PassDesc;
	m_Tech->GetPassByIndex(0)->GetDesc(&PassDesc);
	Device->CreateInputLayout(vertexDesc, sizeof(vertexDesc)/sizeof(D3D10_INPUT_ELEMENT_DESC), PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &m_Layout);

}

Shader::~Shader()
{
	if (m_Effect) m_Effect->Release();	
	if (m_Layout) m_Layout->Release();
}

void Shader::SetCurrentCamera(Vector3 Data)
{
	m_Camera->SetRawValue(&Data, 0, sizeof(Vector3));
}

void Shader::SetLight(Light* Data)
{
	m_Lighting->SetRawValue(Data, 0, sizeof(Light)*8);
}

void Shader::SetProjection(float* Data)
{
	m_ProjectionMatrix->SetMatrix(Data);
}

void Shader::SetProjection(D3DXMATRIX* Data)
{
	m_ProjectionMatrix->SetMatrix((float*)&Data);
}

void Shader::SetWorld(float* Data)
{
	m_WorldMatrix->SetMatrix(Data);
}

void Shader::SetWorld(D3DXMATRIX* Data)
{
	m_WorldMatrix->SetMatrix((float*)&Data);
}

void Shader::SetView(float* Data)
{
	m_ViewMatrix->SetMatrix(Data);
}

void Shader::SetView(D3DXMATRIX* Data)
{
	m_ViewMatrix->SetMatrix((float*)&Data);
}

void Shader::SetWVP(float* Data)
{
	m_WVP->SetMatrix(Data);
}

void Shader::SetWVP(D3DXMATRIX* Data)
{
	m_WVP->SetMatrix((float*)&Data);
}

void Shader::SetDeltaTime(float Data)
{
	m_DeltaTime->SetRawValue(&Data, 0, sizeof(float));
}

void Shader::SetTotalTime(float Data)
{
	m_TotalTime->SetRawValue(&Data, 0, sizeof(float));
}

void Shader::SetBlendingState(int Data)
{
	m_BlendingEffect->SetRawValue(&Data, 0, sizeof(int));
}

ID3D10InputLayout* Shader::GetVertexLayout()
{
	return m_Layout;
}

ID3D10EffectTechnique* Shader::GetTech()
{
	return m_Tech;
}