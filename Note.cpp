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
		//currentRowIndex++;
		Long previousRowIndex = currentRowIndex;
		currentRowIndex = this->Next();
		if (previousRowIndex < currentRowIndex)
		{
			//7.2 ���� ���� ó��������ġ�� �̵��Ѵ�.
			currentLetterIndex = 0;
		}
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
	//3. ���� ���� ��ü content�� ���Ѵ�.
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
		//�ѱ��̳� ����, Ư������ ���� ���̳� �����̽��� �ִ��� ����
		bool isThereTabOrSpaceBeforeChar = false;
		//�ѱ��̳� ����, Ư�����ڿ��� �̵��� �־����� ����
		bool isThereMovingOnChar = false;
		//6.1 �ٿ��� �о�� �� ���� �� ĭ ��ŭ ���ҽ�Ų��.
		i = currentLettersLength - 1;
		currentLetterIndex--;
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
			isThereTabOrSpaceBeforeChar = true;
			//6.4.1 ���� ������ġ�� ���ҽ�Ų��.
			currentLetterIndex--;
			i--;
			//6.4.2 ���ڸ� �����Ѵ�.
			letterContent = rowContent[i];
		}
		//6.5 ���� ���� ��ġ�� 1�̰� ���� ���ڰ� �����̽�(����)�����̸�
		if (currentLetterIndex == 1 && letterContent == " ")
		{
			isThereTabOrSpaceBeforeChar = true;
			//6.5.1 ���� ������ġ�� ���ҽ�Ų��.
			currentLetterIndex--;
			i--;
		}
		//6.6 ���� ���� ��ġ�� 1���� ũ�� ���� ���ڰ� �ǹ����ε��� �ݺ��Ѵ�.
		while (currentLetterIndex > 1 && letterContent == "\t")
		{
			isThereTabOrSpaceBeforeChar = true;
			//6.6.1 ���� ������ġ�� ���ҽ�Ų��.
			currentLetterIndex--;
			i--;
			//6.6.2 ���ڸ� �����Ѵ�.
			letterContent = rowContent[i];
		}
		//6.7 ���� ���� ��ġ�� 1�̰� ���� ���ڰ� �ǹ����̸�
		if (currentLetterIndex == 1 && letterContent == "\t")
		{
			isThereTabOrSpaceBeforeChar = true;
			//6.7.1 ���� ������ġ�� ���ҽ�Ų��.
			currentLetterIndex--;
			i--;
		}
		//6.8 ���� ���� ��ġ�� 1���� ũ�� ���� ���ڰ� �����̽��� �ǹ��ڰ� �ƴϸ�
		if (currentLetterIndex > 1 && letterContent != " " && letterContent != "\t")
		{
			if (isThereTabOrSpaceBeforeChar == false)
			{
				//6.9.1 ���� ���� ��ġ�� ���ҽ�Ų��.
				currentLetterIndex--;
				i--;
			}
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
			isThereMovingOnChar = true;
		}
		//6.9 ���� ���� ��ġ�� 0���� ũ�� ���� ���ڰ� �����̽�(����)���� �׸���
		//�ǹ��ڰ� �ƴѵ��� �ݺ��Ѵ�.
		while (currentLetterIndex > 0 && letterContent != " " && letterContent != "\t")
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
			isThereMovingOnChar = true;
		}
		//6.10 ���� letterContent�� �д´�
		letterContent = rowContent[i];
		//6.11 ���� ���� ��ġ�� 0���� ũ��(���̳� ���� ���ڰ� ��������)
		if (currentLetterIndex > 0)
		{
			//6.11.1 ���̳� ���鹮�� �ڷ� ������ġ�� �ٽ� �Ű��ش�.
			currentLetterIndex++;
			i++;
		}
		//6.12 letterContent�� ���̳� �����̸�
		else if (letterContent == "\t" || letterContent == " ")
		{
			//6.12.1 �ؽ�Ʈ �̵� ���� ���̳� �����̽� �̵��� �־�����
			if (isThereTabOrSpaceBeforeChar == true)
			{
				//6.12.1.1 �ؽ�Ʈ �̵��� �־�����
				if (isThereMovingOnChar == true)
				{
					//6.12.1.1.1 ���̳� ���鹮�� �ڷ� ������ġ�� �ٽ� �Ű��ش�.
					currentLetterIndex++;
					i++;
				}
				//6.12.1.2 �ؽ�Ʈ �̵��� ��������
				else
				{
					//6.12.1.2.1 currentLetterIndex�� 0�̸�
					if (currentLetterIndex == 0)
					{
						//6.12.1.2.1.1 currentRowIndex�� 0���� ũ��
						if (currentRowIndex > 0)
						{
							//6.12.1.2.1.1.1 ���� ĳ���� ���� ��ġ�� ���������� �̵���Ų��.
							currentRowIndex = this->Move(currentRowIndex - 1);
							row = this->GetAt(currentRowIndex);
							currentLetterIndex = row->Last();
						}
					}
				}
			}
			//6.12.2 �ؽ�Ʈ �̵� ���� ���̳� �����̽� �̵��� ��������
			else if (isThereTabOrSpaceBeforeChar == false)
			{
				//6.12.2.1 �ؽ�Ʈ �̵��� �־�����
				if (isThereMovingOnChar == true)
				{
					currentLetterIndex++;
					i++;
				}
				//6.12.2.2 �ؽ�Ʈ �̵��� ��������
				else
				{

					if (currentRowIndex > 0)
					{
						//7.3.1 ���� ĳ���� ���� ��ġ�� ���������� �̵���Ų��.
						currentRowIndex = this->Move(currentRowIndex - 1);
						row = this->GetAt(currentRowIndex);
						currentLetterIndex = row->Last();
					}
				}
				
			}
		}
		//6.11 ���̳� ���鹮�ڰ� �ȳ�������
		
		////6.11 ���� ĳ���� ���� ��ġ�� 1�̰� ���� ���ڰ� �����̽�(����) ���ڿ� �ǹ��ڰ� �ƴϸ�
		//if (currentLetterIndex == 1 && letterContent != " " && letterContent != "\t")
		//{
		//	//6.10.1 ���� ���� ���ڰ� �ѱ��̸�
		//	if ((rowContent[i] & 0x80))//�ѱ��̸�(2byte����)
		//	{
		//		//6.10.1.1 2byte�� �����Ѵ�.
		//		letterContent = rowContent[i];
		//		//�������� �Ž��� ���� ������ ����� �ѱ��� �����ϱ� ���ؼ� �ӽ������� �ؾ���.
		//		koreanCompositionTemp = letterContent;
		//		i--;
		//		letterContent = rowContent[i];
		//		letterContent += koreanCompositionTemp;
		//	}
		//	//6.10.2 �ѱ��� �ƴϸ�
		//	else
		//	{
		//		//6.10.2.1 1byte�� �����Ѵ�
		//		letterContent = rowContent[i];
		//	}
		//	//6.10.3 ���� ���� ��ġ�� ���ҽ�Ų��.
		//	currentLetterIndex--;
		//	i--;
		//	letterContent = rowContent[i];
		//	lastMovingIsChar = true;
		//}
		////6.11 ���� ���� ��ġ�� 0�̸�
		//if (lastMovingIsChar == false && currentLetterIndex == 0)
		//{
		//	//6.11.1 ���ڸ� �����Ѵ�.
		//	//letterContent = rowContent[i];
		//	//6.11.2 ���� ���� ù��° ���ڰ� �����̽� �Ǵ� �ǹ����̸�
		//	if (letterContent == "\t" || letterContent == " ")
		//	{
		//		//6.11.2.1 ���� ���� ��ġ�� 1��ŭ ���ҽ�Ų��.(���� �ٷ� �̵���Ų��.)
		//		currentRowIndex--;
		//		//6.11.2.2 ���� ���� ��ġ�� underflow�̸�
		//		if (currentRowIndex < 0)
		//		{
		//			//6.11.2.2.1 ���� ���� ��ġ�� �ּҰ����� �����Ѵ�.
		//			currentRowIndex = 0;
		//			currentRowIndex = this->Move(currentRowIndex);
		//			row = this->GetAt(currentRowIndex);
		//			currentLetterIndex = row->First();
		//		}
		//		//6.11.2.3 ���� ���� ��ġ�� underflow�� �ƴϸ�(this->current >=0)
		//		else
		//		{
		//			//6.11.2.3.1 ���� ĳ���� ���� ��ġ�� ���������� �̵���Ų��.
		//			currentRowIndex = this->Move(currentRowIndex);
		//			row = this->GetAt(currentRowIndex);
		//			currentLetterIndex = row->Last();
		//		}
		//	}
		//}
		//else if (lastMovingIsChar == false && currentLetterIndex > 0)
		//{
		//	currentLetterIndex++;
		//	i++;
		//}
		//else if (lastMovingIsChar == true && currentLetterIndex == 0)
		//{
		//	if (letterContent == "\t" || letterContent == " ")
		//	{
		//		currentLetterIndex++;
		//		i++;
		//	}
		//}
		

		/*if (letterContent == " " || letterContent == "\t")
		{
			currentLetterIndex++;
		}*/

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
	while (currentRowContentLength > 0 && i >= currentRowContentLength)
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
	//13. ���� ���� ��ġ�� ���� ���� ������ ���� ��ġ�̰� ��Ʈ�� ���� �ּ� 2�� �̻��̸�
	if (currentLetterIndex == currentRow->GetLength() && this->GetLength() > 1)
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
			//2.2.2 ���� ���� ù��° ���ڰ� �����̽� �Ǵ� �ǹ����̸�
			//else
			if (letter == "\t" || letter == " ")
			{
				//2.2.2.1 ���� ���� ��ġ�� 1��ŭ ���ҽ�Ų��.(���� �ٷ� �̵���Ų��.)
				this->current--;
				//2.2.2.2 ���� ���� ��ġ�� underflow�̸�
				if (this->current < 0)
				{
					//2.2.2.2.1 ���� ���� ��ġ�� �ּҰ����� �����Ѵ�.
					this->current = 0;
				}
				//2.2.2.3 ���� ���� ��ġ�� underflow�� �ƴϸ�(this->current >=0)
				else
				{
					//2.2.2.3.1 ���� ĳ���� ���� ��ġ�� ���������� �̵���Ų��.
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

//���ÿ��� �����ϰ� �����
Glyph* Note::CopySelectedTextsAndRemove(Long selectedStartRowPos,
	Long selectedStartLetterPos, Long selectedEndRowPos, Long selectedEndLetterPos)
{
	//������ ������ �����ʹ�����
	Long startRowIndex = 0;//�����ϴ� ���� ��ġ
	Long startLetterIndex = 0;//�����ϴ� ���� ��ġ
	Long endRowIndex = 0;//������ ���� ��ġ
	Long endLetterIndex = 0;//������ ���� ��ġ
	Glyph* letter = 0;//������ �ּҸ� ���� ����
	Long nextRowIndex = 0;//���� ���� �ּ���ġ
	Glyph* startRow = 0;//�����ϴ� ���� �ּҸ� ���� ����
	//1. Note�� �����Ѵ�.
	Glyph* copyNote = new Note();
	//2. Row�� �����Ѵ�.
	Glyph* copyRow = new Row();
	//3. ���� ������ Row�� command�� Note�� �߰��Ѵ�.
	copyNote->Add(copyRow);
	//4. ������ ���۵Ǵ� �ٰ� ������ ������ ���� ������
	//(�� �� �������� ������ �Ǿ� �����Ƿ� ���� �������� �ʰ� ���ڵ鸸 ������)
	if (selectedStartRowPos == selectedEndRowPos)
	{
		//4.1 ������ ���������� ����Ǿ�����
		if (selectedStartLetterPos < selectedEndLetterPos)
		{
			//4.1.1 �����ϴ� ������ġ�� ������ ���۵Ǵ� ������ġ�� ���Ѵ�.
			startLetterIndex = selectedStartLetterPos;
			//4.1.2 ������ ������ġ�� ������ ������ ������ġ�� ���Ѵ�.
			endLetterIndex = selectedEndLetterPos;
			//4.1.3 �����ϴ� ���� ��ġ�� ������ �����ϴ� ���� ��ġ�� ���Ѵ�.
			startRowIndex = selectedStartRowPos;

		}
		//4.2 ������ �������� ����Ǿ�����
		else
		{
			//4.2.1 �����ϴ� ������ġ�� ������ ������ ������ġ�� ���Ѵ�.
			startLetterIndex = selectedEndLetterPos;
			//4.2.2 ������ ������ġ�� ������ �����ϴ� ������ġ�� ���Ѵ�.
			endLetterIndex = selectedStartLetterPos;
			//4.2.3 �����ϴ� ���� ��ġ�� ������ ������ ���� ��ġ�� ���Ѵ�.
			startRowIndex = selectedEndRowPos;
		}
		//4.3 �����ϴ� ���� ���Ѵ�.
		startRow = this->GetAt(startRowIndex);
		//4.4 �����ϴ� ������ġ���� ������ ���ڱ��� �����.
		while (startLetterIndex < endLetterIndex)
		{
			//4.4.1 ���ڸ� ����� ���� ���ڸ� ���Ѵ�.
			letter = startRow->GetAt(startLetterIndex);
			//4.4.2 ���ڸ� ���� �����ؼ� ���� ������ �ٿ� �����Ѵ�.
			copyRow->Add(letter->Clone());
			//4.4.3 �ٿ��� ���ڸ� �����.
			startRow->Remove(startLetterIndex);
			//4.4.4 �ٿ��� ���ڰ� �������� ���� ������ �ٰ� �����ϴ� ������ ���� ���ڰ�
			//������ �����ϴ� ������ ��ġ�� �մ���� ���� �ǹǷ� ������ ������ ���� ���� ���ҽ�Ų��. 
			endLetterIndex--;
		}
	}
	//5. ������ ���۵Ǵ� �ٰ� ������ ������ ���� ���� �ٸ���
	//(������ ���� �ٿ� ���ļ� �Ǿ� �ֱ� ������ ���ڰ� �� ������ ���� ����������)
	else
	{
		//5.1 ������ ���������� ����Ǿ����� 
		if (selectedStartRowPos < selectedEndRowPos)
		{
			//5.1.1 �����ϴ� ������ġ�� ������ ���۵Ǵ� ������ġ�� ���Ѵ�.
			startLetterIndex = selectedStartLetterPos;
			//5.1.2 ������ ������ġ�� ������ ������ ������ġ�� ���Ѵ�.
			endLetterIndex = selectedEndLetterPos;
			//5.1.3 �����ϴ� ���� ��ġ�� ������ �����ϴ� ���� ��ġ�� ���Ѵ�.
			startRowIndex = selectedStartRowPos;
			//5.1.4 ������ ���� ��ġ�� ������ ������ ���� ��ġ�� ���Ѵ�.
			endRowIndex = selectedEndRowPos;
		}
		//5.2 ������ �������� ����Ǿ�����
		else
		{
			//5.2.1 �����ϴ� ������ġ�� ������ ������ ������ġ�� ���Ѵ�.
			startLetterIndex = selectedEndLetterPos;
			//5.2.2 ������ ������ġ�� ������ �����ϴ� ������ġ�� ���Ѵ�.
			endLetterIndex = selectedStartLetterPos;
			//5.2.3 �����ϴ� ���� ��ġ�� ������ ������ ���� ��ġ�� ���Ѵ�.
			startRowIndex = selectedEndRowPos;
			//5.2.4 ������ ���� ��ġ�� ������ �����ϴ� ���� ��ġ�� ���Ѵ�.
			endRowIndex = selectedStartRowPos;
		}
		//5.3 �����ϴ� ���� ���Ѵ�.
		startRow = this->GetAt(startRowIndex);
		Glyph* endRow = 0;//������ ���� ��ġ
		Glyph* row = 0;//���� �ּҸ� ���� ����
		Long letterIndex = 0;//���� ��ġ
		Long rowIndex = 0;//���� ��ġ
		//5.4 �����ϴ� ������ġ���� �����ϴ� ���� ������ ���ڱ��� �����.
		while (startLetterIndex < startRow->GetLength())
		{
			//5.4.1 ���ڸ� ����� ���� ���ڸ� ���Ѵ�.
			letter = startRow->GetAt(startLetterIndex);
			//5.4.2 ���ڸ� ���� �����ؼ� ���� ������ �ٿ� �����Ѵ�.
			copyRow->Add(letter->Clone());
			//5.4.3 �ٿ��� ���ڸ� �����.
			startRow->Remove(startLetterIndex);
		}
		//6.5 �����ϴ� ���� �����ٺ��� ������ �������� ���ڿ� ���� �����.
		nextRowIndex = startRowIndex + 1;
		while (nextRowIndex < endRowIndex)
		{
			//6.5.1 ���� ���Ѵ�.
			row = this->GetAt(nextRowIndex);
			//6.5.2 ������ ���� ��¥ ���̸�
			if (!dynamic_cast<DummyRow*>(row))
			{
				//6.5.2.1 ��¥ ���� �����Ѵ�.
				copyRow = new Row();
				//6.5.2.2 ��¥ ���� ���� ���� ��Ʈ�� �߰��Ѵ�.
				rowIndex = copyNote->Add(copyRow);
			}
			//6.5.3 ������ġ�� ����ġ��Ų��.
			letterIndex = 0;
			//6.5.4 �ٿ��� ������ ���ڱ��� �ݺ��Ѵ�.
			while (letterIndex < row->GetLength())
			{
				//6.5.4.1 ���ڸ� ����� ���� ���ڸ� ���Ѵ�.
				letter = row->GetAt(letterIndex);
				//6.5.4.2 ���ڸ� ���� �����ؼ� ��¥ �ٿ� �����Ѵ�.
				copyRow->Add(letter->Clone());
				//6.5.4.3 ���� ���ڸ� �����.
				row->Remove(letterIndex);
			}
			//6.5.5 ���� ���ڸ� �������⶧���� �޸��忡�� ���� �����.
			this->Remove(nextRowIndex);
			//6.5.6 ���� ������ ������ ������ ������ ���� ��ġ�� ��ĭ �մ������.
			endRowIndex--;
		}
		//6.6 ������ ���� ���Ѵ�.
		endRow = this->GetAt(endRowIndex);
		//6.7 ������ ���� ��¥ ���̸�
		if (!dynamic_cast<DummyRow*>(endRow))
		{
			//6.7.1 Row�� �����Ѵ�.
			copyRow = new Row();
			//6.7.2 ��¥ ���� ���� ���� ��Ʈ�� �߰��Ѵ�.
			rowIndex = copyNote->Add(copyRow);
		}
		//6.8 ������ ���� ó������ ������ ���ڱ��� ���ڸ� �����.
		letterIndex = 0;
		while (letterIndex < endLetterIndex)
		{
			//6.8.1 ���ڸ� ����� ���� ���ڸ� ���Ѵ�.
			letter = endRow->GetAt(letterIndex);
			//6.8.2 ���ڸ� ���� �����ؼ� DummyRow�� �����Ѵ�.
			copyRow->Add(letter->Clone());
			//6.8.3 ������ ���� ���ڸ� �����.
			endRow->Remove(letterIndex);
			//6.8.4 ������ ���� ù���ڸ� ����� ���� ���ں��� ������ ��ĭ��
			//������� ������ ������ ������ġ�� -1 ���ҽ�Ų��.
			endLetterIndex--;
		}
		//6.9 ������ ���� �����ϴ� �ٷ� Join��Ų��.
		endRow->Join(startRow);
		//6.10 ������ ���� �����ϴ� �ٷ� Join�Ǿ��� ������
		//������ ���� �޸��忡�� �����.
		this->Remove(endRowIndex);
		//6.11 ���� ���� ��ġ�� �����ϴ� ���� ��ġ�� �����Ѵ�.
		this->current = startRowIndex;
		row = this->GetAt(startRowIndex);
		//6.12 ���� ������ ��ġ�� �����ϴ� ������ ��ġ�� �����Ѵ�.
		row->Move(startLetterIndex);
	}
	//7. ������ ������ ��ȯ�Ѵ�.
	return copyNote;
}

//���ÿ��� �����
void Note::RemoveSelectedTexts(Long selectedStartRowPos,
	Long selectedStartLetterPos, Long selectedEndRowPos, Long selectedEndLetterPos)
{
	//������ ������ �����ʹ�����
	Long startRowIndex = 0;//�����ϴ� ���� ��ġ
	Long startLetterIndex = 0;//�����ϴ� ���� ��ġ
	Long endRowIndex = 0;//������ ���� ��ġ
	Long endLetterIndex = 0;//������ ���� ��ġ
	Glyph* letter = 0;//������ �ּҸ� ���� ����
	Long nextRowIndex = 0;//���� ���� �ּ���ġ
	Glyph* startRow = 0;//�����ϴ� ���� �ּҸ� ���� ����
	//1. ������ ���۵Ǵ� �ٰ� ������ ������ ���� ������
	//(�� �� �������� ������ �Ǿ� �����Ƿ� ���� �������� �ʰ� ���ڵ鸸 ������)
	if (selectedStartRowPos == selectedEndRowPos)
	{
		//1.1 ������ ���������� ����Ǿ�����
		if (selectedStartLetterPos < selectedEndLetterPos)
		{
			//1.1.1 �����ϴ� ������ġ�� ������ ���۵Ǵ� ������ġ�� ���Ѵ�.
			startLetterIndex = selectedStartLetterPos;
			//1.1.2 ������ ������ġ�� ������ ������ ������ġ�� ���Ѵ�.
			endLetterIndex = selectedEndLetterPos;
			//1.1.3 �����ϴ� ���� ��ġ�� ������ �����ϴ� ���� ��ġ�� ���Ѵ�.
			startRowIndex = selectedStartRowPos;

		}
		//1.2 ������ �������� ����Ǿ�����
		else
		{
			//1.2.1 �����ϴ� ������ġ�� ������ ������ ������ġ�� ���Ѵ�.
			startLetterIndex = selectedEndLetterPos;
			//1.2.2 ������ ������ġ�� ������ �����ϴ� ������ġ�� ���Ѵ�.
			endLetterIndex = selectedStartLetterPos;
			//1.2.3 �����ϴ� ���� ��ġ�� ������ ������ ���� ��ġ�� ���Ѵ�.
			startRowIndex = selectedEndRowPos;
		}
		//1.3 ������ ���۵Ǵ� ���� ���Ѵ�.
		startRow = this->GetAt(startRowIndex);
		//1.4 �����ϴ� ������ġ���� ������ ���ڱ��� �����.
		while (startLetterIndex < endLetterIndex)
		{
			//1.4.1 �ٿ��� ���ڸ� �����.
			startRow->Remove(startLetterIndex);
			//1.4.2 �ٿ��� ���ڰ� �������� ���� ������ �ٰ� �����ϴ� ������ ���� ���ڰ�
			//������ �����ϴ� ������ ��ġ�� �մ���� ���� �ǹǷ� ������ ������ ���� ���� ���ҽ�Ų��. 
			endLetterIndex--;
		}
	}
	//2. ������ ���۵Ǵ� �ٰ� ������ ������ ���� ���� �ٸ���
	//(������ ���� �ٿ� ���ļ� �Ǿ� �ֱ� ������ ���ڰ� �� ������ ���� ����������)
	else
	{
		//2.1 ������ ���������� ����Ǿ����� 
		if (selectedStartRowPos < selectedEndRowPos)
		{
			//2.1.1 �����ϴ� ������ġ�� ������ ���۵Ǵ� ������ġ�� ���Ѵ�.
			startLetterIndex = selectedStartLetterPos;
			//2.1.2 ������ ������ġ�� ������ ������ ������ġ�� ���Ѵ�.
			endLetterIndex = selectedEndLetterPos;
			//2.1.3 �����ϴ� ���� ��ġ�� ������ �����ϴ� ���� ��ġ�� ���Ѵ�.
			startRowIndex = selectedStartRowPos;
			//2.1.4 ������ ���� ��ġ�� ������ ������ ���� ��ġ�� ���Ѵ�.
			endRowIndex = selectedEndRowPos;
		}
		//2.2 ������ �������� ����Ǿ�����
		else
		{
			//2.2.1 �����ϴ� ������ġ�� ������ ������ ������ġ�� ���Ѵ�.
			startLetterIndex = selectedEndLetterPos;
			//2.2.2 ������ ������ġ�� ������ �����ϴ� ������ġ�� ���Ѵ�.
			endLetterIndex = selectedStartLetterPos;
			//2.2.3 �����ϴ� ���� ��ġ�� ������ ������ ���� ��ġ�� ���Ѵ�.
			startRowIndex = selectedEndRowPos;
			//2.2.4 ������ ���� ��ġ�� ������ �����ϴ� ���� ��ġ�� ���Ѵ�.
			endRowIndex = selectedStartRowPos;
		}
		Glyph* endRow = 0;//������ ���� ��ġ
		Glyph* row = 0;//���� �ּҸ� ���� ����
		Long letterIndex = 0;//���� ��ġ
		//2.3 ������ ���۵Ǵ� ���� ���Ѵ�.
		startRow = this->GetAt(startRowIndex);
		//2.4 �����ϴ� ������ġ���� �����ϴ� ���� ������ ���ڱ��� �����.
		while (startLetterIndex < startRow->GetLength())
		{
			//2.4.1 �ٿ��� ���ڸ� �����.
			startRow->Remove(startLetterIndex);
		}
		//2.5 �����ϴ� ���� �����ٺ��� ������ �������� ���ڿ� ���� �����.
		nextRowIndex = startRowIndex + 1;
		while (nextRowIndex < endRowIndex)
		{
			//2.5.1 ���� ���Ѵ�.
			row = this->GetAt(nextRowIndex);
			//2.5.2 ������ġ�� ����ġ��Ų��.
			letterIndex = 0;
			//2.5.3 �ٿ��� ������ ���ڱ��� �ݺ��Ѵ�.
			while (letterIndex < row->GetLength())
			{
				//2.5.3.1 ���� ���ڸ� �����.
				row->Remove(letterIndex);
			}
			//2.5.4 ���� ���ڸ� �������⶧���� �޸��忡�� ���� �����.
			this->Remove(nextRowIndex);
			//2.5.5 ���� ������ ������ ������ ������ ���� ��ġ�� ��ĭ �մ������.
			endRowIndex--;
		}
		//2.6 ������ ���� ���Ѵ�.
		endRow = this->GetAt(endRowIndex);
		//2.7 ������ ���� ó������ ������ ���ڱ��� ���ڸ� �����.
		letterIndex = 0;
		while (letterIndex < endLetterIndex)
		{
			//2.7.1 ������ ���� ���ڸ� �����.
			endRow->Remove(letterIndex);
			//2.7.2 ������ ���� ù���ڸ� ����� ���� ���ں��� ������ ��ĭ��
			//������� ������ ������ ������ġ�� -1 ���ҽ�Ų��.
			endLetterIndex--;
		}
		//2.8 ������ ���� �����ϴ� �ٷ� Join��Ų��.
		endRow->Join(startRow);
		//2.9 ������ ���� �����ϴ� �ٷ� Join�Ǿ��� ������
		//������ ���� �޸��忡�� �����.
		this->Remove(endRowIndex);
		//2.10 ���� ���� ��ġ�� �����ϴ� ���� ��ġ�� �����Ѵ�.
		this->current = startRowIndex;
		row = this->GetAt(startRowIndex);
		//2.11 ���� ������ ��ġ�� �����ϴ� ������ ��ġ�� �����Ѵ�.
		row->Move(startLetterIndex);
	}
}

//�ش���ġ�� texts�� �����ϱ�
Long Note::InsertTexts(Long currentRowIndex, Long currentLetterIndex, Glyph* note)
{
	//1. ���� ���� ���Ѵ�.
	Glyph* currentRow = this->GetAt(currentRowIndex);
	//2. �޸����� ���� ������ġ�� ���� ���ڰ������� ������
	//�ڿ� split�� ���ڰ� ������ split�ϰ� ������ ���ڶ� �ڿ� ���ڰ� ������ split���� �ʴ´�.
	Glyph* splitedRow = 0;
	bool isSplited = false;
	if (currentLetterIndex < currentRow->GetLength())
	{
		//2.1 �޸����� ���� �ٿ��� ���� ������ġ �������� split��Ų��.(splitedRow�� ���� �Ҵ��)
		splitedRow = currentRow->Split(currentLetterIndex);
		//2.2 split�� �Ǿ��� ������ isSplited�� true�� �ٲ��ش�.
		isSplited = true;
	}
	//3. command�� ���� ��Ʈ�� ù��° ���� ���Ѵ�.
	Long firstCopyRowPos = 0;
	Glyph* firstCopyRow = note->GetAt(firstCopyRowPos);
	Glyph* letter = 0;//���ڸ� ���� ����
	//4. command�� ���� ��Ʈ�� ù��° ���� �������� �������� �ݺ��Ѵ�.
	Long letterIndex = 0;
	while (letterIndex < firstCopyRow->GetLength())
	{
		//4.1 command�� ���� ��Ʈ�� ù��° ���� ���ڸ� ���Ѵ�.
		letter = firstCopyRow->GetAt(letterIndex);
		//4.2 ���ڸ� �޸����� ���� �ٿ� �߰��Ѵ�.(���� ����)
		currentRow->Add(letter->Clone());
		//4.3 ���� ���ڷ� �̵��Ѵ�.
		letterIndex++;
	}
	//5. command�� ���� ��Ʈ�� ù��° �ٿ��� ���� �ٷ� �̵��Ѵ�.
	Long nextCopyRowPos = firstCopyRowPos + 1;
	Glyph* copyRow = firstCopyRow;//ù��° ���� ����Ʈ
	Long i = currentRowIndex;
	//6. Ŭ�������� ���� ��Ʈ(������ ����)�� �������ٱ��� �ݺ��Ѵ�.
	while (nextCopyRowPos < note->GetLength())
	{
		//6.1 ������ ���� ���Ѵ�.
		copyRow = note->GetAt(nextCopyRowPos);
		//6.2 �޸����� ���� ���� ���� �ٷ� �̵��Ѵ�.
		i++;
		//6.3 ���� ���� �޸����� ���� ���� ��ġ �������� �����ִ´�.(���� ����)
		i = this->Add(i, copyRow->Clone());
		//6.4 ������ ��Ʈ���� ���� �ٷ� �̵��Ѵ�.
		nextCopyRowPos++;
	}
	//7. �޸��忡�� ���� ���� ���Ѵ�.
	currentRow = this->GetAt(i);
	//8. �޸��忡�� ���� ������ġ�� ���Ѵ�.
	currentLetterIndex = currentRow->GetCurrent();
	//9. �޸��忡�� �Ʊ� split�ߴ� ���ڵ��� ������
	if (isSplited == true)
	{
		//9.1 split�� ���� �޸����� ���� �ٿ� Join��Ų��.
		splitedRow->Join(currentRow);
		//9.2 splitedRow�� �Ҵ����� �����ش�.
		if (splitedRow != 0)
		{
			delete splitedRow;
		}
		//9.3 �޸��忡�� ���� ������ġ�� �ٽ� �������ش�.
		currentRow->Move(currentLetterIndex);
	}
	//10. ���� ���� ��ġ�� ��ȯ�Ѵ�.
	return i;
}

//Visitor Pattern
void Note::Accept(GlyphVisitor* glyphVisitor)
{
	glyphVisitor->VisitNote(this);
}