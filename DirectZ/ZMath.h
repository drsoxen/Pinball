struct Vector2
{
	float X, Y;
	Vector2(){ X = Y = 0.0f; }
	Vector2(float X, float Y) 
	{
		this->X = X;
		this->Y = Y;
	}

	Vector2 Normalize()
	{
		Vector2 norm( 0, 0 );  
		float temp = sqrtf(powf(X,2) + powf(Y,2));
		if (temp == 0) return *this;
		norm.X = X / temp;
		norm.Y = Y / temp;
		return norm;		
	}

	Vector2 Lerp(Vector2 pV1, Vector2 pV2,float s)
	{
		Vector2 Out;
		Out.X = pV1.X + s * (pV2.X - pV1.X);
		Out.Y = pV1.Y + s * (pV2.Y - pV1.Y);
		return Out;
	}

	Vector2 Scale(Vector2 pV, float s )
	{
		Vector2 Out;
		Out.X = pV.X * s;
		Out.Y = pV.Y * s;
		return Out;
	}

	Vector2 Minimize(Vector2 pV1,  Vector2 pV2 )
	{
		Vector2 Out;
		Out.X = pV1.X < pV2.X ? pV1.X : pV2.X;
		Out.Y = pV1.Y < pV2.Y ? pV1.Y : pV2.Y;
		return Out;
	}

	Vector2 Maximize(Vector2 pV1, Vector2 pV2 )
	{
		Vector2 Out;
		Out.X = pV1.X > pV2.X ? pV1.X : pV2.X;
		Out.Y = pV1.Y > pV2.Y ? pV1.Y : pV2.Y;
		return Out;
	}

	float Dot(Vector2 toDot)	{return (this->X * toDot.X) + (this->Y * toDot.Y);}
	float Length(Vector2 pV){return sqrtf(pV.X * pV.X + pV.Y * pV.Y);}
	static Vector2 Zero(){return Vector2(0,0);}

	Vector2 operator+(Vector2 toAdd){return Vector2(this->X + toAdd.X, this->Y + toAdd.Y);}
	Vector2 operator+(float toAdd){return Vector2(this->X + toAdd, this->Y + toAdd);}
	Vector2 operator+=(float toAdd){return Vector2(this->X += toAdd, this->Y += toAdd);}
	Vector2 operator+=(Vector2 toAdd){return Vector2(this->X += toAdd.X, this->Y += toAdd.Y);}
	Vector2 operator-(Vector2 toSub){return Vector2(this->X - toSub.X, this->Y - toSub.Y);}
	Vector2 operator-(float toSub){return Vector2(this->X - toSub, this->Y - toSub);}
	Vector2 operator-=(float toSub){return Vector2(this->X -= toSub, this->Y -= toSub);}
	Vector2 operator*(Vector2 toMult){return Vector2(this->X * toMult.X, this->Y * toMult.Y);}
	Vector2 operator*(float toMult){return Vector2(this->X * toMult, this->Y * toMult);}
	Vector2 operator*=(float toMult){return Vector2(this->X *= toMult, this->Y *= toMult);}
	Vector2 operator*=(Vector2 toMult){return Vector2(this->X *= toMult.X, this->Y *= toMult.Y);}
	Vector2 operator/(Vector2 toDiv){return Vector2(this->X / toDiv.X, this->Y / toDiv.Y);}
	Vector2 operator/(float toDiv){return Vector2(this->X / toDiv, this->Y / toDiv);}
	Vector2 operator/=(float toDiv){return Vector2(this->X /= toDiv, this->Y /= toDiv);}
	Vector2 operator-(){return Vector2(-this->X, -this->Y);};
};

struct Vector3
{
	float X, Y, Z;
	Vector3(){ X = Y = Z = 0.0f; }
	Vector3(float X, float Y, float Z)
	{
		this->X = X;
		this->Y = Y;
		this->Z = Z;
	}

	Vector3 Lerp(Vector3 pV1, Vector3 pV2, float s )
	{
		Vector3 pOut;
		pOut.X = pV1.X + s * (pV2.X - pV1.X);
		pOut.Y = pV1.Y + s * (pV2.Y - pV1.Y);
		pOut.Z = pV1.Z + s * (pV2.Z - pV1.Z);
		return pOut;
	}

	Vector3 Cross(Vector3 pV1, Vector3 pV2 )
	{
		Vector3 v;
		Vector3 pOut;

		v.X = pV1.Y * pV2.Z - pV1.Z * pV2.Y;
		v.Y = pV1.Z * pV2.X - pV1.X * pV2.Z;
		v.Z = pV1.X * pV2.Y - pV1.Y * pV2.X;

		pOut = v;
		return pOut;
	}

	float Dot(Vector3 pV1, Vector3 pV2 ){return pV1.X * pV2.X + pV1.Y * pV2.Y + pV1.Z * pV2.Z;}
	float Length(Vector3 pV){return sqrtf(pV.X * pV.X + pV.Y * pV.Y + pV.Z * pV.Z);}
	static Vector3 Zero(){return Vector3(0,0,0);}

	Vector3 operator+(Vector2 toAdd){return Vector3(this->X + toAdd.X, this->Y + toAdd.Y,this->Z);}
	Vector3 operator+(Vector3 toAdd){return Vector3(this->X + toAdd.X, this->Y + toAdd.Y,this->Z + toAdd.Z);}
	Vector3 operator+(float toAdd){return Vector3(this->X + toAdd, this->Y + toAdd, this->Z + toAdd);}
	Vector3 operator+=(Vector3 toAdd){return Vector3(this->X += toAdd.X, this->Y += toAdd.Y,this->Z += toAdd.Z);}
	Vector3 operator+=(float toAdd){return Vector3(this->X += toAdd, this->Y += toAdd, this->Z += toAdd);}
	Vector3 operator+=(Vector2 toAdd){return Vector3(this->X += toAdd.X, this->Y += toAdd.Y,this->Z);}
	Vector3 operator-(Vector2 toSub){return Vector3(this->X - toSub.X, this->Y - toSub.Y,this->Z);}
	Vector3 operator-(Vector3 toSub){return Vector3(this->X - toSub.X, this->Y - toSub.Y, this->Z - toSub.Z);}
	Vector3 operator-(float toSub){return Vector3(this->X - toSub, this->Y - toSub, this->Z - toSub);}
	Vector3 operator-=(float toSub){return Vector3(this->X -= toSub, this->Y -= toSub, this->Z -= toSub);}
	Vector3 operator-=(Vector2 toSub){return Vector3(this->X -= toSub.X, this->Y -= toSub.Y,this->Z);}
	Vector3 operator-=(Vector3 toSub){return Vector3(this->X -= toSub.X, this->Y -= toSub.Y, this->Z -= toSub.Z);}
	Vector3 operator*(Vector3 toMult){return Vector3(this->X * toMult.X, this->Y * toMult.Y, this->Z * toMult.Z);}
	Vector3 operator*(float toMult){return Vector3(this->X * toMult, this->Y * toMult, this->Z * toMult);}
	Vector3 operator*=(float toMult){return Vector3(this->X *= toMult, this->Y *= toMult, this->Z *= toMult);}
	Vector3 operator/(Vector3 toDiv){return Vector3(this->X / toDiv.X, this->Y / toDiv.Y, this->Z / toDiv.Z);}
	Vector3 operator/(float toDiv){return Vector3(this->X / toDiv, this->Y / toDiv, this->Z / toDiv);}
	Vector3 operator/=(float toDiv){return Vector3(this->X /= toDiv, this->Y /= toDiv, this->Z /= toDiv);}
	Vector3 operator-(){return Vector3(-this->X, -this->Y, -this->Z	);}

#ifdef __D3DX9MATH_H__
	Vector3 operator=(D3DXVECTOR3 toEqu){return Vector3(this->X = toEqu.x, this->Y = toEqu.y, this->Z = toEqu.z);}
#endif
};

struct Colour 
{
	float r, g, b, a;
	Colour(){r = g = b = a = 0;}
	Colour(float r, float g, float b, float a) 
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
	static Colour Zero(){return Colour(0,0,0,0);}
	static Colour One(){return Colour(1,1,1,1);}
};

struct Vertex
{
	Vector3 Position;
	float s, t;
	Vector3 Normal;
	Colour Diffuse, Specular;
	Vertex()
	{
		s = t = 0.0f; 
		this->Diffuse = Colour::One();
		this->Specular = Colour::One();
	}

	Vertex(Vector3 Position, float s, float t,Vector3 Normal) 
	{
		this->Position = Position;
		this->s = s;
		this->t = t;
		this->Normal = Normal;
		this->Diffuse = Colour::One();
		this->Specular = Colour::One();
	}

	Vertex(Vector3 Position, float s, float t,Vector3 Normal,Colour Diffuse, Colour Specular) 
	{
		this->Position = Position;
		this->s = s;
		this->t = t;
		this->Normal = Normal;
		//this->Diffuse = Diffuse;
		//this->Specular = Specular;
		this->Diffuse = Colour::One();
		this->Specular = Colour::One();
	}
};

struct Light
{
	Light()	{memset(this, 0, sizeof(Light));}

	Vector3 pos;
	float range;

	Vector3 dir;
	int lightType;

	Colour ambient;
	Colour diffuse;
	Colour specular;

	Vector3 att;
	float spotPow;

	int lightOn;
	Vector3 pad;
};

enum MouseButtons
{
	LEFTBUTTON,
	RIGHTBUTTON,
	CENTERBUTTON
};
