# Monitor
Public DNS as of this writing: http://ec2-52-207-234-165.compute-1.amazonaws.com

API: \ping
		Should return pong
	 \read
	 	Returns a list of the crap you want in JSON format.
	 \add
	 	Add your crap. Whatever you send me will all go on one line per request.

# Devops scripts
The "monitor" script in the root dir is not the C++ executable application - this sits in src folder. To set up the app executable as a linux long-running service you have to do the following:

1) ssh in to the EC2 box: sh -i <path>/<to>/<pem> ubuntu@ec2-52-207-234-165.compute-1.amazonaws.com. The <path>/<to>/<pem> is the relative path to the pem file which is not saved to the repo. 
	If you don't have it, it's probably because you shouldn't.
	
2) Upload the monitor service script from your local machine to EC2 home directory (/ubuntu/home) on the EC2 box (call this from your local machine);
	scp -i <path>/<to>/<pem> <path>/<to>/<monitor_service_script> ubuntu@ec2-52-207-234-165.compute-1.amazonaws.com: (note the colon at the end)
	
3) Move this file to /etc/init.d on EC2. Make sure it is an executable; sudo chmod 755 monitor from the same dir as the monitor script.

4) Run the following command to be able allow the service to run in background after termination of ssh session; sudo update-rc.d monitor defaults.

5) copy the src/monitor executable from local to remote as shown above. then in your ssh session mv the file to /usr/local/bin and make sure it is accompanied by a file call device_log_file in the same dir.

6) to start or stop the script run: sudo /etc/init.d/monitor start|stop.

7) if this doesnt work. Try destroying the session and recreating a new one. Apparently this can be important.

Nginx is set up to proxy any port 80 calls on to port 8080 which is what the app listens on:
In /etc/nginx/sites-available/monitor we have:

	server { # simple reverse-proxy
	  listen       80;
	  server_name  ec2-52-207-234-165.compute-1.amazonaws.com;

	  location / {
		proxy_pass      http://127.0.0.1:8080;
	  }
	}
	
and in sites-enabled we create a symlink: ln -s /etc/nginx/sites-available/monitor monitor
