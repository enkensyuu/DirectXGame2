#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include"ViewProjection.h"
#include "DebugText.h"
#include"EnemyBullet.h"


class Player;

class RailCamera;

/// <summary>
/// 敵
/// </summary>
class Enemy
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

	// 接近フェーズ行動
	void Move();

	//　離脱フェーズ行動
	void LevelUp();

	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();

	// 発射間隔
	static const int kFireInterval = 60;

	// 接近フェーズ初期化
	void ApproachInitialize();

	void SetPlayer(Player* player) { player_ = player; }

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	// 弾リストを取得
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	float Radius();

	float Hp();

private:
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// 行動フェーズ
	enum class Phase
	{
		Move,	//	接近する
		LevelUp,		//	離脱する
	};

	// フェーズ
	Phase phase_ = Phase::Move;

	// 弾
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	// 発射タイマー
	int32_t FireTimer = 0;

	// 自キャラ
	Player* player_ = nullptr;

	const float radius_ = 1.0f;

	bool ChangeSpeedFlagX = 0;

	bool ChangeSpeedFlagY = 0;

	int32_t ChangeTimeX = 240;
	int32_t ChangeTimeY = 240;

	int32_t hp = 200;
};