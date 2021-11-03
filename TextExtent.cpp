#include "TextExtent.h"
#include "NotepadForm.h"
#include "Glyph.h"

//디폴트생성자
TextExtent::TextExtent(NotepadForm* notepadForm)
{
	//1. notepadForm의 주소를 저장한다.
	this->notepadForm = notepadForm;
	//2. CClientDC를 생성한다.
    CClientDC dc(this->notepadForm);
    //3. CFont를 생성한다.
    CFont font;
    //4. 글씨크기와 글씨체를 정하다.
    font.CreateFontIndirect(&this->notepadForm->font.GetLogFont());
    //5. 폰트를 dc에 지정한다.
    HFONT oldFont;
    oldFont = (HFONT)dc.SelectObject(font);
    //6. TEXTMETRIC을 생성한다.
    TEXTMETRIC textmetric;
    //7. 글꼴의 정보를 얻는다.
    dc.GetTextMetrics(&textmetric);
    //8. 0~127번의 아스키코드표에 있는 글자들의 폭을 반복해서 widths 배열에 각각 저장한다.
    //(아스키코드표의 글자들은 전부 폭이 각각 다르기 때문에 따로 저장해줌.)
    CString letter;
    CSize letterSize;
    //아스키코드 0번째는 NULL이기 때문에 NULL의 폭을 구하면 뻑이나기 때문에 1부터 시작함.
    char i = 1;
    //char 아스키코드 범위가 -128~127(256)이므로 127을 넘어가면 자동으로 -128이 되기때문에
    //i>0 조건이 없으면 무한반복이 되기때문에 원하는 결과(아스키코드 내에서 반복)를 얻기 위해선
    //i>0이 반드시 필요하다.
    while (i < 128 && i>0)
    {
        letter = i;
        letterSize = dc.GetTextExtent(letter);
        this->widths[i] = letterSize.cx;
        i++;
    }
    
    //9. 129번째 배열요소에 한글의 폭을 저장한다.(한글은 폭이 같기 때문에 한 개의 폭만 저장해줌.)
    //i는 최대 127까지만 이용가능하므로 그냥 숫자 128로 대입해주자!
    letter = "ㄱ";
    letterSize = dc.GetTextExtent(letter);
    this->widths[128] = letterSize.cx;
    letter = "가";
    letterSize = dc.GetTextExtent(letter);
    this->widths[129] = letterSize.cx;
    //10. i=0 번째에 쓰레기값 대신에 스페이스를 저장한다.
    //아스키코드 0번째는 null이므로 폭이 저장될 수 없으나 쓰레기값대신 스페이스를 저장해줌.
    letter = " ";
    letterSize = dc.GetTextExtent(letter);
    this->widths[0] = letterSize.cx;
    //11. Tab키의 길이를 저장해준다.
    letter = "        ";
    letterSize = dc.GetTextExtent(letter);
    this->widths[9] = letterSize.cx;
    //12. 평균 폰트 높이를 저장한다.
    this->height = textmetric.tmHeight;
    //13. 끝내다.
    dc.SelectObject(oldFont);
    DeleteObject(font);
}

//GetTextWidth
Long TextExtent::GetTextWidth(string content)//count(캐럿의 가로 위치)는 필요없음!
{
#if 0
    CString rowText = CString(this->notepadForm->current->
        GetPartOfContent(this->notepadForm->current->GetCurrent()).c_str());
    CString letter = CString(this->notepadForm->current->GetAt(i - 1)->GetContent().c_str());
#endif  
    //1. i가 length보다 작은동안 반복한다.
    Long i = 0;
    Long width = 0;
    Long length = (Long)content.length();
    char letter[3];

    //한글은 배열요소 2개를 차지하고 아스키코드 문자는 배열요소 1개를 차지하기 때문에, count를 외부에서
    //입력받지 말고, string의 length를 구하면 배열요소의 개수를 알려주기 때문에 이를 이용하자!
    while (i < length)
    {
        letter[0] = content[i];
        letter[1] = '\0';
        //1.1 한글이면
        if ((content[i] & 0x80))
        {
            letter[1] = content[i + 1];
            letter[2] = '\0';
            if (letter >= "ㄱ" && letter <= "ㅣ")
            {
                //1.1.1 한글의 폭을 더해준다.
                width += this->widths[128];
            }
            else
            {
                width += this->widths[129];
            }
            i++;//한글은 배열요소가 2개이기 때문에 1을 더해줘서 2개를 차지하게 한다.
        }
        //1.2 한글이 아니면(아스키코드 문자이면)
        //아스키코드는 배열요소 1개를 차지하기 때문에 따로 +1을 해줄필요가 없다!
        else
        {
            //여기서 content[i]는 한글이 아닌 아스키코드이기때문에 숫자로 활용이 가능하다!
            //따라서 아스키코드랑 비교하며 따로 찾을 필요없이 바로 배열요소에 대입하는 숫자로 활용가능!
            width += this->widths[content[i]];
#if 0
            //1.2.1 j가 128보다 작은동안 그리고 con
            char j = 0;
            while (j < 128 && content[i] != j)
            {
                j++;
            }
            if (j < 128)
            {
                width += this->widths[j];
            }
#endif
        }
        i++;
    }
    return width;
}

//소멸자
TextExtent::~TextExtent()
{

}