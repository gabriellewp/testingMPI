FROM ubuntu:latest
RUN apt-get -y update
RUN apt-get -y upgrade
RUN apt-get -y install wget
RUN apt-get -y install build-essential libssl-dev
RUN apt-get install -y git
RUN apt-get install -y llvm-11 llvm-11-dev clang-11
RUN mv /usr/bin/clang-11 /usr/bin/clang
RUN mv /usr/bin/clang++-11 /usr/bin/clang++
RUN apt-get install -y uuid-dev
RUN apt-get install -y lldb-11 libllvm11 llvm-11-runtime
RUN apt-get install -y lld
RUN apt-get install -y libncurses5-dev libncursesw5-dev
RUN apt-get install -y gcc
RUN apt-get install -y libboost-all-dev
RUN DEBIAN_FRONTEND="noninteractive" apt-get install -y openmpi-bin openmpi-common libopenmpi-dev libgtk2.0-dev 
RUN DEBIAN_FRONTEND="noninteractive" apt-get install -y mpich #also replace with openmpi
RUN cd /home && wget http://downloads.sourceforge.net/project/boost/boost/1.79.0/boost_1_79_0.tar.gz \
  && tar xfz boost_1_79_0.tar.gz \
  && rm boost_1_79_0.tar.gz \
  && cd boost_1_79_0 \
  && ./bootstrap.sh --prefix=/usr/local --with-libraries=program_options \
  && ./b2 install \
  && cd /home \
  && rm -rf boost_1_79_0
CMD echo "Done"