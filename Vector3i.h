#pragma once
class Vector3i
{
public:
	Vector3i();
	Vector3i(int x, int y, int z);
	void setPos(int x, int y, int z);

	int& operator[](const int i);
	int operator[](const int i) const;
	const int* getPos() const;
	float norm() const;

private:
	int pos[3];
};

Vector3i operator+(const Vector3i& v1, const Vector3i& v2);
Vector3i operator-(const Vector3i& v1, const Vector3i& v2);
Vector3i operator*(const int s, const Vector3i& vec3);
Vector3i operator*(const Vector3i& vec3, const int s);
bool operator==(const Vector3i& v1, const Vector3i& v2);
int dotProduct(const Vector3i& v1, const Vector3i& v2);