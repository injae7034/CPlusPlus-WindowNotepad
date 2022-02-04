#include "GlyphFinder.h"
#include "Glyph.h"
#include "DummyRow.h"
#include "RowAutoChange.h"

//#include<Findtext.dlg> 찾기공통대화상자를 커스터마이징하기 위해서 필요함
//이걸 적어 놓고 Ctrl + F5 를 하면 오류가 나는데 오류목록을 클릭해서 FindText.Dlg에 들어가서
//FindDialog의 내용을 복사해서 resource에서 Dialog를 생성한뒤 거기에 복사한 내용을
//붙여넣으면 커스터마이징 된 찾기 대화상자가 생성됨!

/*
//
//  Find and Replace Dialogs.
//

FINDDLGORD DIALOG LOADONCALL MOVEABLE DISCARDABLE 30, 73, 236, 74
STYLE WS_BORDER | WS_CAPTION | DS_MODALFRAME | WS_POPUP | WS_SYSMENU |
      DS_3DLOOK
CAPTION "Find"
FONT 8, "MS Shell Dlg"
BEGIN
    LTEXT           "Fi&nd what:", -1, 4, 8, 42, 8
    EDITTEXT        edt1, 47, 7, 128, 12, WS_GROUP | WS_TABSTOP | ES_AUTOHSCROLL

    AUTOCHECKBOX    "Match &whole word only", chx1, 4, 26, 100, 12, WS_GROUP
    AUTOCHECKBOX    "Match &case", chx2, 4, 42, 64, 12
    AUTOCHECKBOX    "W&rap around", chx3, 4, 58, 64, 12

    GROUPBOX        "Direction", grp1, 107, 26, 68, 28, WS_GROUP
    AUTORADIOBUTTON "&Up", rad1, 111, 38, 25, 12, WS_GROUP
    AUTORADIOBUTTON "&Down", rad2, 138, 38, 35, 12

    DEFPUSHBUTTON   "&Find Next", IDOK, 182, 5, 50, 14, WS_GROUP
    PUSHBUTTON      "Cancel", IDCANCEL, 182, 23, 50, 14
    PUSHBUTTON      "&Help", pshHelp, 182, 45, 50, 14

    LTEXT           "", stc1, 4, 56, 1, 1, NOT WS_VISIBLE
END

*/


//디폴트생성자
GlyphFinder::GlyphFinder(Glyph* note)
{
	this->note = note;
}

//아래로 찾기
void GlyphFinder::FindDown(string keyword, Long* findingRowIndex, Long* findingLetterIndex,
	Long* keywordLetterCount)
{
	//1. 찾을 문자열을 입력받는다.
	//2. 현재 줄의 위치와 글자 위치를 구한다.
    Long currentRowIndex = this->note->GetCurrent();
    Long currentLetterIndex = this->note->GetAt(currentRowIndex)->GetCurrent();
    string currentLetters = this->note->GetAt(currentRowIndex)
        ->GetPartOfContent(currentLetterIndex);
    Long currentLettersLength = currentLetters.length();
    //3. 현재 줄의 위치가 note의 줄의 개수보다 작은동안 그리고 해당문자열을 찾을 때까지 반복한다.
    bool isMatched = false;
    *findingRowIndex = -1;
    *findingLetterIndex = -1;
    *keywordLetterCount = 0;// 문자열의 길이말고 찾을 문자열의 글자개수
    Long matchedLettersCount = 0;
    Long keywordLength = keyword.length();// 찾을 문자열의 길이(한글은 2, 영문 및 특수문자는 1)
    Long matchedLength = 0;// 찾을 문자열과 줄의 글자들이 서로 일치하는 문자열의 길이
    Glyph* row = 0;
    Glyph* nextRow = 0;
    string letterContentOfRow;
    string partContentOfKeyword;
    Long nextRowIndex = 0;
    string realRowContent;
    string dummyRowContent;
    Long rowContentLength = 0;
    Long contentLength = 0;
    Long i = 0;//줄의 content에서 글자를 읽을 배열요소 및 반복제변수
    Long j = 0;//찾을 문자열의 content에서 글자를 읽을 배열요소 및 반복제어변수
    while (currentRowIndex < this->note->GetLength() && isMatched == false)
    {
        //3.1 줄을 구한다.
        row = this->note->GetAt(currentRowIndex);
        //3.2 줄의 content를 구한다.
        realRowContent = row->GetContent();
        //3.3 다음 줄을 구한다.
        nextRowIndex = currentRowIndex + 1;
        nextRow = this->note->GetAt(currentRowIndex + 1);
        //3.4 다음 줄의 위치가 줄의 개수보다 작은동안 그리고 다음 줄이 가짜 줄인 동안 반복한다.
        while (nextRowIndex < this->note->GetLength() &&
            dynamic_cast<DummyRow*>(nextRow))
        {
            //3.4.1 가짜줄의 content를 구한다.
            dummyRowContent = nextRow->GetContent();
            //3.4.2 가짜줄의 content를 진짜 줄의 content에 더해준다.
            realRowContent += dummyRowContent;
            //3.4.3 다음 줄로 이동한다.
            nextRowIndex++;
            //3.4.4 다음 줄을 구한다.
            nextRow = this->note->GetAt(nextRowIndex);
        }

        currentRowIndex = nextRowIndex - 1;

        //3.5 줄의 content 길이를 구한다.
        rowContentLength = realRowContent.length();
        //3.6 줄에서 읽을 글자 위치를 초기화시킨다.
        i = currentLettersLength;
        //3.7 줄의 content 길이만큼 그리고 해당 문자열을 찾을때까지 반복한다.
        while (i < rowContentLength && isMatched == false)
        {
            //3.7.2 줄에서 i번째 읽을 글자가 한글이면
            if ((realRowContent[i] & 0x80))//한글이면(2byte문자)
            {
                //3.7.2.1 2byte로 저장한다.
                letterContentOfRow = realRowContent[i];
                i++;
                letterContentOfRow += realRowContent[i];
            }
            //3.7.3 한글이 아니면
            else
            {
                //3.7.3.1 1byte로 저장한다
                letterContentOfRow = realRowContent[i];
            }
            //3.7.4 찾을 문자열에서 j번째 읽을 글자가 한글이면
            if ((keyword[j] & 0x80))//한글이면(2byte문자)
            {
                //3.7.4.1 2byte로 저장한다.
                partContentOfKeyword = keyword[j];
                j++;
                partContentOfKeyword += keyword[j];
                //3.7.4.2 길이를 2로 설정한다(한글이 길이를 2 차지하기 때문에)
                contentLength = 2;
            }
            //3.7.5 한글이 아니면
            else
            {
                //3.7.5.1 1byte로 저장한다
                partContentOfKeyword = keyword[j];
                //3.7.5.2 길이를 1로 설정한다.(한글을 제외한 영문 및 특수문자는 길이가 1이기 때문에)
                contentLength = 1;
            }
            //3.7.6 줄에서 읽은 글자와 찾을 문자열에서 읽을 글자가 서로 일치하면
            if (letterContentOfRow == partContentOfKeyword)
            {
                //3.7.6.1 일치하는 문자열의 길이를 세준다.
                matchedLength += contentLength;
                //3.7.6.2 일치하는 문자열의 글자 개수를 세준다.
                matchedLettersCount++;
                //3.7.6.3 찾을 문자열에서 읽을 글자위치를 다음으로 이동시킨다.
                j++;
            }
            //3.7.7 줄에서 읽은 글자와 찾을 문자열에서 읽을 글자가 서로 다르면
            else
            {
                //3.7.7.1 일치하는 문자열의 길이를 0으로 초기화해준다.
                matchedLength = 0;
                //3.7.7.2 일치하는 문자열의 글자 개수를 0으로 초기화해준다.
                matchedLettersCount = 0;
                //3.7.7.3 찾을 문자열에서 읽을 글자위치를 처음으로 이동시킨다.
                j = 0;
            }
            //메모장에서 현재 글자의 위치를 다음으로 이동시킨다.
            currentLetterIndex++;
            //3.7.8 찾을 문자열을 row의 content에서 찾았으면
            //일치하는 문자열의 길이와 찾을 문자열의 길이가 일치하면
            if(matchedLength == keywordLength)
            {
                //3.7.8.1 찾았다고 표시한다.
                isMatched = true;
                //자동개행을 앞에서 이미 고려했기 때문에 여기서는 자동개행에 대해서 따로 해줄 필요X
                //3.7.8.2 자동개행이 되었든 안되었든 자동개행이 된 상태에서
                //줄의 위치와 글자위치를 구해준다.
                RowAutoChange rowAutoChange(this->notepadForm);
                Long changedRowPos = 0;
                Long changedLetterPos = 0;
                Long originRowPos = currentRowIndex;
                Long originLetterPos = currentLetterIndex;
                //3.7.8.3 변경된 화면 크기에 맞는 줄과 캐럿의 위치를 구한다.
                rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
                   &changedRowPos);
                //3.7.8.4 찾은 줄의 위치와 찾은 글자 위치, 그리고 찾은 문자열의 글자 개수를 출력한다.
                *findingRowIndex = currentRowIndex;
                *findingLetterIndex = currentLetterIndex;
                *keywordLetterCount = matchedLettersCount;
            }
            //3.7.9 줄의 content에서 다음 글자을 읽는다.
            i++;
        }
        //3.8 currentLettersLength를 0으로 초기화해준다.
        currentLettersLength = 0;
        currentLetterIndex = 0;
        //3.9 다음 줄로 이동한다.
        currentRowIndex++;
    }
    //4. 끝내다.
}

//소멸자
GlyphFinder::~GlyphFinder()
{

}