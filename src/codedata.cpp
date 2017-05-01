#include "codedata.h"
using namespace std;
bool CodeData::load(const char *filename)
{
    FILE *fp=fopen(filename,"rb");
    if(!fp)return false;
    bool ret=load(fp);
    fclose(fp);
    return ret;
}
bool CodeData::load(FILE *fp)
{
    fread(&config,sizeof(config),1,fp);
    if(config.magic!=HEADER_MAGIC)return false;
    int_table=unique_ptr<int[]>(new int[config.int_table_size]);
    float_table=unique_ptr<double[]>(new double[config.float_table_size]);
    string_table=unique_ptr<char*[]>(new char*[config.string_table_size]);
    string_raw=unique_ptr<char[]>(new char[config.string_raw_size]);
    code_raw=unique_ptr<CODE[]>(new CODE[config.code_raw_size]);
    

    fread(int_table.get(),sizeof(int),config.int_table_size,fp);
    fread(float_table.get(),sizeof(double),config.float_table_size,fp);
    fread(string_raw.get(),1,config.string_raw_size,fp);
    fread(code_raw.get(),sizeof(CODE),config.code_raw_size,fp);
    
    char *buffer=string_raw.get();
    string_table[0]=buffer;
    for(int i=1;i<config.string_table_size;i++)
    {
        while(*buffer++);
        string_table[i]=buffer;
    }
    return true;
}

bool write_code_data(const char *filename,int entry_point,vector<int> int_table_list,vector<FLOAT_TYPE> float_table_list,vector<string> string_table_list,vector<CODE> code)
{
    FILE *fp=fopen(filename,"wb");
    if(!fp)return false;
    CodeConfig config;
    config.magic=HEADER_MAGIC;
    config.int_table_size=int_table_list.size();
    config.float_table_size=float_table_list.size();
    config.string_table_size=string_table_list.size();
    config.string_raw_size=0;
    config.entry_point=entry_point;
    config.code_raw_size=code.size();
    for(int i=0;i<string_table_list.size();i++)config.string_raw_size+=string_table_list[i].length()+1;
    fwrite(&config,sizeof(config),1,fp);

    // int table
    fwrite(&int_table_list[0],sizeof(int_table_list[0]),int_table_list.size(),fp);
    // float table
    fwrite(&float_table_list[0],sizeof(float_table_list[0]),float_table_list.size(),fp);
    //string table
    for(int i=0;i<string_table_list.size();i++)
    {
        string &s=string_table_list[i];
        fwrite(s.c_str(),1,s.length()+1,fp);
    }
    //code
    fwrite(&code[0],sizeof(code[0]),code.size(),fp);
    fclose(fp);
    return true;
}