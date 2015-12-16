/*
CPS592 Interactive Media
Assignment4
assignment4_ying.h
Ying Zhou
03/24/2015
*/

using namespace std;

struct Color
{
public:
	GLfloat R;
	GLfloat G;
	GLfloat B;

	Color(){ R = 0.0; G = 0.0; B = 0.0; }
	Color(GLfloat c1, GLfloat c2, GLfloat c3){ R = c1; G = c2; B = c3; }
	~Color(){}

	const Color operator * (const Color& right) const
	{
		Color result;
		result.R = R * right.R;
		result.G = G * right.G;
		result.B = B * right.B;
		return result;
	}

	const Color operator + (const Color& right) const
	{
		Color result;
		result.R = R + right.R;
		result.G = G + right.G;
		result.B = B + right.B;
		return result;
	}

	const Color operator - (const Color& right) const
	{
		Color result;
		result.R = R - right.R;
		result.G = G - right.G;
		result.B = B - right.B;
		return result;
	}

	const Color operator * (const float right) const
	{
		Color result;
		result.R = R * right;
		result.G = G * right;
		result.B = B * right;
		return result;
	}
};

struct Vertex
{
public:
	float x;
	float y;
	float z;
	float unit_x;
	float unit_y;
	float unit_z;

	Vertex(){ x = 0; y = 0; z = 0; }
	Vertex(float mx, float my, float mz){ x = mx; y = my; z = mz; }
	~Vertex(){}

	const Vertex operator - (const Vertex& right) const
	{
		Vertex result;
		result.x = x - right.x;
		result.y = y - right.y;
		result.z = z - right.z;
		return result;
	}

	const Vertex operator + (const Vertex& right) const
	{
		Vertex result;
		result.x = x + right.x;
		result.y = y + right.y;
		result.z = z + right.z;
		return result;
	}

	const Vertex operator * (const float right) const
	{
		Vertex result;
		result.x = x * right;
		result.y = y * right;
		result.z = z * right;
		return result;
	}

	void unitCalculate()			//calculate the unit vertors
	{
		float help = x * x + y * y + z * z;
		unit_x = x / sqrt(help);
		unit_y = y / sqrt(help);
		unit_z = z / sqrt(help);
	}

	float dotProduct(const Vertex& right) const
	{
		float result;
		result = unit_x * right.unit_x + unit_y * right.unit_y + unit_z * right.unit_z;
		return result;
	}

	Vertex cross(const Vertex& right)
	{
		Vertex result;
		result.x = y * right.z - right.y * z;
		result.y = z * right.x - right.z * x;
		result.z = x * right.y - right.x * y;

		return result;
	}

};

struct Triangle
{
public:
	int v1;
	int v2;
	int v3;
	Vertex normalValue;

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