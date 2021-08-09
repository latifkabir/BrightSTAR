#/bin/csh -f

# Run this script with csh
    
set List = $1

set DBServer     = db04.star.bnl.gov
# set DBPortRunLog = 3414 #For RUn 15
set DBPortRunLog = 3416   #For Run 17
set DBName       = RunLog

if (-f ${List}_FillNo.dat) then
	rm ${List}_FillNo.dat
endif
echo '' > ${List}_FillNo.dat

foreach ListItem (`cat $List`)

	mysql -h $DBServer -P $DBPortRunLog -N \
		  -e 'SELECT distinct \
		  		beamInfo.runNumber, \
				beamInfo.yellowFillNumber, \
				(runDescriptor.endRunTime - runDescriptor.startRunTime) \
		  	  FROM \
			  	beamInfo, \
				runDescriptor \
			  WHERE \
			  	beamInfo.runNumber=runDescriptor.runNumber AND \
			  	beamInfo.runNumber='$ListItem' AND \
				beamInfo.blueFillNumber>0.0' \
		  $DBName >> ${List}_FillNo.dat
end

sed -i 's/.00000000//g' ${List}_FillNo.dat
