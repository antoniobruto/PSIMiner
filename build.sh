clear
echo "---------------------------------------------------------------"
echo "              Prefix-Sequence Inference Miner"
echo "                        (PSIMiner)"
echo "          Code developed by Antonio A. Bruto da Costa"
echo "   Submit issues at https://github.com/antoniobruto/PSIMiner"
echo "---------------------------------------------------------------"
BUILD_DIR=build 

if [ ! -d "$BUILD_DIR" ]; then
	# Control will enter here if $BUILD_DIR doesn't exist.
	mkdir build  
else
	rm -rf build  
	mkdir build  
fi

cp -f parsers/*.l build/ 
cp -f parsers/*.y build/  
cp -f parsers/prefixes/* build/  
cp -f code/*.* build/  

cd build

#LOG_DIRECTORY=logs 

#clear
#rm assertions.txt
if [ -f "dtree.txt" ]; then
	rm dtree.txt 
fi

#if [ ! -d "$LOG_DIRECTORY" ]; then
  # Control will enter here if $LOG_DIRECTORY doesn't exist.
  #mkdir logs 
#fi

lex -PintervalList intervalList.l 
bison -d intervalList.y -p intervalList 

lex -PpredToPy predToPy.l 
bison -d predToPy.y -p predToPy 

lex -PlearnedOP learnedOP.l 
bison -d learnedOP.y -p learnedOP 


#gcc structs.c predToPy.tab.c lex.yy.c -o intervalAnalysis

cat preIntervalList|cat - lex.intervalList.c > temp.c && mv temp.c lex.intervalList.c 
cat preIntervalList|cat - intervalList.tab.c > temp.c && mv temp.c intervalList.tab.c 

cat prePredToPy|cat - lex.predToPy.c > temp.c && mv temp.c lex.predToPy.c 
cat prePredToPy|cat - predToPy.tab.c > temp.c && mv temp.c predToPy.tab.c 


cat prelearnedOP|cat - lex.learnedOP.c > temp.c && mv temp.c lex.learnedOP.c 
cat prelearnedOP|cat - learnedOP.tab.c > temp.c && mv temp.c learnedOP.tab.c 

gcc -w structs.c psiMinerStructs.c  lex.predToPy.c predToPy.tab.c lex.intervalList.c intervalList.tab.c lex.learnedOP.c learnedOP.tab.c psiMiner.c  -o psiMiner -lm 
#gcc  structs.c amsMinerStructs.c  amsMiner.c lex.predToPy.c predToPy.tab.c lex.intervalList.c intervalList.tab.c -o amsMiner -lm
#Use -fno-access-control for g++

#cp psiMiner ../
#cd ..
#clear
echo "To run PSI-Miner, run the 'psiMiner' binary from the <build> directory"
