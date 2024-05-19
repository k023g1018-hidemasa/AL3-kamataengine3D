#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include"ViewProjection.h"
#include<cassert>



class  Skydome {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, ViewProjection* viewProjection);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();




private:

	ViewProjection* viewProjection_ = nullptr;
	uint32_t textureHandle_ = 0u;
	//ワールド返還データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_=nullptr;

};

 
