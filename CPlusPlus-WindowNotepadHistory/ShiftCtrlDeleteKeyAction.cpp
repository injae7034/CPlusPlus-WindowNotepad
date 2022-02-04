#include "ShiftCtrlDeleteKeyAction.h"
#include "Glyph.h"

//����Ʈ������
ShiftCtrlDeleteKeyAction::ShiftCtrlDeleteKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void ShiftCtrlDeleteKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. ���� ���� ��ġ�� ���Ѵ�.
	Long currentRowPos = this->notepadForm->note->GetCurrent();
	//2. ���� ������ ��ġ�� ���Ѵ�.
	Long currentLetterPos = this->notepadForm->current->GetCurrent();
	//3. �޸��忡�� ���õ� texts�� ������
	if (this->notepadForm->isSelecting == false)
	{
		//3.1 ���� ��Ʈ�� ������ ���� ��ġ�� ���Ѵ�.
		Long lastRowPos = this->notepadForm->note->GetLength() - 1;
		//3.2 ���� �ٿ��� ������ ������ ��ġ�� ���Ѵ�.
		Long lastLetterPos = this->notepadForm->current->GetLength();
		//���� ������ ���̸� ���� ���� �� ����, ������ �ٿ��� ���� ��ġ�� �������� ������ �ƹ��͵� ���Ͼ
		// ���� ���� ��ġ�� �������� �ƴϰ�, ���� ���� ��ġ�� �������̸� ���� ���� ���� �ٷ� ���Խ�Ų��.
		//3.3 ���� ���� ��ġ�� ��Ʈ�� ������ �� ��ġ���� �۰�, ���� ���� ��ġ�� �������̸�
		Glyph* currentRow = 0;
		Glyph* nextRow = 0;
		if (currentRowPos < lastRowPos && currentLetterPos == lastLetterPos)
		{
			//3.3.1 ���� ���� ���Ѵ�.
			currentRow = this->notepadForm->note->GetAt(currentRowPos);
			//3.3.2 ���� ���� ���� ���� ���Ѵ�.
			nextRow = this->notepadForm->note->GetAt(currentRowPos + 1);
			//3.3.3 ���� ���� ���� �ٿ� ��ģ��.
			nextRow->Join(currentRow);
			//3.3.4 Note���� ���� ���� �ּҸ� �����.
			this->notepadForm->note->Remove(currentRowPos + 1);
			//3.3.5 ���� ���� ���� ��ġ�� ������ �������̱� ������ �������ش�.
			Long index = this->notepadForm->current->First();
			while (index < lastLetterPos)
			{
				this->notepadForm->current->Next();
				index++;
				//�̷����ϸ� index�� ���� overflow�� ���� �ʱ� ������ �ݺ����� ��� �� ���Եǰ�,
				//�׷� �ᱹ�� ���ѹݺ��� �ȴ�.!!
				//index = this->current->Next();
			}
			//3.3.6 �޸��� ���� *�� �߰��Ѵ�.
			string name = this->notepadForm->fileName;
			name.insert(0, "*");
			name += " - �޸���";
			this->notepadForm->SetWindowText(CString(name.c_str()));
			//3.3.7 �޸��忡 ��������� ������ �����Ѵ�.
			this->notepadForm->isDirty = true;
		}
		// ���� ���� ��ġ�� �������� �ƴ� ��(���� ���� �������̵� �ƴϵ� �������)
		//���� ������ ���� ���ں��� ���� ������ ���ڱ��� �� �����.
		//3.4 ���� ���� ��ġ�� �������� �ƴϸ�
		else if (currentLetterPos < lastLetterPos)
		{
			//3.4.1 ������ ������ġ�� ���������ġ���� ū���� �ݺ��Ѵ�.
			while (lastLetterPos > currentLetterPos)
			{
				//3.4.1.1 ���ڸ� �����.
				this->notepadForm->current->Remove(lastLetterPos - 1);
				lastLetterPos--;
			}
			//3.4.2 �޸��� ���� *�� �߰��Ѵ�.
			string name = this->notepadForm->fileName;
			name.insert(0, "*");
			name += " - �޸���";
			this->notepadForm->SetWindowText(CString(name.c_str()));
			//3.4.3 �޸��忡 ��������� ������ �����Ѵ�.
			this->notepadForm->isDirty = true;
			//3.4.4 �ڵ� �� �ٲ� �޴��� üũ�Ǿ����� Ȯ���Ѵ�.
			UINT state = this->notepadForm->GetMenu()->
				GetMenuState(IDM_ROW_AUTOCHANGE, MF_BYCOMMAND);
			//3.4.5 �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
			if (state == MF_CHECKED)
			{
				//3.4.5.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
				//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
				this->notepadForm->SendMessage(WM_SIZE);
			}
		}
	}
	//4. �޸��忡�� ���õ� texts�� ������
	else
	{
		//4.1 RemoveCommand�� �޼����� ������ ���ÿ����� �����.
		this->notepadForm->SendMessage(WM_COMMAND, IDM_NOTE_REMOVE);
	}
}


//�Ҹ���
ShiftCtrlDeleteKeyAction::~ShiftCtrlDeleteKeyAction()
{

}