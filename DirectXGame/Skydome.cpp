#include "Skydome.h"

void Skydome::Initialize(Model* model, ViewProjection* viewProjection) {

	assert(model);
	model_ = model;
	//textureHandle_ = textureHandle;//この辺はセッタのやつ
	worldTransform_.Initialize();//これは関数を呼んでる？なにしてんだ？
    //行列をバッファに
	viewProjection_ = viewProjection;
}
void Skydome::Update() {

    worldTransform_.TransferMatrix();

}

void Skydome::Draw() {
	//ドロうの中身の定義
	model_->Draw(worldTransform_
		,*viewProjection_//ここだけ配列（ポインタにしてんのはなんで）多分ビュープロが行列になってるから？
	);
}

