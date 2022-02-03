#ifndef _SELECTTEXT_H
#define _SELECTTEXT_H

typedef signed long int Long;

class NotepadForm;

class SelectText
{
public:
	NotepadForm* notepadForm;
public:
	SelectText(NotepadForm* notepadForm = 0);//����Ʈ������
	void Do(Long previousRowIndex, Long previousLetterIndex, Long currentRowIndex,
		Long currentLetterIndex);
	void Undo();
	~SelectText();//�Ҹ���
};

#endif // !_SELECTTEXT_H
