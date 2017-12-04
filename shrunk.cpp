#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <limits.h>
#include <cmath>
using namespace std;

void get_shrunk(ifstream & fin,ofstream & fout);

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
		struct hentry{
			int index;
			hnode *node;
		};
		int weight[256];
		int hcode[256];
		int size;
		int len;
		int tlen;
		hnode *root;
		void get_list(vector<hentry> &list){
			for(int i = 0 ; i < 256;i++){
				if(weight[i]!=0){
					hentry newone;
					newone.index = weight[i];
					hnode *newnode = new hnode(i);
					newone.node = newnode;
					list.push_back(newone);
				}
			}
		}		
		void sort(vector<hentry> list){
			for(int i = 0 ; i < list.size()-1;i++){
				int index = i;
				for(int j = i+1 ; j <list.size();j++ ){
					if(list[j].index > list[index].index) index = j;
				}
				hentry temp2 = list[index];
				list[index] = list[i];
				list[i] = temp2;
			}
		}
		void get_htree(){
			vector<hentry> list;
			get_list(list);
			while(list.size() > 1){
				hentry temp1 = list.back();
				list.pop_back();
				hentry temp2 = list.back();
				list.pop_back();
				hentry newone;
				newone.index = temp1.index+temp2.index;
				hnode *newnode = new hnode('!',temp1.node,temp2.node);
				newone.node = newnode;
				list.push_back(newone);
				for(int i = list.size()-1;i>0;i--){
					if(list[i].index <= list[i-1].index) break;
					hentry temp = list[i];
					list[i] = list[i-1];
					list[i-1] = temp;
				}
			}
			root = list[0].node;
		}
		void get_hcode(hnode * temp,int code = 1){
			len++;
			if(temp->left == NULL){
				hcode[temp->entry] = code;
				len +=2;
			}
			else{
				get_hcode(temp->left,code*2);
				get_hcode(temp->right,code*2+1);
			}
		}
		void _list(hnode * temp,vector<unsigned char> &list){
			if(temp== NULL)	list.push_back('#');
			else{
				list.push_back(temp->entry);
				_list(temp->left,list);
				_list(temp->right,list);
			}
		}
	public:
		tree():size(0),root(NULL),len(0){
			for(int i = 0 ; i <256;i++){
				weight[i] = 0;
				hcode[i] = 0;
			}
		}
		int get_size(){
			return size;
		}
		int get_len(){
			return len;
		}
		void add(unsigned char entry){
			weight[entry]++;
			size++;
		}
		void refresh(){
			len = 0;
			get_htree();
			get_hcode(root);
		}
		int get(unsigned char entry){
			return hcode[entry];
		}
		unsigned char * list(){
			vector<unsigned char> temp;
			_list(root,temp);
			unsigned char *temp2 = new unsigned char[temp.size()+1];
			for(int i = 0 ; i < temp.size();i++){
				temp2[i] = temp[i];
				temp2[i+1] = '\0';
			}
			return temp2;
		}
		void check(){
			for(int i = 0 ; i < 256;i++){
				cout << (char)i << " " << weight[i] <<" " ;
				unsigned int temp = hcode[i];
				while(temp!=0){
					cout << (temp &1);
					temp >>=1;
				}
				cout << endl;
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
	cout << "please input the file name:" << endl;
	char ifn[20];
	cin >> ifn;
	fin.open(ifn,ios_base::in | ios_base::binary);
	if(!fin.is_open()){
		cout << "wrong name" << endl;
		cin.get();
		cin.get();
		exit(0);
	}
	char ofn[20];
	char *temp;
	strtok(ifn,".");
	strcpy(ofn,ifn);
	strcat(ofn,".zzz");
	fout.open(ofn,ios_base::out| ios_base::binary);
	temp = strtok(NULL,"\0");
	fout << temp << endl;
	get_shrunk(fin,fout);
	fin.close();
	fout.close();
	return 0;
}

void get_shrunk(ifstream & fin,ofstream & fout){
	unsigned char read_buff;
	tree t1;
	while(fin.read((char*)&read_buff,sizeof(char))) t1.add(read_buff);
	t1.refresh();
//	t1.check();
//	t1.show_tree();
//	cout << t1.get_len() << endl; 
//	cout << t1.get_size() << endl;

	unsigned char* list = t1.list();
	fout << t1.get_len() << endl; 
	fout << t1.get_size() << endl;
	fout.write((char*)list,t1.get_len()+1);
	
	fin.clear();
	fin.seekg(0);
	unsigned char out_buff = 0;
	int out_len = 0;
	int read_len;
	while(fin.read((char*)&read_buff,sizeof(char))){
//		cout << read_buff;
		unsigned int  hcode = t1.get(read_buff);
		int code_len = 0;
		unsigned int code_copy = hcode;
		while(code_copy!=0){
			code_len++;
			code_copy/=2;
		}
		code_len--;
		while(code_len > 0){
			out_buff <<= 1;
			out_buff |= ((hcode >> (code_len-1)) & 1);
//			cout << (out_buff & 1);
			code_len--;
			out_len++;
			if(out_len==8){
//				cout << endl;
//				for(int i = 0 ; i < 8;i++){
//					cout << ((out_buff >> (7-i)) & 1) ;
//				}
//				cout << "bug" << endl;
				fout.write((char*)&out_buff,1);
				out_len = 0;
				out_buff = 0;
			}
		}
	}
	out_buff <<= (8-out_len);
	fout.write((char*)&out_buff,1);
}
