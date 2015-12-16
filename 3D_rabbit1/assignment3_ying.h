/*
CPS592 Interactive Media
Assignment3
assignment3_ying.h
Ying Zhou
03/16/2015
*/

using namespace std;

struct Vertex
{
public:
	float x;
	float y;
	float z;

	Vertex(){ x = 0; y = 0; z = 0; }
	Vertex(float mx, float my, float mz){ x = mx; y = my; z = mz; }
	~Vertex(){}
};

struct Triangle
{
public:
	int v1;
	int v2;
	int v3;

	Triangle(){ v1 = 0; v2 = 0; v3 = 0; }
	Triangle(int mx, int my, int mz){ v1 = mx; v2 = my; v3 = mz; }
	~Triangle(){}
};

struct Mat
{
public:
	float elem[4][4];

	/*Mat times another Mat function*/
	const Mat operator *(const Mat& right) const
	{
		Mat result;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j< 4; j++)
			{
				result.elem[i][j] = elem[i][0] * right.elem[0][j] + elem[i][1] * right.elem[1][j] + elem[i][2] * right.elem[2][j] + elem[i][3] * right.elem[3][j];
			}
		}
		return result;
	}
	/*Mat times a vertex and return another vertex*/
	const Vertex operator *(const Vertex& right) const
	{
		Vertex result;
		float help[4];
		float w = 1.0f;
		for (int i = 0; i < 4; i++)
		{
			help[i] = elem[i][0] * right.x + elem[i][1] * right.y + elem[i][2] * right.z + elem[i][3] * w;
		}
		result.x = help[0];
		result.y = help[1];
		result.z = help[2];
		return result;
	}
	//Initial
	Mat()
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
				this->elem[i][j] = 0.0f;
		}
	}
};