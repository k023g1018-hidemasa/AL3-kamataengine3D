#include"Vector3.h"

Vector3 operator+(const Vector3& objA, const Vector3& objB) { return Vector3(objA) += objB; }

Vector3 operator*(const Vector3& objA, const Vector3& objB) { 
Vector3 num{};
	num.x = objA.x * objB.x;
    num.y = objA.y * objB.y;
	num.z = objA.z * objB.z;
	return num;


}

Vector3 Vector3::operator+=(const Vector3& obj) 
	{ // 機能の拡張をしてるからvectorの中に入れてヨシ
		// ｃｐｐでやらないとインクルードしたときに多重宣言になってしまうobj
		Vector3 num{};
		num.x += obj.x;
		num.y += obj.y;
		num.z += obj.z;

		return num;
	}

