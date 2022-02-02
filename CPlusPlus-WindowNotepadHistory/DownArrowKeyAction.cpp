#include "DownArrowKeyAction.h"
#include "Glyph.h"
#include "TextExtent.h"

//����Ʈ������
DownArrowKeyAction::DownArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void DownArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. �������� �̵��ϱ� ���� ĳ���� ���� ���� ��ġ�� �����Ѵ�.
	Long previousRowIndex = this->notepadForm->note->GetCurrent();
	//2. �������� �̵��ϱ� ���� ĳ���� ���� ���� ��ġ�� �����Ѵ�.
	Long previousCaretIndex = this->notepadForm->current->GetCurrent();
	//3. ĳ���� ���� ���� ��ġ�� ���� �ٷ� �̵���Ų��.
	Long currentRowIndex = this->notepadForm->note->Next();
	//4. �̵��ϱ� �� ĳ���� ���� ��ġ�� �̵��� �� ĳ���� ���� ��ġ�� ���� �ٸ���(���������� �̵��� ������)
	if (previousRowIndex != currentRowIndex)
	{
		//4.1 ���� ���� �̵��� �� ĳ���� ���� ��ġ�� �ִ� �ٷ� �����Ѵ�.
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
		//4.2 �̵��ϱ� �� ĳ���� ���� ��ġ�� 0�� �ƴϰ� �̵��� �� �������� ���ڰ����� 0�� �ƴϸ�
		if (previousCaretIndex != 0 && this->notepadForm->current->GetLength() != 0)
		{
			//4.2.1 �̵��ϱ� �� ���� �ؽ�Ʈ ���� ���Ѵ�.
			Long previousRowTextWidth = this->notepadForm->textExtent->
				GetTextWidth(this->notepadForm->note->GetAt(previousRowIndex)
					->GetPartOfContent(previousCaretIndex));
			//4.2.2 ĳ���� ���� ���� ��ġ�� ó������ �̵���Ų��.
			this->notepadForm->current->First();//ĳ���� ���� ��ġ�� �� ó��(0)���� ����
			//4.2.3 ù��° ���ڸ� �б� ���� ĳ���� 1 ������Ų��.
			Long i = this->notepadForm->current->Next();
			//4.2.4 ���� ���� �ý�Ʈ�� ���� ���Ѵ�.
			Long currentRowTextWidth = this->notepadForm->textExtent->
				GetTextWidth(this->notepadForm->current->GetPartOfContent(i).c_str());
			//4.2.5 i(ĳ���� ���� ������ġ)�� length(�������� ���ڰ���)���� �۰�
			//���� ���� �ؽ�Ʈ ũ�Ⱑ �̵��ϱ� �� ���� �ؽ�Ʈ ũ�⺸�� �������� �ݺ��Ѵ�.
			while (i < this->notepadForm->current->GetLength()
				&& currentRowTextWidth < previousRowTextWidth)
			{
				//4.2.5.1 i(ĳ���� ���� ������ġ)�� ������Ų��
				//(ĳ���� ���� ��ġ�� ���� ĭ���� �̵���Ų��.).
				i = this->notepadForm->current->Next();
				//4.2.5.2 ���� ���� ĳ���� ���� ��ġ������ �ý�Ʈ ���� ���Ѵ�.
				currentRowTextWidth = this->notepadForm->textExtent->
					GetTextWidth(this->notepadForm->current->GetPartOfContent(i).c_str());
			}
			//4.2.6 ���� ���� �ؽ�Ʈ ������ ���� ���� �ؽ�Ʈ ���� ����.
			Long difference = currentRowTextWidth - previousRowTextWidth;
			//4.2.7 ���̰� 0���� ũ��(�������� �ؽ�Ʈ�� �� ���� ���� ���)
			if (difference > 0)
			{
				//4.2.7.1 ĳ���� ���� ������ġ ������ ���� ���� ���Ѵ�.
				Long letterWidth = this->notepadForm->textExtent->
					GetTextWidth(this->notepadForm->current->GetAt(i - 1)->GetContent());
				Long halfLetterSize = letterWidth / 2;
				//4.2.7.2 ���̰� ���� ����ũ���� ���ݺ��� ���ų� ũ��
				if (difference >= halfLetterSize)
				{
					//4.2.7.2.1 ĳ���� ���� ���� ��ġ�� �������� �̵��Ѵ�.
					this->notepadForm->current->Previous();
				}
			}
		}
		//4.3 ĳ���� ���� ���� ��ġ�� 0 �Ǵ� ĳ���� ���� ���� ���ڰ����� 0���̸�
		else
		{
			//4.3.1 ���� ĳ���� ���� ��ġ�� 0���� �̵���Ų��.
			this->notepadForm->current->First();
		}
	}
}

//�Ҹ���
DownArrowKeyAction::~DownArrowKeyAction()
{

}