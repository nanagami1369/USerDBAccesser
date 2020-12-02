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
RUN apt-get update && apt-get update -y && apt-get install cmake -y
RUN cmake -DDB_TYPE=memory /workdir && make

FROM httpd:2.4
RUN sed -ri 's/#LoadModule cgid_module/LoadModule cgid_module/g; \
    s/#AddHandler cgi-script .cgi/AddHandler cgi-script .cgi/g' /usr/local/apache2/conf/httpd.conf
COPY --from=builder /workdir/UserDBAccesser.cgi /usr/local/apache2/cgi-bin
