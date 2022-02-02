//�����׽�Ʈ�ó�����


//#include "Note.h"
#include "Row.h"
#include "SingleByteLetter.h"
#include "DoubleByteLetter.h"
//#include "Letter.h"

#include "GlyphCreator.h"//���丮�޼ҵ�� ���� GlyphCreator����� ������ ���ư�!
#include<iostream>
using namespace std;

int main(int argc, char* argv[])
{
	//1. �޸����� �����.
	GlyphCreator glyphCreator;
	Glyph* note = glyphCreator.Create((char*)"\0");
	//2. ù��° ���� �����.
	Glyph* row = glyphCreator.Create((char*)"\n");
	if (dynamic_cast<Row*>(row))
	{
		cout << "���� ��������ϴ�." << endl;
	}
	Long rowIndex;
	Long letterIndex;
	rowIndex = note->Add(row);
	//3. ù��° �ٿ� 'A'�� �Է��Ѵ�.
	Glyph* letter = glyphCreator.Create((char*)"A");
	if (dynamic_cast<SingleByteLetter*>(letter))
	{
		cout << "���� ���ĺ��� �����Ǿ����ϴ�." << endl;
	}
	cout << letter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(letter);
	Glyph* glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	//4. ù��° �ٿ� 'p'�� �Է��Ѵ�.
	letter = glyphCreator.Create((char*)"p");
	cout << letter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(letter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	//5. ù��° �ٿ� 'p'�� �Է��Ѵ�.
	letter = glyphCreator.Create((char*)"p");
	cout << letter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(letter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	//6. ù��° �ٿ� 'l'�� �Է��Ѵ�.
	letter = glyphCreator.Create((char*)"l");
	cout << letter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(letter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	//7. ù��° �ٿ� 'e'�� �Է��Ѵ�.
	letter = glyphCreator.Create((char*)"e");
	cout << letter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(letter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	//8. ù��° �ٿ� "��"�� �Է��Ѵ�.
	letter = glyphCreator.Create((char*)"��");
	if (dynamic_cast<DoubleByteLetter*>(letter))
	{
		cout << "�ѱ��� �����Ǿ����ϴ�." << endl;
	}
	cout << letter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(letter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	//9. �ι�° ���� �����.
	row = glyphCreator.Create((char*)"\n");
	rowIndex = note->Add(row);
	//10. �ι�° �ٿ� "��"�� �Է��Ѵ�.
	letter = glyphCreator.Create((char*)"��");
	cout << letter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(letter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	cout << "<��Ʈ��ü���>" << endl;
	cout << note->GetContent() << endl;
	//11. �ι�° �ٿ� "��"�� �Է��Ѵ�.
	letter = glyphCreator.Create((char*)"��");
	cout << letter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(letter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	cout << "<��Ʈ��ü���>" << endl;
	cout << note->GetContent() << endl;
	//12. �ι�° �ٿ� "��"�� �Է��Ѵ�.
	letter = glyphCreator.Create((char*)"��");
	cout << letter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(letter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	cout << "<��Ʈ��ü���>" << endl;
	cout << note->GetContent() << endl;
	//13. �ι�° �ٿ� "."�� �Է��Ѵ�.
	letter = glyphCreator.Create((char*)".");
	cout << letter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(letter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	cout << "<��Ʈ��ü���>" << endl;
	cout << note->GetContent() << endl;
	//14. ����° ���� �����.
	row = glyphCreator.Create((char*)"\r");
	rowIndex = note->Add(row);
	//15. ����° �ٿ� "��"�� �Է��Ѵ�.
	letter = glyphCreator.Create((char*)"��");
	cout << letter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(letter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	cout << "<��Ʈ��ü���>" << endl;
	cout << note->GetContent() << endl;
	//16. ����° �ٿ� "��"�� �Է��Ѵ�.
	letter = glyphCreator.Create((char*)"��");
	cout << letter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(letter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	cout << "<��Ʈ��ü���>" << endl;
	cout << note->GetContent() << endl;
	//17. ����° �ٿ� "��"�� �Է��Ѵ�.
	letter = glyphCreator.Create((char*)"��");
	cout << letter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(letter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	cout << "<��Ʈ��ü���>" << endl;
	cout << note->GetContent() << endl;
	//18. �׹�° ���� �����.
	row = glyphCreator.Create((char*)"\r");
	rowIndex = note->Add(row);
	//19. �׹�° �ٿ� "��"�� �Է��Ѵ�.
	letter = glyphCreator.Create((char*)"��");
	cout << letter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(letter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	cout << "<��Ʈ��ü���>" << endl;
	cout << note->GetContent() << endl;
	//20. �׹�° �ٿ� "��"�� �Է��Ѵ�.
	letter = glyphCreator.Create((char*)"��");
	cout << letter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(letter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	cout << "<��Ʈ��ü���>" << endl;
	cout << note->GetContent() << endl;
	//21. �׹�° �ٿ� "��"�� �Է��Ѵ�.
	letter = glyphCreator.Create((char*)"��");
	cout << letter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(letter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	cout << "<��Ʈ��ü���>" << endl;
	cout << note->GetContent() << endl;
	//22. �׹�° �ٿ� "."�� �Է��Ѵ�.
	letter = glyphCreator.Create((char*)".");
	cout << letter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(letter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	cout << "<��Ʈ��ü���>" << endl;
	cout << note->GetContent() << endl;
	//23. �׹�° �ٿ� "."�� �����.
	note->GetAt(rowIndex)->Remove(letterIndex);
	cout << "<��Ʈ��ü���>" << endl;
	cout << note->GetContent() << endl;
	//24. �׹�° �ٿ� "��"�� �����.
	note->GetAt(rowIndex)->Remove(0);
	cout << "<��Ʈ��ü���>" << endl;
	cout << note->GetContent() << endl;
	//note�� �Ҵ������ϸ� ���� �Ҵ�Ȱ� ��������
	if (note != 0)
	{
		delete note;
	}

	return 0;
#if 0
	//1. �޸����� �����.
	Glyph* note = new Note;
	//2. ù��° ���� �����.
	Glyph* firstRow = new Row;
	Long rowIndex = note->Add(firstRow);
	//3. ù��° �ٿ� 'A'�� �Է��Ѵ�.
	Glyph* firstLetter = new SingleByteLetter('A');
	cout << firstLetter->GetContent() << endl;
	Long letterIndex = note->GetAt(rowIndex)->Add(firstLetter);
	Glyph* glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	//4. ù��° �ٿ� 'p'�� �Է��Ѵ�.
	Glyph* secondLetter = new SingleByteLetter('p');
	cout << secondLetter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(secondLetter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	//5. ù��° �ٿ� 'p'�� �Է��Ѵ�.
	Glyph* thirdLetter = new SingleByteLetter('p');
	cout << thirdLetter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(thirdLetter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	//6. ù��° �ٿ� 'l'�� �Է��Ѵ�.
	Glyph* fourthLetter = new SingleByteLetter('l');
	cout << fourthLetter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(fourthLetter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	//7. ù��° �ٿ� 'e'�� �Է��Ѵ�.
	Glyph* fifthLetter = new SingleByteLetter('e');
	cout << fifthLetter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(fifthLetter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	//8. ù��° �ٿ� "��"�� �Է��Ѵ�.
	Glyph* sixthLetter = new DoubleByteLetter((char*)"��");
	cout << sixthLetter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(sixthLetter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	//9. �ι�° ���� �����.
	Glyph* secondRow = new Row;
	rowIndex = note->Add(secondRow);
	//10. �ι�° �ٿ� "��"�� �Է��Ѵ�.
	Glyph* seventhLetter = new DoubleByteLetter((char*)"��");
	cout << seventhLetter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(seventhLetter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	cout << "<��Ʈ��ü���>" << endl;
	cout << note->GetContent() << endl;
	//11. �ι�° �ٿ� "��"�� �Է��Ѵ�.
	Glyph* eighthLetter = new DoubleByteLetter((char*)"��");
	cout << eighthLetter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(eighthLetter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	cout << "<��Ʈ��ü���>" << endl;
	cout << note->GetContent() << endl;
	//12. �ι�° �ٿ� "��"�� �Է��Ѵ�.
	Glyph* ninethLetter = new DoubleByteLetter((char*)"��");
	cout << ninethLetter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(ninethLetter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	cout << "<��Ʈ��ü���>" << endl;
	cout << note->GetContent() << endl;
	//13. �ι�° �ٿ� "."�� �Է��Ѵ�.
	Glyph* tenthLetter = new DoubleByteLetter((char*)".");
	cout << tenthLetter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(tenthLetter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	cout << "<��Ʈ��ü���>" << endl;
	cout << note->GetContent() << endl;
	//14. ����° ���� �����.
	Glyph* thirdRow = new Row;
	rowIndex = note->Add(thirdRow);
	//15. ����° �ٿ� "��"�� �Է��Ѵ�.
	Glyph* elventhLetter = new DoubleByteLetter((char*)"��");
	cout << elventhLetter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(elventhLetter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	cout << "<��Ʈ��ü���>" << endl;
	cout << note->GetContent() << endl;
	//16. ����° �ٿ� "��"�� �Է��Ѵ�.
	Glyph* twelvethLetter = new DoubleByteLetter((char*)"��");
	cout << twelvethLetter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(twelvethLetter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	cout << "<��Ʈ��ü���>" << endl;
	cout << note->GetContent() << endl;
	//17. ����° �ٿ� "��"�� �Է��Ѵ�.
	Glyph* thirteenthLetter = new DoubleByteLetter((char*)"��");
	cout << thirteenthLetter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(thirteenthLetter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	cout << "<��Ʈ��ü���>" << endl;
	cout << note->GetContent() << endl;
	//18. �׹�° ���� �����.
	Glyph* fourthRow = new Row;
	rowIndex = note->Add(fourthRow);
	//19. �׹�° �ٿ� "��"�� �Է��Ѵ�.
	Glyph* fourteenthLetter = new DoubleByteLetter((char*)"��");
	cout << fourteenthLetter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(fourteenthLetter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	cout << "<��Ʈ��ü���>" << endl;
	cout << note->GetContent() << endl;
	//20. �׹�° �ٿ� "��"�� �Է��Ѵ�.
	Glyph* fifteenthLetter = new DoubleByteLetter((char*)"��");
	cout << fifteenthLetter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(fifteenthLetter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	cout << "<��Ʈ��ü���>" << endl;
	cout << note->GetContent() << endl;
	//21. �׹�° �ٿ� "��"�� �Է��Ѵ�.
	Glyph* sixteenthLetter = new DoubleByteLetter((char*)"��");
	cout << sixteenthLetter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(sixteenthLetter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	cout << "<��Ʈ��ü���>" << endl;
	cout << note->GetContent() << endl;
	//22. �׹�° �ٿ� "."�� �Է��Ѵ�.
	Glyph* seventeenthLetter = new DoubleByteLetter((char*)".");
	cout << seventeenthLetter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(seventeenthLetter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	cout << "<��Ʈ��ü���>" << endl;
	cout << note->GetContent() << endl;
#endif
}