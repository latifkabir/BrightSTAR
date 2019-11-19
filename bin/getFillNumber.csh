#/bin/csh -f

set List = $1

set DBServer     = db04.star.bnl.gov
set DBPortRunLog = 3414
set DBName       = RunLog

if (-f ex$List) then
	rm ex$List
endif
echo '' > ex$List

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
		  $DBName >> ex$List
end

sed -i 's/.00000000//g' ex$List
