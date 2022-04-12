echo "We've been dancin' with Mr. Brownstone"
cp /Users/giha/Documents/buildWave/CONVERT CONVERT
export DANA_PATH=/Users/giha/Documents/root/waveFiss/bin/
echo "Beginning CONVERT"
./CONVERT wave.in > outConvert.txt

# cp /Users/giha/Documents/root/waveFiss/bin/singlesprocessing singlesprocessing

echo "Beginning singlesprocessing"
${DANA_PATH}/singlesprocessing danaFile.root outFile.root # > outSingles.txt

# cp /Users/giha/Documents/root/waveFiss/bin/eventbuild eventbuild
echo "Beginning eventbuild"
${DANA_PATH}/eventbuild outFile.root event.root # > outEvent.txt
