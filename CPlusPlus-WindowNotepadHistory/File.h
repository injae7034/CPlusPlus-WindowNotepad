#ifndef _FILE_H
#define _FILE_H

#include<string>
using namespace std;
class NotepadForm;//�ּҸ� ����ϱ⶧���� ���漱������.

class File
{
public:
	File(NotepadForm* notepadForm = 0);//����Ʈ������
	File(NotepadForm *notepadForm, string name, string path);//�Ű������� 3�� ������
	File(const File& source);//���������
	~File();//�Ҹ���
	File& operator=(const File& source);//ġȯ������
	void Load(string name, string path);//�ҷ�����
	void Save(string name, string path);//�����ϱ�
	//�ζ����Լ�
	string& GetName() const;
	string& GetPath() const;
public://notepadForm�� �ּҴ� public���� ������.
	NotepadForm* notepadForm;
private:
	string name;
	string path;
};

//�ζ����Լ�����
inline string& File::GetName() const
{
	return const_cast<string&>(this->name);
}
inline string& File::GetPath() const
{
	return const_cast<string&>(this->path);
}

#endif // !_FILE_H

