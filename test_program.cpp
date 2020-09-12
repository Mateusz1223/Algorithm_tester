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


int main (int argc, char *argv[]) {
  if(argc < 3)
  {
    cout << "Usage: ..."; // program test path
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
          cmd = cmd + " < " + path + "/" + fileName + " > test.out";

          system(&cmd[0]);

          //compare test.out with proper .out file
          fstream tested;
          string testedStr = "";

          fstream solution;
          string solutionStr = "";

          string line;

          tested.open("test.out", ios::in);
          if(!tested.good())
          { 
            cout<<"Something went wrong! Canot acces work file."<<endl;
            return EXIT_FAILURE;
          }

          while (getline(tested, line))
          {
            line = rtrim(line);
            testedStr += line+"\n";
          }
          testedStr = rtrim(testedStr);

          tested.close();

          string path2 = path + "/" + fileNameNoExtension+".out";

          solution.open(path2, ios::in);
          if(!solution.good())
          { 
            cout<<"No .out file for test "<<fileNameNoExtension<<endl;
            return EXIT_FAILURE;
          }

          while (getline(solution, line))
          {
            line = rtrim(line);
            solutionStr += line+"\n";
          }
          solutionStr = rtrim(solutionStr);

          solution.close();

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