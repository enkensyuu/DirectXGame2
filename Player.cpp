#include"Player.h"
#include <cassert>

Matrix4 Mat_Identity()
{
	//単位行列
	Matrix4 matIdentity;
	matIdentity.m[0][0] = 1;
	matIdentity.m[1][1] = 1;
	matIdentity.m[2][2] = 1;
	matIdentity.m[3][3] = 1;

	return matIdentity;
}

Matrix4 Mat_Translation(Vector3 translation)
{
	//平行移動行列を宣言
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	matTrans.m[3][0] = translation.x;
	matTrans.m[3][1] = translation.y;
	matTrans.m[3][2] = translation.z;
	matTrans.m[3][3] = 1;

	return matTrans;
}

void Player::Initialize(Model* model, uint32_t textuerhandle)
{
	// NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = textuerhandle;

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
}

void Player::Update()
{
	// キャラクターの移動ベクトル
	Vector3 move = { 0,0,0 };

	const float kPlayerSpeed = 0.2f;

	if (input_->PushKey(DIK_LEFT))
	{
		move.x -= kPlayerSpeed;
	}
	else if (input_->PushKey(DIK_RIGHT))
	{
		move.x += kPlayerSpeed;
	}
	else if (input_->PushKey(DIK_UP))
	{
		move.y += kPlayerSpeed;
	}
	else if (input_->PushKey(DIK_DOWN))
	{
		move.y -= kPlayerSpeed;
	}

	//　座標移動(ベクトルの加算)
	worldTransform_.translation_ += move;

	// 移動限界座標
	const float kMoveLimitX = 35;
	const float kMoveLimitY = 19;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	// 行列更新
	worldTransform_.matWorld_ = Mat_Identity();
	worldTransform_.matWorld_ *= Mat_Translation(worldTransform_.translation_);

	worldTransform_.TransferMatrix();

	// デバッグ用表示
	debugText_->SetPos(50, 50);
	debugText_->Printf("Player:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
}

void Player::Draw(ViewProjection& viewProjection_)
{
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}
