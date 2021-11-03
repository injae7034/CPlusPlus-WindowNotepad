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
	virtual Long GetStartXPos();//�����ϴ� ������ ��ġ���ϱ�
	virtual Long GetStartYPos();//�����ϴ� ���� ��ġ���ϱ�
	virtual Long GetSelectedEndXPos();//������ ������ ������ ��ġ���ϱ�
	virtual Long GetSelectedEndYPos();//������ ������ ���� ��ġ���ϱ�
	virtual Long GetPastingEndXPos();//�ٿ��ֱⰡ ������ ������ ��ġ���ϱ�
	virtual Long GetPastingEndYPos();//�ٿ��ֱⰡ ������ ���� ��ġ���ϱ�
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
	Long startXPos;//�����ϴ� ������ ��ġ
	Long startYPos;//�����ϴ� ���� ��ġ
	Long selectedEndXPos;//������ ������ ������ġ
	Long selectedEndYPos;//������ ������ ����ġ
	Long pastingEndXPos;//�ٿ��ֱⰡ ������ ������ġ
	Long pastingEndYPos;//�ٿ��ֱⰡ ������ ����ġ
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