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

//자동개행을 하다
void RowAutoChange::Do()
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
		while (letterIndex < row->GetLength() && rowTextWidth <= pageWidth)
		{
			//3.4.1 증가된 letterIndex까지의 가로 길이를 측정한다.
			rowTextWidth = this->notepadForm->textExtent->GetTextWidth
			(row->GetPartOfContent(letterIndex + 1));
			//3.4.2 letterIndex를 증가시킨다.
			letterIndex++;
		}
		//3.5 rowIndex번째 줄의 가로 길이가 현재 화면의 가로 길이보다 크면
		if (rowTextWidth > pageWidth)
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

//자동개행을 취소하다
void RowAutoChange::Undo()
{
	Long rowIndex = 0;
	Glyph* row = 0;
	Glyph* previousRow = 0;

	//1. Note의 총 줄의 개수보다 작은동안 반복한다.
	while (rowIndex < this->notepadForm->note->GetLength())
	{
		//1.1 메모장에서 rowIndex번째 줄을 구한다.
		row = this->notepadForm->note->GetAt(rowIndex);
		//1.2 DummyRow이면
		if (dynamic_cast<DummyRow*>(row))
		{
			//1.2.1 DummyRow 이전 줄(Row)을 구한다.
			previousRow = this->notepadForm->note->GetAt(rowIndex - 1);
			//1.2.2 DummyRow를 이전 줄(Row)에 합친다.
			row->Join(previousRow);
			//1.2.3 Note에서 DummyRow의 주소를 지운다.
			this->notepadForm->note->Remove(rowIndex);
		}
		//1.3 DummyRow가 아니면
		else
		{
			//1.3.1 다음 줄로 이동한다.
			rowIndex++;
		}
	}
}

//자동개행 후 줄과 캐럿의 위치를 통해 자동개행 전 원래 줄과 캐럿의 위치를 구한다
void RowAutoChange::GetOriginPos(Long changedCaretPos, Long changedRowPos,
	Long* originCaretPos, Long* originRowPos)
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
			changedCaretPos += letterCount;
			//7.3.4 i를 증가시킨다.
			i++;
		}
	}
	//8. 자동개행 전 원래 캐럿의 위치를 저장한다.(출력한다.)
	*originCaretPos = changedCaretPos;

#if 0
	//자동개행 후 줄의 위치를 구해 줄을 구하고 진짜줄(Row)가 나올 동안 -1을 하면서 거슬러 올라간다.
	//5. 자동개행 후 줄의 위치를 입력해 줄을 구한다.
	row = this->notepadForm->note->GetAt(changedRowPos);
	//6. 구한 줄이 가짜 줄인 동안 반복한다.
	while (dynamic_cast<DummyRow*>(row))
	{
		//6.1 자동개행 후 캐럿의 위치에서 줄의 글자개수만큼 더해준다.
		//개수는 줄마다 다르다 영문만 있는 줄이 한글만 있는 줄이나 한글이랑 영문이랑 섞인 줄보다
		//개수가 더많다 즉, 줄마다 개수가 달라지기 때문에 줄을 더해주면 안되고 줄마다 길이를 구해서
		//그에 맞게 개수를 더해줘야한다.!!
		changedCaretPos += row->GetLength();
		//6.2 줄의 위치를 감소시킨다.
		changedRowPos--;
		//6.3 줄을 구한다.
		row = this->notepadForm->note->GetAt(changedRowPos);
	}
	//7. 자동개행 전 줄의 위치를 저장한다.(출력한다.)
	*originCaretPos = changedCaretPos;
	//8. 끝내다.
#endif
#if 0
	//2. 현재 줄을 구한다.
	Glyph *row= this->notepadForm->note->GetAt(changedRowPos);
	//3. 현재 줄이 가짜줄인동안 반복한다.
	Long rowIndex = changedRowPos;
	while (dynamic_cast<DummyRow*>(row))
	{
		//3.1 줄의 위치를 감소시킨다.
		rowIndex--;
		//3.2 줄을 구한다.
		row = this->notepadForm->note->GetAt(rowIndex);
	}
	//4. 자동개행 전 원래 줄의 위치를 저장한다.
	*originRowPos = rowIndex;
	//5. 줄의 위치가 입력 받은 현재 줄의 위치보다 작은동안 반복한다.
	Long caretIndex = 0;
	Long count = 0;// originCaretPos
	while (rowIndex < changedRowPos)
	{
		//5.1 줄을 구한다.
		row = this->notepadForm->note->GetAt(rowIndex);
		//5.2 줄에서 캐럿을 처음으로 보낸다.
		caretIndex = row->First();
		//5.3 캐럿이 현재 줄의 글자개수보다 작은동안 반복한다.
		while (caretIndex < row->GetLength())
		{
			//5.3.1 현재 줄에서 캐럿을 다음으로 보낸다.
			caretIndex = row->Next();
			//5.3.2 count를 센다.
			count++;
		}
		//5.4 현재 줄을 1 증가시킨다.
		rowIndex++;
	}
	//6. 줄을 구한다.(rowIndex=changedRowPos)
	row = this->notepadForm->note->GetAt(rowIndex);
	//7. 줄에서 캐럿을 처음으로 보낸다.
	caretIndex = row->First();
	//8. 캐럿이 입력받은 캐럿의 위치보다 작은동안 반복한다.
	while (caretIndex < changedCaretPos)
	{
		//8.1 현재 줄에서 캐럿을 다음으로 보낸다.
		caretIndex = row->Next();
		//8.2 count를 센다.
		count++;
	}
	//9. 자동개행 전 원래 캐럿의 위치를 저장한다.
	*originCaretPos = count;
	//10. 자동개행 전 원래 줄의 위치와 자동개행 전 원래 캐럿의 위치를 출력한다.
	//11. 끝내다.
#endif
}

//자동개행 전 원래 줄과 캐럿의 위치를 통해 자동개행 후 줄과 캐럿의 위치를 구한다.
void RowAutoChange::GetChangedPos(Long originCaretPos, Long originRowPos,
	Long* changedCaretPos, Long* changedRowPos)
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
	while (originCaretPos > letterCount)
	{
		//5.1 원래 캐럿의 위치에 현재 줄의 글자개수를 뺀다.
		originCaretPos -= letterCount;
		//5.2 i를 증가시킨다.
		i++;
		//5.3 row를 구한다.
		row = this->notepadForm->note->GetAt(i);
		letterCount = row->GetLength();
	}
	//6. 자동개행후 줄의 위치를 저장한다.(출력한다.)
	*changedRowPos = i;
	//7. 자동개행 후 캐럿의 위치를 저장한다.(출력한다.)
	*changedCaretPos = originCaretPos;
	//8. 끝내다.
#if 0
	//1. 자동개행 전 원래 줄과 캐럿의 위치를 입력받는다.
	//2. 자동개행 전 원래 줄의 위치(originRowPos)보다 작거나 같은 동안
	//그리고 rowIndex가 note의 줄의 개수보다 작은동안 반복한다.
	Long rowLength = this->notepadForm->note->GetLength();
	Long nextRowPos = originRowPos + 1;
	Long rowIndex = 0;
	Glyph* row = 0;
	Long i = 0;
	while (i <= nextRowPos && rowIndex < rowLength)
	{
		//2.1 row를 구한다.
		row = this->notepadForm->note->GetAt(rowIndex);
		//2.2 row가 진짜 줄이면
		if (dynamic_cast<Row*>(row))
		{
			//2.2.1 반복제어변수를 센다.
			i++;
		}
		//2.3 rowIndex를 센다.
		rowIndex++;
	}
	//3. rowIndex를 통해 현재 줄을 구한다.
	i = rowIndex;
	row = this->notepadForm->note->GetAt(i);
	//4. 현재줄이 가짜줄인동안 반복한다.
	while (dynamic_cast<DummyRow*>(row))
	{
		//4.1 줄의 위치를 감소시킨다.
		i--;
		//4.2 줄을 구한다.
		row = this->notepadForm->note->GetAt(i);
	}
	//5. i가 rowIndex보다 작은동안 반복한다.
	Long count = originCaretPos;
	row = this->notepadForm->note->GetAt(i);
	rowLength = row->GetLength();
	while (i < rowIndex)
	{
		//5.1 현재 캐럿의 위치에서 현재 줄의 개수만큼 뺀다.
		count -= rowLength;
		//5.2 i를 증가시킨다.
		i++;
	}
	//6. rowIndex와 count를 출력한다.
	*changedRowPos = rowIndex;
	*changedCaretPos = count;
	//7. 끝내다.
#endif
}

//소멸자
RowAutoChange::~RowAutoChange()
{

}