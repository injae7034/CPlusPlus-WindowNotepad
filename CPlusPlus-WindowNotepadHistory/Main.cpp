//메인테스트시나리오


//#include "Note.h"
#include "Row.h"
#include "SingleByteLetter.h"
#include "DoubleByteLetter.h"
//#include "Letter.h"

#include "GlyphCreator.h"//팩토리메소드로 인해 GlyphCreator헤더만 있으면 돌아감!
#include<iostream>
using namespace std;

int main(int argc, char* argv[])
{
	//1. 메모장을 만든다.
	GlyphCreator glyphCreator;
	Glyph* note = glyphCreator.Create((char*)"\0");
	//2. 첫번째 줄을 만든다.
	Glyph* row = glyphCreator.Create((char*)"\n");
	if (dynamic_cast<Row*>(row))
	{
		cout << "줄이 생성됬습니다." << endl;
	}
	Long rowIndex;
	Long letterIndex;
	rowIndex = note->Add(row);
	//3. 첫번째 줄에 'A'를 입력한다.
	Glyph* letter = glyphCreator.Create((char*)"A");
	if (dynamic_cast<SingleByteLetter*>(letter))
	{
		cout << "영어 알파벳이 생성되었습니다." << endl;
	}
	cout << letter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(letter);
	Glyph* glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	//4. 첫번째 줄에 'p'를 입력한다.
	letter = glyphCreator.Create((char*)"p");
	cout << letter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(letter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	//5. 첫번째 줄에 'p'를 입력한다.
	letter = glyphCreator.Create((char*)"p");
	cout << letter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(letter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	//6. 첫번째 줄에 'l'를 입력한다.
	letter = glyphCreator.Create((char*)"l");
	cout << letter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(letter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	//7. 첫번째 줄에 'e'를 입력한다.
	letter = glyphCreator.Create((char*)"e");
	cout << letter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(letter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	//8. 첫번째 줄에 "은"을 입력한다.
	letter = glyphCreator.Create((char*)"은");
	if (dynamic_cast<DoubleByteLetter*>(letter))
	{
		cout << "한글이 생성되었습니다." << endl;
	}
	cout << letter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(letter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	//9. 두번째 줄을 만든다.
	row = glyphCreator.Create((char*)"\n");
	rowIndex = note->Add(row);
	//10. 두번째 줄에 "사"를 입력한다.
	letter = glyphCreator.Create((char*)"사");
	cout << letter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(letter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	cout << "<노트전체출력>" << endl;
	cout << note->GetContent() << endl;
	//11. 두번째 줄에 "과"를 입력한다.
	letter = glyphCreator.Create((char*)"과");
	cout << letter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(letter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	cout << "<노트전체출력>" << endl;
	cout << note->GetContent() << endl;
	//12. 두번째 줄에 "다"를 입력한다.
	letter = glyphCreator.Create((char*)"다");
	cout << letter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(letter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	cout << "<노트전체출력>" << endl;
	cout << note->GetContent() << endl;
	//13. 두번째 줄에 "."를 입력한다.
	letter = glyphCreator.Create((char*)".");
	cout << letter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(letter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	cout << "<노트전체출력>" << endl;
	cout << note->GetContent() << endl;
	//14. 세번째 줄을 만든다.
	row = glyphCreator.Create((char*)"\r");
	rowIndex = note->Add(row);
	//15. 세번째 줄에 "사"를 입력한다.
	letter = glyphCreator.Create((char*)"사");
	cout << letter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(letter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	cout << "<노트전체출력>" << endl;
	cout << note->GetContent() << endl;
	//16. 세번째 줄에 "과"를 입력한다.
	letter = glyphCreator.Create((char*)"과");
	cout << letter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(letter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	cout << "<노트전체출력>" << endl;
	cout << note->GetContent() << endl;
	//17. 세번째 줄에 "는"를 입력한다.
	letter = glyphCreator.Create((char*)"는");
	cout << letter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(letter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	cout << "<노트전체출력>" << endl;
	cout << note->GetContent() << endl;
	//18. 네번째 줄을 만든다.
	row = glyphCreator.Create((char*)"\r");
	rowIndex = note->Add(row);
	//19. 네번째 줄에 "맛"를 입력한다.
	letter = glyphCreator.Create((char*)"맛");
	cout << letter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(letter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	cout << "<노트전체출력>" << endl;
	cout << note->GetContent() << endl;
	//20. 네번째 줄에 "있"를 입력한다.
	letter = glyphCreator.Create((char*)"있");
	cout << letter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(letter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	cout << "<노트전체출력>" << endl;
	cout << note->GetContent() << endl;
	//21. 네번째 줄에 "다"를 입력한다.
	letter = glyphCreator.Create((char*)"다");
	cout << letter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(letter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	cout << "<노트전체출력>" << endl;
	cout << note->GetContent() << endl;
	//22. 네번째 줄에 "."를 입력한다.
	letter = glyphCreator.Create((char*)".");
	cout << letter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(letter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	cout << "<노트전체출력>" << endl;
	cout << note->GetContent() << endl;
	//23. 네번째 줄에 "."을 지운다.
	note->GetAt(rowIndex)->Remove(letterIndex);
	cout << "<노트전체출력>" << endl;
	cout << note->GetContent() << endl;
	//24. 네번째 줄에 "맛"을 지운다.
	note->GetAt(rowIndex)->Remove(0);
	cout << "<노트전체출력>" << endl;
	cout << note->GetContent() << endl;
	//note만 할당해제하면 힙에 할당된게 다지워짐
	if (note != 0)
	{
		delete note;
	}

	return 0;
#if 0
	//1. 메모장을 만든다.
	Glyph* note = new Note;
	//2. 첫번째 줄을 만든다.
	Glyph* firstRow = new Row;
	Long rowIndex = note->Add(firstRow);
	//3. 첫번째 줄에 'A'를 입력한다.
	Glyph* firstLetter = new SingleByteLetter('A');
	cout << firstLetter->GetContent() << endl;
	Long letterIndex = note->GetAt(rowIndex)->Add(firstLetter);
	Glyph* glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	//4. 첫번째 줄에 'p'를 입력한다.
	Glyph* secondLetter = new SingleByteLetter('p');
	cout << secondLetter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(secondLetter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	//5. 첫번째 줄에 'p'를 입력한다.
	Glyph* thirdLetter = new SingleByteLetter('p');
	cout << thirdLetter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(thirdLetter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	//6. 첫번째 줄에 'l'를 입력한다.
	Glyph* fourthLetter = new SingleByteLetter('l');
	cout << fourthLetter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(fourthLetter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	//7. 첫번째 줄에 'e'를 입력한다.
	Glyph* fifthLetter = new SingleByteLetter('e');
	cout << fifthLetter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(fifthLetter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	//8. 첫번째 줄에 "은"을 입력한다.
	Glyph* sixthLetter = new DoubleByteLetter((char*)"은");
	cout << sixthLetter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(sixthLetter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	//9. 두번째 줄을 만든다.
	Glyph* secondRow = new Row;
	rowIndex = note->Add(secondRow);
	//10. 두번째 줄에 "사"를 입력한다.
	Glyph* seventhLetter = new DoubleByteLetter((char*)"사");
	cout << seventhLetter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(seventhLetter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	cout << "<노트전체출력>" << endl;
	cout << note->GetContent() << endl;
	//11. 두번째 줄에 "과"를 입력한다.
	Glyph* eighthLetter = new DoubleByteLetter((char*)"과");
	cout << eighthLetter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(eighthLetter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	cout << "<노트전체출력>" << endl;
	cout << note->GetContent() << endl;
	//12. 두번째 줄에 "다"를 입력한다.
	Glyph* ninethLetter = new DoubleByteLetter((char*)"다");
	cout << ninethLetter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(ninethLetter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	cout << "<노트전체출력>" << endl;
	cout << note->GetContent() << endl;
	//13. 두번째 줄에 "."를 입력한다.
	Glyph* tenthLetter = new DoubleByteLetter((char*)".");
	cout << tenthLetter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(tenthLetter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	cout << "<노트전체출력>" << endl;
	cout << note->GetContent() << endl;
	//14. 세번째 줄을 만든다.
	Glyph* thirdRow = new Row;
	rowIndex = note->Add(thirdRow);
	//15. 세번째 줄에 "사"를 입력한다.
	Glyph* elventhLetter = new DoubleByteLetter((char*)"사");
	cout << elventhLetter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(elventhLetter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	cout << "<노트전체출력>" << endl;
	cout << note->GetContent() << endl;
	//16. 세번째 줄에 "과"를 입력한다.
	Glyph* twelvethLetter = new DoubleByteLetter((char*)"과");
	cout << twelvethLetter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(twelvethLetter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	cout << "<노트전체출력>" << endl;
	cout << note->GetContent() << endl;
	//17. 세번째 줄에 "는"를 입력한다.
	Glyph* thirteenthLetter = new DoubleByteLetter((char*)"는");
	cout << thirteenthLetter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(thirteenthLetter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	cout << "<노트전체출력>" << endl;
	cout << note->GetContent() << endl;
	//18. 네번째 줄을 만든다.
	Glyph* fourthRow = new Row;
	rowIndex = note->Add(fourthRow);
	//19. 네번째 줄에 "맛"를 입력한다.
	Glyph* fourteenthLetter = new DoubleByteLetter((char*)"맛");
	cout << fourteenthLetter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(fourteenthLetter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	cout << "<노트전체출력>" << endl;
	cout << note->GetContent() << endl;
	//20. 네번째 줄에 "있"를 입력한다.
	Glyph* fifteenthLetter = new DoubleByteLetter((char*)"있");
	cout << fifteenthLetter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(fifteenthLetter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	cout << "<노트전체출력>" << endl;
	cout << note->GetContent() << endl;
	//21. 네번째 줄에 "다"를 입력한다.
	Glyph* sixteenthLetter = new DoubleByteLetter((char*)"다");
	cout << sixteenthLetter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(sixteenthLetter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	cout << "<노트전체출력>" << endl;
	cout << note->GetContent() << endl;
	//22. 네번째 줄에 "."를 입력한다.
	Glyph* seventeenthLetter = new DoubleByteLetter((char*)".");
	cout << seventeenthLetter->GetContent() << endl;
	letterIndex = note->GetAt(rowIndex)->Add(seventeenthLetter);
	glyph = note->GetAt(rowIndex)->GetAt(letterIndex);
	cout << glyph->GetContent() << endl;
	glyph = note->GetAt(rowIndex);
	cout << glyph->GetContent() << endl;
	cout << "<노트전체출력>" << endl;
	cout << note->GetContent() << endl;
#endif
}