#include "Glyph.h"

//디폴트생성자
Glyph::Glyph()
{

}

//소멸자
Glyph::~Glyph()
{

}

//Add
Long Glyph::Add(Glyph* glyph)
{
	return -1;//배열 첨자가 없는 경우 -1을 반환 0이 첫 배열요소이고 배열요소가 없다는 뜻은 -1임.
}

//Remove
Long Glyph::Remove(Long index)
{
	return -1;//배열 첨자가 없는 경우 -1을 반환 0이 첫 배열요소이고 배열요소가 없다는 뜻은 -1임.
}

//GetAt
Glyph* Glyph::GetAt(Long index)
{
	return 0;//위치가 null인 경우 0을 반환
}

//Clone
Glyph* Glyph::Clone()
{
	return 0;//Glyph*(위치)가 null인 경우 0을 반환
}

//GetContent
string Glyph::GetContent()
{
	return "\0";
}



