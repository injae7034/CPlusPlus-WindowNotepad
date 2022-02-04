#include "GlyphReplacement.h"
#include "Glyph.h"

//����Ʈ������
GlyphReplacement::GlyphReplacement(Glyph* note)
{
	this->note = note;
}

//�ٲٱ�
void GlyphReplacement::Replace(string keyword, Long startSelectedRowIndex,
	Long startSelectedLetterIndex)
{
	//1. ���� ���� ��ġ�� ���Ѵ�.(���� ���� ��ġ�� ������ ������ ���� ��ġ��)
	Long endSelectedRowIndex = this->note->GetCurrent();
	//2. ���� ������ ��ġ�� ���Ѵ�.(���� ������ ��ġ�� ������ ������ ������ ��ġ��)
	Long endSelectedLetterIndex = this->note->GetAt(endSelectedRowIndex)->GetCurrent();
	Long letterIndex = startSelectedLetterIndex;
	Long rowIndex = startSelectedRowIndex;
	Glyph* startRow = this->note->GetAt(rowIndex);
	//3. ������ �� ���� �����̸�
	if (startSelectedRowIndex == endSelectedRowIndex)
	{
		//3.1 ������ ���۵Ǵ� ���ں��� ������ ���ڱ��� ���ڸ� �����.
		while (letterIndex < endSelectedLetterIndex)
		{
			//3.1.1 ���ڸ� �����.
			startRow->Remove(letterIndex);
			//3.1.2 ���ڸ� ������ ������ ������ ���ڸ� ������ �� ĭ ����ش�.
			endSelectedLetterIndex--;
		}
	}
	//4. ������ �� ���� �������̸�
	else
	{
		//4.1 ������ ���۵Ǵ� ���� �������� �������� �ݺ��Ѵ�.
		while (letterIndex < startRow->GetLength())
		{
			//4.1.1 ���ڸ� �����.
			startRow->Remove(letterIndex);
		}
		//4.2 ������ ������ �� ������ �ݺ��Ѵ�.
		Glyph* row = 0;
		rowIndex++;
		while (rowIndex < endSelectedRowIndex)
		{
			//4.2.1 ���� ���Ѵ�.
			row = this->note->GetAt(rowIndex);
			//4.2.2 ���� ��ġ�� �ʱ�ȭ��Ų��.
			letterIndex = 0;
			//4.2.3 ���� ��ġ�� ���� �������� �������� �ݺ��Ѵ�.
			while (letterIndex < row->GetLength())
			{
				//4.2.3.1 ���ڸ� �����.
				row->Remove(letterIndex);
			}
			//4.2.4 �ٿ� ������ ���ڰ� �� �������� ������ ���� �����.
			this->note->Remove(rowIndex);
			//4.2.5 ���� �������� ������ endSelectedRowIndex�� ���� ������ ����ش�.
			endSelectedRowIndex--;
		}
		//4.3 ������ ������ ���� ������ ������ ���ڱ��� �ݺ��Ѵ�.
		letterIndex = 0;
		Glyph* endRow = this->note->GetAt(endSelectedRowIndex);
		while (letterIndex < endSelectedLetterIndex)
		{
			//4.3.1 ���ڸ� �����.
			endRow->Remove(letterIndex);
			//4.3.2 ���ڸ� ������ ������ endSelectedLetterIndex�� ������ ��ĭ ����.
			endSelectedLetterIndex--;
		}
		//4.4 ������ ������ ������ ���� ������ ������ ���� ������ ���ڵ��� ������ ���۵Ǵ� �ٿ� Join��Ų��.
		endRow->Join(startRow);
		//4.5 ������ ���� �޸��忡�� �����.
		this->note->Remove(endSelectedRowIndex);
	}
	//���ڸ� �������� ������ ���� �ٲ� �ܾ �߰��Ѵ�.
	
}

//��� �ٲٱ�

//�Ҹ���
GlyphReplacement::~GlyphReplacement()
{

}