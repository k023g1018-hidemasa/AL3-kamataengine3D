#include "CameraController.h"
#include"Player.h"
#include <algorithm>


//p14
/// <summary>
/// これを使うときは引数１に現在の位置（こっちがカメラの位置）、引数二に目的地（こっちはプレイヤーの位置）、
/// </summary>
Vector3 Lerp(const Vector3& a, const Vector3& b, float t) {//nannkaatamanonakanisikkurikonai 
	Vector3 resurt{};
	// return{ t * a.x + (1.0f - t) * b.x ,t * a.y + (1.0f - t) * b.y };//かっこで囲ったら行けるらしい
	resurt.x = t * b.x + (1.0f - t) * a.x;
	resurt.y = t * b.y + (1.0f - t) * a.y;
	resurt.z = t * b.z + (1.0f - t) * a.z;//らーぷあってんの//らーぷは通ってる
	return resurt;
}



void CameraController::Initalize(ViewProjection* viewprojection) {

     viewProjection_ = viewprojection;

}

void CameraController::Update() {
	//追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();//数字が変わってない//カメラも中心じゃなくブロックに向いてる気がするからもしかしたらターゲットの問題？
	//追従対象とオフセットからカメラの座標を計算//演算子のエラーもし動かなかったらここが原因かも（VECTOR3）
	targetVelosity = target_->GetVelocity();//これは命令だからｃｐｐのみに書く
	targetCoordinates.x = targetWorldTransform.translation_.x + targetOffset_.x+targetVelosity.x*kVelocityBias;//キャラがカメラの後ろに行かなかったらベクター3を見てみて二項演算子の間違いの可能性
	targetCoordinates.y = targetWorldTransform.translation_.y + targetOffset_.y+targetVelosity.y * kVelocityBias;
	targetCoordinates.z = targetWorldTransform.translation_.z + targetOffset_.z+targetVelosity.z * kVelocityBias;

	//座標保管によりちゅったり追従 targetCoordinates
	viewProjection_->translation_ = Lerp(viewProjection_->translation_, targetCoordinates, kInterpolationRate);
	//らーぷに入れる数値も大丈夫でコールディナーと？も最終の座標だからそんなに気にしなくていい
	// 、用は動いてるか動いてないかの問題であってらーぷの中身の数字が変わってるのか
	//ビュートランスが現在のカメラ位置でターゲットがそこにいたいカメラ位置
	//それを使ってラーっプは通してるかららーぷがおかしい？
	// ヴュートランスにはラープを通らないと行けないけどビュートランスとたーっげっとの値が一緒になってるから
	//たーっげっとの値自体は元々大丈夫だったからいいハズ

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
