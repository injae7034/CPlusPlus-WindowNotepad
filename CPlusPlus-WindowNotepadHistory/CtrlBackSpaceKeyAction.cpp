#include "CtrlBackSpaceKeyAction.h"
#include "Glyph.h"

//����Ʈ������
CtrlBackSpaceKeyAction::CtrlBackSpaceKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void CtrlBackSpaceKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. ���� ���� ��ġ�� ���Ѵ�.
	Long currentRowPos = this->notepadForm->note->GetCurrent();
	//2. ���� ������ ��ġ�� ���Ѵ�.
	Long currentLetterPos = this->notepadForm->current->GetCurrent();
	//���� ù ���̸� ���� ���� �� ����, ���� ù �ٿ��� ���� ��ġ�� ���� ó���� ������ �ƹ��͵� ���Ͼ
	// ���� ���� ��ġ�� ���� ó���� �ƴϰ�, ���� ���� ��ġ�� ���� ó���̸� ���� ���� ���� �ٷ� ���Խ�Ų��.
	//3. ���� ���� ��ġ�� 0���� ũ��, ���� ���� ��ġ�� 0�̸�
	Glyph* currentRow = 0;
	Glyph* previousRow = 0;
	if (currentRowPos > 0 && currentLetterPos == 0)
	{
		//3.1 ���� ���� ���Ѵ�.
		currentRow = this->notepadForm->note->GetAt(currentRowPos);
		//3.2 ���� ���� ���� ���� ���Ѵ�.
		previousRow = this->notepadForm->note->GetAt(currentRowPos - 1);
		//3.3 ���� ���� ���� ���� ������ ���� ��ġ�� ���Ѵ�.
		Long letterPos = previousRow->GetLength();
		//3.3 ���� ���� ���� �ٿ� ��ģ��.
		currentRow->Join(previousRow);
		//3.4 Note���� ���� ���� �ּҸ� �����.
		this->notepadForm->note->Remove(currentRowPos);
		//3.5 ���� ���� �������� ������ ���� ���� �����Ѵ�.
		currentRowPos = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		//3.6 ���� ���� ���� ��ġ�� ������ �������̱� ������ �������ش�.
		//���� ���� ������ ���� ���� ó�� ���̿� ��ġ�ϵ��� �����Ѵ�.
		Long index = this->notepadForm->current->First();
		while (index < letterPos)
		{
			this->notepadForm->current->Next();
			index++;
			//�̷����ϸ� index�� ���� overflow�� ���� �ʱ� ������ �ݺ����� ��� �� ���Եǰ�,
			//�׷� �ᱹ�� ���ѹݺ��� �ȴ�.!!
			//index = this->current->Next();
		}
		//3.7 �޸��� ���� *�� �߰��Ѵ�.
		string name = this->notepadForm->fileName;
		name.insert(0, "*");
		name += " - �޸���";
		this->notepadForm->SetWindowText(CString(name.c_str()));
		//3.8 �޸��忡 ��������� ������ �����Ѵ�.
		this->notepadForm->IsDirty = true;
	}
	// ���� ���� ��ġ�� ó���� �ƴ� ��(���� ���� ó���̵� �ƴϵ� �������)
	// ���� ���ں��� ���� �������� �ܾ� ������ ���ڵ��� �����.
	//4. ���� ���� ��ġ�� ó���� �ƴϸ�
	else if (currentLetterPos > 0)
	{
		//4.1 ���� �������� �ܾ������ �̵��� ���� ������ġ�� ���Ѵ�.
		Long letterPosAfterMoving = this->notepadForm->current->PreviousWord();
		//4.2 ���� ������ġ�� 0���� ū���� �ݺ��Ѵ�.
		while (currentLetterPos > letterPosAfterMoving)
		{
			//4.2.1 ���ڸ� �����.
			this->notepadForm->current->Remove(currentLetterPos - 1);
			currentLetterPos--;
		}
		//4.3 �޸��� ���� *�� �߰��Ѵ�.
		string name = this->notepadForm->fileName;
		name.insert(0, "*");
		name += " - �޸���";
		this->notepadForm->SetWindowText(CString(name.c_str()));
		//4.4 �޸��忡 ��������� ������ �����Ѵ�.
		this->notepadForm->IsDirty = true;
		//4.5 �ڵ� �� �ٲ� �޴��� üũ�Ǿ����� Ȯ���Ѵ�.
		UINT state = this->notepadForm->GetMenu()->
			GetMenuState(IDM_ROW_AUTOCHANGE, MF_BYCOMMAND);
		//4.6 �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
		if (state == MF_CHECKED)
		{
			//4.6.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
			//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
			this->notepadForm->SendMessage(WM_SIZE);
		}
	}
}


//�Ҹ���
CtrlBackSpaceKeyAction::~CtrlBackSpaceKeyAction()
{

}