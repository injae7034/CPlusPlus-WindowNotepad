#ifndef _SCROLLACTIONCREATOR_H
#define _SCROLLACTIONCREATOR_H

#include "NotepadForm.h"

class ScrollAction;

class ScrollActionCreator
{
public:
	NotepadForm* notepadForm;//����ȭ
public:
	ScrollActionCreator(NotepadForm* notepadForm = 0);//����Ʈ������
	ScrollAction* Create(UINT nSBCode);//���丮�޼ҵ�����
	~ScrollActionCreator();//�Ҹ���
};

#endif // !_SCROLLACTIONCREATOR_H
