#include "RowAutoChange.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "TextExtent.h"
#include "DummyRow.h"

//디폴트생성자
RowAutoChange::RowAutoChange(NotepadForm* notepadForm)
{
	this->notepadForm = notepadForm;
}

//메모장의 전체 줄을 자동개행 시키다
void RowAutoChange::DoAllRows()
{
	Long letterIndex = 0;
	Long rowTextWidth = 0;
	Glyph* glyph = 0;
	Long rowIndex = 0;
	Glyph* row = 0;
	Glyph* previousRow = 0;
	
	//1. 현재 화면의 크기를 구한다.
	CRect rect;
	this->notepadForm->GetClientRect(&rect);
	//2. 현재 화면의 가로 길이를 구한다.
	Long pageWidth = rect.Width();
	//3. Note의 총 줄의 개수보다 작은동안 반복한다.
	while (rowIndex < this->notepadForm->note->GetLength())
	{
		//3.1 메모장에서 rowIndex번째 줄을 구한다.
		row = this->notepadForm->note->GetAt(rowIndex);
		//3.2 letterIndex를 원위치시킨다.
		letterIndex = 0;
		//3.3 rowTextWidth를 원위치시킨다.
		rowTextWidth = 0;
		//3.4 letterIndex가 rowIndex번째 줄의 총글자 개수보다 작은동안 
		//그리고 rowIndex번째 줄의 가로길이가 현재화면의 가로길이보다 작은동안 반복한다.
		while (letterIndex < row->GetLength() && rowTextWidth < pageWidth)
		{
			//3.4.1 증가된 letterIndex까지의 가로 길이를 측정한다.
			rowTextWidth = this->notepadForm->textExtent->GetTextWidth
			(row->GetPartOfContent(letterIndex + 1));
			//3.4.2 letterIndex를 증가시킨다.
			letterIndex++;
		}
		//3.5 rowIndex번째 줄의 가로 길이가 현재 화면의 가로 길이보다 크거나 같으면
		if (rowTextWidth >= pageWidth)
		{
			//3.5.1 letterIndex까지의 길이가 현재화면의 가로 길이(cx)보다 크기 때문에 
			//이 선택문에 들어왔다. 그래서 캐럿이 이전으로 한 칸 이동을 해서 길이를 재면
			//현재화면의 가로 길이(cx)보다 작다. 캐럿(letterIndex)은 다음 글자를 적을 위치를
			//반영하기 때문에 항상 현재 글자보다 한칸 앞서 있다
			//그래서 letterIndex-1에서 split을 해야 화면을 넘는 글자를 다음 줄로 보낼 수 있다.
			letterIndex--;
			//3.5.2 rowIndex번째 줄의 가로 길이가 현재화면의 가로 길이보다 커진 시점의
			//글자부터 rowIndex번째 줄에서 letterIndex 다음 위치에 있는 글자들을 나눈다.
			//(DummyRow생성)
			glyph = row->Split(letterIndex, true);
			//3.5.3 새로운 줄을 rowIndex번째 줄의 다음 위치에 끼워넣는다.
			rowIndex = this->notepadForm->note->Add(rowIndex + 1, glyph);
		}
		//3.6 letterIndex가 rowIndex번째 줄의 총글자 개수보다 크거나 같으면
		else if (letterIndex >= row->GetLength())
		{
			//3.6.1 다음 줄로 이동한다.
			rowIndex++;
		}
	}
}

//메모장의 전체 줄을 자동개행 취소하다
void RowAutoChange::UndoAllRows()
{
	Long rowIndex = 0;
	Glyph* row = 0;
	Glyph* previousRow = 0;

	//1. Note의 총 줄의 개수보다 작은동안 반복한다.
	while (rowIndex < this->notepadForm->note->GetLength())
	{
		//1.1 메모장에서 rowIndex번째 줄을 구한다.
		row = this->notepadForm->note->GetAt(rowIndex);
		//1.2 가짜줄(DummyRow)이면
		if (dynamic_cast<DummyRow*>(row))
		{
			//1.2.1 가짜줄(DummyRow) 이전의 진짜줄(Row)을 구한다.
			previousRow = this->notepadForm->note->GetAt(rowIndex - 1);
			//1.2.2 가짜줄(DummyRow)을 이전의 진짜줄(Row)에 합친다.
			row->Join(previousRow);
			//1.2.3 Note에서 가짜줄(DummyRow)의 주소를 지운다.
			this->notepadForm->note->Remove(rowIndex);
		}
		//1.3 가짜줄(DummyRow)이 아니면(진짜줄(Row)이면)
		else
		{
			//1.3.1 다음 줄로 이동한다.
			rowIndex++;
		}
	}
}

//한 줄만 자동개행시키다
void RowAutoChange::DoRow()
{
	//1. 현재 줄의 위치와 현재 글자 위치 그리고 진짜 줄의 위치를 입력받는다.
	//2. 진짜 줄을 구한다.
	Long i = this->notepadForm->note->GetCurrent();
	Glyph* realRow = this->notepadForm->note->GetAt(i);
	//3. 현재 화면의 크기를 구한다.
	CRect rect;
	this->notepadForm->GetClientRect(&rect);
	//4. 현재 화면의 가로 길이를 구한다.
	Long pageWidth = rect.Width();
	//진짜 줄 한 줄이 현재 화면의 가로 길이에 맞게 자동개행된 후에 몇 줄이 될지 정한다.
	//5. i가 count보다 작거나 같은 동안 반복한다.
	Long count = i + 1;
	Long letterIndex = 0;
	Long rowTextWidth = 0;
	Glyph* row = 0;
	Glyph* dummyRow = 0;
	while (i < count)
	{
		//5.1 메모장에서 i번째 줄을 구한다.
		row = this->notepadForm->note->GetAt(i);
		//5.2 letterIndex를 원위치시킨다.
		letterIndex = 0;
		//5.3 rowTextWidth를 원위치시킨다.
		rowTextWidth = 0;
		//5.4 letterIndex가 rowIndex번째 줄의 총글자 개수보다 작은동안 
		//그리고 rowIndex번째 줄의 가로길이가 현재화면의 가로길이보다 작은동안 반복한다.
		while (letterIndex < row->GetLength() && rowTextWidth < pageWidth)
		{
			//5.4.1 증가된 letterIndex까지의 가로 길이를 측정한다.
			rowTextWidth = this->notepadForm->textExtent->GetTextWidth
			(row->GetPartOfContent(letterIndex + 1));
			//5.4.2 letterIndex를 증가시킨다.
			letterIndex++;
		}
		//5.5 rowIndex번째 줄의 가로 길이가 현재 화면의 가로 길이보다 크거나 같으면
		if (rowTextWidth >= pageWidth)
		{
			//5.5.1 letterIndex까지의 길이가 현재화면의 가로 길이(cx)보다 크기 때문에 
			//이 선택문에 들어왔다. 그래서 캐럿이 이전으로 한 칸 이동을 해서 길이를 재면
			//현재화면의 가로 길이(cx)보다 작다. 캐럿(letterIndex)은 다음 글자를 적을 위치를
			//반영하기 때문에 항상 현재 글자보다 한칸 앞서 있다
			//그래서 letterIndex-1에서 split을 해야 화면을 넘는 글자를 다음 줄로 보낼 수 있다.
			letterIndex--;
			//5.5.2 rowIndex번째 줄의 가로 길이가 현재화면의 가로 길이보다 커진 시점의
			//글자부터 rowIndex번째 줄에서 letterIndex 다음 위치에 있는 글자들을 나눈다.
			//(DummyRow생성)
			dummyRow = row->Split(letterIndex, true);
			//5.5.3 새로운 줄을 rowIndex번째 줄의 다음 위치에 끼워넣는다.
			i = this->notepadForm->note->Add(i + 1, dummyRow);
			//5.5.4 count를 세준다.
			count++;
		}
		//5.6 letterIndex가 rowIndex번째 줄의 총글자 개수보다 크거나 같으면
		else if (letterIndex >= row->GetLength())
		{
			//5.6.1 i를 증가시킨다.
			i++;
		}
	}
	//6. 끝내다.
}

//한 줄만 자동개행 취소하다
void RowAutoChange::UndoRow()
{
	//1. 현재 줄을 구한다.
	Long i = this->notepadForm->note->GetCurrent();
	Glyph* row = this->notepadForm->note->GetAt(i);
	//진짜 줄을 찾는다.
	//2. 가짜줄인동안 반복한다.
	while (dynamic_cast<DummyRow*>(row))
	{
		//2.1 i를 감소시킨다.
		i--;
		//2.2 i번째 줄을 구한다.
		row = this->notepadForm->note->GetAt(i);
	}
	//3. 진짜 줄을 저장한다.
	Glyph* realRow = this->notepadForm->note->GetAt(i);
	//4. 찾은 진짜 줄의 다음 줄을 구한다.
	i++;
	row = this->notepadForm->note->GetAt(i);
	//다음 진짜 줄이 나오기전까지 가짜줄들을 모두 Join시킨다.
	//6. 가짜줄인동안 반복한다.
	while (dynamic_cast<DummyRow*>(row))
	{
		//6.1 가짜줄을 진짜 줄에 Join시킨다.
		row->Join(realRow);
		//6.2 Note에서 가짜줄(DummyRow)의 주소를 지운다.
		this->notepadForm->note->Remove(i);
		//6.3 row를 구한다.
		row = this->notepadForm->note->GetAt(i);
	}
	//7. 끝내다
}

//자동개행 후 줄과 캐럿의 위치를 통해 자동개행 전 원래 줄과 캐럿의 위치를 구한다
void RowAutoChange::GetOriginPos(Long changedLetterPos, Long changedRowPos,
	Long* originLetterPos, Long* originRowPos)
{
	//1. 자동개행 후 줄과 캐럿의 위치를 입력받는다.
	//2. 자동개행 후 캐럿이 있는 곳까지 줄의 개수를 구한다.
	Long rowCount = changedRowPos + 1;
	//자동개행 전에 원래 줄의 위치를 구한다.
	//진짜 줄(Row)이면 count와 i 둘다 세고, 가짜 줄(DummyRow)이면 i만 센다.
	//3. i가 rowCount보다 작은동안 반복한다.
	Glyph* row = 0;
	Long count = 0;
	Long i = 0;
	while (i < rowCount)
	{
		//3.1 진짜줄(Row)인지 가짜줄(DummyRow)인지 판단하기 위해 줄을 구한다.
		row = this->notepadForm->note->GetAt(i);
		//3.2 진짜줄(Row)이면
		//if(dynamic_cast<Row*>(row))로 하면 DummyRow가 Row의 자식이기 때문에
		//Row나 DummyRow나 둘다 선택문에 들어가게 되기 때문에 조건을 바꿔서
		//DummyRow가 아니면 조건문에 들어가게 만들어야 Row만 조건문에 들어가고 DummyRow는 안들어감!
		if (!dynamic_cast<DummyRow*>(row))
		{
			//3.2.1 count를 센다.
			count++;
		}
		//3.3 i를 센다.
		i++;
	}
	//4. 자동개행전 줄의 위치를 저장한다.(출력한다.)
	//count는 개수이므로 1베이스이고 originRowPos는 위치이므로 0베이스이기 때문에 (개수-1)이 필요함!
	*originRowPos = count - 1;
	//자동개행 전 원래 캐럿의 위치를 구한다.
	//5. 자동개행 후 줄의 위치를 입력해 줄을 구한다.
	row = this->notepadForm->note->GetAt(changedRowPos);
	//6. 구한 줄의 진짜 줄(Row)이 있는 위치를 알기 위해 가짜줄(DummyRow)인동안 반복한다.
	//구한 줄이 바로 진짜 줄(Row)이면 반복구조에 들어가지 않고 구한 줄이 가짜줄(DummyRow)이면
	//진짜줄(Row)의 위치를 찾을 때까지 반복한다.
	Long realRowPos = changedRowPos;
	while (dynamic_cast<DummyRow*>(row))
	{
		//6.1 줄의 위치를 감소시킨다.
		realRowPos--;
		//6.2 줄을 구한다.
		row = this->notepadForm->note->GetAt(realRowPos);
	}
	//7. 자동개행 후 줄의 위치가 진짜 줄의 위치가 아니면
	if (changedRowPos != realRowPos)
	{
		//진짜줄(Row)부터 시작해서 가짜줄(DummyRow)의 changedCaretPos까지 캐럿의 위치를 더해준다.
		//7.1 i가 자동개행 후 줄의 위치보다 작은동안 반복한다.
		Long letterCount = 0;
		i = realRowPos;
		while (i < changedRowPos)
		{
			//7.3.1 줄을 구한다.
			row = this->notepadForm->note->GetAt(i);
			//7.3.2 글자 개수를 구한다.
			letterCount = row->GetLength();
			//7.3.3 자동개행 후 캐럿의 위치에 글자개수를 더해준다.
			changedLetterPos += letterCount;
			//7.3.4 i를 증가시킨다.
			i++;
		}
	}
	//8. 자동개행 전 원래 캐럿의 위치를 저장한다.(출력한다.)
	*originLetterPos = changedLetterPos;
}

//자동개행 전 원래 줄과 캐럿의 위치를 통해 자동개행 후 줄과 캐럿의 위치를 구한다.
void RowAutoChange::GetChangedPos(Long originLetterPos, Long originRowPos,
	Long* changedLetterPos, Long* changedRowPos)
{
	//1. 자동개행 전 원래 줄과 캐럿의 위치를 입력받는다.
	//2. 자동개행 전 캐럿이 있는 곳까지 줄의 개수를 구한다.
	Long rowCount = originRowPos + 1;
	//가짜줄(DummyRow)이면 rowCount와 i를 둘다세고, 진짜줄(Row)이면 i만 센다.
	//3. i가 rowCount보다 작은동안 반복한다.
	Long realRowPos = 0;
	Glyph* row = 0;
	Long rowCountOfNote = this->notepadForm->note->GetLength();
	Long i = 0;
	Long rowIndex = 0;
	while (i < rowCount && rowIndex < rowCountOfNote)
	{
		//3.1 row를 구한다.
		row = this->notepadForm->note->GetAt(rowIndex);
		//3.2 row가 진짜 줄이면
		//if(dynamic_cast<Row*>(row))로 하면 DummyRow가 Row의 자식이기 때문에
		//Row나 DummyRow나 둘다 선택문에 들어가게 되기 때문에 조건을 바꿔서
		//DummyRow가 아니면 조건문에 들어가게 만들어야 Row만 조건문에 들어가고 DummyRow는 안들어감!
		if (!dynamic_cast<DummyRow*>(row))
		{
			//3.2.1 반복제어변수를 센다.
			i++;
		}
		//3.3 rowIndex를 센다.
		rowIndex++;
	}
	//진짜 줄의 위치를 구한다.
	//4. i가 rowCount보다 크거나 같으면
	if (i >= rowCount)
	{
		//4.1 진짜줄의 위치를 저장한다.
		realRowPos = rowIndex - 1;
	}
	//5. 현재 줄의 글자개수를 구한다.
	i = realRowPos;
	row = this->notepadForm->note->GetAt(realRowPos);
	Long letterCount = row->GetLength();
	//5. 원래 캐럿의 위치가 현재 줄의 글자개수보다 큰동안 반복한다.
	while (originLetterPos > letterCount)
	{
		//5.1 원래 캐럿의 위치에 현재 줄의 글자개수를 뺀다.
		originLetterPos -= letterCount;
		//5.2 i를 증가시킨다.
		i++;
		//5.3 row를 구한다.
		row = this->notepadForm->note->GetAt(i);
		letterCount = row->GetLength();
	}
	//6. 자동개행후 줄의 위치를 저장한다.(출력한다.)
	*changedRowPos = i;
	//7. 자동개행 후 캐럿의 위치를 저장한다.(출력한다.)
	*changedLetterPos = originLetterPos;
	//8. 끝내다.
}

//소멸자
RowAutoChange::~RowAutoChange()
{

}