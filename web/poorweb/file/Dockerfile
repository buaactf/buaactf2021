FROM nrstech/php-7.4-apache

ADD ./src /var/www/html
ADD ./flag /flag
RUN rm /bin/sh &&\
	ln -s /bin/bash /bin/sh
RUN apt-get install inetutils-ping -y && \
	chmod 644 /flag && \	
	chown -R root:root /var/www/html &&\
	chmod -R 755 /var/www/html