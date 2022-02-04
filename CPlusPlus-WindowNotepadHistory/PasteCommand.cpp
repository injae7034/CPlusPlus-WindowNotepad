#include "PasteCommand.h"
#include "NotepadForm.h"
#include "Note.h"
#include "DummyRow.h"

//����Ʈ������
PasteCommand::PasteCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{

}

//Execute
void PasteCommand::Execute()
{
	//1. �޸����� ���� �ٰ� ������ ��ġ�� ���Ѵ�.
	Long currentRowPos = this->notepadForm->note->GetCurrent();
	Long currentLetterPos = this->notepadForm->current->GetCurrent();
	//2. �޸����� Ŭ�����忡�� ���� ��Ʈ(������ ����)�� ȣ���Ѵ�.
	Glyph* copyNote = this->notepadForm->clipboard->
		GetAt(this->notepadForm->clipboard->GetCurrent());
	//3. �޸����� ���� ���� ���Ѵ�.
	Glyph* currentRow = this->notepadForm->note->GetAt(currentRowPos);
	//4. �޸����� ���� �ٿ��� ���� ������ġ �������� split��Ų��.
	Glyph* splitedRow = currentRow->Split(currentLetterPos);
	//5. Ŭ�������� ���� ��Ʈ�� ù��° ���� ���Ѵ�.
	Long firstCopyRowPos = 0;
	Glyph* firstCopyRow = copyNote->GetAt(firstCopyRowPos);
	Glyph* letter = 0;//���ڸ� ���� ����
	//5. Ŭ�������� ���� ��Ʈ�� ù��° ���� �������� �������� �ݺ��Ѵ�.
	Long letterIndex = 0;
	while (letterIndex < firstCopyRow->GetLength())
	{
		//5.1 Ŭ�������� ���� ��Ʈ�� ù��° ���� ���ڸ� ���Ѵ�.
		letter = firstCopyRow->GetAt(letterIndex);
		//5.2 ���ڸ� �޸����� ���� �ٿ� �߰��Ѵ�.(���� ����)
		currentRow->Add(letter->Clone());
		//5.3 ���� ���ڷ� �̵��Ѵ�.
		letterIndex++;
	}
	//6. Ŭ�������� ���� ��Ʈ�� ù��° �ٿ��� ���� �ٷ� �̵��Ѵ�.
	Long nextCopyRowPos = firstCopyRowPos + 1;
	Glyph* copyRow = firstCopyRow;//ù��° ���� ����Ʈ
	Long i = currentRowPos;
	//7. Ŭ�������� ���� ��Ʈ(������ ����)�� �������ٱ��� �ݺ��Ѵ�.
	while (nextCopyRowPos < copyNote->GetLength())
	{
		//7.1 ������ ���� ���Ѵ�.
		copyRow = copyNote->GetAt(nextCopyRowPos);
		//7.2 �޸����� ���� ���� ���� �ٷ� �̵��Ѵ�.
		i++;
		//7.3 ���� ���� �޸����� ���� ���� ��ġ �������� �����ִ´�.(���� ����)
		i = this->notepadForm->note->Add(i, copyRow->Clone());
		//7.4 ������ ��Ʈ���� ���� �ٷ� �̵��Ѵ�.
		nextCopyRowPos++;
	}
	//8. �޸��忡�� ���� ���� ���Ѵ�.
	this->notepadForm->current = this->notepadForm->note->GetAt(i);
	//9. �޸��忡�� ���� ������ġ�� ���Ѵ�.
	currentLetterPos = this->notepadForm->current->GetCurrent();
	//10. �޸��忡�� ���� �ٿ� �Ʊ� split�ߴ� ���ڵ��� �߰��Ѵ�.
	letterIndex = 0;
	while (letterIndex < splitedRow->GetLength())
	{
		//10.1 ���ڸ� ���Ѵ�.
		letter = splitedRow->GetAt(letterIndex);
		//10.2 ������ �ٿ� ���ڸ� ���� ���縦 �Ѵ�.
		this->notepadForm->current->Add(letter->Clone());
		//10.3 ���� ���ڷ� �̵��Ѵ�.
		letterIndex++;
	}
	//11. �޸��忡�� ���� ������ġ�� �����Ѵ�.
	this->notepadForm->current->Move(currentLetterPos);
	//12. �޸��� ���� *�� �߰��Ѵ�.
	string name = this->notepadForm->fileName;
	name.insert(0, "*");
	name += " - �޸���";
	this->notepadForm->SetWindowText(CString(name.c_str()));
	//13. �޸��忡 ��������� ������ �����Ѵ�.
	this->notepadForm->isDirty = true;
	//14. ��������� �����Ѵ�.
	this->notepadForm->Notify();
	this->notepadForm->Invalidate(TRUE);
}

//�Ҹ���
PasteCommand::~PasteCommand()
{

}