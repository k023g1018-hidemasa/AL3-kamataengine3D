#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;
	
	Vector3 operator+=(const Vector3& obj) {//機能の拡張をしてるからvectorの中に入れてヨシ
		Vector3 num{};
	num.x = x + obj.x;
	num.y = y + obj.y;
	num.z = z+ obj.z;
	
	return num;
     }
    
};