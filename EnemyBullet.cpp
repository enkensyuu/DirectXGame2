#include"EnemyBullet.h"
#include <cassert>
#include"Procession.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	// NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;

	// �e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("black.png");

	// �����Ŏ󂯎�������x�������o�ϐ��ɑ��
	velocity_ = velocity;

	worldTransform_.Initialize();

	// �����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = position;
}

void EnemyBullet::Update()
{
	worldTransform_.matWorld_ = Mat_Identity();
	worldTransform_.matWorld_ = MatWorld(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// ���W���ړ�������(1�t���[�����̈ړ��ʂ𑫂�����)
	worldTransform_.translation_ -= velocity_;

	worldTransform_.TransferMatrix();

	// ���Ԍo�߂Ńf�X
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}
}

void EnemyBullet::Draw(const ViewProjection& viewProjection)
{
	// ���f���̕`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
