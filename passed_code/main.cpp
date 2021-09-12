

#include <iostream>
#include <string.h>
#include <sstream>
#include <vector>
using namespace std;


// defining addr class
class address{
public:
    vector<long long> data;
    long long index;
    long long tag;
    int tag_len;
    string original_data;
    address(string s){
        original_data=s;
        data.resize(s.length());
        for (int i=0; i<s.length(); i++) {
            if (s[i]=='1') {
                data[i]=1;
            }
            else{
                data[i]=0;
            }
        }
    }
    address(){
    }
    
    void calculate_index(vector<int> A,int n){
        index=0;
        for (int i=0; i<n; i++) {
            index = index*2;
            index = index + data[A[i]];
        }
    }
    
    void calculate_tag(vector<int> A,int n,int z){
        tag_len=z;
        int B[z];
        int index=0;
        bool flag;
        for (int i=0; i<n+z; i++) {
            flag=0;
            for (int j=0; j<n; j++) {
                if (A[j]==i) {
                    flag=1;
                }
            }
            if (flag==0) {
                B[index]=i;
                index++;
            }
        }
        
        tag=0;
        for (int i=0; i<z; i++) {
            tag=tag*2;
            tag+= data[B[i]];
        }
    }
};

class Cache{
public:
    vector<vector<long long> > block;
    vector<vector<bool> > NRU;
    int cache_set;
    int associativity;
    
    bool update(address a){
        for (int i=0; i<associativity; i++) {
            if (block[a.index][i]==a.tag) {
                NRU[a.index][i]=0;
                return 0;
            }
        }
        
        for (int i=0; i<associativity; i++) {
            if (NRU[a.index][i]==1) {
                block[a.index][i]=a.tag;
                NRU[a.index][i]=0;
                return 1;
            }
        }
        for (int i=0; i<associativity; i++){
            NRU[a.index][i]=1;
        }
        for (int i=0; i<associativity; i++) {
            if (NRU[a.index][i]==1) {
                block[a.index][i]=a.tag;
                NRU[a.index][i]=0;
                return 1;
            }
       }
        return 1;
    }
    
    Cache(int a, int b){
        cache_set=a;
        associativity=b;
        block.resize(a);
        NRU.resize(a);
        for (int i=0; i<a; i++) {
            block[i].resize(b);
            NRU[i].resize(b);
            for (int j=0; j<b; j++) {
                block[i][j]=-1;
                NRU[i][j]=1;
            }
        }
    }
    Cache(){}
    
    
};

int lg(int n){
    int ans=0;
    while (n!=1) {
        n=n/2;
        ans++;
    }
    return ans;
}

// global variable
int address_bit, block_size, cache_set, associativity;
int byte_offset, index_length, tag_length;
int input_size=0;
int n,m;
vector <int> A;
vector <bool> miss;
int miss_num;
vector <int> ans_index;
vector <string> input;
vector <address> chart;
string name;

void reading(char** argv){
    freopen(argv[1], "r", stdin);
   
    string tmp;
    stringstream ss;

    while (cin>>tmp) {
        if ((int)tmp[0]<=57 && (int)tmp[0]>=48) {
            ss<<tmp;
            ss>>address_bit;
            ss.str("");
            ss.clear();
            break;
        }
    }
    while (cin>>tmp) {
        if ((int)tmp[0]<=57 && (int)tmp[0]>=48) {
            ss<<tmp;
            ss>>block_size;
            ss.str("");
            ss.clear();
            break;
        }
    }
    while (cin>>tmp) {
        if ((int)tmp[0]<=57 && (int)tmp[0]>=48) {
            ss<<tmp;
            ss>>cache_set;
            ss.str("");
            ss.clear();
            break;
        }
    }
    while (cin>>tmp) {
        if ((int)tmp[0]<=57 && (int)tmp[0]>=48) {
            ss<<tmp;
            ss>>associativity;
            ss.str("");
            ss.clear();
            break;
        }
    }
    
    fclose(stdin);
    
    byte_offset = lg(block_size);
    index_length = lg(cache_set);
    tag_length = address_bit - byte_offset - index_length;
    freopen(argv[2], "r", stdin);
   
    while(cin>>tmp){
        if (tmp==".end") {
            break;
        }
        else if (tmp==".benchmark"){
            cin>>name;
        }
        else if ((int)tmp[0]<=57 && (int)tmp[0]>=48){
            input_size++;
            input.push_back(tmp);
        }
    }
    fclose(stdin);

    chart.resize(input_size);
    for (int i=0; i<input_size; i++) {
        chart[i]=address(input[i]);
    }

    n = address_bit - byte_offset;
    m = index_length;
    A.resize(n);
    for (int i=0; i<m; i++) {
        A[i]=i;
    }
    miss.resize(input_size);
    for (int i=0; i<input_size; i++) {
        miss[i]=true;
    }
    miss_num=input_size;
    ans_index.resize(m);
}

void output(char** argv){
    freopen(argv[3], "w", stdout);
    
    cout<<"Address bits: "<<address_bit<<endl;
    cout<<"Block size: "<<block_size<<endl;
    cout<<"Cache sets: "<<cache_set<<endl;
    cout<<"Associativity: "<<associativity<<endl;
    cout<<endl;
    cout<<"Offset bit count: "<<byte_offset<<endl;
    cout<<"Indexing bit count: "<<index_length<<endl;
    cout<<"Indexing bits:";
    for (int i=0; i<index_length; i++) {
        cout<<" "<<ans_index[i];
    }
    cout<<endl<<endl;
    
    cout<<".benchmark "<<name<<endl;

    for (int i=0; i<input_size; i++) {
        cout<<chart[i].original_data;
        if (miss[i]==1) {
            cout<<" miss"<<endl;
        }
        else{
            cout<<" hit"<<endl;
        }
    }
    cout<<".end"<<endl<<endl;
    cout<<"Total cache miss count: "<<miss_num<<endl;
    fclose(stdout);
}


void simulation(vector<int> A,int n){
    
    for (int i=0; i<input_size; i++) {
        chart[i].calculate_index(A, n);
        chart[i].calculate_tag(A, n, tag_length);
    }
    
    Cache cache(cache_set,associativity);
    
    bool tmp_miss[input_size];
    int tmp_miss_num=0;
    int count=0;
    
    for (int i=0; i<input_size; i++) {
        if (cache.update(chart[i])==true) {
            tmp_miss_num++;
            tmp_miss[count]=1;
        }
        else{
            tmp_miss[count]=0;
        }
        count++;
    }
   
    if (tmp_miss_num < miss_num) {
        miss_num=tmp_miss_num;
        for (int i=0; i<input_size; i++) {
            miss[i]=tmp_miss[i];
        }
        for (int i=0;i<n;i++){
            ans_index[i]=address_bit-A[i]-1;
        }
    }
}



int main(int argc, char **argv) {
    reading(argv);
    
        //LSB
        for (int i=0; i<index_length; i++) {
            A[i]=tag_length+i;
        }
        simulation(A, m);
    
    output(argv);    
    return 0;
}
