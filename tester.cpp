#include <iostream>
#include <dirent.h>
#include <fstream>
#include <cstdlib>
#include <string.h>

using namespace std;

bool hideMode = false;
bool analyzingmode = false;
bool faultStopMode = false;

string path;

string rtrim(string s)
{
  string WHITESPACE = " \n\t";
  int end = s.find_last_not_of(WHITESPACE);
  if(end >= 0)
    return s.substr(0, end+1);
  else
    return "";
}

string read_file(string p)
{
  fstream file;
  string line;
  string content="";

  file.open(p, ios::in);
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

void classic_compare(string testName)
{
  //compare test.out with proper .out file
  string outputStr = read_file(path + "/" + "alghorithm_tester.out");

  string solutionStr = read_file(path + "/" + testName+".out");

  if(solutionStr == outputStr)
    cout<<"OK"<<endl;
  else
  {
    if(hideMode == false)
      cout<<"FAULT. Expected: '"<<solutionStr<<"' but given: '"<<outputStr<<"'"<<endl;
    else
      cout<<"FAULT"<<endl;

    if(faultStopMode)
    {
      cout<<"\nFault stop mode. Type \"Y\" to continue...\n";
      while(1)
      {
        string answer;
        cin>>answer;
        if(answer == "Y" || answer == "y");
          break;
      }
      cout<<"\n";
    }
  }
}

void analisys_compare(string testName)
{
  cout<<"\n";

  int line = 0;
  int correctLines = 0;

  fstream solutionFile;
  string solutionLine;

  fstream outputFile;
  string outputLine;

  // open file with solution
  solutionFile.open(path + "/" + testName+".out");
  if(!solutionFile.good())
  { 
    cout<<"Something went wrong! There is no "<<testName<<".out file."<<endl;
    exit(EXIT_FAILURE);
  }

  // open file with program output
  outputFile.open(path + "/" + "alghorithm_tester.out", ios::in);
  if(!outputFile.good())
  { 
    cout<<"Something went wrong! Canot acces work file."<<endl;
    exit(EXIT_FAILURE);
  }

  //files compare
  while(getline(solutionFile, solutionLine))
  {
    line++;

    solutionLine = rtrim(solutionLine);
    
    if(!getline(outputFile, outputLine))
      cout<<"Line "<<line<<": No output";
    else
    {
      solutionLine = rtrim(solutionLine);

      if(solutionLine == outputLine)
        correctLines++;
      else if(hideMode == false)
        cout<<"Line "<<line<<": FAULT. Expected: '"<<solutionLine<<"' but given: '"<<outputLine<<"'"<<endl;
    }
  }

  if(getline(outputFile, outputLine)) cout<<"FAULT. Output to long!!!\n";
  else
  {
    cout<<"------------------------------------\n";
    cout<<"Result: "<<correctLines<<"/"<<line<<"  Percent: "<<(correctLines*100)/line<<"%\n";
    cout<<"------------------------------------\n\n";
  }

  solutionFile.close();
  outputFile.close();
}

int main (int argc, char *argv[]) {
  if(argc < 3)
    cout << "Usage: tester (program to be tested) (tests directory) -h to hide fault message (optional) -a for analyzing mode (optional) -s to stop program after fault (optional)";
  else
  {
    if(argc >= 4)
    {
      for(int i=3; i < argc; i++)
      {
        string arg(argv[i]);
        if(arg == "-h")
          hideMode = true;
        else if(arg == "-a")
            analyzingmode = true;
          else if(arg == "-s")
            faultStopMode = true;
        else
          cout<<"Unknown command: '"<<arg<<"'"<<endl<<endl;
      }
    }

    DIR *dir;
    dirent *ent;

    path = string(argv[2]);
    path = "./"+path;

    if (dir = opendir(&path[0])) 
    {
      while (ent = readdir (dir))
      {
        string fileName(ent->d_name);
        int length = fileName.length();
 
        string testName = fileName.substr(0, length-3);

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

          cout<<testName<<": ";
          if(analyzingmode == false)
            classic_compare(testName);
          else
            analisys_compare(testName);
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