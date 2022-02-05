#include "CtrlLeftArrowKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"
#include "DummyRow.h"

//����Ʈ������
CtrlLeftArrowKeyAction::CtrlLeftArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void CtrlLeftArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. ������ ����ǰ� �ִ� ���̾�����
	if (this->notepadForm->isSelecting == true)
	{
		//1.1. ���õ� �ؽ�Ʈ�� ���������Ѵ�.(������ ������.)
		this->notepadForm->selectingTexts->Undo();
		//1.2 ������ ���� ���·� �ٲ۴�.
		this->notepadForm->isSelecting = false;
		//1.3 ������ ������ ������ ĳ���� x��ǥ�� 0���� �����Ѵ�.
		this->notepadForm->selectedStartXPos = 0;
		//1.4 ������ ������ ������ ĳ���� y��ǥ�� 0���� �����Ѵ�.
		this->notepadForm->selectedStartYPos = 0;
	}
	//2. �޸��忡�� ���� ���� ��ġ�� ���Ѵ�.
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	//3. �޸��忡�� ���� ������ ��ġ�� ���Ѵ�.
	Long currentLetterIndex = this->notepadForm->note->GetAt(currentRowIndex)->GetCurrent();
	//4. �ڵ������� ���� ���� �ƴϸ�
	if (this->notepadForm->isRowAutoChanging == false)
	{
		//4.1 �̵��ϱ� ���� ���� ��ġ�� ���Ѵ�.
		Long previousRowIndex = currentRowIndex;
		//4.2 �̵��ϱ� ���� ������ ��ġ�� ���Ѵ�.
		Long previousLetterIndex = currentLetterIndex;
		//4.3 ��Ʈ���� �ܾ������ �������� �̵��Ѵ�.
		Long currentLetterIndex = this->notepadForm->note->PreviousWord();
		//4.4 �̵��� �Ŀ� ���� ��ġ�� ���Ѵ�.
		Long currentRowIndex = this->notepadForm->note->GetCurrent();
		//4.5 ���� ���� �̵��� ���� �ٷ� �����Ѵ�.
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
	}
	//5. �ڵ������� ���� ���̸�
	else
	{
		//5.1 �޸����� ���� �ٿ��� ó�� ���ں��� ���� ���ڱ����� content�� ���Ѵ�.
		Long i = 0;
		Glyph* letter = 0;
		string letterContent = "";
		string currentLetters = "";
		while (i < currentLetterIndex)
		{
			//5.1.1 ���ڸ� ���Ѵ�.
			letter = this->notepadForm->current->GetAt(i);
			letterContent = letter->GetContent();
			//5.1.2 ���� content�� �����ش�.
			currentLetters += letterContent;
			i++;
		}
        //5.2 �޸����� ���� �ٿ��� ó�����ں��� ���� ���ڱ����� content�� ���̸� ���Ѵ�.
        Long currentLettersLength = currentLetters.length();
        //5.3 ���� ���� ���Ѵ�.
        Glyph* row = this->notepadForm->note->GetAt(currentRowIndex);
        //5.4 ���� content�� ���Ѵ�.
        i = 0;
        string rowContent = "";
        while (i < currentLetterIndex)
        {
            //5.4.1 ���ڸ� ���Ѵ�.
            letter = row->GetAt(i);
            letterContent = letter->GetContent();
            //5.4.2 ���� content�� �����ش�.
            rowContent += letterContent;
            i++;
        }
        //5.5 ���� ���� ��¥ ���̸�(���� ���� ��ġ�� 0�̸� ��¥ ���� �� X)
        if (dynamic_cast<DummyRow*>(row))
        {
            //5.5.1 ���� �ٷ� �̵��Ѵ�.
            Long previousRowIndex = currentRowIndex - 1;
            //5.5.2 ���� ���� ���Ѵ�
            Glyph *previousRow = this->notepadForm->note->GetAt(previousRowIndex);
            //5.5.3 ���� ���� ��¥���ε��� �ݺ��Ѵ�.
            string previousRowContent = "";
            while (dynamic_cast<DummyRow*>(previousRow))
            {
                //5.5.3.1 ���� ���� content�� ���Ѵ�.
                i = 0;
                previousRowContent = "";
                while (i < previousRow->GetLength())
                {
                    //5.5.3.1.1 ���ڸ� ���Ѵ�.
                    letter = previousRow->GetAt(i);
                    letterContent = letter->GetContent();
                    //5.5.3.1.2 ���� content�� �����ش�.
                    previousRowContent += letterContent;
                    i++;
                }
                //5.5.3.2 ���� ���� content�� ���� ���� cotent�� �����ش�.(����)
                previousRowContent += rowContent;
                //5.5.3.3 ������ ���� �Ű� �����Ѵ�.(�ݺ� �������� ����� ������Ű�� ���ؼ�)
                rowContent = previousRowContent;
                //5.5.3.4 �޸����� ���� ������ġ�� ���� ���� ���ڰ����� �����ش�.
                currentLetterIndex += previousRow->GetLength();
                //5.5.3.5 �������� �̵��Ѵ�.
                previousRowIndex--;
                //5.5.3.6 ���� ���� ���Ѵ�.
                previousRow = this->notepadForm->note->GetAt(previousRowIndex);
            }
            //�������� �̵����� �� ��¥ ���� ���ͼ� �ݺ������� ���Ա� ������ ���� ���� ���� ��¥ ���̴�.
            //���� ���� ��(��¥ ��)�� content�� ���ؼ� �Ʊ� �ݺ��������� ���ߴ� ��¥���� content�� �����ش�
            //5.5.4 ��¥ ���� ��ġ�� ���� ���� ��ġ�� �����Ѵ�.
            currentRowIndex = previousRowIndex;
            //5.5.5 ��¥ ���� content�� ���Ѵ�.
            i = 0;
            previousRowContent = "";
            while (i < previousRow->GetLength())
            {
                //5.5.5.1 ���ڸ� ���Ѵ�.
                letter = previousRow->GetAt(i);
                letterContent = letter->GetContent();
                //5.5.5.2 ���� content�� �����ش�.
                previousRowContent += letterContent;
                i++;
            }
            //5.5.6 ��¥ ���� content�� ������Ų��.
            previousRowContent += rowContent;
            //5.5.7 ������ ���� �Ű� �����Ѵ�.(�ݺ� �������� ����� ������Ű�� ���ؼ�)
            rowContent = previousRowContent;
            //5.5.8 �޸����� ���� ������ġ�� ���� ���� ���ڰ����� �����ش�.
            currentLetterIndex += previousRow->GetLength();
            //5.5.9 ���� ���� ���̸� ���Ѵ�.(�ڵ����� ��ҵ� ������ ������ġ���� ����)
            currentLettersLength = rowContent.length();
        }
        //5.6 ��¥ ���� ��ġ�� �����Ѵ�.
        Long realRowIndex = currentRowIndex;
        string koreanCompositionTemp;//�������� �Ž��� �ö󰡱� ������ �������� �ѱ��� �����ϸ� �ѱ��� �����
        //�������� �ʱ� ������ �ѱ��� �ӽ÷� �����ϰ� ���� ������� ������ �ӽ� ����Ұ� �ʿ���.
        //5.7 ���� ���� ��ġ�� 0���� ũ��(���� ���� ��ġ�� 0�̸� �ܾ������ �̵��� �� X)
        if (currentLetterIndex > 0)
        {
            //5.7.1 �ٿ��� �о�� �� ���� �� ĭ ��ŭ ���ҽ�Ų��.
            i = currentLettersLength - 1;
            //currentLetterIndex--;
            //5.7.2 �ٿ��� i��° ���� ���ڰ� �ѱ��̸�
            if ((rowContent[i] & 0x80))//�ѱ��̸�(2byte����)
            {
                //5.8.1.1 2byte�� �����Ѵ�.
                letterContent = rowContent[i];
                //�������� �Ž��� ���� ������ ����� �ѱ��� �����ϱ� ���ؼ� �ӽ������� �ؾ���.
                koreanCompositionTemp = letterContent;
                i--;
                letterContent = rowContent[i];
                letterContent += koreanCompositionTemp;
            }
            //5.8.2 �ѱ��� �ƴϸ�
            else
            {
                //5.8.2.1 1byte�� �����Ѵ�
                letterContent = rowContent[i];
            }
            //5.8.3 ���� ���� ��ġ�� 1���� ũ�� ���� ���ڰ� �����̽�(����)�����ε��� �ݺ��Ѵ�.
            while (currentLetterIndex > 1 && letterContent == " ")
            {
                //5.8.3.1 ���� ������ġ�� ���ҽ�Ų��.
                currentLetterIndex--;
                i--;
                //5.8.3.2 ���ڸ� �����Ѵ�.
                letterContent = rowContent[i];
            }
            //5.8.4 ���� ���� ��ġ�� 1�̰� ���� ���ڰ� �����̽�(����)�����̸�
            if (currentLetterIndex == 1 && letterContent == " ")
            {
                //5.8.4.1 ���� ������ġ�� ���ҽ�Ų��.
                currentLetterIndex--;
                i--;
            }
            //5.8.5 ���� ���� ��ġ�� 1���� ũ�� ���� ���ڰ� �ǹ����ε��� �ݺ��Ѵ�.
            while (currentLetterIndex > 1 && letterContent == "\t")
            {
                //5.8.5.1 ���� ������ġ�� ���ҽ�Ų��.
                currentLetterIndex--;
                i--;
                //5.8.5.2 ���ڸ� �����Ѵ�.
                letterContent = rowContent[i];
            }
            //5.8.6 ���� ���� ��ġ�� 1�̰� ���� ���ڰ� �ǹ����̸�
            if (currentLetterIndex == 1 && letterContent == "\t")
            {
                //5.8.6.1 ���� ������ġ�� ���ҽ�Ų��.
                currentLetterIndex--;
                i--;
            }

            if (currentLetterIndex > 1 && letterContent != " " && letterContent != "\t")
            {
                //5.8.7.1 ���� ���� ���ڰ� �ѱ��̸�
                if ((rowContent[i] & 0x80))//�ѱ��̸�(2byte����)
                {
                    //5.8.7.2.1 2byte�� �����Ѵ�.
                    letterContent = rowContent[i];
                    //�������� �Ž��� ���� ������ ����� �ѱ��� �����ϱ� ���ؼ� �ӽ������� �ؾ���.
                    koreanCompositionTemp = letterContent;
                    i--;
                    letterContent = rowContent[i];
                    letterContent += koreanCompositionTemp;
                }
                //5.8.7.2 �ѱ��� �ƴϸ�
                else
                {
                    //5.8.7.2.1 1byte�� �����Ѵ�
                    letterContent = rowContent[i];
                }
                //5.8.7.3 ���� ���� ��ġ�� ���ҽ�Ų��.
                //currentLetterIndex--;
                //i--;
            }

            //5.8.7 ���� ���� ��ġ�� 1���� ũ�� ���� ���ڰ� �����̽�(����)���� �׸���
            //�ǹ��ڰ� �ƴѵ��� �ݺ��Ѵ�.
            while (currentLetterIndex > 1 && letterContent != " " && letterContent != "\t")
            {
                //5.8.7.3 ���� ���� ��ġ�� ���ҽ�Ų��.
                currentLetterIndex--;
                i--;
                //5.8.7.1 ���� ���� ���ڰ� �ѱ��̸�
                if ((rowContent[i] & 0x80))//�ѱ��̸�(2byte����)
                {
                    //5.8.7.2.1 2byte�� �����Ѵ�.
                    letterContent = rowContent[i];
                    //�������� �Ž��� ���� ������ ����� �ѱ��� �����ϱ� ���ؼ� �ӽ������� �ؾ���.
                    koreanCompositionTemp = letterContent;
                    i--;
                    letterContent = rowContent[i];
                    letterContent += koreanCompositionTemp;
                }
                //5.8.7.2 �ѱ��� �ƴϸ�
                else
                {
                    //5.8.7.2.1 1byte�� �����Ѵ�
                    letterContent = rowContent[i];
                }
            }
            //5.8.8 ���� ĳ���� ���� ��ġ�� 1�̰� ���� ���ڰ� �����̽�(����) ���ڿ� �ǹ��ڰ� �ƴϸ�
            if (currentLetterIndex == 1 && letterContent != " " && letterContent != "\t")
            {
                //5.8.8.1 ���� ���� ���ڰ� �ѱ��̸�
                if ((rowContent[i] & 0x80))//�ѱ��̸�(2byte����)
                {
                    //5.8.8.2.1 2byte�� �����Ѵ�.
                    letterContent = rowContent[i];
                    //�������� �Ž��� ���� ������ ����� �ѱ��� �����ϱ� ���ؼ� �ӽ������� �ؾ���.
                    koreanCompositionTemp = letterContent;
                    i--;
                    letterContent = rowContent[i];
                    letterContent += koreanCompositionTemp;
                }
                //5.8.8.2 �ѱ��� �ƴϸ�
                else
                {
                    //5.8.8.2.1 1byte�� �����Ѵ�
                    letterContent = rowContent[i];
                }
                //5.8.8.3 ���� ���� ��ġ�� ���ҽ�Ų��.
                currentLetterIndex--;
                i--;
            }
            //5.8.9 ���� ���� ��ġ�� 0�̸�
            if (currentLetterIndex == 0)
            {
                //5.8.9.1 ���ڸ� �����Ѵ�.
                //letterContent = rowContent[i];
                //5.8.9.2 ���� ���� ù��° ���ڰ� �����̽� �Ǵ� �ǹ����̸�
                if (letterContent == "\t" || letterContent == " ")
                {
                    //5.8.9.2.1 ���� ���� ��ġ�� 1��ŭ ���ҽ�Ų��.(���� �ٷ� �̵���Ų��.)
                    currentRowIndex--;
                    //5.8.9.2.2 ���� ���� ��ġ�� underflow�̸�
                    if (currentRowIndex < 0)
                    {
                        //5.8.9.2.2.1 ���� ���� ��ġ�� �ּҰ����� �����Ѵ�.
                        currentRowIndex = 0;
                    }
                    //5.8.9.2.3 ���� ���� ��ġ�� underflow�� �ƴϸ�(this->current >=0)
                    else
                    {
                        //5.8.9.2.3.1 ���� ĳ���� ���� ��ġ�� ���������� �̵���Ų��.
                        currentRowIndex = this->notepadForm->note->Move(currentRowIndex);
                        this->notepadForm->current = this->notepadForm->
                            note->GetAt(currentRowIndex);
                        currentLetterIndex = this->notepadForm->current->Last();
                    }
                }
            }
        }
        //5.9 ���� ���� ��ġ�� ���� ó���̸�(0�̸�)
        else if (currentLetterIndex == 0)
        {
            //5.9.1 ���� ���� ��ġ�� 1��ŭ ���ҽ�Ų��.(���� �ٷ� �̵���Ų��.)
            currentRowIndex--;
            //5.9.2 ���� ���� ��ġ�� underflow�̸�
            if (currentRowIndex < 0)
            {
                //5.9.2.1 ���� ���� ��ġ�� �ּҰ����� �����Ѵ�.
                currentRowIndex = 0;
            }
            //5.9.3 ���� ���� ��ġ�� underflow�� �ƴϸ�(this->current >=0)
            else
            {
                //5.9.3.1 ���� ĳ���� ���� ��ġ�� ���������� �̵���Ų��.
                currentRowIndex = this->notepadForm->note->Move(currentRowIndex);
                this->notepadForm->current = this->notepadForm->
                   note->GetAt(currentRowIndex);
                currentLetterIndex = this->notepadForm->current->Last();
            }
        }
        //5.10 �޸��忡�� ���� ���� ���Ѵ�.(��¥ ��)
        Glyph* currentRow = this->notepadForm->note->GetAt(currentRowIndex);
        //5.11 ���� ���� content�� ���Ѵ�.
        Long k = 0;
        string currentRowContent = "";
        while (k < currentRow->GetLength())
        {
            //5.11.1 ���ڸ� ���Ѵ�.
            letter = currentRow->GetAt(k);
            letterContent = letter->GetContent();
            //5.11.2 ���� content�� �����ش�.
            currentRowContent += letterContent;
            k++;
        }
        //5.12 ���� ���� length�� ���Ѵ�.
        Long currentRowContentLength = currentRowContent.length();
        //5.13 i�� ���� ���� length���� ũ�ų� �������� �ݺ��Ѵ�.
        while (i >= currentRowContentLength)
        {
            //5.13.1 ���� ���� ���̸� ����.
            i -= currentRowContentLength;
            //5.13.2 ���� ���� ������ ����.
            currentLetterIndex -= currentRow->GetLength();
            //5.13.3 ���� �ٷ� �̵��Ѵ�.
            currentRowIndex++;
            //5.13.4 ���� ���� ���Ѵ�.
            currentRow = this->notepadForm->note->GetAt(currentRowIndex);
            //5.13.5 ���� ���� content�� ���Ѵ�.
            k = 0;
            currentRowContent = "";
            while (k < currentRow->GetLength())
            {
                //5.13.5.1 ���ڸ� ���Ѵ�.
                letter = currentRow->GetAt(k);
                letterContent = letter->GetContent();
                //5.13.5.2 ���� content�� �����ش�.
                currentRowContent += letterContent;
                k++;
            }
            //5.13.6 ���� ���� content�� length�� ���Ѵ�.
            currentRowContentLength = currentRowContent.length();
        }
        //5.14 ���� ���� ��ġ�� ������ġ�� �ٽ� �������ش�.
        currentRowIndex = this->notepadForm->note->Move(currentRowIndex);
        this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
        currentLetterIndex = this->notepadForm->current->Move(currentLetterIndex);
        //5.15 ���� ���� ��ġ�� ���� ���� ������ ���� ��ġ�̸�
        if (currentLetterIndex == this->notepadForm->current->GetLength())
        {
            //15.1 ���� ���� ���Ѵ�.
            Glyph* currentRow = this->notepadForm->current;
            //15.2 ���� ���� ���Ѵ�.
            Glyph* nextRow = this->notepadForm->note->GetAt(currentRowIndex + 1);
            //15.3 ���� ���� ��¥ ���̰� ���� ���� ��¥ ���̸�
            if (!dynamic_cast<DummyRow*>(nextRow) && dynamic_cast<DummyRow*>(currentRow))
            {
                //15.3.1 ���� �������� �̵��߱� ������ ĳ���� ���� ��ġ�� ���������� �����Ѵ�.
                currentLetterIndex = this->notepadForm->current->Last();
            }
            //15.4 ���� ���� ��¥ ���̰� ���� ���� ��¥ ���̸�
            else if (!dynamic_cast<DummyRow*>(nextRow) && !dynamic_cast<DummyRow*>(currentRow))
            {
                //15.4.1 ���� �������� �̵��߱� ������ ĳ���� ���� ��ġ�� ���������� �����Ѵ�.
                currentLetterIndex = this->notepadForm->current->Last();
            }
            //15.5 �� �̿ܿ���
            else
            {
                //15.5.1 ���� ���� ��ġ�� ������ġ�� �ٽ� �������ش�.
                currentRowIndex = this->notepadForm->note->Move(currentRowIndex + 1);
                this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
                currentLetterIndex = this->notepadForm->current->First();
            }
        }
	}
}

//�Ҹ���
CtrlLeftArrowKeyAction::~CtrlLeftArrowKeyAction()
{

}