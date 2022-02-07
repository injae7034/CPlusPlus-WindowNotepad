#ifndef _PRINTINFORMATION_H
#define _PRINTINFORMATION_H

#include <afxwin.h>
typedef signed long int Long;
class NotepadForm;
class Glyph;
class TextExtent;
class Font;

class PrintInformation {
public:
	NotepadForm* notepadForm;
public:
	PrintInformation(NotepadForm* notepadForm, LOGFONT printLogFont, HDC hdc);
	~PrintInformation();
	//인라인함수
	Glyph* GetPrintNote() const;
	LOGFONT& GetPrintLogFont() const;
	HDC GetHDC() const;
	Long GetPageRowCount() const;
	Long GetPrintableAreaHeight() const;
private:
	Glyph* printNote;
	LOGFONT printLogFont;
	HDC hdc;
	Long pageRowCount;
	Long printableAreaHeight;
};

inline Glyph* PrintInformation::GetPrintNote() const
{
	return const_cast<Glyph*>(this->printNote);
}
inline LOGFONT& PrintInformation::GetPrintLogFont() const
{
	return const_cast<LOGFONT&>(this->printLogFont);
}
inline HDC PrintInformation::GetHDC() const
{
	return this->hdc;
}
inline Long PrintInformation::GetPageRowCount() const
{
	return this->pageRowCount;
}
inline Long PrintInformation::GetPrintableAreaHeight() const
{
	return this->printableAreaHeight;
}
#endif // !_PRINTINFORMATION_H
