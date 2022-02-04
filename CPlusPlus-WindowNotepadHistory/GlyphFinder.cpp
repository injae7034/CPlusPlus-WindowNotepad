#include "GlyphFinder.h"
#include "Glyph.h"
#include "DummyRow.h"

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
        nextRow = this->note->GetAt(currentRowIndex + 1);
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
                //8.7.8.5 i�� ���� ���� content�� length���� ū���� �ݺ��Ѵ�.
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
    //8. ���� ���� ��ġ�� 0���� ũ�ų� �������� �׸��� �ش繮�ڿ��� ã�� ������ �ݺ��Ѵ�.
    while (currentRowIndex >= 0 && isMatched == false)
    {
        //8.1 ���� ���Ѵ�.
        row = this->note->GetAt(currentRowIndex);
        //8.2 ���� content�� ���Ѵ�.
        realRowContent = row->GetContent();
        //8.3 ���� ���� ���Ѵ�.
        nextRowIndex = currentRowIndex + 1;
        nextRow = this->note->GetAt(currentRowIndex + 1);
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
    }
}

//�Ҹ���
GlyphFinder::~GlyphFinder()
{

}