#ifndef _COMMANDCREATOR_H
#define _COMMANDCREATOR_H

#include "Command.h"
#include "NotepadForm.h"

class CommandCreator
{
public:
	CommandCreator(NotepadForm* notepadForm = 0);//����Ʈ������
	Command* Create(UINT nId);//���丮 �޼ҵ� ����
	~CommandCreator();//�Ҹ���
public://notepadForm�� ����ȭ
	NotepadForm* notepadForm;
};

#endif // !_COMMANDCREATOR_H

