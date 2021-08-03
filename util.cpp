/*
 *    uhInfo
 *    Copyright (C) 2018
 */

#include "util.h"

namespace uhiutil::cpu {
   bool native_fq_state = false; // native or relative mode
   int max_cpu_t = 120; // maximum monitoring cpu temperature by default
}

void uhiutil::end_intervals_remove(std::string& str)
{
       if(!str.length()) return; // empty return 1

       if(str[(str.length() - 1)] == '\x0A')  {
                str.pop_back();   
                if(!str.length()) return; // empty return 2   
       }

       //remove itrevals at end
       int ipos = str.length() - 1;
       while(str[ipos] == '\x20') {
             --ipos;
             if(ipos <= 0 ) {
                  ipos = 0;
                  break;
             }
       }
          
       str = str.substr(0,ipos + 1);
       if(str == " ") str = "";
}

std::string uhiutil::execmd(const char* command)
{
    FILE* file = popen(command, "r");
    if(file)
    {
        std::ostringstream stm;
        char line[max_size];
        while(fgets(line, max_size, file)) stm << line;
        pclose(file);
        return stm.str();
    }

    return "";
}

std::string uhiutil::start_intervals_remove(std::string in)
{
   std::string lstr = in;
   int ipos = 0,pos = lstr.length();

   while(lstr[ipos] == '\x20')  ++ipos;

   return lstr.substr(ipos, pos - ipos);
}

void uhiutil::GetDesktopSize(unsigned int *Wh,  unsigned int *Ww)
{

  std::string sub(""),w(""),h("");
  std::string::size_type pos1,pos2;

  std::string result = execmd("xrandr");
  pos1 = result.find("current");
  pos1 += (((std::string)("current")).size());

  pos2 = result.find(",",pos1);
  sub = result.substr(pos1,pos2 - pos1);

  pos1 = sub.find("x");
  w = sub.substr(0,pos1);
  h = sub.substr((pos1 + 1), (sub.size() - pos1));

  *Ww = (  (unsigned int) std::stoi(w)  );
  *Wh = (  (unsigned int) std::stoi(h)  );
}

bool uhiutil::ExistenceVerification(const char *path)
{
   struct stat sb;
   return (stat(path,&sb) == 0);
}

bool uhiutil::newline(std::string &in_line,const char* in_find,Direction drct)
{
    int posns = -1,posne = -1;

    if((posne = in_line.size()) < 1) return false;
    if((posns = in_line.find(in_find)) == -1) return false;

    if(drct == Direction::RIGHT || drct == Direction::RIGHTSKIP) {
          posns += ((((std::string)(in_find)).size()) + ((drct == Direction::RIGHTSKIP) ? 0 : 1));
          in_line = in_line.substr(posns,posne - posns);
    }
    else in_line = in_line.substr(0,posns - ((drct == Direction::LEFTSKIP) ? 0 : 1));

    return true;
}

std::string uhiutil::GetUserName() 
{
   std::string pathuser = execmd("whoami");
   end_intervals_remove(pathuser);
   return pathuser;
}
