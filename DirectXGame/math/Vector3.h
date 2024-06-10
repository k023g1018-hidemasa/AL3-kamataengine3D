#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;
	
	Vector3 operator+=(const Vector3& obj);

};
//二項演算子というもの+-*/当たりの演算子こいつらは外に出す
Vector3 operator+(const Vector3& objA,const Vector3&objB );