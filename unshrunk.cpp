#include <fstream>
#include <iostream>
#include <cstring>
using namespace std;

class tree{
	private:
		struct hnode{
			unsigned char entry;
			hnode * left;
			hnode * right;
			hnode(unsigned char _entry = 0 ,hnode *_left = NULL, hnode *_right=NULL){
				entry = _entry;
				left = _left;
				right = _right;
			}
		};
		void _rebuild(hnode * &temp,unsigned char * &list,int & n){
//			cout << list[n];
			if(list[n] == '#' && !(list[n+2]=='#' && list[n+1]=='#')){
				temp = NULL;
				n++;
			}
			else{
				temp = new hnode(list[n]);
				n++;
				_rebuild(temp->left,list,n);
				_rebuild(temp->right,list,n);
			}
		}
		int size;
		int len;
		int tlen;
		hnode *root;
	public:
		tree():size(0),len(0),tlen(0),root(NULL){}
		void rebuild(unsigned char * list){
			int n = 0 ;
			_rebuild(root,list,n);
		}
		void set_size(int n){
			size = n;
		}
		void set_len(int n){
			len = n;
		}
		void translate(ifstream & fin,ofstream & fout){
			int _size =size;
			unsigned char read_buff;
			hnode * _root = root;
			int read_len;
			while(fin.read((char*)&read_buff,1)){
				read_len = 8;
				while(read_len > 0 && _size){
//					cout << ((read_buff >> (read_len-1)) & 1);
					if(((read_buff >> (read_len-1)) & 1)== 0) _root = _root->left;
					else _root = _root->right;
					if(_root->left == NULL){
//						cout << " " << _root->entry << endl;
						fout.write((char*)&(_root->entry),1);
						_root = root;
						_size--;
					}
					read_len--;
				}
			}
		}
		void show_tree(){
			tlen = 0;
			_show_tree(root);
			cout << endl;
		}
		void _show_tree(hnode *temp){
			tlen++;
			cout << tlen <<"bug";
			if(temp==NULL){
				cout <<'#'<<endl;
				return;
			}
			cout <<(char)temp->entry << endl;
			_show_tree(temp->left);
			_show_tree(temp->right);
		}
};

int main(){
	ifstream fin;
	ofstream fout;
	char ifn[20];
	cout << "input name:" << endl;
	cin >> ifn;
	fin.open(ifn,ios_base::in | ios_base::binary);
	if(!fin.is_open()){
		cout << "wrong name" << endl;
		cin.get();
		cin.get();
		exit(0);
	}
	char temp[20];
	fin.getline(temp,20);
	strtok(ifn,".");
//	strcat(ifn,".zz");
	strcat(ifn,".");
	strcat(ifn,temp);
	fout.open(ifn,ios_base::out| ios_base::binary);
	int len;
	int size;
	fin >>len;
	fin.get();
	fin >> size;
	fin.get();
//	cout << len << endl;
//	cout << size << endl;
	unsigned char temp2[len+1];
	fin.read((char*)temp2,len+1);
	
//	for(int i = 0 ; i < len+1;i++){
//		cout  << i << "bug" << temp2[i] << endl;
//	}
	
	tree t1;
	t1.set_size(size);
	t1.set_len(len);
	t1.rebuild(temp2);
//	t1.show_tree();
	t1.translate(fin,fout);
	
	fin.close();
	fout.close();
	return 0;
}
