#!/bin/bash
#라인을 읽어와서 10번째 부분의 정보를 가져와서 항상 찍도록 구성한다.
i=16;
#file1=runq-sz;
#file2=plist-sz;
#file3=ldavg-1;
#file4=ldavg-5; 
#file5=ldavg-15;



	echo "              frmpg/s   bufpg/s   campg/s" >> $2 
while :

do
	#sed -n ${i}p $1>> sar_load.txt
	sed -n ${i}p $1 >> $2
	let "i += 50"

	if [ "$i" -gt 95551 ]
	 then
	   break       	   #Abandon the while lopp.
	 fi

done
#	cat $2 | awk '{ print $1,	$2 }' >> $2_${file1}.txt
#	cat $2 | awk '{ print $1,	$3 }' >> $2_${file2}.txt
#	cat $2 | awk '{ print $1,	$4 }' >> $2_${file3}.txt
#	cat $2 | awk '{ print $1,	$5 }' >> $2_${file4}.txt
#	cat $2 | awk '{ print $1,	$6 }' >> $2_${file5}.txt
