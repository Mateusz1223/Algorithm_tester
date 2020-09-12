#include <iostream>
#include <dirent.h>
#include <fstream>

using namespace std;

string rtrim(string s)
{
  string WHITESPACE = " \n\t";
  int end = s.find_last_not_of(WHITESPACE);
  if(end >= 0)
    return s.substr(0, end+1);
  else
    return "";
}

string read_file(string path)
{
  fstream file;
  string line;
  string content="";

  file.open(path, ios::in);
  if(!file.good())
  { 
    cout<<"Something went wrong! Canot acces work file."<<endl;
    exit(EXIT_FAILURE);
  }

  while (getline(file, line))
  {
    line = rtrim(line);
    content += line+"\n";
  }
  content = rtrim(content);
  file.close();

  return content;
}

int main (int argc, char *argv[]) {
  if(argc < 3)
  {
    cout << "Usage: tester (program to be tested) (tests directory)";
  }
  else
  {   
    DIR *dir;
    dirent *ent;

    string path(argv[2]);
    path = "./"+path;

    if (dir = opendir(&path[0])) 
    {
      while (ent = readdir (dir))
      {
        string fileName(ent->d_name);
        int length = fileName.length();
 
        string fileNameNoExtension = fileName.substr(0, length-3);

        string extension = "";
        if(length>=3)
        {
          extension = fileName.substr(length-3, length);
        }
        

        if(extension == ".in")
        {

          string cmd(argv[1]);
          cmd = cmd + " < " + path + "/" + fileName + " > " + path + "/" + "alghorithm_tester.out";

          system(&cmd[0]);

          //compare test.out with proper .out file
          string testedStr = read_file(path + "/" + "alghorithm_tester.out");

          string solutionStr = read_file(path + "/" + fileNameNoExtension+".out");

          if(solutionStr == testedStr)
            cout<<fileNameNoExtension<<": OK"<<endl;
          else
            cout<<fileNameNoExtension<<": FAULT. Expected: '"<<solutionStr<<"' but given: '"<<testedStr<<"'";
        }
      }
      closedir (dir);
    }
    else
    {
      cout<<"Directory canot be accesed!"<<endl;
      return EXIT_FAILURE;
    }

  }
  cout<<endl;
  return 0;
}