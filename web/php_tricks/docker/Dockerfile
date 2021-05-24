FROM phusion/baseimage:0.11

RUN sed -i 's/archive.ubuntu.com/mirrors.aliyun.com/g' /etc/apt/sources.list && \
    sed -i 's/security.ubuntu.com/mirrors.aliyun.com/g' /etc/apt/sources.list && \
    apt-get update && apt-get install -y apache2 libapache2-mod-php7.2 php7.2-mbstring&& \
    apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/* /var/www/html/*

RUN mkdir -p /etc/service/apache2/ && \
    printf "#!/bin/sh\n\nexec /usr/sbin/apachectl -D FOREGROUND\n" > /etc/service/apache2/run && \
    chmod 700 /etc/service/apache2/run

RUN sed -i 's/Options Indexes FollowSymLinks/Options None/' /etc/apache2/apache2.conf
RUN sed -i 's/AllowOverride None/AllowOverride All/' /etc/apache2/apache2.conf
RUN echo 'date.timezone = PRC' >> /etc/php/7.2/apache2/php.ini
RUN echo 'session.upload_progress.enabled = Off' >> /etc/php/7.2/apache2/php.ini
RUN echo 'allow_url_fopen = Off' >> /etc/php/7.2/apache2/php.ini
RUN echo 'open_basedir = /var/www/html:/tmp' >> /etc/php/7.2/apache2/php.ini
RUN echo 'display_errors = Off' >> /etc/php/7.2/apache2/php.ini
RUN echo 'disable_functions = pcntl_alarm,pcntl_fork,pcntl_waitpid,pcntl_wait,pcntl_wifexited,pcntl_wifstopped,pcntl_wifsignaled,pcntl_wifcontinued,pcntl_wexitstatus,pcntl_wtermsig,pcntl_wstopsig,pcntl_signal,pcntl_signal_get_handler,pcntl_signal_dispatch,pcntl_get_last_error,pcntl_strerror,pcntl_sigprocmask,pcntl_sigwaitinfo,pcntl_sigtimedwait,pcntl_exec,pcntl_getpriority,pcntl_setpriority,pcntl_async_signals,system,exec,shell_exec,popen,proc_open,passthru,symlink,link,syslog,imap_open,ld,mail' >> /etc/php/7.2/apache2/php.ini

ADD ./html /var/www/html
ADD ./flag /flag

RUN chown root:root -R /var/www/html && \
	chmod 755 -R /var/www/html && \ 
    chmod 777 /var/www/html/upload && \
    chmod 444 /flag

WORKDIR /var/www/html
