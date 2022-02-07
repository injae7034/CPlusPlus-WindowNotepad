#include "PasteCommand.h"
#include "NotepadForm.h"
#include "Note.h"
#include "DummyRow.h"
#include "GlyphCreator.h"
#include "TextExtent.h"
#include "RowAutoChange.h"
#include "SelectingTexts.h"

//����Ʈ������
PasteCommand::PasteCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{
	this->glyph = 0;//�ٿ����� content�� ������ ����
	this->note = 0;//���õ� texts�� ������ ����� ���� ������ ����
	this->startPasteRowIndex = 0;//�ٿ��ֱⰡ ���۵Ǵ� ����ġ
	this->startPasteLetterIndex = 0;//�ٿ��ֱⰡ ���۵Ǵ� ������ġ
	this->rowIndex = notepadForm->note->GetCurrent();
	this->letterIndex = notepadForm->current->GetCurrent();
	this->isUndoMacroEnd = false;
	this->isRedoMacroEnd = false;
	this->isRedone = false;
	this->isDirty = false;//ó���� ������ ���� ��������� �����Ƿ� false�� ����Ʈ����.
	this->isSelectedTextsRemoved = false;//ó���� ������ ���� ���ÿ����� �����������Ƿ� false�� ����Ʈ��
}

//�� this->rowIndex�� this->letterIndex�� �׻� ���� ��¥ �ٰ� ���� ��ġ�� �����Ѵ�. �׸��� Execute�� ��
//�ڵ������� �Ǿ� ������ �ڵ������� �� ���¿� �°� ��ġ�� �������ְ�, �ڵ������� �ȵǾ������� �״�� ���� ��.
//this->startPasteRowIndex�� this->startPasteLetterIndex�� �ڵ������� ���� ���̸� ���� ���� ������ ����
//�����ϰ�, �ڵ������� ���� ���� �ƴϸ� �ڵ������� ���� ���� �ƴ� ������ ���� �����Ѵ�. �׷��� ������ �� �Ź�
//Execute�� �� ���� �ڵ������� ���� ���� ���µ� ���� ���� ���°� �ƴϵ� �� ���� �����ϰ�, rowIndex�� letter
//Index�� �Ź� �ڵ����� ���ο� ���� ���� �ٲٱ� ������ �̻��� ����! �׷��� rowIndex�� letterIndex�� ��¥ �ٰ�
//���� ��ġ�� �����ϰ�, this->startPasteRowIndex�� this->startPasteLetterIndex�� �ڵ����� ���� ���ο� 
//������� �׳� �Ź� �� ���� �����ϸ� �ȴ�.

//Execute
void PasteCommand::Execute()
{
	//1. RowAutoChange�� �����Ѵ�.
	RowAutoChange rowAutoChange(this->notepadForm);
	Long changedRowPos = 0;
	Long changedLetterPos = 0;
	Long originRowPos = this->rowIndex;
	Long originLetterPos = this->letterIndex;
	//2. ���� ���� ��ġ�� ���� ��ġ�� ���Ѵ�.
	Long currentRowPos = this->notepadForm->note->GetCurrent();
	Long currentLetterPos = this->notepadForm->current->GetCurrent();
	//3. PasteCommand�� �ٽ� ����Ǹ�
	if (this->isRedone == true)
	{
		//3.1 ���� ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
		currentRowPos = this->notepadForm->note->Move(this->rowIndex);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->Move(this->letterIndex);
		//3.2 �ڵ������� �������̸�(command�� �ٰ� ���� ��ġ�� �׻� ��¥ �ٰ� ���� ��ġ�� ����Ǿ� ����)
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//3.2.1 ����� ȭ�� ũ�⿡ �´� �ٰ� ĳ���� ��ġ�� ���Ѵ�.
			rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
				&changedRowPos);
			//3.2.2 ���� ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
			currentRowPos = this->notepadForm->note->Move(changedRowPos);
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			currentLetterPos = this->notepadForm->current->Move(changedLetterPos);
		}
	}
	//4. ó�� �����̸�
	if (this->isRedone == false)
	{
		//�ܺ� Ŭ�����忡 ���ڿ��� ����Ǿ� ������ �ܺ�Ŭ�����忡�� ���ڿ��� �޾ƿͼ� contents�� �����Ѵ�.
		//contents�� ���ٷ� ���ڿ��� �����ϴµ� ���� �����ϴ� ���� '\n(���๮��)'�����̴�. �� ���๮�ڸ�
		//���ؼ� �ٴ����� ������ �����ؾ��Ѵ�.
		//4.1 �ܺ� Ŭ�����忡 ���ڿ��� ����Ǿ� ������
		//Ŭ������� ���� ���α׷����� ������ �� �ִ� ����̱� ������ � ���α׷�(������)�� Ŭ�����带
		//����ϴ��� �����ϰ� ����ϴ� ���� ����
		Glyph* newRow = 0;
		unsigned int priority_list = CF_TEXT;
		char* content = NULL;
		if (GetPriorityClipboardFormat(&priority_list, 1) == CF_TEXT)
		{
			//4.1.1 Ŭ�����忡 �ִ� ���ڿ� ������ ��� ���ؼ� Ŭ�����带 ����.
			if (this->notepadForm->OpenClipboard())
			{
				//4.1.1.1 Ŭ�����忡�� ���ڿ��� ����� �޸��� �ڵ� ���� ��´�.
				HANDLE h_clipboard_data = GetClipboardData(CF_TEXT);
				if (h_clipboard_data != NULL)
				{
					//4.1.1.1.1 �޸� �ڵ� ���� �̿��Ͽ� ���� ��밡���� �ּҸ� ��´�.
					char* p_clipboard_data = (char*)::GlobalLock(h_clipboard_data);
					//4.1.1.1.2 Ŭ�����忡 ����� ���ڿ��� ���̸� ���Ѵ�. ('\0'���� ũ��)
					int contentLength = strlen(p_clipboard_data) + 1;
					//4.1.1.1.3 Ŭ�����忡 ����� ���ڿ��� �����ϱ� ���� �޸𸮸� �Ҵ��Ѵ�.
					content = new char[contentLength];
					//4.1.1.1.4 �Ҵ�� �޸𸮿� Ŭ������ ���ڿ��� �����Ѵ�.
					memcpy(content, p_clipboard_data, contentLength);
					//4.1.1.1.5 ���ڿ��� �����ϱ� ���ؼ� Lock�ߴ� �޸𸮸� �����Ѵ�.
					GlobalUnlock(h_clipboard_data);
				}
				//4.1.1.2 Ŭ�����带 �ݴ´�.
				CloseClipboard();
			}
			//4.1.2 ������ �ؽ�Ʈ�� ���� ����Ʈ�� �����Ѵ�.
			this->glyph = new Note();
			//4.1.3 ���� �����Ѵ�. 
			newRow = new Row();
			//4.1.4 ���� ������ ���� ���� ������ ��Ʈ�� �߰��Ѵ�.
			Long rowIndex = this->glyph->Add(newRow);
			//4.1.5 ������ ������ ���������� �ݺ��Ѵ�.(�������� �ι��ڰ� ����Ǿ� ����)
			GlyphCreator glyphCreator;
			Glyph* glyph = 0;
			Long letterIndex = 0;
			char doubleByteLetter[3];
			char singleByteLetter[2];
			Long i = 0;//�迭��� �ʱ�ȭ
			//4.1.6 ������ ������(NULL����)�� �ƴѵ��� �ݺ��Ѵ�
			while (content[i] != '\0')
			{
				//4.1.6.1 ������ ������ �� �迭��Ұ� 2byte �������� 1byte �������� �Ǵ��Ѵ�.
				if ((content[i] & 0x80))//�ѱ��̸�(2byte����)
				{
					//4.1.6.1.1 doubleByteLetter�� �����Ѵ�.
					doubleByteLetter[0] = content[i];
					i++;
					doubleByteLetter[1] = content[i];
					doubleByteLetter[2] = '\0';
					//4.1.6.1.2 doubleByteLetter�� �����Ѵ�.(���丮�޼ҵ�����)
					glyph = glyphCreator.Create((char*)doubleByteLetter);
					//4.1.6.1.3 �ٿ� doubleByteLetter�� �߰��Ѵ�.
					letterIndex = this->glyph->GetAt(rowIndex)->Add(glyph);
				}
				//4.1.6.2 ���๮���̸�(���� �ٲٸ� ���� �߰��Ѵ�)
				else if (content[i] == '\n' || content[i] == '\r')
				{
					//4.1.6.2.1 row�� �����Ѵ�.
					glyph = glyphCreator.Create(content + i);
					//4.1.6.2.2 ���ο� ���� �߰��Ѵ�.
					rowIndex = this->glyph->Add(glyph);
					//4.1.6.2.3 ���� ���ڰ� '\n'�̸�
					if (content[i + 1] == '\n')
					{
						i++;
					}
				}
				//4.1.6.3 �Է¹��� ���ڰ� �ѱ��� �ƴϰ� ���๮�ڰ� �ƴϸ�(1byte����)
				else
				{
					//4.1.6.3.1 singleByteLetter�� �����Ѵ�.
					singleByteLetter[0] = content[i];
					singleByteLetter[1] = '\0';
					//4.1.6.3.2 singleByteLetter�� �����Ѵ�.
					glyph = glyphCreator.Create(singleByteLetter);
					//4.1.6.3.3 �ٿ� singleByteLetter�� �߰��Ѵ�.
					letterIndex = this->glyph->GetAt(rowIndex)->Add(glyph);
				}
				i++;
			}
			//4.1.7 �տ��� content�� �Ҵ��߱� ������ ���⼭ �Ҵ������� ������Ѵ�.
			if (content != 0)
			{
				delete[] content;
			}
			//4.1.8 notepadForm�� clipboard�� ���� ������ Note(������ ����)�� �߰������ش�.
			Long noteIndex = this->notepadForm->clipboard->Add(this->glyph->Clone());
		}
	}
	//5. ������ content�� ������
	if (this->glyph != 0)
	{
		//���ÿ����� ������ ���ÿ��� ����
		//5.1 �޸��忡�� ���õ� texts�� �ְų� �Ǵ� ���õ� ������ ��������
		if (this->notepadForm->isSelecting == true || this->isSelectedTextsRemoved == true)
		{
			//5.1.1 �ٽ� �����̸�
			if (this->isRedone == true)
			{
				//5.1.1.1 notepadForm�� ������ ���۵Ǵ� ���� ��ġ�� ���� ��ġ�� �缳�����ش�. 
				this->notepadForm->selectedStartYPos = this->startPasteRowIndex;
				this->notepadForm->selectedStartXPos = this->startPasteLetterIndex;
			}
			//5.1.2 ������ ���۵Ǵ� �ٰ� ���� ��ġ, ������ ������ �ٰ� ���� ��ġ�� �����Ѵ�.
			Long selectedStartRowPos = this->notepadForm->selectedStartYPos;//������ ���۵Ǵ� ��
			Long selectedStartLetterPos = this->notepadForm->selectedStartXPos;//������ ���۵Ǵ� ����
			Long selectedEndRowPos = currentRowPos;//������ ������ ��
			Long selectedEndLetterPos = currentLetterPos;//������ ������ ����
			//5.1.3 ó�� �����̸�
			if (this->isRedone == false)
			{
				//5.1.3.1 content�� �����ϰ� �����.
				this->note = this->notepadForm->note->CopySelectedTextsAndRemove(selectedStartRowPos,
					selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
			}
			//5.1.4 �ٽ� �����̸�
			else
			{
				//5.1.4.1 content�� �����.
				this->notepadForm->note->RemoveSelectedTexts(selectedStartRowPos,
					selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
			}
			//5.1.5 ������ ������ ������ ���� ���� ��ġ�� �ٽ� �������ش�.(note�ǿ���ȿ��� ���� ���� ��ġ�� ���� 
			//��ġ�� ������ ������ notepadForm�� current(������)�� ������ �� ��� notepadForm���� ���ش�.)
			currentRowPos = this->notepadForm->note->GetCurrent();
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			currentLetterPos = this->notepadForm->current->GetCurrent();
			//5.1.6 �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
			if (this->notepadForm->isRowAutoChanging == true)
			{
				//5.1.6.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
				//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
				this->notepadForm->SendMessage(WM_SIZE);
			}
			//5.1.7 �޸��忡�� ���õ� texts�� �� ������ ������ �޸��忡�� ������ �ȵ� ���·� �ٲ۴�.
			this->notepadForm->isSelecting = false;
			//5.1.8 ���õ� texts�� ������ ������ command�� ���õ� ������ �����ٰ� ǥ���Ѵ�.
			this->isSelectedTextsRemoved = true;
			//5.1.9 ������ ������ ������ ĳ���� x��ǥ�� 0���� �����Ѵ�.
			this->notepadForm->selectedStartXPos = 0;
			//5.1.10 ������ ������ ������ ĳ���� y��ǥ�� 0���� �����Ѵ�.
			this->notepadForm->selectedStartYPos = 0;
			//5.1.11 �����ϱ�, �߶󳻱�, ���� �޴��� ��Ȱ��ȭ ��Ų��.
			this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		}
		//5.2 �ٿ��ֱ� ���� �ٿ��ֱⰡ ���۵Ǵ� ���� ��ġ�� �����Ѵ�.
		this->startPasteRowIndex = currentRowPos;
		//5.3 �ٿ��ֱ� ���� �ٿ��ֱⰡ ���۵Ǵ� ������ ��ġ�� �����Ѵ�.
		this->startPasteLetterIndex = currentLetterPos;
		//5.4 ���� ��ġ�� command�� ������ �ִ� content�� �����Ѵ�.
		Long rowIndex = this->notepadForm->note->
			InsertTexts(currentRowPos, currentLetterPos, this->glyph);
		//5.5 �޸����� ���� ���� �����Ѵ�.
		this->notepadForm->current = this->notepadForm->note->GetAt(rowIndex);
		//5.6 �ڵ������� �������̸� �ٿ����� �ٵ��� �ڵ���������ش�.
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//5.6.1 �ڵ�����Ŭ������ �����Ѵ�.
			RowAutoChange rowAutoChange(this->notepadForm);
			//5.6.2 �κ��ڵ������� �Ѵ�.
			Long endPastedRowPos = rowAutoChange.DoPartRows(currentRowPos, rowIndex);
			//5.6.3 �ٿ��ֱⰡ ������ �ٷ� �̵���Ų��.
			//�ٿ��ֱⰡ ������ ���� OnSize���� �κ��ڵ������� �ؼ� ó���Ǳ� ������ ĳ���� ��ġ�� �������ָ� ��!
			currentRowPos = this->notepadForm->note->Move(endPastedRowPos);
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			currentLetterPos = this->notepadForm->current->GetCurrent();
			this->notepadForm->current->Move(currentLetterPos);
		}
		//5.7 ó�� �����̸�
		if (this->isRedone == false)
		{
			//5.7.1 �޸��� ���� *�� �߰��Ѵ�.
			string name = this->notepadForm->fileName;
			name.insert(0, "*");
			name += " - �޸���";
			this->notepadForm->SetWindowText(CString(name.c_str()));
			//5.7.2 �޸��忡 ��������� ������ �����Ѵ�.
			this->notepadForm->isDirty = true;
		}
		//5.8 ���ڸ� ���� �Ŀ� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
		this->rowIndex = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
		this->letterIndex = this->notepadForm->current->GetCurrent();
		//5.9 �ڵ������� �������̸�(command�� �ٰ� ���� ��ġ�� �׻� ��¥ �ٰ� ���� ��ġ�� �����ؾ���)
		if (this->notepadForm->isRowAutoChanging == true)
		{
			Long changedRowPos = this->rowIndex;
			Long changedLetterPos = this->letterIndex;
			Long originRowPos = 0;
			Long originLetterPos = 0;
			//5.9.1 ����� ȭ�� ũ�⿡ �´� �ٰ� ĳ���� ��ġ�� ���Ѵ�.
			rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
				&originRowPos);
			//5.9.2 command�� ���ڸ� �Է��� �Ŀ� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
			this->rowIndex = originRowPos;
			this->letterIndex = originLetterPos;
		}
		//5.10 command�� �������(�ٿ����� ����)�� ������ ǥ���Ѵ�.
		this->isDirty = true;
	}
}

//�������
void PasteCommand::Unexecute()
{
	//1. ������ ����ǰ� �ִ� ���̾�����
	if (this->notepadForm->isSelecting == true)
	{
		//1.1. ���õ� �ؽ�Ʈ�� ���������Ѵ�.(������ ������.)
		this->notepadForm->selectingTexts->Undo();
		//1.2 ������ ���� ���·� �ٲ۴�.
		this->notepadForm->isSelecting = false;
		//1.3 ������ ������ ������ ĳ���� x��ǥ�� 0���� �����Ѵ�.
		this->notepadForm->selectedStartXPos = 0;
		//1.4 ������ ������ ������ ĳ���� y��ǥ�� 0���� �����Ѵ�.
		this->notepadForm->selectedStartYPos = 0;
	}
	//2. RowAutoChange�� �����Ѵ�.
	RowAutoChange rowAutoChange(this->notepadForm);
	Long changedRowPos = 0;
	Long changedLetterPos = 0;
	Long originRowPos = this->rowIndex;
	Long originLetterPos = this->letterIndex;
	//3. ���� ���� ��ġ�� �̵���Ų��.(ĳ���� �ٸ� ���� ������ �� ���� ���ڰ� �������� ������)
	Long currentRowPos = this->notepadForm->note->Move(this->rowIndex);
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	//4. ���� ������ ��ġ�� �̵���Ų��.
	Long currentLetterPos = this->notepadForm->current->Move(this->letterIndex);
	//5. �ڵ������� �������̸�(command�� �ٰ� ���� ��ġ�� �׻� ��¥ �ٰ� ���� ��ġ�� ����Ǿ� ����)
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//5.1 ����� ȭ�� ũ�⿡ �´� �ٰ� ĳ���� ��ġ�� ���Ѵ�.
		rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
			&changedRowPos);
		//5.2 ���� ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
		currentRowPos = this->notepadForm->note->Move(changedRowPos);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->Move(changedLetterPos);
	}
	//6. ������ ���۵Ǵ� �ٰ� ���� ��ġ, ������ ������ �ٰ� ���� ��ġ�� �����Ѵ�.
	Long selectedStartRowPos = this->startPasteRowIndex;//������ ���۵Ǵ� ��
	Long selectedStartLetterPos = this->startPasteLetterIndex;//������ ���۵Ǵ� ����
	Long selectedEndRowPos = currentRowPos;//������ ������ ��
	Long selectedEndLetterPos = currentLetterPos;//������ ������ ����
	//7. content�� �����.
	this->notepadForm->note->RemoveSelectedTexts(selectedStartRowPos,
		selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
	//8. ������ ������ ������ ���� ���� ��ġ�� �ٽ� �������ش�.(note�ǿ���ȿ��� ���� ���� ��ġ�� ���� ��ġ��
	//������ ������ notepadForm�� current(������)�� ������ �� ��� notepadForm���� ���ش�.)
	currentRowPos = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	currentLetterPos = this->notepadForm->current->GetCurrent();
	//9. �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//9.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
		//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
		this->notepadForm->SendMessage(WM_SIZE);
	}
	//10. ���õ� ������ ��������
	if (this->isSelectedTextsRemoved == true)
	{
		//10.1 ���� ���� ��ġ�� �̵���Ų��.(ĳ���� �ٸ� ���� ������ �� ���� ���ڰ� �������� ������)
		currentRowPos = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->GetCurrent();
		//10.2 �޸��忡�� ���õ� texts�� �ٽ� �����ϱ� ������ �޸��忡�� ������ �� ���·� �ٲ۴�.
		this->notepadForm->isSelecting = true;
		//10.3 ������ �ٽ� �����߱� ������ ĳ���� x��ǥ�� command�� ����� ���� ���� ��ġ�� �����Ѵ�.
		this->notepadForm->selectedStartXPos = currentLetterPos;
		this->startPasteLetterIndex = this->notepadForm->selectedStartXPos;
		//10.4 ������ �ٽ� �����߱� ������ ĳ���� y��ǥ�� command�� ����� ���� ���� ��ġ�� �����Ѵ�.
		this->notepadForm->selectedStartYPos = currentRowPos;
		this->startPasteRowIndex = this->notepadForm->selectedStartYPos;
		//10.5 ���� ��ġ�� command�� ������ �ִ� content�� �����Ѵ�.
		Long rowIndex = this->notepadForm->note->
			InsertTexts(currentRowPos, currentLetterPos, this->note);
		//10.6 �޸����� ���� ���� �����Ѵ�.
		this->notepadForm->current = this->notepadForm->note->GetAt(rowIndex);
		//10.7 �ڵ������� �������̸� �ٿ����� �ٵ��� �ڵ���������ش�.
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//10.7.1 �κ��ڵ������� �Ѵ�.
			Long endPastedRowPos = rowAutoChange.DoPartRows(currentRowPos, rowIndex);
			//10.7.2 �ٿ��ֱⰡ ������ �ٷ� �̵���Ų��.
			//�ٿ��ֱⰡ ������ ���� OnSize���� �κ��ڵ������� �ؼ� ó���Ǳ� ������ ĳ���� ��ġ�� �������ָ� ��!
			currentRowPos = this->notepadForm->note->Move(endPastedRowPos);
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			currentLetterPos = this->notepadForm->current->GetCurrent();
			this->notepadForm->current->Move(currentLetterPos);
		}
		//10.8 ���ÿ����� �ٽ� ����� ������ �����ϱ�, �߶󳻱�, ���� �޴��� Ȱ��ȭ ��Ų��.
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_ENABLED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_ENABLED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_ENABLED);
	}
	//11. ���ڸ� �Է��� �Ŀ� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
	this->rowIndex = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
	this->letterIndex = this->notepadForm->current->GetCurrent();
	//12. �ڵ������� �������̸�(command�� �ٰ� ���� ��ġ�� �׻� ��¥ �ٰ� ���� ��ġ�� �����ؾ���)
	if (this->notepadForm->isRowAutoChanging == true)
	{
		Long changedRowPos = this->rowIndex;
		Long changedLetterPos = this->letterIndex;
		Long originRowPos = 0;
		Long originLetterPos = 0;
		//12.1 ����� ȭ�� ũ�⿡ �´� �ٰ� ĳ���� ��ġ�� ���Ѵ�.
		rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
			&originRowPos);
		//12.2 command�� ���ڸ� �Է��� �Ŀ� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
		this->rowIndex = originRowPos;
		this->letterIndex = originLetterPos;
	}
}

//�ٽý������� ���� ���ϱ�
bool PasteCommand::IsRedone()
{
	return this->isRedone;
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
//������ ���۵Ǵ� ������ ��ġ���ϱ�
Long PasteCommand::GetStartPasteRowIndex()
{
	return this->startPasteRowIndex;
}

//������ ���۵Ǵ� ���� ��ġ���ϱ�
Long PasteCommand::GetStartPasteLetterIndex()
{
	return this->startPasteLetterIndex;
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
//��������� �ִ��� Ȯ�� ����
bool PasteCommand::IsDirty()
{
	return this->isDirty;
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
	if (this->note != 0)
	{
		delete this->note;
	}
	if (this->glyph != 0)
	{
		delete this->glyph;
	}
}