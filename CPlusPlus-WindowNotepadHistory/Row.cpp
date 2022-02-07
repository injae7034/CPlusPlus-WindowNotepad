#include "Row.h"
#include "GlyphVisitor.h"
#include "DoubleByteLetter.h"
#include "SingleByteLetter.h"

//����Ʈ������
Row::Row(Long capacity)
	:Composite(capacity)//Composite�� �⺻�����ڸ� ȣ���ϸ� �ű⼭ �������(capacity, length,
	//current ���� �ʱ�ȭ�ȴ�.!!)
{
	
}

//���������
Row::Row(const Row& source)
	:Composite(source)
{
	
}

//ġȯ������
Row& Row::operator=(const Row& source)
{
	Composite::operator=(source);

	return *this;
}

//�Ҹ���
Row::~Row()
{

}

//Prototype Patter Clone
Glyph* Row::Clone()
{
	return new Row(*this);
}

//GetContent
string Row::GetContent()
{
	string content;//���ڵ��� ������ ����(��)
	string letter;//���ڸ� ������ ����
	Long index = 0;
	//1. ������ ���ڰ�����ŭ �ݺ��Ѵ�.
	while (index < this->length)
	{
		//1.1 letter�� �ѱ���(Row�� �迭��� 1��)�� �����Ѵ�.
		letter = this->glyphs.GetAt(index)->GetContent();
		//1.2 letter�� �ǹ����̸�
		if (letter == "\t")
		{
			//1.2.1 ������ 8���� �ٲ۴�.
			letter = "        ";
		}
		//1.3 �ٿ� ���ڸ� �߰��Ѵ�.
		content += letter;
		index++;
	}
	//2. ���� ��ȯ�Ѵ�.
	return content;
	//3. ������.
}

//GetPartOfContent
string Row::GetPartOfContent(Long current)
{
	string content = "";//���ڵ��� ������ ����(��)
	string letter;//���ڸ� ������ ����
	Long index = 0;
	//1. ���� ���� ĳ���� ���� ��ġ���� �������� �ݺ��Ѵ�.
	while (index < current)
	{
		//1.1 letter�� �ѱ���(Row�� �迭��� 1��)�� �����Ѵ�.
		letter = this->glyphs.GetAt(index)->GetContent();
		//1.2 letter�� �ǹ����̸�
		if (letter == "\t")
		{
			//1.2.1 ������ 8���� �ٲ۴�.
			letter = "        ";
		}
		//1.3 �ٿ� ���ڸ� �߰��Ѵ�.
		content += letter;
		index++;
	}
	//2. ���� ��ȯ�Ѵ�.
	return content;
	//3. ������.
}

//Visitor Pattern
void Row::Accept(GlyphVisitor* glyphVisitor)
{
	glyphVisitor->VisitRow(this);
}

//�ٿ� �ܾ������ �����ֱ�
void Row::AddWord(string word)
{
	//1. ���� ������ ��ġ�� ���Ѵ�.
	Long currentLetterIndex = this->GetCurrent();
	currentLetterIndex--;
	//2. word�� ���� ������ ���Ѵ�.
	Long wordLength = word.length();
	//3. word�� ���� �������� �������� �ݺ��Ѵ�.
	Long i = 0;
	Glyph* letter = 0;
	string koreanLetter;
	while (i < wordLength)
	{
		//3.1 �ѱ��̸�
		if ((word[i] & 0x80))
		{
			//3.1.1 DoubleByteLetter�� �����Ѵ�.
			koreanLetter = word[i];
			i++;
			koreanLetter += word[i];
			letter = new DoubleByteLetter((char*)koreanLetter.c_str(), false);
		}
		//3.2 �ѱ��� �ƴϸ�
		else
		{
			//3.2.1 SingleByteLetter�� �����Ѵ�.
			letter = new SingleByteLetter(word[i], false);
		}
		//3.3 ���� �ٿ� ���ڸ� �߰��Ѵ�.
		currentLetterIndex = this->Add(currentLetterIndex + 1, letter);
		//3.4 keyword�� ���� ���ڷ� �̵��Ѵ�.
		i++;
	}
}