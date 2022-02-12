/*
 *    uhInfo
 *    Copyright (C) 2018
 */

#include "hmonit.h"
#include "uhirc.h"
#include <bitset>
#include <cstring>

bool CMonitor::get_edid_buffer(std::istringstream &ininput,std::string &mntr) const
{
    bool edid_line = false;
    for(std::string line; std::getline(ininput, line);) {

        if(!edid_line && (line.find("EDID") != std::string::npos)) {
	     edid_line = true; 
	     continue;
        }
	
        if(edid_line) {
             if((line[0] == '\t') && (line[1] == '\t')) {
                    line = line.substr(2,line.size() - 2);
		    mntr.append(line);
             }
             else
                 break;
        }
    }

    return edid_line;
}

char CMonitor::convert(const char *chek) const
{
    char ret = 0;
    unsigned long val = (std::bitset<16>(*chek)).to_ulong();

    switch(val)
    {
        case 48 ... 57 : ret = val - 48;break;
        case 65 ... 70 : ret = val - 55;break;
        case 97 ... 102: ret = val - 87;break;
    }

    return ret;
}

std::string CMonitor::calculate(float Vsize,float Hsize) const
{
    std::string str("");
    float res = std::hypot((Hsize * ((float) 10)),(Vsize * ((float) 10))) / (float) 25.4;
    float trnc = std::trunc(res);
    char bf[6] = {0,0,0,0,0,0};

    if(res <= (trnc + 0.37)){
             res = trnc;
             std::sprintf(bf,"%2.0f",res);
    }
    else
        if((res > (trnc + 0.37)) && (res < (trnc + 0.71))) {
             res = trnc + 0.5;
             std::sprintf(bf,"%2.1f",res);
        }
        else {
           res += 1.0;
           res = std::trunc(res);
           std::sprintf(bf,"%2.0f",res);
        }
    str = bf;

    return str;
}

std::string CMonitor::calculate_dimension(const char *pchar,bool xrandr_edid) const
{
      std::string str("");
      unsigned char h_size = 0,v_size = 0;

      if(xrandr_edid) {
           h_size = ((unsigned char)((convert(pchar) << 4) | (convert((pchar + 1)))));
           v_size = ((unsigned char)((convert(pchar + 2) << 4) | (convert((pchar + 3)))));
      }
      else {
           h_size = ((unsigned char) *pchar);
           v_size = ((unsigned char) *(pchar + 1));
      }
   
      if((h_size > 0) && (v_size > 0)) 
           str = calculate((float)v_size,(float)h_size);
 
     return str;
}

std::string CMonitor::GetMonitorName(const char *pName,bool xrandr_edid) const
{
    std::string mname("");
    const char *pChar = pName;
    if(xrandr_edid) {
          std::string workbuffer(""),data("");
          for(int cnt = 108 ; cnt < 217 ; cnt += 36) {
              workbuffer = (pChar + cnt);
              if(workbuffer.substr(0,10) == "000000fc00") {
                    data = workbuffer.substr(10,26);
                    break;
              }
          }

          if(data != "") {
               pChar = data.c_str();
               std::string apd("");
               for(int boch = 0 ; boch < 26 ; boch += 2, pChar += 2) {  
                     if((*pChar == 48 ) && (((*(pChar + 1)) == 97) || ((*(pChar + 1)) == 65))) break;
                     apd = {(char)((convert(pChar) << 4) | (convert((pChar + 1))))};
                     mname.append(apd);
               }
          }
    } 
    else {
          const char *workbuffer = nullptr,*data = nullptr;
          for(int cnt = 54 ; cnt < 109 ; cnt += 18) {
                workbuffer = (pChar + cnt);
                if(*workbuffer == '\x00' && *(workbuffer + 1) == '\x00' && *(workbuffer + 2) == '\x00' && *(workbuffer + 3) == '\xfc' && *(workbuffer + 4) == '\x00') {
                      data = workbuffer + 5;
                      break;
                }
          }
          if(data) {
                pChar = data;
                char apd[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
                for(int boch = 0 ; boch < 14 ; boch++, pChar++) {  
                      if((*pChar == '\x0A' )) break;
                      apd[boch] = *pChar;
                }
                mname.append(apd);
        }
    }

    return mname;
}

void CMonitor::Init()
{
    std::string m_dvendor(""),m_dimension(""),m_dname("");
    if(uhiutil::execmd("xrandr --prop | grep EDID") != "") { // x11
             int mnumb = std::atoi((uhiutil::execmd("xrandr --prop | grep ' connected' | wc -l")).c_str());
             std::string prop = uhiutil::execmd("xrandr --prop"),edid("");
             std::istringstream input(prop);
             do {
	              if(get_edid_buffer(input,edid)) {
                        const char *pchar = edid.c_str() + 16;
                        GETVENDOR(m_dvendor,((convert(pchar) << 12) | (convert((pchar + 1)) <<  8) | (convert((pchar + 2)) << 4) | convert((pchar+ 3)))); // Get vendor
                        m_dimension = calculate_dimension((edid.c_str() + 42)); // calc monitor dimension
                        m_dname = GetMonitorName(edid.c_str()); // Get display name
                  }

                  edid = "";
                  MONITORSTRING(m_dvendor,m_dname,m_dimension);

             }while(--mnumb);
    }
    else { // wayland
             DIR *dirp = opendir(uhiutil::monitor::edid_wl_path); 
             bool link = false,realdir = false;
             struct dirent *dp = 0;
             while((dp = readdir(dirp)) != 0)  {
                  std::string file(dp->d_name); 
                  if(file == "." || file == "..") continue;                  
                  switch(dp->d_type) {
                         case DT_DIR:
                              realdir = true;
                              break;
                         case DT_LNK:
                              link = true;
                              break;
                         default:
                              break;
                  } 
                  if(realdir || link) {
                       std::string rd_buff(""),cmd("");
                       if(realdir) rd_buff = std::string(uhiutil::monitor::edid_wl_path) + std::string(dp->d_name);
                       else {
                          cmd = "readlink -f " + (std::string(uhiutil::monitor::edid_wl_path) + std::string(dp->d_name));
                          rd_buff = uhiutil::execmd(cmd.c_str());
                          uhiutil::end_intervals_remove(rd_buff);
                       }
                       if(!uhiutil::ExistenceVerification(std::string(rd_buff + "/enabled").c_str())) continue;
                       cmd = "cat " + rd_buff + "/enabled"; 
                       cmd = uhiutil::execmd(cmd.c_str());
                       uhiutil::end_intervals_remove(cmd);
                       if(cmd == "enabled") {
                              char buff[1024];
                              int rdb = 0;
                              cmd = rd_buff + "/edid";
                              std::FILE* flp = std::fopen(cmd.c_str(), "r");
                              if(flp) {
                                    rdb = std::fread(buff, sizeof(char), 1024, flp);
                                    std::fclose(flp);
                                    if(rdb) {
                                            GETVENDOR(m_dvendor,((*(buff + 8) << 8) | (*(buff + 9) & 0x00FF))); // Get vendor
                                            m_dimension = calculate_dimension((buff + 21),false); // calc monitor dimension
                                            m_dname = GetMonitorName(buff,false); // Get display name
            
                                            MONITORSTRING(m_dvendor,m_dname,m_dimension);
                                    }
                              }
                       }
                  }
             }
             closedir(dirp); 
    }
    if(m_monitor != "") uhiutil::end_intervals_remove(m_monitor);      
}
