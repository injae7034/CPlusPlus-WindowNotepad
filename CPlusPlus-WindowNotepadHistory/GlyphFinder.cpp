#include "GlyphFinder.h"
#include "Glyph.h"
#include "DummyRow.h"
//#include<Findtext.dlg>// 찾기공통대화상자와 바꾸기공통대화상자를 커스터마이징하기 위해서 필요함
//이걸 적어 놓고 Ctrl + F5 를 하면 오류가 나는데 오류목록을 클릭해서 FindText.Dlg에 들어가서
//FindDialog나 ReplaceDialog의 내용을 복사해서 .rc를 메모장에서 텍스트로 열어서 거기에 복사한 내용을
//붙여넣으면 커스터마이징 된 찾기 또는 바꾸기 대화상자가 생성됨!

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
void GlyphFinder::FindDown(string keyword, Long* findingStartRowIndex,
    Long* findingStartLetterIndex, Long* findingEndRowIndex, Long* findingEndLetterIndex)
{
    //1. 찾을 문자열을 입력받는다.
    //2. 메모장에서 현재 줄의 위치를 구한다.
    Long currentRowIndex = this->note->GetCurrent();
    //3. 메모장에서 현재 글자의 위치를 구한다.
    Long currentLetterIndex = this->note->GetAt(currentRowIndex)->GetCurrent();
    //4. 메모장의 현재 줄에서 현재 글자까지의 content를 구한다.
    string currentLetters = this->note->GetAt(currentRowIndex)
        ->GetPartOfContent(currentLetterIndex);
    //5. 메모장의 현재 줄에서 현재 글자까지의 content의 길이를 구한다.
    Long currentLettersLength = currentLetters.length();
    //6. 찾을 문자열을 찾았는지 여부를 일단 못찾았기 때문에 false로 초기화한다.
    bool isMatched = false;
    //7. 사용할 변수들을 반복문 밖에서 미리 선언 및 초기화시킨다.
    Long matchedLettersCount = 0;//줄에서 찾은 글자와 찾을 문자열의 글자가 서로 일치하는 글자개수
    Long keywordLength = keyword.length();// 찾을 문자열의 길이(한글은 2, 영문 및 특수문자는 1)
    Long matchedLength = 0;// 찾을 문자열과 줄의 글자들이 서로 일치하는 문자열의 길이
    Glyph* row = 0;
    Glyph* nextRow = 0;
    string letterContentOfRow;//줄의 한 글자를 담을 공간
    string partContentOfKeyword;//찾을 문자열의 한 글자를 담을 공간
    Long nextRowIndex = 0;
    string realRowContent;//진짜 줄의 content를 담을 공간
    string dummyRowContent;//가짜 줄의 content를 담을 공감
    Long rowContentLength = 0;//줄의 content의 길이
    Long contentLength = 0;//찾을 문자열의 한글자의 길이
    Glyph* currentRow = 0;
    string currentRowContent;
    Long currentRowContentLength = 0;
    string nextRowContent;
    Long startLetterIndex = 0;
    Long previousRowIndex = 0;
    Long i = 0;//줄의 content에서 글자를 읽을 배열요소 및 반복제변수
    Long j = 0;//찾을 문자열의 content에서 글자를 읽을 배열요소 및 반복제어변수
    //8. 현재 줄의 위치가 note의 줄의 개수보다 작은동안 그리고 해당문자열을 찾을 때까지 반복한다.
    while (currentRowIndex < this->note->GetLength() && isMatched == false)
    {
        //8.1 줄을 구한다.
        row = this->note->GetAt(currentRowIndex);
        //8.2 줄의 content를 구한다.
        realRowContent = row->GetContent();
        //8.3 다음 줄을 구한다.
        nextRowIndex = currentRowIndex + 1;
        nextRow = this->note->GetAt(nextRowIndex);
        //자동개행 하는 것처럼 가짜 줄의 내용을 앞의 줄에 추가한다 왜냐하면 자동개행이 안된 상태에서는
        //한줄내에서만(진짜줄 한줄내에서) 찾고 선택하는게 가능하지만, 자동개행이 된 상태에서는 여러 가짜 
        //줄에서 걸쳐서 찾아지고, 선택이 가능하다(원래라면(자동개행이 안된 상태에서는)한줄이기 때문에)
        //8.4 다음 줄의 위치가 줄의 개수보다 작은동안 그리고 다음 줄이 가짜 줄인 동안 반복한다.
        while (nextRowIndex < this->note->GetLength() &&
            dynamic_cast<DummyRow*>(nextRow))
        {
            //8.4.1 가짜줄의 content를 구한다.
            dummyRowContent = nextRow->GetContent();
            //8.4.2 가짜줄의 content를 진짜 줄의 content에 더해준다.
            realRowContent += dummyRowContent;
            //8.4.3 다음 줄로 이동한다.
            nextRowIndex++;
            //8.4.4 다음 줄을 구한다.
            nextRow = this->note->GetAt(nextRowIndex);
        }
        //8.5 줄의 content 길이를 구한다.
        rowContentLength = realRowContent.length();
        //8.6 줄에서 읽을 글자 위치를 초기화시킨다.
        i = currentLettersLength;
        //8.7 줄의 content 길이만큼 그리고 해당 문자열을 찾을때까지 반복한다.
        while (i < rowContentLength && isMatched == false)
        {
            //8.7.1 줄에서 i번째 읽을 글자가 한글이면
            if ((realRowContent[i] & 0x80))//한글이면(2byte문자)
            {
                //8.7.1.1 2byte로 저장한다.
                letterContentOfRow = realRowContent[i];
                i++;
                letterContentOfRow += realRowContent[i];
            }
            //8.7.2 한글이 아니면
            else
            {
                //8.7.2.1 1byte로 저장한다
                letterContentOfRow = realRowContent[i];
            }
            //8.7.3 찾을 문자열에서 j번째 읽을 글자가 한글이면
            if ((keyword[j] & 0x80))//한글이면(2byte문자)
            {
                //8.7.3.1 2byte로 저장한다.
                partContentOfKeyword = keyword[j];
                j++;
                partContentOfKeyword += keyword[j];
                //8.7.3.2 길이를 2로 설정한다(한글이 길이를 2 차지하기 때문에)
                contentLength = 2;
            }
            //8.7.4 한글이 아니면
            else
            {
                //8.7.4.1 1byte로 저장한다
                partContentOfKeyword = keyword[j];
                //8.7.4.2 길이를 1로 설정한다.(한글을 제외한 영문 및 특수문자는 길이가 1이기 때문에)
                contentLength = 1;
            }
            //8.7.5 줄에서 읽은 글자와 찾을 문자열에서 읽을 글자가 서로 일치하면
            if (letterContentOfRow == partContentOfKeyword)
            {
                //8.7.5.1 일치하는 문자열의 길이를 세준다.
                matchedLength += contentLength;
                //8.7.5.2 일치하는 문자열의 글자 개수를 세준다.
                matchedLettersCount++;
                //8.7.5.3 찾을 문자열에서 읽을 글자위치를 다음으로 이동시킨다.
                j++;
            }
            //8.7.6 줄에서 읽은 글자와 찾을 문자열에서 읽을 글자가 서로 다르면
            else
            {
                //8.7.6.1 일치하는 문자열의 길이를 0으로 초기화해준다.
                matchedLength = 0;
                //8.7.6.2 일치하는 문자열의 글자 개수를 0으로 초기화해준다.
                matchedLettersCount = 0;
                //8.7.6.3 찾을 문자열에서 읽을 글자위치를 처음으로 이동시킨다.
                j = 0;
            }
            //8.7.7 메모장의 현재 줄에서 현재 위치의 글자를 읽었고 같은지 비교를 했기 때문에
            //현재 글자의 위치를 다음으로 이동시켜야 다음 글자를 읽고 찾을 문자열의 글자와 비교할 수 있다.
            currentLetterIndex++;
            //8.7.8 찾을 문자열을 row의 content에서 찾았으면
            //일치하는 문자열의 길이와 찾을 문자열의 길이가 일치하면
            if (matchedLength == keywordLength)
            {
                //8.7.8.1 해당 문자열을 줄에서 찾았다고 표시한다.
                isMatched = true;
                //8.7.8.2 현재 줄을 구한다.
                currentRow = this->note->GetAt(currentRowIndex);
                //8.7.8.3 현재 줄의 content를 구한다.
                currentRowContent = currentRow->GetContent();
                //8.7.8.4 현재 줄의 content의 length를 구한다.
                currentRowContentLength = currentRowContent.length();
                //8.7.8.5 i가 현재 줄의 content의 length보다 크거나 같은동안 반복한다.
                //아까 앞에서 부분자동개행을 취소하는 것처럼 가짜줄의 content들을 진짜줄의 content에
                //추가시켰지만 사실상 현재 메모장에서는 자동개행이 되어 있는 상태이기 때문에
                //자동개행이 된 메모장에서 찾을 문자열을 찾아서 선택을 해야하기 때문에 아까 합쳤던 각각의 
                //가짜줄들을 다시 개별적으로 구해서 합친 줄에서 읽은 글자위치 i와 비교해서 현재 줄의 위치를
                //다시 구해준다.
                while (i >= currentRowContentLength)
                {
                    //8.7.8.5.1 현재 줄의 위치를 증가시킨다.
                    //자동개행이 취소된 상태에서 해당 문자열을 찾았는데 최종적으로 읽은 글자가
                    //현재 개별적인 줄의 길이보다 길기 때문에 다음 줄로 넘어가서 다시 비교해야한다.
                    currentRowIndex++;
                    //8.7.8.5.2 현재 글자위치도 자동개행이 취소된 상태의 글자위치기 때문에 현재 글자 
                    //위치가 현재 줄의 개수보다 크기 때문에 그만큼 빼서 자동개행이 된 상태로 조정한다
                    //앞에서 currentLetterIndex++로 인해 1이 증가된 상태이기 때문에 +1을 따로 안해줘도됨!
                    currentLetterIndex -= currentRow->GetLength();
                    //8.7.8.5.3 현재 줄의 다음줄을 구한다.
                    currentRow = this->note->GetAt(currentRowIndex);
                    //8.7.8.5.4 다음 줄의 content를 저장한다.
                    nextRowContent = currentRow->GetContent();
                    //8.7.8.5.5 다음 줄의 content를 현재 줄의 content에 더해준다.(누적)
                    currentRowContent += nextRowContent;
                    //8.7.8.5.6 누적된 줄의 content의 길이를 구한다.
                    currentRowContentLength = currentRowContent.length();
                }
                //8.7.8.6 찾은 줄의 끝 위치와 찾은 글자 끝위치를 출력한다.
                *findingEndRowIndex = currentRowIndex;
                *findingEndLetterIndex = currentLetterIndex;
                //8.7.8.7 현재 글자 위치에서 찾은 문자열의 글자개수를 빼준다.
                startLetterIndex = currentLetterIndex - matchedLettersCount;
                //8.7.8.8 양수이면
                if (startLetterIndex >= 0)
                {
                    //8.7.8.8.1 찾은 줄의 시작위치를 찾을 줄의 끝 위치와 동일한 값으로 출력한다.
                    *findingStartRowIndex = currentRowIndex;
                    //8.7.8.8.2 찾은 글자의 시작위치를 뺀값을 대입해서 출력한다.
                    *findingStartLetterIndex = startLetterIndex;
                }
                //8.7.8.9 음수이면
                else
                {
                    //8.7.8.9.1 현재 줄의 이전 줄을 구한다.
                    previousRowIndex = currentRowIndex - 1;
                    row = this->note->GetAt(previousRowIndex);
                    //8.7.8.9.2 현재 줄에서 이전 줄로 이동했기 때문에 
                    //찾은 문자열의 글자개수에서 현재 줄의 글자위치를 빼준다
                    matchedLettersCount -= currentLetterIndex;
                    //8.7.8.9.3 은 문자열의 글자개수가 이전 줄의 글자개수보다 큰 동안 반복한다.
                    while (matchedLettersCount > row->GetLength())
                    {
                        //8.7.8.9.3.1 찾은 물자열의 글자개수에서 이전 줄의 글자개수를 빼준다.
                        matchedLettersCount -= row->GetLength();
                        //8.7.8.9.3.2 이전 줄을 구한다.
                        previousRowIndex--;
                        row = this->note->GetAt(previousRowIndex);
                    }
                    //8.7.8.9.4 찾은 줄의 시작위치를 이전 줄의 위치로 출력한다.
                    *findingStartRowIndex = previousRowIndex;
                    //8.7.8.9.5 찾은 글자의 시작위치를 이전 줄의 개수에서 찾을 문자열의 개수를 
                    //뺀 값을 출력한다.
                    *findingStartLetterIndex = row->GetLength() - matchedLettersCount;
                }
            }
            //8.7.9 줄의 content에서 다음 글자을 읽는다.
            i++;
        }
        //8.8 찾을 문자열을 row의 content에서 못찾았으면
        if (isMatched == false)
        {
            //8.8.1 현재 줄의 위치를 진짜줄 이전의 가짜줄까지로 다시 재조정한다.
            currentRowIndex = nextRowIndex - 1;
        }
        //8.9 다음 줄로 이동한다.
        currentRowIndex++;
        //8.10 다음 줄로 이동했기 때문에 currentLettersLength와 currentLetterIndex를 0으로 초기화해준다.
        currentLettersLength = 0;
        currentLetterIndex = 0;
        //여기서 초기화를 안해주면 누적이 되기 때문에 제대로 된 탐색이 안됨 줄이 바뀌면(진짜 줄이 바뀌므로)
        //그 줄에서 일치하는 문자열을 찾지 못했다는 뜻으로 다음 줄에서 다시 일치하는 문자열을 찾겠다는 의미!
        //그렇기 때문에 다음줄에서는 다시 처음부터 찾아야하기 때문에 일치하는 문자열과 관련된 사항들을 모두
        //0으로 초기화 시켜서 다시 시작해준다. 그렇게 해야만 진짜줄 여러줄에 걸쳐서 탐색되는 경우가 없다
        //진짜줄에서는 한줄내에서만 탐색이 되야하기 때문에 여기서 진짜 줄이 바뀌기 때문에
        //찾을 문자열과 관련된 사항들을 모두 0으로 초기화해주고, 가짜줄은 진짜 줄이 바뀌기전에 같이 처리되기
        //때문에 가짜줄일 경우 여러줄에 걸쳐서 탐색이 가능하다.
        //8.11 일치하는 문자열의 길이를 0으로 초기화해준다.
        matchedLength = 0;
        //8.12 일치하는 문자열의 글자 개수를 0으로 초기화해준다.
        matchedLettersCount = 0;
        //8.13 찾을 문자열에서 읽을 글자위치를 처음으로 이동시킨다.
        j = 0;
    }
    //9. 못찾았으면
    if (isMatched == false)
    {
        //4.1 원래 캐럿이 있던 곳에 그대로 있는다.
        *findingEndRowIndex = this->note->GetCurrent();
        *findingEndLetterIndex = this->note->GetAt(*findingEndRowIndex)->GetCurrent();
        *findingStartRowIndex= this->note->GetCurrent();
        *findingStartLetterIndex = this->note->GetAt(*findingStartRowIndex)->GetCurrent();
    }
    //10. 끝내다.
}

//위로 찾기
void GlyphFinder::FindUp(string keyword, Long* findingStartRowIndex, Long* findingStartLetterIndex,
    Long* findingEndRowIndex, Long* findingEndLetterIndex)
{
    //1. 찾을 문자열을 입력받는다.
    //2. 메모장에서 현재 줄의 위치를 구한다.
    Long currentRowIndex = this->note->GetCurrent();
    //3. 메모장에서 현재 글자의 위치를 구한다.
    Long currentLetterIndex = this->note->GetAt(currentRowIndex)->GetCurrent();
    //4. 메모장의 현재 줄에서 현재 글자까지의 content를 구한다.
    string currentLetters = this->note->GetAt(currentRowIndex)
        ->GetPartOfContent(currentLetterIndex);
    //5. 메모장의 현재 줄에서 현재 글자까지의 content의 길이를 구한다.
    Long currentLettersLength = currentLetters.length();
    //6. 찾을 문자열을 찾았는지 여부를 일단 못찾았기 때문에 false로 초기화한다.
    bool isMatched = false;
    //7. 사용할 변수들을 반복문 밖에서 미리 선언 및 초기화시킨다.
    Long matchedLettersCount = 0;//줄에서 찾은 글자와 찾을 문자열의 글자가 서로 일치하는 글자개수
    Long keywordLength = keyword.length();// 찾을 문자열의 길이(한글은 2, 영문 및 특수문자는 1)
    Long matchedLength = 0;// 찾을 문자열과 줄의 글자들이 서로 일치하는 문자열의 길이
    Glyph* row = 0;
    Long realRowIndex = 0;
    Glyph* previousRow = 0;
    string letterContentOfRow;//줄의 한 글자를 담을 공간
    string partContentOfKeyword;//찾을 문자열의 한 글자를 담을 공간
    Long previousRowIndex = -1;
    string rowContent;//줄의 content를 담을 공간
    string previousRowContent;//이전 줄의 content를 담을 공감
    Long contentLength = 0;//찾을 문자열의 한글자의 길이
    Glyph* currentRow = 0;
    string currentRowContent;
    Long currentRowContentLength = 0;
    Long previousRowContentLength = 0;
    Long sum = 0;
    Glyph* nextRow = 0;
    Long nextRowIndex = 0;
    string koreanCompositionTemp;//역순으로 거슬러 올라가기 때문에 역순으로 한글을 조립하면 한글이 제대로
    //조립되지 않기 때문에 한글을 임시로 저장하고 원래 순서대로 조립할 임시 저장소가 필요함.
    Long i = 0;//줄의 content에서 글자를 읽을 배열요소 및 반복제변수
    Long j = keywordLength - 1;//찾을 문자열의 content에서 글자를 읽을 배열요소 및 반복제어변수
    //8. 현재 줄의 위치가 0보다 크거나 같은 동안 그리고 해당문자열을 찾을 때까지 반복한다.
    while (currentRowIndex >= 0 && isMatched == false)
    {
        //8.1 현재 줄을 구한다.
        row = this->note->GetAt(currentRowIndex);
        //8.2 현재 줄의 content를 구한다.
        rowContent = row->GetPartOfContent(currentLetterIndex);
        //8.2 현재 줄이 가짜 줄이면(현재 줄의 위치가 0이면 가짜 줄일 수 X)
        if (dynamic_cast<DummyRow*>(row))
        {

            //8.2.2 이전 줄로 이동한다.
            previousRowIndex = currentRowIndex - 1;
            //8.2.3 이전 줄을 구한다
            previousRow = this->note->GetAt(previousRowIndex);
            //8.2.4 이전 줄이 가짜줄인동안 반복한다.
            while (dynamic_cast<DummyRow*>(previousRow))
            {
                //8.2.4.1 이전 줄의 content를 구한다.
                previousRowContent = previousRow->GetContent();
                //8.2.4.2 이전 줄의 content에 현재 줄의 cotent를 더해준다.(누적)
                previousRowContent += rowContent;
                //8.2.4.3 더해준 값을 옮겨 저장한다.(반복 구조에서 제대로 누적시키기 위해서)
                rowContent = previousRowContent;
                //8.2.4.4 메모장의 현재 글자위치에 이전 줄의 글자개수를 더해준다.
                currentLetterIndex += previousRow->GetLength();
                //8.2.4.5 이전으로 이동한다.
                previousRowIndex--;
                //8.2.4.6 이전 줄을 구한다.
                previousRow = this->note->GetAt(previousRowIndex);
            }
            //이전으로 이동했을 때 진짜 줄이 나와서 반복구조를 나왔기 때문에 진짜 줄에 반복구조에서
            //합친 content를 합쳐준다.
            //8.2.5 자동개행을 부분취소하고 진짜 줄의 위치를 찾아서
            //진짜 줄의 위치를 현재 줄의 위치로 설정한다.
            currentRowIndex = previousRowIndex;
            //8.2.6 진짜 줄의 content를 구한다.
            previousRowContent = previousRow->GetContent();
            //8.2.7 진짜 줄의 content를 누적시킨다.
            previousRowContent += rowContent;
            //8.2.8 더해준 값을 옮겨 저장한다.(반복 구조에서 제대로 누적시키기 위해서)
            rowContent = previousRowContent;
            //8.2.9 메모장의 현재 글자위치에 이전 줄의 글자개수를 더해준다.
            currentLetterIndex += previousRow->GetLength();
            //8.2.10 현재 줄의 길이를 구한다.(자동개행 취소된 마지막 글자위치에서 시작)
            currentLettersLength = rowContent.length();
        }
        
        realRowIndex = currentRowIndex;

        //8.3 줄에서 읽을 글자 위치를 초기화시킨다.
        i = currentLettersLength - 1;
        //8.4 읽을 글자 위치가 0보다 크거나 같은 동안 그리고 해당문자열을 찾을때까지 반복한다.
        while (i >= 0 && isMatched == false)
        {
            //8.4.1 줄에서 i번째 읽을 글자가 한글이면
            if ((rowContent[i] & 0x80))//한글이면(2byte문자)
            {
                //8.4.1.1 2byte로 저장한다.
                letterContentOfRow = rowContent[i];
                //역순으로 거슬러 가기 때문에 제대로 한글을 조립하기 위해서 임시저장을 해야함.
                koreanCompositionTemp = letterContentOfRow;
                i--;
                letterContentOfRow = rowContent[i];
                letterContentOfRow += koreanCompositionTemp;
            }
            //8.4.2 한글이 아니면
            else
            {
                //8.4.2.1 1byte로 저장한다
                letterContentOfRow = rowContent[i];
            }
            //8.4.3 찾을 문자열에서 j번째 읽을 글자가 한글이면
            if ((keyword[j] & 0x80))//한글이면(2byte문자)
            {
                //8.4.3.1 2byte로 저장한다.
                partContentOfKeyword = keyword[j];
                //역순으로 거슬러 가기 때문에 제대로 한글을 조립하기 위해서 임시저장을 해야함.
                koreanCompositionTemp = partContentOfKeyword;
                j--;
                partContentOfKeyword = keyword[j];
                partContentOfKeyword += koreanCompositionTemp;
                //8.4.3.2 길이를 2로 설정한다(한글이 길이를 2 차지하기 때문에)
                contentLength = 2;
            }
            //8.4.4 한글이 아니면
            else
            {
                //8.4.4.1 1byte로 저장한다
                partContentOfKeyword = keyword[j];
                //8.4.4.2 길이를 1로 설정한다.(한글을 제외한 영문 및 특수문자는 길이가 1이기 때문에)
                contentLength = 1;
            }
            //8.4.5 줄에서 읽은 글자와 찾을 문자열에서 읽을 글자가 서로 일치하면
            if (letterContentOfRow == partContentOfKeyword)
            {
                //8.4.5.1 일치하는 문자열의 길이를 세준다.
                matchedLength += contentLength;
                //8.4.5.2 일치하는 문자열의 글자 개수를 세준다.
                matchedLettersCount++;
                //8.4.5.3 찾을 문자열에서 읽을 글자위치를 이전으로 이동시킨다.
                j--;
            }
            //8.4.6 줄에서 읽은 글자와 찾을 문자열에서 읽을 글자가 서로 다르면
            else
            {
                //8.4.6.1 일치하는 문자열의 길이를 0으로 초기화해준다.
                matchedLength = 0;
                //8.4.6.2 일치하는 문자열의 글자 개수를 0으로 초기화해준다.
                matchedLettersCount = 0;
                //8.4.6.3 찾을 문자열에서 읽을 글자위치를 마지막으로 이동시킨다.
                j = keywordLength - 1;
            }
            //8.4.7 메모장의 현재 줄에서 현재 위치의 글자를 읽었고 같은지 비교를 했기 때문에
            //현재 글자의 위치를 이전으로 이동시켜야 이전 글자를 읽고 찾을 문자열의 이전 글자와 비교할 수 있다.
            currentLetterIndex--;
            //8.4.8 찾을 문자열을 row의 content에서 찾았으면
           //일치하는 문자열의 길이와 찾을 문자열의 길이가 일치하면
            if (matchedLength == keywordLength)
            {
                //8.4.8.1 해당 문자열을 줄에서 찾았다고 표시한다.
                isMatched = true;
                //8.4.8.2 메모장에서 현재 줄을 구한다.(진짜 줄)
                currentRow = this->note->GetAt(currentRowIndex);
                //8.4.8.3 현재 줄의 content를 구한다.
                currentRowContent = currentRow->GetContent();
                //8.4.8.4 현재 줄의 length를 구한다.
                currentRowContentLength = currentRowContent.length();
                //8.4.8.5 i가 현재 줄의 length보다 크거나 같은동안 반복한다.
                while (i >= currentRowContentLength)
                {
                    //8.4.8.5.1 현재 줄의 길이를 뺀다.
                    i -= currentRowContentLength;
                    //8.4.8.5.2 현재 줄의 개수를 뺀다.
                    currentLetterIndex -= currentRow->GetLength();
                    //8.4.8.5.3 다음 줄로 이동한다.
                    currentRowIndex++;
                    //8.4.8.5.4 다음 줄을 구한다.
                    currentRow = this->note->GetAt(currentRowIndex);
                    //8.4.8.5.5 다음 줄의 content를 구한다.
                    currentRowContent = currentRow->GetContent();
                    //8.4.8.5.6 다음 줄의 content의 length를 구한다.
                    currentRowContentLength = currentRowContent.length(); 
                }
                //8.6.8.6 찾은 줄의 시작 위치와 찾은 글자 시작 위치를 출력한다.
                *findingStartRowIndex = currentRowIndex;
                *findingStartLetterIndex = currentLetterIndex;
                //8.6.8.6 찾은 글자의 시작위치에서 일치하는 문자열의 글자개수를 더해준다.
                sum = currentLetterIndex + matchedLettersCount;
                //8.6.8.7 더한값이 찾은 줄의 시작위치에 해당하는 줄의 글자개수보다 작거나 같으면
                if (sum <= this->note->GetAt(currentRowIndex)->GetLength())
                {
                    //8.6.8.7.1 찾은 줄의 끝위치를 찾을 줄의 시작 위치와 동일한 값으로 출력한다.
                    *findingEndRowIndex = currentRowIndex;
                    //8.6.8.7.2 찾은 글자의 끝위치를 더한값으로 출력한다.
                    *findingEndLetterIndex = sum;
                }
                //8.6.8.8 더한 값이 줄의 글자개수보다 크면
                else
                {
                    //8.6.8.8.1 현재 줄을 구한다.
                    currentRow = this->note->GetAt(currentRowIndex);
                    //8.6.8.8.2 현재 줄의 다음 줄을 구한다.
                    nextRowIndex = currentRowIndex + 1;
                    nextRow = this->note->GetAt(nextRowIndex);
                    //8.6.8.8.3 현재 줄에서 다음 줄로 이동했기 때문에 더한 값에서 현재 줄의 개수만큼 빼준다.
                    sum -= currentRow->GetLength();
                    //8.6.8.8.4 더한 값이 다음 줄의 글자개수보다 큰 동안 반복한다.
                    while (sum > nextRow->GetLength())
                    {
                        //8.6.8.8.4.1 더한 값에서 다음 줄의 글자개수를 빼준다.
                        sum -= nextRow->GetLength();
                        //8.6.8.8.4.2 다음 줄을 구한다.
                        nextRowIndex--;
                        nextRow = this->note->GetAt(nextRowIndex);
                    }
                    //8.6.8.8.5 찾은 줄의 끝위치를 다음 줄로 정한다.
                    *findingEndRowIndex = nextRowIndex;
                    //8.6.8.8.6 찾은 글자의 끝위치를 더한 값으로 정한다.
                    *findingEndLetterIndex = sum;
                }
            }
            //8.6.8.9 줄의 content에서 이전 글자로 이동한다.
            i--;
        }
        //8.8 찾을 문자열을 row의 content에서 못찾았으면
        if (isMatched == false)
        {
            //8.8.1 현재 줄의 위치를 진짜줄 이전의 가짜줄까지로 다시 재조정한다.
            currentRowIndex = realRowIndex;
        }
        //8.8 이전 줄로 이동한다.
        currentRowIndex--;
        if (currentRowIndex >= 0)
        {
            //8.9 이전 줄을 구한다.
            previousRow = this->note->GetAt(currentRowIndex);
            //8.10 이전 줄로 이동했기 때문에 줄에서 읽을 글자위치를 이전 줄의 content 길이로 설정한다.
            previousRowContent = previousRow->GetContent();
            previousRowContentLength = previousRowContent.length();
            currentLettersLength = previousRowContentLength;
            //8.11 현재 글자 위치를 이전 줄의 글자 개수로 설정한다.
            currentLetterIndex = previousRow->GetLength();
        }
        //8.11 일치하는 문자열의 길이를 0으로 초기화해준다.
        matchedLength = 0;
        //8.12 일치하는 문자열의 글자 개수를 0으로 초기화해준다.
        matchedLettersCount = 0;
        //8.13 찾을 문자열에서 읽을 글자위치를 마지막으로 이동시킨다.
        j = keywordLength - 1;
        //여기서 초기화를 안해주면 누적이 되기 때문에 제대로 된 탐색이 안됨 줄이 바뀌면(진짜 줄이 바뀌므로)
        //그 줄에서 일치하는 문자열을 찾지 못했다는 뜻으로 다음 줄에서 다시 일치하는 문자열을 찾겠다는 의미!
        //그렇기 때문에 다음줄에서는 다시 처음부터 찾아야하기 때문에 일치하는 문자열과 관련된 사항들을 모두
        //0으로 초기화 시켜서 다시 시작해준다. 그렇게 해야만 진짜줄 여러줄에 걸쳐서 탐색되는 경우가 없다
        //진짜줄에서는 한줄내에서만 탐색이 되야하기 때문에 여기서 진짜 줄이 바뀌기 때문에
        //찾을 문자열과 관련된 사항들을 모두 0으로 초기화해주고, 가짜줄은 진짜 줄이 바뀌기전에 같이 처리되기
        //때문에 가짜줄일 경우 여러줄에 걸쳐서 탐색이 가능하다.
    }
    //9. 못찾았으면
    if (isMatched == false)
    {
        //4.1 원래 캐럿이 있던 곳에 그대로 있는다.
        *findingEndRowIndex = this->note->GetCurrent();
        *findingEndLetterIndex = this->note->GetAt(*findingEndRowIndex)->GetCurrent();
        *findingStartRowIndex = this->note->GetCurrent();
        *findingStartLetterIndex = this->note->GetAt(*findingStartRowIndex)->GetCurrent();
    }
    //10. 끝내다.
}

//_stricmp;//대소문자 구분 무시하는 함수(영어 대소문자를 같게 보게 해주는 함수)

//대/소문자 구분없이 아래로 찾기
void GlyphFinder::FindDownWithMatchCase(string keyword, Long* findingStartRowIndex,
    Long* findingStartLetterIndex, Long* findingEndRowIndex, Long* findingEndLetterIndex)
{
    //1. 찾을 문자열을 입력받는다.
   //2. 메모장에서 현재 줄의 위치를 구한다.
    Long currentRowIndex = this->note->GetCurrent();
    //3. 메모장에서 현재 글자의 위치를 구한다.
    Long currentLetterIndex = this->note->GetAt(currentRowIndex)->GetCurrent();
    //4. 메모장의 현재 줄에서 현재 글자까지의 content를 구한다.
    string currentLetters = this->note->GetAt(currentRowIndex)
        ->GetPartOfContent(currentLetterIndex);
    //5. 메모장의 현재 줄에서 현재 글자까지의 content의 길이를 구한다.
    Long currentLettersLength = currentLetters.length();
    //6. 찾을 문자열을 찾았는지 여부를 일단 못찾았기 때문에 false로 초기화한다.
    bool isMatched = false;
    //7. 사용할 변수들을 반복문 밖에서 미리 선언 및 초기화시킨다.
    Long matchedLettersCount = 0;//줄에서 찾은 글자와 찾을 문자열의 글자가 서로 일치하는 글자개수
    Long keywordLength = keyword.length();// 찾을 문자열의 길이(한글은 2, 영문 및 특수문자는 1)
    Long matchedLength = 0;// 찾을 문자열과 줄의 글자들이 서로 일치하는 문자열의 길이
    Glyph* row = 0;
    Glyph* nextRow = 0;
    string letterContentOfRow;//줄의 한 글자를 담을 공간
    string partContentOfKeyword;//찾을 문자열의 한 글자를 담을 공간
    Long nextRowIndex = 0;
    string realRowContent;//진짜 줄의 content를 담을 공간
    string dummyRowContent;//가짜 줄의 content를 담을 공감
    Long rowContentLength = 0;//줄의 content의 길이
    Long contentLength = 0;//찾을 문자열의 한글자의 길이
    Glyph* currentRow = 0;
    string currentRowContent;
    Long currentRowContentLength = 0;
    string nextRowContent;
    Long startLetterIndex = 0;
    Long previousRowIndex = 0;
    Long i = 0;//줄의 content에서 글자를 읽을 배열요소 및 반복제변수
    Long j = 0;//찾을 문자열의 content에서 글자를 읽을 배열요소 및 반복제어변수
    //8. 현재 줄의 위치가 note의 줄의 개수보다 작은동안 그리고 해당문자열을 찾을 때까지 반복한다.
    while (currentRowIndex < this->note->GetLength() && isMatched == false)
    {
        //8.1 줄을 구한다.
        row = this->note->GetAt(currentRowIndex);
        //8.2 줄의 content를 구한다.
        realRowContent = row->GetContent();
        //8.3 다음 줄을 구한다.
        nextRowIndex = currentRowIndex + 1;
        nextRow = this->note->GetAt(nextRowIndex);
        //자동개행 하는 것처럼 가짜 줄의 내용을 앞의 줄에 추가한다 왜냐하면 자동개행이 안된 상태에서는
        //한줄내에서만(진짜줄 한줄내에서) 찾고 선택하는게 가능하지만, 자동개행이 된 상태에서는 여러 가짜 줄에서
        //걸쳐서 찾아지고, 선택이 가능하다(원래라면(자동개행이 안된 상태에서는)한줄이기 때문에)
        //8.4 다음 줄의 위치가 줄의 개수보다 작은동안 그리고 다음 줄이 가짜 줄인 동안 반복한다.
        while (nextRowIndex < this->note->GetLength() &&
            dynamic_cast<DummyRow*>(nextRow))
        {
            //8.4.1 가짜줄의 content를 구한다.
            dummyRowContent = nextRow->GetContent();
            //8.4.2 가짜줄의 content를 진짜 줄의 content에 더해준다.
            realRowContent += dummyRowContent;
            //8.4.3 다음 줄로 이동한다.
            nextRowIndex++;
            //8.4.4 다음 줄을 구한다.
            nextRow = this->note->GetAt(nextRowIndex);
        }
        //8.5 줄의 content 길이를 구한다.
        rowContentLength = realRowContent.length();
        //8.6 줄에서 읽을 글자 위치를 초기화시킨다.
        i = currentLettersLength;
        //8.7 줄의 content 길이만큼 그리고 해당 문자열을 찾을때까지 반복한다.
        while (i < rowContentLength && isMatched == false)
        {
            //8.7.1 줄에서 i번째 읽을 글자가 한글이면
            if ((realRowContent[i] & 0x80))//한글이면(2byte문자)
            {
                //8.7.1.1 2byte로 저장한다.
                letterContentOfRow = realRowContent[i];
                i++;
                letterContentOfRow += realRowContent[i];
            }
            //8.7.2 한글이 아니면
            else
            {
                //8.7.2.1 1byte로 저장한다
                letterContentOfRow = realRowContent[i];
            }
            //8.7.3 찾을 문자열에서 j번째 읽을 글자가 한글이면
            if ((keyword[j] & 0x80))//한글이면(2byte문자)
            {
                //8.7.3.1 2byte로 저장한다.
                partContentOfKeyword = keyword[j];
                j++;
                partContentOfKeyword += keyword[j];
                //8.7.3.2 길이를 2로 설정한다(한글이 길이를 2 차지하기 때문에)
                contentLength = 2;
            }
            //8.7.4 한글이 아니면
            else
            {
                //8.7.4.1 1byte로 저장한다
                partContentOfKeyword = keyword[j];
                //8.7.4.2 길이를 1로 설정한다.(한글을 제외한 영문 및 특수문자는 길이가 1이기 때문에)
                contentLength = 1;
            }
            //8.7.5 줄에서 읽은 글자와 찾을 문자열에서 읽은 글자가 한글이 아니면
            if (letterContentOfRow.length() < 3 && partContentOfKeyword.length() < 3)
            {
                //8.7.5.1 대/소문자 구분없이 비교해준다.
                if (_stricmp(letterContentOfRow.c_str(), partContentOfKeyword.c_str()) == 0)
                {
                    //8.7.5.1.1 일치하는 문자열의 길이를 세준다.
                    matchedLength += contentLength;
                    //8.7.5.1.2 일치하는 문자열의 글자 개수를 세준다.
                    matchedLettersCount++;
                    //8.7.5.1.3 찾을 문자열에서 읽을 글자위치를 다음으로 이동시킨다.
                    j++;
                }
                //8.7.5.2 줄에서 읽은 글자와 찾을 문자열에서 읽을 글자가 서로 다르면
                else
                {
                    //8.7.5.2.1 일치하는 문자열의 길이를 0으로 초기화해준다.
                    matchedLength = 0;
                    //8.7.5.2.2 일치하는 문자열의 글자 개수를 0으로 초기화해준다.
                    matchedLettersCount = 0;
                    //8.7.5.2.3 찾을 문자열에서 읽을 글자위치를 처음으로 이동시킨다.
                    j = 0;
                }

            }
            //8.7.6 줄에서 읽은 글자와 찾을 문자열에서 읽은 글자가 한글이면
            else
            {
                //8.7.6.1 줄에서 읽은 글자와 찾을 문자열에서 읽을 글자가 서로 일치하면
                if (letterContentOfRow == partContentOfKeyword)
                {
                    //8.7.6.1.1 일치하는 문자열의 길이를 세준다.
                    matchedLength += contentLength;
                    //8.7.6.1.2 일치하는 문자열의 글자 개수를 세준다.
                    matchedLettersCount++;
                    //8.7.6.1.3 찾을 문자열에서 읽을 글자위치를 다음으로 이동시킨다.
                    j++;
                }
                //8.7.6.2 줄에서 읽은 글자와 찾을 문자열에서 읽을 글자가 서로 다르면
                else
                {
                    //8.7.6.2.1 일치하는 문자열의 길이를 0으로 초기화해준다.
                    matchedLength = 0;
                    //8.7.6.2.2 일치하는 문자열의 글자 개수를 0으로 초기화해준다.
                    matchedLettersCount = 0;
                    //8.7.6.2.3 찾을 문자열에서 읽을 글자위치를 처음으로 이동시킨다.
                    j = 0;
                }
            }
            //8.7.7 메모장의 현재 줄에서 현재 위치의 글자를 읽었고 같은지 비교를 했기 때문에
            //현재 글자의 위치를 다음으로 이동시켜야 다음 글자를 읽고 찾을 문자열의 글자와 비교할 수 있다.
            currentLetterIndex++;
            //8.7.8 찾을 문자열을 row의 content에서 찾았으면
            //일치하는 문자열의 길이와 찾을 문자열의 길이가 일치하면
            if (matchedLength == keywordLength)
            {
                //8.7.8.1 해당 문자열을 줄에서 찾았다고 표시한다.
                isMatched = true;
                //8.7.8.2 현재 줄을 구한다.
                currentRow = this->note->GetAt(currentRowIndex);
                //8.7.8.3 현재 줄의 content를 구한다.
                currentRowContent = currentRow->GetContent();
                //8.7.8.4 현재 줄의 content의 length를 구한다.
                currentRowContentLength = currentRowContent.length();
                //8.7.8.5 i가 현재 줄의 content의 length보다 크거나 같은동안 반복한다.
                //아까 앞에서 부분자동개행을 취소하는 것처럼 가짜줄의 content들을 진짜줄의 content에
                //추가시켰지만 사실상 현재 메모장에서는 자동개행이 되어 있는 상태이기 때문에
                //자동개행이 된 메모장에서 찾을 문자열을 찾아서 선택을 해야하기 때문에 아까 합쳤던 각각의 
                //가짜줄들을 다시 개별적으로 구해서 합친 줄에서 읽은 글자위치 i와 비교해서 현재 줄의 위치를
                //다시 구해준다.
                while (i >= currentRowContentLength)
                {
                    //8.7.8.5.1 현재 줄의 위치를 증가시킨다.
                    //자동개행이 취소된 상태에서 해당 문자열을 찾았는데 최종적으로 읽은 글자가
                    //현재 개별적인 줄의 길이보다 길기 때문에 다음 줄로 넘어가서 다시 비교해야한다.
                    currentRowIndex++;
                    //8.7.8.5.2 현재 글자위치도 자동개행이 취소된 상태의 글자위치기 때문에
                    //현재 글자위치가 현재 줄의 개수보다 크기 때문에 그만큼 빼서 자동개행이 된 상태로 조정한다
                    //앞에서 currentLetterIndex++로 인해 1이 증가된 상태이기 때문에 +1을 따로 안해줘도됨!
                    currentLetterIndex -= currentRow->GetLength();
                    //8.7.8.5.3 현재 줄의 다음줄을 구한다.
                    currentRow = this->note->GetAt(currentRowIndex);
                    //8.7.8.5.4 다음 줄의 content를 저장한다.
                    nextRowContent = currentRow->GetContent();
                    //8.7.8.5.5 다음 줄의 content를 현재 줄의 content에 더해준다.(누적)
                    currentRowContent += nextRowContent;
                    //8.7.8.5.6 누적된 줄의 content의 길이를 구한다.
                    currentRowContentLength = currentRowContent.length();
                }
                //8.7.8.6 찾은 줄의 끝 위치와 찾은 글자 끝위치를 출력한다.
                *findingEndRowIndex = currentRowIndex;
                *findingEndLetterIndex = currentLetterIndex;
                //8.7.8.7 현재 글자 위치에서 찾은 문자열의 글자개수를 빼준다.
                startLetterIndex = currentLetterIndex - matchedLettersCount;
                //8.7.8.8 양수이면
                if (startLetterIndex >= 0)
                {
                    //8.7.8.8.1 찾은 줄의 시작위치를 찾을 줄의 끝 위치와 동일한 값으로 출력한다.
                    *findingStartRowIndex = currentRowIndex;
                    //8.7.8.8.2 찾은 글자의 시작위치를 뺀값을 대입해서 출력한다.
                    *findingStartLetterIndex = startLetterIndex;
                }
                //8.7.8.9 음수이면
                else
                {
                    //8.7.8.9.1 현재 줄의 이전 줄을 구한다.
                    previousRowIndex = currentRowIndex - 1;
                    row = this->note->GetAt(previousRowIndex);
                    //8.7.8.9.2 현재 줄에서 이전 줄로 이동했기 때문에 
                    //찾은 문자열의 글자개수에서 현재 줄의 글자위치를 빼준다
                    matchedLettersCount -= currentLetterIndex;
                    //8.7.8.9.3 은 문자열의 글자개수가 이전 줄의 글자개수보다 큰 동안 반복한다.
                    while (matchedLettersCount > row->GetLength())
                    {
                        //8.7.8.9.3.1 찾은 물자열의 글자개수에서 이전 줄의 글자개수를 빼준다.
                        matchedLettersCount -= row->GetLength();
                        //8.7.8.9.3.2 이전 줄을 구한다.
                        previousRowIndex--;
                        row = this->note->GetAt(previousRowIndex);
                    }
                    //8.7.8.9.4 찾은 줄의 시작위치를 이전 줄의 위치로 출력한다.
                    *findingStartRowIndex = previousRowIndex;
                    //8.7.8.9.5 찾은 글자의 시작위치를 이전 줄의 개수에서 찾을 문자열의 개수를 
                    //뺀 값을 출력한다.
                    *findingStartLetterIndex = row->GetLength() - matchedLettersCount;
                }
            }
            //8.7.9 줄의 content에서 다음 글자을 읽는다.
            i++;
        }
        //8.8 찾을 문자열을 row의 content에서 못찾았으면
        if (isMatched == false)
        {
            //8.8.1 현재 줄의 위치를 진짜줄 이전의 가짜줄까지로 다시 재조정한다.
            currentRowIndex = nextRowIndex - 1;
        }
        //8.9 다음 줄로 이동한다.
        currentRowIndex++;
        //8.10 다음 줄로 이동했기 때문에 currentLettersLength와 currentLetterIndex를 0으로 초기화해준다.
        currentLettersLength = 0;
        currentLetterIndex = 0;
        //여기서 초기화를 안해주면 누적이 되기 때문에 제대로 된 탐색이 안됨 줄이 바뀌면(진짜 줄이 바뀌므로)
        //그 줄에서 일치하는 문자열을 찾지 못했다는 뜻으로 다음 줄에서 다시 일치하는 문자열을 찾겠다는 의미!
        //그렇기 때문에 다음줄에서는 다시 처음부터 찾아야하기 때문에 일치하는 문자열과 관련된 사항들을 모두
        //0으로 초기화 시켜서 다시 시작해준다. 그렇게 해야만 진짜줄 여러줄에 걸쳐서 탐색되는 경우가 없다
        //진짜줄에서는 한줄내에서만 탐색이 되야하기 때문에 여기서 진짜 줄이 바뀌기 때문에
        //찾을 문자열과 관련된 사항들을 모두 0으로 초기화해주고, 가짜줄은 진짜 줄이 바뀌기전에 같이 처리되기
        //때문에 가짜줄일 경우 여러줄에 걸쳐서 탐색이 가능하다.
        //8.11 일치하는 문자열의 길이를 0으로 초기화해준다.
        matchedLength = 0;
        //8.12 일치하는 문자열의 글자 개수를 0으로 초기화해준다.
        matchedLettersCount = 0;
        //8.13 찾을 문자열에서 읽을 글자위치를 처음으로 이동시킨다.
        j = 0;
    }
    //9. 못찾았으면
    if (isMatched == false)
    {
        //4.1 원래 캐럿이 있던 곳에 그대로 있는다.
        *findingEndRowIndex = this->note->GetCurrent();
        *findingEndLetterIndex = this->note->GetAt(*findingEndRowIndex)->GetCurrent();
        *findingStartRowIndex = this->note->GetCurrent();
        *findingStartLetterIndex = this->note->GetAt(*findingStartRowIndex)->GetCurrent();
    }
    //10. 끝내다.
}

//대/소문자 구분없이 위로 찾기
void GlyphFinder::FindUpWithMatchCase(string keyword, Long* findingStartRowIndex,
    Long* findingStartLetterIndex, Long* findingEndRowIndex, Long* findingEndLetterIndex)
{
    //1. 찾을 문자열을 입력받는다.
   //2. 메모장에서 현재 줄의 위치를 구한다.
    Long currentRowIndex = this->note->GetCurrent();
    //3. 메모장에서 현재 글자의 위치를 구한다.
    Long currentLetterIndex = this->note->GetAt(currentRowIndex)->GetCurrent();
    //4. 메모장의 현재 줄에서 현재 글자까지의 content를 구한다.
    string currentLetters = this->note->GetAt(currentRowIndex)
        ->GetPartOfContent(currentLetterIndex);
    //5. 메모장의 현재 줄에서 현재 글자까지의 content의 길이를 구한다.
    Long currentLettersLength = currentLetters.length();
    //6. 찾을 문자열을 찾았는지 여부를 일단 못찾았기 때문에 false로 초기화한다.
    bool isMatched = false;
    //7. 사용할 변수들을 반복문 밖에서 미리 선언 및 초기화시킨다.
    Long matchedLettersCount = 0;//줄에서 찾은 글자와 찾을 문자열의 글자가 서로 일치하는 글자개수
    Long keywordLength = keyword.length();// 찾을 문자열의 길이(한글은 2, 영문 및 특수문자는 1)
    Long matchedLength = 0;// 찾을 문자열과 줄의 글자들이 서로 일치하는 문자열의 길이
    Glyph* row = 0;
    Long realRowIndex = 0;
    Glyph* previousRow = 0;
    string letterContentOfRow;//줄의 한 글자를 담을 공간
    string partContentOfKeyword;//찾을 문자열의 한 글자를 담을 공간
    Long previousRowIndex = -1;
    string rowContent;//줄의 content를 담을 공간
    string previousRowContent;//이전 줄의 content를 담을 공감
    Long contentLength = 0;//찾을 문자열의 한글자의 길이
    Glyph* currentRow = 0;
    string currentRowContent;
    Long currentRowContentLength = 0;
    Long previousRowContentLength = 0;
    Long sum = 0;
    Glyph* nextRow = 0;
    Long nextRowIndex = 0;
    string koreanCompositionTemp;//역순으로 거슬러 올라가기 때문에 역순으로 한글을 조립하면 한글이 제대로
    //조립되지 않기 때문에 한글을 임시로 저장하고 원래 순서대로 조립할 임시 저장소가 필요함.
    Long i = 0;//줄의 content에서 글자를 읽을 배열요소 및 반복제변수
    Long j = keywordLength - 1;//찾을 문자열의 content에서 글자를 읽을 배열요소 및 반복제어변수
    //8. 현재 줄의 위치가 0보다 크거나 같은 동안 그리고 해당문자열을 찾을 때까지 반복한다.
    while (currentRowIndex >= 0 && isMatched == false)
    {
        //8.1 현재 줄을 구한다.
        row = this->note->GetAt(currentRowIndex);
        //8.2 현재 줄의 content를 구한다.
        rowContent = row->GetPartOfContent(currentLetterIndex);
        //8.2 현재 줄이 가짜 줄이면(현재 줄의 위치가 0이면 가짜 줄일 수 X)
        if (dynamic_cast<DummyRow*>(row))
        {

            //8.2.2 이전 줄로 이동한다.
            previousRowIndex = currentRowIndex - 1;
            //8.2.3 이전 줄을 구한다
            previousRow = this->note->GetAt(previousRowIndex);
            //8.2.4 이전 줄이 가짜줄인동안 반복한다.
            while (dynamic_cast<DummyRow*>(previousRow))
            {
                //8.2.4.1 이전 줄의 content를 구한다.
                previousRowContent = previousRow->GetContent();
                //8.2.4.2 이전 줄의 content에 현재 줄의 cotent를 더해준다.(누적)
                previousRowContent += rowContent;
                //8.2.4.3 더해준 값을 옮겨 저장한다.(반복 구조에서 제대로 누적시키기 위해서)
                rowContent = previousRowContent;
                //8.2.4.4 메모장의 현재 글자위치에 이전 줄의 글자개수를 더해준다.
                currentLetterIndex += previousRow->GetLength();
                //8.2.4.5 이전으로 이동한다.
                previousRowIndex--;
                //8.2.4.6 이전 줄을 구한다.
                previousRow = this->note->GetAt(previousRowIndex);
            }
            //이전으로 이동했을 때 진짜 줄이 나와서 반복구조를 나왔기 때문에 진짜 줄에 반복구조에서
            //합친 content를 합쳐준다.
            //8.2.5 자동개행을 부분취소하고 진짜 줄의 위치를 찾아서
            //진짜 줄의 위치를 현재 줄의 위치로 설정한다.
            currentRowIndex = previousRowIndex;
            //8.2.6 진짜 줄의 content를 구한다.
            previousRowContent = previousRow->GetContent();
            //8.2.7 진짜 줄의 content를 누적시킨다.
            previousRowContent += rowContent;
            //8.2.8 더해준 값을 옮겨 저장한다.(반복 구조에서 제대로 누적시키기 위해서)
            rowContent = previousRowContent;
            //8.2.9 메모장의 현재 글자위치에 이전 줄의 글자개수를 더해준다.
            currentLetterIndex += previousRow->GetLength();
            //8.2.10 현재 줄의 길이를 구한다.(자동개행 취소된 마지막 글자위치에서 시작)
            currentLettersLength = rowContent.length();
        }

        realRowIndex = currentRowIndex;

        //8.3 줄에서 읽을 글자 위치를 초기화시킨다.
        i = currentLettersLength - 1;
        //8.4 읽을 글자 위치가 0보다 크거나 같은 동안 그리고 해당문자열을 찾을때까지 반복한다.
        while (i >= 0 && isMatched == false)
        {
            //8.4.1 줄에서 i번째 읽을 글자가 한글이면
            if ((rowContent[i] & 0x80))//한글이면(2byte문자)
            {
                //8.4.1.1 2byte로 저장한다.
                letterContentOfRow = rowContent[i];
                //역순으로 거슬러 가기 때문에 제대로 한글을 조립하기 위해서 임시저장을 해야함.
                koreanCompositionTemp = letterContentOfRow;
                i--;
                letterContentOfRow = rowContent[i];
                letterContentOfRow += koreanCompositionTemp;
            }
            //8.4.2 한글이 아니면
            else
            {
                //8.4.2.1 1byte로 저장한다
                letterContentOfRow = rowContent[i];
            }
            //8.4.3 찾을 문자열에서 j번째 읽을 글자가 한글이면
            if ((keyword[j] & 0x80))//한글이면(2byte문자)
            {
                //8.4.3.1 2byte로 저장한다.
                partContentOfKeyword = keyword[j];
                //역순으로 거슬러 가기 때문에 제대로 한글을 조립하기 위해서 임시저장을 해야함.
                koreanCompositionTemp = partContentOfKeyword;
                j--;
                partContentOfKeyword = keyword[j];
                partContentOfKeyword += koreanCompositionTemp;
                //8.4.3.2 길이를 2로 설정한다(한글이 길이를 2 차지하기 때문에)
                contentLength = 2;
            }
            //8.4.4 한글이 아니면
            else
            {
                //8.4.4.1 1byte로 저장한다
                partContentOfKeyword = keyword[j];
                //8.4.4.2 길이를 1로 설정한다.(한글을 제외한 영문 및 특수문자는 길이가 1이기 때문에)
                contentLength = 1;
            }
            //8.4.5 줄에서 읽은 글자와 찾을 문자열에서 읽은 글자가 한글이 아니면
            if (letterContentOfRow.length() < 3 && partContentOfKeyword.length() < 3)
            {
                //8.4.5.1 대/소문자 구분없이 비교해준다.
                if (_stricmp(letterContentOfRow.c_str(), partContentOfKeyword.c_str()) == 0)
                {
                    //8.4.5.1.1 일치하는 문자열의 길이를 세준다.
                    matchedLength += contentLength;
                    //8.4.5.1.2 일치하는 문자열의 글자 개수를 세준다.
                    matchedLettersCount++;
                    //8.4.5.1.3 찾을 문자열에서 읽을 글자위치를 이전으로 이동시킨다.
                    j--;
                }
                //8.4.5.2 줄에서 읽은 글자와 찾을 문자열에서 읽을 글자가 서로 다르면
                else
                {
                    //8.4.5.1 일치하는 문자열의 길이를 0으로 초기화해준다.
                    matchedLength = 0;
                    //8.4.5.2 일치하는 문자열의 글자 개수를 0으로 초기화해준다.
                    matchedLettersCount = 0;
                    //8.4.5.3 찾을 문자열에서 읽을 글자위치를 마지막으로 이동시킨다.
                    j = keywordLength - 1;
                }

            }
            //8.4.6 줄에서 읽은 글자와 찾을 문자열에서 읽은 글자가 한글이면
            else
            {
                //8.4.6.1 줄에서 읽은 글자와 찾을 문자열에서 읽을 글자가 서로 일치하면
                if (letterContentOfRow == partContentOfKeyword)
                {
                    //8.4.6.1.1 일치하는 문자열의 길이를 세준다.
                    matchedLength += contentLength;
                    //8.4.6.1.2 일치하는 문자열의 글자 개수를 세준다.
                    matchedLettersCount++;
                    //8.4.6.1.3 찾을 문자열에서 읽을 글자위치를 이전으로 이동시킨다.
                    j--;
                }
                //8.4.6.2 줄에서 읽은 글자와 찾을 문자열에서 읽을 글자가 서로 다르면
                else
                {
                    //8.4.6.2.1 일치하는 문자열의 길이를 0으로 초기화해준다.
                    matchedLength = 0;
                    //8.4.6.2.2 일치하는 문자열의 글자 개수를 0으로 초기화해준다.
                    matchedLettersCount = 0;
                    //8.4.6.2.3 찾을 문자열에서 읽을 글자위치를 마지막으로 이동시킨다.
                    j = keywordLength - 1;
                }
            }
            //8.4.7 메모장의 현재 줄에서 현재 위치의 글자를 읽었고 같은지 비교를 했기 때문에
            //현재 글자의 위치를 이전으로 이동시켜야 이전 글자를 읽고 찾을 문자열의 이전 글자와 비교할 수 있다.
            currentLetterIndex--;
            //8.4.8 찾을 문자열을 row의 content에서 찾았으면
           //일치하는 문자열의 길이와 찾을 문자열의 길이가 일치하면
            if (matchedLength == keywordLength)
            {
                //8.4.8.1 해당 문자열을 줄에서 찾았다고 표시한다.
                isMatched = true;
                //8.4.8.2 메모장에서 현재 줄을 구한다.(진짜 줄)
                currentRow = this->note->GetAt(currentRowIndex);
                //8.4.8.3 현재 줄의 content를 구한다.
                currentRowContent = currentRow->GetContent();
                //8.4.8.4 현재 줄의 length를 구한다.
                currentRowContentLength = currentRowContent.length();
                //8.4.8.5 i가 현재 줄의 length보다 크거나 같은동안 반복한다.
                while (i >= currentRowContentLength)
                {
                    //8.4.8.5.1 현재 줄의 길이를 뺀다.
                    i -= currentRowContentLength;
                    //8.4.8.5.2 현재 줄의 개수를 뺀다.
                    currentLetterIndex -= currentRow->GetLength();
                    //8.4.8.5.3 다음 줄로 이동한다.
                    currentRowIndex++;
                    //8.4.8.5.4 다음 줄을 구한다.
                    currentRow = this->note->GetAt(currentRowIndex);
                    //8.4.8.5.5 다음 줄의 content를 구한다.
                    currentRowContent = currentRow->GetContent();
                    //8.4.8.5.6 다음 줄의 content의 length를 구한다.
                    currentRowContentLength = currentRowContent.length();
                }
                //8.6.8.6 찾은 줄의 시작 위치와 찾은 글자 시작 위치를 출력한다.
                *findingStartRowIndex = currentRowIndex;
                *findingStartLetterIndex = currentLetterIndex;
                //8.6.8.6 찾은 글자의 시작위치에서 일치하는 문자열의 글자개수를 더해준다.
                sum = currentLetterIndex + matchedLettersCount;
                //8.6.8.7 더한값이 찾은 줄의 시작위치에 해당하는 줄의 글자개수보다 작거나 같으면
                if (sum <= this->note->GetAt(currentRowIndex)->GetLength())
                {
                    //8.6.8.7.1 찾은 줄의 끝위치를 찾을 줄의 시작 위치와 동일한 값으로 출력한다.
                    *findingEndRowIndex = currentRowIndex;
                    //8.6.8.7.2 찾은 글자의 끝위치를 더한값으로 출력한다.
                    *findingEndLetterIndex = sum;
                }
                //8.6.8.8 더한 값이 줄의 글자개수보다 크면
                else
                {
                    //8.6.8.8.1 현재 줄을 구한다.
                    currentRow = this->note->GetAt(currentRowIndex);
                    //8.6.8.8.2 현재 줄의 다음 줄을 구한다.
                    nextRowIndex = currentRowIndex + 1;
                    nextRow = this->note->GetAt(nextRowIndex);
                    //8.6.8.8.3 현재 줄에서 다음 줄로 이동했기 때문에 더한 값에서 현재 줄의 개수만큼 빼준다.
                    sum -= currentRow->GetLength();
                    //8.6.8.8.4 더한 값이 다음 줄의 글자개수보다 큰 동안 반복한다.
                    while (sum > nextRow->GetLength())
                    {
                        //8.6.8.8.4.1 더한 값에서 다음 줄의 글자개수를 빼준다.
                        sum -= nextRow->GetLength();
                        //8.6.8.8.4.2 다음 줄을 구한다.
                        nextRowIndex--;
                        nextRow = this->note->GetAt(nextRowIndex);
                    }
                    //8.6.8.8.5 찾은 줄의 끝위치를 다음 줄로 정한다.
                    *findingEndRowIndex = nextRowIndex;
                    //8.6.8.8.6 찾은 글자의 끝위치를 더한 값으로 정한다.
                    *findingEndLetterIndex = sum;
                }
            }
            //8.6.8.9 줄의 content에서 이전 글자로 이동한다.
            i--;
        }
        //8.8 찾을 문자열을 row의 content에서 못찾았으면
        if (isMatched == false)
        {
            //8.8.1 현재 줄의 위치를 진짜줄 이전의 가짜줄까지로 다시 재조정한다.
            currentRowIndex = realRowIndex;
        }
        //8.8 이전 줄로 이동한다.
        currentRowIndex--;
        if (currentRowIndex >= 0)
        {
            //8.9 이전 줄을 구한다.
            previousRow = this->note->GetAt(currentRowIndex);
            //8.10 이전 줄로 이동했기 때문에 줄에서 읽을 글자위치를 이전 줄의 content 길이로 설정한다.
            previousRowContent = previousRow->GetContent();
            previousRowContentLength = previousRowContent.length();
            currentLettersLength = previousRowContentLength;
            //8.11 현재 글자 위치를 이전 줄의 글자 개수로 설정한다.
            currentLetterIndex = previousRow->GetLength();
        }
        //8.11 일치하는 문자열의 길이를 0으로 초기화해준다.
        matchedLength = 0;
        //8.12 일치하는 문자열의 글자 개수를 0으로 초기화해준다.
        matchedLettersCount = 0;
        //8.13 찾을 문자열에서 읽을 글자위치를 마지막으로 이동시킨다.
        j = keywordLength - 1;
        //여기서 초기화를 안해주면 누적이 되기 때문에 제대로 된 탐색이 안됨 줄이 바뀌면(진짜 줄이 바뀌므로)
        //그 줄에서 일치하는 문자열을 찾지 못했다는 뜻으로 다음 줄에서 다시 일치하는 문자열을 찾겠다는 의미!
        //그렇기 때문에 다음줄에서는 다시 처음부터 찾아야하기 때문에 일치하는 문자열과 관련된 사항들을 모두
        //0으로 초기화 시켜서 다시 시작해준다. 그렇게 해야만 진짜줄 여러줄에 걸쳐서 탐색되는 경우가 없다
        //진짜줄에서는 한줄내에서만 탐색이 되야하기 때문에 여기서 진짜 줄이 바뀌기 때문에
        //찾을 문자열과 관련된 사항들을 모두 0으로 초기화해주고, 가짜줄은 진짜 줄이 바뀌기전에 같이 처리되기
        //때문에 가짜줄일 경우 여러줄에 걸쳐서 탐색이 가능하다.
    }
    //9. 못찾았으면
    if (isMatched == false)
    {
        //4.1 원래 캐럿이 있던 곳에 그대로 있는다.
        *findingEndRowIndex = this->note->GetCurrent();
        *findingEndLetterIndex = this->note->GetAt(*findingEndRowIndex)->GetCurrent();
        *findingStartRowIndex = this->note->GetCurrent();
        *findingStartLetterIndex = this->note->GetAt(*findingStartRowIndex)->GetCurrent();
    }
    //10. 끝내다.
}

//소멸자
GlyphFinder::~GlyphFinder()
{

}