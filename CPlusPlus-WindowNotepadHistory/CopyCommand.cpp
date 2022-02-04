#include "CopyCommand.h"
#include "NotepadForm.h"
#include "Note.h"
#include "DummyRow.h"

//����Ʈ������
CopyCommand::CopyCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{

}

//Execute
void CopyCommand::Execute()
{
	//�����ϱ� Command�� ���Դٴ� ���� �ּ� �ѱ����̻��� �����Ұ� �־ ���Դٴ� ����
	//�����Ұ� �������� ���ʿ� ���⿡ ���� �� ���� �׷��Ƿ� �����ϱ��� Execute������
	//��Ʈ�� �����ϰ� ���� �� �� �����ؼ� ���� ������ ��Ʈ�� �߰����� �ϴ°� ����Ʈ�̴�.
	//1. ������ �ؽ�Ʈ�� ���� ����Ʈ�� �����Ѵ�.
	Glyph* copyNote = new Note();
	//2. ���� �����Ѵ�.(������ ������ �� �����̵� ��õ���̵� �ϴ� ������ ���� �ϳ� �����ؾ���.)
	//�׸��� ���縦 ������ ���� ��¥���̵� ��¥���̵� ������� ó�� ������ ���� ������ Row�� ������. 
	Glyph* copyRow = new Row();
	//3. ���� ������ ���� Ŭ�������� ���� ������ ��Ʈ�� �߰��Ѵ�.
	Long rowIndexOfCopyNote = copyNote->Add(copyRow);
	//4. �޸��忡�� ���� �ٰ� ������ ��ġ�� ���Ѵ�.
	Long currentRowPos = this->notepadForm->note->GetCurrent();
	Long currentLetterPos = this->notepadForm->current->GetCurrent();
	//5. ������ ���۵Ǵ� �ٰ� ����, ������ ������ �ٰ� ������ ��ġ�� ���Ѵ�.
	Long selectedStartRowPos = this->notepadForm->selectedStartYPos;//������ ���۵Ǵ� ��
	Long selectedStartLetterPos = this->notepadForm->selectedStartXPos;//������ ���۵Ǵ� ����
	Long selectedEndRowPos = currentRowPos;//������ ������ ��
	Long selectedEndLetterPos = currentLetterPos;//������ ������ ����
	//6. ������ ����� ������� ������ ����(����)���� �����ϴ� �ٰ� ����, ������ �ٰ� ������ġ�� ���Ѵ�.
	Long startRowIndex = 0;//�����ϴ� ���� ��ġ
	Long startLetterIndex = 0;//�����ϴ� ���� ��ġ
	Long endRowIndex = 0;//������ ���� ��ġ
	Long endLetterIndex = 0;//������ ���� ��ġ
	Long i = 0;//�ݺ������
	Glyph* startRow = 0;//�����ϴ� ���� ��ġ
	Glyph* letter = 0;
	//7. ������ ���۵Ǵ� �ٰ� ������ ������ ���� ������(���簡 �� �� ������ �̷�������)
	if (selectedStartRowPos == selectedEndRowPos)
	{
		//7.1.1 ������ ���������� ����Ǿ�����
		if (selectedStartLetterPos < selectedEndLetterPos)
		{
			//7.1.1.1 �����ϴ� ������ġ�� ������ ���۵Ǵ� ������ġ�� ���Ѵ�.
			startLetterIndex = selectedStartLetterPos;
			//7.1.1.2 ������ ������ġ�� ������ ������ ������ġ�� ���Ѵ�.
			endLetterIndex = selectedEndLetterPos;
			//7.1.1.3 �����ϴ� ���� ��ġ�� ������ �����ϴ� ���� ��ġ�� ���Ѵ�.
			startRowIndex = selectedStartRowPos;

		}
		//7.1.2 ������ �������� ����Ǿ�����
		else
		{
			//7.1.2.1 �����ϴ� ������ġ�� ������ ������ ������ġ�� ���Ѵ�.
			startLetterIndex = selectedEndLetterPos;
			//7.1.2.2 ������ ������ġ�� ������ �����ϴ� ������ġ�� ���Ѵ�.
			endLetterIndex = selectedStartLetterPos;
			//7.1.2.3 �����ϴ� ���� ��ġ�� ������ ������ ���� ��ġ�� ���Ѵ�.
			startRowIndex = selectedEndRowPos;
		}
		//7.1.3 �����ϴ� ���� ���Ѵ�.
		startRow = this->notepadForm->note->GetAt(startRowIndex);
		//7.1.4 �����ϴ� ������ġ���� ������ ���ڱ��� �����Ѵ�.
		i = startLetterIndex;
		while (i < endLetterIndex)
		{
			//7.1.4.1 �����ϴ� �ٿ��� �ٿ��� ���ڸ� ���Ѵ�.
			letter = startRow->GetAt(i);
			//7.1.4.2 ���ڰ� ������ �Ǿ��ִµ� Ŭ�����忡 ������ ���ο� ��Ʈ�� �ű� ���� ������ �ȵ�
			//���·� �����ؾ���. �׷��� ���߿� �ٿ��ֱ��Ҷ� �ٿ��ִ� ������� ������ �ȵ� ���·� �ٿ��־���!
			letter->Select(false);
			//7.1.4.3 ���� ���� ��(�����ϴ� ��)�� ���ڸ� �߰��Ѵ�.
			copyRow->Add(letter->Clone());
			//7.1.4.4 �����ϴ� ������ ���� ���ڷ� �̵��ؼ� ���ڸ� �����Ѵ�.
			i++;
		}
	}
	//8. ������ �����ϴ� �ٰ� ������ ������ ���� ���� �ٸ���(������ �����ٿ� �������� �Ǿ� ������)
	else
	{
		//8.1 ������ ���������� ����Ǿ����� 
		if (selectedStartRowPos < selectedEndRowPos)
		{
			//8.1.1 �����ϴ� ������ġ�� ������ ���۵Ǵ� ������ġ�� ���Ѵ�.
			startLetterIndex = selectedStartLetterPos;
			//8.1.2 ������ ������ġ�� ������ ������ ������ġ�� ���Ѵ�.
			endLetterIndex = selectedEndLetterPos;
			//8.1.3 �����ϴ� ���� ��ġ�� ������ �����ϴ� ���� ��ġ�� ���Ѵ�.
			startRowIndex = selectedStartRowPos;
			//8.1.4 ������ ���� ��ġ�� ������ ������ ���� ��ġ�� ���Ѵ�.
			endRowIndex = selectedEndRowPos;
		}
		//8.2 ������ �������� ����Ǿ�����
		else
		{
			//8.2.1 �����ϴ� ������ġ�� ������ ������ ������ġ�� ���Ѵ�.
			startLetterIndex = selectedEndLetterPos;
			//8.2.2 ������ ������ġ�� ������ �����ϴ� ������ġ�� ���Ѵ�.
			endLetterIndex = selectedStartLetterPos;
			//8.2.3 �����ϴ� ���� ��ġ�� ������ ������ ���� ��ġ�� ���Ѵ�.
			startRowIndex = selectedEndRowPos;
			//8.2.4 ������ ���� ��ġ�� ������ �����ϴ� ���� ��ġ�� ���Ѵ�.
			endRowIndex = selectedStartRowPos;
		}
		Glyph* endRow = 0;//������ ���� ��ġ
		Glyph* row = 0;//���� �ּҸ� ���� ����
		Long letterIndex = 0;//���� ��ġ
		//8.3 �����ϴ� ���� ���Ѵ�.
		startRow = this->notepadForm->note->GetAt(startRowIndex);
		//8.4 �����ϴ� ������ġ���� �����ϴ� ���� ������ ���ڱ��� �����Ѵ�.
		i = startLetterIndex;
		while (i < startRow->GetLength())
		{
			//8.4.1 �����ϴ� �ٿ��� ���ڸ� ���Ѵ�.
			letter = startRow->GetAt(i);
			//8.4.2 ���ڰ� ������ �Ǿ��ִµ� Ŭ�����忡 ������ ���ο� ��Ʈ�� �ű� ���� ������ �ȵ�
			//���·� �����ؾ���. �׷��� ���߿� �ٿ��ֱ��Ҷ� �ٿ��ִ� ������� ������ �ȵ� ���·� �ٿ��־���!
			letter->Select(false);
			//8.4.3 ���� ���� ��(�����ϴ� ��)�� ���ڸ� �߰��Ѵ�.
			copyRow->Add(letter->Clone());
			//8.4.4 �����ϴ� ������ ���� ���ڷ� �̵��ؼ� ���ڸ� �����Ѵ�.
			i++;
		}
		//8.5 �����ϴ� ���� �����ٷ� �̵��Ѵ�.
		Long nextRowIndex = startRowIndex + 1;
		Glyph* realRow = copyNote->GetAt(rowIndexOfCopyNote);//��¥ ���� �ּҸ� ���� ����
		//8.6 �����ٺ��� ������ �������� �ݺ��Ѵ�.
		while (nextRowIndex < endRowIndex)
		{
			//8.6.1 �޸����� ���� ���Ѵ�.
			row = this->notepadForm->note->GetAt(nextRowIndex);
			//8.6.2 �޸����� ���� ��¥�̸�
			if (!dynamic_cast<DummyRow*>(row))
			{
				//8.6.2.1 ��¥ ���� �����Ѵ�.
				realRow = new Row();
				//8.6.2.2 ��¥ ���� ���� ���� ��Ʈ�� �߰��Ѵ�.
				rowIndexOfCopyNote = copyNote->Add(realRow);
			}
			//8.6.3 �޸����� ��¥ ���� ó������ ������ ���ڱ��� �ݺ��Ѵ�.
			letterIndex = 0;
			while (letterIndex < row->GetLength())
			{
				//8.6.3.1 �޸����� ��¥ �ٿ��� ���ڸ� ���Ѵ�.
				letter = row->GetAt(letterIndex);
				//8.6.3.2 ���ڰ� ������ �Ǿ��ִµ� Ŭ�����忡 ������ ���ο� ��Ʈ�� �ű� ����
				//������ �ȵ� ���·� �����ؾ���. �׷��� ���߿� �ٿ��ֱ��Ҷ� 
				//�ٿ��ִ� ������� ������ �ȵ� ���·� �ٿ��־���!
				letter->Select(false);
				//8.6.3.3 ���� ���� ��Ʈ�� ���� ���� ��¥ �ٿ� ���ڸ� �߰��Ѵ�.
				realRow->Add(letter->Clone());
				//8.6.3.4 ���� ���ڷ� �̵��ؼ� ���ڸ� �����Ѵ�..
				letterIndex++;
			}
			//8.6.4 ���� �ٷ� �̵��Ѵ�.
			nextRowIndex++;
		}
		//8.7 �޸��忡�� ������ ���� ���Ѵ�.
		endRow = this->notepadForm->note->GetAt(nextRowIndex);
		//8.8 �޸��忡�� ������ ���� ��¥���̸�
		if (!dynamic_cast<DummyRow*>(endRow))
		{
			//8.8.1 ��¥ ���� �����Ѵ�.
			realRow = new Row();
			//8.8.2 ��¥ ���� ���� ���� ��Ʈ�� �߰��Ѵ�.
			rowIndexOfCopyNote = copyNote->Add(realRow);
		}
		//���ο� ��Ʈ�� ��¥ �ٿ� �޸����� ��¥���� ���ڸ� �����Ѵ�.
		//8.9 ���� ��ġ�� ����ġ��Ų��.
		letterIndex = 0;
		//8.10 �޸����� ��¥�ٿ��� ������ ���ڱ��� �ݺ��Ѵ�.
		while (letterIndex < endLetterIndex)
		{
			//8.10.1 �޸����� ��¥�ٿ��� ���ڸ� ���Ѵ�.
			letter = endRow->GetAt(letterIndex);
			//8.10.2 ���ڰ� ������ �Ǿ��ִµ� Ŭ�����忡 ������ ���ο� ��Ʈ�� �ű� ���� ������ �ȵ�
			//���·� �����ؾ���. �׷��� ���߿� �ٿ��ֱ��Ҷ� �ٿ��ִ� ������� ������ �ȵ� ���·� �ٿ��־���!
			letter->Select(false);
			//8.10.3 ���ο� ��Ʈ�� ��¥ �ٿ� ���ڸ� ���������� �߰��Ѵ�.
			realRow->Add(letter->Clone());
			//8.10.4 ���� ���ڷ� �̵��ؼ� ���ڸ� �����Ѵ�.
			letterIndex++;
		}
	}
	//9. notepadForm�� clipboard�� ���� ������ Note(������ ����)�� �߰������ش�.
	this->notepadForm->clipboard->Add(copyNote);
	//10. ������ texts�� ����� ������ �ٿ��ֱ� �޴��� Ȱ��ȭ�����ش�.
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_PASTE, MF_BYCOMMAND | MF_ENABLED);
}

//�Ҹ���
CopyCommand::~CopyCommand()
{

}