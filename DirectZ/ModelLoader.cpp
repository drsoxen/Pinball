#include "Common.h"

void MakeNormals(vector<Vertex> & verts, vector<Vector3> & norms)
{
	for(unsigned int i = 0; i < verts.size(); i += 3)
	{
		Vector3 vA, vB;
		vA.X = verts[i].Position.X - verts[i + 1].Position.X;
		vA.Y = verts[i].Position.Y - verts[i + 1].Position.Y;
		vA.Z = verts[i].Position.Z - verts[i + 1].Position.Z;

		vB.X = verts[i + 1].Position.X - verts[i + 2].Position.X;
		vB.Y = verts[i + 1].Position.Y - verts[i + 2].Position.Y;
		vB.Z = verts[i + 1].Position.Z - verts[i + 2].Position.Z;

		Vector3 n;

		n.X = (vA.Y * vB.Z - vA.Z * vB.Y);
		n.Y = (vA.Z * vB.X - vA.X * vB.Z);
		n.Z = (vA.X * vB.Y - vA.Y * vB.X);

		float Normalize = sqrt((n.X*n.X) + (n.Y*n.Y) + (n.Z*n.Z));

		n.X /= Normalize;
		n.Y /= Normalize;
		n.Z /= Normalize;

		norms.push_back(n);
		norms.push_back(n);
		norms.push_back(n);
	}
}

Model * ModelLoader::Load(char* filename)
{
	vector<float> Verts;
	vector<float> Normals;
	vector<float> TexCoord;
	vector<unsigned int> VertsIndices;
	vector<unsigned int> NormIndices;
	vector<unsigned int> TexCoordIndices;
		
	vector<float> FinalVerts;
	vector<float> FinalNormals;
	vector<float> FinalTexCoord;
	vector<Vertex> FinalList;

	float tempPoint = 0;
	int tempInt[3];

	fstream fs;
	fs.open(filename, fstream::in);

	string input = "";

	while(fs>>input) // Read the file until we've run out of file to read
	{
		if (input == "v") // Verts
		{
			for (short loop = 0; loop < 3; loop++)
			{
				fs>>input; // Get a line of input...
				sscanf_s(input.c_str(), "%f", &tempPoint);
				Verts.push_back(tempPoint); // put it in the vector...
			}
		}
		else if (input == "vn") // Normals
		{
			for (short loop = 0; loop < 3; loop++)
			{
				fs>>input; // Get a line of input...
				sscanf_s(input.c_str(), "%f", &tempPoint);
				Normals.push_back(tempPoint); // put it in the vector...
			}
		}
		else if (input == "vt") // Texture Coordinates
		{
			for (short loop = 0; loop < 3; loop++)
			{
				fs>>input; // Get a line of input...
				sscanf_s(input.c_str(), "%f", &tempPoint);
				TexCoord.push_back(tempPoint); // put it in the vector...
			}
		}
		else if (input == "f") // Faces
		{
			for (short loop = 0; loop < 3; loop++)
			{
				fs>>input; // Get a line of input...
				sscanf_s(input.c_str(), "%d %*c %d %*c %d", &tempInt[0], &tempInt[1], &tempInt[2]);
				VertsIndices.push_back(tempInt[0]-1); // Indices for verts
				TexCoordIndices.push_back(tempInt[1]-1); // Indices for texture coordinates
				NormIndices.push_back(tempInt[2]-1); // Indices for normals
			}
		}
	}

	for (unsigned int loop = 0; loop < VertsIndices.size(); loop++)
	{
		FinalVerts.push_back(Verts[VertsIndices[loop]*3]);
		FinalVerts.push_back(Verts[(VertsIndices[loop]*3)+1]);
		FinalVerts.push_back(Verts[(VertsIndices[loop]*3)+2]);
	}

	for (unsigned int loop = 0; loop < NormIndices.size(); loop++)
	{
		FinalNormals.push_back(Normals[NormIndices[loop]*3]);
		FinalNormals.push_back(Normals[(NormIndices[loop]*3)+1]);
		FinalNormals.push_back(Normals[(NormIndices[loop]*3)+2]);
	}

	for (unsigned int loop = 0; loop < TexCoordIndices.size(); loop++)
	{
		FinalTexCoord.push_back(TexCoord[TexCoordIndices[loop]*3]);
		FinalTexCoord.push_back(TexCoord[(TexCoordIndices[loop]*3)+1]);
		FinalTexCoord.push_back(TexCoord[(TexCoordIndices[loop]*3)+2]);
	}

	for (unsigned int i = 0; i < FinalVerts.size(); i+=3)
	{

		Vertex * tempvertex = new Vertex();
		tempvertex->Position = Vector3(FinalVerts[i],FinalVerts[i+1],FinalVerts[i+2]);
		tempvertex->s = FinalTexCoord[i];
		tempvertex->t = FinalTexCoord[i+1];
		tempvertex->Normal = Vector3(FinalNormals[i],FinalNormals[i+1],FinalNormals[i+2]);

		FinalList.push_back(*tempvertex);
		delete tempvertex;
	}

	Model * model = NULL;

	model = new Model(GraphicsDevice::GetInstance()->GetGraphicsDevice());

	//MakeNormals(FinalList, FinalNormals);

	model->FinalVerts = FinalVerts;
	model->FinalNormals = FinalNormals;
	model->FinalTexCoord = FinalTexCoord;
	model->FinalList = FinalList;	
	fs.close();

	return model;
}

LineModel * ModelLoader::LoadLines(char* filename)
{
	vector<float> Verts;
	vector<Vector3> Normals;
	vector<float> TexCoord;
	vector<unsigned int> VertsIndices;

	vector<float> FinalVerts;

	vector<Vector3> FinalList;

	float tempPoint = 0;
	int tempInt[3];

	fstream fs;
	fs.open(filename, fstream::in);

	string input = "";
	int LineIndexIndex = 0;
	int NewLineIndexIndex = 0;
	vector<vector<int>> LineIndex;
	vector<int>tempVec;

	bool switcher = true;
	int loopCount = 0;
	bool FirstTimeThrough = true;
	int countOfVerts = 0;

	while(fs>>input) // Read the file until we've run out of file to read
	{
		if (input == "v") // Verts
		{
			for (short loop = 0; loop < 3; loop++)
			{
				fs>>input; // Get a line of input...
				sscanf_s(input.c_str(), "%f", &tempPoint);
				Verts.push_back(tempPoint); // put it in the vector...
			}
		}
		else if (input == "l") // Faces
		{
			fs>>input; // Get a line of input...
			LineIndex.push_back(tempVec);
			do 
			{						
				LineIndex[LineIndexIndex].push_back(0);
				sscanf_s(input.c_str(), "%d", &tempInt[0]);
				VertsIndices.push_back(tempInt[0]-1); // put it in the vector...
				fs>>input; // Get a line of input...

			}while(input != "#" && !fs.eof());
			LineIndexIndex++;
			tempVec.clear();
		}
		
	}

	for (unsigned int loop = 0; loop < VertsIndices.size(); loop++ )
	{
		FinalVerts.push_back(Verts[VertsIndices[loop]*3]);
		FinalVerts.push_back(Verts[(VertsIndices[loop]*3)+1]);
		FinalVerts.push_back(Verts[(VertsIndices[loop]*3)+2]);

		if(FirstTimeThrough)
		{
			FirstTimeThrough = false;
			continue;
		}

		if(loop-loopCount == LineIndex[NewLineIndexIndex].size()-1)
		{			
			loopCount = loop+1;
			NewLineIndexIndex++;
			FirstTimeThrough = true;
			continue;
		}

		FinalVerts.push_back(Verts[VertsIndices[loop]*3]);
		FinalVerts.push_back(Verts[(VertsIndices[loop]*3)+1]);
		FinalVerts.push_back(Verts[(VertsIndices[loop]*3)+2]);

	}

	for (unsigned int loop = 0; loop < FinalVerts.size(); loop+=3)
	{

		Vector3 * tempvertex = new Vector3();
		tempvertex->X = FinalVerts[loop];
		tempvertex->Y = FinalVerts[loop+1];
		tempvertex->Z = FinalVerts[loop+2];


		FinalList.push_back(*tempvertex);
		delete tempvertex;
	}

	LineModel * lineModel = NULL;

	lineModel = new LineModel(GraphicsDevice::GetInstance()->GetGraphicsDevice());


	lineModel->FinalVerts = FinalVerts;
	lineModel->FinalList = FinalList;	
	fs.close();

	return lineModel;
}


ModelLoader::ModelLoader()
{
	memset(this, 0, sizeof(ModelLoader));
}

ModelLoader::~ModelLoader()
{

}

SpawnPoints* ModelLoader::LoadSpawnPoints( char* filename )
{
	vector<float> Verts;
	fstream fs;
	fs.open(filename, fstream::in);
	string input = "";
	vector<Vector3> FinalList;
	float tempPoint = 0;

	while(fs>>input) // Read the file until we've run out of file to read
	{
		if (input == "v") // Verts
		{
			for (short loop = 0; loop < 3; loop++)
			{
				fs>>input; // Get a line of input...
				sscanf_s(input.c_str(), "%f", &tempPoint);
				Verts.push_back(tempPoint); // put it in the vector...
			}
		}
	}

	for (unsigned int loop = 0; loop < Verts.size(); loop+=3)
	{

		Vector3 * tempvertex = new Vector3();
		tempvertex->X = Verts[loop];
		tempvertex->Y = Verts[loop+1];
		tempvertex->Z = Verts[loop+2];


		FinalList.push_back(*tempvertex);
		delete tempvertex;
	}

	SpawnPoints * spawns = NULL;

	spawns = new SpawnPoints();

	spawns->FinalList = FinalList;

	fs.close();
	return spawns;
}
