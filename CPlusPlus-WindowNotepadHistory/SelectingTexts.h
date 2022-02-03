#ifndef _SELECTINGTEXTS_H
#define _SELECTINGTEXTS_H

typedef signed long int Long;

class NotepadForm;

class SelectingTexts
{
public:
	NotepadForm* notepadForm;
public:
	SelectingTexts(NotepadForm* notepadForm = 0);//����Ʈ������
	void DoNext(Long previousRowIndex, Long previousLetterIndex, Long currentRowIndex,
		Long currentLetterIndex);
	void DoPrevious(Long previousRowIndex, Long previousLetterIndex, Long currentRowIndex,
		Long currentLetterIndex);
	void Undo();
	~SelectingTexts();//�Ҹ���
};

#endif // !_SELECTINGTEXTS_H
