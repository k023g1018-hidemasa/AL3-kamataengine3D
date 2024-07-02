#include "Vector3.h"

Vector3 Vector3::operator+=(const Vector3& obj) { // 機能の拡張をしてるからvectorの中に入れてヨシ
	// ｃｐｐでやらないとインクルードしたときに多重宣言になってしまう
	this->x += obj.x;
	this->y += obj.y;
	this->z += obj.z;

	return (*this);
}
Vector3 operator+(const Vector3& objA, const Vector3& objB) { 
	return {
	    objA.x + objB.x,
	    objA.y + objB.y,
	    objA.z + objB.z,
	};
}
Vector3 operator*(const Vector3& objA, const float& s) {
	Vector3 num{};
	num.x = objA.x * s;
	num.y = objA.y * s;
	num.z = objA.z * s;
	return num;
}

Vector3 operator*(const float& s, const Vector3& objA) {
	Vector3 num{};
	num.x = objA.x * s;
	num.y = objA.y * s;
	num.z = objA.z * s;
	return num;
}
