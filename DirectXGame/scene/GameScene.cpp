#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

void Expansion()






GameScene::GameScene() {}

GameScene::~GameScene() {

	delete blockModel_;

	for (WorldTransform* worldTransformBlock : worldTransformBlocks_) {
		delete worldTransformBlock;
	}
	worldTransformBlocks_.clear();

}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	blockModel_ = Model::Create();
	BlockTextureHandle_ = TextureManager::Load("cube/cube.jpg");

	//要素数,ここ変えれば配置する数が変わる
	const uint32_t kNumBlockHorizontal = 20;
	//ブロック一個分の横幅、ブロック自体の大きさみたいなもの
	const float kBlockwidth = 2.0f;
	//要素数を変更する、可変長は最初はゼロだからつ要素を作っている（ｎｗＵ）
	worldTransformBlocks_.resize(kNumBlockHorizontal);

	//キューブの生成
	for (uint32_t i = 0; i < kNumBlockHorizontal; ++i) {
	
		worldTransformBlocks_[i] = new WorldTransform();//黄色文字
		worldTransformBlocks_[i]->Initialize();
		worldTransformBlocks_[i]->translation_.x = kBlockwidth * i;
		worldTransformBlocks_[i]->translation_.y = 0.0f;

	}

}

void GameScene::Update() {

	//ブロックの更新
	for (WorldTransform* worldTransformBlock : worldTransformBlocks_) {
	




		worldTransformBlock->matWorld_;
		//定数バッファに転送
		worldTransformBlock->TransferMatrix();
	
	}


}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
