#ifndef _FILE_H
#define _FILE_H

#include<string>
using namespace std;
class NotepadForm;//�ּҷ� �̿�Ǹ� ���漱���� ����

class File
{
public:
	File();//����Ʈ������
	~File();//�Ҹ���
	void Load(NotepadForm* notepadForm, string path);//��������
	void Save(NotepadForm* notepadForm, string path);//�����ϱ�
};

#endif // !_FILE_H

