#ifndef _SELECTTEXT_H
#define _SELECTTEXT_H

typedef signed long int Long;

class NotepadForm;

class SelectText
{
public:
	NotepadForm* notepadForm;
public:
	SelectText(NotepadForm* notepadForm = 0);//디폴트생성자
	void Do(Long previousRowIndex, Long previousLetterIndex, Long currentRowIndex,
		Long currentLetterIndex);
	void Undo();
	~SelectText();//소멸자
};

#endif // !_SELECTTEXT_H
