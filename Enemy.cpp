#include"Enemy.h"
#include <cassert>
#include"Procession.h"

void Enemy::Approach()
{
	Vector3 ApproachSpeed = { 0,0,-0.2f };

	// 移動(ベクトルを加算)
	worldTransform_.translation_ += ApproachSpeed;
	// 規定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f)
	{
		phase_=Phase::Leave;
	}
}

void Enemy::Leave()
{
	Vector3 LeaveSpeed = { 0.2f,0,0.2f };

	// 移動(ベクトルを加算)
	worldTransform_.translation_ += LeaveSpeed;
}

void Enemy::Initialize(Model* model, uint32_t textuerHandle)
{
	// NULLポインタチェック
	assert(model);

	model_ = model;

	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("lui-zi.jpg");

	// シングルトンインスタンスを取得する
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();

	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = { 0,3,100 };
}

void Enemy::Update()
{
	switch (phase_)
	{
	case Phase::Approach:
	default:
		Approach();
		break;
	case Phase::Leave:
		Leave();
		break;
	}

	// キャラクターの移動ベクトル
	//Vector3 move = { 0,0,0 };
	//
	//const float kEnemySpeed = 0.2f;
	//
	//move.z -= kEnemySpeed;
	//
	////　座標移動(ベクトルの加算)
	//worldTransform_.translation_ += move;

	// 行列更新
	worldTransform_.matWorld_ = Mat_Identity();
	worldTransform_.matWorld_ = MatWorld(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection_)
{
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}
