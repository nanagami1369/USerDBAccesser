#!/bin/sh
sed -i "s#{{MYSQL_HOST}}#$MYSQL_HOST#g; \
s#{{MYSQL_USER}}#$MYSQL_USER#g; \
s#{{MYSQL_PASSWORD}}#$MYSQL_PASSWORD#g; \
s#{{MYSQL_DATABASE}}#$MYSQL_DATABASE#g" /usr/local/apache2/conf/httpd.conf

httpd-foreground
