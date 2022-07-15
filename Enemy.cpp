#include"Enemy.h"
#include <cassert>
#include"Procession.h"

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
	worldTransform_.translation_ = {3,0,100};
}

void Enemy::Update()
{
	// キャラクターの移動ベクトル
	Vector3 move = { 0,0,0 };

	const float kEnemySpeed = 0.2f;

	move.z -= kEnemySpeed;

	//　座標移動(ベクトルの加算)
	worldTransform_.translation_ += move;

	// 行列更新
	worldTransform_.matWorld_ = Mat_Identity();
	worldTransform_.matWorld_ = MatWorld(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection_)
{
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}
