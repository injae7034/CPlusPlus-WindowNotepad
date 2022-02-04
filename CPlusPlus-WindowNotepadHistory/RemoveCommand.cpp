#include "RemoveCommand.h"
#include "NotepadForm.h"
#include "Note.h"
#include "DummyRow.h"

//����Ʈ������
RemoveCommand::RemoveCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{

}

//Execute
void RemoveCommand::Execute()
{
	//1. ���� ���� ��ġ�� ���Ѵ�.
	Long currentRowPos = this->notepadForm->note->GetCurrent();
	//2. ���� ������ ��ġ�� ���Ѵ�.
	Long currentLetterPos = this->notepadForm->current->GetCurrent();
	
	Long selectedStartRowPos = this->notepadForm->selectedStartYPos;//������ ���۵Ǵ� ��
	Long selectedStartLetterPos = this->notepadForm->selectedStartXPos;//������ ���۵Ǵ� ����
	Long selectedEndRowPos = currentRowPos;//������ ������ ��
	Long selectedEndLetterPos = currentLetterPos;//������ ������ ����
	//������ ������ �����ʹ�����
	Long startRowIndex = 0;//�����ϴ� ���� ��ġ
	Long startLetterIndex = 0;//�����ϴ� ���� ��ġ
	Long endRowIndex = 0;//������ ���� ��ġ
	Long endLetterIndex = 0;//������ ���� ��ġ
	Glyph* startRow = 0;//�����ϴ� ���� ��ġ
	//3. ������ ���۵Ǵ� �ٰ� ������ ������ ���� ������
	//(�� �� �������� ������ �Ǿ� �����Ƿ� ���� �������� �ʰ� ���ڵ鸸 ������)
	if (selectedStartRowPos == selectedEndRowPos)
	{
		//3.1.1 ������ ���������� ����Ǿ�����
		if (selectedStartLetterPos < selectedEndLetterPos)
		{
			//3.1.1.1 �����ϴ� ������ġ�� ������ ���۵Ǵ� ������ġ�� ���Ѵ�.
			startLetterIndex = selectedStartLetterPos;
			//3.1.1.2 ������ ������ġ�� ������ ������ ������ġ�� ���Ѵ�.
			endLetterIndex = selectedEndLetterPos;
			//3.1.1.3 �����ϴ� ���� ��ġ�� ������ �����ϴ� ���� ��ġ�� ���Ѵ�.
			startRowIndex = selectedStartRowPos;

		}
		//3.1.2 ������ �������� ����Ǿ�����
		else
		{
			//3.1.2.1 �����ϴ� ������ġ�� ������ ������ ������ġ�� ���Ѵ�.
			startLetterIndex = selectedEndLetterPos;
			//3.1.2.2 ������ ������ġ�� ������ �����ϴ� ������ġ�� ���Ѵ�.
			endLetterIndex = selectedStartLetterPos;
			//3.1.2.3 �����ϴ� ���� ��ġ�� ������ ������ ���� ��ġ�� ���Ѵ�.
			startRowIndex = selectedEndRowPos;
		}
		//3.1.3 �����ϴ� ���� ���Ѵ�.
		startRow = this->notepadForm->note->GetAt(startRowIndex);
		//3.1.4 �����ϴ� ������ġ���� ������ ���ڱ��� �����.
		while (startLetterIndex < endLetterIndex)
		{
			//3.1.4.1 �ٿ��� ���ڸ� �����.
			startRow->Remove(startLetterIndex);
			//3.1.4.2 �ٿ��� ���ڰ� �������� ���� ������ �ٰ� �����ϴ� ������ ���� ���ڰ�
			//������ �����ϴ� ������ ��ġ�� �մ���� ���� �ǹǷ� ������ ������ ���� ���� ���ҽ�Ų��. 
			endLetterIndex--;
		}
	}
	//4. ������ ���۵Ǵ� �ٰ� ������ ������ ���� ���� �ٸ���
	//(������ ���� �ٿ� ���ļ� �Ǿ� �ֱ� ������ ���ڰ� �� ������ ���� ����������)
	else
	{
		//4.1 ������ ���������� ����Ǿ����� 
		if (selectedStartRowPos < selectedEndRowPos)
		{
			//4.1.1 �����ϴ� ������ġ�� ������ ���۵Ǵ� ������ġ�� ���Ѵ�.
			startLetterIndex = selectedStartLetterPos;
			//4.1.2 ������ ������ġ�� ������ ������ ������ġ�� ���Ѵ�.
			endLetterIndex = selectedEndLetterPos;
			//4.1.3 �����ϴ� ���� ��ġ�� ������ �����ϴ� ���� ��ġ�� ���Ѵ�.
			startRowIndex = selectedStartRowPos;
			//4.1.4 ������ ���� ��ġ�� ������ ������ ���� ��ġ�� ���Ѵ�.
			endRowIndex = selectedEndRowPos;
		}
		//4.2 ������ �������� ����Ǿ�����
		else
		{
			//4.2.1 �����ϴ� ������ġ�� ������ ������ ������ġ�� ���Ѵ�.
			startLetterIndex = selectedEndLetterPos;
			//4.2.2 ������ ������ġ�� ������ �����ϴ� ������ġ�� ���Ѵ�.
			endLetterIndex = selectedStartLetterPos;
			//4.2.3 �����ϴ� ���� ��ġ�� ������ ������ ���� ��ġ�� ���Ѵ�.
			startRowIndex = selectedEndRowPos;
			//4.2.4 ������ ���� ��ġ�� ������ �����ϴ� ���� ��ġ�� ���Ѵ�.
			endRowIndex = selectedStartRowPos;
		}
		Glyph* endRow = 0;//������ ���� ��ġ
		Glyph* row = 0;//���� �ּҸ� ���� ����
		Long letterIndex = 0;//���� ��ġ
		//4.3 �����ϴ� ���� ���Ѵ�.
		startRow = this->notepadForm->note->GetAt(startRowIndex);
		//4.4 �����ϴ� ������ġ���� �����ϴ� ���� ������ ���ڱ��� �����.
		while (startLetterIndex < startRow->GetLength())
		{
			//4.4.1 �ٿ��� ���ڸ� �����.
			startRow->Remove(startLetterIndex);
		}
		//4.5 �����ϴ� ���� �����ٺ��� ������ �������� ���ڿ� ���� �����.
		Long nextRowIndex = startRowIndex + 1;
		while (nextRowIndex < endRowIndex)
		{
			//4.5.1 ���� ���Ѵ�.
			row = this->notepadForm->note->GetAt(nextRowIndex);
			//4.5.2 ������ġ�� ����ġ��Ų��.
			letterIndex = 0;
			//4.5.3 �ٿ��� ������ ���ڱ��� �ݺ��Ѵ�.
			while (letterIndex < row->GetLength())
			{
				//4.5.3.1 ���� ���ڸ� �����.
				row->Remove(letterIndex);
			}
			//4.5.4 ���� ���ڸ� �������⶧���� �޸��忡�� ���� �����.
			this->notepadForm->note->Remove(nextRowIndex);
			//4.5.5 ���� ������ ������ ������ ������ ���� ��ġ�� ��ĭ �մ������.
			endRowIndex--;
		}
		//4.6 ������ ���� ���Ѵ�.
		endRow = this->notepadForm->note->GetAt(endRowIndex);
		//4.7 ������ ���� ó������ ������ ���ڱ��� ���ڸ� �����.
		letterIndex = 0;
		while (letterIndex < endLetterIndex)
		{
			//4.7.1 ������ ���� ���ڸ� �����.
			endRow->Remove(letterIndex);
			//4.7.2 ������ ���� ù���ڸ� ����� ���� ���ں��� ������ ��ĭ��
			//������� ������ ������ ������ġ�� -1 ���ҽ�Ų��.
			endLetterIndex--;
		}
		//4.8 ������ ���� �����ϴ� �ٷ� Join��Ų��.
		endRow->Join(startRow);
		//4.9 ������ ���� �����ϴ� �ٷ� Join�Ǿ��� ������
		//������ ���� �޸��忡�� �����.
		this->notepadForm->note->Remove(endRowIndex);
		//4.10 ���� ���� ��ġ�� �����ϴ� ���� ��ġ�� �����Ѵ�.
		this->notepadForm->current = this->notepadForm->note->
			GetAt(startRowIndex);
		//4.11 ���� ������ ��ġ�� �����ϴ� ������ ��ġ�� �����Ѵ�.
		this->notepadForm->current->Move(startLetterIndex);
	}
	//5. �޸��� ���� *�� �߰��Ѵ�.
	string name = this->notepadForm->fileName;
	name.insert(0, "*");
	name += " - �޸���";
	this->notepadForm->SetWindowText(CString(name.c_str()));
	//6. �޸��忡 ��������� ������ �����Ѵ�.
	this->notepadForm->isDirty = true;
	//7. �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//7.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
		//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
		this->notepadForm->SendMessage(WM_SIZE);
	}
	//8. �޸��忡�� ���õ� texts�� �� ������ ������ �޸��忡�� ������ �ȵ� ���·� �ٲ۴�.
	this->notepadForm->isSelecting = false;
	//9. ������ ������ ������ ĳ���� x��ǥ�� 0���� �����Ѵ�.
	this->notepadForm->selectedStartXPos = 0;
	//10. ������ ������ ������ ĳ���� y��ǥ�� 0���� �����Ѵ�.
	this->notepadForm->selectedStartYPos = 0;
	//11. �����ϱ�, �߶󳻱�, ���� �޴��� ��Ȱ��ȭ ��Ų��.
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
}

//�Ҹ���
RemoveCommand::~RemoveCommand()
{

}