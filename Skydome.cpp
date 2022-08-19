#include"Skydome.h"
#include <cassert>
#include"Procession.h"

void Skydome::Initialize(Model* model)
{
	//NULLポインタチェック
	assert(model);
	model_ = model;

	//ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.scale_ = { 100,100,100 };
	worldTransform_.translation_ = { 0,0,0 };
}

void Skydome::Update()
{
	// 行列更新
	worldTransform_.matWorld_ = Mat_Identity();
	worldTransform_.matWorld_ = MatWorld(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void Skydome::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}
