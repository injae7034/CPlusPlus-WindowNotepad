#include "PasteCommand.h"
#include "NotepadForm.h"
#include "Note.h"
#include "DummyRow.h"
#include "GlyphCreator.h"
#include "TextExtent.h"
#include "RowAutoChange.h"

//����Ʈ������
PasteCommand::PasteCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{
	this->startRowIndex = notepadForm->note->GetCurrent();
	this->startLetterIndex = notepadForm->current->GetCurrent();
	this->rowIndex = notepadForm->note->GetCurrent();
	this->letterIndex = notepadForm->current->GetCurrent();
	this->glyph = 0;
	this->isUndoMacroEnd = false;
	this->isRedoMacroEnd = false;
	this->isRedone = false;
	this->isSelectedTextsRemoved = false;//ó���� ������ ���� ���ÿ����� �����������Ƿ� false�� ����Ʈ��
}

//Execute
void PasteCommand::Execute()
{
	//1. RowAutoChange�� �����Ѵ�.
	RowAutoChange rowAutoChange(this->notepadForm);
	Long changedRowPos = 0;
	Long changedLetterPos = 0;
	Long originRowPos = this->rowIndex;
	Long originLetterPos = this->letterIndex;
	//���ÿ����� ������ ���ÿ��� ����
	//2. �޸��忡�� ���õ� texts�� ������
	if (this->notepadForm->isSelecting == true)
	{
		//2.1 RemoveCommand�� �޼����� ������ ���ÿ����� �����.
		this->notepadForm->SendMessage(WM_COMMAND, IDM_NOTE_REMOVE);
		//2.2 Command���� ���ÿ����� ���������� ǥ���Ѵ�.
		this->isSelectedTextsRemoved = true;
	}
	//3. ���� ���� ��ġ�� ���� ��ġ�� ���Ѵ�.
	Long currentRowPos = this->notepadForm->note->GetCurrent();
	Long currentLetterPos = this->notepadForm->current->GetCurrent();
	//4. PasteCommand�� �ٽ� ����Ǹ�
	if (this->isRedone == true)
	{
		//4.1 ���� ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
		currentRowPos = this->notepadForm->note->Move(this->rowIndex);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->Move(this->letterIndex);
		//4.2 �ڵ������� �������̸�(command�� �ٰ� ���� ��ġ�� �׻� ��¥ �ٰ� ���� ��ġ�� ����Ǿ� ����)
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//4.2.1 ����� ȭ�� ũ�⿡ �´� �ٰ� ĳ���� ��ġ�� ���Ѵ�.
			rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
				&changedRowPos);
			//4.2.2 ���� ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
			currentRowPos = this->notepadForm->note->Move(changedRowPos);
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			currentLetterPos = this->notepadForm->current->Move(changedLetterPos);
		}
	}
	Glyph* currentRow = 0;
	Glyph* splitedRow = 0;
	Long firstCopyRowPos = 0;
	Glyph* firstCopyRow = 0;
	Glyph* letter = 0;//���ڸ� ���� ����
	Long letterIndex = 0;
	Long nextCopyRowPos = 0;
	Glyph* copyRow = 0;
	Long i = 0;
	bool isSplited = false;
	//5. ó�� �����̸�
	if (this->isRedone == false)
	{
		//�ܺ� Ŭ�����忡 ���ڿ��� ����Ǿ� ������ �ܺ�Ŭ�����忡�� ���ڿ��� �޾ƿͼ� contents�� �����Ѵ�.
		//contents�� ���ٷ� ���ڿ��� �����ϴµ� ���� �����ϴ� ���� '\n(���๮��)'�����̴�. �� ���๮�ڸ�
		//���ؼ� �ٴ����� ������ �����ؾ��Ѵ�.
		//5.1 �ܺ� Ŭ�����忡 ���ڿ��� ����Ǿ� ������
		//Ŭ������� ���� ���α׷����� ������ �� �ִ� ����̱� ������ � ���α׷�(������)�� Ŭ�����带
		//����ϴ��� �����ϰ� ����ϴ� ���� ����
		Glyph* newRow = 0;
		unsigned int priority_list = CF_TEXT;
		char* content = NULL;
		if (GetPriorityClipboardFormat(&priority_list, 1) == CF_TEXT)
		{
			//5.1.1 Ŭ�����忡 �ִ� ���ڿ� ������ ��� ���ؼ� Ŭ�����带 ����.
			if (this->notepadForm->OpenClipboard())
			{
				//5.1.1.1 Ŭ�����忡�� ���ڿ��� ����� �޸��� �ڵ� ���� ��´�.
				HANDLE h_clipboard_data = GetClipboardData(CF_TEXT);
				if (h_clipboard_data != NULL)
				{
					//5.1.1.1.1 �޸� �ڵ� ���� �̿��Ͽ� ���� ��밡���� �ּҸ� ��´�.
					char* p_clipboard_data = (char*)::GlobalLock(h_clipboard_data);
					//5.1.1.1.2 Ŭ�����忡 ����� ���ڿ��� ���̸� ���Ѵ�. ('\0'���� ũ��)
					int contentLength = strlen(p_clipboard_data) + 1;
					//5.1.1.1.3 Ŭ�����忡 ����� ���ڿ��� �����ϱ� ���� �޸𸮸� �Ҵ��Ѵ�.
					content = new char[contentLength];
					//5.1.1.1.4 �Ҵ�� �޸𸮿� Ŭ������ ���ڿ��� �����Ѵ�.
					memcpy(content, p_clipboard_data, contentLength);
					//5.1.1.1.5 ���ڿ��� �����ϱ� ���ؼ� Lock�ߴ� �޸𸮸� �����Ѵ�.
					GlobalUnlock(h_clipboard_data);
				}
				//5.1.1.2 Ŭ�����带 �ݴ´�.
				CloseClipboard();
			}
			//5.1.2 ������ �ؽ�Ʈ�� ���� ����Ʈ�� �����Ѵ�.
			this->glyph = new Note();
			//5.1.3 ���� �����Ѵ�. 
			newRow = new Row();
			//5.1.4 ���� ������ ���� ���� ������ ��Ʈ�� �߰��Ѵ�.
			Long rowIndex = this->glyph->Add(newRow);
			//5.1.5 ������ ������ ���������� �ݺ��Ѵ�.(�������� �ι��ڰ� ����Ǿ� ����)
			GlyphCreator glyphCreator;
			Glyph* glyph = 0;
			Long letterIndex = 0;
			char doubleByteLetter[3];
			char singleByteLetter[2];
			Long i = 0;//�迭��� �ʱ�ȭ
			//5.1.6 ������ ������(NULL����)�� �ƴѵ��� �ݺ��Ѵ�
			while (content[i] != '\0')
			{
				//5.1.6.1 ������ ������ �� �迭��Ұ� 2byte �������� 1byte �������� �Ǵ��Ѵ�.
				if ((content[i] & 0x80))//�ѱ��̸�(2byte����)
				{
					//5.1.6.1.1 doubleByteLetter�� �����Ѵ�.
					doubleByteLetter[0] = content[i];
					i++;
					doubleByteLetter[1] = content[i];
					doubleByteLetter[2] = '\0';
					//5.1.6.1.2 doubleByteLetter�� �����Ѵ�.(���丮�޼ҵ�����)
					glyph = glyphCreator.Create((char*)doubleByteLetter);
					//5.1.6.1.3 �ٿ� doubleByteLetter�� �߰��Ѵ�.
					letterIndex = this->glyph->GetAt(rowIndex)->Add(glyph);
				}
				//5.1.6.2 ���๮���̸�(���� �ٲٸ� ���� �߰��Ѵ�)
				else if (content[i] == '\n' || content[i] == '\r')
				{
					//5.1.6.2.1 row�� �����Ѵ�.
					glyph = glyphCreator.Create(content + i);
					//5.1.6.2.2 ���ο� ���� �߰��Ѵ�.
					rowIndex = this->glyph->Add(glyph);
					//5.1.6.2.3 ���� ���ڰ� '\n'�̸�
					if (content[i + 1] == '\n')
					{
						i++;
					}
				}
				//5.1.6.3 �Է¹��� ���ڰ� �ѱ��� �ƴϰ� ���๮�ڰ� �ƴϸ�(1byte����)
				else
				{
					//5.1.6.3.1 singleByteLetter�� �����Ѵ�.
					singleByteLetter[0] = content[i];
					singleByteLetter[1] = '\0';
					//5.1.6.3.2 singleByteLetter�� �����Ѵ�.
					glyph = glyphCreator.Create(singleByteLetter);
					//5.1.6.3.3 �ٿ� singleByteLetter�� �߰��Ѵ�.
					letterIndex = this->glyph->GetAt(rowIndex)->Add(glyph);
				}
				i++;
			}
			//5.1.7 �տ��� content�� �Ҵ��߱� ������ ���⼭ �Ҵ������� ������Ѵ�.
			if (content != 0)
			{
				delete[] content;
			}
			//5.1.8 notepadForm�� clipboard�� ���� ������ Note(������ ����)�� �߰������ش�.
			Long noteIndex = this->notepadForm->clipboard->Add(this->glyph->Clone());
		}
	}
	//6. ������ content�� ������
	if (this->glyph != 0)
	{
		//�����ϴ� ���� ��ġ�� ������ġ�� �����Ѵ�.
		this->startRowIndex = currentRowPos;
		this->startLetterIndex = currentLetterPos;
		//6. �޸����� ���� ���� ���Ѵ�.
		currentRow = this->notepadForm->note->GetAt(currentRowPos);
		//7. �޸����� ���� ������ġ�� ���� ���ڰ������� ������
		//�ڿ� split�� ���ڰ� ������ split�ϰ� ������ ���ڶ� �ڿ� ���ڰ� ������ split���� �ʴ´�.
		splitedRow = 0;
		if (currentLetterPos < currentRow->GetLength())
		{
			//7.1 �޸����� ���� �ٿ��� ���� ������ġ �������� split��Ų��.(splitedRow�� ���� �Ҵ��)
			splitedRow = currentRow->Split(currentLetterPos);
			//7.2 split�� �Ǿ��� ������ isSplited�� true�� �ٲ��ش�.
			isSplited = true;
		}
		//8. Ŭ�������� ���� ��Ʈ�� ù��° ���� ���Ѵ�.
		firstCopyRowPos = 0;
		firstCopyRow = this->glyph->GetAt(firstCopyRowPos);
		letter = 0;//���ڸ� ���� ����
		//9. Ŭ�������� ���� ��Ʈ�� ù��° ���� �������� �������� �ݺ��Ѵ�.
		letterIndex = 0;
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
		nextCopyRowPos = firstCopyRowPos + 1;
		copyRow = firstCopyRow;//ù��° ���� ����Ʈ
		i = currentRowPos;
		//11. Ŭ�������� ���� ��Ʈ(������ ����)�� �������ٱ��� �ݺ��Ѵ�.
		while (nextCopyRowPos < this->glyph->GetLength())
		{
			//11.1 ������ ���� ���Ѵ�.
			copyRow = this->glyph->GetAt(nextCopyRowPos);
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
		//15. �ڵ������� �������̸� �ٿ����� �ٵ��� �ڵ���������ش�.
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//15.1 �ڵ�����Ŭ������ �����Ѵ�.
			RowAutoChange rowAutoChange(this->notepadForm);
			//15.2 �κ��ڵ������� �Ѵ�.
			Long endPastedRowPos = rowAutoChange.DoPartRows(currentRowPos, i);
			//15.3 �ٿ��ֱⰡ ������ �ٷ� �̵���Ų��.
			//�ٿ��ֱⰡ ������ ���� OnSize���� �κ��ڵ������� �ؼ� ó���Ǳ� ������ ĳ���� ��ġ�� �������ָ� ��!
			this->notepadForm->note->Move(endPastedRowPos);
			this->notepadForm->current->Move(currentLetterPos);
		}
		//10. �޸��� ���� *�� �߰��Ѵ�.
		string name = this->notepadForm->fileName;
		name.insert(0, "*");
		name += " - �޸���";
		this->notepadForm->SetWindowText(CString(name.c_str()));
		//11. �޸��忡 ��������� ������ �����Ѵ�.
		this->notepadForm->isDirty = true;
		//13. ���ڸ� ���� �Ŀ� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
		this->rowIndex = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
		this->letterIndex = this->notepadForm->current->GetCurrent();
		//14. �ڵ������� �������̸�(command�� �ٰ� ���� ��ġ�� �׻� ��¥ �ٰ� ���� ��ġ�� �����ؾ���)
		if (this->notepadForm->isRowAutoChanging == true)
		{
			Long changedRowPos = this->rowIndex;
			Long changedLetterPos = this->letterIndex;
			Long originRowPos = 0;
			Long originLetterPos = 0;
			//14.1 ����� ȭ�� ũ�⿡ �´� �ٰ� ĳ���� ��ġ�� ���Ѵ�.
			rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
				&originRowPos);
			//14.2 command�� ���ڸ� �Է��� �Ŀ� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
			this->rowIndex = originRowPos;
			this->letterIndex = originLetterPos;

			changedRowPos = this->startRowIndex;
			changedLetterPos = this->startLetterIndex;
			originRowPos = 0;
			originLetterPos = 0;
			//14.1 ����� ȭ�� ũ�⿡ �´� �ٰ� ĳ���� ��ġ�� ���Ѵ�.
			rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
				&originRowPos);
			//14.2 command�� ���ڸ� �Է��� �Ŀ� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
			this->startRowIndex = originRowPos;
			this->startLetterIndex = originLetterPos;
		}
	}
}

//�������
void PasteCommand::Unexecute()
{
	//1. RowAutoChange�� �����Ѵ�.
	RowAutoChange rowAutoChange(this->notepadForm);
	Long changedRowPos = 0;
	Long changedLetterPos = 0;
	Long originRowPos = this->startRowIndex;
	Long originLetterPos = this->startLetterIndex;
	//2. ���� �ٰ� ���� ��ġ�� ������ġ�� �̵���Ų��.(ĳ���� �ٸ� ���� ������ �� ���� ���ڰ� �������� ������)
	Long currentRowPos = this->notepadForm->note->Move(this->startRowIndex);
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	Long currentLetterPos = this->notepadForm->current->Move(this->startLetterIndex);
	//3. content�� ������ �ٰ� ���� ��ġ�� ���Ѵ�.
	Long endRowPos = this->rowIndex;
	Long endLetterPos = this->letterIndex;
	//4. �ڵ������� �������̸�(command�� �ٰ� ���� ��ġ�� �׻� ��¥ �ٰ� ���� ��ġ�� ����Ǿ� ����)
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//4.1 ����� ȭ�� ũ�⿡ �´� �ٰ� ĳ���� ��ġ�� ���Ѵ�.
		rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
			&changedRowPos);
		//4.2 ���� ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
		currentRowPos = this->notepadForm->note->Move(changedRowPos);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->Move(changedLetterPos);
		//4.3 ����� ȭ���� ũ�⿡ �°� ������ �ٰ� ������ġ�� �ٽ� ���Ѵ�.
		changedRowPos = 0;
		changedLetterPos = 0;
		originRowPos = this->rowIndex;
		originLetterPos = this->letterIndex;
		rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
			&changedRowPos);
		endRowPos = changedRowPos;
		endLetterPos = changedLetterPos;
	}
	//5. �����ϴ� ���� ���Ѵ�.
	Glyph* startRow = this->notepadForm->current;
	//6. �����ϴ� �ٰ� ������ ���� ������
	if (currentRowPos == endRowPos)
	{
		//6.1 �����ϴ� ������ġ���� ������ ������ġ���� ���ڸ� �����.
		while (currentLetterPos < endLetterPos)
		{
			startRow->Remove(currentLetterPos);
			endLetterPos--;
		}
	}
	//7. �����ϴ� �ٰ� ������ ���� ���� �ٸ���
	else
	{
		Long i = startLetterIndex;
		//7.1 �����ϴ� ���� ���ڸ� �����.
		while (i < startRow->GetLength())
		{
			//7.1.1 ���ڸ� �����.
			startRow->Remove(i);
		}
		//7.2 ���� ���� ���Ѵ�.
		Long nextRowPos = currentRowPos + 1;
		//7.3 ���� ���� ������ �ٺ��� ���� ���� �ݺ��Ѵ�.
		Glyph* row = 0;
		while (nextRowPos < endRowPos)
		{
			//7.3.1 ���� ���Ѵ�.
			row = this->notepadForm->note->GetAt(nextRowPos);
			i = 0;
			//7.3.2 ���� ���ڰ������� �ݺ��Ѵ�.
			while (i < row->GetLength())
			{
				//7.3.2.1 ���ڸ� �����.
				row->Remove(i);
			}
			//7.3.3 ���ڰ� �� �������� ������ ���� �����.
			this->notepadForm->note->Remove(nextRowPos);
			//7.3.4 ���� ���� �������� ������ ���ٴ� ���� ��ġ�� �� �� ���ҽ�Ų��.
			endRowPos--;
		}
		//7.4 ������ ���� ���Ѵ�.
		Glyph* endRow = this->notepadForm->note->GetAt(endRowPos);
		//7.5 ������ ���� ó�� ���ں�Ÿ ������ ������ġ���� �ݺ��Ѵ�.
		i = 0;
		while (i < endLetterPos)
		{
			//7.5.1 ���ڸ� �����.
			endRow->Remove(i);
			endLetterPos--;
		}
		//7.6 ������ ���� �����ϴ� �ٿ� Join��Ų��.
		endRow->Join(startRow);
		//7.7 ������ ���� ��Ʈ���� �����.
		this->notepadForm->note->Remove(endRowPos);
		//7.8 ���� ��ġ�� ������Ų��.
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->Move(startLetterIndex);
	}
	
	//12. �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//12.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
		//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
		this->notepadForm->SendMessage(WM_SIZE);
	}
	//14. ���ڸ� �Է��� �Ŀ� command�� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
	this->rowIndex = this->notepadForm->note->GetCurrent();
	this->letterIndex = this->notepadForm->current->GetCurrent();
	this->startRowIndex = this->rowIndex;
	this->startLetterIndex = this->letterIndex;
	//15. �ڵ������� �������̸�(command�� �ٰ� ���� ��ġ�� �׻� ��¥ �ٰ� ���� ��ġ�� �����ؾ���)
	if (this->notepadForm->isRowAutoChanging == true)
	{
		Long changedRowPos = this->rowIndex;
		Long changedLetterPos = this->letterIndex;
		Long originRowPos = 0;
		Long originLetterPos = 0;
		//15.1 ����� ȭ�� ũ�⿡ �´� �ٰ� ĳ���� ��ġ�� ���Ѵ�.
		rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
			&originRowPos);
		//15.2 command�� ���ڸ� �Է��� �Ŀ� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
		this->rowIndex = originRowPos;
		this->letterIndex = originLetterPos;
		this->startRowIndex = this->rowIndex;
		this->startLetterIndex = this->letterIndex;
	}
}

//�ٽý������� ���� ���ϱ�
bool PasteCommand::IsRedone()
{
	return this->isRedone;
}

//�����ϴ� ���� ��ġ���ϱ�
Long PasteCommand::GetStartRowIndex()
{
	return this->startRowIndex;
}

//�����ϴ� ������ ��ġ���ϱ�
Long PasteCommand::GetStartLetterIndex()
{
	return this->startLetterIndex;
}

//���� ��ġ ���ϱ�
Long PasteCommand::GetRowIndex()
{
	return this->rowIndex;
}
//���� ��ġ ���ϱ�
Long PasteCommand::GetLetterIndex()
{
	return this->letterIndex;
}

//SetMacroEnd(������� �� �ٽý��� ��ũ����� �������� ����)
void PasteCommand::SetUndoMacroEnd()
{
	this->isUndoMacroEnd = true;
}
void PasteCommand::SetRedoMacroEnd()
{
	this->isRedoMacroEnd = true;
}

//������� ������������ ���ϱ�
bool PasteCommand::IsUndoMacroEnd()
{
	return this->isUndoMacroEnd;
}
//�ٽý��� ������������ ���ϱ� 
bool PasteCommand::IsRedoMacroEnd()
{
	return this->isRedoMacroEnd;
}

//SetRedone(�ٽ� �����̶�� ������)
void PasteCommand::SetRedone()
{
	this->isRedone = true;
}

//���ÿ����� ���������� Ȯ�� ����
bool PasteCommand::IsSelectedTextsRemoved()
{
	return this->isSelectedTextsRemoved;
}

//�Ҹ���
PasteCommand::~PasteCommand()
{
	if (this->glyph != 0)
	{
		delete this->glyph;
	}
}