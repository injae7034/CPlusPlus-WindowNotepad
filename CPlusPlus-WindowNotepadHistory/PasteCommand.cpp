#include "PasteCommand.h"
#include "NotepadForm.h"
#include "Note.h"
#include "DummyRow.h"
#include "GlyphCreator.h"

//����Ʈ������
PasteCommand::PasteCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{

}

//Execute
void PasteCommand::Execute()
{
	//1. �޸����� ���� �ٰ� ������ ��ġ�� ���Ѵ�.
	Long currentRowPos = 0;
	Long currentLetterPos = 0;
	//2. �޸��忡�� ���õ� texts�� ������
	if (this->notepadForm->isSelecting == true)
	{
		currentRowPos = this->notepadForm->note->GetCurrent();
		currentLetterPos = this->notepadForm->current->GetCurrent();
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
	}
	//3. ������ texts�� �� ��������(������)
	if (this->notepadForm->isSelecting == false)
	{
		//3.1 �����ϱ�� �ٿ��ֱ� �޴��� ��Ȱ��ȭ ��Ų��.
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	}

	//�ܺ� Ŭ�����忡 ���ڿ��� ����Ǿ� ������ �ܺ�Ŭ�����忡�� ���ڿ��� �޾ƿͼ� contents�� �����Ѵ�.
	//contents�� ���ٷ� ���ڿ��� �����ϴµ� ���� �����ϴ� ���� '\n(���๮��)'�����̴�. �� ���๮�ڸ�
	//���ؼ� �ٴ����� ������ �����ؾ��Ѵ�.
	//4. �ܺ� Ŭ�����忡 ���ڿ��� ����Ǿ� ������
	//Ŭ������� ���� ���α׷����� ������ �� �ִ� ����̱� ������ � ���α׷�(������)�� Ŭ�����带
	//����ϴ��� �����ϰ� ����ϴ� ���� ����
	unsigned int priority_list = CF_TEXT;
	char* content = NULL;
	if (GetPriorityClipboardFormat(&priority_list, 1) == CF_TEXT)
	{
		//4.1 Ŭ�����忡 �ִ� ���ڿ� ������ ��� ���ؼ� Ŭ�����带 ����.
		if (this->notepadForm->OpenClipboard())
		{
			//4.1.1 Ŭ�����忡�� ���ڿ��� ����� �޸��� �ڵ� ���� ��´�.
			HANDLE h_clipboard_data = GetClipboardData(CF_TEXT);
			if (h_clipboard_data != NULL)
			{
				//4.1.1.1 �޸� �ڵ� ���� �̿��Ͽ� ���� ��밡���� �ּҸ� ��´�.
				char* p_clipboard_data = (char*)::GlobalLock(h_clipboard_data);
				//4.1.1.2 Ŭ�����忡 ����� ���ڿ��� ���̸� ���Ѵ�. ('\0'���� ũ��)
				int contentLength = strlen(p_clipboard_data) + 1;
				//4.1.1.3 Ŭ�����忡 ����� ���ڿ��� �����ϱ� ���� �޸𸮸� �Ҵ��Ѵ�.
				content = new char[contentLength];
				//4.1.1.4 �Ҵ�� �޸𸮿� Ŭ������ ���ڿ��� �����Ѵ�.
				memcpy(content, p_clipboard_data, contentLength);
				//4.1.1.5 ���ڿ��� �����ϱ� ���ؼ� Lock�ߴ� �޸𸮸� �����Ѵ�.
				GlobalUnlock(h_clipboard_data);
			}
			//4.1.2 Ŭ�����带 �ݴ´�.
			CloseClipboard();
		}
		//4.2 ������ �ؽ�Ʈ�� ���� ����Ʈ�� �����Ѵ�.
		Glyph* copyNote = new Note();
		//4.3 ���� �����Ѵ�. 
		Glyph* copyRow = new Row();
		//344 ���� ������ ���� ���� ������ ��Ʈ�� �߰��Ѵ�.
		Long rowIndex = copyNote->Add(copyRow);
		//4.5 ������ ������ ���������� �ݺ��Ѵ�.(�������� �ι��ڰ� ����Ǿ� ����)
		GlyphCreator glyphCreator;
		Glyph* glyph = 0;
		Long letterIndex = 0;
		char doubleByteLetter[3];
		char singleByteLetter[2];
		Long i = 0;//�迭��� �ʱ�ȭ
		//4.6 ������ ������(NULL����)�� �ƴѵ��� �ݺ��Ѵ�
		while (content[i] != '\0')
		{
			//4.6.1 ������ ������ �� �迭��Ұ� 2byte �������� 1byte �������� �Ǵ��Ѵ�.
			if ((content[i] & 0x80))//�ѱ��̸�(2byte����)
			{
				//4.6.1.1 doubleByteLetter�� �����Ѵ�.
				doubleByteLetter[0] = content[i];
				i++;
				doubleByteLetter[1] = content[i];
				doubleByteLetter[2] = '\0';
				//4.6.1.2 doubleByteLetter�� �����Ѵ�.(���丮�޼ҵ�����)
				glyph = glyphCreator.Create((char*)doubleByteLetter);
				//4.6.1.3 �ٿ� doubleByteLetter�� �߰��Ѵ�.
				letterIndex = copyNote->GetAt(rowIndex)->Add(glyph);
			}
			//4.6.2 ���๮���̸�(���� �ٲٸ� ���� �߰��Ѵ�)
			else if (content[i] == '\n' || content[i] == '\r')
			{
				//4.6.2.1 row�� �����Ѵ�.
				glyph = glyphCreator.Create(content + i);
				//4.6.2.2 ���ο� ���� �߰��Ѵ�.
				rowIndex = copyNote->Add(glyph);
				//4.6.2.3 ���� ���ڰ� '\n'�̸�
				if (content[i + 1] == '\n')
				{
					i++;
				}
			}
			//4.6.3 �Է¹��� ���ڰ� �ѱ��� �ƴϰ� ���๮�ڰ� �ƴϸ�(1byte����)
			else
			{
				//4.6.3.1 singleByteLetter�� �����Ѵ�.
				singleByteLetter[0] = content[i];
				singleByteLetter[1] = '\0';
				//4.6.3.2 singleByteLetter�� �����Ѵ�.
				glyph = glyphCreator.Create(singleByteLetter);
				//4.6.3.3 �ٿ� singleByteLetter�� �߰��Ѵ�.
				letterIndex = copyNote->GetAt(rowIndex)->Add(glyph);
			}
			i++;
		}
		//4.7 �տ��� content�� �Ҵ��߱� ������ ���⼭ �Ҵ������� ������Ѵ�.
		if (content != 0)
		{
			delete[] content;
		}
		//4.8 notepadForm�� clipboard�� ���� ������ Note(������ ����)�� �߰������ش�.
		Long noteIndex = this->notepadForm->clipboard->Add(copyNote);
	}
	//4. ���õ� texts���� �������� ���� �ٰ� ������ġ�� ����Ǳ� ������ �ٽ� �����ش�.
	currentRowPos = this->notepadForm->note->GetCurrent();
	currentLetterPos = this->notepadForm->current->GetCurrent();
	//5. �޸����� Ŭ�����忡�� ���� ��Ʈ(������ ����)�� ȣ���Ѵ�.
	Glyph* copyNote = this->notepadForm->clipboard->
		GetAt(this->notepadForm->clipboard->GetCurrent());
	//6. �޸����� ���� ���� ���Ѵ�.
	Glyph* currentRow = this->notepadForm->note->GetAt(currentRowPos);
	//7. �޸����� ���� ������ġ�� ���� ���ڰ������� ������
	//�ڿ� split�� ���ڰ� ������ split�ϰ� ������ ���ڶ� �ڿ� ���ڰ� ������ split���� �ʴ´�.
	Glyph* splitedRow = 0;
	bool isSplited = false;
	if (currentLetterPos < currentRow->GetLength())
	{
		//7.1 �޸����� ���� �ٿ��� ���� ������ġ �������� split��Ų��.(splitedRow�� ���� �Ҵ��)
		splitedRow = currentRow->Split(currentLetterPos);
		//7.2 split�� �Ǿ��� ������ isSplited�� true�� �ٲ��ش�.
		isSplited = true;
	}
	//8. Ŭ�������� ���� ��Ʈ�� ù��° ���� ���Ѵ�.
	Long firstCopyRowPos = 0;
	Glyph* firstCopyRow = copyNote->GetAt(firstCopyRowPos);
	Glyph* letter = 0;//���ڸ� ���� ����
	//9. Ŭ�������� ���� ��Ʈ�� ù��° ���� �������� �������� �ݺ��Ѵ�.
	Long letterIndex = 0;
	while (letterIndex < firstCopyRow->GetLength())
	{
		//9.1 Ŭ�������� ���� ��Ʈ�� ù��° ���� ���ڸ� ���Ѵ�.
		letter = firstCopyRow->GetAt(letterIndex);
		//9.2 ���ڸ� �޸����� ���� �ٿ� �߰��Ѵ�.(���� ����)
		currentRow->Add(letter->Clone());
		//9.3 ���� ���ڷ� �̵��Ѵ�.
		letterIndex++;
	}
	//10. Ŭ�������� ���� ��Ʈ�� ù��° �ٿ��� ���� �ٷ� �̵��Ѵ�.
	Long nextCopyRowPos = firstCopyRowPos + 1;
	Glyph* copyRow = firstCopyRow;//ù��° ���� ����Ʈ
	Long i = currentRowPos;
	//11. Ŭ�������� ���� ��Ʈ(������ ����)�� �������ٱ��� �ݺ��Ѵ�.
	while (nextCopyRowPos < copyNote->GetLength())
	{
		//11.1 ������ ���� ���Ѵ�.
		copyRow = copyNote->GetAt(nextCopyRowPos);
		//11.2 �޸����� ���� ���� ���� �ٷ� �̵��Ѵ�.
		i++;
		//11.3 ���� ���� �޸����� ���� ���� ��ġ �������� �����ִ´�.(���� ����)
		i = this->notepadForm->note->Add(i, copyRow->Clone());
		//11.4 ������ ��Ʈ���� ���� �ٷ� �̵��Ѵ�.
		nextCopyRowPos++;
	}
	//12. �޸��忡�� ���� ���� ���Ѵ�.
	this->notepadForm->current = this->notepadForm->note->GetAt(i);
	//13. �޸��忡�� ���� ������ġ�� ���Ѵ�.
	currentLetterPos = this->notepadForm->current->GetCurrent();
	//14. �޸��忡�� �Ʊ� split�ߴ� ���ڵ��� ������
	if (isSplited == true)
	{
		//14.1 split�� ���� �޸����� ���� �ٿ� Join��Ų��.
		splitedRow->Join(this->notepadForm->current);
		//14.2 splitedRow�� �Ҵ����� �����ش�.
		if (splitedRow != 0)
		{
			delete splitedRow;
		}
		//14.3 �޸��忡�� ���� ������ġ�� �ٽ� �������ش�.
		this->notepadForm->current->Move(currentLetterPos);
	}
	//15. �޸��� ���� *�� �߰��Ѵ�.
	string name = this->notepadForm->fileName;
	name.insert(0, "*");
	name += " - �޸���";
	this->notepadForm->SetWindowText(CString(name.c_str()));
	//16. �޸��忡 ��������� ������ �����Ѵ�.
	this->notepadForm->isDirty = true;
	//17. ��������� �����Ѵ�.
	this->notepadForm->Notify();
	this->notepadForm->Invalidate(TRUE);
}

//�Ҹ���
PasteCommand::~PasteCommand()
{

}