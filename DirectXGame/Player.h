#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include"ViewProjection.h"

///自クラス
class Player {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Player();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();
	/// <summary>
	/// 
	/// </summary>
	/// <param name="model">モデルのポインタ</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Initialize(
		Model*model,uint32_t textureHandle,ViewProjection* viewProjection
	
	);
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();


private://変数は変えるよってなったら変わるからプライベートにして変更を阻止
	

	/// <summary>
	///ワールド返還
	/// </summary>
	WorldTransform worldTransform_;
	
	/// <summary>
	/// モデル//一旦預かるだけ
	/// </summary>
	Model* model_ = nullptr;
	ViewProjection* viewProjection_ = nullptr;
	/// <summary>
	/// テクスチャハンドル
	/// </summary>
	uint32_t textureHandle_ = 0u;

};
