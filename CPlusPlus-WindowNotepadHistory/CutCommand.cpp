#include "CutCommand.h"
#include "NotepadForm.h"
#include "Note.h"
#include "DummyRow.h"
#include "RowAutoChange.h"
#include "SelectingTexts.h"

//����Ʈ������
CutCommand::CutCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{
	this->rowIndex = notepadForm->note->GetCurrent();
	this->letterIndex = notepadForm->current->GetCurrent();
	this->glyph = 0;
	this->isUndoMacroEnd = false;
	this->isRedoMacroEnd = false;
	this->isRedone = false;
	this->selectedStartXPos = 0;
	this->selectedStartYPos = 0;
	this->isSelectedTextsRemoved = false;//ó���� ������ ���� ���ÿ����� �����������Ƿ� false�� ����Ʈ��
}

//Execute
void CutCommand::Execute()
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
	//3. RemoveCommand�� �ٽ� ����Ǹ�
	if (this->isRedone == true)
	{
		//3.1 ���� ���� ��ġ�� ������ġ�� ���������ش�.
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
	//4. �ٽ� �����̸�
	if (this->isRedone == true)
	{
		this->notepadForm->selectedStartYPos = this->selectedStartYPos;
		this->notepadForm->selectedStartXPos = this->selectedStartXPos;
	}
	//5. ������ ���۵Ǵ� �ٰ� ���� ��ġ, ������ ������ �ٰ� ���� ��ġ�� �����Ѵ�.
	Long selectedStartRowPos = this->notepadForm->selectedStartYPos;//������ ���۵Ǵ� ��
	Long selectedStartLetterPos = this->notepadForm->selectedStartXPos;//������ ���۵Ǵ� ����
	Long selectedEndRowPos = currentRowPos;//������ ������ ��
	Long selectedEndLetterPos = currentLetterPos;//������ ������ ����
	//6. ó�� �����̸�
	if (this->isRedone == false)
	{
		//6.1 content�� �����ϰ� �����.
		this->glyph = this->notepadForm->note->CopySelectedTextsAndRemove(selectedStartRowPos,
			selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
		//6.2 �޸��� ���� *�� �߰��Ѵ�.
		string name = this->notepadForm->fileName;
		name.insert(0, "*");
		name += " - �޸���";
		this->notepadForm->SetWindowText(CString(name.c_str()));
		//6.3 �޸��忡 ��������� ������ �����Ѵ�.
		this->notepadForm->isDirty = true;
		//6.4 notepadForm�� clipboard�� command�� ���� ������ Note(������ ����)�� �߰������ش�.
		Long noteIndex = this->notepadForm->clipboard->Add(this->glyph->Clone());
		//����Ŭ�����忡 ������ ������ �ܺ�Ŭ������� �ű��
		//6.5 notepadForm�� clipboard�� ���� ������ Note(������ ����)�� �� ���̸� ���Ѵ�.
		CString content = this->notepadForm->clipboard->
			GetAt(noteIndex)->GetContent().c_str();
		//������ ���ڿ��� ���̸� ���Ѵ�. ('\0'���� ������ ũ��)
		int contentLength = content.GetLength() + 1;
		//6.6 �ܺ� Ŭ�����尡 ��������
		//Ŭ������� ���� ���α׷����� ������ �� �ִ� ����̱� ������ � ���α׷�(������)�� Ŭ�����带
		//����ϴ��� �����ϰ� ����ϴ� ���� ����
		if (this->notepadForm->OpenClipboard() != 0)
		{
			//6.6.1 notepadForm�� clipboard�� ���� ������ Note(������ ����)�� Ŭ�����忡 �ű� ������ ����
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
	}
	//7. �ٽ� �����̸�
	else
	{
		//7.1 content�� �����.
		this->notepadForm->note->RemoveSelectedTexts(selectedStartRowPos,
			selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
	}
	//8. ������ ������ ������ ���� ���� ��ġ�� �ٽ� �������ش�.(note�ǿ���ȿ��� ���� ���� ��ġ�� ���� ��ġ��
	//������ ������ notepadForm�� current(������)�� ������ �� ��� notepadForm���� ���ش�.)
	this->notepadForm->current = this->notepadForm->note->
		GetAt(this->notepadForm->note->GetCurrent());
	//9. ������ texts�� ����� ������ �ٿ��ֱ� �޴��� Ȱ��ȭ�����ش�.
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_PASTE, MF_BYCOMMAND | MF_ENABLED);
	//10. �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//10.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
		//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
		this->notepadForm->SendMessage(WM_SIZE);
	}
	//11. �޸��忡�� ���õ� texts�� �� ������ ������ �޸��忡�� ������ �ȵ� ���·� �ٲ۴�.
	this->notepadForm->isSelecting = false;
	//12. ������ ������ ������ ĳ���� x��ǥ�� 0���� �����Ѵ�.
	this->notepadForm->selectedStartXPos = 0;
	//13. ������ ������ ������ ĳ���� y��ǥ�� 0���� �����Ѵ�.
	this->notepadForm->selectedStartYPos = 0;
	//14. �����ϱ�, �߶󳻱�, ���� �޴��� ��Ȱ��ȭ ��Ų��.
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	//15. ���ڸ� ���� �Ŀ� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
	this->rowIndex = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
	this->letterIndex = this->notepadForm->current->GetCurrent();
	//16. �ڵ������� �������̸�(command�� �ٰ� ���� ��ġ�� �׻� ��¥ �ٰ� ���� ��ġ�� �����ؾ���)
	if (this->notepadForm->isRowAutoChanging == true)
	{
		Long changedRowPos = this->rowIndex;
		Long changedLetterPos = this->letterIndex;
		Long originRowPos = 0;
		Long originLetterPos = 0;
		//16.1 ����� ȭ�� ũ�⿡ �´� �ٰ� ĳ���� ��ġ�� ���Ѵ�.
		rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
			&originRowPos);
		//16.2 command�� ���ڸ� �Է��� �Ŀ� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
		this->rowIndex = originRowPos;
		this->letterIndex = originLetterPos;
	}
}

//�������
void CutCommand::Unexecute()
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
		//4.1 ����� ȭ�� ũ�⿡ �´� �ٰ� ĳ���� ��ġ�� ���Ѵ�.
		rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
			&changedRowPos);
		//4.2 ���� ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
		currentRowPos = this->notepadForm->note->Move(changedRowPos);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->Move(changedLetterPos);
	}
	//6. �޸��忡�� ���õ� texts�� �ٽ� �����ϱ� ������ �޸��忡�� ������ �� ���·� �ٲ۴�.
	this->notepadForm->isSelecting = true;
	//7. ������ �ٽ� �����߱� ������ ĳ���� x��ǥ�� command�� ����� ���� ���� ��ġ�� �����Ѵ�.
	this->notepadForm->selectedStartXPos = currentLetterPos;
	this->selectedStartXPos = this->notepadForm->selectedStartXPos;
	//8. ������ �ٽ� �����߱� ������ ĳ���� y��ǥ�� command�� ����� ���� ���� ��ġ�� �����Ѵ�.
	this->notepadForm->selectedStartYPos = currentRowPos;
	this->selectedStartYPos = this->notepadForm->selectedStartYPos;
	//9. ���� ��ġ�� command�� ������ �ִ� content�� �����Ѵ�.
	Long rowIndex = this->notepadForm->note->
		InsertTexts(currentRowPos, currentLetterPos, this->glyph);
	//10. �޸����� ���� ���� �����Ѵ�.
	this->notepadForm->current = this->notepadForm->note->GetAt(rowIndex);
	//11. �ڵ������� �������̸� �ٿ����� �ٵ��� �ڵ���������ش�.
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//11.1 �ڵ�����Ŭ������ �����Ѵ�.
		RowAutoChange rowAutoChange(this->notepadForm);
		//11.2 �κ��ڵ������� �Ѵ�.
		Long endPastedRowPos = rowAutoChange.DoPartRows(currentRowPos, rowIndex);
		//11.3 �ٿ��ֱⰡ ������ �ٷ� �̵���Ų��.
		//�ٿ��ֱⰡ ������ ���� OnSize���� �κ��ڵ������� �ؼ� ó���Ǳ� ������ ĳ���� ��ġ�� �������ָ� ��!
		currentRowPos = this->notepadForm->note->Move(endPastedRowPos);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->GetCurrent();
		this->notepadForm->current->Move(currentLetterPos);
	}
	//12. ���ÿ����� �ٽ� ����� ������ �����ϱ�, �߶󳻱�, ���� �޴��� Ȱ��ȭ ��Ų��.
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_ENABLED);
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_ENABLED);
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_ENABLED);
	//13. ���ڰ� �ٽ� �߰��Ǿ��� ������ ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
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
	}
}

//�ٽý������� ���� ���ϱ�
bool CutCommand::IsRedone()
{
	return this->isRedone;
}

//���� ��ġ ���ϱ�
Long CutCommand::GetRowIndex()
{
	return this->rowIndex;
}
//���� ��ġ ���ϱ�
Long CutCommand::GetLetterIndex()
{
	return this->letterIndex;
}

//������ ���۵Ǵ� ������ ��ġ���ϱ�
Long CutCommand::GetSelectedStartXPos()
{
	return this->selectedStartXPos;
}

//������ ���۵Ǵ� ���� ��ġ���ϱ�
Long CutCommand::GetSelectedStartYPos()
{
	return this->selectedStartYPos;
}

//SetMacroEnd(������� �� �ٽý��� ��ũ����� �������� ����)
void CutCommand::SetUndoMacroEnd()
{
	this->isUndoMacroEnd = true;
}
void CutCommand::SetRedoMacroEnd()
{
	this->isRedoMacroEnd = true;
}

//������� ������������ ���ϱ�
bool CutCommand::IsUndoMacroEnd()
{
	return this->isUndoMacroEnd;
}
//�ٽý��� ������������ ���ϱ� 
bool CutCommand::IsRedoMacroEnd()
{
	return this->isRedoMacroEnd;
}

//SetRedone(�ٽ� �����̶�� ������)
void CutCommand::SetRedone()
{
	this->isRedone = true;
}

//���ÿ����� ���������� Ȯ�� ����
bool CutCommand::IsSelectedTextsRemoved()
{
	return this->isSelectedTextsRemoved;
}

//�Ҹ���
CutCommand::~CutCommand()
{
	if (this->glyph != 0)
	{
		delete this->glyph;
	}
}