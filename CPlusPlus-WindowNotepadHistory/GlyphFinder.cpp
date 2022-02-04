#include "GlyphFinder.h"
#include "Glyph.h"
#include "DummyRow.h"
#include "RowAutoChange.h"

//#include<Findtext.dlg> ã������ȭ���ڸ� Ŀ���͸���¡�ϱ� ���ؼ� �ʿ���
//�̰� ���� ���� Ctrl + F5 �� �ϸ� ������ ���µ� ��������� Ŭ���ؼ� FindText.Dlg�� ����
//FindDialog�� ������ �����ؼ� resource���� Dialog�� �����ѵ� �ű⿡ ������ ������
//�ٿ������� Ŀ���͸���¡ �� ã�� ��ȭ���ڰ� ������!

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


//����Ʈ������
GlyphFinder::GlyphFinder(Glyph* note)
{
	this->note = note;
}

//�Ʒ��� ã��
void GlyphFinder::FindDown(string keyword, Long* findingRowIndex, Long* findingLetterIndex,
	Long* keywordLetterCount)
{
	//1. ã�� ���ڿ��� �Է¹޴´�.
	//2. ���� ���� ��ġ�� ���� ��ġ�� ���Ѵ�.
    Long currentRowIndex = this->note->GetCurrent();
    Long currentLetterIndex = this->note->GetAt(currentRowIndex)->GetCurrent();
    string currentLetters = this->note->GetAt(currentRowIndex)
        ->GetPartOfContent(currentLetterIndex);
    Long currentLettersLength = currentLetters.length();
    //3. ���� ���� ��ġ�� note�� ���� �������� �������� �׸��� �ش繮�ڿ��� ã�� ������ �ݺ��Ѵ�.
    bool isMatched = false;
    *findingRowIndex = -1;
    *findingLetterIndex = -1;
    *keywordLetterCount = 0;// ���ڿ��� ���̸��� ã�� ���ڿ��� ���ڰ���
    Long matchedLettersCount = 0;
    Long keywordLength = keyword.length();// ã�� ���ڿ��� ����(�ѱ��� 2, ���� �� Ư�����ڴ� 1)
    Long matchedLength = 0;// ã�� ���ڿ��� ���� ���ڵ��� ���� ��ġ�ϴ� ���ڿ��� ����
    Glyph* row = 0;
    Glyph* nextRow = 0;
    string letterContentOfRow;
    string partContentOfKeyword;
    Long nextRowIndex = 0;
    string realRowContent;
    string dummyRowContent;
    Long rowContentLength = 0;
    Long contentLength = 0;
    Long i = 0;//���� content���� ���ڸ� ���� �迭��� �� �ݺ�������
    Long j = 0;//ã�� ���ڿ��� content���� ���ڸ� ���� �迭��� �� �ݺ������
    while (currentRowIndex < this->note->GetLength() && isMatched == false)
    {
        //3.1 ���� ���Ѵ�.
        row = this->note->GetAt(currentRowIndex);
        //3.2 ���� content�� ���Ѵ�.
        realRowContent = row->GetContent();
        //3.3 ���� ���� ���Ѵ�.
        nextRowIndex = currentRowIndex + 1;
        nextRow = this->note->GetAt(currentRowIndex + 1);
        //3.4 ���� ���� ��ġ�� ���� �������� �������� �׸��� ���� ���� ��¥ ���� ���� �ݺ��Ѵ�.
        while (nextRowIndex < this->note->GetLength() &&
            dynamic_cast<DummyRow*>(nextRow))
        {
            //3.4.1 ��¥���� content�� ���Ѵ�.
            dummyRowContent = nextRow->GetContent();
            //3.4.2 ��¥���� content�� ��¥ ���� content�� �����ش�.
            realRowContent += dummyRowContent;
            //3.4.3 ���� �ٷ� �̵��Ѵ�.
            nextRowIndex++;
            //3.4.4 ���� ���� ���Ѵ�.
            nextRow = this->note->GetAt(nextRowIndex);
        }

        currentRowIndex = nextRowIndex - 1;

        //3.5 ���� content ���̸� ���Ѵ�.
        rowContentLength = realRowContent.length();
        //3.6 �ٿ��� ���� ���� ��ġ�� �ʱ�ȭ��Ų��.
        i = currentLettersLength;
        //3.7 ���� content ���̸�ŭ �׸��� �ش� ���ڿ��� ã�������� �ݺ��Ѵ�.
        while (i < rowContentLength && isMatched == false)
        {
            //3.7.2 �ٿ��� i��° ���� ���ڰ� �ѱ��̸�
            if ((realRowContent[i] & 0x80))//�ѱ��̸�(2byte����)
            {
                //3.7.2.1 2byte�� �����Ѵ�.
                letterContentOfRow = realRowContent[i];
                i++;
                letterContentOfRow += realRowContent[i];
            }
            //3.7.3 �ѱ��� �ƴϸ�
            else
            {
                //3.7.3.1 1byte�� �����Ѵ�
                letterContentOfRow = realRowContent[i];
            }
            //3.7.4 ã�� ���ڿ����� j��° ���� ���ڰ� �ѱ��̸�
            if ((keyword[j] & 0x80))//�ѱ��̸�(2byte����)
            {
                //3.7.4.1 2byte�� �����Ѵ�.
                partContentOfKeyword = keyword[j];
                j++;
                partContentOfKeyword += keyword[j];
                //3.7.4.2 ���̸� 2�� �����Ѵ�(�ѱ��� ���̸� 2 �����ϱ� ������)
                contentLength = 2;
            }
            //3.7.5 �ѱ��� �ƴϸ�
            else
            {
                //3.7.5.1 1byte�� �����Ѵ�
                partContentOfKeyword = keyword[j];
                //3.7.5.2 ���̸� 1�� �����Ѵ�.(�ѱ��� ������ ���� �� Ư�����ڴ� ���̰� 1�̱� ������)
                contentLength = 1;
            }
            //3.7.6 �ٿ��� ���� ���ڿ� ã�� ���ڿ����� ���� ���ڰ� ���� ��ġ�ϸ�
            if (letterContentOfRow == partContentOfKeyword)
            {
                //3.7.6.1 ��ġ�ϴ� ���ڿ��� ���̸� ���ش�.
                matchedLength += contentLength;
                //3.7.6.2 ��ġ�ϴ� ���ڿ��� ���� ������ ���ش�.
                matchedLettersCount++;
                //3.7.6.3 ã�� ���ڿ����� ���� ������ġ�� �������� �̵���Ų��.
                j++;
            }
            //3.7.7 �ٿ��� ���� ���ڿ� ã�� ���ڿ����� ���� ���ڰ� ���� �ٸ���
            else
            {
                //3.7.7.1 ��ġ�ϴ� ���ڿ��� ���̸� 0���� �ʱ�ȭ���ش�.
                matchedLength = 0;
                //3.7.7.2 ��ġ�ϴ� ���ڿ��� ���� ������ 0���� �ʱ�ȭ���ش�.
                matchedLettersCount = 0;
                //3.7.7.3 ã�� ���ڿ����� ���� ������ġ�� ó������ �̵���Ų��.
                j = 0;
            }
            //�޸��忡�� ���� ������ ��ġ�� �������� �̵���Ų��.
            currentLetterIndex++;
            //3.7.8 ã�� ���ڿ��� row�� content���� ã������
            //��ġ�ϴ� ���ڿ��� ���̿� ã�� ���ڿ��� ���̰� ��ġ�ϸ�
            if(matchedLength == keywordLength)
            {
                //3.7.8.1 ã�Ҵٰ� ǥ���Ѵ�.
                isMatched = true;
                //�ڵ������� �տ��� �̹� ����߱� ������ ���⼭�� �ڵ����࿡ ���ؼ� ���� ���� �ʿ�X
                //3.7.8.2 �ڵ������� �Ǿ��� �ȵǾ��� �ڵ������� �� ���¿���
                //���� ��ġ�� ������ġ�� �����ش�.
                RowAutoChange rowAutoChange(this->notepadForm);
                Long changedRowPos = 0;
                Long changedLetterPos = 0;
                Long originRowPos = currentRowIndex;
                Long originLetterPos = currentLetterIndex;
                //3.7.8.3 ����� ȭ�� ũ�⿡ �´� �ٰ� ĳ���� ��ġ�� ���Ѵ�.
                rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
                   &changedRowPos);
                //3.7.8.4 ã�� ���� ��ġ�� ã�� ���� ��ġ, �׸��� ã�� ���ڿ��� ���� ������ ����Ѵ�.
                *findingRowIndex = currentRowIndex;
                *findingLetterIndex = currentLetterIndex;
                *keywordLetterCount = matchedLettersCount;
            }
            //3.7.9 ���� content���� ���� ������ �д´�.
            i++;
        }
        //3.8 currentLettersLength�� 0���� �ʱ�ȭ���ش�.
        currentLettersLength = 0;
        currentLetterIndex = 0;
        //3.9 ���� �ٷ� �̵��Ѵ�.
        currentRowIndex++;
    }
    //4. ������.
}

//�Ҹ���
GlyphFinder::~GlyphFinder()
{

}