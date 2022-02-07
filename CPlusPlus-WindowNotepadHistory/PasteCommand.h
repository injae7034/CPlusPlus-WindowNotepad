#ifndef _PASTECOMMAND_H
#define _PASTECOMMAND_H

#include "Command.h"

class NotepadForm;
class Glyph;

class PasteCommand :public Command
{
public:
	PasteCommand(NotepadForm* notepadForm = 0);
	void Execute();
	virtual void Unexecute();//�������
	virtual void SetUndoMacroEnd();//���������� �������� ����
	virtual void SetRedoMacroEnd();//�ٽý������ �������� ����
	virtual void SetRedone();//�ٽý����̶�� ǥ�ø� ����
	virtual Long GetRowIndex();//����� ����� ������ ���� ��ġ�� ���ϱ�
	virtual Long GetLetterIndex();//����� ���屭 ������ ĭ�� ��ġ�� ���ϱ�
	virtual Long GetStartPasteRowIndex();//����� ����� �ٿ��ֱⰡ ���۵Ǵ� ���� ��ġ���ϱ�
	virtual Long GetStartPasteLetterIndex();//����� ����� �ٿ��ֱⰡ ���۵Ǵ� ������ ��ġ���ϱ�
	virtual bool IsUndoMacroEnd();//���������� �������� ���ϱ�
	virtual bool IsRedoMacroEnd();//�ٽý������ �������� ���ϱ�
	virtual bool IsRedone();//�ٽý������� �ƴ��� ���ϱ�
	virtual bool IsDirty();//��������� �ִ��� Ȯ�� ����
	virtual bool IsSelectedTextsRemoved();//���ÿ����� �������� ���������� ����
	//�ζ����Լ� ����(�ζ����Լ��� �θ�Ŭ�������� �������� �� ����)
	Glyph* GetNote() const;//����� ����� ���ÿ��� ���ϱ�
	Glyph* GetGlyph() const;//����� ����� ���� ���ϱ�
	~PasteCommand();//�Ҹ���
private:
	Glyph* note;//���ÿ����� ����� ���� ���ÿ����� ������ ����
	Glyph* glyph;//����� ���� ���� ���ڸ� ���� ����
	Long startPasteRowIndex;//�ٿ��ֱⰡ ���۵Ǵ� ������ġ
	Long startPasteLetterIndex;//�ٿ��ֱⰡ ����Ǵ� ������ġ
	Long rowIndex;//�Էµ� ������ ����ġ(�Է��� ������ ���� ��ġ)
	Long letterIndex;//�Էµ� ������ ĭ��ġ(�Է��� ������ ������ ��ġ)
	bool isUndoMacroEnd;//���������� ��������
	bool isRedoMacroEnd;//�ٽý������ ��������
	bool isRedone;//�ٽý������� �ƴ��� ����
	bool isDirty;//��������� �ִ��� ����
	bool isSelectedTextsRemoved;//���ÿ����� �������� ���������� ����
};

//�ζ����Լ� ����
inline Glyph* PasteCommand::GetGlyph() const
{
	return this->glyph;
}
inline Glyph* PasteCommand::GetNote() const
{
	return this->note;
}

#endif // !_PASTECOMMAND_H