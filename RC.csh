#!/bin/csh

# - Script to run RC_split on an arg. period
# Usage: qsub -V RC.csh
# - Arguments are passed as env. variables
#    - RC_EXEDIR
#    - RC_INPATH
#    - RC_OUTPATH
# - Works in combination w/ "./submitJobs.py".

##### CHECK ARG. ENV. VARIABLES RC_...
##### INSTALL LOCALLY I/O DIRS AND FILES AS REQUIRED BY RC_split
##### EXECUTE RC_split W/ OPTION -k -q
##### COPY OUTPUTS TO OUTPATH
#####   INCLUDING kinSIDIS

printf '+++++++++++++++++++++++++++++++\n'
printf ' * RC: Run RC_split \n'
printf '+++++++++++++++++++++++++++++++\n'

##### WORKING DIR = $TMPDIR
if ( $?TMPDIR == 0 ) then
    printf '\n** RC: Inconsistency: No $TMPDIR\n'
else
    cd $TMPDIR
endif

##### CHECK ARG. ENV. VARIABLES RC_...
set exeName = RC_split
if ( $?RC_EXEDIR == 0 ) then
    printf '\n** RC: Inconsistency: No $RC_EXEDIR\n'
    exit 2
else
    set exeDir = $RC_EXEDIR
    set exePath = $exeDir/$exeName
    if ( !( -e  $exePath ) ) then
	printf '\n** RC: Inconsistency: No "%s" in EXEDIR "%s"\n' $exeName $exeDir
	exit 2
    endif
endif
if ( $?RC_INPATH == 0 ) then
    printf '\n** RC: Inconsistency: No $RC_INPATH\n'
    exit 2
else
    set inPath = $RC_INPATH
endif
if ( $?RC_OUTPATH == 0 ) then
    printf '\n** RC: Inconsistency: No $RC_OUTPATH\n'
    exit 2
else
    set outPath = $RC_OUTPATH
endif

##### INSTALL LOCALLY I/O DIRS AND FILES AS REQUIRED BY RC_split
# INPUT DIR
\cp $inPath/filelist.txt .

##### EXECUTE RC_split W/ OPTION -k -q
printf '\n~~~~~~~~~~~~~~~~~~~~~\n'
printf ' * Execute "%s"\n' $exePath
printf   '~~~~~~~~~~~~~~~~~~~~~\n'
$exePath -k -q
set exeStatus = $status
printf   '~~~~~~~~~~~~~~~~~~~~~\n'
if ( $exeStatus != 0 ) then
    printf ' * RC: Execution returns %d\n' $exeStatus
printf   '~~~~~~~~~~~~~~~~~~~~~\n'
endif
    
##### COPY OUTPUTS TO OUTPATH
set copyStatus = 0
set files = `\ls`
printf ' * Copying to "%s":' $outPath; echo $files
foreach file ( $files )
    \cp $file $outPath
    if ( $status != 0 ) then
	set copyStatus = 1
    	printf '** Error copying "%s" to "%s"\n' $file $outPath
    endif
end
#####   INCLUDING count.txt AND kinSIDIS.(pdf|root)
set files = ( kinSIDIS.pdf kinSIDIS.root )
foreach file ( $files )
    if ( -e $file ) then
	if ( $file == kinSIDIS.pdf ) then
	    set fich = $outPath/`printf 'kinSIDIS.pdf'`
	else
	    set fich = $outPath/`printf 'kinSIDIS.root'`
	endif
	printf ' * Copying "%s" to "%s"\n' $file $fich
	\cp $file $fich
	if ( $status != 0 ) then
	    set copyStatus = 1
	    printf '** Error copying "%s" to "%s"\n' $file $fich
	endif
    else
	if ( $exeStatus == 0 ) set exeStatus = 1
	printf '** Inconsistency: No output "%s"\n'
    endif
end

##### ERROR
set error = `expr $exeStatus \* 2 + $copyStatus`
printf '+++++++++++++++++++++++++++++++\n'
printf '\ * RC: Script execution status = %d\n' $error
printf '+++++++++++++++++++++++++++++++\n'
exit $copyStatus
