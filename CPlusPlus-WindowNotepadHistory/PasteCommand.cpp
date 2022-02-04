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
		//2.1 RemoveCommand�� �޼����� ������ ���ÿ����� �����.
		this->notepadForm->SendMessage(WM_COMMAND, IDM_NOTE_REMOVE);
	}
	//�ܺ� Ŭ�����忡 ���ڿ��� ����Ǿ� ������ �ܺ�Ŭ�����忡�� ���ڿ��� �޾ƿͼ� contents�� �����Ѵ�.
	//contents�� ���ٷ� ���ڿ��� �����ϴµ� ���� �����ϴ� ���� '\n(���๮��)'�����̴�. �� ���๮�ڸ�
	//���ؼ� �ٴ����� ������ �����ؾ��Ѵ�.
	//3. �ܺ� Ŭ�����忡 ���ڿ��� ����Ǿ� ������
	//Ŭ������� ���� ���α׷����� ������ �� �ִ� ����̱� ������ � ���α׷�(������)�� Ŭ�����带
	//����ϴ��� �����ϰ� ����ϴ� ���� ����
	unsigned int priority_list = CF_TEXT;
	char* content = NULL;
	if (GetPriorityClipboardFormat(&priority_list, 1) == CF_TEXT)
	{
		//3.1 Ŭ�����忡 �ִ� ���ڿ� ������ ��� ���ؼ� Ŭ�����带 ����.
		if (this->notepadForm->OpenClipboard())
		{
			//3.1.1 Ŭ�����忡�� ���ڿ��� ����� �޸��� �ڵ� ���� ��´�.
			HANDLE h_clipboard_data = GetClipboardData(CF_TEXT);
			if (h_clipboard_data != NULL)
			{
				//3.1.1.1 �޸� �ڵ� ���� �̿��Ͽ� ���� ��밡���� �ּҸ� ��´�.
				char* p_clipboard_data = (char*)::GlobalLock(h_clipboard_data);
				//3.1.1.2 Ŭ�����忡 ����� ���ڿ��� ���̸� ���Ѵ�. ('\0'���� ũ��)
				int contentLength = strlen(p_clipboard_data) + 1;
				//3.1.1.3 Ŭ�����忡 ����� ���ڿ��� �����ϱ� ���� �޸𸮸� �Ҵ��Ѵ�.
				content = new char[contentLength];
				//3.1.1.4 �Ҵ�� �޸𸮿� Ŭ������ ���ڿ��� �����Ѵ�.
				memcpy(content, p_clipboard_data, contentLength);
				//3.1.1.5 ���ڿ��� �����ϱ� ���ؼ� Lock�ߴ� �޸𸮸� �����Ѵ�.
				GlobalUnlock(h_clipboard_data);
			}
			//3.1.2 Ŭ�����带 �ݴ´�.
			CloseClipboard();
		}
		//3.2 ������ �ؽ�Ʈ�� ���� ����Ʈ�� �����Ѵ�.
		Glyph* copyNote = new Note();
		//3.3 ���� �����Ѵ�. 
		Glyph* copyRow = new Row();
		//3.4 ���� ������ ���� ���� ������ ��Ʈ�� �߰��Ѵ�.
		Long rowIndex = copyNote->Add(copyRow);
		//3.5 ������ ������ ���������� �ݺ��Ѵ�.(�������� �ι��ڰ� ����Ǿ� ����)
		GlyphCreator glyphCreator;
		Glyph* glyph = 0;
		Long letterIndex = 0;
		char doubleByteLetter[3];
		char singleByteLetter[2];
		Long i = 0;//�迭��� �ʱ�ȭ
		//3.6 ������ ������(NULL����)�� �ƴѵ��� �ݺ��Ѵ�
		while (content[i] != '\0')
		{
			//3.6.1 ������ ������ �� �迭��Ұ� 2byte �������� 1byte �������� �Ǵ��Ѵ�.
			if ((content[i] & 0x80))//�ѱ��̸�(2byte����)
			{
				//3.6.1.1 doubleByteLetter�� �����Ѵ�.
				doubleByteLetter[0] = content[i];
				i++;
				doubleByteLetter[1] = content[i];
				doubleByteLetter[2] = '\0';
				//3.6.1.2 doubleByteLetter�� �����Ѵ�.(���丮�޼ҵ�����)
				glyph = glyphCreator.Create((char*)doubleByteLetter);
				//3.6.1.3 �ٿ� doubleByteLetter�� �߰��Ѵ�.
				letterIndex = copyNote->GetAt(rowIndex)->Add(glyph);
			}
			//3.6.2 ���๮���̸�(���� �ٲٸ� ���� �߰��Ѵ�)
			else if (content[i] == '\n' || content[i] == '\r')
			{
				//3.6.2.1 row�� �����Ѵ�.
				glyph = glyphCreator.Create(content + i);
				//3.6.2.2 ���ο� ���� �߰��Ѵ�.
				rowIndex = copyNote->Add(glyph);
				//3.6.2.3 ���� ���ڰ� '\n'�̸�
				if (content[i + 1] == '\n')
				{
					i++;
				}
			}
			//3.6.3 �Է¹��� ���ڰ� �ѱ��� �ƴϰ� ���๮�ڰ� �ƴϸ�(1byte����)
			else
			{
				//3.6.3.1 singleByteLetter�� �����Ѵ�.
				singleByteLetter[0] = content[i];
				singleByteLetter[1] = '\0';
				//3.6.3.2 singleByteLetter�� �����Ѵ�.
				glyph = glyphCreator.Create(singleByteLetter);
				//3.6.3.3 �ٿ� singleByteLetter�� �߰��Ѵ�.
				letterIndex = copyNote->GetAt(rowIndex)->Add(glyph);
			}
			i++;
		}
		//3.7 �տ��� content�� �Ҵ��߱� ������ ���⼭ �Ҵ������� ������Ѵ�.
		if (content != 0)
		{
			delete[] content;
		}
		//3.8 notepadForm�� clipboard�� ���� ������ Note(������ ����)�� �߰������ش�.
		Long noteIndex = this->notepadForm->clipboard->Add(copyNote);
	}
	//4. �޸����� Ŭ�����忡�� ���� ��Ʈ(������ ����)�� ȣ���Ѵ�.
	Glyph* copyNote = this->notepadForm->clipboard->
		GetAt(this->notepadForm->clipboard->GetCurrent());
	//5. ���õ� texts���� �������� ���� �ٰ� ������ġ�� ����Ǳ� ������ �ٽ� �����ش�.
	currentRowPos = this->notepadForm->note->GetCurrent();
	currentLetterPos = this->notepadForm->current->GetCurrent();
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
	//16. �޸��� ���� *�� �߰��Ѵ�.
	string name = this->notepadForm->fileName;
	name.insert(0, "*");
	name += " - �޸���";
	this->notepadForm->SetWindowText(CString(name.c_str()));
	//17. �޸��忡 ��������� ������ �����Ѵ�.
	this->notepadForm->isDirty = true;
}

//�Ҹ���
PasteCommand::~PasteCommand()
{

}