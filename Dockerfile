FROM gcc:8.4.0 as builder
WORKDIR /workdir
COPY googletest/ /workdir/googletest
COPY SQLiteCpp/ /workdir/SQLiteCpp
COPY json/ /workdir/json
COPY CGI/ /workdir/CGI
COPY CUI/ /workdir/CUI
COPY src/ /workdir/src
COPY include/ /workdir/include/
COPY tests/ /workdir/tests/
COPY CMakeLists.txt /workdir
RUN apt-get update && apt-get install cmake libmysqlcppconn-dev -y --no-install-recommends
RUN cmake -DDB_TYPE=mysql /workdir && make

FROM httpd:2.4
COPY --from=builder /workdir/UserDBAccesser.cgi /usr/local/apache2/cgi-bin
COPY ./httpd.tpl /usr/local/apache2/conf/httpd.conf
COPY ./start.sh ./
RUN chmod 744 ./start.sh
RUN apt-get update && apt-get install libmysqlcppconn-dev -y --no-install-recommends
CMD ["./start.sh"]
