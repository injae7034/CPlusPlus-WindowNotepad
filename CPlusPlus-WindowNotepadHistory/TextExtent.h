#ifndef _TEXTEXTENT
#define _TEXTEXTENT

#include<string>
using namespace std;
class NotepadForm;
typedef signed long int Long;

class TextExtent
{
public:
	NotepadForm* notepadForm;
public:
	TextExtent(NotepadForm* notepadForm = 0);//디폴트생성자
	//원래 밖에서 캐럿의 현재 가로 위치를 입력받아서 그것을 개수로 활용해 반복을 돌릴려고 했는데
	//한글은 배열요소 2개를 차지하기 때문에 이를 활용할 수 없음. 차라리 string의 length를 이용해
	//배열요소의 개수를 구하고 그 length만큼 반복을 돌리면 되서 더이상 count는 입력받을 필요가 X
	//Long GetTexrWidth(string content, Long count);
	Long GetTextWidth(string content);
	~TextExtent();//소멸자
	//인라인 함수 선언
	Long* GetWidths() const;
	Long GetHeight() const;
private:
	Long widths[129];
	Long height;
};

//인라인함수 정의
inline Long* TextExtent::GetWidths() const
{
	return (Long*)this->widths;
}
inline Long TextExtent::GetHeight() const
{
	return this->height;
}

#endif // !_TEXTEXTENT
