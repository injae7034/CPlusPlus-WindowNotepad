#include "OnCharCommand.h"
#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "CommandHistory.h"

//����Ʈ������ ����
OnCharCommand::OnCharCommand(NotepadForm* notepadForm, UINT nChar,
	Long rowIndex, Long letterIndex, Long startSplitIndex)
	:Command(notepadForm)
{
	this->nChar = nChar;
	this->rowIndex = rowIndex;
	this->letterIndex = letterIndex;
	this->startSplitIndex = startSplitIndex;
	this->isUndoMacroEnd = false;
	this->isRedoMacroEnd = false;
}

//Execute ����
void OnCharCommand::Execute()
{
	Long currentRowPos = 0;
	//1. OnCharCommand�� ���ڰ� ���๮���̸�
	if (this->nChar == '\n' || this->nChar == '\r')
	{
		//1.1 OnCharCommand�� ���� ��ġ�� �� �� �������� �̵��Ѵ�.(������ �� �� ���� ��ġ��
		//�� �� �����Ǿ���, ��ҵǾ��� ������ �ٽ� �����ϱ� ���� �� �� �������� �̵����Ѿ���)
		this->rowIndex--;
		currentRowPos = this->notepadForm->note->Move(this->rowIndex);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		//1.2 ������ ��ҵǾ��� ������ �ٽ� ������ �Ǳ� ���� ĭ�� ���� ��ġ�� �̵���Ų��.
		this->letterIndex = this->notepadForm->current->Move(this->startSplitIndex);
	}
	//2. OnCharCommand�� ���ڰ� ���๮�ڰ� �ƴϸ�
	else
	{
		currentRowPos = this->notepadForm->note->Move(this->rowIndex);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		//2.1 ���ڰ� ������ �� ĭ �̵��� �Ǵµ�, ���ڰ� ��ҵǾ��� ������ �� ĭ �������� �̵���Ų��.
		this->letterIndex--;
		this->notepadForm->current->Move(this->letterIndex);
	}
	//3. OnChar�� �޼����� ������ ���� �ٽ� �������� ǥ���Ѵ�.
	this->notepadForm->isRedone = true;
	//4. OnChar�� �޼����� ������.
	this->notepadForm->SendMessage(WM_CHAR);
	//5. ���ڸ� �Է��� �Ŀ� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
	this->rowIndex = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
	this->letterIndex = this->notepadForm->current->GetCurrent();
}

//Unexcute
void OnCharCommand::Unexecute()
{
	//1. ���� ���� ��ġ�� �̵���Ų��.(ĳ���� �ٸ� ���� ������ �� ���� ���ڰ� �������� ������)
	Long currentRowPos = this->notepadForm->note->Move(this->rowIndex);
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	//2. ���� ������ ��ġ�� �̵���Ų��.
	Long currentLetterPos = this->notepadForm->current->Move(this->letterIndex);
	//3. �޸��忡�� ���õ� texts�� ������
	if (this->notepadForm->isSelecting == false)
	{
		//���� ù ���̸� ���� ���� �� ����, ���� ù �ٿ��� ���� ��ġ�� ���� ó���� ������
		//�ƹ��͵� ���Ͼ. ���� ���� ��ġ�� ���� ó���� �ƴϰ�, ���� ���� ��ġ�� ���� ó���̸�
		//���� ���� ���� �ٿ� Join��Ų��.
		//3.1 ���� ���� ��ġ�� 0���� ũ��, ���� ���� ��ġ�� 0�̸�
		Glyph* currentRow = 0;
		Glyph* previousRow = 0;
		if (currentRowPos > 0 && currentLetterPos == 0)
		{
			//3.1.1 ���� ���� ���Ѵ�.
			currentRow = this->notepadForm->note->GetAt(currentRowPos);
			//3.1.2 ���� ���� ���� ���� ���Ѵ�.
			previousRow = this->notepadForm->note->GetAt(currentRowPos - 1);
			//3.1.3 ���� ���� ���� ���� ������ ���� ��ġ�� ���Ѵ�.
			Long letterPos = previousRow->GetLength();
			//3.1.4 ���� ���� ���� �ٿ� ��ģ��.
			currentRow->Join(previousRow);
			//3.1.5 Note���� ���� ���� �ּҸ� �����.(������ ����� �ȵ�)
			this->notepadForm->note->Remove(currentRowPos);
			//3.1.6 ���� �������� ������ ���� ��ġ�� �ٽ� ���Ѵ�.
			currentRowPos = this->notepadForm->note->GetCurrent();
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			//3.1.7 ���� ���� ���� ��ġ�� ������ �������̱� ������ �������ش�.
			//���� ���� ������ ��ġ�� �̵���Ų��.
			Long index = this->notepadForm->current->Move(letterPos);
			//3.1.8 �޸��� ���� *�� �߰��Ѵ�.
			string name = this->notepadForm->fileName;
			name.insert(0, "*");
			name += " - �޸���";
			this->notepadForm->SetWindowText(CString(name.c_str()));
			//3.1.9 �޸��忡 ��������� ������ �����Ѵ�.
			this->notepadForm->isDirty = true;
		}
		// ���� ���� ��ġ�� ó���� �ƴ� ��(���� ���� ó���̵� �ƴϵ� �������) ���� ���ڸ� �����.
		//3.2 ���� ���� ��ġ�� ó���� �ƴϸ�
		else if (currentLetterPos > 0)
		{
			//3.2.1 ���� ���ڸ� �����.
			this->notepadForm->current->Remove(currentLetterPos - 1);
			//3.2.2 �޸��� ���� *�� �߰��Ѵ�.
			string name = this->notepadForm->fileName;
			name.insert(0, "*");
			name += " - �޸���";
			this->notepadForm->SetWindowText(CString(name.c_str()));
			//3.2.3 �޸��忡 ��������� ������ �����Ѵ�.
			this->notepadForm->isDirty = true;
			//3.2.4 �ڵ� �� �ٲ� �޴��� üũ�Ǿ����� Ȯ���Ѵ�.
			UINT state = this->notepadForm->GetMenu()->
				GetMenuState(IDM_ROW_AUTOCHANGE, MF_BYCOMMAND);
			//3.2.5 �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
			if (state == MF_CHECKED)
			{
				//3.2.5.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
				//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
				this->notepadForm->SendMessage(WM_SIZE);
			}
		}
	}
}

//SetMacroEnd(������� �� �ٽý��� ��ũ����� �������� ����)
void OnCharCommand::SetUndoMacroEnd()
{
	this->isUndoMacroEnd = true;
}
void OnCharCommand::SetRedoMacroEnd()
{
	this->isRedoMacroEnd = true;
}

//�Ҹ��� ����
OnCharCommand::~OnCharCommand()
{

}