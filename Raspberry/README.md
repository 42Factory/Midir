Raspberry installation 
======================

In our case, a Raspberry B+ was used

### Setting up python libraries

First, upgrade package and dependencies :

    $ sudo apt-get update
    $ sudo apt-get upgrade
    $ sudo apt-get dist-upgrade
	
Now install pip :

    $ sudo apt-get install pip
	
Then install python package needed : 

    $ sudo pip install requests
    $ sudo pip install python-firebase
	$ sudo pip install pyserial
	
### Import the script

Download the python script on the raspberry :

    $ cd /home/pi/
	$ wget https://raw.githubusercontent.com/TO_DEFINE/Raspberry/main.py
	
You can now test the script :
	
	$ python main.py
	
### Executing the script on start up of the raspberry

To automatically execute the script on booting, you need to put the command at the end of the /etc/rc.local file

	$ sudo nano /etc/rc.local
	
And then add this line at the end of the file before 'exit 0':
	
	(sleep 10; python /home/pi/main.py)&
	
Now reboot the raspberry, it should execut the script on booting :

	$ sudo reboot