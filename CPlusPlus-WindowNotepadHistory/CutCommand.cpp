#include "CutCommand.h"
#include "NotepadForm.h"
#include "Note.h"
#include "DummyRow.h"

//����Ʈ������
CutCommand::CutCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{

}

//Execute
void CutCommand::Execute()
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
	Long copyLetterIndex = 0;//Ŭ�����忡 ����Ǵ� ���ڵ��� ��ġ�� ������ ���
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
			//7.1.4.2 Ŭ�������� ���� ���� ��(�����ϴ� ��)�� ���ڸ� �߰��Ѵ�.
			copyLetterIndex = copyRow->Add(letter->Clone());
			//7.1.4.3 �޸����� ���ڵ��� ������ �Ǿ� �ִ� ���·� ���� �־�� ������
			//Ŭ�����忡 �Ű����� ���ڵ��� ������ �ȵ� ���·� �ٲ��࿩��(�ֳ��ϸ� �޸��忡��
			//������ �� ���·� ���簡 �Ǿ��� ������ �ϴ� Ŭ�����忡 ������ �� ���·� �����ϰ�
			//���߿� ���� ������ �ȵ� ���·� �ٲ���� �ٿ��ֱ⸦ �� �� ������ �ȵ� ���ڵ��� �ٿ��־���)
			copyRow->GetAt(copyLetterIndex)->Select(false);
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
			//8.4.2 Ŭ�������� ���� ���� ��(�����ϴ� ��)�� ���ڸ� �߰��Ѵ�.
			copyLetterIndex = copyRow->Add(letter->Clone());
			//8.4.3 �޸����� ���ڵ��� ������ �Ǿ� �ִ� ���·� ���� �־�� ������
			//Ŭ�����忡 �Ű����� ���ڵ��� ������ �ȵ� ���·� �ٲ��࿩��(�ֳ��ϸ� �޸��忡��
			//������ �� ���·� ���簡 �Ǿ��� ������ �ϴ� Ŭ�����忡 ������ �� ���·� �����ϰ�
			//���߿� ���� ������ �ȵ� ���·� �ٲ���� �ٿ��ֱ⸦ �� �� ������ �ȵ� ���ڵ��� �ٿ��־���)
			copyRow->GetAt(copyLetterIndex)->Select(false);
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
				//8.6.3.2 Ŭ�������� ���� ���� ��(�����ϴ� ��)�� ���ڸ� �߰��Ѵ�.
				copyLetterIndex = realRow->Add(letter->Clone());
				//8.6.3.3 �޸����� ���ڵ��� ������ �Ǿ� �ִ� ���·� ���� �־�� ������
				//Ŭ�����忡 �Ű����� ���ڵ��� ������ �ȵ� ���·� �ٲ��࿩��(�ֳ��ϸ� �޸��忡��
				//������ �� ���·� ���簡 �Ǿ��� ������ �ϴ� Ŭ�����忡 ������ �� ���·� �����ϰ�
				//���߿� ���� ������ �ȵ� ���·� �ٲ���� �ٿ��ֱ⸦ �� �� ������ �ȵ� ���ڵ��� �ٿ��־���)
				realRow->GetAt(copyLetterIndex)->Select(false);
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
			//8.10.2 Ŭ�������� ���� ���� ��(�����ϴ� ��)�� ���ڸ� �߰��Ѵ�.
			copyLetterIndex = realRow->Add(letter->Clone());
			//8.10.3 �޸����� ���ڵ��� ������ �Ǿ� �ִ� ���·� ���� �־�� ������
			//Ŭ�����忡 �Ű����� ���ڵ��� ������ �ȵ� ���·� �ٲ��࿩��(�ֳ��ϸ� �޸��忡��
			//������ �� ���·� ���簡 �Ǿ��� ������ �ϴ� Ŭ�����忡 ������ �� ���·� �����ϰ�
			//���߿� ���� ������ �ȵ� ���·� �ٲ���� �ٿ��ֱ⸦ �� �� ������ �ȵ� ���ڵ��� �ٿ��־���)
			realRow->GetAt(copyLetterIndex)->Select(false);
			//8.10.4 ���� ���ڷ� �̵��ؼ� ���ڸ� �����Ѵ�.
			letterIndex++;
		}
	}
	//9. notepadForm�� clipboard�� ���� ������ Note(������ ����)�� �߰������ش�.
	Long noteIndex = this->notepadForm->clipboard->Add(copyNote);
	//10. ������ texts�� ����� ������ �ٿ��ֱ� �޴��� Ȱ��ȭ�����ش�.
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_PASTE, MF_BYCOMMAND | MF_ENABLED);


	//����Ŭ�����忡 ������ ������ �ܺ�Ŭ������� �ű��
	//11. notepadForm�� clipboard�� ���� ������ Note(������ ����)�� �� ���̸� ���Ѵ�.
	CString content = this->notepadForm->clipboard->
		GetAt(noteIndex)->GetContent().c_str();
	//������ ���ڿ��� ���̸� ���Ѵ�. ('\0'���� ������ ũ��)
	int contentLength = content.GetLength() + 1;
	//12. �ܺ� Ŭ�����尡 ��������
	//Ŭ������� ���� ���α׷����� ������ �� �ִ� ����̱� ������ � ���α׷�(������)�� Ŭ�����带
	//����ϴ��� �����ϰ� ����ϴ� ���� ����
	if (this->notepadForm->OpenClipboard() != 0)
	{
		//12.1 notepadForm�� clipboard�� ���� ������ Note(������ ����)�� Ŭ�����忡 �ű� ������ ����
		//Ŭ�����忡 ���ڿ��� �����ϱ� ���ؼ��� ���ڿ��� ������ �޸𸮸� Ŭ������� �����ؾ� �մϴ�.
		//���� ���ڿ��� ������ �޸𸮸� ���� ������ �ϴµ�, �� �޸� ������ �ڵ��� ����ؾ� �ϱ�
		//������ HeapAlloc, malloc ���� �� �Լ��� ����� �Ұ����մϴ�.�׷��� �Ʒ��� �ڵ� �� ������
		//����ϴ� GlobalAlloc �Լ��� ����ؼ� �޸𸮸� �Ҵ��ؾ� �ϰ�, Ŭ������� �ٸ� ���α׷���
		//�����ϴ� �����̱� ������ �� �޸� �Ӽ��� GMEM_DDESHARE�� GMEM_MOVEABLE�� �߰��Ѵ�.
		//(�� �޸𸮴� Ŭ������� ���޵Ǳ� ������ �ڽ��� ���α׷����� �����ϸ� �ȵȴ�)
		HGLOBAL hClipboardData = GlobalAlloc(GMEM_DDESHARE | GMEM_MOVEABLE, contentLength);
		//�̷��� �Ҵ�� �޸𸮴� �ڵ� ���� ��ȯ�ϱ� ������ �� �޸𸮿� ���ڿ��� �����ϱ� ���ؼ��� ���
		//������ �ּҸ� ���� �Ѵ�. ���� GlobalLock �Լ��� ����Ͽ� �ڽ��� �� �޸𸮸� ����ϰڴٰ�
		//�����ϰ� ��� ������ �ּҸ� ���� �Ѵ�.�̶�, �����ؾ� �� ���� Ŭ�����忡�� ����ϴ�
		//���ڿ��� ASCII �������� �����Ǿ�� �ϱ� ������ �����ڵ带 ����(MFC ���α׷����� �⺻������
		//(�����ڵ� ������)�ϴ� ���α׷������� ASCII �������� ���ڿ��� �����ؼ� ����ؾ� �Ѵ�.
		char* pchData = (char*)GlobalLock(hClipboardData);
		if (pchData != NULL)
		{
			// �Ҵ�� �޸� ������ ������ ���ڿ��� �����Ѵ�.
			memcpy(pchData, content, contentLength);
			// ���ڿ��� �����ϱ� ���ؼ� Lock �ߴ� �޸𸮸� �����Ѵ�. 
			// Ŭ�����带 ����. 
			GlobalUnlock(hClipboardData);
			// Ŭ�����忡 ����� ���� ���ڿ��� �����Ѵ�. 
			//�ٸ� ���α׷��� Ŭ�����带 ����ϰų� �ٸ� ���ڿ��� Ŭ�����忡 ����Ǿ� ���� �� �ֱ�
			//������ EmptyClipboard �Լ��� ����ؼ� Ŭ�����忡 �����͸� �����ϱ� ���� �غ�
			EmptyClipboard();
			// Ŭ������� ���ڿ��� �����ϰ�, Ŭ�����忡 �����ϰ��� �ϴ� ������ ���ڿ����� �˷��ش�.
			SetClipboardData(CF_TEXT, hClipboardData);
			// Ŭ�����带 �ݴ´�.
			CloseClipboard();
		}
	}

	currentRowPos = this->notepadForm->note->GetCurrent();
	currentLetterPos = this->notepadForm->current->GetCurrent();
	selectedStartRowPos = this->notepadForm->selectedStartYPos;//������ ���۵Ǵ� ��
	selectedStartLetterPos = this->notepadForm->selectedStartXPos;//������ ���۵Ǵ� ����
	selectedEndRowPos = currentRowPos;//������ ������ ��
	selectedEndLetterPos = currentLetterPos;//������ ������ ����
	//������ ������ �����ʹ�����
	startRowIndex = 0;//�����ϴ� ���� ��ġ
	startLetterIndex = 0;//�����ϴ� ���� ��ġ
	endRowIndex = 0;//������ ���� ��ġ
	endLetterIndex = 0;//������ ���� ��ġ
	startRow = 0;//�����ϴ� ���� ��ġ
	//2.1 ������ ���۵Ǵ� �ٰ� ������ ������ ���� ������
	//(�� �� �������� ������ �Ǿ� �����Ƿ� ���� �������� �ʰ� ���ڵ鸸 ������)
	if (selectedStartRowPos == selectedEndRowPos)
	{
		//2.1.1 ������ ���������� ����Ǿ�����
		if (selectedStartLetterPos < selectedEndLetterPos)
		{
			//2.1.1.1 �����ϴ� ������ġ�� ������ ���۵Ǵ� ������ġ�� ���Ѵ�.
			startLetterIndex = selectedStartLetterPos;
			//2.1.1.2 ������ ������ġ�� ������ ������ ������ġ�� ���Ѵ�.
			endLetterIndex = selectedEndLetterPos;
			//2.1.1.3 �����ϴ� ���� ��ġ�� ������ �����ϴ� ���� ��ġ�� ���Ѵ�.
			startRowIndex = selectedStartRowPos;

		}
		//2.1.2 ������ �������� ����Ǿ�����
		else
		{
			//2.1.2.1 �����ϴ� ������ġ�� ������ ������ ������ġ�� ���Ѵ�.
			startLetterIndex = selectedEndLetterPos;
			//2.1.2.2 ������ ������ġ�� ������ �����ϴ� ������ġ�� ���Ѵ�.
			endLetterIndex = selectedStartLetterPos;
			//2.1.2.3 �����ϴ� ���� ��ġ�� ������ ������ ���� ��ġ�� ���Ѵ�.
			startRowIndex = selectedEndRowPos;
		}
		//2.1.3 �����ϴ� ���� ���Ѵ�.
		startRow = this->notepadForm->note->GetAt(startRowIndex);
		//2.1.4 �����ϴ� ������ġ���� ������ ���ڱ��� �����.
		while (startLetterIndex < endLetterIndex)
		{
			//2.1.4.1 �ٿ��� ���ڸ� �����.
			startRow->Remove(startLetterIndex);
			//2.1.4.2 �ٿ��� ���ڰ� �������� ���� ������ �ٰ� �����ϴ� ������ ���� ���ڰ�
			//������ �����ϴ� ������ ��ġ�� �մ���� ���� �ǹǷ� ������ ������ ���� ���� ���ҽ�Ų��. 
			endLetterIndex--;
		}
	}
	//2.2 ������ ���۵Ǵ� �ٰ� ������ ������ ���� ���� �ٸ���
	//(������ ���� �ٿ� ���ļ� �Ǿ� �ֱ� ������ ���ڰ� �� ������ ���� ����������)
	else
	{
		//2.2.1 ������ ���������� ����Ǿ����� 
		if (selectedStartRowPos < selectedEndRowPos)
		{
			//2.2.1.1 �����ϴ� ������ġ�� ������ ���۵Ǵ� ������ġ�� ���Ѵ�.
			startLetterIndex = selectedStartLetterPos;
			//2.2.1.2 ������ ������ġ�� ������ ������ ������ġ�� ���Ѵ�.
			endLetterIndex = selectedEndLetterPos;
			//2.2.1.3 �����ϴ� ���� ��ġ�� ������ �����ϴ� ���� ��ġ�� ���Ѵ�.
			startRowIndex = selectedStartRowPos;
			//2.2.1.4 ������ ���� ��ġ�� ������ ������ ���� ��ġ�� ���Ѵ�.
			endRowIndex = selectedEndRowPos;
		}
		//2.2.2 ������ �������� ����Ǿ�����
		else
		{
			//2.2.2.1 �����ϴ� ������ġ�� ������ ������ ������ġ�� ���Ѵ�.
			startLetterIndex = selectedEndLetterPos;
			//2.2.2.2 ������ ������ġ�� ������ �����ϴ� ������ġ�� ���Ѵ�.
			endLetterIndex = selectedStartLetterPos;
			//2.2.2.3 �����ϴ� ���� ��ġ�� ������ ������ ���� ��ġ�� ���Ѵ�.
			startRowIndex = selectedEndRowPos;
			//2.2.2.4 ������ ���� ��ġ�� ������ �����ϴ� ���� ��ġ�� ���Ѵ�.
			endRowIndex = selectedStartRowPos;
		}
		Glyph* endRow = 0;//������ ���� ��ġ
		Glyph* row = 0;//���� �ּҸ� ���� ����
		Long letterIndex = 0;//���� ��ġ
		//2.2.3 �����ϴ� ���� ���Ѵ�.
		startRow = this->notepadForm->note->GetAt(startRowIndex);
		//2.2.4 �����ϴ� ������ġ���� �����ϴ� ���� ������ ���ڱ��� �����.
		while (startLetterIndex < startRow->GetLength())
		{
			//2.2.4.1 �ٿ��� ���ڸ� �����.
			startRow->Remove(startLetterIndex);
		}
		//2.2.5 �����ϴ� ���� �����ٺ��� ������ �������� ���ڿ� ���� �����.
		Long nextRowIndex = startRowIndex + 1;
		while (nextRowIndex < endRowIndex)
		{
			//2.2.5.1 ���� ���Ѵ�.
			row = this->notepadForm->note->GetAt(nextRowIndex);
			//2.2.5.2 ������ġ�� ����ġ��Ų��.
			letterIndex = 0;
			//2.2.5.3 �ٿ��� ������ ���ڱ��� �ݺ��Ѵ�.
			while (letterIndex < row->GetLength())
			{
				//2.2.5.3.1 ���� ���ڸ� �����.
				row->Remove(letterIndex);
			}
			//2.2.5.4 ���� ���ڸ� �������⶧���� �޸��忡�� ���� �����.
			this->notepadForm->note->Remove(nextRowIndex);
			//2.2.5.5 ���� ������ ������ ������ ������ ���� ��ġ�� ��ĭ �մ������.
			endRowIndex--;
		}
		//2.2.6 ������ ���� ���Ѵ�.
		endRow = this->notepadForm->note->GetAt(endRowIndex);
		//2.2.7 ������ ���� ó������ ������ ���ڱ��� ���ڸ� �����.
		letterIndex = 0;
		while (letterIndex < endLetterIndex)
		{
			//2.2.7.1 ������ ���� ���ڸ� �����.
			endRow->Remove(letterIndex);
			//2.2.7.2 ������ ���� ù���ڸ� ����� ���� ���ں��� ������ ��ĭ��
			//������� ������ ������ ������ġ�� -1 ���ҽ�Ų��.
			endLetterIndex--;
		}
		//2.2.8 ������ ���� �����ϴ� �ٷ� Join��Ų��.
		endRow->Join(startRow);
		//2.2.9 ������ ���� �����ϴ� �ٷ� Join�Ǿ��� ������
		//������ ���� �޸��忡�� �����.
		this->notepadForm->note->Remove(endRowIndex);
		//2.2.10 ���� ���� ��ġ�� �����ϴ� ���� ��ġ�� �����Ѵ�.
		this->notepadForm->current = this->notepadForm->note->
			GetAt(startRowIndex);
		//2.2.11 ���� ������ ��ġ�� �����ϴ� ������ ��ġ�� �����Ѵ�.
		this->notepadForm->current->Move(startLetterIndex);
	}
	//2.3 �޸��忡�� ���õ� texts�� �� ������ ������ �޸��忡�� ������ �ȵ� ���·� �ٲ۴�.
	this->notepadForm->isSelecting = false;
	//2.4 ������ ������ ������ ĳ���� x��ǥ�� 0���� �����Ѵ�.
	this->notepadForm->selectedStartXPos = 0;
	//2.5 ������ ������ ������ ĳ���� y��ǥ�� 0���� �����Ѵ�.
	this->notepadForm->selectedStartYPos = 0;

	//3.1 �����ϱ�, �߶󳻱�, ���� �޴��� ��Ȱ��ȭ ��Ų��.
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);

	//15. �޸��� ���� *�� �߰��Ѵ�.
	string name = this->notepadForm->fileName;
	name.insert(0, "*");
	name += " - �޸���";
	this->notepadForm->SetWindowText(CString(name.c_str()));
	//16. �޸��忡 ��������� ������ �����Ѵ�.
	this->notepadForm->isDirty = true;
}

//�Ҹ���
CutCommand::~CutCommand()
{

}