#ifndef _KEYACTIONCREATOR_H
#define _KEYACTIONCREATOR_H

#include "KeyAction.h"
#include "NotepadForm.h"
//NotepadForm�� �ּҷ� �ۿ� �Ⱦ����� ���漱���� �ص� ���������� UINT�ڷ����� MFC�� ������
//�ִ� �ڷ����̱� ������ "NotepadForm.h"�� ������ UINT�ڷ����� ����� �� ����!

class KeyActionCreator
{
public:
	KeyActionCreator(NotepadForm* notepadForm = 0);//����Ʈ������
	KeyAction* Create(UINT nChar);//���丮 �޼ҵ� ����
	~KeyActionCreator();//�Ҹ���
public://notepadForm�� ����ȭ
	NotepadForm* notepadForm;
};

#endif // !_KEYACTIONCREATOR_H

