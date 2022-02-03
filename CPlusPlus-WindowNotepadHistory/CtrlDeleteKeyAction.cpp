#include "CtrlDeleteKeyAction.h"
#include "Glyph.h"

//����Ʈ������
CtrlDeleteKeyAction::CtrlDeleteKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void CtrlDeleteKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. ���� ���� ��ġ�� ���Ѵ�.
	Long currentRowPos = this->notepadForm->note->GetCurrent();
	//2. ���� ������ ��ġ�� ���Ѵ�.
	Long currentLetterPos = this->notepadForm->current->GetCurrent();
	//3. ���� ��Ʈ�� ������ ���� ��ġ�� ���Ѵ�.
	Long lastRowPos = this->notepadForm->note->GetLength() - 1;
	//4. ���� �ٿ��� ������ ������ ��ġ�� ���Ѵ�.
	Long lastLetterPos = this->notepadForm->current->GetLength();
	//���� ������ ���̸� ���� ���� �� ����, ������ �ٿ��� ���� ��ġ�� �������� ������ �ƹ��͵� ���Ͼ
	// ���� ���� ��ġ�� �������� �ƴϰ�, ���� ���� ��ġ�� �������̸� ���� ���� ���� �ٷ� ���Խ�Ų��.
	//5. ���� ���� ��ġ�� ��Ʈ�� ������ �� ��ġ���� �۰�, ���� ���� ��ġ�� �������̸�
	Glyph* currentRow = 0;
	Glyph* nextRow = 0;
	if (currentRowPos < lastRowPos && currentLetterPos == lastLetterPos)
	{
		//5.1 ���� ���� ���Ѵ�.
		currentRow = this->notepadForm->note->GetAt(currentRowPos);
		//5.2 ���� ���� ���� ���� ���Ѵ�.
		nextRow = this->notepadForm->note->GetAt(currentRowPos + 1);
		//5.3 ���� ���� ���� �ٿ� ��ģ��.
		nextRow->Join(currentRow);
		//5.4 Note���� ���� ���� �ּҸ� �����.
		this->notepadForm->note->Remove(currentRowPos + 1);
		//5.5 ���� ���� ���� ��ġ�� ������ �������̱� ������ �������ش�.
		Long index = this->notepadForm->current->First();
		while (index < lastLetterPos)
		{
			this->notepadForm->current->Next();
			index++;
			//�̷����ϸ� index�� ���� overflow�� ���� �ʱ� ������ �ݺ����� ��� �� ���Եǰ�,
			//�׷� �ᱹ�� ���ѹݺ��� �ȴ�.!!
			//index = this->current->Next();
		}
		//5.6 �޸��� ���� *�� �߰��Ѵ�.
		string name = this->notepadForm->fileName;
		name.insert(0, "*");
		name += " - �޸���";
		this->notepadForm->SetWindowText(CString(name.c_str()));
		//5.7 �޸��忡 ��������� ������ �����Ѵ�.
		this->notepadForm->IsDirty = true;
	}
	// ���� ���� ��ġ�� �������� �ƴ� ��(���� ���� �������̵� �ƴϵ� �������)
	//���� ������ ���� ���ں��� ���� ������ ���ڱ��� �� �����.
	//6. ���� ���� ��ġ�� �������� �ƴϸ�
	else if (currentLetterPos < lastLetterPos)
	{
		//6.1 ������ �������� �ܾ������ �̵��� ���� ������ġ�� ���Ѵ�.
		Long letterPosAfterMoving = this->notepadForm->current->NextWord();
		//6.2 ������ �������� �ܾ������ �̵��� ���� ������ġ�� ���������ġ���� ū���� �ݺ��Ѵ�.
		while (letterPosAfterMoving > currentLetterPos)
		{
			//6.2.1 ���ڸ� �����.
			this->notepadForm->current->Remove(letterPosAfterMoving - 1);
			letterPosAfterMoving--;
		}
		//6.3 �޸��� ���� *�� �߰��Ѵ�.
		string name = this->notepadForm->fileName;
		name.insert(0, "*");
		name += " - �޸���";
		this->notepadForm->SetWindowText(CString(name.c_str()));
		//6.4 �޸��忡 ��������� ������ �����Ѵ�.
		this->notepadForm->IsDirty = true;
		//6.5 �ڵ� �� �ٲ� �޴��� üũ�Ǿ����� Ȯ���Ѵ�.
		UINT state = this->notepadForm->GetMenu()->
			GetMenuState(IDM_ROW_AUTOCHANGE, MF_BYCOMMAND);
		//6.6 �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
		if (state == MF_CHECKED)
		{
			//6.6.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
			//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
			this->notepadForm->SendMessage(WM_SIZE);
		}
	}
}


//�Ҹ���
CtrlDeleteKeyAction::~CtrlDeleteKeyAction()
{

}