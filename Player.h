#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include"ViewProjection.h"
#include "DebugText.h"
#include"PlayerBullet.h"
#include<memory>
#include<list>

class RailCamera;

/// <summary>
/// 自キャラ
/// </summary>
class Player
{
public:
	/// <summary>
	///初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textuerHandle">テクスチャハンドル</param>
	void Initialize(Model* model, uint32_t textuerHandle);

	/// <summary>
	///更新
	/// </summary>
	void Update();

	/// <summary>
	///描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(ViewProjection& viewProjection_);

	void Rotate();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	// 弾リストを取得
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	float Radius();

	void SetCamera(WorldTransform* camera) { worldTransform_.parent_ = camera; }

private:
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// 弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	const float radius_ = 1.0f;
};