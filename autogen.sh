autoheader; 
aclocal; 
automake --add-missing --copy; 
autoconf; 
./configure;
 
 
make;
 
#case "$(uname -s)" in
#
#	Darwin)
#		echo 'Mac OS X'
#		cp src/main omusubin
#		;;
#
#	Linux)
#		echo 'Linux'
#		cp src/main omusubin
#		;;
#
#	CYGWIN*|MINGW32*|MSYS*|MINGW*)
#		echo 'MS Windows'
#		cp src/main omusubin.exe
#		;;
#
#		# Add here more strings to compare
#		# See correspondence table at the bottom of this answer
#
#		*)
#		echo 'Other OS' 
#		cp src/main omusubin
#		;;
#esac
