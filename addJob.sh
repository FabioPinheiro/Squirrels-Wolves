#!bin/bash
machine_count=10
arguments1=inputFile
arguments2=iterations

#universe = parallel #nao mudar
#executable = /usr/sbin/ompiwrapper.sh #nao mudar
#arguments = /mnt/nimbus/pool/CPD/groups/6/Proj2/Squirrels-Wolves/$arguments #mudar!
#log = /mnt/nimbus/pool/CPD/groups/6/Proj2/Squirrels-Wolves/outputs/log/log.txt
#output = /mnt/nimbus/pool/CPD/groups/6/Proj2/Squirrels-Wolves/outputs/out/out.txt
#error = /mnt/nimbus/pool/CPD/groups/6/Proj2/Squirrels-Wolves/outputs/err/err.txt
#machine_count = $machine_count
#getenv = True #usa as variaveis de ambiente de cada sistema (must have)
#queue

#Option1
#cat >~/.inputrc <<EOF
#stuff here##
#EOF

#Option2
#echo "THIS IS STUFF FOR THE FILE" >> index.html
#the dual >> appends the stuff at the end.

echo universe = parallel >job$machine_count$arguments1.sub #nao mudar
echo executable = /usr/sbin/ompiwrapper.sh >> job$machine_count$arguments1.sub #nao mudar
echo arguments = /mnt/nimbus/pool/CPD/groups/6/Proj2/Squirrels-Wolves/$arguments1$arguments2 >> job$machine_count$arguments1.sub #mudar!
echo log = /mnt/nimbus/pool/CPD/groups/6/Proj2/Squirrels-Wolves/outputs/log/log.txt >> job$machine_count$arguments1.sub
echo output = /mnt/nimbus/pool/CPD/groups/6/Proj2/Squirrels-Wolves/outputs/out/out.txt >> job$machine_count$arguments1.sub
echo error = /mnt/nimbus/pool/CPD/groups/6/Proj2/Squirrels-Wolves/outputs/err/err.txt >> job$machine_count$arguments1.sub
echo machine_count = $machine_count >> job$machine_count$arguments1.sub
echo getenv = True >> job$machine_count$arguments1.sub #usa as variaveis de ambiente de cada sistema (must have)
echo queue >> job$machine_count$arguments1.sub

condor_submit job$machine_count$arguments1.sub