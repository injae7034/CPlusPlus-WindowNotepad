#ifndef _KEYACTIONCREATOR_H
#define _KEYACTIONCREATOR_H

#include "NotepadForm.h"
//NotepadForm�� �ּҷ� �ۿ� �Ⱦ����� ���漱���� �ص� ���������� UINT�ڷ����� MFC�� ������
//�ִ� �ڷ����̱� ������ "NotepadForm.h"�� ������ UINT�ڷ����� ����� �� ����!

class KeyAction;//�ּҷθ� ���̱� ������ ���漱�� ���ָ� ��.

class KeyActionCreator
{
public://notepadForm�� ����ȭ
	NotepadForm* notepadForm;
public:
	KeyActionCreator(NotepadForm* notepadForm = 0);//����Ʈ������
	KeyAction* Create(UINT nChar);//���丮 �޼ҵ� ����
	~KeyActionCreator();//�Ҹ���
};

#endif // !_KEYACTIONCREATOR_H

