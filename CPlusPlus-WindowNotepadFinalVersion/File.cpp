#include "File.h"
#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "RowAutoChange.h"
#include<cstdio>
#pragma warning(disable:4996)

//�⺻������
File::File()
{

}

//�Ҹ���
File::~File()
{

}

//��������
void File::Load(NotepadForm* notepadForm, string path)
{
    FILE* file;
    //�Է¹��� ����� ������ ������
    file = fopen((char*)path.c_str(), "rt");
    if (file != NULL)
    {
        GlyphCreator glyphCreator;
        Long rowIndex = 0;
        Long letterIndex = 0;
        Long i;
        char doubleByteLetter[3];
        char singleByteLetter[2];
        Glyph* glyph;
        char* noteContent = 0;//���ڿ� ������ 0���� �ʱ�ȭ����.
        fseek(file, 0, SEEK_END);// ���� �����͸� ������ ������ �̵���Ŵ
        Long size = ftell(file);//������ ��ü ũ�⸦ �˾Ƴ�(���ڿ��̶� �ι��ڴ� ���Ե��� ����)
        //���Ͽ� �о���� ���ڰ� �ִ°�쿡�� ���� �Ҵ���
        //size�� 0�̸� ���Ͽ� �ƹ� ���ڰ� ���ٴ� ����(���⳪ ���๮�ڵ� ���ٴ� ��)
        //���⳪ ���๮�ڴ� ����(�ؽ�Ʈ)�� ������!!
        if (size > 0)//���Ͽ� �ؽ�Ʈ�� ������
        {
            //�ι��ڸ� ������ ������ ������ ���� size+1��ŭ ���� �Ҵ���
            noteContent = new char[size + 1];//������ ��ü ũ�⿡�� +1��ŭ ���� �Ҵ���
        }
        //���� ��ü�� �������� ��쿡�� ���� �Ҵ��ϴ� ������ �˳��ؼ� �ƹ������� ������
        //���� ��ü�� ������ ��쿡�� �ι��ڸ� ������ ������ ���⶧���� �ι��ڸ� �����ϱ� ���ؼ�
        //������ ���ڿ��� �������� ���ϰ� �ȴ� �ֳ��ϸ� fgets�� �ڵ����� �������� �ι��ڸ� �����ϱ⶧����
        //�׷��� �ѱ��� ��� �ѹ����� ������ ��ü��� ������ �������� �ѱ��̸� �ѱ��� ������ �ȴ�
        //�׷��� size�� +1�� �ؼ� �ι��ڸ� ������ �� �ְ� ���������� ��������!!!
        fseek(file, 0, SEEK_SET);//������ ó������ �̵���Ŵ
        //���� �Ҵ��� ���ڿ��迭�� ������ ������. ���� �ֱٿ� fgets�Ѱŷ� ���ŵǰ� ���� ������ �����.
        //fgets�� size+1�� �����̸� size��ŭ �Ǵ� ���๮�ڰ� ��������(�ٹٲ����)���� �а�
        //��������([size+1])�� �ڵ����� �ι��ڸ� �����Ŵ!!
        //noteContent�� ���� �Ҵ��� �Ǿ�������(��, ���Ͽ� ����� ���ڵ��� ������)
        //���๮�ڳ� ���������ؼ� �ƹ����ڵ� ������ ���� �Ҵ��� �ȵ��ְ�
        //�׷��� ���Ͽ��� Load�ؿ� ���ڰ� �ƿ� ���� �����ٷ� ������ �ٷ� ������ �ȴ�
        if (noteContent != 0)//���Ͽ� �ؽ�Ʈ�� ������
        {
            //���Ͽ� �ִ� �ؽ�Ʈ�� �ٴ����� �д´�.
            char* check = fgets((char*)noteContent, size + 1, file);
            //fgets�� ������ ���̸�(���̻� ���� ������ ������(EOF�̸�)) NULL�� ��ȯ��.
            while (check != NULL)//������ ���� �ƴϸ�(EOF�� �ƴϸ�)
            {
                i = 0;//�迭��� �ʱ�ȭ
                //������ ������(NULL����)�� �ƴѵ��� �ݺ��Ѵ�
                while (noteContent[i] != '\0')
                {
                    //������ ������ �� �迭��Ұ� 2byte �������� 1byte �������� �Ǵ��Ѵ�.
                    if ((noteContent[i] & 0x80))//�ѱ��̸�(2byte����)
                    {
                        //doubleByteLetter�� �����Ѵ�.
                        doubleByteLetter[0] = noteContent[i];
                        i++;
                        doubleByteLetter[1] = noteContent[i];
                        doubleByteLetter[2] = '\0';
                        //doubleByteLetter�� �����Ѵ�.(���丮�޼ҵ�����)
                        glyph = glyphCreator.Create((char*)doubleByteLetter);
                        //�ٿ� doubleByteLetter�� �߰��Ѵ�.
                        letterIndex = notepadForm->current->Add(glyph);
                    }
                    //���๮���̸�(���� �ٲٸ� ���� �߰��Ѵ�)
                    else if (noteContent[i] == '\n' || noteContent[i] == '\r')
                    {
                        //row�� �����Ѵ�.
                        glyph = glyphCreator.Create(noteContent + i);
                        //���ο� ���� �߰��Ѵ�.
                        rowIndex = notepadForm->note->Add(glyph);
                        //���� ���� ��ġ�� ���� �����Ѵ�.
                        notepadForm->current = notepadForm->note->GetAt(rowIndex);
                    }
                    //�Է¹��� ���ڰ� �ѱ��� �ƴϰ� ���๮�ڰ� �ƴϸ�(1byte����)
                    else
                    {
                        //singleByteLetter�� �����Ѵ�.
                        singleByteLetter[0] = noteContent[i];
                        singleByteLetter[1] = '\0';
                        //singleByteLetter�� �����Ѵ�.
                        glyph = glyphCreator.Create(singleByteLetter);
                        //�ٿ� singleByteLetter�� �߰��Ѵ�.
                        letterIndex = notepadForm->current->Add(glyph);
                    }
                    i++;
                }
                check = fgets((char*)noteContent, size + 1, file);//���� �Ҵ��� ���ڿ��迭�� ������ ������.
            }
            //������ �Ҵ�����(���� ���� ���ǹ��� noteContent!=0�̹Ƿ� �̹� �Ҵ��� �Ǿ��ֱ⶧����)
            delete[] noteContent;
        }
        fclose(file);//������ ����
    }
}

//�����ϱ�
void File::Save(NotepadForm* notepadForm, string path)
{
    RowAutoChange rowAutoChage(notepadForm);
    bool isUndoAllRows = false;
    Long currentRowIndex = 0;
    Long currentLetterIndex = 0;
    //1. �ڵ������� �Ǿ� ������
    if (notepadForm->isRowAutoChanging == true)
    {
        //1.1 ���� �ٰ� ������ ��ġ�� �����Ѵ�.
        currentRowIndex = notepadForm->note->GetCurrent();
        currentLetterIndex = notepadForm->current->GetCurrent();
        //�ڵ������� �Ǿ� ���� �� �ڵ������� ��Ǯ���ְ� ������ �ϸ� �ڵ����࿡�� DummyRow�� ���
        //��¥ �ٷ� �Ǿ� ����Ǳ� ������ ���߿� �ٽ� �ҷ��� �� DummyRow�� Row�� �νĵǱ� ������
        //�ڵ������� �Ǿ� ������ ��� �ڵ������� Ǯ���صڿ� �����ؾ��Ѵ�!!!
        //1.2 �ڵ������� Ǯ���ش�.
        rowAutoChage.UndoAllRows();
        //1.3 �ڵ������� ��Ǯ���ٴ� ǥ�ø� ���ش�.
        isUndoAllRows = true;
    }
    FILE* file;
    //2. �Է¹��� ����� ������ �����ش�.
    file = fopen((char*)path.c_str(), "wt");
    //3. ������ ��������
    if (file != NULL)
    {
        //3.1 note�� content�� ���Ѵ�.
        string content = notepadForm->note->GetContent();
        //3.2 ���Ͽ� note�� content�� �����Ѵ�.
        fputs((char*)content.c_str(), file);
        //3.3 ������ �ݴ´�.
        fclose(file);
    }
    //4. �ڵ������� Ǯ������
    if (isUndoAllRows == true)
    {
        //4.1 �ٽ� �ڵ������� ���ش�.
        //�ƴϸ� ������ �����ϰ� �޸��� ȭ�鿡�� �ڵ������� Ǯ���� ���°� �ȴ�.
        rowAutoChage.DoAllRows();
        //4.2 �Ʊ� �����ߴ� ���� �ٰ� ���� ��ġ�� �̵��Ѵ�.
        notepadForm->note->Move(currentRowIndex);
        notepadForm->current = notepadForm->note->GetAt(currentRowIndex);
        notepadForm->current->Move(currentLetterIndex);
    }
}
