#include "GlyphReplacement.h"
#include "Glyph.h"

//디폴트생성자
GlyphReplacement::GlyphReplacement(Glyph* note)
{
	this->note = note;
}

//바꾸기
void GlyphReplacement::Replace(string keyword, Long startSelectedRowIndex,
	Long startSelectedLetterIndex)
{
	//1. 현재 줄의 위치를 구한다.(현재 줄의 위치가 선택이 끝나는 줄의 위치임)
	Long endSelectedRowIndex = this->note->GetCurrent();
	//2. 현재 글자의 위치를 구한다.(현재 글자의 위치가 선택이 끝나는 글자의 위치임)
	Long endSelectedLetterIndex = this->note->GetAt(endSelectedRowIndex)->GetCurrent();
	Long letterIndex = startSelectedLetterIndex;
	Long rowIndex = startSelectedRowIndex;
	Glyph* startRow = this->note->GetAt(rowIndex);
	//3. 선택이 된 줄이 한줄이면
	if (startSelectedRowIndex == endSelectedRowIndex)
	{
		//3.1 선택이 시작되는 글자부터 끝나는 글자까지 글자를 지운다.
		while (letterIndex < endSelectedLetterIndex)
		{
			//3.1.1 글자를 지운다.
			startRow->Remove(letterIndex);
			//3.1.2 글자를 지웠기 때문에 끝나는 글자를 앞으로 한 칸 당겨준다.
			endSelectedLetterIndex--;
		}
	}
	//4. 선택이 된 줄이 여러줄이면
	else
	{
		//4.1 선택이 시작되는 줄의 개수보다 작은동안 반복한다.
		while (letterIndex < startRow->GetLength())
		{
			//4.1.1 글자를 지운다.
			startRow->Remove(letterIndex);
		}
		//4.2 선택이 끝나는 줄 전까지 반복한다.
		Glyph* row = 0;
		rowIndex++;
		while (rowIndex < endSelectedRowIndex)
		{
			//4.2.1 줄을 구한다.
			row = this->note->GetAt(rowIndex);
			//4.2.2 글자 위치를 초기화시킨다.
			letterIndex = 0;
			//4.2.3 글자 위치가 줄의 개수보다 작은동안 반복한다.
			while (letterIndex < row->GetLength())
			{
				//4.2.3.1 글자를 지운다.
				row->Remove(letterIndex);
			}
			//4.2.4 줄에 내용인 글자가 다 지워졌기 때문에 줄을 지운다.
			this->note->Remove(rowIndex);
			//4.2.5 줄이 지워졌기 때문에 endSelectedRowIndex를 한줄 앞으로 당겨준다.
			endSelectedRowIndex--;
		}
		//4.3 선택이 끝나는 줄의 선택이 끝나는 글자까지 반복한다.
		letterIndex = 0;
		Glyph* endRow = this->note->GetAt(endSelectedRowIndex);
		while (letterIndex < endSelectedLetterIndex)
		{
			//4.3.1 글자를 지운다.
			endRow->Remove(letterIndex);
			//4.3.2 글자를 지웠기 때문에 endSelectedLetterIndex를 앞으로 한칸 당긴다.
			endSelectedLetterIndex--;
		}
		//4.4 선택이 끝나는 마지막 줄의 선택이 끝나는 글자 이후의 글자들을 선택이 시작되는 줄에 Join시킨다.
		endRow->Join(startRow);
		//4.5 마지막 줄을 메모장에서 지운다.
		this->note->Remove(endSelectedRowIndex);
	}
	//글자를 다지웠기 때문에 이제 바꿀 단어를 추가한다.
	
}

//모두 바꾸기

//소멸자
GlyphReplacement::~GlyphReplacement()
{

}