#include"Enemy.h"
#include <cassert>
#include"Procession.h"
#include"Player.h"
#include<cmath>

void Enemy::Move()
{
	ChangeTimeX--;
	ChangeTimeY--;

	Vector3 MoveSpeedX = { 0.1f,0,0 };
	Vector3 MoveSpeedY = { 0,0.1f,0 };

	// 規定の位置に到達したら離脱
	// 移動限界座標
	const float kMoveLimitX = 13.0f;
	const float kMoveLimitY = 7.0f;

	// 範囲を超えない処理
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);

	// 範囲を超えない処理
	if (ChangeSpeedFlagX == 1 && ChangeTimeX <= 0)
	{
		ChangeSpeedFlagX = 0;
		ChangeTimeX = 240;
	}

	if (ChangeSpeedFlagY == 1 && ChangeTimeY <= 0)
	{
		ChangeTimeY = 240;
		ChangeSpeedFlagY = 0;
	}

	if (ChangeSpeedFlagX == 0)
	{
		worldTransform_.translation_ += MoveSpeedX;
	}

	if (ChangeSpeedFlagY == 0)
	{
		worldTransform_.translation_ += MoveSpeedY;
	}

	if (ChangeSpeedFlagX == 0 && ChangeTimeX <= 0)
	{
		ChangeTimeX = 240;
		ChangeSpeedFlagX = 1;
	}

	if (ChangeSpeedFlagY == 0 && ChangeTimeY <= 0)
	{
		ChangeSpeedFlagY = 1;
		ChangeTimeY = 240;
	}

	if (ChangeSpeedFlagX == 1)
	{
		worldTransform_.translation_ -= MoveSpeedX;
	}

	if (ChangeSpeedFlagY == 1)
	{
		worldTransform_.translation_ -= MoveSpeedY;
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

void Enemy::LevelUp()
{

}

void Enemy::Initialize(Model* model, uint32_t textuerHandle)
{
	// NULLポインタチェック
	assert(model);

	model_ = model;

	hp = 200;

	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("lui-zi.jpg");

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();

	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = { 0,0,40 };

	// キャラクター攻撃処理
	ApproachInitialize();
}

void Enemy::Update()
{

	switch (phase_)
	{
	case Phase::Move:
	default:
		Move();
		break;
	case Phase::LevelUp:
		LevelUp();
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

	// デバッグ用表示
	debugText_->SetPos(0, 0);
	debugText_->Printf("EnemyHP=%d", hp);
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
	Vector3 velocity(0, 0, kBulletSpeed);

	Vector3 PlayerVelocity = player_->GetWorldPosition();
	Vector3 EnemyVelocity = this->GetWorldPosition();

	Vector3 vector = Vectornorm(PlayerVelocity, EnemyVelocity);

	Length(vector);

	Normalize(vector);

	vector *= -kBulletSpeed;
	velocity = vector;

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

void Enemy::OnCollision()
{
	hp -= 10;
}

float Enemy::Radius()
{
	return radius_;
}

float Enemy::Hp()
{
	return hp;
}
