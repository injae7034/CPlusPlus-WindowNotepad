#include "File.h"
#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "RowAutoChange.h"
#include<cstdio>
#pragma warning(disable:4996)

//기본생성자
File::File()
{

}

//소멸자
File::~File()
{

}

//가져오기
void File::Load(NotepadForm* notepadForm, string path)
{
    FILE* file;
    //입력받은 경로의 파일을 열어줌
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
        char* noteContent = 0;//문자열 포인터 0으로 초기화해줌.
        fseek(file, 0, SEEK_END);// 파일 포인터를 파일의 끝으로 이동시킴
        Long size = ftell(file);//파일의 전체 크기를 알아냄(문자열이라도 널문자는 포함되지 않음)
        //파일에 읽어들일 문자가 있는경우에만 힙에 할당함
        //size가 0이면 파일에 아무 문자가 없다는 뜻임(띄어쓰기나 개행문자도 없다는 뜻)
        //띄어쓰기나 개행문자는 문자(텍스트)로 포함함!!
        if (size > 0)//파일에 텍스트가 있으면
        {
            //널문자를 저장할 공간적 여유를 위해 size+1만큼 힙에 할당함
            noteContent = new char[size + 1];//파일의 전체 크기에서 +1만큼 힙에 할당함
        }
        //파일 전체가 여러줄일 경우에는 힙에 할당하는 공간이 넉넉해서 아무문제가 없지만
        //파일 전체가 한줄일 경우에는 널문자를 저장할 공간이 없기때문에 널문자를 저장하기 위해서
        //마지막 문자열을 저장하지 못하게 된다 왜냐하면 fgets는 자동으로 마지막에 널문자를 저장하기때문에
        //그래서 한글의 경우 한문장이 파일의 전체라면 문장의 마지막이 한글이면 한글이 깨지게 된다
        //그래서 size에 +1을 해서 널문자를 저장할 수 있게 여유공간을 제공하자!!!
        fseek(file, 0, SEEK_SET);//파일의 처음으로 이동시킴
        //힙에 할당한 문자열배열에 한줄을 저장함. 가장 최근에 fgets한거로 갱신되고 기존 내용은 사라짐.
        //fgets는 size+1이 길이이면 size만큼 또는 개행문자가 나오기전(줄바뀌기전)까지 읽고
        //마지막에([size+1])는 자동으로 널문자를 저장시킴!!
        //noteContent가 힙에 할당이 되어있으면(즉, 파일에 출력할 문자들이 있으면)
        //개행문자나 띄어쓰기포함해서 아무문자도 없으면 힙에 할당이 안되있고
        //그러면 파일에서 Load해올 문자가 아예 없기 때문바로 파일을 바로 닫으면 된다
        if (noteContent != 0)//파일에 텍스트가 있으면
        {
            //파일에 있는 텍스트를 줄단위로 읽는다.
            char* check = fgets((char*)noteContent, size + 1, file);
            //fgets는 파일의 끝이면(더이상 읽은 파일이 없으면(EOF이면)) NULL을 반환함.
            while (check != NULL)//파일의 끝이 아니면(EOF가 아니면)
            {
                i = 0;//배열요소 초기화
                //문장의 마지막(NULL문자)이 아닌동안 반복한다
                while (noteContent[i] != '\0')
                {
                    //문장을 저장한 각 배열요소가 2byte 문자인지 1byte 문자인지 판단한다.
                    if ((noteContent[i] & 0x80))//한글이면(2byte문자)
                    {
                        //doubleByteLetter에 저장한다.
                        doubleByteLetter[0] = noteContent[i];
                        i++;
                        doubleByteLetter[1] = noteContent[i];
                        doubleByteLetter[2] = '\0';
                        //doubleByteLetter를 생성한다.(팩토리메소드패턴)
                        glyph = glyphCreator.Create((char*)doubleByteLetter);
                        //줄에 doubleByteLetter를 추가한다.
                        letterIndex = notepadForm->current->Add(glyph);
                    }
                    //개행문자이면(줄을 바꾸면 줄을 추가한다)
                    else if (noteContent[i] == '\n' || noteContent[i] == '\r')
                    {
                        //row를 생성한다.
                        glyph = glyphCreator.Create(noteContent + i);
                        //새로운 줄을 추가한다.
                        rowIndex = notepadForm->note->Add(glyph);
                        //현재 줄의 위치를 새로 저장한다.
                        notepadForm->current = notepadForm->note->GetAt(rowIndex);
                    }
                    //입력받은 문자가 한글이 아니고 개행문자가 아니면(1byte문자)
                    else
                    {
                        //singleByteLetter에 저장한다.
                        singleByteLetter[0] = noteContent[i];
                        singleByteLetter[1] = '\0';
                        //singleByteLetter를 생성한다.
                        glyph = glyphCreator.Create(singleByteLetter);
                        //줄에 singleByteLetter를 추가한다.
                        letterIndex = notepadForm->current->Add(glyph);
                    }
                    i++;
                }
                check = fgets((char*)noteContent, size + 1, file);//힙에 할당한 문자열배열에 한줄을 저장함.
            }
            //무조건 할당해제(여기 들어온 조건문이 noteContent!=0이므로 이미 할당이 되어있기때문에)
            delete[] noteContent;
        }
        fclose(file);//파일을 닫음
    }
}

//보관하기
void File::Save(NotepadForm* notepadForm, string path)
{
    RowAutoChange rowAutoChage(notepadForm);
    bool isUndoAllRows = false;
    Long currentRowIndex = 0;
    Long currentLetterIndex = 0;
    //1. 자동개행이 되어 있으면
    if (notepadForm->isRowAutoChanging == true)
    {
        //1.1 현재 줄과 글자의 위치를 저장한다.
        currentRowIndex = notepadForm->note->GetCurrent();
        currentLetterIndex = notepadForm->current->GetCurrent();
        //자동개행이 되어 있을 때 자동개행을 안풀어주고 저장을 하면 자동개행에서 DummyRow도 모두
        //진짜 줄로 되어 저장되기 때문에 나중에 다시 불러올 떄 DummyRow도 Row로 인식되기 때문에
        //자동개행이 되어 있으면 모두 자동개행을 풀어준뒤에 저장해야한다!!!
        //1.2 자동개행을 풀어준다.
        rowAutoChage.UndoAllRows();
        //1.3 자동개행을 다풀었다는 표시를 해준다.
        isUndoAllRows = true;
    }
    FILE* file;
    //2. 입력받은 경로의 파일을 열어준다.
    file = fopen((char*)path.c_str(), "wt");
    //3. 파일이 열렸으면
    if (file != NULL)
    {
        //3.1 note의 content를 구한다.
        string content = notepadForm->note->GetContent();
        //3.2 파일에 note의 content를 저장한다.
        fputs((char*)content.c_str(), file);
        //3.3 파일을 닫는다.
        fclose(file);
    }
    //4. 자동개행을 풀었으면
    if (isUndoAllRows == true)
    {
        //4.1 다시 자동개행을 해준다.
        //아니면 파일을 저장하고 메모장 화면에서 자동개행이 풀리는 상태가 된다.
        rowAutoChage.DoAllRows();
        //4.2 아까 저장했던 현재 줄과 글자 위치로 이동한다.
        notepadForm->note->Move(currentRowIndex);
        notepadForm->current = notepadForm->note->GetAt(currentRowIndex);
        notepadForm->current->Move(currentLetterIndex);
    }
}
