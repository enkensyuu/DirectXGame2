#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include"ViewProjection.h"
#include "DebugText.h"
#include"EnemyBullet.h"

class Player;

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
	void Approach();

	//�@���E�t�F�[�Y�s��
	void Leave();

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
		Approach,	//	�ڋ߂���
		Leave,		//	���E����
	};

	// �t�F�[�Y
	Phase phase_ = Phase::Approach;

	// �e
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	// ���˃^�C�}�[
	int32_t FireTimer = 0;

	// ���L����
	Player* player_ = nullptr;
};