
#include "pch.h"
#include<iostream>
#include<string>
#include<fstream>
#include<cassert>
#include<map> 
#include<cstdlib>
#include<time.h> 
#define MAXN 10000000
using namespace std;

class testfile
{
public:
	testfile countcha(string, testfile);//计算字符数
	testfile countword(testfile, string, int);//计算单词数
	testfile countline(string, testfile);//计算行数
	int getcharacters();
	int getlines();
	int getwords();
	string Title;//存放标题数据
	string Abstract;
	void init();
private:
	int characters;
	int words;
	int lines;
};

int testfile::getcharacters()
{
	return characters;
}

int testfile::getlines()
{
	return lines;
}

int testfile::getwords()
{
	return words;
}

void testfile::init()
{
	words = 0;
	lines = 0;
	characters = 0;
}

testfile testfile::countcha(string t, testfile f1)
{
	int i = 0;
	ifstream myfile;
	myfile.open(t);
	if (!myfile.is_open())
	{
		cout << "文件打开失败" << endl;
		exit(0);
	}
	char c;
	myfile >> noskipws;//强制读入空格和换行符
	while (!myfile.eof())
	{
		myfile >> c;
		if (myfile.eof())
			break;//防止最后一个字符输出两次
		i++;
	}
	f1.characters = i;
	myfile.close();
	return f1;
}

testfile testfile::countline(string t, testfile f1)
{
	ifstream myfile;
	myfile.open(t, ios::in);
	int i = 0, j = 0;
	int flag = 0;
	string temp;//作为getline参数使用
	string ti = "Title: ";
	string ab = "Abstract: ";
	if (!myfile.is_open())
	{
		cout << "文件打开失败" << endl;
		exit(0);
	}
	while (getline(myfile, temp))
	{
		if (temp.empty())
			continue;
		i++;
		if (!temp.compare(0, 7, ti, 0, 7))
		{
			temp.erase(0, 7);
			while (1)
			{
				f1.Title += temp;
				f1.Title += ' ';
				if (!getline(myfile, temp))
				{
					flag = 1;
					break;
				}
				if (temp.empty()) break;
				i++;
				if (!temp.compare(0, 9, ab, 0, 9))
					break;
			}
		}
		if (flag == 1)
			break;
		if ((!temp.compare(0, 9, ab, 0, 9)))
		{
			temp.erase(0, 9);
			while (1)
			{
				f1.Abstract += temp;
				f1.Abstract += ' ';
				if (!getline(myfile, temp))
				{
					flag = 1;
					break;
				}
				if (temp.empty())
					break;
				i++;
			}

		}
		if (flag == 1)
			break;
	}
	f1.lines = i;
	return f1;
}
map<string, int> mapword1;

void loadword(string w, int i)
{
	string wr;
	wr = w;
	map<string, int>::iterator it1 = mapword1.find(wr);//在map红黑树中查找单词 
	if (it1 == mapword1.end())
		mapword1.insert(pair<string, int>(wr, i));//未找到单词，插入单词并设定频次为1 
	else
		it1->second += i;//找到单词，单词出现频次增加 
}

testfile testfile::countword(testfile f1, string ss, int quan)
{
	char temp[4];
	int i = 0, j = 0, flag = 0, words = 0, m = 0, k = 0;
	for (i = 0; i < ss.length(); i++)
	{
		if (!((ss[i] >= 48 && ss[i] <= 57) || (ss[i] >= 97 && ss[i] <= 122)))//跳过非字母和非数字字符 
			continue;
		else
		{
			for (j = 0; j < 4 && i < ss.length(); j++)
			{
				if (!((ss[i] >= 48 && ss[i] <= 57) || (ss[i] >= 97 && ss[i] <= 122)))
					break;
				temp[j] = ss[i++];//temp中存入四个非空格字符
			}
			if (j == 4)
			{
				for (m = 0; m < 4; m++)
				{
					if (temp[m] < 97 || temp[m]>122)
					{
						flag = 1;
						break;//判断这四个字符是否都是字母
					}
				}
				if (flag == 0)//四个字符都是字母的情况，判断为一个单词
				{
					char *w = new char[100];//存放单词 
					for (m = 0; m < 4; m++)
					{
						w[k++] = temp[m];//temp中字符存入w
					}
					while (((ss[i] >= 48 && ss[i] <= 57) || (ss[i] >= 97 && ss[i] <= 122)) && i < ss.length())//继续存入单词剩余字符
					{
						w[k++] = ss[i++];
					}
					w[k] = '\0';
					loadword(w, quan);//可以在此处插入一个外部函数返回一个单词存入map红黑树 
					delete[]w;
					words++;
					k = 0;
				}
				else
				{
					flag = 0;
					j = 0;
				}
			}
		}
	}
	f1.words += words;
	return f1;
}

struct sWord
{
	string w;
	int count;

};//定义一个用于存放单词及频次的结构体 

void merge(sWord *a, sWord *c, int l, int mid, int r) {
	int i = l, j = mid + 1, m = 1;
	while (i <= mid && j <= r) {
		if (a[i].count < a[j].count) c[m++] = a[j++];
		else c[m++] = a[i++];
	}
	while (i <= mid) c[m++] = a[i++];
	while (j <= r) c[m++] = a[j++];
	for (int k = 1; k <= r - l + 1; k++)
		a[l + k - 1] = c[k];
}

void sort(sWord *a, sWord *c, int l, int r) {
	if (l < r) {
		int mid = (l + r) / 2;
		sort(a, c, l, mid);
		sort(a, c, mid + 1, r);
		merge(a, c, l, mid, r);
	}
}

string changeDx(string ss)
{
	int i;
	for (i = 0; i < ss.length(); i++)
	{
		if (ss[i] >= 65 && ss[i] <= 90)
		{
			ss[i] += 32;
		}
	}
	return ss;
}

void outCome1(sWord *ww, int num, string output, testfile f1)
{
	ofstream fout;
	fout.open(output);
	if (!fout)
		cout << "文件打开失败" << endl;
	cout << "characters: " << f1.getcharacters() << endl;
	fout << "characters: " << f1.getcharacters() << endl;
	cout << "words: " << f1.getwords() << endl;
	fout << "words: " << f1.getwords() << endl;
	cout << "lines: " << f1.getlines() << endl;
	fout << "lines: " << f1.getlines() << endl;
	int i;
	if (num < 10)
	{
		for (i = 0; i < num; i++)
		{
			cout << "<" << ww[i].w << ">" << ": " << ww[i].count << endl;
			fout << "<" << ww[i].w << ">" << ": " << ww[i].count << endl;
		}
	}
	else
	{
		for (i = 0; i < 10; i++)

		{
			cout << "<" << ww[i].w << ">" << ": " << ww[i].count << endl;
			fout << "<" << ww[i].w << ">" << ": " << ww[i].count << endl;
		}
	}
	fout.close();
}

void outCome2(sWord *ww, int num, string output, testfile f1)
{
	ofstream fout;
	fout.open(output);
	if (!fout)
		cout << "文件打开失败" << endl;
	cout << "characters: " << f1.getcharacters() << endl;
	fout << "characters: " << f1.getcharacters() << endl;
	cout << "words: " << f1.getwords() << endl;
	fout << "words: " << f1.getwords() << endl;
	cout << "lines: " << f1.getlines() << endl;
	fout << "lines: " << f1.getlines() << endl;
	int i;
	for (i = 0; i < num; i++)
	{
		cout << "<" << ww[i].w << ">" << ": " << ww[i].count << endl;
		fout << "<" << ww[i].w << ">" << ": " << ww[i].count << endl;
	}
	fout.close();
}

int main(int argc, char *argv[])
{
	int i = 0;
	string input;
	string output;
	int judgeW = 0, judgeN = 0, judgeM = 0;
	if (argc == 0)
	{
		cout << "请输入对应的参数" << endl;
		return 0;
	}
	while (1) //循环判断命令行参数
	{
		i++;
		if (i == argc)
			break;
		if (argv[i][0] == '-')
		{
			switch (argv[i][1])
			{
			case 'i':
			{
				i++;
				input = argv[i];
				break;
			}
			case 'o':
			{
				i++;
				output = argv[i];
				break;
			}
			case 'w':
			{
				i++;
				judgeW = atoi(argv[i]);
				break;
			}
			case 'm':
			{
				i++;
				judgeM = atoi(argv[i]);
				break;
			}
			case 'n':
			{
				i++;
				judgeN = atoi(argv[i]);
				break;
			}
			default:
				cout << "输入参数有误" << endl;
				break;
			}
		}
	}
	testfile f1;
	int num = 0;
	f1.init();
	f1 = f1.countcha(input, f1);
	f1 = f1.countline(input, f1);
	f1.Abstract = changeDx(f1.Abstract);
	f1.Title = changeDx(f1.Title);//大小写转换
	if (judgeW == 0)
	{
		f1 = f1.countword(f1, f1.Title, 1);
		f1 = f1.countword(f1, f1.Abstract, 1);
	}
	else
	{
		f1 = f1.countword(f1, f1.Title, 10);
		f1 = f1.countword(f1, f1.Abstract, 1);
	}
	sWord *ww = new sWord[f1.getwords()];//给结构体分配一个大小为单词数目的动态空间 
	sWord *temp = new sWord[f1.getwords()];
	map<string, int>::iterator it;
	it = mapword1.begin();
	for (it; it != mapword1.end(); it++)
	{
		ww[num].w = it->first;
		ww[num].count = it->second;
		num++;
	}
	sort(ww, temp, 0, num - 1);//把已经按字典序排号按出现频率进行从大到小的归并排序
	if (judgeN == 0)//如果没有定义n那么按默认输出
		outCome1(ww, num, output, f1);
	else
	{
		if (judgeN > f1.getwords())
		{
			cout << "输入的n值超过了文本的所有单词数，将为您按序输出文本的所有单词" << endl;//对于参数过大的错误判断
			outCome2(ww, f1.getwords(), output, f1);
		}
		else
		{
			outCome2(ww, judgeN, output, f1);//如果定义了n那么输出前n个
		}
	}
	delete[]temp;
	delete[]ww;
	return 0;
}