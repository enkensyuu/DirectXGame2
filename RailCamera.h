#pragma once
#include "WorldTransform.h"
#include"ViewProjection.h"
#include "DebugText.h"

/// <summary>
/// レールカメラ
/// </summary>
class RailCamera
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& position, const Vector3& rotation);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// ビュープロジェクション
	/// </summary>
	/// <returns></returns>
	const ViewProjection& GetViewProjection() { return viewProjection_; }

	// ワールド行列を取得
	WorldTransform* GetWorldMatrix() { return &worldTransform_; }

	float Radius();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	float Hp();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	const float radius_ = 1.0f;

	int32_t hp = 150;

	
};