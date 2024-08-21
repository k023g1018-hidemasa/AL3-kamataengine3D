#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;

	Vector3 operator+=(const Vector3& obj);
	Vector3 operator-=(const Vector3& obj);
};
// 二項演算子というもの+-*/当たりの演算子こいつらは外に出す
Vector3 operator+(const Vector3& objA, const Vector3& objB);
Vector3 operator-(const Vector3& objA, const Vector3& objB);
Vector3 operator*(const Vector3& objA, const float& s);
Vector3 operator*(const float& s, const Vector3& objA);