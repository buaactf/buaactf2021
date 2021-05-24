FROM ctftraining/base_image_nginx_mysql_php_73
COPY files/ /var/www/html/
RUN sh -c "mysqld_safe &" &&\
    mv /var/www/html/src/* /var/www/html/ &&\
    chown -R www-data:www-data /var/www/html &&\
    chmod -R 755 /var/www/html &&\
    rm -rfv /var/www/html/src &&\
    sleep 5 &&\
    mysql -e "source /var/www/html/db.sql;" -uroot -proot
    
