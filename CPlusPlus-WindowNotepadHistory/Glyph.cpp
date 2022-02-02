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

//Add(Insert)
Long Glyph::Add(Long index, Glyph* glyph)
{
	return -1;
}

//Remove
Long Glyph::Remove(Long index)
{
	return -1;//배열 첨자가 없는 경우 -1을 반환 0이 첫 배열요소이고 배열요소가 없다는 뜻은 -1임.
}

Glyph* Glyph::Split(Long index)
{
	return 0;
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

//GetPartOfContent
string Glyph::GetPartOfContent(Long current)
{
	return "\0";
}

//First
Long Glyph::First()
{
	return -1;
}

//Last
Long Glyph::Last()
{
	return -1;
}

//Previous
Long Glyph::Previous()
{
	return -1;
}

//Next
Long Glyph::Next()
{
	return -1;
}

//NextWord
Long Glyph::NextWord()
{
	return -1;
}

//PreviousWord
Long Glyph::PreviousWord()
{
	return - 1;
}