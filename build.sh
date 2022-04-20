
#!/bin/bash
set -e
pwdBeforeOMPI=$(pwd)
ompiDirName=openmpi
ompiVersion=4.1
ompiPackage=$ompiDirName-$ompiVersion.2
ompiInstDir=$(pwd)/$ompiPackage/installed
if [ ! -d $ompiDirName ]
then 
    wget https://download.open-mpi.org/release/open-mpi/v$ompiVersion/$ompiPackage.tar.gz
    tar -xvzf $ompiPackage.tar.gz
    mkdir --parents $ompiInstDir
    cd $ompiPackage
    
    ./configure --prefix=$ompiInstDir 
    make -j4 all 
    make install
    
    echo "export PATH=\"\$PATH:${ompiInstDir}/bin\"" >> $HOME/.bashrc
    echo "export LD_LIBRARY_PATH=\"\$LD_LIBRARY_PATH:${ompiInstDir}/lib\"" >> $HOME/.bashrc
    source ~/.bashrc
fi
cd ..
rm $ompiPackage.tar.gz
cd $pwdBeforeOMPI

set +e