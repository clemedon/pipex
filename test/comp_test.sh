#!/bash

# Init
touch test/file1
echo "salut\nklasdf\nsalut\nhell0" > test/file1;
touch test/file1_perm test/file2_perm test/cmd1_perm test/cmd2_perm
echo "salut\nklasdf\nsalut\nhell0" > test/file1_perm;
chmod 000 test/file1_perm test/file2_perm test/cmd1_perm test/cmd2_perm;

display () {
    tput setaf 4; echo "out: $1"; tput setaf 2; echo "out: $2"
    tput setaf 4; echo "ret: $3"; tput setaf 2; echo "ret: $4";
    tput sgr0
}

echo "=========== [ Compare Output ] ==========="

echo "[ NORMAL ]"

echo "          [FILE1 CMD1 CMD2 FILE2]"
tput setaf 4;
./pipex test/file1 "cat -e" "wc -l" test/file2; ret1=$(echo $?); out1=$(cat -e test/file2);
tput setaf 2;
< test/file1 cat -e | wc -l > test/file2; ret2=$(echo $?); out2=$(cat -e test/file2);
display " $out1" " $out2" " $ret1" " $ret2"

echo "[ MULTI ]"

echo "          [FILE1 CMD1 CMD2 CMD3 FILE2]"
tput setaf 4;
./pipex test/file1 "cat" "grep salut" "wc -l" test/file2; ret1=$(echo $?); out1=$(cat -e test/file2);
tput setaf 2;
< test/file1 cat -e | grep salut | wc -l > test/file2; ret2=$(echo $?); out2=$(cat -e test/file2);
display " $out1" " $out2" " $ret1" " $ret2"


echo "[ FILE NOT FOUND ]"

echo "          [XXXXX CMD1 CMD2 XXXXX]"
tput setaf 4;
./pipex test/XXXX1 "cat -e" "wc -l" test/XXXX2; ret1=$(echo $?); out1=$(cat -e test/file2);
tput setaf 2;
< test/XXXX1 cat -e | wc -l > test/XXXX2; ret2=$(echo $?); out2=$(cat -e test/file2);
display " $out1" " $out2" " $ret1" " $ret2"

echo "          [XXXXX CMD1 CMD2 FILE2]"
tput setaf 4;
./pipex test/XXXX1 "cat -e" "wc -l" test/file2; ret1=$(echo $?); out1=$(cat -e test/file2);
tput setaf 2;
< test/XXXX1 cat -e | wc -l > test/file2; ret2=$(echo $?); out2=$(cat -e test/file2);
display " $out1" " $out2" " $ret1" " $ret2"

echo "          [FILE1 CMD1 CMD2 XXXXX]"
tput setaf 4;
./pipex test/file1 "cat -e" "wc -l" test/XXXX2; ret1=$(echo $?); out1=$(cat -e test/file2);
tput setaf 2;
< test/file1 cat -e | wc -l > test/XXXX2; ret2=$(echo $?); out2=$(cat -e test/file2);
display " $out1" " $out2" " $ret1" " $ret2"

echo "[ CMD NOT FOUND ]"

echo "          [FILE1 XXXX XXXX FILE2]"
tput setaf 4;
./pipex test/file1 "XXX1" "XXX2" test/file2; ret1=$(echo $?); out1=$(cat -e test/file2);
tput setaf 2;
< test/file1 test/XXX1 | test/XXX2 > test/file2; ret2=$(echo $?); out2=$(cat -e test/file2);
display " $out1" " $out2" " $ret1" " $ret2"

echo "          [FILE1 XXXX CMD2 FILE2]"
tput setaf 4;
./pipex test/file1 "XXX1" "wc -l" test/file2; ret1=$(echo $?); out1=$(cat -e test/file2);
tput setaf 2;
< test/file1 test/XXX1 | wc -l > test/file2; ret2=$(echo $?); out2=$(cat -e test/file2);
display " $out1" " $out2" " $ret1" " $ret2"

echo "          [FILE1 CMD1 XXXX FILE2]"
tput setaf 4;
./pipex test/file1 "cat -e" "XXX2" test/file2; ret1=$(echo $?); out1=$(cat -e test/file2);
tput setaf 2;
< test/file1 cat -e | test/XXX2 > test/file2; ret2=$(echo $?); out2=$(cat -e test/file2);
display " $out1" " $out2" " $ret1" " $ret2"

echo "[ FILE PERMISSION DENIED ]"

echo "          [----- CMD1 CMD2 -----]"
tput setaf 4;
./pipex test/file1_perm "cat -e" "wc -l" test/file2_perm; ret1=$(echo $?); out1=$(cat -e test/file2);
tput setaf 2;
< test/file1_perm cat -e | wc -l > test/file2_perm; ret2=$(echo $?); out2=$(cat -e test/file2);
display " $out1" " $out2" " $ret1" " $ret1"

echo "          [----- CMD1 CMD2 FILE2]"
tput setaf 4;
./pipex test/file1_perm "cat -e" "wc -l" test/file2; ret1=$(echo $?); out1=$(cat -e test/file2);
tput setaf 2;
< test/file1_perm cat -e | wc -l > test/file2; ret2=$(echo $?); out2=$(cat -e test/file2);
display " $out1" " $out2" " $ret1" " $ret2"

echo "          [FILE1 CMD1 CMD2 -----]"
tput setaf 4;
./pipex test/file1 "cat -e" "wc -l" test/file2_perm; ret1=$(echo $?); out1=$(cat -e test/file2);
tput setaf 2;
< test/file1 cat -e | wc -l > test/file2_perm; ret2=$(echo $?); out2=$(cat -e test/file2);
display " $out1" " $out2" " $ret1" " $ret1"

echo "[ CMD PERMISSION DENIED ]"

echo "          [FILE1 ---- ---- FILE2]"
tput setaf 4;
./pipex test/file1 "test/cmd1_perm" "test/cmd2_perm" test/file2; ret1=$(echo $?); out1=$(cat -e test/file2);
tput setaf 2;
< test/file1 test/cmd1_perm | test/cmd2_perm > test/file2; ret2=$(echo $?); out2=$(cat -e test/file2);
display " $out1" " $out2" " $ret1" " $ret2"

echo "          [FILE1 ---- CMD2 FILE2]"
tput setaf 4;
./pipex test/file1 "test/cmd1_perm" "wc -l" test/file2; ret1=$(echo $?); out1=$(cat -e test/file2);
tput setaf 2;
< test/file1 test/cmd1_perm | wc -l > test/file2; ret2=$(echo $?); out2=$(cat -e test/file2);
display " $out1" " $out2" " $ret1" " $ret2"

echo "          [FILE1 CMD1 ---- FILE2]"
tput setaf 4;
./pipex test/file1 "cat -e" "test/cmd2_perm" test/file2; ret1=$(echo $?); out1=$(cat -e test/file2);
tput setaf 2;
< test/file1 cat -e | test/cmd2_perm > test/file2; ret2=$(echo $?); out2=$(cat -e test/file2);
display " $out1" " $out2" " $ret1" " $ret2"

#Quit
rm -f test/file1_perm test/file2_perm test/cmd1_perm test/cmd2_perm
rm -f test/file1 test/file2 test/XXXX2
