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
	this->startYPos = notepadForm->note->GetCurrent();
	this->startXPos = notepadForm->current->GetCurrent();
	this->selectedEndYPos = 0;
	this->selectedEndXPos = 0;
	this->pastingEndYPos = 0;
	this->pastingEndXPos = 0;
	this->isUndoMacroEnd = false;
	this->isRedoMacroEnd = false;
	this->isRedone = false;
	this->isDirty = false;//ó���� ������ ���� ��������� �����Ƿ� false�� ����Ʈ����.
	this->isSelectedTextsRemoved = false;//ó���� ������ ���� ���ÿ����� �����������Ƿ� false�� ����Ʈ��
}

//Execute
void PasteCommand::Execute()
{
	//1. RowAutoChange�� �����Ѵ�.
	RowAutoChange rowAutoChange(this->notepadForm);

	Long changedRowPos = 0;//�ڵ����� �� �� ��ġ
	Long changedLetterPos = 0;//�ڵ����� �� ���� ��ġ
	Long originRowPos = 0;//�ڵ����� �� �� ��ġ
	Long originLetterPos = 0;//�ڵ����� �� ���� ��ġ
	Long currentRowPos = 0;//���� ���� ��ġ
	Long currentLetterPos = 0;//���� ������ ��ġ
	Long selectedStartRowPos = 0;//������ ���۵Ǵ� �� ��ġ
	Long selectedStartLetterPos = 0;//������ ���۵Ǵ� ���� ��ġ
	Long selectedEndRowPos = 0;//������ ������ �� ��ġ
	Long selectedEndLetterPos = 0;//������ ������ ���� ��ġ

	//2. ó�� �����̸�
	if (this->isRedone == false)
	{
		//�ܺ� Ŭ�����忡 ���ڿ��� ����Ǿ� ������ �ܺ�Ŭ�����忡�� ���ڿ��� �޾ƿͼ� contents�� �����Ѵ�.
		//contents�� ���ٷ� ���ڿ��� �����ϴµ� ���� �����ϴ� ���� '\n(���๮��)'�����̴�. �� ���๮�ڸ�
		//���ؼ� �ٴ����� ������ �����ؾ��Ѵ�.
		//2.1 �ܺ� Ŭ�����忡 ���ڿ��� ����Ǿ� ������
		//Ŭ������� ���� ���α׷����� ������ �� �ִ� ����̱� ������ � ���α׷�(������)�� Ŭ�����带
		//����ϴ��� �����ϰ� ����ϴ� ���� ����
		Glyph* newRow = 0;
		unsigned int priority_list = CF_TEXT;
		char* content = NULL;
		if (GetPriorityClipboardFormat(&priority_list, 1) == CF_TEXT)
		{
			//2.1.1 Ŭ�����忡 �ִ� ���ڿ� ������ ��� ���ؼ� Ŭ�����带 ����.
			if (this->notepadForm->OpenClipboard())
			{
				//2.1.1.1 Ŭ�����忡�� ���ڿ��� ����� �޸��� �ڵ� ���� ��´�.
				HANDLE h_clipboard_data = GetClipboardData(CF_TEXT);
				if (h_clipboard_data != NULL)
				{
					//2.1.1.1.1 �޸� �ڵ� ���� �̿��Ͽ� ���� ��밡���� �ּҸ� ��´�.
					char* p_clipboard_data = (char*)::GlobalLock(h_clipboard_data);
					//2.1.1.1.2 Ŭ�����忡 ����� ���ڿ��� ���̸� ���Ѵ�. ('\0'���� ũ��)
					int contentLength = strlen(p_clipboard_data) + 1;
					//2.1.1.1.3 Ŭ�����忡 ����� ���ڿ��� �����ϱ� ���� �޸𸮸� �Ҵ��Ѵ�.
					content = new char[contentLength];
					//2.1.1.1.4 �Ҵ�� �޸𸮿� Ŭ������ ���ڿ��� �����Ѵ�.
					memcpy(content, p_clipboard_data, contentLength);
					//2.1.1.1.5 ���ڿ��� �����ϱ� ���ؼ� Lock�ߴ� �޸𸮸� �����Ѵ�.
					GlobalUnlock(h_clipboard_data);
				}
				//2.1.1.2 Ŭ�����带 �ݴ´�.
				CloseClipboard();
			}
			//2.1.2 ������ �ؽ�Ʈ�� ���� ����Ʈ�� �����Ѵ�.
			this->glyph = new Note();
			//2.1.3 ���� �����Ѵ�. 
			newRow = new Row();
			//2.1.4 ���� ������ ���� ���� ������ ��Ʈ�� �߰��Ѵ�.
			Long rowIndex = this->glyph->Add(newRow);
			//2.1.5 ������ ������ ���������� �ݺ��Ѵ�.(�������� �ι��ڰ� ����Ǿ� ����)
			GlyphCreator glyphCreator;
			Glyph* glyph = 0;
			Long letterIndex = 0;
			char doubleByteLetter[3];
			char singleByteLetter[2];
			Long i = 0;//�迭��� �ʱ�ȭ
			//2.1.6 ������ ������(NULL����)�� �ƴѵ��� �ݺ��Ѵ�
			while (content[i] != '\0')
			{
				//2.1.6.1 ������ ������ �� �迭��Ұ� 2byte �������� 1byte �������� �Ǵ��Ѵ�.
				if ((content[i] & 0x80))//�ѱ��̸�(2byte����)
				{
					//2.1.6.1.1 doubleByteLetter�� �����Ѵ�.
					doubleByteLetter[0] = content[i];
					i++;
					doubleByteLetter[1] = content[i];
					doubleByteLetter[2] = '\0';
					//2.1.6.1.2 doubleByteLetter�� �����Ѵ�.(���丮�޼ҵ�����)
					glyph = glyphCreator.Create((char*)doubleByteLetter);
					//2.1.6.1.3 �ٿ� doubleByteLetter�� �߰��Ѵ�.
					letterIndex = this->glyph->GetAt(rowIndex)->Add(glyph);
				}
				//2.1.6.2 ���๮���̸�(���� �ٲٸ� ���� �߰��Ѵ�)
				else if (content[i] == '\n' || content[i] == '\r')
				{
					//2.1.6.2.1 row�� �����Ѵ�.
					glyph = glyphCreator.Create(content + i);
					//2.1.6.2.2 ���ο� ���� �߰��Ѵ�.
					rowIndex = this->glyph->Add(glyph);
					//2.1.6.2.3 ���� ���ڰ� '\n'�̸�
					if (content[i + 1] == '\n')
					{
						i++;
					}
				}
				//2.1.6.3 �Է¹��� ���ڰ� �ѱ��� �ƴϰ� ���๮�ڰ� �ƴϸ�(1byte����)
				else
				{
					//2.1.6.3.1 singleByteLetter�� �����Ѵ�.
					singleByteLetter[0] = content[i];
					singleByteLetter[1] = '\0';
					//2.1.6.3.2 singleByteLetter�� �����Ѵ�.
					glyph = glyphCreator.Create(singleByteLetter);
					//2.1.6.3.3 �ٿ� singleByteLetter�� �߰��Ѵ�.
					letterIndex = this->glyph->GetAt(rowIndex)->Add(glyph);
				}
				i++;
			}
			//2.1.7 �տ��� content�� �Ҵ��߱� ������ ���⼭ �Ҵ������� ������Ѵ�.
			if (content != 0)
			{
				delete[] content;
			}
			//2.1.8 notepadForm�� clipboard�� ���� ������ Note(������ ����)�� �߰������ش�.
			Long noteIndex = this->notepadForm->clipboard->Add(this->glyph->Clone());
		}
	}
	//3. ������ content�� ������
	if (this->glyph != 0)
	{
		//���ÿ����� ������ ���ÿ��� ����
		//3.1 �޸��忡�� ���õ� texts�� �ְų� �Ǵ� ���õ� ������ ��������
		if (this->notepadForm->isSelecting == true || this->isSelectedTextsRemoved == true)
		{
			//3.1.1 OnReplaceButtonClickedCommand�� ó�� ����Ǹ�
			if (this->isRedone == false)
			{
				//3.1.1.1 ���� ���� ��ġ�� ���� ��ġ�� ���Ѵ�.
				currentRowPos = this->notepadForm->note->GetCurrent();
				currentLetterPos = this->notepadForm->current->GetCurrent();
				//3.1.1.2 ������ ���۵Ǵ� ���� ��ġ�� ���� ��ġ�� ���Ѵ�.
				selectedStartRowPos = this->notepadForm->selectedStartYPos;
				this->startYPos = selectedStartRowPos;
				selectedStartLetterPos = this->notepadForm->selectedStartXPos;
				this->startXPos = selectedStartLetterPos;
				//3.1.1.3 ������ ������ ���� ��ġ�� ���� ��ġ�� ���Ѵ�.
				selectedEndRowPos = currentRowPos;
				this->selectedEndYPos = selectedEndRowPos;
				selectedEndLetterPos = currentLetterPos;
				this->selectedEndXPos = selectedEndLetterPos;
				//3.1.1.4 ���õ� �ܾ�(ã�� �ܾ�)�� command�� �����ؼ� �����ϰ� �����.
				this->note = this->notepadForm->note->CopySelectedTextsAndRemove(selectedStartRowPos,
					selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
				//3.1.1.8 ������ ������ ������ ���� ���� ��ġ�� �ٽ� �������ش�.(note�� ���� �ȿ��� 
				//���� ���� ��ġ�� ���� ��ġ�� ������ ������ notepadForm�� current(������)�� 
				//������ �� ��� notepadForm���� ���ش�.)
				currentRowPos = this->notepadForm->note->GetCurrent();
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
				currentLetterPos = this->notepadForm->current->GetCurrent();
				//3.1.1.9 ���õ� �ܾ ������ ������ �ٽ� �����ϴ� ���� ��ġ�� ������ġ�� �����Ѵ�.
				this->startYPos = currentRowPos;
				this->startXPos = currentLetterPos;
				//3.1.1.10 �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������(���ÿ����� ���� ��� ������ �ڵ������� �ٽ��������)
				if (this->notepadForm->isRowAutoChanging == true)
				{
					//3.1.1.10.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
					//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
					this->notepadForm->SendMessage(WM_SIZE);
					//3.1.1.10.2 ���ÿ����� �����Ǿ��� ������ �ڵ������� ������ ���� ���� ��ġ�� ���� ��ġ��
					//�ٲ� ���ֱ� ������ �����ϴ� ���� ��ġ�� ���� ���� ��ġ�� ������ġ�� �簻��������Ѵ�.
					this->startYPos = this->notepadForm->note->GetCurrent();
					this->startXPos = this->notepadForm->current->GetCurrent();
					//3.1.1.10.3 ���� ���� startYPos�� startXPos�� ��ġ�� �ڵ������� ����� ������ ��ġ�̱�
					//������ �ڵ������� ���� ���¿��� �����ϱⰡ ���۵Ǵ� ��¥ ���� ��ġ�� ������ġ�� ���Ѵ�.
					changedRowPos = this->startYPos;
					changedLetterPos = this->startXPos;
					originRowPos = 0;
					originLetterPos = 0;
					rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
						&originRowPos);
					this->startYPos = originRowPos;
					this->startXPos = originLetterPos;
				}
			}
			//3.1.2 OnReplaceButtonClickedCommand�� �ٽ� ����Ǹ�
			else
			{
				//3.1.2.1 �ڵ������� �������̸�(command�� �ٰ� ���� ��ġ�� �׻� ��¥ �ٰ� ���� ��ġ��
				//����Ǿ� ����)
				if (this->notepadForm->isRowAutoChanging == true)
				{
					//3.1.2.1.1 ����� ȭ�� ũ�⿡ �´� ���� �ٰ� ���� ��ġ�� ���Ѵ�.
					changedRowPos = 0;
					changedLetterPos = 0;
					originRowPos = this->selectedEndYPos;
					originLetterPos = this->selectedEndXPos;
					rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
						&changedRowPos);
					//3.1.2.1.2 �ڵ������� �ݿ��ؼ� ���� ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
					currentRowPos = this->notepadForm->note->Move(changedRowPos);
					this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
					currentLetterPos = this->notepadForm->current->Move(changedLetterPos);
					//3.1.2.1.3 �ڵ������� �ݿ��ؼ� ������ ������ ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
					selectedEndRowPos = currentRowPos;
					selectedEndLetterPos = currentLetterPos;
					//3.1.2.1.4 ����� ȭ�� ũ�⿡ �´� ������ ���۵Ǵ� �ٰ� ���� ��ġ�� ���Ѵ�.
					changedRowPos = 0;
					changedLetterPos = 0;
					originRowPos = this->startYPos;
					originLetterPos = this->startXPos;
					rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
						&changedRowPos);
					//3.1.2.1.5 �ڵ������� �ݿ��ؼ� ������ ���۵Ǵ� �ٰ� ���� ��ġ�� �ٽ� �����Ѵ�.
					selectedStartRowPos = changedRowPos;//������ ���۵Ǵ� ��
					selectedStartLetterPos = changedLetterPos;//������ ���۵Ǵ� ����
				}
				//3.1.2.2 �ڵ����� ���� ���� �ƴϸ�
				else
				{
					//3.1.2.2.1 ���� ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
					currentRowPos = this->notepadForm->note->Move(this->selectedEndYPos);
					this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
					currentLetterPos = this->notepadForm->current->Move(this->selectedEndXPos);
					//3.1.2.2.2 ������ ���۵Ǵ� ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
					selectedStartRowPos = this->startYPos;
					selectedStartLetterPos = this->startXPos;
					//3.1.2.2.3 ������ ������ ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
					selectedEndRowPos = currentRowPos;
					selectedEndLetterPos = currentLetterPos;
				}
				//3.1.2.3 ���õ� �ܾ�(ã�� �ܾ�)�� �����.
				this->notepadForm->note->RemoveSelectedTexts(selectedStartRowPos,
					selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
				//3.1.2.4 ������ ������ ������ ���� ���� ��ġ�� �ٽ� �������ش�.(note�� ���� �ȿ��� ���� ���� ��ġ�� ���� 
				//��ġ�� ������ ������ notepadForm�� current(������)�� ������ �� ��� notepadForm���� ���ش�.)
				currentRowPos = this->notepadForm->note->GetCurrent();
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
				currentLetterPos = this->notepadForm->current->GetCurrent();
				//3.1.2.5 ���ÿ����� �����Ǿ��� ������ �ڵ������� ������ ���� ���� ��ġ�� ���� ��ġ�� �ٲ� ��
				//�ֱ� ������ �����ϴ� ���� ��ġ�� ���� ���� ��ġ�� ������ġ�� �簻��������Ѵ�.
				this->startYPos = this->notepadForm->note->GetCurrent();
				this->startXPos = this->notepadForm->current->GetCurrent();
				//3.1.2.6 �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������(���ÿ����� ���� ��� ������ �ڵ������� �ٽ��������)
				if (this->notepadForm->isRowAutoChanging == true)
				{
					//3.1.2.6.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
					//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
					this->notepadForm->SendMessage(WM_SIZE);
					//3.1.2.6.2 ���ÿ����� �����Ǿ��� ������ �ڵ������� ������ ���� ���� ��ġ�� ���� ��ġ��
					//�ٲ� �� �ֱ� ������ �����ϴ� ���� ��ġ�� ���� ���� ��ġ�� ������ġ�� �簻��������Ѵ�.
					this->startYPos = this->notepadForm->note->GetCurrent();
					this->startXPos = this->notepadForm->current->GetCurrent();
					//3.1.2.6.3 ���� ���� startYPos�� startXPos�� ��ġ�� �ڵ������� ����� ������ ��ġ�̱�
					//������ �ڵ������� ���� ���¿��� �����ϱⰡ ���۵Ǵ� ��¥ ���� ��ġ�� ������ġ�� ���Ѵ�.
					changedRowPos = this->startYPos;
					changedLetterPos = this->startXPos;
					originRowPos = 0;
					originLetterPos = 0;
					rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
						&originRowPos);
					this->startYPos = originRowPos;
					this->startXPos = originLetterPos;
				}
			}
			//3.1.3 �޸��忡�� ���õ� �ܾ ������ ������ �޸��忡�� ������ �ȵ� ���·� �ٲ۴�.
			this->notepadForm->isSelecting = false;
			//3.1.4 ������ ������ ������ ������ ���۵Ǵ� x��ǥ�� 0���� �����Ѵ�.
			this->notepadForm->selectedStartXPos = 0;
			//3.1.5 ������ ������ ������ ������ ���۵Ǵ� y��ǥ�� 0���� �����Ѵ�.
			this->notepadForm->selectedStartYPos = 0;
			//3.1.6 ���õ� ������ �������� ������ �����ϱ�, �߶󳻱�, ���� �޴��� ��Ȱ��ȭ ��Ų��.
			this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			//3.1.7 command�� ���ÿ����� �����ٰ� ǥ���Ѵ�.
			this->isSelectedTextsRemoved = true;
		}
		//3.2 ���ÿ����� ���ų� ���ÿ����� ����������
		else
		{
			//3.2.1 ó�� �����̸�
			if (this->isRedone == false)
			{
				//3.2.1.1 ���� ���� ��ġ�� ���� ��ġ�� ���Ѵ�.
				currentRowPos = this->notepadForm->note->GetCurrent();
				currentLetterPos = this->notepadForm->current->GetCurrent();
				//3.2.1.2 ���� ���� ��ġ�� ������ġ�� �����ϴ� �ٰ� ���� ��ġ�� �����Ѵ�.
				this->startYPos = currentRowPos;
				this->startXPos = currentLetterPos;
				//3.2.1.3 �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������(��¥ ���� ��ġ�� ������ġ�� �����Ѵ�.)
				if (this->notepadForm->isRowAutoChanging == true)
				{
					//3.2.1.3.1 ���� ���� startYPos�� startXPos�� ��ġ�� �ڵ������� ����� ������ ��ġ�̱�
					//������ �ڵ������� ���� ���¿��� �����ϱⰡ ���۵Ǵ� ��¥ ���� ��ġ�� ������ġ�� ���Ѵ�.
					changedRowPos = this->startYPos;
					changedLetterPos = this->startXPos;
					originRowPos = 0;
					originLetterPos = 0;
					rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
						&originRowPos);
					this->startYPos = originRowPos;
					this->startXPos = originLetterPos;
				}
			}
			//3.2.2 �ٽ� �����̸�
			else
			{
				//3.2.2.1 �ڵ������� �������̸�(command�� �����ϴ� �ٰ� ���� ��ġ�� 
				//�׻� ��¥ �ٰ� ���� ��ġ�� ����Ǿ� �ֱ� ������ �ڵ����࿡ �°� �ٲ������)
				if (this->notepadForm->isRowAutoChanging == true)
				{
					//3.2.2.1.1 �ڵ������� �ݿ��Ͽ� ���۵Ǵ� �ٰ� ���� ��ġ�� ���Ѵ�.
					changedRowPos = 0;
					changedLetterPos = 0;
					originRowPos = this->startYPos;
					originLetterPos = this->startXPos;
					rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
						&changedRowPos);
					//3.2.2.1.2 �ڵ������� �ݿ��ؼ� ���� ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
					currentRowPos = this->notepadForm->note->Move(changedRowPos);
					this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
					currentLetterPos = this->notepadForm->current->Move(changedLetterPos);
				}
				//3.2.2.2 �ڵ����� �������� �ƴϸ�
				else
				{
					//3.2.2.2.1 ���� ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
					currentRowPos = this->notepadForm->note->Move(this->startYPos);
					this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
					currentLetterPos = this->notepadForm->current->Move(this->startXPos);
				}
			}
		}
		//3.3 ���� ������ �����ϱ� ���� ���� ��ġ�� �����Ѵ�.
		Long previousRowIndex = currentRowPos;
		//3.4 ���� ������ �����ϰ� ���� ���� ��ġ�� ��ȯ�޴´�.
		currentRowPos = this->notepadForm->note->
			InsertTexts(currentRowPos, currentLetterPos, this->glyph);
		//3.5 �޸����� ���� ���� �����Ѵ�.
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->GetCurrent();
		//3.6 �޸��忡 �ٿ��־��� ������ �ٿ��ֱⰡ ������ �ٰ� ������ġ�� �����Ѵ�.
		this->pastingEndYPos = currentRowPos;
		this->pastingEndXPos = currentLetterPos;
		//3.7 �ڵ������� �������̸� �ٿ����� �ٵ��� �ڵ���������ش�.
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//3.7.1 �κ��ڵ������� �Ѵ�.(������ ���� �����ϰ� �ڵ�������)
			Long endPastedRowPos = rowAutoChange.DoPartRows(previousRowIndex, currentRowPos);
			//3.7.2 �ٿ��ֱⰡ ������ �ٷ� �̵���Ų��.
			//�ٿ��ֱⰡ ������ ���� OnSize���� �κ��ڵ������� �ؼ� ó���Ǳ� ������ ĳ���� ��ġ�� �������ָ� ��!
			currentRowPos = this->notepadForm->note->Move(endPastedRowPos);
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			currentLetterPos = this->notepadForm->current->GetCurrent();
			this->notepadForm->current->Move(currentLetterPos);
			//3.7.3 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
			//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. (������ ���� �ڵ������Ŵ)
			this->notepadForm->SendMessage(WM_SIZE);
			//3.7.4 �޸����� ���� ���� �����Ѵ�.
			currentRowPos = this->notepadForm->note->GetCurrent();
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			currentLetterPos = this->notepadForm->current->GetCurrent();
			//3.7.5 �޸��忡 ���ÿ����� �����Ǿ���, �ڵ������� �ݿ��� ������ ������ �ٰ� ������ġ�� �����Ѵ�. 
			this->pastingEndYPos = currentRowPos;
			this->pastingEndXPos = currentLetterPos;
			//3.7.6 ���� ���� ������ ������ �ٰ� ���� ��ġ�� �ڵ������� ����� ������ ��ġ�̱� ������
			//�ڵ������� ���� ���¿��� �����ϱⰡ ������ ��¥ ���� ��ġ�� ������ġ�� ���Ѵ�.
			changedRowPos = this->pastingEndYPos;
			changedLetterPos = this->pastingEndXPos;
			originRowPos = 0;
			originLetterPos = 0;
			rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
				&originRowPos);
			this->pastingEndYPos = originRowPos;
			this->pastingEndXPos = originLetterPos;
		}
		//3.8 ó�� �����̸�
		if (this->isRedone == false)
		{
			//3.8.1 �޸��� ���� *�� �߰��Ѵ�.
			string name = this->notepadForm->fileName;
			name.insert(0, "*");
			name += " - �޸���";
			this->notepadForm->SetWindowText(CString(name.c_str()));
			//3.8.2 �޸��忡 ��������� ������ �����Ѵ�.
			this->notepadForm->isDirty = true;
			//3.8.3 command�� ��������� ������ ǥ���Ѵ�.
			this->isDirty = true;
		}
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

	Long changedRowPos = 0;//�ڵ����� �� �� ��ġ
	Long changedLetterPos = 0;//�ڵ����� �� ���� ��ġ
	Long originRowPos = 0;//�ڵ����� �� �� ��ġ
	Long originLetterPos = 0;//�ڵ����� �� ���� ��ġ
	Long currentRowPos = 0;//���� ���� ��ġ
	Long currentLetterPos = 0;//���� ������ ��ġ
	Long selectedStartRowPos = 0;//������ ���۵Ǵ� �� ��ġ
	Long selectedStartLetterPos = 0;//������ ���۵Ǵ� ���� ��ġ
	Long selectedEndRowPos = 0;//������ ������ �� ��ġ
	Long selectedEndLetterPos = 0;//������ ������ ���� ��ġ

	//3. �ڵ������� �������̸�(command�� �ٰ� ���� ��ġ�� �׻� ��¥ �ٰ� ���� ��ġ�� ����Ǿ� ����)
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//3.1 �ڵ������� �ݿ��ؼ� �ٿ��ֱⰡ ������(����) �ٰ� ���� ��ġ�� ���Ѵ�.
		changedRowPos = 0;
		changedLetterPos = 0;
		originRowPos = this->pastingEndYPos;
		originLetterPos = this->pastingEndXPos;
		rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
			&changedRowPos);
		//3.2 �ڵ������� �ݿ��ؼ� ���� ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
		currentRowPos = this->notepadForm->note->Move(changedRowPos);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->Move(changedLetterPos);
		//3.3 �ڵ������� �ݿ��ؼ� �ٿ��ֱⰡ ������ ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
		selectedEndRowPos = currentRowPos;
		selectedEndLetterPos = currentLetterPos;
		//3.4 �ڵ������� �ݿ��ؼ� �ٿ��ֱⰡ ���۵Ǵ� �ٰ� ���� ��ġ�� ���Ѵ�.
		changedRowPos = 0;
		changedLetterPos = 0;
		originRowPos = this->startYPos;
		originLetterPos = this->startXPos;
		rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
			&changedRowPos);
		//3.5 �ڵ������� �ݿ��ؼ� �ٿ��ֱⰡ ���۵Ǵ� �ٰ� ���� ��ġ�� �ٽ� �����Ѵ�.
		selectedStartRowPos = changedRowPos;//������ ���۵Ǵ� ��
		selectedStartLetterPos = changedLetterPos;//������ ���۵Ǵ� ����
	}
	//4. �ڵ����� ���� ���� �ƴϸ�
	else
	{
		//4.1 ���� ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
		currentRowPos = this->notepadForm->note->Move(this->pastingEndYPos);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->Move(this->pastingEndXPos);
		//4.2 �ٿ��ֱⰡ ���۵Ǵ� ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
		selectedStartRowPos = this->startYPos;
		selectedStartLetterPos = this->startXPos;
		//4.3 �ٿ��ֱⰡ ������ ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
		selectedEndRowPos = currentRowPos;
		selectedEndLetterPos = currentLetterPos;
	}
	//5. Execute���� �ٿ����� �ٲ� �ܾ �����.
	this->notepadForm->note->RemoveSelectedTexts(selectedStartRowPos,
		selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
	//6. ������ ������ ������ ���� ���� ��ġ�� �ٽ� �������ش�.(note�ǿ���ȿ��� ���� ���� ��ġ�� ���� ��ġ��
	//������ ������ notepadForm�� current(������)�� ������ �� ��� notepadForm���� ���ش�.)
	currentRowPos = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	currentLetterPos = this->notepadForm->current->GetCurrent();
	//7. ���ÿ����� �����Ǿ��� ������ �ڵ������� ������ ���� ���� ��ġ�� ���� ��ġ�� �ٲ� ��
	//�ֱ� ������ �����ϴ� ���� ��ġ�� ���� ���� ��ġ�� ������ġ�� �簻��������Ѵ�.
	this->startYPos = this->notepadForm->note->GetCurrent();
	this->startXPos = this->notepadForm->current->GetCurrent();
	//8. �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//8.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
		//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
		this->notepadForm->SendMessage(WM_SIZE);
		//8.2 ���ÿ����� �����Ǿ��� ������ �ڵ������� ������ ���� ���� ��ġ�� ���� ��ġ�� �ٲ� ��
		//�ֱ� ������ �����ϴ� ���� ��ġ�� ���� ���� ��ġ�� ������ġ�� �簻��������Ѵ�.
		this->startYPos = this->notepadForm->note->GetCurrent();
		this->startXPos = this->notepadForm->current->GetCurrent();
		//8.3 ���� ���� startYPos�� startXPos�� ��ġ�� �ڵ������� ����� ������ ��ġ�̱� ������
		//�ڵ������� ���� ���¿��� �����ϱⰡ ���۵Ǵ� ��¥ ���� ��ġ�� ������ġ�� ���Ѵ�.
		changedRowPos = this->startYPos;
		changedLetterPos = this->startXPos;
		originRowPos = 0;
		originLetterPos = 0;
		rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
			&originRowPos);
		this->startYPos = originRowPos;
		this->startXPos = originLetterPos;
	}
	//9. ���õ� ������ ��������
	if (this->isSelectedTextsRemoved == true)
	{
		//9.1 �ٲ۴ܾ ������ ���� ���ÿ����� �ٽ� �����ϱ� ������ ���� �ٰ� ������ ��ġ��
		//���ÿ����� ���� �ٰ� ���� ��ġ�� �����Ѵ�.
		currentRowPos = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->GetCurrent();
		this->notepadForm->selectedStartYPos = currentRowPos;
		this->notepadForm->selectedStartXPos = currentLetterPos;
		//9.2 �޸��忡�� ���õ� texts�� �ٽ� �����ϱ� ������ �޸��忡�� ������ �� ���·� �ٲ۴�.
		this->notepadForm->isSelecting = true;
		//9.3 ���ÿ����� �ٽ� ����� ������ �����ϱ�, �߶󳻱�, ���� �޴��� Ȱ��ȭ ��Ų��.
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_ENABLED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_ENABLED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_ENABLED);
		//9.4 ���ÿ����� �ٽ� �����ϱ� ���� ���� ��ġ�� �����Ѵ�.
		Long previousRowIndex = currentRowPos;
		//9.5 ���ÿ����� �ٽ� �����ϰ� ���� ���� ��ġ�� ��ȯ�޴´�.
		currentRowPos = this->notepadForm->note->
			InsertTexts(currentRowPos, currentLetterPos, this->note);
		//9.6 �޸����� ���� ���� �����Ѵ�.
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->GetCurrent();
		//9.7 �޸��忡 ���ÿ����� �����Ǿ��� ������ ������ ������ �ٰ� ������ġ�� �����Ѵ�.
		this->selectedEndYPos = currentRowPos;
		this->selectedEndXPos = currentLetterPos;
		//9.8 �ڵ������� �������̸� �ٿ����� �ٵ��� �ڵ���������ش�.
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//9.8.1 �κ��ڵ������� �Ѵ�.(������ ���� �����ϰ� �ڵ�������)
			Long endPastedRowPos = rowAutoChange.DoPartRows(previousRowIndex, currentRowPos);
			//9.8.2 ���ÿ����� ������ �ٷ� �̵���Ų��.
			//���ÿ����� ������ ���� OnSize���� �κ��ڵ������� �ؼ� ó���Ǳ� ������ ĳ���� ��ġ�� �������ָ� ��!
			currentRowPos = this->notepadForm->note->Move(endPastedRowPos);
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			currentLetterPos = this->notepadForm->current->GetCurrent();
			this->notepadForm->current->Move(currentLetterPos);
			//9.8.3 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
			//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. (������ ���� �ڵ������Ŵ)
			this->notepadForm->SendMessage(WM_SIZE);
			//9.8.4 �޸����� ���� ���� �����Ѵ�.
			currentRowPos = this->notepadForm->note->GetCurrent();
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			currentLetterPos = this->notepadForm->current->GetCurrent();
			//9.8.5 �޸��忡 ���ÿ����� �����Ǿ���, �ڵ������� �ݿ��� ������ ������ �ٰ� ������ġ�� �����Ѵ�. 
			this->selectedEndYPos = currentRowPos;
			this->selectedEndXPos = currentLetterPos;
			//9.8.6 ���� ���� ������ ������ �ٰ� ���� ��ġ�� �ڵ������� ����� ������ ��ġ�̱� ������
			//�ڵ������� ���� ���¿��� �����ϱⰡ ������ ��¥ ���� ��ġ�� ������ġ�� ���Ѵ�.
			changedRowPos = this->selectedEndYPos;
			changedLetterPos = this->selectedEndXPos;
			originRowPos = 0;
			originLetterPos = 0;
			rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
				&originRowPos);
			this->selectedEndYPos = originRowPos;
			this->selectedEndXPos = originLetterPos;
		}
	}
}

//�ٽý������� ���� ���ϱ�
bool PasteCommand::IsRedone()
{
	return this->isRedone;
}

//�����ϴ� ���� ��ġ���ϱ�
Long PasteCommand::GetStartYPos()
{
	return this->startYPos;
}
//�����ϴ� ������ ��ġ���ϱ�
Long PasteCommand::GetStartXPos()
{
	return this->startXPos;
}
//������ ������ ���� ��ġ ���ϱ�
Long PasteCommand::GetSelectedEndYPos()
{
	return this->selectedEndYPos;
}
//������ ������ ���� ��ġ ���ϱ�
Long PasteCommand::GetSelectedEndXPos()
{
	return this->selectedEndXPos;
}
//�ٿ��ֱⰡ ������ ���� ��ġ���ϱ�
Long PasteCommand::GetPastingEndYPos()
{
	return this->pastingEndYPos;
}
//�ٿ��ֱⰡ ������ ������ ��ġ���ϱ�
Long PasteCommand::GetPastingEndXPos()
{
	return this->pastingEndXPos;
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