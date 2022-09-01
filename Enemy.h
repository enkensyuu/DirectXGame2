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
/// �G
/// </summary>
class Enemy
{
public:
	/// <summary>
	///������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="textuerHandle">�e�N�X�`���n���h��</param>
	void Initialize(Model* model, uint32_t textuerHandle);

	/// <summary>
	///�X�V
	/// </summary>
	void Update();

	/// <summary>
	///�`��
	/// </summary>
	/// <param name="viewProjection">�r���[�v���W�F�N�V����(�Q�Ɠn��)</param>
	void Draw(ViewProjection& viewProjection_);

	// �ڋ߃t�F�[�Y�s��
	void Move();

	//�@���E�t�F�[�Y�s��
	void LevelUp();

	/// <summary>
	/// �e����
	/// </summary>
	void Fire();

	// ���ˊԊu
	static const int kFireInterval = 60;

	// �ڋ߃t�F�[�Y������
	void ApproachInitialize();

	void SetPlayer(Player* player) { player_ = player; }

	// ���[���h���W���擾
	Vector3 GetWorldPosition();

	// �Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	// �e���X�g���擾
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	float Radius();

	float Hp();

private:
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	// ���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	// ���f��
	Model* model_ = nullptr;

	// �e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	// �s���t�F�[�Y
	enum class Phase
	{
		Move,	//	�ڋ߂���
		LevelUp,		//	���E����
	};

	// �t�F�[�Y
	Phase phase_ = Phase::Move;

	// �e
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	// ���˃^�C�}�[
	int32_t FireTimer = 0;

	// ���L����
	Player* player_ = nullptr;

	const float radius_ = 1.0f;

	bool ChangeSpeedFlagX = 0;

	bool ChangeSpeedFlagY = 0;

	int32_t ChangeTimeX = 240;
	int32_t ChangeTimeY = 240;

	int32_t hp = 200;
};