#include"Enemy.h"
#include <cassert>
#include"Procession.h"
#include"Player.h"

void Enemy::Approach()
{
	Vector3 ApproachSpeed = { 0,0,-0.2f };

	// 移動(ベクトルを加算)
	worldTransform_.translation_ += ApproachSpeed;
	// 規定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f)
	{
		phase_ = Phase::Leave;
	}

	// 発射タイマーカウントダウン
	FireTimer--;
	// 指定時間に達した
	if (FireTimer <= 0)
	{
		// 弾を発射
		Fire();
		// 発射タイマーを初期化
		FireTimer = kFireInterval;
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
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();

	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = { 0,3,100 };

	// キャラクター攻撃処理
	ApproachInitialize();
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

	// デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet)
		{
			return bullet->IsDead();
		}
	);

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

	// 弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Update();
	}
}

void Enemy::Draw(ViewProjection& viewProjection_)
{
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	// 弾描画
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection_);
	}
}

void Enemy::Fire()
{
	assert(player_);
	// 弾の速度
	const float kBulletSpeed = 1.0f;

	Vector3 Playervelocity = player_->GetWorldPosition();
	Vector3 Enemyvelocity = GetWorldPosition();
	Vector3 velocity = Playervelocity -= Enemyvelocity;

	// 弾を生成し、初期化
	std::unique_ptr < EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	// 弾を登録する
	bullets_.push_back(std::move(newBullet));
}

void Enemy::ApproachInitialize()
{
	// 発射タイマーを初期化
	FireTimer = kFireInterval;
}

Vector3 Enemy::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;
	return worldPos;
}

float Enemy::length() const
{
	return std::sqrt(x * x + y * y + z * z);
}

Vector3 Enemy::Normalize()
{
	float len = length();
	if (len != 0)
	{
		return*this /= len;
	}
	return*this;
}

