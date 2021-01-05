FROM gcc:8.4.0 as cgi-builder
RUN apt-get update && apt-get install cmake libmysqlcppconn-dev -y --no-install-recommends
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
RUN cmake -DDB_TYPE=mysql /workdir && make

FROM node:12.20.0 as web-app-builder
WORKDIR /workdir
COPY web-app/package*.json ./
RUN npm install
COPY web-app .
RUN npm run build

FROM httpd:2.4
RUN apt-get update && apt-get install libmysqlcppconn-dev -y --no-install-recommends
COPY --from=cgi-builder /workdir/UserDBAccesser.cgi /usr/local/apache2/cgi-bin
COPY --from=web-app-builder /workdir/dist /usr/local/apache2/htdocs
COPY ./httpd.tpl /usr/local/apache2/conf/httpd.conf
COPY ./start.sh ./
RUN chmod 744 ./start.sh
CMD ["./start.sh"]
