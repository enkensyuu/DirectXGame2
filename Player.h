#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include"ViewProjection.h"
#include "DebugText.h"
#include"PlayerBullet.h"
#include<memory>
#include<list>

/// <summary>
/// ���L����
/// </summary>
class Player
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

	void Rotate();

	/// <summary>
	/// �U��
	/// </summary>
	void Attack();

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

	// �e
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
};