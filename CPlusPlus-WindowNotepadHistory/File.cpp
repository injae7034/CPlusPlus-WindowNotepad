#include "File.h"
#include "NotepadForm.h"
#include "GlyphCreator.h"
//#include<iostream>
//#include<fstream>
#include<string>
#include<cstdio>

using namespace std;
#pragma warning(disable:4996)

//�⺻������
File::File(NotepadForm *notepadForm)
	:name(""), position("")
{
    this->notepadForm = notepadForm;
}

//�Ű����� 3�� ������
File::File(NotepadForm* noterpadForm, string name, string positon)
	:name(name), position(positon)
{
    this->notepadForm = notepadForm;
}

//���������
File::File(const File& source)
	:name(source.name), position(source.position)
{
    this->notepadForm = source.notepadForm;
}

//ġȯ������
File& File::operator=(const File& source)
{
    this->notepadForm = source.notepadForm;
	this->name = source.name;
	this->position = source.position;

    return *this;
}

//�Ҹ���
File::~File()
{

}

//��������
void File::Load(string name)
{
    this->name = name;
    this->name += ".txt";

    FILE* file;

    file = fopen((char*)this->name.c_str(), "rt");
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
        //���⳪ ���๮�ڴ� ���ڷ� ������!!
        if (size > 0)
        {
            noteContent = new char[size + 1];//������ ��ü ũ�⿡�� +1��ŭ ���� �Ҵ���
        }
        //���� ��ü�� �������� ��쿡�� ���� �Ҵ��ϴ� ������ �˳��ؼ� �ƹ������� ������
        //���� ��ü�� ������ ��쿡�� �ι��ڸ� ������ ������ ���⶧���� �ι��ڸ� �����ϱ� ���ؼ�
        //������ ���ڿ��� �������� ���ϰ� �ȴ� �ֳ��ϸ� fgets�� �ڵ����� �������� �ι��ڸ� �����ϱ⶧����
        //�׷��� �ѱ��� ��� �ѹ����� ������ ��ü��� ������ �������� �ѱ��̸� �ѱ��� ������ �ȴ�
        //�׷��� size�� +1�� �ؼ� �ι��ڸ� ������ �� �ְ� ���������� ��������!!!
        fseek(file, 0, SEEK_SET);//������ ó������ �̵���Ŵ
        //���� �Ҵ��� ���ڿ��迭�� ������ ������. ���� �������� fgets�Ѱŷ� ���ŵǰ� ���� ������ �����.
        //fgets�� size+1�� �����̸� size��ŭ �Ǵ� ���๮�ڰ� ��������(�ٹٲ����)���� �а�
        //��������([size+1])�� �ڵ����� �ι��ڸ� �����Ŵ!!
        //noteContent�� ���� �Ҵ��� �Ǿ�������(��, ���Ͽ� ����� ���ڵ��� ������)
        //���๮�ڳ� ���������ؼ� �ƹ����ڵ� ������ ���� �Ҵ��� �ȵ��ְ�
        //�׷��� ���Ͽ��� Load�ؿ� ���ڰ� �ƿ� ���� �����ٷ� ������ �ٷ� ������ �ȴ�
        if (noteContent != 0)
        {
            char* check = fgets((char*)noteContent, size + 1, file);
            //fgets�� ������ ���̸� NULL�� ��ȯ��.
            //������ ���� �ƴϸ�
            while (check != NULL)
            {
                i = 0;//�迭��� �ʱ�ȭ
                //������ ������(NULL����)�� �ƴѵ��� �ݺ��Ѵ�
                while (noteContent[i] != '\0')
                {
                    //������ ������ �� �迭��Ұ� �ѱ����� �������� �Ǵ��Ѵ�.
                    //�ѱ��̸�
                    if ((noteContent[i] & 0x80))
                    {
                        //doubleByteLetter�� �����Ѵ�.
                        doubleByteLetter[0] = noteContent[i];
                        i++;
                        doubleByteLetter[1] = noteContent[i];
                        doubleByteLetter[2] = '\0';
                        //doubleByteLetter�� �����Ѵ�.
                        glyph = glyphCreator.Create((char*)doubleByteLetter);
                        //�ٿ� doubleByteLetter�� �߰��Ѵ�.
                        letterIndex = this->notepadForm->current->Add(glyph);
                    }
                    //���๮���̸�
                    else if (noteContent[i] == '\n' || noteContent[i] == '\r')
                    {
                        //row�� �����Ѵ�.
                        glyph = glyphCreator.Create(noteContent + i);
                        //���ο� ���� �߰��Ѵ�.
                        rowIndex = this->notepadForm->note->Add(glyph);
                        //���� ���� ��ġ�� ���� �����Ѵ�.
                        this->notepadForm->current = this->notepadForm->note->GetAt(rowIndex);
                    }
                    //�Է¹��� ���ڰ� �ѱ��� �ƴϰ� ���๮�ڰ� �ƴϸ�
                    else
                    {
                        //singleByteLetter�� �����Ѵ�.
                        singleByteLetter[0] = noteContent[i];
                        singleByteLetter[1] = '\0';
                        //singleByteLetter�� �����Ѵ�.
                        glyph = glyphCreator.Create(singleByteLetter);
                        //�ٿ� singleByteLetter�� �߰��Ѵ�.
                        letterIndex = this->notepadForm->current->Add(glyph);
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

#if 0      
        while (!feof(file))
        {
            //2.10 �Է¹��� ���ڰ� ���๮�ڰ� �ƴϸ�
            if (content[0] != '\n' && content[0] != '\r')
            {
                //2.10.1 ���� �ٿ� ���ڸ� �߰��Ѵ�.
                rowIndex = this->notepadForm->note->GetAt(letterIndex)->Add(glyph);
                
            }
            //2.11 �Է¹��� ���ڰ� ���๮���̸�
            else if (content[0] == '\n' || content[0] == '\r')
            {
                //2.11.1 ���ο� ���� �߰��Ѵ�.
                letterIndex = this->notepadForm->note->Add(glyph);
                
            }
            fgets(content, 2, file);
            Glyph* glyph = glyphCreator.Create(content);
        }
        this->notepadForm->current = this->notepadForm->note->GetAt(rowIndex);

        fclose(file);


    ifstream readFile;             //���� ������ ���� ����
    readFile.open("�ؽ�Ʈ.txt");    //���� ����
    if (readFile.is_open())    //������ ���ȴ��� Ȯ��
    {
        i = 0;
        while (!readFile.eof())    //���� ������ �о����� Ȯ��
        {
            char arr[256] = this->notepadForm->note->GetAt(i)->GetContent().c_str();
            readFile.getline(arr, 256);//���پ� �о����
        }
    }
    readFile.close();    //���� �ݱ�
}
#endif

//�����ϱ�
void File::Save(string name)
{
    this->name = name;
    this->name += ".txt";
    //�ݺ������
    Long i;
    FILE* file;
    //������ ������
    file = fopen((char*)this->name.c_str(), "wt");
    if (file != NULL)
    {
        //fprintf(file, "Hello World?");
        //char content[100] = "Hello World!";
        //puts(content);
        string content;
        i = 0;
        //���� �ٲ� ��� ���๮�ڸ� �Է��ؼ� �������
        while (i < this->notepadForm->note->GetLength() - 1)
        {
            content = this->notepadForm->note->GetAt(i)->GetContent();
            fputs((char*)content.c_str(), file);
            //����� ��������
            fputs((char*)"\n", file);
            i++;
        }
        //���๮�ھ���(���� �ٲ��� �ʰ�) ������ ������ ���(������� ������ �������� ���)
        if (i < this->notepadForm->note->GetLength())
        {
            content = this->notepadForm->note->GetAt(i)->GetContent();
            //������� �����
            fputs((char*)content.c_str(), file);
        }
        //������ �ݾ���
        fclose(file);
    }

#if 0
    ifstream inFile("������.txt");
    string in_line;
    Long i = 0;
    while (i < this->notepadForm->note->GetLength())
    {
        in_line = this->notepadForm->note->GetAt(i)->GetContent();
        cout << in_line << endl;
        while (getline(inFile, in_line)) {
            cout << in_line << endl;
        }
        i++;
    }
    inFile.close();
#endif
#if 0
    ofstream writeFile;            //�� ������ ���� ����
    writeFile.open("������.txt"); //���� ����
    Long i;
    if (writeFile.is_open())    //������ ���ȴ��� Ȯ��
    {
        i = 0;
        while (i < this->notepadForm->note->GetLength())
        {
            writeFile.write(this->notepadForm->note->GetAt(i)->GetContent().c_str(), this->notepadForm->note->GetAt(i)->GetLength());
            writeFile.write("\n", 1);
            i++;
        }
    }
    writeFile.close();    //���� �ݱ�
#endif
}
