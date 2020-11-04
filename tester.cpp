#include <iostream>
#include <dirent.h>
#include <fstream>
#include <string.h>
#include <chrono> // for time measurement

using namespace std;

bool hideMode = false;
bool analyzingmode = false;
bool faultStopMode = false;

string path;

string rtrim(string s);
void classic_compare(string testName);
void analisys_compare(string testName);


int main (int argc, char *argv[]) {
  if(argc < 3)
  {
    cout << "tester (program to be tested) (tests directory)\n";
    cout << "  -a for analyzing mode (optional)\n";
    cout << "  -h to hide fault informations in analyzing mode (optional)\n";
    cout << "  -s to stop program after fault in classic mode or after analizing one test in analyzing mode (optional)\n";
  }
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
          cout<<"Unknown command: '"<<arg<<"'\n\n";
      }
    }

    cout.setf(ios::fixed,ios::floatfield); // set double print precision to two for printing TIME
    cout.precision(2);

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

          auto start = chrono::high_resolution_clock::now();
          system(&cmd[0]);
          auto finish = chrono::high_resolution_clock::now();
          chrono::duration<double> elapsed = finish - start; 

          cout<<"TEST "<<testName<<": time: "<<elapsed.count()<<"s";
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
      cout<<"Directory canot be accesed!\n";
      return EXIT_FAILURE;
    }

  }
  cout<<'\n';
  return 0;
}


// funkcje

string rtrim(string s)
{
  string WHITESPACE = " \n\t";
  int end = s.find_last_not_of(WHITESPACE);
  if(end >= 0)
    return s.substr(0, end+1);
  else
    return "";
}

void stop()
{
  cout<<"\nStop mode. Type \"Y\" to continue...\n";
  while(1)
  {
    string answer;
    cin>>answer;
    if(answer == "y" || answer == "Y")
      break;
  }
  cout<<"\n";
}

void classic_compare(string testName)
{
  cout<<", ";
  int line = 0;

  fstream solutionFile;
  string solutionLine;

  fstream outputFile;
  string outputLine;

  // open file with solution
  solutionFile.open(path + "/" + testName+".out");
  if(!solutionFile.good())
  { 
    cout<<"Something went wrong! There is no "<<testName<<".out file.\n";
    exit(EXIT_FAILURE);
  }

  // open file with program output
  outputFile.open(path + "/" + "alghorithm_tester.out", ios::in);
  if(!outputFile.good())
  { 
    cout<<"Something went wrong! Canot acces work file.\n";
    exit(EXIT_FAILURE);
  }

  //files compare
  while(getline(solutionFile, solutionLine))
  {
    line++;

    solutionLine = rtrim(solutionLine);
    
    if(!getline(outputFile, outputLine))
    {
      cout<<"line "<<line<<": No output";

      if(faultStopMode)
	     stop();
      return;
    }
    else
    {
      solutionLine = rtrim(solutionLine);

      if(solutionLine != outputLine)
      {
        cout<<"line "<<line<<": FAULT. Expected: '"<<solutionLine<<"' but given: '"<<outputLine<<"'\n";

        solutionFile.close();
        outputFile.close();

        if(faultStopMode)
	   	  stop();

        return;
      }
    }
  }

  if(getline(outputFile, outputLine))
  {
    cout<<"FAULT. Output to long!!!\n";

    if(faultStopMode)
	  stop();
  }
  else
    cout<<"OK\n";

  solutionFile.close();
  outputFile.close();
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
    cout<<"Something went wrong! There is no "<<testName<<".out file.\n";
    exit(EXIT_FAILURE);
  }

  // open file with program output
  outputFile.open(path + "/" + "alghorithm_tester.out", ios::in);
  if(!outputFile.good())
  { 
    cout<<"Something went wrong! Canot acces work file.\n";
    exit(EXIT_FAILURE);
  }

  //files compare
  while(getline(solutionFile, solutionLine))
  {
    line++;

    solutionLine = rtrim(solutionLine);
    
    if(!getline(outputFile, outputLine))
    {
      cout<<"Line "<<line<<": No output";
      if(faultStopMode)
        stop();
      return;
    }
    else
    {
      solutionLine = rtrim(solutionLine);

      if(solutionLine == outputLine)
        correctLines++;
      else if(hideMode == false)
        cout<<"Line "<<line<<": FAULT. Expected: '"<<solutionLine<<"' but given: '"<<outputLine<<"\n";
    }
  }

  if(getline(outputFile, outputLine))
    {
      cout<<"FAULT. Output to long!!!\n";

      if(faultStopMode)
        stop();
    }
  else
  {
    cout<<"------------------------------------\n";
    cout<<"Result: "<<correctLines<<"/"<<line<<"  Percent: "<<(correctLines*100)/line<<"%\n";
    cout<<"------------------------------------\n\n";

    if(faultStopMode)
        stop();
  }

  solutionFile.close();
  outputFile.close();
}