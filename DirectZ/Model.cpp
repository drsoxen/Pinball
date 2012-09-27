#include "Common.h"

Model::Model()
{
	memset(this, 0, sizeof(Model));

	//m_Shader = Shader;
	m_Device = GraphicsDevice::GetInstance()->GetGraphicsDevice();
	SetPosition(Vector3(0,0,0));
	SetRotation(Vector3(0,0,0));
	m_ShaderTexture = NULL;
	m_Texture = NULL;
	m_Child = NULL;
	newOrigin = Vector3(0,0,0);
	ChangedOrigin = false;
	beenInitted = false;
}

void Model::Update()
{

}

void Model::Init()
{
	if(!beenInitted)
	{
		if(ChangedOrigin == true)
		{
			tempOrigin = newOrigin;
			ChangeOrigin(-newOrigin);
		}

		m_VertexCount = FinalVerts.size()/3;

		D3D10_BUFFER_DESC vbd;
		vbd.Usage = D3D10_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(Vertex) * FinalList.size();
		vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;

		D3D10_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = &FinalList[0];
		m_Device->CreateBuffer(&vbd, &vinitData, &m_VertexBuffer);

		if(ChangedOrigin == true)
		{
			ChangeOrigin(tempOrigin);
		}
		beenInitted = true;

		for(int i = 0; i < FinalList.size(); i += 3)
		{
			LineModel * line = new LineModel(m_Device);
			

			Vector3 pos, norm, normd;

			pos = (FinalList[i].Position + FinalList[i+1].Position + FinalList[i+2].Position) / 3.0f;
			normd = pos + ((FinalList[i].Normal + FinalList[i+1].Normal + FinalList[i+2].Normal) / 3.0f) * 5.0f;

			line->m_Shader = m_Shader;
			line->FinalList.push_back(pos);
			line->FinalList.push_back(normd);

			line->Init();
			line->m_VertexCount = 2;

			debugNormals.push_back(line);
		}
	}
}

Model::~Model()
{
}

void Model::Draw(D3DXMATRIX WorldMatrix,D3DXMATRIX ViewMatrix,D3DXMATRIX ProjectionMatrix,float DeltaTime,Light lights[],Vector3 CurrentCam)
{
	D3DXMATRIX transMatrix;
	if(ChangedOrigin == true)
		D3DXMatrixTranslation(&transMatrix, newOrigin.X, newOrigin.Y, newOrigin.Z);
	else
		D3DXMatrixTranslation(&transMatrix, m_Position.X, m_Position.Y, m_Position.Z);


	D3DXMATRIX rot;
	D3DXMatrixRotationYawPitchRoll(&rot,m_Rotation.X, m_Rotation.Y, m_Rotation.Z);

	D3DXMATRIX scale;
	D3DXMatrixIdentity(&scale);

	D3DXMATRIX World = scale * rot * transMatrix;
	World = World * WorldMatrix;

	D3DXMATRIX WVP = World * ViewMatrix * ProjectionMatrix;

	m_Shader->SetWorld(World);
	m_Shader->SetWVP(WVP);
	m_Shader->SetLight(lights);
	m_Shader->SetCurrentCamera(CurrentCam);

	m_ShaderTexture = m_Shader->m_Texture;

	if(m_Texture != NULL)
	{
		m_ShaderTexture->SetResource(m_Texture);
	}

	ID3D10EffectTechnique* tech = m_Shader->GetTech();
	ID3D10InputLayout* layout = m_Shader->GetVertexLayout();

	D3D10_TECHNIQUE_DESC techDesc;
	tech->GetDesc( &techDesc );
	for(UINT p = 0; p < techDesc.Passes; ++p)
	{
		tech->GetPassByIndex( p )->Apply(0);   
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		m_Device->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
		m_Device->Draw(m_VertexCount,0);
	}

	if(m_Child != NULL)
		m_Child->Draw(World,ViewMatrix,ProjectionMatrix,DeltaTime,lights,CurrentCam);
}

void Model::DrawNormals(D3DXMATRIX WorldMatrix,D3DXMATRIX ViewMatrix,D3DXMATRIX ProjectionMatrix,float DeltaTime)
{
	for(int i = 0; i < debugNormals.size(); i++)
		debugNormals[i]->Draw(WorldMatrix,ViewMatrix,ProjectionMatrix,DeltaTime);
}

void Model::SetPosition(Vector3 Position)
{
	m_Position.X = Position.X;
	m_Position.Y = Position.Y;
	m_Position.Z = Position.Z;
}

void Model::SetRotation(Vector3 Rotation)
{
	m_Rotation.X = m_Rotation.X + Rotation.X;
	m_Rotation.Y = m_Rotation.Y + Rotation.Y;
	m_Rotation.Z = m_Rotation.Z + Rotation.Z;

	if (m_Rotation.X > TWOPI) m_Rotation.X -= TWOPI;
	if (m_Rotation.Y > TWOPI) m_Rotation.Y -= TWOPI;
	if (m_Rotation.Z > TWOPI) m_Rotation.Z -= TWOPI;

	if (m_Rotation.X < -TWOPI) m_Rotation.X += TWOPI;
	if (m_Rotation.Y < -TWOPI) m_Rotation.Y += TWOPI;
	if (m_Rotation.Z < -TWOPI) m_Rotation.Z += TWOPI;

}

Vector3 Model::GetPosition()
{
	return m_Position;
}

Vector3 Model::GetRotation()
{
	return m_Rotation;
}

//void Model::SetBallPositionDependent( Ball* m_ball )
//{
//	m_BallDependent = m_ball;
//}

void Model::ChangeOrigin( Vector3 NewOrigin )
{
	this->newOrigin = NewOrigin;

	for (int i = 0; i <  FinalList.size(); i++)
	{
		Vector3 temp  = FinalList[i].Position;
		temp += NewOrigin;
		FinalList[i] = Vertex(temp,FinalList[i].s,FinalList[i].t,FinalList[i].Normal);
	}
}

