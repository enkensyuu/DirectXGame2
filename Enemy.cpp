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

	// �K��̈ʒu�ɓ��B�����痣�E
	// �ړ����E���W
	const float kMoveLimitX = 13.0f;
	const float kMoveLimitY = 7.0f;

	// �͈͂𒴂��Ȃ�����
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);

	// �͈͂𒴂��Ȃ�����
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

	// ���˃^�C�}�[�J�E���g�_�E��
	FireTimer--;
	// �w�莞�ԂɒB����
	if (FireTimer <= 0)
	{
		// �e�𔭎�
		Fire();
		// ���˃^�C�}�[��������
		FireTimer = kFireInterval;
	}
}

void Enemy::LevelUp()
{

}

void Enemy::Initialize(Model* model, uint32_t textuerHandle)
{
	// NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;

	hp = 200;

	// �e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("lui-zi.jpg");

	// �V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();

	// �����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = { 0,0,40 };

	// �L�����N�^�[�U������
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

	// �f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet)
		{
			return bullet->IsDead();
		}
	);

	// �L�����N�^�[�̈ړ��x�N�g��
	//Vector3 move = { 0,0,0 };
	//
	//const float kEnemySpeed = 0.2f;
	//
	//move.z -= kEnemySpeed;
	//
	////�@���W�ړ�(�x�N�g���̉��Z)
	//worldTransform_.translation_ += move;

	// �s��X�V
	worldTransform_.matWorld_ = Mat_Identity();
	worldTransform_.matWorld_ = MatWorld(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();

	// �e�X�V
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Update();
	}

	// �f�o�b�O�p�\��
	debugText_->SetPos(0, 0);
	debugText_->Printf("EnemyHP=%d", hp);
}

void Enemy::Draw(ViewProjection& viewProjection_)
{
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	// �e�`��
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection_);
	}
}

void Enemy::Fire()
{
	assert(player_);

	// �e�̑��x
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	Vector3 PlayerVelocity = player_->GetWorldPosition();
	Vector3 EnemyVelocity = this->GetWorldPosition();

	Vector3 vector = Vectornorm(PlayerVelocity, EnemyVelocity);

	Length(vector);

	Normalize(vector);

	vector *= -kBulletSpeed;
	velocity = vector;

	// �e�𐶐����A������
	std::unique_ptr < EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	// �e��o�^����
	bullets_.push_back(std::move(newBullet));
}

void Enemy::ApproachInitialize()
{
	// ���˃^�C�}�[��������
	FireTimer = kFireInterval;
}

Vector3 Enemy::GetWorldPosition()
{
	// ���[���h���W������ϐ�
	Vector3 worldPos;
	// ���[���h�s��̕��s�ړ��������擾(���[���h���W)
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
