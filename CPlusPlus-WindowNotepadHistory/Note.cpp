#include "Note.h"
#include "Letter.h"
#include "GlyphVisitor.h"
#include "SingleByteLetter.h"
#include "DoubleByteLetter.h"
#include "DummyRow.h"

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
	//1. ������ �������� �ݺ��Ѵ�.
	string content;
	Long rowIndex = 0;
	Long lastRowIndex = this->length - 1;
	while (rowIndex < lastRowIndex)
	{
		content += this->glyphs.GetAt(rowIndex)->GetContent();
		content += '\n';
		rowIndex++;
	}
	//2. ���������� ó���Ѵ�.
	//(������ �ٸ� ���� ó���ϴ� ������ ������ ���� ���� ó�����ϰ� ���� �ݺ����� �־��ָ�
	//���๮�ڰ� ���� �ԷµǼ� ������ �� ������ �� ���� ������� �����̴�.)
	if (rowIndex == lastRowIndex)
	{
		content += this->glyphs.GetAt(rowIndex)->GetContent();
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

//�޸��忡�� �ڵ������� ���� ���� ���¿��� ĳ���� �ܾ������ ���������� �̵���Ű��
void Note::NextWordOnRowAutoChange(Long currentRowIndex, Long currentLetterIndex)
{
	//1. �޸����� ���� �ٿ��� ó�� ���ں��� ���� ���ڱ����� content�� ���Ѵ�.
	Long i = 0;
	Glyph* letter = 0;
	string letterContent = "";
	string currentLetters = "";
	Glyph* row = this->GetAt(currentRowIndex);
	while (i < currentLetterIndex)
	{
		//1.1 ���ڸ� ���Ѵ�.
		letter = row->GetAt(i);
		letterContent = letter->GetContent();
		//1.2 ���� content�� �����ش�.
		currentLetters += letterContent;
		i++;
	}
	//2. �޸����� ���� �ٿ��� ó�����ں��� ���� ���ڱ����� content�� ���̸� ���Ѵ�.
	Long currentLettersLength = currentLetters.length();
	//3. ���� ���� ó�����ں��� ���������ڱ����� content�� ���Ѵ�.
	i = 0;
	string realRowContent = "";
	while (i < row->GetLength())
	{
		//3.1 ���ڸ� ���Ѵ�.
		letter = row->GetAt(i);
		letterContent = letter->GetContent();
		//3.2 ���� content�� �����ش�.
		realRowContent += letterContent;
		i++;
	}
	//4. ���� ���� ���Ѵ�.
	Long nextRowIndex = currentRowIndex + 1;
	Glyph* nextRow = this->GetAt(nextRowIndex);
	//5. ���� ���� ��ġ�� ���� �������� �������� �׸��� ���� ���� ��¥ ���� ���� �ݺ��Ѵ�.
	string dummyRowContent = "";
	while (nextRowIndex < this->GetLength() &&
		dynamic_cast<DummyRow*>(nextRow))
	{
		//5.1 ��¥���� content�� ���Ѵ�.
		i = 0;
		dummyRowContent = "";
		while (i < nextRow->GetLength())
		{
			//5.6.1.1 ���ڸ� ���Ѵ�.
			letter = nextRow->GetAt(i);
			letterContent = letter->GetContent();
			//5.6.1.2 ���� content�� �����ش�.
			dummyRowContent += letterContent;
			i++;
		}
		//5.2 ��¥���� content�� ��¥ ���� content�� �����ش�.
		realRowContent += dummyRowContent;
		//5.3 ���� �ٷ� �̵��Ѵ�.
		nextRowIndex++;
		//5.4 ���� ���� ���Ѵ�.
		nextRow = this->GetAt(nextRowIndex);
	}
	//6. ��¥ ���� ���� ��¥ ���� content ���̸� ���Ѵ�.
	Long rowContentLength = realRowContent.length();
	Long rowLastContentLength = rowContentLength - 1;
	i = currentLettersLength;
	Long j = 0;
	//7. ���� ���� ��ġ�� ���� ���� ������ ���� ��ġ��(���� ���� ��¥ �� �Ǵ� ��¥ �� ������
	//���� ���� ������ ��¥ ����. �׷��� ���� ���� ������ ���� ��ġ�� ������ ���� ��(��¥ ��)�� �̵���.)
	if (i == rowContentLength)
	{
		//7.1 ���� �ٷ� �̵��Ѵ�.
		currentRowIndex++;
		//7.2 ���� ���� ó��������ġ�� �̵��Ѵ�.
		currentLetterIndex = 0;
		//7.3 ���� ���Ѵ�.
		row = this->GetAt(currentRowIndex);
		//7.4 ���� content�� ���Ѵ�.
		j = 0;
		realRowContent = "";
		while (j < row->GetLength())
		{
			//7.4.1 ���ڸ� ���Ѵ�.
			letter = row->GetAt(j);
			letterContent = letter->GetContent();
			//7.4.2 ���� content�� �����ش�.
			realRowContent += letterContent;
			j++;
		}
		//7.5 ���� ���� ���Ѵ�.
		nextRowIndex = currentRowIndex + 1;
		nextRow = this->GetAt(nextRowIndex);
		//7.6 ���� ���� ��ġ�� ���� �������� �������� �׸��� ���� ���� ��¥ ���� ���� �ݺ��Ѵ�.
		dummyRowContent = "";
		while (nextRowIndex < this->GetLength() &&
			dynamic_cast<DummyRow*>(nextRow))
		{
			//7.6.1 ��¥���� content�� ���Ѵ�.
			j = 0;
			dummyRowContent = "";
			while (j < nextRow->GetLength())
			{
				//7.6.1.1 ���ڸ� ���Ѵ�.
				letter = nextRow->GetAt(j);
				letterContent = letter->GetContent();
				//7.6.1.2 ���� content�� �����ش�.
				dummyRowContent += letterContent;
				j++;
			}
			//7.8.2 ��¥���� content�� ��¥ ���� content�� �����ش�.
			realRowContent += dummyRowContent;
			//7.8.3 ���� �ٷ� �̵��Ѵ�.
			nextRowIndex++;
			//7.8.4 ���� ���� ���Ѵ�.
			nextRow = this->GetAt(nextRowIndex);
		}
		//7.7 ���� content ���̸� ���Ѵ�.
		rowContentLength = realRowContent.length();
		i = 0;
		letterContent = realRowContent[i];//�ѱ��ڸ� ���� ����
		//7.8 ���� ���ڰ� �ǹ��ڰų� �����̽������̸�
		if (letterContent == "\t" || letterContent == " ")
		{
			//7.8.1 ������ ���� ��ġ�� ���Ѵ�.
			rowLastContentLength = rowContentLength - 1;
			//7.8.2 ������ ������ġ���� �������� �׸��� �ǹ����ε��� �ݺ��Ѵ�.
			while (i < rowLastContentLength && letterContent == "\t")
			{
				//7.8.2.1 ���� ������ġ�� ������Ų��.
				currentLetterIndex++;
				i++;
				//7.8.2.2 ���ڸ� �����Ѵ�.
				letterContent = realRowContent[i];
			}
			//7.8.3 ������ ������ġ�� ���� �ǹ����̸�
			if (i == rowLastContentLength && letterContent == "\t")
			{
				//7.8.3.1 ���� ������ġ�� ������Ų��.
				currentLetterIndex++;
				i++;
			}
			//7.8.4 ������ ������ġ���� �������� �׸��� �����̽������ε��� �ݺ��Ѵ�.
			while (i < rowLastContentLength && letterContent == " ")
			{
				//7.8.4.1 ���� ������ġ�� ������Ų��.
				currentLetterIndex++;
				i++;
				//7.8.4.2 ���ڸ� �����Ѵ�.
				letterContent = realRowContent[i];
			}
			//7.8.5 ������ ������ġ�� ���� �����̽������̸�
			if (i == rowLastContentLength && letterContent == " ")
			{
				//7.8.5.1 ���� ������ġ�� ������Ų��.
				currentLetterIndex++;
				i++;
			}
		}
	}
	//8. �װ� �ƴϸ�
	else
	{
		//8.1 �ѱ��̸�
		if ((realRowContent[i] & 0x80))//�ѱ��̸�(2byte����)
		{
			//8.1.1 2byte�� �����Ѵ�.
			letterContent = realRowContent[i];
			i++;
			letterContent += realRowContent[i];
		}
		//8.2 �ѱ��� �ƴϸ�
		else
		{
			//8.2.1 1byte�� �����Ѵ�
			letterContent = realRowContent[i];
		}
		//8.3 ������ ������ġ���� �������� �׸��� �ǹ��ڿ� �����̽����ڰ� �ƴѵ��� �ݺ��Ѵ�.
		while (i < rowLastContentLength && letterContent != " " && letterContent != "\t")
		{
			//8.3.1 ���� ������ġ�� ������Ų��.
			currentLetterIndex++;
			i++;
			//8.3.2 �ٿ��� i��° ���� ���ڰ� �ѱ��̸�
			if ((realRowContent[i] & 0x80))//�ѱ��̸�(2byte����)
			{
				//8.3.2.1 2byte�� �����Ѵ�.
				letterContent = realRowContent[i];
				i++;
				letterContent += realRowContent[i];
			}
			//8.3.3 �ѱ��� �ƴϸ�
			else
			{
				//8.3.3.1 1byte�� �����Ѵ�
				letterContent = realRowContent[i];
			}
		}
		//8.4 ������ ������ġ�� ����, �ǹ��ڿ� �����̽� ���ڰ� �ƴϸ�
		if (i == rowLastContentLength && letterContent != " " && letterContent != "\t")
		{
			//8.4.1 ���� ������ġ�� ������Ų��.
			currentLetterIndex++;
			i++;
		}
		//8.5 ������ ������ġ���� �������� �׸��� �ǹ����ε��� �ݺ��Ѵ�.
		while (i < rowLastContentLength && letterContent == "\t")
		{
			//8.5.1 ���� ������ġ�� ������Ų��.
			currentLetterIndex++;
			i++;
			//8.5.2 ���ڸ� �����Ѵ�.
			letterContent = realRowContent[i];
		}
		//8.6 ������ ������ġ�� ���� �ǹ����̸�
		if (i == rowLastContentLength && letterContent == "\t")
		{
			//8.6.1 ���� ������ġ�� ������Ų��.
			currentLetterIndex++;
			i++;
		}
		//8.7 ������ ������ġ���� �������� �׸��� �����̽������ε��� �ݺ��Ѵ�.
		while (i < rowLastContentLength && letterContent == " ")
		{
			//8.7.1 ���� ������ġ�� ������Ų��.
			currentLetterIndex++;
			i++;
			//8.7.2 ���ڸ� �����Ѵ�.
			letterContent = realRowContent[i];
		}
		//8.8 ������ ������ġ�� ���� �����̽������̸�
		if (i == rowLastContentLength && letterContent == " ")
		{
			//8.8.1 ���� ������ġ�� ������Ų��.
			currentLetterIndex++;
			i++;
		}
	}
	//9. ���� ���� ���Ѵ�.
	Glyph* currentRow = this->GetAt(currentRowIndex);
	//10. ���� ���� content�� ���Ѵ�.
	j = 0;
	string currentRowContent = "";
	while (j < currentRow->GetLength())
	{
		//10.1 ���ڸ� ���Ѵ�.
		letter = currentRow->GetAt(j);
		letterContent = letter->GetContent();
		//10.2 ���� content�� �����ش�.
		currentRowContent += letterContent;
		j++;
	}
	//11. ���� ���� content�� length�� ���Ѵ�.
	Long currentRowContentLength = currentRowContent.length();
	//12. i�� ���� ���� content�� length���� ũ�ų� �������� �ݺ��Ѵ�.
	//�Ʊ� �տ��� �κ��ڵ������� ����ϴ� ��ó�� ��¥���� content���� ��¥���� content��
	//�߰��������� ��ǻ� ���� �޸��忡���� �ڵ������� �Ǿ� �ִ� �����̱� ������
	//�ڵ������� �� �޸��忡�� ã�� ���ڿ��� ã�Ƽ� ������ �ؾ��ϱ� ������ �Ʊ� ���ƴ� ������ 
	//��¥�ٵ��� �ٽ� ���������� ���ؼ� ��ģ �ٿ��� ���� ������ġ i�� ���ؼ� ���� ���� ��ġ��
	//�ٽ� �����ش�.
	string nextRowContent = "";
	while (i > currentRowContentLength)
	{
		//12.1 ���� ���� ��ġ�� ������Ų��.
		//�ڵ������� ��ҵ� ���¿��� �ش� ���ڿ��� ã�Ҵµ� ���������� ���� ���ڰ�
		//���� �������� ���� ���̺��� ��� ������ ���� �ٷ� �Ѿ�� �ٽ� ���ؾ��Ѵ�.
		currentRowIndex++;
		//12.2 ���� ������ġ�� �ڵ������� ��ҵ� ������ ������ġ�� ������ ���� ���� 
		//��ġ�� ���� ���� �������� ũ�� ������ �׸�ŭ ���� �ڵ������� �� ���·� �����Ѵ�
		//�տ��� currentLetterIndex++�� ���� 1�� ������ �����̱� ������ +1�� ���� �����൵��!
		currentLetterIndex -= currentRow->GetLength();
		//12.3 ���� ���� �������� ���Ѵ�.
		currentRow = this->GetAt(currentRowIndex);
		//12.4 ���� ���� content�� �����Ѵ�.
		j = 0;
		nextRowContent = "";
		while (j < currentRow->GetLength())
		{
			//12.4.1 ���ڸ� ���Ѵ�.
			letter = currentRow->GetAt(j);
			letterContent = letter->GetContent();
			//12.4.2 ���� content�� �����ش�.
			nextRowContent += letterContent;
			j++;
		}
		//12.5 ���� ���� content�� ���� ���� content�� �����ش�.(����)
		currentRowContent += nextRowContent;
		//12.6 ������ ���� content�� ���̸� ���Ѵ�.
		currentRowContentLength = currentRowContent.length();
	}
	//13. ���� ���� ��ġ�� ������ġ�� �ٽ� �������ش�.
	currentRowIndex = this->Move(currentRowIndex);
	currentRow = this->GetAt(currentRowIndex);
	currentLetterIndex = currentRow->Move(currentLetterIndex);
	//14. ���� ���� ������ ���� �ƴϸ�
	Long lastRowIndex = this->GetLength() - 1;
	if (currentRowIndex < lastRowIndex)
	{
		//14.1 ���� ���� ���Ѵ�.
		nextRow = this->GetAt(currentRowIndex + 1);
		//14.2 ���� ���� ��¥ ���̸�
		if (dynamic_cast<DummyRow*>(nextRow))
		{
			//14.2.1 ���� ������ġ�� ������ �������� ������
			Long letterIndex = currentRow->GetLength();
			if (currentLetterIndex == letterIndex)
			{
				//14.2.1.1 ���� �ٷ� �̵���Ų��.
				currentRowIndex = this->Next();
				//14.2.1.2 ���� ���� �������� �̵��� �ٷ� �����Ѵ�.
				currentRow = this->GetAt(currentRowIndex);
				//14.2.1.3 ĳ���� ���� ��ġ�� ó������ ������.(���� ���� ���� �ٷ� �Ű����� ������
				//ĳ���� ���� ���� ������ ��ġ���� ���� ���� ó�� ��ġ�� �̵��Ѵ�.)
				currentLetterIndex = currentRow->First();
			}
		}
	}
}

//�޸��忡�� �ڵ������� ���� ���� ���¿��� ĳ���� �ܾ������ �������� �̵���Ű��
void Note::PreviousWordOnRowAutoChange(Long currentRowIndex, Long currentLetterIndex)
{
	//1. �޸����� ���� �ٿ��� ó�� ���ں��� ���� ���ڱ����� content�� ���Ѵ�.
	Long i = 0;
	Glyph* letter = 0;
	string letterContent = "";
	string currentLetters = "";
	Glyph* row = this->GetAt(currentRowIndex);
	while (i < currentLetterIndex)
	{
		//1.1 ���ڸ� ���Ѵ�.
		letter = row->GetAt(i);
		letterContent = letter->GetContent();
		//1.2 ���� content�� �����ش�.
		currentLetters += letterContent;
		i++;
	}
	//2. �޸����� ���� �ٿ��� ó�����ں��� ���� ���ڱ����� content�� ���̸� ���Ѵ�.
	Long currentLettersLength = currentLetters.length();
	//3. ���� content�� ���Ѵ�.
	i = 0;
	string rowContent = "";
	while (i < currentLetterIndex)
	{
		//3.1 ���ڸ� ���Ѵ�.
		letter = row->GetAt(i);
		letterContent = letter->GetContent();
		//3.2 ���� content�� �����ش�.
		rowContent += letterContent;
		i++;
	}
	//4. ���� ���� ��¥ ���̸�(���� ���� ��ġ�� 0�̸� ��¥ ���� �� X)
	if (dynamic_cast<DummyRow*>(row))
	{
		//4.1 ���� �ٷ� �̵��Ѵ�.
		Long previousRowIndex = currentRowIndex - 1;
		//4.2 ���� ���� ���Ѵ�
		Glyph* previousRow = this->GetAt(previousRowIndex);
		//4.3 ���� ���� ��¥���ε��� �ݺ��Ѵ�.
		string previousRowContent = "";
		while (dynamic_cast<DummyRow*>(previousRow))
		{
			//4.3.1 ���� ���� content�� ���Ѵ�.
			i = 0;
			previousRowContent = "";
			while (i < previousRow->GetLength())
			{
				//4.3.1.1 ���ڸ� ���Ѵ�.
				letter = previousRow->GetAt(i);
				letterContent = letter->GetContent();
				//4.3.1.2 ���� content�� �����ش�.
				previousRowContent += letterContent;
				i++;
			}
			//4.3.2 ���� ���� content�� ���� ���� cotent�� �����ش�.(����)
			previousRowContent += rowContent;
			//4.3.3 ������ ���� �Ű� �����Ѵ�.(�ݺ� �������� ����� ������Ű�� ���ؼ�)
			rowContent = previousRowContent;
			//4.3.4 �޸����� ���� ������ġ�� ���� ���� ���ڰ����� �����ش�.
			currentLetterIndex += previousRow->GetLength();
			//4.3.5 �������� �̵��Ѵ�.
			previousRowIndex--;
			//4.3.6 ���� ���� ���Ѵ�.
			previousRow = this->GetAt(previousRowIndex);
		}
		//�������� �̵����� �� ��¥ ���� ���ͼ� �ݺ������� ���Ա� ������ ���� ���� ���� ��¥ ���̴�.
		//���� ���� ��(��¥ ��)�� content�� ���ؼ� �Ʊ� �ݺ��������� ���ߴ� ��¥���� content�� �����ش�
		//4.4 ��¥ ���� ��ġ�� ���� ���� ��ġ�� �����Ѵ�.
		currentRowIndex = previousRowIndex;
		//4.5 ��¥ ���� content�� ���Ѵ�.
		i = 0;
		previousRowContent = "";
		while (i < previousRow->GetLength())
		{
			//4.5.1 ���ڸ� ���Ѵ�.
			letter = previousRow->GetAt(i);
			letterContent = letter->GetContent();
			//4.5.2 ���� content�� �����ش�.
			previousRowContent += letterContent;
			i++;
		}
		//4.6 ��¥ ���� content�� ������Ų��.
		previousRowContent += rowContent;
		//4.7 ������ ���� �Ű� �����Ѵ�.(�ݺ� �������� ����� ������Ű�� ���ؼ�)
		rowContent = previousRowContent;
		//4.8 �޸����� ���� ������ġ�� ���� ���� ���ڰ����� �����ش�.
		currentLetterIndex += previousRow->GetLength();
		//4.9 ���� ���� ���̸� ���Ѵ�.(�ڵ����� ��ҵ� ������ ������ġ���� ����)
		currentLettersLength = rowContent.length();
	}
	//5. ��¥ ���� ��ġ�� �����Ѵ�.
	Long realRowIndex = currentRowIndex;
	string koreanCompositionTemp;//�������� �Ž��� �ö󰡱� ������ �������� �ѱ��� �����ϸ� �ѱ��� �����
	//�������� �ʱ� ������ �ѱ��� �ӽ÷� �����ϰ� ���� ������� ������ �ӽ� ����Ұ� �ʿ���.
	//6. ���� ���� ��ġ�� 0���� ũ��(���� ���� ��ġ�� 0�̸� �ܾ������ �̵��� �� X)
	if (currentLetterIndex > 0)
	{
		//6.1 �ٿ��� �о�� �� ���� �� ĭ ��ŭ ���ҽ�Ų��.
		i = currentLettersLength - 1;
		//currentLetterIndex--;
		//6.2 �ٿ��� i��° ���� ���ڰ� �ѱ��̸�
		if ((rowContent[i] & 0x80))//�ѱ��̸�(2byte����)
		{
			//6.2.1 2byte�� �����Ѵ�.
			letterContent = rowContent[i];
			//�������� �Ž��� ���� ������ ����� �ѱ��� �����ϱ� ���ؼ� �ӽ������� �ؾ���.
			koreanCompositionTemp = letterContent;
			i--;
			letterContent = rowContent[i];
			letterContent += koreanCompositionTemp;
		}
		//6.3 �ѱ��� �ƴϸ�
		else
		{
			//6.3.1 1byte�� �����Ѵ�
			letterContent = rowContent[i];
		}
		//6.4 ���� ���� ��ġ�� 1���� ũ�� ���� ���ڰ� �����̽�(����)�����ε��� �ݺ��Ѵ�.
		while (currentLetterIndex > 1 && letterContent == " ")
		{
			//6.4.1 ���� ������ġ�� ���ҽ�Ų��.
			currentLetterIndex--;
			i--;
			//6.4.2 ���ڸ� �����Ѵ�.
			letterContent = rowContent[i];
		}
		//6.5 ���� ���� ��ġ�� 1�̰� ���� ���ڰ� �����̽�(����)�����̸�
		if (currentLetterIndex == 1 && letterContent == " ")
		{
			//6.5.1 ���� ������ġ�� ���ҽ�Ų��.
			currentLetterIndex--;
			i--;
		}
		//6.6 ���� ���� ��ġ�� 1���� ũ�� ���� ���ڰ� �ǹ����ε��� �ݺ��Ѵ�.
		while (currentLetterIndex > 1 && letterContent == "\t")
		{
			//6.6.1 ���� ������ġ�� ���ҽ�Ų��.
			currentLetterIndex--;
			i--;
			//6.6.2 ���ڸ� �����Ѵ�.
			letterContent = rowContent[i];
		}
		//6.7 ���� ���� ��ġ�� 1�̰� ���� ���ڰ� �ǹ����̸�
		if (currentLetterIndex == 1 && letterContent == "\t")
		{
			//6.7.1 ���� ������ġ�� ���ҽ�Ų��.
			currentLetterIndex--;
			i--;
		}
		//6.8 ���� ���� ��ġ�� 1���� ũ�� ���� ���ڰ� �����̽��� �ǹ��ڰ� �ƴѵ��� �ݺ��Ѵ�.
		if (currentLetterIndex > 1 && letterContent != " " && letterContent != "\t")
		{
			//6.8.1 ���� ���� ���ڰ� �ѱ��̸�
			if ((rowContent[i] & 0x80))//�ѱ��̸�(2byte����)
			{
				//6.8.1 2byte�� �����Ѵ�.
				letterContent = rowContent[i];
				//�������� �Ž��� ���� ������ ����� �ѱ��� �����ϱ� ���ؼ� �ӽ������� �ؾ���.
				koreanCompositionTemp = letterContent;
				i--;
				letterContent = rowContent[i];
				letterContent += koreanCompositionTemp;
			}
			//6.8.2 �ѱ��� �ƴϸ�
			else
			{
				//6.8.2.1 1byte�� �����Ѵ�
				letterContent = rowContent[i];
			}
		}
		//6.9 ���� ���� ��ġ�� 1���� ũ�� ���� ���ڰ� �����̽�(����)���� �׸���
		//�ǹ��ڰ� �ƴѵ��� �ݺ��Ѵ�.
		while (currentLetterIndex > 1 && letterContent != " " && letterContent != "\t")
		{
			//6.9.1 ���� ���� ��ġ�� ���ҽ�Ų��.
			currentLetterIndex--;
			i--;
			//6.9.2 ���� ���� ���ڰ� �ѱ��̸�
			if ((rowContent[i] & 0x80))//�ѱ��̸�(2byte����)
			{
				//6.9.2.1 2byte�� �����Ѵ�.
				letterContent = rowContent[i];
				//�������� �Ž��� ���� ������ ����� �ѱ��� �����ϱ� ���ؼ� �ӽ������� �ؾ���.
				koreanCompositionTemp = letterContent;
				i--;
				letterContent = rowContent[i];
				letterContent += koreanCompositionTemp;
			}
			//6.9.3 �ѱ��� �ƴϸ�
			else
			{
				//6.9.3.1 1byte�� �����Ѵ�
				letterContent = rowContent[i];
			}
		}
		//6.10 ���� ĳ���� ���� ��ġ�� 1�̰� ���� ���ڰ� �����̽�(����) ���ڿ� �ǹ��ڰ� �ƴϸ�
		if (currentLetterIndex == 1 && letterContent != " " && letterContent != "\t")
		{
			//6.10.1 ���� ���� ���ڰ� �ѱ��̸�
			if ((rowContent[i] & 0x80))//�ѱ��̸�(2byte����)
			{
				//6.10.1.1 2byte�� �����Ѵ�.
				letterContent = rowContent[i];
				//�������� �Ž��� ���� ������ ����� �ѱ��� �����ϱ� ���ؼ� �ӽ������� �ؾ���.
				koreanCompositionTemp = letterContent;
				i--;
				letterContent = rowContent[i];
				letterContent += koreanCompositionTemp;
			}
			//6.10.2 �ѱ��� �ƴϸ�
			else
			{
				//6.10.2.1 1byte�� �����Ѵ�
				letterContent = rowContent[i];
			}
			//6.10.3 ���� ���� ��ġ�� ���ҽ�Ų��.
			currentLetterIndex--;
			i--;
		}
		//6.11 ���� ���� ��ġ�� 0�̸�
		if (currentLetterIndex == 0)
		{
			//6.11.1 ���ڸ� �����Ѵ�.
			//letterContent = rowContent[i];
			//6.11.2 ���� ���� ù��° ���ڰ� �����̽� �Ǵ� �ǹ����̸�
			if (letterContent == "\t" || letterContent == " ")
			{
				//6.11.2.1 ���� ���� ��ġ�� 1��ŭ ���ҽ�Ų��.(���� �ٷ� �̵���Ų��.)
				currentRowIndex--;
				//6.11.2.2 ���� ���� ��ġ�� underflow�̸�
				if (currentRowIndex < 0)
				{
					//6.11.2.2.1 ���� ���� ��ġ�� �ּҰ����� �����Ѵ�.
					currentRowIndex = 0;
				}
				//6.11.2.3 ���� ���� ��ġ�� underflow�� �ƴϸ�(this->current >=0)
				else
				{
					//6.11.2.3.1 ���� ĳ���� ���� ��ġ�� ���������� �̵���Ų��.
					currentRowIndex = this->Move(currentRowIndex);
					row = this->GetAt(currentRowIndex);
					currentLetterIndex = row->Last();
				}
			}
		}
	}
	//7. ���� ���� ��ġ�� ���� ó���̸�(0�̸�)
	else if (currentLetterIndex == 0)
	{
		//7.1 ���� ���� ��ġ�� 1��ŭ ���ҽ�Ų��.(���� �ٷ� �̵���Ų��.)
		currentRowIndex--;
		//7.2 ���� ���� ��ġ�� underflow�̸�
		if (currentRowIndex < 0)
		{
			//7.2.1 ���� ���� ��ġ�� �ּҰ����� �����Ѵ�.
			currentRowIndex = 0;
		}
		//7.3 ���� ���� ��ġ�� underflow�� �ƴϸ�(this->current >=0)
		else
		{
			//7.3.1 ���� ĳ���� ���� ��ġ�� ���������� �̵���Ų��.
			currentRowIndex = this->Move(currentRowIndex);
			row = this->GetAt(currentRowIndex);
			currentLetterIndex = row->Last();
		}
	}
	//8. �޸��忡�� ���� ���� ���Ѵ�.(��¥ ��)
	Glyph* currentRow = this->GetAt(currentRowIndex);
	//9. ���� ���� content�� ���Ѵ�.
	Long k = 0;
	string currentRowContent = "";
	while (k < currentRow->GetLength())
	{
		//9.1 ���ڸ� ���Ѵ�.
		letter = currentRow->GetAt(k);
		letterContent = letter->GetContent();
		//9.2 ���� content�� �����ش�.
		currentRowContent += letterContent;
		k++;
	}
	//10. ���� ���� length�� ���Ѵ�.
	Long currentRowContentLength = currentRowContent.length();
	//11. i�� ���� ���� length���� ũ�ų� �������� �ݺ��Ѵ�.
	while (i >= currentRowContentLength)
	{
		//11.1 ���� ���� ���̸� ����.
		i -= currentRowContentLength;
		//11.2 ���� ���� ������ ����.
		currentLetterIndex -= currentRow->GetLength();
		//11.3 ���� �ٷ� �̵��Ѵ�.
		currentRowIndex++;
		//11.4 ���� ���� ���Ѵ�.
		currentRow = this->GetAt(currentRowIndex);
		//11.5 ���� ���� content�� ���Ѵ�.
		k = 0;
		currentRowContent = "";
		while (k < currentRow->GetLength())
		{
			//11.5.1 ���ڸ� ���Ѵ�.
			letter = currentRow->GetAt(k);
			letterContent = letter->GetContent();
			//11.5.2 ���� content�� �����ش�.
			currentRowContent += letterContent;
			k++;
		}
		//11.6 ���� ���� content�� length�� ���Ѵ�.
		currentRowContentLength = currentRowContent.length();
	}
	//12. ���� ���� ��ġ�� ������ġ�� �ٽ� �������ش�.
	currentRowIndex = this->Move(currentRowIndex);
	currentRow = this->GetAt(currentRowIndex);
	currentLetterIndex = currentRow->Move(currentLetterIndex);
	//13. ���� ���� ��ġ�� ���� ���� ������ ���� ��ġ�̸�
	if (currentLetterIndex == currentRow->GetLength())
	{
		//13.1 ���� ���� ���Ѵ�.
		Glyph* nextRow = this->GetAt(currentRowIndex + 1);
		//13.2 ���� ���� ��¥ ���̰� ���� ���� ��¥ ���̸�
		if (!dynamic_cast<DummyRow*>(nextRow) && dynamic_cast<DummyRow*>(currentRow))
		{
			//13.2.1 ���� �������� �̵��߱� ������ ĳ���� ���� ��ġ�� ���������� �����Ѵ�.
			currentLetterIndex = currentRow->Last();
		}
		//13.3 ���� ���� ��¥ ���̰� ���� ���� ��¥ ���̸�
		else if (!dynamic_cast<DummyRow*>(nextRow) && !dynamic_cast<DummyRow*>(currentRow))
		{
			//13.3.1 ���� �������� �̵��߱� ������ ĳ���� ���� ��ġ�� ���������� �����Ѵ�.
			currentLetterIndex = currentRow->Last();
		}
		//13.4 �� �̿ܿ���
		else
		{
			//13.4.1 ���� ���� ��ġ�� ������ġ�� �ٽ� �������ش�.
			currentRowIndex = this->Move(currentRowIndex + 1);
			currentRow = this->GetAt(currentRowIndex);
			currentLetterIndex = currentRow->First();
		}
	}
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
			//�ʿ����
			//2.2.2 ���� ���� ù��° ���ڰ� �����̽��� �ǹ��ڰ� �ƴϸ�(�̹� ��ġ�� 0�̹Ƿ� �̵����ϸ��)
			//(�ѱ��̳� ����, Ư������, �����̸�)
			//if (letter != "\t" && letter != " ")
			//{
				//2.2.2.1 ���� ĳ���� ���� ��ġ�� 0���� �̵���Ų��.
			//	index = this->GetAt(this->current)->First();
			//}
			//2.2.3 ���� ���� ù��° ���ڰ� �����̽� �Ǵ� �ǹ����̸�
			//else
			if (letter == "\t" || letter == " ")
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
		//2.1 ���� ���Ѵ�.
		row = this->GetAt(rowIndex);
		//2.2 ���� ���ڰ����� ���Ѵ�.
		letterCount = row->GetLength();
		//2.2 ���� ��ġ�� ����ġ��Ų��.
		letterIndex = 0;
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

//Visitor Pattern
void Note::Accept(GlyphVisitor* glyphVisitor)
{
	glyphVisitor->VisitNote(this);
}