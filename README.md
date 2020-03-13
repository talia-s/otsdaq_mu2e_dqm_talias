Here is a bit of information about this Repo and its use:

# Log in From on mu2edaq01

```
$ ssh -X mu2edaq13
$ cd ~test_stand/ots_agg
$ source setup_ots.sh
$ mz 
```
# Create the directory from this Repo
```
$ create repo via: 
$ UpdateOTS.sh  --pullall 
$ source ots_get_and_fix_repo.sh otsdaq-mu2e-dqm
$ mz
```

# Too push code to this repo:
```
$ git status
$ git add NEW_STUFF
$ git commit -m "Information about new stuff"
$ git push origin develop (develop is branch name here)
```

# Modify CMake files in order to compile
```
$ cp CMakeLists.txt 
```
from mu2e trigger directory. Will need to add in similar files to each subdirectory. Check for places we might need to change things.

Also remember to add in the new directory to the Head CMakeList.txt in srcs:

```
$ cd YOUR_TOP_DIRECTORY_NAME/srcs
$ EDITOR_NAME CMakeList.txt
```
Copy another package block and change the name :

```
set(otsdaq_mu2e_dqm_not_in_ups true)
include_directories ( ${CMAKE_CURRENT_SOURCE_DIR}/otsdaq_mu2e_dqm )
include_directories ( $ENV{MRB_BUILDDIR}/otsdaq_mu2e_dqm )

```
# Clean Build:
```
$ mz
```
