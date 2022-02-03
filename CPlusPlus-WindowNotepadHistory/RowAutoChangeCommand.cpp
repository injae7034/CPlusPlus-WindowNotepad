#include "RowAutoChangeCommand.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "TextExtent.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "DummyRow.h"

//����Ʈ������
RowAutoChangeCommand::RowAutoChangeCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{

}

//Execute
void RowAutoChangeCommand::Execute()
{
	Long caretIndex = 0;
	Long rowTextWidth = 0;
	Glyph* glyph = 0;
	Long rowIndex = 0;
	Glyph* row = 0;
	Glyph* previousRow = 0;
	//1. �ڵ� �� �ٲ� �޴��� üũ�Ǿ����� Ȯ���Ѵ�.
	UINT state = this->notepadForm->GetMenu()->
		GetMenuState(IDM_ROW_AUTOCHANGE, MF_BYCOMMAND);
	//2. �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ���� ������
	if (state == MF_UNCHECKED)
	{
		//2.1 �ڵ� �� �ٲ� �޴��� üũ���ش�.
		this->notepadForm->GetMenu()->
			CheckMenuItem(IDM_ROW_AUTOCHANGE, MF_CHECKED | MF_BYCOMMAND);
		//2.2 Note�� �� ���� �������� �������� �ݺ��Ѵ�.
		while (rowIndex < this->notepadForm->note->GetLength())
		{
			//2.2.1 �޸��忡�� rowIndex��° ���� ���Ѵ�.
			row = this->notepadForm->note->GetAt(rowIndex);
			//2.2.2 caretIndex�� ����ġ��Ų��.
			caretIndex = 0;
			//2.2.3 caretIndex�� ������Ų��.
			caretIndex++;
			//2.2.4 rowIndex��° �ٿ��� caretIndex���� �ؽ�Ʈ�� ���α��̸� �����Ѵ�.
			rowTextWidth = this->notepadForm->textExtent->GetTextWidth
				(row->GetPartOfContent(caretIndex).c_str());
			//2.2.5 caretIndex�� rowIndex��° ���� �ѱ��� �������� �������� 
			//�׸��� rowIndex��° ���� ���α��̰� ����ȭ���� ���α��̺��� �������� �ݺ��Ѵ�.
			while (caretIndex < row->GetLength() &&
				rowTextWidth <= this->notepadForm->scrollController->
				scroll[0]->GetPageSize())
			{
				//2.2.5.1 caretIndex�� ������Ų��.
				caretIndex++;
				//2.2.5.2 ������ caretIndex������ ���� ���̸� �����Ѵ�.
				rowTextWidth = this->notepadForm->textExtent->GetTextWidth
				(row->GetPartOfContent(caretIndex));
			}
			//2.2.6 rowIndex��° ���� ���� ���̰� ���� ȭ���� ���� ���̺��� ũ��
			if (rowTextWidth > this->notepadForm->scrollController->
				scroll[0]->GetPageSize())
			{
				//2.2.6.1 caretIndex������ ���̰� ����ȭ���� ���� ����(cx)���� ũ�� ������ 
				//�� ���ù��� ���Դ�. �׷��� ĳ���� �������� �� ĭ �̵��� �ؼ� ���̸� ���
				//����ȭ���� ���� ����(cx)���� �۴�. ĳ��(caretIndex)�� ���� ���ڸ� ���� ��ġ��
				//�ݿ��ϱ� ������ �׻� ���� ���ں��� ��ĭ �ռ� �ִ�
				//�׷��� caretIndex-1���� split�� �ؾ� ȭ���� �Ѵ� ���ڸ� ���� �ٷ� ���� �� �ִ�.
				caretIndex--;
				//2.2.6.2 rowIndex��° ���� ���� ���̰� ����ȭ���� ���� ���̺��� Ŀ�� ������
				//���ں��� rowIndex��° �ٿ��� caretIndex ���� ��ġ�� �ִ� ���ڵ��� ������.
				//(DummyRow����)
				glyph = row->Split(caretIndex, true);
				//2.2.6.3 ���ο� ���� rowIndex��° ���� ���� ��ġ�� �����ִ´�.
				rowIndex = this->notepadForm->note->Add(rowIndex + 1, glyph);
			}
			//2.2.7 caretIndex�� rowIndex��° ���� �ѱ��� �������� ũ�ų� ������
			else if (caretIndex >= row->GetLength())
			{
				//2.2.7.1 ���� �ٷ� �̵��Ѵ�.
				rowIndex++;
			}
		}
	}
	//3. �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
	else
	{
		//3.1 �ڵ� �� �ٲ� �޴��� �ִ� üũ�� ���ش�.	
		this->notepadForm->GetMenu()->
			CheckMenuItem(IDM_ROW_AUTOCHANGE, MF_UNCHECKED | MF_BYCOMMAND);
		//3.2 Note�� �� ���� �������� �������� �ݺ��Ѵ�.
		while (rowIndex < this->notepadForm->note->GetLength())
		{
			//3.2.1 �޸��忡�� rowIndex��° ���� ���Ѵ�.
			row = this->notepadForm->note->GetAt(rowIndex);
			//3.2.2 DummyRow�̸�
			if (dynamic_cast<DummyRow*>(row))
			{
				//3.2.2.1 DummyRow ���� ��(Row)�� ���Ѵ�.
				previousRow = this->notepadForm->note->GetAt(rowIndex - 1);
				//3.2.2.2 DummyRow�� ���� ��(Row)�� ��ģ��.
				row->Join(previousRow);
				//3.2.2.3 Note���� DummyRow�� �ּҸ� �����.
				this->notepadForm->note->Remove(rowIndex);
			}
			//3.2.3 DummyRow�� �ƴϸ�
			else
			{
				//3.2.3.1 ���� �ٷ� �̵��Ѵ�.
				rowIndex++;
			}
		}
	}
	//4. ĳ���� ���� ó������ ������.
	this->notepadForm->note->First();
	this->notepadForm->current = this->notepadForm->note->
		GetAt(this->notepadForm->note->GetCurrent());
	this->notepadForm->current->First();
	//5. ������ũ���� ���� ó������ ������.
	this->notepadForm->scrollController->scroll[1]->First();
	//6. ������ũ�ѹ��� Thumb�� ������ũ���� ������ġ�� �̵���Ų��.
	this->notepadForm->SetScrollPos(SB_VERT,
		this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
	//7. ��ũ���� ��ġ�� ����Ǿ����� �˸���.
	this->notepadForm->Notify();
	//8. ��������� �����Ѵ�.
	this->notepadForm->Invalidate(TRUE);
}


//�Ҹ���
RowAutoChangeCommand::~RowAutoChangeCommand()
{

}