#include "RowAutoChange.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "TextExtent.h"
#include "DummyRow.h"

//����Ʈ������
RowAutoChange::RowAutoChange(NotepadForm* notepadForm)
{
	this->notepadForm = notepadForm;
}

//�ڵ������� �ϴ�
void RowAutoChange::Do()
{
	Long letterIndex = 0;
	Long rowTextWidth = 0;
	Glyph* glyph = 0;
	Long rowIndex = 0;
	Glyph* row = 0;
	Glyph* previousRow = 0;
	
	//1. ���� ȭ���� ũ�⸦ ���Ѵ�.
	CRect rect;
	this->notepadForm->GetClientRect(&rect);
	//2. ���� ȭ���� ���� ���̸� ���Ѵ�.
	Long pageWidth = rect.Width();
	//3. Note�� �� ���� �������� �������� �ݺ��Ѵ�.
	while (rowIndex < this->notepadForm->note->GetLength())
	{
		//3.1 �޸��忡�� rowIndex��° ���� ���Ѵ�.
		row = this->notepadForm->note->GetAt(rowIndex);
		//3.2 letterIndex�� ����ġ��Ų��.
		letterIndex = 0;
		//3.3 rowTextWidth�� ����ġ��Ų��.
		rowTextWidth = 0;
		//3.4 letterIndex�� rowIndex��° ���� �ѱ��� �������� �������� 
		//�׸��� rowIndex��° ���� ���α��̰� ����ȭ���� ���α��̺��� �������� �ݺ��Ѵ�.
		while (letterIndex < row->GetLength() && rowTextWidth <= pageWidth)
		{
			//3.4.1 ������ letterIndex������ ���� ���̸� �����Ѵ�.
			rowTextWidth = this->notepadForm->textExtent->GetTextWidth
			(row->GetPartOfContent(letterIndex + 1));
			//3.4.2 letterIndex�� ������Ų��.
			letterIndex++;
		}
		//3.5 rowIndex��° ���� ���� ���̰� ���� ȭ���� ���� ���̺��� ũ��
		if (rowTextWidth > pageWidth)
		{
			//3.5.1 letterIndex������ ���̰� ����ȭ���� ���� ����(cx)���� ũ�� ������ 
			//�� ���ù��� ���Դ�. �׷��� ĳ���� �������� �� ĭ �̵��� �ؼ� ���̸� ���
			//����ȭ���� ���� ����(cx)���� �۴�. ĳ��(letterIndex)�� ���� ���ڸ� ���� ��ġ��
			//�ݿ��ϱ� ������ �׻� ���� ���ں��� ��ĭ �ռ� �ִ�
			//�׷��� letterIndex-1���� split�� �ؾ� ȭ���� �Ѵ� ���ڸ� ���� �ٷ� ���� �� �ִ�.
			letterIndex--;
			//3.5.2 rowIndex��° ���� ���� ���̰� ����ȭ���� ���� ���̺��� Ŀ�� ������
			//���ں��� rowIndex��° �ٿ��� letterIndex ���� ��ġ�� �ִ� ���ڵ��� ������.
			//(DummyRow����)
			glyph = row->Split(letterIndex, true);
			//3.5.3 ���ο� ���� rowIndex��° ���� ���� ��ġ�� �����ִ´�.
			rowIndex = this->notepadForm->note->Add(rowIndex + 1, glyph);
		}
		//3.6 letterIndex�� rowIndex��° ���� �ѱ��� �������� ũ�ų� ������
		else if (letterIndex >= row->GetLength())
		{
			//3.6.1 ���� �ٷ� �̵��Ѵ�.
			rowIndex++;
		}
	}
}

//�ڵ������� ����ϴ�
void RowAutoChange::Undo()
{
	Long rowIndex = 0;
	Glyph* row = 0;
	Glyph* previousRow = 0;

	//1. Note�� �� ���� �������� �������� �ݺ��Ѵ�.
	while (rowIndex < this->notepadForm->note->GetLength())
	{
		//1.1 �޸��忡�� rowIndex��° ���� ���Ѵ�.
		row = this->notepadForm->note->GetAt(rowIndex);
		//1.2 DummyRow�̸�
		if (dynamic_cast<DummyRow*>(row))
		{
			//1.2.1 DummyRow ���� ��(Row)�� ���Ѵ�.
			previousRow = this->notepadForm->note->GetAt(rowIndex - 1);
			//1.2.2 DummyRow�� ���� ��(Row)�� ��ģ��.
			row->Join(previousRow);
			//1.2.3 Note���� DummyRow�� �ּҸ� �����.
			this->notepadForm->note->Remove(rowIndex);
		}
		//1.3 DummyRow�� �ƴϸ�
		else
		{
			//1.3.1 ���� �ٷ� �̵��Ѵ�.
			rowIndex++;
		}
	}
}

//�ڵ����� �� �ٰ� ĳ���� ��ġ�� ���� �ڵ����� �� ���� �ٰ� ĳ���� ��ġ�� ���Ѵ�
void RowAutoChange::GetOriginPos(Long changedCaretPos, Long changedRowPos,
	Long* originCaretPos, Long* originRowPos)
{
	//1. �ڵ����� �� �ٰ� ĳ���� ��ġ�� �Է¹޴´�.
	//2. �ڵ����� �� ĳ���� �ִ� ������ ���� ������ ���Ѵ�.
	Long rowCount = changedRowPos + 1;
	//�ڵ����� ���� ���� ���� ��ġ�� ���Ѵ�.
	//��¥ ��(Row)�̸� count�� i �Ѵ� ����, ��¥ ��(DummyRow)�̸� i�� ����.
	//3. i�� rowCount���� �������� �ݺ��Ѵ�.
	Glyph* row = 0;
	Long count = 0;
	Long i = 0;
	while (i < rowCount)
	{
		//3.1 ��¥��(Row)���� ��¥��(DummyRow)���� �Ǵ��ϱ� ���� ���� ���Ѵ�.
		row = this->notepadForm->note->GetAt(i);
		//3.2 ��¥��(Row)�̸�
		//if(dynamic_cast<Row*>(row))�� �ϸ� DummyRow�� Row�� �ڽ��̱� ������
		//Row�� DummyRow�� �Ѵ� ���ù��� ���� �Ǳ� ������ ������ �ٲ㼭
		//DummyRow�� �ƴϸ� ���ǹ��� ���� ������ Row�� ���ǹ��� ���� DummyRow�� �ȵ�!
		if (!dynamic_cast<DummyRow*>(row))
		{
			//3.2.1 count�� ����.
			count++;
		}
		//3.3 i�� ����.
		i++;
	}
	//4. �ڵ������� ���� ��ġ�� �����Ѵ�.(����Ѵ�.)
	//count�� �����̹Ƿ� 1���̽��̰� originRowPos�� ��ġ�̹Ƿ� 0���̽��̱� ������ (����-1)�� �ʿ���!
	*originRowPos = count - 1;
	//�ڵ����� �� ���� ĳ���� ��ġ�� ���Ѵ�.
	//5. �ڵ����� �� ���� ��ġ�� �Է��� ���� ���Ѵ�.
	row = this->notepadForm->note->GetAt(changedRowPos);
	//6. ���� ���� ��¥ ��(Row)�� �ִ� ��ġ�� �˱� ���� ��¥��(DummyRow)�ε��� �ݺ��Ѵ�.
	//���� ���� �ٷ� ��¥ ��(Row)�̸� �ݺ������� ���� �ʰ� ���� ���� ��¥��(DummyRow)�̸�
	//��¥��(Row)�� ��ġ�� ã�� ������ �ݺ��Ѵ�.
	Long realRowPos = changedRowPos;
	while (dynamic_cast<DummyRow*>(row))
	{
		//6.1 ���� ��ġ�� ���ҽ�Ų��.
		realRowPos--;
		//6.2 ���� ���Ѵ�.
		row = this->notepadForm->note->GetAt(realRowPos);
	}
	//7. �ڵ����� �� ���� ��ġ�� ��¥ ���� ��ġ�� �ƴϸ�
	if (changedRowPos != realRowPos)
	{
		//��¥��(Row)���� �����ؼ� ��¥��(DummyRow)�� changedCaretPos���� ĳ���� ��ġ�� �����ش�.
		//7.1 i�� �ڵ����� �� ���� ��ġ���� �������� �ݺ��Ѵ�.
		Long letterCount = 0;
		i = realRowPos;
		while (i < changedRowPos)
		{
			//7.3.1 ���� ���Ѵ�.
			row = this->notepadForm->note->GetAt(i);
			//7.3.2 ���� ������ ���Ѵ�.
			letterCount = row->GetLength();
			//7.3.3 �ڵ����� �� ĳ���� ��ġ�� ���ڰ����� �����ش�.
			changedCaretPos += letterCount;
			//7.3.4 i�� ������Ų��.
			i++;
		}
	}
	//8. �ڵ����� �� ���� ĳ���� ��ġ�� �����Ѵ�.(����Ѵ�.)
	*originCaretPos = changedCaretPos;

#if 0
	//�ڵ����� �� ���� ��ġ�� ���� ���� ���ϰ� ��¥��(Row)�� ���� ���� -1�� �ϸ鼭 �Ž��� �ö󰣴�.
	//5. �ڵ����� �� ���� ��ġ�� �Է��� ���� ���Ѵ�.
	row = this->notepadForm->note->GetAt(changedRowPos);
	//6. ���� ���� ��¥ ���� ���� �ݺ��Ѵ�.
	while (dynamic_cast<DummyRow*>(row))
	{
		//6.1 �ڵ����� �� ĳ���� ��ġ���� ���� ���ڰ�����ŭ �����ش�.
		//������ �ٸ��� �ٸ��� ������ �ִ� ���� �ѱ۸� �ִ� ���̳� �ѱ��̶� �����̶� ���� �ٺ���
		//������ ������ ��, �ٸ��� ������ �޶����� ������ ���� �����ָ� �ȵǰ� �ٸ��� ���̸� ���ؼ�
		//�׿� �°� ������ ��������Ѵ�.!!
		changedCaretPos += row->GetLength();
		//6.2 ���� ��ġ�� ���ҽ�Ų��.
		changedRowPos--;
		//6.3 ���� ���Ѵ�.
		row = this->notepadForm->note->GetAt(changedRowPos);
	}
	//7. �ڵ����� �� ���� ��ġ�� �����Ѵ�.(����Ѵ�.)
	*originCaretPos = changedCaretPos;
	//8. ������.
#endif
#if 0
	//2. ���� ���� ���Ѵ�.
	Glyph *row= this->notepadForm->note->GetAt(changedRowPos);
	//3. ���� ���� ��¥���ε��� �ݺ��Ѵ�.
	Long rowIndex = changedRowPos;
	while (dynamic_cast<DummyRow*>(row))
	{
		//3.1 ���� ��ġ�� ���ҽ�Ų��.
		rowIndex--;
		//3.2 ���� ���Ѵ�.
		row = this->notepadForm->note->GetAt(rowIndex);
	}
	//4. �ڵ����� �� ���� ���� ��ġ�� �����Ѵ�.
	*originRowPos = rowIndex;
	//5. ���� ��ġ�� �Է� ���� ���� ���� ��ġ���� �������� �ݺ��Ѵ�.
	Long caretIndex = 0;
	Long count = 0;// originCaretPos
	while (rowIndex < changedRowPos)
	{
		//5.1 ���� ���Ѵ�.
		row = this->notepadForm->note->GetAt(rowIndex);
		//5.2 �ٿ��� ĳ���� ó������ ������.
		caretIndex = row->First();
		//5.3 ĳ���� ���� ���� ���ڰ������� �������� �ݺ��Ѵ�.
		while (caretIndex < row->GetLength())
		{
			//5.3.1 ���� �ٿ��� ĳ���� �������� ������.
			caretIndex = row->Next();
			//5.3.2 count�� ����.
			count++;
		}
		//5.4 ���� ���� 1 ������Ų��.
		rowIndex++;
	}
	//6. ���� ���Ѵ�.(rowIndex=changedRowPos)
	row = this->notepadForm->note->GetAt(rowIndex);
	//7. �ٿ��� ĳ���� ó������ ������.
	caretIndex = row->First();
	//8. ĳ���� �Է¹��� ĳ���� ��ġ���� �������� �ݺ��Ѵ�.
	while (caretIndex < changedCaretPos)
	{
		//8.1 ���� �ٿ��� ĳ���� �������� ������.
		caretIndex = row->Next();
		//8.2 count�� ����.
		count++;
	}
	//9. �ڵ����� �� ���� ĳ���� ��ġ�� �����Ѵ�.
	*originCaretPos = count;
	//10. �ڵ����� �� ���� ���� ��ġ�� �ڵ����� �� ���� ĳ���� ��ġ�� ����Ѵ�.
	//11. ������.
#endif
}

//�ڵ����� �� ���� �ٰ� ĳ���� ��ġ�� ���� �ڵ����� �� �ٰ� ĳ���� ��ġ�� ���Ѵ�.
void RowAutoChange::GetChangedPos(Long originCaretPos, Long originRowPos,
	Long* changedCaretPos, Long* changedRowPos)
{
	//1. �ڵ����� �� ���� �ٰ� ĳ���� ��ġ�� �Է¹޴´�.
	//2. �ڵ����� �� ĳ���� �ִ� ������ ���� ������ ���Ѵ�.
	Long rowCount = originRowPos + 1;
	//��¥��(DummyRow)�̸� rowCount�� i�� �Ѵټ���, ��¥��(Row)�̸� i�� ����.
	//3. i�� rowCount���� �������� �ݺ��Ѵ�.
	Long realRowPos = 0;
	Glyph* row = 0;
	Long rowCountOfNote = this->notepadForm->note->GetLength();
	Long i = 0;
	Long rowIndex = 0;
	while (i < rowCount && rowIndex < rowCountOfNote)
	{
		//3.1 row�� ���Ѵ�.
		row = this->notepadForm->note->GetAt(rowIndex);
		//3.2 row�� ��¥ ���̸�
		//if(dynamic_cast<Row*>(row))�� �ϸ� DummyRow�� Row�� �ڽ��̱� ������
		//Row�� DummyRow�� �Ѵ� ���ù��� ���� �Ǳ� ������ ������ �ٲ㼭
		//DummyRow�� �ƴϸ� ���ǹ��� ���� ������ Row�� ���ǹ��� ���� DummyRow�� �ȵ�!
		if (!dynamic_cast<DummyRow*>(row))
		{
			//3.2.1 �ݺ�������� ����.
			i++;
		}
		//3.3 rowIndex�� ����.
		rowIndex++;
	}
	//��¥ ���� ��ġ�� ���Ѵ�.
	//4. i�� rowCount���� ũ�ų� ������
	if (i >= rowCount)
	{
		//4.1 ��¥���� ��ġ�� �����Ѵ�.
		realRowPos = rowIndex - 1;
	}
	//5. ���� ���� ���ڰ����� ���Ѵ�.
	i = realRowPos;
	row = this->notepadForm->note->GetAt(realRowPos);
	Long letterCount = row->GetLength();
	//5. ���� ĳ���� ��ġ�� ���� ���� ���ڰ������� ū���� �ݺ��Ѵ�.
	while (originCaretPos > letterCount)
	{
		//5.1 ���� ĳ���� ��ġ�� ���� ���� ���ڰ����� ����.
		originCaretPos -= letterCount;
		//5.2 i�� ������Ų��.
		i++;
		//5.3 row�� ���Ѵ�.
		row = this->notepadForm->note->GetAt(i);
		letterCount = row->GetLength();
	}
	//6. �ڵ������� ���� ��ġ�� �����Ѵ�.(����Ѵ�.)
	*changedRowPos = i;
	//7. �ڵ����� �� ĳ���� ��ġ�� �����Ѵ�.(����Ѵ�.)
	*changedCaretPos = originCaretPos;
	//8. ������.
#if 0
	//1. �ڵ����� �� ���� �ٰ� ĳ���� ��ġ�� �Է¹޴´�.
	//2. �ڵ����� �� ���� ���� ��ġ(originRowPos)���� �۰ų� ���� ����
	//�׸��� rowIndex�� note�� ���� �������� �������� �ݺ��Ѵ�.
	Long rowLength = this->notepadForm->note->GetLength();
	Long nextRowPos = originRowPos + 1;
	Long rowIndex = 0;
	Glyph* row = 0;
	Long i = 0;
	while (i <= nextRowPos && rowIndex < rowLength)
	{
		//2.1 row�� ���Ѵ�.
		row = this->notepadForm->note->GetAt(rowIndex);
		//2.2 row�� ��¥ ���̸�
		if (dynamic_cast<Row*>(row))
		{
			//2.2.1 �ݺ�������� ����.
			i++;
		}
		//2.3 rowIndex�� ����.
		rowIndex++;
	}
	//3. rowIndex�� ���� ���� ���� ���Ѵ�.
	i = rowIndex;
	row = this->notepadForm->note->GetAt(i);
	//4. �������� ��¥���ε��� �ݺ��Ѵ�.
	while (dynamic_cast<DummyRow*>(row))
	{
		//4.1 ���� ��ġ�� ���ҽ�Ų��.
		i--;
		//4.2 ���� ���Ѵ�.
		row = this->notepadForm->note->GetAt(i);
	}
	//5. i�� rowIndex���� �������� �ݺ��Ѵ�.
	Long count = originCaretPos;
	row = this->notepadForm->note->GetAt(i);
	rowLength = row->GetLength();
	while (i < rowIndex)
	{
		//5.1 ���� ĳ���� ��ġ���� ���� ���� ������ŭ ����.
		count -= rowLength;
		//5.2 i�� ������Ų��.
		i++;
	}
	//6. rowIndex�� count�� ����Ѵ�.
	*changedRowPos = rowIndex;
	*changedCaretPos = count;
	//7. ������.
#endif
}

//�Ҹ���
RowAutoChange::~RowAutoChange()
{

}