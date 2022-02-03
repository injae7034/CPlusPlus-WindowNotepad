#ifndef _SELECTINGTEXTS_H
#define _SELECTINGTEXTS_H

typedef signed long int Long;

class NotepadForm;

class SelectingTexts
{
public:
	NotepadForm* notepadForm;
public:
	SelectingTexts(NotepadForm* notepadForm = 0);//디폴트생성자
	void DoNext(Long previousRowIndex, Long previousLetterIndex, Long currentRowIndex,
		Long currentLetterIndex);
	void DoPrevious(Long previousRowIndex, Long previousLetterIndex, Long currentRowIndex,
		Long currentLetterIndex);
	void Undo();
	~SelectingTexts();//소멸자
};

#endif // !_SELECTINGTEXTS_H
