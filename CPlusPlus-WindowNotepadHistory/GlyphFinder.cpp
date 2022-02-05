#include "GlyphFinder.h"
#include "Glyph.h"
#include "DummyRow.h"
//#include<Findtext.dlg>// ã������ȭ���ڿ� �ٲٱ�����ȭ���ڸ� Ŀ���͸���¡�ϱ� ���ؼ� �ʿ���
//�̰� ���� ���� Ctrl + F5 �� �ϸ� ������ ���µ� ��������� Ŭ���ؼ� FindText.Dlg�� ����
//FindDialog�� ReplaceDialog�� ������ �����ؼ� .rc�� �޸��忡�� �ؽ�Ʈ�� ��� �ű⿡ ������ ������
//�ٿ������� Ŀ���͸���¡ �� ã�� �Ǵ� �ٲٱ� ��ȭ���ڰ� ������!

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
void GlyphFinder::FindDown(string keyword, Long* findingStartRowIndex,
    Long* findingStartLetterIndex, Long* findingEndRowIndex, Long* findingEndLetterIndex)
{
    //1. ã�� ���ڿ��� �Է¹޴´�.
    //2. �޸��忡�� ���� ���� ��ġ�� ���Ѵ�.
    Long currentRowIndex = this->note->GetCurrent();
    //3. �޸��忡�� ���� ������ ��ġ�� ���Ѵ�.
    Long currentLetterIndex = this->note->GetAt(currentRowIndex)->GetCurrent();
    //4. �޸����� ���� �ٿ��� ���� ���ڱ����� content�� ���Ѵ�.
    string currentLetters = this->note->GetAt(currentRowIndex)
        ->GetPartOfContent(currentLetterIndex);
    //5. �޸����� ���� �ٿ��� ���� ���ڱ����� content�� ���̸� ���Ѵ�.
    Long currentLettersLength = currentLetters.length();
    //6. ã�� ���ڿ��� ã�Ҵ��� ���θ� �ϴ� ��ã�ұ� ������ false�� �ʱ�ȭ�Ѵ�.
    bool isMatched = false;
    //7. ����� �������� �ݺ��� �ۿ��� �̸� ���� �� �ʱ�ȭ��Ų��.
    Long matchedLettersCount = 0;//�ٿ��� ã�� ���ڿ� ã�� ���ڿ��� ���ڰ� ���� ��ġ�ϴ� ���ڰ���
    Long keywordLength = keyword.length();// ã�� ���ڿ��� ����(�ѱ��� 2, ���� �� Ư�����ڴ� 1)
    Long matchedLength = 0;// ã�� ���ڿ��� ���� ���ڵ��� ���� ��ġ�ϴ� ���ڿ��� ����
    Glyph* row = 0;
    Glyph* nextRow = 0;
    string letterContentOfRow;//���� �� ���ڸ� ���� ����
    string partContentOfKeyword;//ã�� ���ڿ��� �� ���ڸ� ���� ����
    Long nextRowIndex = 0;
    string realRowContent;//��¥ ���� content�� ���� ����
    string dummyRowContent;//��¥ ���� content�� ���� ����
    Long rowContentLength = 0;//���� content�� ����
    Long contentLength = 0;//ã�� ���ڿ��� �ѱ����� ����
    Glyph* currentRow = 0;
    string currentRowContent;
    Long currentRowContentLength = 0;
    string nextRowContent;
    Long startLetterIndex = 0;
    Long previousRowIndex = 0;
    Long i = 0;//���� content���� ���ڸ� ���� �迭��� �� �ݺ�������
    Long j = 0;//ã�� ���ڿ��� content���� ���ڸ� ���� �迭��� �� �ݺ������
    //8. ���� ���� ��ġ�� note�� ���� �������� �������� �׸��� �ش繮�ڿ��� ã�� ������ �ݺ��Ѵ�.
    while (currentRowIndex < this->note->GetLength() && isMatched == false)
    {
        //8.1 ���� ���Ѵ�.
        row = this->note->GetAt(currentRowIndex);
        //8.2 ���� content�� ���Ѵ�.
        realRowContent = row->GetContent();
        //8.3 ���� ���� ���Ѵ�.
        nextRowIndex = currentRowIndex + 1;
        nextRow = this->note->GetAt(nextRowIndex);
        //�ڵ����� �ϴ� ��ó�� ��¥ ���� ������ ���� �ٿ� �߰��Ѵ� �ֳ��ϸ� �ڵ������� �ȵ� ���¿�����
        //���ٳ�������(��¥�� ���ٳ�����) ã�� �����ϴ°� ����������, �ڵ������� �� ���¿����� ���� ��¥ 
        //�ٿ��� ���ļ� ã������, ������ �����ϴ�(�������(�ڵ������� �ȵ� ���¿�����)�����̱� ������)
        //8.4 ���� ���� ��ġ�� ���� �������� �������� �׸��� ���� ���� ��¥ ���� ���� �ݺ��Ѵ�.
        while (nextRowIndex < this->note->GetLength() &&
            dynamic_cast<DummyRow*>(nextRow))
        {
            //8.4.1 ��¥���� content�� ���Ѵ�.
            dummyRowContent = nextRow->GetContent();
            //8.4.2 ��¥���� content�� ��¥ ���� content�� �����ش�.
            realRowContent += dummyRowContent;
            //8.4.3 ���� �ٷ� �̵��Ѵ�.
            nextRowIndex++;
            //8.4.4 ���� ���� ���Ѵ�.
            nextRow = this->note->GetAt(nextRowIndex);
        }
        //8.5 ���� content ���̸� ���Ѵ�.
        rowContentLength = realRowContent.length();
        //8.6 �ٿ��� ���� ���� ��ġ�� �ʱ�ȭ��Ų��.
        i = currentLettersLength;
        //8.7 ���� content ���̸�ŭ �׸��� �ش� ���ڿ��� ã�������� �ݺ��Ѵ�.
        while (i < rowContentLength && isMatched == false)
        {
            //8.7.1 �ٿ��� i��° ���� ���ڰ� �ѱ��̸�
            if ((realRowContent[i] & 0x80))//�ѱ��̸�(2byte����)
            {
                //8.7.1.1 2byte�� �����Ѵ�.
                letterContentOfRow = realRowContent[i];
                i++;
                letterContentOfRow += realRowContent[i];
            }
            //8.7.2 �ѱ��� �ƴϸ�
            else
            {
                //8.7.2.1 1byte�� �����Ѵ�
                letterContentOfRow = realRowContent[i];
            }
            //8.7.3 ã�� ���ڿ����� j��° ���� ���ڰ� �ѱ��̸�
            if ((keyword[j] & 0x80))//�ѱ��̸�(2byte����)
            {
                //8.7.3.1 2byte�� �����Ѵ�.
                partContentOfKeyword = keyword[j];
                j++;
                partContentOfKeyword += keyword[j];
                //8.7.3.2 ���̸� 2�� �����Ѵ�(�ѱ��� ���̸� 2 �����ϱ� ������)
                contentLength = 2;
            }
            //8.7.4 �ѱ��� �ƴϸ�
            else
            {
                //8.7.4.1 1byte�� �����Ѵ�
                partContentOfKeyword = keyword[j];
                //8.7.4.2 ���̸� 1�� �����Ѵ�.(�ѱ��� ������ ���� �� Ư�����ڴ� ���̰� 1�̱� ������)
                contentLength = 1;
            }
            //8.7.5 �ٿ��� ���� ���ڿ� ã�� ���ڿ����� ���� ���ڰ� ���� ��ġ�ϸ�
            if (letterContentOfRow == partContentOfKeyword)
            {
                //8.7.5.1 ��ġ�ϴ� ���ڿ��� ���̸� ���ش�.
                matchedLength += contentLength;
                //8.7.5.2 ��ġ�ϴ� ���ڿ��� ���� ������ ���ش�.
                matchedLettersCount++;
                //8.7.5.3 ã�� ���ڿ����� ���� ������ġ�� �������� �̵���Ų��.
                j++;
            }
            //8.7.6 �ٿ��� ���� ���ڿ� ã�� ���ڿ����� ���� ���ڰ� ���� �ٸ���
            else
            {
                //8.7.6.1 ��ġ�ϴ� ���ڿ��� ���̸� 0���� �ʱ�ȭ���ش�.
                matchedLength = 0;
                //8.7.6.2 ��ġ�ϴ� ���ڿ��� ���� ������ 0���� �ʱ�ȭ���ش�.
                matchedLettersCount = 0;
                //8.7.6.3 ã�� ���ڿ����� ���� ������ġ�� ó������ �̵���Ų��.
                j = 0;
            }
            //8.7.7 �޸����� ���� �ٿ��� ���� ��ġ�� ���ڸ� �о��� ������ �񱳸� �߱� ������
            //���� ������ ��ġ�� �������� �̵����Ѿ� ���� ���ڸ� �а� ã�� ���ڿ��� ���ڿ� ���� �� �ִ�.
            currentLetterIndex++;
            //8.7.8 ã�� ���ڿ��� row�� content���� ã������
            //��ġ�ϴ� ���ڿ��� ���̿� ã�� ���ڿ��� ���̰� ��ġ�ϸ�
            if (matchedLength == keywordLength)
            {
                //8.7.8.1 �ش� ���ڿ��� �ٿ��� ã�Ҵٰ� ǥ���Ѵ�.
                isMatched = true;
                //8.7.8.2 ���� ���� ���Ѵ�.
                currentRow = this->note->GetAt(currentRowIndex);
                //8.7.8.3 ���� ���� content�� ���Ѵ�.
                currentRowContent = currentRow->GetContent();
                //8.7.8.4 ���� ���� content�� length�� ���Ѵ�.
                currentRowContentLength = currentRowContent.length();
                //8.7.8.5 i�� ���� ���� content�� length���� ũ�ų� �������� �ݺ��Ѵ�.
                //�Ʊ� �տ��� �κ��ڵ������� ����ϴ� ��ó�� ��¥���� content���� ��¥���� content��
                //�߰��������� ��ǻ� ���� �޸��忡���� �ڵ������� �Ǿ� �ִ� �����̱� ������
                //�ڵ������� �� �޸��忡�� ã�� ���ڿ��� ã�Ƽ� ������ �ؾ��ϱ� ������ �Ʊ� ���ƴ� ������ 
                //��¥�ٵ��� �ٽ� ���������� ���ؼ� ��ģ �ٿ��� ���� ������ġ i�� ���ؼ� ���� ���� ��ġ��
                //�ٽ� �����ش�.
                while (i >= currentRowContentLength)
                {
                    //8.7.8.5.1 ���� ���� ��ġ�� ������Ų��.
                    //�ڵ������� ��ҵ� ���¿��� �ش� ���ڿ��� ã�Ҵµ� ���������� ���� ���ڰ�
                    //���� �������� ���� ���̺��� ��� ������ ���� �ٷ� �Ѿ�� �ٽ� ���ؾ��Ѵ�.
                    currentRowIndex++;
                    //8.7.8.5.2 ���� ������ġ�� �ڵ������� ��ҵ� ������ ������ġ�� ������ ���� ���� 
                    //��ġ�� ���� ���� �������� ũ�� ������ �׸�ŭ ���� �ڵ������� �� ���·� �����Ѵ�
                    //�տ��� currentLetterIndex++�� ���� 1�� ������ �����̱� ������ +1�� ���� �����൵��!
                    currentLetterIndex -= currentRow->GetLength();
                    //8.7.8.5.3 ���� ���� �������� ���Ѵ�.
                    currentRow = this->note->GetAt(currentRowIndex);
                    //8.7.8.5.4 ���� ���� content�� �����Ѵ�.
                    nextRowContent = currentRow->GetContent();
                    //8.7.8.5.5 ���� ���� content�� ���� ���� content�� �����ش�.(����)
                    currentRowContent += nextRowContent;
                    //8.7.8.5.6 ������ ���� content�� ���̸� ���Ѵ�.
                    currentRowContentLength = currentRowContent.length();
                }
                //8.7.8.6 ã�� ���� �� ��ġ�� ã�� ���� ����ġ�� ����Ѵ�.
                *findingEndRowIndex = currentRowIndex;
                *findingEndLetterIndex = currentLetterIndex;
                //8.7.8.7 ���� ���� ��ġ���� ã�� ���ڿ��� ���ڰ����� ���ش�.
                startLetterIndex = currentLetterIndex - matchedLettersCount;
                //8.7.8.8 ����̸�
                if (startLetterIndex >= 0)
                {
                    //8.7.8.8.1 ã�� ���� ������ġ�� ã�� ���� �� ��ġ�� ������ ������ ����Ѵ�.
                    *findingStartRowIndex = currentRowIndex;
                    //8.7.8.8.2 ã�� ������ ������ġ�� ������ �����ؼ� ����Ѵ�.
                    *findingStartLetterIndex = startLetterIndex;
                }
                //8.7.8.9 �����̸�
                else
                {
                    //8.7.8.9.1 ���� ���� ���� ���� ���Ѵ�.
                    previousRowIndex = currentRowIndex - 1;
                    row = this->note->GetAt(previousRowIndex);
                    //8.7.8.9.2 ���� �ٿ��� ���� �ٷ� �̵��߱� ������ 
                    //ã�� ���ڿ��� ���ڰ������� ���� ���� ������ġ�� ���ش�
                    matchedLettersCount -= currentLetterIndex;
                    //8.7.8.9.3 �� ���ڿ��� ���ڰ����� ���� ���� ���ڰ������� ū ���� �ݺ��Ѵ�.
                    while (matchedLettersCount > row->GetLength())
                    {
                        //8.7.8.9.3.1 ã�� ���ڿ��� ���ڰ������� ���� ���� ���ڰ����� ���ش�.
                        matchedLettersCount -= row->GetLength();
                        //8.7.8.9.3.2 ���� ���� ���Ѵ�.
                        previousRowIndex--;
                        row = this->note->GetAt(previousRowIndex);
                    }
                    //8.7.8.9.4 ã�� ���� ������ġ�� ���� ���� ��ġ�� ����Ѵ�.
                    *findingStartRowIndex = previousRowIndex;
                    //8.7.8.9.5 ã�� ������ ������ġ�� ���� ���� �������� ã�� ���ڿ��� ������ 
                    //�� ���� ����Ѵ�.
                    *findingStartLetterIndex = row->GetLength() - matchedLettersCount;
                }
            }
            //8.7.9 ���� content���� ���� ������ �д´�.
            i++;
        }
        //8.8 ã�� ���ڿ��� row�� content���� ��ã������
        if (isMatched == false)
        {
            //8.8.1 ���� ���� ��ġ�� ��¥�� ������ ��¥�ٱ����� �ٽ� �������Ѵ�.
            currentRowIndex = nextRowIndex - 1;
        }
        //8.9 ���� �ٷ� �̵��Ѵ�.
        currentRowIndex++;
        //8.10 ���� �ٷ� �̵��߱� ������ currentLettersLength�� currentLetterIndex�� 0���� �ʱ�ȭ���ش�.
        currentLettersLength = 0;
        currentLetterIndex = 0;
        //���⼭ �ʱ�ȭ�� �����ָ� ������ �Ǳ� ������ ����� �� Ž���� �ȵ� ���� �ٲ��(��¥ ���� �ٲ�Ƿ�)
        //�� �ٿ��� ��ġ�ϴ� ���ڿ��� ã�� ���ߴٴ� ������ ���� �ٿ��� �ٽ� ��ġ�ϴ� ���ڿ��� ã�ڴٴ� �ǹ�!
        //�׷��� ������ �����ٿ����� �ٽ� ó������ ã�ƾ��ϱ� ������ ��ġ�ϴ� ���ڿ��� ���õ� ���׵��� ���
        //0���� �ʱ�ȭ ���Ѽ� �ٽ� �������ش�. �׷��� �ؾ߸� ��¥�� �����ٿ� ���ļ� Ž���Ǵ� ��찡 ����
        //��¥�ٿ����� ���ٳ������� Ž���� �Ǿ��ϱ� ������ ���⼭ ��¥ ���� �ٲ�� ������
        //ã�� ���ڿ��� ���õ� ���׵��� ��� 0���� �ʱ�ȭ���ְ�, ��¥���� ��¥ ���� �ٲ������ ���� ó���Ǳ�
        //������ ��¥���� ��� �����ٿ� ���ļ� Ž���� �����ϴ�.
        //8.11 ��ġ�ϴ� ���ڿ��� ���̸� 0���� �ʱ�ȭ���ش�.
        matchedLength = 0;
        //8.12 ��ġ�ϴ� ���ڿ��� ���� ������ 0���� �ʱ�ȭ���ش�.
        matchedLettersCount = 0;
        //8.13 ã�� ���ڿ����� ���� ������ġ�� ó������ �̵���Ų��.
        j = 0;
    }
    //9. ��ã������
    if (isMatched == false)
    {
        //4.1 ���� ĳ���� �ִ� ���� �״�� �ִ´�.
        *findingEndRowIndex = this->note->GetCurrent();
        *findingEndLetterIndex = this->note->GetAt(*findingEndRowIndex)->GetCurrent();
        *findingStartRowIndex= this->note->GetCurrent();
        *findingStartLetterIndex = this->note->GetAt(*findingStartRowIndex)->GetCurrent();
    }
    //10. ������.
}

//���� ã��
void GlyphFinder::FindUp(string keyword, Long* findingStartRowIndex, Long* findingStartLetterIndex,
    Long* findingEndRowIndex, Long* findingEndLetterIndex)
{
    //1. ã�� ���ڿ��� �Է¹޴´�.
    //2. �޸��忡�� ���� ���� ��ġ�� ���Ѵ�.
    Long currentRowIndex = this->note->GetCurrent();
    //3. �޸��忡�� ���� ������ ��ġ�� ���Ѵ�.
    Long currentLetterIndex = this->note->GetAt(currentRowIndex)->GetCurrent();
    //4. �޸����� ���� �ٿ��� ���� ���ڱ����� content�� ���Ѵ�.
    string currentLetters = this->note->GetAt(currentRowIndex)
        ->GetPartOfContent(currentLetterIndex);
    //5. �޸����� ���� �ٿ��� ���� ���ڱ����� content�� ���̸� ���Ѵ�.
    Long currentLettersLength = currentLetters.length();
    //6. ã�� ���ڿ��� ã�Ҵ��� ���θ� �ϴ� ��ã�ұ� ������ false�� �ʱ�ȭ�Ѵ�.
    bool isMatched = false;
    //7. ����� �������� �ݺ��� �ۿ��� �̸� ���� �� �ʱ�ȭ��Ų��.
    Long matchedLettersCount = 0;//�ٿ��� ã�� ���ڿ� ã�� ���ڿ��� ���ڰ� ���� ��ġ�ϴ� ���ڰ���
    Long keywordLength = keyword.length();// ã�� ���ڿ��� ����(�ѱ��� 2, ���� �� Ư�����ڴ� 1)
    Long matchedLength = 0;// ã�� ���ڿ��� ���� ���ڵ��� ���� ��ġ�ϴ� ���ڿ��� ����
    Glyph* row = 0;
    Long realRowIndex = 0;
    Glyph* previousRow = 0;
    string letterContentOfRow;//���� �� ���ڸ� ���� ����
    string partContentOfKeyword;//ã�� ���ڿ��� �� ���ڸ� ���� ����
    Long previousRowIndex = -1;
    string rowContent;//���� content�� ���� ����
    string previousRowContent;//���� ���� content�� ���� ����
    Long contentLength = 0;//ã�� ���ڿ��� �ѱ����� ����
    Glyph* currentRow = 0;
    string currentRowContent;
    Long currentRowContentLength = 0;
    Long previousRowContentLength = 0;
    Long sum = 0;
    Glyph* nextRow = 0;
    Long nextRowIndex = 0;
    string koreanCompositionTemp;//�������� �Ž��� �ö󰡱� ������ �������� �ѱ��� �����ϸ� �ѱ��� �����
    //�������� �ʱ� ������ �ѱ��� �ӽ÷� �����ϰ� ���� ������� ������ �ӽ� ����Ұ� �ʿ���.
    Long i = 0;//���� content���� ���ڸ� ���� �迭��� �� �ݺ�������
    Long j = keywordLength - 1;//ã�� ���ڿ��� content���� ���ڸ� ���� �迭��� �� �ݺ������
    //8. ���� ���� ��ġ�� 0���� ũ�ų� ���� ���� �׸��� �ش繮�ڿ��� ã�� ������ �ݺ��Ѵ�.
    while (currentRowIndex >= 0 && isMatched == false)
    {
        //8.1 ���� ���� ���Ѵ�.
        row = this->note->GetAt(currentRowIndex);
        //8.2 ���� ���� content�� ���Ѵ�.
        rowContent = row->GetPartOfContent(currentLetterIndex);
        //8.2 ���� ���� ��¥ ���̸�(���� ���� ��ġ�� 0�̸� ��¥ ���� �� X)
        if (dynamic_cast<DummyRow*>(row))
        {

            //8.2.2 ���� �ٷ� �̵��Ѵ�.
            previousRowIndex = currentRowIndex - 1;
            //8.2.3 ���� ���� ���Ѵ�
            previousRow = this->note->GetAt(previousRowIndex);
            //8.2.4 ���� ���� ��¥���ε��� �ݺ��Ѵ�.
            while (dynamic_cast<DummyRow*>(previousRow))
            {
                //8.2.4.1 ���� ���� content�� ���Ѵ�.
                previousRowContent = previousRow->GetContent();
                //8.2.4.2 ���� ���� content�� ���� ���� cotent�� �����ش�.(����)
                previousRowContent += rowContent;
                //8.2.4.3 ������ ���� �Ű� �����Ѵ�.(�ݺ� �������� ����� ������Ű�� ���ؼ�)
                rowContent = previousRowContent;
                //8.2.4.4 �޸����� ���� ������ġ�� ���� ���� ���ڰ����� �����ش�.
                currentLetterIndex += previousRow->GetLength();
                //8.2.4.5 �������� �̵��Ѵ�.
                previousRowIndex--;
                //8.2.4.6 ���� ���� ���Ѵ�.
                previousRow = this->note->GetAt(previousRowIndex);
            }
            //�������� �̵����� �� ��¥ ���� ���ͼ� �ݺ������� ���Ա� ������ ��¥ �ٿ� �ݺ���������
            //��ģ content�� �����ش�.
            //8.2.5 �ڵ������� �κ�����ϰ� ��¥ ���� ��ġ�� ã�Ƽ�
            //��¥ ���� ��ġ�� ���� ���� ��ġ�� �����Ѵ�.
            currentRowIndex = previousRowIndex;
            //8.2.6 ��¥ ���� content�� ���Ѵ�.
            previousRowContent = previousRow->GetContent();
            //8.2.7 ��¥ ���� content�� ������Ų��.
            previousRowContent += rowContent;
            //8.2.8 ������ ���� �Ű� �����Ѵ�.(�ݺ� �������� ����� ������Ű�� ���ؼ�)
            rowContent = previousRowContent;
            //8.2.9 �޸����� ���� ������ġ�� ���� ���� ���ڰ����� �����ش�.
            currentLetterIndex += previousRow->GetLength();
            //8.2.10 ���� ���� ���̸� ���Ѵ�.(�ڵ����� ��ҵ� ������ ������ġ���� ����)
            currentLettersLength = rowContent.length();
        }
        
        realRowIndex = currentRowIndex;

        //8.3 �ٿ��� ���� ���� ��ġ�� �ʱ�ȭ��Ų��.
        i = currentLettersLength - 1;
        //8.4 ���� ���� ��ġ�� 0���� ũ�ų� ���� ���� �׸��� �ش繮�ڿ��� ã�������� �ݺ��Ѵ�.
        while (i >= 0 && isMatched == false)
        {
            //8.4.1 �ٿ��� i��° ���� ���ڰ� �ѱ��̸�
            if ((rowContent[i] & 0x80))//�ѱ��̸�(2byte����)
            {
                //8.4.1.1 2byte�� �����Ѵ�.
                letterContentOfRow = rowContent[i];
                //�������� �Ž��� ���� ������ ����� �ѱ��� �����ϱ� ���ؼ� �ӽ������� �ؾ���.
                koreanCompositionTemp = letterContentOfRow;
                i--;
                letterContentOfRow = rowContent[i];
                letterContentOfRow += koreanCompositionTemp;
            }
            //8.4.2 �ѱ��� �ƴϸ�
            else
            {
                //8.4.2.1 1byte�� �����Ѵ�
                letterContentOfRow = rowContent[i];
            }
            //8.4.3 ã�� ���ڿ����� j��° ���� ���ڰ� �ѱ��̸�
            if ((keyword[j] & 0x80))//�ѱ��̸�(2byte����)
            {
                //8.4.3.1 2byte�� �����Ѵ�.
                partContentOfKeyword = keyword[j];
                //�������� �Ž��� ���� ������ ����� �ѱ��� �����ϱ� ���ؼ� �ӽ������� �ؾ���.
                koreanCompositionTemp = partContentOfKeyword;
                j--;
                partContentOfKeyword = keyword[j];
                partContentOfKeyword += koreanCompositionTemp;
                //8.4.3.2 ���̸� 2�� �����Ѵ�(�ѱ��� ���̸� 2 �����ϱ� ������)
                contentLength = 2;
            }
            //8.4.4 �ѱ��� �ƴϸ�
            else
            {
                //8.4.4.1 1byte�� �����Ѵ�
                partContentOfKeyword = keyword[j];
                //8.4.4.2 ���̸� 1�� �����Ѵ�.(�ѱ��� ������ ���� �� Ư�����ڴ� ���̰� 1�̱� ������)
                contentLength = 1;
            }
            //8.4.5 �ٿ��� ���� ���ڿ� ã�� ���ڿ����� ���� ���ڰ� ���� ��ġ�ϸ�
            if (letterContentOfRow == partContentOfKeyword)
            {
                //8.4.5.1 ��ġ�ϴ� ���ڿ��� ���̸� ���ش�.
                matchedLength += contentLength;
                //8.4.5.2 ��ġ�ϴ� ���ڿ��� ���� ������ ���ش�.
                matchedLettersCount++;
                //8.4.5.3 ã�� ���ڿ����� ���� ������ġ�� �������� �̵���Ų��.
                j--;
            }
            //8.4.6 �ٿ��� ���� ���ڿ� ã�� ���ڿ����� ���� ���ڰ� ���� �ٸ���
            else
            {
                //8.4.6.1 ��ġ�ϴ� ���ڿ��� ���̸� 0���� �ʱ�ȭ���ش�.
                matchedLength = 0;
                //8.4.6.2 ��ġ�ϴ� ���ڿ��� ���� ������ 0���� �ʱ�ȭ���ش�.
                matchedLettersCount = 0;
                //8.4.6.3 ã�� ���ڿ����� ���� ������ġ�� ���������� �̵���Ų��.
                j = keywordLength - 1;
            }
            //8.4.7 �޸����� ���� �ٿ��� ���� ��ġ�� ���ڸ� �о��� ������ �񱳸� �߱� ������
            //���� ������ ��ġ�� �������� �̵����Ѿ� ���� ���ڸ� �а� ã�� ���ڿ��� ���� ���ڿ� ���� �� �ִ�.
            currentLetterIndex--;
            //8.4.8 ã�� ���ڿ��� row�� content���� ã������
           //��ġ�ϴ� ���ڿ��� ���̿� ã�� ���ڿ��� ���̰� ��ġ�ϸ�
            if (matchedLength == keywordLength)
            {
                //8.4.8.1 �ش� ���ڿ��� �ٿ��� ã�Ҵٰ� ǥ���Ѵ�.
                isMatched = true;
                //8.4.8.2 �޸��忡�� ���� ���� ���Ѵ�.(��¥ ��)
                currentRow = this->note->GetAt(currentRowIndex);
                //8.4.8.3 ���� ���� content�� ���Ѵ�.
                currentRowContent = currentRow->GetContent();
                //8.4.8.4 ���� ���� length�� ���Ѵ�.
                currentRowContentLength = currentRowContent.length();
                //8.4.8.5 i�� ���� ���� length���� ũ�ų� �������� �ݺ��Ѵ�.
                while (i >= currentRowContentLength)
                {
                    //8.4.8.5.1 ���� ���� ���̸� ����.
                    i -= currentRowContentLength;
                    //8.4.8.5.2 ���� ���� ������ ����.
                    currentLetterIndex -= currentRow->GetLength();
                    //8.4.8.5.3 ���� �ٷ� �̵��Ѵ�.
                    currentRowIndex++;
                    //8.4.8.5.4 ���� ���� ���Ѵ�.
                    currentRow = this->note->GetAt(currentRowIndex);
                    //8.4.8.5.5 ���� ���� content�� ���Ѵ�.
                    currentRowContent = currentRow->GetContent();
                    //8.4.8.5.6 ���� ���� content�� length�� ���Ѵ�.
                    currentRowContentLength = currentRowContent.length(); 
                }
                //8.6.8.6 ã�� ���� ���� ��ġ�� ã�� ���� ���� ��ġ�� ����Ѵ�.
                *findingStartRowIndex = currentRowIndex;
                *findingStartLetterIndex = currentLetterIndex;
                //8.6.8.6 ã�� ������ ������ġ���� ��ġ�ϴ� ���ڿ��� ���ڰ����� �����ش�.
                sum = currentLetterIndex + matchedLettersCount;
                //8.6.8.7 ���Ѱ��� ã�� ���� ������ġ�� �ش��ϴ� ���� ���ڰ������� �۰ų� ������
                if (sum <= this->note->GetAt(currentRowIndex)->GetLength())
                {
                    //8.6.8.7.1 ã�� ���� ����ġ�� ã�� ���� ���� ��ġ�� ������ ������ ����Ѵ�.
                    *findingEndRowIndex = currentRowIndex;
                    //8.6.8.7.2 ã�� ������ ����ġ�� ���Ѱ����� ����Ѵ�.
                    *findingEndLetterIndex = sum;
                }
                //8.6.8.8 ���� ���� ���� ���ڰ������� ũ��
                else
                {
                    //8.6.8.8.1 ���� ���� ���Ѵ�.
                    currentRow = this->note->GetAt(currentRowIndex);
                    //8.6.8.8.2 ���� ���� ���� ���� ���Ѵ�.
                    nextRowIndex = currentRowIndex + 1;
                    nextRow = this->note->GetAt(nextRowIndex);
                    //8.6.8.8.3 ���� �ٿ��� ���� �ٷ� �̵��߱� ������ ���� ������ ���� ���� ������ŭ ���ش�.
                    sum -= currentRow->GetLength();
                    //8.6.8.8.4 ���� ���� ���� ���� ���ڰ������� ū ���� �ݺ��Ѵ�.
                    while (sum > nextRow->GetLength())
                    {
                        //8.6.8.8.4.1 ���� ������ ���� ���� ���ڰ����� ���ش�.
                        sum -= nextRow->GetLength();
                        //8.6.8.8.4.2 ���� ���� ���Ѵ�.
                        nextRowIndex--;
                        nextRow = this->note->GetAt(nextRowIndex);
                    }
                    //8.6.8.8.5 ã�� ���� ����ġ�� ���� �ٷ� ���Ѵ�.
                    *findingEndRowIndex = nextRowIndex;
                    //8.6.8.8.6 ã�� ������ ����ġ�� ���� ������ ���Ѵ�.
                    *findingEndLetterIndex = sum;
                }
            }
            //8.6.8.9 ���� content���� ���� ���ڷ� �̵��Ѵ�.
            i--;
        }
        //8.8 ã�� ���ڿ��� row�� content���� ��ã������
        if (isMatched == false)
        {
            //8.8.1 ���� ���� ��ġ�� ��¥�� ������ ��¥�ٱ����� �ٽ� �������Ѵ�.
            currentRowIndex = realRowIndex;
        }
        //8.8 ���� �ٷ� �̵��Ѵ�.
        currentRowIndex--;
        if (currentRowIndex >= 0)
        {
            //8.9 ���� ���� ���Ѵ�.
            previousRow = this->note->GetAt(currentRowIndex);
            //8.10 ���� �ٷ� �̵��߱� ������ �ٿ��� ���� ������ġ�� ���� ���� content ���̷� �����Ѵ�.
            previousRowContent = previousRow->GetContent();
            previousRowContentLength = previousRowContent.length();
            currentLettersLength = previousRowContentLength;
            //8.11 ���� ���� ��ġ�� ���� ���� ���� ������ �����Ѵ�.
            currentLetterIndex = previousRow->GetLength();
        }
        //8.11 ��ġ�ϴ� ���ڿ��� ���̸� 0���� �ʱ�ȭ���ش�.
        matchedLength = 0;
        //8.12 ��ġ�ϴ� ���ڿ��� ���� ������ 0���� �ʱ�ȭ���ش�.
        matchedLettersCount = 0;
        //8.13 ã�� ���ڿ����� ���� ������ġ�� ���������� �̵���Ų��.
        j = keywordLength - 1;
        //���⼭ �ʱ�ȭ�� �����ָ� ������ �Ǳ� ������ ����� �� Ž���� �ȵ� ���� �ٲ��(��¥ ���� �ٲ�Ƿ�)
        //�� �ٿ��� ��ġ�ϴ� ���ڿ��� ã�� ���ߴٴ� ������ ���� �ٿ��� �ٽ� ��ġ�ϴ� ���ڿ��� ã�ڴٴ� �ǹ�!
        //�׷��� ������ �����ٿ����� �ٽ� ó������ ã�ƾ��ϱ� ������ ��ġ�ϴ� ���ڿ��� ���õ� ���׵��� ���
        //0���� �ʱ�ȭ ���Ѽ� �ٽ� �������ش�. �׷��� �ؾ߸� ��¥�� �����ٿ� ���ļ� Ž���Ǵ� ��찡 ����
        //��¥�ٿ����� ���ٳ������� Ž���� �Ǿ��ϱ� ������ ���⼭ ��¥ ���� �ٲ�� ������
        //ã�� ���ڿ��� ���õ� ���׵��� ��� 0���� �ʱ�ȭ���ְ�, ��¥���� ��¥ ���� �ٲ������ ���� ó���Ǳ�
        //������ ��¥���� ��� �����ٿ� ���ļ� Ž���� �����ϴ�.
    }
    //9. ��ã������
    if (isMatched == false)
    {
        //4.1 ���� ĳ���� �ִ� ���� �״�� �ִ´�.
        *findingEndRowIndex = this->note->GetCurrent();
        *findingEndLetterIndex = this->note->GetAt(*findingEndRowIndex)->GetCurrent();
        *findingStartRowIndex = this->note->GetCurrent();
        *findingStartLetterIndex = this->note->GetAt(*findingStartRowIndex)->GetCurrent();
    }
    //10. ������.
}

//_stricmp;//��ҹ��� ���� �����ϴ� �Լ�(���� ��ҹ��ڸ� ���� ���� ���ִ� �Լ�)

//��/�ҹ��� ���о��� �Ʒ��� ã��
void GlyphFinder::FindDownWithMatchCase(string keyword, Long* findingStartRowIndex,
    Long* findingStartLetterIndex, Long* findingEndRowIndex, Long* findingEndLetterIndex)
{
    //1. ã�� ���ڿ��� �Է¹޴´�.
   //2. �޸��忡�� ���� ���� ��ġ�� ���Ѵ�.
    Long currentRowIndex = this->note->GetCurrent();
    //3. �޸��忡�� ���� ������ ��ġ�� ���Ѵ�.
    Long currentLetterIndex = this->note->GetAt(currentRowIndex)->GetCurrent();
    //4. �޸����� ���� �ٿ��� ���� ���ڱ����� content�� ���Ѵ�.
    string currentLetters = this->note->GetAt(currentRowIndex)
        ->GetPartOfContent(currentLetterIndex);
    //5. �޸����� ���� �ٿ��� ���� ���ڱ����� content�� ���̸� ���Ѵ�.
    Long currentLettersLength = currentLetters.length();
    //6. ã�� ���ڿ��� ã�Ҵ��� ���θ� �ϴ� ��ã�ұ� ������ false�� �ʱ�ȭ�Ѵ�.
    bool isMatched = false;
    //7. ����� �������� �ݺ��� �ۿ��� �̸� ���� �� �ʱ�ȭ��Ų��.
    Long matchedLettersCount = 0;//�ٿ��� ã�� ���ڿ� ã�� ���ڿ��� ���ڰ� ���� ��ġ�ϴ� ���ڰ���
    Long keywordLength = keyword.length();// ã�� ���ڿ��� ����(�ѱ��� 2, ���� �� Ư�����ڴ� 1)
    Long matchedLength = 0;// ã�� ���ڿ��� ���� ���ڵ��� ���� ��ġ�ϴ� ���ڿ��� ����
    Glyph* row = 0;
    Glyph* nextRow = 0;
    string letterContentOfRow;//���� �� ���ڸ� ���� ����
    string partContentOfKeyword;//ã�� ���ڿ��� �� ���ڸ� ���� ����
    Long nextRowIndex = 0;
    string realRowContent;//��¥ ���� content�� ���� ����
    string dummyRowContent;//��¥ ���� content�� ���� ����
    Long rowContentLength = 0;//���� content�� ����
    Long contentLength = 0;//ã�� ���ڿ��� �ѱ����� ����
    Glyph* currentRow = 0;
    string currentRowContent;
    Long currentRowContentLength = 0;
    string nextRowContent;
    Long startLetterIndex = 0;
    Long previousRowIndex = 0;
    Long i = 0;//���� content���� ���ڸ� ���� �迭��� �� �ݺ�������
    Long j = 0;//ã�� ���ڿ��� content���� ���ڸ� ���� �迭��� �� �ݺ������
    //8. ���� ���� ��ġ�� note�� ���� �������� �������� �׸��� �ش繮�ڿ��� ã�� ������ �ݺ��Ѵ�.
    while (currentRowIndex < this->note->GetLength() && isMatched == false)
    {
        //8.1 ���� ���Ѵ�.
        row = this->note->GetAt(currentRowIndex);
        //8.2 ���� content�� ���Ѵ�.
        realRowContent = row->GetContent();
        //8.3 ���� ���� ���Ѵ�.
        nextRowIndex = currentRowIndex + 1;
        nextRow = this->note->GetAt(nextRowIndex);
        //�ڵ����� �ϴ� ��ó�� ��¥ ���� ������ ���� �ٿ� �߰��Ѵ� �ֳ��ϸ� �ڵ������� �ȵ� ���¿�����
        //���ٳ�������(��¥�� ���ٳ�����) ã�� �����ϴ°� ����������, �ڵ������� �� ���¿����� ���� ��¥ �ٿ���
        //���ļ� ã������, ������ �����ϴ�(�������(�ڵ������� �ȵ� ���¿�����)�����̱� ������)
        //8.4 ���� ���� ��ġ�� ���� �������� �������� �׸��� ���� ���� ��¥ ���� ���� �ݺ��Ѵ�.
        while (nextRowIndex < this->note->GetLength() &&
            dynamic_cast<DummyRow*>(nextRow))
        {
            //8.4.1 ��¥���� content�� ���Ѵ�.
            dummyRowContent = nextRow->GetContent();
            //8.4.2 ��¥���� content�� ��¥ ���� content�� �����ش�.
            realRowContent += dummyRowContent;
            //8.4.3 ���� �ٷ� �̵��Ѵ�.
            nextRowIndex++;
            //8.4.4 ���� ���� ���Ѵ�.
            nextRow = this->note->GetAt(nextRowIndex);
        }
        //8.5 ���� content ���̸� ���Ѵ�.
        rowContentLength = realRowContent.length();
        //8.6 �ٿ��� ���� ���� ��ġ�� �ʱ�ȭ��Ų��.
        i = currentLettersLength;
        //8.7 ���� content ���̸�ŭ �׸��� �ش� ���ڿ��� ã�������� �ݺ��Ѵ�.
        while (i < rowContentLength && isMatched == false)
        {
            //8.7.1 �ٿ��� i��° ���� ���ڰ� �ѱ��̸�
            if ((realRowContent[i] & 0x80))//�ѱ��̸�(2byte����)
            {
                //8.7.1.1 2byte�� �����Ѵ�.
                letterContentOfRow = realRowContent[i];
                i++;
                letterContentOfRow += realRowContent[i];
            }
            //8.7.2 �ѱ��� �ƴϸ�
            else
            {
                //8.7.2.1 1byte�� �����Ѵ�
                letterContentOfRow = realRowContent[i];
            }
            //8.7.3 ã�� ���ڿ����� j��° ���� ���ڰ� �ѱ��̸�
            if ((keyword[j] & 0x80))//�ѱ��̸�(2byte����)
            {
                //8.7.3.1 2byte�� �����Ѵ�.
                partContentOfKeyword = keyword[j];
                j++;
                partContentOfKeyword += keyword[j];
                //8.7.3.2 ���̸� 2�� �����Ѵ�(�ѱ��� ���̸� 2 �����ϱ� ������)
                contentLength = 2;
            }
            //8.7.4 �ѱ��� �ƴϸ�
            else
            {
                //8.7.4.1 1byte�� �����Ѵ�
                partContentOfKeyword = keyword[j];
                //8.7.4.2 ���̸� 1�� �����Ѵ�.(�ѱ��� ������ ���� �� Ư�����ڴ� ���̰� 1�̱� ������)
                contentLength = 1;
            }
            //8.7.5 �ٿ��� ���� ���ڿ� ã�� ���ڿ����� ���� ���ڰ� �ѱ��� �ƴϸ�
            if (letterContentOfRow.length() < 3 && partContentOfKeyword.length() < 3)
            {
                //8.7.5.1 ��/�ҹ��� ���о��� �����ش�.
                if (_stricmp(letterContentOfRow.c_str(), partContentOfKeyword.c_str()) == 0)
                {
                    //8.7.5.1.1 ��ġ�ϴ� ���ڿ��� ���̸� ���ش�.
                    matchedLength += contentLength;
                    //8.7.5.1.2 ��ġ�ϴ� ���ڿ��� ���� ������ ���ش�.
                    matchedLettersCount++;
                    //8.7.5.1.3 ã�� ���ڿ����� ���� ������ġ�� �������� �̵���Ų��.
                    j++;
                }
                //8.7.5.2 �ٿ��� ���� ���ڿ� ã�� ���ڿ����� ���� ���ڰ� ���� �ٸ���
                else
                {
                    //8.7.5.2.1 ��ġ�ϴ� ���ڿ��� ���̸� 0���� �ʱ�ȭ���ش�.
                    matchedLength = 0;
                    //8.7.5.2.2 ��ġ�ϴ� ���ڿ��� ���� ������ 0���� �ʱ�ȭ���ش�.
                    matchedLettersCount = 0;
                    //8.7.5.2.3 ã�� ���ڿ����� ���� ������ġ�� ó������ �̵���Ų��.
                    j = 0;
                }

            }
            //8.7.6 �ٿ��� ���� ���ڿ� ã�� ���ڿ����� ���� ���ڰ� �ѱ��̸�
            else
            {
                //8.7.6.1 �ٿ��� ���� ���ڿ� ã�� ���ڿ����� ���� ���ڰ� ���� ��ġ�ϸ�
                if (letterContentOfRow == partContentOfKeyword)
                {
                    //8.7.6.1.1 ��ġ�ϴ� ���ڿ��� ���̸� ���ش�.
                    matchedLength += contentLength;
                    //8.7.6.1.2 ��ġ�ϴ� ���ڿ��� ���� ������ ���ش�.
                    matchedLettersCount++;
                    //8.7.6.1.3 ã�� ���ڿ����� ���� ������ġ�� �������� �̵���Ų��.
                    j++;
                }
                //8.7.6.2 �ٿ��� ���� ���ڿ� ã�� ���ڿ����� ���� ���ڰ� ���� �ٸ���
                else
                {
                    //8.7.6.2.1 ��ġ�ϴ� ���ڿ��� ���̸� 0���� �ʱ�ȭ���ش�.
                    matchedLength = 0;
                    //8.7.6.2.2 ��ġ�ϴ� ���ڿ��� ���� ������ 0���� �ʱ�ȭ���ش�.
                    matchedLettersCount = 0;
                    //8.7.6.2.3 ã�� ���ڿ����� ���� ������ġ�� ó������ �̵���Ų��.
                    j = 0;
                }
            }
            //8.7.7 �޸����� ���� �ٿ��� ���� ��ġ�� ���ڸ� �о��� ������ �񱳸� �߱� ������
            //���� ������ ��ġ�� �������� �̵����Ѿ� ���� ���ڸ� �а� ã�� ���ڿ��� ���ڿ� ���� �� �ִ�.
            currentLetterIndex++;
            //8.7.8 ã�� ���ڿ��� row�� content���� ã������
            //��ġ�ϴ� ���ڿ��� ���̿� ã�� ���ڿ��� ���̰� ��ġ�ϸ�
            if (matchedLength == keywordLength)
            {
                //8.7.8.1 �ش� ���ڿ��� �ٿ��� ã�Ҵٰ� ǥ���Ѵ�.
                isMatched = true;
                //8.7.8.2 ���� ���� ���Ѵ�.
                currentRow = this->note->GetAt(currentRowIndex);
                //8.7.8.3 ���� ���� content�� ���Ѵ�.
                currentRowContent = currentRow->GetContent();
                //8.7.8.4 ���� ���� content�� length�� ���Ѵ�.
                currentRowContentLength = currentRowContent.length();
                //8.7.8.5 i�� ���� ���� content�� length���� ũ�ų� �������� �ݺ��Ѵ�.
                //�Ʊ� �տ��� �κ��ڵ������� ����ϴ� ��ó�� ��¥���� content���� ��¥���� content��
                //�߰��������� ��ǻ� ���� �޸��忡���� �ڵ������� �Ǿ� �ִ� �����̱� ������
                //�ڵ������� �� �޸��忡�� ã�� ���ڿ��� ã�Ƽ� ������ �ؾ��ϱ� ������ �Ʊ� ���ƴ� ������ 
                //��¥�ٵ��� �ٽ� ���������� ���ؼ� ��ģ �ٿ��� ���� ������ġ i�� ���ؼ� ���� ���� ��ġ��
                //�ٽ� �����ش�.
                while (i >= currentRowContentLength)
                {
                    //8.7.8.5.1 ���� ���� ��ġ�� ������Ų��.
                    //�ڵ������� ��ҵ� ���¿��� �ش� ���ڿ��� ã�Ҵµ� ���������� ���� ���ڰ�
                    //���� �������� ���� ���̺��� ��� ������ ���� �ٷ� �Ѿ�� �ٽ� ���ؾ��Ѵ�.
                    currentRowIndex++;
                    //8.7.8.5.2 ���� ������ġ�� �ڵ������� ��ҵ� ������ ������ġ�� ������
                    //���� ������ġ�� ���� ���� �������� ũ�� ������ �׸�ŭ ���� �ڵ������� �� ���·� �����Ѵ�
                    //�տ��� currentLetterIndex++�� ���� 1�� ������ �����̱� ������ +1�� ���� �����൵��!
                    currentLetterIndex -= currentRow->GetLength();
                    //8.7.8.5.3 ���� ���� �������� ���Ѵ�.
                    currentRow = this->note->GetAt(currentRowIndex);
                    //8.7.8.5.4 ���� ���� content�� �����Ѵ�.
                    nextRowContent = currentRow->GetContent();
                    //8.7.8.5.5 ���� ���� content�� ���� ���� content�� �����ش�.(����)
                    currentRowContent += nextRowContent;
                    //8.7.8.5.6 ������ ���� content�� ���̸� ���Ѵ�.
                    currentRowContentLength = currentRowContent.length();
                }
                //8.7.8.6 ã�� ���� �� ��ġ�� ã�� ���� ����ġ�� ����Ѵ�.
                *findingEndRowIndex = currentRowIndex;
                *findingEndLetterIndex = currentLetterIndex;
                //8.7.8.7 ���� ���� ��ġ���� ã�� ���ڿ��� ���ڰ����� ���ش�.
                startLetterIndex = currentLetterIndex - matchedLettersCount;
                //8.7.8.8 ����̸�
                if (startLetterIndex >= 0)
                {
                    //8.7.8.8.1 ã�� ���� ������ġ�� ã�� ���� �� ��ġ�� ������ ������ ����Ѵ�.
                    *findingStartRowIndex = currentRowIndex;
                    //8.7.8.8.2 ã�� ������ ������ġ�� ������ �����ؼ� ����Ѵ�.
                    *findingStartLetterIndex = startLetterIndex;
                }
                //8.7.8.9 �����̸�
                else
                {
                    //8.7.8.9.1 ���� ���� ���� ���� ���Ѵ�.
                    previousRowIndex = currentRowIndex - 1;
                    row = this->note->GetAt(previousRowIndex);
                    //8.7.8.9.2 ���� �ٿ��� ���� �ٷ� �̵��߱� ������ 
                    //ã�� ���ڿ��� ���ڰ������� ���� ���� ������ġ�� ���ش�
                    matchedLettersCount -= currentLetterIndex;
                    //8.7.8.9.3 �� ���ڿ��� ���ڰ����� ���� ���� ���ڰ������� ū ���� �ݺ��Ѵ�.
                    while (matchedLettersCount > row->GetLength())
                    {
                        //8.7.8.9.3.1 ã�� ���ڿ��� ���ڰ������� ���� ���� ���ڰ����� ���ش�.
                        matchedLettersCount -= row->GetLength();
                        //8.7.8.9.3.2 ���� ���� ���Ѵ�.
                        previousRowIndex--;
                        row = this->note->GetAt(previousRowIndex);
                    }
                    //8.7.8.9.4 ã�� ���� ������ġ�� ���� ���� ��ġ�� ����Ѵ�.
                    *findingStartRowIndex = previousRowIndex;
                    //8.7.8.9.5 ã�� ������ ������ġ�� ���� ���� �������� ã�� ���ڿ��� ������ 
                    //�� ���� ����Ѵ�.
                    *findingStartLetterIndex = row->GetLength() - matchedLettersCount;
                }
            }
            //8.7.9 ���� content���� ���� ������ �д´�.
            i++;
        }
        //8.8 ã�� ���ڿ��� row�� content���� ��ã������
        if (isMatched == false)
        {
            //8.8.1 ���� ���� ��ġ�� ��¥�� ������ ��¥�ٱ����� �ٽ� �������Ѵ�.
            currentRowIndex = nextRowIndex - 1;
        }
        //8.9 ���� �ٷ� �̵��Ѵ�.
        currentRowIndex++;
        //8.10 ���� �ٷ� �̵��߱� ������ currentLettersLength�� currentLetterIndex�� 0���� �ʱ�ȭ���ش�.
        currentLettersLength = 0;
        currentLetterIndex = 0;
        //���⼭ �ʱ�ȭ�� �����ָ� ������ �Ǳ� ������ ����� �� Ž���� �ȵ� ���� �ٲ��(��¥ ���� �ٲ�Ƿ�)
        //�� �ٿ��� ��ġ�ϴ� ���ڿ��� ã�� ���ߴٴ� ������ ���� �ٿ��� �ٽ� ��ġ�ϴ� ���ڿ��� ã�ڴٴ� �ǹ�!
        //�׷��� ������ �����ٿ����� �ٽ� ó������ ã�ƾ��ϱ� ������ ��ġ�ϴ� ���ڿ��� ���õ� ���׵��� ���
        //0���� �ʱ�ȭ ���Ѽ� �ٽ� �������ش�. �׷��� �ؾ߸� ��¥�� �����ٿ� ���ļ� Ž���Ǵ� ��찡 ����
        //��¥�ٿ����� ���ٳ������� Ž���� �Ǿ��ϱ� ������ ���⼭ ��¥ ���� �ٲ�� ������
        //ã�� ���ڿ��� ���õ� ���׵��� ��� 0���� �ʱ�ȭ���ְ�, ��¥���� ��¥ ���� �ٲ������ ���� ó���Ǳ�
        //������ ��¥���� ��� �����ٿ� ���ļ� Ž���� �����ϴ�.
        //8.11 ��ġ�ϴ� ���ڿ��� ���̸� 0���� �ʱ�ȭ���ش�.
        matchedLength = 0;
        //8.12 ��ġ�ϴ� ���ڿ��� ���� ������ 0���� �ʱ�ȭ���ش�.
        matchedLettersCount = 0;
        //8.13 ã�� ���ڿ����� ���� ������ġ�� ó������ �̵���Ų��.
        j = 0;
    }
    //9. ��ã������
    if (isMatched == false)
    {
        //4.1 ���� ĳ���� �ִ� ���� �״�� �ִ´�.
        *findingEndRowIndex = this->note->GetCurrent();
        *findingEndLetterIndex = this->note->GetAt(*findingEndRowIndex)->GetCurrent();
        *findingStartRowIndex = this->note->GetCurrent();
        *findingStartLetterIndex = this->note->GetAt(*findingStartRowIndex)->GetCurrent();
    }
    //10. ������.
}

//��/�ҹ��� ���о��� ���� ã��
void GlyphFinder::FindUpWithMatchCase(string keyword, Long* findingStartRowIndex,
    Long* findingStartLetterIndex, Long* findingEndRowIndex, Long* findingEndLetterIndex)
{
    //1. ã�� ���ڿ��� �Է¹޴´�.
   //2. �޸��忡�� ���� ���� ��ġ�� ���Ѵ�.
    Long currentRowIndex = this->note->GetCurrent();
    //3. �޸��忡�� ���� ������ ��ġ�� ���Ѵ�.
    Long currentLetterIndex = this->note->GetAt(currentRowIndex)->GetCurrent();
    //4. �޸����� ���� �ٿ��� ���� ���ڱ����� content�� ���Ѵ�.
    string currentLetters = this->note->GetAt(currentRowIndex)
        ->GetPartOfContent(currentLetterIndex);
    //5. �޸����� ���� �ٿ��� ���� ���ڱ����� content�� ���̸� ���Ѵ�.
    Long currentLettersLength = currentLetters.length();
    //6. ã�� ���ڿ��� ã�Ҵ��� ���θ� �ϴ� ��ã�ұ� ������ false�� �ʱ�ȭ�Ѵ�.
    bool isMatched = false;
    //7. ����� �������� �ݺ��� �ۿ��� �̸� ���� �� �ʱ�ȭ��Ų��.
    Long matchedLettersCount = 0;//�ٿ��� ã�� ���ڿ� ã�� ���ڿ��� ���ڰ� ���� ��ġ�ϴ� ���ڰ���
    Long keywordLength = keyword.length();// ã�� ���ڿ��� ����(�ѱ��� 2, ���� �� Ư�����ڴ� 1)
    Long matchedLength = 0;// ã�� ���ڿ��� ���� ���ڵ��� ���� ��ġ�ϴ� ���ڿ��� ����
    Glyph* row = 0;
    Long realRowIndex = 0;
    Glyph* previousRow = 0;
    string letterContentOfRow;//���� �� ���ڸ� ���� ����
    string partContentOfKeyword;//ã�� ���ڿ��� �� ���ڸ� ���� ����
    Long previousRowIndex = -1;
    string rowContent;//���� content�� ���� ����
    string previousRowContent;//���� ���� content�� ���� ����
    Long contentLength = 0;//ã�� ���ڿ��� �ѱ����� ����
    Glyph* currentRow = 0;
    string currentRowContent;
    Long currentRowContentLength = 0;
    Long previousRowContentLength = 0;
    Long sum = 0;
    Glyph* nextRow = 0;
    Long nextRowIndex = 0;
    string koreanCompositionTemp;//�������� �Ž��� �ö󰡱� ������ �������� �ѱ��� �����ϸ� �ѱ��� �����
    //�������� �ʱ� ������ �ѱ��� �ӽ÷� �����ϰ� ���� ������� ������ �ӽ� ����Ұ� �ʿ���.
    Long i = 0;//���� content���� ���ڸ� ���� �迭��� �� �ݺ�������
    Long j = keywordLength - 1;//ã�� ���ڿ��� content���� ���ڸ� ���� �迭��� �� �ݺ������
    //8. ���� ���� ��ġ�� 0���� ũ�ų� ���� ���� �׸��� �ش繮�ڿ��� ã�� ������ �ݺ��Ѵ�.
    while (currentRowIndex >= 0 && isMatched == false)
    {
        //8.1 ���� ���� ���Ѵ�.
        row = this->note->GetAt(currentRowIndex);
        //8.2 ���� ���� content�� ���Ѵ�.
        rowContent = row->GetPartOfContent(currentLetterIndex);
        //8.2 ���� ���� ��¥ ���̸�(���� ���� ��ġ�� 0�̸� ��¥ ���� �� X)
        if (dynamic_cast<DummyRow*>(row))
        {

            //8.2.2 ���� �ٷ� �̵��Ѵ�.
            previousRowIndex = currentRowIndex - 1;
            //8.2.3 ���� ���� ���Ѵ�
            previousRow = this->note->GetAt(previousRowIndex);
            //8.2.4 ���� ���� ��¥���ε��� �ݺ��Ѵ�.
            while (dynamic_cast<DummyRow*>(previousRow))
            {
                //8.2.4.1 ���� ���� content�� ���Ѵ�.
                previousRowContent = previousRow->GetContent();
                //8.2.4.2 ���� ���� content�� ���� ���� cotent�� �����ش�.(����)
                previousRowContent += rowContent;
                //8.2.4.3 ������ ���� �Ű� �����Ѵ�.(�ݺ� �������� ����� ������Ű�� ���ؼ�)
                rowContent = previousRowContent;
                //8.2.4.4 �޸����� ���� ������ġ�� ���� ���� ���ڰ����� �����ش�.
                currentLetterIndex += previousRow->GetLength();
                //8.2.4.5 �������� �̵��Ѵ�.
                previousRowIndex--;
                //8.2.4.6 ���� ���� ���Ѵ�.
                previousRow = this->note->GetAt(previousRowIndex);
            }
            //�������� �̵����� �� ��¥ ���� ���ͼ� �ݺ������� ���Ա� ������ ��¥ �ٿ� �ݺ���������
            //��ģ content�� �����ش�.
            //8.2.5 �ڵ������� �κ�����ϰ� ��¥ ���� ��ġ�� ã�Ƽ�
            //��¥ ���� ��ġ�� ���� ���� ��ġ�� �����Ѵ�.
            currentRowIndex = previousRowIndex;
            //8.2.6 ��¥ ���� content�� ���Ѵ�.
            previousRowContent = previousRow->GetContent();
            //8.2.7 ��¥ ���� content�� ������Ų��.
            previousRowContent += rowContent;
            //8.2.8 ������ ���� �Ű� �����Ѵ�.(�ݺ� �������� ����� ������Ű�� ���ؼ�)
            rowContent = previousRowContent;
            //8.2.9 �޸����� ���� ������ġ�� ���� ���� ���ڰ����� �����ش�.
            currentLetterIndex += previousRow->GetLength();
            //8.2.10 ���� ���� ���̸� ���Ѵ�.(�ڵ����� ��ҵ� ������ ������ġ���� ����)
            currentLettersLength = rowContent.length();
        }

        realRowIndex = currentRowIndex;

        //8.3 �ٿ��� ���� ���� ��ġ�� �ʱ�ȭ��Ų��.
        i = currentLettersLength - 1;
        //8.4 ���� ���� ��ġ�� 0���� ũ�ų� ���� ���� �׸��� �ش繮�ڿ��� ã�������� �ݺ��Ѵ�.
        while (i >= 0 && isMatched == false)
        {
            //8.4.1 �ٿ��� i��° ���� ���ڰ� �ѱ��̸�
            if ((rowContent[i] & 0x80))//�ѱ��̸�(2byte����)
            {
                //8.4.1.1 2byte�� �����Ѵ�.
                letterContentOfRow = rowContent[i];
                //�������� �Ž��� ���� ������ ����� �ѱ��� �����ϱ� ���ؼ� �ӽ������� �ؾ���.
                koreanCompositionTemp = letterContentOfRow;
                i--;
                letterContentOfRow = rowContent[i];
                letterContentOfRow += koreanCompositionTemp;
            }
            //8.4.2 �ѱ��� �ƴϸ�
            else
            {
                //8.4.2.1 1byte�� �����Ѵ�
                letterContentOfRow = rowContent[i];
            }
            //8.4.3 ã�� ���ڿ����� j��° ���� ���ڰ� �ѱ��̸�
            if ((keyword[j] & 0x80))//�ѱ��̸�(2byte����)
            {
                //8.4.3.1 2byte�� �����Ѵ�.
                partContentOfKeyword = keyword[j];
                //�������� �Ž��� ���� ������ ����� �ѱ��� �����ϱ� ���ؼ� �ӽ������� �ؾ���.
                koreanCompositionTemp = partContentOfKeyword;
                j--;
                partContentOfKeyword = keyword[j];
                partContentOfKeyword += koreanCompositionTemp;
                //8.4.3.2 ���̸� 2�� �����Ѵ�(�ѱ��� ���̸� 2 �����ϱ� ������)
                contentLength = 2;
            }
            //8.4.4 �ѱ��� �ƴϸ�
            else
            {
                //8.4.4.1 1byte�� �����Ѵ�
                partContentOfKeyword = keyword[j];
                //8.4.4.2 ���̸� 1�� �����Ѵ�.(�ѱ��� ������ ���� �� Ư�����ڴ� ���̰� 1�̱� ������)
                contentLength = 1;
            }
            //8.4.5 �ٿ��� ���� ���ڿ� ã�� ���ڿ����� ���� ���ڰ� �ѱ��� �ƴϸ�
            if (letterContentOfRow.length() < 3 && partContentOfKeyword.length() < 3)
            {
                //8.4.5.1 ��/�ҹ��� ���о��� �����ش�.
                if (_stricmp(letterContentOfRow.c_str(), partContentOfKeyword.c_str()) == 0)
                {
                    //8.4.5.1.1 ��ġ�ϴ� ���ڿ��� ���̸� ���ش�.
                    matchedLength += contentLength;
                    //8.4.5.1.2 ��ġ�ϴ� ���ڿ��� ���� ������ ���ش�.
                    matchedLettersCount++;
                    //8.4.5.1.3 ã�� ���ڿ����� ���� ������ġ�� �������� �̵���Ų��.
                    j--;
                }
                //8.4.5.2 �ٿ��� ���� ���ڿ� ã�� ���ڿ����� ���� ���ڰ� ���� �ٸ���
                else
                {
                    //8.4.5.1 ��ġ�ϴ� ���ڿ��� ���̸� 0���� �ʱ�ȭ���ش�.
                    matchedLength = 0;
                    //8.4.5.2 ��ġ�ϴ� ���ڿ��� ���� ������ 0���� �ʱ�ȭ���ش�.
                    matchedLettersCount = 0;
                    //8.4.5.3 ã�� ���ڿ����� ���� ������ġ�� ���������� �̵���Ų��.
                    j = keywordLength - 1;
                }

            }
            //8.4.6 �ٿ��� ���� ���ڿ� ã�� ���ڿ����� ���� ���ڰ� �ѱ��̸�
            else
            {
                //8.4.6.1 �ٿ��� ���� ���ڿ� ã�� ���ڿ����� ���� ���ڰ� ���� ��ġ�ϸ�
                if (letterContentOfRow == partContentOfKeyword)
                {
                    //8.4.6.1.1 ��ġ�ϴ� ���ڿ��� ���̸� ���ش�.
                    matchedLength += contentLength;
                    //8.4.6.1.2 ��ġ�ϴ� ���ڿ��� ���� ������ ���ش�.
                    matchedLettersCount++;
                    //8.4.6.1.3 ã�� ���ڿ����� ���� ������ġ�� �������� �̵���Ų��.
                    j--;
                }
                //8.4.6.2 �ٿ��� ���� ���ڿ� ã�� ���ڿ����� ���� ���ڰ� ���� �ٸ���
                else
                {
                    //8.4.6.2.1 ��ġ�ϴ� ���ڿ��� ���̸� 0���� �ʱ�ȭ���ش�.
                    matchedLength = 0;
                    //8.4.6.2.2 ��ġ�ϴ� ���ڿ��� ���� ������ 0���� �ʱ�ȭ���ش�.
                    matchedLettersCount = 0;
                    //8.4.6.2.3 ã�� ���ڿ����� ���� ������ġ�� ���������� �̵���Ų��.
                    j = keywordLength - 1;
                }
            }
            //8.4.7 �޸����� ���� �ٿ��� ���� ��ġ�� ���ڸ� �о��� ������ �񱳸� �߱� ������
            //���� ������ ��ġ�� �������� �̵����Ѿ� ���� ���ڸ� �а� ã�� ���ڿ��� ���� ���ڿ� ���� �� �ִ�.
            currentLetterIndex--;
            //8.4.8 ã�� ���ڿ��� row�� content���� ã������
           //��ġ�ϴ� ���ڿ��� ���̿� ã�� ���ڿ��� ���̰� ��ġ�ϸ�
            if (matchedLength == keywordLength)
            {
                //8.4.8.1 �ش� ���ڿ��� �ٿ��� ã�Ҵٰ� ǥ���Ѵ�.
                isMatched = true;
                //8.4.8.2 �޸��忡�� ���� ���� ���Ѵ�.(��¥ ��)
                currentRow = this->note->GetAt(currentRowIndex);
                //8.4.8.3 ���� ���� content�� ���Ѵ�.
                currentRowContent = currentRow->GetContent();
                //8.4.8.4 ���� ���� length�� ���Ѵ�.
                currentRowContentLength = currentRowContent.length();
                //8.4.8.5 i�� ���� ���� length���� ũ�ų� �������� �ݺ��Ѵ�.
                while (i >= currentRowContentLength)
                {
                    //8.4.8.5.1 ���� ���� ���̸� ����.
                    i -= currentRowContentLength;
                    //8.4.8.5.2 ���� ���� ������ ����.
                    currentLetterIndex -= currentRow->GetLength();
                    //8.4.8.5.3 ���� �ٷ� �̵��Ѵ�.
                    currentRowIndex++;
                    //8.4.8.5.4 ���� ���� ���Ѵ�.
                    currentRow = this->note->GetAt(currentRowIndex);
                    //8.4.8.5.5 ���� ���� content�� ���Ѵ�.
                    currentRowContent = currentRow->GetContent();
                    //8.4.8.5.6 ���� ���� content�� length�� ���Ѵ�.
                    currentRowContentLength = currentRowContent.length();
                }
                //8.6.8.6 ã�� ���� ���� ��ġ�� ã�� ���� ���� ��ġ�� ����Ѵ�.
                *findingStartRowIndex = currentRowIndex;
                *findingStartLetterIndex = currentLetterIndex;
                //8.6.8.6 ã�� ������ ������ġ���� ��ġ�ϴ� ���ڿ��� ���ڰ����� �����ش�.
                sum = currentLetterIndex + matchedLettersCount;
                //8.6.8.7 ���Ѱ��� ã�� ���� ������ġ�� �ش��ϴ� ���� ���ڰ������� �۰ų� ������
                if (sum <= this->note->GetAt(currentRowIndex)->GetLength())
                {
                    //8.6.8.7.1 ã�� ���� ����ġ�� ã�� ���� ���� ��ġ�� ������ ������ ����Ѵ�.
                    *findingEndRowIndex = currentRowIndex;
                    //8.6.8.7.2 ã�� ������ ����ġ�� ���Ѱ����� ����Ѵ�.
                    *findingEndLetterIndex = sum;
                }
                //8.6.8.8 ���� ���� ���� ���ڰ������� ũ��
                else
                {
                    //8.6.8.8.1 ���� ���� ���Ѵ�.
                    currentRow = this->note->GetAt(currentRowIndex);
                    //8.6.8.8.2 ���� ���� ���� ���� ���Ѵ�.
                    nextRowIndex = currentRowIndex + 1;
                    nextRow = this->note->GetAt(nextRowIndex);
                    //8.6.8.8.3 ���� �ٿ��� ���� �ٷ� �̵��߱� ������ ���� ������ ���� ���� ������ŭ ���ش�.
                    sum -= currentRow->GetLength();
                    //8.6.8.8.4 ���� ���� ���� ���� ���ڰ������� ū ���� �ݺ��Ѵ�.
                    while (sum > nextRow->GetLength())
                    {
                        //8.6.8.8.4.1 ���� ������ ���� ���� ���ڰ����� ���ش�.
                        sum -= nextRow->GetLength();
                        //8.6.8.8.4.2 ���� ���� ���Ѵ�.
                        nextRowIndex--;
                        nextRow = this->note->GetAt(nextRowIndex);
                    }
                    //8.6.8.8.5 ã�� ���� ����ġ�� ���� �ٷ� ���Ѵ�.
                    *findingEndRowIndex = nextRowIndex;
                    //8.6.8.8.6 ã�� ������ ����ġ�� ���� ������ ���Ѵ�.
                    *findingEndLetterIndex = sum;
                }
            }
            //8.6.8.9 ���� content���� ���� ���ڷ� �̵��Ѵ�.
            i--;
        }
        //8.8 ã�� ���ڿ��� row�� content���� ��ã������
        if (isMatched == false)
        {
            //8.8.1 ���� ���� ��ġ�� ��¥�� ������ ��¥�ٱ����� �ٽ� �������Ѵ�.
            currentRowIndex = realRowIndex;
        }
        //8.8 ���� �ٷ� �̵��Ѵ�.
        currentRowIndex--;
        if (currentRowIndex >= 0)
        {
            //8.9 ���� ���� ���Ѵ�.
            previousRow = this->note->GetAt(currentRowIndex);
            //8.10 ���� �ٷ� �̵��߱� ������ �ٿ��� ���� ������ġ�� ���� ���� content ���̷� �����Ѵ�.
            previousRowContent = previousRow->GetContent();
            previousRowContentLength = previousRowContent.length();
            currentLettersLength = previousRowContentLength;
            //8.11 ���� ���� ��ġ�� ���� ���� ���� ������ �����Ѵ�.
            currentLetterIndex = previousRow->GetLength();
        }
        //8.11 ��ġ�ϴ� ���ڿ��� ���̸� 0���� �ʱ�ȭ���ش�.
        matchedLength = 0;
        //8.12 ��ġ�ϴ� ���ڿ��� ���� ������ 0���� �ʱ�ȭ���ش�.
        matchedLettersCount = 0;
        //8.13 ã�� ���ڿ����� ���� ������ġ�� ���������� �̵���Ų��.
        j = keywordLength - 1;
        //���⼭ �ʱ�ȭ�� �����ָ� ������ �Ǳ� ������ ����� �� Ž���� �ȵ� ���� �ٲ��(��¥ ���� �ٲ�Ƿ�)
        //�� �ٿ��� ��ġ�ϴ� ���ڿ��� ã�� ���ߴٴ� ������ ���� �ٿ��� �ٽ� ��ġ�ϴ� ���ڿ��� ã�ڴٴ� �ǹ�!
        //�׷��� ������ �����ٿ����� �ٽ� ó������ ã�ƾ��ϱ� ������ ��ġ�ϴ� ���ڿ��� ���õ� ���׵��� ���
        //0���� �ʱ�ȭ ���Ѽ� �ٽ� �������ش�. �׷��� �ؾ߸� ��¥�� �����ٿ� ���ļ� Ž���Ǵ� ��찡 ����
        //��¥�ٿ����� ���ٳ������� Ž���� �Ǿ��ϱ� ������ ���⼭ ��¥ ���� �ٲ�� ������
        //ã�� ���ڿ��� ���õ� ���׵��� ��� 0���� �ʱ�ȭ���ְ�, ��¥���� ��¥ ���� �ٲ������ ���� ó���Ǳ�
        //������ ��¥���� ��� �����ٿ� ���ļ� Ž���� �����ϴ�.
    }
    //9. ��ã������
    if (isMatched == false)
    {
        //4.1 ���� ĳ���� �ִ� ���� �״�� �ִ´�.
        *findingEndRowIndex = this->note->GetCurrent();
        *findingEndLetterIndex = this->note->GetAt(*findingEndRowIndex)->GetCurrent();
        *findingStartRowIndex = this->note->GetCurrent();
        *findingStartLetterIndex = this->note->GetAt(*findingStartRowIndex)->GetCurrent();
    }
    //10. ������.
}

//�Ҹ���
GlyphFinder::~GlyphFinder()
{

}