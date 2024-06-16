#include "CameraController.h"
#include"Player.h"
#include <algorithm>


//p14
Vector3 Lerp(const Vector3& a, const Vector3& b, float t) {
	Vector3 resurt{};
	// return{ t * a.x + (1.0f - t) * b.x ,t * a.y + (1.0f - t) * b.y };//かっこで囲ったら行けるらしい
	resurt.x = t * a.x + (1.0f - t) * b.x;
	resurt.y = t * a.y + (1.0f - t) * b.y;
	resurt.z = t * a.z + (1.0f - t) * b.z;
	return resurt;
}



void CameraController::Initalize(ViewProjection* viewprojection) {

     viewProjection_ = viewprojection;

}

void CameraController::Update() {
	//追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();//数字が変わってない//カメラも中心じゃなくブロックに向いてる気がするからもしかしたらターゲットの問題？
	//追従対象とオフセットからカメラの座標を計算//演算子のエラーもし動かなかったらここが原因かも（VECTOR3）
	targetCoordinates.x = targetWorldTransform.translation_.x + targetOffset_.x;
	targetCoordinates.y = targetWorldTransform.translation_.y + targetOffset_.y;
	targetCoordinates.z = targetWorldTransform.translation_.z + targetOffset_.z;

	//座標保管によりちゅったり追従
	viewProjection_->translation_ = Lerp(viewProjection_->translation_, targetCoordinates, kInterpolationRate);


	//viewProjection_->translation_.x = targetWorldTransform.translation_.x + targetOffset_.x;
	//viewProjection_->translation_.y = targetWorldTransform.translation_.y + targetOffset_.y;
	//viewProjection_->translation_.z = targetWorldTransform.translation_.z + targetOffset_.z;
	


	//	//移動範囲制限
	viewProjection_->translation_.x = std::max(viewProjection_->translation_.x, movableArea_.left);
	viewProjection_->translation_.x = std::min(viewProjection_->translation_.x, movableArea_.right);
	viewProjection_->translation_.y = std::max(viewProjection_->translation_.y, movableArea_.bottom);
	viewProjection_->translation_.y = std::min(viewProjection_->translation_.y, movableArea_.top);
	//行列を更新する
	viewProjection_->UpdateMatrix();

}

void CameraController::Reset() {
	//追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	//追従対象とオフセットからカメラの座標を計算//xyzの演算子エラーを直すかxyzを核果
	viewProjection_->translation_.x = targetWorldTransform.translation_.x + targetOffset_.x;
	viewProjection_->translation_.y = targetWorldTransform.translation_.y + targetOffset_.y;
	viewProjection_->translation_.z = targetWorldTransform.translation_ .z+ targetOffset_.z;


}
