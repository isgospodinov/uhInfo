#!/bin/sh

showhelp()
{
  echo "help :"
  echo "To build just type (make)."
  echo "To cleaning up type (make clean)."
  echo "To get help : (make help)"
  echo "To install or uninstall you must to be sudo!"
  echo -n " 1) To install : "
  echo "(sudo make install)"
  echo -n " 2) To uninstall : "
  echo "(sudo make uninstall)"
}

before()
{
   if test -e /usr/include/udisks2/udisks ; then   
      echo "** Udisks2 development files exist. **"
      cnt=0
      br=0
      FILES="/usr/include/udisks2/udisks/*"
      for file in $FILES; do
          br=$( cat $file | grep '<udisks2' | wc -l )
          cnt=$(($cnt + $br))
      done
#cnt=0
      if [ "$cnt" = 0 ] ; then
          echo "** But check failed "$cnt" **"
          echo "** Go to '/usr/include/udisks2/udisks' directory end edit *.h files. **"
          echo "** When you encounter '<udisks/' replace it with '<udisks2/udisks/'. **"
          echo "** Save changes and build uhInfo. **"
      else
          echo "** And check passed "$cnt". **"
      fi
    else
          echo "** Udisks2 development files don't exist. **"
          echo "** Install Udisks2 development files. **"
          echo "** Go to '/usr/include/udisks2/udisks' directory end edit *.h files. **"
          echo "** When you encounter '<udisks/' replace it with '<udisks2/udisks/'. **"
          echo "** Save changes and build uhInfo. **"
    fi
    
    echo "** Build started. **"
}

after()
{
   echo "** Build complete. **"
}

check()
{
   if ! [ -x build/uhInfo ] ; then
      chmod +x build/uhInfo
   fi
}

copy_delete()
{
   res=1 #exit_fail
# internal comands : 
#    0 -delete 
#    1 - copy
   if [ "$1" = 1 ] ; then
        if cp $2 $3 ; then
                res=0 #exit_success 
        else res=1 #exit_fail 
        fi
    else
        if rm -f $2 ; then
                res=0 #exit_success
        else res=1 #exit_fail 
        fi
    fi

   return $res
}

installuhI()
{
   echo "Installing..."
#Icon
   if test -e /usr/local/share/icons/uhI.png ; then   
      echo "  Icon already installed.Noting to do."
   else echo -n "  Installing icon."
        if ! test -d /usr/local/share/icons ; then
           mkdir /usr/local/share/icons
        fi
        if copy_delete 1 data/uhI.png /usr/local/share/icons ; then
            echo "Success."
        else echo "Failure."
        fi
   fi
#Shortcut
   if test -e /usr/local/share/applications/uhInfo.desktop ; then   
      echo "  Shortcut already installed.Noting to do."
   else echo -n "  Installing shortcut."
        if ! test -d /usr/local/share/applications ; then
           mkdir /usr/local/share/applications
        fi
        if copy_delete 1 data/uhInfo.desktop /usr/local/share/applications ; then
            echo "Success."
        else echo "Failure."
        fi
   fi
#Program
   if test -e /usr/local/bin/uhInfo ; then   
      echo "  Program already installed.Noting to do."
   else echo -n "  Installing program."  
        if copy_delete 1 build/uhInfo /usr/local/bin ; then
            echo "Success."
        else echo "Failure."
        fi
   fi

   echo "Installation finish."
}

uninstalluhI()
{
   echo "Uninstalling..."
#Icon
   if test -e /usr/local/share/icons/uhI.png ; then
      echo -n "  Deleting icon."
      if copy_delete 0 /usr/local/share/icons/uhI.png ; then
            echo "Success."
      else echo "Failure."
      fi
   else echo "  Icon has been deleted.Noting to do."
   fi
#Shortcut
   if test -e /usr/local/share/applications/uhInfo.desktop ; then  
      echo -n "  Deleting shortcut."
      if copy_delete 0 /usr/local/share/applications/uhInfo.desktop ; then
            echo "Success."
      else echo "Failure."
      fi
   else echo "  Shortcut has been deleted.Noting to do."
   fi
#Program
   if test -e /usr/local/bin/uhInfo ; then  
      echo -n "  Deleting program."
      if copy_delete 0 /usr/local/bin/uhInfo ; then
            echo "Success."
      else echo "Failure."
      fi
   else echo "  Program has been deleted.Noting to do."
   fi

   echo "Uninstall finish."
}

getuser()
{
   usr=$(echo $(printenv | grep SUDO_USER) | sed -e 's/^SUDO_USER=//')
   if [ "$usr" = "" ] ; then
      usr=$(echo $(printenv | grep USER=) | sed -e 's/^USER=//')
   fi
}

setupuhIconf()
{
   echo "Setting up configuration storage..."
   getuser $usr
   if [ "$usr" = "root" ] || [ "$usr" = "" ] ; then
       echo "  Creation rejected."
       return
   fi

   if test -e /home/$usr/.uhInfo ; then   
      echo "  Configuration storage exist.Noting to do."
   else echo "  Create configuration storage."
        if ! test -d /home/$usr/.uhInfo ; then
           mkdir /home/$usr/.uhInfo
           chmod o=rwx /home/$usr/.uhInfo
           touch /home/$usr/.uhInfo/uhInfo.cfg
           chmod o=rwx /home/$usr/.uhInfo/uhInfo.cfg
           echo "  Configuration storage created."
        fi
   fi
}

removeuhIconf()
{
   echo "Removing configuration storage..."
   getuser $usr
   if [ "$usr" = "root" ] || [ "$usr" = "" ] ; then
       echo "  Deletion rejected."
       return
   fi

   if test -e /home/$usr/.uhInfo ; then
      rm -rf /home/$usr/.uhInfo
      echo "  Configuration storage removed."
   else echo "  Configuration storage do not exist.Noting to do."
   fi
}

case $1 in
  -i ) check
       installuhI
       setupuhIconf
       ;;
  -ui ) uninstalluhI
        removeuhIconf
       ;;
  -ib ) before
       ;;
  -ia ) after
       ;;
  * ) showhelp
      ;;
esac
 
