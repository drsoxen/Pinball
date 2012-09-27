#include "Common.h"

LineModel::LineModel(ID3D10Device* device)
{
	memset(this, 0, sizeof(LineModel));

	//m_Shader = Shader;
	m_Device = GraphicsDevice::GetInstance()->GetGraphicsDevice();
	SetPosition(Vector3(0,0,0));
	SetRotation(Vector3(0,0,0));
	m_Child = NULL;
	m_Type = DEFAULT;
	newOrigin = Vector3(0,0,0);
	D3DXMatrixIdentity(&rotationMatrix);
	ChangedOrigin = false;
	beenInitted = false;
}

void LineModel::Update()
{

}

void LineModel::Init()
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
		vbd.ByteWidth = sizeof(Vector3) * FinalList.size();
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
	}
}

LineModel::~LineModel()
{
}

void LineModel::Draw(D3DXMATRIX WorldMatrix,D3DXMATRIX ViewMatrix,D3DXMATRIX ProjectionMatrix,float DeltaTime)
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

	ID3D10EffectTechnique* tech = m_Shader->GetTech();
	ID3D10InputLayout* layout = m_Shader->GetVertexLayout();

	D3D10_TECHNIQUE_DESC techDesc;
	tech->GetDesc( &techDesc );
	for(UINT p = 0; p < techDesc.Passes; ++p)
	{
		tech->GetPassByIndex( p )->Apply(0);   
		UINT stride = sizeof(Vector3);
		UINT offset = 0;
		m_Device->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
		m_Device->Draw(m_VertexCount, 0);
	}
}

void LineModel::SetPosition(Vector3 Position)
{
	m_Position.X = Position.X;
	m_Position.Y = Position.Y;
	m_Position.Z = Position.Z;
}

void LineModel::SetRotation(Vector3 Rotation)
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


Vector3 LineModel::GetPosition()
{
	return m_Position;
}

Vector3 LineModel::GetRotation()
{
	return m_Rotation;
}

vector<Vector3>* LineModel::GetVerts()
{
	return &Verticies;
}

void LineModel::ChangeOrigin( Vector3 NewOrigin )
{
	this->newOrigin = NewOrigin;

	for (int i = 0; i <  FinalList.size(); i++)
	{
		FinalList[i] += NewOrigin;
	}
}

void LineModel::RotateVertsAroundOrigin( Vector3 Rotation )
{
	SetRotation(Rotation);
	Vector3 tempOrigin = newOrigin;
	ChangeOrigin(-newOrigin);
	for (int i = 0; i <  FinalList.size(); i++)
	{
		D3DXVECTOR3 outv;
		D3DXVECTOR3 temp (FinalList[i].X,FinalList[i].Y,FinalList[i].Z);
		D3DXMatrixRotationYawPitchRoll(&rotationMatrix,Rotation.Y,Rotation.X,Rotation.Z);
		D3DXVec3TransformCoord(&outv, &temp, &rotationMatrix);
		FinalList[i] = Vector3(outv.x,outv.y,outv.z);
	}
	ChangeOrigin(tempOrigin);
}
