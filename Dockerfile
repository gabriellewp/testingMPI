FROM ubuntu:latest
RUN apt-get -y update
RUN apt-get -y upgrade
RUN apt-get -y install build-essential libssl-dev
RUN apt-get install -y git
RUN apt-get install -y llvm-10 llvm-10-dev clang-10
RUN mv /usr/bin/clang-10 /usr/bin/clang
RUN mv /usr/bin/clang++-10 /usr/bin/clang++
RUN apt-get install -y uuid-dev
RUN apt-get install -y lldb-10 libllvm10 llvm-10-runtime
RUN apt-get install -y lld
RUN apt-get install -y libncurses5-dev libncursesw5-dev
RUN DEBIAN_FRONTEND="noninteractive" apt-get install -y mpich #also replace with openmpi

CMD echo "Done"