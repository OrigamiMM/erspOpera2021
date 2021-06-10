#~/bin/bash
echo "simulator" | sudo -S apt-get install g++ python3
yes | sudo apt-get install g++ python3 python3-dev pkg-config sqlite3
yes | sudo apt-get install python3-setuptools git
yes | sudo apt-get install qt5-default mercurial
yes | sudo apt-get install gir1.2-goocanvas-2.0 python-gi python-gi-cairo python-pygraphviz python3-gi python3-gi-cairo python3-pygraphviz gir1.2-gtk-3.0 ipython ipython3
yes | sudo apt install python-dev
cd ~/ns-allinone-3.29
python2 ./build.py --enable-examples --enable-tests --
cd ~/ns-allinone-3.29/ns-3.29
./waf --run scratch/first --vis
