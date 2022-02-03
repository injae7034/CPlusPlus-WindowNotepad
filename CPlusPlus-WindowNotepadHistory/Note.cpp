#include "Note.h"
#include "Letter.h"

//����Ʈ������
Note::Note(Long capacity)
	:Composite(capacity)
{

}

//���������
Note::Note(const Note& source)
	:Composite(source)
{

}

//ġȯ������
Note& Note::operator=(const Note& source)
{
	//this->Composite::operator=(source);
	Composite::operator=(source);

	return *this;
}

//�Ҹ���
Note::~Note()
{

}

//Prototype Patter Clone
Glyph* Note::Clone()
{
	return new Note(*this);
}

//GetContent
string Note::GetContent()
{
	string content;
	Long index = 0;
	while (index < this->length)
	{
		content += this->glyphs.GetAt(index)->GetContent();
		content += '\n';
		index++;
	}

	return content;
}

//ĳ���� ���� ���� ��ġ�� ���������� �̵���Ű��
Long Note::Last()
{
	//1. ĳ���� ���� ���� ��ġ�� ��Ʈ�� �� ������ �̵���Ų��.
	this->current = this->length - 1;
	//2. ĳ���� ���� ���� ��ġ�� ����Ѵ�.
	return this->current;
}

//ĳ���� ���� ���� ��ġ�� �� �� ����(�Ʒ�)���� �̵���Ű��
Long Note::Next()
{
	//�̵���ų ���� �׻� ���� �̵��� ��Ű��, �� ������ overflow�̸� �ִ밪���� ����������.
	//1. ĳ���� ���� ���� ��ġ�� �� �� �������� �̵���Ų��.
	this->current++;
	//2. ĳ���� ���� ���� ��ġ�� overflow�̸�
	if (this->current >= this->length)
	{
		//2.1 ĳ���� ���� ���� ��ġ�� �ִ밪���� �����Ѵ�.
		this->current = this->length - 1;
	}
	//3. ĳ���� ���� ���� ��ġ�� ����Ѵ�.
	return this->current;
}

//Note���ؿ� �°� Add, Remove ������(�������̵�)
Long Note::Add(Glyph* glyph)
{
	//1. �߰��� ���� �ּҸ� �Է¹޴´�.
	//2. ��뷮�� �Ҵ緮���� ������
	Long index;
	if (this->length < this->capacity)
	{
		//2.1 ����ִ� ������ ���������� �����Ѵ�.
		index = this->glyphs.Store(this->length, glyph);
	}
	//3. �׷��� ������
	else
	{
		//3.1 ������ �� ĭ �÷��� �������� �����Ѵ�.
		index = this->glyphs.AppendFromRear(glyph);
		//3.2 �Ҵ緮�� ������Ų��.
		this->capacity++;
	}
	//4. ��뷮�� ������Ų��.
	this->length++;
	//5. ĳ���� ���� ���� ��ġ(���� ���� ��ġ)�� �����Ѵ�.(Note����)
	this->current = index;
	//6. ���� ���� ��ġ�� ����Ѵ�.(Note����)
	return index;
}

//Add(Insert)
Long Note::Add(Long index, Glyph* glyph)
{
	//1. ���� �� ���� index�� ���� �ּҸ� �Է¹޴´�.
	//2. ���� �迭 ����� index������ ���� �ִ´�.
	index = this->glyphs.Insert(index, glyph);
	//3. �Ҵ緮�� ������Ų��.
	this->capacity++;
	//4. ��뷮�� ������Ų��.
	this->length++;
	//5. ĳ���� ���� ���� ��ġ(���� ���� ��ġ)�� �����Ѵ�.(Note����)
	this->current = index;
	//6. ���� ���� ��ġ�� ����Ѵ�.(Note����)
	return index;
}

Long Note::Remove(Long index)
{
	//1. ���� ���� ��ġ�� �Է¹޴´�.
	//2. ���� ����� ���� ����� ���� ��ġ���� ���� ��ġ�� ���� ��ġ�� �Ǳ� ������ 
	//���� ĳ���� ���� ��ġ�� index-1�� �����Ѵ�.
	this->current = index - 1;
	//3. �ش�迭��Ұ� �ּҸ� �����ϰ� �ֱ� ������ �ش�迭����� ���� �Ҵ�� ����(��)�� ���� ��������.
	Glyph* glyph = this->glyphs[index];
	if (glyph != 0)
	{
		delete glyph;
	}
	//4. �ش���ġ�� �迭����� ���� �Ҵ�� ����(��)�� �������� ���� �� �ּҸ� �����.
	index = this->glyphs.Delete(index);
	//5. �Ҵ緮�� ���ҽ�Ų��.
	this->capacity--;
	//6. ��뷮�� ���ҽ�Ų��.
	this->length--;
	//7. index�� ����Ѵ�.
	return index;
}

//��Ʈ���� ĳ���� �ܾ������ ���������� �̵���Ű��
Long Note::NextWord()
{
	Long index = 0;
	string letter;
	//1. ĳ���� ���� ������ġ�� �������� ���ڰ����� ���� ���
	//(ĳ���� ���� ���� ������ ���� �ڿ� ��ġ�ϰ� �ִ� ����̰� �� ��쿡�� ���� �ٷ� �̵���!)
	//��, �����ٿ��� �����ٷ� �̵��ϴ� ���(�ٿ��� �ٷ� �̵��ϴ� ���) ó���ϱ�
	if (this->GetAt(this->current)->GetCurrent()
		== this->GetAt(this->current)->GetLength())
	{
		//1.1 ĳ���� ���� ���� ��ġ�� 1��ŭ ������Ų��.
		this->current++;
		//1.2 ĳ���� ���� ���� ��ġ�� overflow�̸�
		if (this->current >= this->length)
		{
			//1.2.1 ĳ���� ���� ���� ��ġ�� �ִ밪���� �����Ѵ�.
			this->current = this->length - 1;
			//1.2.2 ĳ���� ���� ���� ��ġ�� ���������� �����Ѵ�.
			index = this->GetAt(this->current)->Last();
		}
		//1.3 ĳ���� ���� ���� ��ġ�� overflow�� �ƴϸ�
		else
		{
			//1.3.1 ĳ���� ���� �ٷ� �̵��߱� ������ ĳ���� ���� ���� ��ġ�� 0���� �����Ѵ�.
			index = this->GetAt(this->current)->First();
			//1.3.2 �������� �̵��� ���� ���� ������ 0���� ũ��
			//�������� �̵��� ���� ���ڸ� �ּ� 1�� �̻��� ������ ������
			//�������� �̵��� ���� ���� ������ 0�̸� ���� ���ڰ� ���⶧���� ���ڸ� ������ �ȵǰ�
			//First�� �̵����ױ� ������ �� �ڸ��� �״�� ������ ��.
			if (this->GetAt(this->current)->GetLength() > 0)
			{
				//1.3.2.1. �������� �̵��� ���� ù��° ���ڸ� �д´�.
				letter = this->GetAt(this->current)->GetAt(index)->GetContent();
				//1.3.2.2 ������ ���� ��ġ�� �����Ѵ�.
				Long lastPositionOfLetter = this->GetAt(this->current)->GetLength() - 1;
				//1.3.2.3 ĳ���� ���� ���� ��ġ�� ���� ������ ���� ��ġ���� �������� �׸���
				//���� ���� ���ڰ� �ǹ����� ���� �ݺ��Ѵ�.
				while (index < lastPositionOfLetter && letter == "\t")
				{
					//1.3.2.1.1 ĳ���� ������ġ�� �������� �̵���Ų��.
					index = this->GetAt(this->current)->Next();
					//1.3.2.1.2 ���� ���ڸ� �д´�.
					letter = this->GetAt(this->current)->GetAt(index)->GetContent();
				}
				//1.3.2.4 ĳ���� ���� ���� ��ġ�� ���� ������ ���� ��ġ�� ����,
				//���� ���� ���ڰ� �ǹ����̸�
				if (index == lastPositionOfLetter && letter == "\t")
				{
					//1.3.2.4.1 ĳ���� ���� ��ġ�� ���������� �̵���Ų��.
					index = this->GetAt(this->current)->Last();
				}
				//1.3.2.5 ĳ���� ���� ���� ��ġ�� ���� ������ ���� ��ġ���� �������� �׸���
				//���� ���� ���ڰ� ����(�����̽��� ����)�� ���� �ݺ��Ѵ�.
				while (index < lastPositionOfLetter && letter == " ")
				{
					//1.3.2.2.1 ĳ���� ������ġ�� �������� �̵���Ų��.
					index = this->GetAt(this->current)->Next();
					//1.3.2.2.2 ���� ���ڸ� �д´�.
					letter = this->GetAt(this->current)->GetAt(index)->GetContent();
				}
				//1.3.2.6 ĳ���� ���� ���� ��ġ�� ���� ������ ���� ��ġ�� ����,
				//���� ���� ���ڰ� �����̽�����(����)�̸�
				if (index == lastPositionOfLetter && letter == " ")
				{
					//1.3.2.6.1 ĳ���� ���� ��ġ�� ���������� �̵���Ų��.
					index = this->GetAt(this->current)->Last();
				}
			}
		}
	}
	//2. ĳ���� ���� ������ġ�� �������� ���ڰ����� �ٸ� ���(���ٳ������� �̵��ϴ� ���)
	else
	{
		//2.1 ���� ���� WordNext�� ȣ���Ѵ�.
		index = this->GetAt(this->current)->NextWord();
	}
	//3. ĳ���� ���� ���� ��ġ�� ����Ѵ�.
	return index;
}

//��Ʈ���� ĳ���� �ܾ������ �������� �̵���Ű��
Long Note::PreviousWord()
{
	string letter;//���ڸ� ���� ����
	//1. ���� ĳ���� ��ġ�� ���Ѵ�.
	Long index = this->GetAt(this->current)->GetCurrent();
	//2. ���� ĳ���� ��ġ�� 0���� ũ��
	if (index > 0)
	{
		//2.1 ���� ���� WordPrevious�� ȣ���Ѵ�.
		index = this->GetAt(this->current)->PreviousWord();
		//2.2 index�� 0�̸�
		if (index == 0)
		{
			//2.2.1 ���� ���� ���� ù��° ���ڸ� �д´�.
			letter = this->GetAt(this->current)->GetAt(index)->GetContent();
			//2.2.2 ���� ���� ù��° ���ڰ� �����̽��� �ǹ��ڰ� �ƴϸ�
			//(�ѱ��̳� ����, Ư������, �����̸�)
			if (letter != "\t" && letter != " ")
			{
				//2.2.2.1 ���� ĳ���� ���� ��ġ�� 0���� �̵���Ų��.
				index = this->GetAt(this->current)->First();
			}
			//2.2.3 ���� ���� ù��° ���ڰ� �����̽� �Ǵ� �ǹ����̸�
			else
			{
				//2.2.3.1 ���� ���� ��ġ�� 1��ŭ ���ҽ�Ų��.(���� �ٷ� �̵���Ų��.)
				this->current--;
				//2.2.3.2 ���� ���� ��ġ�� underflow�̸�
				if (this->current < 0)
				{
					//2.2.3.2.1 ���� ���� ��ġ�� �ּҰ����� �����Ѵ�.
					this->current = 0;
				}
				//2.2.3.3 ���� ���� ��ġ�� underflow�� �ƴϸ�(this->current >=0)
				else
				{
					//2.2.3.3.1 ���� ĳ���� ���� ��ġ�� ���������� �̵���Ų��.
					index = this->GetAt(this->current)->Last();
				}
			}
		}
	}
	//3. �̵��ϱ��� ĳ���� ���� ��ġ�� 0�̸�
	else if (index == 0)
	{
		//3.1 ĳ���� ���� ���� ��ġ�� 1��ŭ ���ҽ�Ų��.(���� �ٷ� �̵���Ų��.)
		this->current--;
		//3.2 ĳ���� ���� ���� ��ġ�� underflow�̸�
		if (this->current < 0)
		{
			//3.2.1 ĳ���� ���� ���� ��ġ�� �ּҰ����� �����Ѵ�.
			this->current = 0;
		}
		//3.3 ĳ���� ���� ���� ��ġ�� underflow�� �ƴϸ�(this->current >=0)
		else
		{
			//3.3.1 ���� ĳ���� ���� ��ġ�� ���������� �̵���Ų��.
			index = this->GetAt(this->current)->Last();
		}
	}
	return index;
}

void Note::CalculateSelectedRange(Long* startingRowPos, Long* startingLetterPos,
	Long* endingRowPos, Long* endingLetterPos)
{
	//1. ù��, ùĭ���� ������.
	*startingRowPos = 0;
	*startingLetterPos = 0;
	*endingRowPos = 0;
	*endingLetterPos = 0;
	Long rowIndex = 0;
	Long letterIndex = 0;
	Long letterCount = 0;
	Long rowCount = this->length;
	Glyph* row = 0;
	Glyph* letter = 0;
	bool isSelected = false;
	//2. ���� ��ġ�� ���� �������� �������� �׸��� false�ε��� �ݺ��Ѵ�.
	while (rowIndex < rowCount && isSelected == false)
	{
		//2.1 ���� ���ڰ����� ���Ѵ�.
		letterCount = this->GetAt(rowIndex)->GetLength();
		//2.2 ���� ��ġ�� ����ġ��Ų��.
		letterIndex = 0;
		//2.3 ���� ���Ѵ�.
		row = this->GetAt(rowIndex);
		//2.4 ������ġ�� ���ڰ������� �������� �׸��� false�ε��� �ݺ��Ѵ�.
		while (letterIndex < letterCount && isSelected == false)
		{
			//2.4.1 ���ڸ� ���Ѵ�.
			letter = row->GetAt(letterIndex);
			//2.4.2 ���ڰ� ���õǾ� ������
			if (letter->IsSelected() == true)
			{
				//2.4.2.1 isSelected�� true�� �ٲ۴�.
				isSelected = true;
				//2.4.2.2 ���� ���� ��ġ�� �����Ѵ�.
				*startingRowPos = rowIndex;
				//2.4.2.3 ���� ������ ��ġ�� �����Ѵ�.
				*startingLetterPos = letterIndex;
			}
			//2.4.3 ���� ��ġ�� ������Ų��.
			letterIndex++;
		}
		//2.5 ���� ��ġ�� ������Ų��.
		rowIndex++;
	}
	//3. ���� �ٰ� ���ڸ� ����ġ�����ش�.
	rowIndex = *startingRowPos;
	Long index = *startingLetterPos;
	//3. ���� ��ġ�� ���� �������� �������� �׸��� true�ε��� �ݺ��Ѵ�.
	while (rowIndex < rowCount && isSelected == true)
	{
		//3.1 ���� ���Ѵ�.
		row = this->GetAt(rowIndex);
		//3.2 ���� ���ڰ����� ���Ѵ�.
		letterCount = row->GetLength();
		//3.3 ���� ��ġ�� ����ġ��Ų��.
		letterIndex = index;
		//3.4 ������ġ�� ���ڰ������� �������� �׸��� true�ε��� �ݺ��Ѵ�.
		while (letterIndex < letterCount && isSelected == true)
		{
			//3.4.1 ���ڸ� ���Ѵ�.
			letter = row->GetAt(letterIndex);
			//3.4.2 ���ڰ� ���õǾ� ������
			if (letter->IsSelected() == true)
			{
				//3.4.2.1 ������ ���� ��ġ�� �����Ѵ�.
				*endingRowPos = rowIndex;
				//3.4.2.2 ������ ������ ��ġ�� �����Ѵ�.
				*endingLetterPos = letterIndex;
			}
			//3.4.3 ���ڰ� ���õǾ� ���� ������
			else
			{
				//3.4.3.1 isSelected�� false�� �ٲ۴�.
				isSelected = false;
			}
			//3.4.3 ���� ��ġ�� ������Ų��.
			letterIndex++;
		}
		//3.5 ���� ��ġ�� ������Ų��.
		rowIndex++;
		index = 0;
	}
}